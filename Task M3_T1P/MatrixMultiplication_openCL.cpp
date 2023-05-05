#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <CL/cl.hpp>

using namespace std;

#define SIZE 50

const char* kernel_source =
    "_kernel void matrix_mult(_global const float* A, __global const float* B, __global float* C, const int size) {\n"
    "    int i = get_global_id(0);\n"
    "    int j = get_global_id(1);\n"
    "    float sum = 0.0f;\n"
    "    for (int k = 0; k < size; k++) {\n"
    "        sum += A[i * size + k] * B[k * size + j];\n"
    "    }\n"
    "    C[i * size + j] = sum;\n"
    "}\n";

int main(int argc, char** argv) {
    int i, j, k, num_procs, rank, chunk_size, start_index, end_index;
    float A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Calculate the chunk size and the start and end indices for each process
    chunk_size = SIZE / num_procs;
    start_index = rank * chunk_size;
    end_index = start_index + chunk_size - 1;

    // Initialize matrices A and B with random values
    srand(time(NULL));
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // Initialize OpenCL
    cl::Context context(CL_DEVICE_TYPE_GPU);
    vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    cl::CommandQueue queue(context, devices[0]);

    // Create the input and output buffers
    cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, SIZE * SIZE * sizeof(float));
    cl::Buffer buffer_B(context, CL_MEM_READ_ONLY, SIZE * SIZE * sizeof(float));
    cl::Buffer buffer_C(context, CL_MEM_WRITE_ONLY, SIZE * SIZE * sizeof(float));

    // Write the input data to the input buffers
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, SIZE * SIZE * sizeof(float), A);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, SIZE * SIZE * sizeof(float), B);

    // Create the kernel
    cl::Program program(context, kernel_source);
    program.build(devices);
    cl::Kernel kernel(program, "matrix_mult");

    // Set the kernel arguments
    kernel.setArg(0, buffer_A);
    kernel.setArg(1, buffer_B);
    kernel.setArg(2, buffer_C);
    kernel.setArg(3, SIZE);

    // Execute the kernel
    cl::NDRange global(SIZE, SIZE);
    cl::NDRange local(16, 16);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

    // Read the output data from the output buffer
    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, SIZE * SIZE * sizeof(float), C);

    // Reduce the results from each process into the master process
    MPI_Reduce(C + start_index, C, chunk_size * SIZE, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the result in the master process
    if (rank == 0) {
    cout << "Result Matrix:" << endl;
    for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
    cout << C[i][j] << " ";
    }
    cout << endl;
    }
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
    
