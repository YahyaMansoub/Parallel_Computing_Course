# WSL Quick Reference - Exercise 4

## 🚀 Quick Start (Copy & Paste)

```bash
# Navigate to ex4
cd ~/your-repo-path/TP5/ex4

# Complete workflow (one command)
make full
```

## 📋 Essential Commands

### Compilation
```bash
make                    # Compile both programs
make clean              # Remove executables
```

### Testing
```bash
# Test single run
mpirun -np 4 ./matvec_mpi 1000

# Quick test
make test
```

### Full Benchmark
```bash
# Run all benchmarks
make benchmark

# Or manually:
./run_benchmark.sh
```

### Visualization
```bash
# Generate plots
make visualize

# Or manually:
python3 visualize.py
```

## 🎯 What to Run for Exercise 4

### Minimum Requirements:
```bash
# 1. Compile
make

# 2. Run benchmark with multiple sizes and process counts
./run_benchmark.sh

# 3. Generate speedup/efficiency plots
python3 visualize.py
```

### Full Solution:
```bash
# One command does everything:
make full
```

This will:
- ✅ Compile baseline and MPI versions
- ✅ Run benchmarks (N = 500, 1000, 2000, 4000, 8000)
- ✅ Test with P = 1, 2, 4, 8, 16 processes
- ✅ Generate speedup/efficiency plots
- ✅ Create metrics.csv with all results

## 📊 Check Your Results

```bash
# View raw results
cat results.csv

# View computed metrics
cat metrics.csv

# View plots (if in WSL with GUI or WSLg)
xdg-open speedup.png
xdg-open efficiency.png
```

## ⚠️ Troubleshooting

```bash
# If MPI not installed:
sudo apt update
sudo apt install mpich

# If Python packages missing:
pip3 install pandas matplotlib numpy

# If permission denied:
chmod +x run_benchmark.sh
chmod +x visualize.py
```

## 📝 Expected Output Example

```
Running serial baseline (N=1000)...
N=1000 serial_time=0.001234 s

Running MPI version with 4 processes (N=1000)...
N=1000 P=4 time=0.000412 s max_err=2.345678e-15
```

Speedup = 0.001234 / 0.000412 ≈ 3.0x
Efficiency = 3.0 / 4 = 75%

## 📁 Files You Should Submit

After running everything:
- ✅ `matvec_mpi.c` (your implementation - already complete)
- ✅ `results.csv` (benchmark data)
- ✅ `speedup.png` (speedup plot)
- ✅ `efficiency.png` (efficiency plot)
- ✅ `solution.md` (analysis and explanation)
