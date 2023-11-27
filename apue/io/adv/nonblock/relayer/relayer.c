#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "relayer.h"

#define BUFSIZE 1024

enum
{
	STATE_R=1,
	STATE_W,
	STATE_Ex,
	STATE_T
};

struct rel_fsa_st
{
	int state;
	int sfd,dfd;
	char buf[BUFSIZE];
	int len,pos;
	int64_t count;
	char *errstr;
};

struct rel_job_st
{
	int fd1,fd2;
	int job_state;
	pthread_mutex_t mut_job_state;
	pthread_cond_t cond_job_state;
	int fd1_save,fd2_save;
	struct rel_fsa_st fsa12,fsa21;

};


static struct rel_job_st *rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;
static pthread_t tid_relayer;


void fsa_driver(struct rel_fsa_st *fsa)
{
	int ret;
	
	switch(fsa->state)
	{
		case STATE_R:
			fsa->len = read(fsa->sfd, fsa->buf, BUFSIZE);
			if(fsa->len == 0)
				fsa->state = STATE_T;
			else
			{
				if(fsa->len < 0)
				{
					if(errno == EAGAIN)
						fsa->state = STATE_R;
					else
					{
						fsa->errstr = "read()";
						fsa->state = STATE_Ex;
					}	
				}
				else // fsa->len > 0
				{
					fsa->pos = 0;
					fsa->state = STATE_W;
				}
			}
			break;

		case STATE_W:
			ret = write(fsa->dfd, fsa->buf+fsa->pos, fsa->len);
            if(ret < 0)
            {
				if(errno == EAGAIN)
					fsa->state = STATE_W;
				else
				{
					fsa->errstr = "write()";
					fsa->state = STATE_Ex;
				}
			}
			else // ret > 0
			{
				fsa->pos += ret;
                fsa->len -= ret;
				fsa->count += ret;
				if(fsa->len == 0)
					fsa->state = STATE_R;
				else
					fsa->state = STATE_W;

			}
            break;

		case STATE_Ex:
			perror(fsa->errstr);
			fsa->state = STATE_T;
            break;

		case STATE_T:
			//exit(0);
            break;

		default:
			abort();
			break;

	}

}

void *thr_relayer(void *p)
{
	int i;

	while(1)
	{
		pthread_mutex_lock(&mut_rel_job);

		for(i = 0 ; i < REL_JOBMAX; i++)
		{
			if(rel_job[i] != NULL)
			{
				pthread_mutex_lock(&rel_job[i]->mut_job_state);
				if(rel_job[i]->job_state == STATE_RUNNING)
				{
					fsa_driver(&rel_job[i]->fsa12);
					fsa_driver(&rel_job[i]->fsa21);
				}
				if(rel_job[i]->fsa12.state == STATE_T && rel_job[i]->fsa21.state == STATE_T)
				{
					rel_job[i]->job_state = STATE_OVER;
					pthread_cond_broadcast(&rel_job[i]->cond_job_state);
				}	
				pthread_mutex_unlock(&rel_job[i]->mut_job_state);
			}
		}
		pthread_mutex_unlock(&mut_rel_job);
	}	


}


//module_unload

void module_load()
{
	int err;
	err = pthread_create(&tid_relayer,NULL, thr_relayer,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

//	atexit(module_unload);


}

static int get_free_pos_unlocked()
{
	int i;
	for(i = 0 ; i < REL_JOBMAX; i++)
		if(rel_job[i] == NULL)
			return i;
	return -1;
}


int rel_addjob(int fd1,int fd2)
{
	struct rel_job_st *me;
	int pos;



	pthread_once(&init_once,module_load);


	me = malloc(sizeof(*me));
	if(me == NULL)
		return -ENOMEM;

	me->fd1 = fd1;
	me->fd2 = fd2;
	me->job_state = STATE_RUNNING;
	me->fd1_save = fcntl(me->fd1,F_GETFL);
	fcntl(me->fd1,F_SETFL,me->fd1_save|O_NONBLOCK);
	me->fd2_save = fcntl(me->fd2,F_GETFL);
	fcntl(me->fd2,F_SETFL,me->fd2_save|O_NONBLOCK);
	
	pthread_mutex_init(&me->mut_job_state,NULL);
	pthread_cond_init(&me->cond_job_state,NULL);

	me->fsa12.state = STATE_R;
    me->fsa12.sfd = me->fd1;
    me->fsa12.dfd = me->fd2;
	me->fsa12.count = 0;
    me->fsa21.state = STATE_R;
    me->fsa21.sfd = me->fd2;
    me->fsa21.dfd = me->fd1;
	me->fsa21.count = 0;

	pthread_mutex_lock(&mut_rel_job);	

	pos = get_free_pos_unlocked();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_rel_job);
		fcntl(me->fd1,F_SETFL,me->fd1_save);
    	fcntl(me->fd2,F_SETFL,me->fd2_save);
		free(me);
		return -ENOSPC;
	}
	rel_job[pos] = me;

	pthread_mutex_unlock(&mut_rel_job);

	return pos;
}


int rel_canceljob(int id)
{
	
	pthread_mutex_lock(&mut_rel_job);
	if(id < 0 || id >= REL_JOBMAX || rel_job[id] == NULL)
	{
		pthread_mutex_unlock(&mut_rel_job);
		return -EINVAL;
	}
	pthread_mutex_unlock(&mut_rel_job);

	pthread_mutex_lock(&rel_job[id]->mut_job_state);
	if(rel_job[id]->job_state == STATE_RUNNING)
	{
		rel_job[id]->job_state = STATE_CANCELED;
		pthread_cond_broadcast(&rel_job[id]->cond_job_state);
	}
	else
	{
		if(rel_job[id]->job_state == STATE_CANCELED)
		{
			pthread_mutex_unlock(&rel_job[id]->mut_job_state);
			return -ECANCELED;
		}
		else	// STATE_OVER
		{
			pthread_mutex_unlock(&rel_job[id]->mut_job_state);
			return -EBUSY;
		}
	}
	pthread_mutex_unlock(&rel_job[id]->mut_job_state);
}

static void fetch_stat(int id,struct rel_stat_st *statp)
{
	statp->fd1 = rel_job[id]->fd1;
	statp->fd2 = rel_job[id]->fd2;
	statp->state = rel_job[id]->job_state;
	statp->count12 = rel_job[id]->fsa12.count;
	statp->count21 = rel_job[id]->fsa21.count;
}

int rel_statjob(int id,struct rel_stat_st *statp)
{
	pthread_mutex_lock(&mut_rel_job);
    if(id < 0 || id >= REL_JOBMAX || rel_job[id] == NULL)
    {
        pthread_mutex_unlock(&mut_rel_job);
        return -EINVAL;
    }
    pthread_mutex_unlock(&mut_rel_job);

	if(statp != NULL)
	{
		fetch_stat(id,statp);	
	}
	return 0;
}

int rel_waitjob(int id,struct rel_stat_st *statp)
{
	pthread_mutex_lock(&mut_rel_job);
    if(id < 0 || id >= REL_JOBMAX || rel_job[id] == NULL)
    {
        pthread_mutex_unlock(&mut_rel_job);
        return -EINVAL;
    }

    pthread_mutex_unlock(&mut_rel_job);
	

	pthread_mutex_lock(&rel_job[id]->mut_job_state);

	while(rel_job[id]->job_state == STATE_RUNNING)
		pthread_cond_wait(&rel_job[id]->cond_job_state,&rel_job[id]->mut_job_state);
	pthread_mutex_unlock(&rel_job[id]->mut_job_state);

	if(statp != NULL)
    {
        fetch_stat(id,statp);
    }

	pthread_mutex_destroy(&rel_job[id]->mut_job_state);	
	pthread_cond_destroy(&rel_job[id]->cond_job_state);

	pthread_mutex_lock(&mut_rel_job);
	free(rel_job[id]);
	rel_job[id] = NULL;
	pthread_mutex_unlock(&mut_rel_job);

    return 0;


}





