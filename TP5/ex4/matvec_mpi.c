#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void matrixVectorLocal(const double* A_local, const double* b,
                              double* x_local, int local_rows, int N) {
    for (int i = 0; i < local_rows; ++i) {
        double sum = 0.0;
        const double* row = &A_local[i * N];
        for (int j = 0; j < N; ++j) sum += row[j] * b[j];
        x_local[i] = sum;
    }
}

static void build_counts_displs(int N, int P, int* rows, int* row_displs,
                                int* A_counts, int* A_displs,
                                int* x_counts, int* x_displs) {
    int base = N / P;
    int rem  = N % P;

    int offset = 0;
    for (int r = 0; r < P; ++r) {
        rows[r] = base + (r < rem ? 1 : 0);
        row_displs[r] = offset;
        offset += rows[r];
    }

    for (int r = 0; r < P; ++r) {
        x_counts[r] = rows[r];
        x_displs[r] = row_displs[r];
        A_counts[r] = rows[r] * N;
        A_displs[r] = row_displs[r] * N;
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (argc != 2) {
        if (rank == 0) printf("Usage: %s <matrix_size>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    int N = atoi(argv[1]);
    if (N <= 0) {
        if (rank == 0) printf("Matrix size must be positive.\n");
        MPI_Finalize();
        return 1;
    }

    // --- Build distribution metadata ---
    int* rows      = (int*)malloc(P * sizeof(int));
    int* row_disp  = (int*)malloc(P * sizeof(int));
    int* A_counts  = (int*)malloc(P * sizeof(int));
    int* A_displs  = (int*)malloc(P * sizeof(int));
    int* x_counts  = (int*)malloc(P * sizeof(int));
    int* x_displs  = (int*)malloc(P * sizeof(int));

    build_counts_displs(N, P, rows, row_disp, A_counts, A_displs, x_counts, x_displs);

    int local_rows = rows[rank];

    // --- Allocate local buffers ---
    double* A_local = (double*)malloc((size_t)local_rows * (size_t)N * sizeof(double));
    double* b       = (double*)malloc((size_t)N * sizeof(double));
    double* x_local = (double*)malloc((size_t)local_rows * sizeof(double));

    // --- Rank 0 owns full A for initialization + optional correctness ---
    double* A = NULL;
    double* x_serial = NULL;
    double* x_parallel = NULL;

    if (rank == 0) {
        A = (double*)malloc((size_t)N * (size_t)N * sizeof(double));
        x_serial = (double*)malloc((size_t)N * sizeof(double));
        x_parallel = (double*)malloc((size_t)N * sizeof(double));
        if (!A || !x_serial || !x_parallel) {
            printf("Memory allocation failed on rank 0.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        srand48(42);

        // Minimal init similar to your snippet (note: uninitialized entries become garbage if not set)
        // Better: initialize all A to 0 first for deterministic behavior.
        for (int i = 0; i < N*N; ++i) A[i] = 0.0;

        int limit = (N < 100) ? N : 100;
        for (int j = 0; j < limit; ++j) A[0 * N + j] = drand48();

        if (N > 1 && N > 100) {
            int copy_len = (N - 100 < 100) ? (N - 100) : 100;
            for (int j = 0; j < copy_len; ++j)
                A[1 * N + (100 + j)] = A[0 * N + j];
        }

        for (int i = 0; i < N; ++i) A[i * N + i] = drand48();

        for (int i = 0; i < N; ++i) b[i] = drand48();

        // serial reference (on rank 0)
        for (int i = 0; i < N; ++i) {
            double sum = 0.0;
            for (int j = 0; j < N; ++j) sum += A[i * N + j] * b[j];
            x_serial[i] = sum;
        }
    }

    // Everyone needs b
    MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of A (uneven)
    MPI_Scatterv(A, A_counts, A_displs, MPI_DOUBLE,
                 A_local, local_rows * N, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // Local compute
    matrixVectorLocal(A_local, b, x_local, local_rows, N);

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();
    double local_time = t1 - t0;

    // Gather x
    MPI_Gatherv(x_local, local_rows, MPI_DOUBLE,
                x_parallel, x_counts, x_displs, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Timing summary: take max time across ranks (true parallel time)
    double tmax;
    MPI_Reduce(&local_time, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Correctness check
        double max_err = 0.0;
        for (int i = 0; i < N; ++i) {
            double diff = fabs(x_parallel[i] - x_serial[i]);
            if (diff > max_err) max_err = diff;
        }
        printf("N=%d P=%d time=%f s max_err=%e\n", N, P, tmax, max_err);
    }

    // Cleanup
    free(A_local); free(b); free(x_local);
    free(rows); free(row_disp); free(A_counts); free(A_displs); free(x_counts); free(x_displs);

    if (rank == 0) { free(A); free(x_serial); free(x_parallel); }

    MPI_Finalize();
    return 0;
}