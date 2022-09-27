#pragma once

#ifdef __APPLE__
# include <OpenCL/opencl.h>     
#else
#include <CL/cl.h>              
#endif

#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <sstream>

using namespace std;

size_t selection_of_parameters(size_t& n, size_t& k, size_t& m);
vector<float> opencl(vector<float>& matrix_A, vector<float>& matrix_B, size_t n, size_t k, size_t m,
					int device1, float& algorithm_time, float& kernel_time, int implementationKey);
cl_device_id search_device(int deviceKey);
void check_errors(cl_int error, int &point);
void check_queue(cl_command_queue queue, int &point);
void printMatrixInFile(vector<float>& matrix, int n, int m, size_t temp_n, size_t temp_m, char* argv[], int implementationKey);