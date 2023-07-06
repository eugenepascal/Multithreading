#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <mqueue.h>

static void receiver(int fd, size_t data_size)
{
  char data[data_size];
  unsigned int priority = 0;
  ssize_t len = -1;

  len = mq_receive(fd, data, data_size, &priority);

  if(len == -1)
  {
    perror("mq_receive");
    return;
  }
 
  data[len] = 0x00;
  fprintf(stdout, "Data received: %s, priority: %d\n", data, priority);

  if(mq_close(fd) == -1)
  {
    perror("mq_close");
  }
}

static void sender(int fd, size_t data_size)
{
  char data[data_size];

  strncpy(data, "Test", data_size);
  data[data_size - 1] = 0x00;

  if(mq_send(fd, data, data_size, 8) != 0)
  {
    perror("mq_send");
  }

  if(mq_close(fd) == -1)
  {
    perror("mq_close");
  }

  if(mq_unlink("/test_mq") == -1)
  {
    perror("mq_unlink");
  }
}

int main(int argc, char** argv)
{
  int fd = -1;
  pid_t pid = -1;
  size_t data_size = 0;
  struct mq_attr attr;

  fd = mq_open("/test_mq", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, NULL);
  if(fd == (mqd_t)-1)
  {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  if(mq_getattr(fd, &attr) != -1)
  {
    fprintf(stdout, "Max message: %ld, max size: %ld\n", 
        attr.mq_maxmsg, attr.mq_msgsize);

    data_size = attr.mq_msgsize;
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
    receiver(fd, data_size);

    fprintf(stdout, "Child exiting\n");
    break;
  default:
    /* father */
    sender(fd, data_size);

    /* wait children to avoid zombies */
    waitpid(pid, NULL, 0);
    
    fprintf(stdout, "Father exiting\n");
    break;
  }

  return EXIT_SUCCESS;
}

