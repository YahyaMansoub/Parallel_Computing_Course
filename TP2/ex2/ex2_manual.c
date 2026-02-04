#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000

int main() {
    double a = 1.1, b = 1.2;
    double x = 0.0, y = 0.0;
    clock_t start, end;

    start = clock();
    
    // Manual optimization: compute a*b once and reuse it
    double ab = a * b;
    
    // Manual loop unrolling and instruction scheduling
    // Process 2 iterations at a time to expose ILP
    int i;
    for (i = 0; i < N - 1; i += 2) {
        x = ab + x;  // stream 1, iteration 1
        y = ab + y;  // stream 2, iteration 1 (independent, can execute in parallel)
        x = ab + x;  // stream 1, iteration 2
        y = ab + y;  // stream 2, iteration 2
    }
    
    // Handle remaining iteration if N is odd
    for (; i < N; i++) {
        x = ab + x;
        y = ab + y;
    }
    
    end = clock();

    printf("x = %f, y = %f, time = %f s\n",
           x, y, (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
