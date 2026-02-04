#!/bin/bash

# Exercise 3: Complete Analysis Script
# This script runs the full profiling and analysis pipeline

echo "========================================================================"
echo "Exercise 3: Amdahl's Law and Gustafson's Law - Complete Analysis"
echo "========================================================================"
echo ""

# Set working directory
cd "$(dirname "$0")"

echo "Step 1: Compiling programs..."
echo "------------------------------------------------------------------------"
gcc -O2 -g program.c -o program
gcc -O2 -g program_N5M.c -o program_N5M
gcc -O2 -g program_N10M.c -o program_N10M

if [ $? -eq 0 ]; then
    echo "✓ All programs compiled successfully"
else
    echo "✗ Compilation failed"
    exit 1
fi
echo ""

echo "Step 2: Running Valgrind profiling (this may take a while)..."
echo "------------------------------------------------------------------------"

# Profile N=5M
echo "Profiling N=5M..."
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N5M ./program_N5M > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ N=5M profiling complete"
else
    echo "✗ N=5M profiling failed"
fi

# Profile N=10M
echo "Profiling N=10M..."
valgrind --tool=callgrind --callgrind-out-file=callgrind.out.N10M ./program_N10M > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ N=10M profiling complete"
else
    echo "✗ N=10M profiling failed"
fi
echo ""

echo "Step 3: Analyzing Callgrind results..."
echo "------------------------------------------------------------------------"
echo ""
echo "Analysis for N=5M:"
echo "-------------------"
callgrind_annotate callgrind.out.N5M | head -35
echo ""
echo "Analysis for N=10M:"
echo "-------------------"
callgrind_annotate callgrind.out.N10M | head -35
echo ""

echo "Step 4: Calculating scaling metrics..."
echo "------------------------------------------------------------------------"
python3 calculate_scaling.py
echo ""

echo "Step 5: Summary of results..."
echo "------------------------------------------------------------------------"
echo ""
echo "Generated files:"
ls -lh *.csv *.out.* program* 2>/dev/null | grep -v ".c"
echo ""

echo "========================================================================"
echo "Analysis Complete!"
echo "========================================================================"
echo ""
echo "Next steps:"
echo "  1. Review answers.md for detailed theoretical analysis"
echo "  2. Open visualizations.ipynb to generate plots"
echo "  3. Check profiling_results.csv and scaling_results.csv for data"
echo ""
echo "Key findings:"
echo "  - Sequential fraction (fs): ~30.7%"
echo "  - Maximum Amdahl speedup: ~3.26x"
echo "  - Bottleneck: add_noise() function"
echo ""
