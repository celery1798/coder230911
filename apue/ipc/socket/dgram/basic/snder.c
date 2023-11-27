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
	int sd;
	struct sockaddr_in  raddr;
	socklen_t raddr_len;
	struct msg_st sbuf;	

	char ipstr[IPSTRSIZE];

	if(argc < 2)
	{
		fprintf(stderr,"Usage:...\n");
		exit(1);
	}

	sd = socket(AF_INET, SOCK_DGRAM ,0 /*IPPROTO_UDP*/);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	memset(&sbuf,'\0',sizeof(sbuf));
	strcpy(sbuf.name,"Alan");
	sbuf.math = htonl(rand()%100);
	sbuf.chinese = htonl(rand()%100);

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(RCVPORT));
	inet_pton(AF_INET, argv[1], &raddr.sin_addr);

	if(sendto(sd, &sbuf,sizeof(sbuf), 0 , (void *)&raddr,sizeof(raddr)) < 0)
	{
		perror("sendto()");
		exit(1);
	}

	puts("OK!");

	close(sd);

	exit(0);
}



