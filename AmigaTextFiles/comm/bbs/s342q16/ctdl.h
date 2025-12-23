/*
*       Ctdl.h
*
* #include file for all Citadel C files.
* Now includes only #defines and structs.
*/
#include "stdio.h"
#include "slist.h"
#include "sysdep.h"
#define CTDL_HEADER     1
/*
*       History
*
* 85Oct16 HAW  Add code for OFFICE-STUFF parameter.
* 85Aug29 HAW  Install code to allow double msg files for autobackup.
* 85Jun19 HAW  Implant exit values so batch files can be made useful.
* 85May27 HAW  Start adding networking gunk.
* 85May22 HAW  MAXLOGTAB now sysop selectable.
* 85May06 HAW  Add daily bailout parameter.
* 85May05 HAW  Add SYSDISK parameter.
* 85Mar20 HAW  Add timestamp code.
* 85Feb21 HAW  Add directory names.
* 85Feb20 HAW  Implement IMPERVIOUS flag.
* 85Feb18 HAW  Insert global variables for baud search.
* 85Jan20 HAW  Insert code to read from system clock.
* 84Aug30 HAW  Begin conversion to MS-DOS.
*/
#define NAMESIZE       20       /* length of room names                 */
#define SECTSIZE      128       /* Size of a sector (XMODEM)            */
#define YM_BLOCK_SIZE 1024      /* Size of a sector (YMODEM)            */
typedef char label[NAMESIZE];   /* Semi-generic                         */
/*
* Citadel programs use readSysTab() and writeSysTab() to write an
* image of the external variables in RAM to disk, and later restore
* it.  The image is stored in ctdlTabl.sys .  If ctdlTabl.sys is lost,
* confg.com will automatically reconstruct the hard way when invoked,
* and write a new ctdlTabl.sys out when finished.  CtdlTabl.sys is
* always destroyed after reading, to minimize the possibility of
* reading an out-of-date version.  In general, the technique works
* well and saves time and head-banging on bootup.  You should,
* however, note carefully the following caution:
*  o  Whenever you change the declarations in Ctdl.h you should:
*   -->  destroy the current ctdlTabl.sys file
*   -->  recompile and reload all citadel programs which access
*        ctdlTabl.sys -- currently citadel.com & configur.com
*   -->  use configur.com to build a new ctdlTabl.sys file
*
* If you ignore these warnings, little pixies will prick you in your
* sleep for the rest of your life.
*/
struct MiscBool
  {
  BOOLEAN_FLAG(HoldOnLost);   /* Save entries when carrier lost?      */
  BOOLEAN_FLAG(mirror);       /* mirror msg option?                   */
  BOOLEAN_FLAG(unlogEnterOk); /* TRUE if OK to enter messages anon    */
  BOOLEAN_FLAG(unlogReadOk);  /* TRUE if unlogged folks can read mess */
  BOOLEAN_FLAG(unlogLoginOk); /* TRUE if spontan. new accounts ok.    */
  BOOLEAN_FLAG(nonAideRoomOk);/* TRUE general folks can make rooms    */
  BOOLEAN_FLAG(noMail);       /* TRUE if mail is not allowed          */
  BOOLEAN_FLAG(noChat);       /* TRUE if not accepting chats          */

  BOOLEAN_FLAG(netParticipant);/* TRUE if participating in the net    */
  BOOLEAN_FLAG(aideSeeAll);   /* TRUE if aides see private rooms      */
  BOOLEAN_FLAG(debug);        /* TRUE for debug phase                 */
  BOOLEAN_FLAG(NetDft);       /* TRUE if new users get net privs      */
  BOOLEAN_FLAG(SysopEditor);  /* TRUE if there is a sysop editor      */
  BOOLEAN_FLAG(IsDoor);       /* TRUE if this installation is a door  */
  BOOLEAN_FLAG(RouteMail);    /* TRUE if we will route mail           */
  BOOLEAN_FLAG(DoorDft);      /* TRUE if new users get door privs     */

  BOOLEAN_FLAG(AnonSessions); /* TRUE if anonymous calls are recorded */
  BOOLEAN_FLAG(DL_Default);   /* TRUE if new callers get dl privs     */
  BOOLEAN_FLAG(NetScanBad);   /* TRUE if net msgs scanned for bad words*/
  BOOLEAN_FLAG(QwkMail);      /* TRUE if we support qwk packets        */
  };
/* Let's begin by defining the configuration struct.    */
/* This is part of the contents of ctdltabl.sys         */
typedef struct
  {
  SECTOR_ID maxMSector; /* Max # of sectors (simulated)         */
  MSG_NUMBER oldest;    /* 32-bit ID# of first message in system*/
  MSG_NUMBER newest;    /* 32-bit ID# of last  message in system*/
  UNS_16  nodeName;   /* Offsets in codeBuf                   */
  UNS_16  nodeTitle;
  UNS_16  nodeId;
  UNS_16  nodeDomain;   /* home domain of this installation */
  UNS_16  bRoom;
  UNS_16  MainFloor;    /* Main floor name                      */
  UNS_16  DialPrefixes[7], netSuffix;
  NET_AREA receptArea;        /* Area to accept files sent via net here */
  int  sizeArea;              /* How much room to allow for same (K)  */
  int  maxFileSize;           /* In K                                 */
  label SysopName;
  int  cryptSeed;
  UNS_16 InitColumns;
  UNS_16 LoginAttempts;
  char Audit;                 /* 0=none, 1=normal, 2=no net sessions  */
  DependentData DepData;
  char filter[128];           /* input character translation table    */
  SYS_AREA homeArea,    /* Location: Help files                 */
  msgArea,    /* Message file                         */
  msg2Area,   /* Mirror message file                  */
  logArea,    /* Log file                             */
  roomArea,   /* Room file                            */
  netArea,    /* Net files                            */
  domainArea, /* Domain directories                   */
  auditArea,  /* General auditing                     */
  floorArea,  /* The floor file                       */
  holdArea,   /* Held messages (lost carrier)         */
  tempArea,   /* all temporary files here             */
  bioArea,    /* Biographies here                     */
  bannerArea, /* all banner type files here           */
  QwkWorkArea,/* Current working files: Qwk Mail(unused) */
  QwkName,    /* Current Qwk Packet name              */
  QwkLocation,/* Current Qwk Packet Town name         */
  QwkFileArea;/* Current Qwk Files area               */
  char sysBaud;   /* What's our baud rate going to be?    */
  UNS_16  QwkMaxRooms; /* maximum number of rooms collectable in one packet*/
  UNS_16  QwkMaxPacket;/* maximum number of messages in one packet */
  UNS_16  netSize;    /* How many on the net?                 */
  UNS_16  DomainHandlers; /* How many domains do we service?  */
  UNS_16  MailHub;    /* Who's our mail hub?      */
  UNS_16  MailHubGen;   /*          */
  char    DomainDisplay[11];  /* customizable display of domain names */
  UNS_16 EvNumber;
  /*      stuff to distinguish the various Citadel programs               */
  #define CITADEL         0       /* principal program                    */
  #define xxxxx           1       /* unused                               */
  #define NET             2       /* network downloader                   */
  #define ARCHIVER        3       /* backup program       (future)        */
  #define CONFIGUR        4
  #define UTILITY         5
  char    weAre;              /* set first thing by main()            */
  UNS_16     paramVers;
  /*                      Stuff to size system with:                      */
  /* WARNING!!! if you expand MAXROOMS beyond 128, horrible, horrible     */
  /* things will happen to the userlog entries, which will no longer      */
  /* fit in the assigned 256-byte records and will overwrite things       */
  /* with normally undesirable results. 82Nov10CrT                        */
  UNS_16  MAXLOGTAB,  /* number of log entries supported      */
  MailSlots,
  MsgsPerrm,
  MaxRooms,
  SharedRooms;
  /*              Stuff nowadays usually in bdscio.h:                     */
  /* values for functions to return: */
  #define TRUE            1
  #define FALSE           0
  #define ERROR          -1
  #define SAMESTRING      0       /* value for strcmp() & friend          */
  #define PTR_SIZE        (sizeof (char *))         /* could cause problems */
  /*                      Stuff for rooms:                                */
  #define LOBBY           0       /* Lobby> is >always< room 0.           */
  #define MAILROOM        1       /* Mail>  is >always< room 1.           */
  #define AIDEROOM        2       /* Aide> is >always< room 2.            */
  #define MAXCODE       800
  unsigned char codeBuf[MAXCODE];/* buffer for configuration routines */
  unsigned char scratch[10];  /* scratch space for config routines    */
  AN_UNSIGNED shave[8];       /* shave-and-a-haircut/2 bits pauses    */
  UNS_16  catChar;    /* Location of next write in msg file   */
  SECTOR_ID catSector;
  UNS_16  AnonMailLength; /* anonymous mail max length    */
  UNS_16  sizeLTentry;  /* contains size of a logTab entry      */
  UNS_16  ConTimeOut;   /* seconds in CONSOLE mode before timeout */
  char sysPassword[100];      /* Remote sysop                         */
  char SysopArchive[40];  /* where to archive sysop mail    */
  struct MiscBool BoolFlags;  /* Buncha flags                         */

  }
CONFIG;            /* And that's all of the variables we want to save */
/*
*       Room data
*/
#define MAXGEN    32       /* five bits of generation=>32 of them */
#define FORGET_OFFSET (MAXGEN / 2)     /* For forgetting rooms  */
#define RO_OFFSET ((MAXGEN / 2) + 4) /* For r/o room write priv */
/* these define what knowRoom() return */
#define UNKNOWN_ROOM  0 /* user does not know of room.    */
#define KNOW_ROOM 1 /* user knows room.     */
#define FORGOTTEN_ROOM  2 /* user has forgotten room.   */
#define WRITE_PRIVS 3 /* user has write privs in r/o room */
#define DEAD_ROOM 4 /* user is looking at dead room   */
#define RO  1   /* temporary for some ifdefs */
#define UN_STACK        40      /* stack of rooms */
#define MSG_BULK        (MSGSPERRM * sizeof (theMessages))
#define RB_SIZE         (sizeof(roomBuf) - (PTR_SIZE * 1))
#define RB_TOTAL_SIZE   (RB_SIZE + MSG_BULK)
struct rflags
  {
  /* Room flags                           */
  BOOLEAN_FLAG(INUSE);        /* Room in use?                         */
  BOOLEAN_FLAG(PUBLIC);       /* Room public?                         */
  BOOLEAN_FLAG(ISDIR);        /* Room directory?                      */
  BOOLEAN_FLAG(PERMROOM);     /* Room permanent?                      */
  BOOLEAN_FLAG(SKIP);         /* Room skipped? (temporary for user)   */
  BOOLEAN_FLAG(UPLOAD);       /* Can room be uploaded to?             */
  BOOLEAN_FLAG(DOWNLOAD);     /* Can room be downloaded from?         */
  BOOLEAN_FLAG(SHARED);       /* Is this a shared room?               */
  BOOLEAN_FLAG(ARCHIVE);      /* Is this room archived somewhere?     */
  BOOLEAN_FLAG(ANON);         /* All messages anonymous?              */
  BOOLEAN_FLAG(NO_NET_DOWNLOAD); /* Accessible via the net for download? */
  BOOLEAN_FLAG(INVITE);
  BOOLEAN_FLAG(AUTO_NET);
  BOOLEAN_FLAG(ALL_NET);
  BOOLEAN_FLAG(READ_ONLY);   /* room is read-only */
  BOOLEAN_FLAG(rflag9);
  BOOLEAN_FLAG(rflag10);
  BOOLEAN_FLAG(rflag11);
  BOOLEAN_FLAG(rflag12);
  BOOLEAN_FLAG(rflag13);
  BOOLEAN_FLAG(rflag14);
  BOOLEAN_FLAG(rflag15);
  BOOLEAN_FLAG(rflag16);
  BOOLEAN_FLAG(rflag17);

  };
typedef struct
  {
  /* The summation of a room              */
  AN_UNSIGNED   rtgen;        /* generation # of room                 */
  struct rflags rtflags;      /* public/private flag etc              */
  label         rtname;       /* name of room                         */
  MSG_NUMBER    rtlastMessage;/* # of most recent message in room     */
  MSG_NUMBER    rtlastNet;    /* Highest outgoing net message         */
  char          rtShareType;  /* What type of sharing for this room?  */
  int           rtFlIndex;    /* Index into the floors                */

  }
rTable ;                      /* And see ROOMA.C for declaration      */
typedef struct
  {
  MSG_NUMBER rbmsgNo;     /* every message gets unique#           */
  SECTOR_ID rbmsgLoc;     /* sector message starts in             */

  }
theMessages;
typedef struct
  {
  /* The appearance of a room:            */
  AN_UNSIGNED   rbgen;  /* generation # of room                 */
  struct rflags rbflags;  /* same bits as flags above             */
  label         rbname; /* name of room                         */
  ROOM_AREA     rbArea; /* area this room is attached to        */
  char          rbShareType;  /* room share type                      */
  UNS_16        rbFlIndex;  /* index into the floors                */
  theMessages *msg;

  }
aRoom ;
/*
*       userlog stuff
*/
#define CRYPTADD        117    /*                                      */
#define LB_SIZE         (sizeof (logBuf) - (PTR_SIZE * 2))
#define MAIL_BULK       (MAILSLOTS * sizeof (theMessages))
#define GEN_BULK        (MAXROOMS * sizeof (AN_UNSIGNED))
#define LB_TOTAL_SIZE   (LB_SIZE + MAIL_BULK + GEN_BULK)
struct lflags
  {
  /* Flags for person in log              */
  BOOLEAN_FLAG(FLOORS);       /* Unused for now                       */
  BOOLEAN_FLAG(LFMASK);       /* Linefeeds?                           */
  BOOLEAN_FLAG(EXPERT);       /* Expert?                              */
  BOOLEAN_FLAG(AIDE);         /* Vice-Grand-Poobah?                   */
  BOOLEAN_FLAG(L_INUSE);      /* Is this slot in use?                 */
  BOOLEAN_FLAG(TIME);         /* Send time to user of msg creation?   */
  BOOLEAN_FLAG(OLDTOO);       /* Print out last oldmessage on <N>ew?  */
  BOOLEAN_FLAG(NET_PRIVS);    /* User have net privileges?            */
  BOOLEAN_FLAG(RUGGIE);       /* Juvenile? Future fun-ness maybe      */
  BOOLEAN_FLAG(HALF_DUP);     /* half duplex?                   */
  BOOLEAN_FLAG(TWIT);         /* twit?                          */
  BOOLEAN_FLAG(DOOR_PRIVS);   /* door privies?                  */
  BOOLEAN_FLAG(PERMANENT);    /* permanent account?             */
  BOOLEAN_FLAG(DL_PRIVS);     /* sigh                           */
  BOOLEAN_FLAG(ALT_RE);       /* alternative (old-style) .RE    */
  BOOLEAN_FLAG(NoPrompt);     /* message entry prompt           */
  BOOLEAN_FLAG(ANSI);         /* Flag: Ansii enabled:1          */
  BOOLEAN_FLAG(MSG_PAUSE);    /* Flag: Pause between msgs:1     */
  BOOLEAN_FLAG(lflag5);
  BOOLEAN_FLAG(lflag6);
  BOOLEAN_FLAG(lflag7);
  BOOLEAN_FLAG(lflag8);
  BOOLEAN_FLAG(lflag9);

  };
#define MAXVISIT        8       /* #visits we remember old newestLo for */
#define GENSHIFT        3       /* Where the generation # is            */
#define CALLMASK        7       /* For finding last visit               */
typedef struct
  {
  /* The appearance of a user:            */
  AN_UNSIGNED   lbnulls;      /* #nulls, lCase, lFeeds                */
  struct lflags lbflags;      /* LFMASK, EXPERT, AIDE, INUSE, etc.    */
  AN_UNSIGNED   lbwidth;      /* terminal width                       */
  int           credit;       /* Credit for long distance calls       */
  label         lbname;       /* caller's name                        */
  label         lbpw;         /* caller's password                    */
  MSG_NUMBER    lbvisit[MAXVISIT];/* newestLo for this and 7 prev. visits */
  long          lblaston;     /* seconds since arbitrary date         */
  AN_UNSIGNED   lbdelay;  /* milliseconds delay     */
  AN_UNSIGNED   *lbgen;/* 5 bits gen, 3 bits lastvisit       */
  theMessages *lbMail;

  }
logBuffer ;
typedef struct
  {
  /* Summation of a person:               */
  UNS_16   ltpwhash;    /* hash of password                     */
  UNS_16   ltnmhash;    /* hash of name                         */
  UNS_16   ltlogSlot;   /* location in userlog.buf              */
  MSG_NUMBER ltnewest;        /* last message on last call            */
  char  ltpermanent;    /* permanent account?     */

  }
LogTable ;                    /* And see LOG.C for declaration        */
/* this is a mail forwarding structure.  Managed by slist, on   */
/* disk it's known as ctdlfwd.sys.  */
typedef struct
  {
  char *UserName;
  char *System;
  char *Alias;

  }
ForwardMail;
/*
*     terminal stuff
*/
#define SPECIAL         27      /* <ESC>        console escape char     */
#define CON_NEXT        20      /* ^T           console request char    */
typedef struct
  {
  char *unambig;              /* name of the file */
  char FileDate[8];           /* yymmmdd<0>   */
  long FileSize;              /* size of file */

  }
DirEntry;
/*
*   List handling structures - specific.
*   See SLIST.C for generic handling functionality
*/
/*
* This structure is used to implement the archival lists.  Each element of
* this sort of list contains two things:
* o The number of the room it is associated with.  There should never be more
*   than one instance of this number in the list.  We should probably attempt
*   to cull out duplicates.  This will be dependent on the behavior of the
*   old LIBARCH code.
* o The name of the file to archive to.
*/
typedef struct
  {
  UNS_16 num;
  UNS_16 num2;
  char *string;

  }
NumToString;
#define CC_SIZE         140
#define HasCC(x)        ((x)->mbCC.start != NULL)
#define HasOverrides(x) ((x)->mbOverride.start != NULL)
#define SCREEN          0
#define MSGBASE         1
#define TEXTFILE        2
/* this is useful in events and other places */
typedef struct
  {
  UNS_16 first;
  long second;

  }
TwoNumbers;
/*
*     message stuff
*/
#define MAXTEXT         7500    /* maximum chars in edit buffer         */
#define MAXWORD         256     /* maximum length of a word             */
#define IDIOT_TRIGGER   8       /* Idiot trigger                        */
#define HELD 3
/* output identifications */
#define WHATEVER  0 /* Everything except what we list after */
#define MSGS    1 /* Msg output       */
#define DL_MSGS   2 /* Download messages      */
#define STATIC_MSG_SIZE (sizeof msgBuf - (sizeof msgBuf.mbCC + sizeof msgBuf.mbOverride + sizeof msgBuf.mbtext + sizeof msgBuf.mbInternal + sizeof msgBuf.mbForeign))
#define MoveMsgBuffer(x, y)     memcpy(x, y,(long)(sizeof *x) - PTR_SIZE),(y)->mbForeign.start = (y)->mbCC.start = (y)->mbOverride.start = NULL,strCpy((x)->mbtext, (y)->mbtext);
#define O_NET_PATH_SIZE         100
typedef struct
  {
  /* This is what a msg looks like        */
  int  mbheadChar       ;     /* start of message                     */
  SECTOR_ID     mbheadSector; /* start of message                     */
  char  mbauth[129];    /* name of author                       */
  label mbdate ;    /* creation date                        */
  label mbtime ;    /* creation time                        */
  label mbId   ;    /* local number of message              */
  label mboname;    /* short human name for origin system   */
  label mborig ;    /* US xxx xxx xxxx style ID             */
  label mbroom ;    /* creation room                        */
  label mbsrcId;    /* message ID on system of origin       */
  char  mbto[129];    /* private message to                   */
  char  mbaddr[(NAMESIZE * 2) + 10];/* address of system for net routing    */
  char  mbOther[O_NET_PATH_SIZE];/* OtherNet address                  */
  label mbreply;    /* reply pointer -- Mail only   */
  label mbdomain;   /* home domain of message   */
  SListBase mbCC;   /* lists of CC type people              */
  SListBase mbOverride; /* for overriding the mbto field        */
  SListBase mbInternal; /* for overriding the mbto field        */
  SListBase mbForeign;  /* list of foreign fields   */
  char  *mbtext;    /* buffer text is edited in             */

  }
MessageBuffer;
/* values for showMess routine */
#define NEWoNLY         0
#define OLDaNDnEW       1
#define OLDoNLY         2
#define GLOBALnEW       3
#define PHRASE_SIZE     50
typedef struct
  {
  SListBase Users;
  /* char  System[(2 * NAMESIZE) + 10]; */
  char  Phrase[PHRASE_SIZE];
  long  Date;
  char  LocalOnly;

  }
OptValues;
/* definitions for determination of net message display */
#define ALL_MESSAGES    0
#define LOCAL_ONLY      1
struct mBuf
  {
  DATA_BLOCK    sectBuf;
  int           thisChar;
  SECTOR_ID     thisSector;
  int           oldChar;
  SECTOR_ID     oldSector;

  };
typedef struct
  {
  MSG_NUMBER ltnewest;        /* last message on last call            */
  SECTOR_ID  loc;

  }
CheckPoint;
#define CHECKPT   "chkpt"
/*
*     modem stuff
*/
#define NEWCARRIER   0x01       /* returned to main prog on login       */
#define CPT_SIGNAL      18      /* ^R                                   */
#define MODEM           0       /* current user of system is            */
#define CONSOLE         1       /* one of these                         */
/*  output XON/XOFF etc flag... */
#define OUTOK           0       /* normal output                        */
#define OUTPAUSE        1       /* a pause has been requested           */
#define OUTNEXT         2       /* quit this message, get the next      */
#define OUTSKIP         3       /* stop current process                 */
#define OUTPARAGRAPH    4       /* skip to next paragraph               */
#define IMPERVIOUS      5       /* make current output unstoppable      */
#define NET_CALL        6       /* net call detected - only banner      */
#define STROLL_DETECTED 7       /* net call detected - only banner      */
#define NEITHER         0       /* don't echo input at all              */
#define CALLER          1       /* echo to caller only --passwords etc  */
#define BOTH            2       /* echo to caller and console both      */
/* These are bit flags passed to string input functions */
#define NO_ECHO         0x01  /* Echo input as X's                    */
#define BS_VALID  0x02  /* return BS_RETURN on BS at zero */
#define QUEST_SPECIAL 0x04  /* question mark is special?    */
#define CR_ON_ABORT 0x08  /* question mark is special?    */
#define TWICE   2 /* for MenuList() - icky kludge   */
/* this is for command acquisition intelligence */
#define TERM  "\001"
#define NTERM "\002"
/* return values for command acquisition */
#define BACKED_OUT  0
#define BAD_SELECT  1
#define GOOD_SELECT 2
/* message manipulation values */
#define NO_CHANGE 0
#define DELETED   1
#define NETTED    2
/* Result code defines, to be returned by system dependent functions */
#define R_300           0
#define R_1200          1
#define R_2400          2
#define R_4800          3
#define R_9600          4
#define R_14400         5
#define R_19200         6
#define R_38400         7
#define R_57600         8
#define R_RING          9
#define R_DIAL         10
#define R_NODIAL       11
#define R_AOK          12
#define R_NOCARR       13
#define R_BUSY         14
/*
*     event stuff
*/
#define SUNDAYS         1
#define MONDAYS         2
#define TUESDAYS        4
#define WEDNESDAYS      8
#define THURSDAYS       16
#define FRIDAYS         32
#define SATURDAYS       64
#define ALL_DAYS        127     /* 0x7F */
/* event types */
#define TYPREEMPT       0
#define TYNON           1
#define TYQUIET         2
/* event classes */
#define CLNET           0
#define CLEXTERN        1
#define CLREL           2
#define CL_DL_TIME      3
#define CL_ANYTIME_NET  4
#define CL_DOOR_TIME    5
#define CL_AUTODOOR     6
#define CL_CHAT_ON  7
#define CL_CHAT_OFF 8
#define CL_REDIRECT 9
#define CL_NEWUSERS_ALLOWED 10
#define CL_NEWUSERS_DISALLOWED  11
#define CL_UNTIL_NET  12
#define CL_NETCACHE 13
/* number of event classes supported */
#define EVENT_CLASS_COUNT 14
#define ALL_NETS        ~(0l)
#define MAX_NET         32
#define NO_NETS         0l
#define PRIORITY_MAIL (1l << 31)
typedef struct
  {
  int            EvDur,               /* Event duration       */
  EvWarn;              /* Event warning pointer*/
  unsigned char  EvClass,             /* Event Class          */
  EvType;              /* Event Type           */
  MULTI_NET_DATA EvExitVal;           /* Event Exit value     */
  UNS_16         EvMinutes;           /* From midnight        */
  union
    {
    struct
      {
      long       EvDeadTime;      /* Anytime netting      */
      int        EvAnyDur;        /* Anytime netting      */

      }
    Anytime;
    label      EvUserName;          /* Autodoor target acct */
    struct
      {
      /* Redirect incoming  */
      label EvSystem;   /* Valid system   */
      char  EvFilename[MAX_FILENAME]; /* incoming file */
      int   EvHomeDir;    /* points into codeBuf  */

      }
    Redirect;

    }
  vars;

  }
EVENT;
/*
* net stuff
*/
/* SYSBAUD constants    */
#define ONLY_300        0       /* 300 baud only                        */
#define BOTH_300_1200   1       /* +1200 baud                           */
#define TH_3_12_24      2       /* +2400 baud                           */
#define B_4             3       /* +4800 baud                           */
#define B_5             4       /* +9600 baud                           */
#define B_6             5       /* 14400 baud                           */
#define B_7             6       /* 19200 baud                           */
#define B_8             7       /* 38400 baud                           */
#define B_9             8       /* 57600 baud                           */
/* ITL constants        */
#define ITL_SUCCESS     0
#define ITL_BAD_TRANS   1
#define ITL_NO_OPEN     2
/* Network request codes        */
#define HANGUP          0       /* Terminate networking                 */
#define NORMAL_MAIL     1       /* Send normal Mail                     */
#define R_FILE_REQ      2       /* Request a single file                */
#define A_FILE_REQ      3       /* Request a number of files            */
#define NET_ROOM        5       /* Send a shared room                   */
#define CHECK_MAIL      6       /* Check for recipient validity         */
#define SEND_FILE       7       /* Send a file to another system        */
#define NET_ROUTE_ROOM  8       /* Send a routed shared room            */
#define ROUTE_MAIL      9       /* Send route mail                      */
#define ITL_COMPACT 10  /* Compact messages during transfer */
#define FAST_MSGS 21  /* Mass transfer      */
#define ITL_PROTOCOL    100     /* Switch to different protocol         */
#define ROLE_REVERSAL   201     /* Reverse roles                        */
#define SYS_NET_PWD     202     /* System password stuff                */
#define BAD             0       /* Reply Codes: this indicates bad      */
#define GOOD            1       /* And this indicates good              */
/* These refer to negative ack mail     */
#define NO_ERROR        0       /* No error (ends transmission)         */
#define NO_RECIPIENT    1       /* No recipient found                   */
#define BAD_FORM        2       /* Something's wrong                    */
#define UNKNOWN         99      /* Something's REALLY wrong (eek!)      */
#define PEON            0
#define REG_HOST        1
#define BACKBONE        2       /* Kinda like a hub     */
#define PASS_BACKBONE   3       /* Used in "mode" only, not rbSharedType */
#define ACTIVE_BACKBONE 4
/* These used with ITL_PROTOCOL command         */
#define XM_ITL          "0"
#define YM_ITL          "1"
#define WXM_ITL         "2"
/* These are used with ITL_COMPACT command  */
#define COMPACT_1 "0"
#define NET_GEN         32
#define NON_NET   0
#define NORMAL_NET  1
#define ANYTIME_NET 2
#define ANY_CALL  3
#define STROLL_CALL 4       /* net call detected - only banner      */
#define UNTIL_NET 5
#define NET_CACHE 6
#define NOT_SYSTEM      0
#define BAD_FORMAT      1
#define NO_SYSTEM       2
#define IS_SYSTEM       3
#define SYSTEM_IS_US  4
/* domain function responses  */
#define REFUSE    0
#define OURS    1
#define LOCALROUTE  2
#define DOMAINFILE  3
/* domain file upload results */
#define DOMAIN_SUCCESS  0
#define DOMAIN_FAILURE  1
/* route mail send errors */
#define NO_SUCH_FILE  0
#define REFUSED_ROUTE 1
#define GOOD_SEND 2
#define UNKNOWN_ERROR 3
/* netController() flag values -- OR these values together */
#define REPORT_FAILURE  0x01
#define LEISURELY 0x02
/*
* this structure is the flags attached to a node
*/
struct nflags
  {
  /* Any and all reasons to call this node*/
  BOOLEAN_FLAG(normal_mail);  /* Outgoing normal mail?                */
  BOOLEAN_FLAG(in_use);       /* Is this record even in use?          */
  BOOLEAN_FLAG(room_files);   /* Any file requests?                   */
  BOOLEAN_FLAG(local);        /* Is this node local?                  */
  BOOLEAN_FLAG(spine);        /* Will we be a spine?                  */
  BOOLEAN_FLAG(send_files);
  BOOLEAN_FLAG(is_spine);     /* Is that system a spine?              */
  BOOLEAN_FLAG(OtherNet);
  BOOLEAN_FLAG(HasRouted);
  BOOLEAN_FLAG(RouteFor);
  BOOLEAN_FLAG(RouteTo);
  BOOLEAN_FLAG(Stadel);
  BOOLEAN_FLAG(RouteLock);
  BOOLEAN_FLAG(ExternalDialer);
  BOOLEAN_FLAG(NoDL);
  BOOLEAN_FLAG(MassTransfer);
  BOOLEAN_FLAG(Zip);             /* Amiga update */
  BOOLEAN_FLAG(Zoo);
  BOOLEAN_FLAG(Arc);
  BOOLEAN_FLAG(Lha);

  };
#define GetMode(x)  ((x) & 7)
#define SetMode(x, y) x = (x & (~7)) + y;
#define GetFA(x)  ((x) & 8)
#define SetFA(x)  x |= 8;
#define UnSetFA(x)  x &= (~8);
#define CACHE_END_NAME    "%d.msg"
#define V_CACHE_END_NAME  "v%d.msg"
#define RECOVERY_FILE "incase.net"
#define FAST_TRANS_FILE "\001\002\003"
typedef struct
  {
  char        *addr1, *addr2, *addr3;
  MSG_NUMBER  HiSent;
  int (*sendfunc)(int x);

  }
NetInfo;
typedef struct
  {
  MSG_NUMBER lastMess;        /* Highest net message in this room     */
  unsigned   srgen;           /* High bit of gen is used flag         */
  unsigned   srslot;
  int        mode;    /* low 3 bits is mode, fourth bit tells */
  /* if there's a file of msgs to send  */

  }
SharedRoom;
#define NT_SIZE         (sizeof (*netTab) - (PTR_SIZE))
#define NB_SIZE         (sizeof (netBuf)  - (PTR_SIZE))
#define SR_BULK         (SHARED_ROOMS * sizeof (SharedRoom))
#define NT_TOTAL_SIZE   (NT_SIZE + SR_BULK)
#define NB_TOTAL_SIZE   (NB_SIZE + SR_BULK)
typedef struct
  {
  label          netId;       /* Node id      */
  label          netName;     /* Node name    */
  char           nbShort[3];  /* short hand */
  label          OurPwd;
  label          TheirPwd;
  MULTI_NET_DATA MemberNets;
  struct nflags  nbflags;     /* Flags        */
  char           baudCode;    /* Baud code for this node   */
  char           nbGen;       /* Generation value for this node */
  char           access[40];  /* For alternative access       */
  int      nbRoute;     /* what system do we route via? */
  UNS_16     nbRouteGen;     /* what system do we route via? */
  UNS_16     nbHiRouteInd;        /* internal housekeeping        */
  unsigned long  nbLastConnect; /* last connection with this system */
  SharedRoom *netRooms;

  }
NetBuffer;
typedef struct
  {
  int            ntnmhash;
  int            ntidhash;
  char           ntShort[3];
  struct nflags  ntflags ;
  MULTI_NET_DATA ntMemberNets;
  char           ntGen;
  SharedRoom     *netTRooms;

  }
NetTable;
struct cmd_data
  {
  /* Commands for networking              */
  AN_UNSIGNED command;
  char        fields[4][NAMESIZE];

  };
struct netMLstruct
  {
  MSG_NUMBER ML_id;
  SECTOR_ID  ML_loc;

  };
struct fl_req
  {
  label room;
  label roomfile;
  NET_AREA flArea;
  label filename;

  };
struct fl_send
  {
  NET_AREA snArea;
  label sFilename;

  };
/*
* Error values for reasons on not sharing
*/
#define NO_ROOM   0
#define NOT_SHARING 1 /* not a shared room */
#define NOT_SHARED  2 /* not sharing with you */
#define NO_PWD    3
#define FOUND   4
typedef struct
  {
  label Room;   /* this is the target */
  char  virtual;  /* the rest contains results */
  int   room;
  int   index;
  char  reason;

  }
RoomSearch;
/*
*     Floor data structures
*/
/* Display modes for floor summaries    */
#define INT_EXPERT      0       /* First display for experts            */
#define INT_NOVICE      1       /* First display for novices            */
#define ONLY_FLOORS     2       /* Floors only, no rooms 'tall.         */
#define NOT_INTRO       3       /* 'K' is done.                         */
#define FORGOTTEN       4       /* Forgotten rooms list                 */
#define MATCH_SEL       10
#define DR_SEL          11
#define SH_SEL          12
#define PR_SEL          13
#define ANON_SEL        14
#define READONLY        15
struct floor
  {
  label FlName;
  char  FlInuse;
  label FlModerator;

  };
/*
*     Exit values for errorlevels
*/
#define SYSOP_EXIT              0        /* "Normal"     */
#define RECURSE_EXIT            1
#define CRASH_EXIT              2
#define REMOTE_SYSOP_EXIT       3
#define DOOR_EXIT               4
/*
*     Useful psuedo functions
*/
#define onLine()        (haveCarrier    ||   onConsole)
#define wrNetId(x)      ((strCmpU(x, ALL_LOCALS) != 0) ? x : WRITE_LOCALS)
#define putMLNet(f,b)   if (fwrite(&b, sizeof(b), 1, f) != 1)\
crashout("putMLNet crash")
#define getMLNet(f,b)   (fread(&b, sizeof(b), 1, f) == 1)
#define putSLNet(b, f)  if (fwrite(&b, sizeof(b), 1, f) != 1)\
crashout("putSLNet crash")
#define getSLNet(b, f)  (fread(&b, sizeof(b), 1, f) == 1)
#define TheSysop()      (aide && strCmpU(cfg.SysopName, logBuf.lbname) == SAMESTRING && onConsole)
#define SomeSysop()     (TheSysop() || (remoteSysop && strCmpU(cfg.SysopName, logBuf.lbname) == SAMESTRING))
#define HalfSysop()     (aide && (remoteSysop || onConsole))
#define setNeedsProcessing(i)   netBuf.netRooms[i].srslot |= 0x8000
#define chkNeedsProcessing(i)   (netBuf.netRooms[i].srslot & 0x8000)
#define resetNeedsProcessing(i) netBuf.netRooms[i].srslot &= 0x7FFF
#define isSharedRoom(h, i)      (netTab[h].netTRooms[i].srgen & 0x8000)
#define resetNetRoomFlag(i)     netBuf.netRooms[i].srgen &= 0x7FFF;\
netTab[thisNet].netTRooms[(i)].srgen &= 0x7FFF;
#define netRoomSlot(i)          (netBuf.netRooms[i].srslot & 0x7FFF)
#define netTabRoomSlot(h, i)    (netTab[h].netTRooms[i].srslot & 0x7FFF)
#define netGen(h, i)            (netTab[h].netTRooms[i].srgen & 0x7FFF)
#define roomValidate(h, i)  (roomTab[netTabRoomSlot(h, i)].rtgen==netGen(h,i) \
&& roomTab[netTabRoomSlot(h, i)].rtflags.INUSE \
&& roomTab[netTabRoomSlot(h, i)].rtflags.SHARED)
#define INTERVALS 8             /* Half second intervals                */
#define minimum(x,y)    ((x) < (y) ? (x) : (y))
/* #define max(x,y)        ((x) > (y) ? (x) : (y)) */
#define initLogBuf(x)   (x)->lbgen = (AN_UNSIGNED *) special_GetDynamic(GEN_BULK,__FILE__, __LINE__),\
(x)->lbMail = (theMessages *)special_GetDynamic(MAIL_BULK, __FILE__, __LINE__)
#define killLogBuf(x)   free((x)->lbgen), free((x)->lbMail)
#define initRoomBuf(x)  (x)->msg = (theMessages *)special_GetDynamic(max(MAIL_BULK, MSG_BULK), __FILE__, __LINE__)
#define killRoomBuf(x)  free((x)->msg)
#define initNetBuf(x)   (x)->netRooms = (SharedRoom *)special_GetDynamic(sizeof (*(x)->netRooms) * SHARED_ROOMS, __FILE__, __LINE__)
#define killNetBuf(x)   free((x)->netRooms)
#define NumElems(x)     (sizeof (x)) / (sizeof (x[0]))
#define copyLogBuf(x, y)  memcpy(y, x, LB_SIZE),\
memcpy((y)->lbMail, (x)->lbMail, (long)MAIL_BULK),\
memcpy((y)->lbgen, (x)->lbgen, (long)GEN_BULK)
#define HasPriorityMail(n)  (netTab[n].ntMemberNets & PRIORITY_MAIL)
extern int ClassActive[];
#define Dl_Limit_On()   (ClassActive[CL_DL_TIME])
#define Door_Limit_On() (ClassActive[CL_DOOR_TIME])
extern char **ValidMenuOpts, *Menu;
#define RegisterThisMenu(x, y)      Menu = x, ValidMenuOpts = y;
#define NodeDisabled(x) (!(netTab[x].ntMemberNets & ALL_NETS))
#define CompExtension(CompType)    Formats[(CompType) - 1].Format
/*
*     Call log stuff
*/
#define BAUD            0       /* This message concerns baud rate      */
#define L_IN            1       /*  "      "       "     login          */
#define L_OUT           2       /*  "      "       "     logout         */
#define CARRLOSS        3       /*  "      "       "     carr-loss      */
#define FIRST_IN        4       /*  "      "       "     init           */
#define LAST_OUT        5       /*  "      "       "     close-down     */
#define EVIL_SIGNAL     6       /*  "      "       "     user errors    */
#define CRASH_OUT       7       /*  "      "       "     crash down     */
#define INTO_NET        8       /*  "      "       "     net entry      */
#define OUTOF_NET       9       /*  "      "       "     net exit       */
#define DOOR_RETURN     10      /*  "      "       "     door returns   */
#define DOOR_OUT        11      /*  "      "       "     door exits     */
#define BADWORDS_SIGNAL 12      /*  "      "       "     bad words      */
#define TRIED_CHAT      13      /*  "      "       "     chat attempts  */
#define FL_START        0       /* starting a file transfer             */
#define FL_FAIL         1       /* file transfer failed                 */
#define FL_SUCCESS      2       /* file transfer success                */
#define FL_EX_END       3       /* external file transfer finish        */
/*
*     Transfer protocol constants
*/
#define ASCII           0
#define XMDM            1
#define YMDM            2
#define WXMDM           3
#define TOP_PROTOCOL  WXMDM
#define InternalProtocol(x) (x >= 0 && x <= TOP_PROTOCOL)
#define MAX_WX_ERRORS   10
#define ERRORMAX        10       /* How many errors to suffer through   */
#define RETRYMAX        10
#define MINUTE          60       /* How long is a minute?               */
#define STARTUP         1        /* Code to start a transfer            */
#define FINISH          2        /* Code to cleanup a transfer          */
/* Reception startup error values */
#define TRAN_SUCCESS    0       /* Successful transfer                  */
#define NO_LUCK         1       /* Never return this to caller          */
#define CANCEL          2       /* Session encountered a CAN            */
#define NO_START        3       /* Transfer never even started!         */
#define TRAN_FAILURE    4       /* Something blew...                    */
/* Internally used values, not returned to caller       */
#define NO_ERROR        0
#define BAD_DLE         1
#define EARLY_SYN       2
#define DATA_TIMEOUT    3
#define BAD_CRC         4
#define BAD_CKSM        5
#define BAD_SEC_COMP    6
#define SYNCH_ERROR     7       /* Fatal */
#define WRITE_ERROR     8       /* Fatal */
#define CARR_LOSS       9       /* Fatal */
/* Internal WXMODEM enumerations for transmission window */
#define NOT_USED        0
#define SECTOR_READY    1
#define SENT            2
#define ACKED           3
/* ASCII characters: */
#define SOH             1
#define STX             2
#define CNTRLC          3
#define CNTRLD          4      /* control D character */
#define EOT             4
#define ETX   3
#define ACK             6
#define BELL            7
#define BACKSPACE       8
#define CNTRLI          9       /* aka tab                              */
#define TAB             9       /* aka ^I                               */
#define NEWLINE        10       /* "linefeed" to philistines.           */
#define CNTRLl         12       /* Sysop privileges                     */
#define CNTRLO         15
#define DLE            16
#define XON            17
#define XOFF           19       /* control-s                            */
#define NAK            21
#define SYN            22
#define CAN            24
#define CNTRLZ         26
#define CPMEOF     CNTRLZ
#define ESC            27       /* altmode                              */
#define CRC_START     'C'       /* CRC Mode for WC                      */
#define DEL          0x7F       /* delete char                          */
#define IS_NUMEROUS     0x01
#define IS_DL           0x02
#define NEEDS_FIN       0x04
#define RIGAMAROLE      0x08
#define NOT_AVAILABLE   0x10
#define NEEDS_HDR       0x20    /* True only for file transfer          */
typedef struct
  {
  char *GenericName;
  UNS_16  KludgeFactor;
  UNS_16  flags;                 /* Bit map - see above */
  char *name;
  char *MsgTran;
  char *BlbName;
  char *UpBlbName;
  int  (*method)(int c);
  UNS_16 BlockSize;
  int  (*SendHdr)(long fileSize, char *fileName);
  int  (*CleanUp)(void);

  }
PROTO_TABLE;
typedef struct
  {
  UNS_16 ThisBlock;      /* Block # of this block */
  AN_UNSIGNED *buf;
  CRC_TYPE ThisCRC;        /* So we only calculate once */
  char status;        /* Init these to NOT_USED */

  }
TransferBlock;
#define NORMAL          0
#define DISK            1
/*
* Compression types for use in netting
*/
#define NO_COMP   -1
#define LHA_COMP  1
#define ZIP_COMP  2
#define ZOO_COMP  3
#define ARC_COMP  4
#define COMP_MAX  ARC_COMP
/*
* Protocols for Mass Transfers
*/
#define DEFAULT_PROTOCOL  -1
#define XM_PROTOCOL   0
#define YM_PROTOCOL   1
#define WX_PROTOCOL   2
#define ZM_PROTOCOL   3
/*
* Table for decoding funny files
*/
typedef struct
  {
  char *Format;
  char Many;
  char (*Func)(FILE *fd, ...);

  }
FunnyInfo;
extern FunnyInfo Formats[];
/*
*     SEA ARC reading structure
*/
typedef struct
  {
  char   ArchiveMark;
  char   Header;
  char name [13];     /* file name */
  UNS_32 size;        /* size of compressed file */
  UNS_16 date;        /* file date*/
  UNS_16 time;        /* file time */
  UNS_16 crc;         /* cyclic redundancy check */
  UNS_32 length;      /* true file length */

  }
ARCbuf;
/*
*     PK ZIP header structure
*/
typedef struct
  {
  UNS_32 Signature;
  UNS_16 ExtVersion;
  UNS_16 BitFlags;
  UNS_16 Method;
  UNS_16 FileTime;
  UNS_16 FileDate;
  UNS_32 CRC;
  UNS_32 CompSize;
  UNS_32 NormalSize;
  UNS_16 NameLength;
  UNS_16 FieldLength;

  }
ZipHeader;
/*
*     ZOO header information
* These structures were taken directly from Rahul Dhesi's code
* and are under his copyright.
*/
#define SIZ_TEXT  20                   /* Size of header text */
#define FNAMESIZE 13                   /* Size of DOS filename */
#define LFNAMESIZE 256                 /* Size of long filename */
#define PATHSIZE 256                   /* Max length of pathname */
typedef struct
  {
  char text[SIZ_TEXT];
  UNS_32 zoo_tag;
  UNS_32 zoo_start;
  UNS_32 zoo_minus;
  char major_ver;
  char minor_ver;
  char type;
  UNS_32 acmt_pos;
  UNS_16 acmt_len;
  UNS_16 vdata;

  }
zoo_header;
typedef struct
  {
  UNS_32 zoo_tag;
  char type;
  char packing_method;
  UNS_32 next;
  UNS_32 offset;
  UNS_16 date;
  UNS_16 time;
  UNS_16 file_crc;
  UNS_32 org_size;
  UNS_32 size_now;
  char major_ver;
  char minor_ver;
  char deleted;
  char struc;
  UNS_32 comment;
  UNS_16 cmt_size;
  char fname[FNAMESIZE];
  UNS_16 var_dir_len;
  char tz;
  UNS_16 dir_crc;
  /* fields for variable part of directory entry follow */
  char namlen;
  char dirlen;
  char lfname[LFNAMESIZE];
  char dirname[PATHSIZE];
  UNS_16 system_id;
  UNS_32 fattr;
  UNS_16 vflag;
  UNS_16 version_no;

  }
zoo_direntry;
/*
*     LHA structure information
*     courtesy Daniel Durbin
*/
typedef struct
  {
  /* Local file header */
  char unknown1[2];   /* ? */
  char method[5];     /* compression method */
  UNS_32 csize;     /* compressed size */
  UNS_32 fsize;     /* uncompressed size */
  UNS_16 ftime;     /* last mod file time (msdos format) */
  UNS_16 fdate;     /* last mod file date */
  char fattr;     /* file attributes */
  char unknown2;      /* ? */
  char namelen;     /* filename length */

  }
LZHead;
/*
*     Gif header information
*/
typedef struct
  {
  char   Sig[6];  /* no null */
  UNS_16 Width;
  UNS_16 Height;
  char   Colors;

  }
GifHeader;
/*
*     Timer Assignments
*/
#define WORK_TIMER      0       /* scratch timer        */
#define NEXT_ANYNET     1
#define USER_TIMER      2
#define NET_SESSION     3
/*
*     Multi-tasking (OS) defines
*/
#define INUSE_PAUSE 0
#define IDLE_PAUSE  1
#define NET_PAUSE 2
#define CHAT_NICE 3
/*
*     Data Entry Types
*/
#define MSG_ENTRY 0
#define FILE_ENTRY  1
#define INFO_ENTRY  2
#define BIO_ENTRY 3
/*
*     Finger saving defines
*/
#define termWidth       logBuf.lbwidth
#define termNulls       logBuf.lbnulls
#define termLF          logBuf.lbflags.LFMASK
#define expert          logBuf.lbflags.EXPERT
#define aide            logBuf.lbflags.AIDE
#define sendTime        logBuf.lbflags.TIME
#define oldToo          logBuf.lbflags.OLDTOO
#define FloorMode       logBuf.lbflags.FLOORS
#define HalfDup         logBuf.lbflags.HALF_DUP
#define thisFloor       roomBuf.rbFlIndex
#define DoorPriv        logBuf.lbflags.DOOR_PRIVS
#define MAILSLOTS       cfg.MailSlots
#define MSGSPERRM       cfg.MsgsPerrm
#define MAXROOMS        cfg.MaxRooms
#define SHARED_ROOMS    cfg.SharedRooms
/*
* This is icky, and should be a lesson to all would-be
* header writers.
*/
#include "ctdlvirt.h"
#include "ctdlansi.h"
#include "ansisys.h"
/* And that's it for this file */
