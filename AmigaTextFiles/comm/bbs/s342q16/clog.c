#define TITLE   " CITADEL Log - Display User Utility   V3.42"
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
/*#include <clib/dos_protos.h> */
/*#include <clib/exec_protos.h>*/
#define LEGEND	"\nA indicates Aide\nE indicates Expert\n\
N indicates Network Privileges\nD indicates Door Privileges\n\
F indicates File Privileges\nP indicates Permanent Account\n\
O indicates Floor Mode\nT indicates a Twit\nR indicates a Ruggie\n"

extern CONFIG      cfg;            /* A buncha variables           */
extern logBuffer   logBuf;         /* Pippul buffer                */
extern FILE        *logfl;         /* log file descriptor          */
extern LogTable    *logTab;
extern int         thisLog;

char dodebug = FALSE;

void showlog(int i, int pw);
void ShowHash(void);
int main ( int argc , char **argv );

int main(argc,argv)
int  argc;
char **argv;
  {
  int i, entry, pw = FALSE, selected = FALSE, x, j;
  SYS_FILE fn;
  cfg.weAre = UTILITY;
  printf("%s\n%s\n", TITLE, COPYRIGHT);

  if (!readSysTab(FALSE, TRUE)) exit(1);
  sprintf(fn, "%sctdllog.sys", &cfg.logArea);
  if ((logfl = fopen(fn, "rb")) == NULL)
    {
    printf("Can't open the Citadel log!\n");
    exit(1);

    }
  initLogBuf(&logBuf);
  if (argc >= 2)
    {
    for (i = 1; i < argc; i++)
    if (strCmp(argv[i], "-P") == SAMESTRING)
      {
      pw = TRUE;
      printf("Listing Passwords.\n");

      }
    else if (strCmp(argv[i], "-D") == SAMESTRING)
      {
      dodebug = TRUE;

      }
    else selected = TRUE;

    }
  entry = 1;
  if (!selected)
    {
    for ( i = 0; i < cfg.MAXLOGTAB; i++ )
      {
      getLog(&logBuf,i);
      printf("log #%-4d",i);
      if (logBuf.lbflags.L_INUSE)
      showlog(entry++,pw);
      else
        {
        putchar('\n');

        }

      }

    }
  else
    {
    for (i = 1; i < argc; i++)
    if (strCmp(argv[i], "-P") != SAMESTRING)
      {
      x = hash(argv[i]);
      for (j = 0; j < cfg.MAXLOGTAB; j++)
        {
        if (x == logTab[j].ltnmhash)
          {
          getLog(&logBuf, logTab[j].ltlogSlot);
          if (logBuf.lbflags.L_INUSE &&
          strCmpU(argv[i], logBuf.lbname) == SAMESTRING)
          showlog(entry++, pw);

          }

        }

      }

    }
  printf(LEGEND);
  return 0;
  }
void showlog(i,pw)
int i,pw;
  {
  char *LastOn(long lastdate, char s);
  printf("%4d: %-20s",i,logBuf.lbname);
  if (pw)
    {
    printf("%-20s",logBuf.lbpw);

    }
  if (dodebug)
  ShowHash();
  /*  printf(" %sAide, %sExpert, %d col. %c%c%c%c %s\n", */
	printf(" %d col. %c%c%c%c%c%c%c%c%c %s\n",
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
  }

void ShowHash()
  {
  int i;
  for (i = 0; i < cfg.MAXLOGTAB; i++)
  if (logTab[i].ltlogSlot == thisLog)
    {
    printf("ltab=%02d, hash=%02d ", logTab[i].ltpwhash, hash(logBuf.lbpw));

    }

  }

void crashout(str)
char *str;
  {
  exit(printf(str));

  }
