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
#include <sys/epoll.h>
	

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
	int epfd;
	struct epoll_event ev;

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

	epfd = epoll_create(10);
	if(epfd < 0)
	{
		perror("epoll_create()");
		exit(1);
	}

	ev.data.fd = fd1;
    ev.events = 0;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd1,&ev);
	ev.data.fd = fd2;
	ev.events = 0;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd2,&ev);

	while(fsa12.state != STATE_T || fsa21.state != STATE_T)
	{
	//布置监视任务
		ev.data.fd = fd1;
		ev.events = 0;
		if(fsa12.state == STATE_R)
			ev.events |= EPOLLIN;
		if(fsa21.state == STATE_W)
			ev.events |= EPOLLOUT;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &ev);
	

		ev.data.fd = fd2;
		ev.events = 0;
		if(fsa12.state == STATE_W)
			ev.events |= EPOLLOUT;
		if(fsa21.state == STATE_R)
			ev.events |= EPOLLIN;
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &ev);

	//监视
		if(fsa12.state < STATE_AUTO || fsa21.state < STATE_AUTO)
		{
			while(epoll_wait(epfd, &ev, 1, -1) < 0 )
			{
				if(errno == EINTR)
					continue;
				perror("epoll_wait()");	
				exit(1);
			}
		}
	//获取监视结果
		if( ev.data.fd == fd1 && ev.events & EPOLLIN || \
		ev.data.fd == fd2 && ev.events & EPOLLOUT || \
		fsa12.state > STATE_AUTO)
			fsa_driver(&fsa12);
		if( ev.data.fd == fd2 && ev.events & EPOLLIN || \
		ev.data.fd == fd1 && ev.events & EPOLLOUT || \
		fsa21.state > STATE_AUTO)
			fsa_driver(&fsa21);
	}
	
	fcntl(fd1,F_SETFL,fd1_save);
	fcntl(fd2,F_SETFL,fd2_save);

	close(epfd);	//!!!!!
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


