#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;

	fd = open("/tmp/out",O_WRONLY|O_CREAT|O_TRUNC,0644);
	/*if error*/

//	close(1);
//	dup(fd);

	dup2(fd, 1);

	if(fd != 1)
		close(fd);
/*****************************/	
	puts("hello");


//恢复

	exit(0);
}



