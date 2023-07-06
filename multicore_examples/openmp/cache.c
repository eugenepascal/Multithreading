#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

int main (int argc, char *argv[])
{
    const int M = 10240;
    int* tab = malloc(sizeof(int) * M * M);
    long global_sum = 0;

    if(!tab)
    {
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    for(int i = 0 ; i < M ; i++)
    {
      for(int j = 0 ; j < M ; j++)
      {
        tab[i * M + j] = i;
      }
    }

    #pragma omp parallel for shared(global_sum, tab)
    for(int i = 0 ; i < M ; i++)
    {
      int sum = 0;

      for(int j = 0 ; j < M ; j++)
      {
        sum += tab[i * M + j];
      }

      #pragma omp atomic
      global_sum += sum;
    }

    free(tab);

    printf("global_sum = %ld\n", global_sum);
    return EXIT_SUCCESS;
}

