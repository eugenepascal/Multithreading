#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

int main (int argc, char *argv[])
{
  long long sum = 0;
  long tmp = 0;

  #pragma omp parallel shared(sum) private(tmp) num_threads(4)
  {
    tmp = 0;

    #pragma omp for
    for(long i = 0 ; i < 102400 ; i++)
    {
      for(long j = 0 ; j < 102400 ; j++)
      {
        tmp += (i + j);
      }
    }

    #pragma omp atomic
    sum += tmp;
  }

  fprintf(stdout, "Sum: %lld\n", sum);
  return EXIT_SUCCESS;
}
