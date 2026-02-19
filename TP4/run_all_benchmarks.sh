#!/bin/bash
# TP4 Benchmark Automation Script
# Compiles and runs all benchmarks, generating CSV files for visualization

echo "=========================================="
echo "  TP4 Benchmark Automation"
echo "=========================================="
echo ""

# Define base directory
BASE_DIR="/mnt/c/Users/MANSOUB YAHYA/Desktop/DESK/GitHub/Parallel_Computing_Course/TP4"
cd "$BASE_DIR"

# Color codes
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Function to compile and run benchmark
run_benchmark() {
    local ex_dir=$1
    local source=$2
    local output=$3
    local result_file=$4
    local flags=$5
    
    echo -e "${BLUE}=== $ex_dir ===${NC}"
    cd "$BASE_DIR/$ex_dir"
    
    # Compile
    echo -n "  Compiling... "
    if gcc "$source" -O2 -fopenmp $flags -o "$output" 2>/dev/null; then
        echo -e "${GREEN}OK${NC}"
    else
        echo -e "${RED}FAILED${NC}"
        return 1
    fi
    
    # Run and generate CSV
    echo -n "  Generating data... "
    if ./"$output" > "$result_file" 2>/dev/null; then
        local lines=$(wc -l < "$result_file")
        echo -e "${GREEN}OK${NC} ($lines lines)"
    else
        echo -e "${RED}FAILED${NC}"
        return 1
    fi
    
    # Preview
    echo "  Preview:"
    head -n 3 "$result_file" | sed 's/^/    /'
    echo ""
    
    cd "$BASE_DIR"
}

# Exercise 1: Sections
run_benchmark "ex1" "benchmark.c" "benchmark" "results.csv" "-lm"

# Exercise 2: Master/Single
run_benchmark "ex2" "benchmark.c" "benchmark" "results.csv" ""

# Exercise 3: Load Balancing
run_benchmark "ex3" "benchmark.c" "benchmark" "results.csv" "-lm"

# Exercise 4: Barrier/Nowait (different naming)
echo -e "${BLUE}=== ex4 ===${NC}"
cd "$BASE_DIR/ex4"
echo -n "  Compiling... "
if gcc dmvm_barrier_nowait.c -O2 -fopenmp -o ex4_O2 2>/dev/null; then
    echo -e "${GREEN}OK${NC}"
else
    echo -e "${RED}FAILED${NC}"
    exit 1
fi

echo -n "  Generating data... "
if ./ex4_O2 > dmvm_barrier_nowait.csv 2>/dev/null; then
    lines=$(wc -l < dmvm_barrier_nowait.csv)
    echo -e "${GREEN}OK${NC} ($lines lines)"
else
    echo -e "${RED}FAILED${NC}"
    exit 1
fi

echo "  Preview:"
head -n 3 dmvm_barrier_nowait.csv | sed 's/^/    /'
echo ""

cd "$BASE_DIR"

# Summary
echo "=========================================="
echo -e "  ${GREEN}All benchmarks completed!${NC}"
echo "=========================================="
echo ""
echo "Generated files:"
echo "  - ex1/results.csv"
echo "  - ex2/results.csv"
echo "  - ex3/results.csv"
echo "  - ex4/dmvm_barrier_nowait.csv"
echo ""
echo "Next steps:"
echo "  1. Review data: cat ex1/results.csv"
echo "  2. Visualize: jupyter notebook ex1/viz_auto.ipynb"
echo "  3. Read insights: cat ex1/SOLUTION.md"
echo ""
echo "Quick visualization launch:"
echo "  jupyter notebook"
echo ""
