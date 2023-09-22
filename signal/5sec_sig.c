#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static volatile int loop = 1;

void alrm_handler(int s)
{
	loop = 0;
}

int main(int argc, char *argv[])
{
	int i;
	int64_t	count = 0;

	signal(SIGALRM,alrm_handler);
	alarm(5);

	while(loop)
		count++;
	printf("%ld\n",count);

	exit(0);
}



