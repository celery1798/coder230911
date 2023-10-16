#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "proto.h"



int main(int argc, char *argv[])
{
	key_t key;
	struct msg_st rbuf;	
	int msgid;

	key = ftok(KEYPATH,KEYPROJ);
	if(key < 0)
	{
		perror("ftok()");
		exit(1);
	}

	msgid = msgget(key, IPC_CREAT|0600);
	if(msgid < 0)
	{
		perror("msgget()");
		exit(1);
	}

	while(1)
	{
		if(msgrcv(msgid, &rbuf, sizeof(rbuf)-sizeof(long), 0, 0) < 0)
		{
			perror("msgrcv()");
			exit(1);
		}
		if(rbuf.mtype == MYMSGTYPE)
		{
			printf("Name:%s\n",rbuf.name);
			printf("Math:%d\n",rbuf.math);
			printf("Chinese:%d\n",rbuf.chinese);
		}	
	}
	msgctl(msgid, IPC_RMID, NULL);

	exit(0);
}



