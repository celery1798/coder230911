#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
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
#define PROCNUM		4


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

void server_loop(int sd)
{
	struct sockaddr_in  raddr;
	socklen_t raddr_len;
	char ipstr[IPSTRSIZE];
	int newsd;

	raddr_len = sizeof(raddr);
	while(1)
	{

		newsd = accept(sd, (void *)&raddr, &raddr_len);
		if(newsd < 0)
		{
			if(errno == EAGAIN || errno == EINTR)
				continue;
			perror("accept()");
			exit(1);
		}
		
		inet_ntop(AF_INET, &raddr.sin_addr,ipstr,IPSTRSIZE);
		printf("Client:%s[%d]\n", ipstr ,ntohs(raddr.sin_port));
		server_job(newsd);
		close(newsd);
	}
}


int main(int argc, char *argv[])
{
	int sd;
	struct sockaddr_in laddr;
	int i;
	pid_t pid;

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

	for(i = 0 ; i < PROCNUM; i++)
	{
		pid = fork();
    	if(pid < 0)
 	   {
    	    perror("fork()");
        	exit(1);
	    }
    	if(pid == 0)    // child
 	   {
			server_loop(sd);
			close(sd);
			exit(0);
		}
	}	


	for(i = 0 ; i < PROCNUM; i++)
		wait(NULL);

	close(sd);

	exit(0);

}




