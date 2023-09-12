#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	FILE *fp;
	int ch;

	fp = fopen("a.txt","r");
	
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);	
	}

	ch = fgetc(fp);
	if(ch == EOF)
	{
		printf("error!\n");
		exit(2);
	}
	fputc(ch,stdout);

	fclose(fp);

	exit(0);
}



