#include <stdio.h>
#include <unistd.h>

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








