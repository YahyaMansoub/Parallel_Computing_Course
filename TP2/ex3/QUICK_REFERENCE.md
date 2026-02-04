# Exercise 3 - Quick Reference Card

## Formulas

### Amdahl's Law (Strong Scaling)
```
S(p) = 1 / (fs + fp/p)
S(p) = 1 / (fs + (1-fs)/p)

Where:
- S(p) = Speedup with p processors
- fs = Sequential fraction
- fp = Parallelizable fraction (1 - fs)
- p = Number of processors

Maximum Speedup: S_max = 1/fs
```

### Gustafson's Law (Weak Scaling)
```
S(p) = fs + p × fp
S(p) = p - fs(p - 1)

Where:
- S(p) = Scaled speedup with p processors
- fs = Sequential fraction
- fp = Parallelizable fraction
- p = Number of processors
```

### Efficiency
```
E(p) = S(p) / p

Where:
- E(p) = Parallel efficiency
- S(p) = Speedup
- p = Number of processors

E(p) = 1.0 means perfect scaling (100% efficient)
```

## Measured Values (from Profiling)

### Sequential Fraction: fs ≈ 0.307 (30.7%)
- Source: `add_noise()` function
- Data dependency prevents parallelization

### Parallelizable Fraction: fp ≈ 0.693 (69.3%)
- Sources: `init_b()`, `compute_addition()`, `reduction()`
- No dependencies between iterations

### Maximum Theoretical Speedup
```
S_max = 1 / 0.307 ≈ 3.26x
```

## Speedup Table

| p  | Amdahl | Gustafson | Efficiency | Notes |
|----|--------|-----------|------------|-------|
| 1  | 1.00   | 1.00      | 100.0%     | Baseline |
| 2  | 1.53   | 1.69      | 76.5%      | Good scaling |
| 4  | 2.08   | 3.08      | 52.0%      | Acceptable |
| 8  | 2.54   | 5.85      | 31.7%      | Optimal point |
| 16 | 2.85   | 11.39     | 17.8%      | Diminishing returns |
| 32 | 3.04   | 22.47     | 9.5%       | Poor efficiency |
| 64 | 3.14   | 44.63     | 4.9%       | Wasteful |

## Key Concepts

### Strong Scaling (Amdahl)
- **Fixed problem size**
- Problem: Process 100M elements
- Question: "How fast can I process THIS dataset?"
- Reality: Most real-world scenarios
- Limitation: Sequential portion limits speedup

### Weak Scaling (Gustafson)
- **Growing problem size**
- Problem: Process 100M × p elements
- Question: "How much MORE work can I do?"
- Reality: Simulations, rendering, big data
- Advantage: Better scaling characteristics

## Valgrind Commands

### Profile the program
```bash
valgrind --tool=callgrind --callgrind-out-file=output.out ./program
```

### Analyze results
```bash
callgrind_annotate output.out
```

### Visualize (if kcachegrind installed)
```bash
kcachegrind output.out
```

## Calculate Sequential Fraction

From Callgrind output:
```
fs = Instructions(sequential_part) / Total_Instructions

For this program:
fs = Instructions(add_noise) / Total_Instructions
fs = 50,000,002 / 162,642,161
fs ≈ 0.3074 (for N=10M)
```

## When to Use Each Law

### Use Amdahl's Law when:
- Fixed dataset to process
- Specific problem to solve faster
- Limited resources/data
- Example: Analyze this specific dataset

### Use Gustafson's Law when:
- Problem size can grow
- More resources = more work
- Scalable workloads
- Example: Simulate at higher resolution

## Interpretation Guidelines

### Good Speedup
- S(p) close to p (linear)
- Efficiency > 70%
- Example: S(8) = 7.2

### Poor Speedup
- S(p) << p
- Efficiency < 20%
- Example: S(64) = 3.14

### Saturation
- S(p) stops growing
- Adding processors doesn't help
- Reached sequential limit

## Optimization Strategies

### To Improve This Program:
1. **Eliminate `add_noise()` dependency**
   - Use different algorithm
   - Pre-compute values
   - Accept limitation

2. **Better parallelization**
   - Tree-based reduction (O(log N))
   - SIMD vectorization
   - GPU acceleration

3. **Overlap computation**
   - Pipeline stages
   - Async operations
   - Hide sequential latency

## Common Mistakes

❌ **Wrong**: Expecting linear speedup with 30% sequential code
✓ **Right**: Maximum 3.26x speedup regardless of processors

❌ **Wrong**: Using Gustafson for fixed-size problems
✓ **Right**: Use Amdahl for fixed problems, Gustafson for scalable

❌ **Wrong**: Ignoring efficiency, only looking at speedup
✓ **Right**: Consider cost-effectiveness (speedup/processors)

❌ **Wrong**: More processors always better
✓ **Right**: Diminishing returns after optimal point (~8-16 here)

## File Locations

- **Theoretical answers**: `answers.md`
- **Visualizations**: `visualizations.ipynb`
- **Calculations**: `calculate_scaling.py`
- **Data**: `profiling_results.csv`, `scaling_results.csv`
- **Profiling**: `callgrind.out.N5M`, `callgrind.out.N10M`
- **Quick start**: `run_analysis.sh`
- **Overview**: `README.md`

## Quick Commands

```bash
# Compile all
gcc -O2 -g program.c -o program

# Profile
valgrind --tool=callgrind ./program

# Calculate
python3 calculate_scaling.py

# Visualize
jupyter notebook visualizations.ipynb

# Run everything
./run_analysis.sh
```

## Expected Results

### Question 1
- Sequential: `add_noise()` (data dependency)
- Parallel: `init_b()`, `compute_addition()`, `reduction()`
- All O(N) complexity

### Question 2
- fs ≈ 30.7% (measured via Valgrind)

### Question 3
- Amdahl speedup saturates at ~3.26x
- Efficiency drops rapidly after p=8

### Question 4
- fs remains constant across N values
- Both parts scale linearly with N

### Question 5
- Gustafson shows near-linear scaling
- Much better than Amdahl for large p
- 44.6x vs 3.1x at p=64

## Important Insights

1. **30.7% sequential = 3.26x max speedup** (fundamental limit)
2. **Optimal processor count: 8-16** (balance speedup/efficiency)
3. **fs constant across N** (proportional scaling)
4. **Algorithm matters more than hardware** (fix the bottleneck!)
5. **Context determines law** (Amdahl vs Gustafson)
