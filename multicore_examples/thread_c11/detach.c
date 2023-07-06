#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
  int ret = 0;

  ret = thrd_create(&th, thread_func, th_arg);

  if(ret == 0) /* success */
  {
    printf("Thread created !\n");
    thrd_detach(th);
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

