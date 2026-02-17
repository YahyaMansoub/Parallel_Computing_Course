
#include <stdio.h>
#include <math.h>
#include <omp.h>

#ifndef N
#define N 2000000
#endif

static double task_light(int N) {
    double x = 0.0;
    for (int i = 0; i < N; i++) x += sin(i * 0.001);
    return x;
}

static double task_moderate(int N) {
    double x = 0.0;
    for (int i = 0; i < 5*N; i++) x += sqrt(i * 0.5) * cos(i * 0.001);
    return x;
}

// heavy split into chunks for better load balance
static double task_heavy_range(int start, int end) {
    double x = 0.0;
    for (int i = start; i < end; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    }
    return x;
}

int main(void) {
    double A=0.0, B=0.0, C=0.0;

    // -------------------------
    // 1) Naive: 3 sections
    // -------------------------
    double t0 = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            { A = task_light(N); }

            #pragma omp section
            { B = task_moderate(N); }

            #pragma omp section
            { C = task_heavy_range(0, 20*N); }
        }
    }
    double t1 = omp_get_wtime();
    printf("[Naive sections]  A=%f B=%f C=%f\n", A, B, C);
    printf("[Naive sections]  Time = %f s\n\n", t1 - t0);

    // -------------------------
    // 2) Optimized: split heavy
    //    (works best if you run with 4 threads)
    // -------------------------
    double C1=0.0, C2=0.0, C3=0.0, Csum=0.0;
    int H = 20*N;
    int p1 = H/3, p2 = 2*H/3;

    t0 = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp sections
        {
            // pack light+moderate together (so they don't waste a whole thread)
            #pragma omp section
            { A = task_light(N) + task_moderate(N); }

            // split heavy into 3 independent chunks
            #pragma omp section
            { C1 = task_heavy_range(0, p1); }

            #pragma omp section
            { C2 = task_heavy_range(p1, p2); }

            #pragma omp section
            { C3 = task_heavy_range(p2, H); }
        }
    }
    Csum = C1 + C2 + C3;
    t1 = omp_get_wtime();

    printf("[Optimized sections] A(=light+moderate)=%f  C=%f\n", A, Csum);
    printf("[Optimized sections] Time = %f s\n", t1 - t0);

    return 0;
}
