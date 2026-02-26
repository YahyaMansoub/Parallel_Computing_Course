#!/bin/bash
# Benchmark script for Matrix-Vector Multiplication MPI
# Runs different matrix sizes with varying numbers of processes

OUTPUT_FILE="results.csv"

# Clean up old results
rm -f $OUTPUT_FILE

# Write CSV header
echo "N,P,time_s,max_err" > $OUTPUT_FILE

# Matrix sizes to test
SIZES=(500 1000 2000 4000 8000)

# Number of processes to test
PROCESSES=(1 2 4 8 16)

echo "Starting MPI Matrix-Vector Multiplication Benchmark..."
echo "=================================================="

# First, run baseline (serial) for each size
echo "Running baseline (serial) tests..."
for N in "${SIZES[@]}"; do
    echo "  Running baseline N=$N..."
    ./baseline $N
done

echo ""
echo "Running MPI tests..."
# Run MPI versions
for N in "${SIZES[@]}"; do
    for P in "${PROCESSES[@]}"; do
        echo "  Running N=$N, P=$P..."
        # Run and capture output
        OUTPUT=$(mpirun -np $P ./matvec_mpi $N 2>&1)
        
        # Extract time and error from output
        # Expected format: N=1000 P=4 time=0.001234 s max_err=1.23e-15
        TIME=$(echo "$OUTPUT" | grep -oP 'time=\K[0-9.]+')
        ERR=$(echo "$OUTPUT" | grep -oP 'max_err=\K[0-9.e+-]+')
        
        if [ -n "$TIME" ]; then
            echo "$N,$P,$TIME,$ERR" >> $OUTPUT_FILE
            echo "    -> time=${TIME}s, error=$ERR"
        else
            echo "    -> ERROR: Failed to parse output"
            echo "    Output: $OUTPUT"
        fi
    done
    echo ""
done

echo "=================================================="
echo "Benchmark complete! Results saved to $OUTPUT_FILE"
echo ""
echo "You can now run: python3 visualize.py"
