#include <stdio.h>
#include <stdlib.h>

void f1()
{
	puts("f1() is working.");	
}

void f2()
{
    puts("f2() is working.");
}

void f3()
{
    puts("f3() is working.");
}


int main(int argc, char *argv[])
{
	puts("Begin!");

	atexit(f1);	
	atexit(f2);	
	atexit(f3);	


	puts("End!");

	exit(0);
}



