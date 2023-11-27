#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fps,*fpd;
	int ch;

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

	while((ch = fgetc(fps)) != EOF)
		fputc(ch,fpd);
	
	fclose(fps);
	fclose(fpd);

	exit(0);
}



