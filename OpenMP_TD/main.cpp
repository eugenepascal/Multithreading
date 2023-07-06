#include <iostream>
#include <omp.h>

int main() {
#pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();
        std::cout << "Hello world from thread " << id << std::endl;
    }
    return 0;
}
