#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int fd,i;
	struct stat  statres;
	char *str;
	int count = 0;

	fd = open("/etc/services",O_RDONLY);
	if(fd < 0)
	{
		perror("open()");
		exit(1);	
	}

	if(fstat(fd, &statres ) < 0)
	{
		perror("fstat()");
		exit(1);
	}


	str = mmap(NULL, statres.st_size,PROT_READ, MAP_SHARED,fd, 0);	
	if(str == MAP_FAILED)
	{
		perror("mmap()");
		exit(1);
	}
	
	for(i = 0 ; i < statres.st_size; i++)
		if(str[i] == 'a')
			count++;
	printf("%d\n",count);

	munmap(str ,statres.st_size);

	close(fd);

	exit(0);
}



