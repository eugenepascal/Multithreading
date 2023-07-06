#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <pthread.h>

static pthread_key_t key;

struct thread_info
{
  char buf[255];
  long val;
};

void* thread_func(void* arg)
{
  struct thread_info* info = pthread_getspecific(key);

  if(info == NULL)
  {
    info = malloc(sizeof(struct thread_info));

    snprintf(info->buf, sizeof(info->buf), "Thread %ld", (long)arg);
    info->val = (long)arg;
    pthread_setspecific(key, info);
  }
  else
  {
    printf("failed %d\n", errno);
  }

  sleep(1);
  for(int i = 0 ; i < 2 ; i++)
  {
    struct thread_info* thinfo = pthread_getspecific(key);

    if(thinfo)
    {
      fprintf(stdout, "We are in %s (val = %ld)\n", thinfo->buf, thinfo->val);
    }
    sleep(1);
  }

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 4;
  pthread_t th[nb];
  void* th_ret = 0;
  int ret = 0;

  if(pthread_key_create(&key, free) != 0)
  {
    fprintf(stderr, "Failed to create tss\n");
    exit(EXIT_FAILURE);
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    ret = pthread_create(&th[i], NULL, thread_func, (void*)i);

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

  pthread_key_delete(key);
  printf("Exit program\n");
  return EXIT_SUCCESS;
}

