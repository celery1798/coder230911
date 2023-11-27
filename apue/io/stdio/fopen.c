#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	FILE *fp;

	fp = fopen("a.txt","w");
	
	if(fp == NULL)
	{
		printf("fopen():%s\n",strerror(errno));		
//		printf("errno = %d\n",errno);
//		perror("fopen()");
		exit(1);	
	}
	printf("ok\n");

	fclose(fp);

	exit(0);
}



