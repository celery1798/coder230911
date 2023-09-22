#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "mytbf.h"

typedef void (*sighandler_t)(int);

static struct mytbf_st *job[MYTBF_MAX];
static int inited = 1;
static sighandler_t alrm_handler_save;

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
};

static void alrm_handler(int s)
{
	int i;
	alarm(1);
	
	for(i = 0 ; i < MYTBF_MAX; i++)
	{
		if(job[i] != NULL)
		{
			job[i]->token += job[i]->cps;
			if(job[i]->token > job[i]->burst)
				job[i]->token = job[i]->burst;
		}
	}
}
static void module_unload()
{
	int i;
    signal(SIGALRM,alrm_handler_save);
    alarm(0);
	for(i = 0 ; i < MYTBF_MAX; i++)
        free(job[i]);
}

static void module_load()
{
	alrm_handler_save = signal(SIGALRM,alrm_handler);
	alarm(1);
	atexit(module_unload);
}

static int get_free_pos()
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

	if(inited)
	{
		module_load();
		inited = 0;
	}
	pos = get_free_pos();
	if(pos < 0)
		return NULL;

	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;

	me->cps = cps;
	me->burst = burst;
	me->token = 0;
	me->pos = pos;
	job[pos] = me;

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
		return -1;

	while(me->token <= 0)
		pause();

	n = min(me->token,size);
	me->token -= n;

	return n;
}

int mytbf_returntoken(mytbf_t *p,int size)
{
	struct mytbf_st *me = p;
	
	if(size < 0)
        return -1;

	me->token += size;
	if(me->token > me->burst)
		me->token = me->burst;
	return 0;
}

int mytbf_destroy(mytbf_t *p)
{
	struct mytbf_st *me = p;
	job[me->pos] = NULL;
	free(me);
	return 0;
}





