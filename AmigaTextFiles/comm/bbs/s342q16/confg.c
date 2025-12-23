/*
*           confg.c
*
* Configuration program for Citadel bulletin board system.
*/
#define CONFIGURE
#include "ctdl.h"
#include "math.h"
/*
*       History
*
* 85Dec26 HAW  Add CALL-LOG define.
* 85Nov15 HAW  MS-DOS library update.
* 85Oct27 HAW  Kill CERMETEK.
* 85Oct17 HAW  Add paramVers, change bauds to array, searchBaud chg.
* 85Oct16 HAW  Kill CLOCK, add officeStuff.
* 85Aug24 HAW  Add duomessage file, NETDISK specification.
* 85Jul07 HAW  Update so won't go through total recon. on init.
* 85May27 HAW  Start stuffing in auto-networking stuff.
* 85May22 HAW  Start conversion to make log file size sysop selectable.
* 85May11 HAW  Make "Lobby" sysop definable
* 85May06 HAW  Add bail out code.
* 85May05 HAW  Add helpDisk parameter for 3 disk system.
* 85Apr10 HAW  Fix logSort, alphabetize file.
* 85Mar11 HAW  Put all user functions in this file.
* 85Feb18 HAW  Add baud search stuff.
* 85Jan20 HAW  Use MSDOS #define for date stuff.
* 84Sep05 HAW  Isolate strangeness in compiler's library.  See note.
* 84Aug30 HAW  Onwards to MS-DOS!
* 84Apr08 HAW  Update to BDS C 1.50a begun.
* 82Nov20 CrT  Created.
*/
/*
*       Contents
*
* dGetWord()    reads a word off disk
* init()      system startup initialization
* main()      main controller
* illegal()   abort bottleneck
* msgInit()   sets up cfg.catChar, catSect etc.
* zapMsgFile()    initialize ctdlmsg.sys
* realZap()   does work of zapMsgFile()
* indexRooms()    build RAM index to ctdlroom.sys
* noteRoom()    enter room into RAM index
* zapRoomFile()   erase & re-initialize ctdlroom.sys
* setSpace()    set default disk and user#
* hash()      hashes a string to an integer
* logInit()   builds the RAM index to CTDLLOG.SYS
* noteLog()   enters a userlog record into RAM index
* sortLog()   sort CTDLLOG by time since last call
* wrapup()    finishes and writes ctdlTabl.sys
* zapLogFile()    erases & re-initializes CTDLLOG.SYS
*/
/*
*   Strangenesses   (Hue, Jr., 12Sep84)
* Have discovered that the line:
* sscanf(line, "\"%s\"", str);
* is not parsed the same way by this compiler as it is by BDS;
* this is highly unfortunate and excrable.  So, all porters
* should note that scanf() is not, in any way, "portable."  If
* BDS is "non-standard", then the standard sucks.
*/
#define BAUDS "Valid SYSBAUD values: 0=300, 1=1200, 2=2400, 3=4800,\n4=9600, 5=14.4, 6=19.2, 7=38.4, 8=57.6"
struct GenList
  {
  char *GenName;
  int  GenVal;

  };
extern FILE *msgfl, *msgfl2;    /* file descriptor for the msg file */
char   *baseRoom;
int   mailCount=0;
char   msgZap =  FALSE,
logZap =  FALSE,
roomZap = FALSE;
char  FirstInit = FALSE;
char  ReInit = FALSE;
static DATA_BLOCK sectBuf;
long FloorSize;
int  DefaultPrefix;
void  Zap_Nodes_Message_Index(void);  /* zap last netted message */


static char NetParse(char *line, int *offset, char *var, int arg, int SetVal);
extern SListBase Events;    /* event list     */
extern CONFIG    cfg;      /* The configuration variable  */
extern MessageBuffer   msgBuf;    /* The -sole- message buffer  */
extern NetBuffer netBuf;
extern rTable    *roomTab;        /* RAM index of rooms   */
extern aRoom     roomBuf;        /* room buffer     */
extern EVENT     *EventTab;
extern int       thisRoom;        /* room currently in roomBuf  */
extern int       thisNet;
extern LogTable  *logTab;        /* RAM index of pippuls    */
extern logBuffer logBuf;    /* Log buffer of a person       */
extern SListBase Serves;
extern FILE      *logfl;    /* log file descriptor    */
extern FILE      *roomfl;        /* file descriptor for rooms */
extern FILE      *netfl;
extern LogTable  *logTab;        /* RAM index of pippuls    */
extern int       thisLog;        /* entry currently in logBuf */
extern NetTable  *netTab;
void CheckNet( void );
/*
* init()
*
* The master system initialization.
*/
void init(int attended)
  {
  extern char   *W_R_ANY;
  extern char   *R_W_ANY;
  extern char   *READ_ANY;
  extern char   *WRITE_ANY;
  unsigned char c;
  SYS_FILE      tempName;
  extern int    errno, _doserrno;
  cfg.sizeLTentry = sizeof(*logTab);
  cfg.BoolFlags.debug       = FALSE;
  cfg.BoolFlags.noChat      = TRUE;
  /* shave-and-a-haircut/two bits pause pattern for ringing sysop: */
  cfg.shave[0]    = 40;
  cfg.shave[1]    = 20;
  cfg.shave[2]    = 20;
  cfg.shave[3]    = 40;
  cfg.shave[4]    = 80;
  cfg.shave[5]    = 40;
  cfg.shave[6]    =250;
  /* initialize input character-translation table:  */
  for (c = 0;  c < '\40';  c++)
    {
    cfg.filter[c] = '\0';    /* control chars -> nulls  */

    }
  cfg.filter[1] = 'y';    /* fix from 3.43, carried back to this version */
  cfg.filter[7] = '7';
  for (c='\40'; c < 128;   c++)
    {
    cfg.filter[c] = c;        /* pass printing chars  */

    }
  cfg.filter[SPECIAL]     = SPECIAL;
  cfg.filter[CNTRLl]      = CNTRLl;
  cfg.filter[DEL      ]   = BACKSPACE;
  cfg.filter[BACKSPACE]   = BACKSPACE;
  cfg.filter[XOFF     ]   = XOFF     ;
  cfg.filter[XON      ]   = XON      ;
  cfg.filter['\r'     ]   = NEWLINE  ;
  cfg.filter[CNTRLO   ]   = 'N'      ;
  mvToHomeDisk(&cfg.homeArea);
  makeSysName(tempName, "ctdlmsg.sys",  &cfg.msgArea);
  if ((msgfl = fopen(tempName, R_W_ANY)) == NULL)
    {
    if (!attended)illegal("!System must be attended(ctdlmsg.sys) for creation!");
    printf(" %s not found, creating new file. \n", tempName);
    if ((msgfl = fopen(tempName, W_R_ANY)) == NULL)    illegal("?Can't create the message file!");
    printf(" (Be sure to initialize it!)\n");

    }
  makeSysName(tempName, "ctdllog.sys", &cfg.logArea);
  /* open userlog file */
  if ((logfl = fopen(tempName, R_W_ANY)) == NULL)
    {
    if (!attended)illegal("!System must be attended(ctdllog.sys) for creation!");
    printf(" %s not found, creating new file. \n", tempName);
    if ((logfl = fopen(tempName, W_R_ANY)) == NULL) illegal("?Can't create log file!");
    printf(" (Be sure to initialize it!)\n");

    }
  makeSysName(tempName, "ctdlroom.sys", &cfg.roomArea);
  /* open room file */
  if ((roomfl = fopen(tempName, R_W_ANY)) == NULL)
    {
    if (!attended)illegal("!System must be attended(ctdlroom.sys) for creation!");
    printf(" %s not found, creating new file. \n", tempName);
    if ((roomfl = fopen(tempName, W_R_ANY)) == NULL) illegal("?Can't create room file!");
    printf(" (Be sure to initialize it!)\n");

    }
  if (cfg.BoolFlags.netParticipant)
    {
    makeSysName(tempName, "ctdlnet.sys", &cfg.netArea);
    if ((netfl = fopen(tempName, READ_ANY)) == NULL)
      {
      printf(" %s not found, creating new file.\n", tempName);
      if ((netfl = fopen(tempName, WRITE_ANY)) == NULL)illegal("?Can't create the net file!");

      }

    }
  CheckFloors();
  if (attended || FirstInit)
    {
    char cx;
    printf("\n Erase and initialize log, message and/or room files?");
    cx = simpleGetch();
    if (FirstInit || toUpper(cx)  == 'Y')
      {
      /* each of these has an additional go/no-go interrogation: */
      msgZap  = zapMsgFile();
      roomZap = zapRoomFile();
      logZap  = zapLogFile();

      }

    }

  }
/*
* main()
*
* Main manager for confg.c.
*/
int main(int, char **);
int main(int argc, char **argv)
  {
  FILE *fBuf, *pwdfl;
  char line[90], status, *strchr(), *g;
  char onlyParams = FALSE, CleanCalllog;
  char var[90];
  int  arg;
  int  i, offset = 1;
  extern char *READ_TEXT;
  int  SetVal;
  if (argc > 1)
    {
    if (strCmpU(argv[1], "exists") == 0)
      {
      fBuf = fopen("exists", "w");
      fprintf(fBuf, "I exist.");
      fclose(fBuf);
      exit(1);

      }

    }
  cfg.paramVers = 11;
  printf("%s Configurator (V%d.8)\n%s\n\n", VARIANT_NAME,
  cfg.paramVers, COPYRIGHT);
  if (access(LOCKFILE, 0) != -1)
    {
    char cx;
    printf(
    "You are apparently reconfiguring from within Citadel, which is a No No!\n"
    "Do you wish to continue? ");
    cx = simpleGetch();
    if (toUpper(cx) != 'Y')
    exit(7);
    unlink(LOCKFILE);

    }
  zero_struct(cfg);
  for (i = 1;  i < argc; i++)
    {
    if (strCmpU(argv[i], "onlyParams") == SAMESTRING )
      onlyParams = TRUE;
    else if (strCmpU(argv[i], "FirstInit") == SAMESTRING)
      FirstInit = TRUE;
    else if (strCmpU(argv[i], "ReInit") == SAMESTRING)
      ReInit = TRUE;
    else if( argv[i][0] == '-' )
      {
      switch (argv[i][1])
        {
        case 'o':   /* Only Parameters */
        case 'O':   onlyParams = TRUE; break;
        case 'F':   /* First Initialization */
        case 'I':
        case 'f':
        case 'i':   FirstInit = TRUE; break;
        case 'r':   /* Re-Initialization */
        case 'R':   ReInit = TRUE; break;
        default:
          fprintf(stderr,"Parameter Error, Assuming ReInit\n");
          ReInit = TRUE;
        };
      }
    else if (!sysArgs(argv[i]))
      ReInit = TRUE;
    }
  cfg.weAre     = CONFIGUR;
  if (!(SetVal = readSysTab(FALSE, FALSE)))
    {
    cfg.sysPassword[0] = 0;
    cfg.MAXLOGTAB = 0;      /* Initialize, just in case   */
    cfg.ConTimeOut = -1;  /* optional parameter */
    if (onlyParams)
      {
      printf("'onlyParams' parameter ignored\n");
      onlyParams = FALSE;

      }

    }
  else
    {
    if (EventTab != NULL) free(EventTab);
    i = cfg.BoolFlags.noChat;
    zero_struct(cfg.BoolFlags);
    cfg.BoolFlags.noChat = i;

    }
  cfg.paramVers = 11;   /* yes, set it twice*   */
  InitBuffers();        /* initializes message buffers */
  initSysSpec();        /* Call implementation specific code  */
  /* these are mostly optional param values */
  cfg.SysopName[0]     = 0;
  cfg.SysopArchive[0]  = 0;
  strCpy(cfg.DomainDisplay, " _ %s");
  cfg.InitColumns      = 40;
  cfg.BoolFlags.IsDoor = FALSE;
  cfg.BoolFlags.NetScanBad = FALSE;
  cfg.EvNumber   = 0;
  cfg.MailHub    = 0;
  cfg.DomainHandlers   = 0;
  KillList(&Serves);
  CleanCalllog   = FALSE;
  cfg.Audit     = 0;
  cfg.BoolFlags.RouteMail  = TRUE;
  cfg.BoolFlags.DL_Default = TRUE;
  cfg.AnonMailLength   = 0;
  cfg.LoginAttempts    = 0;
  zero_array(cfg.DialPrefixes);
  cfg.MailHub   = 0;
  EventTab = NULL;
  if ((fBuf = fopen("ctdlcnfg.sys", READ_TEXT)) == NULL)
    {
    /* ASCII mode   */
    printf("?Can't find ctdlCnfg.sys!\n");
    exit(1);

    }
  while (fgets(line, 90, fBuf) != NULL)
    {
    if (line[0] != '#') continue;
    if (sscanf(line, "%s %d ", var, &arg))
      {
      printf("%s", line);
      if (NetParse(line, &offset, var, arg, SetVal))
        {
        continue;

        }
      else if (strCmpU(var, "#CRYPTSEED" )    == SAMESTRING)
        {
        cfg.cryptSeed   = arg;

        }
      else if (strCmpU(var, "#MESSAGEK"  )    == SAMESTRING)
        {
        cfg.maxMSector  = arg*(1024/MSG_SECT_SIZE);

        }
      else if (strCmpU(var, "#LOGINOK"   )    == SAMESTRING)
        {
        cfg.BoolFlags.unlogLoginOk= arg;

        }
      else if (strCmpU(var, "#ISDOOR"   )    == SAMESTRING)
        {
        cfg.BoolFlags.IsDoor = TRUE;

        }
      else if (strCmpU(var, "#DoorPrivs")     == SAMESTRING)
        {
        cfg.BoolFlags.DoorDft     = arg;

        }
      else if (strCmpU(var, "#FILE-PRIV-DEFAULT")     == SAMESTRING)
        {
        cfg.BoolFlags.DL_Default     = arg;

        }
      else if (strCmpU(var, "#ENTEROK"   )    == SAMESTRING)
        {
        cfg.BoolFlags.unlogEnterOk= arg;

        }
      else if (strCmpU(var, "#READOK"    )    == SAMESTRING)
        {
        cfg.BoolFlags.unlogReadOk = arg;

        }
      else if (strCmpU(var, "#ROOMOK"    )    == SAMESTRING)
        {
        cfg.BoolFlags.nonAideRoomOk=arg;

        }
      else if (strCmpU(var, "#ALLMAIL"   )    == SAMESTRING)
        {
        cfg.BoolFlags.noMail      = !arg;

        }
      else if (strCmpU(var, "#UNLOGGED-WIDTH") == SAMESTRING)
        {
        cfg.InitColumns = arg;

        }
      else if (strCmpU(var, "#LOGIN-ATTEMPTS") == SAMESTRING)
        {
        cfg.LoginAttempts = arg;

        }
      else if (strCmpU(var, "#ANON-MAIL-LENGTH") == SAMESTRING)
        {
        cfg.AnonMailLength = arg;

        }
      else if (strCmpU(var, "#CLEAN-CALLLOG") == SAMESTRING)
        {
        CleanCalllog = TRUE;

        }
      else if (strCmpU(var, "#ANONYMOUS-SESSIONS") == SAMESTRING)
        {
        cfg.BoolFlags.AnonSessions = TRUE;

        }
      else if (strCmpU(var, "#MIRRORMSG" )    == SAMESTRING)
        {
        cfg.BoolFlags.mirror = arg;

        }
      else if (strCmpU(var, "#LOGSIZE"   )    == SAMESTRING)
        {
        if (SetVal)
          {
          if (cfg.MAXLOGTAB != arg)
          illegal(
          "LOGSIZE parameter does not equal old value!");

          }
        else
          {
          cfg.MAXLOGTAB   = arg;
          logTab = (LogTable *)
          GetDynamic(sizeof(*logTab) * arg);

          }

        }
      else if (strCmpU(var, "#MAXROOMS"  )    == SAMESTRING)
        {
        if (SetVal)
          {
          if (MAXROOMS != arg)
          illegal(
          "MAXROOMS parameter does not equal old value!");

          }
        else
          {
          MAXROOMS = arg;
          if (MAXROOMS <= 3)
          illegal("MAXROOMS must be greater than 3!");
          roomTab = (rTable *)
          GetDynamic(MAXROOMS * sizeof *roomTab);

          }

        }
      else if (strCmpU(var, "#MSG-SLOTS" )    == SAMESTRING)
        {
        if (SetVal)
          {
          if (MSGSPERRM != arg)
          illegal(
          "MSGSPERRM parameter does not equal old value!");

          }
        else
          {
          MSGSPERRM = arg;

          }

        }
      else if (strCmpU(var, "#MAIL-SLOTS")    == SAMESTRING)
        {
        if (SetVal)
          {
          if (MAILSLOTS != arg)
          illegal(
          "MAILSLOTS parameter does not equal old value!");

          }
        else
          {
          MAILSLOTS = arg;

          }

        }
      else if (strCmpU(var, "#AIDESEEALL")    == SAMESTRING)
        {
        cfg.BoolFlags.aideSeeAll = arg;

        }
      else if (strCmpU(var, "#CONSOLE-TIMEOUT")    == SAMESTRING)
        {
        cfg.ConTimeOut = arg;

        }
      else if (strCmpU(var, "#SYSBAUD"   )    == SAMESTRING)
        {
        cfg.sysBaud   = arg;
        if (arg > 8 || arg < 0)
          {
          illegal(BAUDS);

          }

        }
      else if (strCmpU(var, "#event"    ) == SAMESTRING)
        {
        offset = EatEvent(line, offset);

        }
      else if (strCmpU(var, "#nodeTitle") == SAMESTRING)
        {
        readString(line, &cfg.codeBuf[offset], TRUE);
        cfg.nodeTitle = offset;
        while (cfg.codeBuf[offset])
        offset++;
        offset++;

        }
      else if (strCmpU(var, "#sysPassword") == SAMESTRING)
        {
        readString(line, cfg.sysPassword, FALSE);
        if ((pwdfl = fopen(cfg.sysPassword, READ_TEXT)) == NULL)
          {
          printf("\nNo system password file found\n");
          cfg.sysPassword[0] = 0;

          }
        else
          {
          fgets(cfg.sysPassword, 199, pwdfl);
          /*  cfg.sysPassword[strLen(cfg.sysPassword) - 1] = 0;*/
          while ((g = strchr(cfg.sysPassword, '\n')) != NULL)
          *g = 0;
          if (strLen(cfg.sysPassword) < 15)
            {
            printf("\nSystem password is too short -- ignored\n");
            cfg.sysPassword[0] = 0;

            }
          fclose(pwdfl);

          }

        }
      else if (strCmpU(var, "#sysopName") == SAMESTRING)
        {
        readString(line, msgBuf.mbtext, FALSE);
        if (strLen(msgBuf.mbtext) > 19)
        illegal("SysopName too long; must be less than 20");
        strCpy(cfg.SysopName, msgBuf.mbtext);

        }
      else if (strCmpU(var, "#SYSOP-ARCHIVE") == SAMESTRING)
        {
        readString(line, msgBuf.mbtext, FALSE);
        if (strLen(msgBuf.mbtext) > sizeof cfg.SysopArchive)
          {
          sprintf(msgBuf.mbtext, "SysopName too long; must be less than %d", sizeof cfg.SysopArchive);
          illegal(msgBuf.mbtext);

          }
        strCpy(cfg.SysopArchive, msgBuf.mbtext);

        }
      else if (strCmpU(var, "#baseRoom") == SAMESTRING)
        {
        readString(line, &cfg.codeBuf[offset], TRUE);
        if (strLen(&cfg.codeBuf[offset]) > 19)
        illegal("baseRoom too long; must be less than 20");
        cfg.bRoom = offset;
        baseRoom = &cfg.codeBuf[offset];
        while (cfg.codeBuf[offset])
        offset++;
        offset++;

        }
      else if (strCmpU(var, "#MainFloor") == SAMESTRING)
        {
        readString(line, &cfg.codeBuf[offset], TRUE);
        if (strLen(&cfg.codeBuf[offset]) > 19)
        illegal("#MainFloor too long; must be less than 20");
        cfg.MainFloor = offset;
        while (cfg.codeBuf[offset])
        offset++;
        offset++;

        }
      else if (strCmpU(var, "#alldone") == SAMESTRING)
        {
        break;

        }
      else
        {
        offset = sysSpecs(line, offset, &status, fBuf);
        if (!status)
        printf("? -- no variable '%s' known! -- ignored.\n", var);

        }

      }

    }
  for (i = 0; i <= B_7; i++)
  if (cfg.DialPrefixes[i] == 0)
    {
    cfg.DialPrefixes[i] = DefaultPrefix;

    }
  if (cfg.Audit == 1)
  if (CleanCalllog) cfg.Audit = 2;
  initLogBuf(&logBuf);
  initRoomBuf(&roomBuf);
  initNetBuf(&netBuf);
  if (!SysDepIntegrity(&offset))  exit(100);
  RunList(&Events, EvIsDoor);
  printf("offset=%d\n", offset);
  if (offset < MAXCODE)
    {
    if (!onlyParams) init(!ReInit);
    else
      {
      CheckFloors();
      if (cfg.BoolFlags.netParticipant)
      CheckNet();

      }
    wrapup(onlyParams);

    }
  else
    {
    illegal(
    "\7codeBuf[] overflow! Recompile with larger MAXCODE or reduce ctdlCnfg.sys\7"
    );

    }
  return 0;

  }
/*
* readString()
*
* This function reads a '#<id> "<value>"  since scanf can't.
*/
void readString(char *source, char *destination, char doProc)
  {
  char string[300], last = 0;
  int  i, j;
  for (i = 0; source[i] != '"' && source[i]; i++)
  ;
  if (!source[i])
    {
    sPrintf(string, "Couldn't find beginning \" in -%s-", source);
    illegal(string);

    }
  for (j = 0, i++; source[i] &&
  (source[i] != '"' || (doProc && last == '\\'));
  i++, j++)
    {
    string[j] = source[i];
    last = source[i];

    }
  if (!source[i])
    {
    sPrintf(string, "Couldn't find ending \" in -%s-", source);
    illegal(string);

    }
  string[j] = '\0';
  strCpy(destination, string);
  if (doProc) xlatfmt(destination);

  }
/*
* isoctal() * xlatfmt()
*
* contributed by Dale Schumacher, allow embedding of formatting info a la'
* "C" style: \n, \t, etc....
*/
char isoctal( int c )
  {
  return  (char)(( c >= '0' ) && ( c <= '7' ));

  }
void xlatfmt( char *s )
  {
  register char *p, *q;
  register int i;
  for( p=q=s; *q; ++q )
    {
    if ( *q == '\\' )
    switch( *++q )
      {
      case 'n' :
      *p++ = '\n';
      break;
      case 't' :
      *p++ = '\t';
      break;
      case 'r' :
      *p++ = '\r';
      break;
      case 'f' :
      *p++ = '\f';
      break;
      default :
      if ( isoctal( *q ))
        {
        i = (( *q++ ) - '0' );
        if ( isoctal( *q ))
          {
          i <<= 3;
          i += (( *q++ ) - '0' );
          if ( isoctal( *q ))
            {
            i <<= 3;
            i += (*q -'0');

            }
          else
          --q;

          }
        else
        --q;
        *p++ = 0xFF & ((char) i);

        }
      else
      *p++ = *q;
      break;

      }
    else
    *p++ = *q;

    }
  *p = '\0';

  }
/*
* illegal()
*
* This will print out configure error message and aborts.
*/
void illegal(char *errorstring)
  {
  printf("\007\nERROR IN CONFIGURATION:\n%s\nABORTING", errorstring);
  exit(100);

  }
/*
* dGetWord()
*
* This fetches one word from current message, off disk, returns TRUE if more
* words follow, else FALSE.
*/
char dGetWord(char *dest, int lim)
  {
  char c;
  --lim;      /* play it safe */
  /* pick up any leading blanks: */
  for (c = getMsgChar();   c == ' '  &&  c && lim;   c = getMsgChar())
    {
    if (lim)
      {
      *dest++ = c;   lim--;

      }

    }
  /* step through word: */
  for (   ;  c != ' ' && c && lim;   c = getMsgChar())
    {
    if (lim)
      {
      *dest++ = c;   lim--;

      }

    }
  /* trailing blanks: */
  for (   ;   c == ' ' && c && lim;   c = getMsgChar())
    {
    if (lim)
      {
      *dest++ = c;   lim--;

      }

    }
  if (c)  unGetMsgChar(c);    /* took one too many    */
  *dest = '\0';        /* tie off string       */
  return  c;

  }
/*
* msgInit()
*
* This sets up lowId, highId, cfg.catSector and cfg.catChar, by scanning over
* ctdlmsg.sys.
*/
void msgInit()
  {
  MSG_NUMBER first, here;
  CheckPoint Cpt;
  FILE *fd;
  extern struct mBuf mFile1;
  if ((fd = fopen(CHECKPT, READ_ANY)) != NULL)
    {
    fread(&Cpt, sizeof Cpt, 1, fd);
    fclose(fd);
    if (findMessage(Cpt.loc, Cpt.ltnewest, TRUE))
      {
      do
        {
        printf("message# %s\r", msgBuf.mbId);
        while (dGetWord(msgBuf.mbtext, MAXTEXT));
        cfg.catSector   = mFile1.thisSector;
        cfg.catChar     = mFile1.thisChar;
        cfg.newest      = atol(msgBuf.mbId);
        getMessage(getMsgChar, FALSE, FALSE, TRUE);

        }
      while (atol(msgBuf.mbId) >= Cpt.ltnewest);
      cfg.oldest = atol(msgBuf.mbId);
      printf("\n\noldest=%lu\n", cfg.oldest);
      printf("newest=%lu\n", cfg.newest);
      return;

      }

    }
  startAt(msgfl, &mFile1, 0, 0);
  getMessage(getMsgChar, FALSE, FALSE, TRUE);
  /* get the ID# */
  first = atol(msgBuf.mbId);
  printf("message# %lu\n", first);
  cfg.newest = cfg.oldest = first;
  cfg.catSector   = mFile1.thisSector;
  cfg.catChar     = mFile1.thisChar;
  for (getMessage(getMsgChar, FALSE, FALSE, TRUE);
  here = atol(msgBuf.mbId), here != first;
  getMessage(getMsgChar, FALSE, FALSE, TRUE))
    {
    printf("message# %lu\r", here);
    if (strCmpU("Mail", msgBuf.mbroom) == 0) mailCount++;
    /* find highest and lowest message IDs: */
    if (here < cfg.oldest && here != 0l)
      {
      cfg.oldest = here;

      }
    if (here > cfg.newest)
      {
      cfg.newest = here;
      /* read rest of message in and remember where it ends,  */
      /* in case it turns out to be the last message    */
      /* in which case, that's where to start writing next message*/
      while (dGetWord(msgBuf.mbtext, MAXTEXT));
      cfg.catSector   = mFile1.thisSector;
      cfg.catChar     = mFile1.thisChar;

      }

    }
  printf("\n\noldest=%lu\n", cfg.oldest);
  printf("newest=%lu\n", cfg.newest);

  }
/*
* zapMsgFile()
*
* This function initializes ctdlmsg.sys.
*/
char zapMsgFile()
  {
  extern char *W_R_ANY;
  char cx, fn[80];
  if (!FirstInit)
    {
    printf("\nDestroy all current messages? ");
    cx = simpleGetch();
    if (toUpper(cx) != 'Y')   return FALSE;

    }
  if (cfg.BoolFlags.mirror) printf("Creating primary message file.\n");
  if( realZap() )
    {
    if (cfg.BoolFlags.mirror)
      {
      fclose(msgfl);
      makeSysName(fn, "ctdlmsg.sys", &cfg.msg2Area);
      if ((msgfl = fopen(fn, W_R_ANY)) == NULL)
      illegal("?Can't create the secondary message file!");
      printf("Creating secondary message file.\n");
      realZap();
      };
    };
  return TRUE;

  }
/*
* realZap()
*
* This does the work of zapMsgFile.
*/
char realZap()
  {
  int   i;
  unsigned long sect;
  /* put null message in first sector... */
  sectBuf[0]  = 0xFF; /*   \        */
  sectBuf[1]  =  '1'; /*    >  Message ID "1" MS-DOS style  */
  sectBuf[2]  = '\0'; /*   /        */
  sectBuf[3]  =  'M'; /*   \    Null messsage        */
  sectBuf[4]  = '\0'; /*   /        */
  cfg.newest = cfg.oldest = 1l;
  cfg.catSector   = 0;
  cfg.catChar     = 5;
  for (i=5;  i<MSG_SECT_SIZE;  i++) sectBuf[i] = 0;
  crypte(sectBuf, MSG_SECT_SIZE, 0);       /* encrypt      */
  if (fwrite(sectBuf, MSG_SECT_SIZE, 1, msgfl) != 1)
    {
    printf("zapMsgFil: write failed\n");
    return FALSE;
    }
  crypte(sectBuf, MSG_SECT_SIZE, 0);       /* decrypt      */
  sectBuf[0] = 0;
  crypte(sectBuf, MSG_SECT_SIZE, 0);       /* encrypt      */
  printf("\n%d sectors to be cleared\n", cfg.maxMSector);
  for (sect = 1l;  sect < cfg.maxMSector;  sect++)
    {
    printf("%07.7lu\r", sect);
    if (fwrite(sectBuf, MSG_SECT_SIZE, 1, msgfl) != 1)
      {
      printf("zapMsgFil: write failed\n");
      return FALSE;
      }

    }
  Zap_Nodes_Message_Index();               /* zap last netted message */
  crypte(sectBuf, MSG_SECT_SIZE, 0);       /* decrypt      */
  return TRUE;

  }
/*
* indexRooms()
*
* This will build a RAM index to CTDLROOM.SYS, displays stats.
*/
void indexRooms()
  {
  int  goodRoom, m, roomCount, slot, WriteIt;
  zero_struct(roomBuf.rbflags);
  zero_struct(roomBuf.rbArea);
  roomBuf.rbgen = 0;
  roomBuf.rbname[0] = 0;
  roomBuf.rbShareType = 0;
  for (m = 0; m < MSGSPERRM; m++)
    {
    roomBuf.msg[m].rbmsgNo = 0l;
    roomBuf.msg[m].rbmsgLoc = 0;

    }
  strCpy(roomBuf.rbname, "Mail");
  roomBuf.rbflags.PUBLIC =
  roomBuf.rbflags.PERMROOM =
  roomBuf.rbflags.INUSE = TRUE;
  putRoom(MAILROOM);
  roomCount = 0;
  for (slot = 0;  slot < MAXROOMS;  slot++)
    {
    getRoom(slot);
    printf("Checking room #%3d: ", slot);
    WriteIt = FALSE;
    if (roomBuf.rbflags.INUSE == 1)
      {
      WriteIt = TRUE;
      roomBuf.rbflags.INUSE = 0;    /* clear "inUse" flag */
      if (roomBuf.rbFlIndex >= (int) FloorSize)
      roomBuf.rbFlIndex = 0;
      for (m = 0, goodRoom = FALSE; m < MSGSPERRM && !goodRoom; m++)
        {
        if (roomBuf.msg[m].rbmsgNo > cfg.oldest)
          {
          goodRoom    = TRUE;

          }

        }
      if (goodRoom   || roomBuf.rbflags.PERMROOM == 1)
        {
        roomBuf.rbflags.INUSE = 1;

        }
      if (roomBuf.rbflags.INUSE == 1)
        {
        if (slot == 0)         /* Ugly kludge */
        strCpy(roomBuf.rbname, baseRoom);
        roomCount++;

        }
      else
        {
        zero_struct(roomBuf.rbflags);

        }

      }
    printf("%-20s\r",
    (roomBuf.rbflags.INUSE == 1) ? roomBuf.rbname : "<not in use>");
    if (WriteIt)
      {
      if (roomBuf.rbflags.INUSE && roomBuf.rbflags.SHARED)
      roomTab[slot].rtlastNet = findHighestNative();
      else
      roomTab[slot].rtlastNet = 0l;
      noteRoom();
      putRoom(slot);

      }

    }
  printf("\n %d of %d rooms in use\n", roomCount, MAXROOMS);

  }
/*
* findHighestNative()
*
* This finds the highest native message in a room.
*/
MSG_NUMBER findHighestNative()
  {
  int rover;
  MSG_NUMBER ourHighest;
  theMessages *temp;
  temp = (theMessages *) GetDynamic(MSG_BULK);
  copy_ptr(roomBuf.msg, temp, MSGSPERRM);
  qsort(temp, MSGSPERRM, sizeof temp[0], msgSort);
  ourHighest = 0l;
  for (rover = 0; rover < MSGSPERRM; rover++)
    {
    if (temp[rover].rbmsgNo != 0l &&
    temp[rover].rbmsgNo >= cfg.oldest &&
    temp[rover].rbmsgNo <= cfg.newest &&
    cfindMessage(temp[rover].rbmsgLoc, temp[rover].rbmsgNo))
      {
      if (strCmpU(msgBuf.mbaddr, R_SH_MARK) == SAMESTRING ||
      strCmpU(msgBuf.mbaddr, NON_LOC_NET) == SAMESTRING)
        {
        ourHighest = temp[rover].rbmsgNo;
        break;

        }

      }

    }
  free(temp);
  return ourHighest;

  }
/*
* msgSort()
*
* This function sorts messages by their native msg id.
*/
int msgSort(theMessages *s1, theMessages *s2)
  {
  if (s1->rbmsgNo < s2->rbmsgNo) return 1;
  if (s1->rbmsgNo > s2->rbmsgNo) return -1;
  return 0;

  }
/*
* noteRoom()
*
* This function will enter room into RAM index array.
*/
void noteRoom()
  {
  int   i;
  MSG_NUMBER last;
  last = 0l;
  for (i = 0;  i < MSGSPERRM;  i++)
    {
    if (roomBuf.msg[i].rbmsgNo > cfg.newest)
      {
      roomBuf.msg[i].rbmsgNo = 0l;

      }
    if (roomBuf.msg[i].rbmsgNo > last)
      {
      last = roomBuf.msg[i].rbmsgNo;

      }

    }
  roomTab[thisRoom].rtlastMessage = last     ;
  roomTab[thisRoom].rtShareType   = roomBuf.rbShareType;
  strCpy(roomTab[thisRoom].rtname, roomBuf.rbname) ;
  roomTab[thisRoom].rtgen     = roomBuf.rbgen  ;
  roomTab[thisRoom].rtFlIndex = roomBuf.rbFlIndex;
  copy_struct(roomBuf.rbflags, roomTab[thisRoom].rtflags);

  }
/*
* zapRoomFile()
*
* This function erases and re-initializes CTDLROOM.SYS.
*/
char zapRoomFile()
  {
  int i;
  char cx;
  if (!FirstInit)
    {
    printf("\nWipe room file? ");
    cx = simpleGetch();
    if (toUpper(cx) != 'Y') return FALSE;
    printf("\n");

    }
  zero_struct(roomBuf.rbflags);
  roomBuf.rbgen     = 0;
  roomBuf.rbname[0] = 0;   /* unnecessary -- but I like it...  */
  for (i = 0;  i < MSGSPERRM;  i++)
    {
    roomBuf.msg[i].rbmsgNo =  0l;
    roomBuf.msg[i].rbmsgLoc = 0 ;

    }
  printf("maxrooms=%d\n", MAXROOMS);
  for (thisRoom = 0;  thisRoom < MAXROOMS;  thisRoom++)
    {
    printf("clearing room %d\r", thisRoom);
    RoomSys(thisRoom);
    putRoom(thisRoom);
    noteRoom();

    }
  printf("\n");
  /* Lobby> always exists -- guarantees us a place to stand! */
  thisRoom      = 0      ;
  strCpy(roomBuf.rbname, baseRoom)    ;
  roomBuf.rbflags.PERMROOM = TRUE;
  roomBuf.rbflags.PUBLIC   = TRUE;
  roomBuf.rbflags.INUSE    = TRUE;
  RoomSys(0);
  putRoom(LOBBY);
  noteRoom();
  /* Mail> is also permanent...       */
  thisRoom      = MAILROOM      ;
  strCpy(roomBuf.rbname, "Mail")      ;
  RoomSys(1);
  /* Don't bother to copy flags, they remain the same (right?)    */
  putRoom(MAILROOM);
  noteRoom();
  /* Aide> also...    */
  thisRoom      = AIDEROOM      ;
  strCpy(roomBuf.rbname, "Aide")      ;
  roomBuf.rbflags.PERMROOM = TRUE;
  roomBuf.rbflags.PUBLIC   = FALSE;
  roomBuf.rbflags.INUSE    = TRUE;
  RoomSys(2);
  putRoom(AIDEROOM);
  noteRoom();
  return TRUE;

  }
/*
* logInit()
*
* This function indexes ctdllog.sys.
*/
void logInit()
  {
  int i;
  int count = 0;
  #ifdef IS_RIGHT
  if (rewind(logfl) != 0) illegal("Rewinding logfl failed!");
  #else
  rewind(logfl);
  #endif
  /* clear logTab */
  for (i = 0; i < cfg.MAXLOGTAB; i++) logTab[i].ltnewest = 0l;
  /* load logTab: */
  for (thisLog = 0;  thisLog < cfg.MAXLOGTAB;  thisLog++)
    {
    printf("log #%04d", thisLog);
    getLog(&logBuf, thisLog);
    /* count valid entries:      */
    if (logBuf.lbflags.L_INUSE == 1)
      {
      count++;
      printf("   %20s", logBuf.lbname);

      }
    else printf("   <not in use>       ");
    printf("\r");
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
  printf("\n logInit--%d valid log entries\n", count);
  printf("sort Log...\n");
  qsort(logTab, cfg.MAXLOGTAB, cfg.sizeLTentry, logSort);

  }
/*
* logSort()
*
* This function Sorts 2 entries in logTab.
*/
int logSort(LogTable *s1, LogTable *s2)
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
* noteLog()
*
* This notes a logTab entry in RAM buffer in master index.
*/
void noteLog()
  {
  int i, slot;
  /* figure out who it belongs between: */
  for (i = 0;  logTab[i].ltnewest > logBuf.lbvisit[0];  i++);
  /* note location and open it up:  */
  slot = i;
  slideLTab(slot, cfg.MAXLOGTAB-1);
  /* insert new record */
  logTab[slot].ltnewest       = logBuf.lbvisit[0]  ;
  logTab[slot].ltlogSlot      = thisLog     ;
  logTab[slot].ltpwhash       = hash(logBuf.lbpw)  ;
  logTab[slot].ltnmhash       = hash(logBuf.lbname);

  }
/*
* slideLTab()
*
* This function slides bottom N slots in logTab down.  For sorting.
*/
void slideLTab(int slot, int last)
  {
  int i;
  /* open slot up: (movmem isn't guaranteed on overlaps) */
  for (i = last - 1;  i >= slot;  i--)
    {
    movmem(&logTab[i], &logTab[i + 1], cfg.sizeLTentry);

    }

  }
/*
* wrapup()
*
* This finishes up and writes ctdlTabl.sys out, finally.
*/
void wrapup(char onlyParams)
  {
  printf("\ncreating ctdlTabl.sys table\n");
  if (!onlyParams)
    {
    if (!msgZap)  msgInit();
    if (!roomZap) indexRooms();
    cfg.weAre = CITADEL;
    if (!logZap)  logInit();
    netInit();
    if (mailCount)
    printf("%d of the messages were Mail\n", mailCount);

    }
  EventTab = (EVENT *) GetDynamic(cfg.EvNumber * sizeof *EventTab);
  if (!FinalSystemCheck(onlyParams)) exit(2);
  if (!onlyParams)
    {
    if (cfg.BoolFlags.netParticipant)
    fclose(netfl);
    fclose(roomfl);
    fclose(msgfl);
    fclose(logfl);

    }
  RunList(&Events, EventWrite); /* because we keep this in a linked list */
  printf("writeSysTab = %d\n", writeSysTab());

  }
/*
* zapLogFile()
*
* This erases & re-initializes ctdllog.sys.
*/
char zapLogFile()
  {
  int  i;
  char cx;
  if (!FirstInit)
    {
    printf("\nWipe out log file? ");
    cx = simpleGetch();
    if (toUpper(cx) != 'Y')   return FALSE;
    printf("\n");

    }
  /* clear RAM buffer out:      */
  logBuf.lbflags.L_INUSE = FALSE;
  for (i = 0;  i < MAILSLOTS;  i++)
    {
    logBuf.lbMail[i].rbmsgLoc = 0l;
    logBuf.lbMail[i].rbmsgNo  = 0l;

    }
  for (i = 0;  i < NAMESIZE;  i++)
    {
    logBuf.lbname[i] = 0;
    logBuf.lbpw[i]   = 0;

    }
  /* write empty buffer all over file;  */
  for (i = 0; i < cfg.MAXLOGTAB;  i++)
    {
    printf("Clearing log #%d\r", i);
    putLog(&logBuf, i);
    logTab[i].ltnewest = logBuf.lbvisit[0];
    logTab[i].ltlogSlot= i;
    logTab[i].ltnmhash = hash(logBuf.lbname);
    logTab[i].ltpwhash = hash(logBuf.lbpw  );

    }
  return TRUE;

  }
/*
* netInit()
*
* This will Initialize RAM index for Ctdlnet.sys.
*/
void netInit()
  {
  label temp;
  int i = 0;
  long length;
  if (!cfg.BoolFlags.netParticipant) return;
  totalBytes(&length, netfl);
  cfg.netSize = (int) (length / NB_TOTAL_SIZE);
  if (cfg.netSize)
  netTab = (NetTable *) GetDynamic(sizeof (*netTab) * cfg.netSize);
  else
  netTab = NULL;
  while (i < cfg.netSize)
    {
    getNet(i, &netBuf);
    netTab[i].netTRooms = (SharedRoom *)
    GetDynamic(sizeof (*netBuf.netRooms) * SHARED_ROOMS);
    normId(netBuf.netId, temp);
    netTab[i].ntnmhash = hash(netBuf.netName);
    netTab[i].ntidhash = hash(temp);
    strCpy(netTab[i].ntShort, netBuf.nbShort);
    copy_struct(netBuf.nbflags, netTab[i].ntflags);
    movmem(netBuf.netRooms, netTab[i].netTRooms,
    sizeof *netBuf.netRooms * SHARED_ROOMS);
    netTab[i].ntMemberNets = netBuf.MemberNets;
    printf("System %3d. %s\n", i,
    (netBuf.nbflags.in_use) ? netBuf.netName : "<not in use>");
    i++;

    }
  if (cfg.MailHub != 0)
    {
    if (searchNameNet(cfg.MailHub + cfg.codeBuf, &netBuf) == -1)
    illegal("Your #MailHub node was not found in your CtdlNet.Sys.");
    if (netBuf.nbRoute != -1 &&
    netTab[netBuf.nbRoute].ntGen == netBuf.nbRouteGen)
    illegal("You must have a direct connect to your #MailHub.");

    }

  }
/*
* strCmpU()
*
* This is strcmp(), but ignoring case distinctions.  Found in most C libraries
* as stricmp().  Someday we should switch over.
*/
int strCmpU(char s[], char t[])
  {
  int  i;
  i = 0;
  while (toUpper(s[i]) == toUpper(t[i]))
    {
    if (s[i++] == '\0')  return SAMESTRING ;

    }
  return  toUpper(s[i]) - toUpper(t[i]);

  }
/*
* crashout()
*
* This handles a fatal error, for library functions.
*/
void crashout(char *str)
  {
  illegal(str);

  }
/*
* cfindMessage()
*
* This function gets all set up to do something with a message.
*/
char cfindMessage(SECTOR_ID loc, MSG_NUMBER id)
  {
  long atol();
  MSG_NUMBER here;
  extern struct mBuf mFile1;
  startAt(msgfl, &mFile1, loc, 0);
  do
    {
    getMessage(getMsgChar, FALSE, FALSE, TRUE);
    here = atol(msgBuf.mbId);

    }
  while (here != id &&  mFile1.thisSector == loc);
  return (char)((here == id));

  }
/*
* CheckFloors()
*
* This function will check the floors.
*/
void CheckFloors()
  {
  SYS_FILE     tempName;
  FILE   *flrfl;
  struct floor FloorBuf;
  struct floor *fptr;
  extern char  *R_W_ANY, *WRITE_ANY;
  int rover, cindex;
  int flag;
  char *cptr;

  makeSysName(tempName, "ctdlflr.sys", &cfg.floorArea);

  if ((flrfl = fopen(tempName, R_W_ANY)) == NULL)
    {
    printf(" %s not found, creating new file.\n", tempName);
    if ((flrfl = fopen(tempName, WRITE_ANY)) == NULL)
    illegal("?Can't create the floor file!");

    }
  /** create initial floor entry **/
  strCpy(FloorBuf.FlName, cfg.codeBuf + cfg.MainFloor);
  FloorBuf.FlInuse = TRUE;
  memset(FloorBuf.FlModerator, '\0',sizeof(label));
  /** write floor creation entry **/
  if( fwrite(&FloorBuf, sizeof FloorBuf, 1, flrfl) != 1)
    {
    printf("Write Error: %s\n",tempName);
    };
  totalBytes(&FloorSize, flrfl);
  FloorSize /= sizeof FloorBuf;
  printf("Current Floor file size is %ld entries\n",FloorSize);
  fclose(flrfl);
  /*  strip unused records  */
  if ((flrfl = fopen(tempName, READ_ANY)) == NULL)
    {
    illegal("?Can't read the floor file!");
    };
  fptr = (struct floor *)malloc(FloorSize * sizeof(struct floor) );
  /* get buffer for whole file */
  if( fptr == NULL )
    {
    free(fptr);
    illegal(" cannot allocate memory for a floor file!");
    };
  /* read whole file */
  if( fread( fptr, sizeof(struct floor), FloorSize, flrfl) != FloorSize )
    {
    free(fptr);
    illegal("Unable to read floor file!");
    };
  fclose(flrfl);
  /* Now, we know that there is at least one good record so we
     check from the end until we find a record in use
  */
  flag = TRUE;
  for( rover = 0; rover < FloorSize && fptr[rover].FlInuse  == TRUE; rover++)
    {
    /* validate that the Name are alpha characters and
       printable. Also test to see if In use.
    */
    cptr = &fptr[rover].FlModerator[0];
    if( ( *cptr >= 'a' && *cptr <= 'z' ) || ( *cptr >= 'A' && *cptr <= 'Z' ) )
      {
      printf("%20s is moderator for %s\n",fptr[rover].FlModerator,fptr[rover].FlName);
      }
    else
      {
      for( cindex = 0; cindex <20 ; cindex++)
        {
        cptr[cindex] = '\0';
        };
      };
    };
  FloorSize = rover;
  printf("New Floor file size is %ld entries\n",FloorSize);
  if ((flrfl = fopen(tempName, W_R_ANY)) == NULL)
    {
    free(fptr);
    illegal("?Can't Open the floor file to re-write!");
    };
  if( fwrite(fptr, sizeof(struct floor), FloorSize, flrfl) != FloorSize)
    {
    printf("Write Error: %s\n",tempName);
    };
  free(fptr);
  fclose(flrfl);
  }
/*
* CheckNet()
*
* This creates the Ctdlnet.sys if needed.
*/
void CheckNet()
  {
  SYS_FILE      tempName;
  makeSysName(tempName, "ctdlnet.sys", &cfg.netArea);
  if ((netfl = fopen(tempName, READ_ANY)) == NULL)
    {
    printf(" %s not found, creating new file.\n", tempName);
    if ((netfl = fopen(tempName, WRITE_ANY)) == NULL)
    illegal("?Can't create the net file!");

    }

  }
/*
* NetParse()
*
* This function is responsible for parsing the network parameters.
*/
static char NetParse(char *line, int *offset, char *var, int arg, int SetVal)
  {
  char temp[100];
  if (strCmpU(var, "#NewNetPrivs")    == SAMESTRING)
    {
    cfg.BoolFlags.NetDft      = arg;
    return TRUE;

    }
  else if (strCmpU(var, "#RouteMail")     == SAMESTRING)
    {
    cfg.BoolFlags.RouteMail = arg;
    return TRUE;

    }
  else if (strCmpU(var, "#SCAN-NET-MESSAGES") == SAMESTRING)
    {
    cfg.BoolFlags.NetScanBad = TRUE;
    return TRUE;

    }
  else if (strCmpU(var, "#SHARED-ROOMS")  == SAMESTRING)
    {
    if (SetVal)
      {
      if (SHARED_ROOMS != arg && cfg.netSize != 0)
      illegal(
      "SHARED-ROOMS parameter does not equal old value!");
      else
      SHARED_ROOMS = arg;

      }
    else
      {
      SHARED_ROOMS = arg;

      }
    return TRUE;

    }
  else if (strCmpU(var, "#NETWORK"   )    == SAMESTRING)
    {
    cfg.BoolFlags.netParticipant = arg;
    return TRUE;
    #ifdef NEED_LONGHAUL

    }
  else if (strCmpU(var, "#LONG-HAUL" )    == SAMESTRING)
    {
    cfg.BoolFlags.longHaul = arg;
    return TRUE;
    #endif

    }
  else if (strCmpU(var, "#NET_AREA_SIZE") == SAMESTRING)
    {
    cfg.sizeArea = arg;
    return TRUE;

    }
  else if (strCmpU(var, "#MAX_NET_FILE")    == SAMESTRING)
    {
    cfg.maxFileSize = arg;
    return TRUE;

    }
  else if (strCmpU(var, "#callOutSuffix") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.netSuffix = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#callOutPrefix") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    DefaultPrefix = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut300") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[ONLY_300] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut1200") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[BOTH_300_1200] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut2400") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[TH_3_12_24] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut4800") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[B_4] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut9600") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[B_5] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut14400") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[B_6] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DialOut19200") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], TRUE);
    cfg.DialPrefixes[B_7] = *offset;
    while (cfg.codeBuf[*offset])
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#nodeName" ) == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], FALSE);
    NormStr(&cfg.codeBuf[*offset]);
    if (strLen(&cfg.codeBuf[*offset]) > 19)
    illegal("nodeName too long; must be less than 20");
    if (strchr(&cfg.codeBuf[*offset], '_') != NULL ||
    strchr(&cfg.codeBuf[*offset], '.') != NULL)
    illegal("The characters '.' and '_' are illegal in node names!");
    cfg.nodeName    = *offset;
    while (cfg.codeBuf[*offset]) /* step over string     */
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#nodeId"   ) == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], FALSE);
    if (strLen(&cfg.codeBuf[*offset]) > 19)
    illegal("nodeId too long; must be less than 20");
    cfg.nodeId      = *offset;
    while (cfg.codeBuf[*offset]) /* step over string     */
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#MailHub"  ) == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], FALSE);
    if (strLen(&cfg.codeBuf[*offset]) > 19)
    illegal("MailHub too long; must be less than 20");
    cfg.MailHub      = *offset;
    while (cfg.codeBuf[*offset]) /* step over string     */
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#DomainDisplay") == SAMESTRING)
    {
    readString(line, cfg.DomainDisplay, FALSE);
    if (strLen(cfg.DomainDisplay) >= sizeof cfg.DomainDisplay)
    illegal("DomainDisplay is too long, must be less than 11");
    return TRUE;

    }
  else if (strCmpU(var, "#nodeDomain") == SAMESTRING)
    {
    readString(line, &cfg.codeBuf[*offset], FALSE);
    if (strLen(&cfg.codeBuf[*offset]) > 19)
    illegal("nodeDomain too long; must be less than 20");
    if (strchr(cfg.codeBuf + *offset, '_') != NULL ||
    strchr(cfg.codeBuf + *offset, '.') != NULL)
    illegal("Domain names cannot have '_' or '.' in them.");
    cfg.nodeDomain = *offset;
    while (cfg.codeBuf[*offset]) /* step over string     */
    (*offset)++;
    (*offset)++;
    return TRUE;

    }
  else if (strCmpU(var, "#ServeDomain") == SAMESTRING)
    {
    readString(line, temp, FALSE);
    if (strLen(temp) > 19)
    illegal("ServeDomain value is too long, must be less than 19 characters.");
    AddData(&Serves, strdup(temp), NULL, FALSE);
    cfg.DomainHandlers++;
    return TRUE;

    }
  return FALSE;

  }

void  Zap_Nodes_Message_Index()  /* zap last netted message */
  {
  int node, slot;
  for(node = 0; node < cfg.netSize; node++)
    {
    if( !netTab[node].ntflags.in_use )continue;
    getNet(node,&netBuf);
    for( slot=0; slot < SHARED_ROOMS; slot++)
      {
      if( !roomTab[netTabRoomSlot(node, slot)].rtflags.SHARED)continue;
      netTab[node].netTRooms[slot].lastMess = 0;
      netBuf.netRooms[slot].lastMess = 0;
      };
    putNet(node,&netBuf);
    };

  }
