// pi_mpi.c
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

static long long local_N_for_rank(long long N, int P, int r, long long *start_i) {
    long long base = N / P;
    long long rem  = N % P;
    long long nloc = base + (r < rem ? 1 : 0);

    // prefix sum for start index
    long long off = 0;
    for (int k = 0; k < r; ++k) off += base + (k < rem ? 1 : 0);
    *start_i = off;
    return nloc;
}

static double pi_serial(long long N) {
    double step = 1.0 / (double)N;
    double sum = 0.0;
    for (long long i = 0; i < N; ++i) {
        double x = (i + 0.5) * step;
        sum += 1.0 / (1.0 + x * x);
    }
    return 4.0 * step * sum;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (argc != 2) {
        if (rank == 0) fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    long long N = atoll(argv[1]);
    if (N <= 0) {
        if (rank == 0) fprintf(stderr, "N must be > 0\n");
        MPI_Finalize();
        return 1;
    }

    // --- Parallel computation ---
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long start_i = 0;
    long long nloc = local_N_for_rank(N, P, rank, &start_i);

    double step = 1.0 / (double)N;
    double local_sum = 0.0;

    for (long long k = 0; k < nloc; ++k) {
        long long i = start_i + k;
        double x = (i + 0.5) * step;
        local_sum += 1.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    double t_par = t1 - t0;

    if (rank == 0) {
        double pi_par = 4.0 * step * global_sum;

        // --- Serial computation (for speedup) ---
        double s0 = MPI_Wtime();
        double pi_ser = pi_serial(N);
        double s1 = MPI_Wtime();
        double t_ser = s1 - s0;

        double max_err = (pi_par > pi_ser) ? (pi_par - pi_ser) : (pi_ser - pi_par);
        double speedup = t_ser / t_par;
        double eff = speedup / (double)P;

        printf("N=%lld  P=%d\n", N, P);
        printf("pi_parallel = %.15f\n", pi_par);
        printf("pi_serial   = %.15f\n", pi_ser);
        printf("abs_error   = %.3e\n", max_err);
        printf("time_parallel = %.6f s\n", t_par);
        printf("time_serial   = %.6f s\n", t_ser);
        printf("speedup = %.4f\n", speedup);
        printf("efficiency = %.4f\n", eff);

        // Optional: CSV line for plotting
        // P,N,t_serial,t_parallel,speedup,eff
        printf("CSV,%d,%lld,%.6f,%.6f,%.6f,%.6f\n",
               P, N, t_ser, t_par, speedup, eff);
    }

    MPI_Finalize();
    return 0;
}