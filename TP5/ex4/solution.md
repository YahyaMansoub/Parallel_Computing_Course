# Exercise 4: Matrix-Vector Product - Solution

## Implementation Overview

### Serial Implementation (baseline.c)
Standard matrix-vector multiplication:
```
x[i] = Σ(A[i][j] * b[j]) for j = 0 to N-1
```

Each element of result vector `x` is computed by taking the dot product of a row of `A` with vector `b`.

### MPI Parallel Implementation (matvec_mpi.c)

#### Strategy: Row Distribution
- Matrix rows are distributed across processes
- Each process computes its assigned rows of the result vector
- Vector `b` must be available to all processes (broadcast)

#### Key MPI Operations

1. **Distribution Calculation** (`build_counts_displs`)
   - Handles uneven distribution when N % P ≠ 0
   - First `rem` processes get `⌈N/P⌉` rows
   - Remaining processes get `⌊N/P⌋` rows

2. **Broadcast Vector b** (`MPI_Bcast`)
   - All processes need the complete vector `b`
   - Broadcast from rank 0 to all processes

3. **Scatter Matrix Rows** (`MPI_Scatterv`)
   - Rank 0 scatters different-sized chunks of A to each process
   - Each process receives its `local_rows * N` elements

4. **Local Computation**
   - Each process independently computes its portion of result
   - No inter-process communication during computation

5. **Gather Results** (`MPI_Gatherv`)
   - All processes send their local results to rank 0
   - Rank 0 reconstructs the complete result vector

#### Handling Non-Divisible N

Example: N=10, P=3
```
base = 10 / 3 = 3
rem  = 10 % 3 = 1

Process 0: rows = 3 + 1 = 4  (rows 0-3)
Process 1: rows = 3 + 0 = 3  (rows 4-6)
Process 2: rows = 3 + 0 = 3  (rows 7-9)
```

Code implementation:
```c
int base = N / P;
int rem  = N % P;

for (int r = 0; r < P; ++r) {
    rows[r] = base + (r < rem ? 1 : 0);
    // ...
}
```

### Correctness Verification
- Rank 0 computes serial result for comparison
- Compares parallel result with serial element-wise
- Reports maximum absolute error
- Typical error: ~10^-15 (floating-point precision)

## Performance Analysis

### Expected Speedup Characteristics

**Strong Scaling** (fixed problem size, varying P):
- Small N: Poor speedup (communication overhead dominates)
- Large N: Better speedup (more computation per communication)

**Factors Affecting Performance:**

1. **Communication Overhead**
   - Broadcasting `b`: O(N)
   - Scattering `A`: O(N²/P)
   - Gathering result: O(N/P)

2. **Computation**
   - Each process: O(N²/P) operations
   - Large N → computation dominates → better speedup

3. **Amdahl's Law**
   - Initialization and verification are serial
   - Limits maximum achievable speedup

### Typical Results

| N    | P=1  | P=2  | P=4  | P=8  |
|------|------|------|------|------|
| 1000 | 1.0x | 1.8x | 3.2x | 5.1x |
| 4000 | 1.0x | 1.9x | 3.7x | 6.8x |
| 8000 | 1.0x | 2.0x | 3.9x | 7.5x |

*Note: Actual results vary based on hardware and network*

### Efficiency Analysis

Efficiency = Speedup / P

- **High N, Low P**: Usually >80% efficiency
- **Low N, High P**: Can drop to <50%
- **Sweet spot**: Balance problem size with process count

## Running the Code in WSL

### Complete Workflow

```bash
# 1. Navigate to ex4 directory
cd TP5/ex4

# 2. Compile everything
make

# 3. Quick test
make test

# 4. Run full benchmark
make benchmark

# 5. Generate plots
make visualize

# Or do everything in one command:
make full
```

### Step-by-Step Commands

```bash
# Compile programs
gcc -O2 -o baseline baseline.c -lm
mpicc -O2 -o matvec_mpi matvec_mpi.c -lm

# Run serial version
./baseline 1000

# Run MPI version with different process counts
mpirun -np 1 ./matvec_mpi 1000
mpirun -np 2 ./matvec_mpi 1000
mpirun -np 4 ./matvec_mpi 1000
mpirun -np 8 ./matvec_mpi 1000

# Full benchmark
chmod +x run_benchmark.sh
./run_benchmark.sh

# Generate visualizations
python3 visualize.py
```

## Output Files

After running benchmarks:
- `results.csv` - Raw benchmark data (N, P, time, error)
- `metrics.csv` - Computed speedup and efficiency
- `speedup.png` - Speedup vs process count plot
- `efficiency.png` - Efficiency vs process count plot
- `execution_time.png` - Execution time comparison

## Key Takeaways

1. **Row distribution** is natural for matrix-vector multiplication
2. **MPI_Scatterv/Gatherv** handle uneven distribution elegantly
3. **Communication costs** are significant for small problems
4. **Larger problems** show better parallel efficiency
5. **Correctness checking** is essential (floating-point errors possible)

## Questions to Consider

1. Why does efficiency typically decrease as P increases?
2. What happens if N << P (matrix smaller than process count)?
3. Could we use column distribution instead? What changes?
4. How does network speed affect performance?
5. What is the minimum N for good speedup with P processes?
