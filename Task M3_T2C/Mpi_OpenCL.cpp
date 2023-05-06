// Name: Saatvik

#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <CL/cl.hpp>

using namespace std;

void quicksort(vector<int>& arr, int left, int right, cl::Context& context, cl::CommandQueue& queue) {
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

        // Create OpenCL buffer objects for the subarrays
        cl::Buffer buffer_left(context, CL_MEM_READ_WRITE, sizeof(int) * (j - left + 1));
        cl::Buffer buffer_right(context, CL_MEM_READ_WRITE, sizeof(int) * (right - j));

        // Copy the subarrays to the buffer objects
        queue.enqueueWriteBuffer(buffer_left, CL_TRUE, 0, sizeof(int) * (j - left + 1), &arr[left]);
        queue.enqueueWriteBuffer(buffer_right, CL_TRUE, 0, sizeof(int) * (right - j), &arr[j + 1]);

        // Create an OpenCL program object and compile the kernel
        string source = "__kernel void quicksort(__global int *arr, int left, int right) { "
                        "    if (left < right) { "
                        "        int pivot = arr[(left + right) / 2]; "
                        "        int i = left - 1; "
                        "        int j = right + 1; "
                        "        while (true) { "
                        "            do { i++; } while (arr[i] < pivot); "
                        "            do { j--; } while (arr[j] > pivot); "
                        "            if (i >= j) break; "
                        "            int temp = arr[i]; "
                        "            arr[i] = arr[j]; "
                        "            arr[j] = temp; "
                        "        } "
                        "        quicksort(arr, left, j); "
                        "        quicksort(arr, j + 1, right); "
                        "    } "
                        "} ";
        cl::Program program(context, source);
        program.build();

        // Create an OpenCL kernel object
        cl::Kernel kernel(program, "quicksort");

        // Set the kernel arguments
        kernel.setArg(0, buffer_left);
        kernel.setArg(1, left);
        kernel.setArg(2, j);
        kernel.setArg(3, buffer_right);
        kernel.setArg(4, j + 1);
        kernel.setArg(5, right);

        // Execute the kernel
        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(j - left + 1 + right - j), cl::NullRange);

        // Copy the sorted subarrays back to the original array
        queue.enqueueReadBuffer(buffer_left, CL_TRUE, 0, sizeof(int) * (j - left + 1), &arr[left]);
        queue.enqueueReadBuffer(buffer_right, CL_TRUE, 0, sizeof(int) * (right - j), &arr[j + 1]);


        // Merge the sorted chunks using quicksort
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

        // Initialize OpenCL
        cl_int err = CL_SUCCESS;
        cl::Context context(CL_DEVICE_TYPE_GPU);
        cl::Program program(context, "#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable\n#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable\n\n__kernel void add(__global int* a, __global int* b, __global int* c, const unsigned int size)\n{\n int id = get_global_id(0);\n if(id < size)\n c[id] = a[id] + b[id];\n}\n");

        // Get the number of OpenCL devices available
        std::vectorcl::Device devices;
        context.getInfo(CL_CONTEXT_DEVICES, &devices);
        int numDevices = devices.size();

        // Create OpenCL buffers
        cl::Buffer arrBuffer(context, CL_MEM_READ_WRITE, n * sizeof(int));
        cl::Buffer resultBuffer(context, CL_MEM_READ_WRITE, n * sizeof(int));

        // Copy the input array to the OpenCL buffer
        cl::CommandQueue queue(context, devices[0], 0, &err);
        queue.enqueueWriteBuffer(arrBuffer, CL_TRUE, 0, n * sizeof(int), arr.data());

        // Set up the kernel and arguments
        cl::Kernel kernel(program, "add", &err);
        kernel.setArg(0, arrBuffer);
        kernel.setArg(1, resultBuffer);
        kernel.setArg(2, n);

        // Execute the kernel on each OpenCL device
        std::vectorcl::Event events(numDevices);
        for (int i = 0; i < numDevices; i++) {
        cl::CommandQueue queue(context, devices[i], 0, &err);
        err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(n), cl::NullRange, NULL, &events[i]);
        }

        // Wait for all kernels to finish
        cl::Event::waitForEvents(events);

        // Read the result from the OpenCL buffer
        std::vector<int> result(n);
        queue.enqueueReadBuffer(resultBuffer, CL_TRUE, 0, n * sizeof(int), result.data());

        // Output the OpenCL result
        cout << "OpenCL result: ";
        for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
        }
        cout << endl << flush;

        // Clean up
        MPI_Finalize();
        return 0;
