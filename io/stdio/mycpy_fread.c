#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 128

int main(int argc, char *argv[])
{
	FILE *fps,*fpd;
	char buf[BUFSIZE];
	int n;

	if(argc < 3)
	{
		fprintf(stdout,"Usage: %s <src_file> <dest_file>\n",argv[0]);
		exit(1);
	}

	if((fps = fopen(argv[1],"r")) == NULL)
	{
		perror("fopen()");
		exit(1);	
	}
	if((fpd = fopen(argv[2],"w")) == NULL)
    {
        perror("fopen()");
        exit(1);    
    }

	while((n = fread(buf,1,BUFSIZE,fps)) > 0)
		fwrite(buf,1,n,fpd);
	
	fclose(fps);
	fclose(fpd);

	exit(0);
}



