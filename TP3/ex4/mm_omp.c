#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

static void usage(const char *p) {
    fprintf(stderr,
        "Usage: %s m n schedule chunk\n"
        "  schedule: static | dynamic | guided\n"
        "  chunk: positive int (e.g., 1, 8, 64)\n", p);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 5) usage(argv[0]);

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    const char *sched_s = argv[3];
    int chunk = atoi(argv[4]);
    if (m <= 0 || n <= 0 || chunk <= 0) usage(argv[0]);

    omp_sched_t sched;
    if (!strcmp(sched_s, "static")) sched = omp_sched_static;
    else if (!strcmp(sched_s, "dynamic")) sched = omp_sched_dynamic;
    else if (!strcmp(sched_s, "guided")) sched = omp_sched_guided;
    else usage(argv[0]);

    omp_set_schedule(sched, chunk);

    double *a = (double*)malloc((size_t)m * n * sizeof(double));
    double *b = (double*)malloc((size_t)n * m * sizeof(double));
    double *c = (double*)malloc((size_t)m * m * sizeof(double));
    if (!a || !b || !c) { fprintf(stderr, "malloc failed\n"); return 1; }

    // init (can also be parallelized, but not required)
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            a[i * n + j] = (i + 1) + (j + 1);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            b[i * m + j] = (i + 1) - (j + 1);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            c[i * m + j] = 0.0;

    double t0 = omp_get_wtime();

    // Matrix multiplication: parallelize (i,j) space
    #pragma omp parallel for collapse(2) schedule(runtime)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            double sum = 0.0;                 // private accumulator (fast)
            for (int k = 0; k < n; k++) {
                sum += a[i * n + k] * b[k * m + j];
            }
            c[i * m + j] = sum;               // single write, no race
        }
    }

    double t1 = omp_get_wtime();

    // tiny checksum to prevent “dead code” assumptions
    double chk = 0.0;
    for (int i = 0; i < m*m; i += (m*m/10 + 1)) chk += c[i];

    int threads = omp_get_max_threads();
    printf("m=%d n=%d threads=%d schedule=%s chunk=%d time=%f checksum=%f\n",
           m, n, threads, sched_s, chunk, t1 - t0, chk);

    free(a); free(b); free(c);
    return 0;
}
