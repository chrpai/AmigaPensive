#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



#define VERSION "1.0"
static char *version = "\0$VER: killline " VERSION " " __AMIGADATE__ "";
static char line[1024],match[1024];



int main(int argc, char *argv[])
{
	char *p;
	int i=1;
	BOOL okay = TRUE;

	strcpy(match,"");
	while (i < argc) {
		strcat(match,argv[i]);
		if (++i < argc)
			strcat(match," ");
	}
	if (argc > 1)
		strcat(match,"\n");

	while (fgets(line,1024,stdin)) {
		if (p=strchr(line,'\n')) {
			if (!okay || strcmp(line,match)!=0)
				fputs(line,stdout);
			okay = TRUE;
		}
		else {
			fputs(line,stdout);
			okay = FALSE;
		}
	}

	exit(0);
}

