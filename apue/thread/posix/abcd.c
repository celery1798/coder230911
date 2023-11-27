#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

#define THRNUM		4
#define BUFSIZE		128

static pthread_mutex_t mut[THRNUM];

int next(int n)
{
	if(n+1 == THRNUM)	
		return 0;
	return n+1;	
}

void *thr_func(void *p)
{
	int n = (int)p;
	int ch = 'a'+ n;

	while(1)
	{
		pthread_mutex_lock(mut+n);
		write(1, &ch ,1);
		pthread_mutex_unlock(mut+next(n));
	}
	pthread_exit(NULL);				

}

int main(int argc, char *argv[])
{
	int i,err;
	pthread_t tid[THRNUM];
	
	for(i = 0; i < THRNUM; i++)
	{
		pthread_mutex_init(mut+i,NULL);
		pthread_mutex_lock(mut+i);
		err = pthread_create(tid+i,NULL, thr_func, (void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);
		}
	}

	pthread_mutex_unlock(mut+0);

	alarm(5);

	for(i = 0; i < THRNUM; i++)
		pthread_join(tid[i],NULL);


	exit(0);
}



