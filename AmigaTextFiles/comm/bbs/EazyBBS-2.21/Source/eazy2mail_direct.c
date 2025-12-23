
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



#define VERSION "1.7"
static char *version = "\0$VER: eazy2mail " VERSION " " __AMIGADATE__ "";



void space2under(char *s)
{
	while (s && *s) {
		if (*s == ' ')
			*s = '_';
		s++;
	}
}



int main(int argc, char *argv[])
{
	if (argc == 6) {
		char tmp[80];
		FILE *fp;

		sprintf(tmp, "%s%ld", "MB_TMP:Eazy2Mail.tmp", (long) FindTask(NULL));

		if (fp = fopen(tmp, "w")) {
			register int c = 0, last = 0;
			time_t t;
			struct tm *ut;
			char uname[16];
			char *sname = argv[2];
			char *tonam = argv[4];
			char *realn = argv[3];
			char *sbjct = argv[5];
			char exec[256];

			time(&t);
			ut = gmtime(&t);

			strcpy(uname, argv[1]);
			space2under(uname);

			fprintf(fp, "From: %s@%s (%s)\n", uname, sname, realn);
			fprintf(fp, "To: %s\n", tonam);
			fprintf(fp, "Sender: eazybbs@%s\n", sname);
			fprintf(fp, "Subject: %s\n", sbjct);
			fprintf(fp, "MIME-Version: 1.0\n");
			fprintf(fp, "Content-Type: text/plain; charset=ISO-8859-1\n");
			fprintf(fp, "Content-Transfer-Encoding: 8bit\n");
			fprintf(fp, "\n");

			while ((c = getchar()) != EOF) {
				if (last == '\\') {
					switch (c) {
					case '\\':
						fputc(c, fp);
						break;
					default:
						break;
					}
					last = 0;
				}
				else if (c == '\\') {
					last = c;
				}
				else {
					fputc(c, fp);
					last = c;
				}
			}
			fprintf(fp, "\n");

			fclose(fp);

			sprintf(exec, "sendmail <%s", tmp);
			System(exec, NULL);
			DeleteFile(tmp);
		}
	}
	else {
		fprintf(stderr, "\033[1;33mEazyBBS\033[0m © 1988-1993 Andreas M. Kirchwitz\n"
			"Usage: eazy2mail \033[3m<user> <site> <real> <to> <subject> \033[0m <text\n");
	}

	exit(0);
}
