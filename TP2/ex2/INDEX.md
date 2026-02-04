# Exercise 2: Instruction Scheduling - Complete Documentation Index

## 📚 Documentation Files (Read in this order)

### 1. **README.md** - Start here!
Quick overview, compilation commands, and key results summary.

### 2. **answers.md** - Complete Analysis (Main deliverable)
Comprehensive answers to all exercise questions including:
- Performance comparison tables
- Detailed optimization analysis
- Assembly code explanations
- Manual optimization results
- Conclusions and key takeaways

### 3. **assembly_comparison.md** - Deep Dive
Side-by-side assembly code comparison with:
- Annotated assembly listings
- Instruction count analysis
- Pipeline visualizations
- Memory bandwidth calculations
- Detailed optimization breakdowns

## 🔧 Source Files

- **ex2.c** - Original exercise code
- **ex2_manual.c** - Manually optimized version

## 📦 Compiled Artifacts

### Executables
- **ex2_O0** - Compiled with -O0 (no optimization)
- **ex2_O2** - Compiled with -O2 (full optimization)
- **ex2_manual_O0** - Manual optimization with -O0

### Assembly Files
- **O0.s** - Assembly from -O0 compilation
- **O2.s** - Assembly from -O2 compilation  
- **ex2_manual_O0.s** - Assembly from manual version
- **ex2.s** - Original assembly (intermediate)

## 🚀 Scripts

- **run_exercise.sh** - Automated compilation and benchmarking

## 📊 Quick Results

```
┌─────────────────┬──────────┬──────────┬────────────┐
│ Version         │ Time     │ Speedup  │ vs O2      │
├─────────────────┼──────────┼──────────┼────────────┤
│ Original -O0    │ 0.282 s  │ 1.00x    │ 0.25x      │
│ Manual -O0      │ 0.263 s  │ 1.07x    │ 0.27x      │
│ Compiler -O2    │ 0.071 s  │ 3.95x    │ 1.00x      │
└─────────────────┴──────────┴──────────┴────────────┘
```

## 🎯 Key Findings

1. **Compiler wins decisively**: -O2 is 3.95x faster than -O0
2. **Manual optimization limited**: Only 7% improvement at -O0 level
3. **Major optimizations**: Constant folding, register allocation, loop unrolling
4. **Memory is the bottleneck**: Eliminating memory access = massive speedup
5. **Always use optimization flags** in production!

## 📖 How to Use This Exercise

### For Quick Understanding:
```bash
cat README.md           # Overview
./ex2_O0                # Run baseline
./ex2_O2                # Run optimized
cat answers.md | less   # Read analysis
```

### For Deep Learning:
```bash
cat assembly_comparison.md          # Detailed assembly analysis
diff O0.s O2.s                      # See code changes
cat answers.md | grep "Optimization" # Find key insights
```

### For Reproduction:
```bash
./run_exercise.sh       # Re-run everything
```

### For Teaching/Presentation:
1. Show [README.md](README.md) for overview
2. Run live demos of the three executables
3. Display [assembly_comparison.md](assembly_comparison.md) side-by-side view
4. Discuss findings from [answers.md](answers.md)

## 🔍 What Each File Answers

| File | Answers |
|------|---------|
| **answers.md** | All exercise questions, complete analysis |
| **assembly_comparison.md** | How does the assembly differ? Why? |
| **README.md** | How do I compile and run this? |
| **run_exercise.sh** | Can I automate the whole process? |

## 💡 Learning Objectives Covered

✅ Understanding instruction scheduling  
✅ Analyzing compiler optimizations  
✅ Reading assembly code  
✅ Measuring performance impact  
✅ Manual vs automatic optimization  
✅ Instruction-level parallelism (ILP)  
✅ Register allocation importance  
✅ Memory access costs  
✅ Loop optimization techniques  

## 🎓 Exercise Requirements Checklist

- [x] Compare CPU execution time using -O0 and -O2
- [x] Identify main optimizations at -O2 vs -O0
- [x] Analyze instruction scheduling
- [x] Examine loop transformations
- [x] Understand ILP exploitation
- [x] Implement manually optimized version
- [x] Compile manual version with -O0
- [x] Compare manual vs automatic optimization
- [x] Draw conclusions about compiler effectiveness
- [x] Generate assembly files
- [x] Provide detailed analysis in markdown

## 📝 Citation

If you use this exercise material:

```
Exercise 2: Instruction Scheduling
Course: TP2 - Foundations of Parallel Computing
Implementation: Complete with benchmarks, assembly analysis, and documentation
Tools: GCC, Intel ASM syntax, Linux
```

## 🆘 Troubleshooting

**Q: Timing results differ from documented values?**  
A: Normal! Results depend on CPU, system load, and CPU frequency scaling. Relative speedup should be similar.

**Q: Can't execute the files?**  
A: Make script executable: `chmod +x run_exercise.sh`

**Q: Assembly files look different?**  
A: GCC version differences. Core optimizations should be similar.

**Q: Want to see more detail?**  
A: Use `gcc -O2 -fverbose-asm` for annotated assembly or `objdump -d ex2_O2` for disassembly.

---

**All exercise requirements completed! ✓**

For the main answer document, see: **[answers.md](answers.md)**
