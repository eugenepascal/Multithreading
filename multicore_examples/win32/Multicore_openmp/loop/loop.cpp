#include <cstdlib>

#include <iostream>

#include <omp.h>

int main(int argc, char** argv)
{
	#pragma omp parallel for
	for(int i = 0 ; i < 10 ; i++)
	{
		// stdout on Windows is not thread-safe
		#pragma omp critical
		std::cout << omp_get_thread_num() << " -> " << i << std::endl;
	}
	return EXIT_SUCCESS;
}
