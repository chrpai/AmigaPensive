#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



/* ... from AmigaElm */
#include "getsystime.c"



#define NEWS_BATCHSCRIPT	"MB_DATA:UUCP/DeliverNews.scp"
#define NEWS_BATCHLOCK		"MB_DATA:UUCP/DeliverNews.lock"
#define NEWS_BATCHNUM		"MB_DATA:UUCP/DeliverNews.num"
#define NEWS_BATCHFILE		"MB_DATA:UUCP/News.%ld"



#define VERSION "1.9"
static char *version = "\0$VER: eazy2news " VERSION " " __AMIGADATE__ "";
/* static char *version = "\0$VER: eazy2news " VERSION " (" __DATE__ ")"; */

static char *day_names[] =
{"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static char *month_names[] =
{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};



void space2under(char *s)
{
	if (s) {
		while (*s) {
			if (*s == ' ')
				*s = '_';
			s++;
		}
	}
}



int main(int argc, char *argv[])
{
	if (argc == 7 && OpenTimer4MsgId()) {
		char *excld = argv[2];
		FILE *fp, *scp, *lock, *num;
		long seqnum = 0;
		char tmp[80];

		/* try to get lock */

		while (!(lock = fopen(NEWS_BATCHLOCK, "w")))
			Delay(100);

		/* we have the lock, now want the maximum seq-number */

		if (num = fopen(NEWS_BATCHNUM, "r")) {
			fscanf(num, "%ld", &seqnum);
			fclose(num);
		}

		++seqnum;

		sprintf(tmp, NEWS_BATCHFILE, seqnum);

		if (seqnum > 1) {
			while (!(scp = fopen(NEWS_BATCHSCRIPT, "a"))) {
				Delay(100);
			}
		}
		else {
			while (!(scp = fopen(NEWS_BATCHSCRIPT, "w"))) {
				Delay(100);
			}
			fprintf(scp, "failat 21\n");
			fprintf(scp, "delete %s\n", NEWS_BATCHNUM);
		}

		/* write back new seq-number while script-file is locked by us */
		if (num = fopen(NEWS_BATCHNUM, "w")) {
			fprintf(num, "%ld\n", seqnum);
			fclose(num);
		}

		/* now finish writing to script-file */
		fprintf(scp, "relaynews -x %s -r -s <%s\ndelete %s\n", excld, tmp, tmp);

		if (fp = fopen(tmp, "w")) {
			register int c = 0, last = 0;
			time_t t;
			struct timeval tv;
			struct tm *ut;
			char uname[16];
			char *sname = argv[3];
			char *ngrps = argv[1];
			char *realn = argv[5];
			char *sbjct = argv[6];

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

			/* This is not strictly MIME compliant, but who cares? */

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
		}

		/* close script-file here to prevent it from being started */
		fclose(scp);

		while (chmod(NEWS_BATCHSCRIPT, S_IWRITE | S_IREAD | S_IEXECUTE | S_IDELETE | S_ISCRIPT))
			Delay(100);

		/* close lock-file here to prevent other ports from processing news */
		fclose(lock);

		CloseTimer4MsgId();
	}
	else {
		fprintf(stderr, "\033[1;33mEazyBBS\033[0m © 1988-1995 Andreas M. Kirchwitz\n"
			"Usage: eazy2news \033[3m<newsgroup> <excl-cmd> <site> <user> <real> <subject> \033[0m <text\n");
	}

	exit(0);
}
