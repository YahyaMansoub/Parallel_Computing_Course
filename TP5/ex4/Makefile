# Makefile for MPI Matrix-Vector Multiplication
# Exercise 4 - TP5

CC = gcc
MPICC = mpicc
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lm

# Targets
SERIAL = baseline
MPI = matvec_mpi

# Default target: build both programs
all: $(SERIAL) $(MPI)

# Serial version
$(SERIAL): baseline.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# MPI version
$(MPI): matvec_mpi.c
	$(MPICC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Quick test
test: all
	@echo "Running serial baseline (N=1000)..."
	./$(SERIAL) 1000
	@echo ""
	@echo "Running MPI version with 4 processes (N=1000)..."
	mpirun -np 4 ./$(MPI) 1000

# Run benchmarks
benchmark: all
	@echo "Running full benchmark suite..."
	chmod +x run_benchmark.sh
	./run_benchmark.sh

# Generate visualizations
visualize: benchmark
	@echo "Generating plots..."
	python3 visualize.py

# Full pipeline: compile, benchmark, visualize
full: all benchmark visualize
	@echo ""
	@echo "============================================"
	@echo "Complete! Check the following files:"
	@echo "  - results.csv (raw benchmark data)"
	@echo "  - metrics.csv (speedup/efficiency)"
	@echo "  - speedup.png (speedup plot)"
	@echo "  - efficiency.png (efficiency plot)"
	@echo "  - execution_time.png (time comparison)"
	@echo "============================================"

# Clean build artifacts
clean:
	rm -f $(SERIAL) $(MPI)

# Clean everything including results
distclean: clean
	rm -f results.csv metrics.csv
	rm -f speedup.png efficiency.png execution_time.png

.PHONY: all test benchmark visualize full clean distclean
