#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int work (int* nb, int size)
{
    int ret = 0;

    for(int i = 0; i < size; i++)
    {
        ret += nb[i];
    }
    return ret;
}

int main (int argc, char** argv)
{
    int world_size = 0;
    int world_rank = 0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len = 0;
    int* nb = NULL;
    int nb_size = 128;
    int* sub_nb = NULL;
    int sub_size = 0;
    int root = 0;
    int* response = NULL;
    int sum = 0;

    MPI_Init (NULL, NULL);
    MPI_Comm_size (MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank (MPI_COMM_WORLD, &world_rank);
    MPI_Get_processor_name (processor_name, &name_len);

    fprintf (stdout, "Hello world from processor %s, rank %d of %d processors\n",
        processor_name, world_rank, world_size);

    if(world_rank == root)
    {
        nb = new int[nb_size];

        /* populate array with random int */
        for(int i = 0; i < nb_size; i++)
        {
            nb[i] = i;
        }
    }

    if(!(nb_size % world_size))
    {
        sub_size = nb_size / world_size;
        sub_nb = new int[sub_size];

        /* distribute data to send to nodes */
        MPI_Scatter (nb, sub_size, MPI_INT, sub_nb, sub_size, MPI_INT, root,
            MPI_COMM_WORLD);
    }
    else
    {
        int* sendcounts = new int[world_size];
        int* displs = new int[world_size];
        int tmp = nb_size / world_size;
        int rem = nb_size % world_size;
        int shift = 0;

        /* populate array which will contains elements number and
         * offset for each processor
         */
        for(int i = 0; i < world_size; i++)
        {
            sendcounts[i] = tmp;
            if(rem)
            {
                sendcounts[i]++;
                rem--;
            }

            displs[i] = shift;
            shift += sendcounts[i];
        }

        sub_size = sendcounts[world_rank];
        sub_nb = new int[sub_size];

        MPI_Scatterv (nb, sendcounts, displs, MPI_INT, sub_nb, sub_size, MPI_INT,
            root, MPI_COMM_WORLD);
        delete[] sendcounts;
        delete[] displs;
    }

    if(world_rank == root)
    {
        response = new int[world_size];
    }

    sum = work (sub_nb, sub_size);

    MPI_Gather (&sum, 1, MPI_INT, response, 1, MPI_INT, root, MPI_COMM_WORLD);

    if(world_rank == root)
    {
        sum = work (response, world_size);
        fprintf (stdout, "Sum is %d\n", sum);

        /* verify answer by computing sequentially */
        fprintf (stdout, "Sequential : %d\n", work (nb, nb_size));
        delete[] response;
    }

    delete[] sub_nb;
    delete[] nb;

    MPI_Finalize ();
    return EXIT_SUCCESS;
}
