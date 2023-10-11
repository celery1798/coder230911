#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT	30000000
#define RIGHT	30000200

int main(int argc, char *argv[])
{
	int i,j,mark;
	
#pragma omp parallel for private(j,mark)	
	for(i = LEFT; i <= RIGHT; i++)
	{
		mark = 1;		
		for(j = 2; j < i/2; j++)
		{
			if(i % j == 0)
			{
				mark = 0;
				break;
			}	
		}
		if(mark)
			printf("%d is a primer.\n",i);

	}


	exit(0);
}



