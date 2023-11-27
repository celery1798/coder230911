#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
#pragma omp parallel sections
{
#pragma omp section
	printf("[%d]:hello\n",omp_get_thread_num());

#pragma omp section	
	printf("[%d]:world\n",omp_get_thread_num());
}
	exit(0);
}



