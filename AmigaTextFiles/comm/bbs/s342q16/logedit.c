/*
*                               Logedit.c
*
* Log editor for Citadel-86, V 2.x.
*/
/*
*                               history
*
* 89Apr16 HAW  New visual support for PC Clones.  (non-portable)
* 87Oct11 HAW  V3 update.
* 85Nov16 HAW  Modified for MS-DOS libraries.
* 85Aug10 HAW  Created.
*/
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
/*
*                               Contents
*
*       crashout()              irrecoverable error
*       getUNumber()            Gets number from console
*       getUtilString()         Gets string from console
*       logSum()                Sums up the log for logTab
*       main()                  Main controller
*/
char NoShow = FALSE;
extern CONFIG    cfg;
extern SListBase MailForward;
extern LogTable *logTab;        /* RAM index of pippuls */
extern logBuffer logBuf;        /* Pippul buffer        */
extern FILE     *logfl;       /* log file descriptor    */
extern int      thisLog;
void getUtilString(char *prompt, char *buf, int lim);
void logSum(void);
void crashout ( char *str );
void handleEditing ( void );
int getUNumber ( char *prompt , unsigned bottom , unsigned top );
void getUtilString ( char *prompt , char *buf , int lim );
int logSort ( LogTable *s1 , LogTable *s2 );
int main ( int argc , char **argv );
void showlog ( void );
void Display_Flag (short,short, char *);

/*
* crashout()
*
* This handles an irrecoverable error.
*/
void crashout(str)
char *str;
  {
  printf(str);
  exit(10);

  }
/*
* handleEditing()
*
* This handles editing accounts in a very primitive manner.
*/
void handleEditing()
  {
  char c;
  char mess[40];
  int account;
  label NewName;
  logBuffer lBuf;
  ForwardMail *data;
  initLogBuf(&lBuf);
  sPrintf(mess, "Which account (%d to quit): ", cfg.MAXLOGTAB);
  account = getUNumber(mess, 0, cfg.MAXLOGTAB);
  getLog(&logBuf, account);
  while (account != cfg.MAXLOGTAB)
    {
    showlog();
    printf("LogEdit Menu\n");
    printf("A - toggle Active Acct   \t S - toggle Aide Status\n");
    printf("E - toggle Expert mode   \t N - toggle Net Privileges\n");
    printf("D - toggle Door Privs    \t L - toggle Download Privileges\n");
    printf("B - toggle Permanent Acct\t F - toggle Floor mode\n");
    printf("T - toggle Twit Status   \t R - toggle Ruggie Status\n");
    printf("C - change user name     \n");
    printf("W - change column width  \t G - Go to New account\n");
    printf("J - change LD Net Credit \t Q - Quit\n");
    c = getchar();
    while( getchar() != '\n');
    switch( c )
      {
      case 'j':     /* Net Credit */
      case 'J':
        sPrintf(mess, "Currently %d credits, Enter New:", logBuf.credit);
        logBuf.credit = (int) getUNumber(mess,1,1000);
        putLog(&logBuf, account);
        break;
      case 'w':
      case 'W':
        sprintf(mess,"Enter new column width(40-80):");
        logBuf.lbwidth = getUNumber(mess,40,80);
        putLog(&logBuf, account);
        break;
      case 'g':
      case 'G': /* go to next account */
        sPrintf(mess, "Which account (%d to quit): ", cfg.MAXLOGTAB);
        account = getUNumber(mess, 0, cfg.MAXLOGTAB);
        getLog(&logBuf, account);
        break;
      case 'a':
      case 'A': /* activate an account */
      if (logBuf.lbflags.L_INUSE != 1)
        {
        logBuf.lbflags.L_INUSE = 1;

        }
      else
        {
        logBuf.lbflags.L_INUSE = 0;
        if (SearchList(&MailForward, logBuf.lbname) != NULL)
          {
          KillData(&MailForward, logBuf.lbname);
          UpdateForwarding();

          };

        };
      putLog(&logBuf, account);
      break;
      case 'q' :
      case 'Q' :  /* Quit */
      return;
      break;
      case 'C' :  /* Change a User's name */
      case 'c' :
      getUtilString("new name", NewName, NAMESIZE);
      if (strLen(NewName) != 0)
        {
        if (findPerson(NewName, &lBuf) != ERROR)
          {
          printf("%s already in use.\n", NewName);

          }
        else
          {
          if ((data = SearchList(&MailForward,logBuf.lbname)) != NULL)
            {
            data->UserName = strdup(NewName);
            KillData(&MailForward, logBuf.lbname);
            UpdateForwarding();

            };
          strCpy(logBuf.lbname, NewName);
          putLog(&logBuf, account);

          };

        }
      else printf("No name entered, aborted change\n");
      break;
      case 'S' :
      case 's' :
      logBuf.lbflags.AIDE =  (logBuf.lbflags.AIDE) ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'E' :
      case 'e' :
      logBuf.lbflags.EXPERT   = logBuf.lbflags.EXPERT ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'N' :
      case 'n' :
      logBuf.lbflags.NET_PRIVS= logBuf.lbflags.NET_PRIVS ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'D' :
      case 'd' :
      logBuf.lbflags.DOOR_PRIVS = logBuf.lbflags.DOOR_PRIVS ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'L' :
      case 'l' :
      logBuf.lbflags.DL_PRIVS = logBuf.lbflags.DL_PRIVS ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'B' :
      case 'b' :
      logBuf.lbflags.PERMANENT= logBuf.lbflags.PERMANENT ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'F' :
      case 'f' :
      logBuf.lbflags.FLOORS   = logBuf.lbflags.FLOORS ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'T' :
      case 't' :
      logBuf.lbflags.TWIT     = logBuf.lbflags.TWIT   ? 0 : 1;
      putLog(&logBuf, account);
      break;
      case 'R' :
      case 'r' :
      logBuf.lbflags.RUGGIE   = logBuf.lbflags.RUGGIE ? 0 : 1;
      putLog(&logBuf, account);
      break;
      Default:  printf("Invalid function Choice\n");

      };

    };
  /* Loop until User enters Quit */

  }
/*
* getUNumber()
*
* This prompts for a number in (bottom, top) range.
*/
int getUNumber(prompt, bottom, top)
char   *prompt;
unsigned bottom;
unsigned top;
  {
  unsigned try;
  char numstring[NAMESIZE];
  do
    {
    getUtilString(prompt, numstring, NAMESIZE);
    try     = atoi(numstring);
    if (try < bottom)  printf("Sorry, must be at least %d\n", bottom);
    if (try > top   )  printf("Sorry, must be no more than %d\n", top);

    }
  while (try < bottom ||  try > top);
  return (int)try;

  }
/*
* getUtilString()
*
* This function gets a string from the user.
*/
void getUtilString(prompt, buf, lim)
char *prompt;
char *buf;
int  lim;       /* max # chars to read */
  {
  char *c;
  short x;
  printf("Enter %s:", prompt, lim);
  c = fgets(buf,lim,stdin);
  for(x=0; x<lim;x++)if( buf[x] == '\n')buf[x] = '\0';
  printf("\n");

  }
/*
* logSort()
*
* This is used for sorting the log.
*/
int logSort(s1, s2)
LogTable *s1, *s2;
  {
  if (s1->ltnmhash == 0 && s2->ltnmhash == 0)
  return 0;
  if (s1->ltnmhash == 0 && s2->ltnmhash != 0)
  return 1;
  if (s1->ltnmhash != 0 && s2->ltnmhash == 0)
  return -1;
  if (s1->ltnewest < s2->ltnewest)
  return 1;
  if (s1->ltnewest > s2->ltnewest)
  return -1;
  return 0;

  }
/*
* logSum()
*
* This goes through the log to correct logTab.
*/
void logSum()
  {
  int i;
  int logSort();
  int count = 0;
  #ifdef WORKS
  if (rewind(logfl) != 0) crashout("Rewinding logfl failed!");
  #else
  rewind(logfl);
  #endif
  /* clear logTab */
  for (i = 0;i < cfg.MAXLOGTAB;i++) logTab[i].ltnewest = 0l;
  /* load logTab: */
  for (thisLog = 0;thisLog < cfg.MAXLOGTAB;thisLog++)
    {
    getLog(&logBuf, thisLog);
    /* count valid entries:     */
    if (logBuf.lbflags.L_INUSE)
      {
      if (strLen(logBuf.lbname) >= NAMESIZE ||
      strLen(logBuf.lbpw) >= NAMESIZE )
        {
        logBuf.lbflags.L_INUSE = FALSE;
        putLog(&logBuf, thisLog);

        }
      else
        {
        count++;

        }

      };
    /* copy relevant info into index:   */
    logTab[thisLog].ltnewest = logBuf.lbvisit[0];
    logTab[thisLog].ltlogSlot= thisLog;
    if (logBuf.lbflags.L_INUSE == 1)
      {
      logTab[thisLog].ltnmhash = hash(logBuf.lbname);
      logTab[thisLog].ltpwhash = hash(logBuf.lbpw  );
      logTab[thisLog].ltpermanent = logBuf.lbflags.PERMANENT;

      }
    else
      {
      logTab[thisLog].ltnmhash = 0;
      logTab[thisLog].ltpwhash = 0;

      }

    }
  printf(" logInit--%d valid log entries\n", count);
  qsort((char *)logTab, cfg.MAXLOGTAB, cfg.sizeLTentry, logSort);

  }
/*
* main()
*
* Main controller.
*/
main(argc, argv)
int argc;
char **argv;
  {
  SYS_FILE logfile;
  printf("LOGEDIT V3.42\n%s\n", COPYRIGHT);
  cfg.weAre = UTILITY;
  if (!readSysTab(FALSE, TRUE)) exit(1);
  if (access(LOCKFILE, 0) != -1)
    {
    printf("Please do not run LogEdit from Outside Commands.\n");
    writeSysTab();
    exit(1);

    }
  makeSysName(logfile, "ctdllog.sys", &cfg.logArea);
  openFile(logfile, &logfl);
  initLogBuf(&logBuf);
  makeSysName(logfile, "ctdlfwd.sys", &cfg.roomArea);
  MakeList(&MailForward, logfile, NULL);
  handleEditing();
  logSum();
  writeSysTab();
  return 0;
  }
void showlog()
  {
  char *LastOn(long lastdate, char s);
  printf(" %-20s",logBuf.lbname);
  printf("%8ld", logBuf.lbvisit[0]);
  printf(" %d columns. Date:%s\n",logBuf.lbwidth,LastOn(logBuf.lblaston, TRUE));
  Display_Flag(0,logBuf.lbflags.AIDE       ,"AIDE Status        ");
  Display_Flag(1,logBuf.lbflags.EXPERT     ,"Expert User        ");
  Display_Flag(0,logBuf.lbflags.NET_PRIVS  ,"Network Privileges ");
  Display_Flag(1,logBuf.lbflags.DOOR_PRIVS ,"Door Privileges    ");
  Display_Flag(0,logBuf.lbflags.DL_PRIVS   ,"Download Privileges");
  Display_Flag(1,logBuf.lbflags.PERMANENT  ,"Permanent Account  ");
  Display_Flag(0,logBuf.lbflags.FLOORS     ,"Floor mode         ");
  Display_Flag(1,logBuf.lbflags.TWIT       ,"TWIT User          ");
  Display_Flag(0,logBuf.lbflags.RUGGIE     ,"RUGGIE User        ");
  Display_Flag(1,logBuf.lbflags.L_INUSE    ,"Active Account     ");
  Display_Flag(0,logBuf.lbflags.LFMASK     ,"LF FLAG            ");
  Display_Flag(1,logBuf.lbflags.OLDTOO     ,"OLD STYLE          ");
  Display_Flag(0,logBuf.lbflags.HALF_DUP   ,"HALF DUPLEX        ");
  Display_Flag(1,logBuf.lbflags.ALT_RE     ,"ALT. Read Messages ");
  Display_Flag(0,logBuf.lbflags.NoPrompt   ,"Room Name Prompt   ");
  printf("\n");
  }
void Display_Flag(flag,bit,text)
short  flag, bit;
char  *text;
  {
  printf(" %s: %s%c",text,(bit ? "Yes" : "No "), (flag==0 ? '\t':'\n'));

  }
