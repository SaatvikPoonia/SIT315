// Name: Saatvik

#include <iostream>
#include <vector>
#include <chrono> // include the chrono library for timing

using namespace std;

// Function to add two vectors element-wise and store the result in a third vector
void addVectors(vector<int> &a, vector<int> &b, vector<int> &c)
{
    int n = a.size();
    for (int i = 0; i < n; i++)
    {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    // Create two input vectors
    int n = 1000000;
    vector<int> a(n); // create vector a of size n
    vector<int> b(n); // create vector b of size n
    vector<int> c(n); // create vector c of size n
    for (int i = 0; i < n; i++)
    {
        a[i] = i;       // set the i-th element of vector a to i
        b[i] = i * 2;   // set the i-th element of vector b to 2*i
    }

    // Measure the execution time
    auto start_time = chrono::steady_clock::now(); // get the start time using chrono library
    addVectors(a, b, c); // call the function to add vectors a and b and store the result in vector c
    auto end_time = chrono::steady_clock::now();   // get the end time using chrono library
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // calculate the duration in microseconds using chrono library

    // Print the result and execution time
    for (int i = 0; i < n; i++)
    {
        cout << c[i] << " "; // print the i-th element of vector c
    }
    cout << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl; // print the execution time

    return 0;
}
