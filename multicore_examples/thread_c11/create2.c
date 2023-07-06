#include <stdio.h>
#include <stdlib.h>

#include <threads.h>

struct thread_arg
{
  int integer;
};

int thread_func(void* arg)
{
  struct thread_arg* th_arg = (struct thread_arg*)arg;

  printf("Thread function\n");

  printf("Argument is %d\n", th_arg->integer);
  return 42;
}

int main(int argc, char** argv)
{
  thrd_t th;
  struct thread_arg th_arg;
  int th_ret = 0;
  int ret = 0;

  th_arg.integer = 67;

  ret = thrd_create(&th, thread_func, &th_arg);

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
  printf("Thread returns %d\n", th_ret);
  printf("Exit program\n");
  return EXIT_SUCCESS;
}

