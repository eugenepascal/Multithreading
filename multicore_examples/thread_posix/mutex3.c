#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

static int a = 0;
static pthread_mutex_t mtx;

void usleep(unsigned int us)
{
  struct timespec tv;

  tv.tv_sec = us / 1000000; 
  tv.tv_nsec = (us % 1000000) * 1000;
  nanosleep(&tv, NULL);
}

void* thread_func(void* arg)
{
  for(int i = 0 ; i < 10000 ; i++)
  {
    pthread_mutex_lock(&mtx);
    printf("[%lu] a = %d\n", pthread_self(), a);
    /* simulate long operation ! */
    usleep(10);
    pthread_mutex_unlock(&mtx);
  }

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 5;
  pthread_t th[nb];
  void* th_ret = NULL;
  int ret = 0;

  pthread_mutex_init(&mtx, NULL);

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

  for(int i = 0 ; i < 250 ; i++)
  {
    pthread_mutex_lock(&mtx);
    a++;
    pthread_mutex_unlock(&mtx);
    usleep(100);
  }

  for(int i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    pthread_join(th[i], &th_ret);
  }

  pthread_mutex_destroy(&mtx);
  printf("a = %d\n", a);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

