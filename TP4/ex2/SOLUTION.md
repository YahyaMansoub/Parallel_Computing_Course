# Exercise 2: Master and Single Directives

## Objective
Learn the difference between `master` and `single` directives for exclusive execution in OpenMP.

## Problem Analysis
- **Input**: Matrix A of size N×N
- **Operations**:
  1. Initialize matrix
  2. Print matrix
  3. Compute sum of all elements

## Key Concepts

### Master Directive
```c
#pragma omp master
{
    // Only the master thread (thread 0) executes this
    // NO implicit barrier at the end
}
```
- **Thread**: Only thread ID 0
- **Barrier**: None (other threads continue immediately)
- **Use case**: I/O or updates that must be done by master

### Single Directive
```c
#pragma omp single
{
    // One thread executes (any thread, first to arrive)
    // IMPLICIT barrier at the end (unless nowait)
}
```
- **Thread**: First thread to arrive
- **Barrier**: Yes (ensures all threads see the result)
- **Use case**: Work that should be done once, needs synchronization

## Code Structure

### Serial Version (`init_code.c`)
```c
1. Initialize matrix    → O(N²)
2. Print matrix        → O(N²)
3. Compute sum         → O(N²)
```

### Parallel Version (`master_single.c`)
```c
#pragma omp parallel
{
    #pragma omp master
    { init_matrix(); }        // Only thread 0
    
    #pragma omp barrier       // EXPLICIT: wait for init
    
    #pragma omp single
    { print_matrix(); }       // Any one thread
    // Implicit barrier here
    
    #pragma omp for reduction(+:sum)
    for (...) { sum += A[i]; }  // All threads
}
```

## Synchronization Analysis

| Directive | Who Executes | Implicit Barrier | Typical Use |
|-----------|--------------|------------------|-------------|
| `master` | Thread 0 only | NO | Master-specific init |
| `single` | Any one thread | YES | One-time setup |
| `for` | All threads | YES (unless nowait) | Parallel loop |

## Why the Explicit Barrier?
```c
#pragma omp master
{ init_matrix(); }

#pragma omp barrier  // ← CRITICAL!
```
**Without barrier**: Other threads might access A before it's initialized!
- Master has NO implicit barrier
- Other threads would race ahead
- Data race → undefined behavior

## Compilation & Execution

### Serial Version
```bash
# Compile
gcc -O0 init_code.c -o serial
gcc -O2 init_code.c -o serial_O2

# Run
./serial
```

### Parallel Version
```bash
# Compile
gcc -O0 master_single.c -fopenmp -o parallel
gcc -O2 master_single.c -fopenmp -o parallel_O2

# Run with different thread counts
export OMP_NUM_THREADS=1
./parallel

export OMP_NUM_THREADS=4
./parallel

export OMP_NUM_THREADS=8
./parallel

# Benchmark
time ./serial
time OMP_NUM_THREADS=4 ./parallel
```

## Performance Characteristics

### Expected Speedup
- **Init**: No speedup (serial by master)
- **Print**: No speedup (serial by single)
- **Sum**: Linear speedup (parallel for with reduction)

### Bottlenecks
1. Serial initialization (master only)
2. Serial printing (single thread)
3. Only sum computation is parallel

### Overall Speedup
$$\text{Speedup} = \frac{1}{\frac{T_{\text{init}} + T_{\text{print}}}{T_{\text{total}}} + \frac{T_{\text{sum}}}{T_{\text{total}} \cdot p}}$$

Where $p$ = number of threads (Amdahl's Law)

## Alternative: Parallel Initialization
```c
#pragma omp parallel
{
    // Parallel initialization
    #pragma omp for collapse(2)
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i*n + j] = (double)(i + j);
    
    #pragma omp single
    { print_matrix(); }
    
    #pragma omp for reduction(+:sum)
    for (int i = 0; i < n*n; i++)
        sum += A[i];
}
```

## Common Mistakes

❌ **Forgetting barrier after master**
```c
#pragma omp master
{ init(); }
// Other threads race ahead! ← BUG
#pragma omp single
{ use_data(); }
```

✅ **Correct: Add barrier**
```c
#pragma omp master
{ init(); }
#pragma omp barrier  // ← FIXED
#pragma omp single
{ use_data(); }
```

❌ **Using master when single is better**
```c
#pragma omp master
{ read_input_file(); }  // No implicit barrier!
#pragma omp barrier     // Need explicit
```

✅ **Better: Use single**
```c
#pragma omp single
{ read_input_file(); }  // Built-in barrier
```

## Expected Output
```
Matrix:
  0.0   1.0   2.0 ...
  1.0   2.0   3.0 ...
  ...
Sum = 999000000.000000
Time (OpenMP) = 0.015 s
```

## Verification Checklist
- [ ] Matrix initialized correctly (no uninitialized values)
- [ ] Print happens once (not multiple times)
- [ ] Sum matches serial version
- [ ] No data races (use `-fsanitize=thread` to check)

## Analysis Questions
1. **Why use master for init?** → Convention (could use single)
2. **What if we remove the barrier?** → Data race!
3. **Can init be parallelized?** → Yes, use `parallel for`
4. **Expected speedup?** → Modest (only sum is parallel)

## Memory Safety
```bash
# Compile with debug flags
gcc master_single.c -fopenmp -g -fsanitize=thread -o parallel_debug

# Run to detect data races
./parallel_debug
```

## Performance Metrics
For N=1000:
- Serial time: ~0.008s
- Parallel time (4 threads): ~0.003s
- Speedup: ~2.7x (not 4x due to serial portions)
- Efficiency: ~67%
