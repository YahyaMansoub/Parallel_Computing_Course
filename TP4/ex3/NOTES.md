# Ex3 Quick Notes

## Task
Load balancing with sections (light, moderate, heavy tasks)

## Key Problem
Naive sections: 1:5:20 workload ratio → massive imbalance!

## Solution
1. **Merge**: Combine light + moderate
2. **Split**: Divide heavy into 3 chunks
3. Result: 6:7:7:7 balanced workload

## Commands
```bash
# Compile
gcc sections.c -O2 -fopenmp -lm -o sections

# Run naive (3 threads)
OMP_NUM_THREADS=3 ./sections

# Run optimized (4 threads)
OMP_NUM_THREADS=4 ./sections

# Benchmark
time OMP_NUM_THREADS=3 ./sections
time OMP_NUM_THREADS=4 ./sections
```

## Load Balance Formula
Efficiency = T_avg / T_max (aim for >90%)

## Better Alternative
Use `#pragma omp task` with work stealing!
