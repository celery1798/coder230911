#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//sleep 1000

int main(int argc, char *argv[])
{
	pid_t pid;

	puts("Begin!");

	fflush(NULL);

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	if(pid == 0)		// parent
	{

		//execl("/usr/bin/sleep","sleep","1000",NULL);
		execl("/usr/bin/sleep","httpd","1000",NULL);
		perror("execl()");
		exit(1);
	}
	else
		wait(NULL);

	puts("End!");

	exit(0);
}



