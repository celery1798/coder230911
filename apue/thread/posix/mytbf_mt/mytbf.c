#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "mytbf.h"


static struct mytbf_st *job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_t tid_alrm;
static pthread_once_t inited = PTHREAD_ONCE_INIT;

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
	pthread_mutex_t mut;
};

static void *thr_alrm(void *unused)
{
	int i;
	
	while(1)
	{
		pthread_mutex_lock(&mut_job);
		for(i = 0 ; i < MYTBF_MAX; i++)
		{
			if(job[i] != NULL)
			{
				pthread_mutex_lock(&job[i]->mut);
				job[i]->token += job[i]->cps;
				if(job[i]->token > job[i]->burst)
					job[i]->token = job[i]->burst;
				pthread_mutex_unlock(&job[i]->mut);	
			}
		}
		pthread_mutex_unlock(&mut_job);	
		sleep(1);
	}

	pthread_exit(NULL);
}
static void module_unload()
{
	int i;

	pthread_cancel(tid_alrm);
	pthread_join(tid_alrm,NULL);

	/*free*/

	pthread_mutex_destroy(&mut_job);	
}

static void module_load()
{
	int err;

	err = pthread_create(&tid_alrm,NULL, thr_alrm,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

	atexit(module_unload);
}

static int get_free_pos_unlocked()
{
	int i;
	for(i = 0 ; i < MYTBF_MAX; i++)
		if(job[i] == NULL)
			return i;
	return -1;
}

mytbf_t *mytbf_init(int cps,int burst)
{
	struct mytbf_st *me;
	int pos;

	pthread_once(&inited,module_load);
/*
	lock(a);
	if(inited)
	{
		inited = 0;
		module_load();
	}	
	unlock(a);
	*/

	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;

	me->cps = cps;
	me->burst = burst;
	me->token = 0;
	pthread_mutex_init(&me->mut,NULL);

	pthread_mutex_lock(&mut_job);

	pos = get_free_pos_unlocked();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_job);
		return NULL;
	}	
	job[pos] = me;

	pthread_mutex_unlock(&mut_job);

	me->pos = pos;

	return me;

}

static int min(int a,int b)
{
	if(a < b)
		return a;
	return b;
}

int mytbf_fetchtoken(mytbf_t *p,int size)
{
	struct mytbf_st *me = p;
	int n;

	if(size < 0)
		return -EINVAL;

	pthread_mutex_lock(&me->mut);
	while(me->token <= 0)
	{
		pthread_mutex_unlock(&me->mut);
		sched_yield();
		pthread_mutex_lock(&me->mut);
	}

	n = min(me->token,size);
	me->token -= n;
	pthread_mutex_unlock(&me->mut);

	return n;
}

int mytbf_returntoken(mytbf_t *p,int size)
{
	struct mytbf_st *me = p;
	
	if(size < 0)
        return -1;

	pthread_mutex_lock(&me->mut);
	me->token += size;
	if(me->token > me->burst)
		me->token = me->burst;
	pthread_mutex_unlock(&me->mut);
	return 0;
}

int mytbf_destroy(mytbf_t *p)
{
	struct mytbf_st *me = p;
	pthread_mutex_lock(&mut_job);
	job[me->pos] = NULL;
	pthread_mutex_unlock(&mut_job);

	pthread_mutex_destroy(&me->mut);
	free(me);
	return 0;
}





