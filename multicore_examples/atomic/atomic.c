#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>

#include <pthread.h>

static atomic_int a;

void* thread_func(void* arg)
{
  //int idx = *((int*)arg);
  //printf("Thread %d\n", idx);

  for(int i = 0 ; i < 1000000 ; i++)
  {
    atomic_fetch_add(&a, 1);
  }

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 2;
  pthread_t th[nb];
  int th_arg[nb];
  void* th_ret = NULL;
  int ret = 0;

  atomic_init(&a, 0);

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

  for(size_t i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    pthread_join(th[i], &th_ret);
  }

  printf("a = %d\n", a);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

