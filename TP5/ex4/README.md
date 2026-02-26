# Exercise 4: Matrix-Vector Product with MPI

## Overview
This exercise implements parallel matrix-vector multiplication using MPI (Message Passing Interface).

**Key Features:**
- ✅ Handles matrices of any size N
- ✅ Supports non-divisible N (uneven distribution across processes)
- ✅ Includes correctness verification
- ✅ Performance benchmarking with speedup/efficiency analysis

## Files

- `baseline.c` - Serial implementation (reference)
- `matvec_mpi.c` - MPI parallel implementation
- `run_benchmark.sh` - Automated benchmark script
- `visualize.py` - Python script to generate plots
- `Makefile` - Build automation

## How to Run in WSL

### 1. Compile the Programs

```bash
# Compile both serial and MPI versions
make

# Or compile individually:
gcc -O2 -o baseline baseline.c -lm
mpicc -O2 -o matvec_mpi matvec_mpi.c -lm
```

### 2. Run a Single Test

**Serial (baseline):**
```bash
./baseline 1000
```

**MPI version:**
```bash
# Run with 4 processes, matrix size 1000
mpirun -np 4 ./matvec_mpi 1000

# Different process counts
mpirun -np 2 ./matvec_mpi 2000
mpirun -np 8 ./matvec_mpi 4000
```

### 3. Run Full Benchmark

```bash
# Make script executable
chmod +x run_benchmark.sh

# Run all benchmarks (this may take a few minutes)
./run_benchmark.sh
```

This will:
- Test matrix sizes: 500, 1000, 2000, 4000, 8000
- Test process counts: 1, 2, 4, 8, 16
- Save results to `results.csv`

### 4. Generate Plots

```bash
# Install required Python packages (if not already installed)
pip3 install pandas matplotlib numpy

# Generate speedup and efficiency plots
python3 visualize.py
```

This creates:
- `speedup.png` - Speedup vs number of processes
- `efficiency.png` - Efficiency vs number of processes
- `execution_time.png` - Execution time comparison
- `metrics.csv` - Detailed performance metrics

## Understanding the Results

### Speedup
Speedup = T_serial / T_parallel

- **Ideal speedup**: Linear (2x speedup with 2 processes, 4x with 4, etc.)
- **Reality**: Usually sub-linear due to communication overhead

### Efficiency
Efficiency = Speedup / P (where P = number of processes)

- **100% efficiency**: Perfect parallelization
- **<100%**: Communication overhead, load imbalance, or Amdahl's law effects

## Example Output

```
N=1000 P=1 time=0.001234 s max_err=0.000000e+00
N=1000 P=2 time=0.000678 s max_err=1.234567e-15
N=1000 P=4 time=0.000412 s max_err=2.345678e-15
```

## Implementation Details

### Distribution Strategy
- Rows are distributed across processes using **Scatterv** (handles uneven distribution)
- Each process computes its local rows: `x_local[i] = A_local[i] * b`
- Vector `b` is broadcast to all processes (all need it)
- Results are gathered using **Gatherv**

### Handling N % P ≠ 0
When N is not divisible by P:
- First `(N % P)` processes get `⌈N/P⌉` rows
- Remaining processes get `⌊N/P⌋` rows

Example: N=10, P=3
- Process 0: rows 0-3 (4 rows)
- Process 1: rows 4-7 (4 rows)
- Process 2: rows 8-9 (2 rows)

## Quick Reference Commands

```bash
# Clean and rebuild
make clean
make

# Quick test
mpirun -np 4 ./matvec_mpi 1000

# Full benchmark + visualization
./run_benchmark.sh && python3 visualize.py

# View results
cat results.csv
cat metrics.csv
```

## Troubleshooting

**Error: "mpicc not found"**
```bash
sudo apt-get update
sudo apt-get install mpich
```

**Error: "Permission denied"**
```bash
chmod +x run_benchmark.sh
chmod +x visualize.py
```

**Python dependencies missing:**
```bash
pip3 install pandas matplotlib numpy
```

## Expected Performance

Larger matrices typically show better speedup due to:
- Higher computation-to-communication ratio
- Better cache utilization
- Amortized initialization costs

Small matrices may show poor speedup or even slowdown due to:
- Communication overhead dominates
- Initialization costs
- Less work per process
