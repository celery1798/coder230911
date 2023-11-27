#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <glob.h>
#include <string.h>

#define DELIMS	" \t\n"

struct cmd_st
{
	glob_t globres;
//	int flag1;
//	char arr[10];
};


void prompt()
{
	printf("mysh-0.1$ ");	
}

void parse(char *line, struct cmd_st *res)
{
// ls    -l   -i  /etc  -> "ls" "-l" ..
	char *tok;
	int i = 0;

	while(1)
	{
		tok = strsep(&line, DELIMS);	
		if(tok == NULL)
			break;
		if(tok[0] == '\0')
			continue;
		glob(tok, GLOB_NOCHECK|GLOB_APPEND*i ,NULL, &res->globres);
		i = 1;
	}
}

void parsefree(struct cmd_st *res)
{
	globfree(&res->globres)

}


int main(int argc, char *argv[])
{
	char *linebuf = NULL;
	size_t linesize = 0;
	struct cmd_st cmd;
	pid_t pid;

	while(1)
	{
		//命令行提示符
		prompt();

		//接受输入
		if(getline(&linebuf,&linesize,stdin) < 0)
			break;

		//解析命令行上的内容
		
		parse(linebuf, &cmd);


		//实现指定命令的功能

		if( 0 /*内部命令*/)
		{
		
		}
		else		/*外部命令*/
		{
				pid = fork();
				if(pid < 0)
				{
						perror("fork()");
						exit(1);
				}
				if(pid == 0)
				{
						execvp(cmd.globres.gl_pathv[0] ,cmd.globres.gl_pathv );
						perror("execvp()");
						exit(1);
				}
				else
						wait(NULL);
		}
		parsefree(&cmd);
	}

	exit(0);
}	










