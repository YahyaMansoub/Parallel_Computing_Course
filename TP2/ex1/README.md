# TP2 - Exercise 1: Loop Optimization Analysis

## Overview

This exercise demonstrates the performance impact of **manual loop unrolling** across different data types and compiler optimization levels. It explores fundamental concepts in performance optimization:

- **Loop overhead reduction**
- **Instruction-Level Parallelism (ILP)**
- **Memory bandwidth limitations**
- **Compiler optimization effectiveness**

---

## Files Structure

### Source Code
- **`initial_code.c`**: Original baseline implementation (U=1, no unrolling)
- **`unrol_man.c`**: Example manual unrolling (U=4)
- **`BAZOKA_U.c`**: Complete benchmark with U=1-32 for `double` type
- **`benchmark_all_types.c`**: Extended benchmark supporting `double`, `float`, `int`, `short` types

### Results Data
- **`results_O0.csv`**: Original double-precision results at -O0 (all U=1-32)
- **`results_O2.csv`**: Original double-precision results at -O2
- **`results_all_types_O0.csv`**: All types at -O0 (U=1,2,4,8,16,32)
- **`results_all_types_O2.csv`**: All types at -O2
- **`performance_summary.csv`**: Exported summary statistics

### Analysis & Documentation
- **`answers.md`**: **★ Main documentation** with complete analysis (500+ lines)
- **`analyze_performance.py`**: Automated analysis script with bandwidth calculations
- **`analysis_output.txt`**: Console output from analysis script
- **`Analysis.ipynb`**: Original visualization notebook
- **`comprehensive_analysis.ipynb`**: **★ Enhanced notebook** with 7 comprehensive visualizations

### Compiled Binaries
- `bench_O0`, `bench_O2`: Original benchmarks
- `bench_all_types_O0`, `bench_all_types_O2`: Multi-type benchmarks
- `sum_O0_Unroll4`, `sum_O2_Unroll4`: Example unrolled versions

---

## Quick Start

### 1. Run Benchmarks

```bash
# Compile for all types
gcc -O0 -g -o bench_all_types_O0 benchmark_all_types.c
gcc -O2 -g -o bench_all_types_O2 benchmark_all_types.c

# Run benchmarks
./bench_all_types_O0 double > results_all_types_O0.csv
./bench_all_types_O0 float >> results_all_types_O0.csv
./bench_all_types_O0 int >> results_all_types_O0.csv
./bench_all_types_O0 short >> results_all_types_O0.csv

./bench_all_types_O2 double > results_all_types_O2.csv
./bench_all_types_O2 float >> results_all_types_O2.csv
./bench_all_types_O2 int >> results_all_types_O2.csv
./bench_all_types_O2 short >> results_all_types_O2.csv
```

### 2. Analyze Results

```bash
# Run automated analysis
python3 analyze_performance.py results_all_types_O0.csv results_all_types_O2.csv

# Or open Jupyter notebook for visualizations
jupyter notebook comprehensive_analysis.ipynb
```

### 3. View Documentation

```bash
# Read complete analysis
cat answers.md

# Or open in markdown viewer
code answers.md
```

---

## Key Results Summary

### Performance at -O0 (No Optimization)

| Type   | U=1 Time | Best U | Best Time | Speedup | BW Efficiency |
|--------|----------|--------|-----------|---------|---------------|
| double | 2.737 ms | 4      | 2.637 ms  | 1.04x   | 10.1%         |
| float  | 2.669 ms | 4      | 2.625 ms  | 1.02x   | 5.1%          |
| int    | 2.123 ms | 16     | 0.631 ms  | **3.36x** | 21.1%       |
| short  | 2.039 ms | 16     | 0.597 ms  | **3.42x** | 11.2%       |

**Observation**: Integer types benefit massively from unrolling due to fast addition making loop overhead dominant.

### Performance at -O2 (Compiler Optimization)

| Type   | U=1 Time | Best U | Best Time | Manual Speedup | Compiler Speedup | Total Speedup | BW Efficiency |
|--------|----------|--------|-----------|----------------|------------------|---------------|---------------|
| double | 1.175 ms | 8      | 0.732 ms  | 1.60x          | 2.33x            | 3.73x         | 36.5%         |
| float  | 0.791 ms | 16     | 0.752 ms  | 1.05x          | 3.37x            | 3.37x         | 17.7%         |
| int    | 0.353 ms | 16     | 0.156 ms  | **2.26x**      | **6.02x**        | **13.6x**     | **85.3%**     |
| short  | 0.166 ms | 16     | 0.151 ms  | 1.10x          | **12.25x**       | 13.5x         | 44.1%         |

**Key Finding**: Integer summation achieves **85% bandwidth efficiency** - very close to theoretical memory limit!

---

## Important Insights

### 1. Loop Overhead Matters for Fast Operations

Integer addition is extremely fast (1 cycle), so loop overhead (increment, compare, branch) dominates:
- U=1: Loop overhead = 100% of iteration cost
- U=16: Loop overhead = 6.25% of iteration cost
- **Result**: 3.4x speedup for int/short at -O0

### 2. Compiler Optimization is Crucial

Compiler `-O2` provides massive speedups:
- `double`: 2.33x
- `float`: 3.37x
- `int`: 6.02x
- `short`: **12.25x** (exceptional auto-vectorization)

### 3. Manual Unrolling Still Helps (Sometimes)

Even with `-O2`, manual unrolling benefits:
- **double**: ✓ 1.60x additional speedup
- **float**: ✗ Only 1.05x (compiler handles it)
- **int**: ✓ 2.26x additional speedup
- **short**: ✓ 1.10x additional speedup

### 4. Memory Bandwidth is the Ultimate Limit

Theoretical minimum time (30 GB/s bandwidth):
- `double`: 0.267 ms → achieved 0.732 ms (36.5% efficient)
- `int`: 0.133 ms → achieved 0.156 ms (**85.3% efficient!**)

No amount of optimization can go faster than memory can deliver data.

### 5. Optimal Unrolling Factor

**General recommendation: U=8-16**

| Unroll Factor | Effect |
|---------------|--------|
| U=1-2         | High loop overhead |
| U=4-8         | Good ILP exploitation |
| U=8-16        | Near-optimal, approaching bandwidth limit |
| U=32+         | Diminishing returns, register pressure |

---

## Why Performance Saturates

### Phase 1: Loop Overhead Reduction (U=1→4)
- Fewer iterations = less overhead
- Most significant for integer types

### Phase 2: ILP Exploitation (U=4→8)
- CPU can execute multiple independent operations
- Typical x86-64: 4-8 operations per cycle

### Phase 3: Bandwidth Saturation (U=8→16)
- Memory fetches become bottleneck
- Formula: T_min = (N × sizeof(type)) / Bandwidth

### Phase 4: Register Pressure (U=32+)
- x86-64 has only 16 general-purpose registers
- High unrolling causes register spills to memory
- **Performance can actually degrade**

---

## Bandwidth Analysis

### Theoretical Limits (30 GB/s memory bandwidth)

```
Type      Data Size    T_min (ms)    Best Measured    Efficiency
----------------------------------------------------------------------
double    7.63 MB      0.267 ms      0.732 ms         36.5%
float     3.81 MB      0.133 ms      0.752 ms         17.7%
int       3.81 MB      0.133 ms      0.156 ms         85.3% ★
short     1.91 MB      0.067 ms      0.151 ms         44.1%
```

**Why int achieves highest efficiency:**
- Simple integer addition optimizes perfectly
- Minimal instruction overhead
- Compiler can generate optimal assembly

---

## Compiler Optimization Comparison

### Does Manual Unrolling Help with -O2?

```
Type      Manual @ -O0    Manual @ -O2    Still Beneficial?
----------------------------------------------------------------
double    1.04x           1.60x           ✓ YES (significant)
float     1.02x           1.05x           ✗ NO (compiler wins)
int       3.36x           2.26x           ✓ YES (significant)
short     3.42x           1.10x           ✓ YES (minor)
```

**Recommendation:**
- Always use `-O2` or `-O3` first
- Profile your specific workload
- Try manual unrolling U=8 if compute-bound
- Don't exceed U=16 (diminishing returns)

---

## Visualizations Generated

The `comprehensive_analysis.ipynb` notebook generates:

1. **Performance vs Unroll Factor** (all types, -O0 vs -O2)
2. **Speedup Analysis** (manual unrolling effectiveness)
3. **Compiler Optimization Effect** (how much -O2 helps)
4. **Bandwidth Efficiency** (theoretical vs measured)
5. **Time Comparison** (BW-limited vs measured)
6. **Speedup Curves** (normalized to U=1)
7. **Summary Statistics** (comprehensive tables)

All plots exported as high-resolution PNGs (300 DPI).

---

## How to Extend This Analysis

### Test Different Problem Sizes

```c
// Modify N in benchmark_all_types.c
#define N 10000000  // 10M elements instead of 1M
```

Larger N → loop overhead becomes less significant → lower speedups from unrolling.

### Measure Your Actual Bandwidth

```bash
# Install bandwidth benchmark tool
git clone https://github.com/jeffhammond/STREAM
cd STREAM
gcc -O2 -fopenmp stream.c -o stream
./stream
```

Update `MEMORY_BANDWIDTH_GB_S` in `analyze_performance.py` with your measured value.

### Try Different Accumulator Patterns

```c
// Multiple accumulators for better ILP
double sum0=0, sum1=0, sum2=0, sum3=0;
for (int i = 0; i < N; i += 4) {
    sum0 += a[i];
    sum1 += a[i+1];
    sum2 += a[i+2];
    sum3 += a[i+3];
}
sum = sum0 + sum1 + sum2 + sum3;
```

This breaks data dependencies and can improve pipelining.

---

## Practical Recommendations

### For Production Code

1. **Enable compiler optimization**: `-O2` or `-O3`
2. **Use architecture-specific flags**: `-march=native`
3. **Profile before optimizing**: Use `perf`, `valgrind`, or `vtune`
4. **Try manual unrolling U=8** if profiling shows compute-bound loop
5. **Don't exceed U=16** (register pressure)
6. **Consider SIMD intrinsics** if manual unrolling insufficient
7. **Measure actual speedup** on your target hardware

### When to Stop Optimizing

✓ Stop when:
- Achieved >80% bandwidth efficiency
- Further unrolling provides <5% improvement
- Code becomes hard to maintain

✗ Don't stop if:
- Still <50% bandwidth efficiency
- Significant time spent in the loop (profiler)
- Haven't tried compiler optimization yet

---

## Problem Statement Reference

**Exercise Requirements:**
1. ✅ Implement unrolling U=1,2,4,...,32
2. ✅ Measure execution times
3. ✅ Identify best unroll factor
4. ✅ Compare -O0 vs -O2
5. ✅ Test manual unrolling benefit with -O2
6. ✅ Repeat for float, int, short
7. ✅ Estimate bandwidth-limited T_min
8. ✅ Explain why performance improves then saturates

**All requirements fully addressed in `answers.md` and analysis scripts.**

---

## Conclusion

This exercise demonstrates that **performance optimization requires understanding**:

1. **Hardware limits** (bandwidth, ILP, registers)
2. **Compiler capabilities** (auto-vectorization, unrolling)
3. **Workload characteristics** (compute vs memory-bound)
4. **Measurement methodology** (profiling, benchmarking)

**Key Takeaway**: Compiler optimization provides 2-12x speedup. Manual techniques can add another 1.1-2.3x, but only if you understand what's limiting performance.

---

## Further Reading

- [Agner Fog's Optimization Manuals](https://www.agner.org/optimize/)
- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/)
- [What Every Programmer Should Know About Memory](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf)
- [STREAM Benchmark](https://www.cs.virginia.edu/stream/)

---

**Author**: Exercise 1 Analysis  
**Course**: TP2 - Foundations of Parallel Computing  
**Date**: 2026
