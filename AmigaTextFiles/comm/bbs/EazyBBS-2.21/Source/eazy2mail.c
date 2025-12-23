#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



#define VERSION "1.10"
static char *version = "\0$VER: eazy2mail " VERSION " " __AMIGADATE__ "";
/* static char *version = "\0$VER: eazy2mail " VERSION " (" __DATE__ ")"; */



#define MAIL_BATCHSCRIPT	"MB_DATA:UUCP/DeliverMail.scp"
#define MAIL_BATCHLOCK		"MB_DATA:UUCP/DeliverMail.lock"
#define MAIL_BATCHNUM		"MB_DATA:UUCP/DeliverMail.num"
#define MAIL_BATCHFILE		"MB_DATA:UUCP/Mail.%ld"



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
	if (argc == 6) {
		FILE *fp, *scp, *lock, *num;
		long seqnum = 0;
		char tmp[80];

		/* try to get lock */

		while (!(lock = fopen(MAIL_BATCHLOCK, "w")))
			Delay(100);

		/* we have the lock, now want the maximum seq-number */

		if (num = fopen(MAIL_BATCHNUM, "r")) {
			fscanf(num, "%ld", &seqnum);
			fclose(num);
		}

		++seqnum;

		sprintf(tmp, MAIL_BATCHFILE, seqnum);

		if (seqnum > 1) {
			while (!(scp = fopen(MAIL_BATCHSCRIPT, "a"))) {
				Delay(100);
			}
		}
		else {
			while (!(scp = fopen(MAIL_BATCHSCRIPT, "w"))) {
				Delay(100);
			}
			fprintf(scp, "failat 21\n");
			fprintf(scp, "delete %s\n", MAIL_BATCHNUM);
		}

		/* write back new seq-number while script-file is locked by us */
		if (num = fopen(MAIL_BATCHNUM, "w")) {
			fprintf(num, "%ld\n", seqnum);
			fclose(num);
		}

		/* now finish writing to script-file */
		fprintf(scp, "sendmail <%s\ndelete %s\n", tmp, tmp);

		if (fp = fopen(tmp, "w")) {
			register int c = 0, last = 0;
			time_t t;
			struct tm *ut;
			char uname[16];
			char *sname = argv[2];
			char *tonam = argv[4];
			char *realn = argv[3];
			char *sbjct = argv[5];

			time(&t);
			ut = gmtime(&t);

			strcpy(uname, argv[1]);
			space2under(uname);

			fprintf(fp, "From: %s@%s (%s)\n", uname, sname, realn);
			fprintf(fp, "To: %s\n", tonam);
			fprintf(fp, "Sender: eazybbs@%s\n", sname);
			fprintf(fp, "Subject: %s\n", sbjct);
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

		while (chmod(MAIL_BATCHSCRIPT, S_IWRITE | S_IREAD | S_IEXECUTE | S_IDELETE | S_ISCRIPT))
			Delay(100);

		/* close lock-file here to prevent other ports from processing mails */
		fclose(lock);

	}
	else {
		fprintf(stderr, "\033[1;33mEazyBBS\033[0m © 1988-1995 Andreas M. Kirchwitz\n"
			"Usage: eazy2mail \033[3m<user> <site> <real> <to> <subject> \033[0m <text\n");
	}

	exit(0);
}
