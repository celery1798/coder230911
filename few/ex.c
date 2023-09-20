#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//date +%s

int main(int argc, char *argv[])
{
	puts("Begin!");

	fflush(NULL);

	execl("/usr/bin/date","date","+%s",NULL);
	perror("execl()");
	exit(1);

	puts("End!");

	exit(0);
}



