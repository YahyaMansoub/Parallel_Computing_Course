#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static inline double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

static void *xaligned_malloc(size_t bytes, size_t alignment) {
    void *p = NULL;
    if (posix_memalign(&p, alignment, bytes) != 0) return NULL;
    return p;
}

static void init_mats(double *A, double *B, double *C, int n) {
    size_t nn = (size_t)n * (size_t)n;
    for (size_t i = 0; i < nn; i++) {
        A[i] = ((double)((i % 97) - 48)) * 0.01;
        B[i] = ((double)((i % 89) - 44)) * 0.01;
        C[i] = 0.0;
    }
}

static double checksum_sparse(const double *C, int n) {
    size_t nn = (size_t)n * (size_t)n;
    size_t step = nn / 1024 + 1;
    double s = 0.0;
    for (size_t i = 0; i < nn; i += step) s += C[i];
    return s;
}

static inline int imin(int a, int b) { return a < b ? a : b; }


static void mxm_blocked(const double *restrict A,
                        const double *restrict B,
                        double *restrict C,
                        int n, int BS)
{
    for (int i0 = 0; i0 < n; i0 += BS) {
        int i_max = imin(i0 + BS, n);
        for (int k0 = 0; k0 < n; k0 += BS) {
            int k_max = imin(k0 + BS, n);
            for (int j0 = 0; j0 < n; j0 += BS) {
                int j_max = imin(j0 + BS, n);

                for (int i = i0; i < i_max; i++) {
                    double *Ci = &C[(size_t)i * n];
                    for (int k = k0; k < k_max; k++) {
                        double aik = A[(size_t)i * n + k];      
                        const double *Bk = &B[(size_t)k * n];   
                        for (int j = j0; j < j_max; j++) {
                            Ci[j] += aik * Bk[j];
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    int n    = (argc >= 2) ? atoi(argv[1]) : 512;
    int reps = (argc >= 3) ? atoi(argv[2]) : 2;

    size_t bytes = (size_t)n * (size_t)n * sizeof(double);
    double *A = xaligned_malloc(bytes, 64);
    double *B = xaligned_malloc(bytes, 64);
    double *C = xaligned_malloc(bytes, 64);
    if (!A || !B || !C) {
        fprintf(stderr, "alloc failed\n");
        return 1;
    }

    
    int default_B[] = {4, 8, 16, 24, 32, 48, 64, 96, 128};
    int *Bs = default_B;
    int nb  = (int)(sizeof(default_B) / sizeof(default_B[0]));

    if (argc >= 4) {
        nb = argc - 3;
        Bs = (int*)malloc((size_t)nb * sizeof(int));
        if (!Bs) { fprintf(stderr, "alloc failed\n"); return 1; }
        for (int i = 0; i < nb; i++) Bs[i] = atoi(argv[3 + i]);
    }

    printf("block,n,reps,time_ms,gflops,bandwidth_GBps,check\n");

    for (int bi = 0; bi < nb; bi++) {
        int BS = Bs[bi];
        if (BS <= 0) continue;

        init_mats(A, B, C, n);

        double t0 = now_sec();
        for (int r = 0; r < reps; r++) mxm_blocked(A, B, C, n, BS);
        double t1 = now_sec();
        double sec = t1 - t0;

        double flops = (double)reps * 2.0 * (double)n * (double)n * (double)n;
        double gflops = flops / sec / 1e9;

        
        double bytes_useful = (double)reps * 4.0 * (double)n * (double)n * (double)sizeof(double);
        double bw = bytes_useful / sec / 1e9;

        printf("%d,%d,%d,%.3f,%.3f,%.3f,%.6e\n",
               BS, n, reps, sec * 1000.0, gflops, bw, checksum_sparse(C, n));
    }

    if (argc >= 4) free(Bs);
    free(A); free(B); free(C);
    return 0;
}
