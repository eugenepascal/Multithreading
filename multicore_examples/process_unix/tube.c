#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	pid_t pid;
  /* fd[0] = read
   * fd[1] = write
   */
  int fd[2];

  if(pipe(fd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
	if(pid == -1) /* erreur */
	{
    perror("fork");
    close(fd[0]);
    close(fd[1]);
    exit(EXIT_FAILURE);
	}

	if(pid == 0) /* child */
	{
    char data[1500];

    close(fd[1]);
    while(read(fd[0], data, sizeof(data)) > 0)
    {
      fprintf(stdout, "Child reads: %s\n", data);
    }

    close(fd[0]);
	}
	else /* father */
	{
    ssize_t ret = -1;

    close(fd[0]);

    ret = write(fd[1], "Test fd", strlen("Test tube"));
    if(ret == -1)
    {
      perror("write");
    }

    printf("Father writes %zd bytes\n", ret);

    close(fd[1]);
    waitpid(pid, NULL, 0);
	}

  printf("Process %u ends\n", getpid());
	return 0;
}

