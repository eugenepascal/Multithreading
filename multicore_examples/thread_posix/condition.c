#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

static int good_a = 0;
static pthread_mutex_t mtx;
static pthread_cond_t cond;

void* thread_func(void* arg)
{
  int a = 0;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;
    if(a == 1000000)
    {
      pthread_mutex_lock(&mtx);
      {
        pthread_cond_broadcast(&cond);
        good_a = 1;
      }
      pthread_mutex_unlock(&mtx);
    }
  }

  return NULL;
}

int main(int argc, char** argv)
{
  pthread_t th;
  void* th_ret = NULL;
  int ret = 0;

  pthread_mutex_init(&mtx, NULL);
  pthread_cond_init(&cond, NULL);

  ret = pthread_create(&th, NULL, thread_func, NULL);

  if(ret == 0) /* success */
  {
    printf("Thread created !\n");
  }
  else
  {
    printf("Error creating thread (%d)\n", ret);
  }

  pthread_mutex_lock(&mtx);
  while(good_a != 1)
  {
    printf("Wait for good_a to be 1\n");
    pthread_cond_wait(&cond, &mtx);
  }
  pthread_mutex_unlock(&mtx);

  printf("a = %d\n", good_a);

  /* wait threads to terminate */
  pthread_join(th, &th_ret);

  pthread_mutex_destroy(&mtx);
  pthread_cond_destroy(&cond);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

