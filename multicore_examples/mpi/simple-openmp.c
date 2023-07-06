#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#if _OPENMP
#include <omp.h>
#endif

int main(int argc, char** argv)
{
  int world_size = 0;
  int world_rank = 0;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len = 0;

#if _OPENMP
  int required = MPI_THREAD_SERIALIZED;
  int provided = 0;

  if(MPI_Init_thread(NULL, NULL, required, &provided) != MPI_SUCCESS)
  {
    fprintf(stderr, "Failed to initialize MPI.\n");
    exit(EXIT_FAILURE);
  }

  if(provided < required)
  {
    fprintf(stderr, "Failed to configure MPI thread.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

#else
  MPI_Init(NULL, NULL);
#endif

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Get_processor_name(processor_name, &name_len);

  fprintf(stdout, "Hello world from processor %s, rank %d of %d processors\n",
      processor_name, world_rank, world_size);

  #pragma omp parallel
  {
    fprintf(stdout, "Process %d thread %d\n", world_rank, omp_get_thread_num());
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
