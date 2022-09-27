#include "library.h"

void printMatrixInFile(vector<float>& matrix, int n, int m, size_t temp_n, size_t temp_m, char* argv[], int implementationKey)
{
	ofstream out_matrix(argv[3]);

	if (!out_matrix.is_open())
	{
		cout << "Error: output file did not open!" << endl;
	}
	switch (implementationKey)
	{
	case 1:
	{
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				out_matrix << matrix[i * n + j] << " ";
			}
			out_matrix << endl;
		}
		break;
	}
	case 2:
	{
		for (int i = 0; i < m; i++)
		{
			if (i >= temp_m)
			{
				break;
			}
			for (int j = 0; j < n; j++)
			{
				if (j >= temp_n)
				{
					break;
				}
				out_matrix << matrix[i * n + j] << " ";
			}
			out_matrix << endl;
		}
		break;
	}
	default:
		break;
	}

	out_matrix.close();
}