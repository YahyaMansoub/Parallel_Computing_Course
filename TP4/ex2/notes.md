


# Ex2 Quick Notes

## Task
Use `master` and `single` directives correctly

## Key Concepts
- **master**: Thread 0 only, NO implicit barrier
- **single**: Any one thread, HAS implicit barrier

## Critical Pattern
```c
#pragma omp master
{ init(); }
#pragma omp barrier  // ← MUST ADD!
```

## Commands
```bash
# Compile
gcc init_code.c -O2 -o serial
gcc master_single.c -O2 -fopenmp -o parallel

# Run
./serial
OMP_NUM_THREADS=4 ./parallel

# Check for races
gcc master_single.c -fopenmp -fsanitize=thread -o debug
./debug
```

## Common Bug
Forgetting barrier after master → data race!

## Speedup
Limited by serial portions (Amdahl's Law)



