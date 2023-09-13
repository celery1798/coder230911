#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	putchar('a');
	write(1,"b",1);
	
	putchar('a');
	write(1,"b",1);

	putchar('a');
	write(1,"b",1);


	exit(0);
}



