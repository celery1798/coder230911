#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>


int main(int argc, char *argv[])
{

	struct passwd *gpd;

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	gpd = getpwuid(atoi(argv[1]));
	/*if error*/
	
	puts(gpd->pw_name);


	exit(0);
}



