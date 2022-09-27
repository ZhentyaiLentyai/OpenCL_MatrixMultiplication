#include "library.h"

size_t selection_of_parameters(size_t &n, size_t &k, size_t &m)
{
	int local_size = 16;
	if(n < local_size)
	{
		n = local_size;
	}
	if (k < local_size)
	{
		k = local_size;
	}
	if (m < local_size)
	{
		m = local_size;
	}

	while(n > local_size && n % local_size != 0)
	{
		n++;
	}
	while (k > local_size && k % local_size != 0)
	{
		k++;
	}
	while (m > local_size && m % local_size != 0)
	{
		m++;
	}


	return 0;
}