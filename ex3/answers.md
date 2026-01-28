# Exercise 3 — Blocked Matrix Multiplication (Results + Analysis)

## 1) Block matrix multiplication (what it is)
Blocked (tiled) multiplication computes `C = A·B` by splitting matrices into `B×B` tiles and computing a whole tile of `C` using matching tiles of `A` and `B`.  
This improves cache locality because the same small blocks are reused many times while they remain in cache.

## 2) Data (n=512, reps=2, -O2)
CSV columns: `block, time_ms, gflops, bandwidth_GBps`

| Block B | Time (ms) | GFLOP/s | Bandwidth (GB/s) |
|---:|---:|---:|---:|
| 8   | 123.774 | 4.337 | 0.136 |
| 16  | 100.399 | 5.347 | 0.167 |
| 32  | 71.660  | 7.492 | 0.234 |
| 64  | 64.991  | 8.261 | 0.258 |
| 128 | 65.476  | 8.200 | 0.256 |

## 3) Compare CPU time and bandwidth for each block size
- Time drops significantly from B=8 → B=64 (123.8 ms → 65.0 ms).
- GFLOP/s rises from B=8 → B=64 (4.34 → 8.26 GFLOP/s).
- Past B=64, performance saturates: B=128 is slightly worse than B=64.

## 4) Optimal block size
**Optimal block size is B = 64** (lowest time, highest GFLOP/s).

## 5) Why B=64 is best
- For small blocks (B=8,16), tiles fit easily in cache but do too little work per tile → more loop overhead and less efficient reuse/vectorization.
- Increasing B increases reuse: each loaded tile of A and B participates in ~B³ operations, so cache lines are reused more before eviction.
- When B gets too large (here B≈128), the working set (A_tile + B_tile + C_tile) starts to stress cache capacity/associativity and causes more evictions, so gains stop.

## 6) Note about the printed “bandwidth”
The very low bandwidth values suggest the bandwidth formula in the code is not measuring DRAM traffic; it’s a simplified “useful bytes” estimate.  
For this exercise, the reliable comparison is **time_ms** and **GFLOP/s**, which clearly show B=64 is optimal.
