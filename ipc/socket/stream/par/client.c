#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "proto.h"

#define IPSTRSIZE	128
#define BUFSIZE		1024



int main(int argc, char *argv[])
{
	int sd,newsd;
	struct sockaddr_in raddr;
	char ipstr[IPSTRSIZE];

	if(argc < 2)
	{
		fprintf(stderr,"Usage:...\n");
		exit(1);
	}


	sd = socket(AF_INET, SOCK_STREAM ,0 /*IPPROTO_TCP, IPPROTO_SCTP*/);
	if(sd < 0)
	{
		perror("socket()");
		exit(1);
	}

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(atoi(SERVERPORT));
	inet_pton(AF_INET, argv[1] , &raddr.sin_addr);
	if(connect(sd, (void *)&raddr, sizeof(raddr)) < 0)
	{
		perror("connect()");
		exit(1);
	}
	
//	recv()

	FILE *fp;
	long long stamp;

	fp = fdopen(sd,"r+");
	if(fp == NULL)
	{
		perror("fdopen()");
		exit(1);
	}

	if(fscanf(fp,FMT_STAMP, &stamp) < 1)
	{
		exit(2);
	}
	else
		printf("stamp = %lld\n",stamp);
	

	fclose(fp);

	exit(0);
}



