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
    MPI_Request request;
    int request_complete = 0;
    MPI_Status status;
    int dst = 1; /* send to node 1 */
    int do_work = 1;

    MPI_Isend(nb, sizeof(nb) / sizeof(int), MPI_INT,
        dst, TAG_MSG, MPI_COMM_WORLD, &request);

    while(do_work)
    {
      fprintf(stdout, "Node %d go to sleep\n", world_rank);

      /* work */
      /* ... */

      do_work = rand() % 2;
      fprintf(stdout, "Node %d %d\n", world_rank, do_work);

      if(!request_complete)
      {
        fprintf(stdout, "Node %d tests if request is sent\n", world_rank);

        MPI_Test(&request, &request_complete, &status);
        if(request_complete)
        {
          break;
        }
      }
    }

    /* no more work to do, if data are not yet sent, wait ! */
    if(!request_complete)
    {
      MPI_Wait(&request, &status);
    }
    fprintf(stdout, "Node %d finished sending data\n", world_rank);
  }
  else if(world_rank == 1)
  {
    int nb[5];
    int src = 0; /* receive from node 0 (master) */
    MPI_Request request;
    MPI_Status status;

    fprintf(stdout, "Receive data\n");
    MPI_Irecv(nb, sizeof(nb) / sizeof(int), MPI_INT,
        src, TAG_MSG, MPI_COMM_WORLD, &request);

    fprintf(stdout, "Node %d waits for data:", world_rank);
    MPI_Wait(&request, &status);

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

