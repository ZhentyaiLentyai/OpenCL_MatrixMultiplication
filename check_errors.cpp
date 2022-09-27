#include "library.h"

void check_errors(cl_int error, int &point)
{
	if (error != CL_SUCCESS)
	{
		switch (point)
		{
		case 0:
		{
			cerr << "Error in function clCreateContextFromType" << endl;
			break;
		}
		case 1:
		{
			cerr << "Error in function clCreateCommandQueue" << endl;
			break;
		}
		case 2:
		{
			cerr << "Error in function clCreateProgramWithSource" << endl;
			break;
		}
		case 3:
		{
			cerr << "Error in function clBuildProgram" << endl;
			break;
		}
		case 4:
		{
			cerr << "Error in function clCreateKernel" << endl;
			break;
		}
		case 5:
		{
			cerr << "Error in function clEnqueueWriteBuffer (a_matrix)" << endl;
			break;
		}
		case 6: 
		{
			cerr << "Error in function clEnqueueWriteBuffer (b_matrix)" << endl;
			break;
		}
		case 7:
		{
			cerr << "Error in function clGetKernelWorkGroupInfo" << endl;
			break;
		}
		case 8:
		{
			cerr << "Error in function clEnqueueNDRangeKernel" << endl;
			break;
		}
	
		default:
			cerr << "Not sure what the error is, sorry" << endl;
			break;
		}
	}
	point++;
}