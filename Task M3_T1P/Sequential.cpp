// Name: Saatvik

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
  int size;

  // Prompt user for matrix size
  cout << "Enter matrix size: ";
  cin >> size;

  // Declare 3 matrices of size 'size' x 'size'
  double A[size][size], B[size][size], C[size][size];
  int i, j, k;

  // Initialize matrices A and B with random values
  // srand is used to seed the random number generator
  srand(time(NULL));
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      A[i][j] = rand() % 10;
      B[i][j] = rand() % 10;
    }
  }

  // Compute matrix multiplication
  // Loop through each element in matrices A and B to compute C
  clock_t start_time = clock();
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      C[i][j] = 0;
      for (k = 0; k < size; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  clock_t end_time = clock();

  // Print the execution time in seconds
  // Print the execution time in milliseconds
  double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC * 1000;
  cout << "Execution time: " << time_taken << " milliseconds" << endl;

  return 0;
}
