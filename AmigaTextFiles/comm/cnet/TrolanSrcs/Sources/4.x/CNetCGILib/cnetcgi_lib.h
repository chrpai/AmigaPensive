/*
** CNet CGI Header Definitions
** by Kelly Cochran
** Last updated: Sat Feb 07 14:04:43 1998
*/

#ifndef CNETCGI_LIB_H
#define CNETCGI_LIB_H

extern struct MainPort *myp;
extern struct Library  *CNetBase;
extern struct Library  *CNet4Base;

BOOL new_mail_check(char *uucp, char *date);
void PrintHour(struct IsDate *date, char *dout);
void unescape_url(char *url);
void plustospace(char *str);
char x2c(char *what);
BOOL auth_user(struct UserData *udata);
char *getenv(const char *name);
char *makeword(char *line, char stop);
void getword(char *word, char *line, char stop);

#endif
