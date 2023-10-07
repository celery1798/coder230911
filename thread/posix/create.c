#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *func(void *p)
{
	puts("Thread is working");
//	sleep(1000);
	return NULL;
}

int main(int argc, char *argv[])
{
	int err;
	pthread_t tid;

	puts("Begin!");

	err = pthread_create(&tid,NULL,func,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

	puts("End");

//	sleep(1000);

	exit(0);
}



