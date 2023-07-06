#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

static pthread_mutex_t mtx;
static pthread_mutex_t mtx2;

void usleep(unsigned int us)
{
  struct timespec tv;

  tv.tv_sec = us / 1000000; 
  tv.tv_nsec = (us % 1000000) * 1000;
  nanosleep(&tv, NULL);
}

void* thread_func(void* arg)
{
  pthread_mutex_lock(&mtx);
  usleep(100);
  pthread_mutex_lock(&mtx2);

  /* simulate long operation ! */
  usleep(10);

  pthread_mutex_unlock(&mtx2);
  pthread_mutex_unlock(&mtx);

  return NULL;
}

void* thread_func2(void* arg)
{
  pthread_mutex_lock(&mtx2);
  usleep(100); 
  pthread_mutex_lock(&mtx);

  /* simulate long operation ! */
  usleep(10);

  pthread_mutex_unlock(&mtx);
  pthread_mutex_unlock(&mtx2);

  return NULL;
}

int main(int argc, char** argv)
{
  pthread_t th1;
  pthread_t th2;
  void* th_ret = NULL;

  pthread_mutex_init(&mtx, NULL);
  pthread_mutex_init(&mtx2, NULL);

  pthread_create(&th1, NULL, thread_func, NULL);
  pthread_create(&th2, NULL, thread_func2, NULL);
  fprintf(stdout, "Thread started\n");

  /* join thread */
  pthread_join(th1, &th_ret);
  pthread_join(th2, &th_ret);

  pthread_mutex_destroy(&mtx);
  pthread_mutex_destroy(&mtx2);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

