# Ex4 Quick Notes

## Task  
Dense matrix-vector multiply with barrier optimization

## Key Concepts
- **Implicit barrier**: Automatic at end of `for`, `single`, `sections`
- **nowait**: Removes implicit barrier → better performance
- **Explicit barrier**: `#pragma omp barrier` when needed

## Critical Rule
```c
#pragma omp for nowait
for (...) A[i] = ...;

#pragma omp barrier  // ← ADD if using A below!
```

## Commands
```bash
# Compile
gcc dmvm_barrier_nowait.c -O2 -fopenmp -o ex4

# Run benchmark
./ex4 > results.csv

# Check for races
gcc dmvm_barrier_nowait.c -O2 -fopenmp -fsanitize=thread -o debug
./debug
```

## When to Use Nowait
✅ Results not needed immediately
✅ Independent operations
✅ Pipeline patterns

❌ Data dependencies exist
❌ Results needed for reduction

## Performance Gain
Typically 1-5% from removing barrier overhead
