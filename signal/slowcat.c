#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
	int sfd,dfd = 1;
	char buf[BUFSIZE];
	int len,ret,pos;

	if(argc < 2)
	{
		fprintf(stdout,"Usage: %s <src_file> <dest_file>\n",argv[0]);
		exit(1);
	}

	do{
		sfd = open(argv[1],O_RDONLY);
		if(sfd < 0)
		{
			if(errno != EINTR)
			{	
				perror("open()");
				exit(1);
			}
		}
	}while(sfd < 0);
	

	while(1)
	{
			len = read(sfd, buf, BUFSIZE);
			if(len < 0)
			{
				if(errno ==EINTR)
					continue;
				perror("read()");
				exit(1);
			}
			if(len == 0)
					break;
			// len > 0

			pos = 0;
			while(len > 0)
			{
					ret = write(dfd, buf+pos, len);
					if(ret < 0)
					{	
						if(errno == EINTR)
                    		continue;		
						perror("write()");
						exit(1);
					}
					pos += ret;
					len -= ret;
			}
	}
	close(sfd);

	exit(0);
}



