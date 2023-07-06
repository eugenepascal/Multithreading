#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;

void init_once(void)
{
  fprintf(stdout, "Once initialization\n");
}

void* thread_func(void* arg)
{
  pthread_once(&once, init_once);

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 4;
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

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

