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
	pthread_attr_t attr;

	puts("Begin!");


	pthread_attr_init(&attr);

	pthread_attr_setstacksize(&attr,1024*1024);

	err = pthread_create(&tid,&attr,func,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

	pthread_join(tid ,NULL);

	pthread_attr_destroy(&attr);

	puts("End");

	exit(0);
}



