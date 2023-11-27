#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define PAT		"/etc"

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *cur;
	
	dp = opendir(PAT);
	/*if error*/	

	while((cur = readdir(dp)) != NULL)
	{
		puts(cur->d_name);	
	}

	closedir(dp);

	exit(0);
}



