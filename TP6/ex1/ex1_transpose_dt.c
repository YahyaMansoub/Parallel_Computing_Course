
#include <mpi.h>
#include <stdio.h>

static void print_a(int a[4][5]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) printf("%d ", a[i][j]);
        printf("\n");
    }
}
static void print_at(int at[5][4]) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) printf("%d ", at[i][j]);
        printf("\n");
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (P < 2) {
        if (rank == 0) fprintf(stderr, "Run with at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        int a[4][5];
        int v = 1;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 5; ++j)
                a[i][j] = v++;

        printf("Process 0 - Matrix a:\n");
        print_a(a);

        // Step 6: regular send (contiguous 20 ints)
        MPI_Send(&a[0][0], 20, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }

    if (rank == 1) {
        int at[5][4] = {0};

        // Step 4+5: build + commit derived type
        MPI_Datatype col_type, transpose_type;

        // One column of at (5 elements), stride 4 ints between rows
        MPI_Type_vector(5, 1, 4, MPI_INT, &col_type);

        // 4 columns, starting points separated by sizeof(int) bytes
        MPI_Type_create_hvector(4, 1, (MPI_Aint)sizeof(int), col_type, &transpose_type);

        MPI_Type_commit(&transpose_type);

        // Step 5: single recv that writes transpose directly into at
        MPI_Recv(&at[0][0], 1, transpose_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Step 7: display received transpose
        printf("Process 1 - Matrix transposee at:\n");
        print_at(at);

        MPI_Type_free(&transpose_type);
        MPI_Type_free(&col_type);
    }

    MPI_Finalize();
    return 0;
}