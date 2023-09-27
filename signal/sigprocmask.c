#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void int_handler(int s)
{
	write(1,"!",1);
}

int main(int argc, char *argv[])
{
	int i;
	sigset_t set,oset,saveset;

	signal(SIGINT,int_handler);
	
	sigemptyset(&set);
	sigaddset(&set,SIGINT);


	sigprocmask(SIG_UNBLOCK, &set, &saveset);
	while(1)
	{
//		sigprocmask(SIG_BLOCK, &set, NULL);
		sigprocmask(SIG_BLOCK, &set, &oset);
		for(i = 0 ; i < 5; i++)
		{
			write(1,"*",1);	
			sleep(1);
		}
		write(1,"\n",1);
//		sigprocmask(SIG_UNBLOCK, &set, NULL);
		sigprocmask(SIG_SETMASK, &oset, NULL);
	}

	sigprocmask(SIG_SETMASK,&saveset,NULL);

//	SIGINT 恢复
	exit(0);
}








