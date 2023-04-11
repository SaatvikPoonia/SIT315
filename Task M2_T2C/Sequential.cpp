// Name: Saatvik

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

void quicksort(vector<int>& arr, int left, int right) {
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];
    
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    
    if (left < j) quicksort(arr, left, j);
    if (i < right) quicksort(arr, i, right);
}

double get_execution_time(clock_t start_time, clock_t end_time) {
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

int main() {
    srand(time(NULL));
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    
    vector<int> arr(n);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = rand() % 1000;
    }
    
    clock_t start_time = clock();
    quicksort(arr, 0, arr.size() - 1);
    clock_t end_time = clock();
    
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    cout << "Execution time: " << get_execution_time(start_time, end_time) << " seconds" << endl;
    
    return 0;
}

