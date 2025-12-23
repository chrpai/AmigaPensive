#define TITLE   " CITADEL - Convert User Utility   V3.42"
/**
***     This program creates one file per Citadel log user
***     in the format that ADDUSER can use.  The filenames will
***     be in the format:  Lognnn.ao
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

extern CONFIG      cfg;            /* A buncha variables           */
extern logBuffer   logBuf;         /* Pippul buffer                */
extern FILE        *logfl;         /* log file descriptor          */
extern LogTable    *logTab;
extern int         thisLog;

void showlog(int i);
int  main ( int, char **);

int main(argc,argv)
int  argc;
char **argv;
  {
  int i;
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
  for ( i = 0; i < cfg.MAXLOGTAB; i++ )
    {
    getLog(&logBuf,i);
    printf("log #%-4d\r",i);
    if (logBuf.lbflags.L_INUSE) showlog(i);
    }
  return 0;
  }
void showlog(i)
int i;
  {
  FILE *user;
  char filename[30];
  sprintf(filename,"log%03d.ao",i);
  user = fopen(filename,"w");
  if( user == NULL )
    {
    printf("Could not open file %s for user:%s\n",filename,logBuf.lbname);
    return;
    };
  fprintf(user,"%s\n",logBuf.lbname);
  fprintf(user,"%s\n",logBuf.lbpw);
  if( logBuf.lbwidth < 40)logBuf.lbwidth = 40;
  if( logBuf.lbwidth > 80)logBuf.lbwidth = 80;
  fprintf(user,"%d\n",logBuf.lbwidth);
  fprintf(user,"%s\n",logBuf.lbflags.EXPERT       ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.FLOORS       ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.LFMASK       ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.TIME         ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.OLDTOO       ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.AIDE         ? "ON" : "OFF");
  fprintf(user,"%d\n",logBuf.lbnulls);
  fprintf(user,"%d\n",logBuf.credit);
  fprintf(user,"%s\n",logBuf.lbflags.NET_PRIVS    ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.DOOR_PRIVS   ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.DL_PRIVS     ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.PERMANENT    ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.HALF_DUP     ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.TWIT         ? "ON" : "OFF");
  fprintf(user,"%d\n",logBuf.lbdelay);
  fprintf(user,"%s\n",logBuf.lbflags.ALT_RE       ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.NoPrompt     ? "ON" : "OFF");
  fprintf(user,"%s\n",logBuf.lbflags.RUGGIE     ? "ON" : "OFF");
  fclose(user);
  }
void crashout(str)
char *str;
  {
  exit(printf(str));

  }
