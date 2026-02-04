# Exercise 4: Matrix Multiplication with Parallel Scaling Analysis

## Overview

This exercise demonstrates **near-perfect parallel scaling** through analysis of a matrix multiplication program. Unlike Exercise 3, where a large sequential fraction limited scalability, this program shows how O(N³) parallel work can dominate O(N) sequential overhead.

## The Program

```c
void generate_noise(double *noise)  // O(N) - Sequential
void init_matrix(double *M)         // O(N²) - Parallelizable  
void matmul(A, B, C, noise)         // O(N³) - Highly parallelizable
```

**Key Characteristic**: As matrix size N increases, the parallelizable work (O(N³)) grows much faster than sequential work (O(N)), leading to **improving scalability**.

## Files in this Directory

### Source Code
- **program.c** - Main program (N=512)
- **program_N128.c** - Fast profiling version (N=128)
- **program_N256.c** - Medium profiling version (N=256)
- **program**, **program_N128**, **program_N256** - Compiled binaries

### Analysis & Results
- **answers.md** - Complete answers with detailed analysis
- **calculate_scaling.py** - Python script for speedup calculations
- **visualizations.ipynb** - Jupyter notebook with plots
- **profiling_results.csv** - Valgrind profiling data
- **scaling_results.csv** - Speedup calculations
- **callgrind.out.*** - Valgrind output files

### Generated Plots
- **q1_q2_amdahl_scaling.png** - Near-perfect linear scaling
- **q3_amdahl_vs_gustafson.png** - Both laws converge
- **q4_problem_size_effect.png** - fs decreases with N
- **q5_comparison_with_ex3.png** - Dramatic contrast
- **instruction_distribution.png** - Work distribution visualization

## Quick Start

### 1. Compile
```bash
cd TP2/ex4
gcc -O2 -g program_N128.c -o program_N128
gcc -O2 -g program_N256.c -o program_N256
gcc -O2 -g program.c -o program
```

### 2. Profile
```bash
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N128 ./program_N128
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N256 ./program_N256
callgrind_annotate callgrind.out.N128
```

### 3. Calculate
```bash
python3 calculate_scaling.py
```

### 4. Visualize
```bash
jupyter notebook visualizations.ipynb
```

## Key Results

### Sequential Fraction (fs)

| Matrix Size N | Sequential Part | Parallel Part | fs | Trend |
|---------------|----------------|---------------|-----|-------|
| 128           | 641 instr (O(N)) | 15.2M instr (O(N³)) | **0.0042%** | ↓ |
| 256           | 1,281 instr | 119.6M instr | **0.0011%** | ↓ |
| 512           | 2,562 instr | 949.9M instr | **0.0003%** | ↓ |

**Critical Insight**: Sequential fraction decreases as fs ∝ 1/N²

### Speedup at p=64

| Metric | N=128 | N=256 | N=512 |
|--------|-------|-------|-------|
| **Amdahl Speedup** | 63.83x | 63.96x | 63.99x |
| **Gustafson Speedup** | 64.00x | 64.00x | 64.00x |
| **Efficiency** | 99.7% | 99.9% | 100.0% |
| **Max Speedup** | 23,904x | 93,352x | 370,765x |

### Comparison with Exercise 3

| Metric | Exercise 3 | Exercise 4 (N=512) |
|--------|-----------|-------------------|
| Sequential fraction | **30.7%** (constant) | **0.0003%** (→0) |
| Speedup @ p=64 | 3.14x | **63.99x** |
| Efficiency @ p=64 | 4.9% | **100.0%** |
| Max speedup | 3.26x | **370,765x** |
| Scalability | Poor | **Excellent** |

## Why Such Different Behavior?

### Exercise 3: Limited Scalability
```
Sequential:     O(N)  ──┐
Parallel:       O(N)  ──┤ Same complexity!
                        ├→ fs = constant ≈ 30%
Sequential fraction:    │  S_max ≈ 3.26x
                        └→ Poor scaling
```

### Exercise 4: Excellent Scalability
```
Sequential:     O(N)   ──┐
Parallel:       O(N³)  ──┤ Cubic >> Linear!
                         ├→ fs ∝ 1/N² → 0
Sequential fraction:     │  S_max → ∞
                         └→ Near-linear scaling
```

## Detailed Analysis

### Question 1: Sequential Fraction Measurement
- **Method**: Valgrind/Callgrind profiling
- **Result**: fs ranges from 0.0042% (N=128) to 0.0003% (N=512)
- **Trend**: Decreases with problem size

### Question 2: Strong Scaling (Amdahl)
- **Near-perfect linear speedup**: S(64) ≈ 64x
- **Efficiency > 99%** even at 64 processors
- **No saturation** within reasonable processor counts

### Question 3: Weak Scaling (Gustafson)
- **Identical to Amdahl**: Both predict S(p) ≈ p
- **Convergence**: When fs → 0, both laws agree
- **Implication**: Strong and weak scaling both excellent

### Question 4: Problem Size Effect
- **Larger N = Better scaling**: fs ∝ 1/N²
- **Maximum speedup grows**: S_max ∝ N²
- **Practical benefit**: Use larger matrices for better efficiency

### Question 5: Comparison with Exercise 3
- **Dramatic contrast** in scaling behavior
- **Different complexity classes** lead to different fs trends
- **Algorithm structure matters** more than hardware

## Mathematical Analysis

### Sequential Fraction Scaling
```
Total time:      T = T_seq + T_par
                T = O(N) + O(N³)

Sequential fraction:
                fs = O(N) / (O(N) + O(N³))
                fs = O(N) / O(N³)
                fs = O(1/N²)

As N → ∞:       fs → 0
```

### Maximum Speedup
```
S_max = 1/fs = O(N²)

N=128:  S_max ≈ 23,904x
N=256:  S_max ≈ 93,352x
N=512:  S_max ≈ 370,765x
N=1024: S_max ≈ 1,483,060x  (extrapolated)
```

## Practical Implications

### 1. Matrix Operations Scale Well
- Linear algebra is highly parallelizable
- Scientific computing benefits greatly from parallelization
- GPU acceleration is effective for these workloads

### 2. Problem Size Matters
- Larger problems have better parallel efficiency
- Don't parallelize small matrices
- Scale problem with available resources

### 3. Algorithm Design is Critical
- O(N³) work vs O(N) overhead → excellent scaling
- O(N) work vs O(N) overhead → poor scaling (Exercise 3)
- Choose algorithms with high computational intensity

### 4. Hardware Utilization
- Can efficiently use 64+ processors
- Near 100% efficiency achievable
- Investment in parallel hardware pays off

## Real-World Applications

### Similar Scaling Characteristics:
- **Matrix operations**: BLAS, LAPACK
- **FFT**: O(N log N) parallel, O(log N) sequential
- **Molecular dynamics**: O(N²) interactions
- **Deep learning**: Matrix multiplications dominate
- **Scientific simulations**: Physics, chemistry, engineering

### Limited Scaling (Like Exercise 3):
- **Scans/reductions**: Inherent dependencies
- **Graph algorithms**: Irregular access patterns
- **Sequential algorithms**: Fundamental limitations

## Performance Guidelines

### When to Parallelize Matrix Operations:
✅ **DO parallelize** when:
- N > 128 (sequential overhead negligible)
- Multiple processors available
- Computation-bound (not memory-bound)

❌ **DON'T parallelize** when:
- N < 64 (overhead may dominate)
- Memory bandwidth is bottleneck
- Communication cost > computation time

### Optimal Configuration:
- **N=128**: Good for 16-32 processors
- **N=256**: Excellent for 64+ processors
- **N=512**: Scales to 100s of processors
- **N=1024**: Can utilize 1000s of processors

## Comparison Summary

|Feature | Exercise 3 | Exercise 4 |
|--------|-----------|-----------|
| **Problem type** | Array operations | Matrix multiplication |
| **Sequential** | O(N) | O(N) |
| **Parallel** | O(N) | O(N³) |
| **fs behavior** | Constant | Decreases (∝ 1/N²) |
| **Scalability** | Limited | Excellent |
| **Optimal p** | 8-16 | 64+ |
| **Real-world** | Reductions, scans | Linear algebra, DL |

## Lessons Learned

1. **Computational Complexity Dominates**
   - O(N³) >> O(N) → excellent scaling
   - Algorithm's computational intensity is key

2. **Problem Size Effect Varies**
   - Exercise 3: fs constant → fixed limitations
   - Exercise 4: fs → 0 → improving scalability

3. **Amdahl vs Gustafson Convergence**
   - When fs ≈ 0, both laws predict linear scaling
   - Strong and weak scaling both achievable

4. **Hardware Investment ROI**
   - Exercise 3: Poor ROI beyond 16 processors
   - Exercise 4: Good ROI even at 64+ processors

5. **Algorithm Selection Matters**
   - Matrix multiplication: Ideal for parallelization
   - Sequential algorithms: Fundamental limits

## References

- **Amdahl's Law**: Gene Amdahl, 1967
- **Gustafson's Law**: John Gustafson, 1988
- **BLAS**: Basic Linear Algebra Subprograms
- **Matrix Multiplication**: Strassen, Coppersmith-Winograd algorithms

## Conclusion

Exercise 4 demonstrates the **best-case scenario** for parallel computing:
- Negligible sequential overhead (< 0.01%)
- Massive parallelizable workload (O(N³))
- Near-perfect linear scaling
- Efficiency > 99% at 64 processors

This contrasts dramatically with Exercise 3, highlighting that:
- **Not all algorithms parallelize equally well**
- **Computational complexity determines scalability**
- **Problem size can improve or limit parallel efficiency**
- **Algorithm design matters more than hardware**

Matrix multiplication represents the gold standard for parallelization, while Exercise 3's array operations show the challenges of parallelizing algorithms with significant sequential dependencies.
