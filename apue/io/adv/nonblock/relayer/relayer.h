#ifndef _RELAYER_H__
#define _RELAYER_H__

#define REL_JOBMAX		1024


enum
{
	STATE_RUNNING=1,
	STATE_CANCELED,
	STATE_OVER
};
struct rel_stat_st
{
	int fd1,fd2;
	int state;
	int64_t count12,count21;
	//time
};


int rel_addjob(int fd1,int fd2);

int rel_canceljob(int );

int rel_statjob(int ,struct rel_stat_st *);

int rel_waitjob(int ,struct rel_stat_st *);

#endif




