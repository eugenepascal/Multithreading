#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <threads.h>

static int a = 0;
static mtx_t mtx;

void usleep(unsigned int us)
{
  struct timespec tv;

  tv.tv_sec = us / 1000000; 
  tv.tv_nsec = (us % 1000000) * 1000;
  nanosleep(&tv, NULL);
}

int thread_func(void* arg)
{
  for(int i = 0 ; i < 10000 ; i++)
  {
    mtx_lock(&mtx);
    printf("[%lu] a = %d\n", thrd_current(), a);
    /* simulate long operation ! */
    usleep(10);
    mtx_unlock(&mtx);
  }

  return 0;
}

int main(int argc, char** argv)
{
  size_t nb = 5;
  thrd_t th[nb];
  int th_ret = 0;
  int ret = 0;

  mtx_init(&mtx, mtx_timed);

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

  for(int i = 0 ; i < 250 ; i++)
  {
    mtx_lock(&mtx);
    a++;
    mtx_unlock(&mtx);
    usleep(100);
  }

  for(int i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    thrd_join(th[i], &th_ret);
  }

  mtx_destroy(&mtx);
  printf("a = %d\n", a);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

