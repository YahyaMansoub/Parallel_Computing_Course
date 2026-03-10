#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IDX(i, j, ld) ((i) * (ld) + (j))

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s NX NY G PATTERN [PRINT_FINAL]\n"
        "  NX, NY       global grid size\n"
        "  G            number of generations\n"
        "  PATTERN      block | blinker | glider | single | random | empty\n"
        "  PRINT_FINAL  1 to print final global grid on rank 0, 0 otherwise\n",
        prog);
}

static unsigned char cell_from_pattern(int gi, int gj, int NX, int NY, const char *pattern) {
    if (strcmp(pattern, "empty") == 0) return 0;

    if (strcmp(pattern, "single") == 0) {
        int cx = NX / 2, cy = NY / 2;
        return (gi == cx && gj == cy) ? 1 : 0;
    }

    if (strcmp(pattern, "block") == 0) {
        int x = NX / 2 - 1;
        int y = NY / 2 - 1;
        return ((gi == x || gi == x + 1) && (gj == y || gj == y + 1)) ? 1 : 0;
    }

    if (strcmp(pattern, "blinker") == 0) {
        int x = NX / 2;
        int y = NY / 2 - 1;
        return (gi == x && (gj == y || gj == y + 1 || gj == y + 2)) ? 1 : 0;
    }

    if (strcmp(pattern, "glider") == 0) {
        int x = 1, y = 1;
        if ((gi == x     && gj == y + 1) ||
            (gi == x + 1 && gj == y + 2) ||
            (gi == x + 2 && gj == y    ) ||
            (gi == x + 2 && gj == y + 1) ||
            (gi == x + 2 && gj == y + 2))
            return 1;
        return 0;
    }

    if (strcmp(pattern, "random") == 0) {
        unsigned int h = (unsigned int)(gi * 1103515245u + gj * 12345u + 0x9e3779b9u);
        return (h % 5 == 0) ? 1 : 0;
    }

    return 0;
}

static void init_local_grid(unsigned char *grid, int local_nx, int local_ny,
                            int global_row0, int global_col0,
                            int NX, int NY, const char *pattern) {
    int ld = local_ny + 2;
    for (int i = 1; i <= local_nx; i++) {
        for (int j = 1; j <= local_ny; j++) {
            int gi = global_row0 + (i - 1);
            int gj = global_col0 + (j - 1);
            grid[IDX(i, j, ld)] = cell_from_pattern(gi, gj, NX, NY, pattern);
        }
    }
}

static int count_neighbors(const unsigned char *grid, int i, int j, int ld) {
    return grid[IDX(i - 1, j - 1, ld)] + grid[IDX(i - 1, j, ld)] + grid[IDX(i - 1, j + 1, ld)] +
           grid[IDX(i,     j - 1, ld)]                             + grid[IDX(i,     j + 1, ld)] +
           grid[IDX(i + 1, j - 1, ld)] + grid[IDX(i + 1, j, ld)] + grid[IDX(i + 1, j + 1, ld)];
}

static void step_game(const unsigned char *cur, unsigned char *next, int local_nx, int local_ny) {
    int ld = local_ny + 2;
    for (int i = 1; i <= local_nx; i++) {
        for (int j = 1; j <= local_ny; j++) {
            int n = count_neighbors(cur, i, j, ld);
            unsigned char alive = cur[IDX(i, j, ld)];
            if (alive) {
                next[IDX(i, j, ld)] = (n == 2 || n == 3) ? 1 : 0;
            } else {
                next[IDX(i, j, ld)] = (n == 3) ? 1 : 0;
            }
        }
    }
}

static void pack_interior(const unsigned char *grid, unsigned char *buf, int local_nx, int local_ny) {
    int ld = local_ny + 2;
    for (int i = 0; i < local_nx; i++) {
        memcpy(buf + i * local_ny, &grid[IDX(i + 1, 1, ld)], (size_t)local_ny * sizeof(unsigned char));
    }
}

static void reconstruct_and_print(MPI_Comm cart_comm,
                                  const unsigned char *local_grid,
                                  int local_nx, int local_ny,
                                  int NX, int NY,
                                  int generation) {
    int size, rank;
    MPI_Comm_size(cart_comm, &size);
    MPI_Comm_rank(cart_comm, &rank);

    int block_size = local_nx * local_ny;
    unsigned char *sendbuf = (unsigned char *)malloc((size_t)block_size * sizeof(unsigned char));
    pack_interior(local_grid, sendbuf, local_nx, local_ny);

    unsigned char *recvbuf = NULL;
    unsigned char *global  = NULL;
    if (rank == 0) {
        recvbuf = (unsigned char *)malloc((size_t)size * block_size * sizeof(unsigned char));
        global  = (unsigned char *)calloc((size_t)NX * NY, sizeof(unsigned char));
    }

    MPI_Gather(sendbuf, block_size, MPI_UNSIGNED_CHAR,
               recvbuf, block_size, MPI_UNSIGNED_CHAR,
               0, cart_comm);

    if (rank == 0) {
        for (int p = 0; p < size; p++) {
            int coords[2];
            MPI_Cart_coords(cart_comm, p, 2, coords);
            int row0 = coords[0] * local_nx;
            int col0 = coords[1] * local_ny;

            const unsigned char *block = recvbuf + p * block_size;
            for (int i = 0; i < local_nx; i++) {
                for (int j = 0; j < local_ny; j++) {
                    global[(row0 + i) * NY + (col0 + j)] = block[i * local_ny + j];
                }
            }
        }

        printf("Rank 0 - Generation %d:\n", generation);
        for (int i = 0; i < NX; i++) {
            for (int j = 0; j < NY; j++) {
                printf("%d ", global[i * NY + j]);
            }
            printf("\n");
        }
        printf("\n");

        free(recvbuf);
        free(global);
    }

    free(sendbuf);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (argc < 5) {
        if (world_rank == 0) usage(argv[0]);
        MPI_Finalize();
        return 1;
    }

    int NX = atoi(argv[1]);
    int NY = atoi(argv[2]);
    int G  = atoi(argv[3]);
    const char *pattern = argv[4];
    int print_final = (argc >= 6) ? atoi(argv[5]) : 1;

    int dims[2] = {0, 0};
    MPI_Dims_create(world_size, 2, dims);

    int periods[2] = {1, 1};
    int reorder = 0;
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cart_comm);

    int rank;
    MPI_Comm_rank(cart_comm, &rank);

    int coords[2];
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    if (NX % dims[0] != 0 || NY % dims[1] != 0) {
        if (rank == 0) {
            fprintf(stderr,
                    "Error: NX and NY must be divisible by process grid dims.\n"
                    "Got NX=%d NY=%d, dims=(%d,%d)\n",
                    NX, NY, dims[0], dims[1]);
        }
        MPI_Comm_free(&cart_comm);
        MPI_Finalize();
        return 1;
    }

    int local_nx = NX / dims[0];
    int local_ny = NY / dims[1];
    int ld = local_ny + 2;

    unsigned char *cur  = (unsigned char *)calloc((size_t)(local_nx + 2) * (local_ny + 2), sizeof(unsigned char));
    unsigned char *next = (unsigned char *)calloc((size_t)(local_nx + 2) * (local_ny + 2), sizeof(unsigned char));
    if (!cur || !next) {
        fprintf(stderr, "Rank %d: allocation failed\n", rank);
        MPI_Abort(cart_comm, 1);
    }

    int global_row0 = coords[0] * local_nx;
    int global_col0 = coords[1] * local_ny;
    init_local_grid(cur, local_nx, local_ny, global_row0, global_col0, NX, NY, pattern);

    int north, south, west, east;
    MPI_Cart_shift(cart_comm, 0, 1, &north, &south);
    MPI_Cart_shift(cart_comm, 1, 1, &west,  &east);

    MPI_Datatype col_type;
    MPI_Type_vector(local_nx + 2, 1, ld, MPI_UNSIGNED_CHAR, &col_type);
    MPI_Type_commit(&col_type);

    for (int gen = 1; gen <= G; gen++) {
        /* exchange top/bottom rows */
        MPI_Sendrecv(&cur[IDX(1,        1, ld)], local_ny, MPI_UNSIGNED_CHAR, north, 100,
                     &cur[IDX(local_nx+1,1, ld)], local_ny, MPI_UNSIGNED_CHAR, south, 100,
                     cart_comm, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&cur[IDX(local_nx, 1, ld)], local_ny, MPI_UNSIGNED_CHAR, south, 101,
                     &cur[IDX(0,        1, ld)], local_ny, MPI_UNSIGNED_CHAR, north, 101,
                     cart_comm, MPI_STATUS_IGNORE);

        /* exchange left/right columns, including ghost rows so corners are propagated */
        MPI_Sendrecv(&cur[IDX(0, 1,         ld)], 1, col_type, west,  200,
                     &cur[IDX(0, local_ny+1,ld)], 1, col_type, east,  200,
                     cart_comm, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&cur[IDX(0, local_ny,  ld)], 1, col_type, east,  201,
                     &cur[IDX(0, 0,         ld)], 1, col_type, west,  201,
                     cart_comm, MPI_STATUS_IGNORE);

        step_game(cur, next, local_nx, local_ny);

        unsigned char *tmp = cur;
        cur = next;
        next = tmp;
    }

    int local_alive = 0, global_alive = 0;
    for (int i = 1; i <= local_nx; i++) {
        for (int j = 1; j <= local_ny; j++) {
            local_alive += cur[IDX(i, j, ld)];
        }
    }
    MPI_Reduce(&local_alive, &global_alive, 1, MPI_INT, MPI_SUM, 0, cart_comm);

    if (rank == 0) {
        printf("Process grid: %d x %d\n", dims[0], dims[1]);
        printf("Global alive cells after %d generations: %d\n\n", G, global_alive);
    }

    if (print_final) {
        reconstruct_and_print(cart_comm, cur, local_nx, local_ny, NX, NY, G);
    }

    MPI_Type_free(&col_type);
    free(cur);
    free(next);
    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}