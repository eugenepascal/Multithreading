#include <stdio.h>
#include <stdlib.h>

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
  void* th_ret = NULL;
  int ret = 0;

  ret = pthread_create(&th, NULL, thread_func, th_arg);

  if(ret == 0) /* success */
  {
    printf("Thread created !\n");
  }
  else
  {
    printf("Error creating thread (%d)\n", ret);
  }

  /* wait thread to terminate */
  pthread_join(th, &th_ret);
  printf("Exit program\n");
  return EXIT_SUCCESS;
}

