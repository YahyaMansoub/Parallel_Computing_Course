# TP4 - Complete Workflow Guide

## Quick Start: Automated Workflow

Each exercise now has an **automated benchmark** that generates CSV data for visualization without manual data entry.

### Standard Workflow for Each Exercise

```bash
# 1. Navigate to exercise
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1

# 2. Compile benchmark
gcc benchmark.c -O2 -fopenmp -lm -o benchmark

# 3. Generate CSV automatically
./benchmark > results.csv

# 4. Visualize results
jupyter notebook viz_auto.ipynb
```

That's it! No manual data entry needed.

---

## Detailed Workflow by Exercise

### Exercise 1: Sections (Statistical Computations)

**Goal**: Understand sections and workload distribution

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1

# Step 1: Compile benchmark
gcc benchmark.c -O2 -fopenmp -lm -o benchmark

# Step 2: Generate performance data
./benchmark > results.csv

# Step 3: Review data
cat results.csv

# Step 4: Visualize (automated CSV loading)
jupyter notebook viz_auto.ipynb
```

**What the benchmark tests**:
- Serial baseline (threads=0)
- Parallel with 1, 2, 3, 4, 8 threads
- Computes: sum, max, stddev over 1M elements
- Averages 5 runs for accuracy

**Key Files**:
- `benchmark.c` → Automated performance testing
- `viz_auto.ipynb` → Loads CSV and generates plots
- `results.csv` → Generated data (threads, time, speedup, efficiency)
- `SOLUTION.md` → Detailed explanations
- `NOTES.md` → Quick reference

---

### Exercise 2: Master and Single

**Goal**: Compare master vs single, analyze Amdahl's Law

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex2

# Compile benchmark
gcc benchmark.c -O2 -fopenmp -o benchmark

# Generate data
./benchmark > results.csv

# Quick analysis
python -c "
import pandas as pd
df = pd.read_csv('results.csv')
serial = df[df['threads']==0].iloc[0]
f_serial = serial['init_time'] / serial['total_time']
max_speedup = 1 / f_serial
print(f'Serial fraction: {f_serial:.2%}')
print(f'Amdahl max speedup: {max_speedup:.2f}x')
print(f'\\nBest actual speedup: {df[df[\"threads\"]>0][\"speedup\"].max():.2f}x')
"

# Visualize
jupyter notebook viz_auto.ipynb
```

**What the benchmark measures**:
- Init time (serial portion with `master`)
- Sum time (parallel portion)
- Total time
- Tests threads: 1, 2, 4, 8

**Key Insight**: Notice how speedup plateaus due to serial bottleneck (Amdahl's Law)

---

### Exercise 3: Load Balancing

**Goal**: Compare naive (imbalanced) vs optimized (balanced) sections

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex3

# Compile benchmark
gcc benchmark.c -O2 -fopenmp -lm -o benchmark

# Generate data (tests both versions)
./benchmark > results.csv

# Quick comparison
python -c "
import pandas as pd
df = pd.read_csv('results.csv')
naive = df[df['version']=='naive']
opt = df[df['version']=='optimized']

print('Naive:')
print(naive[['threads', 'speedup', 'efficiency']])
print(f'\\nBest speedup: {naive[\"speedup\"].max():.2f}x')

print('\\nOptimized:')
print(opt[['threads', 'speedup', 'efficiency']])
print(f'\\nBest speedup: {opt[\"speedup\"].max():.2f}x')

improvement = opt['speedup'].max() / naive['speedup'].max()
print(f'\\nImprovement: {improvement:.2f}x')
"

# Visualize
jupyter notebook viz_auto.ipynb
```

**What the benchmark compares**:
- **Naive**: 3 sections with 1:5:20 workload ratio
- **Optimized**: 4 sections with ~6:7:7:7 balanced ratio
- Tests threads: 1, 2, 3, 4, 8

**Key Takeaway**: Better load balancing → higher speedup

---

### Exercise 4: Barrier and Nowait

**Goal**: Measure barrier overhead, optimize with nowait

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex4

# Compile (program already outputs CSV)
gcc dmvm_barrier_nowait.c -O2 -fopenmp -o ex4_O2

# Generate data
./ex4_O2 > dmvm_barrier_nowait.csv

# Quick analysis
python -c "
import pandas as pd
df = pd.read_csv('dmvm_barrier_nowait.csv')
avg_overhead = ((df['v1_time'] - df['v3_time']) / df['v1_time'] * 100).mean()
print(df)
print(f'\\nAverage barrier overhead: {avg_overhead:.2f}%')
"

# Visualize
jupyter notebook visual.ipynb
```

**What the program tests**:
- V1: With barriers (synchronized)
- V2: With nowait (unsynchronized)
- V3: Optimized with selective barriers

**Key Pattern**: Use `nowait` when independence exists

---

## Complete Example Session

### Run All Exercises at Once

```bash
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4

# Compile all benchmarks
for ex in ex1 ex2 ex3; do
  echo "Compiling $ex..."
  (cd $ex && gcc benchmark.c -O2 -fopenmp -lm -o benchmark)
done

# Special case for ex4
(cd ex4 && gcc dmvm_barrier_nowait.c -O2 -fopenmp -o ex4_O2)

# Generate all CSV files
./ex1/benchmark > ./ex1/results.csv
./ex2/benchmark > ./ex2/results.csv
./ex3/benchmark > ./ex3/results.csv
./ex4/ex4_O2 > ./ex4/dmvm_barrier_nowait.csv

echo "All data generated! Open Jupyter notebooks to visualize."
```

### Launch All Visualizations

```bash
# Option 1: Open each individually
jupyter notebook ex1/viz_auto.ipynb
jupyter notebook ex2/viz_auto.ipynb
jupyter notebook ex3/viz_auto.ipynb
jupyter notebook ex4/visual.ipynb

# Option 2: Start Jupyter in TP4 root
jupyter notebook
# Then navigate to each notebook in the browser
```

---

## File Organization

### Each Exercise Contains:

```
ex1/
├── benchmark.c              # ← Automated performance testing
├── viz_auto.ipynb          # ← Visualization (loads CSV automatically)
├── results.csv             # ← Generated by benchmark
├── SOLUTION.md             # ← Comprehensive explanations
├── NOTES.md                # ← Quick reference
├── initial_code.c          # ← Original serial code
└── code_with_sections.c    # ← Parallel implementation
```

### Documentation Hierarchy:

1. **NOTES.md** (30 sec read)
   - Minimalistic quick reference
   - Key commands and patterns
   - When you just need a reminder

2. **SOLUTION.md** (5 min read)
   - Detailed explanations
   - Performance analysis
   - When you want to understand deeply

3. **README.md** (at TP4 root)
   - Overview of all exercises
   - Learning objectives
   - Project structure

4. **WSL_COMMANDS.md**
   - All compilation commands
   - Benchmarking workflows
   - Troubleshooting

---

## Data Collection Methods

### Method 1: Automated Benchmark (Recommended)
```bash
gcc benchmark.c -O2 -fopenmp -lm -o benchmark
./benchmark > results.csv
jupyter notebook viz_auto.ipynb  # Automatically loads CSV
```

**Pros**: 
- No manual data entry
- Consistent measurements
- Averages multiple runs
- Easy to reproduce

---

### Method 2: Manual Timing (Original)
```bash
gcc code.c -O2 -fopenmp -o program
time ./program  # Manually copy results into notebook
jupyter notebook visualization.ipynb  # Enter data manually
```

**Use when**: Exploring custom configurations

---

## Visualization Notebooks Comparison

| File | Data Source | Purpose |
|------|-------------|---------|
| `viz_auto.ipynb` | `results.csv` (auto-generated) | **Recommended**: No manual entry |
| `visualization.ipynb` | Manual entry | Original: For reference or custom tests |
| `visual.ipynb` (ex4) | CSV (built into program) | Ex4 already had CSV support |

---

## Performance Analysis Checklist

### For Each Exercise:

- [ ] Compile benchmark
- [ ] Generate CSV data
- [ ] Review raw data (`cat results.csv`)
- [ ] Open visualization notebook
- [ ] Run all cells
- [ ] Identify optimal thread count
- [ ] Calculate speedup and efficiency
- [ ] Note any anomalies or overhead
- [ ] Compare to theoretical predictions
- [ ] Read SOLUTION.md for deeper insights

---

## Common Questions

### Q: Which notebook should I use?

**A**: Use `viz_auto.ipynb` (ex1-ex3) or `visual.ipynb` (ex4) - they load CSV automatically.

### Q: Do I need to recompile after changing thread counts?

**A**: No! The benchmark tests multiple thread counts in one run.

### Q: How many runs does the benchmark average?

**A**: 3-5 runs (check `NUM_RUNS` in benchmark.c)

### Q: Can I customize thread counts tested?

**A**: Yes, edit the `threads[]` array in benchmark.c:
```c
int threads[] = {1, 2, 4, 8, 16};  // Add/remove as needed
```

### Q: What if results look inconsistent?

**A**: 
1. Close other applications
2. Increase `NUM_RUNS` in benchmark.c
3. Compile with `-O2` optimization
4. Check CPU frequency scaling (may be in power-saving mode)

---

## Troubleshooting

### Issue: "cannot open 'results.csv'"

**Solution**: Run benchmark first:
```bash
./benchmark > results.csv
```

### Issue: Benchmark hangs

**Solution**: Reduce array size or iterations in benchmark.c

### Issue: Speedup < 1 (slower than serial)

**Possible causes**:
- Too much overhead for small problem size
- Incorrect parallelization
- False sharing (check array alignment)

### Issue: CSV has wrong format

**Solution**: Ensure output redirection is correct:
```bash
./benchmark > results.csv  # Not ./benchmark | tee results.csv
```

---

## Next Steps

1. **Run all benchmarks**: Generate all CSV files
2. **Review visualizations**: Understand performance characteristics
3. **Read SOLUTION.md files**: Connect theory to results
4. **Experiment**: Modify code and re-run benchmarks
5. **Compare**: Ex1 vs Ex3 (sections), Ex2 (Amdahl's Law), Ex4 (barriers)

---

## Quick Command Reference

```bash
# Navigate
cd /mnt/c/Users/MANSOUB\ YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4/ex1

# Compile
gcc benchmark.c -O2 -fopenmp -lm -o benchmark

# Run
./benchmark > results.csv

# View
cat results.csv
column -t -s, results.csv

# Visualize
jupyter notebook viz_auto.ipynb

# Repeat for ex2, ex3...
```

---

**Happy benchmarking!** 🚀
