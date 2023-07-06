#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

int main (int argc, char *argv[])
{
  long long sum = 0;
  long sum_local[32];

  for(int i = 0 ; i < 32 ; i++)
  {
    sum_local[i] = 0;
  }

  #pragma omp parallel shared(sum_local, sum)
  {
    int id = omp_get_thread_num();

    #pragma omp for
    for(long i = 0 ; i < 102400 ; i++)
    {
      for(long j = 0 ; j < 102400 ; j++)
      {
        sum_local[id] += (i + j);
      }
    }

    #pragma omp atomic
    sum += sum_local[id];
  }

  fprintf(stdout, "Sum: %lld\n", sum);
  return EXIT_SUCCESS;
}
