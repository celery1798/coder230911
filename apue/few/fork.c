#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;

	printf("[%d]Begin!",getpid());
	
	fflush(NULL);

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	if(pid > 0)		// parent
	{
		printf("[%d]Parent is working\n",getpid());
	}
	else			//child
	{
		printf("[%d]Child is working\n",getpid());
	}

	printf("[%d]End!\n",getpid());

	sleep(1000);

	exit(0);
}



