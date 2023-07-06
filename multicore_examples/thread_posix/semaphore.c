#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

static sem_t sem;

void* thread_func(void* arg)
{
  size_t num = (size_t)arg;

  if(sem_wait(&sem) != -1)
  {
    printf("Hello from thread %zu\n", num);
    sleep(1);
  }
  else
  {
    perror("sem_wait");
  }
  sem_post(&sem);

  return NULL;
}

int main(int argc, char** argv)
{
  size_t nb = 10;
  pthread_t th[nb];
  void* th_ret = NULL;
  int ret = 0;

  if(sem_init(&sem, 0, 2) == -1)
  {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    ret = pthread_create(&th[i], NULL, thread_func, (void*)i);

    if(ret != 0)
    {
      printf("Error creating thread (%d)\n", ret);
    }
  }

  for(size_t i = 0 ; i < nb ; i++)
  {
    /* wait threads to terminate */
    pthread_join(th[i], &th_ret);
  }

  sem_destroy(&sem);

  printf("Exit program\n");
  return EXIT_SUCCESS;
}

