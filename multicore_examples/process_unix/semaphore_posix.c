#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <semaphore.h>

static void process(sem_t* sem)
{
  sem_wait(sem); 
  {
    fprintf(stdout, "%u access zone!\n", getpid());
    sleep(2);
  }
  sem_post(sem);
}

int main(int argc, char** argv)
{
  pid_t pid = -1;
  sem_t* sem = NULL;

  sem = sem_open("/test_sem", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 2);

  if(sem == NULL)
  {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  pid = fork();

  switch(pid)
  {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
    break;
  case 0:
    /* children */
    process(sem);

    fprintf(stdout, "Child exiting\n");
    sem_close(sem);
    sem_destroy(sem);
    break;
  default:
    /* father */
    process(sem);

    /* wait children to avoid zombies */
    waitpid(pid, NULL, 0);
    
    fprintf(stdout, "Father exiting\n");
    sem_close(sem);
    sem_destroy(sem);
    sem_unlink("/test_sem");
    break;
  }

  return EXIT_SUCCESS;
}

