#include <stdio.h>
#include <unistd.h>

/*
详见5.4节
行缓冲：换行的时候刷新，满了的时候刷新，强制刷新（标准输出就是这种缓冲模式）
全缓冲：满了的时候刷新，强制刷新（默认使用全缓冲，除了终端设备）
无缓冲：需要立刻输出，如stderr
*/



int main()
{
/*
	int i = 0;

	printf("befor while() i = %d\n",i);

	while(  1  )
	{
		i++;
		sleep(1);
	printf("while() i = %d\n",i);
		if(i < -1)
			break;
	}

	printf("i = %d\n",i);
*/

	int i;

	for(i = 0 ; i < 5; i++)
	{
		putchar('X');
//		fflush(stdout);
		fflush(NULL);
		sleep(1);
	}	

	putchar('\n');

	return 0;
}








