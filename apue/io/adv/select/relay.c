#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
	

#define BUFSIZE 1024

#define TTY1	"/dev/tty11"
#define TTY2	"/dev/tty12"

enum
{
	STATE_R=1,
	STATE_W,
STATE_AUTO,
	STATE_Ex,
	STATE_T
};

struct fsa_st
{
	int state;
	int sfd,dfd;
	char buf[BUFSIZE];
	int len,pos;
	char *errstr;
};

void fsa_driver(struct fsa_st *fsa)
{
	int ret;
	
	switch(fsa->state)
	{
		case STATE_R:
			fsa->len = read(fsa->sfd, fsa->buf, BUFSIZE);
			if(fsa->len == 0)
				fsa->state = STATE_T;
			else
			{
				if(fsa->len < 0)
				{
					if(errno == EAGAIN)
						fsa->state = STATE_R;
					else
					{
						fsa->errstr = "read()";
						fsa->state = STATE_Ex;
					}	
				}
				else // fsa->len > 0
				{
					fsa->pos = 0;
					fsa->state = STATE_W;
				}
			}
			break;

		case STATE_W:
			ret = write(fsa->dfd, fsa->buf+fsa->pos, fsa->len);
            if(ret < 0)
            {
				if(errno == EAGAIN)
					fsa->state = STATE_W;
				else
				{
					fsa->errstr = "write()";
					fsa->state = STATE_Ex;
				}
			}
			else // ret > 0
			{
				fsa->pos += ret;
                fsa->len -= ret;
				if(fsa->len == 0)
					fsa->state = STATE_R;
				else
					fsa->state = STATE_W;

			}
            break;

		case STATE_Ex:
			perror(fsa->errstr);
			fsa->state = STATE_T;
            break;

		case STATE_T:
			//exit(0);
            break;

		default:
			abort();
			break;

	}

}

int max(int a,int b)
{
	if(a > b)
		return a;
	return b;
}

void relay(int fd1,int fd2)
{
	int fd1_save,fd2_save;	
	struct fsa_st fsa12,fsa21;
	fd_set rset,wset;

	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);
	fd2_save = fcntl(fd2,F_GETFL);
    fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);

	fsa12.state = STATE_R;
	fsa12.sfd = fd1;
	fsa12.dfd = fd2;
	fsa21.state = STATE_R;
	fsa21.sfd = fd2;
	fsa21.dfd = fd1;

	while(fsa12.state != STATE_T || fsa21.state != STATE_T)
	{
	//布置监视任务
		FD_ZERO(&rset);
		FD_ZERO(&wset);

		if(fsa12.state == STATE_R)
			FD_SET(fsa12.sfd, &rset);
		if(fsa12.state == STATE_W)
			FD_SET(fsa12.dfd, &wset);
		if(fsa21.state == STATE_R)
			FD_SET(fsa21.sfd, &rset);
		if(fsa21.state == STATE_W)
			FD_SET(fsa21.dfd, &wset);

	//监视
		if(fsa12.state < STATE_AUTO || fsa21.state < STATE_AUTO)
		{
			if(select(max(fd1,fd2)+1, &rset, &wset, NULL, NULL) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("select()");	
				exit(1);
			}
		}
	//获取监视结果
		if(FD_ISSET(fd1,&rset) || FD_ISSET(fd2,&wset) || fsa12.state > STATE_AUTO)
			fsa_driver(&fsa12);
		if(FD_ISSET(fd2,&rset) || FD_ISSET(fd1,&wset) || fsa21.state > STATE_AUTO)
			fsa_driver(&fsa21);
	}
	
	fcntl(fd1,F_SETFL,fd1_save);
	fcntl(fd2,F_SETFL,fd2_save);
}


int main(int argc, char *argv[])
{
	int fd1,fd2;

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


	relay(fd1,fd2);


	//xxxx


	close(fd1);
	close(fd2);

	exit(0);

}	


