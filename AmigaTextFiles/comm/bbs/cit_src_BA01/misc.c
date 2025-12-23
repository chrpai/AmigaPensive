/*
*       misc.c
*
* Random functions.
*/
/*
*       history
*
* 86Aug19 HAW  Kill history because of space problems.
* 84Jun10 JLS  Function changedate() installed.
* 84May01 HAW  Starting 1.50a upgrade.
* 83Mar12 CrT  from msg.c
* 83Mar03 CrT & SB   Various bug fixes...
* 83Feb27 CrT  Save private mail for sender as well as recipient.
* 83Feb23 Various.  transmitFile() won't drop first char on WC...
* 82Dec06 CrT  2.00 release.
* 82Nov05 CrT  Stream retrieval.  Handles messages longer than MAXTEXT.
* 82Nov04 CrT  Revised disk format implemented.
* 82Nov03 CrT  Individual history begun.  General cleanup.
*/
#include "ctdl.h"
#include <dos.h>
#define  TURBO_C_VSPRINTF_BUG
/*
*       contents
*
* ARCDir()    ARC TOC entries
* calcrc()    calculates CRC
* changeDate()      allow changing of date
* CheckDLimit()     exceeded download time limit?
* civTime()   MilTime to CivTime
* CompressedDir()   manager of reading compressed dirs
* configure()   sets terminal parameters via dialogue
* crashout()    crashes out of Citadel in case of bug
* doFormatted()   for wildCard
* doCR()      newline on modem and console
* download()    menu-level routine for WC-protocol sends
* formRoom()    room prompt formatting
* getCdate()    gets date from system clock.
* GetSecond()   get seconds of minute, for multibanner
* GifDir()    important data of a GIF file.
* HelpIfPresent()   print help file if present
* ingestFile()    puts file in held message buffer
* lbyte()     finds 0 byte of a string
* patchDebug()    display/patch byte
* printDate()   prints out date
* putBufChar()    .EWM/.EXM/.EWN/.EXN internal
* putFLChar()   readFile() -> disk file interface
* reconfigure()   Reconfigures a user
* TranFiles()   Handles file transfers to users
* TranSend()    Does send work of TranFiles()
* transmitFile()    send a host file, no formatting
* tutorial()    first level for printing a help file
* upLoad()    menu-level read-via-WC-protocol fn
* visible()   convert control chars to letters
* writeTutorial()   prints a .hlp file
* ZIPDir()    ZIP TOC entries
*/

static void Display_User_Configuration(void);


extern int ClassActive[EVENT_CLASS_COUNT]; /* which classes are active? */
char  *monthTab[13] =
  {
  "", "Jan", "Feb", "Mar",
  "Apr", "May", "Jun",
  "Jul", "Aug", "Sep",
  "Oct", "Nov", "Dec"

  };
FILE  *upfd;
int masterCount;
/*int acount;*/
long byteRate; /* Bytes/sec that modem is set for.     */
int DirAlign = 0;
long  LowFree;
char  AlignChar;
char  *NoFileStr = "\n No %s.\n";
char  *who_str = "who";
/* char   *VERSION = "3.42.s6"; */
char  *VERSION = VERSION_NAME;
char  *ALL_LOCALS  = "&L";
char  *R_SH_MARK   = "&&";
char  *LOC_NET     = "++";
char  *NON_LOC_NET = "%%";
char  *WRITE_LOCALS = "All Local Systems";
char  *APrivateRoom = "A Private Room";
char  *LCHeld = "log%d.hld";
char  FormatFlag = FALSE;
long  Dl_Limit = -1l;
long  *DL_Total;   /* Blech */
extern SListBase MailForward;
extern char FileTransStat;
PROTO_TABLE Table[] =
  {
    {
    "Ascii", 0, (IS_NUMEROUS | NEEDS_HDR), "ASCII", NULL, NULL, NULL,
    (int (*)(int))outMod, 1, AsciiHeader, NULL

    }
  ,
    {
    "Xmodem", 13, (RIGAMAROLE | IS_DL), "Xmodem", "Xmodem",
    "wcdown.blb", "wcupload.blb", sendWCChar, SECTSIZE, NULL,
    XYClear

    }
  ,
    {
    "Ymodem", 11, (IS_NUMEROUS | RIGAMAROLE | IS_DL | NEEDS_FIN | NEEDS_HDR),
    "Ymodem BATCH", "Ymodem SINGLE", "ymdown.blb", "ymodemup.blb",
    sendYMChar, YM_BLOCK_SIZE, YMHdr, XYClear

    }
  ,
  #ifdef WXMODEM_AVAILABLE
    {
    "Wxmodem", 13, (RIGAMAROLE | IS_DL), "WXModem",
    "WXModem", "wxdown.blb", "wxup.blb", sendWXModem, SECTSIZE,
    NULL, ClearWX

    }
  #else
    {
    NULL, 13, NOT_AVAILABLE, NULL, NULL, NULL, NULL, NULL, 0,
    NULL, NULL

    }
  #endif

  };
int fixVers = 603;
int majorVers = 114;
char *netVersion = "1.16";
extern CONFIG    cfg;   /* Lots an lots of variables    */
extern logBuffer   logBuf;    /* Person buffer    */
extern logBuffer   logTmp;    /* Person buffer    */
extern aRoom     roomBuf;   /* Room buffer    */
extern rTable    *roomTab;
extern MessageBuffer     msgBuf;
extern MessageBuffer     tempMess;
extern NetBuffer   netBuf;
extern int     outPut;
extern char    onConsole;
extern AN_UNSIGNED crtColumn; /* where are we on screen now?  */
extern char    loggedIn;  /* Is we logged in?     */
extern char    outFlag;    /* Output flag     */
extern char    haveCarrier;    /* Do we still got carrier?     */
extern char    heldMess;
extern int     TransProtocol;  /* transfer protocol in use     */
extern char    prevChar;  /* previous char output   */
extern char    textDownload;   /* flag     */
extern int     thisRoom;
extern int     thisLog;
extern char    whichIO;    /* Where I/O is     */
extern char    echo;     /* Should we echo? echo? echo?  */
extern FILE    *msgfl;
extern FILE    *roomfl;
extern FILE    *logfl;
extern int     exitValue;
extern char    *LCHeld, *WRITE_ANY, *WRITE_TEXT;
extern char    PrintBanner;
FunnyInfo Formats[] =
  {
    {   "LHA", TRUE,  LZHDir    },
    {   "ZIP", TRUE,  ZIPDir    },
    {   "ZOO", TRUE,  ZOODir    },
    {   "ARC", TRUE,  ARCDir    },
    {   "LZH", TRUE,  LZHDir    },
    {   "GIF", FALSE, GifDir    },
    {   "FRA", FALSE, GifDir    },
    {    NULL, FALSE, NULL    },
  };
/*
* CompressType()
*
* This function finds the type of file the specified file is.
*/
int CompressType(char *name)
  {
  int format;
  char *c;
  if ((c = strchr(name, '.')) != NULL)
    {
    for (format = 0; Formats[format].Format != NULL; format++)
      {
      if (strCmpU(c + 1, Formats[format].Format) == SAMESTRING)return format;

      };

    }
  return -1;

  }
/*
* AsciiHeader()
*
* This will entitle an ASCII file transfer.
*/
int AsciiHeader(long fileSize, char *filename)
  {
  char work[10];
  doCR();
  mPrintf("[ %s : %s bytes ]", filename, PrintPretty(fileSize, work));
  doCR();
  doCR();
  return TRUE;

  }
#define FN_LENGTH 90
/*
* CompressedDir()
*
* This function reads the TOC of compressed files using a table of function
* pointers (for generic use) and displays it.
*/
void CompressedDir(DirEntry *fn)
  {
  FILE  *fd;
  char  FileName[FN_LENGTH];
  char  DateStr[20];
  long  RealSize, SmallSize;
  int   count = 0;
  long  compressed = 0l, realsize = 0l;
  int   format;
  extern char *READ_ANY;
  extern int DirAlign;
  extern char AlignChar;
  if (outFlag != OUTOK) return;
  if ((format = CompressType(fn->unambig)) == ERROR) return;
  mPrintf("\n %s", fn->unambig);
  if (FindFileComment(fn->unambig))
    {
    DirAlign = strLen(fn->unambig) + 3;
    AlignChar = 0;
    mPrintf(":%s", strchr(msgBuf.mbtext, ' '));
    DirAlign = 0;

    }
  mPrintf("\n ");
  if ((fd = fopen(fn->unambig, READ_ANY)) == NULL)
    {
    mPrintf("INTERNAL FILE ERROR!\n ");
    return ;

    }
  if (Formats[format].Many)
    {
    mPrintf("\n%7s   %8s%6s Name \n ", "Crunched", "Normal  ", " Date ");
    while ((*Formats[format].Func)(fd, FileName, &RealSize, &SmallSize,
    DateStr))
      {
      count++;
      mPrintf("\n %7ld %8ld %6s %s", SmallSize, RealSize, DateStr,FileName);
      compressed += SmallSize;
      realsize   += RealSize;

      }
    mPrintf("\n  ------- --------");
    mPrintf("\n %7ld %8ld %d files\n ", compressed, realsize, count);

    }
  else
    {
    (*Formats[format].Func)(fd, TRUE, msgBuf.mbtext);
    mPrintf("%s", msgBuf.mbtext);

    }
  fclose(fd);

  }
/*
* ARCDir()
*
* This function reads an ARC TOC entry and sets for the next one.
*/
char ARCDir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr)
  {
  ARCbuf buf;
  #ifndef IS_MOTOROLA
  if (fread(&buf, sizeof buf, 1, fd) <= 0)
  return FALSE;
  #else
  /* this mess is due to Lattice C doing structure padding on Amigas */
  fread(&buf.ArchiveMark, 1, 1, fd);
  fread(&buf.Header, 1, 1, fd);
  fread(buf.name, 13, 1, fd);
  fread(&buf.size, 4, 1, fd);
  fread(&buf.date, 2, 1, fd);
  fread(&buf.time, 2, 1, fd);
  fread(&buf.crc, 2, 1, fd);
  fread(&buf.length, 4, 1, fd);
  #endif
  if (buf.ArchiveMark != 0x1a || buf.Header == 0)
  return FALSE;
  strCpy(FileName, buf.name);
  #ifdef IS_MOTOROLA
  Intel32ToMotorola(&buf.size);
  Intel32ToMotorola(&buf.length);
  Intel16ToMotorola(&buf.date);
  #endif
  *SSize = buf.size;
  *RSize = buf.length;
  DosToNormal(DateStr, buf.date);
  fseek(fd, buf.size, SEEK_CUR);
  return TRUE;

  }
/*
* ZIPDir()
*
* This function reads a ZIP TOC entry and sets for the next one.
*/
char ZIPDir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr)
  {
  ZipHeader ZBuf;
  if (fread(&ZBuf, sizeof ZBuf, 1, fd) < 1) return FALSE;
  #ifdef IS_MOTOROLA
  Intel32ToMotorola(&ZBuf.Signature);
  Intel32ToMotorola(&ZBuf.CompSize);
  Intel32ToMotorola(&ZBuf.NormalSize);
  Intel16ToMotorola(&ZBuf.NameLength);
  Intel16ToMotorola(&ZBuf.FieldLength);
  Intel16ToMotorola(&ZBuf.FileDate);
  #endif
  if (ZBuf.Signature != 0x04034b50) return FALSE;
  if (ZBuf.NameLength < 0 || ZBuf.NameLength > FN_LENGTH) return FALSE;
  fread(FileName, ZBuf.NameLength, 1, fd);
  FileName[ZBuf.NameLength] = 0;
  fseek(fd, ZBuf.FieldLength + ZBuf.CompSize, 1);
  *SSize = ZBuf.CompSize;
  *RSize = ZBuf.NormalSize;
  DosToNormal(DateStr, ZBuf.FileDate);
  return TRUE;
  }
  /*
  * GifDir()
  *
  * This reads the important data of a GIF file.
  */
  char GifDir(FILE *fd, char longexpl, char *sbuf)
    {
    GifHeader buf;
    fread(&buf, sizeof buf, 1, fd);
    #ifdef IS_MOTOROLA
    Intel16ToMotorola(&buf.Width);
    Intel16ToMotorola(&buf.Height);
    #endif
    sbuf[0] = 0;
    if (longexpl)
    sPrintf(sbuf, "File is %.6s, ", buf.Sig);
    sPrintf(lbyte(sbuf), (longexpl) ? "%d X %d, %d colors.\n " :
    "%3d X %3d X %2d", buf.Width, buf.Height,
    1 << ((buf.Colors & 0x07) + 1));
    return TRUE;

    }
  /*
  * ZOODir()
  *
  * This handles reading a Zoo entry.
  */
  char ZOODir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr)
    {
    zoo_header zh;
    zoo_direntry de;
    static char ZooStart = TRUE;
    if (ZooStart)
      {
      ZooStart = FALSE;
      fread(&zh, sizeof zh, 1, fd);
      #ifdef IS_MOTOROLA
      Intel32ToMotorola(&zh.zoo_start);
      #endif
      fseek(fd, zh.zoo_start, 0);

      }
    do
      {
      if (fread(&de, sizeof de, 1, fd) < 1)
        {
        ZooStart = TRUE;
        return FALSE;

        }
      #ifdef IS_MOTOROLA
      Intel32ToMotorola(&de.next);
      #endif
      fseek(fd, de.next, 0);

      }
    while (de.deleted);
    #ifdef IS_MOTOROLA
    Intel32ToMotorola(&de.size_now);
    Intel32ToMotorola(&de.org_size);
    Intel16ToMotorola(&de.date);
    #endif
    *SSize = de.size_now;
    *RSize = de.org_size;
    strcpy(FileName, de.fname);
    DosToNormal(DateStr, de.date);
    return TRUE;

    }
  /*
  * LZHDir()
  *
  * This function handles reading a lzh entry.
  *
  * courtesy Daniel Durbin.
  */
  char LZHDir(FILE *fd, char *FileName, long *RSize, long *SSize, char *DateStr)
    {
    LZHead header;
    #ifndef IS_MOTOROLA
    if (fread(&header, sizeof(header), 1, fd) < 1)
    return FALSE;
    #else
    if (fread(header.unknown1, sizeof header.unknown1, 1, fd) < 1)
    return FALSE;
    if (fread(header.method, sizeof header.method, 1, fd) < 1)
    return FALSE;
    if (fread(&header.csize, sizeof header.csize, 1, fd) < 1)
    return FALSE;
    if (fread(&header.fsize, sizeof header.fsize, 1, fd) < 1)
    return FALSE;
    if (fread(&header.ftime, sizeof header.ftime, 1, fd) < 1)
    return FALSE;
    if (fread(&header.fdate, sizeof header.fdate, 1, fd) < 1)
    return FALSE;
    if (fread(&header.fattr, sizeof header.fattr, 1, fd) < 1)
    return FALSE;
    if (fread(&header.unknown2, sizeof header.unknown2, 1, fd) < 1)
    return FALSE;
    if (fread(&header.namelen, sizeof header.namelen, 1, fd) < 1)
    return FALSE;
    #endif
    if (header.namelen < 1 || header.namelen >= FN_LENGTH)
    return FALSE;
    if (fread(FileName, 1, header.namelen, fd) != header.namelen)
    return FALSE;
    FileName[header.namelen] = 0;
    fgetc(fd), fgetc(fd); /* gets CRC I guess */
    /* I don't* know what this is jumping over - but it works! */
    if (header.unknown2 == 1)
      {
      fgetc(fd);
      fgetc(fd);
      fgetc(fd);

      }
    #ifdef IS_MOTOROLA
    Intel32ToMotorola(&header.fsize);
    Intel32ToMotorola(&header.csize);
    Intel16ToMotorola(&header.fdate);
    #endif
    *SSize = header.csize;
    *RSize = header.fsize;
    DosToNormal(DateStr, header.fdate);
    fseek(fd, header.csize, SEEK_CUR);
    return TRUE;

    }
  /*
  * DosToNormal()
  *
  * This function converts a DOS-formatted date to a formatted string.  This
  * perhaps should reside in the system dependent code....
  */
  void DosToNormal(char *DateStr, UNS_16 DosDate)
    {
    if (((DosDate & 0x1e0) >> 5) > 12 ||
    ((DosDate & 0x1e0) >> 5) < 1)
    strCpy(DateStr, "No Date");
    else
    sprintf(DateStr, "%d%s%02d", ((DosDate & 0xfe00) >> 9) + 80,
    monthTab[(DosDate & 0x1e0) >> 5], DosDate & 0x1f);

    }
  /*
  * calcrc()
  *
  * Calculates CRC for a given block.
  */
  CRC_TYPE calcrc(unsigned char *ptr, int count)
    {
    register CRC_TYPE checksum;
    register int i;
    checksum=0;
    while (count--)
      {
      i=(checksum >> 8) & 0xff;
      i ^= *ptr++;
      i ^= i >> 4;
      checksum <<= 8;
      checksum ^= i;
      i <<= 5;
      checksum ^= i;
      i <<= 7;
      checksum ^= i;

      }
    return(checksum);

    }
  #ifdef NEED_AVAILABLE
  /************************************************************************/
  /*  changedate() gets the date from the aide and remembers it */
  /************************************************************************/
  void changeDate()
    {
    int year, day, hours, minutes, mon;
    char *month;
    Output_Citadel_Message("CHNGDT",NULL,NULL,NULL);
    if (!getYesNo("ETDTMN"))
    return ;
    do
      {
      year    = (int) getNumber("YEARPR",  87l, 99l) + 1900;
      mon     = (int) getNumber("MONTPR", 1l,  12l)     ;
      day     = (int) getNumber("DAYPRM",   1l,  31l)     ;
      hours   = (int) getNumber("HOURPR",   0l, 23l)     ;
      minutes = (int) getNumber("MINTPR", 0l, 59l)     ;

      }
    while (!setRawDate(year, mon, day, hours, minutes));
    /*    InitEvents(); */

    }
  #endif
  /*
  * CheckDLimit()
  *
  * This checks to see if the next d/l will exceed the the limit or if it'll
  * interfere with a preemptive event.  It returns FALSE on interference,
  * TRUE otherwise.
  */
  char CheckDLimit(long estimated)
    {
    char *problem;
    extern char *DlMsgPtr;
    if (!aide && Dl_Limit_On() &&
    (*DL_Total) + estimated >= Dl_Limit * 60)
      {
      mPrintf("I'm sorry, that would exceed the current cumulative download time limit ");
      if (strLen(DlMsgPtr) != 0)
      mPrintf("of %s", DlMsgPtr);
      mPrintf(" -- you've currently spent %ld:%02ld in downloading.\n ",
      (*DL_Total) / 60l, (*DL_Total) % 60l);
      return FALSE;

      }
    if ((problem = ChkPreempt(estimated)) != NULL)
      {
      mPrintf("Sorry, that would interfere with %s.\n ", problem);
      return FALSE;

      }
    return TRUE;

    }
  /*
  * civTime()
  *
  * Military time to Civilian time.
  */
  void civTime(int *hours, char **which)
    {
    if (*hours >= 12)
    *which = "pm";
    else
    *which = "am";
    if (*hours >= 13)
    *hours -= 12;
    if (*hours == 0)
    *hours = 12;

    }
  /*
  * configure()
  *
  * This sets up the terminal width etc via dialogue.
  */
  char configure(logBuffer *lBuf, char AllQuestions, char AllowAbort)
    {
    int width, xwidth;    /* really! ugly kludge -- fix someday */
    lBuf->lbnulls   = 0;
    logBuf.lbdelay  = 0;
    width = termWidth;
    do
      {
      /* this gross width stuff is caused by that #define in ctdl.h */
      termWidth = width;
      lBuf->lbwidth   = (int) getNumber("COLSCR", 0l, 132l);
      if( !gotCarrier() )
        {
        haveCarrier      = FALSE;
        };
      xwidth = lBuf->lbwidth;
      if (onLine() && lBuf->lbwidth == 0 && AllowAbort)
        {
        termWidth = width;
        if (getYesNo("ABORTA")) return FALSE;

        }
      if (lBuf->lbwidth < 40)
        {
        termWidth = width;
        Output_Citadel_Message("WIDT40",NULL, NULL, NULL);
        }
      lBuf->lbwidth = xwidth;

      }
    while ( onLine() && lBuf->lbwidth < 40 );
    if (AllQuestions)
      {
      lBuf->lbflags.LFMASK = getYesNo("NEEDLF") ? TRUE : FALSE;

      }
    else
      {
      Output_Citadel_Message("XXXXXX",NULL,NULL,NULL);
      lBuf->lbflags.LFMASK = getYesNo("ISBLNK") ? FALSE : TRUE;

      }
    lBuf->lbflags.EXPERT      = getYesNo("AREEXP") ? TRUE : FALSE;
    if (lBuf->lbflags.EXPERT || AllQuestions)
      {
      lBuf->lbflags.TIME      = getYesNo("PRTTME") ? TRUE : FALSE;
      lBuf->lbflags.OLDTOO    = getYesNo("PRTOLD") ? TRUE : FALSE;
      lBuf->lbflags.FLOORS    = getYesNo("FLRMOD");
      lBuf->lbflags.ANSI      = getYesNo("ANSIHD");
      lBuf->lbflags.MSG_PAUSE = getYesNo("MSGPAS");
      }
    else
      {
      lBuf->lbflags.OLDTOO    = FALSE;
      lBuf->lbflags.TIME      = TRUE;
      lBuf->lbflags.FLOORS    = TRUE;
      lBuf->lbflags.HALF_DUP  = FALSE;
      lBuf->lbflags.ANSI      = FALSE;  /* No ANSI colors */
      lBuf->lbflags.MSG_PAUSE = FALSE;  /* no pause       */
      }
    return TRUE;

    }
  /*
  * crashout()
  *
  * Problems?  Out we go!!! This is a general error exit function.
  */
  void crashout(char *message)
    {
    FILE *fd;       /* Record some crash data */
    exitValue = CRASH_EXIT;
    outFlag = IMPERVIOUS;
    Output_Citadel_Message("REDALT",(long)message,NULL,NULL);
    HangUp(FALSE);
    logMessage(L_OUT, "", 0);
    logMessage(CRASH_OUT, "", 0);
    if( (fd = safeopen("crash", "w")) != NULL )
      {
      fprintf(fd, message);
      fclose(fd);
      };
    writeSysTab();
    ModemShutdown(TRUE);
    systemShutdown(0);
    exit(exitValue);

    }
  /*
  * doFormatted()
  *
  * This does a tutorial for a wildCard call.
  */
  void doFormatted(DirEntry *fn)
    {
    tutorial(fn->unambig, FALSE);

    }
  /*
  * doCR()
  *
  * This does a newline on modem and console.
  */
  void doCR()
    {
    int i;
    crtColumn   = 1;
    if (outFlag != OUTOK &&     /* output is being s(kip)ped    */
    outFlag != IMPERVIOUS)
    return;
    if (outPut == DISK) fprintf(upfd, "\n");
    else
      {
      if (TransProtocol == ASCII)
      mputChar(NEWLINE);
      if (haveCarrier)
        {
        (*Table[TransProtocol].method)('\r');
        if (TransProtocol == ASCII)
        for (i = termNulls;  i;  i--) outMod(0);
        if (termLF)
        (*Table[TransProtocol].method)('\n');

        }
      /* Kludge alert!  Kludge alert! */
      /* We don't have to check TransProtocol, though. */
      if (DirAlign != 0 && termWidth > 22)
        {
        #ifndef TURBO_C_VSPRINTF_BUG
        mPrintf("%*c%c ", DirAlign, ' ', AlignChar);
        #else
        /* SUPER YUCKY! */
        crtColumn += DirAlign + 1;
        for (i = 0; i < DirAlign; i++)
          {
          mputChar(' ');
          if (haveCarrier)
          (*Table[TransProtocol].method)(' ');

          }
        mputChar(AlignChar);
        mputChar(' ');
        if (haveCarrier)
          {
          (*Table[TransProtocol].method)(AlignChar);
          (*Table[TransProtocol].method)(' ');

          }
        #endif

        }

      }
    prevChar    = ' ';

    }
  /*
  * download()
  *
  * This is the is the menu-level send-message-via-protocol function.
  */
  void download(char whichMess, char revOrder, char protocol, char global,
  int Compression)
    {
    char result;
    int  count;
    extern char *APPEND_TEXT, Showing;
    char CompFile[30];
    outFlag     = OUTOK;
    if (Table[protocol].MsgTran != NULL)
       Output_Citadel_Message("DLFUNC",
                (long)(InternalProtocol(protocol) ? Table[protocol].MsgTran :
                FindProtoName(protocol)), NULL, NULL);
    if (Compression != NO_COMP)
       Output_Citadel_Message("DLCMPR",(long)GetCompEnglish(Compression), NULL, NULL);
    if (InternalProtocol(protocol) && !expert &&
    Table[protocol].BlbName != NULL)
    tutorial(Table[protocol].BlbName, TRUE);
    if (protocol != ASCII)
      {
      if (!getYesNo("RDYBEG"))  return;

      }
    if (!InternalProtocol(protocol) || Compression != NO_COMP)
      {
      result = TRAN_SUCCESS;
      ToTempArea();
      Output_Citadel_Message("DELAYM",NULL,NULL,NULL);
      echo = NEITHER;
      if (!redirect("msgs")) return;

      }
    else
      {
      if (protocol != ASCII) echo = NEITHER;
      result = Transmission(protocol, STARTUP);

      }
    if (protocol != ASCII || Compression != NO_COMP) Showing = DL_MSGS;
    if (result == TRAN_SUCCESS)
      {
      if (!global)
        {
        count = showMessages(whichMess, revOrder,
        logBuf.lbvisit[logBuf.lbgen[thisRoom] & CALLMASK],
        OptionValidate);
        if (count == 0) Output_Citadel_Message("NONEWM",NULL,NULL,NULL);

        }
      else
      doGlobal(whichMess, revOrder);
      if (!InternalProtocol(protocol) || Compression != NO_COMP)
        {
        undirect();
        if (Compression != NO_COMP)
          {
          sPrintf(CompFile, "msgs.%s", CompExtension(Compression));
          Compress(Compression, "msgs", CompFile);
          unlink("msgs"); /* because we use TranSend to send files */
          if (access(CompFile, 0) != 0)
            {
            mPrintf("Error: The compression failed.\n ");
            KillTempArea();
            return;

            }

          }
        else strCpy(CompFile, "msgs");
        TranSend(protocol, transmitFile, ALL_FILES, "", FALSE);
        unlink("msgs");
        unlink(CompFile);
        KillTempArea();

        }
      else Transmission(TransProtocol, FINISH);
      Showing = WHATEVER;

      }
    echo = BOTH;
    TransProtocol = ASCII;
    /*
    * If we have a console timeout during message display (during a Pause,
    * most likely), onLine() will not be true at this point.  But setUp()
    * will blindly set it to TRUE, so we have to call this with some care.
    */
    if (onLine())
    setUp(FALSE);

    }
  int StartingRoom, CurRoom;
  /*
  * doGlobal()
  *
  * Does .R{Y,W,X,other protocols}G
  */
  void doGlobal(char whichMess, char revOrder)
    {
    extern char PhraseUser;
    StartingRoom = CurRoom = thisRoom;
    while (
    ((whichMess == NEWoNLY && !PhraseUser) ? gotoRoom("", 'R') : NextSeq())
    && (gotCarrier() || onConsole))
      {
      givePrompt();
      mPrintf("read\n ");
      showMessages(whichMess, revOrder,
      logBuf.lbvisit[logBuf.lbgen[thisRoom] & CALLMASK],
      OptionValidate);
      doCR();   /* aesthetics, pig-dogs. */
      if (outFlag == OUTSKIP) break;

      }

    }
  /*
  * NextSeq()
  *
  * This finds next room in sequence for doGlobal().
  */
  int NextSeq()
    {
    int i;
    i = (CurRoom + 1) % MAXROOMS;
    while (i != StartingRoom)
      {
      if (roomTab[i].rtflags.INUSE &&
      KnownRoom(i) != UNKNOWN_ROOM)
        {
        getRoom(i);
        CurRoom = i;
        return TRUE;

        }
      i = (i + 1) % MAXROOMS;

      }
    return FALSE;

    }
  /*
  * formHeader()
  *
  * This returns a string with the msg header formatted.
  */
char *formHeader()
    {
    static char header[250];
    header[0] = 0;      /* Initialize the genie.... */
    if (msgBuf.mbdate[ 0])  sPrintf(lbyte(header), "  %s ",DisplayDate( msgBuf.mbdate ));
    if (msgBuf.mbtime[ 0] && sendTime) sPrintf(lbyte(header), "%s ", msgBuf.mbtime);
    if (msgBuf.mbauth[ 0]) sPrintf(lbyte(header), "from %s" ,msgBuf.mbauth );
    NormStr(msgBuf.mboname);
    if (msgBuf.mboname[0])
      {
      sPrintf(lbyte(header), " @ %s", msgBuf.mboname);
      if (msgBuf.mbdomain[0])sPrintf(lbyte(header), cfg.DomainDisplay, msgBuf.mbdomain);
      };
    if (strCmpU(msgBuf.mbroom, roomBuf.rbname) != SAMESTRING)
      {
      strCat(header, " in ");
      if (roomExists(msgBuf.mbroom) != ERROR)
      sPrintf(lbyte(header), formRoom(roomExists(msgBuf.mbroom), FALSE,
      FALSE));
      else
      sPrintf(lbyte(header), "%s>", msgBuf.mbroom);

      };
    if (msgBuf.mbto[   0])
      {
      sPrintf(lbyte(header), " to %s" , msgBuf.mbto);
      if (!msgBuf.mbauth[0] && thisRoom == MAILROOM &&
      strLen(cfg.SysopName) != 0)     /* Mail to sysop */
      sPrintf(lbyte(header), " (%s)", cfg.SysopName);

      };
    if (msgBuf.mbaddr[ 0] &&
    strncmp(msgBuf.mbaddr, R_SH_MARK, strLen(R_SH_MARK)) != SAMESTRING &&
    strncmp(msgBuf.mbaddr, LOC_NET, strLen(LOC_NET)) != SAMESTRING &&
    strncmp(msgBuf.mbaddr, NON_LOC_NET, strLen(NON_LOC_NET)) != SAMESTRING)
    sPrintf(lbyte(header), " (on %s)", strCmpU(msgBuf.mbaddr, ALL_LOCALS) ?
    msgBuf.mbaddr : "All Local Systems");
    return header;

    }
  /*
  * formRoom()
  *
  * This returns a string with the room formatted, including the prompt type.
  */
  char display[40];
  char matrix[2][2] =
    {
      { '>', ')'  },
      { ']', ':'  }

    };

#define ANSII_NONE   "[0m"       /* reset, ansii off */
#define ANSII_RED    "[31;40m"   /* RED    */
#define ANSII_GREEN  "[32;40m"   /* GREEN  */
#define ANSII_ORANGE "[33;40m"   /* ORANGE */
#define ANSII_BLUE   "[34;40m"   /* BLUE   */


char *ccode[4] =
    {
    ANSII_ORANGE,  /* Orange: Regular, no directory   */
    ANSII_RED,     /* RED:    Networked, no directory */
    ANSII_GREEN,   /* GREEN:  Regular, Directory      */
    ANSII_BLUE     /* BLUE:   Networked, Directory    */
    };

char *formRoom(int roomNo, int showPriv, int noDiscrimination)
    {
    int   one, two, color;
    one = roomTab[roomNo].rtflags.ISDIR;
    two = (roomTab[roomNo].rtflags.SHARED && cfg.BoolFlags.netParticipant);
    if (roomTab[roomNo].rtflags.INUSE)
      {
      if (!noDiscrimination && !roomTab[roomNo].rtflags.PUBLIC)
        strcpy(display, APrivateRoom);
      else
        {
        if( logBuf.lbflags.ANSI )
          {
          color = one*2 + two;
          sPrintf(display, "%s%s%s%c%s"
          ,ccode[color], roomTab[roomNo].rtname,ANSII_NONE, matrix[one][two],
          (!roomTab[roomNo].rtflags.PUBLIC && showPriv) ? "*" : "");
          }
        else
          {
          sPrintf(display, "%s%c%s", roomTab[roomNo].rtname, matrix[one][two],
          (!roomTab[roomNo].rtflags.PUBLIC && showPriv) ? "*" : "");
          };
        };
      }
    else display[0] = '\0';
    return display;

    }
  /*
  * getCdate()
  *
  * This retrieves system date and returns in the parameters.
  */
  void getCdate(int *year, char **month, int *day, int *hours, int *minutes)
    {
    int mon, seconds, milli;
    getRawDate(year, &mon, day, hours, minutes, &seconds, &milli);
    *year -= 1900;
    *month = monthTab[mon];

    }
  /*
  * GetSecond()
  *
  * This will return the second of the minute.  For multibanner.
  */
  int GetSecond()
    {
    int y, d, h, m, seconds, ml, mon;
    getRawDate(&y, &mon, &d, &h, &m, &seconds, &ml);
    return seconds;

    }
  /*
  * HelpIfPresent()
  *
  * This will print help file if present, but not complain.
  */
  char HelpIfPresent(char *filename)
    {
    SYS_FILE fn;
    makeSysName(fn, filename, &cfg.homeArea);
    if (access(fn, 4) == 0)
      {
      tutorial(filename, TRUE);
      return TRUE;

      }
    else return FALSE;

    }
  /*
  * ingestFile()
  *
  * This puts the given file in the held msg buffer.
  */
  char ingestFile(char *name, MessageBuffer *msg)
    {
    char  filename[100];  /* Paths, etc.... */
    FILE  *fd;
    int   c, d, index;
    extern char *READ_TEXT;
    strCpy(filename, name);
    if ((fd = safeopen(filename, READ_TEXT)) == NULL)
      {
      return FALSE;

      }
    index = (heldMess) ? strLen(msg->mbtext) : 0;
    while ((c = fgetc(fd)) != EOF && index < MAXTEXT - 2)
      {
      if (c)
        {
        if (c == '\n')
          {
          /*
          * this should shave off trailing spaces.
          */
          while (index - 1 >= 0 && msg->mbtext[index - 1] == ' ')
          index--;
          while (!(d = fgetc(fd)))   /* skip any following zero bytes */
          ;
          if (d == '\n' || d == ' ' || d == EOF)
            {
            msg->mbtext[index++] = c;
            if (d != EOF)
            msg->mbtext[index++] = d;

            }
          else if (d)
            {
            msg->mbtext[index++] = ' ';
            msg->mbtext[index++] = d;

            }

          }
        else msg->mbtext[index++] = c;

        }

      }
    msg->mbtext[index] = 0;
    fclose(fd);
    CleanEnd(msg->mbtext);
    if (msg == &tempMess) heldMess = TRUE;
    return TRUE;

    }
  /*
  * formDate()
  *
  * This function forms the current date.
  */
  char *formDate()
    {
    static char dateLine[40];
    int  day, year, h, m;
    char *month;
    getCdate(&year, &month, &day, &h, &m);
    sPrintf(dateLine, "%d%s%02d", year, month, day);
    return dateLine;

    }
  /*
  * Current_Time()
  *
  * This function will get the current time, format cutely.
  */
  char *Current_Time()
    {
    char  *ml, *month;
    int   year, day, h, m;
    static char Time[13];
    getCdate(&year, &month, &day, &h, &m);
    civTime(&h, &ml);
    sPrintf(Time, "%d:%02d %s", h, m, ml);
    return Time;

    }
  /*
  * MultiBanner()
  *
  * This function handles the multibanner feature.
  */
  char MultiBanner(char *str)
    {
    SYS_FILE temp;
    makeBanner(temp, str, GetSecond());
    if (access(temp, 4) == 0)
      {
      tutorial(temp, FALSE);
      return TRUE;

      }
    else return FALSE;

    }
  /*
  * putBufChar()
  *
  * This is used to upload messages via protocol.
  * returns: ERROR on problems else TRUE.
  */
  int putBufChar(int c)
    {
    char result;
    if (masterCount == MAXTEXT + 10) return TRUE;
    if (masterCount > MAXTEXT - 2) return ERROR;
    /* This is necessary for a ProComm bug */
    if (c == CPMEOF)
      {
      masterCount = MAXTEXT + 10;
      return TRUE;

      }
    c &= 0x7F;          /* strip high bit */
    result = cfg.filter[c];
    if (result == '\0')
      {
      return TRUE;

      }
    msgBuf.mbtext[masterCount++] = result;
    msgBuf.mbtext[masterCount]   = 0;   /* EOL just for luck    */
    return TRUE;

    }
  /*
  * putFLChar()
  *
  * This is used to upload files.
  * returns: ERROR on problems else TRUE.
  */
  int putFLChar(int c)
    {
    extern FILE *netLog;
    if (fputc(c, upfd) != EOF)  return TRUE;
    /* else */      splitF(netLog, "Write error: %d\n", ferror(upfd));
    return ERROR;

    }
  /*
  * reconfigure()
  *
  * This function reconfigures a user, depending on their selection on the
  * dot command.
  *
  * Note: returns TRUE on backspace, FALSE otherwise
  */
  char reconfigure()
    {
    extern char MeetDisabled;
    char  *ON  = "ON", *OFF = "OFF";
    label alias, domain;
    char  system[(2 * NAMESIZE) + 10];
    int cost;
    extern int thisNet;
    char *ConfgOpts[] =
      {
      "C(omplete Reconfigure)  ", "E(xpert)          ", "F(loor mode)\n",
      "H(alf-duplex mode)      ", "L(inefeeds)       ", "N(ulls)\n",
      "O(ld messsage on new)   ", "T(ime of messages)", "D(elay)\n",
      "W(idth in columns)      ", "\r",                 "\n",
      "Z(Old .RE)              ", "G(raphics)        ", "\b",
      "Mail Forwarding         ", "Y(ield after msg)   ",
      "Prompt (message entry)  ", "R(date foRmat)\n",
      " ", " ", ""

      };
    static char *NOW = "Now %s.";
    RegisterThisMenu("confg.mnu", ConfgOpts);
    if (cfg.BoolFlags.netParticipant)
    ExtraOption(ConfgOpts, "Address\n");
    if (!MeetDisabled && loggedIn)
      {
      ExtraOption(ConfgOpts, "Biography");

      }
    switch (GetMenuChar())
      {
      case '\b': mPrintf(" \b"); PushBack('\b'); return TRUE;
      case 'A':     /* Forwarding address on the network */
      if (!ReqNodeName("FWRDML", alias, domain, FALSE,
      FALSE, TRUE, FALSE, FALSE, &netBuf) &&
      onLine())
        {
        /* in case carrier is lost */
        if (SearchList(&MailForward, logBuf.lbname) == NULL)
        break;
        else if (getYesNo("STOPFM"))
          {
          KillData(&MailForward, logBuf.lbname);
          UpdateForwarding();
          break;

          }

        }
      else if (onLine())
        {
        /* in case carrier is lost */
        sPrintf(system, (strLen(domain) != 0) ? "%s _ %s" : "%s%s",
        alias, domain);
        if (strLen(domain) != 0) cost = FindCost(domain);
        else cost = !netBuf.nbflags.local;
        if (cost > logBuf.credit)
          Output_Citadel_Message("WARNCR",(long)system,NULL,NULL);
        getString("ALIASF", alias, NAMESIZE, 0);
        if (onLine())
          {
          /* i.e., didn't drop carrier */
          AddMailForward(logBuf.lbname, system,
          (strLen(alias) != 0) ? alias : logBuf.lbname);
          if (!logBuf.lbflags.NET_PRIVS)
            Output_Citadel_Message("NETPRI",NULL, NULL, NULL);

          }

        }
      break;
      case 'M':
      getString("ALIASM", alias, NAMESIZE, 0);
      if (strLen(alias) != 0)
        {
        if (findPerson(alias, &logTmp) == ERROR)
            Output_Citadel_Message("NOPERS",(long)alias, NULL, NULL);
        else if (strCmpU(logTmp.lbname, logBuf.lbname) == SAMESTRING)
            Output_Citadel_Message("YRNAME",NULL, NULL, NULL);
        else
        AddMailForward(logBuf.lbname, NULL, alias);

        }
      else if (getYesNo("STOPFM"))
        {
        KillLocalFwd(logBuf.lbname);

        }
      break;
      case 'B':
      EditBio();
      break;
      case 'C':
      configure(&logBuf, FALSE, FALSE);
      break;
      case 'D':
      logBuf.lbdelay   = (int) getNumber("MDELAY", 0l, 255l);
      break;
      case 'R':
      cost  = (int) getNumber("MSGCDT",0l,2l);
      switch (cost)
        {
        case 0: /* standard */
          logBuf.lbflags.DF_DATE = 0;
          logBuf.lbflags.LG_DATE = 0;
          break;
        case 1: /* long     */
          logBuf.lbflags.DF_DATE = 1;
          logBuf.lbflags.LG_DATE = 1;
          break;
        case 2: /* short    */
          logBuf.lbflags.DF_DATE = 1;
          logBuf.lbflags.LG_DATE = 0;
          break;
        };
      break;
      case 'G':
      logBuf.lbflags.ANSI = getYesNo("ANSIHD");
      break;
      case 'Y':
      logBuf.lbflags.MSG_PAUSE = getYesNo("MSGPAS");
      break;
      case 'E':
      mPrintf(NOW, (expert = !expert) ? ON : OFF);
      break;
      case 'P':
      mPrintf(NOW,(logBuf.lbflags.NoPrompt=!logBuf.lbflags.NoPrompt) ? OFF : ON);
      break;
      case 'Z':
      mPrintf(NOW, (logBuf.lbflags.ALT_RE = !logBuf.lbflags.ALT_RE) ? ON : OFF);
      break;
      case 'F':
      mPrintf(NOW, (FloorMode = !FloorMode) ? ON : OFF);
      break;
      case 'H':
      mPrintf(NOW, (HalfDup = !HalfDup) ? ON : OFF);
      break;
      case 'L':
      mPrintf(NOW, (termLF = !termLF) ? ON : OFF);
      break;
      case 'N':
      termNulls   = (int) getNumber("NULLSN", 0l, 255l);
      break;
      case 'O':
      mPrintf(NOW, (oldToo = !oldToo) ? ON : OFF);
      break;
      case 'T':
      mPrintf(NOW, (sendTime = !sendTime) ? ON : OFF);
      break;
      case 'W':
      termWidth   = (int) getNumber("COLSCRNO", 40l, 132l);
      break;
      case '\r':
      case '\n':
        tutorial("confg.mnu", TRUE);
      case '?':
        Display_User_Configuration();
      break;

      }
    storeLog();         /* save the current log entry!*/
    return FALSE;

    }

static void Display_User_Configuration()
  {
  ForwardMail *address;
  mPrintf("\n Current user setup:");
  mPrintf("\n     User Status: %10s           ANSI Headers: %3s",
  (expert) ? "Expert" : "Novice", (logBuf.lbflags.ANSI) ? "Yes" : "No");
  mPrintf("\n     Floor Mode : %10s           Use Alt .RE : %3s",
  (FloorMode) ? "On" : "Off",   (logBuf.lbflags.ALT_RE) ? "Yes" : "No");
  mPrintf("\n Linefeed Needed: %10s           Nulls       : %3d",
  (termLF) ? "Yes" : "No",  termNulls);
  mPrintf("\n Print msg time : %10s           Screen width: %3d",
  (sendTime) ? "Yes" : "No", termWidth);
  mPrintf("\n Last old msg   : %10s           Half-Duplex : %3s",
  (oldToo) ? "Yes" : "No", (HalfDup) ? "Yes" : "No");
  mPrintf("\n Char. delay    : %10d           Room Prompt : %3s",
  logBuf.lbdelay, (logBuf.lbflags.NoPrompt) ? "No" : "Yes");
  mPrintf("\n Message Pause  : %10s", (logBuf.lbflags.MSG_PAUSE) ? "Yes": "No");
  if (cfg.BoolFlags.netParticipant &&
     (address = SearchList(&MailForward, logBuf.lbname)) != NULL)
    {
    mPrintf("\n Forward Mail> to %s @%s.", address->Alias,address->System);
    };
  if (FindLocalForward(logBuf.lbname) != NULL)
    {
    mPrintf("\n Forward Mail> to local account %s.",
      FindLocalForward(logBuf.lbname));
    };
  mPrintf("\n ");
  }
  /*
  * SaveInterrupted()
  *
  * This saves an interrupted message.
  */
  void SaveInterrupted(MessageBuffer *SomeMsg)
    {
    SYS_FILE temp;
    SYS_FILE save_mess;
    extern char CCOutFlag;
    if (cfg.BoolFlags.HoldOnLost)
      {
      sPrintf(temp, LCHeld, thisLog);
      makeSysName(save_mess, temp, &cfg.holdArea);
      if (access(save_mess, 0) == -1)
        {
        if ((upfd = safeopen(save_mess, WRITE_ANY)) == NULL)
        printf("Failed to open save file!\n");
        else
          {
          crypte(SomeMsg, STATIC_MSG_SIZE, thisLog);
          fwrite(SomeMsg, STATIC_MSG_SIZE, 1, upfd);
          crypte(SomeMsg->mbtext, MAXTEXT, thisLog);
          fwrite(SomeMsg->mbtext, MAXTEXT, 1, upfd);
          CCOutFlag = TEXTFILE;
          RunList(&SomeMsg->mbCC, DisplayCC);
          fclose(upfd);

          }

        }

      }

    }
  /*
  * TranFiles()
  *
  * This handles transfer of files to users:
  *
  * 1. Gets number of files, number of bytes.
  * 2. Performs time calculations.
  * 3. Starts up protocols.
  */
  void TranFiles(int protocol, char *phrase)
    {
    extern unsigned long netBytes;
    int NumFiles;
    char FileSpec[100];
    getNormStr("EFILEN", FileSpec, sizeof FileSpec, 0);
    if (strLen(FileSpec) == 0) return;
    netBytes = 0l;
    NumFiles = wildCard(getSize, FileSpec, TRUE, phrase, TRUE);
    if (NumFiles <= 0l)
      {
      mPrintf("Sorry, no match for '%s'.\n ", FileSpec);
      return;

      }
    if (!TranAdmin(protocol, NumFiles)) return;
    TranSend(protocol, transmitFile, FileSpec, phrase, TRUE);

    }
  /*
  * TranAdmin()
  *
  * Transfer file administrator.
  */
  char TranAdmin(int protocol, int NumFiles)
    {
    long seconds;
    extern unsigned long netBytes;
    int s;
    if (NumFiles != 1 && ((InternalProtocol(protocol) &&
    !(Table[protocol].flags & IS_NUMEROUS)) ||
    (!InternalProtocol(protocol) && !DoesNumerous(protocol))))
      {
      mPrintf("%s does not support batch mode.\n ",
      (InternalProtocol(protocol)) ?
      Table[protocol].name : FindProtoName(protocol));
      return FALSE;

      }
    if (!InternalProtocol(protocol) || Table[protocol].flags & RIGAMAROLE)
      {
      mPrintf("This %s transfer involves %s bytes (",
      (!InternalProtocol(protocol)) ? FindProtoName(protocol) :
      Table[protocol].name,
      PrintPretty(netBytes, msgBuf.mbtext));
      mPrintf("%d file%s", NumFiles, NumFiles == 1 ? "" : "s");
      if (InternalProtocol(protocol))
      mPrintf(" : %ld blocks",
      ((netBytes+(Table[protocol].BlockSize-1))/Table[protocol].BlockSize));
      mPrintf(").  ");
      s = (!InternalProtocol(protocol)) ? 1 : protocol;
      if (Table[s].KludgeFactor != 0 && byteRate != 0)
        {
        seconds = (long) Table[s].KludgeFactor * netBytes /
        (byteRate * 10);
        if (!CheckDLimit(seconds)) return FALSE;
        mPrintf("It should take %ld:%02ld.\n ", seconds/60, seconds % 60);

        }
      return getYesNo("RDYBEG");

      }
    return TRUE;

    }
/*
 * TranSend()
 *
 * This does the send work of TranFiles().
 */
void TranSend(int protocol, void (*fn)(DirEntry *f), char *FileSpec,
                                                 char *phrase, char NeedToMove)
{
    char success;
    DirEntry temp = { "", "", 0l };
    SpecialMessage("Status:File Transfer");

    startTimer(WORK_TIMER);

    if (InternalProtocol(protocol)) {
        TransProtocol = protocol;

        wildCard((FormatFlag) ? doFormatted : fn, FileSpec, NeedToMove,
                                        phrase, TRUE);

        if (Table[protocol].flags & NEEDS_FIN)
            (*fn)(&temp);
    }
    else {
        success = ExternalProtocol(protocol, FALSE, FileSpec, phrase, NeedToMove);
        if (success == TRAN_SUCCESS)
            FileTransStat = FL_SUCCESS;
        else
            FileTransStat = FL_FAIL;
    }

    if (!InternalProtocol(protocol) || (Table[protocol].flags & IS_DL))
        *DL_Total += chkTimeSince(WORK_TIMER);

    TransProtocol = ASCII;
    if (!InternalProtocol(protocol) || (Table[protocol].flags & RIGAMAROLE))
        oChar(BELL);
}


  /*
  * transmitFile()
  *
  * This dumps a host file with no formatting.
  */
  void transmitFile(DirEntry *file)
    {
    FILE *fbuf;
    long fileSize = 0l;
    char *filename, success;
    extern char *READ_ANY;
    filename = file->unambig;
    if (strLen(filename) == 0)          return; /* no filename */
    fbuf = safeopen(filename, READ_ANY);
    if ( fbuf == NULL)                  return; /* cannot open */
    totalBytes(&fileSize, fbuf);
    if (Table[TransProtocol].flags & RIGAMAROLE)
       printf("%s: %s (%ld bytes, %ld blocks)\n", Table[TransProtocol].name, filename, fileSize,
            ((fileSize+(Table[TransProtocol].BlockSize-1))/Table[TransProtocol].BlockSize));
    fileMessage(FL_START, filename, TRUE, TransProtocol, 0l);
    if (Transmission(TransProtocol, STARTUP) != TRAN_SUCCESS)
      {
      fclose(fbuf);
      fileMessage(FL_FAIL, filename, TRUE, TransProtocol, fileSize);
      return ;
      }
    if (Table[TransProtocol].flags & NEEDS_HDR)
      {
      if (!(*Table[TransProtocol].SendHdr)(fileSize, filename))
        {
        fclose(fbuf);
        fileMessage(FL_FAIL, filename, TRUE, TransProtocol, fileSize);
        return ;
        };
      };
    SendThatDamnFile(fbuf, Table[TransProtocol].method);
    success = (Transmission(TransProtocol, FINISH) != TRAN_SUCCESS) ? FL_FAIL : FL_SUCCESS;
    fileMessage(success, filename, TRUE, TransProtocol, fileSize);
    if (TransProtocol == ASCII && outFlag != OUTOK)
      {
      doCR();
      outFlag = OUTOK;
      };
   }
/*
 * SendThatDamnFile()
 *
 * This will actually send the file.
 */
void SendThatDamnFile(FILE *fbuf, int (*method)(int c))
  {
    int c, delay_factor = 0;
    #ifdef AMIGA
    int oldc = 0;
    #endif
    if( loggedIn )
      {
      if( !onConsole && logBuf.lbdelay > 0 )
        {
        delay_factor = ( 256 - logBuf.lbdelay );
        };
      };
    while ((c = fgetc(fbuf)) != EOF && (c != CPMEOF || !textDownload))
      {
      if (TransProtocol == ASCII) mputChar(c);
      if (gotCarrier()) if (!(*method)(c))break;
      if (TransProtocol == ASCII)
        {
        if( loggedIn )
          {
          if (!onConsole && logBuf.lbdelay > 0 )
            {
            if( delay_factor-- <= 0 )
              {
              delay_factor = ( 256 - logBuf.lbdelay );
              MilliSecPause(3);
              };
            };
          };
        if (mAbort() || (whichIO == MODEM && !gotCarrier())) break;
        #ifdef AMIGA
        /* make handling text files more sane for non-Amiga people */
        if (gotCarrier() && c == '\n' && oldc != '\r')(*method)('\r');
        #endif

        }
      #ifdef AMIGA
      oldc = c;
      #endif

      }
    fclose(fbuf);
    textDownload = FALSE;

    }
  /*
  * tutorial()
  *
  * This prints file <filename> on the modem & console.
  * Returns:  TRUE on success else ERROR.
  */
  char tutorial(char *filename, char addHelpArea)
    {
    /* This includes writeTutorial */
    FILE     *fbuf;
    SYS_FILE fn;
    char     line[MAXWORD];
    extern char *READ_TEXT;
    if (addHelpArea)
    makeSysName(fn, filename, &cfg.homeArea);
    else
    strCpy(fn, filename);
    if ((fbuf = safeopen(fn, READ_TEXT)) == NULL)
      {
      mPrintf(NoFileStr, filename);
      return ERROR;

      }
    if (outFlag != IMPERVIOUS) outFlag     = OUTOK;
    Output_Citadel_Message("HOTHLP", NULL, NULL, NULL);
    while (fgets(line, MAXWORD, fbuf) && outFlag != OUTSKIP)
    mPrintf("%s", line);
    if (!PrintBanner) outFlag     = OUTOK;
    fclose(fbuf);
    return TRUE;    /* good as anything */

    }

/*
 * upLoad()
 *
 * This enters a file into current directory.
 */
void upLoad(int WC, char *file, char NeedToMove)
{
    char fileName[MAX_FILENAME - 1];
    char curdir[100];
    char successful;
    long size=0;

    getcwd(curdir, 100);
    if (file)
        strcpy(fileName, file);
    else
        getNormStr("EFILEN", fileName, sizeof fileName, 0);
    if (!fileName[0]) return;

        /* Can't tolerate bad file names */
    if (!ValidDirFileName(fileName)) {
        mPrintf("Illegal file name.\n ");
        return ;
    }

    if (NeedToMove) {
            if (!setSpace(&roomBuf)) {          /* System error -- yucky. */
                return ;
        }
    }

    if (LowFree != 0 && RoomLeft(&roomBuf) < LowFree) {
        mPrintf("Sorry, not enough room left on disk.\n ");
        if (NeedToMove)
            homeSpace();
        return ;
    }

    if (access(fileName, 0) != -1) {
                                    /* File already exists */
        mPrintf("\n File: %s already exists.\n ", fileName);
        if (NeedToMove)
            homeSpace();
        return;
    } else {                    /* Go for it */
        if (!expert && InternalProtocol(WC)) {
            homeSpace();
            tutorial(Table[WC].UpBlbName, TRUE);
            if (!NeedToMove)
                chdir(curdir);
            else {
      /****
                if (!aide && roomBuf.rbflags.REDIRECT_UPLOADS
                    && cfg.newupArea.naDirname[0]) {
                    netSetNewArea(&cfg.newupArea);
                }
                else
      *****/
                    setSpace(&roomBuf);
            }
        }

        if (!getYesNo("RDYBEG")) {
            if (NeedToMove)
                homeSpace();
            return;
        }

        if (!InternalProtocol(WC)) {
            fileMessage(FL_START, fileName, FALSE, WC, 0l);
            successful = (ExternalProtocol(WC, TRUE, fileName, NULL, NeedToMove)
                                                == TRAN_SUCCESS);
            if (successful) {       /* so we can get the file size */
                upfd = safeopen(fileName, READ_ANY);
            }
        }
        else {
            if ((upfd = safeopen(fileName, WRITE_ANY)) == NULL) {
                mPrintf("\n Can't create %s!\n", fileName);
                if (NeedToMove)
                    homeSpace();
                return;
            }
#ifdef HORRID_AMIGA_LATTICE_BUG
            setnbf(upfd);
#endif
            fileMessage(FL_START, fileName, FALSE, WC, 0l);
            successful = (Reception(WC, putFLChar) == TRAN_SUCCESS);
        }

        if (successful) totalBytes(&size, upfd);
        fclose(upfd);

        if (!successful) unlink(fileName);
        else if ((successful = FileIntegrity(fileName))==0) {
            unlink(fileName);
        }

        if (NeedToMove)
            homeSpace();

        fileMessage(successful ? FL_SUCCESS : FL_FAIL, fileName,
                                        FALSE, WC, size);
        if (successful && fileName[0] != '*' && NeedToMove)  {
  /***          if (loggedIn)
                upldcnt++;  ***/
            FileCommentUpdate(fileName, TRUE);
        }
    }
}


  /*
  * FileCommentUpdate()
  *
  * This updates the file comment.
  */
  void FileCommentUpdate(char *fileName, char aideMsg)
    {
    char *tmp;
    /* MsgEntryType = FILE_ENTRY; */
    do
      {
      msgBuf.mbtext[0] = 0;
      FindFileComment(fileName);
      mPrintf("Please enter a description of %s (end with blank line).\n ", fileName);
      doCR();

      }
    while (onLine() && !GetBalance(ASCII, msgBuf.mbtext, MAXTEXT-50));
    if (onLine())
      {
      setSpace(&roomBuf); /* update file comments */
      while ((tmp = strchr(msgBuf.mbtext, '\r')) != NULL)
      *tmp = ' ';
      while ((tmp = strchr(msgBuf.mbtext, '\n')) != NULL)
      *tmp = ' ';
      if (aideMsg || strLen(msgBuf.mbtext) != 0)
        {
        if (loggedIn && !roomBuf.rbflags.ANON)
        sPrintf(lbyte(msgBuf.mbtext), " [%s].", logBuf.lbname);
        updFiletag(fileName, msgBuf.mbtext);

        }
      homeSpace();
      if (aideMsg || strLen(msgBuf.mbtext) != 0)
        {
        tmp = strdup(msgBuf.mbtext);
        ZeroMsgBuffer(&msgBuf);
        sPrintf(msgBuf.mbtext, "File \"%s\" uploaded into %s.",
        fileName, formRoom(thisRoom, FALSE, FALSE));
        if (loggedIn)
        sPrintf(lbyte(msgBuf.mbtext) - 1, " by %s.", logBuf.lbname);
        if (aideMsg) aideMessage(NULL,FALSE);
        sPrintf(msgBuf.mbtext, "File \"%s\" uploaded into %s:\n \n%s",
        fileName, formRoom(thisRoom, FALSE, FALSE), tmp);
        strCpy(msgBuf.mbauth, "Citadel");
        putMessage(&logBuf);  /* Now save message in this room*/
        noteRoom();

        }
      if (tmp != NULL) free(tmp);

      }

    }
  /*
  * visible()
  *
  * This converts given char to printable form if nonprinting.
  */
  char visible(AN_UNSIGNED c)
    {
    if (c==0xFF)  c = '$' ;   /* start-of-message in message.buf  */
    c       = c & 0x7F  ;   /* kill high bit otherwise    */
    if ( c < ' ') c = c + 'A' -1;   /* make all control chars letters   */
    if (c== 0x7F) c = '~' ;   /* catch DELETE too     */
    return (char)(c);

    }
  char *Menu = NULL;
  char **ValidMenuOpts;
  /*
  * GetMenuChar()
  *
  * This will get a character for a menu.
  */
  int GetMenuChar()
    {
    int c, i;
    c = toUpper(iChar());
    for (i = 0; ValidMenuOpts[i][0]; i++)
    if (c == ValidMenuOpts[i][0])
    break;
    if (!ValidMenuOpts[i][0])
      {
      if (!onLine() || (expert && c != '?'))
        {
        c = 0;
        mPrintf(" ?\n ");

        }
      else
        {
        c = '?';
        if (Menu != NULL) tutorial(Menu, TRUE);

        }

      }
    else mPrintf("%s ", ValidMenuOpts[i] + 1);
    return c;

    }
  /*
  * ExtraOption()
  *
  * This adds an option to a menu.
  */
  void ExtraOption(char *Opts[], char *NewOpt)
    {
    int i;
    for (i = 0; Opts[i][0] && Opts[i][0] != ' '; i++)
    ;
    if (!Opts[i][0])
    crashout("INTERNAL: No room for new option!");
    Opts[i] = NewOpt;

    }
  static char *MPtr;
  static int  MCount;
  /*
  * CmdMenuList()
  *
  * This reads in a command line, doing backspacing.
  */
  int CmdMenuList(char *Opts[], SListBase *Selects, char *HelpFile, char *buf,
  char moreYet, char OneMore)
    {
    int c, rover;
    char *cmd;
    void CopyBuf();
    do
      {
      c = toUpper(iChar());
      if (c == '\b')
        {
        mPrintf(" \b");
        if ((cmd = GetLast(Selects)) == NULL)
          {
          if (!OneMore) oChar(' ');
          return BACKED_OUT;

          }
        for (rover = 0; rover < strlen(cmd) - 1; rover++)
        if (cmd[rover] != '\b') mPrintf("\b \b");
        else oChar(' ');
        KillData(Selects, cmd);
        if (OneMore)
        mPrintf("\b \b");

        }
      else
        {
        for (rover = 0; Opts[rover][0] != 0; rover++)
        if (toUpper(Opts[rover][1]) == toupper(c))
        break;
        if (c == 0 || Opts[rover][0] == 0 ||
        SearchList(Selects, Opts[rover] + 1) != NULL)
          {
          if (!onLine() || (expert && c != '?'))
            {
            mPrintf(" ?\n ");

            }
          else if (HelpFile != NULL)
            {
            tutorial(HelpFile, TRUE);

            }
          else mPrintf(" ? (Type '?' for menu)\n \n"   );
          buf[0] = 1;   /* general error */
          return BAD_SELECT;

          }
        mPrintf("%s ", Opts[rover] + 2);
        AddData(Selects, Opts[rover] + 1, NULL, FALSE);

        }

      }
    while ((c == '\b' || Opts[rover][0] != TERM[0]) && moreYet);
    MPtr = buf;
    MCount = 0;
    RunList(Selects, CopyBuf);
    return GOOD_SELECT;

    }
  void CopyBuf(char *name)
    {
    MPtr[MCount++] = name[0];
    MPtr[MCount] = 0;

    }
  /*
  * FindSelect()
  *
  * This function will find an element of a command selection.
  */
  void *FindSelect(char *element, char *data)
    {
    if (element == data) return element;
    return NULL;

    }

