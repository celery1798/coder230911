#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proto.h"

#define IPSTRSIZE	128
#define BUFSIZE		1024

#define PROCMAX		20		
#define MINSPARESERVER	5
#define MAXSPARESERVER	10

#define SIG_NOTIFY		SIGUSR2

static struct server_st *serverpool;
static int sd;
static int count_idle,count_busy;

enum
{
	STATE_IDLE=0,
	STATE_BUSY
};

struct server_st
{
	pid_t pid;
	int state;
//	int reused;	
};

void usr2_handler(int s)
{
	return ;
}


void server_job(int sd)
{
	char buf[BUFSIZE];
	int len;

	len = sprintf(buf, FMT_STAMP, (long long)time(NULL));

	sleep(5);

	if(send(sd, buf,len , 0) < 0)
	{
		perror("send()");
		exit(1);
	}

}

void server_loop(int sd,int pos)
{
	struct sockaddr_in  raddr;
	socklen_t raddr_len;
	char ipstr[IPSTRSIZE];
	int newsd;
	pid_t ppid;

	ppid = getppid();

	raddr_len = sizeof(raddr);
	while(1)
	{
		//idle   叫父进程查看池内状态
		serverpool[pos].state = STATE_IDLE;
		kill(ppid ,SIG_NOTIFY);

		newsd = accept(sd, (void *)&raddr, &raddr_len);
		if(newsd < 0)
		{
			if(errno == EAGAIN || errno == EINTR)
				continue;
			perror("accept()");
			exit(1);
		}
		//busy  叫父进程查看状态
		serverpool[pos].state = STATE_BUSY;
		kill(ppid ,SIG_NOTIFY);

		inet_ntop(AF_INET, &raddr.sin_addr,ipstr,IPSTRSIZE);
//		printf("Client:%s[%d]\n", ipstr ,ntohs(raddr.sin_port));
		server_job(newsd);
		close(newsd);
	}
}

void scanpoll(void)
{
	int i;
	int idle = 0,busy = 0;

	for(i = 0 ; i < PROCMAX; i++)
	{
		if(serverpool[i].pid == -1)
			continue;

		if(kill(serverpool[i].pid,0))
			serverpool[i].pid = -1;

		if(serverpool[i].state == STATE_IDLE)
			idle++;
		else if(serverpool[i].state == STATE_BUSY)
				busy++;
		else
			abort();

	}

	count_busy = busy;
	count_idle = idle;
}

int add_1_server(void)
{
	pid_t pid;
	int i;
	if(count_idle + count_busy >= PROCMAX)
		return -1;

	for(i = 0 ; i < PROCMAX; i++)
		if(serverpool[i].pid == -1)
			break;
	
	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
    if(pid == 0)    // child
 	{
		server_loop(sd,i);
		close(sd);
		exit(0);
	}
	else //parent
	{
		serverpool[i].pid = pid;
		serverpool[i].state = STATE_IDLE;
		count_idle ++;
	}
	return 0;
}

int del_1_server(void )
{
	int i;

	if(count_idle <= 0)
		return -1;

	for(i = 0 ; i < PROCMAX; i++)
	{
        if(serverpool[i].pid != -1 && serverpool[i].state == STATE_IDLE)
		{
			kill(serverpool[i].pid, SIGTERM);
			serverpool[i].pid = -1;
			count_idle --;
			break;
		}
	}
	return 0;
}



int main(int argc, char *argv[])
{
	struct sockaddr_in laddr;
	int i;
	pid_t pid;

	struct sigaction sa,osa1,osa2;
	sigset_t set,oset;

	
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sa, &osa1);
	/*if error*/

	sa.sa_handler = usr2_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIG_NOTIFY, &sa, &osa2);
	/*if error*/

	sigemptyset(&set);
	sigaddset(&set, SIG_NOTIFY);
	sigprocmask(SIG_BLOCK,&set, &oset);
	/*if error*/

	//shm init
	serverpool = mmap(NULL, sizeof(struct server_st)*PROCMAX, PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	for(i = 0 ; i < PROCMAX; i++)
	{
		serverpool[i].pid = -1;
	}


	//socket
	sd = socket(AF_INET, SOCK_STREAM ,0 /*IPPROTO_TCP, IPPROTO_SCTP*/);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	int val = 1;
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&val,sizeof(val)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(SERVERPORT));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);

	if(bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	if(listen(sd, 200) < 0)
	{
		perror("listen()");
		exit(1);
	}

	for(i = 0 ; i < MINSPARESERVER; i++)
	{
		add_1_server();
	}


	while(1)
	{
		sigsuspend(&oset);

		scanpoll();  //count_idle ? count_busy ?

		if(count_idle > MAXSPARESERVER) //count_busy > 10
		{
			for(i = 0 ; i < (count_idle-MAXSPARESERVER); i++)
				del_1_server();
		}
		else
		{
			if(count_idle < MINSPARESERVER) // count_busy < 5 
			{
				for(i = 0 ; i < MINSPARESERVER-count_idle ; i++)
					add_1_server();
			}
		}

		for(i = 0 ; i < PROCMAX; i++)
		{
			if(serverpool[i].pid == -1)
				putchar(' ');
			else
			{
				if(serverpool[i].state == STATE_IDLE)
					putchar('.');
				else
					putchar('X');
			}
		}
		putchar('\n');
	}

	sigprocmask(SIG_SETMASK,&oset,NULL);
//	signal

	close(sd);
	exit(0);
}








