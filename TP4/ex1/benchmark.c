#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000
#define NUM_RUNS 5  // Multiple runs for averaging

typedef struct {
    double sum;
    double max;
    double stddev;
    double time;
} Result;

Result run_serial(double *A) {
    Result r = {0};
    double t0 = omp_get_wtime();
    
    // Compute sum and max
    double sum = 0.0;
    double max = A[0];
    for (int i = 0; i < N; i++) {
        sum += A[i];
        if (A[i] > max) max = A[i];
    }
    
    // Compute mean
    double mean = sum / N;
    
    // Compute standard deviation
    double stddev = 0.0;
    for (int i = 0; i < N; i++)
        stddev += (A[i] - mean) * (A[i] - mean);
    stddev = sqrt(stddev / N);
    
    double t1 = omp_get_wtime();
    
    r.sum = sum;
    r.max = max;
    r.stddev = stddev;
    r.time = t1 - t0;
    return r;
}

Result run_parallel(double *A, int num_threads) {
    Result r = {0};
    omp_set_num_threads(num_threads);
    
    double sum = 0.0, mean = 0.0, stddev = 0.0, max = 0.0;
    int mean_ready = 0;
    max = A[0];
    
    double t0 = omp_get_wtime();
    
    #pragma omp parallel shared(A, sum, mean, stddev, max, mean_ready)
    {
        #pragma omp sections
        {
            // Section 1: sum and mean
            #pragma omp section
            {
                double local_sum = 0.0;
                for (int i = 0; i < N; i++) {
                    local_sum += A[i];
                }
                sum = local_sum;
                mean = sum / N;
                #pragma omp atomic write
                mean_ready = 1;
                #pragma omp flush(mean_ready, mean)
            }
            
            // Section 2: max
            #pragma omp section
            {
                double local_max = A[0];
                for (int i = 1; i < N; i++) {
                    if (A[i] > local_max) local_max = A[i];
                }
                max = local_max;
            }
            
            // Section 3: stddev
            #pragma omp section
            {
                int ready = 0;
                while (!ready) {
                    #pragma omp flush(mean_ready, mean)
                    ready = mean_ready;
                }
                double local_var = 0.0;
                for (int i = 0; i < N; i++) {
                    double d = A[i] - mean;
                    local_var += d * d;
                }
                stddev = sqrt(local_var / N);
            }
        }
    }
    
    double t1 = omp_get_wtime();
    
    r.sum = sum;
    r.max = max;
    r.stddev = stddev;
    r.time = t1 - t0;
    return r;
}

int main(void) {
    // Allocate and initialize array
    double *A = (double *)malloc((size_t)N * sizeof(double));
    if (!A) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    srand(0);
    for (int i = 0; i < N; i++) {
        A[i] = (double)rand() / RAND_MAX;
    }
    
    // Thread counts to test
    int thread_counts[] = {1, 2, 3, 4, 8};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);
    
    // Run serial version
    double serial_time = 0.0;
    Result serial_result;
    for (int run = 0; run < NUM_RUNS; run++) {
        serial_result = run_serial(A);
        serial_time += serial_result.time;
    }
    serial_time /= NUM_RUNS;
    
    // Print CSV header
    printf("threads,time,speedup,efficiency,sum,max,stddev\n");
    
    // Print serial result (threads=0 to indicate serial)
    printf("0,%.6f,1.000,1.000,%.6f,%.6f,%.6f\n",
           serial_time, serial_result.sum, serial_result.max, serial_result.stddev);
    
    // Run parallel versions
    for (int i = 0; i < num_configs; i++) {
        int threads = thread_counts[i];
        double total_time = 0.0;
        Result result;
        
        for (int run = 0; run < NUM_RUNS; run++) {
            result = run_parallel(A, threads);
            total_time += result.time;
        }
        total_time /= NUM_RUNS;
        
        double speedup = serial_time / total_time;
        double efficiency = speedup / threads;
        
        printf("%d,%.6f,%.3f,%.3f,%.6f,%.6f,%.6f\n",
               threads, total_time, speedup, efficiency,
               result.sum, result.max, result.stddev);
    }
    
    free(A);
    return 0;
}




