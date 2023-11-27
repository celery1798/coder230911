#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "relayer.h"


#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"
#define TTY3	"/dev/tty10"
#define TTY4	"/dev/tty9"

int main(int argc, char *argv[])
{
	int fd1,fd2,job1,fd3,fd4,job2;
	struct rel_stat_st st;

	if((fd1 = open(TTY1,O_RDWR)) < 0)
	{
		perror("open()");
		exit(1);	
	}
	write(fd1,"TTY1\n",5);

	if((fd2 = open(TTY2,O_RDWR|O_NONBLOCK)) < 0)
	{
		perror("open()");
		exit(1);	
	}
	write(fd2,"TTY2\n",5);


	job1 = rel_addjob(fd1,fd2);
	if(job1 < 0 )
	{
		fprintf(stderr,"rel_addjob():%s\n",strerror(-job1));
		exit(1);
	}



	if((fd3 = open(TTY3,O_RDWR)) < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd3,"TTY3\n",5);

    if((fd4 = open(TTY4,O_RDWR|O_NONBLOCK)) < 0)
    {
        perror("open()");
        exit(1);
    }
    write(fd4,"TTY4\n",5);


    job2 = rel_addjob(fd3,fd4);
    if(job2 < 0  )
    {
		fprintf(stderr,"rel_addjob():%s\n",strerror(-job2));
        exit(1);

    }



	while(1)
	{
		rel_statjob(job1,&st);
		printf("job1:1->2(%ld)\t2->1(%ld)\n",st.count12,st.count21);
		rel_statjob(job2,&st);
		printf("job2:1->2(%ld)\t2->1(%ld)\n\n",st.count12,st.count21);
		sleep(1);
	}


	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);

	exit(0);

}	


