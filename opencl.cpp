#include "library.h"

const char* matrixMultiple =
"__kernel void multiMatrix(int n, int k, int m, global const float* a, global const float* b, global float* c)	\n"
"{																												\n"
"	int row = get_global_id(0);																					\n"
"	int col = get_global_id(1);																					\n"
"	float temp = 0;																								\n"
"	for (int i = 0; i < k; i++) {																				\n"
"		temp += a[row * k + i] * b[col + i * n]; 																\n"
"	}																											\n"
"	c[row * n + col] = temp;																					\n"
"}																												\n";

const char* matrixMultiple2 =
"__kernel void multiMatrix2(int n, int k, int m, global const float* a, global const float* b, global float* c)	\n"
"{																												\n"
"#define local_size 16																							\n"
"const int row = get_local_id(0);																				\n"
"const int col = get_local_id(1);																				\n"
"const int global_row = local_size * get_group_id(0) + row;														\n"
"const int global_col = local_size * get_group_id(1) + col;														\n"
"__local float _a[local_size][local_size];																		\n"
"__local float _b[local_size][local_size];																		\n"
"float temp = 0.0f;																								\n"
"const int tile = k / local_size;																				\n"
"	for (int i = 0; i < tile; i++) {																			\n"
"		const int frow = local_size * i + row;																	\n"
"		const int fcol = local_size * i + col;																	\n"
"		_a[col][row] = a[global_row * k + fcol];																\n"
"		_b[col][row] = b[frow * n + global_col];																\n"
"		barrier(CLK_LOCAL_MEM_FENCE);																			\n"
"		for (int k = 0; k < local_size; k++) {																	\n"
"			temp += _a[k][row] * _b[col][k];																	\n"
"		}																										\n"
"	barrier(CLK_LOCAL_MEM_FENCE);																				\n"
"	}																											\n"
"c[global_row * n + global_col] = temp;																			\n"
"}																												\n";

const char* matrixMultiple3 =
"__kernel void multiMatrix3(int k, int m, __global float8 * B, __global float8 * A, __global float8 * C)\n"
"{																										\n"
"#define local_size 16																					\n"
"	const int row = get_local_id(0);																	\n"
"	const int col = get_local_id(1);																	\n"
"	const int global_row = (local_size / 8) * get_group_id(0) + row; 									\n"
"	const int global_col = local_size * get_group_id(1) + col; 											\n"
"	__local float8 buff_A[local_size][local_size / 8];													\n"
"	__local float8 buff_B[local_size][local_size / 8];													\n"
"	float8 temp = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };									\n"
"	const int num_blocks = k  / local_size;																\n"
"	for (int i = 0; i < num_blocks; i++) {																\n"
"		const int block_row = (local_size / 8) * i + row;												\n"
"		const int block_col = local_size * i + col;														\n"
"		buff_A[col][row] = A[block_col * (m / 8) + global_row];											\n"
"		buff_B[col][row] = B[block_row + global_col * (k / 8)];											\n"
"		barrier(CLK_LOCAL_MEM_FENCE);																	\n"
"		float8 vec_A;																					\n"
"		float8 vec_B;																					\n"
"		float val_B;																					\n"
"		for (int j = 0; j < (local_size / 8); j++) {													\n"
"			vec_B = buff_B[col][j];																		\n"
"			for (int l = 0; l < 8; l++) {																\n"
"				vec_A = buff_A[8 * j + l][row];															\n"
"				switch (l) {																			\n"
"				case 0:																					\n"
"					val_B = vec_B[0]; break;															\n"
"				case 1:																					\n" 
"					val_B = vec_B[1]; break;															\n"
"				case 2: 																				\n"
"					val_B = vec_B[2]; break;															\n"
"				case 3: 																				\n"
"					val_B = vec_B[3]; break;															\n"
"				case 4: 																				\n"
"					val_B = vec_B[4]; break;															\n"
"				case 5:																					\n"
"					val_B = vec_B[5]; break;															\n"
"				case 6: 																				\n"
"					val_B = vec_B[6]; break;															\n"
"				case 7: 																				\n"
"					val_B = vec_B[7]; break;															\n"
"				}																						\n"
"				temp[0] += vec_A[0] * val_B;															\n"
"				temp[1] += vec_A[1] * val_B;															\n"
"				temp[2] += vec_A[2] * val_B;															\n"
"				temp[3] += vec_A[3] * val_B;															\n"
"				temp[4] += vec_A[4] * val_B;															\n"
"				temp[5] += vec_A[5] * val_B;															\n"
"				temp[6] += vec_A[6] * val_B;															\n"
"				temp[7] += vec_A[7] * val_B;															\n"
"			}																							\n"
"		}																								\n"
"		barrier(CLK_LOCAL_MEM_FENCE);																	\n"
"	}																									\n"
"	C[global_col * (m / 8) + global_row] = temp;														\n"
"}																										\n";

vector<float> opencl(vector<float>& matrix_A, vector<float>& matrix_B, size_t n, size_t k, size_t m,
	int deviceKey, float& algorithmTime, float& kernelTime, int implementationKey)
{
	size_t N = n, K = k, M = m;
	vector<float> matrix_C(n * m);

	cl_device_id device = search_device(deviceKey);

	cl_int error = 0;

	int pointError = 0;

	cl_context context = clCreateContextFromType(0, CL_DEVICE_TYPE_ALL, NULL, NULL, &error);
	check_errors(error, pointError);

	size_t size = 0;
	clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &size);
	clGetContextInfo(context, CL_CONTEXT_DEVICES, size, device, NULL);

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &error);

	int pointQueue = 0;
	check_errors(error, pointError); check_queue(queue, pointQueue);

	size_t lenghtKernel[] = {0};
	
	if(implementationKey == 1)
	{
		lenghtKernel[0] = { strlen(matrixMultiple) };
	}
	else if(implementationKey == 2)
	{
		lenghtKernel[0] = { strlen(matrixMultiple2) };
	}
	else 
	{
		lenghtKernel[0] = { strlen(matrixMultiple3) };
	}
	
	cl_program program = 0;
	if(implementationKey == 1)
	{
		program = clCreateProgramWithSource(context, 1, &matrixMultiple, lenghtKernel, &error); 
	}
	else if (implementationKey == 2)
	{
		program = clCreateProgramWithSource(context, 1, &matrixMultiple2, lenghtKernel, &error);
	}
	else
	{
		program = clCreateProgramWithSource(context, 1, &matrixMultiple3, lenghtKernel, &error);
	}
	check_errors(error, pointError); check_queue(queue, pointQueue);

	error = clBuildProgram(program, 1, &device, NULL, NULL, NULL); 
	check_errors(error, pointError);
	
	if (error != CL_SUCCESS) {
		cout << "Build prog failed" << endl;
		size_t logSize = 0;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
		char* log = new char[logSize];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, log, nullptr);
		cout << log;
	}

	auto algorithmStart = std::chrono::high_resolution_clock::now();

	cl_kernel kernel = 0;
	if (implementationKey == 1) 
	{
		kernel = clCreateKernel(program, "multiMatrix", &error); 
	}
	else if(implementationKey == 2)
	{
		kernel = clCreateKernel(program, "multiMatrix2", &error);
	}
	else
	{
		kernel = clCreateKernel(program, "multiMatrix3", &error);
	}
	check_errors(error, pointError); check_queue(queue, pointQueue);
	
	cl_mem a_matrix = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * k * m, NULL, NULL); check_queue(queue, pointQueue);
	cl_mem b_matrix = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n * k, NULL, NULL); check_queue(queue, pointQueue);
	cl_mem c_matrix = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n * m, NULL, NULL); check_queue(queue, pointQueue);

	float* a = &matrix_A[0];
	float* b = &matrix_B[0];
	float* c = &matrix_C[0];

	error = clEnqueueWriteBuffer(queue, a_matrix, CL_FALSE, 0, sizeof(float) * k * m, (void*)a, 0, NULL, NULL); check_errors(error, pointError);
	error = clEnqueueWriteBuffer(queue, b_matrix, CL_FALSE, 0, sizeof(float) * n * k, (void*)b, 0, NULL, NULL); check_errors(error, pointError);

	if(implementationKey == 3)
	{
		clSetKernelArg(kernel, 0, sizeof(int), &k);
		clSetKernelArg(kernel, 1, sizeof(int), &m);
		clSetKernelArg(kernel, 2, sizeof(cl_mem), &a_matrix);
		clSetKernelArg(kernel, 3, sizeof(cl_mem), &b_matrix);
		clSetKernelArg(kernel, 4, sizeof(cl_mem), &c_matrix);
	}
	else 
	{
		clSetKernelArg(kernel, 0, sizeof(int), &n);
		clSetKernelArg(kernel, 1, sizeof(int), &k);
		clSetKernelArg(kernel, 2, sizeof(int), &m);
		clSetKernelArg(kernel, 3, sizeof(cl_mem), &a_matrix);
		clSetKernelArg(kernel, 4, sizeof(cl_mem), &b_matrix);
		clSetKernelArg(kernel, 5, sizeof(cl_mem), &c_matrix);
	}
	
	size_t group = 0;
	error = clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &group, NULL);
	check_errors(error, pointError);

	size_t* global = new size_t[2];
	size_t* local = new size_t[2];

	if(implementationKey == 3)
	{
		global[0] = (size_t)m / 8;
		global[1] = (size_t)n;

		local[0] = (size_t)16 / 8;
		local[1] = (size_t)16;
	}
	else 
	{
		global[0] = (size_t)m;
		global[1] = (size_t)n;

		local[0] = (size_t)16;
		local[1] = (size_t)16;
	}
	
	cl_event evt;

	auto kernelStart = chrono::high_resolution_clock::now();

	error = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global, local, 0, NULL, &evt);
	check_errors(error, pointError);

	auto kernelEnd = chrono::high_resolution_clock::now();

	clWaitForEvents(1, &evt);

	clEnqueueReadBuffer(queue, c_matrix, CL_TRUE, 0, sizeof(float) * N * M, c, 0, NULL, NULL);
	auto algorithmEnd = chrono::high_resolution_clock::now();

	error = clFlush(queue);
	error = clFinish(queue);

	clReleaseMemObject(a_matrix);
	clReleaseMemObject(b_matrix);
	clReleaseMemObject(c_matrix);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	chrono::duration<float, milli> kernel_double_ms = kernelEnd - kernelStart;
	chrono::duration<float, milli> algorithm_double_ms = algorithmEnd - algorithmStart;
	kernelTime = kernel_double_ms.count();
	algorithmTime = algorithm_double_ms.count();

	return matrix_C;
}