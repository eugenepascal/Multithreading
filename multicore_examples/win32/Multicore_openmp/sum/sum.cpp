#include <cstdlib>

#include <iostream>

#include <omp.h>

int main(int argc, char** argv)
{
	double start = 0;
	int* tab = NULL;
	int sum = 0;
	int loc_sum = 0;
	int nb_elements = 128;
	//int nb_elements = 1024 * 1024;
	//int nb_elements = 2048 * 2048;
	int nb_threads = 4;
	//int nb_threads = 1;
	
	// create big array
	tab = new int[nb_elements];

	// populate array
	for(int i = 0 ; i < nb_elements ; i++)
	{
		tab[i] = i;
	}

	// start time
	start = omp_get_wtime();

	// create block on threads with a shared variable and private ones
	#pragma omp parallel num_threads(nb_threads) shared(sum, tab) private(loc_sum)
	{
		loc_sum = 0;

		// calculate sum of assigned array of numbers
		#pragma omp for
		for(int i = 0 ; i < nb_elements ; i++)
		{
			loc_sum += tab[i];
		}
		
		// be sure to protect shared variable with a lock
		#pragma omp critical
		{
			std::cout << omp_get_thread_num() << " -> " << loc_sum <<  std::endl;
			sum += loc_sum;
		}
	}

	std::cout << "Calculation finished in " << (omp_get_wtime() - start) << " s" << std::endl;
	std::cout << "Sum is " << sum << std::endl;
	
	delete[] tab;

	return EXIT_SUCCESS;
}
