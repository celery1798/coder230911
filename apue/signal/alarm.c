#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void alrm_handler(int s)
{
	write(1,"!",1);
}

int main(int argc, char *argv[])
{
	int i;

	signal(SIGALRM,alrm_handler);
	alarm(5);

	for(i = 0 ; i < 10; i++)
	{
		write(1,"*",1);	
		sleep(1);
	}

	exit(0);
}



