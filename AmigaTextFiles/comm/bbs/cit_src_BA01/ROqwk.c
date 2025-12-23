/**
 AsgQWK.c
 QWIK off-line reader for Asgarde-86.  Modified for the Citadel 68K

 history
 93Sep15 GSM  Created.
 94OCT21 AFP  Modifications for Citadel-68K

 Contents
 QWKmain              Main entry to QWK routines.
 QWKUserConfig        QWK user configuration menu.
 ScanRooms            Room selection menu.
 UpdateLastRead
 CreateQWKPacket
 ExtractAndAdd
 OkToSend
 menus
 qwkglobalreplace
 qwkReadDate
 qwkReadTime
 RepReadDate
 RepReadTime
 ResetRooms
 ArchQWK
 CleanUpPacket
**/

#include "ctdl.h"

#define VRSN           "V0.3c68k"
int Zero;
/**
  external variable declarations in LOGEDIT.C
**/
/************************************************************************/
extern CONFIG cfg;
extern LogTable *logTab;          /* RAM index of pippuls */
extern logBuffer logBuf;          /* Pippul buffer */
extern aRoom roomBuf;
extern MessageBuffer msgBuf;     /* Message buffer */
extern MessageBuffer tempMess;   /* Message buffer */
extern NetBuffer netBuf;
extern rTable *roomTab;
extern NetTable *netTab;
extern FILE *logfl;               /* log file descriptor **/
extern FILE *roomfl;
extern FILE *netfl;
extern struct floor *FloorTab;
extern char NotForgotten;
extern char onConsole;
extern char haveCarrier;
extern char outFlag;              /* will be one of the above */
extern AN_UNSIGNED crtColumn;     /* where are we on screen now? */
char noStop;
extern long InChatTime;
extern char loggedIn;
extern int thisLog;
extern char *READ_ANY;
extern char FileTransStat;
extern FILE *upfd;
static char *MonthTab[] =
  {
  "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE",
  "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"

  };
SYS_FILE  tempname;
SYS_FILE  tempFile;
int       totconf,
totfnd,
tottoyou;
unsigned char qwkbuf[128];
long      currentsector;
FILE     *qwkindfd,
*qwkmsgfd,
*qwkctlfd,
*qwkpersfd;
struct qwkheader
  {
  unsigned char status;
  unsigned char number[7];
  unsigned char date[8];
  unsigned char time[5];
  unsigned char to[25];
  unsigned char from[25];
  unsigned char subject[25];
  unsigned char password[12];
  unsigned char ref_no[8];
  unsigned char size[6];
  unsigned char active;
  unsigned int conf;
  unsigned int logical_msgno;
  unsigned char nettag;

  };
/*
*
* * the important stuff from the header
*/
/*
* true header format, only fields I use
*/
struct qwkdefinition
  {
  char      YourOwn;
  char      ResetBBSMsg;
  char      NewFileScan;
  char      FileDate[8];
  char      Bulletins;
  int       MaxPacket;
  int       MaxRoom;
  char      UpProtocol;
  char      DownProtocol;
  char      Archiver;
  char      Hangafterupload;

  };
struct qwkdefinition qwkdef;
typedef struct
  {
  int       Selected;
  MSG_NUMBER OldLastMessageNo;
  MSG_NUMBER LastMessageNo;
  MSG_NUMBER CurrentLastMessageNo;
  int       AlternateConferenceNo;

  }
QwkRoom;
typedef struct
  {
  int       messagesfound;
  int       messagestoyou;

  }
UsedQwkRoom;
QwkRoom  *QwkRooms;               /** RAM index of rooms */
UsedQwkRoom *UsedQwkRooms;
#ifdef ANSI_PROTOTYPING
char      QWKmain(void);
void      QWKUserConfig(void);
void      ScanRooms(void);
void      UpdateLastRead(void);
void      CreateQWKPacket(void);
void      ExtractAndAdd(int room);
char      OkToSend(void);
void      menus(void);
void      qwkglobalreplace(char *buf, char *qwkold);
void      qwkReadDate(char *date, char *datestr);
void      qwkReadTime(char *time, char *timestr);
void      RepReadDate(char *date, char *datestr);
void      RepReadTime(char *time, char *timestr);
void      ResetRooms(void);
long      ArchQWK(void);
void      InputREPPacket(void);
void      ImportQWKPacket(void);
void      CleanUpPacket(void);
char      Process_Current_Message(SECTOR_ID  loc,MSG_NUMBER id);
void      Jsystem(char *);
float     FloatToMSB(float f);
float     MSBtoFloat(float f);
#endif
/**
  itoa(number,string,base);
  number - number to convert to the requested base
  string - destination string for number
  base   - conversion base
**/
void itoa(int number, char *string, int base);
void itoa(int number, char *string, int base)
  {
  sprintf(string,"%d",number);  /** cheap shortcut...*/
  }


/************************************************************************/
/*
* QWKmain() main controller.
*/
/************************************************************************/
char
QWKmain()
  {
  FILE     *qwklogfd;
  char      HasLog;
  char      tempStr[20];
  int       rover;
  if( !cfg.BoolFlags.QwkMail )
    {
    Output_Citadel_Message("QWKNSP", NULL, NULL, NULL);
    return GOOD_SELECT;
    };
  Output_Citadel_Message("QWKSUP",(LONG)VRSN, (LONG)COPYRIGHT, NULL);
  if (!loggedIn)
    {
    Output_Citadel_Message("QWKLOG",NULL, NULL, NULL);
    return GOOD_SELECT;
    };
  QwkRooms = (QwkRoom *) GetDynamic(MAXROOMS * (sizeof(*QwkRooms)));
  UsedQwkRooms = (UsedQwkRoom *) GetDynamic(MAXROOMS * (sizeof(*UsedQwkRooms)));
  HasLog = FALSE;
  sprintf(tempStr, "qwklog.%03d", thisLog);
  makeSysName(tempname, tempStr, &cfg.QwkFileArea);
  if ((qwklogfd = fopen(tempname, "rb")) != NULL)
    {
    if (fread(&qwkdef, sizeof qwkdef, 1, qwklogfd) == 1)
      {
      HasLog = TRUE;
      fread(QwkRooms, (sizeof(*QwkRooms)) * MAXROOMS, 1, qwklogfd);

      };
    fclose(qwklogfd);

    };
  if (HasLog == FALSE)
    {
    qwkdef.YourOwn = TRUE;
    qwkdef.NewFileScan = FALSE;
    strcpy(qwkdef.FileDate, "12/31/99");
    qwkdef.Bulletins = FALSE;
    qwkdef.MaxPacket = cfg.QwkMaxPacket;
    qwkdef.MaxRoom =   cfg.QwkMaxRooms;
    qwkdef.UpProtocol = 'X';            /* Xmodem is always the default */
    qwkdef.DownProtocol = 'X';
    qwkdef.Archiver = 1;
    qwkdef.Hangafterupload = FALSE;
    for (rover = 0; rover < MAXROOMS; rover++)
      {
      QwkRooms[rover].Selected = 0;
      QwkRooms[rover].OldLastMessageNo = 0l;
      QwkRooms[rover].LastMessageNo =
      logBuf.lbvisit[logBuf.lbgen[rover] & CALLMASK];
      QwkRooms[rover].CurrentLastMessageNo = 0l;
      QwkRooms[rover].AlternateConferenceNo = 0;

      };

    };
  menus();
  sprintf(tempStr, "qwklog.%03d", thisLog);
  makeSysName(tempname, tempStr, &cfg.QwkFileArea);
  if ((qwklogfd = fopen(tempname, "wb")) != NULL)
    {
    if (fwrite(&qwkdef, sizeof qwkdef, 1, qwklogfd) == 1)
      {
      fwrite(QwkRooms, (sizeof(*QwkRooms)) * MAXROOMS, 1, qwklogfd);
      };
    fclose(qwklogfd);
    }
  free(QwkRooms);
  free(UsedQwkRooms);
  getRoom(LOBBY);
  return GOOD_SELECT;

  }
/************************************************************************/
/*
* QWKUserConfig() various settings for users.
*/
/************************************************************************/
void
QWKUserConfig()
  {
  char      done = FALSE;
  char      letter,
  letter1;
  char *protocol;
  do
    {
    Output_Citadel_Message("QWKCFG",NULL, NULL, NULL);
    Output_Citadel_Message("QWKCR1"
      ,(long)(qwkdef.YourOwn ? "Yes" : "No")
      ,(long)(qwkdef.Hangafterupload ? "Yes" : "No"), NULL);
    Output_Citadel_Message("QWKCR2", qwkdef.MaxPacket, qwkdef.MaxRoom, NULL);
    switch ( qwkdef.DownProtocol )
      {
      case 'X': protocol = "XMODEM"; break;
      case 'Y': protocol = "YMODEM"; break;
      default:
       protocol = FindProtoName(qwkdef.DownProtocol);
       if( protocol == NULL )
         {
         protocol = "XMODEM";
         qwkdef.DownProtocol = 'X';
         };
      };
    Output_Citadel_Message("QWKCR3",(long)protocol
      ,(long)GetCompEnglish(qwkdef.Archiver), NULL);
    /**
    sessioncheck = 0;
      chkTimeSince(USER_SESSION) - InChatTime;
    if (sessionLimit > 0)
    mPrintf("You have been on for %lu minutes, and have %lu minutes left.\n ",
    (sessioncheck / 60), (((sessionLimit * 60) - sessioncheck) / 60));
    **/
    Output_Citadel_Message("QWKCR4", NULL, NULL, NULL);
    letter1 = toUpper(iChar());
    mPrintf("\n ");
    switch (letter1)
      {
      case 'C':      ScanRooms();      break;
      case 'R':      ResetRooms();     break;
      case 'S':      qwkdef.YourOwn = !qwkdef.YourOwn; break;
      case 'T':
      Output_Citadel_Message("QWKCR5",NULL, NULL, NULL);
      letter = toUpper(iChar());
      mPrintf("\n ");
      if ((qwkdef.DownProtocol = FindProtocolCode(letter, FALSE)) == -1)
        {
        if( letter != 'X' && letter != 'Y' )
          {
          mPrintf("invalid protocol, setting to Xmodem\n ");
          qwkdef.DownProtocol = 'X';
          }
        else qwkdef.DownProtocol = letter;
        }
      break;
      case 'P':      qwkdef.Archiver = GetUserCompression();  break;
      /*
      * case 'D':  break;
      */
      case 'M':
      qwkdef.MaxPacket = getNumber("QWKTMS", 0, cfg.QwkMaxPacket);
      qwkdef.MaxRoom   = getNumber("QWKMPC", 0, cfg.QwkMaxRooms);
      break;
      case 'G': qwkdef.Hangafterupload = !qwkdef.Hangafterupload;  break;
      /**
        case 'F':  mPrintf("Not used at this time\n "); break;
      **/
      case '?':
      case 'H':    Output_Citadel_Message("QCFHLP", NULL, NULL, NULL);    break;
      case 'Q':    done = TRUE;    break;
      default:
      done = TRUE;
      break;

      }
    if (!done)
    writeSysTab();

    }
  while (!done);

  }
/************************************************************************/
/*
* ScanRooms() Room selection setup routine.
*/
/************************************************************************/
#define ALL 0
#undef  SELECTED
#define SELECTED 1
void
ScanRooms()
  {
  int       rover,
  currentstart,
  i,
  newstart,
  done,
  innerdone,
  lpcnt;
  int       lines,
  pagelength,
  chosen;
  char      tempStr[10],
  scanflag,
  letter[5];
  newstart = 0;
  currentstart = 0;
  i = 0;
  /*  if (logBuf.lbscrnlngth == 0) */
  pagelength = 23;
  /*  else
  pagelength = logBuf.lbscrnlngth;
  */
  done = FALSE;
  scanflag = ALL;
  do
    {
    lines = 0;
    for (rover = currentstart; rover < MAXROOMS; rover++)
      {
      /*
      * deep breath ... should rewrite this, prime example of programming
      * via accretion.
      */
      if ((KnownRoom(rover) || (NotForgotten &&
      roomTab[rover].rtflags.INUSE &&
      (aide && (cfg.BoolFlags.aideSeeAll || onConsole) &&
      (!roomTab[rover].rtflags.INVITE || onConsole)))))
        {
        if (QwkRooms[rover].Selected)
        sprintf(tempStr, "(%7.7lu)", QwkRooms[rover].LastMessageNo);
        else
        memset(tempStr, NULL, 7);
        if ((scanflag == SELECTED && QwkRooms[rover].Selected) ||
        (scanflag == ALL))
          {
          mPrintf("%3d)%s%-20s %9s ", rover,(QwkRooms[rover].Selected ? "*" : " "),
          roomTab[rover].rtname, tempStr);
          i++;
          if (i == 2)
            {
            mPrintf("\n ");
            i = 0;
            lines = lines + 1;

            }
          if (lines == (pagelength - 5))
            {
            newstart = rover + 1;
            rover = MAXROOMS;

            }

          }

        }

      }
    innerdone = FALSE;
    do
      {
      /**
        mPrintf("You have been on for %d minutes, and have %d minutes left.\n ", ontime, maxtime);
      **/
      if (newstart < MAXROOMS)Output_Citadel_Message("QWKCC1",NULL, NULL, NULL);
      Output_Citadel_Message("QWKCCL",NULL, NULL, NULL);
      getString("", letter, 5, 0);
      if (strlen(letter) == 0)
      letter[0] = '?';
      for (lpcnt = 0; lpcnt < strlen(letter); lpcnt++)
      letter[lpcnt] = toUpper(letter[lpcnt]);
      mPrintf("\n ");
      switch (letter[0])
        {
        case 'T':
        currentstart = 0;
        innerdone = TRUE;
        break;
        case '+':
        currentstart = newstart;
        if (currentstart > MAXROOMS)
        currentstart = MAXROOMS - (pagelength - 5);
        innerdone = TRUE;
        break;
        case '-':
        currentstart = currentstart - (pagelength - 5);
        if (currentstart < 0)
        currentstart = 0;
        innerdone = TRUE;
        break;
        case 'L':        innerdone = TRUE;        break;
        case 'A':        scanflag = ALL;          break;
        case 'O':        scanflag = SELECTED;     break;
        case 'Q':  done = TRUE; innerdone = TRUE; break;
        case '*':
        case 'R':
        for (chosen = 0; chosen < MAXROOMS; chosen++)
          {
          if (KnownRoom(chosen) ||
          (NotForgotten && roomTab[chosen].rtflags.INUSE &&
          (aide && (cfg.BoolFlags.aideSeeAll || onConsole)) &&
          (!roomTab[chosen].rtflags.INVITE || onConsole)))
            {
            if (!QwkRooms[chosen].Selected || letter[0] == '*')
            QwkRooms[chosen].Selected = TRUE;
            else
            QwkRooms[chosen].Selected = FALSE;

            }

          }
        break;
        case '?':
          Output_Citadel_Message("QWKCLH",NULL, NULL, NULL);
        break;
        default:
        /*
        * chosen = getNumber("QWKRMN", 0, MAXROOMS);
        */
        chosen = (int) atoi(letter);
        if (KnownRoom(chosen) ||
        (NotForgotten && roomTab[chosen].rtflags.INUSE &&
        (aide && (cfg.BoolFlags.aideSeeAll || onConsole)) &&
        (!roomTab[rover].rtflags.INVITE || onConsole)))
          {
          if (QwkRooms[chosen].Selected)
          QwkRooms[chosen].Selected = FALSE;
          else
          QwkRooms[chosen].Selected = TRUE;

          }
        else
          {
          mPrintf("Room(conference) not known or not in use.\n ");

          }
        break;

        }

      }
    while (!innerdone);

    }
  while (!done);

  }
/**
 UpdateLastRead() Updates QWK message pointer structure.  Called
 only after a succesful download.
**/
void
UpdateLastRead()
  {
  int       rover;
  for (rover = 0; rover < MAXROOMS; rover++)
    {
    if (UsedQwkRooms[rover].messagesfound > 0)
      {
      QwkRooms[rover].OldLastMessageNo = QwkRooms[rover].LastMessageNo;
      QwkRooms[rover].LastMessageNo = QwkRooms[rover].CurrentLastMessageNo;
      QwkRooms[rover].CurrentLastMessageNo = 0l;

      }

    }

  }
/**
CreateQWKPacket()
Scans all rooms for new messages and extracts them IF the room is selected.
**/
void
CreateQWKPacket()
  {
  int       rover;
  int       year,  month,  day,  hours,  minutes,  seconds,  milli;
  int         success;
  char      tempstr[30],  tempstr2[30],  c, *tempc;
  char *protocol;
  long      packetSize;
  getRawDate(&year, &month, &day, &hours, &minutes,
  &seconds, &milli);
  makeSysName(tempname, "messages.dat", &cfg.QwkWorkArea);
  if ((qwkmsgfd = fopen(tempname, "wb")) == NULL)
    {
    crashout("Cannot Open messages data file!");

    }
  memset(qwkbuf, ' ', 128);
  sprintf(qwkbuf, "Produced by QMAIL...Copyright (c) 1987 by Sparkware. All Rights Reserved.");
  success = strlen(qwkbuf);
  qwkbuf[success] = ' ';
  fwrite(qwkbuf, 128, 1, qwkmsgfd);
  currentsector = 1;
  makeSysName(tempname, "personal.ndx", &cfg.QwkWorkArea);
  if ((qwkpersfd = fopen(tempname, "wb")) == NULL)
    {
    crashout("Cannot Open personal.ndx data file!");

    }
  makeSysName(tempname, "control.dat", &cfg.QwkWorkArea);
  if ((qwkctlfd = fopen(tempname, "wb")) == NULL)
    {
    crashout("Cannot Open control data file!");

    }
  fputs((cfg.codeBuf + cfg.nodeName), qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  if( ( tempc = strchr(cfg.QwkLocation.saDirname,'/')) )*tempc = '\0';
  fputs(cfg.QwkLocation.saDirname, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fputs((cfg.nodeId + cfg.codeBuf), qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fputs(cfg.SysopName, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  sprintf(tempstr, "0,%s", (cfg.codeBuf + cfg.nodeTitle));
  fputs(tempstr, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  sprintf(tempstr, "%02d-%02d-%04d,%02d:%02d:%02d", month, day, year,
  hours, minutes, seconds);
  fputs(tempstr, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  sprintf(tempstr, "%s", strupr(logBuf.lbname));
  fputs(tempstr, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fputs(" ", qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fputs("0", qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  mPrintf("\n Preparing mail packet...\n ");
  mPrintf("Press S to abort scan\n \n ");
  mPrintf("Total message limit: %d\n ", qwkdef.MaxPacket);
  mPrintf("Room(Conference) message limit: %d\n ", qwkdef.MaxRoom);
  mPrintf("                                          High    Last    Number      To\n ");
  mPrintf(" Number  Conference                    Message    Read     Found     You\n ");
  mPrintf("------------------------------------------------------------------------\n ");
  totfnd = 0;
  totconf = 0;
  outFlag = OUTOK;
  for (rover = 0; rover < MAXROOMS && onLine() && outFlag == OUTOK; rover++)
    {
    if (rover == MAILROOM)
      {
      getRoom(rover);
      fillMailRoom();   /* update room also  */

      }
    UsedQwkRooms[rover].messagesfound = 0;
    UsedQwkRooms[rover].messagestoyou = 0;
    if (QwkRooms[rover].Selected && roomTab[rover].rtflags.INUSE)
      {
      mPrintf("   %3d     %-20s        %7.7lu %7.7lu ", rover,
      roomTab[rover].rtname, roomTab[rover].rtlastMessage,
      QwkRooms[rover].LastMessageNo);
      ExtractAndAdd(rover);
      mPrintf("    %5d   %5d\n ", UsedQwkRooms[rover].messagesfound,
      UsedQwkRooms[rover].messagestoyou);
      tottoyou += UsedQwkRooms[rover].messagestoyou;
      mAbort();

      }

    }
  if (outFlag == OUTOK)
    {
    mPrintf("Total messages found: %d\n ", totfnd);
    }
  sprintf(tempstr, "%d", totfnd);
  fputs(tempstr, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  sprintf(tempstr, "%d", totconf - 1);
  fputs(tempstr, qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  for (rover = 0; rover < MAXROOMS; rover++)
    {
    if (UsedQwkRooms[rover].messagesfound != 0)
      {
      sprintf(tempstr, "%d", rover);
      fputs(tempstr, qwkctlfd);
      fputc('\r', qwkctlfd);
      fputc(NEWLINE, qwkctlfd);
      fputs(roomTab[rover].rtname, qwkctlfd);
      fputc('\r', qwkctlfd);
      fputc(NEWLINE, qwkctlfd);

      }

    }
  fputs("BULLETIN.QWK", qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fputs("NEWS.QWK", qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fputs("GOODBYE.QWK", qwkctlfd);
  fputc('\r', qwkctlfd);
  fputc(NEWLINE, qwkctlfd);
  fclose(qwkctlfd);
  fclose(qwkmsgfd);
  fclose(qwkpersfd);
  if (outFlag == OUTOK && totfnd > 0)
    {
    mPrintf("Would you like to receive this packet,\n ");
    mPrintf("[Y]es, [N]o, [G]oodbye when done? ");
    c = toUpper(iChar());
    mPrintf("\n ");
    switch (c)
      {
      case 'Y':
      case 'G':
      mPrintf("Packing QWK packet with %s\n ", GetCompEnglish(qwkdef.Archiver));
      packetSize = ArchQWK();
      mPrintf("Packet size: %lu bytes\n ", packetSize);
      sprintf(tempstr, "%s.qwk", cfg.QwkName.saDirname);
      if( !onConsole )
        {
       if( qwkdef.DownProtocol == 'X' )
         {
         protocol = "XMODEM";
         }
       else if( qwkdef.DownProtocol == 'Y' )
         {
         protocol = "YMODEM";
         }
       else
         {
         protocol = FindProtoName(qwkdef.DownProtocol);
         if( protocol == NULL )
           {
           protocol = "XMODEM";
           qwkdef.DownProtocol = 'X';
           };
         };
        mPrintf("Start your %s download now\n ",protocol);
        };
      strcpy(tempstr2, cfg.QwkWorkArea.saDirname);
      success = strlen(tempstr2);
      tempstr2[success - 1] = NULL;
      if( cfg.BoolFlags.debug )  printf("%s\n", tempstr2);
      if (chdir(tempstr2) != 0) printf("chdir failed!\n");
      if( cfg.BoolFlags.debug )  printf("%s\n", getcwd(NULL, 100));;
      if( onConsole )
        {
        char buf[128];
        char tmp[40];
        getString("GTQWKD",tmp,40,0);
        sPrintf(buf,"copy >NIL: %s %s",tempstr,tmp);
        Jsystem(buf);
        FileTransStat = FL_SUCCESS;
        }
      else
        {
        FileTransStat = FL_START;
        TranFiles(qwkdef.DownProtocol,  FALSE);
        };
      if (c == 'G')  HangUp(TRUE);
      if( cfg.BoolFlags.debug ) printf("%d\n", FileTransStat);
      if (FileTransStat == FL_SUCCESS)  UpdateLastRead();
      break;

      }

    }
  outFlag = OUTOK;
  CleanUpPacket();
  homeSpace();

  }
/**
 ExtractAndAdd() Reads the selected room for new messages and


 places them in the QWK packet in proper format.
**/
#define LOADIT !NORMAL
void
ExtractAndAdd(int room)
  {
  float record_number;
  struct qwkheader qwkhead;
  int    length,  chrcnt,  blocks,  tempcheck,  tc;
  SYS_FILE  tempname;
  char     *mp;
  char      nwlcp = (signed char)227;
  int       i,count,  h,  j,  start,  finish;
  MSG_NUMBER lowLim,  highLim,  msgNo,  MsgRead;
  char        tempstr[50], conf, *fix1, *fix2;
  getRoom(room);
  if (room == MAILROOM)
  fillMailRoom();       /*
  * update room also
  */
  start = 0;
  finish = (room == MAILROOM) ? MAILSLOTS : MSGSPERRM;
  lowLim = QwkRooms[room].LastMessageNo + 1l;
  highLim = cfg.newest;
  if (cfg.oldest > lowLim)
    {
    lowLim = cfg.oldest;

    }
  MsgRead = lowLim;
  sprintf(tempstr, "%03d.ndx", room);
  makeSysName(tempname, tempstr, &cfg.QwkWorkArea);
  if ((qwkindfd = fopen(tempname, "wb")) == NULL)
    {
    sprintf(tempstr, "Cannot create index file for conference %d", room);
    crashout(tempstr);

    }
  for (i = start; i != finish && (onLine()) &&
  totfnd < qwkdef.MaxPacket && outFlag == OUTOK &&
  UsedQwkRooms[room].messagesfound < qwkdef.MaxRoom; i++)
    {
    msgNo = (roomBuf.msg[i].rbmsgNo & S_MSG_MASK);
    /**
    * Now check to see if msg is in "to be read" range, OR if we are
    * reading New AND the message is marked as SKIPPED (only happens in
    * Mail).  Note at the moment we're not going to worry about net mode --
    * we don't use this loop for sending Mail, although we do for other
    * rooms.
    */
    if (
    (msgNo >= lowLim && highLim >= msgNo)
    )
      {
      noStop = LOADIT;
      if ( Process_Current_Message(roomBuf.msg[i].rbmsgLoc, msgNo) &&
        OkToSend())
        {
        noStop = NORMAL;
        UsedQwkRooms[room].messagesfound++;
        if (msgNo > MsgRead)
        MsgRead = msgNo;
        totfnd++;
        memset(&qwkhead, ' ', 128);
        sprintf(tempstr, "/\n /%c/", 227);
        qwkglobalreplace(msgBuf.mbtext, tempstr);
        sprintf(tempstr, "/\n/%c/", ' ');
        qwkglobalreplace(msgBuf.mbtext, tempstr);
        length = strlen(msgBuf.mbtext);
        length = (length / 79) + length;
        blocks = (length / 128) + 2;
        qwkhead.status = ' ';
        itoa(totfnd, qwkhead.number, 10);
        tempcheck = strlen(qwkhead.number);
        qwkhead.number[tempcheck] = ' ';
        qwkReadDate(msgBuf.mbdate, qwkhead.date);
        tempcheck = strlen(qwkhead.date);
        qwkhead.date[tempcheck] = ' ';
        qwkReadTime(msgBuf.mbtime, qwkhead.time);
        tempcheck = strlen(qwkhead.time);
        qwkhead.time[tempcheck] = ' ';
        if (!msgBuf.mbto[0])
        strcpy(qwkhead.to, "ALL");
        else
        strncpy(qwkhead.to, strupr(msgBuf.mbto), 25);
        tempcheck = strlen(qwkhead.to);
        qwkhead.to[tempcheck] = ' ';
        if (strcmpi(msgBuf.mbto, logBuf.lbname) == SAMESTRING)
        UsedQwkRooms[room].messagestoyou++;
        if (!roomBuf.rbflags.ANON)
          {
          strncpy(qwkhead.from, strupr(msgBuf.mbauth),24);
          qwkhead.from[24] = '\0';     /* prevent long name problems */
          tempcheck = strlen(qwkhead.from);
          qwkhead.from[tempcheck] = ' ';

          }
 /**
        strncpy(qwkhead.subject, msgBuf.mbSubj, 25);
 **/
        strcpy(qwkhead.subject, roomBuf.rbname);
        qwkhead.subject[20] = '\0';
        tempcheck = strlen(qwkhead.subject);
        qwkhead.subject[tempcheck] = ' ';
        memset(qwkhead.password, ' ', 12);
        itoa(UsedQwkRooms[room].messagesfound, qwkhead.ref_no, 10);
        tempcheck = strlen(qwkhead.ref_no);
        qwkhead.ref_no[tempcheck] = ' ';
        itoa(blocks, qwkhead.size, 10);
        tempcheck = strlen(qwkhead.size);
        qwkhead.size[tempcheck] = ' ';
        qwkhead.active = 225;
        fix1 = (char *)&room;
        fix2 = (char *)&qwkhead.conf;
        fix2[0] = fix1[1];
        fix2[1] = fix1[0];
        fix1 = (char *)&totfnd;
        fix2 = (char *)&qwkhead.logical_msgno;
        fix2[0] = fix1[1];
        fix2[1] = fix1[0];
        qwkhead.nettag = ' ';
        currentsector++;
        fwrite(&qwkhead, 128, 1, qwkmsgfd);
        record_number = (float) currentsector;
        record_number = FloatToMSB(record_number);
        fix1 = (char *)&record_number;
        for(count=3; count >= 0; count--)fputc(fix1[count],qwkindfd);
        conf = (char) ( room & 0xFF );
        fputc(conf,  qwkindfd);
        if( msgBuf.mbto[0] != '\0')
          {
          if (strcmpi(msgBuf.mbto, logBuf.lbname) == SAMESTRING)
            {
            fix1 = (char *)&record_number;
            for(count=3; count >= 0; count--)fputc(fix1[count],qwkpersfd);
            conf = (char) ( room & 0xFF );
            fputc(conf,  qwkpersfd);

            };
          };
        mp = msgBuf.mbtext;
        chrcnt = 0;
        for (h = 1; h < blocks; h++)
          {
          memset(qwkbuf, ' ', 128);
          for (j = 0; j < 128; j++)
            {
            if (*mp != NULL)
              {
              if (*mp == nwlcp)
              chrcnt = 0;
              if (j < 128)
                {
                qwkbuf[j] = *mp;
                chrcnt++;
                mp++;

                }
              if (chrcnt == 79)
                {
                tc = j;
                do
                  {
                  if (tc >= 0 && qwkbuf[tc] == ' ')
                    {
                    qwkbuf[tc] = 227;
                    chrcnt = 0;

                    }
                  else
                    {
                    chrcnt--;
                    tc--;

                    }
                  if (chrcnt == 45)
                    {
                    j++;
                    qwkbuf[j] = 227;
                    chrcnt = 0;

                    }

                  }
                while (chrcnt > 44);

                }

              }
            else
              {
              qwkbuf[j] = 227;
              break;

              }

            }
          fwrite(qwkbuf, 128, 1, qwkmsgfd);
          currentsector++;

          }

        }

      }

    }
  fclose(qwkindfd);
  noStop = NORMAL;
  if (UsedQwkRooms[room].messagesfound == 0)
    {
    sprintf(tempstr, "%03d.ndx", room);
    makeSysName(tempname, tempstr, &cfg.QwkWorkArea);
    unlink(tempname);

    }
  else
    {
    totconf++;
    QwkRooms[room].CurrentLastMessageNo = MsgRead;

    }

  }
/**
OkToSend() Checks the current message to make sure it is a
selected message to be sent.
**/
char
OkToSend()
  {
  /*
  * if ((strcmpi(msgBuf.mbauth, logBuf.lbname)==SAMESTRING) &&
  * qwkdef.YourOwn)
 return TRUE;
  */
  if ((strcmpi(msgBuf.mbauth, logBuf.lbname) == SAMESTRING) &&
  !qwkdef.YourOwn)
  return FALSE;
  return TRUE;

  }
/**
 menus() Main menu routine.
**/
void
menus()
  {
  char      letter,
  done = FALSE;
  do
    {
    outFlag = OUTOK;
    Output_Citadel_Message("QWKMNM", NULL, NULL, NULL);
    letter = toUpper(iChar());
    mPrintf("\n ");
    switch (letter)
      {
      case 'C':      QWKUserConfig();                  break;
      case 'D':      CreateQWKPacket();                break;
      case 'H':      Output_Citadel_Message("QWKHLP", NULL, NULL, NULL); break;
      case 'G':
      if (getYesNo("CONFRM") )
        {
        terminate( /* hangUp == */ TRUE, TRUE);
        return;
        };
      case 'Q':      done = TRUE;                      break;
      default:       done = TRUE;                      break;

      }
    if (!done) writeSysTab();

    }
  while (!done);

  }
/**
 globalreplace()
 Replace the first delimited string with the second delimited
 string.  An optional repeat count precedes the 1st delimiter.
 If a repeat count isn't given, it is assumed to be MAXTEXT+1.
 The delimiter may be any character except a numeric character.
**/
void
qwkglobalreplace(char *buf, char *qwkold)
  {
  #define STRINGSIZE  (4*SECTSIZE)
  char       *p,
  *bufend,
  delim,
  *old,
  *new;
  int       i,
  cnt,
  oldlen,
  newlen,
  diff;
  if (!*qwkold)
  return;       /* Quit if no string  */
  cnt = (int) atoi(qwkold);
  if (!cnt)
  cnt = strlen(buf);
  p = qwkold;
  while (isdigit(*p))   /* Skip over the count if any  */
  p++;
  delim = *p;
  new = strchr(old = ++p, delim) + 1;
  p = strchr(new, delim);
  new[-1] =     /* Terminate old string  */
  *p = 0;       /* Terminate new string  */
  newlen = strlen(new);
  oldlen = strlen(old);
  diff = newlen - oldlen;
  if (!*old)return;  /*  keep dolts from entering a  */
                     /*   NULL as the search string.  */
  for (p = buf, i = cnt; bufend = buf + strlen(buf), p < bufend && i &&
  onLine() && !outFlag; p += newlen, i--)
    {
    p = matchString(p, old, bufend);
    if (!p)
    break;
    if (newlen > oldlen && diff >= (MAXTEXT - strlen(buf) - 1))
      {
      /*
      * mPrintf(" Buffer Full\n ");
      */
      break;

      }
    crtColumn = 1;
    /*
    * mPrintf("%4d\b\b\b\b", cnt-i);
    */
  /*  replace(p, new, oldlen, newlen); */

    }
  outFlag = OUTOK;
  /*
  * mPrintf(" Replaced %d", cnt -i);
 doCR();
  */

  }
/**
 qwkReadDate()
 This function interprets the citadel date string and returns a
 QWK formatted date string.
**/
void
qwkReadDate(char *date, char *datestr)
  {
  int       rover,
  found;
  int       year=90,
  month = 1,
  day = 1;
  label     mon;
  if( date[0] )
    {
    if (!isdigit(date[0]))
      {
      year = 90;
      month = 1;
      day = 1;

      }
    else
      {
      year = atoi(date);
      while (isdigit(*date))
      date++;
      for (rover = 0; isalpha(*date); date++, rover++)
      mon[rover] = toUpper(*date);
      mon[rover] = 0;
      if (rover == 0)
        {
        year = 90;
        month = 1;
        day = 1;

        }
      else
        {
        for (found = rover = 0; rover < NumElems(MonthTab); rover++)
        if (strncmp(mon, MonthTab[rover], strLen(mon)) == SAMESTRING)
          {
          found++;
          month = rover + 1;

          }
        if (found != 1)                     month = 1;
        if ((day = atoi(date)) == 0)        day = 1;

        }

      }

    }
  sprintf(datestr, "%02d-%02d-%02d", month, day, year);
  return;

  }
/**

qwkReadTime()

This function interprets the citadel time string and returns a
QWK formatted time string.

**/
void
qwkReadTime(char *time, char *timestr)
  {
  int   hours,  minutes;
  if (!time[0])
    {
    hours = 00;
    minutes = 00;

    }
  else
    {
    if (!isdigit(time[0]))
      {
      hours = 00;
      minutes = 00;

      }
    else
      {
      hours = atoi(time);
      while (isdigit(*time))
      time++;
      time++;
      minutes = atoi(time);
      while (isdigit(*time))
      time++;
      if (strchr(time, 'P') || strchr(time, 'p'))
      hours += 12;

      }

    }
  sprintf(timestr, "%02d:%02d", hours, minutes);
  return;

  }
/**

RepReadDate()

This function interprets the QWK REP date string and returns a
Citadel formatted date string.

**/
void
RepReadDate(char *date, char *datestr)
  {
  char     *monthTab[13] =
    {
    "", "Jan", "Feb", "Mar",
    "Apr", "May", "Jun",
    "Jul", "Aug", "Sep",
    "Oct", "Nov", "Dec"

    };
  int       year,  month,  day;
  if (!isdigit(date[0]))
    {
    year = 0;
    month = 0;
    day = 0;
    }
  else
    {
    month = atoi(date);
    while (isdigit(*date))      date++;
    date++;
    day = atoi(date);
    while (isdigit(*date))      date++;
    date++;
    year = atoi(date);
    };
  if (year != 0 && month != 0 && day != 0)
    sprintf(datestr, "%02d%3s%02d", year, monthTab[month - 1], day);
  else
    datestr[0] = NULL;
  return;
  }

/**

RepReadTime()

This function interprets the QWK REP time string and returns a
Citadel formatted time string.

**/
void
RepReadTime(char *time, char *timestr)
  {
  int     hours,  minutes;
  if (!time[0])
    {
    hours = 00;
    minutes = 00;

    }
  else
    {
    if (!isdigit(time[0]))
      {
      hours = 00;
      minutes = 00;

      }
    else
      {
      hours = atoi(time);
      while (isdigit(*time))
      time++;
      time++;
      minutes = atoi(time);
      while (isdigit(*time))
      time++;

      }

    }
  if (hours != 0 && minutes != 0)
  sprintf(timestr, "%02d:%02d %s", (hours > 12) ? hours - 12 : hours,
  minutes, (hours > 12) ? "pm" : "am");
  else
  timestr[0] = NULL;
  return;

  }
/**

ResetRooms()

This function is a menu system for selecting and reseting message
pointers for QWK packets.
**/

void
ResetRooms()
  {
  char      done = FALSE;
  char      letter1;
  int       rover;
  long      offset;
  outFlag = OUTOK;
  do
    {
    Output_Citadel_Message("QWKRSM", NULL, NULL, NULL);
    letter1 = toUpper(iChar());
    mPrintf("\n ");
    switch (letter1)
      {
      case '0':
      for (rover = 0; rover < MAXROOMS; rover++)
        {
        QwkRooms[rover].LastMessageNo = 0l;

        }
      break;
      case '1':
      for (rover = 0; rover < MAXROOMS; rover++)
        {
        QwkRooms[rover].LastMessageNo = cfg.newest;

        }
      break;
      case '2':
      for (rover = 0; rover < MAXROOMS; rover++)
        {
        QwkRooms[rover].LastMessageNo =
        QwkRooms[rover].OldLastMessageNo;

        }
      break;
      case '3':
      for (rover = 0; rover < MAXROOMS; rover++)
        {
        QwkRooms[rover].LastMessageNo =
        logBuf.lbvisit[logBuf.lbgen[rover] & CALLMASK];

        }
      break;
      case '4':
      offset = getNumber("QWKNML", 1, cfg.MsgsPerrm);
      for (rover = 0; rover < MAXROOMS; rover++)
        {
        QwkRooms[rover].LastMessageNo =
        roomTab[rover].rtlastMessage - offset;

        }
      break;
      case '5':
      mPrintf("not active at this time\n ");
      break;
      case 'Q':
      done = TRUE;
      break;
      case 'H':
      default:
      Output_Citadel_Message("QWKRSH",NULL, NULL, NULL);
      break;

      }

    }
  while (!done);
  return;

  }
/**

ArchQWK()

This function will archive all files from the QWKworkarea using
whatever archiving method the BBS supports and the user has
selected.  It will then return the size of the resulting file.

**/
long
ArchQWK()
  {
  FILE     *fbuf;
  char      tempStr[20],
  tempstr[30];
  long      fileSize = 0l;
  sprintf(tempStr, "%s.qwk", cfg.QwkName.saDirname);
  makeSysName(tempname, tempStr, &cfg.QwkWorkArea);
  strcpy(tempstr, "*.*");
  makeSysName(tempFile, tempstr, &cfg.QwkWorkArea);
  Compress(qwkdef.Archiver, tempFile, tempname);
  if ((fbuf = safeopen(tempname, READ_ANY)) != NULL)
    {
    totalBytes(&fileSize, fbuf);
    fclose(fbuf);

    }
  /*
  * printf("ArchQWK C1: %u, %s\n", fileSize, tempname); iChar();
  */
  return fileSize;

  }
/**
 CleanUpPacket()
This function deletes all files created in the QWKworkarea after
a packet is processed, good or bad.  If the USER is at the console
then it will move the files to a destination instead.
**/
void
CleanUpPacket()
  {
  char      tempstr[30],
  tempstr2[100];
  int       rover;
  strcpy(tempstr2, (char *)&cfg.QwkWorkArea.saDirname);
  rover = strlen(tempstr2);
  tempstr2[rover - 1] = NULL;
  /**
    printf("%s\n", tempstr2);
  **/
  if (chdir(tempstr2) != 0) printf("chdir failed!\n");
  /**
    printf("%s\n", getcwd(NULL, 100));
   iChar();
  **/
  getcwd(tempstr2, 100);
  printf("Current Directory: %s\n ", tempstr2);
  for (rover = 0; rover < MAXROOMS; rover++)
    {
    if (UsedQwkRooms[rover].messagesfound != 0)
      {
      sprintf(tempstr, "%03d.ndx", rover);
      unlink(tempstr);

      }

    }
  strcpy(tempstr, "personal.ndx");
  unlink(tempstr);
  sprintf(tempstr, "messages.dat");
  unlink(tempstr);
  sprintf(tempstr, "control.dat");
  unlink(tempstr);
  sprintf(tempstr, "%s.qwk", cfg.QwkName.saDirname);
  printf("removing used packet:%s\n",tempstr);
  unlink(tempstr);

  }
/*
*
* This gets all set up to do something with a message.  We use this rather
* than the findMessage in libmsg.c so we can automatically read in the 'M'
* field.
*/
extern struct mBuf mFile1;
extern FILE        *msgfl;


char Process_Current_Message(loc, id)
SECTOR_ID  loc;         /* sector in message.buf */
MSG_NUMBER id;          /* unique-for-some-time ID# */
  {
  MSG_NUMBER here;
  startAt(msgfl, &mFile1, loc, 0);
  do
    {
    getMessage(getMsgChar, FALSE, TRUE, TRUE);
    here = atol(msgBuf.mbId);

    }
  while (here != id &&  mFile1.thisSector == loc);
  return (char) ((here == id));

  }

union Conv_Union
{
    unsigned char       uc[4];
    unsigned short int  ui[2];
    float               f[1];
};
/**
  MSBIN.C - MSBIN-IEEE/IEEE-MSBIN conversion routines.
**/

/**
  FloatToMSB() - Convert a IEEE floating point number to MSBIN format.

  Arguments : (float f) Number to be converted.
  Returns   : (float) Same number in MSBIN format.
  Notes     : Routine courtsey of Jefferey Foy, Modified by J. Dawson
**/
float FloatToMSB(float f)
{
    union Conv_Union t;
    short int sign, exp;

    t.f[0] = f;

    sign = t.uc[0] / 0x80;
    exp  = ((t.ui[0] >> 7) - 0x7F + 0x81) & 0xFF;
    t.ui[0] = (t.ui[0] & 0x7F) | (sign << 7) | (exp << 8);

    return(t.f[0]);
}

/**
  MSBToFloat() - Convert a MSBIN into IEEE floating point.

  Arguments   : (float f) Number to be converted.
  Returns     : (float) Same number in IEEE floating point.
  Notes       : Routine courtsey of Jeffery Foy, Modified by Jim Dawson
**/

float MSBtoFloat(float f)
{
    union Conv_Union t;
    short int sign, exp;

    t.f[0] = f;

    sign = t.uc[1] / 0x80;
    exp  = (t.uc[0] - 0x81 + 0x7F) & 0xFF;

    t.ui[0] = (t.ui[0] & 0x7f) | (exp << 7)| (sign << 15);
    return(t.f[0]);
}
