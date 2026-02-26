
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef N_FEATURES
#define N_FEATURES 5
#endif

typedef struct {
    double x[N_FEATURES];
    double y;
} Sample;

static double drand_u01(void) {
    // drand48() is POSIX; if unavailable, replace with rand()/RAND_MAX.
    return drand48();
}

static void generate_data(Sample *data, int n_samples, unsigned long seed) {
    srand48((long)seed);

    // True weights (including bias at w_true[0])
    const double w_true[N_FEATURES] = { 1.5, -0.9, 0.6, 0.2, -0.3 };

    for (int i = 0; i < n_samples; ++i) {
        data[i].x[0] = 1.0; // bias feature
        for (int j = 1; j < N_FEATURES; ++j) {
            // Features in [-1, 1]
            data[i].x[j] = 2.0 * drand_u01() - 1.0;
        }

        double y = 0.0;
        for (int j = 0; j < N_FEATURES; ++j) y += w_true[j] * data[i].x[j];

        // small noise
        double noise = 0.05 * (2.0 * drand_u01() - 1.0);
        data[i].y = y + noise;
    }
}

static void build_counts_displs(int N, int P, int *counts, int *displs) {
    int base = N / P;
    int rem  = N % P;

    int off = 0;
    for (int r = 0; r < P; ++r) {
        counts[r] = base + (r < rem ? 1 : 0);
        displs[r] = off;
        off += counts[r];
    }
}

static MPI_Datatype create_sample_type(void) {
    MPI_Datatype sample_type;

    Sample s;
    MPI_Aint base, addr_x, addr_y;
    MPI_Get_address(&s, &base);
    MPI_Get_address(&s.x[0], &addr_x);
    MPI_Get_address(&s.y, &addr_y);

    int blocklen[2] = { N_FEATURES, 1 };
    MPI_Aint disp[2];
    disp[0] = addr_x - base;
    disp[1] = addr_y - base;

    MPI_Datatype types[2] = { MPI_DOUBLE, MPI_DOUBLE };

    MPI_Type_create_struct(2, blocklen, disp, types, &sample_type);
    MPI_Type_commit(&sample_type);
    return sample_type;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    int N = (argc >= 2) ? atoi(argv[1]) : 200000;  // samples
    int max_epochs = (argc >= 3) ? atoi(argv[2]) : 1000;
    double lr = (argc >= 4) ? atof(argv[3]) : 0.05;
    double thresh = (argc >= 5) ? atof(argv[4]) : 1e-2;

    if (N <= 0 || max_epochs <= 0 || lr <= 0.0 || thresh <= 0.0) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s N_SAMPLES [MAX_EPOCHS] [LR] [THRESH]\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    MPI_Datatype sample_type = create_sample_type();

    // Root creates full dataset
    Sample *all = NULL;
    int *counts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        all = (Sample*)malloc((size_t)N * sizeof(Sample));
        if (!all) {
            fprintf(stderr, "Rank0: malloc failed for %d samples\n", N);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        generate_data(all, N, 1234ul);

        counts = (int*)malloc((size_t)P * sizeof(int));
        displs = (int*)malloc((size_t)P * sizeof(int));
        if (!counts || !displs) {
            fprintf(stderr, "Rank0: malloc failed for counts/displs\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        build_counts_displs(N, P, counts, displs);
    }

    // Broadcast local_n info
    int local_n = 0;
    if (rank == 0) local_n = counts[0];
    MPI_Scatter(counts, 1, MPI_INT, &local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate local buffer (allocate at least 1 element for MPI friendliness)
    int alloc_n = (local_n > 0) ? local_n : 1;
    Sample *local = (Sample*)malloc((size_t)alloc_n * sizeof(Sample));
    if (!local) {
        fprintf(stderr, "Rank %d: malloc failed for local samples\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Scatterv dataset using derived type
    MPI_Scatterv(all, counts, displs, sample_type,
                 local, local_n, sample_type,
                 0, MPI_COMM_WORLD);

    if (rank == 0) {
        free(all);
        free(counts);
        free(displs);
        all = NULL; counts = NULL; displs = NULL;
    }

    // Initialize weights on all ranks
    double w[N_FEATURES];
    for (int j = 0; j < N_FEATURES; ++j) w[j] = 0.0;

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    double global_mse = 0.0;
    int stop_epoch = -1;

    for (int epoch = 1; epoch <= max_epochs; ++epoch) {
        // Local accumulators
        double local_grad[N_FEATURES];
        for (int j = 0; j < N_FEATURES; ++j) local_grad[j] = 0.0;

        double local_loss_sum = 0.0; // sum of squared errors

        for (int i = 0; i < local_n; ++i) {
            double pred = 0.0;
            for (int j = 0; j < N_FEATURES; ++j) pred += w[j] * local[i].x[j];
            double err = pred - local[i].y;

            local_loss_sum += err * err;
            for (int j = 0; j < N_FEATURES; ++j) {
                local_grad[j] += err * local[i].x[j]; // accumulate X^T * err
            }
        }

        // Reduce to global sums
        double global_grad[N_FEATURES];
        for (int j = 0; j < N_FEATURES; ++j) global_grad[j] = 0.0;

        double global_loss_sum = 0.0;
        MPI_Allreduce(local_grad, global_grad, N_FEATURES, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&local_loss_sum, &global_loss_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        // MSE over full dataset
        global_mse = global_loss_sum / (double)N;

        // Gradient for MSE: (2/N) * X^T * err
        double scale = 2.0 / (double)N;
        for (int j = 0; j < N_FEATURES; ++j) {
            w[j] -= lr * (scale * global_grad[j]);
        }

        if (rank == 0 && (epoch % 10 == 0 || epoch == 1)) {
            printf("Epoch %d | Loss (MSE): %.6f | w[0]: %.4f, w[1]: %.4f\n",
                   epoch, global_mse, w[0], w[1]);
            fflush(stdout);
        }

        if (global_mse < thresh) {
            stop_epoch = epoch;
            break;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    if (rank == 0) {
        if (stop_epoch > 0) {
            printf("Early stopping at epoch %d — loss %.6f < %.1e\n", stop_epoch, global_mse, thresh);
        } else {
            printf("Finished %d epochs — final loss %.6f\n", max_epochs, global_mse);
        }
        printf("Training time: %.3f seconds (MPI)\n", (t1 - t0));
    }

    free(local);

    MPI_Type_free(&sample_type);
    MPI_Finalize();
    return 0;
}
