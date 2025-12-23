/*
** $VER: zeus.h 37.0 (12.01.94)
**       ____________
**      (           / ___  __ __  ____
**       \_____    / / _/ / // / /___/
**            /   / / /  / // / //__
**           /   / / _/ / // / /__ /
**          /   / / /  / (/ / ___//
**         /   / /__/ (____/ /___/
**        /   /_____
**       /          \
**      /____________)  bulletin board system
** 
** Useful definitions for the zeus.library
**
** (C) Copyright 1992-1994 Alex May and Nick Loman
** All rights reserved
**
** AmigaE includes by Neil Williams 5.4.99
**
*/

OPT MODULE
OPT EXPORT
OPT PREPROCESS

MODULE 'exec/types',
       'exec/lists',
       'exec/nodes',
       'exec/ports',
       'exec/semaphores',
       'dos/dos',
       'dos/dosextens',
       'graphics/gfxbase',
       'intuition/intuitionbase',
       'exec/libraries'

#define  ZEUS_COPYRIGHT    'Zeus Bulletin Board System (c)1992-1995 Alex May & Nick Loman'

/*
** The function description structure
*/

OBJECT fdfunc
   command:PTR TO CHAR
   function:LONG        -> pointer to a function
   next:LONG            /* Private - set to NULL and do not reference! -> aptr */
   flags:LONG
ENDOBJECT

CONST  FD_ASYNC      = $00000001
CONST  FD_NEEDLINE   = $00000002

/**********************************************************************/
/*            The structure passed to the XPandCC functions           */
/**********************************************************************/

OBJECT ccargs
   line:PTR TO line
   cc:LONG
   maxlen:LONG
   buffer:PTR TO CHAR
   args[40]:ARRAY OF CHAR
   format[30]:ARRAY OF CHAR
ENDOBJECT

/**********************************************************************/
/*                     Defines for the user flags                     */
/**********************************************************************/

CONST  UF_MORE       = $00000001
CONST  UF_EXPERT     = $00000002
CONST  UF_HOTKEYS    = $00000004
CONST  UF_NO8BIT     = $00000008
CONST  UF_VALIDATED  = $00000010
CONST  UF_CLRSCR     = $00000020
CONST  UF_LOGUSER    = $00000040
CONST  UF_ANSICOLR   = $00000080     /* Was UF_NOANSI */
CONST  UF_CRLF       = $00000100
CONST  UF_VALID      = $00000200
CONST  UF_WANTRIP    = $00000400
CONST  UF_DELETED    = $00000800
CONST  UF_AMIGAANSI  = $00001000
CONST  UF_PROTECTED  = $00002000
CONST  UF_NORESTRICT = $00004000
CONST  UF_ANSIPOS    = $00008000
CONST  UF_ANSICOMP   = $00010000
CONST  UF_ANIMPROMPTS= $00020000
CONST  UF_ANSIMUSIC  = $00040000

#define  UF_USEANSI    (UF_ANSICOLR OR UF_ANSIPOS OR UF_ANSICOMP)

/**********************************************************************/
/*          Structures involved in the user list traversing           */
/**********************************************************************/

OBJECT useridx
   name[40]:ARRAY OF CHAR
   alias[40]:ARRAY OF CHAR
   datapos;
ENDOBJECT

CONST  KEY_ALPHAFORWARD    = $0001
CONST  KEY_ALPHAREVERSE    = $0002
CONST  KEY_CHRONOFORWARD   = $0003
CONST  KEY_CHRONOREVERSE   = $0004

OBJECT userkKey
   current:PTR TO useridx              /* The current Index structure   */
   /* Data beyond this point is private and subject to change */
ENDOBJECT

/**********************************************************************/
/*                        Main User structure                         */
/**********************************************************************/

OBJECT user
   name[40]:ARRAY OF CHAR
   alias[40]:ARRAY OF CHAR
   password[20]:ARRAY OF CHAR
   voicephone[20]:ARRAY OF CHAR
   dataphone[20]:ARRAY OF CHAR
   street[40]:ARRAY OF CHAR
   town[20]:ARRAY OF CHAR
   county[20]:ARRAY OF CHAR
   postcode[12]:ARRAY OF CHAR
   country[20]:ARRAY OF CHAR

   firstcall:LONG
   lastcall:LONG
   lastbaudrate:LONG
   flags:LONG

   ripfsz:CHAR
   logoffcode:CHAR                     /* see lo_* defines */

   expire:INT                          /* number of days from first call the user may logon */

   credit:LONG

   minsleft:INT
   numcalls:INT
   maxcallsperday:INT
   maxminspercall:INT
   maxminsperday:INT
   
   dob_year:INT                        /* date of birth */
   dob_mon:CHAR
   dob_day:CHAR

   access:CHAR
   computer:CHAR
   scrwidth:CHAR
   scrheight:CHAR
   menuset:CHAR
   language:CHAR
   callstoday:CHAR
   gender:CHAR
ENDOBJECT

/*
** Defines for ur_Gender
*/

CONST  GEN_UNKNOWN    = 0
CONST  GEN_MALE       = 1
CONST  GEN_FEMALE     = 2

/*
** Defines for ur_LogOffCode
*/

CONST  LO_NORMAL      = 0
CONST  LO_INACTIVE    = 1
CONST  LO_BOOTED      = 2
CONST  LO_NOCARRIER   = 3
CONST  LO_NOTIME      = 4

/*
** Flags for reading
*/

CONST  IR_RAW        = $00000001
CONST  IR_SECRET     = $00000002
CONST  IR_NOSHOW     = $00000004
CONST  IR_ONLYCHARS  = $00000008
CONST  IR_ONLYOPTS   = $00000010
CONST  IR_ONLYNUMS   = $00000020
CONST  IR_ONLYALPHA  = $00000040
CONST  IR_MAKEUPPER  = $00000080     /* Make all characters uppercase */
CONST  IR_MAKELOWER  = $00000100     /* Make all characters lowercase */
CONST  IR_DEFAULTSTR = $00000200     /* Use default string            */
CONST  IR_NOWAIT     = $00000400     /* Return immediatly             */
CONST  IR_ISMENU     = $00000800     /* Allow command stacking        */
CONST  IR_NONEWLINE  = $00001000     /* Do not append a new line      */
CONST  IR_XOPTS      = $00002000
CONST  IR_MAKENICE   = $00004000
CONST  IR_HISTORY    = $00008000
CONST  IR_ADDHIST    = $00010000
CONST  IR_NOCD       = $00020000     /* Don't check for carrier loss  */
CONST  IR_EOLRET     = $00040000     /* Return when buffer is full    */
CONST  IR_CHECKMSGS  = $00080000     /* Return when line message(s) are available */
CONST  IR_STRIP      = $00100000     /* Strips leading/trailing spaces */

/**********************************************************************/
/*                   Defines for the dc_Flags field                   */
/**********************************************************************/

CONST  DCF_OPENDEV   = $00000001
CONST  DCF_SCRIPT    = $00000002
CONST  DCF_USEDSR    = $00000004
CONST  DCF_CHECKCAR  = $00000008
CONST  DCF_USERADBOOG= $00000010
CONST  DCF_FLOATBAUD = $00000020

/**********************************************************************/
/*                      The device config struct                      */
/**********************************************************************/

OBJECT dev_cfg
   device[40]:ARRAY OF CHAR
   unit:LONG
   devbuffsize:LONG
   baud:LONG
   brktime:LONG
   flagS:LONG
   ring[40]:ARRAY OF CHAR
   nocon[40]:ARRAY OF CHAR
   con[40]:ARRAY OF CHAR
   answer[40]:ARRAY OF CHAR
   hangup[40]:ARRAY OF CHAR
   init[40]:ARRAY OF CHAR
   datalen:CHAR
   stopbits:CHAR
   handshake:CHAR
   parity:CHAR
ENDOBJECT

/*
** IEMSI structure
**
** Only valid if ( line->le_TermCaps & DOES_IEMSI )
*/

OBJECT ie_info
   buff:PTR TO CHAR      /* Buffer containing data sent from user */
   name:PTR TO CHAR      /* These fields are offsets into ie_buff */
   alias:PTR TO CHAR
   location:PTR TO CHAR
   datanum:PTR TO CHAR
   voicenum:PTR TO CHAR
   password:PTR TO CHAR
   bdate:PTR TO CHAR
   crtdef:PTR TO CHAR
   protocols:PTR TO CHAR
   capabs:PTR TO CHAR
   requests:PTR TO CHAR
   software:PTR TO CHAR
   transtab:PTR TO CHAR
ENDOBJECT

/*
** The line structure as obtained by GetLine()
*/

OBJECT line
   user:PTR TO user             /* pointer to user online or null            */
   devcfg:PTR TO dev_cfg        /* pointer to line's device config or null   */
   number:LONG                  /* number of this line                       */
   realbaud:LONG                /* read baud rate (or null for local)        */
   carrier:INT                  /* carrier present if true                   */
                                /*  BOOL = 16bits                            */
   status:PTR TO CHAR           /* status string                             */
   lang:PTR TO language         /* current language                          */
   minsonline:LONG              /* minutes line has been active              */
   connectflags:INT             /* connection flags (see cd_* defines)       */
   anim:PTR TO CHAR             /* ansi prompt animation pointer             */
   animpos:INT                  /* position in the ansi anim                 */
   type:INT
   termcaps:INT                 /* terminal capabilities                     */
   iemsi:PTR TO ie_info         /* iemsi information                         */
   inactive:INT                 /* number of seconds since user last pressed a key */
   lockbaud:LONG                /* computer -> modem baud                    */
   access:lh                    /* access restriction list                   */
   flags:INT
   trptri:PTR TO CHAR           /* pointer to current input translation table or null (usually points to le->le_transi) */
   trptro:PTR TO CHAR           /* pointer to current output translation table or null (usually points to le->le_transo) */
   transi[256]:ARRAY OF CHAR    /* input translation */
   transo[256]:ARRAY OF CHAR    /* output translation */
   logontime:LONG               /* time user logged on (time_t) */
ENDOBJECT

/*
** Defines for le->le_Type
**
** le->le_DevCfg is only valid for TYPE_SERIAL
*/

CONST  TYPE_LOCAL    = 0
CONST  TYPE_SERIAL   = 1

CONST  TYPE_ACCESS   = $FFFF

/*
** Defines for the le->le_TermCaps field
*/

CONST  DOES_ANSI     = $0001
CONST  DOES_RIP      = $0002
CONST  DOES_IEMSI    = $0004
CONST  DOES_8BIT     = $0008      /* For forcing 8 bit */

/*
** Flags for le->le_Flags
*/

CONST  LE_NOTAKETIME = $0001

/*
** The flags that can be obtained by GetLineStatus()
*/

CONST  LN_OPEN       = $00000001        /* Redundant                           */
CONST  LN_LISTEN     = $00000002        /* Redundant                           */
CONST  LN_DIE        = $00000004        /* Pending kill line                   */
CONST  LN_ALLOWBRK   = $00000008
CONST  LN_WRITING    = $00000010        /* Writing data                        */
CONST  LN_READING    = $00000020        /* Reading data                        */
CONST  LN_INDOOR     = $00000040        /* In a CLIDoor                        */
CONST  LN_DEAD       = $00000080
CONST  LN_DONECP     = $00000100        /* Redundant                           */
CONST  LN_KILLSCR    = $00000200        /* Pending kill script                 */
CONST  LN_DEVOPEN    = $00000400        /* Device is open (TYPE_SERIAL only)   */
CONST  LN_INCHAT     = $00000800        /* In chat                             */
CONST  LN_STOPCHAT   = $00001000        /* Pending chat end                    */
CONST  LN_LOCAL      = $00002000        /* Line is local                       */
CONST  LN_DYING      = $00004000        /* Line has received LN_DIE            */
CONST  LN_CNTMINS    = $00008000        /* Counting minutes online             */
CONST  LN_ONLINE     = $00010000        /* Line is Online                      */
CONST  LN_BOOTUSER   = $00020000        /* Boot user off!                      */
CONST  LN_OUTOFF     = $00040000        /* Output is turned off                */
CONST  LN_RIPENABLED = $00080000        /* RIP is enabled                      */
CONST  LN_LOCALINPUT = $00100000        /* Last input came from local          */
CONST  LN_SERIALOFF  = $00200000        /* Serial output is disabled           */
CONST  LN_NOCD       = $00400000
CONST  LN_NOCHAT     = $00800000
CONST  LN_NOSERIN    = $01000000
CONST  LN_ICONIFY    = $02000000        /* Watchers iconify setting            */

OBJECT p_sem
   sem:ss
   name[10]:ARRAY OF CHAR
   data:PTR TO CHAR
ENDOBJECT

OBJECT p_sems
   func_sem:p_sem
   user_sem:p_sem
   prog_sem:p_sem
   line_sem:p_sem
   dead_sem:p_sem
   gfnc_sem:p_sem
ENDOBJECT

/**********************************************************************/
/*               Saved Data for recording system stats                */
/**********************************************************************/

OBJECT saveddata
   calls:LONG
   localcalls:LONG
   callstoday:LONG

   users:LONG
   logins[24]:ARRAY OF LONG
ENDOBJECT

OBJECT extrasaved
   daysup:LONG
   hoursup:CHAR
   minsup:CHAR

   es_firstrun:LONG    -> time_t

   active[24]:ARRAY OF LONG
   online[24]:ARRAY OF LONG
ENDOBJECT

/**********************************************************************/
/*                      Various System Variables                      */
/**********************************************************************/

OBJECT sysvars
   useridxbuff:INT
   carchkinterval:INT
   inactime:INT
   systemname[60]:ARRAY OF CHAR
   systemlocation[60]:ARRAY OF CHAR
   sysopname[40]:ARRAY OF CHAR
   lastcalls:INT
   bufferedtext:INT
   maxtextsize:LONG
   flags:LONG
ENDOBJECT

CONST     SVF_COUNTLOCAL   = $00000001

/**********************************************************************/
/*                               Paths                                */
/**********************************************************************/

OBJECT paths
   user[50]:ARRAY OF CHAR
   text[50]:ARRAY OF CHAR
   scripts[50]:ARRAY OF CHAR
   mail[50]:ARRAY OF CHAR
   file[50]:ARRAY OF CHAR
   doors[50]:ARRAY OF CHAR
   logs[50]:ARRAY OF CHAR
   inbound[50]:ARRAY OF CHAR
   outbound[50]:ARRAY OF CHAR
ENDOBJECT

/*
** Language config as found in Shared->LangList
*/

OBJECT language
   node:ln
   lang:LONG       -> aptr
   name:PTR TO CHAR
   number:LONG
   tran[16]:ARRAY OF CHAR
   press_return:PTR TO CHAR
   yesno:PTR TO CHAR
   yesnocont:PTR TO CHAR
   def_yes:PTR TO CHAR
   def_no:PTR TO CHAR
   yes:PTR TO CHAR
   no:PTR TO CHAR
   cont:PTR TO CHAR
   privchar:PTR TO CHAR
   cc_yes:PTR TO CHAR
   cc_no:PTR TO CHAR
   cc_on:PTR TO CHAR
   cc_off:PTR TO CHAR
   cc_strform:PTR TO CHAR
   cc_numform:PTR TO CHAR
   cc_timeform:PTR TO CHAR
   more:PTR TO CHAR
ENDOBJECT

/*
** Last caller structure for Shared->LastCalls
*/

OBJECT lastcaller
   node:ln                          /* For linkage                                        */
   line:INT                         /* Line number                                        */
   name[40]:ARRAY OF CHAR           /* Users name                                         */
   logon:LONG /* time_t */          /* LogOn time                                         */
   logoff:LONG /* time_t */         /* LogOff time                                        */
   user:user                        /* _Copy_ of the user structure as it was at logoff   */
ENDOBJECT

/*
** Information from Zeus.key
*/

OBJECT keyinfo
   created:datestamp                /* DateStamp of Key creation  */
   serial:LONG                      /* Zeus serial number         */
   name[40]:ARRAY OF CHAR           /* Name of registered sysop   */
ENDOBJECT

/**********************************************************************/
/*                     The shared data structure                      */
/**********************************************************************/

OBJECT shareddata
   dosbase:PTR TO doslibrary
   gfxbase:PTR TO gfxbase
   intuitionbase:PTR TO intuitionbase
   diskfontbase:PTR TO lib
   gadtoolsbase:PTR TO lib
   rexxsysbase:PTR TO lib
   utilitybase:PTR TO lib
   workbenchbase:PTR TO lib
   pipeutilbase:PTR TO lib

   rsrccount:LONG                            /* private */
 
   sems:PTR TO p_sems                        /* private */
   saved:PTR TO saveddata
   pipeproc:PTR TO process

   hiline:LONG                               /* high number of lines present*/
   active:LONG                               /* number of lines active */

   ln_array:PTR TO LONG    /* **line_node !! */  /* private */
   
   users:LONG                                /* how many users */
   
   sysvars:PTR TO sysvars                    /* system variables */
   paths:PTR TO paths                        /* paths config */
   langlist:lh                               /* list of defined languages */

   sysop:INT               /* bool */        /* true - sysop available for chat */
   
   lastcalls:lh                              /* last callers of this session */
   numlast:INT                               /* number of last callers in list */
   
   zver:INT                                  /* version of zeus */
   zrev:INT                                  /* revision of zeus */
   
   globals:LONG                              /* number of global commands defined */
   
   lcsem:PTR TO ss                           /* semaphore for the last calls list - use it! */
   
   mslist:lh                                 /* menu set list */
   mssem:ss                                  /* menu set semaphore */
   
   banlist:lh                                /* banned user wildcards */
   bansem:ss

   keyinfo:PTR TO keyinfo                    /* info from zeus.key */

   translist:lh                              /* character translations */

   extra:PTR TO extrasaved                   /* Extra Zeus Stats */
ENDOBJECT

/*
** Flags for writing
*/

CONST  WF_NOMORE        = $00000001     /* No more prompts            */
CONST  WF_ISMENU        = $00000002     /* Is a menu                  */
CONST  WF_BREAK         = $00000004     /* Allow breaking             */
CONST  WF_NOCOOK        = $00000010     /* Dont 'cook' the data       */
CONST  WF_SPECIALMORE   = $00000020     /* Return when screen is full */
CONST  WF_USEMOREYN     = $00000040     /* Prompt is 'More Y/N'       */
CONST  WF_RIPONLY       = $00000080     /* Only display if RIP        */
CONST  WF_TXTONLY       = $00000100     /* Only display if not RIP    */
CONST  WF_USEMOREYNC    = $00000200     /* Prompt is 'More Y/N/C      */
CONST  WF_BUFFER        = $00000400     /* Buffer this file           */
CONST  WF_ISRIP         = $00001000     /* Is RIP graphics            */
CONST  WF_NORESET       = $00002000     /* Don't send RIP reset code  */
CONST  WF_NOEXPAND      = $00004000     /* Don't expand control codes */
CONST  WF_LOCALDIR      = $00008000
CONST  WF_NOCD          = $00010000

/**********************************************************************/
/*                  The structure passed to ReadLine                  */
/**********************************************************************/

OBJECT readvars
   linenum:LONG
   buff:PTR TO CHAR
   opts:PTR TO CHAR
   maxlen:LONG
   minlen:LONG
   flags:LONG
ENDOBJECT

/**********************************************************************/
/*                  Structure used in chat routines                   */
/**********************************************************************/

OBJECT chatchar
   ch:CHAR
   type:CHAR
ENDOBJECT

CONST  TYPE_USER  = $00
CONST  TYPE_SYSOP = $01
CONST  TYPE_FINISH= $02
CONST  TYPE_START = $03

/**********************************************************************/
/*             Structures and defines for Control Gadgets             */
/**********************************************************************/

OBJECT conmsg
   msg:mn
   gadid:INT
   action:INT
ENDOBJECT

CONST  GAD_SELECT     = 1     /* Codes in ConMsg->cm_Action */
CONST  GAD_QUIT       = 2
CONST  WIN_ICONIFY    = 3

/*
** Structures and defines for use when transfering files
*/

CONST  XFER_SEND           = 1     /* Mode options */
CONST  XFER_RECEIVE        = 2

CONST  FILE_ABORTED        = 0     /* Codes found in XferInfoNode->Status */

CONST  FILE_SENT           = 1
CONST  FILE_PARTIAL_SEND   = 2

CONST  FILE_RECEIVED       = 3
CONST  FILE_PARTIAL_RECEIVE = 4

OBJECT xferinfo
   send:lh                             /* List of XferInfoNodes      */
   recv:lh                             /* List of XferInfoNodes      */
   le:PTR TO line
   xpr:PTR TO LONG          /* *xprproto */

   filessent:LONG                      /* number of files sent       */
   filesrcvd:LONG                      /* number of files received   */

   sendcps:LONG                        /* average send cps           */
   recvcps:LONG                        /* average receive cps        */
   sendbytes:LONG                      /* number of bytes sent       */
   recvbytes:LONG                      /* number of bytes received   */
   senderrs:LONG                       /* number of send errors      */
   recverrs:LONG                       /* number of receive errors   */
   sendtimeouts:LONG                   /* number of send timeouts    */
   recvtimeouts:LONG                   /* number of receive timeouts */
   mode:LONG                           /* initial mode               */
ENDOBJECT

OBJECT xferinfonode
   node:ln
   filename[108]:ARRAY OF CHAR
   length:LONG                         /* Length of full file (or -1 if not known) */
   xferd:LONG                          /* Amount of bytes transfered */
   status:INT                          /* Final status of file */
   cps:LONG                            /* Chars per second */
ENDOBJECT

/*
** Defines for Pipe functions
*/

CONST     PIPE_READ     = 1
CONST     PIPE_WRITE    = 2
CONST     PIPE_EOF      = 1

CONST     PIPE_REQ_WRITE   = 1           /* Request Types */
CONST     PIPE_REQ_READ    = 2
CONST     PIPE_REQ_ABORT   = 3

/*
** Defines for LineOutputMode
*/

CONST     OUTPUT_ON     = 0
CONST     OUTPUT_OFF    = 1

/*
** Bulletin related structures/defines
*/

OBJECT bulletin
   number:LONG                /* number of bulletin   */
   name[50]:ARRAY OF CHAR     /* name of bulletin     */
   key[10]:ARRAY OF CHAR      /* key                  */
   filename[100]:ARRAY OF CHAR /* filename             */
   created /* time_t */       /* creation date        */
   expires /* time_t */       /* expiry date or -1    */

   minacc:CHAR                /* minimum access needed */
   maxacc:CHAR                /* Maximum access needed */
ENDOBJECT

OBJECT bltnnode
   node:PTR TO ln             /* Node.ln_Name points to Bltn.Key */
   bltn:bulletin
ENDOBJECT

/*
** Tags for ReadLineTags and ReadLineTagList
*/

CONST  TAG_USER  =  $80000000

#define  RLT_Opts          (TAG_USER+1)
#define  RLT_MaxLen        (TAG_USER+2)
#define  RLT_MinLen        (TAG_USER+3)
#define  RLT_Flags         (TAG_USER+4)
#define  RLT_TimeOut       (TAG_USER+5)

#define  RLT_Raw           (TAG_USER+6)
#define  RLT_Secret        (TAG_USER+7)
#define  RLT_NoShow        (TAG_USER+8)
#define  RLT_OnlyChars     (TAG_USER+9)
#define  RLT_OnlyOpts      (TAG_USER+10)
#define  RLT_OnlyNums      (TAG_USER+11)
#define  RLT_OnlyAlpha     (TAG_USER+12)
#define  RLT_MakeUpper     (TAG_USER+13)
#define  RLT_MakeLower     (TAG_USER+14)
#define  RLT_DefaultStr    (TAG_USER+15)
#define  RLT_NoWait        (TAG_USER+16)
#define  RLT_IsMenu        (TAG_USER+17)
#define  RLT_NoNewLine     (TAG_USER+18)
#define  RLT_XOpts         (TAG_USER+19)
#define  RLT_NoCD          (TAG_USER+20)
#define  RLT_MakeNice      (TAG_USER+21)
#define  RLT_Prompt        (TAG_USER+22)
#define  RLT_EolRet        (TAG_USER+23)
#define  RLT_CheckMsgs     (TAG_USER+24)
#define  RLT_History       (TAG_USER+25)
#define  RLT_AddHist       (TAG_USER+26)

->#ifndef USE_DOORLIB

/*
** Prototypes for pl.lib & plnb.lib
*/

->long LPuts( long, UBYTE *, ULONG );

->#else

/*
** Prototypes for zdoor.lib & zdoornb.lib
*/

->long LPuts( UBYTE * );

->#endif

/*
** Watchers
*/

OBJECT watchermsg
   msg:mn
   code:LONG
   result:LONG
   line:LONG      /* Line Number */
   flags:LONG
ENDOBJECT

/*
** Defines for WatcherMsg->Code
*/

CONST  WM_OFFLINE    = 0           /* End of session */
CONST  WM_ONLINE     = 1           /* Start of session */
CONST  WM_RIPON      = 2
CONST  WM_RIPOFF     = 3
CONST  WM_ICONIFY    = 4
CONST  WM_UNICONIFY  = 5
CONST  WM_USERON     = 6           /* User has been PUTONLINE */
CONST  WM_NEWSTATUS  = 7           /* New line->le_Status */
CONST  WM_TOFRONT    = 8           /* Move Screen/Window to front */
CONST  WM_TOBACK     = 9           /* Move Screen/Window to back */
CONST  WM_UPDATEUSER = 10          /* User details have been changed */
CONST  WM_QUIT       = 11

/*
** Defines for WatcherMsg->Result
*/

CONST  WR_MOVETOTOP  = 1
CONST  WR_MOVETOBOT  = 2

/*
** Watcher function key structures/defines
*/

CONST  FKEY_EXEC     = 0           /* Types for FKey->Type */
CONST  FKEY_TEXT     = 1

OBJECT fkey
   data[60]:ARRAY OF CHAR
   type:INT
ENDOBJECT

OBJECT watchersem
   sem:ss
   name[20]:ARRAY OF CHAR
   keys[20]:ARRAY OF fkey
   opencnt:INT
ENDOBJECT

#define WATCHSEMNAME      'WATCHKEYSEM'

/*
** Flags returned by GetConnectMode()
*/

CONST  CD_ARQ     = $0001
CONST  CD_HST     = $0002
CONST  CD_SYNC    = $0004
CONST  CD_V42BIS  = $0008
CONST  CD_LAPM    = $0010
CONST  CD_MNP     = $0020
CONST  CD_MNP5    = $0040

/*
** Menu Set Flags
*/

CONST  MS_RIP        = $0001
CONST  MS_AMIGA      = $0002
CONST  MS_8BIT       = $0004
CONST  MS_ANSICOLR   = $0008
CONST  MS_NOMORE     = $0010
CONST  MS_ANSIPOS    = $0020
CONST  MS_ANSICOMP   = $0040

OBJECT menuset
   node:ln
   number:INT
   name[50]:ARRAY OF CHAR
   flags:INT
ENDOBJECT

OBJECT bannode
   node:mln
   data[50]:ARRAY OF CHAR
ENDOBJECT

CONST     LSC_PATHS        = $0001
CONST     LSC_SYSVARS      = $0002
CONST     LSC_LANGS        = $0004
CONST     LSC_MENUS        = $0008
CONST     LSC_BANNED       = $0010
CONST     LSC_COMPUTERS    = $0020
CONST     LSC_ALL          = $FFFF

CONST     LSC_RET_OK          = 0
CONST     LSC_RET_DOSERR      = 1
CONST     LSC_RET_MEMERR      = 2
CONST     LSC_RET_NOZEUS      = 3
CONST     LSC_RET_OLDCFG      = 4
CONST     LSC_RET_NOCFG       = 5
CONST     LSC_RET_DEFLANGERR  = 6
CONST     LSC_RET_LANGERR     = 7

/*
** Structures and defines for Line access
**
** These nodes are found in the line structure in the le_Access list
*/

OBJECT lineacc
   node:mln
   fhor, fmin:CHAR          /* from time                  */
   thor, tmin:CHAR          /* to time                    */
   flags:LONG               /* flags - see acc_* defines  */
   minbaud, maxbaud:LONG    /* min/max baudrates          */
   minacc, maxacc:CHAR      /* min/max access levels      */
ENDOBJECT

CONST  ACC_LOGONS    = $00000001
CONST  ACC_MINBAUD   = $00000002
CONST  ACC_MAXBAUD   = $00000004
CONST  ACC_MINACCESS = $00000008
CONST  ACC_MAXACCESS = $00000010

CONST  EDIT_ANSI     = -1
CONST  EDIT_NOANSI   = -2

OBJECT editopts
   history:PTR TO lh
   num:LONG
   buff:PTR TO CHAR
   work:PTR TO CHAR
   opts:PTR TO CHAR
   xopts:PTR TO CHAR
   minlen
   maxlen
   flags
ENDOBJECT

/*
** ZSegs - Returned by ZLoadSeg
*/

OBJECT zsegs
   seg:PTR TO segment       /* May be NULL */
   seglist:LONG /* bptr */
ENDOBJECT

/*
** TransNode - Found on Shared->TransList
*/

OBJECT transnode
   n:ln
   tag[12]:ARRAY OF CHAR       /* tag                      */
   name[30]:ARRAY OF CHAR      /* name of this translation */
   itab[256]:ARRAY OF CHAR     /* input table              */
   otab[256]:ARRAY OF CHAR     /* Output table             */
ENDOBJECT

/*
** User ExtraData
*/

#define     ID_CTRI     "CTRI"    /* Translation In  */
#define     ID_CTRO     "CTRO"    /* Translation Out */

/*
** Defines for SetCLIMode()
*/

CONST  MORE_OFF    = 0
CONST  MORE_ON     = 1

/*
** InterNet config structure
*/

OBJECT in_cfg
   username[50]:ARRAY OF CHAR
   addr[60]:ARRAY OF CHAR
   organization[50]:ARRAY OF CHAR
   newsreceipt[50]:ARRAY OF CHAR
   newsdir[256]:ARRAY OF CHAR
   tempdir[256]:ARRAY OF CHAR
   spooldir[256]:ARRAY OF CHAR
   maildir[256]:ARRAY OF CHAR
   sendnews[256]:ARRAY OF CHAR
   sendmail[256]:ARRAY OF CHAR
   rnews[256]:ARRAY OF CHAR
   timezone[5]:ARRAY OF CHAR
   gatedomain[30]:ARRAY OF CHAR
   gatetype:INT
   gmtoff:LONG
   hdrdef:INT
   flags:INT
   indexes:LONG
   yzone, ynet, ynode, ypoint:INT
   gzone, gnet, gnode, gpoint:INT
ENDOBJECT

CONST  GATE_LOCAL     = 0
CONST  GATE_PRIVATE   = 1
CONST  GATE_PUBLIC    = 2

OBJECT in_req
   cfg:PTR TO in_cfg
   hdrs:PTR TO lh
   gacc:PTR TO lh
ENDOBJECT

OBJECT   hdrline
   hdr[81]:ARRAY OF CHAR
   type:CHAR
ENDOBJECT

CONST  HDR_PORT      = 0
CONST  HDR_KLUDGE    = 1
CONST  HDR_DELETE    = 2

OBJECT hdrnode
   n:mln
   h:hdrline
ENDOBJECT

OBJECT gateaccnode
   n:mln
   type:INT
   zone:INT
   net:INT
   node:INT
   point:INT
ENDOBJECT

CONST  GACC_GATE     = 0
CONST  GACC_NOGATE   = 1

CONST  ADDR_WILD     = $FFFF

/*
** Return codes from DoEdit()
*/

CONST  EDIT_RETURN   = -1
CONST  EDIT_EOL      = -2

