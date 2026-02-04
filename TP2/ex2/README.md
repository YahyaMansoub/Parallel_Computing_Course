# Exercise 2: Instruction Scheduling

## Quick Start

Run all tests with a single command:
```bash
./run_exercise.sh
```

## Files Overview

### Source Code
- **ex2.c** - Original code from the exercise
- **ex2_manual.c** - Manually optimized version with loop unrolling and constant hoisting

### Compiled Executables
- **ex2_O0** - Original compiled with `-O0` (no optimization)
- **ex2_O2** - Original compiled with `-O2` (full optimization)
- **ex2_manual_O0** - Manual optimization compiled with `-O0`

### Assembly Files
- **O0.s** - Assembly from `-O0` compilation
- **O2.s** - Assembly from `-O2` compilation
- **ex2_manual_O0.s** - Assembly from manually optimized version

### Documentation
- **answers.md** - Complete analysis with results, comparisons, and conclusions
- **run_exercise.sh** - Automation script for compilation and execution

## Manual Compilation Commands

### Compile executables:
```bash
gcc -O0 -fno-omit-frame-pointer ex2.c -o ex2_O0
gcc -O2 -fno-omit-frame-pointer ex2.c -o ex2_O2
gcc -O0 -fno-omit-frame-pointer ex2_manual.c -o ex2_manual_O0
```

### Generate assembly:
```bash
gcc -O0 -fno-omit-frame-pointer -S -masm=intel ex2.c -o O0.s
gcc -O2 -fno-omit-frame-pointer -S -masm=intel ex2.c -o O2.s
gcc -O0 -fno-omit-frame-pointer -S -masm=intel ex2_manual.c -o ex2_manual_O0.s
```

### Run benchmarks:
```bash
./ex2_O0          # ~0.28 seconds
./ex2_O2          # ~0.07 seconds (3.95x faster!)
./ex2_manual_O0   # ~0.26 seconds (1.07x faster than O0)
```

## Key Results Summary

| Version | Time | Speedup vs O0 |
|---------|------|---------------|
| -O0 | 0.282 s | 1.00x |
| -O2 | 0.071 s | **3.95x** |
| Manual -O0 | 0.263 s | 1.07x |

## Main Findings

1. **Compiler -O2 is nearly 4x faster** than -O0
2. **Key optimizations**: constant folding, loop unrolling, register allocation, instruction scheduling
3. **Manual optimization** provides modest 7% improvement but compiler still wins by 3.7x
4. **Conclusion**: Modern compilers are highly effective - always use optimization flags!

## View Analysis

For detailed analysis of assembly code, optimizations, and conclusions:
```bash
cat answers.md
# or open in your favorite text editor
```

## Assembly Code Highlights

### O0 Loop (Inefficient)
```assembly
movsd xmm0, QWORD PTR -32[rbp]  ; Load a
mulsd xmm0, QWORD PTR -40[rbp]  ; a * b
movsd xmm1, QWORD PTR -8[rbp]   ; Load x
addsd xmm0, xmm1                ; a*b + x
movsd QWORD PTR -8[rbp], xmm0   ; Store x
; ... repeat for y ...
```

### O2 Loop (Optimized)
```assembly
movsd xmm0, QWORD PTR .LC1[rip] ; Load precomputed a*b
.L2:
    addsd xmm1, xmm0            ; Add (unrolled)
    addsd xmm1, xmm0            ; Add (unrolled)
    sub eax, 2                  ; Counter -= 2
    jne .L2                     ; Loop
```

The O2 version:
- ✅ No multiplications in loop
- ✅ No memory accesses in loop
- ✅ Loop unrolling (2 iterations)
- ✅ All operations in registers
