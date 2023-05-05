#include <iostream>
#include <mpi.h>
#include <chrono>

using namespace std;

int main(int argc, char* argv[])
{
    int rank, size, i, j, k, chunk_size, n;
    double *A, *B, *C, *tmp;

    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the total number of processes

    // Only the root process reads the size of the matrices
    if (rank == 0) {
        cout << "Enter the size of the matrices: ";
        cin >> n;
    }

    // Broadcast the size of the matrices to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide work among nodes
    chunk_size = n / size;

    // Allocate memory for the matrices on all processes
    A = new double[n * chunk_size];
    B = new double[n * n];
    C = new double[n * chunk_size];
    tmp = new double[n * chunk_size];

    // Only the root process initializes the matrices
    if (rank == 0) {
        // Initialize matrices A and B with random values
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                A[i * n + j] = rand() % 10;
                B[i * n + j] = rand() % 10;
            }
        }
    }

    // Scatter the rows of matrix A to all nodes
    MPI_Scatter(A, chunk_size * n, MPI_DOUBLE, tmp, chunk_size * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast matrix B to all nodes
    MPI_Bcast(B, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Start the timer
    auto start_time = chrono::high_resolution_clock::now();

    // Compute the partial matrix multiplication
    for (i = 0; i < chunk_size; i++) {
        for (j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (k = 0; k < n; k++) {
                C[i * n + j] += tmp[i * n + k] * B[k * n + j];
            }
        }
    }

    // Gather the results from all nodes
    MPI_Gather(C, chunk_size * n, MPI_DOUBLE, C, chunk_size * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Stop the timer
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    // Only the root process prints the result and execution time
    if (rank == 0) {
        cout << "Result:" << endl;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                cout << C[i * n + j] << " ";
            }
            cout << endl;
        }
        cout << "Execution time: " << duration << " milliseconds" << endl;
    }

    // Free the memory
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] tmp;

    MPI_Finalize(); // Finalize
   return 0;
}
