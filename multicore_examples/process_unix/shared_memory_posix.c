#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <semaphore.h>

static void sender(char* data, size_t data_size, sem_t* sem)
{
  strncpy (data, "Test", data_size); 
  data[data_size - 1] = 0x00;

  sleep(2);
  fprintf(stdout, "Post semaphore\n"); 
  sem_post(sem);
}

static void receiver(char* data, size_t data_size, sem_t* sem)
{
  sem_wait(sem);
  fprintf(stdout, "Wait semaphore done\n"); 
  fprintf(stdout, "Shared memory data: %s\n", data);
}


int main(int argc, char** argv)
{
  pid_t pid = -1;
  sem_t* sem = NULL;
  int fd_memory = -1;
  const size_t data_size = 1500;
  char* data = NULL;

  fd_memory = shm_open("/test_shm", O_CREAT | O_EXCL | O_RDWR,
      S_IRUSR | S_IWUSR);

  if(fd_memory == -1)
  {
    perror("fd_memory");
    exit(EXIT_FAILURE);
  }

  ftruncate(fd_memory, data_size);
  data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_memory,0);

  sem = sem_open("/test_sem", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

  if(sem == SEM_FAILED)
  {
    perror("sem_open");
    munmap(data, data_size);
    close(fd_memory);
    exit(EXIT_FAILURE);
  }

  pid = fork();

  switch(pid)
  {
  case -1:
    perror("fork");
    munmap(data, data_size);
    close(fd_memory);
    sem_close(sem);
    sem_unlink("/test_sem");
    exit(EXIT_FAILURE);
    break;
  case 0:
    /* children */
    receiver(data, data_size, sem);

    munmap(data, data_size);
    close(fd_memory);
    sem_close(sem);
    sem_destroy(sem);
    fprintf(stdout, "Child exiting\n");
    break;
  default:
    /* father */
    sender(data, data_size, sem);

    /* wait children to avoid zombies */
    waitpid(pid, NULL, 0);
    
    munmap(data, data_size);
    close(fd_memory);
    shm_unlink("/test_shm");
    sem_close(sem);
    sem_destroy(sem);
    sem_unlink("/test_sem");

    fprintf(stdout, "Father exiting\n");
    break;
  }

  return EXIT_SUCCESS;
}

