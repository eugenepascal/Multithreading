#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <threads.h>

static tss_t key;

struct thread_info
{
  char buf[255];
  long val;
};

int thread_func(void* arg)
{
  struct thread_info* info = tss_get(key);

  if(info == NULL)
  {
    info = malloc(sizeof(struct thread_info));

    snprintf(info->buf, sizeof(info->buf), "Thread %ld", (long)arg);
    info->val = (long)arg;
    tss_set(key, info);
  }
  else
  {
    printf("failed %d\n", errno);
  }

  sleep(1);
  for(int i = 0 ; i < 2 ; i++)
  {

    struct thread_info* thinfo = tss_get(key);
    if(thinfo)
    {
      fprintf(stdout, "We are in %s (val = %ld)\n", thinfo->buf, thinfo->val);
    }
    sleep(1);
  }

  return 0;
}

int main(int argc, char** argv)
{
  size_t nb = 4;
  thrd_t th[nb];
  int th_ret = 0;
  int ret = 0;

  if(tss_create(&key, free) != thrd_success)
  {
    fprintf(stderr, "Failed to create tss\n");
    exit(EXIT_FAILURE);
  }
 
  for(size_t i = 0 ; i < nb ; i++)
  {
    ret = thrd_create(&th[i], thread_func, (void*)i);

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
    thrd_join(th[i], &th_ret);
  }

  tss_delete(key);
  printf("Exit program\n");
  return EXIT_SUCCESS;
}

