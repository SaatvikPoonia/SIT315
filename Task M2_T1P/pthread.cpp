// Name: Saatvik

// Including Liraries
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <string>
using namespace std;
#include <iomanip>

// Initialising the threads & N
#define NUM_THREADS 100
#define N 100

// Initialising the variable for mutex
pthread_mutex_t mutx;

// Initialising the variables
int a[N][N], b[N][N], mult[N][N], r1 = N, c1 = N, r2 = N, c2 = N, i, j, k;

// The thread function which is performed by different threads
void *mat_mul(void *rowt)
{
    long row = (long)rowt;

    for (int i = 0; i < c2; i++)
        for (int j = 0; j < c1; j++)
        {
            mult[row][i] += a[row][j] * b[j][i];
        }
    pthread_exit(NULL);
}

int main()
{

    // calculating the number of cores in the process
    int cores = std::thread::hardware_concurrency();
    pthread_mutex_init(&mutx, NULL);

    cout << " the number of cores on this machine = " << cores << endl;

    // initialising the all the matrix
    for (i = 0; i < r1; ++i)
        for (j = 0; j < c1; ++j)
        {
            a[i][j] = rand() % 20;
            b[i][j] = rand() % 20;
            mult[i][j] = 0;
        }

    // calculating the time using cronic
    auto start = std::chrono::high_resolution_clock::now();

    // using different threads to multiply the threads
    // creating new threads and run the multiply matrix function
    pthread_t threads[NUM_THREADS];
    for (int rowt = 0; rowt < r1; rowt++)
    {
        pthread_create(&threads[rowt], NULL, mat_mul, (void *)rowt);
    }

    auto end = std::chrono::high_resolution_clock::now();                                      // end time
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); // duration in nanoseconds
    std::cout << "Task duration: " << std::fixed << std::setprecision(9) << duration << " nanoseconds" << std::endl;

    // joining all the threads
    for (int rowt = 0; rowt < r1; rowt++)
    {
        pthread_join(threads[rowt], NULL);
    }
    return 0;
}