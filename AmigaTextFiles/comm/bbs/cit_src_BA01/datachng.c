/*
*                               Datachng.c
*
* Data expander and shrinker for C-86.
*/
/*
*                               history
*
* 89Apr13 HAW  Fix problem with expanding shared rooms (V1.3)
* 87Nov30 HAW  Fix problem with expanding rooms (V1.2)
* 87Nov?? HAW  Fix problem with reducing Mail.
* 87Oct12 HAW  Modified from Lexpand.
* 85Nov16 HAW  Modified for MS-DOS libraries.
* 85May23 HAW  Created.
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
*       getUtilNumber()         prompt user for a number, limited range
*       init()                  initialization
*       main()                  main controller
*       openFile()              opens a .sys file
*/
#define DoFree(p)       if ((p) != NULL)  {  free(p);  }
extern CONFIG    cfg;
extern LogTable  *logTab;       /* RAM index of pippuls */
extern logBuffer logBuf;        /* Pippul buffer        */
extern aRoom     roomBuf;
extern NetBuffer netBuf;
extern rTable    *roomTab;
extern NetTable  *netTab;
extern FILE      *logfl;        /* log file descriptor  */
extern FILE      *roomfl;
extern FILE      *netfl;
#ifdef ANSI_PROTOTYPING
int  getUtilNumber(char *prompt, unsigned bottom, unsigned top);
void getUtilString(char *prompt, char *buf, int lim);
void init(void);
void menus(void);
void LogExpand(void);
void RoomHandler(void);
void LogGenChange(int NewSize);
void MsgHandler(void);
void remakeRoomFl(void);
void remakeLogFl(void);
void remakeNetFl(void);
void MailHandler(void);
int  NEUtilGetch(void);
int  UtilGetch(void);
void SharedHandler(void);
void NetArchHandler(void);
char getUYesNo(char *prompt);
#endif

int  mPrintf(char *format, ...) {return 0; }  /* stub to quiet the linker */

/*
* crashout()
*
* This handles the irrecoverable error.
*/
void crashout(str)
char *str;
  {
  exit(printf(str));

  }
/*
* getUtilNumber()
*
* This function prompts for a number in (bottom, top) range.
*/
int getUtilNumber(prompt, bottom, top)
char   *prompt;
unsigned bottom;
unsigned top;
  {
  int try;
  char numstring[NAMESIZE];
  do
    {
    getUtilString(prompt, numstring, NAMESIZE);
    try     = atoi(numstring);
    if (try < bottom)  printf("Sorry, must be at least %d\n", bottom);
    if (try > top   )  printf("Sorry, must be no more than %d\n", top);

    }
  while (try < bottom ||  try > top);
  return  try;

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
  char c;
  int  i;
  if(strLen(prompt) > 0)
    {
    printf("\nEnter %s : ", prompt, lim);

    };
  i   = 0;
  while (       (c = getchar()) !='\n' && i < lim )
    {
    buf[i++] = c;
    };
  buf[i]  = '\0';
  if( i == lim )while ( getchar() != '\n');
  putchar('\n');

  }
/*
* getUYesNo()
*
* This handles a yes/no question.
*/
char getUYesNo(prompt)
char *prompt;
  {
  int  toReturn = ERROR;
  char c;
  do
    {
    printf("\n%s? (Y/N)", prompt);
    c = getchar();
    while( getchar() != '\n');
    switch ( toUpper(c) )
      {
      case 'Y': toReturn = TRUE; break;
      case 'N': toReturn = FALSE; break;
      default: printf("You must answer Yes or No\n");
      }
    }
  while (toReturn == ERROR);
  return (char)toReturn;

  }
/*
* init()
*
* This handles initializization details.
*/
void init()
  {
  SYS_FILE tempname;
  cfg.weAre = UTILITY;
  if (!readSysTab(FALSE, TRUE)) exit(1);  /* No system table? Tacky, tacky*/
  if (access(LOCKFILE, 0) != -1)
    {
    printf("Please do not run DataChng using Outside Commands.\n");
    exit(1);

    }
  mvToHomeDisk(&cfg.homeArea);
  makeSysName(tempname, "ctdllog.sys", &cfg.logArea);
  openFile(tempname,  &logfl );
  makeSysName(tempname, "ctdlroom.sys", &cfg.roomArea);
  openFile(tempname,  &roomfl );
  if (cfg.BoolFlags.netParticipant)
    {
    makeSysName(tempname, "ctdlnet.sys", &cfg.netArea);
    openFile(tempname,  &netfl );
    initNetBuf(&netBuf);

    }
  initLogBuf(&logBuf);
  initRoomBuf(&roomBuf);

  }
/*
* main()
*
* This is the main controller.
*/
int main(void);
int main()
  {
  printf("Citadel Data File Expand/Reduce %s\n%s\n", VERSION_NAME,COPYRIGHT);
  init();
  menus();
  return 0;
  }
/*
* menus()
*
* Menu stuff.
*/
void menus()
  {
  char c;
  char done = FALSE;
  if (!getUYesNo("HAVE YOU BACKED UP YOUR SYSTEM YET")) crashout("THEN DO SO FIRST!");
  do
    {
    printf("\n\n\n%20c Citadel Data Changer %s\n\n", ' ', VERSION_NAME);
    printf("a) Expand log (presently %d accounts)\n", cfg.MAXLOGTAB);
    printf("b) Expand or contract room table (presently %d slots)\n", MAXROOMS);
    printf("c) Expand or contract the number of messages per room ");
    printf("(presently %d slots)\n", MSGSPERRM);
    printf("d) Expand or contract the number of Mail messages per user ");
    printf("(presently %d slots)\n", MAILSLOTS);
    if (cfg.BoolFlags.netParticipant)
      {
      printf("e) Expand or contract the number of shared rooms/system ");
      printf("(presently %d rooms)\n", SHARED_ROOMS);

      }
    c = getchar();
    while( getchar() != '\n');
    switch (toUpper(c))
      {
      case 'A': LogExpand();    break;
      case 'B': RoomHandler();  break;
      case 'C': MsgHandler();     break;
      case 'D': MailHandler();  break;
      case 'E': if (cfg.BoolFlags.netParticipant)
        {
        SharedHandler();        break;

        }
      default: done = TRUE;             break;

      }
    if (!done) writeSysTab();

    }
  while (!done);

  }
/*
* LogExpand()
*
* This function handles expanding the user log.
*/
void LogExpand()
  {
  int i, j;
  printf("Current log size is %d\n", cfg.MAXLOGTAB);
  i = getUtilNumber("New size", cfg.MAXLOGTAB, 65535);
  if (i > cfg.MAXLOGTAB)
    {
    logTab = (LogTable *) realloc(logTab, sizeof(*logTab) * i);
    logBuf.lbname[0]      = 0;
    logBuf.lbpw[0]      = 0;
    logBuf.lbflags.L_INUSE = FALSE;
    for (j = cfg.MAXLOGTAB; j < i; j++)
      {
      putLog(&logBuf, j);
      logTab[j].ltpwhash  = 0;
      logTab[j].ltnmhash  = 0;
      logTab[j].ltlogSlot = j;
      printf("Clearing %d\r", j);

      }
    cfg.MAXLOGTAB = i;

    }
  printf("Don't forget to update LOGSIZE in CTDLCNFG.SYS with %d!\n",
  cfg.MAXLOGTAB);

  }
/*
* RoomHandler()
*
* This handles the expansion or contraction of the room file.
*/
void RoomHandler()
  {
  int NewSize, i;
  aRoom *TempTab;
  extern char *WRITE_ANY;
  printf("Current room size is %d\n", MAXROOMS);
  NewSize = getUtilNumber("New size", 3, 65535);
  if (NewSize == MAXROOMS) return ;
  if (NewSize < MAXROOMS)
    {
    for (i = MAXROOMS-1; i >= NewSize; i--)
    if (roomTab[i].rtflags.INUSE)
      {
      printf(
      "A room called '%s' will be lost.\n", roomTab[i].rtname);
      if (!getUYesNo("Do you still wish to contract the rooms"))
      return;

      };
    TempTab = (aRoom *) GetDynamic(sizeof roomBuf * NewSize);
    for (i = 0; i < NewSize; i++)
      {
      getRoom(i);
      movmem(&roomBuf, TempTab+i, sizeof roomBuf);
      TempTab[i].msg = (theMessages *) GetDynamic(MSG_BULK);
      movmem(roomBuf.msg, TempTab[i].msg, MSG_BULK);

      }
    remakeRoomFl();
    for (i = 0; i < NewSize; i++)
      {
      movmem(TempTab+i, &roomBuf, RB_SIZE);
      movmem(TempTab[i].msg, roomBuf.msg, MSG_BULK);
      putRoom(i);
      DoFree(TempTab[i].msg);

      }
    DoFree(TempTab);
    LogGenChange(NewSize);

    }
  else
    {
    roomTab = (rTable *) realloc(roomTab, sizeof *roomTab * NewSize);
    setmem(&roomBuf, RB_SIZE, 0);   /* Zilch all but the pointers */
    setmem(roomBuf.msg, MSG_BULK, 0);
    for (i = MAXROOMS; i <= NewSize; i++)
      {
      zero_struct(roomTab[i]);
      RoomSys(i);
      putRoom(i);

      }
    LogGenChange(NewSize);

    }
  MAXROOMS = NewSize;
  printf("Please remember to changes MAXROOMS to %d\n\n", MAXROOMS);

  }
/*
* LogGenChange()
*
* This function will change the generation array of the log.  This needs to
* be done when the number of rooms in the system changes.
*/
void LogGenChange(NewSize)
int NewSize;
  {
  int i, oldSize;
  logBuffer *NewLogs;
  extern char *WRITE_ANY;
  SYS_FILE tempname;
  NewLogs = (logBuffer *) GetDynamic(sizeof logBuf * cfg.MAXLOGTAB);
  for (i = 0; i < cfg.MAXLOGTAB; i++)
    {
    getLog(&logBuf, i);
    if (logBuf.lbflags.L_INUSE) printf("reading %s\n", logBuf.lbname);
    movmem(&logBuf, NewLogs + i, LB_SIZE);
    NewLogs[i].lbgen = (AN_UNSIGNED *) GetDynamic(
    NewSize * sizeof (AN_UNSIGNED));
    setmem(NewLogs[i].lbgen, NewSize * sizeof (AN_UNSIGNED), 0);
    movmem(logBuf.lbgen, NewLogs[i].lbgen,
    minimum(NewSize, MAXROOMS) * sizeof (AN_UNSIGNED));
    NewLogs[i].lbMail = (theMessages *) GetDynamic(MAIL_BULK);
    movmem(logBuf.lbMail, NewLogs[i].lbMail, MAIL_BULK);

    }
  oldSize = MAXROOMS;
  MAXROOMS = NewSize;
  fclose(logfl);
  makeSysName(tempname, "ctdllog.sys", &cfg.logArea);
  unlink(tempname);
  if ((logfl = fopen(tempname, WRITE_ANY)) == NULL)
    {
    crashout("Couldn't reopen log file!  (Return to your backups!)");

    }
  DoFree(logBuf.lbgen);
  logBuf.lbgen = (AN_UNSIGNED *) GetDynamic(GEN_BULK);
  for (i = 0; i < cfg.MAXLOGTAB; i++)
    {
    movmem(NewLogs + i, &logBuf, LB_SIZE);
    movmem(NewLogs[i].lbgen, logBuf.lbgen, NewSize * sizeof (AN_UNSIGNED));
    movmem(NewLogs[i].lbMail, logBuf.lbMail, MAIL_BULK);
    DoFree(NewLogs[i].lbgen);
    DoFree(NewLogs[i].lbMail);
    putLog(&logBuf, i);
    if (logBuf.lbflags.L_INUSE)
    printf("writing %s\n", logBuf.lbname);

    }
  DoFree(NewLogs);
  fclose(logfl);
  openFile(tempname, &logfl);
  MAXROOMS = oldSize;

  }
/*
* MsgHandler()
*
* This function handles changes to MSGSPERRM.
*/
void MsgHandler()
  {
  int NewSize, i, r1, r2, newBulk;
  aRoom *TempRooms;
  printf("Current messages per room is %d\n", MSGSPERRM);
  NewSize = getUtilNumber("New size", 0, 65535);
  newBulk = sizeof (theMessages) * NewSize;
  if (NewSize == MSGSPERRM) return ;
  TempRooms = (aRoom *) GetDynamic(sizeof roomBuf * MAXROOMS);
  for (i = 0; i < MAXROOMS; i++)
    {
    getRoom(i);
    movmem(&roomBuf, TempRooms+i, RB_SIZE);
    TempRooms[i].msg = (theMessages *) GetDynamic(newBulk);
    setmem(TempRooms[i].msg, newBulk, 0);
    for (r1 = MSGSPERRM - 1, r2 = NewSize - 1;
    r1 != -1 && r2 != -1; r1--, r2--)
      {
      TempRooms[i].msg[r2].rbmsgNo  = roomBuf.msg[r1].rbmsgNo;
      TempRooms[i].msg[r2].rbmsgLoc = roomBuf.msg[r1].rbmsgLoc;

      }

    }
  remakeRoomFl();
  killRoomBuf(&roomBuf);
  MSGSPERRM = NewSize;
  for (i = 0; i < MAXROOMS; i++)
    {
    movmem(TempRooms+i, &roomBuf, RB_SIZE);
    roomBuf.msg = TempRooms[i].msg;     /* Cheat */
    putRoom(i);
    DoFree(TempRooms[i].msg);

    }
  DoFree(TempRooms);
  initRoomBuf(&roomBuf);
  printf("Please remember to change MSG-SLOTS to %d\n\n", MSGSPERRM);

  }
/*
* remakeRoomFl()
*
* This function handles killing and re-opening the room file.
*/
void remakeRoomFl()
  {
  SYS_FILE tempname;
  extern char *W_R_ANY;
  fclose(roomfl);
  makeSysName(tempname, "ctdlroom.sys", &cfg.roomArea);
  unlink(tempname);
  if ((roomfl = fopen(tempname, W_R_ANY)) == NULL)
    {
    crashout("Couldn't reopen room file!  (Return to your backups!)");

    }

  }
/*
* remakeLogFl()
*
* This function handles killing and re-opening the log file.
*/
void remakeLogFl()
  {
  SYS_FILE tempname;
  extern char *W_R_ANY;
  fclose(logfl);
  makeSysName(tempname, "ctdllog.sys", &cfg.logArea);
  unlink(tempname);
  if ((logfl = fopen(tempname, W_R_ANY)) == NULL)
    {
    crashout("Couldn't reopen log file!  (Return to your backups!)");

    }

  }
/*
* remakeNetFl()
*
* This function handles killing and re-opening the net file.
*/
void remakeNetFl()
  {
  SYS_FILE tempname;
  extern char *W_R_ANY;
  fclose(netfl);
  makeSysName(tempname, "ctdlnet.sys", &cfg.netArea);
  unlink(tempname);
  if ((netfl = fopen(tempname, W_R_ANY)) == NULL)
    {
    crashout("Couldn't reopen net file!  (Return to your backups!)");

    }

  }
/*
* MailHandler()
*
* This function handles the contraction/expansion of MAILSLOTS.
*/
void MailHandler()
  {
  int NewSize, i, old, r1, r2;
  logBuffer *NewLog;
  old = MAILSLOTS;
  printf("Current messages per user in Mail is %d\n", MAILSLOTS);
  NewSize = getUtilNumber("New size", 0, 65535);
  if (NewSize == MAILSLOTS) return ;
  NewLog = (logBuffer *) GetDynamic(sizeof logBuf * cfg.MAXLOGTAB);
  for (i = 0; i < cfg.MAXLOGTAB; i++)
    {
    getLog(&logBuf, i);
    movmem(&logBuf, NewLog + i, LB_SIZE);
    MAILSLOTS = NewSize;
    initLogBuf(&NewLog[i]);
    MAILSLOTS = old;
    for (r1 = MAILSLOTS-1, r2 = NewSize-1;
    r1 != -1 && r2 != -1; r1--, r2--)
      {
      NewLog[i].lbMail[r2].rbmsgNo  = logBuf.lbMail[r1].rbmsgNo;
      NewLog[i].lbMail[r2].rbmsgLoc = logBuf.lbMail[r1].rbmsgLoc;

      }
    movmem(logBuf.lbgen, NewLog[i].lbgen, GEN_BULK);

    }
  remakeLogFl();
  killLogBuf(&logBuf);
  MAILSLOTS = NewSize;
  for (i = 0; i < cfg.MAXLOGTAB; i++)
    {
    movmem(NewLog + i, &logBuf, sizeof logBuf);
    putLog(&logBuf, i);
    killLogBuf(&logBuf);  /* this looks strange but works */

    }
  DoFree(NewLog);
  initLogBuf(&logBuf);
  killRoomBuf(&roomBuf);
  initRoomBuf(&roomBuf);
  printf("Please remember to change your MAIL-SLOTS parameter to %d\n\n",
  MAILSLOTS);

  }
#define check(x) if (test) printf("%c\n", x);
/*
* SharedHandler()
*
* This function changes the number of shared rooms/system.
*/
void SharedHandler()
  {
  int NewSize, i, old;
  NetBuffer *NewNet;
  char test;
  old = SHARED_ROOMS;
  printf("Current shared rooms/system is %d\n", SHARED_ROOMS);
  NewSize = getUtilNumber("new shared rooms/system", 0, 65535);
  if (NewSize == SHARED_ROOMS) return ;
  if (cfg.netSize == 0)
    {
    SHARED_ROOMS = NewSize;
    return ;

    }
  if (NewSize < SHARED_ROOMS)
  if (!getUYesNo("That value could result in losing shared rooms.  Continue"))
  return;
  test=FALSE;
  NewNet = (NetBuffer *) GetDynamic(sizeof netBuf * cfg.netSize);
  for (i = 0; i < cfg.netSize; i++)
    {
    check('a');
    getNet(i, &netBuf);
    check('b');
    SHARED_ROOMS = NewSize;
    initNetBuf(&NewNet[i]);
    check('c');
    movmem(&netBuf, NewNet + i, NB_SIZE);
    check('d');
    setmem(NewNet[i].netRooms, SR_BULK, 0);     /* 0 it */
    check('e');
    SHARED_ROOMS = old;
    check('f');
    movmem(netBuf.netRooms, NewNet[i].netRooms,
    minimum(SR_BULK, (NewSize * (sizeof (SharedRoom)))));
    check('g');

    }
  /*test=TRUE;*/
  check('1');
  remakeNetFl();
  killNetBuf(&netBuf);
  SHARED_ROOMS = NewSize;
  for (i = 0; i < cfg.netSize; i++)
    {
    check('z');
    movmem(NewNet + i, &netBuf, sizeof netBuf);
    check('y');
    DoFree(netTab[i].netTRooms);
    netTab[i].netTRooms = GetDynamic(SR_BULK);
    check('x');
    putNet(i, &netBuf);
    check('w');
    DoFree(netBuf.netRooms);
    check('v');
    check('u');

    }
  check('2');
  DoFree(NewNet);
  initNetBuf(&netBuf);
  printf("Please change your SHARED-ROOMS parameter to %d.\n\n",
  SHARED_ROOMS);
  check('3');

  }
