// Name: Saatvik

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000

int main(int argc, char** argv) {
    int rank, size, i, j, k;
    double start, end;
    double A[N][N], B[N][N], C[N][N];
    
    // Initialize MPI and get the rank and size of the current process
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Initialize matrices A and B on process 0
    if (rank == 0) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = (double) rand() / RAND_MAX;
                B[i][j] = (double) rand() / RAND_MAX;
            }
        }
    }
    
    // Broadcast matrices A and B to all processes
    MPI_Bcast(A, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Compute matrix multiplication
    start = MPI_Wtime();
    for (i = rank; i < N; i += size) {
        for (j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end = MPI_Wtime();
    
    // Gather results from all processes to process 0
    MPI_Gather(C + rank * N / size, N*N/size, MPI_DOUBLE, C, N*N/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Print time taken by process 0
    if (rank == 0) {
        printf("Time taken: %f seconds\n", end - start);
    }
    
    // Finalize MPI
    MPI_Finalize();
    return 0;
}
