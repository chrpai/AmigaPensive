#ifndef LIBRARIES_ZEUS_H
#define LIBRARIES_ZEUS_H

/*
** $VER: zeus.h 37.1 (14.7.99)
** histR: zeus.h 37.0 (12.01.94)
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
** (C) Copyright 1992-1999 Zeus Developments
** All rights reserved
**
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

#ifndef EXEC_SEMAPHORES_H
#include <exec/semaphores.h>
#endif

#ifndef _TIME_H
#include <time.h>
#endif

#ifndef DOS_DOSEXTENS_H
#include <dos/dosextens.h>
#endif

#define  ZEUS_COPYRIGHT    "Zeus Bulletin Board System (c)1992-2000 Zeus Developments"

/*
** The function description structure
*/

struct   fdfunc
{
	UBYTE    *command;
	long     (*function)( APTR );
	APTR     next;       /* Private - set to NULL and do not reference! */
	ULONG    flags;
};

#define  FD_ASYNC       0x00000001
#define  FD_NEEDLINE    0x00000002

/**********************************************************************/
/*            The structure passed to the XPandCC functions           */
/**********************************************************************/

struct CCArgs
{
	struct   Line  *Line;
	ULONG           CC;
	ULONG           MaxLen;
	UBYTE          *Buffer;
	UBYTE           Args[40];
	UBYTE           Format[30];
};

/**********************************************************************/
/*                     Defines for the user flags                     */
/**********************************************************************/

#define  UF_MORE        0x00000001
#define  UF_EXPERT      0x00000002
#define  UF_HOTKEYS     0x00000004
#define  UF_NO8BIT      0x00000008
#define  UF_VALIDATED   0x00000010
#define  UF_CLRSCR      0x00000020
#define  UF_LOGUSER     0x00000040
#define  UF_ANSICOLR    0x00000080     /* Was UF_NOANSI */
#define  UF_CRLF        0x00000100
#define  UF_VALID       0x00000200
#define  UF_WANTRIP     0x00000400
#define  UF_DELETED     0x00000800
#define  UF_AMIGAANSI   0x00001000
#define  UF_PROTECTED   0x00002000
#define  UF_NORESTRICT  0x00004000
#define  UF_ANSIPOS     0x00008000
#define  UF_ANSICOMP    0x00010000
#define  UF_ANIMPROMPTS 0x00020000
#define  UF_ANSIMUSIC   0x00040000

#define  UF_USEANSI     (UF_ANSICOLR|UF_ANSIPOS|UF_ANSICOMP)

/**********************************************************************/
/*          Structures involved in the user list traversing           */
/**********************************************************************/

struct   UserIdx
{
	UBYTE           Name[40];
	UBYTE           Alias[40];
	ULONG           DataPos;
};

#define  KEY_ALPHAFORWARD     0x0001
#define  KEY_ALPHAREVERSE     0x0002
#define  KEY_CHRONOFORWARD    0x0003
#define  KEY_CHRONOREVERSE    0x0004

struct   UserKey
{
	struct   UserIdx  *uk_Current;      /* The current Index structure   */

	/* Data beyond this point is private and subject to change */
};

/**********************************************************************/
/*                        Main User structure                         */
/**********************************************************************/

struct   User
{
	UBYTE           ur_Name[40];
	UBYTE           ur_Alias[40];
	UBYTE           ur_Password[20];
	UBYTE           ur_VoicePhone[20];
	UBYTE           ur_DataPhone[20];
	UBYTE           ur_Street[40];
	UBYTE           ur_Town[20];
	UBYTE           ur_County[20];
	UBYTE           ur_PostCode[12];
	UBYTE           ur_Country[20];

	long            ur_FirstCall;
	long            ur_LastCall;
	ULONG           ur_LastBaudRate;
	ULONG           ur_Flags;

	UBYTE           ur_RIPfsz;
	UBYTE           ur_LogOffCode;      /* See LO_* defines */
	
	UWORD           ur_Expire;          /* Number of days from first call the user may logon */

	long            ur_Credit;

	UWORD           ur_MinsLeft;
	UWORD           ur_NumCalls;
	UWORD           ur_MaxCallsPerDay;
	UWORD           ur_MaxMinsPerCall;
	UWORD           ur_MaxMinsPerDay;
	
	UWORD           ur_DOB_Year;           /* Date of birth */
	UBYTE           ur_DOB_Mon;
	UBYTE           ur_DOB_Day;

	UBYTE           ur_Access;
	UBYTE           ur_Computer;
	UBYTE           ur_ScrWidth;
	UBYTE           ur_ScrHeight;
	UBYTE           ur_MenuSet;
	UBYTE           ur_Language;
	UBYTE           ur_CallsToday;   
	UBYTE           ur_Gender;
};

/*
** Defines for ur_Gender
*/

#define  GEN_UNKNOWN    0
#define  GEN_MALE       1
#define  GEN_FEMALE     2

/*
** Defines for ur_LogOffCode
*/

#define  LO_NORMAL      0
#define  LO_INACTIVE    1
#define  LO_BOOTED      2
#define  LO_NOCARRIER   3
#define  LO_NOTIME      4

/*
** Flags for reading
*/

#define  IR_RAW         0x00000001
#define  IR_SECRET      0x00000002
#define  IR_NOSHOW      0x00000004
#define  IR_ONLYCHARS   0x00000008
#define  IR_ONLYOPTS    0x00000010
#define  IR_ONLYNUMS    0x00000020
#define  IR_ONLYALPHA   0x00000040
#define  IR_MAKEUPPER   0x00000080     /* Make all characters uppercase */
#define  IR_MAKELOWER   0x00000100     /* Make all characters lowercase */
#define  IR_DEFAULTSTR  0x00000200     /* Use default string            */
#define  IR_NOWAIT      0x00000400     /* Return immediatly             */
#define  IR_ISMENU      0x00000800     /* Allow command stacking        */
#define  IR_NONEWLINE   0x00001000     /* Do not append a new line      */
#define  IR_XOPTS       0x00002000
#define  IR_MAKENICE    0x00004000
#define  IR_HISTORY     0x00008000
#define  IR_ADDHIST     0x00010000
#define  IR_NOCD        0x00020000     /* Don't check for carrier loss  */
#define  IR_EOLRET      0x00040000     /* Return when buffer is full    */
#define  IR_CHECKMSGS   0x00080000     /* Return when line message(s) are available */
#define  IR_STRIP       0x00100000     /* Strips leading/trailing spaces */

/**********************************************************************/
/*                   Defines for the dc_Flags field                   */
/**********************************************************************/

#define  DCF_OPENDEV    0x00000001
#define  DCF_SCRIPT     0x00000002
#define  DCF_USEDSR     0x00000004
#define  DCF_CHECKCAR   0x00000008
#define  DCF_USERADBOOG 0x00000010
#define  DCF_FLOATBAUD  0x00000020

/**********************************************************************/
/*                      The device config struct                      */
/**********************************************************************/

struct   Dev_Cfg
{
	UBYTE              dc_Device[40];
	ULONG              dc_Unit;
	ULONG              dc_DevBuffSize;
	ULONG              dc_Baud;
	ULONG              dc_BrkTime;
	ULONG              dc_Flags;
	UBYTE              dc_Ring[40];
	UBYTE              dc_NoCon[40];
	UBYTE              dc_Con[40];
	UBYTE              dc_Answer[40];
	UBYTE              dc_HangUp[40];
	UBYTE              dc_Init[40];
	UBYTE              dc_DataLen;
	UBYTE              dc_StopBits;
	UBYTE              dc_Handshake;
	UBYTE              dc_Parity;
};

/*
** IEMSI structure
**
** Only valid if ( line->le_TermCaps & DOES_IEMSI )
*/

struct   ie_info
{
	UBYTE    *ie_Buff;      /* Buffer containing data sent from user */
	UBYTE    *ie_Name;      /* These fields are offsets into ie_buff */
	UBYTE    *ie_Alias;
	UBYTE    *ie_Location;
	UBYTE    *ie_DataNum;
	UBYTE    *ie_VoiceNum;
	UBYTE    *ie_Password;
	UBYTE    *ie_BDate;
	UBYTE    *ie_CrtDef;
	UBYTE    *ie_Protocols;
	UBYTE    *ie_Capabs;
	UBYTE    *ie_Requests;
	UBYTE    *ie_Software;
	UBYTE    *ie_TransTab;
};

/*
** The line structure as obtained by GetLine()
*/

struct   Line
{
	struct   User        *le_User;            /* Pointer to user online or NULL            */
	struct   Dev_Cfg     *le_DevCfg;          /* Pointer to line's device config or NULL   */
	ULONG                 le_Number;          /* Number of this line                       */
	ULONG                 le_RealBaud;        /* Read baud rate (or NULL for local)        */
	BOOL                  le_Carrier;         /* Carrier present if TRUE                   */
	UBYTE                *le_Status;          /* Status string                             */
	struct   Language    *le_Lang;            /* Current language                          */
	ULONG                 le_MinsOnline;      /* Minutes line has been active              */
	UWORD                 le_ConnectFlags;    /* Connection flags (See CD_* defines)       */
	UBYTE                *le_Anim;            /* ANSI Prompt Animation Pointer             */
	UWORD                 le_AnimPos;         /* Position in the ANSI Anim                 */
	UWORD                 le_Type;
	UWORD                 le_TermCaps;        /* Terminal capabilities                     */
	struct   ie_info     *le_IEmsi;           /* IEMSI information                         */
	UWORD                 le_Inactive;        /* Number of seconds since user last pressed a key */
	ULONG                 le_LockBaud;        /* Computer -> Modem Baud                    */
	struct   List         le_Access;          /* Access restriction list                   */
	UWORD                 le_Flags;
	UBYTE                *le_TrPtrI;          /* Pointer to current input translation table or NULL (usually points to le->le_TransI) */
	UBYTE                *le_TrPtrO;          /* Pointer to current output translation table or NULL (usually points to le->le_TransO) */
	UBYTE                 le_TransI[256];     /* Input translation */
	UBYTE                 le_TransO[256];     /* Output translation */
	time_t                le_LogOnTime;       /* Time user logged on */
	UWORD                 le_Uploads;         /* Number of files uploaded IN THIS CALL - copied to last callers list */
	UWORD                 le_Downloads;       /* Number of files downloaded */
	ULONG                 le_UploadsBytes;    /* Number of bytes uploaded */
	ULONG                 le_DownloadsBytes;  /* Number of bytes downloaded */
	UWORD                 le_Messages;        /* Number of messages written IN THIS CALL */
	UWORD                 le_StatsFlags;      /* Flags for statistics only */
};

/*
** Defines for le->le_Type
**
** le->le_DevCfg is only valid for TYPE_SERIAL
*/

#define  TYPE_LOCAL     0
#define  TYPE_SERIAL    1

#define  TYPE_ACCESS    0xFFFF

/*
** Defines for the (struct Line *)le->le_TermCaps field
*/

#define  DOES_ANSI      0x0001
#define  DOES_RIP       0x0002
#define  DOES_IEMSI     0x0004
#define  DOES_8BIT      0x0008      /* For forcing 8 bit */
#define  DOES_ZAPP      0x0010

/*
** Flags for le->le_Flags
*/

#define  LE_NOTAKETIME  0x0001

/*
** Flags for (struct Line *)le->le_StatsFlags AND LastCaller->StatsFlags
*/

#define  LE_STATS_PAGED  0x0001            /* Paged SysOp, or SysOp broke in */
#define  LE_STATS_NEW    0x0002            /* New user on this call */
#define  LE_STATS_MAILER 0x0004            /* Call was a Fido mailer (or InterNet, maybe) */

/*
** The flags that can be obtained by GetLineStatus()
*/

#define  LN_OPEN        0x00000001        /* Redundant                           */
#define  LN_LISTEN      0x00000002        /* Redundant                           */
#define  LN_DIE         0x00000004        /* Pending kill line                   */
#define  LN_ALLOWBRK    0x00000008
#define  LN_WRITING     0x00000010        /* Writing data                        */
#define  LN_READING     0x00000020        /* Reading data                        */
#define  LN_INDOOR      0x00000040        /* In a CLIDoor                        */
#define  LN_DEAD        0x00000080
#define  LN_DONECP      0x00000100        /* Redundant                           */
#define  LN_KILLSCR     0x00000200        /* Pending kill script                 */
#define  LN_DEVOPEN     0x00000400        /* Device is open (TYPE_SERIAL only)   */
#define  LN_INCHAT      0x00000800        /* In chat                             */
#define  LN_STOPCHAT    0x00001000        /* Pending chat end                    */
#define  LN_LOCAL       0x00002000        /* Line is local                       */
#define  LN_DYING       0x00004000        /* Line has received LN_DIE            */
#define  LN_CNTMINS     0x00008000        /* Counting minutes online             */
#define  LN_ONLINE      0x00010000        /* Line is Online                      */
#define  LN_BOOTUSER    0x00020000        /* Boot user off!                      */
#define  LN_OUTOFF      0x00040000        /* Output is turned off                */
#define  LN_RIPENABLED  0x00080000        /* RIP is enabled                      */
#define  LN_LOCALINPUT  0x00100000        /* Last input came from local          */
#define  LN_SERIALOFF   0x00200000        /* Serial output is disabled           */
#define  LN_NOCD        0x00400000
#define  LN_NOCHAT      0x00800000
#define  LN_NOSERIN     0x01000000
#define  LN_ICONIFY     0x02000000        /* Watchers iconify setting            */

struct   P_Sem
{
	struct   SignalSemaphore    ps_Sem;
	UBYTE                       ps_Name[10];
	void                       *ps_Data;
};

struct   P_Sems
{
	struct   P_Sem    Func_Sem;
	struct   P_Sem    User_Sem;
	struct   P_Sem    Prog_Sem;
	struct   P_Sem    Line_Sem;
	struct   P_Sem    Dead_Sem;
	struct   P_Sem    GFnc_Sem;
};

/**********************************************************************/
/*               Saved Data for recording system stats                */
/**********************************************************************/

struct SavedData
{
	ULONG sd_Calls;         // Total calls
	ULONG sd_LocalCalls;    // Total local calls
	ULONG sd_CallsToday;    // Total calls today

	ULONG sd_Users;         // Unused!
	ULONG sd_LogIns[24];    // Calls per each hour today
};

struct ExtraSaved
{
	ULONG    es_DaysUp;
	UBYTE    es_HoursUp;
	UBYTE    es_MinsUp;

	time_t   es_FirstRun;

	ULONG    es_Active[24];
	ULONG    es_Online[24];

	/* nOw2: 27/12/00 - centralised statistics management follows. */

	UWORD    es_FilesUpToday;
	UWORD    es_FilesDownToday;
	ULONG    es_BytesUpToday;
	ULONG    es_BytesDownToday;

	UWORD    es_FilesUpRecord;
	time_t   es_FilesUpRecordDate;
	UWORD    es_FilesDownRecord;
	time_t   es_FilesDownRecordDate;
	ULONG    es_BytesUpRecord;
	time_t   es_BytesUpRecordDate;
	ULONG    es_BytesDownRecord;
	time_t   es_BytesDownRecordDate;

	ULONG    es_MsgsToday;
	ULONG    es_MsgsRecord;
	time_t   es_MsgsRecordDate;

	UWORD    es_PagesToday;             // SysOp pages
	UWORD    es_PagesRecord;
	time_t   es_PagesRecordDate;

	UWORD    es_NewToday;               // New users
	UWORD    es_NewRecord;
	time_t   es_NewRecordDate;

	ULONG    es_CallsRecord;            // For today's calls see SavedData
	time_t   es_CallsRecordDate;
};

/**********************************************************************/
/*                      Various System Variables                      */
/**********************************************************************/

struct SysVars
{
	UWORD    sv_UserIdxBuff;
	UWORD    sv_CarChkInterval;
	UWORD    sv_InacTime;
	UBYTE    sv_SystemName[60];
	UBYTE    sv_SystemLocation[60];
	UBYTE    sv_SysOpName[40];
	UWORD    sv_LastCalls;
	UWORD    sv_BufferedText;
	ULONG    sv_MaxTextSize;
	ULONG    sv_Flags;
};

#define     SVF_COUNTLOCAL    0x00000001

/**********************************************************************/
/*                               Paths                                */
/**********************************************************************/

struct Paths
{
	UBYTE    User[50];
	UBYTE    Text[50];
	UBYTE    Scripts[50];
	UBYTE    Mail[50];
	UBYTE    File[50];
	UBYTE    Doors[50];
	UBYTE    Logs[50];
	UBYTE    Inbound[50];
	UBYTE    Outbound[50];
};

/*
** Language config as found in Shared->LangList
*/

struct   Language
{
	struct   Node      Node;
	APTR               Lang;
	UBYTE             *Name;
	ULONG              Number;
	UBYTE              Tran[16];
	UBYTE             *Press_Return;
	UBYTE             *YesNo;
	UBYTE             *YesNoCont;
	UBYTE             *Def_Yes;
	UBYTE             *Def_No;
	UBYTE             *Yes;
	UBYTE             *No;
	UBYTE             *Cont;
	UBYTE             *PrivChar;
	UBYTE             *CC_Yes;
	UBYTE             *CC_No;
	UBYTE             *CC_On;
	UBYTE             *CC_Off;
	UBYTE             *CC_StrForm;
	UBYTE             *CC_NumForm;
	UBYTE             *CC_TimeForm;
	UBYTE             *More;
};

/*
** Last caller structure for Shared->LastCalls
*/

struct   LastCaller
{
	struct   Node         Node;               /* For linkage                                        */
	UWORD                 Line;               /* Line number                                        */
	UBYTE                 Name[40];           /* Users name                                         */
	time_t                LogOn;              /* LogOn time                                         */
	time_t                LogOff;             /* LogOff time                                        */
	struct   User         User;               /* _Copy_ of the user structure as it was at logoff   */
	UWORD                 Uploads;            /* Number of files uploaded IN THIS CALL - copied to last callers list */
	UWORD                 Downloads;          /* Number of files downloaded */
	ULONG                 UploadsBytes;       /* Number of bytes uploaded */
	ULONG                 DownloadsBytes;     /* Number of bytes downloaded */
	UWORD                 Messages;           /* Number of messages written IN THIS CALL */
	UWORD                 StatsFlags;         /* Flags for statistics only; see Line->le_StatsFlags */
};

/*
** Information from Zeus.key
*/

struct   KeyInfo
{
	struct   DateStamp   Created;    /* DateStamp of Key creation  */
	ULONG                Serial;     /* Zeus serial number         */
	UBYTE                Name[40];   /* Name of registered sysop   */
};

/**********************************************************************/
/*                     The shared data structure                      */
/**********************************************************************/

struct SharedData
{
	struct   DosLibrary        *DOSBase;
	struct   GfxBase           *GfxBase;
	struct   IntuitionBase     *IntuitionBase;
	struct   Library           *DiskfontBase;
	struct   Library           *GadToolsBase;
	struct   Library           *RexxSysBase;
	struct   Library           *UtilityBase;
	struct   Library           *WorkbenchBase;
	struct   Library           *PipeUtilBase;

	ULONG                       RsrcCount;    /* Private */
 
	struct   P_Sems            *Sems;         /* Private */
	struct   SavedData          Saved;
	struct   Process           *PipeProc;

	ULONG                       HiLine;       /* High number of lines present */
	ULONG                       Active;       /* Number of lines active */

	struct   Line_Node        **ln_array;     /* Private */
	
	ULONG                       Users;        /* How many users */
	
	struct   SysVars           *SysVars;      /* System variables */
	struct   Paths             *Paths;        /* Paths config */
	struct   List               LangList;     /* List of defined languages */

	BOOL                        SysOp;        /* TRUE - SysOp available for chat */
	
	struct   List               LastCalls;    /* Last callers of this session */
	UWORD                       NumLast;      /* Number of last callers in list */
	
	UWORD                       ZVer;         /* Version of Zeus */
	UWORD                       ZRev;         /* Revision of Zeus */
	
	long                        Globals;      /* Number of global commands defined */
	
	struct   SignalSemaphore   *LCSem;        /* Semaphore for the last calls list - use it! */
	
	struct   List               MSList;       /* Menu Set list */
	struct   SignalSemaphore    MSSem;        /* Menu Set semaphore */
	
	struct   List               BanList;      /* Banned user wildcards */
	struct   SignalSemaphore    BanSem;

	struct   KeyInfo           *KeyInfo;      /* Info from Zeus.key */

	struct   List               TransList;    /* Character translations */

	struct   ExtraSaved        *Extra;        /* Extra Zeus Stats */

	struct   MsgPort           *SWPort;       /* Private! */
};

/*
** Flags for writing
*/

#define  WF_NOMORE         0x00000001     /* No more prompts            */
#define  WF_ISMENU         0x00000002     /* Is a menu                  */
#define  WF_BREAK          0x00000004     /* Allow breaking             */
#define  WF_NOCOOK         0x00000010     /* Dont 'cook' the data       */
#define  WF_SPECIALMORE    0x00000020     /* Return when screen is full */
#define  WF_USEMOREYN      0x00000040     /* Prompt is 'More Y/N'       */
#define  WF_RIPONLY        0x00000080     /* Only display if RIP        */
#define  WF_TXTONLY        0x00000100     /* Only display if not RIP    */
#define  WF_USEMOREYNC     0x00000200     /* Prompt is 'More Y/N/C      */
#define  WF_BUFFER         0x00000400     /* Buffer this file           */
#define  WF_ISRIP          0x00001000     /* Is RIP graphics            */
#define  WF_NORESET        0x00002000     /* Don't send RIP reset code  */
#define  WF_NOEXPAND       0x00004000     /* Don't expand control codes */
#define  WF_LOCALDIR       0x00008000
#define  WF_NOCD           0x00010000
#define  WF_NOANSIPOS      0x00020000
#define  WF_NOANSICOMP     0x00040000

/**********************************************************************/
/*                  The structure passed to ReadLine                  */
/**********************************************************************/

struct ReadVars
{
	long      LineNum;
	UBYTE    *Buff;
	UBYTE    *Opts;
	ULONG     MaxLen;
	ULONG     MinLen;
	ULONG     Flags;
};

/**********************************************************************/
/*                  Structure used in chat routines                   */
/**********************************************************************/

struct   ChatChar
{
	UBYTE    ch;
	UBYTE    type;
};

#define  TYPE_USER   0x00
#define  TYPE_SYSOP  0x01
#define  TYPE_FINISH 0x02
#define  TYPE_START  0x03

/**********************************************************************/
/*             Structures and defines for Control Gadgets             */
/**********************************************************************/

struct   ConMsg
{
	struct   Message     cm_Msg;
	UWORD                cm_GadID;
	UWORD                cm_Action;
};

#define  GAD_SELECT     1     /* Codes in ConMsg->cm_Action */
#define  GAD_QUIT       2
#define  WIN_ICONIFY    3

/*
** Structures and defines for use when transfering files
*/

#define  XFER_SEND            1     /* Mode options */
#define  XFER_RECEIVE         2

#define  FILE_ABORTED         0     /* Codes found in XferInfoNode->Status */

#define  FILE_SENT            1
#define  FILE_PARTIAL_SEND    2

#define  FILE_RECEIVED        3
#define  FILE_PARTIAL_RECEIVE 4

struct   XferInfo
{
	struct   List      Send;            /* List of XferInfoNodes      */
	struct   List      Recv;            /* List of XferInfoNodes      */
	struct   Line     *le;
	struct   XprProto *xpr;

	ULONG              FilesSent;       /* Number of files sent       */
	ULONG              FilesRcvd;       /* Number of files received   */

	ULONG              SendCPS;         /* Average send cps           */
	ULONG              RecvCPS;         /* Average receive cps        */
	ULONG              SendBytes;       /* Number of bytes sent       */
	ULONG              RecvBytes;       /* Number of bytes received   */
	ULONG              SendErrs;        /* Number of send errors      */
	ULONG              RecvErrs;        /* Number of receive errors   */
	ULONG              SendTimeOuts;    /* Number of send timeouts    */
	ULONG              RecvTimeOuts;    /* Number of receive timeouts */
	ULONG              Mode;            /* Initial mode               */
};

struct   XferInfoNode
{
	struct   Node     Node;
	UBYTE             Filename[108];
	ULONG             Length;           /* Length of full file (or -1 if not known) */
	ULONG             Xferd;            /* Amount of bytes transfered */
	UWORD             Status;           /* Final status of file */
	ULONG             Cps;              /* Chars per second */
};

/*
** Defines for Pipe functions
*/

#define     PIPE_READ      1
#define     PIPE_WRITE     2
#define     PIPE_EOF       1

#define     PIPE_REQ_WRITE    1           /* Request Types */
#define     PIPE_REQ_READ     2
#define     PIPE_REQ_ABORT    3

/*
** Defines for LineOutputMode
*/

#define     OUTPUT_ON      0
#define     OUTPUT_OFF     1

/*
** Bulletin related structures/defines
*/

struct   Bulletin
{
	LONG     Number;           /* Number of bulletin   */
	UBYTE    Name[50];         /* Name of bulletin     */
	UBYTE    Key[10];          /* Key                  */
	UBYTE    Filename[100];    /* Filename             */
	time_t   Created;          /* Creation date        */
	time_t   Expires;          /* Expiry date or -1    */

	UBYTE    MinAcc;           /* Minimum access needed */
	UBYTE    MaxAcc;           /* Maximum access needed */
};

struct   BltnNode
{
	struct   Node        Node; /* Node.ln_Name points to Bltn.Key */
	struct   Bulletin    Bltn;
};

/*
** Tags for ReadLineTags and ReadLineTagList
*/

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

#ifndef USE_DOORLIB

/*
** Prototypes for pl.lib & plnb.lib
*/

long LPuts( long, UBYTE *, ULONG );
long ReadLineTags( long, UBYTE *, ... );

#else

/*
** Prototypes for zdoor.lib & zdoornb.lib
*/

long LPuts( UBYTE * );
long ReadLineTags( UBYTE *, ... );

#endif

/*
** Watchers
*/

struct   WatcherMsg
{
	struct   Message      Msg;
	ULONG                 Code;
	ULONG                 Result;
	long                  Line;      /* Line Number */
	ULONG                 Flags;
};

/*
** Defines for WatcherMsg->Code
*/

#define  WM_OFFLINE     0           /* End of session */
#define  WM_ONLINE      1           /* Start of session */
#define  WM_RIPON       2
#define  WM_RIPOFF      3
#define  WM_ICONIFY     4
#define  WM_UNICONIFY   5
#define  WM_USERON      6           /* User has been PUTONLINE */
#define  WM_NEWSTATUS   7           /* New line->le_Status */
#define  WM_TOFRONT     8           /* Move Screen/Window to front */
#define  WM_TOBACK      9           /* Move Screen/Window to back */
#define  WM_UPDATEUSER  10          /* User details have been changed */
#define  WM_QUIT        11

/*
** Defines for WatcherMsg->Result
*/

#define  WR_MOVETOTOP   1
#define  WR_MOVETOBOT   2

/*
** Line number you should pass AddWatcher to receive messages on all lines
*/

#define  AW_ALLLINES    (0x80000000)

/*
** Priorities for MsgPort->mp_Node.ln_Pri when adding a watcher with a
** screen.
*/

#define  AW_ICONIFIED      (100)
#define  AW_UNICONIFIED    (-100)

/*
** Watcher function key structures/defines
*/

#define  FKEY_EXEC      0           /* Types for FKey->Type */
#define  FKEY_TEXT      1

struct   FKey
{
	UBYTE    Data[60];
	UWORD    Type;
};

struct   WatcherSem
{
	struct   SignalSemaphore   ws_Sem;
	UBYTE                      ws_Name[20];
	struct   FKey              ws_Keys[20];
	UWORD                      ws_OpenCnt;
};

#define  WATCHSEMNAME      "WATCHKEYSEM"

/*
** Flags returned by GetConnectMode()
*/

#define  CD_ARQ      0x0001
#define  CD_HST      0x0002
#define  CD_SYNC     0x0004
#define  CD_V42BIS   0x0008
#define  CD_LAPM     0x0010
#define  CD_MNP      0x0020
#define  CD_MNP5     0x0040

/*
** Menu Set Flags
*/

#define  MS_RIP         0x0001
#define  MS_AMIGA       0x0002
#define  MS_8BIT        0x0004
#define  MS_ANSICOLR    0x0008
#define  MS_NOMORE      0x0010
#define  MS_ANSIPOS     0x0020
#define  MS_ANSICOMP    0x0040

struct   MenuSet
{
	struct   Node      ms_Node;
	UWORD              ms_Number;
	UBYTE              ms_Name[50];
	UWORD              ms_Flags;
};

struct   BanNode
{
	struct   MinNode   bn_Node;
	UBYTE              bn_Data[50];
};

#define     LSC_PATHS         0x0001
#define     LSC_SYSVARS       0x0002
#define     LSC_LANGS         0x0004
#define     LSC_MENUS         0x0008
#define     LSC_BANNED        0x0010
#define     LSC_COMPUTERS     0x0020
#define     LSC_ALL           0xFFFF

#define     LSC_RET_OK           0
#define     LSC_RET_DOSERR       1
#define     LSC_RET_MEMERR       2
#define     LSC_RET_NOZEUS       3
#define     LSC_RET_OLDCFG       4
#define     LSC_RET_NOCFG        5
#define     LSC_RET_DEFLANGERR   6
#define     LSC_RET_LANGERR      7

/*
** Structures and defines for Line access
**
** These nodes are found in the line structure in the le_Access list
*/

struct   LineAcc
{
	struct   MinNode     Node;
	UBYTE                FHor, FMin;          /* From time                  */
	UBYTE                THor, TMin;          /* To time                    */
	UWORD                Restrict;            /* See ACR_* defines          */
	UWORD                Flags;               /* Flags - see ACC_* defines  */
	ULONG                MinBaud, MaxBaud;    /* Min/Max Baudrates          */
	UBYTE                MinAcc, MaxAcc;      /* Min/Max Access levels      */
};

								/* UWORD */
#define  ACC_MINBAUD    0x00000002
#define  ACC_MAXBAUD    0x00000004
#define  ACC_MINACCESS  0x00000008
#define  ACC_MAXACCESS  0x00000010
#define  ACC_DIALBACK   0x00000020  /* Ring once, wait 10s, ring back to connect. */

#define  ACR_NORMAL     (0)
#define  ACR_NOLOGON    (1)
#define  ACR_MAILONLY   (2)
#define  ACR_NOANSWER   (3)

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d)   \
	((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#define  EDIT_ANSI      (-1)
#define  EDIT_NOANSI    (-2)

struct   EditOpts
{
	struct   List     *History;
	long               Num;
	UBYTE             *Buff;
	UBYTE             *Work;
	UBYTE             *Opts;
	UBYTE             *XOpts;
	ULONG              MinLen;
	ULONG              MaxLen;
	ULONG              Flags;
};

/*
** ZSegs - Returned by ZLoadSeg
*/

struct   ZSegs
{
	struct   Segment  *Seg;       /* May be NULL */
	BPTR               SegList;
	BPTR               SegDir;
};

/*
** TransNode - Found on Shared->TransList
*/

struct TransNode
{
	struct   Node        n;
	UBYTE                Tag[12];       /* Tag                      */
	UBYTE                Name[30];      /* Name of this translation */
	UBYTE                ITab[256];     /* Input table              */
	UBYTE                OTab[256];     /* Output table             */
};

/*
** User ExtraData
*/

#define     ID_CTRI     MAKE_ID( 'C', 'T', 'R', 'I' )    /* Translation In  */
#define     ID_CTRO     MAKE_ID( 'C', 'T', 'R', 'O' )    /* Translation Out */

/*
** Defines for SetCLIMode()
*/

#define  MORE_OFF    (0)
#define  MORE_ON     (1)

/*
** InterNet config structure
*/

struct   IN_Cfg
{
	UBYTE    UserName[50];
	UBYTE    Addr[60];
	UBYTE    Organization[50];
	UBYTE    NewsReceipt[50];
	UBYTE    NewsDir[256];
	UBYTE    TempDir[256];
	UBYTE    SpoolDir[256];
	UBYTE    MailDir[256];
	UBYTE    SendNews[256];
	UBYTE    SendMail[256];
	UBYTE    RNews[256];
	UBYTE    TimeZone[5];
	UBYTE    GateDomain[30];
	UWORD    GateType;
	long     GMTOff;
	UWORD    HdrDef;
	UWORD    Flags;
	ULONG    Indexes;
	UWORD    YZone, YNet, YNode, YPoint;
	UWORD    GZone, GNet, GNode, GPoint;
	UWORD    WordWrap;
	UBYTE    MailDelim[256];
	UBYTE    NewsDelim[256];
	UBYTE    XTempDir[256];
};

#define  GATE_LOCAL     (0)
#define  GATE_PRIVATE   (1)
#define  GATE_PUBLIC    (2)

#define  IF_MIMERECOG   (1<<0)
#define  IF_EXTRACT     (1<<1)
#define  IF_XUUE        (1<<2)
#define  IF_XMULTIUUE   (1<<3)
#define  IF_XMIME       (1<<4)

struct   IN_Req
{
	struct   IN_Cfg      *cfg;
	struct   List        *hdrs;
	struct   List        *gacc;
};

struct   HdrLine
{
	UBYTE    Hdr[81];
	UBYTE    Type;
};

#define  HDR_PORT       (0)
#define  HDR_KLUDGE     (1)
#define  HDR_DELETE     (2)

struct   HdrNode
{
	struct   MinNode  n;
	struct   HdrLine  h;
};

struct   GateAccNode
{
	struct   MinNode  n;
	UWORD             Type;
	UWORD             Zone;
	UWORD             Net;
	UWORD             Node;
	UWORD             Point;
};

#define  GACC_GATE      (0)
#define  GACC_NOGATE    (1)

#define  ADDR_WILD      (0xFFFF)

/*
** Return codes from DoEdit()
*/

#define  EDIT_RETURN    (-1)
#define  EDIT_EOL       (-2)

/*
** Structure for GetFID()
*/

struct   fid
{
	UBYTE    *name;
	UBYTE    *os;
	UBYTE    *type;
	UBYTE    *contact;

	UWORD     zone, net, node, point;
};

#endif      /* LIBRARIES_ZEUS_H */
