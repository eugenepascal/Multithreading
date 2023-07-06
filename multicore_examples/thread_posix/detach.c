#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>

void* thread_func(void* arg)
{
  printf("Thread function\n");

  return NULL;
}

int main(int argc, char** argv)
{
  pthread_t th;
  void* th_arg = NULL;
  pthread_attr_t th_attr;
  int ret = 0;

  if(pthread_attr_init(&th_attr) != 0)
  {
    printf("Error creating attribute\n");
    return EXIT_FAILURE;
  }

  pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);

  ret = pthread_create(&th, &th_attr, thread_func, th_arg);

  if(ret == 0) /* success */
  {
    printf("Thread created !\n");
  }
  else
  {
    printf("Error creating thread (%d)\n", ret);
  }

  /* no need to join */
  sleep(5);
  printf("Exit program\n");
  return EXIT_SUCCESS;
}

