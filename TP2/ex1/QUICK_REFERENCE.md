# Exercise 1: Loop Optimization - Quick Reference

## 📊 Results at a Glance

### Best Performance Summary

| Type   | -O0 Best | -O2 Best | Total Speedup | BW Efficiency |
|--------|----------|----------|---------------|---------------|
| double | 2.637 ms | 0.732 ms | **3.7x**      | 36.5%         |
| float  | 2.625 ms | 0.752 ms | **3.4x**      | 17.7%         |
| int    | 0.631 ms | 0.156 ms | **13.6x**     | **85.3%** ⭐  |
| short  | 0.597 ms | 0.151 ms | **13.5x**     | 44.1%         |

⭐ **int** achieves near-bandwidth-limited performance!

---

## 🎯 Key Findings (TL;DR)

1. **Compiler optimization**: 2-12x speedup (always use `-O2`)
2. **Manual unrolling**: Additional 1.05-2.26x (type-dependent)
3. **Optimal U=8-16**: Best balance of ILP and register usage
4. **Integer ops**: Most sensitive to loop overhead (3.4x gain)
5. **Float ops**: Compiler auto-vectorization handles it (1.05x gain)

---

## 🚀 Optimal Unroll Factors

| Type   | -O0 | -O2 | Reasoning                        |
|--------|-----|-----|----------------------------------|
| double | U=4 | U=8 | FP overhead benefits from unroll |
| float  | U=4 | U=1 | Compiler vectorizes well         |
| int    | U=16| U=16| Fast ops need high U             |
| short  | U=16| U=16| Similar to int                   |

---

## 💡 When Does Manual Unrolling Help?

### ✅ **STILL BENEFICIAL** with -O2
- **double**: 1.60x additional speedup
- **int**: 2.26x additional speedup

### ❌ **NOT WORTH IT** with -O2  
- **float**: Only 1.05x (compiler already optimized)
- **short**: Only 1.10x (12x compiler speedup dominates)

---

## 📈 Performance Improvement Pattern

```
U=1  → U=2:   Large gain (loop overhead reduction)
U=2  → U=4:   Good gain (ILP starts helping)
U=4  → U=8:   Moderate gain (ILP exploitation)
U=8  → U=16:  Small gain (approaching bandwidth limit)
U=16 → U=32:  Minimal/negative (register pressure)
```

**Stop at U=16!** Beyond that, diminishing returns and risk of slowdown.

---

## 🧮 Bandwidth Limits

### Formula
```
T_min = (N × sizeof(type)) / Bandwidth
```

### Example (30 GB/s bandwidth, N=1M)
```
double:  1M × 8 bytes / 30 GB/s = 0.267 ms
int:     1M × 4 bytes / 30 GB/s = 0.133 ms
```

### Measured vs Theoretical
```
Type      T_min     Measured    Gap
----------------------------------------
double    0.267 ms  0.732 ms    2.7x slower
int       0.133 ms  0.156 ms    1.2x slower ✓
```

**int is closest to theoretical limit!**

---

## 🔧 Compiler Command Reference

### Compile Benchmarks
```bash
# No optimization
gcc -O0 -g -o bench_O0 program.c

# With optimization
gcc -O2 -g -o bench_O2 program.c

# Maximum optimization + native CPU
gcc -O3 -march=native -o bench_fast program.c
```

### Run Benchmarks
```bash
# Single type
./bench_all_types_O0 double > results_O0.csv

# All types
for type in double float int short; do
    ./bench_all_types_O0 $type >> results_O0.csv
done
```

### Analyze
```bash
python3 analyze_performance.py results_O0.csv results_O2.csv
```

---

## 📚 File Guide

### Must-Read
- **`answers.md`** (449 lines): Complete analysis with all explanations
- **`README.md`** (343 lines): Overview and practical guide

### Code
- **`benchmark_all_types.c`**: Main benchmark (supports all types)
- **`BAZOKA_U.c`**: Original double-only version (all U=1-32)

### Data
- **`results_all_types_O0.csv`**: Raw benchmarks at -O0
- **`results_all_types_O2.csv`**: Raw benchmarks at -O2

### Analysis
- **`analyze_performance.py`**: Automated bandwidth analysis
- **`comprehensive_analysis.ipynb`**: Jupyter notebook with plots

---

## 🎓 Concepts Explained

### Loop Overhead
```c
// U=1: N iterations of overhead
for (int i = 0; i < N; i++)
    sum += a[i];  // N × (inc + cmp + branch)

// U=4: N/4 iterations of overhead
for (int i = 0; i < N; i += 4)
    sum += a[i] + ... + a[i+3];  // (N/4) × (inc + cmp + branch)
```

### Instruction-Level Parallelism (ILP)
```assembly
; No ILP (sequential)
add rax, [mem]     ; must wait for result
add rax, [mem+8]   ; depends on previous

; With ILP (parallel)
add rax, [mem]     ; independent
add rbx, [mem+8]   ; can execute in parallel
add rcx, [mem+16]  ; can execute in parallel
add rax, rbx       ; final combine
```

### Register Pressure
```
Registers Available:  16 (x86-64)
U=8  → Need ~8 registers:  ✓ OK
U=16 → Need ~16 registers: ✓ Tight
U=32 → Need ~32 registers: ✗ SPILL TO MEMORY
```

---

## 🏆 Best Practices

### 1. Always Start with Compiler Optimization
```bash
gcc -O2 program.c  # or -O3
```
**Gain**: 2-12x speedup (free!)

### 2. Profile Before Optimizing
```bash
gcc -O2 -g -o program program.c
perf record ./program
perf report
```

### 3. Try Manual Unrolling (U=8)
```c
for (int i = 0; i < N; i += 8) {
    sum += a[i] + a[i+1] + ... + a[i+7];
}
// Handle remainder
for (; i < N; i++) sum += a[i];
```

### 4. Measure Actual Speedup
```bash
time ./program_U1
time ./program_U8
```

### 5. Don't Over-Optimize
- Stop at U=16
- If < 5% improvement, not worth complexity
- If > 80% BW efficiency, you're done

---

## ⚠️ Common Pitfalls

### ❌ **DON'T**: Unroll without compiler optimization
```bash
gcc -O0 program.c  # Missing 2-12x gain!
```

### ❌ **DON'T**: Exceed U=16
```c
// U=32 or higher → register spills → slower!
```

### ❌ **DON'T**: Ignore bandwidth limits
```
T_measured << T_bandwidth_limit → More optimization won't help
```

### ✅ **DO**: Combine techniques
```bash
gcc -O2 -march=native program.c  # Compiler + CPU-specific
```

---

## 📊 Real-World Example

### Problem
Sum 1 million doubles in production code.

### Approach
```c
// 1. Start with simple code + compiler optimization
gcc -O2 -march=native sum.c

// 2. Measure: 1.175 ms (baseline)

// 3. Add manual unrolling U=8
for (int i = 0; i < N; i += 8)
    sum += a[i] + ... + a[i+7];

// 4. Measure: 0.732 ms (1.6x faster)

// 5. Check bandwidth limit: 0.267 ms theoretical
//    → Still 2.7x away, but good enough for production
```

### Result
- **Total speedup**: 3.7x vs naive -O0 code
- **Development time**: 30 minutes
- **Code complexity**: Minimal (11 extra lines)
- **Maintainability**: Good (clear pattern)

---

## 🔬 Experimental Results

### Compiler Effectiveness (-O0 → -O2 at U=1)

```
short:  12.25x ⭐⭐⭐⭐⭐ (exceptional)
int:     6.02x ⭐⭐⭐⭐
float:   3.37x ⭐⭐⭐
double:  2.33x ⭐⭐
```

### Manual Unrolling Effectiveness (U=1 → best at -O2)

```
int:    2.26x ⭐⭐⭐
double: 1.60x ⭐⭐
short:  1.10x ⭐
float:  1.05x
```

---

## 🎯 Bottom Line

### For `int`/`short` types:
1. Use `-O2` (6-12x speedup)
2. Add manual U=16 (2-3x more)
3. **Total**: 13-14x improvement ✓

### For `double` type:
1. Use `-O2` (2.3x speedup)
2. Add manual U=8 (1.6x more)
3. **Total**: 3.7x improvement ✓

### For `float` type:
1. Use `-O2` (3.4x speedup)
2. Manual unrolling not needed (1.05x gain)
3. **Total**: 3.4x improvement ✓

---

## 🚀 Next Steps

1. **Run your benchmarks**: See actual numbers on your hardware
2. **Plot results**: Use `comprehensive_analysis.ipynb`
3. **Measure bandwidth**: Install STREAM benchmark
4. **Try SIMD**: Explore AVX2/AVX-512 intrinsics
5. **Parallelize**: Use OpenMP for multi-core speedup

---

## 📖 Further Optimization

Beyond loop unrolling:
- **Cache blocking**: Improve data locality
- **SIMD intrinsics**: AVX2 can process 4 doubles/cycle
- **Multithreading**: OpenMP for multi-core parallelism
- **GPU offload**: CUDA/OpenCL for massive parallelism

**Potential total speedup**: 100-1000x possible with all techniques!

---

**Last Updated**: February 2026  
**Exercise**: TP2 Exercise 1 - Loop Optimizations  
**Status**: ✅ Complete (all requirements met)
