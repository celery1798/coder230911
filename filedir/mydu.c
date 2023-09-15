#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>
#include <string.h>

#define PATHSIZE	1024

int path_noloop(const char *path)
{// /etc/.  /etc/..        /etc/..a... /etc/aaa/bbbb/ccc/.a. 

	char *ptr;
	ptr = strrchr(path,'/');
	/*if error*/

	if(strcmp(ptr+1,".") ==0 || strcmp(ptr+1,"..") == 0)
		return 0;
	return 1;
}


int64_t mydu(const char *path)
{
	struct stat statres;
	char nextpath[PATHSIZE];
	int64_t sum;
	glob_t globres;
	int i;

	if(lstat(path,&statres) < 0)
	{
		perror("lstat()");
		exit(1);
	}

	if(!S_ISDIR(statres.st_mode))	//is not a dir
	{
		return statres.st_blocks;
	}

	// is a dir . path = "/etc"
	//	"/etc/*"


	strncpy(nextpath,path,512);
	strncat(nextpath,"/*",5);
	glob(nextpath,0, NULL, &globres);

	// "/etc/.*"

	strncpy(nextpath,path,512);
    strncat(nextpath,"/.*",5);
    glob(nextpath,GLOB_APPEND,NULL, &globres);

	sum = statres.st_blocks;
    for(i = 0 ; i < globres.gl_pathc ; i++)
	{	
		if(path_noloop(globres.gl_pathv[i]))
        	sum += mydu(globres.gl_pathv[i]);
	}	
	return sum;
}


int main(int argc, char *argv[])
{

	if(argc != 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	printf("%lld\n",(long long)mydu(argv[1])/2);


	exit(0);
}



