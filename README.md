Implemented (<implementation_number> in program arguments):
- simple matrix multiplication without using local memory;
- multiplication using local memory and processing one element in each thread;
- multiplication using local memory and vector processing of several elements in each thread.

Arguments are passed to the program via the command line:
ocl1 <device_number> <input_file_name> <output_file_name> <implementation_number>

Input file format: n k m
/* first matrix, k columns by m rows, space separated elements */
/* second matrix, n columns by k rows, space separated elements */

Output file format: n m
/* matrix, size n columns by m rows, elements separated by space */

Kernel works like this:
the entire list of available devices is taken and sorted in order:
1. Discrete video card(s);
2. Integrated video card;
3. Processor(s).
