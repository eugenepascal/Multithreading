#include <cstdio>
#include <omp.h>

int main() {
#pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();
        std::printf("Hello world from thread %d\n", id);
    }
    return 0;
}
