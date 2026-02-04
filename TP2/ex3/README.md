# Exercise 3: Amdahl's Law and Gustafson's Law

## Overview

This exercise demonstrates the fundamental laws governing parallel computing performance: **Amdahl's Law** (strong scaling) and **Gustafson's Law** (weak scaling). We analyze a C program with both sequential and parallelizable components to understand the limits of parallelization.

## Files in this Directory

### Source Code
- **`program.c`** - Main program with N=100M (original specification)
- **`program_N5M.c`** - Version with N=5M for faster profiling
- **`program_N10M.c`** - Version with N=10M for faster profiling
- **`program`**, **`program_N5M`**, **`program_N10M`** - Compiled binaries

### Analysis & Results
- **`answers.md`** - Detailed answers to all questions with theoretical analysis
- **`calculate_scaling.py`** - Python script to compute Amdahl/Gustafson speedups
- **`visualizations.ipynb`** - Jupyter notebook with plots and visualizations
- **`profiling_results.csv`** - Valgrind profiling data for different N values
- **`scaling_results.csv`** - Computed speedups for all configurations
- **`callgrind.out.N5M`**, **`callgrind.out.N10M`** - Valgrind output files

### Generated Plots
- **`q3_amdahl_strong_scaling.png`** - Amdahl's Law speedup and efficiency
- **`q4_problem_size_effect.png`** - Effect of N on sequential fraction
- **`q5_amdahl_vs_gustafson.png`** - Comparison of scaling laws
- **`function_breakdown.png`** - Per-function instruction analysis

## Program Structure

The program consists of four main functions:

### Sequential Part (Cannot be Parallelized)
```c
void add_noise(double *a) {
    a[0] = 1.0;
    for (int i = 1; i < N; i++) {
        a[i] = a[i-1] * 1.0000001;  // Data dependency!
    }
}
```
- **Why Sequential?** Each iteration depends on the previous value
- **Contribution**: ~30.7% of total execution time

### Parallelizable Parts
```c
void init_b(double *b)              // ~17% of execution
void compute_addition(a, b, c)      // ~37% of execution  
double reduction(double *c)         // ~15% of execution
```
- **Why Parallelizable?** No dependencies between loop iterations
- **Total Contribution**: ~69.3% of execution time

## How to Run

### 1. Compile the Programs
```bash
gcc -O2 -g program.c -o program
gcc -O2 -g program_N5M.c -o program_N5M
gcc -O2 -g program_N10M.c -o program_N10M
```

### 2. Profile with Valgrind
```bash
# For N=5M (faster)
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N5M ./program_N5M

# For N=10M
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N10M ./program_N10M

# Analyze results
callgrind_annotate callgrind.out.N5M
```

### 3. Calculate Scaling Metrics
```bash
python3 calculate_scaling.py
```

This generates:
- `profiling_results.csv` - Sequential fractions for each N
- `scaling_results.csv` - Speedup values for all processor counts

### 4. Generate Visualizations
```bash
jupyter notebook visualizations.ipynb
```

Or run all cells to generate the plots.

## Key Results

### Sequential Fraction
- **fs ≈ 30.7%** (measured across all N values)
- **fp ≈ 69.3%** (parallelizable fraction)
- **Remains constant** as N changes (both parts scale linearly)

### Maximum Theoretical Speedup (Amdahl)
```
S_max = 1 / fs = 1 / 0.307 ≈ 3.26x
```

No matter how many processors we use, we cannot exceed 3.26x speedup!

### Speedup Comparison

| Processors | Amdahl S(p) | Gustafson S(p) | Efficiency |
|------------|-------------|----------------|------------|
| 1          | 1.00        | 1.00           | 100.0%     |
| 2          | 1.53        | 1.69           | 76.5%      |
| 4          | 2.08        | 3.08           | 52.0%      |
| 8          | 2.54        | 5.85           | 31.7%      |
| 16         | 2.85        | 11.39          | 17.8%      |
| 32         | 3.04        | 22.47          | 9.5%       |
| 64         | 3.14        | 44.63          | 4.9%       |

## Questions and Answers

### Question 1: Code Analysis
- **Sequential**: `add_noise()` - O(N) time, cannot parallelize
- **Parallelizable**: `init_b()`, `compute_addition()`, `reduction()` - all O(N), can become O(N/p)
- **See**: [answers.md](answers.md) for detailed analysis

### Question 2: Measuring Sequential Fraction
- **Method**: Valgrind/Callgrind profiling
- **Result**: fs ≈ 0.307 (30.7%)
- **Calculation**: Instructions(add_noise) / Total Instructions

### Question 3: Strong Scaling (Amdahl's Law)
- **Formula**: S(p) = 1 / (fs + fp/p)
- **Max Speedup**: ~3.26x
- **Saturation**: Occurs at p=16-32
- **See**: `q3_amdahl_strong_scaling.png`

### Question 4: Effect of Problem Size
- **Finding**: Sequential fraction remains constant (~30.7%) for all N
- **Reason**: Both sequential and parallel parts scale linearly with N
- **See**: `q4_problem_size_effect.png`

### Question 5: Weak Scaling (Gustafson's Law)
- **Formula**: S(p) = p - fs(p-1) = fs + p×fp
- **Result**: Nearly linear scaling (44.6x at p=64)
- **Comparison**: Much more optimistic than Amdahl
- **See**: `q5_amdahl_vs_gustafson.png`

## Understanding the Laws

### Amdahl's Law (Strong Scaling)
- **Scenario**: Fixed problem size, add more processors
- **Example**: Process same 100M elements on 1, 2, 4, ... processors
- **Reality**: Fixed dataset you need to process faster
- **Limitation**: Sequential part limits speedup

### Gustafson's Law (Weak Scaling)
- **Scenario**: Problem size grows with processors
- **Example**: Each processor handles 100M elements (total work scales)
- **Reality**: Higher resolution simulation, more detailed model
- **Advantage**: Sequential overhead becomes relatively smaller

### Which Law Applies?
- **Use Amdahl**: When you have a fixed dataset/problem
- **Use Gustafson**: When you can scale the problem size
- **Real World**: Often a mix - some fixed overhead, some scalable work

## Visualizations

The notebook generates comprehensive visualizations:

1. **Amdahl Speedup Curve** - Shows saturation effect
2. **Efficiency vs Processors** - Shows declining returns
3. **Problem Size Effect** - Sequential fraction across N values
4. **Amdahl vs Gustafson** - Direct comparison
5. **Function Breakdown** - Per-function contribution analysis

## Performance Implications

### For This Program
1. **Bottleneck**: The `add_noise()` function limits scalability
2. **Solution**: 
   - Remove the data dependency if possible
   - Use a different algorithm that allows parallelization
   - Accept the limitation (some problems are inherently sequential)

3. **Optimal Processor Count**: 8-16 processors
   - Beyond this, efficiency drops below 20%
   - Cost/benefit ratio becomes unfavorable

### General Lessons
1. **Identify Sequential Parts**: They limit your maximum speedup
2. **Measure, Don't Guess**: Profile to find true bottlenecks
3. **Amdahl is Pessimistic**: But often realistic for fixed problems
4. **Gustafson is Optimistic**: Applicable when work can scale
5. **Efficiency Matters**: Raw speedup isn't everything

## Additional Analysis

### Cost-Effectiveness
At what point do additional processors stop being worthwhile?

- **p=8**: 2.54x speedup, 31.7% efficiency - Good value
- **p=16**: 2.85x speedup, 17.8% efficiency - Questionable
- **p=32**: 3.04x speedup, 9.5% efficiency - Poor value
- **p=64**: 3.14x speedup, 4.9% efficiency - Wasteful

### To Improve This Program
1. **Eliminate Sequential Dependency**: Redesign `add_noise()`
2. **Overlap Computation**: Pipeline the sequential and parallel parts
3. **Use Better Algorithms**: Tree-based reduction instead of linear
4. **Accept Reality**: Some problems are inherently sequential

## References

- **Amdahl's Law**: Gene Amdahl, 1967
- **Gustafson's Law**: John Gustafson, 1988
- **Valgrind**: https://valgrind.org/
- **Callgrind**: https://valgrind.org/docs/manual/cl-manual.html

## Conclusion

This exercise demonstrates that:
1. **Parallelization has limits** - Sequential portions create hard bounds
2. **Measurement is crucial** - Profile to understand true bottlenecks
3. **Context matters** - Strong vs weak scaling scenarios differ
4. **Efficiency declines** - More processors ≠ proportional speedup
5. **Algorithm design is key** - Some algorithms parallelize better than others

The measured sequential fraction of ~30.7% means we can never exceed 3.26x speedup regardless of processor count. This is a fundamental limitation that can only be addressed by redesigning the algorithm or accepting the constraint.
