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
int at_addjob_repeat(int sec, at_jobfunc_t *jobp ,void *arg);
/*
	同上
*/

int at_canceljob(int );
/*
return: == 0        成功，指定任务已取消
		== -EINVAL  失败，参数非法
		== -EBUSY	失败，指定任务已完成
*/

int at_waitjob(int);
/*
return: == 0        成功，指定任务已释放
		== -EINVAL  失败，参数非法
		== -EBUSY   失败,指定任务为周期性任务
*/

#endif




