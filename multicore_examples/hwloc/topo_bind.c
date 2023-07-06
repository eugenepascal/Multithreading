#include <stdio.h>
#include <stdlib.h>

#include <hwloc.h>

/**
 * \brief Entry point of the program.
 * \param argc number of arguments.
 * \param argv array of arguments.
 * \return EXIT_SUCCESS.
 */
int main(int argc, char** argv)
{
  hwloc_topology_t topology;
  int core_depth = 0;
  int nb_cores = -1;
  int bindtocore = argc > 1 ? atoi(argv[1]) : 0;
  int bindtopu = argc > 2 ? atoi(argv[2]) : 0;

  fprintf(stdout, "Try to bind on core %d PU %d\n", bindtocore, bindtopu);

  /* init and load topology */
  if(hwloc_topology_init(&topology) != 0)
  {
    fprintf(stderr, "Failed to initialize hwloc topology!\n");
    exit(EXIT_FAILURE);
  }

  if(hwloc_topology_load(topology) != 0)
  {
    fprintf(stderr, "Failed to load hwloc topology!\n");
    hwloc_topology_destroy(topology);
    exit(EXIT_FAILURE);
  }

  // PU is last depth so core is last depth - 1
  core_depth = hwloc_topology_get_depth(topology) - 2;
  nb_cores = hwloc_get_nbobjs_by_depth(topology, core_depth);

  if(bindtocore >= nb_cores)
  {
    fprintf(stdout, "Want to bind to core %d but it is out of range!\n",
        bindtocore);

    /*
    hwloc_topology_destroy(topology);
    exit(EXIT_FAILURE);
    */
  }

  for(int i = 0 ; i < nb_cores ; i++)
  {
    hwloc_obj_t core = hwloc_get_obj_by_depth(topology, core_depth, i);

    fprintf(stdout, "Core %d (%d)\n", core->logical_index, core->os_index);

    for(int j = 0; j < core->arity; j++)
    {
      hwloc_obj_t p = core->children[j];
    
      fprintf(stdout, "\tPU %d (%d)\n", p->logical_index, p->os_index);

      if(i == bindtocore && j == bindtopu)
      {
        if(hwloc_set_cpubind(topology, p->cpuset, HWLOC_CPUBIND_THREAD) != 0)
        {
          fprintf(stderr, "Failed to bind to PU %d\n", p->logical_index);
        }
        else
        {
          fprintf(stdout, "Bind to core %d PU %d success\n", i, j);
        }
      }
    }
  }

  /* do some "computations" */
  for(long i = 0 ; i < 10000000L ; i++)
  {
    for(long j = 0 ; j < 10000000L ; j++)
    {
    }
  }

  /* cleanup */
  hwloc_topology_destroy(topology);

  return EXIT_SUCCESS;;
}

