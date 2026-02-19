# Exercise 1: Parallel Sections - Statistical Computations

## Objective
Convert a serial program computing statistical metrics (sum, max, stddev) to parallel using OpenMP `sections`.

## Problem Analysis
- **Input**: Array A of N=1,000,000 random doubles
- **Compute**: Sum, Maximum, Standard Deviation
- **Dependencies**: stddev depends on mean (which depends on sum)

## Serial Code Analysis
```c
// Three sequential loops:
1. Compute sum and max     → O(N)
2. Compute mean from sum   → O(1)
3. Compute stddev from mean → O(N)
```

## Parallel Strategy
Use `#pragma omp sections` to divide independent work:
- **Section 1**: Compute sum and mean (sequential dependency)
- **Section 2**: Compute max (independent)
- **Section 3**: Compute stddev (waits for mean via flag)

## Key Synchronization
```c
#pragma omp atomic write
mean_ready = 1;
#pragma omp flush(mean_ready, mean)
```
- **Atomic write**: Ensures thread-safe flag update
- **Flush**: Forces memory visibility across threads
- **Busy wait**: Section 3 spins until mean is ready

## Performance Characteristics
- **Max threads used**: 3 (one per section)
- **Load balance**: Section 1 and 3 do O(N) work; Section 2 also O(N)
- **Overhead**: Busy waiting in Section 3, synchronization costs

## Compilation & Execution

### Serial Version
```bash
gcc -O0 initial_code.c -o serial -lm
gcc -O2 initial_code.c -o serial_O2 -lm
./serial
```

### Parallel Version
```bash
gcc -O0 code_with_sections.c -fopenmp -o parallel -lm
gcc -O2 code_with_sections.c -fopenmp -o parallel_O2 -lm

# Run with different thread counts (max useful = 3)
export OMP_NUM_THREADS=3
./parallel

# Benchmark comparison
time ./serial
time ./parallel
```

## Expected Output
```
Sum     = 499870.123456
Max     = 0.999999
Std Dev = 0.288675
```

## Analysis Questions
1. **Why only 3 threads useful?** → Only 3 sections defined
2. **Is speedup 3x?** → No, due to:
   - Busy waiting overhead
   - Imbalanced workload
   - Thread creation cost
3. **Better approach?** → Use `#pragma omp parallel for reduction`

## Performance Metrics to Collect
- Execution time (serial vs parallel)
- Speedup = T_serial / T_parallel
- Efficiency = Speedup / num_threads
- Overhead from synchronization

## Improvements
```c
// Better: Use reduction for sum/max, then stddev in second parallel region
#pragma omp parallel for reduction(+:sum) reduction(max:max)
for (int i = 0; i < N; i++) { ... }

mean = sum / N;

#pragma omp parallel for reduction(+:var)
for (int i = 0; i < N; i++) {
    double d = A[i] - mean;
    var += d * d;
}
```

## Common Pitfalls
- ❌ Forgetting `#pragma omp flush`
- ❌ Data race on `mean` without synchronization
- ❌ Using more than 3 threads (wastes resources)
- ❌ Not measuring overhead of busy waiting
