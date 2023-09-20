#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//date +%s

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	if(pid == 0)		// parent
	{
		execl("/bin/sh", "sh", "-c", "date +%s",  NULL);
		perror("execl()");
		exit(1);
	}
	else
		wait(NULL);


	exit(0);
}



