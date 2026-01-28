````md
# Exercise 2 — Optimizing Matrix Multiplication (mxm)

## Implementations
We compare two loop orders for `C = A * B` (row-major storage):

- **ijk (standard):**
  ```c
  for i
    for j
      for k
        C[i][j] += A[i][k] * B[k][j];
````

* **ikj (cache-optimized):**

  ```c
  for i
    for k
      for j
        C[i][j] += A[i][k] * B[k][j];
  ```

Both compute the same result (verified by identical `check = -9.690600e+00`).

---

## Results (n = 512, reps = 2)

### With `-O0`

| kernel | time (ms) | GFLOP/s | BW (GB/s) |
| ------ | --------: | ------: | --------: |
| ijk    |  1964.392 |   0.273 |     2.191 |
| ikj    |  1395.264 |   0.385 |     4.620 |

**Observation:** `ikj` is faster (~1.41×) and shows higher effective bandwidth.

**Reason:** In row-major C, `B[k][j]` is contiguous in memory when `j` is the inner loop.

* In **ijk**, inner loop is `k`, so `B[k][j]` walks down a *column* of `B` → large stride → many cache misses.
* In **ikj**, inner loop is `j`, so `B[k][j]` walks across a *row* of `B` → unit stride → cache lines are fully used.

---

### With `-O2`

| kernel | time (ms) | GFLOP/s | BW (GB/s) |
| ------ | --------: | ------: | --------: |
| ijk    |  2574.639 |   0.209 |     1.671 |
| ikj    |   152.038 |   3.531 |    42.401 |

**Observation:** `ikj` becomes dramatically faster (~16.9×). `ijk` actually gets worse than `-O0`.

**Why `ikj` improves so much with `-O2`:**

* The inner loop in `ikj` is a classic pattern:
  [
  C_\text{row} ;+=; a_{ik} \cdot B_\text{row}
  ]
  This streams contiguously through `B[k,*]` and `C[i,*]`, so the compiler can:

  * keep `aik` in a register,
  * unroll and schedule loads/stores well,
  * often auto-vectorize the inner `j` loop (SIMD),
  * benefit from hardware prefetching.

This raises compute throughput and also produces high measured “effective bandwidth”.

**Why `ijk` can get worse with `-O2`:**

* In `ijk`, access to `B[k][j]` is strided (column-like), which is hard to prefetch and hard to vectorize.
* With optimization enabled, the compiler may apply unrolling/vector attempts or more aggressive scheduling that increases cache/TLB pressure for a bad access pattern, so performance can degrade instead of improve.

---

## Conclusion

Reordering loops to make memory accesses **unit-stride** is crucial for performance.

* **`ikj`** is better because it accesses `B` and `C` contiguously in the inner loop, maximizing cache-line reuse and enabling compiler + hardware optimizations.
* **`ijk`** suffers because it accesses a column of `B` in row-major layout, causing frequent cache misses and limiting optimization benefits.

This explains the large speedup and bandwidth increase observed for `ikj`, especially under `-O2`.

```
```
