// Name: Saatvik

#include <chrono>
#include <iostream>
using namespace std;
#include <iomanip>
#define N 100

int main()
{
    // Initialising the variables
    int a[N][N], b[N][N], mult[N][N], r1 = N, c1 = N, r2 = N, c2 = N, i, j, k;
    
    // Initialsing the matrixs
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c1; ++j)
        {
            a[i][j] = rand() % 20;
            b[i][j] = rand() % 20;
            mult[i][j]=0;
        }
    // Calculating the time
    auto start = std::chrono::high_resolution_clock::now();

    //Multiplying the matrix using the algorithm
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c2; ++j)
            for(k = 0; k < c1; ++k)
            {
                mult[i][j] += a[i][k] * b[k][j];
            }
    
    auto end = std::chrono::high_resolution_clock::now(); // end time
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); // duration in nanoseconds
    std::cout << "Task duration: " << std::fixed << std::setprecision(9) << duration << " nanoseconds" << std::endl;

    return 0;
}