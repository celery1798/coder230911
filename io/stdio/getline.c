#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


ssize_t mygetline(char **lineptr, size_t *n, FILE *stream)
{

}

int main(int argc, char *argv[])
{
	FILE *fp;
	char *linebuf = NULL;
	size_t linesize = 0;

	if(argc < 2)
	{
		fprintf(stdout,"Usage: %s <src_file>\n",argv[0]);
		exit(1);
	}

	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);	
	}

	while(1)
	{
		if(mygetline( &linebuf, &linesize,fp) < 0)
		{	
			perror("getline()");
			exit(1);
		}
		printf("linesize = %ld\n",linesize);
		fputs(linebuf,stdout);
	}	


	fclose(fp);

	exit(0);
}



