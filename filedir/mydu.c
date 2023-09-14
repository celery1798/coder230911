#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>
#include <string.h>

int main(int argc, char *argv[])
{

	if(argc != 2)
	{


	}

	// argv[1] is not a dir




	// argv[1] is a dir   /etc

	glob("/etc/*",,,,)			-> "/etc/vim"  is or isnot a dir	
									/etc/vim/*   /etc/vim/.*

	glob("/etc/.*",,,,)


	exit(0);
}



