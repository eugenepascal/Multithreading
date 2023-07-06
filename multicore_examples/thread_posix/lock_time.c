#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>

#include <pthread.h>

pthread_mutex_t lock;
pthread_spinlock_t spinlock;
pthread_barrier_t bar;

static void spinlock_lock(pthread_spinlock_t* spin)
{
  struct timespec tv;

  /* 25 us */
  tv.tv_sec = 0;
  tv.tv_nsec = 25000;

  while(pthread_spin_trylock(spin) != 0)
  {
    nanosleep(&tv, NULL);
  }
}

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
  if((stop->tv_nsec - start->tv_nsec) < 0)
  {
    result->tv_sec = stop->tv_sec - start->tv_sec - 1;
    result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
  }
  else
  {
    result->tv_sec = stop->tv_sec - start->tv_sec;
    result->tv_nsec = stop->tv_nsec - start->tv_nsec;
  }

  return;
}

void* task(void* arg)
{
	struct timespec start;
	struct timespec stop;
	struct timespec result;
	struct timespec total;
  int cpt = 0;
  _Atomic int cpt_atomic = ATOMIC_VAR_INIT(0);

  (void)arg;

  memset(&total, 0x00, sizeof(struct timespec));
  pthread_barrier_wait(&bar);

  /* atomic */
  for(unsigned int i = 0 ; i < 90000 ; i++)
  {
    clock_gettime(CLOCK_REALTIME, &start);
    atomic_fetch_add(&cpt_atomic, 1);
    clock_gettime(CLOCK_REALTIME, &stop);

    timespec_diff(&start, &stop, &result);
    total.tv_sec += result.tv_sec;
    total.tv_nsec += result.tv_nsec;
  }
    
  printf("Execution time %ld.%09ld s (atomic)\n", total.tv_sec / 90000,
      total.tv_nsec / 90000);
  
  memset(&total, 0x00, sizeof(struct timespec));
  pthread_barrier_wait(&bar);

  /* no lock */
  for(unsigned int i = 0 ; i < 90000 ; i++)
  {
    clock_gettime(CLOCK_REALTIME, &start);
    cpt++;
    clock_gettime(CLOCK_REALTIME, &stop);

    timespec_diff(&start, &stop, &result);
    total.tv_sec += result.tv_sec;
    total.tv_nsec += result.tv_nsec;
  }
  printf("Execution time %ld.%09ld s (no lock)\n", total.tv_sec / 90000,
      total.tv_nsec / 90000);
  
  memset(&total, 0x00, sizeof(struct timespec));
  pthread_barrier_wait(&bar);

  /* mutex */
  for(unsigned int i = 0 ; i < 90000 ; i++)
  {
    clock_gettime(CLOCK_REALTIME, &start);
    pthread_mutex_lock(&lock);
    cpt++;
    pthread_mutex_unlock(&lock);
    clock_gettime(CLOCK_REALTIME, &stop);

    timespec_diff(&start, &stop, &result);
    total.tv_sec += result.tv_sec;
    total.tv_nsec += result.tv_nsec;
  }
  printf("Execution time %ld.%09ld s (pthread mutex)\n", total.tv_sec / 90000,
      total.tv_nsec / 90000);
 
  memset(&total, 0x00, sizeof(struct timespec));
  pthread_barrier_wait(&bar);
  
  /* spinlock */
  for(unsigned int i = 0 ; i < 90000 ; i++)
  {
    clock_gettime(CLOCK_REALTIME, &start);
    spinlock_lock(&spinlock);
    cpt++;
    //sleep(10);
    pthread_spin_unlock(&spinlock);
    clock_gettime(CLOCK_REALTIME, &stop);
    
    timespec_diff(&start, &stop, &result);
    total.tv_sec += result.tv_sec;
    total.tv_nsec += result.tv_nsec;
  }
  printf("Execution time %ld.%09ld s (pthread spinlock)\n", total.tv_sec / 90000,
      total.tv_nsec / 90000);
  return NULL;
}

int main(int argc, char** argv)
{
  pthread_t id[2];
  
  pthread_mutex_init(&lock, NULL);
  pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

  for(size_t i = 0 ; i < 2 ; i++)
  {
    if(pthread_create(&id[i], NULL, task, NULL) != 0)
    {
      perror("pthread_create");
      exit(EXIT_SUCCESS);
    }
  }

  pthread_barrier_init(&bar, NULL, 2);

  for(size_t i = 0 ; i < 2 ; i++)
  {
    pthread_join(id[i], NULL);
  }

	return EXIT_SUCCESS;
}

