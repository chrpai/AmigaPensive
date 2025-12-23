
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



/* ... from AmigaElm */
#include "getsystime.c"



#define VERSION "1.7"
static char *version = "\0$VER: eazy2news " VERSION " " __AMIGADATE__ "";
static char *day_names[] =
{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static char *month_names[] =
{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};



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
	if (argc == 7 && OpenTimer4MsgId()) {
		char tmp[80];
		FILE *fp;

		sprintf(tmp, "%s%ld", "MB_TMP:Eazy2News.tmp", (long) FindTask(NULL));

		if (fp = fopen(tmp, "w")) {
			register int c = 0, last = 0;
			time_t t;
			struct timeval tv;
			struct tm *ut;
			char uname[16];
			char *sname = argv[3];
			char *ngrps = argv[1];
			char *excld = argv[2];
			char *realn = argv[5];
			char *sbjct = argv[6];
			char exec[256];

			/* for Date: */
			time(&t);
			ut = gmtime(&t);

			/* for Message-Id: */
			GetTimer4MsgId(&tv);

			strcpy(uname, argv[4]);
			space2under(uname);

			fprintf(fp, "Path: news\n");
			fprintf(fp, "Newsgroups: %s\n", ngrps);
			fprintf(fp, "From: %s@%s (%s)\n", uname, sname, realn);
			fprintf(fp, "Sender: eazybbs@%s\n", sname);
			fprintf(fp, "Subject: %s\n", sbjct);
			fprintf(fp, "Message-Id: <%lx.%lx-eazybbs@%s>\n", tv.tv_secs, tv.tv_micro, sname);
			fprintf(fp, "Date: %s, %.2d %s %d %.2d:%.2d:%.2d %sGMT\n",
				day_names[ut->tm_wday],
				ut->tm_mday, month_names[ut->tm_mon], ut->tm_year + 1900,
				ut->tm_hour, ut->tm_min, ut->tm_sec, ut->tm_isdst ? "DST" : "");
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

			sprintf(exec, "relaynews -x %s -r -s <%s", excld, tmp);
			System(exec, NULL);
			DeleteFile(tmp);
		}

		CloseTimer4MsgId();

	}
	else {
		fprintf(stderr, "\033[1;33mEazyBBS\033[0m © 1988-1994 Andreas M. Kirchwitz\n"
			"Usage: eazy2news \033[3m<newsgroup> <excl-cmd> <site> <user> <real> <subject> \033[0m <text\n");
	}

	exit(0);
}
