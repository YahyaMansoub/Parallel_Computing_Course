# Exercise 2: Instruction Scheduling - Analysis and Answers

## Overview
This exercise demonstrates the impact of instruction scheduling and compiler optimizations on program performance. We analyze a simple loop-based program that performs independent floating-point operations, compiled with different optimization levels.

---

## 1. Performance Comparison: -O0 vs -O2

### Execution Time Results

| Version | Run 1 (s) | Run 2 (s) | Run 3 (s) | Average (s) | Speedup |
|---------|-----------|-----------|-----------|-------------|---------|
| **-O0** | 0.281634 | 0.281950 | 0.282889 | **0.282158** | 1.0x (baseline) |
| **-O2** | 0.071496 | 0.071455 | 0.071588 | **0.071513** | **3.95x** |
| **Manual -O0** | 0.262090 | 0.263129 | 0.265103 | **0.263441** | **1.07x** |

### Key Observations:
- **-O2 is ~3.95x faster than -O0**: The compiler performs aggressive optimizations
- **Manual optimization with -O0 provides ~7% improvement**: Shows that some optimization can be achieved at the source level
- **-O2 still outperforms manual -O0 by ~3.7x**: Compiler optimizations are significantly more effective

---

## 2. Main Optimizations Performed by -O2 Compiler

### 2.1 Constant Propagation and Precomputation

**O0 Behavior:**
```assembly
movsd xmm0, QWORD PTR -32[rbp]  ; load a
mulsd xmm0, QWORD PTR -40[rbp]  ; xmm0 = a*b
movsd xmm1, QWORD PTR -8[rbp]   ; load x
addsd xmm0, xmm1                ; xmm0 = a*b + x
movsd QWORD PTR -8[rbp], xmm0   ; store x
movsd xmm0, QWORD PTR -32[rbp]  ; load a (again!)
mulsd xmm0, QWORD PTR -40[rbp]  ; xmm0 = a*b (again!)
movsd xmm1, QWORD PTR -16[rbp]  ; load y
addsd xmm0, xmm1                ; xmm0 = a*b + y
movsd QWORD PTR -16[rbp], xmm0  ; store y
```

**O2 Optimization:**
```assembly
movsd xmm0, QWORD PTR .LC1[rip]  ; Load a*b once (precomputed: 1.1 * 1.2 = 1.32)
.L2:
    addsd xmm1, xmm0             ; x += 1.32
    addsd xmm1, xmm0             ; y += 1.32
    sub eax, 2                   ; decrement counter by 2
    jne .L2
```

**Impact:** The compiler:
- Precomputes `a * b = 1.32` at compile time (constant folding)
- Eliminates redundant multiplications inside the loop
- Reduces memory accesses dramatically

### 2.2 Loop Transformations

#### a) Loop Unrolling
- **O2 processes 2 iterations per loop iteration** (visible in `sub eax, 2`)
- Reduces loop overhead (fewer branch instructions)
- Exposes more instruction-level parallelism

#### b) Loop Strength Reduction
- Counter optimization: Uses simple integer decrement instead of comparison
- Changes from `if (i <= 99999999)` to `jne .L2` (jump if not zero)

### 2.3 Instruction Scheduling and ILP (Instruction-Level Parallelism)

**O0 Pattern:**
```
MUL → LOAD → ADD → STORE → MUL → LOAD → ADD → STORE
```
- Sequential execution with many dependencies
- Each operation waits for the previous to complete
- Multiple memory accesses per iteration

**O2 Pattern:**
```assembly
addsd xmm1, xmm0  ; Independent addition 1
addsd xmm1, xmm0  ; Independent addition 2 (can execute in parallel)
```
- Both additions can execute in parallel on modern CPUs (superscalar execution)
- No memory loads/stores inside the loop
- Maximizes CPU pipeline utilization

### 2.4 Register Allocation Optimization

**O0:**
- Uses memory locations on the stack: `QWORD PTR -32[rbp]`, `QWORD PTR -40[rbp]`
- Every variable access requires memory load/store
- High memory traffic

**O2:**
- Keeps all variables in registers: `xmm0`, `xmm1`, `eax`
- Single memory store outside the loop
- Eliminates memory bottleneck

### 2.5 Dead Code Elimination and Store Optimization

**O2 further optimizes:**
- Recognizes that `x` and `y` are only used for final output
- Merges both streams into a single register (`xmm1`)
- Single store operation after the loop completes

---

## 3. Assembly Code Analysis

### O0 Assembly - Loop Body (.L3)
```assembly
.L3:
    movsd xmm0, QWORD PTR -32[rbp]    ; Load 'a' from stack
    mulsd xmm0, QWORD PTR -40[rbp]    ; Multiply a * b
    movsd xmm1, QWORD PTR -8[rbp]     ; Load 'x' from stack
    addsd xmm0, xmm1                  ; Compute a*b + x
    movsd QWORD PTR -8[rbp], xmm0     ; Store result to 'x'
    
    movsd xmm0, QWORD PTR -32[rbp]    ; Load 'a' AGAIN
    mulsd xmm0, QWORD PTR -40[rbp]    ; Multiply a * b AGAIN
    movsd xmm1, QWORD PTR -16[rbp]    ; Load 'y' from stack
    addsd xmm0, xmm1                  ; Compute a*b + y
    movsd QWORD PTR -16[rbp], xmm0    ; Store result to 'y'
    
    add DWORD PTR -20[rbp], 1         ; Increment loop counter
.L2:
    cmp DWORD PTR -20[rbp], 99999999  ; Compare counter
    jle .L3                           ; Jump if less or equal
```

**Inefficiencies:**
- 2 multiplications per iteration (redundant)
- 4 memory loads per iteration
- 2 memory stores per iteration
- Full comparison for loop condition

### O2 Assembly - Loop Body (.L2)
```assembly
movsd xmm0, QWORD PTR .LC1[rip]  ; Load precomputed a*b = 1.32 (ONCE, outside loop)
mov eax, 100000000                ; Initialize counter

.L2:
    addsd xmm1, xmm0              ; xmm1 = xmm1 + 1.32 (handles both x and y)
    addsd xmm1, xmm0              ; xmm1 = xmm1 + 1.32 (2nd iteration unrolled)
    sub eax, 2                    ; Decrement counter by 2
    jne .L2                       ; Loop if not zero
```

**Optimizations:**
- 0 multiplications in the loop
- 0 memory accesses in the loop
- All operations use registers
- Loop unrolling (2 iterations per loop)
- Simplified loop exit condition

---

## 4. Manual Optimization Implementation

### Strategy
Our manually optimized version (`ex2_manual.c`) implements:

1. **Constant Hoisting**: Move `a * b` computation outside the loop
   ```c
   double ab = a * b;  // Compute once
   ```

2. **Loop Unrolling**: Process multiple iterations together
   ```c
   for (i = 0; i < N - 1; i += 2) {
       x = ab + x;  // iteration 1, stream 1
       y = ab + y;  // iteration 1, stream 2
       x = ab + x;  // iteration 2, stream 1
       y = ab + y;  // iteration 2, stream 2
   }
   ```

3. **Instruction Scheduling**: Interleave independent operations to expose parallelism

### Results Analysis

| Optimization Level | Time (avg) | vs O0 | vs O2 |
|-------------------|-----------|-------|-------|
| Original -O0 | 0.282 s | 1.00x | 0.25x |
| Manual -O0 | 0.263 s | **1.07x** | 0.27x |
| Original -O2 | 0.071 s | 3.95x | **1.00x** |

**Performance Improvement:**
- Manual optimization achieves **~7% speedup** over baseline -O0
- Still **3.7x slower** than compiler -O2 optimization

---

## 5. Conclusions

### 5.1 Compiler Effectiveness
Modern compilers (GCC -O2) are **exceptionally effective** at optimization:
- Nearly **4x speedup** with a simple flag
- Performs multiple sophisticated optimizations automatically
- Better than manual optimization at -O0 level

### 5.2 Why Compiler Wins

The compiler has several advantages:
1. **Global Analysis**: Sees the entire program context
2. **Target-Specific**: Knows exact CPU architecture and capabilities
3. **Multiple Passes**: Applies numerous optimization techniques in sequence
4. **Pattern Recognition**: Identifies optimization opportunities humans might miss

### 5.3 Limitations of Manual Optimization

Manual optimization with -O0 is limited because:
- We can only optimize at the source level
- The compiler still generates naive assembly from our source
- Cannot control register allocation without optimization flags
- Cannot disable memory traffic without compiler help

### 5.4 Key Takeaways

1. **Instruction Scheduling Matters**: Reordering independent operations improves pipeline utilization
2. **Memory Access is Expensive**: Register allocation is crucial for performance
3. **Loop Optimizations are Critical**: Unrolling, strength reduction, and invariant code motion provide significant gains
4. **Trust the Compiler**: Modern compilers are incredibly sophisticated - use optimization flags!
5. **ILP Exploitation**: Exposing instruction-level parallelism allows modern CPUs to execute multiple operations simultaneously

### 5.5 Practical Implications

For production code:
- **Always use compiler optimizations** (`-O2` or `-O3` for GCC)
- Write clear, simple code - let the compiler optimize
- Manual optimization is only necessary for:
  - Algorithmic improvements
  - Cache optimization
  - Domain-specific knowledge the compiler doesn't have
- Profile before optimizing - measure, don't guess!

---

## 6. Detailed Optimization Summary

| Optimization Technique | O0 | Manual O0 | O2 | Impact |
|------------------------|-------|----------|-----|---------|
| Constant Folding | ❌ | ✅ | ✅ | Eliminates 2 MUL ops per iteration |
| Loop Unrolling | ❌ | ✅ | ✅ | Reduces branch overhead |
| Register Allocation | ❌ | ❌ | ✅ | Eliminates memory traffic |
| Instruction Scheduling | ❌ | Partial | ✅ | Improves pipeline efficiency |
| Strength Reduction | ❌ | ❌ | ✅ | Simplifies loop control |
| Dead Code Elimination | ❌ | ❌ | ✅ | Removes unnecessary operations |

**Legend:**
- ✅ = Optimization applied
- ❌ = Optimization not applied
- Partial = Partially applied

---

## Final Performance Summary

```
Speedup Factor:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
O2 vs O0:        ████████████████░░░░  3.95x
Manual vs O0:    ██░░░░░░░░░░░░░░░░░░  1.07x
O2 vs Manual:    ██████████████░░░░░░  3.69x
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

**The compiler's -O2 optimization is the clear winner, demonstrating the power of modern compiler technology in exploiting instruction-level parallelism and performing sophisticated code transformations.**
