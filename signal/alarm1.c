#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	int i;

	alarm(5);
	alarm(10);
	alarm(1);

	
	while(1);

	exit(0);
}



