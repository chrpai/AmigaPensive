
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <proto/dos.h>
#include <proto/exec.h>



#define VERSION "1.4"
static char *version = "\0$VER: mail2eazy " VERSION " " __AMIGADATE__ "";
static char allargs[1024];



void space2under(char *s)
{
	while (s && *s) {
		if (*s == ' ')
			*s = '_';
		s++;
	}
}



void kill_quotes(char *s)
{
	while (s && *s) {
		if (*s == '\"')
			*s = '\'';
		s++;
	}
}



char *DOS_escape(char *str)
/* put * (escape char) before " and * */
{
	if (str) {
		char *s = str;
		while (*s) {
			if (*s == '"' || *s == '*') {
				strins(s, "*");
				s++;
			}
			s++;
		}
		/* strins(str,"\""); */
		/* strcat(str,"\""); */
	}
	return (str);
}



int main(int argc, char *argv[])
{
	int i=1;

	strcpy(allargs,"");
	while (i < argc) {
		strcat(allargs,argv[i]);
		if (++i < argc)
			strcat(allargs," ");
	}

	if (argc > 1) {
		char tmp[80];
		FILE *fp;

		sprintf(tmp, "%s%ld", "MB_TMP:Mail2Eazy.tmp", (long) FindTask(NULL));

		if (fp = fopen(tmp, "w")) {
			register int c = 0;
			char username[256];
			char uucpadr[256];
			char *to_user = allargs;
			char subject[256];
			char exec[256];
			char str[1024];
			BOOL ende = FALSE;

			strcpy(username, "unknown");
			strcpy(subject, "none");
			strcpy(uucpadr, "");

			while (!ende && fgets(str, 1023, stdin)) {
				if (strlen(str) == 0 || strcmp(str, "\n") == 0)
					ende = TRUE;
				else if ((strnicmp(str, "From: ", 6) == 0 && strlen(uucpadr) == 0) || strnicmp(str, "Reply-To: ", 10) == 0) {
					char *p;
					char *str2;
					str2 = strstr(str, ": ") + 2;

					strcpy(uucpadr, str2);
					kill_quotes(uucpadr);
					if (strchr(uucpadr, '\n'))
						*strchr(uucpadr, '\n') = 0;

#if 0
					strcpy(username, str2);
					p = username;
					while (*p && *p != '@') {
						p++;
					}
					if (*p == '@')
						*p = 0;
#endif
					if (p = strchr(str2, '@')) {
						int pos;
						for (pos = 0, p--; p >= str2 && *p != ' ' && *p != '\t' && *p != '<'; p--)
							username[pos++] = *p;
						username[pos] = 0;
						strrev(username);
					}

					if (strchr(username, '\n'))
						*strchr(username, '\n') = 0;
					username[15] = 0;
					kill_quotes(username);
				}
				else if (strncmp(str, "Subject: ", 9) == 0) {
					strcpy(subject, str + 9);
					kill_quotes(subject);
					if (strchr(subject, '\n'))
						*strchr(subject, '\n') = 0;
					subject[40] = 0;
				}
			}

			while ((c = fgetc(stdin)) != EOF) {
				if (c == '\\')
					fputc('\\', fp);
				fputc(c, fp);
			}
			fprintf(fp, "\n");

			fclose(fp);

#if 0
			DOS_escape(username);
			DOS_escape(to_user);
			DOS_escape(uucpadr);
			DOS_escape(subject);
#endif

			sprintf(exec, "MB:C/ImportMail \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"", username, to_user, uucpadr, subject, tmp);
			System(exec, NULL);
			DeleteFile(tmp);
		}
	}
	else {
		fprintf(stderr, "\033[1;33mEazyBBS\033[0m © 1988-1995 Andreas M. Kirchwitz\n"
			"Usage: mail2eazy \033[3m<to> \033[0m <text\n");
	}

	exit(0);
}
