# Ex1 Quick Notes

## Task
Serial → Parallel with `sections` (sum, max, stddev)

## Key Concepts
- `#pragma omp sections` - divide work into independent sections
- Synchronization: atomic write + flush + busy wait
- Max useful threads = number of sections (3)

## Commands
```bash
# Compile
gcc initial_code.c -O2 -o serial -lm
gcc code_with_sections.c -O2 -fopenmp -o parallel -lm

# Run
./serial
OMP_NUM_THREADS=3 ./parallel
```

## Bottleneck
Section 3 busy-waits for mean → overhead

## Better Approach
Use `reduction` instead of `sections`
