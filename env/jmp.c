#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf save;

void d()
{   
    printf("[%s]Begin\n",__FUNCTION__);
    printf("[%s]Jump now!\n",__FUNCTION__);
	longjmp(save,0);
    printf("[%s]End\n",__FUNCTION__);
}
void c()
{   
    printf("[%s]Begin\n",__FUNCTION__);
    printf("[%s]Call d()\n",__FUNCTION__);
    d();
    printf("[%s]d() returned\n",__FUNCTION__);
    printf("[%s]End\n",__FUNCTION__);
}
void b()
{   
    printf("[%s]Begin\n",__FUNCTION__);
    printf("[%s]Call c()\n",__FUNCTION__);
    c();
    printf("[%s]c() returned\n",__FUNCTION__);
    printf("[%s]End\n",__FUNCTION__);
}

void a()
{
	int ret;
    printf("[%s]Begin\n",__FUNCTION__);
	ret = setjmp(save);
	if(ret == 0)
	{
    	printf("[%s]Call b()\n",__FUNCTION__);
    	b();
		printf("[%s]b() returned\n",__FUNCTION__);
    }
	else
		printf("[%s]Jumped back here with code %d\n",__FUNCTION__,ret);
    printf("[%s]End\n",__FUNCTION__);
}

int main(int argc, char *argv[])
{
	printf("[%s]Begin\n",__FUNCTION__);
	printf("[%s]Call a()\n",__FUNCTION__);
	a();
	printf("[%s]a() returned\n",__FUNCTION__);
	printf("[%s]End\n",__FUNCTION__);

	exit(0);
}



