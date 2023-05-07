// Name: Saatvik

#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void quicksort(vector<int>& arr, int left, int right) {
    if (left < right) {
        // Choose a pivot element
        int pivot = arr[(left + right) / 2];
        // Partition the array around the pivot
        int i = left - 1;
        int j = right + 1;
        while (true) {
            do { i++; } while (arr[i] < pivot);
            do { j--; } while (arr[j] > pivot);
            if (i >= j) break;
            swap(arr[i], arr[j]);
        }
        // Recursively sort the subarrays
        quicksort(arr, left, j);
        quicksort(arr, j + 1, right);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 20;
    const int chunkSize = n / size;

    vector<int> arr(n);
    if (rank == 0) {
        // Generate random numbers for the input array
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100;
        }
        cout << "Unsorted array: ";
        for (int i = 0; i < n; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    // Scatter the input array to all processes
    vector<int> localArr(chunkSize);
    MPI_Scatter(arr.data(), chunkSize, MPI_INT, localArr.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort the local chunk of the input array
    sort(localArr.begin(), localArr.end());

    // Gather the sorted chunks from all processes to the root process
    MPI_Gather(localArr.data(), chunkSize, MPI_INT, arr.data(), chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Merge the sorted chunks using quicksort
        double startTime = MPI_Wtime();
        quicksort(arr, 0, n - 1);

        // Output the sorted array and the execution time
        double endTime = MPI_Wtime();
        double elapsedTime = endTime - startTime;

        cout << "Sorted array: ";
        for (int i = 0; i < n; i++) {
            cout << arr[i] << " ";
        }
        cout << endl << flush;

        cout << "Execution time: " << elapsedTime << " seconds" << endl << flush;
    }

    MPI_Finalize();
    return 0;
}
