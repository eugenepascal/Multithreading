#include <stdio.h>
#include <stdlib.h>

#include <threads.h>

int thread_func(void* arg)
{
  printf("Thread function\n");

  return 0;
}

int main(int argc, char** argv)
{
  thrd_t th;
  void* th_arg = NULL;
  int th_ret = 0;
  int ret = 0;

  ret = thrd_create(&th, thread_func, th_arg);

  if(ret == 0) /* success */
  {
    printf("Thread created !\n");
  }
  else
  {
    printf("Error creating thread (%d)\n", ret);
  }

  /* wait thread to terminate */
  thrd_join(th, &th_ret);
  printf("Exit program\n");
  return EXIT_SUCCESS;
}

