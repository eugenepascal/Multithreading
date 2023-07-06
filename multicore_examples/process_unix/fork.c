#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv)
{
	pid_t pid = fork();

	if(pid == -1) /* erreur */
	{
		printf("Erreur fork (%d)\n", errno);
	}
	if(pid == 0) /* fils */
	{
		printf("Je suis le fils!\n");
	}
	else /* père */
	{
		printf("Je suis le père, %u est mon fils\n", pid)                         ;
	}

  printf("Processus %u quitte\n", getpid());
	return 0;
}

