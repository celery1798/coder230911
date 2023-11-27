#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

.....

xxf2xxxf1xxf1


void f1(void *p)
{
	puts(p);
}
void f2(void *p)
{
	puts(p);
}

int main(int argc, char *argv[])
{
	int job1;
	
	puts("Begin");

	job1 = at_addjob(5,f1,"aaa");
	if(job1 < 0)
	{
		strerror(-job);
		exit(1;);
	}
	at_addjob(2,f2,"bbb");
	at_addjob(7,f1,"ccc");

	puts("End");

	while(1)
	{
		write(1,".",1)
		sleep(1);
	}
	exit(0);
}



