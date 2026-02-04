# Exercise 1: Loop Optimizations - Complete Analysis

## Problem Statement

Analyze the performance of manual loop unrolling for array summation with different:
- **Unrolling factors**: U = 1, 2, 4, 8, 16, 32
- **Data types**: `double`, `float`, `int`, `short`
- **Optimization levels**: `-O0`, `-O2`

Compare manual unrolling effectiveness and identify bandwidth-limited vs compute-limited regimes.

---

## Experimental Setup

**Hardware**: Standard x86_64 processor with ~30 GB/s memory bandwidth  
**Software**: GCC compiler with `-O0` (no optimization) and `-O2` (optimizations enabled)  
**Benchmark**: Sum 1,000,000 array elements with varying unroll factors  
**Timing**: High-resolution `clock_gettime(CLOCK_MONOTONIC)` in milliseconds

---

## Results Summary

### Performance at -O0 (No Compiler Optimization)

| Data Type | Size | U=1 (ms) | Best U | Best Time (ms) | Speedup | BW Limit (ms) | Efficiency |
|-----------|------|----------|--------|----------------|---------|---------------|------------|
| `double`  | 8B   | 2.737    | 4      | 2.637          | 1.04x   | 0.267         | 10.1%      |
| `float`   | 4B   | 2.669    | 4      | 2.625          | 1.02x   | 0.133         | 5.1%       |
| `int`     | 4B   | 2.123    | 16     | 0.631          | 3.36x   | 0.133         | 21.1%      |
| `short`   | 2B   | 2.039    | 16     | 0.597          | 3.42x   | 0.067         | 11.2%      |

**Key Observations at -O0:**
- **Floating-point types** (`double`, `float`): Minimal benefit from unrolling (~1.02-1.04x)
  - Likely due to complex FP instruction latencies
  - Loop overhead relatively small compared to FP operations
  
- **Integer types** (`int`, `short`): Significant speedup (~3.36-3.42x)
  - Fast integer addition makes loop overhead dominant
  - Unrolling U=16 achieves best performance
  - U=32 shows slight degradation (register pressure)

### Performance at -O2 (Compiler Optimization)

| Data Type | Size | U=1 (ms) | Best U | Best Time (ms) | Speedup | BW Limit (ms) | Efficiency |
|-----------|------|----------|--------|----------------|---------|---------------|------------|
| `double`  | 8B   | 1.175    | 8      | 0.732          | 1.60x   | 0.267         | 36.5%      |
| `float`   | 4B   | 0.791    | 16     | 0.752          | 1.05x   | 0.133         | 17.7%      |
| `int`     | 4B   | 0.353    | 16     | 0.156          | 2.26x   | 0.133         | 85.3%      |
| `short`   | 2B   | 0.166    | 16     | 0.151          | 1.10x   | 0.067         | 44.1%      |

**Key Observations at -O2:**
- Compiler already applies aggressive optimizations (vectorization, pipelining)
- Manual unrolling **still beneficial** for `double` (1.60x) and `int` (2.26x)
- `float` and `short` show minimal benefit (compiler handles well)
- `int` achieves **85% bandwidth efficiency** - approaching memory limit!

---

## Detailed Analysis by Data Type

### 1. Double Precision (8 bytes)

**At -O0:**
```
U=1:  2.737 ms  (baseline)
U=2:  2.685 ms  (1.02x speedup)
U=4:  2.637 ms  (1.04x speedup) ← Best
U=8:  2.703 ms  (1.01x)
U=16: 2.666 ms  (1.03x)
U=32: 2.681 ms  (1.02x)
```
- Very modest improvement (~4%)
- Floating-point operations dominate, not loop overhead

**At -O2:**
```
U=1:  1.175 ms  (2.33x faster than -O0)
U=2:  0.777 ms  (1.51x speedup vs U=1)
U=4:  0.762 ms  (1.54x)
U=8:  0.732 ms  (1.60x) ← Best
U=16: 0.760 ms  (1.55x)
U=32: 0.809 ms  (1.45x)
```
- Compiler optimization: **2.33x speedup** at U=1
- Manual unrolling adds **1.60x more** at U=8
- **Combined speedup**: 2.33 × 1.60 = **3.73x total** vs -O0/U=1
- Manual unrolling **STILL BENEFICIAL** even with -O2

### 2. Single Precision Float (4 bytes)

**At -O0:**
```
Best: U=4 at 2.625 ms (1.02x vs U=1)
```
- Almost no benefit from manual unrolling
- Compiler struggles with basic loop

**At -O2:**
```
Best: U=16 at 0.752 ms (1.05x vs U=1)
U=1:  0.791 ms  (3.37x faster than -O0)
```
- Compiler optimization: **3.37x speedup**
- Manual unrolling: only **1.05x** additional gain
- **Conclusion**: Compiler auto-vectorization (SSE/AVX) makes manual unrolling mostly unnecessary

### 3. Integer (4 bytes)

**At -O0:**
```
U=1:  2.123 ms
U=2:  1.202 ms  (1.77x)
U=4:  0.743 ms  (2.86x)
U=8:  0.656 ms  (3.24x)
U=16: 0.631 ms  (3.36x) ← Best
U=32: 0.662 ms  (3.21x) - slight degradation
```
- **Massive improvement** from unrolling
- Integer addition extremely fast → loop overhead dominates
- Clear diminishing returns after U=16

**At -O2:**
```
U=1:  0.353 ms  (6.02x faster than -O0)
U=16: 0.156 ms  (2.26x faster than U=1)
```
- **85% bandwidth efficiency** - approaching theoretical limit!
- Bandwidth limit: 0.133 ms → achieving 0.156 ms (very close)
- Manual unrolling **HIGHLY BENEFICIAL** (2.26x gain)

### 4. Short Integer (2 bytes)

**At -O0:**
```
U=1:  2.039 ms
U=16: 0.597 ms  (3.42x) ← Best speedup
```
- Similar pattern to `int`: large gains from unrolling

**At -O2:**
```
U=1:  0.166 ms  (12.25x faster than -O0!) 
U=16: 0.151 ms  (1.10x vs U=1)
```
- **Massive compiler optimization** (12.25x)
- Manual unrolling gives only 1.10x more
- Bandwidth limit: 0.067 ms → achieving 0.151 ms (44% efficiency)
- Still some room for improvement

---

## Bandwidth Analysis

### Theoretical Minimum Time (Bandwidth-Limited)

$$
T_{\text{min}} = \frac{N \times \text{sizeof(type)}}{\text{Bandwidth}}
$$

Assuming **30 GB/s** memory bandwidth:

| Type   | Size | Data (MB) | T_min (ms) | Best Measured | Efficiency |
|--------|------|-----------|------------|---------------|------------|
| double | 8B   | 7.63 MB   | 0.267 ms   | 0.732 ms      | 36.5%      |
| float  | 4B   | 3.81 MB   | 0.133 ms   | 0.752 ms      | 17.7%      |
| int    | 4B   | 3.81 MB   | 0.133 ms   | 0.156 ms      | **85.3%**  |
| short  | 2B   | 1.91 MB   | 0.067 ms   | 0.151 ms      | 44.1%      |

**Findings:**
- **Integer (int)** achieves highest bandwidth efficiency (85%)
- Compiler can aggressively optimize simple integer operations
- Floating-point types show lower efficiency (FP instruction overhead)
- No kernel reaches theoretical limit → some compute overhead remains

---

## Why Performance Improves Then Saturates

### 1. Loop Overhead Reduction

Without unrolling (U=1):
```c
for (int i = 0; i < N; i++)
    sum += a[i];
```
- **N iterations**: increment `i`, compare `i < N`, conditional branch
- **Overhead per iteration**: ~3-5 instructions

With unrolling (U=4):
```c
for (int i = 0; i < N; i += 4)
    sum += a[i] + a[i+1] + a[i+2] + a[i+3];
```
- **N/4 iterations**: same overhead, but amortized over 4 additions
- **Effective overhead**: reduced by factor of 4

**Impact**: Most significant for integer types where addition is fast

### 2. Instruction-Level Parallelism (ILP)

Modern CPUs can execute **multiple independent instructions per cycle**.

**Without unrolling:**
```assembly
add rax, [array + i*8]     ; load + add, has data dependency
inc i                       ; next iteration waits for add to complete
```
- Each addition depends on previous result
- No parallelism: **1 operation per cycle**

**With unrolling:**
```assembly
movsd xmm0, [array + i*8]      ; load element i
addsd xmm0, [array + i*8 + 8]  ; independent from next load
movsd xmm1, [array + i*8 + 16] ; can execute in parallel
addsd xmm1, [array + i*8 + 24]
addsd xmm0, xmm1               ; final combine
```
- Multiple independent loads/adds
- CPU can execute **2-4 operations per cycle**

**Benefit saturates** when unroll factor exceeds CPU's execution width (typically 4-8 operations).

### 3. Memory Bandwidth Saturation

As loop overhead decreases, **memory bandwidth becomes the bottleneck**.

**Performance progression:**
```
U=1:   Loop-bound     (slow)
U=2:   Less overhead  (faster)
U=4:   ILP helps      (much faster)
U=8:   Good ILP       (approaching limit)
U=16:  Near BW limit  (small gain)
U=32:  BW-limited     (no gain, possibly slower)
```

**At U=16-32**: Memory system can't deliver data fast enough, unrolling doesn't help.

### 4. Register Pressure

**Problem with excessive unrolling (U=32):**
- Need registers to hold intermediate sums
- x86-64 has only **16 general-purpose registers** (or 16 SIMD registers)
- High U forces **register spilling** to memory
- **Performance degradation** due to extra memory operations

**Evidence in results:**
- `int` at -O0: U=16 (0.631 ms) vs U=32 (0.662 ms) → slower!
- `short` at -O2: U=16 (0.151 ms) vs U=32 (0.204 ms) → 35% slower!

---

## Comparison: Manual Unrolling vs Compiler Optimization

### Does Manual Unrolling Help with -O2?

| Type   | Manual Speedup @ -O0 | Manual Speedup @ -O2 | Still Beneficial? |
|--------|----------------------|----------------------|-------------------|
| double | 1.04x                | 1.60x                | ✓ **YES**         |
| float  | 1.02x                | 1.05x                | ✗ **NO**          |
| int    | 3.36x                | 2.26x                | ✓ **YES**         |
| short  | 3.42x                | 1.10x                | ✓ **YES** (minor) |

### Compiler Optimization Effectiveness

| Type   | Compiler Speedup (U=1: -O0 → -O2) | Auto-Vectorization |
|--------|-----------------------------------|---------------------|
| double | 2.33x                             | Moderate            |
| float  | 3.37x                             | Good (SSE/AVX)      |
| int    | 6.02x                             | Excellent           |
| short  | **12.25x**                        | Exceptional         |

**Key Insights:**
1. **Compiler is very good** at optimizing integer code (6-12x speedup)
2. **Manual unrolling still helps** for `double` and `int` even with -O2
3. **Float operations**: Compiler auto-vectorization makes manual unrolling redundant
4. **Short type**: Compiler's 12x speedup + 1.1x manual = best overall

---

## Optimal Unrolling Factors

### Recommendations by Type and Optimization Level

| Type   | -O0 Best U | -O2 Best U | Reasoning                                    |
|--------|------------|------------|----------------------------------------------|
| double | U=4        | U=8        | FP overhead → moderate unrolling helps       |
| float  | U=4        | U=1        | Compiler vectorizes well, manual unnecessary |
| int    | U=16       | U=16       | Fast ops → high U needed, bandwidth-limited  |
| short  | U=16       | U=16       | Similar to int, very efficient at -O2        |

**General Rule:**
- **Without optimization (-O0)**: U=4-16 depending on type
- **With optimization (-O2)**: U=8-16 for int/double, U=1-4 for float
- **Above U=16**: Diminishing returns, risk of register pressure

---

## Practical Recommendations

### 1. When to Use Manual Unrolling

✓ **Use manual unrolling when:**
- Working with **integer types** (significant gains)
- Targeting **compute-bound** kernels (loop overhead matters)
- Need **portable performance** across compilers
- Compiler doesn't auto-vectorize your loop

✗ **Don't bother when:**
- Using **floating-point** with modern compilers (-O2/-O3)
- Already **bandwidth-limited** (unrolling won't help)
- Unroll factor exceeds **register availability** (U > 16)

### 2. Optimization Strategy

**Step 1**: Always enable compiler optimizations (`-O2` or `-O3`)  
**Step 2**: Profile to identify bottleneck (compute vs bandwidth)  
**Step 3**: If compute-bound, try manual unrolling U=4-8  
**Step 4**: Measure actual speedup, don't exceed U=16  
**Step 5**: Consider SIMD intrinsics if manual unrolling insufficient

### 3. Type-Specific Advice

**Double precision:**
- Use `-O2` (2.3x speedup)
- Add manual U=8 (extra 1.6x)
- Total: **3.7x improvement**

**Float:**
- Just use `-O2` (3.4x speedup)
- Manual unrolling gives minimal benefit
- Compiler auto-vectorization is sufficient

**Integer (int/short):**
- Use `-O2` (6-12x speedup!)
- Add manual U=16 (extra 1.1-2.3x)
- Most likely to hit bandwidth limits

---

## Experimental Validation

### Code Example: U=4 Manual Unrolling

```c
// Baseline (U=1)
for (int i = 0; i < N; i++)
    sum += a[i];

// Manual unroll U=4
int i = 0;
for (; i + 4 <= N; i += 4) {
    sum += a[i] + a[i+1] + a[i+2] + a[i+3];
}
// Handle remaining elements
for (; i < N; i++)
    sum += a[i];
```

### Compilation Commands

```bash
# No optimization
gcc -O0 -g -o benchmark_O0 benchmark.c

# With optimization
gcc -O2 -g -o benchmark_O2 benchmark.c

# Run benchmarks
./benchmark_O0 double > results_O0.csv
./benchmark_O2 double > results_O2.csv
```

### Data Files Generated

- `results_all_types_O0.csv`: All types at -O0
- `results_all_types_O2.csv`: All types at -O2
- `analysis_output.txt`: Automated analysis with bandwidth calculations

---

## Conclusions

### Key Findings

1. **Manual unrolling effectiveness depends on data type:**
   - Integer types: **3-4x speedup** at -O0, still **2-3x** at -O2
   - Floating-point: **1.05-1.6x** at -O2, compiler handles most optimization

2. **Compiler optimization is essential:**
   - `-O2` provides **2.3-12x speedup** vs `-O0`
   - Short integers benefit most (12x compiler speedup)

3. **Bandwidth limits are real:**
   - Integer summation achieves **85% bandwidth efficiency**
   - Further optimization won't help beyond memory limits

4. **Optimal unrolling factor is U=8-16:**
   - Below U=8: Not exploiting full ILP
   - Above U=16: Risk of register pressure, no bandwidth gains

5. **Manual unrolling still has value:**
   - Even with `-O2`, manual U=8-16 can provide **1.6-2.3x** for int/double
   - Portable across compilers/architectures
   - Educational: understanding low-level performance

### Performance Improvement Checklist

- [x] Enable compiler optimizations (`-O2`): **2-12x speedup**
- [x] Manual unrolling U=4-16: **1.05-3.4x additional**
- [ ] SIMD intrinsics (AVX2/AVX-512): potential 4-8x more
- [ ] Cache blocking for large arrays
- [ ] Parallel summation (OpenMP/threads)

### Final Recommendation

**For production code:**
```c
// Compile with: gcc -O2 -march=native -o fast program.c

#define UNROLL 8
for (int i = 0; i < N; i += UNROLL) {
    sum += a[i] + a[i+1] + a[i+2] + a[i+3]
         + a[i+4] + a[i+5] + a[i+6] + a[i+7];
}
```

This combination of compiler optimization + moderate manual unrolling (U=8) provides the best balance of:
- **Performance**: Within 2-3x of bandwidth limits
- **Portability**: Works across different CPUs
- **Maintainability**: Simple, readable code

---

## References

- **Bandwidth calculation**: `T_min = (N × sizeof(type)) / BW`
- **ILP**: Instruction-Level Parallelism in modern x86-64 CPUs (4-8 wide)
- **Register count**: x86-64 has 16 GP registers, 16 SIMD registers (XMM/YMM/ZMM)
- **Compiler flags**: `-O2` enables auto-vectorization, loop unrolling, SLP vectorization

**See also:**
- `benchmark_all_types.c`: Full implementation with U=1,2,4,8,16,32
- `analyze_performance.py`: Automated analysis script
- `results_all_types_O0.csv` / `results_all_types_O2.csv`: Raw data
- `analysis_output.txt`: Detailed bandwidth and efficiency calculations
