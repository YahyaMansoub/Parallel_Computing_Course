# Exercise 3: Navigation Guide

## 🎯 Start Here

**New to this exercise?** Read in this order:
1. [README.md](README.md) - Complete overview
2. [answers.md](answers.md) - Detailed answers to all questions
3. [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - Formulas and key values
4. [SUMMARY.md](SUMMARY.md) - What was accomplished

## 📂 File Organization

### 📖 Documentation
| File | Purpose | When to Use |
|------|---------|-------------|
| [README.md](README.md) | Complete guide with explanations | Understanding the exercise |
| [answers.md](answers.md) | Detailed answers to Questions 1-5 | Studying theory |
| [QUICK_REFERENCE.md](QUICK_REFERENCE.md) | Formulas and lookup table | Quick reference |
| [SUMMARY.md](SUMMARY.md) | Implementation summary | Overview of completion |
| **INDEX.md** (this file) | Navigation guide | Finding files |

### 💻 Source Code
| File | Purpose | When to Use |
|------|---------|-------------|
| [program.c](program.c) | Main program (N=100M) | Original specification |
| [program_N5M.c](program_N5M.c) | Fast version (N=5M) | Quick profiling |
| [program_N10M.c](program_N10M.c) | Medium version (N=10M) | Standard profiling |
| program, program_N5M, program_N10M | Compiled binaries | Execution |

### 🔧 Analysis Tools
| File | Purpose | When to Use |
|------|---------|-------------|
| [calculate_scaling.py](calculate_scaling.py) | Compute speedups | Generate results |
| [run_analysis.sh](run_analysis.sh) | Automated pipeline | Run everything |
| [visualizations.ipynb](visualizations.ipynb) | Generate plots | Create visualizations |

### 📊 Data Files
| File | Purpose | Contains |
|------|---------|----------|
| [profiling_results.csv](profiling_results.csv) | Profiling data | Sequential fractions for N=5M, 10M, 100M |
| [scaling_results.csv](scaling_results.csv) | Speedup calculations | Amdahl & Gustafson for all p values |
| callgrind.out.N5M | Valgrind output | Profiling for N=5M |
| callgrind.out.N10M | Valgrind output | Profiling for N=10M |
| callgrind.out.N100M | Valgrind output | Profiling for N=100M |

## 🎓 By Question

### Question 1: Code Analysis
- **Answer**: [answers.md](answers.md#question-1--code-analysis)
- **Code**: [program.c](program.c)
- **Explanation**: Identify sequential vs parallelizable parts

### Question 2: Measuring Sequential Fraction
- **Answer**: [answers.md](answers.md#question-2--measuring-the-sequential-fraction)
- **Data**: [profiling_results.csv](profiling_results.csv)
- **Commands**: See [README.md](README.md#2-profile-with-valgrind)
- **Result**: fs ≈ 0.307 (30.7%)

### Question 3: Strong Scaling (Amdahl)
- **Answer**: [answers.md](answers.md#question-3--strong-scaling-amdahls-law)
- **Data**: [scaling_results.csv](scaling_results.csv)
- **Visualization**: Run [visualizations.ipynb](visualizations.ipynb) → q3_amdahl_strong_scaling.png
- **Key Result**: Max speedup ≈ 3.26x

### Question 4: Effect of Problem Size
- **Answer**: [answers.md](answers.md#question-4--effect-of-problem-size)
- **Data**: [profiling_results.csv](profiling_results.csv)
- **Visualization**: Run [visualizations.ipynb](visualizations.ipynb) → q4_problem_size_effect.png
- **Finding**: fs remains constant (~30.7%)

### Question 5: Weak Scaling (Gustafson)
- **Answer**: [answers.md](answers.md#question-5--weak-scaling-gustafsons-law)
- **Data**: [scaling_results.csv](scaling_results.csv)
- **Visualization**: Run [visualizations.ipynb](visualizations.ipynb) → q5_amdahl_vs_gustafson.png
- **Comparison**: Gustafson much more optimistic

## 🚀 Common Tasks

### I want to understand the theory
1. Read [answers.md](answers.md) - comprehensive explanations
2. Check [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - formulas
3. Review [README.md](README.md) - practical context

### I want to run the analysis
```bash
# Option 1: Run everything automatically
./run_analysis.sh

# Option 2: Step by step
gcc -O2 -g program_N10M.c -o program_N10M
valgrind --tool=callgrind ./program_N10M
python3 calculate_scaling.py
jupyter notebook visualizations.ipynb
```

### I want to see the results
- **Sequential fraction**: Check [profiling_results.csv](profiling_results.csv)
- **Speedup values**: Check [scaling_results.csv](scaling_results.csv)
- **Plots**: Run [visualizations.ipynb](visualizations.ipynb)
- **Summary**: Read [SUMMARY.md](SUMMARY.md)

### I want to lookup a formula
- **Amdahl's Law**: [QUICK_REFERENCE.md](QUICK_REFERENCE.md#amdahls-law-strong-scaling)
- **Gustafson's Law**: [QUICK_REFERENCE.md](QUICK_REFERENCE.md#gustafsons-law-weak-scaling)
- **Efficiency**: [QUICK_REFERENCE.md](QUICK_REFERENCE.md#efficiency)

### I want to verify profiling
```bash
# View profiling data
callgrind_annotate callgrind.out.N10M | head -50

# Or use GUI
kcachegrind callgrind.out.N10M
```

## 📈 Key Values (Quick Lookup)

```
Sequential Fraction:    fs ≈ 0.307 (30.7%)
Parallelizable Fraction: fp ≈ 0.693 (69.3%)
Maximum Speedup:        S_max ≈ 3.26x
Optimal Processors:     p = 8-16
```

### Speedup at Common Processor Counts
| p | Amdahl | Gustafson | Efficiency |
|---|--------|-----------|------------|
| 2 | 1.53x  | 1.69x     | 76.5%      |
| 4 | 2.08x  | 3.08x     | 52.0%      |
| 8 | 2.54x  | 5.85x     | 31.7%      |
| 16| 2.85x  | 11.39x    | 17.8%      |
| 64| 3.14x  | 44.63x    | 4.9%       |

Full table: [scaling_results.csv](scaling_results.csv)

## 🎨 Visualizations

All plots are generated by [visualizations.ipynb](visualizations.ipynb):

1. **q3_amdahl_strong_scaling.png**
   - Amdahl speedup curve
   - Parallel efficiency plot
   - Shows saturation effect

2. **q4_problem_size_effect.png**
   - Speedup for different N values
   - Sequential fraction vs N
   - Demonstrates constant fs

3. **q5_amdahl_vs_gustafson.png**
   - Direct comparison of laws
   - Speedup ratio plot
   - Shows divergence at high p

4. **function_breakdown.png**
   - Pie chart of instructions
   - Bar chart by function
   - Identifies bottlenecks

## 🔍 Troubleshooting

### "ModuleNotFoundError: No module named 'pandas'"
- **Solution**: The script uses only standard libraries (csv)
- If error persists, check you're running the latest version

### "Command 'valgrind' not found"
- **Install**: `sudo apt-get install valgrind`

### "Jupyter notebook won't open"
- **Install**: `pip install jupyter matplotlib numpy`
- **Alternative**: View results in CSV files

### "Profiling takes too long"
- **Use**: program_N5M instead of program (100M)
- N=5M completes in ~30 seconds vs ~10 minutes for N=100M

## 📚 Learning Path

### Beginner
1. Read [README.md](README.md) introduction
2. Run `./run_analysis.sh`
3. Review [QUICK_REFERENCE.md](QUICK_REFERENCE.md)
4. Check [scaling_results.csv](scaling_results.csv)

### Intermediate
1. Read [answers.md](answers.md) completely
2. Understand profiling: `callgrind_annotate callgrind.out.N10M`
3. Study [calculate_scaling.py](calculate_scaling.py) code
4. Run [visualizations.ipynb](visualizations.ipynb)

### Advanced
1. Modify [program.c](program.c) to change fs
2. Profile with different N values
3. Implement parallel versions
4. Compare theoretical vs actual speedups
5. Experiment with different algorithms

## 🎯 Exercise Goals

By completing this exercise, you should understand:

- ✅ How to profile programs with Valgrind
- ✅ How to identify sequential bottlenecks
- ✅ How to calculate sequential fractions
- ✅ How to apply Amdahl's Law
- ✅ How to apply Gustafson's Law
- ✅ Difference between strong and weak scaling
- ✅ Why efficiency declines with more processors
- ✅ When parallelization is worthwhile

## 💡 Tips

1. **Start with documentation** - Don't jump into code
2. **Understand the theory** - Formulas are in [QUICK_REFERENCE.md](QUICK_REFERENCE.md)
3. **Run step by step** - Use [run_analysis.sh](run_analysis.sh) or manual commands
4. **Visualize results** - Plots help understanding
5. **Compare predictions** - Theory vs measured values

## 🆘 Need Help?

1. **Formulas**: → [QUICK_REFERENCE.md](QUICK_REFERENCE.md)
2. **Theory**: → [answers.md](answers.md)
3. **How to run**: → [README.md](README.md)
4. **What's done**: → [SUMMARY.md](SUMMARY.md)
5. **Navigate**: → **INDEX.md** (this file)

## ✅ Checklist

Use this to verify completion:

- [ ] Read [README.md](README.md)
- [ ] Compiled all programs
- [ ] Run Valgrind profiling
- [ ] Generated [profiling_results.csv](profiling_results.csv)
- [ ] Calculated speedups → [scaling_results.csv](scaling_results.csv)
- [ ] Answered Question 1 (code analysis)
- [ ] Answered Question 2 (sequential fraction)
- [ ] Answered Question 3 (Amdahl)
- [ ] Answered Question 4 (problem size)
- [ ] Answered Question 5 (Gustafson)
- [ ] Generated visualizations
- [ ] Understood key concepts

## 📞 Quick Reference

| Need | Go To |
|------|-------|
| Formulas | [QUICK_REFERENCE.md](QUICK_REFERENCE.md) |
| Answers | [answers.md](answers.md) |
| Guide | [README.md](README.md) |
| Data | [scaling_results.csv](scaling_results.csv) |
| Code | [program.c](program.c) |
| Run | [run_analysis.sh](run_analysis.sh) |
| Plots | [visualizations.ipynb](visualizations.ipynb) |
| Summary | [SUMMARY.md](SUMMARY.md) |

---

**Last Updated**: February 4, 2026  
**Exercise**: TP2 - Exercise 3  
**Topic**: Amdahl's Law and Gustafson's Law
