#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int daemonize()
{
	pid_t pid;
	int fd;

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		return -1;
	}

	if(pid > 0)
		exit(0);
	//child

//	close(0); close(1); close(2);
	
	fd = open("/dev/null",O_RDWR);
	if(fd < 0)
		perror("open()");
	else
	{
		dup2(fd,0);
		dup2(fd,1);
		dup2(fd,2);
		if(fd > 2)
			close(fd);
	}

	setsid();
	/* if error -2 */

	umask(0);
	chdir("/");

	return 0;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	int i;

	//daemon
	if(daemonize())
		exit(1);


	fp = fopen("/tmp/out","w");
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);
	}

	for(i = 0; ; i++)
	{
		fprintf(fp,"%d\n",i);
		fflush(fp);
		sleep(1);
	}

	fclose(fp);

	exit(0);
}





