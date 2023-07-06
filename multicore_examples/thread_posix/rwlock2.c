#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

static int a = 0;
static pthread_rwlock_t mtx;

void usleep(unsigned int us)
{
  struct timespec tv;

  tv.tv_sec = us / 1000000; 
  tv.tv_nsec = (us % 1000000) * 1000;
  nanosleep(&tv, NULL);
}

void* thread_func(void* arg)
{
  unsigned int idx = *((int*)arg);

  for(int i = 0 ; i < 10000 ; i++)
  {
    pthread_rwlock_rdlock(&mtx);
    printf("[%d] a = %d\n", idx, a);
    /* pthread_rwlock_* costs a bit more than mutex 
     * BUT for long "read" operation it is better than mutex
     */
    /* simulate long operation ! */
    usleep(10);
    pthread_rwlock_unlock(&mtx);
  }

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 5;
  pthread_t th[nb];
  unsigned int th_arg[nb];
  void* th_ret = NULL;
  int ret = 0;

  pthread_rwlock_init(&mtx, NULL);

  for(size_t i = 0 ; i < nb ; i++)
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

  for(int i = 0 ; i < 250 ; i++)
  {
    pthread_rwlock_wrlock(&mtx);
    a++;
    pthread_rwlock_unlock(&mtx);
    usleep(250);
  }

  for(int i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    pthread_join(th[i], &th_ret);
  }

  pthread_rwlock_destroy(&mtx);
  printf("a = %d\n", a);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

