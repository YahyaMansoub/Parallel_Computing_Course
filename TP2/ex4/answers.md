# Exercise 4: Matrix Multiplication with Scaling Analysis

## Program Overview

This exercise analyzes a C program that combines:
1. **Sequential noise generation** - O(N) complexity with data dependency
2. **Parallelizable matrix initialization** - O(N²) complexity
3. **Highly parallelizable matrix multiplication** - O(N³) complexity

The key difference from Exercise 3: **the parallelizable part dominates as N increases**!

---

## Question 1 — Measuring Sequential Fraction (Valgrind/Callgrind)

### Profiling Methodology

```bash
# Compile with optimization and debug symbols
gcc -O2 -g program_N128.c -o program_N128
gcc -O2 -g program_N256.c -o program_N256

# Profile with Valgrind
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N128 ./program_N128
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N256 ./program_N256

# Analyze results
callgrind_annotate callgrind.out.N128
callgrind_annotate callgrind.out.N256
```

### Profiling Results

| Matrix Size N | Total Instructions | generate_noise | init_matrix (×2) | matmul | Sequential Fraction (fs) |
|---------------|-------------------|----------------|------------------|--------|-------------------------|
| 128           | 15,322,709        | 641            | 352,282          | 14,828,682 | **0.000042 (0.0042%)** |
| 256           | 119,584,319       | 1,281          | 1,409,050        | 118,032,650 | **0.000011 (0.0011%)** |
| 512           | ~949,899,962      | ~2,562         | ~5,636,200       | ~944,261,200 | **0.000003 (0.0003%)** |

### Sequential Fraction Calculation

```
fs = Instructions(generate_noise) / Total Instructions

For N=128:  fs = 641 / 15,322,709 ≈ 0.0000418 (0.0042%)
For N=256:  fs = 1,281 / 119,584,319 ≈ 0.0000107 (0.0011%)
For N=512:  fs = 2,562 / 949,899,962 ≈ 0.0000027 (0.0003%)
```

### Key Observation

**Sequential fraction decreases dramatically with problem size!**

- `generate_noise()`: O(N) - grows linearly
- `matmul()`: O(N³) - grows cubically
- As N increases, sequential part becomes negligible

---

## Question 2 — Strong Scaling (Amdahl's Law)

### Amdahl's Law Formula

```
S(p) = 1 / (fs + (1-fs)/p)
```

### Speedup Results for Different Matrix Sizes

#### N = 128 (fs = 0.000042)

| Processors (p) | Amdahl Speedup S(p) | Efficiency E(p) | Maximum Theoretical |
|----------------|---------------------|-----------------|---------------------|
| 1              | 1.000               | 100.0%          | S_max = 23,904x    |
| 2              | 2.000               | 100.0%          |                     |
| 4              | 3.999               | 100.0%          |                     |
| 8              | 7.998               | 100.0%          |                     |
| 16             | 15.990              | 99.9%           |                     |
| 32             | 31.959              | 99.9%           |                     |
| 64             | 63.832              | 99.7%           |                     |

#### N = 256 (fs = 0.000011)

| Processors (p) | Amdahl Speedup S(p) | Efficiency E(p) | Maximum Theoretical |
|----------------|---------------------|-----------------|---------------------|
| 1              | 1.000               | 100.0%          | S_max = 93,352x    |
| 2              | 2.000               | 100.0%          |                     |
| 4              | 4.000               | 100.0%          |                     |
| 8              | 7.999               | 100.0%          |                     |
| 16             | 15.997              | 100.0%          |                     |
| 32             | 31.989              | 100.0%          |                     |
| 64             | 63.957              | 99.9%           |                     |

#### N = 512 (fs = 0.000003)

| Processors (p) | Amdahl Speedup S(p) | Efficiency E(p) | Maximum Theoretical |
|----------------|---------------------|-----------------|---------------------|
| 1              | 1.000               | 100.0%          | S_max = 370,765x   |
| 2              | 2.000               | 100.0%          |                     |
| 4              | 4.000               | 100.0%          |                     |
| 8              | 8.000               | 100.0%          |                     |
| 16             | 15.999              | 100.0%          |                     |
| 32             | 31.997              | 100.0%          |                     |
| 64             | 63.989              | 100.0%          |                     |

### Analysis

**Near-perfect linear scaling!**

- Sequential fraction is so small (< 0.01%) that it's negligible
- Speedup is almost perfectly linear with processor count
- Efficiency remains above 99% even at 64 processors
- Maximum theoretical speedup is extremely high

**Why such good scaling?**
- Matrix multiplication dominates execution time (96-99%)
- Each element of result matrix is independent
- O(N³) work vastly outweighs O(N) sequential overhead

---

## Question 3 — Weak Scaling (Gustafson's Law)

### Gustafson's Law Formula

```
S(p) = p - fs(p - 1) = fs + p × fp
```

### Comparison: Amdahl vs Gustafson

#### N = 256

| Processors (p) | Amdahl S(p) | Gustafson S(p) | Difference |
|----------------|-------------|----------------|------------|
| 1              | 1.000       | 1.000          | 0.000      |
| 2              | 2.000       | 2.000          | 0.000      |
| 4              | 4.000       | 4.000          | 0.000      |
| 8              | 7.999       | 8.000          | 0.001      |
| 16             | 15.997      | 16.000         | 0.003      |
| 32             | 31.989      | 32.000         | 0.011      |
| 64             | 63.957      | 64.000         | 0.043      |

### Key Observation

**Amdahl and Gustafson predictions are nearly identical!**

- Both show near-linear scaling
- Difference is negligible (< 0.1%)
- Sequential fraction is so small that strong and weak scaling converge

**Why?**
- When fs ≈ 0, both formulas approach: S(p) ≈ p
- The sequential overhead is insignificant
- Problem is "embarrassingly parallelizable"

---

## Question 4 — Effect of Problem Size

### Sequential Fraction vs Matrix Size

| Matrix Size N | Sequential Part (generate_noise) | Parallel Part (matmul) | Sequential Fraction fs |
|---------------|----------------------------------|------------------------|----------------------|
| 128           | ~641 instructions (O(N))         | ~14.8M (O(N³))         | 0.0042%              |
| 256           | ~1,281 instructions (O(N))       | ~118M (O(N³))          | 0.0011%              |
| 512           | ~2,562 instructions (O(N))       | ~944M (O(N³))          | 0.0003%              |

### Scaling Behavior

```
Sequential part:     T_seq = O(N)
Parallelizable part: T_par = O(N³)

Sequential fraction: fs = O(N) / (O(N) + O(N³)) = O(1/N²)

As N → ∞:  fs → 0
```

### Impact on Speedup

As matrix size increases:
1. **Sequential fraction decreases** - fs ∝ 1/N²
2. **Maximum speedup increases** - S_max = 1/fs grows
3. **Scalability improves** - Better efficiency at high processor counts

**This is opposite to Exercise 3!**
- Exercise 3: fs constant (~30.7%), limited scalability
- Exercise 4: fs → 0, unlimited scalability for large N

---

## Question 5 — Comparison with Exercise 3

### Fundamental Difference

| Aspect | Exercise 3 | Exercise 4 |
|--------|-----------|-----------|
| **Sequential part** | `add_noise()` - O(N) | `generate_noise()` - O(N) |
| **Parallel part** | Element operations - O(N) | Matrix multiplication - O(N³) |
| **Sequential fraction** | **Constant (~30.7%)** | **Decreases (∝ 1/N²)** |
| **Scalability** | **Limited (S_max ≈ 3.26x)** | **Excellent (S_max → ∞)** |

### Detailed Comparison

#### Sequential Fraction

**Exercise 3:**
```
fs ≈ 0.307 (30.7%) - remains constant as N changes
Both sequential and parallel parts are O(N)
```

**Exercise 4:**
```
fs = 0.000042 (N=128) → 0.000003 (N=512)
Sequential O(N), parallel O(N³)
```

#### Maximum Speedup

**Exercise 3:**
```
S_max = 1/0.307 ≈ 3.26x (hard limit)
Adding processors beyond ~16 is wasteful
```

**Exercise 4:**
```
N=128:  S_max ≈ 23,904x
N=256:  S_max ≈ 93,352x
N=512:  S_max ≈ 370,765x
Practically unlimited scaling potential
```

#### Speedup at p=64

| Metric | Exercise 3 | Exercise 4 (N=512) |
|--------|-----------|-------------------|
| Amdahl Speedup | 3.14x | 63.99x |
| Gustafson Speedup | 44.63x | 64.00x |
| Efficiency | 4.9% | 100.0% |

### Speedup Curves Comparison

**Exercise 3 (fs = 0.307):**
```
p=1:  S=1.00   (100% efficiency)
p=8:  S=2.54   (31.7% efficiency) ← Optimal point
p=16: S=2.85   (17.8% efficiency)
p=64: S=3.14   (4.9% efficiency)  ← Saturated
```

**Exercise 4 (fs = 0.000003, N=512):**
```
p=1:  S=1.00   (100.0% efficiency)
p=8:  S=8.00   (100.0% efficiency)
p=16: S=16.00  (100.0% efficiency)
p=64: S=63.99  (100.0% efficiency) ← Still linear!
```

### Why Such Different Behavior?

**Exercise 3: Array Operations**
- All operations (sequential + parallel) scale linearly: O(N)
- Sequential portion is fixed percentage (~30%)
- Amdahl's Law severely limits scalability
- Adding processors hits diminishing returns quickly

**Exercise 4: Matrix Multiplication**
- Sequential part: O(N) - linear
- Parallel part: O(N³) - cubic
- Sequential portion shrinks with problem size: O(1/N²)
- Near-perfect scaling for reasonably large matrices
- Can efficiently use many more processors

### Practical Implications

**Exercise 3 Type Problems:**
- Sequential bottleneck is fundamental
- Limited parallelization benefit
- Focus on algorithm redesign to reduce sequential portion
- Optimal: 8-16 processors

**Exercise 4 Type Problems:**
- Highly parallelizable
- Excellent scaling characteristics
- Can benefit from large processor counts
- Scale problem size with available resources

---

## Summary Table

| Metric | Exercise 3 | Exercise 4 (N=512) |
|--------|-----------|-------------------|
| Sequential fraction | 30.7% (constant) | 0.0003% (→0) |
| Parallel fraction | 69.3% | 99.9997% |
| Max theoretical speedup | 3.26x | 370,765x |
| Speedup at p=64 | 3.14x | 63.99x |
| Efficiency at p=64 | 4.9% | 100.0% |
| Scalability | Poor | Excellent |
| Optimal processors | 8-16 | 64+ |
| Amdahl vs Gustafson | Large difference | Nearly identical |

---

## Key Insights

### 1. **Computational Complexity Matters**
- O(N³) work dominates O(N) overhead
- Larger problems have better parallel efficiency
- Not all problems are created equal

### 2. **Problem Size Effect**
- Exercise 3: fs constant, scalability fixed
- Exercise 4: fs → 0, scalability improves
- Matrix operations have inherent parallelism

### 3. **When Amdahl's Law Doesn't Bite**
- With fs < 0.01%, Amdahl's Law is permissive
- Linear scaling is achievable
- Hardware can be fully utilized

### 4. **Algorithm Structure is Critical**
- Data dependencies limit parallelism (Exercise 3)
- Independent computations enable parallelism (Exercise 4)
- Design matters more than hardware

### 5. **Real-World Applications**
- Exercise 3 type: Simple reductions, scans
- Exercise 4 type: Matrix ops, FFT, molecular dynamics
- Know your algorithm's parallelization potential

---

## Conclusion

**Exercise 4 demonstrates the ideal parallel computing scenario:**
- Tiny sequential fraction (< 0.01%)
- Massive parallelizable workload (O(N³))
- Near-linear speedup achievable
- Excellent efficiency even at high processor counts

**This contrasts sharply with Exercise 3:**
- Large sequential fraction (30.7%)
- Limited parallelizable workload
- Speedup saturates quickly
- Poor efficiency beyond 8-16 processors

**The lesson:** Not all parallel programs are equal. Matrix multiplication represents the "best case" for parallelization, while array operations with dependencies represent more challenging scenarios. Understanding your algorithm's computational complexity and dependency structure is essential for predicting parallel performance.
