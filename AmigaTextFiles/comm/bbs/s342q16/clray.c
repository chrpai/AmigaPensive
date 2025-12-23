/**
***     This program allows inspection of the Citadel log
***     displaying misc things about the user.
***     Modified by H.A. White, May 2, 1984.
***     Modified by HAW Nov 2, 1988, to display net flags & copyright notice.
**/
#include "ctdl.h"    /* header file  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"

#define LEGEND	"\nA indicates Aide\nE indicates Expert\n\
N indicates Network Privileges\nD indicates Door Privileges\n\
F indicates File Privileges\nP indicates Permanent Account\n\
O indicates Floor Mode\nT indicates a Twit\nR indicates a Ruggie\n"

char dodebug;
char doUsers;

#define TITLE   " CITADEL Log - Display User Utility   V3.42"

extern CONFIG    cfg;            /* A buncha variables           */
extern LogTable    *logTab;        /* RAM index of pippuls         */
extern logBuffer logBuf;         /* Pippul buffer                */
extern FILE             *logfl;         /* log file descriptor          */

FILE *fopen();
int showlog(int i, int x);
int main(int, char **);
int main(argc, argv)
char **argv;
int argc;
{
    int i, entry=0;
    SYS_FILE fn;

    printf("%s\n%s\n\n", TITLE, COPYRIGHT);
    dodebug = doUsers = FALSE;
    while(--argc > 0 )
      {
      if( argv[argc][0] == '-' )
        {
        switch (argv[argc][1])
          {
          case 'd' :
          case 'D' : dodebug = TRUE; break;
          case 'u' :
          case 'U' : doUsers = TRUE; break;
          default  : printf(" Illegal option:%s\n",argv[argc]);
          };
        }
      else printf(" Illegal argument:%s\n",argv[argc]);
      };
    cfg.weAre = UTILITY;
    if (!readSysTab(FALSE, TRUE)) exit(1);
    makeSysName(fn, "ctdllog.sys", &cfg.logArea);
    if ((logfl = fopen(fn, "rb")) == NULL) {
        printf("Sorry can't open the Citadel log!  Please inform the Sysop.\n");
        exit(1);
    }
    initLogBuf(&logBuf);
    for (i = 0; i < cfg.MAXLOGTAB; i++) {
        if (logTab[i].ltpwhash != 0 &&
            logTab[i].ltnmhash != 0) {
            getLog(&logBuf,logTab[i].ltlogSlot);
            if (showlog(entry, logTab[i].ltlogSlot)) entry++;
        }
    }
    printf("\n\n%d valid entries.\n", entry);
    if( doUsers )printf(LEGEND);
return 0;
}

int showlog(i,x)
int i, x;
{
    char *LastOn(long lastdate, char s);

    if (logBuf.lbflags.L_INUSE)
      {
      if( doUsers ) printf("\nacct #%3d %4d:",x,i);
      printf(" %-20s",logBuf.lbname);
      if (dodebug) printf("%8ld", logBuf.lbvisit[0]);
      if( doUsers )printf(" %d col. %c%c%c%c%c%c%c%c%c %s",
			logBuf.lbwidth,
			logBuf.lbflags.AIDE       ? 'A' : ' ',
			logBuf.lbflags.EXPERT     ? 'E' : ' ',
			logBuf.lbflags.NET_PRIVS  ? 'N' : ' ',
			logBuf.lbflags.DOOR_PRIVS ? 'D' : ' ',
			logBuf.lbflags.DL_PRIVS   ? 'F' : ' ',
			logBuf.lbflags.PERMANENT  ? 'P' : ' ',
      logBuf.lbflags.FLOORS     ? 'O' : ' ',
      logBuf.lbflags.TWIT       ? 'T' : ' ',
      logBuf.lbflags.RUGGIE     ? 'R' : ' ',
			LastOn(logBuf.lblaston, TRUE));
      if( (i % 3) == 2 && !doUsers )printf("\n");
      return TRUE;
    }
    printf("\n");
    return FALSE;
}

void crashout(str)
char *str;
{
    exit(printf(str));
}
