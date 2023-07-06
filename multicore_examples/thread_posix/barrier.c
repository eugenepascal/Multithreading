#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

static pthread_barrier_t barrier;

void* thread_func(void* arg)
{
  int a = 0;
  unsigned int idx = *((int*)arg);
  printf("Thread %d starts\n", idx);

  for(int i = 0 ; i < 1000 ; i++)
  {
    a++;
  }

  printf("Thread %d waits\n", idx);
  pthread_barrier_wait(&barrier);

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 10;
  pthread_t th[nb];
  unsigned int th_arg[nb];
  void* th_ret = NULL;
  int ret = 0;

  ret = pthread_barrier_init(&barrier, NULL, nb + 1);
  if(ret != 0)
  {
    printf("Error creating barrier (%d)\n", ret);
    exit(EXIT_FAILURE);
  }

  for(int i = 0 ; i < nb ; i++)
  {
    th_arg[i] = i;
    ret = pthread_create(&th[i], NULL, thread_func, &th_arg[i]);

    if(ret == 0) /* success */
    {
      printf("Thread created !\n");
    }
    else
    {
      printf("Error creating thread (%d)\n", ret);
    }
  }

  pthread_barrier_wait(&barrier);
  printf("All threads are initialized\n");

  for(size_t i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    pthread_join(th[i], &th_ret);
  }

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

