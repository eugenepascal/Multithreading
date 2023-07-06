#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <threads.h>

static int good_a = 0;
static mtx_t mtx;
static cnd_t cond;

int thread_func(void* arg)
{
  int a = 0;

  for(int i = 0 ; i < 1000000 ; i++)
  {
    a++;
    if(a == 1000000)
    {
      mtx_lock(&mtx);
      {
        good_a = 1;
        cnd_broadcast(&cond);
      }
      mtx_unlock(&mtx);
    }
  }

  return 0;
}

int main(int argc, char** argv)
{
  thrd_t th;
  int th_ret = 0;
  int ret = 0;

  mtx_init(&mtx, mtx_timed);
  cnd_init(&cond);

  ret = thrd_create(&th, thread_func, NULL);

  if(ret == 0) /* success */
  {
    printf("Thread created !\n");
  }
  else
  {
    printf("Error creating thread (%d)\n", ret);
  }

  mtx_lock(&mtx);
  while(good_a != 1)
  {
    printf("Wait for good_a to be 1\n");
    cnd_wait(&cond, &mtx);
  }
  mtx_unlock(&mtx);

  printf("a = %d\n", good_a);

  /* wait threads to terminate */
  thrd_join(th, &th_ret);

  mtx_destroy(&mtx);
  cnd_destroy(&cond);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

