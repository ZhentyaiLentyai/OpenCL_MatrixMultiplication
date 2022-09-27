#include "library.h"

int main(int argc, char* argv[])
{
	float algorithmTime = 0.0;
	float kernelTime = 0.0;

	int deviceKey = atoi(argv[1]);
	int implementationKey = atoi(argv[4]);

	switch (implementationKey)
	{
	case 1:
	{
		ifstream input(argv[2]);
		if (!input.is_open()) {
			cout << "Failed to open file_input" << endl;
			return 1;
		}

		stringstream read;
		read << input.rdbuf();
		size_t n, k, m;
		read >> n;
		read >> k;
		read >> m;

		vector<float> matrix_A(k * m);
		vector<float> matrix_B(n * k);
		vector<float> matrix_C(n * m);

		for (int i = 0; i < m * k; i++)
		{
			read >> matrix_A[i];
		}

		for (int i = 0; i < n * k; i++)
		{
			read >> matrix_B[i];
		}
		input.close();

		matrix_C = opencl(matrix_A, matrix_B, n, k, m, deviceKey, algorithmTime, kernelTime, implementationKey);
		printMatrixInFile(matrix_C, n, m, NULL, NULL, argv, implementationKey);
		break;
	}
	case 2:
	{
		ifstream input(argv[2]);
		if (!input.is_open()) {
			cout << "Failed to open file_input" << endl;
			return 1;
		}

		stringstream read;
		read << input.rdbuf();
		size_t n, k, m;
		read >> n;
		read >> k;
		read >> m;

		size_t temp_n = n;
		size_t temp_k = k;
		size_t temp_m = m;

		selection_of_parameters(n, k, m);

		vector<float> matrix_A(k * m);
		vector<float> matrix_B(n * k);
		vector<float> matrix_C(n * m);

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < k; j++)
			{
				if (j >= temp_k || i >= temp_m)
				{
					matrix_A[i * k + j] = 0;
				}
				else
				{
					read >> matrix_A[i * k + j];
				}
			}
		}

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (j >= temp_n || i >= temp_k)
				{
					matrix_B[i * n + j] = 0;
				}
				else
				{
					read >> matrix_B[i * n + j];
				}
			}
		}

		input.close();

		matrix_C = opencl(matrix_A, matrix_B, n, k, m, deviceKey, algorithmTime, kernelTime, implementationKey);
		printMatrixInFile(matrix_C, n, m, temp_n, temp_m, argv, implementationKey);
		break;
	}
	case 3:
	{
		ifstream input(argv[2]);
		if (!input.is_open()) {
			cout << "Failed to open file_input" << endl;
			return 1;
		}

		stringstream read;
		read << input.rdbuf();
		size_t n, k, m;
		read >> n;
		read >> k;
		read >> m;

		size_t temp_n = n;
		size_t temp_k = k;
		size_t temp_m = m;

		selection_of_parameters(n, k, m);

		vector<float> matrix_A(k* m);
		vector<float> matrix_B(n* k);
		vector<float> matrix_C(n* m);

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < k; j++)
			{
				if (j >= temp_k || i >= temp_m)
				{
					matrix_A[i * k + j] = 0;
				}
				else
				{
					read >> matrix_A[i * k + j];
				}
			}
		}

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (j >= temp_n || i >= temp_k)
				{
					matrix_B[i * n + j] = 0;
				}
				else
				{
					read >> matrix_B[i * n + j];
				}
			}
		}

		input.close();

		matrix_C = opencl(matrix_A, matrix_B, n, k, m, deviceKey, algorithmTime, kernelTime, implementationKey);
		printMatrixInFile(matrix_C, n, m, temp_n, temp_m, argv, 2);
		break;
	} 
	default:
		cerr << "Error: argv[4] - not true" << endl;
		break;
	}


	printf("\nTime: %f\t%f \n", kernelTime, algorithmTime);

    return 0;
}