#include "library.h"

void check_queue(cl_command_queue queue, int &point)
{
	if (!queue)
	{
		switch (point)
		{
		case 0:
		{
			cerr << "Error: Command queue not created" << endl;
			break;
		}
		case 1:
		{
			cerr << "Error: Source code program not created" << endl;
			break;
		}
		case 2:
		{
			cerr << "Error: Kernel object not created" << endl;
			break;
		}
		case 3:
		{
			cerr << "Error: Buffer object \"a_matrix\" not created" << endl;
			break;
		}
		case 4:
		{
			cerr << "Error: Buffer object \"b_matrix\" not created" << endl;
			break;
		}
		case 5:
		{
			cerr << "Error: Buffer object \"c_matrix\" not created" << endl;
			break;
		}
		default:
			cerr << "Not sure what the error is, sorry" << endl;
			break;
		}
	}
	point++;
}