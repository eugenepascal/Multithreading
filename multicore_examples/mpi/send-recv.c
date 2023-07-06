#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define TAG_MSG 42

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

  if(world_rank == 0)
  {
    int nb[] = {0, 1, 2, 3, 4};

    /* send message to every nodes except ourself (id 0) */
    for(int i = 1 ; i < world_size ; i++)
    {
      MPI_Send(nb, sizeof(nb) / sizeof(int), MPI_INT,
          i, TAG_MSG, MPI_COMM_WORLD);
    }

    fprintf(stdout, "Node %d finished sending data\n", world_rank);
  }
  else
  {
    int nb[5];
    int src = 0; /* receive from node 0 (master) */

    MPI_Recv(nb, sizeof(nb) / sizeof(int), MPI_INT,
        src, TAG_MSG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    fprintf(stdout, "Node %d received:", world_rank);
    for(int i = 0 ; i < sizeof(nb) / sizeof(int) ; i++)
    {
      fprintf(stdout, " %d", nb[i]);
    }
    fprintf(stdout, "\n");
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}

