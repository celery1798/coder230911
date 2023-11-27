#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *func(void *p)
{
	puts("Thread is working");

	pthread_exit(NULL);
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

	pthread_join(tid ,NULL);

	puts("End");

	exit(0);
}



