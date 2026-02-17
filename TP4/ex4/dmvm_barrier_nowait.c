
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static void init(int n, int m, double *mat, double *rhs, double *lhs) {
    for (int c = 0; c < n; ++c) {
        rhs[c] = 1.0;
        for (int r = 0; r < m; ++r) mat[r + c * m] = 1.0; // column-major
    }
    for (int r = 0; r < m; ++r) lhs[r] = 0.0;
}

/*
  Dense matrix-vector multiply:
    lhs[r] = sum_c mat[r + c*m] * rhs[c]
  Parallelize over r (rows) => no races.
*/

static double dmvm_v1_implicit_barrier(int n, int m, double *lhs, const double *rhs, const double *mat) {
    double t0 = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int r = 0; r < m; ++r) {
        double acc = 0.0;
        for (int c = 0; c < n; ++c) acc += mat[r + c * m] * rhs[c];
        lhs[r] = acc;
    } // implicit barrier here

    return omp_get_wtime() - t0;
}

static double dmvm_v2_dynamic_nowait(int n, int m, double *lhs, const double *rhs, const double *mat) {
    double t0 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic) nowait
        for (int r = 0; r < m; ++r) {
            double acc = 0.0;
            for (int c = 0; c < n; ++c) acc += mat[r + c * m] * rhs[c];
            lhs[r] = acc;
        }

        // if you use lhs below (checksum/print), you MUST sync:
        #pragma omp barrier
    }

    return omp_get_wtime() - t0;
}

static double dmvm_v3_static_nowait(int n, int m, double *lhs, const double *rhs, const double *mat) {
    double t0 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp for schedule(static) nowait
        for (int r = 0; r < m; ++r) {
            double acc = 0.0;
            for (int c = 0; c < n; ++c) acc += mat[r + c * m] * rhs[c];
            lhs[r] = acc;
        }

        // if you use lhs below (checksum/print), you MUST sync:
        #pragma omp barrier
    }

    return omp_get_wtime() - t0;
}

static double checksum(int m, const double *lhs) {
    double s = 0.0;
    for (int r = 0; r < m; ++r) s += lhs[r];
    return s;
}

int main(void) {
    const int n = 40000; // columns
    const int m = 600;   // rows

    double *mat = (double*)malloc((size_t)n * (size_t)m * sizeof(double));
    double *rhs = (double*)malloc((size_t)n * sizeof(double));
    double *lhs = (double*)malloc((size_t)m * sizeof(double));
    if (!mat || !rhs || !lhs) {
        printf("malloc failed\n");
        return 1;
    }

    // flop count: for each (r,c): 1 mul + 1 add => ~2*n*m flops
    const double flops = 2.0 * (double)n * (double)m;

    int thread_list[] = {1, 2, 4, 8, 16};
    int T = (int)(sizeof(thread_list)/sizeof(thread_list[0]));

    // Store 1-thread baselines for speedup/efficiency (per version)
    double base_v1 = 0.0, base_v3 = 0.0;

    printf("n=%d m=%d flops_per_run=%.0f\n\n", n, m, flops);
    printf("threads, v1_time, v1_speedup, v1_eff, v1_mflops, v3_time, v3_speedup, v3_eff, v3_mflops\n");

    for (int i = 0; i < T; ++i) {
        int th = thread_list[i];
        omp_set_num_threads(th);

        // Version 1
        init(n, m, mat, rhs, lhs);
        double t1 = dmvm_v1_implicit_barrier(n, m, lhs, rhs, mat);
        double cs1 = checksum(m, lhs);

        // Version 3
        init(n, m, mat, rhs, lhs);
        double t3 = dmvm_v3_static_nowait(n, m, lhs, rhs, mat);
        double cs3 = checksum(m, lhs);

        // Optional: Version 2 (printed to stderr-ish style)
        init(n, m, mat, rhs, lhs);
        double t2 = dmvm_v2_dynamic_nowait(n, m, lhs, rhs, mat);
        double cs2 = checksum(m, lhs);
        (void)cs2;

        if (th == 1) { base_v1 = t1; base_v3 = t3; }

        double sp1 = base_v1 / t1;
        double ef1 = sp1 / th;
        double mf1 = (flops / t1) / 1e6;

        double sp3 = base_v3 / t3;
        double ef3 = sp3 / th;
        double mf3 = (flops / t3) / 1e6;

        printf("%d, %.6f, %.3f, %.3f, %.2f, %.6f, %.3f, %.3f, %.2f\n",
               th, t1, sp1, ef1, mf1, t3, sp3, ef3, mf3);

        // quick correctness sanity (should match)
        if (cs1 != cs3) {
            printf("# warning: checksum mismatch: cs1=%f cs3=%f\n", cs1, cs3);
        }

        // show v2 time quickly (not in CSV line)
        printf("# v2(dynamic+nowait) threads=%d time=%.6f checksum=%.1f\n", th, t2, cs2);
    }

    free(mat);
    free(rhs);
    free(lhs);
    return 0;
}
