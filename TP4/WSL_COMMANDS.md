# TP4 - Quick Reference: WSL Commands

## Exercise 1: Sections (Statistical Computations)

### Compile
```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1

# Serial version
gcc initial_code.c -O0 -o serial -lm
gcc initial_code.c -O2 -o serial_O2 -lm

# Parallel version
gcc code_with_sections.c -O0 -fopenmp -o parallel -lm
gcc code_with_sections.c -O2 -fopenmp -o parallel_O2 -lm
```

### Run
```bash
# Serial
./serial
time ./serial

# Parallel (3 threads max useful)
export OMP_NUM_THREADS=3
./parallel
time ./parallel

# Try other thread counts
for t in 1 2 3 4 8; do
  echo "Threads: $t"
  OMP_NUM_THREADS=$t ./parallel
done
```

---

## Exercise 2: Master and Single

### Compile
```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex2

# Serial
gcc init_code.c -O0 -o serial
gcc init_code.c -O2 -o serial_O2

# Parallel
gcc master_single.c -O0 -fopenmp -o parallel
gcc master_single.c -O2 -fopenmp -o parallel_O2

# Debug (thread sanitizer)
gcc master_single.c -fopenmp -fsanitize=thread -g -o parallel_debug
```

### Run
```bash
# Serial
./serial

# Parallel with different thread counts
for t in 1 2 4 8; do
  echo "=== Threads: $t ==="
  OMP_NUM_THREADS=$t ./parallel
done

# Check for data races
./parallel_debug

# Benchmark
time ./serial
time OMP_NUM_THREADS=4 ./parallel
```

---

## Exercise 3: Load Balancing

### Compile
```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex3

# Compile with optimization and math library
gcc sections.c -O0 -fopenmp -lm -o sections
gcc sections.c -O2 -fopenmp -lm -o sections_O2
```

### Run
```bash
# Naive version (3 threads)
OMP_NUM_THREADS=3 ./sections

# Optimized version (4 threads)
OMP_NUM_THREADS=4 ./sections

# Compare performance
echo "=== Naive (3 threads) ==="
time OMP_NUM_THREADS=3 ./sections_O2

echo "=== Optimized (4 threads) ==="
time OMP_NUM_THREADS=4 ./sections_O2

# Test scalability
for t in 1 2 3 4 8; do
  echo "Threads: $t"
  OMP_NUM_THREADS=$t ./sections_O2
done
```

---

## Exercise 4: Barrier and Nowait

### Compile
```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex4

# Standard compilation
gcc dmvm_barrier_nowait.c -O0 -fopenmp -o ex4
gcc dmvm_barrier_nowait.c -O2 -fopenmp -o ex4_O2

# Debug with thread sanitizer
gcc dmvm_barrier_nowait.c -O2 -fopenmp -fsanitize=thread -g -o ex4_debug
```

### Run
```bash
# Run benchmark (tests multiple thread counts)
./ex4_O2

# Save results to CSV
./ex4_O2 > results.csv

# View results
cat results.csv
column -t -s, results.csv | less

# Check for data races
./ex4_debug

# Single thread count test
OMP_NUM_THREADS=4 ./ex4_O2
```

### Visualize Results
```bash
# Run visualization (in Jupyter or Python)
python -c "
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv', comment='#')
df.columns = df.columns.str.strip()

print(df)
print(f\"Mean barrier overhead: {((df['v1_time'] - df['v3_time']) / df['v1_time'] * 100).mean():.2f}%\")
"
```

---

## Automated Benchmarking (Generates CSV for Visualization)

### Exercise 1: Sections Benchmark

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1

# Compile benchmark
gcc benchmark.c -O2 -fopenmp -lm -o benchmark

# Generate CSV data
./benchmark > results.csv

# View results
cat results.csv
column -t -s, results.csv

# Visualize in Jupyter
jupyter notebook viz_auto.ipynb
```

**CSV Format**: `threads,time,speedup,efficiency,sum,max,stddev`
- `threads=0`: Serial baseline
- `threads>0`: Parallel results

---

### Exercise 2: Master/Single Benchmark

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex2

# Compile benchmark
gcc benchmark.c -O2 -fopenmp -o benchmark

# Generate CSV data
./benchmark > results.csv

# Quick analysis
python -c "
import pandas as pd
df = pd.read_csv('results.csv')
serial = df[df['threads']==0].iloc[0]
f_s = serial['init_time'] / serial['total_time']
print(f'Serial fraction: {f_s:.2%}')
print(f'Max speedup (Amdahl): {1/f_s:.2f}x')
"

# Visualize
jupyter notebook viz_auto.ipynb
```

**CSV Format**: `threads,total_time,init_time,sum_time,speedup,efficiency,sum_value`

---

### Exercise 3: Load Balancing Benchmark

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex3

# Compile benchmark
gcc benchmark.c -O2 -fopenmp -lm -o benchmark

# Generate CSV data
./benchmark > results.csv

# Compare versions
python -c "
import pandas as pd
df = pd.read_csv('results.csv')
naive = df[df['version']=='naive']
opt = df[df['version']=='optimized']
print('Best Naive:', naive['speedup'].max())
print('Best Optimized:', opt['speedup'].max())
"

# Visualize
jupyter notebook viz_auto.ipynb
```

**CSV Format**: `version,threads,time,speedup,efficiency,result_sum`
- `version`: serial, naive, optimized

---

### Exercise 4: Barrier Benchmark (Already Included)

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex4

# Already generates CSV directly
gcc dmvm_barrier_nowait.c -O2 -fopenmp -o ex4_O2
./ex4_O2 > dmvm_barrier_nowait.csv

# Visualize
jupyter notebook visual.ipynb
```

---

## General Commands

### Environment Setup
```bash
# Check OpenMP support
gcc --version
gcc -fopenmp -dM -E - </dev/null | grep -i openmp

# Set thread count (various methods)
export OMP_NUM_THREADS=4
OMP_NUM_THREADS=4 ./program

# Check CPU info
lscpu | grep -E "^CPU\(s\)|Core|Thread"
nproc
```

### Compilation Flags Reference
```bash
# Basic compilation
gcc program.c -fopenmp -o program

# With optimization
gcc program.c -O2 -fopenmp -o program

# With math library
gcc program.c -O2 -fopenmp -lm -o program

# Debug with sanitizer
gcc program.c -fopenmp -fsanitize=thread -g -o program_debug

# All warnings
gcc program.c -fopenmp -Wall -Wextra -o program
```

### Benchmarking
```bash
# Simple timing
time ./program

# Multiple runs with average
for i in {1..5}; do time ./program; done 2>&1 | grep real

# With different thread counts
for t in 1 2 4 8 16; do
  echo "Threads: $t"
  time OMP_NUM_THREADS=$t ./program
done

# Save timing to file
{ time ./program ; } 2> timing.txt
```

### Performance Analysis
```bash
# System monitor (in separate terminal)
htop
top -H  # Show threads

# CPU affinity
OMP_PROC_BIND=close OMP_NUM_THREADS=4 ./program
OMP_PROC_BIND=spread OMP_NUM_THREADS=4 ./program

# Detailed OpenMP info
OMP_DISPLAY_ENV=true ./program

# perf profiling (if available)
perf stat -e instructions,cycles,cache-misses ./program
```

---

## Jupyter Notebook Commands

### Launch Jupyter
```bash
# From WSL
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1
jupyter notebook visualization.ipynb

# Or from Windows terminal
cd "C:\Users\MANSOUB YAHYA\Desktop\DESK\GitHub\Parallel_Computing_Course\TP4\ex1"
jupyter notebook visualization.ipynb
```

### Run All Visualizations
```bash
# Convert notebook to Python and run
jupyter nbconvert --to python visualization.ipynb
python visualization.py

# Or run specific cells
jupyter nbconvert --execute --to notebook visualization.ipynb
```

---

## Quick Workflow

### Standard Workflow for Each Exercise
```bash
# 1. Navigate to exercise directory
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1

# 2. Compile both versions
gcc initial_code.c -O2 -o serial -lm
gcc code_with_sections.c -O2 -fopenmp -o parallel -lm

# 3. Run and time
time ./serial
time OMP_NUM_THREADS=4 ./parallel

# 4. Generate data for visualization
./parallel > results.txt

# 5. Open visualization notebook
jupyter notebook visualization.ipynb
```

---

## Common Issues & Solutions

### Issue: Permission Denied
```bash
chmod +x serial parallel
./serial
```

### Issue: OpenMP Not Found
```bash
# Install gcc with OpenMP support
sudo apt update
sudo apt install gcc libomp-dev
```

### Issue: Math Library Errors
```bash
# Add -lm flag
gcc program.c -fopenmp -lm -o program
```

### Issue: Thread Sanitizer False Positives
```bash
# Suppress known issues
export TSAN_OPTIONS="suppressions=tsan.supp"
```

---

## Quick Performance Comparison Script

Save as `benchmark_all.sh`:
```bash
#!/bin/bash

echo "TP4 Performance Benchmark"
echo "========================="

for ex in ex1 ex2 ex3 ex4; do
  echo ""
  echo "=== $ex ==="
  cd "$ex"
  
  if [ -f "serial" ] && [ -f "parallel" ]; then
    echo "Serial:"
    time ./serial 2>&1 | tail -1
    
    echo "Parallel (4 threads):"
    time OMP_NUM_THREADS=4 ./parallel 2>&1 | tail -1
  fi
  
  cd ..
done
```

Run with:
```bash
chmod +x benchmark_all.sh
./benchmark_all.sh
```
