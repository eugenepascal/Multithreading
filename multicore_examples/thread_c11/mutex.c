#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <threads.h>

static int a = 0;

int thread_func(void* arg)
{
  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;
  }

  return 0;
}

int main(int argc, char** argv)
{
  size_t nb = 2;
  thrd_t th[nb];
  int th_ret = 0;
  int ret = 0;

  for(size_t i = 0 ; i < nb ; i++)
  {
    ret = thrd_create(&th[i], thread_func, NULL);

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

  printf("a = %d\n", a);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

