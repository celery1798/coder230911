#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>
#include <string.h>

#define PAT		"/etc/a*.conf"

int myerrfunc(const char *epath, int eerrno)
{
	printf("-----%s:%s\n",epath,strerror(eerrno));
}

int main(int argc, char *argv[])
{
	glob_t globres;
	int err,i;

	err = glob(PAT,0,NULL/*myerrfunc*/,&globres);
	if(err)
	{
		printf("err = %d\n",err);
		exit(1);
	}

	for(i = 0 ;i < globres.gl_pathc; i++)
		puts(globres.gl_pathv[i]);

	exit(0);
}



