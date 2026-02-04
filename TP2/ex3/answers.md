# Exercise 3: Amdahl's Law and Gustafson's Law

## Question 1 — Code Analysis

### 1. Strictly Sequential Part

The strictly sequential part of the program is:

**`add_noise(a)`** - This function is **inherently sequential** because:
- Each iteration depends on the previous one: `a[i] = a[i-1] * 1.0000001`
- There is a data dependency chain that cannot be broken
- Cannot be parallelized without changing the algorithm

### 2. Parallelizable Part

The parallelizable parts are:

1. **`init_b(b)`** - Initialization of array b
   - Each element `b[i] = i * 0.5` is independent
   - No data dependencies between iterations
   - Can be parallelized

2. **`compute_addition(a, b, c)`** - Element-wise addition
   - Each element `c[i] = a[i] + b[i]` is independent
   - No data dependencies between iterations
   - Can be parallelized

3. **`reduction(c)`** - Sum reduction
   - Can be parallelized using parallel reduction algorithms
   - Though it has dependencies, it can be done in O(log N) time with parallel reduction
   - Individual partial sums can be computed independently, then combined

### 3. Time Complexity

For N elements:

| Function | Time Complexity | Type |
|----------|----------------|------|
| `add_noise(a)` | O(N) | Sequential |
| `init_b(b)` | O(N) | Parallelizable → O(N/p) |
| `compute_addition(a,b,c)` | O(N) | Parallelizable → O(N/p) |
| `reduction(c)` | O(N) | Parallelizable → O(N/p) or O(log N) with tree reduction |

Where p is the number of processors.

---

## Question 2 — Measuring the Sequential Fraction

### Valgrind Profiling Results

From the Callgrind output:

```
Total Instructions: 6,500,157,241

Function breakdown:
- compute_addition: 2,200,000,014 (33.85%)
- add_noise:        1,799,999,997 (27.69%)
- init_b:           1,300,000,012 (20.00%)
- reduction:        1,200,000,013 (18.46%)
```

### Sequential Fraction Calculation

The sequential part is only `add_noise`:

```
fs = Instructions(add_noise) / Total Instructions
fs = 1,799,999,997 / 6,500,157,241
fs ≈ 0.2769 or 27.69%
```

The parallelizable part:
```
fp = (init_b + compute_addition + reduction) / Total
fp = (1,300,000,012 + 2,200,000,014 + 1,200,000,013) / 6,500,157,241
fp = 4,700,000,039 / 6,500,157,241
fp ≈ 0.7231 or 72.31%
```

**Sequential Fraction: fs ≈ 0.277 (27.7%)**

---

## Question 3 — Strong Scaling (Amdahl's Law)

Amdahl's Law formula:
```
S(p) = 1 / (fs + fp/p)
```

Where:
- fs = 0.2769 (sequential fraction)
- fp = 0.7231 (parallelizable fraction)
- p = number of processors

### Theoretical Speedup Calculations

| Processors (p) | Speedup S(p) | Efficiency E(p) = S(p)/p |
|----------------|--------------|--------------------------|
| 1              | 1.000        | 1.000                    |
| 2              | 1.564        | 0.782                    |
| 4              | 2.296        | 0.574                    |
| 8              | 3.053        | 0.382                    |
| 16             | 3.569        | 0.223                    |
| 32             | 3.801        | 0.119                    |
| 64             | 3.901        | 0.061                    |

### Maximum Theoretical Speedup

The maximum speedup (as p → ∞):
```
S_max = 1 / fs = 1 / 0.2769 ≈ 3.61
```

### Why Speedup Saturates

The speedup saturates because:
1. **Amdahl's Law Limitation**: The sequential part (27.7%) cannot be parallelized
2. As p increases, the parallelizable part's time approaches zero
3. We're left with only the sequential part's execution time
4. The maximum speedup is bounded by 1/fs ≈ 3.61x
5. Beyond p=16-32, adding more processors provides diminishing returns

---

## Question 4 — Effect of Problem Size

### Expected Results for Different N Values

| N | Sequential Part (add_noise) | Parallelizable Parts | fs |
|---|----------------------------|---------------------|-----|
| 5×10⁶ | ~90M instructions | ~235M instructions | ~27.7% |
| 10⁷ | ~180M instructions | ~470M instructions | ~27.7% |
| 10⁸ | ~1.8B instructions | ~4.7B instructions | ~27.7% |

### Analysis

For this particular problem:
- **fs remains approximately constant** across different N values
- Both sequential and parallelizable parts scale linearly with N
- The ratio remains the same (~27.7% sequential)
- This is different from problems where overhead is constant (leading to decreasing fs with larger N)

### Speedup Curves Comparison

All three problem sizes will have similar speedup curves because:
- Same sequential fraction (fs ≈ 0.277)
- Same maximum speedup (S_max ≈ 3.61)
- Same saturation behavior

---

## Question 5 — Weak Scaling (Gustafson's Law)

Gustafson's Law formula:
```
S(p) = p - fs(p - 1) = fs + p × fp
```

Where:
- fs = 0.2769 (sequential fraction)
- fp = 0.7231 (parallelizable fraction)
- p = number of processors

### Gustafson Speedup Calculations

| Processors (p) | Gustafson S(p) | Amdahl S(p) |
|----------------|----------------|-------------|
| 1              | 1.000          | 1.000       |
| 2              | 1.723          | 1.564       |
| 4              | 3.169          | 2.296       |
| 8              | 6.062          | 3.053       |
| 16             | 11.847         | 3.569       |
| 32             | 23.417         | 3.801       |
| 64             | 46.556         | 3.901       |

### Comparison of Amdahl's vs Gustafson's Laws

**Key Differences:**

1. **Problem Size Assumption:**
   - **Amdahl**: Fixed problem size (strong scaling)
   - **Gustafson**: Problem size grows with p (weak scaling)

2. **Speedup Behavior:**
   - **Amdahl**: Saturates at S_max = 1/fs ≈ 3.61
   - **Gustafson**: Grows nearly linearly with p

3. **Practical Implications:**
   - **Amdahl**: Pessimistic - shows limitations of parallelization
   - **Gustafson**: Optimistic - shows potential when problem scales

4. **Use Cases:**
   - **Amdahl**: When problem size is fixed (e.g., specific dataset)
   - **Gustafson**: When problem size increases with resources (e.g., higher resolution simulations)

### Interpretation

For this program:
- **Amdahl's Law** applies when we process a fixed N=100M elements on different processor counts
- **Gustafson's Law** applies if we increase N proportionally with p (e.g., N=100M/p per processor)
- The 27.7% sequential fraction significantly limits Amdahl speedup but allows good Gustafson scaling

---

## Summary

- **Sequential Fraction**: fs ≈ 27.7% (add_noise function)
- **Max Amdahl Speedup**: ~3.61x (saturates quickly)
- **Gustafson Scaling**: Near-linear for weak scaling scenarios
- **Bottleneck**: The data-dependent add_noise function limits parallelization
