#include <stdio.h>
#include <math.h>
#include <omp.h>

#ifndef N
#define N 2000000
#endif

#define NUM_RUNS 3

static double task_light(int n) {
    double x = 0.0;
    for (int i = 0; i < n; i++) x += sin(i * 0.001);
    return x;
}

static double task_moderate(int n) {
    double x = 0.0;
    for (int i = 0; i < 5*n; i++) x += sqrt(i * 0.5) * cos(i * 0.001);
    return x;
}

static double task_heavy_range(int start, int end) {
    double x = 0.0;
    for (int i = start; i < end; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    }
    return x;
}

typedef struct {
    double time;
    double result_sum;
} Result;

Result run_serial(int n) {
    Result r = {0};
    double t0 = omp_get_wtime();
    
    double A = task_light(n);
    double B = task_moderate(n);
    double C = task_heavy_range(0, 20*n);
    
    double t1 = omp_get_wtime();
    r.time = t1 - t0;
    r.result_sum = A + B + C;
    return r;
}

Result run_naive(int n, int num_threads) {
    Result r = {0};
    omp_set_num_threads(num_threads);
    
    double A = 0.0, B = 0.0, C = 0.0;
    
    double t0 = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            { A = task_light(n); }
            
            #pragma omp section
            { B = task_moderate(n); }
            
            #pragma omp section
            { C = task_heavy_range(0, 20*n); }
        }
    }
    double t1 = omp_get_wtime();
    
    r.time = t1 - t0;
    r.result_sum = A + B + C;
    return r;
}

Result run_optimized(int n, int num_threads) {
    Result r = {0};
    omp_set_num_threads(num_threads);
    
    double A = 0.0;
    double C1 = 0.0, C2 = 0.0, C3 = 0.0;
    int H = 20*n;
    int p1 = H/3, p2 = 2*H/3;
    
    double t0 = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            { A = task_light(n) + task_moderate(n); }
            
            #pragma omp section
            { C1 = task_heavy_range(0, p1); }
            
            #pragma omp section
            { C2 = task_heavy_range(p1, p2); }
            
            #pragma omp section
            { C3 = task_heavy_range(p2, H); }
        }
    }
    double t1 = omp_get_wtime();
    
    r.time = t1 - t0;
    r.result_sum = A + C1 + C2 + C3;
    return r;
}

int main(void) {
    int n = N;
    
    // Thread counts to test
    int thread_counts[] = {1, 2, 3, 4, 8};
    int num_configs = sizeof(thread_counts) / sizeof(thread_counts[0]);
    
    // Run serial version
    double serial_time = 0.0;
    Result serial_result;
    for (int run = 0; run < NUM_RUNS; run++) {
        serial_result = run_serial(n);
        serial_time += serial_result.time;
    }
    serial_time /= NUM_RUNS;
    
    // Print CSV header
    printf("version,threads,time,speedup,efficiency,result_sum\n");
    
    // Print serial result
    printf("serial,0,%.6f,1.000,1.000,%.6f\n",
           serial_time, serial_result.result_sum);
    
    // Run naive versions
    for (int i = 0; i < num_configs; i++) {
        int threads = thread_counts[i];
        double total_time = 0.0;
        Result result;
        
        for (int run = 0; run < NUM_RUNS; run++) {
            result = run_naive(n, threads);
            total_time += result.time;
        }
        total_time /= NUM_RUNS;
        
        double speedup = serial_time / total_time;
        double efficiency = speedup / threads;
        
        printf("naive,%d,%.6f,%.3f,%.3f,%.6f\n",
               threads, total_time, speedup, efficiency, result.result_sum);
    }
    
    // Run optimized versions
    for (int i = 0; i < num_configs; i++) {
        int threads = thread_counts[i];
        double total_time = 0.0;
        Result result;
        
        for (int run = 0; run < NUM_RUNS; run++) {
            result = run_optimized(n, threads);
            total_time += result.time;
        }
        total_time /= NUM_RUNS;
        
        double speedup = serial_time / total_time;
        double efficiency = speedup / threads;
        
        printf("optimized,%d,%.6f,%.3f,%.3f,%.6f\n",
               threads, total_time, speedup, efficiency, result.result_sum);
    }
    
    return 0;
}
