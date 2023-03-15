// Name: Saatvik

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <omp.h>
using namespace std;
#include <iomanip>

int main()
{
    int a[100][100], b[100][100], mult[100][100], r1 = 100, c1 = 100, r2 = 100, c2 = 100, i, j, k;

    // If column of first matrix in not equal to row of second matrix,
    // ask the user to enter the size of matrix again.
    while (c1!=r2)
    {
        cout << "Error! column of first matrix not equal to row of second.";

        cout << "Enter rows and columns for first matrix: ";
        cin >> r1 >> c1;

        cout << "Enter rows and columns for second matrix: ";
        cin >> r2 >> c2;
    }

    // Storing elements of first matrix.
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c1; ++j)
        {
            a[i][j] = rand() % 20;
        }

    // Storing elements of second matrix.
    for(i = 0; i < r2; ++i)
        for(j = 0; j < c2; ++j)
        {
            b[i][j] = rand() % 20;
        }

    // Initializing elements of matrix mult to 0.
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c2; ++j)
        {
            mult[i][j]=0;
        }

    // Multiplying matrix a and b and storing in array mult.
    auto start = std::chrono::high_resolution_clock::now();
    cout << "hello" << endl;
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c2; ++j)
          #pragma omp parallel
          for(k = 0; k < c1; ++k)
          {
            mult[i][j] += a[i][k] * b[k][j];
          }
    auto end = std::chrono::high_resolution_clock::now(); // end time
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); // duration in nanoseconds
    std::cout << "Task duration: " << std::fixed << std::setprecision(9) << duration << " nanoseconds" << std::endl;
}