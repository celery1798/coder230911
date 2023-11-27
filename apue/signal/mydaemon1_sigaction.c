#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

static	FILE *fp;

int daemonize()
{
	pid_t pid;
	int fd;

	pid = fork();
	if(pid < 0)
	{
		syslog(LOG_ERR,"fork():%s",strerror(errno));
		return -1;
	}

	if(pid > 0)
		exit(0);
	//child

//	close(0); close(1); close(2);
	
	fd = open("/dev/null",O_RDWR);
	if(fd < 0)
		syslog(LOG_WARNING,"open():%s",strerror(errno));
	else
	{
		dup2(fd,0);
		dup2(fd,1);
		dup2(fd,2);
		if(fd > 2)
			close(fd);
	}

	setsid();
	/* if error -2 */

	umask(0);
	chdir("/");

	return 0;
}

void daemon_exit(int s)
{
	fclose(fp);
	closelog();
	exit(0);
}

int main(int argc, char *argv[])
{
	int i;
	struct sigaction sa;

	
//	signal(SIGINT,daemon_exit);
	sa.sa_handler = daemon_exit;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask,SIGQUIT);
	sigaddset(&sa.sa_mask,SIGTERM);
	sigaddset(&sa.sa_mask,SIGINT);
	sa.sa_flags = 0;
	sigaction(SIGINT,&sa,NULL);
	/*if error*/

//	signal(SIGQUIT,daemon_exit);
	sigaction(SIGQUIT,&sa,NULL);
    /*if error*/

//	signal(SIGTERM,daemon_exit);
	sigaction(SIGTERM,&sa,NULL);
    /*if error*/




	openlog("mydaemon",LOG_PID,LOG_DAEMON);

	//daemon
	if(daemonize())
	{
		syslog(LOG_ERR,"daemonize() failed. ");
	}
	else
	{
		syslog(LOG_INFO,"daemonize() successed. ");
	}

	fp = fopen("/tmp/out","w");
	if(fp == NULL)
	{
		syslog(LOG_ERR,"fopen():%s",strerror(errno));
		exit(1);
	}

	for(i = 0; ; i++)
	{
		fprintf(fp,"%d\n",i);
		fflush(fp);
		syslog(LOG_DEBUG,"%d was printed",i);
		sleep(1);
	}


	exit(0);
}





