#!/bin/bash
# Compilation and Execution Script for Exercise 2
# Instruction Scheduling Analysis

echo "=========================================="
echo "Exercise 2: Instruction Scheduling"
echo "=========================================="
echo ""

# Set working directory
cd "$(dirname "$0")"

echo "Step 1: Compiling all versions..."
echo "----------------------------------"

# Compile original code with O0
echo "Compiling ex2.c with -O0..."
gcc -O0 -fno-omit-frame-pointer ex2.c -o ex2_O0
if [ $? -eq 0 ]; then
    echo "✓ ex2_O0 compiled successfully"
else
    echo "✗ ex2_O0 compilation failed"
    exit 1
fi

# Compile original code with O2
echo "Compiling ex2.c with -O2..."
gcc -O2 -fno-omit-frame-pointer ex2.c -o ex2_O2
if [ $? -eq 0 ]; then
    echo "✓ ex2_O2 compiled successfully"
else
    echo "✗ ex2_O2 compilation failed"
    exit 1
fi

# Compile manually optimized code with O0
echo "Compiling ex2_manual.c with -O0..."
gcc -O0 -fno-omit-frame-pointer ex2_manual.c -o ex2_manual_O0
if [ $? -eq 0 ]; then
    echo "✓ ex2_manual_O0 compiled successfully"
else
    echo "✗ ex2_manual_O0 compilation failed"
    exit 1
fi

echo ""
echo "Step 2: Generating assembly files..."
echo "-------------------------------------"

# Generate assembly for O0
gcc -O0 -fno-omit-frame-pointer -S -masm=intel ex2.c -o O0.s
echo "✓ O0.s generated"

# Generate assembly for O2
gcc -O2 -fno-omit-frame-pointer -S -masm=intel ex2.c -o O2.s
echo "✓ O2.s generated"

# Generate assembly for manual optimization
gcc -O0 -fno-omit-frame-pointer -S -masm=intel ex2_manual.c -o ex2_manual_O0.s
echo "✓ ex2_manual_O0.s generated"

echo ""
echo "Step 3: Running benchmarks (3 runs each)..."
echo "--------------------------------------------"

echo ""
echo "Running ex2_O0 (original with -O0):"
for i in 1 2 3; do
    echo "  Run $i:"
    ./ex2_O0
done

echo ""
echo "Running ex2_O2 (original with -O2):"
for i in 1 2 3; do
    echo "  Run $i:"
    ./ex2_O2
done

echo ""
echo "Running ex2_manual_O0 (manually optimized with -O0):"
for i in 1 2 3; do
    echo "  Run $i:"
    ./ex2_manual_O0
done

echo ""
echo "=========================================="
echo "All tests completed!"
echo "=========================================="
echo ""
echo "Generated files:"
echo "  - Executables: ex2_O0, ex2_O2, ex2_manual_O0"
echo "  - Assembly: O0.s, O2.s, ex2_manual_O0.s"
echo "  - Analysis: answers.md"
echo ""
echo "To view assembly code:"
echo "  cat O0.s    # View -O0 assembly"
echo "  cat O2.s    # View -O2 assembly"
echo ""
echo "To read the analysis:"
echo "  cat answers.md"
echo ""
