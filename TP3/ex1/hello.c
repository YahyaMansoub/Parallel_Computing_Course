#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = 4; // I've tried different combinations here 

        printf("Hello from the rank %d thread\n", tid);

        #pragma omp single
        printf("Parallel execution of hello_world with %d threads\n", nthreads);
    }
    return 0;
}
