#ifndef _ANYTIMER_H__
#define _ANYTIMER_H__

#define JOB_MAX		1024

typedef void at_jobfunc_t(void *);

int at_addjob(int sec, at_jobfunc_t *jobp ,void *arg);
/*
return:	>= 0		成功，返回任务id号
		== -EINVAL	失败，参数非法
		== -ENOMEM	失败，空间申请失败
		== -ENOSPC	失败，任务数组满
*/




#endif




