#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <threads.h>

static thread_local uintptr_t val = 0;

int thread_func(void* arg)
{
  val = (uintptr_t)arg;
  sleep(1);
  fprintf(stdout, "[%ld] val = %lu\n", thrd_current(), val);
  return 0;
}

int main(int argc, char** argv)
{
  size_t nb = 4;
  thrd_t th[nb];
  int th_ret = 0;
  int ret = 0;

  for(size_t i = 0 ; i < nb ; i++)
  {
    ret = thrd_create(&th[i], thread_func, (void*)i);

    if(ret == 0) /* success */
    {
      printf("Thread created !\n");
    }
    else
    {
      printf("Error creating thread (%d)\n", ret);
    }
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    thrd_join(th[i], &th_ret);
  }

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

