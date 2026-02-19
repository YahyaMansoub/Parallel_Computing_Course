#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifndef N
#define N 1000
#endif

#define NUM_RUNS 5

static void init_matrix(int n, double *A) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i*n + j] = (double)(i + j);
}

static double sum_matrix(int n, double *A) {
    double sum = 0.0;
    for (int idx = 0; idx < n*n; idx++)
        sum += A[idx];
    return sum;
}

typedef struct {
    double init_time;
    double sum_time;
    double total_time;
    double sum_value;
} Result;

Result run_serial(int n, double *A) {
    Result r = {0};
    
    double t0 = omp_get_wtime();
    
    // Init
    double t_init_start = omp_get_wtime();
    init_matrix(n, A);
    double t_init_end = omp_get_wtime();
    
    // Sum
    double t_sum_start = omp_get_wtime();
    double sum = sum_matrix(n, A);
    double t_sum_end = omp_get_wtime();
    
    double t1 = omp_get_wtime();
    
    r.init_time = t_init_end - t_init_start;
    r.sum_time = t_sum_end - t_sum_start;
    r.total_time = t1 - t0;
    r.sum_value = sum;
    return r;
}

Result run_parallel(int n, double *A, int num_threads) {
    Result r = {0};
    omp_set_num_threads(num_threads);
    
    double t0 = omp_get_wtime();
    double sum = 0.0;
    
    double t_init_start = omp_get_wtime();
    #pragma omp parallel shared(A)
    {
        #pragma omp master
        {
            init_matrix(n, A);
        }
        #pragma omp barrier
    }
    double t_init_end = omp_get_wtime();
    
    double t_sum_start = omp_get_wtime();
    #pragma omp parallel shared(A, sum)
    {
        #pragma omp for reduction(+:sum)
        for (int idx = 0; idx < n*n; idx++) {
            sum += A[idx];
        }
    }
    double t_sum_end = omp_get_wtime();
    
    double t1 = omp_get_wtime();
    
    r.init_time = t_init_end - t_init_start;
    r.sum_time = t_sum_end - t_sum_start;
    r.total_time = t1 - t0;
    r.sum_value = sum;
    return r;
}

int main(void) {
    int n = N;
    double *A = (double*)malloc((size_t)n * (size_t)n * sizeof(double));
    if (!A) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    
    // Thread counts to test
    int thread_counts[] = {1, 2, 4, 8};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);
    
    // Run serial version
    double serial_time = 0.0, serial_init = 0.0, serial_sum = 0.0;
    Result serial_result;
    for (int run = 0; run < NUM_RUNS; run++) {
        serial_result = run_serial(n, A);
        serial_time += serial_result.total_time;
        serial_init += serial_result.init_time;
        serial_sum += serial_result.sum_time;
    }
    serial_time /= NUM_RUNS;
    serial_init /= NUM_RUNS;
    serial_sum /= NUM_RUNS;
    
    // Print CSV header
    printf("threads,total_time,init_time,sum_time,speedup,efficiency,sum_value\n");
    
    // Print serial result
    printf("0,%.6f,%.6f,%.6f,1.000,1.000,%.1f\n",
           serial_time, serial_init, serial_sum, serial_result.sum_value);
    
    // Run parallel versions
    for (int i = 0; i < num_configs; i++) {
        int threads = thread_counts[i];
        double total_time = 0.0, init_time = 0.0, sum_time = 0.0;
        Result result;
        
        for (int run = 0; run < NUM_RUNS; run++) {
            result = run_parallel(n, A, threads);
            total_time += result.total_time;
            init_time += result.init_time;
            sum_time += result.sum_time;
        }
        total_time /= NUM_RUNS;
        init_time /= NUM_RUNS;
        sum_time /= NUM_RUNS;
        
        double speedup = serial_time / total_time;
        double efficiency = speedup / threads;
        
        printf("%d,%.6f,%.6f,%.6f,%.3f,%.3f,%.1f\n",
               threads, total_time, init_time, sum_time,
               speedup, efficiency, result.sum_value);
    }
    
    free(A);
    return 0;
}
