#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <threads.h>

static mtx_t mtx;
static mtx_t mtx2;

void usleep(unsigned int us)
{
  struct timespec tv;

  tv.tv_sec = us / 1000000; 
  tv.tv_nsec = (us % 1000000) * 1000;
  nanosleep(&tv, NULL);
}

int thread_func(void* arg)
{
  mtx_lock(&mtx);
  usleep(100);
  mtx_lock(&mtx2);

  /* simulate long operation ! */
  usleep(10);

  mtx_unlock(&mtx2);
  mtx_unlock(&mtx);

  return 0;
}

int thread_func2(void* arg)
{
  mtx_lock(&mtx2);
  usleep(100); 
  mtx_lock(&mtx);

  /* simulate long operation ! */
  usleep(10);

  mtx_unlock(&mtx);
  mtx_unlock(&mtx2);

  return 0;
}

int main(int argc, char** argv)
{
  thrd_t th1;
  thrd_t th2;
  int th_ret = 0;

  mtx_init(&mtx, mtx_timed);
  mtx_init(&mtx2, mtx_timed);

  thrd_create(&th1, thread_func, NULL);
  thrd_create(&th2, thread_func2, NULL);
  fprintf(stdout, "Thread started\n");

  /* join thread */
  thrd_join(th1, &th_ret);
  thrd_join(th2, &th_ret);

  mtx_destroy(&mtx);
  mtx_destroy(&mtx2);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

