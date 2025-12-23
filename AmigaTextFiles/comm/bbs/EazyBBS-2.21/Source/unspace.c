#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



#define VERSION "1.2"
static char *version = "\0$VER: unspace " VERSION " " __AMIGADATE__ "";



int main(int argc, char *argv[])
{
	int i = 1;

	while (i < argc) {
		char *s = argv[i];
		while (*s) {
			if (*s == ' ')
				putchar('_');
			else
				putchar(*s);
			++s;
		}
		if (++i < argc)
			putchar('_');
	}
	putchar('\n');

	exit(0);
}
