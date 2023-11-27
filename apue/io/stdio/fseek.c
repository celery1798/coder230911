#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	FILE *fp;

	fp = fopen("a.txt","r+");
	
	if(fp == NULL)
	{
		printf("fopen():%s\n",strerror(errno));		
		exit(1);	
	}

	fseek(fp,3,SEEK_SET);
	fputc('X',fp);
	printf("%ld\n",ftell(fp));

	fclose(fp);

	exit(0);
}



