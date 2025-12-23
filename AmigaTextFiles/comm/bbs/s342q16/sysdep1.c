/************************************************************************/
/*                              sysdep1.c                               */
/*                                                                      */
/*      This is the repository of most of the system dependent code     */
/*      in Citadel.  We hope, pray, and proselytize, at least.          */
/************************************************************************/
/************************************************************************/
/*                              history                                 */
/*                                                                      */
/* 89Oct16 AP   Amiga                                                   */
/* 87Apr01 HAW  File tagging completed; bug for .RD/.RE fixed.          */
/* 86Dec14 HAW  Reorganized into areas.                                 */
/* 86Nov25 HAW  Created.                                                */
/************************************************************************/
#define SYSTEM_DEPENDENT
#define TIMER_FUNCTIONS_NEEDED
#include "ctdl.h"
#include "dos.h"
#include "time.h"
#include "string.h"
#include "stdarg.h"
/************************************************************************/
/*                              Contents                                */
/*                                                                      */
/*              AREAS:                                                  */
/*      bigDirectory()          gets an "extended" directory            */
/* #    bigDirs()               work function for bigDirectory()        */
/*      freeFileList()          Free file list                          */
/*      getArea()               get an area from the sysop              */
/*      goodArea()              get good area                           */
/*      homeSpace()             takes us to our home space              */
/*      netGetArea()            get area for dumping networked file(s)  */
/*      netSetNewArea()         set area for dumping networked file(s)  */
/*      prtNetArea()            makes human readable form of NET_AREA   */
/*      setSpace()              goto specified "area"                   */
/* #    realSetSpace()          does work of setSpace, others           */
/* #    MSDOSparse()            parses a string for MSDOS filename      */
/* #    fileType()              gets file type for MSDOS                */
/*      sysGetSendFiles()       specify where to send files from        */
/*      updFiletag()            updates a filetag                       */
/*      sysRoomLeft()           how much room left in net recept area   */
/* #    getSize()               gets size of a file                     */
/*      sysSendFiles()          system dep stuff for sending files      */
/* #    doSendWork()            does work of sysSendFiles()             */
/*              BAUD HANDLER:                                           */
/* #    check_CR()              scan input for carriage returns         */
/*      Find_baud()             does flip flop search for baud          */
/*      getNetBaud()            check for baud of network caller        */
/*              CONSOLE HANDLING:                                       */
/*      KBReady()               returns TRUE if a console char is ready */
/*      mputChar()              Do our own for some Console output      */
/*                                                                      */
/*              # == local for this implementation only                 */
/************************************************************************/
#define NO_IDEA         0
#define SINGLE_FILE     1
#define IS_DIR          2
#define AMB_FILE        3
/************************************************************************/
/*      Globals -- there shouldn't be anything here but statics and     */
/*      externs.                                                        */
/************************************************************************/
/* These MUST be defined! */
short ConDelay;
char *R_W_ANY     = "rb+";
char *READ_ANY    = "rb";
char *READ_TEXT   = "ra";
char *APPEND_TEXT = "aa";
char *APPEND_ANY  = "ab";
char *A_C_TEXT    = "ab";
char *WRITE_TEXT  = "wa";
char *W_R_ANY     = "wb+";
char *WRITE_ANY   = "wb";
static FILE *fileTags;                  /* For the file tags            */

char results_debug=FALSE;  /* turn on to debug modem results codes */
char results_cid=FALSE;    /* turn on to collect caller ID info if present */


int Jsystem(char *);
void Do_Stack_Check(void);

char *rates[] =
  {
  "300",   "1200",  "2400",   "4800",    "9600",
  "14400", "19200", "38400", "57600", "Unknown"

  };
static short LastBaudIndex;

/* Here's the rest of the goo */
extern logBuffer logBuf;         /* Log buffer of a person       */
extern SListBase ResList;
extern aRoom     roomBuf;
extern MessageBuffer   msgBuf;
extern CONFIG    cfg;            /* Lots an lots of variables    */
extern char onConsole;                  /* Who's in control?!?          */
extern char whichIO;                    /* CONSOLE or MODEM             */
extern char anyEcho;
extern char echo;
extern FILE *upfd;
extern char modStat;
extern char echoChar;
extern char haveCarrier;
extern long netBytes;
extern char outFlag;
extern char *indexTable;
void *ChPhrase(DirEntry *e, DirEntry *d);
/************************************************************************/
/* Section 3.4. AREAS:                                                  */
/*   The model of Citadel includes a provision for "directory rooms."   */
/* A directory room is defined as the ability to look in on some section*/
/* of the host system's file section.  In order to avoid tying the      */
/* directory structure of Citadel to any particular operating system, an*/
/* "abstraction" has been implemented.  Each room that has been desig-  */
/* nated as a directory will have an "area" associated with it.  This   */
/* "area" is dependent on each implementation; access to "areas" is     */
/* through routines located in this module.  Therefore, the abstract    */
/* directory model of Citadel is in the main code modules, and should   */
/* not require any changes from port to port.  The only changes neces-  */
/* sary should be in this file (SYSDEP.C), where the porter must decide */
/* upon and implement a mapping of how a directory room peeks in on his */
/* or her file system.                                                  */
/*   Basically, the routines are fairly simple in purpose.              */
/*                                                                      */
/*      "And if pigs had wings they could fly!"                         */
/************************************************************************/
char        ourHomeSpace[150];
char MatchList[] = ".-_*?";
/************************************************************************/
/*      ValidDirFileName() validate a file name                         */
/************************************************************************/
char ValidDirFileName(char *ptr)
  {
  Do_Stack_Check();
  while (*ptr)
    {
    if (isalpha(*ptr) || isdigit(*ptr) || strchr(MatchList, *ptr))
    ++ptr;
    else
    return FALSE;

    }
  return TRUE;

  }
/************************************************************************/
/*      RoomLeft() how much room left on this disk or whatever?         */
/*      NB: Returns in K, not bytes!                                    */
/************************************************************************/
long RoomLeft(aRoom *room)
  {
  char *temp, dir[150];
  long temp3, temp2;
  long toReturn;
  /* this should never happen, but just in case ... */
  Do_Stack_Check();
  if ((temp = FindDirName(room->rbArea)) == NULL)  return 0;
  strCpy(dir, temp);
  diskSpaceLeft(dir, &temp3, &temp2);
  toReturn = temp2 / 1024l;
  return toReturn;

  }

/************************************************************************/
/*      getArea() get area to assign to a directory room from sysop     */
/*      returns FALSE if problems or abort                              */
/************************************************************************/
char getArea(aRoom *roomData)
  {
  char dir[200];
  SYS_FILE filename;
  extern char *DirFileName;
  extern SListBase DirBase;
  Do_Stack_Check();
  if (!goodArea("DIRNAM", dir))
    {
    return FALSE;

    }
  makeSysName(filename, DirFileName, &cfg.roomArea);
  upfd = safeopen(filename, WRITE_TEXT);
  AddData(&DirBase, NtoStrInit(roomData->rbArea, dir, 0, FALSE),
  WrtNtoStr, /* Kill duplicates */ TRUE);
  fclose(upfd);
  return TRUE;

  }
/************************************************************************/
/*      fileType() gets file type for MSDOS                             */
/************************************************************************/
fileType(char *theDir)
  {
  FILE *fd;
  Do_Stack_Check();
  if (strchr(theDir, '*') != NULL ||
  strchr(theDir, '?') != NULL)
    {
    return AMB_FILE;

    }
  if (realSetSpace(theDir))
    {
    homeSpace();
    return IS_DIR;

    }
  if ((fd = safeopen(theDir, READ_TEXT)) != NULL)
    {
    fclose(fd);
    return SINGLE_FILE;

    }
  return NO_IDEA;

  }
/************************************************************************/
/*      goodArea() Gets a valid path from the sysop. Drive should be    */
/*                  set already.                                        */
/************************************************************************/
goodArea(char *prompt, char *dir)
  {
  int  c;
  char dir_x[150];
  Do_Stack_Check();
  while (TRUE)
    {
    if (!getXString(prompt, dir_x, 149, "", ""))
    return FALSE;
    c = fileType(dir_x);
    switch (c)
      {
      case IS_DIR:
      strCpy(dir, dir_x);
      return TRUE;
      case NO_IDEA:
      if (strLen(dir_x) != 0)
        {
        Output_Citadel_Message("DNTEXS",(long)dir_x,NULL,NULL);
        if (getYesNo("CREATE"))
          {
          if (mkdir(dir_x) != 0)
            {
            Output_Citadel_Message("ERRDIR",(long)dir_x,NULL,NULL);
            homeSpace();

            }
          else
            {
            homeSpace();
            strCpy(dir, dir_x);
            return TRUE;

            }

          }

        }
      else
        {
        strCpy(dir, dir_x);
        return TRUE;

        }
      break;
      default:
        Output_Citadel_Message("ERRNDR",(long)dir_x,NULL,NULL);
      }

    }

  }
/************************************************************************/
/*      homeSpace() takes us home!                                      */
/************************************************************************/
void homeSpace()
  {
  Do_Stack_Check();
  realSetSpace(ourHomeSpace);
  }
/************************************************************************/
/*      netGetArea() Get area to store a file from networking           */
/************************************************************************/
netGetAreaV2(MenuId id,char *fn, struct fl_req *file_data, char ambiguous)
  {
  char goodname;
  Do_Stack_Check();
  if (!goodArea("DIRNSY", file_data->flArea.naDirname)) return FALSE;
  if (!ambiguous)
    {
    realSetSpace(file_data->flArea.naDirname);
    do
      {
      if (!getXString("FILNSY",file_data->filename, NAMESIZE, fn, fn))
        {
        homeSpace();
        return FALSE;

        }
      if (access(file_data->filename, 0) == 0)
        {
        Output_Citadel_Message("AREXTS",(long)file_data->filename,NULL,NULL);
        goodname = SysopGetYesNo(id,"ISTHIS","THEPRM");

        }
      else goodname = TRUE;

      }
    while (!goodname);
    homeSpace();

    }
  else
    {
    Output_Citadel_Message("AMBFRS",NULL,NULL,NULL);
    file_data->filename[0] = 0;
    return FALSE;
    }
  return TRUE;

  }
/************************************************************************/
/*      prtNetArea() human readable form of a NET_AREA                  */
/************************************************************************/
char *prtNetArea(NET_AREA *netArea)
  {
  Do_Stack_Check();
  return netArea->naDirname;

  }
/************************************************************************/
/*      dirString() a directory string                                                                          */
/************************************************************************/
void dirString(char *target, ROOM_AREA *area)
  {
  char *temp;
  Do_Stack_Check();
  if ((temp = FindDirName(*area)) != NULL)
  strCpy(target, temp);
  else strCpy(target, "ERROR");

  }
/************************************************************************/
/*      realSetSpace() does work of setSpace                            */
/************************************************************************/
char currentdir[150];
char realSetSpace(char *dir)
  {
  extern FILE *netLog;
  char result;
  Do_Stack_Check();
  if (cfg.BoolFlags.debug)
    {
    splitF(netLog,"Changing from %s\n",currentdir);
    splitF(netLog,"To:%s\n",dir);
    };
  strcpy(currentdir,dir);
  SpecialMessage(dir);
  result = (char )(chdir(dir)== 0 );
  return result;
  }
/************************************************************************/
/*      FindDirName() finds the directory associated with some room     */
/************************************************************************/
char *FindDirName(int roomNo)
  {
  Do_Stack_Check();
  return (char *)
  SearchList(&DirBase, NtoStrInit(roomNo, "", 0, TRUE));

  }
/************************************************************************/
/*      setSpace() moves us to an area                                  */
/************************************************************************/
char setSpace(aRoom *roomData)
  {
  char   *temp;
  extern SListBase DirBase;
  Do_Stack_Check();
  if ((temp = FindDirName(roomData->rbArea)) == NULL)
    {
    Output_Citadel_Message("ERRDRM",NULL,NULL,NULL);
    return FALSE;

    }
  if (!realSetSpace(temp))
    {
    Output_Citadel_Message("ERRDME",(long)temp,NULL,NULL);
    return FALSE;

    }
  return TRUE;

  }
#ifdef READY
/************************************************************************/
/*      writeArea(x) prints the area associated with this room          */
/************************************************************************/
void writeArea(char rightNow, aRoom *roomData, char *buf)
  {
  Do_Stack_Check();
  if (rightNow)
  mPrintf("%s\n ", FindDirName(roomData->rbArea));
  else
    {
    if (strLen(FindDirName(roomData->rbArea)) != 0)
    sPrintf(buf, "'%s'", FindDirName(roomData->rbArea));
    else
    sPrintf(buf, "'%c:'", locDisk);

    }

  }
#endif
/************************************************************************/
/*      sysGetSendFiles() where to find files to send to another system */
/************************************************************************/
char sysGetSendFilesV2(MenuId id,char *name, struct fl_send *sendWhat)
  {
  Do_Stack_Check();
  strCpy(sendWhat->snArea.naDirname, name);
  if (fileType(sendWhat->snArea.naDirname) == NO_IDEA)
    {
    if (SysopGetYesNo(id,NULL,"FILDIR"))
    return TRUE;
    return FALSE;

    }
  return TRUE;

  }
/************************************************************************/
/*      MoveFile() move a file to a new name and location               */
/************************************************************************/
void MoveFile(char *oldname, char *newname)
  {
  char buffer[100];
  FILE *infd, *outfd;
  int s;
  Do_Stack_Check();
  if (toUpper(*oldname) == toUpper(*newname))
    {
    rename(oldname, newname);
    if (access(newname, 0) == 0) return;

    }
  sPrintf(buffer, "copy >NIL: %s %s", oldname, newname);
  Jsystem(buffer);
  if (access(newname, 0) == 0)
    {
    unlink(oldname);

    }
  else
    {
    if ((infd = safeopen(oldname, READ_ANY)) == NULL) return;
    if ((outfd = safeopen(newname, WRITE_ANY)) == NULL)
      {
      fclose(infd);
      return;

      }
    while ((s = fread(buffer, sizeof buffer, 1, infd)) > 0)
    fwrite(buffer, s, 1, outfd);
    fclose(infd);
    fclose(outfd);
    unlink(oldname);

    }

  }
/************************************************************************/
/*      sysRoomLeft() how much room left in net recept area             */
/************************************************************************/
long sysRoomLeft()
  {
  long temp, temp2, temp3;
  Do_Stack_Check();
  temp = (long) ( (long) cfg.sizeArea * 1024);
  netBytes = 0l;
  doSendWork(ALL_FILES, getSize);
  diskSpaceLeft(NULL, &temp3, &temp2);
  return (minimum(temp - netBytes, temp2));

  }
/************************************************************************/
/*      sysSendFiles() system dep stuff for sending files               */
/************************************************************************/
void sysSendFiles(struct fl_send *sendWhat)
  {
  char   temp[100], *last;
  label  mask;
  Do_Stack_Check();
  strCpy(temp, sendWhat->snArea.naDirname);
  switch (fileType(sendWhat->snArea.naDirname))
    {
    case IS_DIR:
    strCpy(mask, "*");
    break;
    case SINGLE_FILE:
    case AMB_FILE:
    if ((last = strrchr(temp, '/')) == NULL &&
    (last = strrchr(temp, ':')) == NULL)
      {
      strCpy(mask, temp);
      temp[0] = 0;

      }
    else
      {
      strCpy(mask, last + 1);
      if (last != temp && *last != ':')
      *last = 0;
      else
      *(last + 1) = 0;

      }
    break;
    case NO_IDEA:
    default:
    sPrintf(temp, "Couldn't do anything with '%s'.",
    sendWhat->snArea.naDirname);
    netResult(temp);
    return;

    }
  if (!realSetSpace(temp))
    {
    sPrintf(msgBuf.mbtext, "Send file didn't know what to do with %s.",
    temp);
    netResult(msgBuf.mbtext);

    }
  else
  doSendWork(mask, netSendFile);
  homeSpace();

  }
/************************************************************************/
/*      doSendWork() does work of sysSendFiles()                        */
/************************************************************************/
void doSendWork(char *filename, void (*fn)(DirEntry *f))
  {
  Do_Stack_Check();
  wildCard(fn, filename, FALSE, "", FALSE);

  }
/************************************************************************/
/*      DoDomainDirectory() work on a domain directory                  */
/************************************************************************/
void DoDomainDirectory(int i, char kill)
  {
  char num[8];
  DOMAIN_FILE name;
  Do_Stack_Check();
  sPrintf(num, "%d", i);
  makeSysName(name, num, &cfg.domainArea);
  if (kill) rmdir(name);
  else      mkdir(name);

  }
/************************************************************************/
/* Section 3.5. BAUD HANDLER:                                           */
/*    The code in here has to discover what baud rate the caller is at. */
/* For some computers, this should be ridiculously easy.                */
/************************************************************************/
#define NO_GOOD         0
#define CR_CAUGHT       1
#define NET_CAUGHT      2
#define STROLL_CAUGHT   3
/************************************************************************/
/*      check_CR() Checks for CRs from the data port for half a second. */
/************************************************************************/
static short crcnt;
static short timer1,timer2;
char check_CR()
  {
  extern FILE *netLog;
  struct timePacket ff;
  int c=0;
  int  clcnt=0;
  char clast[8];  /* debug: last eight characters */
  memset(clast,0,8);
  Do_Stack_Check();
  setTimer(&ff);
  while (milliTimeSince(&ff) < timer1 )
    {
    if (MIReady())
      {
      if( c != 13 )crcnt = 0;
      clast[clcnt & 0x07] = c = inp();
      clcnt++;
      switch (c)
        {
        case 27: return CR_CAUGHT;break;  /* one escape, or two CRs */
        case 13:                          /* cr from user?, must have 2 */
        if( ++crcnt >1)
          {
          return CR_CAUGHT;
          };
        break;
        case 7:
        if (cfg.BoolFlags.netParticipant)
          {
          if (receive(1) == 13)
            {
            crcnt = 1;
            if (receive(1) == 69)
              {
              return NET_CAUGHT;
              };
            };
          };
        break;
        case 68:
        if (receive(1) == 79)
          {
          if (receive(1) == 35)
            {
            return STROLL_CAUGHT;
            };
          };
        break;
        default:
        if (clcnt > 0 && cfg.BoolFlags.debug)
          {
          splitF(netLog,"%x %x %x %x\n ",clast[0],clast[1],clast[2],clast[3]);
          splitF(netLog,"%x %x %x %x\n ",clast[4],clast[5],clast[6],clast[7]);
          };
        };
      };
    };
  return NO_GOOD;
  }
/************************************************************************/
/*      Find_Baud() Finds the baud from sysop and user supplied data.   */
/************************************************************************/
char Find_baud(char **whatRate)
  {
  char noGood = NO_GOOD;
  int  Time = 0;
  long  baudRunner;                    /* Only try for 60 seconds      */
  char flag;
  extern FILE *netLog;
  extern long byteRate;
  crcnt = 0;
  Do_Stack_Check();
  *whatRate = rates[9];  /* default to unknown */
  flag = FALSE;
  if ((baudRunner = (long)getModemId()) != ERROR)
    {
    CitadelBaudRate(baudRunner, "Find_baud");
    flag = TRUE;
    if (cfg.BoolFlags.debug) splitF(netLog, "getModemId = %d\n",baudRunner);
    };
  if (cfg.DepData.LockPort >= 0 && !flag)  /* if serial port locked, use it */
    {
    baudRunner = cfg.DepData.LockPort;
    flag = TRUE;
    if (cfg.BoolFlags.debug) splitF(netLog, "LockPort Forced = %d\n",baudRunner);
    };
  pause(10);                    /* To clear line noise and eat banner */
  while (MIReady())   inp();   /* eat noise and banners        */
  if( cfg.DepData.LockPort >= 0 || flag )
    {
    timer1 = 50;               /* time spent looking for User/Net session */
    timer2 =  4;               /* number of autobaud attempts */
    }
  else
    {
    timer1 = 75;                /* time spent looking for User/Net session */
    timer2 = (cfg.sysBaud+1)*3; /* autobaud attempts, try 3 attempts */
    baudRunner = 0;
    };
  while (gotCarrier() && noGood == NO_GOOD &&  (Time < timer2))
    {
    Time++;
    if( !flag )
      {
      CitadelBaudRate(baudRunner,"Find_baud");
      };
    noGood = check_CR();
    if (noGood == NO_GOOD && !flag)
      {
      if( baudRunner++ > cfg.sysBaud )baudRunner = 0;
      };
    };
  if( noGood == NO_GOOD && !flag )
    {
    baudRunner = cfg.sysBaud;
    if( !flag ) CitadelBaudRate(baudRunner, "Find_baud");
    };
  LastBaudIndex = baudRunner;
  *whatRate = rates[baudRunner];
  byteRate =  (atol(rates[baudRunner]) ) / 10L;
  if (cfg.BoolFlags.debug) splitF(netLog, "Time:%d Baud:%s\n",Time,*whatRate);
  if (noGood == NET_CAUGHT)
    {
    netController(0, 0, NO_NETS, ANY_CALL, 0);
    if (cfg.BoolFlags.debug) splitF(netLog, "Baud -%s-->Net_Caught\n",*whatRate);
    return FALSE;       /* pretend nothing happened */

    }
  if (noGood == STROLL_CAUGHT)
    {
    StrollIt();
    if (cfg.BoolFlags.debug) splitF(netLog, "Baud -%s-->STROLL_Caught\n",*whatRate);
    return FALSE;       /* pretend nothing happened */

    }
  if (cfg.BoolFlags.debug) splitF(netLog, "Baud-%s-->other\n",*whatRate);
  return (char)(noGood == CR_CAUGHT || flag); /* return TRUE if USER */

  }
/************************************************************************/
/*      getModemId() Try to read baud id from modem                     */
/************************************************************************/
#define BA_BUF_SIZE     80
  extern char results_debug;  /* turn on to debug modem results codes */
  extern char results_cid;    /* turn on to collect caller ID info if present */

int getModemId()
  {
  char c, buffer[BA_BUF_SIZE];         /* Hopefully, overkill */
  extern FILE *netLog;
  struct timePacket ff;
  int i;
  char save_all = FALSE;
  UNS_16 *j;
  extern char ResultCodesAvailable;
  Do_Stack_Check();
  if (!ResultCodesAvailable) return ERROR;
  setTimer(&ff);
  i = 0;
  while (timeSince(&ff) < 5l)
    {
    if (MIReady())
      {
      if ((c = inp()) == '\r')
        {
        buffer[i] = 0;
        if( i <= 1 )continue;
        if( buffer[0] == 'E' && buffer[1]== 07 )return ERROR;
        if( results_debug )splitF(NULL, "buffer search:-%s-\n",buffer);
        if( save_all )splitF(NULL, "CID:%s\n",buffer);
        j = (int *) SearchList(&ResList, buffer);
        if (j != NULL)
          {
          switch (*j)
            {
            case R_RING:
              if( results_cid )  /* toggle to collect info between rings */
                {
                save_all = TRUE;
                };
              break;
            case R_AOK:      break;
            case R_NOCARR:
            case R_BUSY:
            case R_DIAL:
            case R_NODIAL:   return ERROR;
            case R_300:
            case R_1200:
            case R_2400:
            case R_4800:
            case R_9600:
            case R_14400:
            case R_19200:
            case R_38400:
            case R_57600:
            return (int)*j;
            default:
              splitF(NULL, "Modem text not in RESULTS.SYS\n");
            break;

            };
          i = 0;

          }
        else i = 0;

        }
      else
        {
        if (c != '\n')
          {
          buffer[i++] = c;
          }
        if (i >= BA_BUF_SIZE - 4)
          {
          /* Fudge factor */
          i = 0;
          if( results_debug )splitF(NULL, "buffer flush:-%s-\n",buffer);

          }

        }

      }

    }
  buffer[i] = 0;      /* debug for now */
  splitF(NULL, "FAILURE to find in RESULTS.SYS:%s\n", buffer);
  return ERROR;

  }
/************************************************************************/
/*      getNetBaud()  gets baud of network caller -- refer to SysDep.doc*/
/************************************************************************/
char getNetBaud()
  {
  extern FILE *netLog;
  long   Time, baudRunner;
  extern long byteRate;
  char        found = FALSE, notFinished;
  extern char inNet;
  Do_Stack_Check();
  /* If anytime answer, then we already have baud rate */
  if (inNet == ANY_CALL || inNet == STROLL_CALL)
    {
    found      = TRUE;
    baudRunner = LastBaudIndex;

    }
  else if (GetFirst(&ResList))
    {
    if ((baudRunner = getModemId()) != ERROR)
      {
      found = TRUE;

      }

    }
  pause(10);         /* Pause a  second */
  if (found)
    {
    CitadelBaudRate(baudRunner, "getNetBaud");
    byteRate = atol(rates[baudRunner]) / 10L;
    for (Time = 0; gotCarrier() && Time < 25; Time++)
      {
      if (check_for_init(FALSE)) return TRUE;
      }
    if (gotCarrier())
      {
      outFlag = IMPERVIOUS;
      Output_Citadel_Message("LATERN",timeLeft(),NULL,NULL);

      }

    }
  else
    {
    while (MIReady())   inp();          /* Clear garbage        */
    for (Time = 0; gotCarrier() && Time < 25; Time++)
      {
      for (notFinished = TRUE, baudRunner = 0;
      gotCarrier() && notFinished;)
        {
        byteRate = atol(rates[baudRunner]) / 10L;
        CitadelBaudRate(baudRunner, "getNetBaud");
        if (check_for_init(FALSE)) return TRUE;  /* get connection */
        notFinished = !(baudRunner == cfg.sysBaud);
        baudRunner++;

        }

      }
    if (gotCarrier())
      {
      outFlag = IMPERVIOUS;
      for (baudRunner = cfg.sysBaud; baudRunner > -1; baudRunner--)
        {
        CitadelBaudRate(baudRunner, "getNetBaud");
        Output_Citadel_Message("LATERN",timeLeft(),NULL,NULL);
        }
      outFlag = OUTOK;

      }

    }
  if (!gotCarrier()) splitF(netLog, "GetNetBaud:Lost carrier\n");
  killConnection();
  return FALSE;

  }
/************************************************************************/
/* Section 3.3. CONSOLE HANDLING:                                       */
/*    These functions are responsible for handling console I/O.         */
/************************************************************************/
/************************************************************************/
/*      mputChar()                                                      */
/************************************************************************/

int ConPutChar(char myChar);

void mputChar(char c)
  {
  extern char ChatMode;
  Do_Stack_Check();
  if ((c == '\0') || (c == BELL && cfg.BoolFlags.noChat && !onConsole))
  return;
  if (!(whichIO == CONSOLE || onConsole) && !anyEcho)
  return;
  if (c != ESC &&       (echo == BOTH ||
  (whichIO == CONSOLE && (echo != NEITHER || echoChar))))
    {
    ConPutChar(c);

    }
  if (onConsole && !ChatMode && ConDelay > 0) pause(ConDelay);

  }
