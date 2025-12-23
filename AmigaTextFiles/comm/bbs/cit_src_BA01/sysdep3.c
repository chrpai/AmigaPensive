/************************************************************************/
/*                              sysdep3.c                               */
/*                                                                      */
/*      This is the repository of most of the system dependent code     */
/*      in Citadel.  We hope, pray, and proselytize, at least.          */
/************************************************************************/
/************************************************************************/
/*                              history                                 */
/*                                                                      */
/* 86Dec14 HAW  Reorganized into areas.                                 */
/* 86Nov25 HAW  Created.                                                */
/************************************************************************/
#define SYSTEM_DEPENDENT
#define TIMER_FUNCTIONS_NEEDED
#include "ctdl.h"
#include "stdarg.h"
#include "string.h"
#include "dos.h"
#include "time.h"
#include "fcntl.h"
#include "minrexx.h"
/************************************************************************/
/*                              Contents                                */
/*                                                                      */
/*              MODEM HANDLING:                                         */
/*      inp()                   modem input with system overtones       */
/*      MIReady()               check MS-DOS interrupt for data         */
/*      modemClose()            closes down modem I/O                   */
/*      outMod()                bottom-level modem output               */
/*      rawModemInit()          initialize modem                        */
/*      ReInitModem()           reinitialize for recalcitrant modems    */
/*              SYSTEM FORMATTING:                                      */
/*      dPrintf()               printf() that writes to disk            */
/*      mPrintf()               writes a line to modem & console        */
/*      mTrPrintf()             special mPrintf for WC transfers        */
/* #    splitF()                debug formatter                         */
/*              TIMERS:                                                 */
/*      chkTimeSince()          check how long since timer initialized  */
/* #    milliTimeSince()        How long in milliseconds have passed    */
/*      pause()                 pauses for N/100 seconds                */
/* #    setTimer()              start a specific timer.                 */
/*      startTimer()            Initialize a general timer              */
/*      timeSince()             how long since general timer init       */
/*      ReadDate()              interprets time and returns it in secs. */
/*              CONSOLE STUFF, continued                                */
/*      ScreenUser()            update fn for givePrompt()              */
/*      ScrNewUser()            status line updates                     */
/*      SpecialMessage()        special status line messages            */
/*              MISCELLANEOUS:                                          */
/* #    diskSpaceLeft()         amount of space left on specified disk  */
/*      getRawDate()            gets date from system                   */
/*      giveSpaceLeft()         give amount of space left on disk sys   */
/* #    initBadList()           read in list of bad filenames           */
/* #    interpret()             interprets a configuration routine      */
/*      receive()               read modem char or time out             */
/*      runPCPdial()            does a PCPursuit dial                   */
/*      safeopen()              opens a file                            */
/*      setRawDate()            set date (system dependent code)        */
/*      systemCommands()        run outside commands in the O.S.        */
/*      systemInit()            system dependent init                   */
/*      systemShutdown()        system dependent shutdown               */
/*      WhatDay()               returns what day it is                  */
/*                                                                      */
/*              # == local for this implementation only                 */
/************************************************************************/
/************************************************************************/
/*      Globals -- there shouldn't be anything here but statics and     */
/*      externs.                                                        */
/************************************************************************/
extern char *R_W_ANY;
extern char *READ_ANY;
extern char *READ_TEXT;
extern char *APPEND_TEXT;
extern char *APPEND_ANY;
extern char *A_C_TEXT;
extern char *WRITE_TEXT;
extern char *W_R_ANY;
extern char *WRITE_ANY;

void  Load_Citadel_Messages(void);

void  Do_Stack_Check(void);
void jtimer(unsigned long *timeptr);
int CheckMessageReply(struct Message *inmsg);
void Save_Modem(char *,int ,char *);
void Read_Total_Data(void);
long char_in;           /* total characters input  */


/* Here's the rest of the goo */
SListBase DirBase  =
  {
  NULL, ChkNtoStr, NULL, FreeNtoStr, EatNMapStr

  };
SListBase ResList  =
  {
  NULL, ChkStrtoN, NULL, FreeNtoStr, ResIntrp

  };
SListBase BellList =
  {
  NULL, ChkTwoNumbers, NULL, free, EatTwoNumbers

  };
char *DirFileName = "ctdldir.sys";
char *ResFileName = "results.sys";
char ResultCodesAvailable;
static char AuditDir[100];
extern logBuffer logBuf;         /* Log buffer of a person       */
extern aRoom     roomBuf;
extern MessageBuffer   msgBuf;
extern CONFIG    cfg;            /* Lots an lots of variables    */
extern NetBuffer netBuf;
extern char onConsole;                  /* Who's in control?!?          */
extern char whichIO;                    /* CONSOLE or MODEM             */
extern char anyEcho;
extern char echo;
extern char modStat;
extern char echoChar;
extern char haveCarrier;
extern char justLostCarrier;
extern char outFlag;
extern char *indexTable;
extern char loggedIn;
/************************************************************************/
/* Section 3.1. MODEM HANDLING:                                         */
/*    These functions are responsible for handling modem I/O.           */
/************************************************************************/
extern struct IOStdReq *consoleReadMsg;
extern struct MsgPort *consoleReadPort;
extern struct IOExtSer *mySerReadMsg;
extern struct MsgPort *mySerReadPort;
extern struct IOExtSer *mySerWriteMsg;
extern struct MsgPort *mySerWritePort;
extern struct timerequest *myTimerMsg;
extern struct MsgPort *myTimerPort;
extern unsigned char serletter,conletter;
extern int serqueued,conqueued;
extern long byteRate;
extern int SerialOpen,DoBufferConsole;
char HasPushBack = FALSE;
long PB,FastBuffAllowed = 1;
long BaudTable[] =
  {
  300, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600

  };
#define MAXSERIALBUFFER (528)   /* set to 1056 before*/
unsigned char SIBuffer[MAXSERIALBUFFER], *SIBufferPtr, *SIBufferEnd;
long SIBuffered = FALSE;
int QueueSerRead ( int where );
struct Message *MyGetMsg ( struct MsgPort *MyMsgPort );
int OpenSerial ( int starting );
int CloseSerial ( int final );
int UnQueueSerRead ( void );
int ConPutChar ( char myChar );
int Jsystem ( char *cmdline );
int openStuff ( void );
void closeStuff ( int oldprobval );
int Amiga_System_Input ( void );
int QueueRead ( struct IOStdReq *request , char *whereto );
/**** Special Debug code****/
/* capture data to ram:debug.modem */

char   netdebuglog = FALSE;

void Save_Modem(buff,size ,who)
char *buff, *who;
int size;
  {
  FILE *ram;
  if( netdebuglog )
    {
    ram = fopen("ram:debug.modem","a");
    if( ram == NULL )
      {
      splitF(NULL," Unable to open ram:debug.modem\n");
      }
    else
      {
      fprintf(ram,"\n%s(%d)",who,size);
      fwrite(buff,size,1,ram);
      fclose(ram);
      };
    };
  }

/************************************************************************/
/*      inp() reads data from port.  Should not be called if there is   */
/*      no data present (for good reason).                              */
/************************************************************************/
unsigned char inp()
  {
  unsigned char temp;
  long readsize;
  Do_Stack_Check();
  char_in++;
  if (HasPushBack)
    {
    HasPushBack = FALSE;
    return (unsigned char) PB;

    }
  if (SIBuffered)
    {
    temp = *SIBufferPtr++;
    if (SIBufferPtr == SIBufferEnd)
      {
      SIBuffered = FALSE;
      QueueSerRead(11);

      }
    return temp;

    }
  WaitPort(mySerReadPort);
  MyGetMsg(mySerReadPort);
  if (mySerReadMsg->IOSer.io_Error)
    {
    printf("\n[1]IOError = %ld\n",(long)mySerReadMsg->IOSer.io_Error);
    mySerReadMsg->IOSer.io_Error = 0;
    if( !gotCarrier() )
      {
      haveCarrier      = FALSE;
      justLostCarrier  = TRUE;
      };
    }
  serqueued = FALSE;
  temp = serletter;
  if (FastBuffAllowed)
    {
    mySerReadMsg->IOSer.io_Command = SDCMD_QUERY;
    DoIO((struct IORequest *)mySerReadMsg);

    }
  if (FastBuffAllowed && mySerReadMsg->IOSer.io_Actual)
    {
    readsize = (mySerReadMsg->IOSer.io_Actual > MAXSERIALBUFFER) ? MAXSERIALBUFFER : mySerReadMsg->IOSer.io_Actual;
    mySerReadMsg->IOSer.io_Command = CMD_READ;
    mySerReadMsg->IOSer.io_Data = (APTR)SIBuffer;
    mySerReadMsg->IOSer.io_Length = readsize;
    DoIO((struct IORequest *)mySerReadMsg);
    if (cfg.BoolFlags.debug)Save_Modem(SIBuffer,readsize," In:");

    if (mySerReadMsg->IOSer.io_Error)
      {
      printf("\n[2]IOError = %ld\n",(long)mySerReadMsg->IOSer.io_Error);
      mySerReadMsg->IOSer.io_Error = 0;
      if( !gotCarrier() )
        {
        haveCarrier      = FALSE;
        justLostCarrier  = TRUE;
        return '\0';
        };
      }
    if (mySerReadMsg->IOSer.io_Actual)
      {
      char_in += mySerReadMsg->IOSer.io_Actual;
      SIBufferPtr = SIBuffer;
      SIBufferEnd = SIBuffer + mySerReadMsg->IOSer.io_Actual;
      SIBuffered = TRUE;
      if (cfg.BoolFlags.debug)Save_Modem(SIBufferEnd,mySerReadMsg->IOSer.io_Actual," In:");

      }
    else
      {
      QueueSerRead(22);

      }

    }
  else
    {
    QueueSerRead(2);

    }
  return temp;

  }
/************************************************************************/
/*      ModemPushBack()  Pushes a character back to modem               */
/************************************************************************/
void ModemPushBack(int c)
  {
  Do_Stack_Check();
  PB = c;
  HasPushBack = TRUE;

  }
/************************************************************************/
/*      MIReady() Ostensibly checks to see if input from modem ready    */
/************************************************************************/
int MIReady()
  {
  Do_Stack_Check();
  if (HasPushBack) return(TRUE);
  if (SIBuffered) return(TRUE);
  if (!SerialOpen || !serqueued)
    {
    return(FALSE);
    }
  if (CheckMessageReply((struct Message *)mySerReadMsg))
  return(TRUE);
  return FALSE;

  }
/************************************************************************/
/*      gotCarrier() carrier?                                           */
/************************************************************************/
int gotCarrier()
  {
  extern struct IOExtSer *mySerStatusMsg;
  extern char internalserial;
  Do_Stack_Check();
  if (!SerialOpen) return FALSE;
  if (internalserial)
    {
    if ((*((BYTE *) 0xbfd000)) & 0x20) return(FALSE);

    }
  else
    {
    mySerStatusMsg->IOSer.io_Command = SDCMD_QUERY;
    DoIO((struct IORequest *)mySerStatusMsg);
    if (mySerStatusMsg->io_Status & 0x20) return(FALSE);

    }
  return TRUE;

  }
/************************************************************************/
/*      changeBauds() change baud rates                                 */
/************************************************************************/
int changeBauds(MenuId id)
  {
  Do_Stack_Check();
  CitadelBaudRate((long) SysopGetNumber(id,"BAUDST", 0l, 8l),"changeBauds");
  return(0);

  }
/************************************************************************/
/*      modemClose() Responsible for shutting down I/O                  */
/*      For IBMs, interrupts must be disabled; dropping of DTR is       */
/*      impossible otherwise, for some reason unknown to me.    HAW     */
/*      88Dec16: modified to drop carrier only if need be.  HAW         */
/************************************************************************/
void ModemShutdown(char KillCarr)
  {
  Do_Stack_Check();
  if (KillCarr)
    {
    DisableModem(FALSE);
    pause(50);

    }

  }
/************************************************************************/
/*      ModemOpen() open the modem for the first time                   */
/************************************************************************/
void ModemOpen(char DoorReturn)
  {
  unsigned char *ptr;
  Do_Stack_Check();
  OpenSerial(TRUE);
  ptr = cfg.DepData.ModemSetup;
  if (!DoorReturn)
    {
    CitadelBaudRate(cfg.sysBaud,"ModemOpen");
    if (*ptr)
      {
      pause(20);
      do
        {
        outMod(*ptr);
        if (*ptr == '\r') pause(10);
        ++ptr;

        }
      while (*ptr);
      pause(20);

      }
    QueueSerRead(3);

    }

  }
/************************************************************************/
/*      HangUp()  hang up the modem                                     */
/************************************************************************/
void HangUp(char FromNet)
  {
  Do_Stack_Check();
  if (!FromNet && cfg.DepData.LockPort >= 0) pause(100);
  DisableModem(FromNet);
  EnableModem(FromNet);

  }
void Reinitialize()
  {
  ReInitModem();

  }
/************************************************************************/
/*      ReInitModem() reinitialize modem at a high speed.               */
/************************************************************************/
void ReInitModem()
  {
  int hadserial;
  Do_Stack_Check();
  if (strLen(cfg.DepData.HiSpeedInit) != 0 && !gotCarrier())
    {
    hadserial = TRUE;
    if (!SerialOpen)
      {
      hadserial = FALSE;
      OpenSerial(TRUE);
      }
    CitadelBaudRate(cfg.sysBaud,"ReInitModem");
    moPuts(cfg.DepData.HiSpeedInit);
    moPuts("\r");
    pause(100);
    if (!hadserial) CloseSerial(FALSE);

    }

  }

/************************************************************************/
/*      CitadelBaudRate() set the baud rate.                            */
/************************************************************************/

long Get_CPS(long, char *);

void CitadelBaudRate(long baudcode,char *caller)
  {
  long currentrate;
  long realRate;
  extern UBYTE OpenSerFlags;
  Do_Stack_Check();
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"CitadelBaudRate(%ld,%s)\n",baudcode,caller);
    };
  currentrate = ( cfg.DepData.LockPort >= 0 ) ? cfg.DepData.LockPort: baudcode;
  byteRate = Get_CPS(baudcode,caller);       /* set the user character per second */
  realRate = Get_CPS(currentrate,caller)*10; /* set the serial port baud rate */
  if (SerialOpen)
    {
    UnQueueSerRead();
    mySerReadMsg->io_Baud     = mySerStatusMsg->io_Baud     = mySerWriteMsg->io_Baud     = realRate;
    mySerReadMsg->io_SerFlags = mySerStatusMsg->io_SerFlags = mySerWriteMsg->io_SerFlags = OpenSerFlags;
    mySerWriteMsg->IOSer.io_Command = SDCMD_SETPARAMS;
    DoIO((struct IORequest *)mySerWriteMsg);
    QueueSerRead(4);
    };
  }


long Get_CPS(long crate, char *who)
  {
  long cpsrate;
  crate &= 0xffff; /* strip sign if too large */
  switch (crate)
    {
    case     0:
    case    30:
    case   300: cpsrate = 30;break;
    case     1:
    case   120:
    case  1200: cpsrate = 120;break;
    case     2:
    case   240:
    case  2400: cpsrate = 240;break;
    case     3:
    case   480:
    case  4800: cpsrate = 480;break;
    case     4:
    case   960:
    case  9600: cpsrate = 960;break;
    case     5:
    case  1440:
    case 14400: cpsrate = 1440;break;
    case     6:
    case  1920:
    case 19200: cpsrate = 1920;break;
    case     7:
    case  3840:
    case 38400: cpsrate = 3840;break;
    case     8:
    case  5760:
    case 57600: cpsrate = 5760;break;
    default:
      splitF(NULL,"Invalid Baud Code:%ld from %s\n",crate,who);
      cpsrate = Get_CPS(cfg.sysBaud, who);  /* good default */
    };
  return cpsrate;
  }

void DisableModem(char FromNet)
  {
  extern char internalserial;
  unsigned char  *ptr = (unsigned char *)0x0BFD000;
  Do_Stack_Check();
  if( internalserial )
    {
    *ptr &= 0x7f;           /* turn off DTR */
    }
  else  CloseSerial(FALSE);
  pause(150);
  }
void EnableModem(char FromNet)
  {
  extern char internalserial;
  unsigned char  *ptr = (unsigned char *)0x0BFD000;
  Do_Stack_Check();
  if( internalserial )
    {
    *ptr |= 0x80;           /*turn on  DTR */
    }
  else    OpenSerial(FALSE);
  if( FromNet )Reinitialize();  /* if from net session, then reinit */
  }
int RottenDial(char *callout_string)
  {
  Do_Stack_Check();
  return(FALSE);

  }
/************************************************************************/
/* Section 3.7. SYSTEM FORMATTING:                                      */
/*    These functions take care of formatting to strange places not     */
/* handled by normal C library functions.                               */
/*   dPrintf() print to disk, using putMsgChar().                       */
/*   mPrintf() print out the modem port via a mFormat() call.           */
/*   splitF() debug function, prints to both screen and disk.           */
/************************************************************************/
char bigbuffer[MAXTEXT];
/************************************************************************/
/*      sprintf() write from format+args to supplied string             */
/************************************************************************/
/*int sprintf(char *garp, const char *format, ...)
  {
  va_list argptr;
  va_start(argptr, format);
  vsprintf(garp, format, argptr);
  va_end(argptr);
  return (int)strlen(garp);
  } */
/************************************************************************/
/*      dPrintf() write from format+args to disk, appends a null byte   */
/************************************************************************/
void dPrintf(char *format, ...)
  {
  va_list argptr;
  char *garp;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  garp = bigbuffer;
  while(*garp)
    {
    putMsgChar(*garp);
    ++garp;

    }
  putMsgChar(0);

  }
/************************************************************************/
/*      mPrintf() formats format+args to modem and console              */
/************************************************************************/
int mPrintf(char *format, ...)
  {
  va_list argptr;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  mFormat(bigbuffer);
  return(1);

  }
/************************************************************************/
/*      printf() formats format+args to console                         */
/************************************************************************/
int printf(const char *format, ...)
  {
  FILE *fd;
  va_list argptr;
  int     i;
  extern  int  iconify_window;  /* if TRUE, ignore the output */
  extern  char ConOpen;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  if (ConOpen)
    {
    if (DoBufferConsole)
      for (i = 0; bigbuffer[i]; i++) ConPutChar(bigbuffer[i]);
    else
      {
      BufferingOn();
      for (i = 0; bigbuffer[i]; i++) ConPutChar(bigbuffer[i]);
      BufferingOff();

      };
    }
  else if( !iconify_window )
    {
    if ((fd = fopen("crash.sys", A_C_TEXT)) != NULL)
      {
      fprintf(fd,bigbuffer);
      fclose(fd);
      pause(10);  /* short pause to save the world if we die */
      };
    };
  return(1);

  }

/**
  NetPrintf()

  This formats format+args to a transmission function,
  adds a 0 byte on end.
**/
int NetPrintf(int (*method)(int c), char *format, ...)
{
    va_list argptr;
    char *g;
    int i;
    g = bigbuffer;
    va_start(argptr, format);
    vsprintf(bigbuffer, format, argptr);
    va_end(argptr);
    for (i = 0; g[i]; i++)
       {
       if (g[i] == '\n') g[i] = '\r';
       if (!(*method)(g[i])) return FALSE;
       };
    if (!(*method)(0)) return FALSE;  /* Send NULL since it did before */
    return TRUE;
}


/************************************************************************/
/*      splitF() formats format+args to file and console                */
/************************************************************************/
void splitF(FILE *diskFile, char *format, ...)
  {
  FILE  *fd;
  extern char *A_C_TEXT;
  char CLogFn[80];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  printf(bigbuffer);
  if (diskFile != NULL)
    {
    fwrite(bigbuffer, strlen(bigbuffer), 1, diskFile);
    fflush(diskFile);

    }
  else
    {
    if( cfg.Audit == 0)return;  /* exit if not configured Auditing*/
    makeAuditName(CLogFn, "debug.sys");
    if ((fd = fopen(CLogFn, A_C_TEXT)) != NULL)
      {
      fprintf(fd,bigbuffer);
      fclose(fd);

      }
    else
      {
      printf("DEBUG OPEN Failure: filename is %s",CLogFn );
      };
    pause(10);  /* short pause to save the world if we die */
    };
  }
/************************************************************************/
/*      mTrPrintf() formats format+args to a transmission function      */
/************************************************************************/
void mTrPrintf(char *format, ...)
  {
  va_list argptr;
  int i;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  for (i = 0; bigbuffer[i]; i++) sendITLchar(bigbuffer[i]);
  sendITLchar(0);      /* Send NULL since it did before */

  }
int (*ToFileWork)(int s) = putFLChar;
/************************************************************************/
/*      ToFile() sends the data to file pointed at by upfd              */
/************************************************************************/
void ToFile(char *format, ...)
  {
  va_list argptr;
  int i;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  for (i = 0; bigbuffer[i]; i++) (*ToFileWork)(bigbuffer[i]);
  (*ToFileWork)(0);      /* Send NULL since it did before */

  }
/************************************************************************/
/* Section 3.8. TIMERS:                                                 */
/*    Basically, the idea here is that two functions are available to   */
/* the rest of Citadel.  One starts a timer.  The other allows checking */
/* that timer, to see how much time has passed since that timer was     */
/* started.  The remainder of the functions in this section are internal*/
/* to this implementation, mostly for use by receive().                 */
/* 88Jun28: Now multiple timers accessible to Citadel are supported.    */
/************************************************************************/
static struct timePacket localTimers[10];
/************************************************************************/
/*      chkTimeSince() buffer for timing stuff.  A call to startTimer() */
/*      must have preceded calls to this function.                      */
/*      RETURNS: Time in seconds since last call to startTimer().       */
/************************************************************************/
long chkTimeSince(int TimerId)
  {
  return timeSince(&localTimers[TimerId]);

  }
/************************************************************************/
/*      milliTimeSince() Calculate how many milliseconds have passed    */
/************************************************************************/
long milliTimeSince(struct timePacket *Slast)
  {
  long retVal;
  unsigned long curclock[2];
  jtimer(curclock);
  retVal = (curclock[0] != Slast->clock[0]) ? 100l : 0l;
  retVal += (curclock[1] - Slast->clock[1]) / 10000;
  return retVal;

  }
#ifdef MSDOS_PAUSE
/************************************************************************/
/*      pause() busy-waits N/100 seconds                                */
/************************************************************************/
void pause(int i)
  {
  struct timePacket x;
  long              (*fn)(struct timePacket *r), limit;
  fn = (i <= 99) ? milliTimeSince : timeSince;
  limit = (i <= 99) ? (long) i : (long) (i / 100);    /* Kludge */
  setTimer(&x);
  while ((*fn)(&x) <= limit)
  ;

  }
#endif
/************************************************************************/
/*      setTimer() get ready for timing something                       */
/************************************************************************/
void setTimer(struct timePacket *Slast)
  {
  jtimer(Slast->clock);

  }
/************************************************************************/
/*      startTimer() Initialize a general timer                         */
/************************************************************************/
void startTimer(int TimerId)
  {
  setTimer(&localTimers[TimerId]);

  }
/************************************************************************/
/*      timeSince() Calculate how many seconds have passed since "x"    */
/************************************************************************/
long timeSince(struct timePacket *Slast)
  {
  long retVal;
  unsigned long curclock[2];
  jtimer(curclock);
  retVal = curclock[0] - Slast->clock[0];
  return retVal;

  }
#define LeapYear(x)     ((x % 4) ? FALSE : ((year % 100) ? TRUE : FALSE))
/************************************************************************/
/*      ReadDate() interprets the string and returns it in seconds      */
/************************************************************************/
int ReadDate(char *date, long *RetTime)
  {
  static char *MonthTab[] =
    {
    "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE",
    "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"

    };
  static char *KeyWords[] =
    {
    "TODAY", "YESTERDAY"

    };
  static char DayPMonth[] =
    {
    31,28,31,30,31,30,31,31,30,31,30,31

    };
  int rover, found, keyword = -1;
  int   year, month, day, hours, minutes, seconds, milli;
  label mon;
  char *d = date;
  char  darray[6];              /* see format for utpack() */
  Do_Stack_Check();
  if (strLen(date) == 0) return FALSE;
  if (isdigit(date[0]))
    {
    year = (char) atoi(date) ;
    while (isdigit(*date)) date++;

    }
  else
    {
    getRawDate(&year, &month, &day, &hours, &minutes,
    &seconds, &milli);
    darray[1] = (char) month;
    darray[2] = (char) day;

    }
  for (rover = 0; isalpha(*date); date++, rover++)mon[rover] = toUpper(*date);
  mon[rover] = 0;
  if (rover == 0)
    {
    if( isdigit(d[0]) )
      {
      *RetTime = CurAbsolute() - ( atol(d)*86400l);
      return TRUE;
      }
    return ERROR;
    };
  for (found = rover = 0; rover < NumElems(MonthTab); rover++)
  if (strncmp(mon, MonthTab[rover], strLen(mon)) == SAMESTRING)
    {
    found++;
    darray[1] = (char) (rover + 1);

    }
  if (found != 1)
    {
    for (keyword = -1, rover = 0; rover < NumElems(KeyWords); rover++)
    if (strncmp(mon, KeyWords[rover], strLen(mon)) == SAMESTRING)
      {
      keyword = rover;

      }
    if (keyword == -1)
    return ERROR;
    else
      {
      switch(keyword)
        {
        case 0: /* TODAY */           break;
        case 1: /* YESTERDAY */
        --darray[2];
        if (!darray[2])
          {
          --darray[1];
          if (!darray[1])
            {
            darray[1] = 12;
            --year;

            }
          darray[2] = DayPMonth[darray[1] - 1];
          if (darray[1] == 2 && LeapYear(year))
          darray[2] = 29;

          }
        break;

        }

      }

    }
  if ((keyword == -1) && ((darray[2] = (char) atoi(date)) == 0)) return ERROR;
  darray[3] = 0;
  darray[4] = 0;
  darray[5] = 0;
  if (cfg.BoolFlags.tdebug)splitF(NULL,"Date is %d %d %d.\n",year,darray[1],darray[2]);
  if( year > 2205 ) return ERROR;
  if( year < 1970 ) return ERROR;
  darray[0] = year - 1970;
  *RetTime = utpack(darray);
  return TRUE;

  }
/************************************************************************/
/*      AbsToReadable() return a human string representing that date    */
/************************************************************************/
char *AbsToReadable(unsigned long lastdate)
  {
  struct tm   *data;
  char        *m;
  static char buffer[40];
  extern char *monthTab[];
  Do_Stack_Check();
  /* 0l represents never in our scheme */
  if (lastdate == 0l) return "Never";
  data = localtime((time_t const *)&lastdate);
  civTime(&data->tm_hour, &m);
  sPrintf(buffer, "%d%s%02d @ %d:%02d %s",
  data->tm_year, monthTab[data->tm_mon + 1],
  data->tm_mday, data->tm_hour, data->tm_min, m);
  return buffer;

  }
/************************************************************************/
/*      CurAbsolute() current time in absolute terms.                   */
/************************************************************************/
long CurAbsolute()
  {
  Do_Stack_Check();
  return time(NULL);

  }
/************************************************************************/
/*      getCh() Gets char from console.  We do not use a para. define   */
/*      here because we must have a pointer to function elsewhere, and  */
/*      the way we do it here should save a bit of space.               */
/************************************************************************/
int getCh()
  {
  int temp;
  extern struct IOStdReq *consoleReadMsg;
  Do_Stack_Check();
  WaitPort(consoleReadPort);
  MyGetMsg(consoleReadPort);
  conqueued = FALSE;
  temp = conletter;
  QueueRead(consoleReadMsg, &conletter);
  return temp;

  }
/************************************************************************/
/*      KBReady() returns TRUE if a console char is ready               */
/************************************************************************/
char KBReady()
  {
  extern char ConOpen;
  Do_Stack_Check();
  if (!ConOpen || !conqueued) return(FALSE);
  if (CheckMessageReply((struct Message *)consoleReadMsg))
  return TRUE;
  return(FALSE);

  }
/************************************************************************/
/*      Section 3.9. MISCELLANEOUS                                      */
/************************************************************************/
int strncmpU(char *first, char *second, int length)
  {
  Do_Stack_Check();
  while ((*first) && (*second) && (length > 0))
    {
    if (toupper(*first) != toupper(*second))
    return((int)(*first - *second));
    ++first;
    ++second;
    --length;

    }
  return(0);

  }
/************************************************************************/
/*      diskSpaceLeft() Amount of space left on specified disk          */
/************************************************************************/
void diskSpaceLeft(char *drive, long *sectors, long *bytes)
  {
  struct InfoData dfreeblk;
  char *fs, temp[150];
  Do_Stack_Check();
  if (drive != NULL)
    {
    if (strncmpU(drive,"RAM:",4) == SAMESTRING)
      {
      *bytes = AvailMem(0L) - 100000;
      if (*bytes < 0)
      *bytes = 0;
      *sectors = ((*bytes) + 127) / SECTSIZE;
      return;

      }
    else
      {
      strcpy(temp, drive);
      fs = strchr(temp,':');
      if (fs != NULL)
      *fs = 0;
      else
        {
        *bytes = 0;
        *sectors = 0;
        return;

        }
      getdfs(temp, &dfreeblk);

      }

    }
  else  getdfs(NULL, &dfreeblk);
  *bytes = (long) ((dfreeblk.id_NumBlocks - dfreeblk.id_NumBlocksUsed) *
  dfreeblk.id_BytesPerBlock);
  *sectors = ((*bytes) + 127) / SECTSIZE;

  }
/************************************************************************/
/*      getRawDate() gets raw date from MSDOS                           */
/************************************************************************/
void getRawDate(int *year, int *month, int *day, int *hours, int *minutes,
int *seconds, int *milli)
  {
  long t;
  struct tm *v;
  Do_Stack_Check();
  time(&t);
  v        = localtime(&t);
  *year    = v->tm_year + 1900;
  *month   = v->tm_mon + 1;
  *day     = v->tm_mday;
  *hours   = v->tm_hour;
  *minutes = v->tm_min;
  *seconds = v->tm_sec;
  *milli   = 0;

  }
/************************************************************************/
/*      giveSpaceLeft() give amount of space left on system.            */
/************************************************************************/
void giveSpaceLeft(aRoom *roomData)
  {
  long        sectors, bytes;
  extern char remoteSysop;
  Do_Stack_Check();
  if (setSpace(roomData))
    {
    diskSpaceLeft(NULL, &sectors, &bytes);
    if (!remoteSysop)
    printf("\nThere are %s bytes left\n",
    PrintPretty(bytes, msgBuf.mbtext));
    else
    mPrintf("\nThere are %s bytes left\n",
    PrintPretty(bytes, msgBuf.mbtext));

    }
  homeSpace();

  }
/************************************************************************/
/*      receive() gets a modem character, or times out ...              */
/*      Returns:        char on success else ERROR                      */
/************************************************************************/
int receive(int seconds)
  {
  unsigned long smask,tmask,got;
  int tout;
  Do_Stack_Check();
  if (!gotCarrier()) return ERROR;
  if (MIReady()) return (int)inp();
  tmask = (1L << myTimerPort->mp_SigBit);
  smask = (1L << mySerReadPort->mp_SigBit);
  myTimerMsg->tr_node.io_Command = TR_ADDREQUEST;
  myTimerMsg->tr_time.tv_secs = seconds;
  myTimerMsg->tr_time.tv_micro = 0;
  SendIO((struct IORequest *)myTimerMsg);
  tout = 1;
  got = Wait(tmask | smask | SIGBREAKF_CTRL_F);
  if (got & tmask)
    {
    WaitPort(myTimerPort);
    MyGetMsg(myTimerPort);
    tout = 0;

    }
  if (got & smask || got & SIGBREAKF_CTRL_F )
    {
    if (tout)
      {
      AbortIO((struct IORequest *)myTimerMsg);
      WaitPort(myTimerPort);
      MyGetMsg(myTimerPort);
      }

    }
  if (MIReady()) return (int)inp();
  return ERROR;

  }
#ifdef AMIGA_READY
#ifdef NEED_AVAILABLE
/************************************************************************/
/*      setRawDate() set system date                                    */
/************************************************************************/
char setRawDate(int year, int month, int day, int hour, int mins)
  {
  struct time timeblk;
  struct date dateblk;
  Do_Stack_Check();
  timeblk.ti_min  = mins;
  timeblk.ti_hour = hour;
  timeblk.ti_sec  = 0   ;
  timeblk.ti_hund = 0   ;
  dateblk.da_year = year;
  dateblk.da_day  = day ;
  dateblk.da_mon  = month;
  setdate(&dateblk);
  settime(&timeblk);
  return TRUE;

  }
#endif
/************************************************************************/
/*      safeopen() Opens a file                                         */
/************************************************************************/
FILE *safeopen(char *fn, char *mode)
  {
  struct stat buff;
  Do_Stack_Check();
  if (stat(fn, &buff) == 0)
  if (buff.st_mode & S_IFCHR)
  return NULL;
  return fopen(fn, mode);

  }
#endif
/************************************************************************/
/*      specCmpU() special compare for this version's file tags         */
/************************************************************************/
int specCmpU(char *f1, char *f2)
  {
  Do_Stack_Check();
  while (toUpper(*f1) == toUpper(*f2)) f1++, f2++;
  if (*f1 == 0 && *f2 == ' ') return SAMESTRING;
  if (*f2 == 0) return -1;
  if (toUpper(*f1) < toUpper(*f2)) return -1;
  return 1;

  }
/************************************************************************/
/*      systemCommands() Some MS-DOS commands                           */
/************************************************************************/
void systemCommands()
  {
  FILE *fd;
  char cx, sbigbuffer[200];
  long countmeup;
  Do_Stack_Check();
  while (onLine())
    {
    outFlag = OUTOK;
    mPrintf("\n System commands: ");
    cx = iChar();
    switch (toUpper(cx))
      {
      case 'F':
      FastBuffAllowed = !FastBuffAllowed;
      mPrintf("astbuffer %sallowed\n ",(FastBuffAllowed) ? "" : "not ");
      break;
      case 'B':
      countmeup = 0;
      while(MIReady())
        {
        ++countmeup;
        inp();

        }
      printf("Flushed out %ld chars\n",countmeup);
      break;
      case 'X':
      mPrintf("\bExit from System Commands menu\n ");
      return ;
      case 'D':
      mPrintf("elete file\n ");
      getNormStr("EFILEN", sbigbuffer, 199, 0);
      doCR();
      mPrintf("File %s.\n ", (unlink(sbigbuffer) == 0) ?
      "deleted" : "not found");
      break;
      case 'O':
      mPrintf("utside commands\n One moment, please.");
      writeSysTab();
      if ((fd = safeopen(LOCKFILE, "w")) != NULL)
        {
        fprintf(fd,
        "This is the Citadel-86 LOCK file, which is here to prevent you from\n"
        "accidentally bringing up Citadel from within Citadel.  Do not delete\n"
        "this file unless you are certain that you do not have Citadel up already.\n");
        fclose(fd);

        }
      getString("CMDLIN", sbigbuffer, 198, 0);
      if (whichIO == CONSOLE || strLen(sbigbuffer) != 0)
        {
        Jsystem(sbigbuffer);
        /*
        * This is a kludge.  Since this is in a system dependency
        * file, I'm not going to bother to really explain or feel
        * guilty.
        */
        if (!gotCarrier()) DisableModem(FALSE);
        homeSpace();

        }
      unlink(indexTable);
      unlink(LOCKFILE);
      break;
      case '?':
      tutorial("sysopt.mnu", TRUE);
      break;
      default:
        Output_Citadel_Message("HLPMNU",NULL,NULL,NULL);
      break;

      }

    }

  }
int ProblemValue;
/************************************************************************/
/*      systemInit() system dependent initialization                    */
/************************************************************************/
int systemInit()
  {
  extern char currentdir[150];     /* current working directory */
  extern char ourHomeSpace[150];   /* current home area */
  SYS_FILE filename;
  extern int  __msflag;                 /* use MSDOS wildcards for now */
  Do_Stack_Check();
  __msflag = 1;
  Load_Citadel_Messages();
  ProblemValue = openStuff();
  if (ProblemValue < 99) return(TRUE);
  getcwd(ourHomeSpace, 149);
  currentdir[0] = '\0';
  if (cfg.Audit != 0)
    {
    /* ugly kludge */
    chdir(cfg.auditArea.saDirname);
    getcwd(AuditDir, sizeof AuditDir);
    chdir(ourHomeSpace);

    }
  else strcpy(AuditDir,ourHomeSpace);    /* fix invalid audit area */
  makeSysName(filename, DirFileName, &cfg.roomArea);
  MakeList(&DirBase, filename, NULL);
  makeSysName(filename, ResFileName, &cfg.roomArea);
  ResultCodesAvailable = MakeList(&ResList, filename, NULL);
  ArcInit();
  #ifdef NEEDED
  ctrlbrk(Control_C);
  #endif
  InitExternEditors();
  InitProtocols();
  Read_Total_Data();
  return 0;

  }
/************************************************************************/
/*      ResIntrp() interprets a line from RESULTS.SYS.                  */
/************************************************************************/
void *ResIntrp(char *line)
  {
  char *mid;
  int rover;
  NumToString *temp;
  static struct
    {
    char *ResName;
    int  ResVal;

    } translate[] =
    {
      { "#RESULT-300",   R_300 },
      { "#RESULT-1200",  R_1200 },
      { "#RESULT-2400",  R_2400 },
      { "#RESULT-4800",  R_4800 },
      { "#RESULT-9600",  R_9600 },
      { "#RESULT-14400", R_14400 },
      { "#RESULT-19200", R_19200 },
      { "#RESULT-38400", R_38400 },
      { "#RESULT-57600", R_57600 },
      { "#RING",        R_RING },
      { "#DIALTONE",    R_DIAL },
      { "#NO-DIALTONE", R_NODIAL },
      { "#OK",          R_AOK },
      { "#NO-CARRIER",  R_NOCARR },
      { "#BUSY",        R_BUSY }
     };
  Do_Stack_Check();
  if ((mid = strchr(line, ' ')) != NULL)
    {
    *mid = 0;
    mid++;
    while ( *mid == ' ' || *mid == '\t')mid++;  /* skip white space */
    for (rover = 0; rover < NumElems(translate); rover++)
    if (strCmpU(line, translate[rover].ResName) == SAMESTRING)
      {
      temp = (NumToString *) GetDynamic(sizeof *temp);
      temp->num = translate[rover].ResVal;
      temp->string = strdup(mid);
      if (cfg.BoolFlags.debug)
        {
        splitF(NULL," Result code: %d String:-%s-\n",temp->num,temp->string);
        };

      return temp;

      }
    splitF(NULL," Invalid line in RESULT.SYS:%s\n",line);
    }
  return NULL;

  }
/************************************************************************/
/*      ResultVal() if the given value is present, return the parameter */
/*      it matches.                                                     */
/************************************************************************/
int ResultVal(char *buf)
  {
  UNS_16 *j;
  Do_Stack_Check();
  j = (UNS_16 *) SearchList(&ResList, buf);
  if (j != NULL)
  return (int)*j;
  else
  return ERROR;

  }
/************************************************************************/
/*      systemShutdown() system dependent shutdown code                 */
/************************************************************************/
void systemShutdown(int SystemErrorValue)
  {
  Do_Stack_Check();
  closeStuff(ProblemValue);
  }
/************************************************************************/
/*      BeNice() be nice to the nice operating system                   */
/************************************************************************/
void BeNice(int x)
  {
  int delay;
  Do_Stack_Check();
  if( x != NET_PAUSE)Amiga_System_Input();
  switch(x)
    {
    case INUSE_PAUSE: delay =  5;break;
    case   CHAT_NICE: delay =  2;break;
    case  IDLE_PAUSE:
      {
      delay = 50;
      SpecialMessage("Status:Idle");
      break;
      };
    case   NET_PAUSE: delay =  1;break;
    }
  pause(delay);
  }
void StrollIt(void)
  {

  }
/************************************************************************/
/*      safeopen()                                                      */
/************************************************************************/
FILE *safeopen(char *fn, char *mode)
  {
  FILE *rtfp;

  Do_Stack_Check();
  rtfp = fopen(fn, mode);
  /*if (cfg.BoolFlags.debug && rtfp == NULL)
     splitF(NULL," result = %08.8X \n",rtfp); */
  return  rtfp;

  }
/************************************************************************/
/*              totalBytes() how many bytes in this here file?                                  */
/************************************************************************/
void totalBytes(long *size, FILE *fd)
  {
  long oldpos;                  /* old position of this file */
  Do_Stack_Check();
  oldpos = ftell(fd);           /* save old position */
  fseek(fd,0l,SEEK_END);       /* go to end of file */
  *size = ftell(fd);            /* get current position */
  fseek(fd,oldpos,SEEK_SET);    /* back to begining */
  }
/************************************************************************/
/*      WhatDay() Returns what day it is (0=Sunday...)                  */
/************************************************************************/
int WhatDay()
  {
  long t;
  char *s;
  int  rover;
  char *Days[] =
    {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"

    };
  Do_Stack_Check();
  time(&t);
  s = ctime(&t);
  /* now find out what day it is */
  for (rover = 0; rover < 7; rover++)
  if (strncmp(s, Days[rover], 3) == 0)
    {
    return rover;

    }

  }
/************************************************************************/
/*       DialExternal()                                                 */
/************************************************************************/
int DialExternal(NetBuffer *netBuf)
  {
  Do_Stack_Check();
  UnQueueSerRead();
  Jsystem(netBuf->access);
  homeSpace();
  QueueSerRead(5);
  return gotCarrier();

  }
void makeAuditName(char *logfn, char *name)
  {
  Do_Stack_Check();
  sPrintf(logfn, "%s%s%s", AuditDir, (AuditDir[strlen(AuditDir)-1] == ':') ? "" : "/", name);

  }
void Intel32ToMotorola(UNS_32 *val)
  {
  unsigned long temp;
  Do_Stack_Check();
  temp = *val;
  *val = ((ULONG)(temp & 0xff) << 24) + ((ULONG)(temp & 0xff00) << 8) +
  ((ULONG)(temp & 0xff0000) >> 8) + ((ULONG)(temp &0xff000000) >> 24);

  }
void Intel16ToMotorola(UNS_16 *val)
  {
  UNS_16 temp;
  Do_Stack_Check();
  temp = *val;
  *val = ((temp & 0xff) << 8) + ((temp & 0xff00) >> 8);

  }
int CheckMessageReply(struct Message *inmsg)
  {
  struct Message *tempmsg;
  struct MsgPort *replyport;
  Do_Stack_Check();
  replyport = inmsg->mn_ReplyPort;
  if (!replyport) return ERROR;
  Disable();
  tempmsg = (struct Message *)replyport->mp_MsgList.lh_Head;
  while(tempmsg)
    {
    if (tempmsg == inmsg)
      {
      Enable();
      return TRUE;

      }
    tempmsg = (struct Message *)tempmsg->mn_Node.ln_Succ;

    }
  Enable();
  return FALSE;

  }
void jtimer(unsigned long *timeptr)
  {
  Do_Stack_Check();
  myTimerMsg->tr_node.io_Command = TR_GETSYSTIME;
  DoIO((struct IORequest *)myTimerMsg);
  timeptr[0] = myTimerMsg->tr_time.tv_secs;
  timeptr[1] = myTimerMsg->tr_time.tv_micro;
  }

/*
**  THIS IS THE START OF THE SYSOP MENU HANDLING CODE
*/
int    GetMenuSelection(MenuId id);
MenuId CreateIOWindow(short leftedge, short topedge, short width, short height, char *title);
void   CloseIOWindow(MenuId MenuPtr);
struct Window *DoOpenWindow(short leftedge, short topedge, short width, short height, char *title);
int    CreateInteractiveWindowCon(MenuId id);
int    DeleteInteractiveWindowCon(MenuId id);
void   IWCWrite(MenuId id, char *str, long len);
int    IWCRead(MenuId id, char *str, long len);
void   IWCReadString(MenuId id, char *prompt, char *buf, int lim);
/*
**      RegisterSysopMenu() set up for doing special sysop menus
*/
MenuId RegisterSysopMenu(char *MenuName, char *Opts[], char *MenuTitle)
  {
  int i;
  Do_Stack_Check();
  mPrintf(MenuTitle);
  for (i = 0; Opts[i][0] && Opts[i][0] != ' '; i++)
    {
    printf("%s",Opts[i]);
    };

  RegisterThisMenu(MenuName, Opts);
  return NO_MENU;

  }
/************************************************************************/
/*      GetSysopMenuChar() gets a selection from the sysop              */
/************************************************************************/
int GetSysopMenuChar(MenuId id)
  {
  Do_Stack_Check();
  return GetMenuChar();

  }
/*
/*      NeedSysopInpPrompt()  A kludge for checking for input
*/
char NeedSysopInpPrompt(void)
  {
  Do_Stack_Check();
  return((char)onConsole);

  }
/*
**      CloseSysopMenu() closes the sysop's menu
*/
void CloseSysopMenu(MenuId id)
  {
  Do_Stack_Check();

  }

/*
/*      SysopMenuPrompt() prompt handling in sysop menu
*/
void SysopMenuPrompt(MenuId id, char *prompt)
  {
  Do_Stack_Check();
  if (!onConsole || id == NO_MENU)
  mPrintf(prompt);

  }

/*
**      SysopError() show an error to the sysop
*/
void SysopError(MenuId id, char *prompt)
  {
  Do_Stack_Check();
  mPrintf(prompt);

  }

/*
**      SysopGetYesNo() get yes/no from sysop
*/
char SysopGetYesNo(MenuId id, char *info, char *prompt)
  {
  Do_Stack_Check();
  if( info != NULL)Output_Citadel_Message(info, NULL, NULL, NULL);
  return getYesNo(prompt);
  }
/************************************************************************/
/*      SysopRequestString() gets a string from the sysop.              */
/************************************************************************/
void SysopRequestString(MenuId id, char *prompt, char *buf, int size, int flags)
  {
  Do_Stack_Check();
  getNormStr(prompt, buf, size, flags);

  }
/************************************************************************/
/*      SysopInfoReport() report information to sysop                   */
/************************************************************************/
void SysopInfoReport(MenuId id, char *info)
  {
  Do_Stack_Check();
  mPrintf(info);

  }
/************************************************************************/
/*      SysopDisplayInfo() Display info to the sysop                    */
/************************************************************************/
void SysopDisplayInfo(MenuId id, char *info, char *title)
  {
  Do_Stack_Check();
  mPrintf(title);
  mPrintf(info);

  }
/************************************************************************/
/*      SysopGetNumber() get a number from sysop                        */
/************************************************************************/
long SysopGetNumber(MenuId id, char *prompt, long bottom, long top)
  {
  Do_Stack_Check();
  return getNumber(prompt, bottom, top);

  }
/************************************************************************/
/*      SysopContinual() open a window for some output                  */
/************************************************************************/
MenuId SysopContinual(char *title, char *prompt, int MaxWidth, int Depth)
  {
  Do_Stack_Check();
/**
  print only if not NULL and length greater than zero
**/
  if( title  != NULL)if( strlen(title) )mPrintf(title);
  if( prompt != NULL)if( strlen(prompt))Output_Citadel_Message(prompt,NULL,NULL,NULL);
  return NO_MENU;

  }
/************************************************************************/
/*      SysopContinualString() get a string from a window               */
/************************************************************************/
void SysopContinualString(MenuId id, char *prompt, char *buf, int size, int flags)
  {
  Do_Stack_Check();
  getNormStr(prompt, buf, size, 0);

  }
/************************************************************************/
/*      SysopPrintf() formats format+args to sysop window               */
/************************************************************************/
int SysopPrintf(MenuId id, char *format, ...)
  {
  va_list argptr;
  va_start(argptr, format);
  vsprintf(bigbuffer, format, argptr);
  va_end(argptr);
  Do_Stack_Check();
  mFormat(bigbuffer);
  return 0;

  }
/************************************************************************/
/*      SysopCloseContinual() closes continaual IO window               */
/************************************************************************/
void SysopCloseContinual(MenuId id)
  {
  Do_Stack_Check();

  }
/*  These are my functions to support all of these above  */
int GetMenuSelection(MenuId id)
  {
  int goodchar;
  char testchar;
  Do_Stack_Check();
  if (id == NO_MENU || !id->MenuCharList || !id->MenuCharList[0])
  return(0);
  goodchar = 0;
  do
    {
    IWCRead(id, &testchar, 1);
    testchar = toUpper(testchar);
    if (strchr(id->MenuCharList, testchar) && !goodchar)
      {
      goodchar = testchar;

      }

    }
  while(!goodchar);
  return(goodchar);

  }
MenuId CreateIOWindow(short leftedge, short topedge, short width, short height, char *title)
  {
  MenuId MenuPtr;
  struct Window *WPtr;
  Do_Stack_Check();
  WPtr = DoOpenWindow(leftedge, topedge, width, height, title);
  if (!WPtr)
    {
    return NO_MENU;

    }
  MenuPtr = (MenuId)malloc(sizeof(struct MenuData));
  if (!MenuPtr)
    {
    CloseWindow(WPtr);
    return NO_MENU;

    }
  memset(MenuPtr, sizeof(struct MenuData), 0);
  MenuPtr->MenuWindow = WPtr;
  MenuPtr->MenuCharList = NULL;
  if (!CreateInteractiveWindowCon(MenuPtr))
    {
    CloseWindow(WPtr);
    free(MenuPtr);
    return NO_MENU;

    }
  return(MenuPtr);

  }
void CloseIOWindow(MenuId MenuPtr)
  {
  Do_Stack_Check();
  if (MenuPtr != NO_MENU)
    {
    DeleteInteractiveWindowCon(MenuPtr);
    if (MenuPtr->MenuWindow) CloseWindow(MenuPtr->MenuWindow);
    free(MenuPtr);

    }

  }
struct Window *DoOpenWindow(short leftedge, short topedge, short width, short height, char *title)
  {
  extern struct RastPort *myScreensRPort;
  extern struct Screen *myScreen;
  struct NewWindow *NWPtr;
  struct Window *WPtr;
  short tlength;
  Do_Stack_Check();
  NWPtr = (struct NewWindow *)malloc(sizeof(struct NewWindow));
  if (NWPtr)
    {
    if (title)
      {
      tlength = (strlen(title) + 3) * myScreensRPort->TxWidth;
      if ((width < tlength) && (topedge + tlength < myScreen->Width))
      width = tlength;

      }
    if ((leftedge < 0) || (topedge < 0) || (width < 0) || (height < 0) ||
    (leftedge + width > myScreen->Width) ||
    (topedge + height > myScreen->Height))
      {
      return(NULL);

      }
    NWPtr->LeftEdge             = leftedge;
    NWPtr->TopEdge              = topedge;
    NWPtr->Width                = width;
    NWPtr->Height               = height;
    NWPtr->DetailPen    = (UBYTE)-1;
    NWPtr->BlockPen             = (UBYTE)-1;
    NWPtr->IDCMPFlags   = NULL;
    NWPtr->Flags                = SMART_REFRESH | NOCAREREFRESH | ACTIVATE | RMBTRAP | WINDOWDEPTH;
    NWPtr->FirstGadget  = NULL;
    NWPtr->CheckMark    = NULL;
    NWPtr->Title                = title;
    NWPtr->Screen               = myScreen;
    NWPtr->BitMap               = NULL;
    NWPtr->MinWidth             = 0;
    NWPtr->MinHeight    = 0;
    NWPtr->MaxWidth             = 32767;
    NWPtr->MaxHeight    = 32767;
    NWPtr->Type         = CUSTOMSCREEN;
    WPtr = OpenWindow(NWPtr);
    free(NWPtr);
    return(WPtr);

    }
  else
    {
    return(NULL);

    }

  }
int CreateInteractiveWindowCon(MenuId id)
  {
  int error;
  Do_Stack_Check();
  if (id == NO_MENU)return(0);
  if (!(id->MenuIOPort = CreatePort(0,0))) return(0);
  if (!(id->MenuIOMsg = (struct IOStdReq *)CreateExtIO(id->MenuIOPort, (LONG)sizeof(struct IOStdReq))))
    {
    return(DeleteInteractiveWindowCon(id));

    }
  id->MenuIOMsg->io_Data = id->MenuWindow;
  id->MenuIOMsg->io_Length = sizeof(struct Window);
  error = OpenDevice("console.device", 0, (struct IORequest *)id->MenuIOMsg, 0);
  id->DeviceOpen = !error;
  return(!error);

  }
int DeleteInteractiveWindowCon(MenuId id)
  {
  Do_Stack_Check();
  if (id == NO_MENU)  return(0);
  if (id->DeviceOpen)
    {
    CloseDevice((struct IORequest *)id->MenuIOMsg);
    id->DeviceOpen = 0;

    }
  if (id->MenuIOMsg)
    {
    DeleteExtIO((struct IORequest *)id->MenuIOMsg);
    id->MenuIOMsg = 0;

    }
  if (id->MenuIOPort)
    {
    DeletePort(id->MenuIOPort);
    id->MenuIOPort = 0;

    }
  return(0);

  }
void IWCWrite(MenuId id, char *str, long len)
  {
  Do_Stack_Check();
  if (id == NO_MENU || !id->DeviceOpen)  return;
  id->MenuIOMsg->io_Command = CMD_WRITE;
  id->MenuIOMsg->io_Data = (APTR)str;
  id->MenuIOMsg->io_Length = len;
  id->MenuIOMsg->io_Message.mn_ReplyPort = id->MenuIOPort;
  DoIO((struct IORequest *)id->MenuIOMsg);

  }
int IWCRead(MenuId id, char *str, long len)
  {
  Do_Stack_Check();
  if (id == NO_MENU || !id->DeviceOpen)  return(0);
  if (!id->MenuIOMsg)
    {
    mPrintf("No MenuIOMsg: system error in sysdep3.c\n");
    return(0);
    }
  id->MenuIOMsg->io_Command = CMD_READ;
  id->MenuIOMsg->io_Data = (APTR)str;
  id->MenuIOMsg->io_Length = len;
  id->MenuIOMsg->io_Message.mn_ReplyPort = id->MenuIOPort;
  DoIO((struct IORequest *)id->MenuIOMsg);
  return(1);

  }
void IWCReadString(MenuId id, char *prompt, char *buf, int lim)
  {
  int  i;
  char c;
  Do_Stack_Check();
  if (id == NO_MENU || !id->DeviceOpen)  return;
  if (strLen(prompt) > 0)
    {
    SysopPrintf(id, "Enter %s\n : ", prompt);

    }
  i = 0;
  while (IWCRead(id, &c, 1) && c != NEWLINE && c != 13)
    {
    /* handle delete chars: */
    if (c == BACKSPACE)
      {
      if (i > 0)
        {
        c = '\b';
        IWCWrite(id, &c, 1);
        c = ' ';
        IWCWrite(id, &c, 1);
        c = '\b';
        IWCWrite(id, &c, 1);
        i--;

        }
      else
        {
        c = BELL;
        IWCWrite(id, &c, 1);

        }

      }
    else
      {
      if (i >= lim)
        {
        c = BELL;
        IWCWrite(id, &c, 1);
        c = BACKSPACE;
        IWCWrite(id, &c, 1);
        i--;

        }
      else
        {
        IWCWrite(id, &c, 1);
        buf[i++] = c;

        }

      }
    if (i >= lim)
      {
      c = BELL;
      IWCWrite(id, &c, 1);

      }

    }
  c = NEWLINE;
  IWCWrite(id, &c, 1);
  buf[i]  = '\0';
  return;

  }
