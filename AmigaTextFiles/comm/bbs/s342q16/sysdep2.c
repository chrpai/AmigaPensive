#include "dos.h"
#include "stdio.h"
#include "ctdl.h"
#include "string.h"
#include "stdarg.h"
#include "clib/dos_protos.h"
#include "minrexx.h"

char *SysVers            = SYSDEP_NAME ;

char ARexx_Name[20];
int  SystemPort          = 0;
#define CITADEL_REQUIRED_STACK  8192L
#ifdef CTDL_BACKGROUND
long _stack              = CITADEL_REQUIRED_STACK;
char *_procname          = "CTDL";
long _priority           = 0;
long _BackGroundIO       = 1;
extern BPTR _Backstdout;
#endif

int  iconify_window      = FALSE;   /* we are not iconified so printf uses
                                       crash.sys for output in startup     */



/* CTDL EXTERNS */
extern CONFIG cfg;
extern char CallSysop;
extern char ExitToMsdos;
extern char whichIO;
extern int exitValue;
extern char onConsole; /* Where IO is ... */
extern logBuffer logBuf;         /* Buffer for the pippuls       */
extern aRoom  roomBuf;           /* Room buffer                  */
extern char ForceNet,anyEcho;


/* EXTERNS */
void Save_Modem(char *,int ,char *);
void Do_Stack_Check(void);
void Dos_Error(long,char *);
void ReActivate_Window(void);  /* allow window to be activated */
int BufferModemFlush(void);
int OpenSerialPorts(void);
int CloseSerial(int final);
void CloseSerialPorts(void);
int Handle_Window_Msg(struct IntuiMessage *InMsg);
int Handle_Little_Window_Msg(struct IntuiMessage *InMsg);
int OpenLittleWindow(void);
int CloseLittleWindow(void);
int BufferConsoleFlush(void);
int OpenConsoleStuff(void);
int CloseConsoleStuff(void);
struct Message *MyGetMsg(struct MsgPort *MyMsgPort);
/*
void  DebugIoRequest (char * string,struct IOExtSer *iorequest);
void DebugIoStdReq(struct IOStdReq *Std);
void Debug_Unit(   struct Unit *Un);
void Debug_Device( struct Device *Dev);
void Debug_Message(struct Message *Msg);
void Debug_Device( struct Device *Dev);
void Debug_space(void);
*/
void timer(unsigned long t[2]);
/* GLOBALS */
extern char *VERSION;
struct IntuitionBase *IntuitionBase = NULL;
struct GfxBase       *GfxBase = NULL;
/*struct Library       *ReqBase = NULL; */
struct RastPort      *myScreensRPort = NULL;
UWORD myScreensBLine = 6;
char ConOpen = FALSE;
char UsingREXX = FALSE;
unsigned char conletter,serletter;
short SerialOpen = 0,TimerOpen = 1,probval,oldfrom,serqueued = FALSE,conqueued = FALSE;
struct IOStdReq    *consoleReadMsg   = NULL;
struct MsgPort     *consoleReadPort  = NULL;
struct IOStdReq    *consoleWriteMsg  = NULL;
struct MsgPort     *consoleWritePort = NULL;
struct IOExtSer    *mySerReadMsg     = NULL;
struct MsgPort     *mySerReadPort    = NULL;
struct IOExtSer    *mySerWriteMsg    = NULL;
struct MsgPort     *mySerWritePort   = NULL;
struct IOExtSer    *mySerStatusMsg   = NULL;
struct MsgPort     *mySerStatusPort  = NULL;
struct timerequest *myTimerMsg       = NULL;
struct MsgPort     *myTimerPort      = NULL;

extern char logNetResults;
extern FILE *netLog;

long char_out;                 /* total characters output */
extern long char_in;           /* total characters input */
extern long start_time;        /* total time for net session */

long total_time;               /* total time for all net sessions */
long total_char_in;            /* totals for all time */
long total_char_out;           /* totals for all time */
int level;

#ifdef HANDLE_FALSE_CARRIER
struct MsgPort     *SpecialMP;
char SpecialMPName[32];
struct MySpecialMessage
  {
  struct Message Message;
  APTR FCAddress;

  }
*SpecialMsg;
#endif
struct Screen *myScreen = NULL;
struct NewScreen myNewScreen =
  {
  0,0,640,200,1,0,1,
  HIRES, CUSTOMSCREEN, NULL, ">>>--> Citadel BBS <--<<<",
  NULL, NULL

  };

char window_title[84];
char NewScreenTitle[84];
char lastuser[28];
char NewWindowTitle[84];
/*
** Screen/Window  Title format
** 1                   23                39
** [ Citadel Version | Date/Time stamp | System Status      ]
** 1                   25                              50
** [ Current User    | Current Room                   | Previous User     ]
*/
struct Window *myWindow = NULL, *LittleWindow = NULL;
struct NewWindow myNewWindow =
  {
  0,10,640,190,(UBYTE)-1,(UBYTE)-1,
  CLOSEWINDOW,
  SMART_REFRESH|NOCAREREFRESH|BORDERLESS|RMBTRAP|
  WINDOWCLOSE|WINDOWSIZING|WINDOWDRAG|WINDOWDEPTH,
  NULL, NULL, NULL, NULL, NULL,
  200,40, 1000,1000, CUSTOMSCREEN

  };
long timerworks = FALSE;
long timercount = 0;
char internalserial;
#ifdef HANDLE_FALSE_CARRIER
char FalseCarrier = 0;
#endif
UBYTE OpenSerFlags;
struct Task    *MyTask    = NULL;
struct Process *MyProcess = NULL;
struct CommandLineInterface *CLI;
ULONG TStackSz, PStackSz, DefStackSz;
char WBWindow,CLINum;
#define TALKTOREXX
#ifdef TALKTOREXX
/*
*   We need our include file.
*/
#include "minrexx.h"
/*
*   These are the REXX functions defined at the bottom of the file.
*/
int disp(struct RexxMsg *msg, struct rexxCommandList *dat, char *p);
void rexxsetchat(struct RexxMsg *msg, char *p);
void rexxsetecho(struct RexxMsg *msg, char *p);
void rexxsetnud(struct RexxMsg *msg, char *p);
void rexxexit(struct RexxMsg *msg, char *p);
void rexxversion(struct RexxMsg *msg, char *p);
void rexxserialenable(struct RexxMsg *msg, char *p);
void rexxopenconsole(struct RexxMsg *msg, char *p);
void rexxiconify(struct RexxMsg *msg, char *p);
#ifdef HANDLE_FALSE_CARRIER
void rexxcarrier(struct RexxMsg *msg, char *p);
#endif
/*
*   Here is our command association list.  Note that in this case,
*   we are setting the userdata field to be a function to call.
*   Dispatch will still take place through moodisp(), so common head
*   and tail stuff can go there.
*
*   Commands are all lower case, so we match either upper or lower.
*   (This is a requirement of minrexx.)
*/
struct rexxCommandList rcl[] =
  {
    { "setchat",          (APTR)rexxsetchat },
    { "setecho",          (APTR)rexxsetecho },
    { "setnouserdisable", (APTR)rexxsetnud },
    { "exit",             (APTR)rexxexit },
    { "version",          (APTR)rexxversion },
    { "serialenable",     (APTR)rexxserialenable },
    { "openconsole",      (APTR)rexxopenconsole },
    { "iconify",          (APTR)rexxiconify },
  #ifdef HANDLE_FALSE_CARRIER
    { "carrier",          (APTR)rexxcarrier },
  #endif
    { NULL, NULL } };
#endif

unsigned long Set_Timer(unsigned long oldtime ) /* return the delta from oldtime in microsecs*/
  {
  unsigned long clock[2];
  timer(clock);      /* get old time */
  return ( clock[0] - oldtime );
  }

static char lognet[80];
static char date2[6], date1[6];
static void Write_Total_Data(void);
void Read_Total_Data(void);

void Read_Total_Data()
  {
  char *mon;
  FILE *ip;
  int lyr, ldum;
  if( cfg.Audit == 0 )return;
  makeAuditName(lognet, "Network_stats.sys");
  if( (ip = fopen(lognet, "r")) != NULL )
    {
    fread(&date1,         6, 1, ip);
    fread(&total_time,    4, 1, ip);
    fread(&total_char_in, 4, 1, ip);
    fread(&total_char_out,4, 1, ip);
    fclose(ip);
    getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
    sprintf(date2,"%3s%2d",mon,lyr);      /* monthYear */
    if( strcmp(date1,date2) != 0 )
      {
      /* reset, new month  */
      getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
      sprintf(date1,"%3s%2d",mon,lyr);      /* monthYear */
      total_time    = 0;
      total_char_in = 0;
      total_char_out= 0;
      Write_Total_Data();
      }
    }
  else   /* no file, create it */
    {
    getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
    sprintf(date1,"%3s%2d",mon,lyr);      /* monthYear */
    total_time    = 0;
    total_char_in  = 0;   /* new file */
    total_char_out = 0;
    Write_Total_Data();
    }
  }

static void Write_Total_Data()
  {
  FILE *ip;
  if( cfg.Audit == 0 )return;
  makeAuditName(lognet, "Network_stats.sys");
  if( (ip = fopen(lognet, "w")) != NULL )
    {
    fwrite(date1,          6, 1, ip);
    fwrite(&total_time,    4, 1, ip);
    fwrite(&total_char_in, 4, 1, ip);
    fwrite(&total_char_out,4, 1, ip);
    fclose(ip);
    }
  }

void Compute_Data(char *name)
  {
  int lyr,ldum;
  char *mon;
  FILE *ip;
  long cps;           /* computed data rate */
  long work_time;
  work_time = Set_Timer(start_time);
  if( logNetResults )
    {
    cps = ( char_in + char_out ) / work_time;
    splitF(netLog," Characters Input:%ld Characters Output:%ld  %ld cps\n", char_in, char_out, cps);
    if( cfg.Audit  )
      {
      makeAuditName(lognet,"Network_Istats.sys");
      if( (ip = fopen(lognet, "a")) != NULL)
        {
        getCdate(&lyr,&mon,&ldum,&ldum,&ldum);
        sprintf(date2,"%3s%2d",mon,lyr);      /* monthYear */
        fprintf(ip,"%s %20s %10ld %10ld %10ld\n",date2,name, char_in, char_out, work_time);
        fclose(ip);
        };

      };
    }
  Read_Total_Data();
  total_time     += work_time;
  total_char_in  += char_in;
  total_char_out += char_out;
  Write_Total_Data();
  }

int nodie(void)
  {
  Do_Stack_Check();
  return(0);

  }
int openStuff(void)
  {
  int error;
  extern char anyEcho;
  extern char BpsSet;
  extern long byteRate;
  extern long BaudTable[];
  #ifdef TALKTOREXX
  /*
  *   If we are talking to REXX, we need this additional local.
  */
  long rexxbit ;
  #endif
  Do_Stack_Check();
  probval   = 99;
  termWidth = 40;
  total_char_in = total_char_out = 0;
  IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",33);
  if (!IntuitionBase)
    {
    Dos_Error(1,"Open intuition.Library failed");
    probval = 1;
    goto thisend;

    }
  GfxBase = (struct GfxBase *)OpenLibrary("graphics.library",0);
  if (!GfxBase)
    {
    Dos_Error(2,"Open graphics.library failed");
    probval = 2;
    goto thisend;

    }
/*
  ReqBase = (struct Library *)OpenLibrary("req.library",0);
  if (!ReqBase)
    {
    Dos_Error(2,"Open req.library failed");
    probval = 2;
    goto thisend;

    }
*/
  MyTask     = FindTask(0L);
  MyProcess  = (struct Process *)MyTask;
  CLI = (struct CommandLineInterface *)(MyProcess->pr_CLI << 2);
  if( CLI )
    {
    DefStackSz = CLI->cli_DefaultStack << 2;
    }
  else
    {
    DefStackSz = MyProcess->pr_StackSize;
    };
  if( DefStackSz < CITADEL_REQUIRED_STACK )
    {
    printf("Current Default Stack = %ld\n",DefStackSz);
    printf("This program needs a stack greater than %ld\n",CITADEL_REQUIRED_STACK);
    Dos_Error(3,"Insufficient Stack Space allocated");
    probval = 3;
    goto thisend;

    }
  WBWindow = cfg.DepData.ScreenDepth;
  error    = OpenConsoleStuff();
  if (error < 0)
    {
    Dos_Error(error,"Console Open Error");
    probval = 11;
    goto thisend;

    }
  #ifdef CTDL_BACKGROUND
  Close(_Backstdout);
  _Backstdout = 0;
  #endif
  anyEcho = cfg.DepData.StartUpEcho;
  onbreak(nodie);
  error = OpenSerialPorts();
  if (error == ERROR)
    {
    probval = 25;
    Dos_Error(probval,"Open Serial Port Failed");
    goto thisend;

    }
  SerialOpen = FALSE;
  myTimerPort = CreatePort(0,0);
  if (!myTimerPort)
    {
    probval = 31;
    Dos_Error(probval,"Create Port Failed");
    goto thisend;

    }
  myTimerMsg = (struct timerequest *)CreateExtIO(myTimerPort, sizeof(struct timerequest));
  if (!myTimerMsg)
    {
    probval = 32;
    Dos_Error(probval,"Create I/O Requests Failed");
    goto thisend;

    }
  TimerOpen = OpenDevice(TIMERNAME,UNIT_VBLANK,(struct IORequest *)myTimerMsg,0);
  if (TimerOpen != 0)
    {
    probval = 33;
    Dos_Error(probval,"Open timer.device failed");
    goto thisend;

    }
  timerworks = TRUE;
  #ifdef TALKTOREXX
  /*
  *   For rexx, we open up a Rexx port, and send out the first command,
  *   if there was one.  We send it out asynchronously; no reason not to.
  */
  sprintf(ARexx_Name,"Citadel_68K");
  if( FindPort(ARexx_Name) != NULL )
    {
    sprintf(ARexx_Name,"Citadel_68K_%d",CLINum);
    if( FindPort(ARexx_Name) != NULL )
      {
      printf("Unique ARexx_Port Name Failure:%s\n",ARexx_Name);
      };
    };
  rexxbit = upRexxPort(ARexx_Name, rcl, "rexx", &disp) ;
  if (rexxbit != 0)
    {
    UsingREXX = TRUE;
    printf("Rexx Port Name [Citadel_68K]\n ");

    }
  #endif
  #ifdef HANDLE_FALSE_CARRIER
  sPrintf(SpecialMPName,"Citadel68K_Spcl_%d",CLINum);
  SpecialMP = CreatePort(SpecialMPName,0);
  if (SpecialMP)
    {
    printf("Special Port Name [%s]\n ",SpecialMPName);
    #ifdef AUTO_EXEC_FALSECARRIER
    sPrintf(NewWindowTitle,"run jays:falsecarrier/fc %d",CLINum);
    Execute(NewWindowTitle,0L,0L);
    WaitPort(SpecialMP);
    SpecialMsg = (struct MySpecialMsg *)GetMsg(SpecialMP);
    if (SpecialMsg)
      {
      SpecialMsg->FCAddress = &FalseCarrier;
      ReplyMsg((struct Message *)SpecialMsg);

      }
    #endif

    }
  #endif
  if (!BpsSet)  byteRate = (BaudTable[cfg.sysBaud] / 10L);
  thisend:
  if (probval != 99)
    {
    fprintf(stderr, "Startup Error Code %d\n",probval);
    }
  else       printf("System Initialized ok\n");
  strcpy(lastuser," ***None*** ");
  return (probval);

  }
void closeStuff(int oldprobval)
  {
  Do_Stack_Check();
  #ifdef TALKTOREXX
  /*
  *   With Rexx, we need to bring the port down.  You might make this
  *   part of exit() for programs that have multiple paths to exit.
  */
  if (UsingREXX) dnRexxPort() ;
  #endif
  #if HANDLE_FALSE_CARRIER
  if (SpecialMP)  DeletePort(SpecialMP);
  #endif
  timerworks = FALSE;
  if ((probval > 34) && !TimerOpen)  CloseDevice((struct IORequest *)myTimerMsg);
  if ((probval > 33) && myTimerMsg)  DeleteExtIO((struct IORequest *)myTimerMsg);
  if ((probval > 32) && myTimerPort) DeletePort(myTimerPort);
  if ((probval > 25) && SerialOpen)  CloseSerial(TRUE);
  if (probval > 25)  CloseSerialPorts();
  if (probval > 11)  CloseConsoleStuff();
  if (LittleWindow)  CloseWindow(LittleWindow);
/*  if (ReqBase)  CloseLibrary(ReqBase); */
  if (GfxBase)  CloseLibrary((struct Library *)GfxBase);
  if (IntuitionBase)  CloseLibrary((struct Library *)IntuitionBase);

  }


#define FULL_BUFFER ( 1279 )
int DoBufferModem,BufferModemCount;
UBYTE ModemBuffer[FULL_BUFFER+1];
int DoBufferConsole,BufferConsoleCount;
char ConsoleBuffer[128];

void BufferingOn(void)
  {
  Do_Stack_Check();
  DoBufferModem = TRUE;
  DoBufferConsole = TRUE;

  }
void BufferingOff(void)
  {
  Do_Stack_Check();
  DoBufferModem = FALSE;
  BufferModemFlush();
  DoBufferConsole = FALSE;
  BufferConsoleFlush();

  }
int BufferModemFlush(void)
  {
  Do_Stack_Check();
  if (SerialOpen && BufferModemCount && gotCarrier())
    {
    mySerWriteMsg->IOSer.io_Data = (APTR)ModemBuffer;
    mySerWriteMsg->IOSer.io_Length = BufferModemCount;
    mySerWriteMsg->IOSer.io_Command = CMD_WRITE;
    char_out += BufferModemCount;
    DoIO((       struct IORequest *)mySerWriteMsg);
    }
  BufferModemCount = 0;
  return(TRUE);

  }
char static_send;
char outMod(int character)
  {
  Do_Stack_Check();
  if(DoBufferModem)
    {
    ModemBuffer[BufferModemCount++] = character;
    if(BufferModemCount > FULL_BUFFER )
      {
      BufferModemFlush();

      }

    }
  else
    {
    if (SerialOpen)
      {
      static_send = character;
      mySerWriteMsg->IOSer.io_Data = (APTR)&static_send;
      mySerWriteMsg->IOSer.io_Length = 1;
      mySerWriteMsg->IOSer.io_Command = CMD_WRITE;
      DoIO(       (struct IORequest *)mySerWriteMsg);
      char_out++;
      }

    }
  return(TRUE);

  }
int QueueSerRead(int where)
  {
  Do_Stack_Check();
  if (!serqueued && SerialOpen)
    {
    mySerReadMsg->IOSer.io_Command = CMD_READ;
    mySerReadMsg->IOSer.io_Data = (APTR)&serletter;
    mySerReadMsg->IOSer.io_Length = 1;
    char_in++;
    SendIO((struct IORequest *)mySerReadMsg);
    serqueued = TRUE;

    }
  return(TRUE);

  }
int UnQueueSerRead(void)
  {
  Do_Stack_Check();
  if (serqueued && SerialOpen)
    {
    AbortIO((struct IORequest *)mySerReadMsg);
    WaitPort(mySerReadPort);
    MyGetMsg(mySerReadPort);
    serqueued = FALSE;

    }
  return(TRUE);

  }
int serPutStr(struct IOExtSer *mess,char *str)
  {
  Do_Stack_Check();
  if (SerialOpen)
    {
    mySerWriteMsg->IOSer.io_Command = CMD_WRITE;
    mySerWriteMsg->IOSer.io_Data = (APTR)str;
    mySerWriteMsg->IOSer.io_Length = strlen(str);
    char_out += mySerWriteMsg->IOSer.io_Length;
    DoIO(       (struct IORequest *)mySerWriteMsg);


    }
  return(TRUE);

  }
#ifdef NEEDED
/*
* This code provided by Gabriel Broner, a s/w engineer at CPT Corp.
*
* First argument is the mask, second argument is a non-ambiguous string to
* check against.
*/
testsamefn (s1, s2)
char *s1, *s2;
  {
  Do_Stack_Check();
  if (*s1 == 0)
    {
    if (*(s1 - 1) == '*' || *s2 == 0)
    return TRUE;
    return FALSE;

    }
  if (*s2 == 0)
  return FALSE;
  if (*s1 == *s2 || *s1 == '?')
  return testsamefn(s1 + 1, s2 + 1);
  if (*s1 == '*')
  return (testsamefn(s1+1, s2) || testsamefn(s1, s2+1));
  return FALSE;

  }
#endif
int OpenSerialPorts(void)
  {
  int erred;
  erred = 0;
  Do_Stack_Check();
  mySerReadPort = CreatePort(0,0);
  if (!mySerReadPort)
    {
    erred = 1;
    Dos_Error(erred,"Create Read Port Failed");

    }
  if (!erred)
    {
    mySerReadMsg = (struct IOExtSer *)CreateExtIO(mySerReadPort, sizeof(struct IOExtSer));
    if (!mySerReadMsg)
      {
      erred = 2;
      Dos_Error(erred,"Create Read ExtIO Failed");

      }

    }
  if (!erred)
    {
    mySerWritePort = CreatePort(0,0);
    if (mySerWritePort == NULL)
      {
      erred = 3;
      Dos_Error(erred,"Create Write Port Failed");

      }

    }
  if (!erred)
    {
    mySerWriteMsg = (struct IOExtSer *)CreateExtIO(mySerWritePort, sizeof(struct IOExtSer));
    if (mySerWriteMsg == NULL)
      {
      erred = 4;
      Dos_Error(erred,"Create Write ExtIO Failed");

      }

    }
  if (!erred)
    {
    mySerStatusPort = CreatePort(0,0);
    if (mySerStatusPort == NULL)
      {
      erred = 5;
      Dos_Error(erred,"Create Status Port Failed");

      }

    }
  if (!erred)
    {
    mySerStatusMsg = (struct IOExtSer *)CreateExtIO(mySerStatusPort,
    sizeof(struct IOExtSer));
    if (mySerStatusMsg == NULL)
      {
      erred = 6;
      Dos_Error(erred,"Create Status ExtIO Failed");

      }

    }
  if (erred)
    {
    CloseSerialPorts();

    }
  return((erred) ? 0 : 1);

  }
int OpenSerial(int starting)
  {
  int serror;
  extern long byteRate;
  ULONG devnum;
  char devname[32];
  Do_Stack_Check();
  if (!SerialOpen)
    {
    strncpy(devname,cfg.DepData.DevName,31);
    devname[31] = '\0';
    devnum = cfg.DepData.UnitNumber;
    if (!strcmp(devname,"serial.device") && devnum != 0)
    internalserial = TRUE;
    else  internalserial = FALSE;
    OpenSerFlags = SERF_XDISABLED | SERF_SHARED|SERF_RAD_BOOGIE;
    if (cfg.DepData.Clock & SER_7WIRE)
      {
      OpenSerFlags |= SERF_7WIRE;
      };
    mySerReadMsg->io_SerFlags = OpenSerFlags;
    serror = OpenDevice(devname,devnum,(struct IORequest *)mySerReadMsg,0);
    if (serror != 0)
      {
      Dos_Error(serror,"Open Device for serial port failed");
      return(ERROR);
      };
    mySerWriteMsg->io_SerFlags = OpenSerFlags;
    serror = OpenDevice(devname,devnum,(struct IORequest *)mySerWriteMsg,0);
    if (serror != 0)
      {
      Dos_Error(serror,"Open Device for serial port failed");
      CloseDevice((struct IORequest *)mySerReadMsg);
      return(ERROR);

      }
    mySerStatusMsg->io_SerFlags = OpenSerFlags;
    serror = OpenDevice(devname,devnum,(struct IORequest *)mySerStatusMsg,0);
    if (serror != 0)
      {
      Dos_Error(serror,"Open Device for serial port failed");
      CloseDevice((struct IORequest *)mySerReadMsg);
      CloseDevice((struct IORequest *)mySerWriteMsg);
      return(ERROR);

      }
    mySerStatusMsg->io_RBufLen  = mySerReadMsg->io_RBufLen  = mySerWriteMsg->io_RBufLen = 2048;
    mySerStatusMsg->io_ReadLen  = mySerReadMsg->io_ReadLen  = mySerWriteMsg->io_ReadLen = 8;
    mySerStatusMsg->io_WriteLen = mySerReadMsg->io_WriteLen = mySerWriteMsg->io_WriteLen = 8;
    mySerStatusMsg->io_StopBits = mySerReadMsg->io_StopBits = mySerWriteMsg->io_StopBits = 1;
    mySerStatusMsg->io_TermArray.TermArray0 = mySerReadMsg->io_TermArray.TermArray0 = mySerWriteMsg->io_TermArray.TermArray0 = NULL;
    mySerStatusMsg->io_TermArray.TermArray1 = mySerReadMsg->io_TermArray.TermArray1 = mySerWriteMsg->io_TermArray.TermArray1 = NULL;
    mySerStatusMsg->IOSer.io_Command = SDCMD_SETPARAMS;
    DoIO(       (struct IORequest *)mySerStatusMsg);
    SerialOpen = TRUE;
    if (byteRate != 0)
      CitadelBaudRate(byteRate, "OpenSerial:byteRate");
    else CitadelBaudRate(cfg.sysBaud,"OpenSerial:sysBaud");
    QueueSerRead(1);

    }
  return(TRUE);

  }
int CloseSerial(int final)
  {
  Do_Stack_Check();
  if (SerialOpen)
    {
    UnQueueSerRead();
    CloseDevice((struct IORequest *)mySerStatusMsg);
    CloseDevice((struct IORequest *)mySerWriteMsg);
    CloseDevice((struct IORequest *)mySerReadMsg);
    SerialOpen = FALSE;
    return(FALSE);

    }
  return(TRUE);

  }
void CloseSerialPorts(void)
  {
  Do_Stack_Check();
  if (mySerStatusMsg)
    {
    DeleteExtIO((struct IORequest *)mySerStatusMsg);
    mySerStatusMsg = NULL;

    }
  if (mySerWriteMsg)
    {
    DeleteExtIO((struct IORequest *)mySerWriteMsg);
    mySerWriteMsg = NULL;

    }
  if (mySerReadMsg)
    {
    DeleteExtIO((struct IORequest *)mySerReadMsg);
    mySerReadMsg = NULL;

    }
  if (mySerStatusPort)
    {
    DeletePort(mySerStatusPort);
    mySerStatusPort = NULL;

    }
  if (mySerWritePort)
    {
    DeletePort(mySerWritePort);
    mySerWritePort = NULL;

    }
  if (mySerReadPort)
    {
    DeletePort(mySerReadPort);
    mySerReadPort = NULL;

    }

  }
char time_in[30];
char sstatus[30];
int timeupdatecount,old_user;
int old_shr, old_smn;
void ScrTimeUpdate(int shr, int smn)
  {
  Do_Stack_Check();
  timeupdatecount = 0;
  if (!ConOpen) return;
  if( old_shr == shr && old_smn == smn )return;
  old_shr = shr;
  old_smn = smn;
  if (myScreensRPort)
    {
    sPrintf(NewScreenTitle,"Citadel 68K V%s.%s| %2d:%02d %7s | %-30s"
    ,VERSION,SysVers,shr,smn,formDate(),sstatus);
    SetWindowTitles(myWindow,window_title,NewScreenTitle);
    }

  }
void ScrNewUser(void)
  {
  char temp_title[84],*smo;
  int syr,sdy,shr,smn;
  Do_Stack_Check();
  getCdate(&syr,&smo,&sdy,&shr,&smn);
  if (!ConOpen || !myWindow) return;
  if (!old_user && logBuf.lbname[0])
    {
    sPrintf(time_in,"In: %2d:%02d                    ",shr,smn);
    old_user = 1;

    }
  if (!logBuf.lbname[0] && old_user)
    {
    strcpy(time_in," *** None ***               ");
    old_user = 0;

    }
  sPrintf(temp_title,"User:%-19s|Room:%-19s|%c%c%c%s|Last:%s"
  ,logBuf.lbname,  roomBuf.rbname,
  (cfg.BoolFlags.noChat) ?  ' ' : 'C',
  (cfg.BoolFlags.debug)  ?  'D' : ' ',
  (anyEcho)              ?  'E' : ' ',
  (CallSysop)            ? "^T" :((ForceNet) ? "^A" : "  "),
  lastuser);
  strncpy(window_title,temp_title,83);
  old_smn = -1;
  SpecialMessage(time_in);
  }
char oldmsg[30];
void SpecialMessage(char *message)
  {
  short j;
  char realmsg[30];
  char *smo;
  int syr,sdy,shr,smn;
  Do_Stack_Check();
  timeupdatecount = 0;
  if (!ConOpen) return;
  if (myScreensRPort)
    {
    j = strlen(message);
    memset(realmsg,'\0',30);
    if( j > 29)j = 29;
    strncpy(realmsg,message,j);
    realmsg[j] = '\0';
    if( strcmp(oldmsg,realmsg) != 0 )      /* do not update unless change*/
      {
      sprintf(sstatus,"%-30s",realmsg);
      getCdate(&syr,&smo,&sdy,&shr,&smn);
      ScrTimeUpdate(shr, smn);
      strcpy(oldmsg,realmsg);
      };
    }

  }
/************************************************************************/
/*   pause() busy-waits N/100 seconds            */
/************************************************************************/
void pause(int i)
  {
  ULONG secs, micros;
  Do_Stack_Check();
  if( myTimerMsg == NULL)return;
  secs = i / 100;
  micros = (i - (secs * 100)) * 10000;
  if (!secs && !micros)  micros = 10000;
  if (micros < 0 || micros > 999999)
    {
    printf("[pause(%d) micros = %ld]",i,micros);
    micros = 10000;

    }
  myTimerMsg->tr_node.io_Command = TR_ADDREQUEST;
  myTimerMsg->tr_node.io_Message.mn_ReplyPort = myTimerPort;
  myTimerMsg->tr_time.tv_secs = secs;
  myTimerMsg->tr_time.tv_micro = micros;
  DoIO((struct IORequest *)myTimerMsg);

  }
void MilliSecPause(int x)
  {  /* really a 100 microsecond delay per x */
  long micros;
  Do_Stack_Check();
  if( myTimerMsg == NULL)return;
  if( x < 1 || x > 1000)x = 1000;  /* out of range, give max */
  micros = x * 100;                /* time in 100 microsecond units */
  myTimerMsg->tr_node.io_Command = TR_ADDREQUEST;
  myTimerMsg->tr_node.io_Message.mn_ReplyPort = myTimerPort;
  myTimerMsg->tr_time.tv_secs  = 0;
  myTimerMsg->tr_time.tv_micro = micros;
  DoIO((struct IORequest *)myTimerMsg);

  }
void timer(unsigned long t[2])
  {
  Do_Stack_Check();
  if( myTimerMsg == NULL)return;
  myTimerMsg->tr_node.io_Message.mn_ReplyPort = myTimerPort;
  myTimerMsg->tr_node.io_Command = TR_GETSYSTIME;
  DoIO((struct IORequest *)myTimerMsg);
  t[0] = myTimerMsg->tr_time.tv_secs;
  t[1] = myTimerMsg->tr_time.tv_micro;

  }
#ifdef TALKTOREXX
/*
*   Now we get into the actual code necessary for our REXX port; functions
*   that do the real work.  Note that this program was not structured
*   particularly nicely for Rexx; I had to write each of these functions.
*   Many programs have these subroutines already in place; they are called
*   as part of the event loop.  This progam, however, just has one big
*   switch statement with different actions . . .
*
*   First, our locals.
*/
int args[4] ;              /* what args did we see to this function? */
int parsed ;               /* was argument parsing successful? */
int userreplied ;          /* has the current message been replied to yet? */
/*
*   This function takes a pointer to a pointer to a string, grabs the
*   next number, returns it, and advances the pointer to the string to
*   point after the number.
*/
int getnm(char **where)
  {
  register char *p = *where ;
  register int val = 0 ;
  int gotone = 0 ;
  Do_Stack_Check();
  while (*p <= ' ' && *p)  p++ ;
  while ('0' <= *p && *p <= '9')
    {
    gotone = 1 ;
    val = 10 * val + *p++ - '0' ;

    }
  if (gotone == 0)  parsed = 0 ;
  *where = p ;
  return(val) ;

  }
/*
*   This function trys to find `n' numeric arguments in the command
*   string, and stuffs them into the args array.
*/
void parseargs(char *p, int n)
  {
  register int i ;
  Do_Stack_Check();
  while (*p > ' ' && *p)  p++ ;
  for (i=0; i<n; i++)  args[i] = getnm(&p) ;

  }
/*
*   This is our main dispatch function.  We check to make sure a Window
*   currently exists.  Then, we store away the `current color' and change
*   it to Rexx's current color, call our handler function, and then restore
*   the color.  If our handler replied, we return a 1 to indicate that.
*   If the parse and everything else was successful, we return a 0.
*   Otherwise, we return a failure of 20 to indicate that the arguments
*   were messed up.
*/
int disp(struct RexxMsg *msg, struct rexxCommandList *dat, char *p)
  {
  parsed = 1 ;
  Do_Stack_Check();
  userreplied = 0 ;
  ((int (*)())(dat->userdata))(msg, p) ;
  if (! parsed) replyRexxCmd(msg, (long)parsed, 0L, NULL) ;
  return TRUE;

  }
#endif
#ifdef TALKTOREXX
char dorexxexit = FALSE;
#endif
int Amiga_System_Input(void)
  {
  struct IntuiMessage *InMsg;
  Do_Stack_Check();
  #ifdef TALKTOREXX
  /*
  *   Handle any Rexx messages.
  */
  if (UsingREXX)  dispRexxPort() ;
  #endif
  #ifdef HANDLE_FALSE_CARRIER
  if (SpecialMP)
    {
    SpecialMsg = (struct MySpecialMsg *)GetMsg(SpecialMP);
    if (SpecialMsg)
      {
      SpecialMsg->FCAddress = &FalseCarrier;
      ReplyMsg(SpecialMsg);

      }

    }
  #endif
  if (ConOpen)
    {
    InMsg = (struct IntuiMessage *)GetMsg(myWindow->UserPort);
    if (InMsg != NULL) Handle_Window_Msg(InMsg);

    }
  if (LittleWindow != NULL)
    {
    InMsg = (struct IntuiMessage *)GetMsg(LittleWindow->UserPort);
    if (InMsg != NULL) Handle_Little_Window_Msg(InMsg);

    }
  return(0);

  }
#ifdef TALKTOREXX
void rexxsetchat(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 1) ;
  cfg.BoolFlags.noChat = (args[0]) ? FALSE : TRUE;
  ScrNewUser();

  }
void rexxsetecho(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 1) ;
  anyEcho = (args[0]) ? TRUE : FALSE;
  ScrNewUser();

  }
void rexxsetnud(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 1) ;
  CallSysop = (args[0]) ? TRUE : FALSE;
  ScrNewUser();

  }
extern char haveCarrier;
char retstr[4];
void rexxexit(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 1) ;
  strcpy(retstr,"0");
  if (!haveCarrier || args[0] != 0)
    {
    ExitToMsdos = TRUE;
    exitValue = 0;
    strcpy(retstr,"1");

    }
  userreplied = 1;
  replyRexxCmd(msg, 0L, 0L, retstr) ;

  }
void rexxversion(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  userreplied = 1 ;
  replyRexxCmd(msg, 0L, 0L, VERSION) ;

  }
void rexxserialenable(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 2) ;
  strcpy(retstr,"0");
  if (!haveCarrier || args[1] != 0)
    {
    if (args[0])
      {
      SerialOpen = OpenSerial(FALSE);
      if (SerialOpen == TRUE) strcpy(retstr,"1");

      }
    else
      {
      SerialOpen = CloseSerial(FALSE);
      if (!SerialOpen) strcpy(retstr,"1");

      }

    }
  userreplied = 1;
  replyRexxCmd(msg, 0L, 0L, retstr) ;

  }
void rexxopenconsole(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 1) ;
  strcpy(retstr,"0");
  if (args[0])
    {
    if (!ConOpen)
      {
      OpenConsoleStuff();
      iconify_window = FALSE;
      };
    }
  else
    {
    if (ConOpen)
      {
      if (whichIO == MODEM)
        {
        CloseConsoleStuff();
        iconify_window = TRUE;
        };
      };
    };
  if (ConOpen)
  strcpy(retstr,"1");
  else
  strcpy(retstr,"0");
  userreplied = 1;
  replyRexxCmd(msg, 0L, 0L, retstr) ;

  }
void rexxiconify(struct RexxMsg *msg, char *p)
  {
  int error;
  Do_Stack_Check();
  parseargs(p, 1) ;
  strcpy(retstr,"0");
  if (args[0])
    {
    if (ConOpen)
      {
      if (whichIO == MODEM)
        {
        error = OpenLittleWindow();
        if (error == TRUE)
          {
          CloseConsoleStuff();
          strcpy(retstr,"1");
          iconify_window = TRUE;
          }

        }

      }

    }
  else
    {
    if (!ConOpen)
      {
      error = OpenConsoleStuff();
      if (error > 0)
        {
        strcpy(retstr,"1");
        if (LittleWindow != NULL)
        CloseLittleWindow();
        iconify_window = FALSE;

        }

      }

    }

  userreplied = 1;
  replyRexxCmd(msg, 0L, 0L, retstr) ;

  }
#ifdef HANDLE_FALSE_CARRIER
void rexxcarrier(struct RexxMsg *msg, char *p)
  {
  Do_Stack_Check();
  parseargs(p, 1) ;
  FalseCarrier = (args[0]) ? TRUE : FALSE;

  }
#endif
#endif
int Handle_Window_Msg(struct IntuiMessage *InMsg)
  {
  ULONG msgclass;
  int err;
  Do_Stack_Check();
  msgclass = InMsg->Class;
  ReplyMsg((struct Message *)InMsg);
  if (msgclass == CLOSEWINDOW)
    {
    if (whichIO == MODEM)
      {
      err = OpenLittleWindow();
      if (err == FALSE)
        {
        DisplayBeep(NULL);

        }
      else
        {
        CloseConsoleStuff();

        }

      }

    }
  return(TRUE);

  }
int Handle_Little_Window_Msg(struct IntuiMessage *InMsg)
  {
  int err;
  Do_Stack_Check();
  ReplyMsg((struct Message *)InMsg);
  err = OpenConsoleStuff();
  if (err < 1)
    {
    DisplayBeep(NULL);

    }
  else
    {
    CloseLittleWindow();

    }
  return(TRUE);

  }
struct NewWindow NewLittleWindow =
  {
  100,0,188,12,(UBYTE)-1,(UBYTE)-1,
  CLOSEWINDOW,
  WINDOWDEPTH | WINDOWCLOSE | WINDOWDRAG | SMART_REFRESH | RMBTRAP,
  NULL, NULL,
  (UBYTE *)"Amiga Citadel",
  NULL,NULL,0,0,1000,1000,WBENCHSCREEN

  };
int OpenLittleWindow(void)
  {
  Do_Stack_Check();
  if (LittleWindow == NULL)
    {
    LittleWindow = OpenWindow(&NewLittleWindow);

    }
  if (LittleWindow != NULL)
  return(1);
  return(0);

  }
int CloseLittleWindow(void)
  {
  Do_Stack_Check();
  if (LittleWindow != NULL)
    {
    CloseWindow(LittleWindow);
    LittleWindow = NULL;
    return(1);

    }
  else return(0);

  }

int Jsystem(char *cmdline)
  {
  long ecode;
  char dir[80];
  BPTR nilfh;
  Do_Stack_Check();
  if (cfg.BoolFlags.debug)
    {
    (void)getcd(0,dir);
    splitF(netLog, " Dir:%s\n Command:%s\n",dir,cmdline);
    };
  (void)SetIoErr(0);       /* initialize the error code */
  pause(1);
  nilfh = Open("NIL:",MODE_OLDFILE);
  Execute(cmdline,NULL,nilfh);  /* execute the command */
  ecode = IoErr();          /* check for error */
  if( ecode != 0 )Dos_Error(ecode,cmdline);
  return((int)ecode);
  }

void Dos_Error(err,str)   /* format a AmigaDos Error and put in debug.sys */
long err;
char *str;
  {
  char *mon;
  int yr,day,hour,min;
  char buf[80];
  char CLogFn[80];            /* buffer for calllog.sys */
  Do_Stack_Check();
  if( cfg.Audit == 0)return;  /* exit if the Sysop has not configed Auditing*/
  makeAuditName(CLogFn, "debug.sys");
  getCdate(&yr,&mon,&day,&hour,&min);
  sprintf(buf,"%2d%s%02d %d:%02d - Dos Error:%ld",yr,mon,day,hour,min,err);
  CallMsg(CLogFn,buf);        /* time and date of the error */
  sprintf(buf,"%s",str);
  CallMsg(CLogFn, buf);       /* report the command */
  (void)Fault(err,"Reason:",buf,80);
  CallMsg(CLogFn, buf);       /* report the error in text */
  (void)getcd(0,buf);
  CallMsg(CLogFn, buf);       /* report the current directory */
  pause(3);
  }

void ReActivate_Window()
  {
  Do_Stack_Check();
  if( !ConOpen ) OpenConsoleStuff();/* reactivate window if closed */
  ScreenToFront(myWindow->WScreen); /* make sure that the screen is front */
  WindowToFront(myWindow);          /* make sure the window is in front */
  ActivateWindow(myWindow);         /* activate, it can't hurt */
  }

int QueueRead(struct IOStdReq *request,char *whereto)
  {
  Do_Stack_Check();
  if (ConOpen && !conqueued)
    {
    consoleReadMsg->io_Command = CMD_READ;
    consoleReadMsg->io_Data = (APTR)&conletter;
    consoleReadMsg->io_Length = 1;
    SendIO((struct IORequest *)consoleReadMsg);
    conqueued = TRUE;

    }
  return(TRUE);

  }
int UnQueueRead(void)
  {
  Do_Stack_Check();
  if (conqueued && ConOpen)
    {
    AbortIO((struct IORequest *)consoleReadMsg);
    WaitPort(consoleReadPort);
    MyGetMsg(consoleReadPort);
    conqueued = FALSE;

    }
  return(TRUE);

  }
int ConPutStr(char *myString)
  {
  Do_Stack_Check();
  if (!ConOpen || !myString || !strlen(myString))  return(0);
  consoleWriteMsg->io_Command = CMD_WRITE;
  consoleWriteMsg->io_Data = (APTR)myString;
  consoleWriteMsg->io_Length = strlen(myString);
  DoIO((struct IORequest *)consoleWriteMsg);
  return(1);

  }
int BufferConsoleFlush(void)
  {
  Do_Stack_Check();
  if (ConOpen && BufferConsoleCount)
    {
    consoleWriteMsg->io_Data = (APTR)ConsoleBuffer;
    consoleWriteMsg->io_Length = BufferConsoleCount;
    consoleWriteMsg->io_Command = CMD_WRITE;
    DoIO((struct IORequest *)consoleWriteMsg);

    }
  BufferConsoleCount = 0;
  return(TRUE);

  }
int ConPutChar(char myChar)
  {
  Do_Stack_Check();
  if (DoBufferConsole)
    {
    ConsoleBuffer[BufferConsoleCount++] = myChar;
    if(BufferConsoleCount > 126)
      {
      BufferConsoleFlush();

      }

    }
  else
    {
    if (ConOpen)
      {
      consoleWriteMsg->io_Command = CMD_WRITE;
      consoleWriteMsg->io_Data = (APTR)&myChar;
      consoleWriteMsg->io_Length = 1;
      DoIO((struct IORequest *)consoleWriteMsg);

      }

    }
  return(TRUE);

  }
int OpenConsoleStuff(void)
  {
  UWORD tmpcolors[2];
  int error,erred;
  Do_Stack_Check();
  if (ConOpen)  return(1);
  erred = 0;
  if (!WBWindow)
    {
    myNewScreen.Width     = cfg.DepData.ScreenWidth;
    myNewScreen.Height    = cfg.DepData.ScreenHeight;
    myNewScreen.Depth     = 1;  /* 3;   /* 8 colors */
    if (cfg.DepData.ScreenHeight > 399) myNewScreen.ViewModes |= LACE;
    sPrintf(NewScreenTitle,"Amiga Citadel V%s.%s",VERSION,SysVers);
    myNewScreen.DefaultTitle = (UBYTE *)NewScreenTitle;
    myScreen = (struct Screen *)OpenScreen(&myNewScreen);
    if (!myScreen) return(-1);
    myNewWindow.Screen = myScreen;
    myScreensRPort = &(myScreen->RastPort);
    myScreensBLine = myScreensRPort->Font->tf_Baseline + 1;

    }
  else
    {
    myNewWindow.Type    = WBENCHSCREEN;
    myNewWindow.Screen  = NULL;
    myScreensRPort      = NULL;
    myNewWindow.TopEdge = 0;

    }
  strcpy(NewWindowTitle,"Citadel Console");
  myNewWindow.Title = (UBYTE *)NewWindowTitle;
  myNewWindow.Width = cfg.DepData.ScreenWidth;
  if (!WBWindow)
    {
    myNewWindow.Height     = cfg.DepData.ScreenHeight - 10;
    myNewWindow.Flags     &= ~(WINDOWSIZING | WINDOWDRAG);

    }
  else  myNewWindow.Height = cfg.DepData.ScreenHeight;
  myWindow                 = OpenWindow(&myNewWindow);
  if (!myWindow)
    {
    if(myScreen)
      {
      CloseScreen(myScreen);
      myScreen = NULL;

      }
    return(-2);

    };
  if (myScreen)
    {
    tmpcolors[0] = cfg.DepData.Color0;
    tmpcolors[1] = cfg.DepData.Color1;
    LoadRGB4(ViewPortAddress(myWindow),tmpcolors,2);
    ((struct Process *)FindTask(0))->pr_WindowPtr = (APTR)myWindow;

    };
  consoleReadPort = CreatePort(0,0);
  if (!consoleReadPort)
    {
    erred = 3;

    }
  if (!erred)
    {
    consoleReadMsg = (struct IOStdReq *)CreateExtIO(consoleReadPort, sizeof(struct IOStdReq));
    if (!consoleReadMsg)
      {
      erred = 4;

      }

    }
  if (!erred)
    {
    consoleWritePort = CreatePort(0,0);
    if (!consoleWritePort)
      {
      erred = 5;

      }

    }
  if (!erred)
    {
    consoleWriteMsg = (struct IOStdReq *)CreateExtIO(consoleWritePort, sizeof(struct IOStdReq));
    if (!consoleWriteMsg)
      {
      erred = 6;

      }

    }
  if (!erred)
    {
    consoleReadMsg->io_Data = (APTR)myWindow;
    consoleReadMsg->io_Length = sizeof(struct Window);
    error = OpenDevice("console.device",0,(struct IORequest *)consoleReadMsg,0);
    if (error)
      {
      erred = 7;

      }

    }
  if (!erred)
    {
    consoleWriteMsg->io_Data = (APTR)myWindow;
    consoleWriteMsg->io_Length = sizeof(struct Window);
    consoleWriteMsg->io_Device = consoleReadMsg->io_Device;
    consoleWriteMsg->io_Unit = consoleReadMsg->io_Unit;
    ConOpen = TRUE;
    printf("Console opened: Columns = %d, Rows = %d\n ",
    myNewWindow.Width / myWindow->RPort->TxWidth, (myNewWindow.Height - myWindow->RPort->TxHeight - 2) / myWindow->RPort->TxHeight);
    QueueRead(consoleReadMsg,&conletter);

    }
  ReActivate_Window();
  if (erred)
    {
    CloseConsoleStuff();

    }
  return(0-erred);

  }
int CloseConsoleStuff(void)
  {
  Do_Stack_Check();
  UnQueueRead();
  if (ConOpen)  CloseDevice((struct IORequest *)consoleReadMsg);
  ConOpen = FALSE;
  if (consoleWriteMsg)
    {
    DeleteExtIO((struct IORequest *)consoleWriteMsg);
    consoleWriteMsg = NULL;

    }
  if (consoleWritePort)
    {
    DeletePort(consoleWritePort);
    consoleWritePort = NULL;

    }
  if (consoleReadMsg)
    {
    DeleteExtIO((struct IORequest *)consoleReadMsg);
    consoleReadMsg = NULL;

    }
  if (consoleReadPort)
    {
    DeletePort(consoleReadPort);
    consoleReadPort = NULL;

    }
  if (myWindow)
    {
    if (((struct Process *)FindTask(0))->pr_WindowPtr == myWindow)
    ((struct Process *)FindTask(0))->pr_WindowPtr = (APTR)NULL;
    CloseWindow(myWindow);
    myWindow = NULL;

    }
  if (myScreen)
    {
    CloseScreen(myScreen);
    myScreen = NULL;

    }
  return(TRUE);

  }
struct Message *MyGetMsg(struct MsgPort *MyMsgPort)
  {
  struct Message *RepliedMessage;
  Do_Stack_Check();
  if (RepliedMessage = GetMsg(MyMsgPort))
    {
    SetSignal(0L,(1L << MyMsgPort->mp_SigBit));

    }
  return(RepliedMessage);

  }
int SysdepCheckArg(char *arg)
  {
  Do_Stack_Check();
  return(0);

  }
/*
 * CitSystem()
 *
 * This function formats the format & arguments and then runs the result via
 * system().
 */
int CitSystem(char RestoreVideo, char *format, ...)
{
    va_list argptr;
    char *garp;
    extern char *bigbuffer;
    Do_Stack_Check();
    garp = bigbuffer;
    va_start(argptr, format);
    vsprintf(garp, format, argptr);
    va_end(argptr);
/*    if (RestoreVideo) StopVideo();  */
    Jsystem(garp);
/*    if (RestoreVideo) VideoInit();  */
    return 0;
}
unsigned long S_min = 0xFFFFFFFF;  /* min value seen */
unsigned long S_max = 0x00000000;  /* max value seen */
void Do_Stack_Check()
  {
  unsigned long S_current;
  S_current = getreg(REG_A7);  /* grab current stack pointer */
  if( S_current < S_min )S_min = S_current;
  if( S_current > S_max )S_max = S_current;
  }
/*
** functions for caller count and user privledges
*/
char logcall[100];
long Get_Call_Count()
  {
  FILE *ip;
  long count=0;
  if( cfg.Audit == 0)return 0;  /* exit if the Sysop has not configed Auditing*/
  makeAuditName(logcall, "user_count.sys");
  if( (ip = fopen(logcall,"r")) != NULL)
    {
    fread(&count,4,1,ip);
    fclose(ip);
    };
  return count;
  }

void Update_Caller_Count()
  {
  FILE *ip;
  long count=0;
  if( cfg.Audit == 0 ) return; /* Audit Area not setup */
  makeAuditName(logcall, "user_count.sys");
  if( (ip = fopen(logcall,"rb")) != NULL)
    {
    fread(&count,4,1,ip);
    fclose(ip);
    };
  if( (ip = fopen(logcall,"wb")) != NULL)
    {
    fseek(ip,0,SEEK_SET);
    count++;
    fwrite(&count,4,1,ip);
    fclose(ip);
    };
  }

char *Display_Privledges()
  {
  extern char      loggedIn;  /* Are we logged in?       */
  int count=0;                /* check for no privileges */
  logcall[0] = '\0';          /* initialize string       */
  if( loggedIn )
    {
    if (logBuf.lbflags.AIDE)
      {
      count++;
      strcat(logcall,"Aide, ");
      };

    if (logBuf.lbflags.NET_PRIVS)
      {
      count++;
      strcat(logcall,"NET, ");
      };

    if (logBuf.lbflags.DOOR_PRIVS)
      {
      count++;
      strcat(logcall,"Door, ");
      };

    if (logBuf.lbflags.DL_PRIVS)
      {
      count++;
      strcat(logcall,"File, ");
      };

    if (logBuf.lbflags.PERMANENT)
      {
      count++;
      strcat(logcall,"Permanent, ");
      };

    if (logBuf.lbflags.TWIT)
      {
      count++;
      logcall[0]='\0';
      strcat(logcall,"Full ");
      };
    };
  if( count == 0)strcpy( logcall, " No ");
  count = strlen(logcall);
  if( logcall[count-2] == ',')logcall[count-2] = '\0';
  return &logcall[0];
  }
/*
void Debug_space()
  {
  short i;
  for(i=0;i<level;i++)splitF(netLog," ");
  }
void Debug_Message(struct Message *Msg)
  {
  level++;
  Debug_space();splitF(netLog," struct Message:%08lX\n",Msg);
  level--;
  }
void Debug_Device( struct Device *Dev)
  {
  level++;
  Debug_space();splitF(netLog," struct Device:%08lX\n",Dev);  level--;
  }
void Debug_Unit(   struct Unit *Un)
  {
  level++;
  Debug_space();splitF(netLog," struct Unit :%08lX\n",Un);  level--;
  level--;
  }
int debug_count=0;
void DebugIoStdReq(struct IOStdReq *Std)
  {
  Debug_space();splitF(netLog,"struct IOStdReq:%08lX\n",Std);
  level++;
  Debug_Message(&Std->io_Message);
  Debug_Device( Std->io_Device);
  Debug_Unit(   Std->io_Unit);
  Debug_space();splitF(netLog,"io_Command:%04X\n",Std->io_Command);
  Debug_space();splitF(netLog,"  io_Flags:%02X\n",Std->io_Flags);
  Debug_space();splitF(netLog,"  io_Error:%02X\n",Std->io_Error);
  Debug_space();splitF(netLog," io_Actual:%08lX\n",Std->io_Actual);
  Debug_space();splitF(netLog," io_Length:%08lX\n",Std->io_Length);
  Debug_space();splitF(netLog,"   io_Data:%08lX\n",Std->io_Data);
  Debug_space();splitF(netLog," io_Offset:%08lX\n",Std->io_Offset);
  level--;
  }
void  DebugIoRequest (char * string,struct IOExtSer *iorequest)
  {
        int  mask = iorequest->io_SerFlags;
  if (!cfg.BoolFlags.debug)return;
  if( (debug_count++ % 1000 ) != 0 )return;
  Debug_space();splitF(netLog," %s=struct IOExtSer:%08lX\n",string,iorequest);
  DebugIoStdReq(&iorequest->IOSer);
  Debug_space();splitF(netLog,"CtlChar    0x%lx \n", iorequest->io_CtlChar);
  Debug_space();splitF(netLog,"RBufLen    %ld   \n", iorequest->io_RBufLen);
  Debug_space();splitF(netLog,"ExtFlags   0x%lx \n", iorequest->io_ExtFlags);
  Debug_space();splitF(netLog,"Baud       %ld   \n", iorequest->io_Baud);
  Debug_space();splitF(netLog,"BrkTime    %ld   \n", iorequest->io_BrkTime);
  Debug_space();splitF(netLog,"ReadLen    %ld   \n", iorequest->io_ReadLen);
  Debug_space();splitF(netLog,"WriteLen   %ld   \n", iorequest->io_WriteLen);
  Debug_space();splitF(netLog,"StopBits   %ld   \n", iorequest->io_StopBits);
  Debug_space();splitF(netLog,"SerFlags   0x%lx \n", iorequest->io_SerFlags);
        if (mask)
          {
    Debug_space();splitF(netLog,"           ");
                if (mask & SERF_XDISABLED)  Debug_space();splitF(netLog,"XDISABLED ");
                if (mask & SERF_EOFMODE)    Debug_space();splitF(netLog,"EOFMODE ");
                if (mask & SERF_SHARED)     Debug_space();splitF(netLog,"SHARED ");
                if (mask & SERF_RAD_BOOGIE) Debug_space();splitF(netLog,"RAD_BOOGIE ");
                if (mask & SERF_QUEUEDBRK)  Debug_space();splitF(netLog,"QUEUEDBRK ");
                if (mask & SERF_7WIRE)      Debug_space();splitF(netLog,"7WIRE ");
                if (mask & SERF_PARTY_ODD)  Debug_space();splitF(netLog,"PARTY_ODD ");
                if (mask & SERF_PARTY_ON)   Debug_space();splitF(netLog,"PARTY_ON");
    Debug_space();splitF(netLog,"\n");
          }
  Debug_space();splitF(netLog,"Status     0x%lx \n", iorequest->io_Status);
        return;
}
*/
