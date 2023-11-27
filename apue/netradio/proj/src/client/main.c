
#include <stdio.h>

//#include "../include/proto.h"
#include <proto.h>

int main()
{
		
	//  搭建socket

	//	获取节目单，打印，获取用户输入

	//	接收用户所选定的频道包，交给子进程播放


	pipe();

	pid = fork()
	if(pid < 0)
	{

	}
	if(pid == 0)
	{
		close(pd[1]);

		dup(pd[0],0);

		fd = open("/dev/null",O_RDWR);

		dup2(fd,1);
		dup2(fd,2);

		execl("/usr/local/bin/mpg123","mpg123","-",NULL);


	}
	else // parent
	{

		while(  )
		{
		recvfrom();
		
		写管道；
		}
	}


}







