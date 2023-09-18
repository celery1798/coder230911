#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define STRSIZE	1024

int main(int argc,char *argv[])
{
	struct tm *tm;
	time_t stamp;
	char timestr[STRSIZE];
	char fmt[STRSIZE] = {'\0'};
	int ch;

	stamp = time(NULL);
	tm = localtime(&stamp);

	while(1)
	{
		ch = getopt(argc, argv, "y:mdH:MS");
		if(ch < 0)
			break;
		
		switch(ch)
		{
			case 'y':
				if(strcmp(optarg,"2") == 0)
					strcat(fmt,"%y ");
				else if(strcmp(optarg,"4") == 0)
						strcat(fmt,"%Y ");
				else
					fprintf(stderr,"Ivaild argument of -y.\n");
				break;

			case 'm':
				strcat(fmt,"%m ");
				break;

			case 'd':
				strcat(fmt,"%d ");
				break;

			case 'H':
				if(strcmp(optarg,"12") == 0)
                    strcat(fmt,"%I(%P) ");
                else if(strcmp(optarg,"24") == 0)
                        strcat(fmt,"%H ");
                else
                    fprintf(stderr,"Ivaild argument of -H.\n");
                break;

			case 'M':
				strcat(fmt,"%M ");
				break;

			case 'S':
				strcat(fmt,"%S ");
				break;

			default:
				break;
		}

	}


	strftime(timestr,STRSIZE, fmt ,tm);	
	puts(timestr);


	exit(0);

}




