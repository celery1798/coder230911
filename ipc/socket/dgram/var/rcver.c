#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>


#include "proto.h"

#define IPSTRSIZE	128

int main(int argc, char *argv[])
{
	int sd,size;
	struct sockaddr_in laddr, raddr;
	socklen_t raddr_len;
	struct msg_st *rbuf;	
	char ipstr[IPSTRSIZE];

	sd = socket(AF_INET, SOCK_DGRAM ,0 /*IPPROTO_UDP*/);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);

	if(bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
	{
		perror("bind()");
		exit(1);
	}

	size = sizeof(*rbuf) + NAMEMAX - 1;
	rbuf = malloc(size);
	if(rbuf == NULL)
		exit(1);


	raddr_len = sizeof(raddr);
	while(1)
	{

		if(recvfrom(sd, rbuf,size, 0 , (void *)&raddr, &raddr_len) < 0)
		{
			perror("recvfrom()");
			exit(1);
		}
	
		inet_ntop(AF_INET, &raddr.sin_addr, ipstr,IPSTRSIZE);
		printf("--MESSAGE FROM:%s[%d]--\n", ipstr, ntohs(raddr.sin_port));

		printf("Name:%s\n",rbuf->name);
		printf("Math:%d\n",ntohl(rbuf->math));
		printf("Chinese:%d\n",ntohl(rbuf->chinese));
	}

	close(sd);
	free(rbuf);

	exit(0);
}



