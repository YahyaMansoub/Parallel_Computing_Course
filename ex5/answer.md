# Exercise 5 — HPL benchmark (answers)

## How the logs/results were obtained
- HPL was built from **hpl-2.3** and executed on the node using **1 MPI rank (P=1,Q=1)** and **1 thread**.
- For each configuration `(N, NB)`, the run output (stdout) is the “log”.
- The values to record are taken directly from the final summary table line that starts with **`WR...`**:

  `WR00L2L2   N   NB   P   Q   Time   Gflops`

- Numerical correctness is read from the residual line ending with **`PASSED`**, e.g.:

  `... = 7.6505e-03 ...... PASSED`

Example (provided run):
- `WR00L2L2  1000  1  1  1  0.18  3.7328e+00`
- Validation: `PASSED`

---

## Hardware + theoretical peak (single core)
Given:
- Base freq = 2.2 GHz
- 32 DP flops/cycle (AVX-512 FMA)

\[
P_{\text{core}} = 1 \times 2.2\times 10^9 \times 32 = 70.4\ \text{GFLOP/s}
\]

---

## 1) Compare measured performance vs theoretical peak
For the provided run (N=1000, NB=1):
- \[
P_{\text{HPL}} = 3.7328\ \text{GFLOP/s} \ll P_{\text{core}} = 70.4\ \text{GFLOP/s}
\]

So the achieved performance is far below the single-core peak.

---

## 2) Efficiency for each experiment
\[
\eta = \frac{P_{\text{HPL}}}{P_{\text{core}}}
\]

For the provided run:
\[
\eta = \frac{3.7328}{70.4} \approx 0.0530 \approx 5.30\%
\]

(Apply the same formula to the other 35 `(N,NB)` runs using their `Gflops`.)

---

## 3) Influence of N and NB
### Effect of increasing N
- As **N increases**, the computation becomes more dominated by large **BLAS-3 (DGEMM-like)** updates.
- Result: **GFLOP/s usually increases** (better amortization of overhead), while **time increases strongly** (~O(N³)).

### Effect of NB
- **Very small NB (e.g., 1, 2, 4):** too much overhead + poor cache reuse → lower GFLOP/s.
- **Moderate NB:** best cache blocking → higher GFLOP/s.
- **Too large NB:** panels/working sets can become less cache-friendly → GFLOP/s may plateau or drop.

### Best NB (how to identify)
- For each fixed **N**, pick the **NB** that gives the **maximum `Gflops`** in the `WR...` lines.
- With only the single provided run (N=1000, NB=1), I can’t claim the best NB yet; it’s determined by the max over the NB sweep.

---

## 4) Why measured performance is lower than theoretical peak
- Peak assumes **perfect AVX-512 FMA every cycle**; real execution has stalls, branches, and non-FMA instructions.
- HPL isn’t pure DGEMM: it includes **panel factorization, pivoting/swaps, and bookkeeping**, which are less compute-dense.
- **Cache/memory effects** + BLAS kernel overhead reduce sustained throughput.
- Actual frequency may differ from the idealized assumption (turbo limits / throttling / library behavior).
