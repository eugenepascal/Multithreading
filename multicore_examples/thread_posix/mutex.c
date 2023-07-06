#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

static int a = 0;

void* thread_func(void* arg)
{
  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;
  }

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 2;
  pthread_t th[nb];
  void* th_ret = NULL;
  int ret = 0;

  for(size_t i = 0 ; i < nb ; i++)
  {
    ret = pthread_create(&th[i], NULL, thread_func, NULL);

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
    pthread_join(th[i], &th_ret);
  }

  printf("a = %d\n", a);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

