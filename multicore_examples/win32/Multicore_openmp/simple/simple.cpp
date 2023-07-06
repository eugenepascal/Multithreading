#include <cstdlib>

#include <iostream>

#include <omp.h>

int main(int argc, char** argv)
{
	#pragma omp parallel
	{
		std::cout << omp_get_thread_num() << "/" << omp_get_num_threads() << "-> Hello" << std::endl;
	}
	return EXIT_SUCCESS;
}
