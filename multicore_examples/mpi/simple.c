#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(int argc, char** argv)
{
  int world_size = 0;
  int world_rank = 0;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len = 0;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Get_processor_name(processor_name, &name_len);

  fprintf(stdout, "Hello world from processor %s, rank %d of %d processors\n",
      processor_name, world_rank, world_size);

  MPI_Finalize();
  return EXIT_SUCCESS;
}

