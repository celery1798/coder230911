#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{

	puts( getenv("PWD"));


	exit(0);
}



