
#ifndef PLMSG_H
#define PLMSG_H

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/semaphores.h>

#include <dos/dos.h>
#include <dos/datetime.h>
#include <time.h>

#include <zconfig.h>
//#include <libraries/zmf.h>


/**********************************************************************/
/* Used in MsgGlobals (further down).                                 */
/* Contains the number of areas existing.  This is not expandable -   */
/* it is part of the MsgGlobals structure rather than a pointer.      */
/**********************************************************************/
struct  PlMsgData
{
		  ULONG   Areas;
} ;

/**********************************************************************/
/* The following are various ways in which Zeus stores FTN addresses. */
/**********************************************************************/

struct  FidoAddr
{
		  UWORD   Zone;
		  UWORD   Net;
		  UWORD   Node;
		  UWORD   Point;
		  char    Domain[20];
} ;

struct  AdrNode
{
		  struct  Node            node;
		  struct  FidoAddr        Adr;
} ;


struct  FourDFido
{
		  ULONG   Zone;
		  ULONG   Net;
		  ULONG   Node;
		  ULONG   Point;
} ;

struct  FiveDFido
{
		  ULONG   Zone;
		  ULONG   Net;
		  ULONG   Node;
		  short   Point;
		  short   Domainref;
} ;

struct  FidoAddress
{
		  UWORD   Zone;
		  UWORD   Net;
		  UWORD   Node;
		  UWORD   Point;
		  char    Domain[8];
} ;

/**********************************************************************/
/*                         struct Msg->Flags                          */
/* Zeus's own message flags.  Do not confuse with FidoNet flags.      */
/**********************************************************************/

#define MI_PRIVATE              0x0001
#define MI_FILEATTACHED         0x0002
#define MI_REPLY                0x0004
#define MI_DELETED              0x0008
#define MI_SENDERANONYMOUS      0x0010
#define MI_RECEIVEANONYMOUS     0x0020
#define MI_DELETEUPONRECEIVE    0x0040
#define MI_RECEIVED             0x0080
#define MI_CRASH                0x0100
#define MI_HOLD                 0x0200
#define MI_DIRECT               0x0400
#define MI_CROSSREPLY           0x0800
#define MI_AUDITREQ             0x1000        /* FidoNet audit request. */

struct  Msg
{
		  ULONG                   MsgSection;   /* Area containing message */
		  ULONG                   Number;       /* Number of message in area */
		  ULONG                   TextPosition; /* mbzeus.library - offset of text in datafile */
		  ULONG                   TextLength;   /* mbzeus.library - length of text */
		  char                    From[36];
		  char                    To[36];
		  char                    Subject[72];
		  time_t                  Date;         /* Seconds since 1/1/1970 */
		  time_t                  ArriveDate;
		  struct  FidoAddress     FromAddress;
		  struct  FidoAddress     DestAddress;
		  ULONG                   ReplyTo;      /* Msg number this is a reply to, or NULL */
		  ULONG                   NextReply;    /* A reply to this message, or NULL */
		  ULONG                   Flags;        /* MI_#? flags */
} ;

struct          MsgAreaFlags
{
		  int     Kludges         : 1;
		  int     Paths           : 1;
		  int     SeenBys         : 1;
		  int     PVTBase         : 1;
		  int     HiAscii         : 1;
		  int     Quoting         : 1;
		  int     Alias           : 1;
		  int     UserDelMsgs     : 1;
		  int     Moderator       : 1;
		  int     AnsiBase        : 1;
		  int     TextStyles      : 1;
		  int     MaskOut         : 1;
		  int     Unused          : 20;
} ;


/**********************************************************************/
/* These structures are read only, and accessed via                   */
/* MsgGlobals.  Please do not access the data files directly!         */
/**********************************************************************/

struct  GeneralCfg
{
		  short                    TextCol;
		  short                    QuoteCol;
		  short                    SeenByCol;
		  short                    KludgeCol;
		  char                     TextANSI[8];
		  char                     QuoteANSI[8];
		  char                     SeenByANSI[8];
		  char                     KludgeANSI[8];
} ;

struct  PlMsgConfig
{
		  struct  List            *AreaList;
		  struct  List            *Editors;
		  struct  List            *FidoAddresses;
		  struct  GeneralCfg       General;
		  struct  List             OriginList;
		  UWORD                    NumberOrigins;
		  struct  List             ExeCommands;
} ;

/* Used both Globally (in MsgGlobals) and for each
 * user (in UserXData struct MSVD
 */
struct  MsgVars
{
		  ULONG                   MessagesWritten;
		  ULONG                   MWrittenToday;
		  ULONG                   MaxMessagesDay;
		  time_t                  LastMessageWritten;
} ;

struct  OldSvd
{
		  ULONG                   Editor;
		  ULONG                   MsgArea;
		  ULONG                   Sig;
} ;

/* Used internally with the ARexx Topic functions.
 */
struct  TopicNode
{
		  struct  MinNode         tn_Node;
		  UBYTE                   tn_Subject[72];
		  struct  List            tn_List;
} ;

struct  TopicEntry
{
		  struct  MinNode         te_Node;
		  ULONG                   te_Number;
} ;

struct  TopicRead
{
		  struct  List             Subjects;
		  struct  TopicNode       *tn;
		  struct  TopicEntry      *te;
} ;

/* UserXData MSVD struct.  Used to store a users preferences.
 */
struct  MsgSavedData
{
		  ULONG                   Editor;   /* Editor number */
		  ULONG                   MsgArea;  /* Current message area at time of logoff */
		  ULONG                   Sig;      /* Current SIG number at time of logoff */
		  struct  MsgVars         Vars;     /* Statistics on user's messaging */
} ;


/* Internally used for the MsgFiltering ARexx commands.
 */
#define FILTER_OR       0
#define FILTER_AND      1

struct  MsgFilter
{
		  BOOL    InUse;
		  char    From[36];
		  char    To[36];
		  char    Subject[72];
		  char    Text[256];
		  time_t  After;
		  time_t  Before;
		  BOOL    Complete;
		  UWORD   Flag;
} ;

/* Data on each message area.
 */
struct  MsgIdx
{
		  ULONG                    Area;          /* Which area */
		  ULONG                    Low;           /* Lowest msg number */
		  ULONG                    High;          /* Highest msg number */
		  ULONG                    HighSentByNet; /* ZMP internal: last exported */
		  ULONG                    HSBInterNet;   /* INMP internal: Obsolete */
} ;
								
/**********************************************************************/
/*                              BASETYPE                              */
/**********************************************************************/

#define BT_LOCAL        0     /* Local only - no Tag name */
#define BT_ECHOMAIL     1     /* FTN Echo */
#define BT_NETMAIL      2     /* FTN Netmail area */
#define BT_UUCP         3     /* Newsgroup. Unused currently */
#define BT_PVTUUCP      4     /* E-Mail. Also unused */
#define BT_BAD          5     /* Bad area for Dupe, Illegal, etc. messages */
#define BT_BULLETIN     5     /* Same as BT_BAD for compatibility */

/*
** Format of Zeus:Config/Msg/MsgAreas.Dat
** Do not access directly, subject to change in fields and length!
*/

#ifndef MSGCONFIG

struct  svdarea
{
		  ULONG                    number;
		  UBYTE                    name[64];
		  UBYTE                    tagname[60];
		  ULONG                    unique;
		  UBYTE                    basetype;
		  ULONG                    maxmsgs;
		  UBYTE                    low_access;
		  UBYTE                    high_access;
		  struct  FidoAddress      address;
		  UWORD                    origin;
		  UWORD                    exe_cmd;
		  struct  MsgAreaFlags     flags;
		  UBYTE                    mblib_name     [30];
		  UBYTE                    gender;        /* 0 = N/A 1 = MALE 2 = FEMALE */
		  UBYTE                    age;
		  UBYTE                    usenet_followup[80];
} ;

/**********************************************************************/
/*          MsgArea - The Message Area Structure Definition           */
/**********************************************************************/

#define ORIGIN_RANDOM   0

struct  msgarea
{
		  ULONG                    number;      /* Area number */
		  UBYTE                    name[64];    /* Personalised name of area */
		  UBYTE                    tagname[60]; /* FidoNet/UseNet area tag */
		  ULONG                    unique;      /* DataManager internal: for tracking over renumbering */
		  UBYTE                    basetype;    /* Type of base, ONE of BT_#? */
		  ULONG                    maxmsgs;     /* Number of messages to trim to */
		  UBYTE                    low_access;  /* Lowest user access */
		  UBYTE                    high_access; /* Highest user access */
		  struct  FidoAddress      address;     /* For a FTN area, its address */
		  UWORD                    origin;      /* Number of origin to use, 0 = Random */
		  UWORD                    exe_cmd;     /* Number of executable to run on WriteMsg, 0 = none */
		  struct  MsgAreaFlags     flags;       /* MA_#? */
		  UBYTE                    mblib_name[30];/* Library used to store messages */
		  UBYTE                    gender;        /* 0 = N/A 1 = MALE 2 = FEMALE */
		  UBYTE                    age;         /* Minimum age for user to gain access */
		  UBYTE                    usenet_followup[1];
} ;


struct   WholeMessage
{
		  struct Msg               wm_Msg;
		  UBYTE                   *wm_Text;
		  ULONG                    wm_TextLength;
} ;


struct      WriteMsgStruct
{
		  char                     From[36];
		  char                     To[36];
		  char                     Subject[72];

		  struct  FidoAddress      FromAddress;
		  struct  FidoAddress      DestAddress;
		  struct  FidoAddress      RouteAddress;

		  LONG                     ReplyTo;
		  ULONG                    Flags;         /* Standard Message Flags */
		  ULONG                    MessageArea;

		  UBYTE                   *FidoText;      /* Use what you like of  */
		  UBYTE                   *Text;          /* these different texts */
		  UBYTE                    Origin[80];    /* Use in this order.    */
		  UBYTE                   *SeenBys;

		  /* Mail Processor Use */

		  UBYTE                   *FullText;
		  long                     FullTextLength;
		  BOOL                     NoPointerUpdate;
		  time_t                   OriginalTime;
} ;

/**********************************************************************/
/*                    UserMsgData (Internal Vars)                     */
/**********************************************************************/

struct          UserMsgData
{
		  ULONG                    CurrentArea;
		  struct  WholeMessage    *Msg;
		  ULONG                    Wrap;
		  LONG                     CurrentEditor;
		  ULONG                    CurrentSig;
		  struct  MsgFilter        Filter;
		  struct  WriteMsgStruct   WMS;
		  struct  MsgVars          Vars;
		  struct  TopicRead        Topic;
		  struct  msgarea          ChArea;
		  struct  List             AccessChanges;
} ;

/**********************************************************************/
/*                   PLMSG Internal User Structure                    */
/**********************************************************************/

struct  PMUser
{
		  BOOL                    un_OnLine;
		  ULONG                   un_LineNumber;
		  char                    un_UserName[40];
		  struct  UserMsgData     un_Umd;
} ;

#endif

struct  MsgLibraries
{
		  struct  SignalSemaphore   MsgBaseSem;
		  struct  List              MsgBaseList;
		  int                       NumberBases;
} ;

/**********************************************************************/
/*                          Message Globals                           */
/**********************************************************************/

struct  MsgGlobals
{
		  struct  PlMsgConfig     *mg_Config;
		  struct  PMUser          *mg_User;
		  struct  PlMsgData        mg_SystemData;
		  ULONG                    mg_NumOfUsers;
		  BOOL                     LogOpen, WinLogOpen;
		  BPTR                     LogHandle, WinLogHandle;
		  struct  SignalSemaphore  mg_LockMessageBases;
		  struct  Paths           *mg_PipePaths;
		  struct  MsgLibraries     mg_Libs;
		  struct  ListIndex       *ListIdx;                /* Msg Area List Indexing */
		  ULONG                    IndexEntries;
} ;

/**********************************************************************/
/*                Standard FTSC-1 FidoNet Msg File                    */
/**********************************************************************/

#define  FIDOPRIVATE    0x0001
#define  FIDOCRASH      0x0002
#define  FIDORECD       0x0004
#define  FIDOSENT       0x0008
#define  FIDOFILEAT     0x0010
#define  FIDOINTRANS    0x0020
#define  FIDOORPHAN     0x0040
#define  FIDOKILL       0x0080
#define  FIDOLOCAL      0x0100
#define  FIDOHOLD       0x0200
#define  FIDONOTUSED    0x0400
#define  FIDOFILEREQ    0x0800
#define  FIDORETNREQ    0x1000
#define  FIOOISRETREC   0x2000
#define  FIDOAUDITREQ   0x4000
#define  FIDOUPDTREQ    0x8000

struct fidohdr
{
		  char     from[36];
		  char     to  [36];
		  char     subject[72];
		  char     date[20];
		  short    timesRead;
		  short    destNode;
		  short    origNode;
		  short    cost;
		  short    origNet;
		  short    destNet;
		  short    destZone;
		  short    origZone;
		  short    destPoint;
		  short    origPoint;
		  short    replyTo;
		  short    attribute;
		  short    nextReply; 
} ;

/**********************************************************************/
/*                 pipeutil.library/SaveMessage Args                  */
/**********************************************************************/


/**********************************************************************/
/*                          Node Structures                           */
/**********************************************************************/

struct MsgAreaNode
{
		  struct  Node             nn_Node;
		  struct  msgarea         *area;
		  struct  SignalSemaphore  sem;
		  struct  MsgLinkNode     *linknode;
};

struct  AddressNode
{
		  struct Node              an_Node;
		  struct FourDFido         an_Address;
};

#define smfTO           0x0001
#define smfFROM         0x0002
#define smfSUBJECT      0x0004
#define smfTEXT         0x0008

struct  TagNameNode
{
		  struct  Node            Node;
		  char                    TagName[TAGNAME_LENGTH];
} ;

struct  ArcNameNode
{
		  struct  Node            Node;
		  char                    Archiver[80];
} ;


struct  WaitingMail
{
		  ULONG                   Area;
		  ULONG                   Number;
} ;


/**********************************************************************/
/*                       Message Area Accesses                        */
/**********************************************************************/

#define MA_ACCESS       0x0001
#define MA_READ         0x0002
#define MA_WRITE        0x0004
#define MA_WRITEPVT     0x0008
#define MA_PRIVATEREAD  0x0010
#define MA_KILLOWN      0x0020
#define MA_KILLALL      0x0040
#define MA_FILEATTACH   0x0080
#define MA_CRASHNETMAIL 0x0100
#define MA_SYSOP        0x0200
#define MA_MASKEDOUT    0x0400
#define MA_PACK         0x0800

struct  HighPointer
{
		  ULONG   Area;
		  LONG    Ptr;
} ;

/*

Old, obsolete versions.

struct
HighPointerNode
{
		  struct  MinNode         hpn_Node;
		  int                     hpn_Area;
		  ULONG                   hpn_Pointer;
} ;

struct  HiPointer
{
		  int                     Area;
		  ULONG                   Pointer;
} ;*/

struct  UserMsgAccess
{
		  ULONG   MaxMsgsPerDay;
} ;

struct  MsgInfo
{
		  char    Name[40]; /* Include Version Number */
		  char    Info[40];
		  char    Author[40];
} ;

struct  MsgLinkNode
{
		  struct  Node             mln_Node;
		  char                     mln_LibName[40];
		  struct  MsgInfo          mln_Info;
		  struct  Library         *mln_Lib;
} ;

struct  PointerNode
{
		  struct  Node    pn_Node;
		  struct  MsgIdx  pn_Idx;
} ;

struct  LoadNode
{
		  struct  Node    node;
		  char            Name[40];
} ;

struct  MsgScanKey
{
		  BPTR            descfH, datafH;
		  int             RecordsLoaded;
		  int             RecordsReturned;
		  int             Buffering;
		  char            *Records;
		  int             ScanFlag, Direction;
		  short           noBackScan;
		  ULONG           AreaNumber;
		  APTR            UserData;
} ;

struct  Executable
{
		  char                    Name[108];
		  WORD                    Pri;
		  UWORD                   Stack;
} ;

struct  ExeNode
{
		  struct  Node            node;
		  struct  Executable      exe;
} ;


/* Statistics structs generated by ZMP for each message area involved in
 * importing FTN/UseNet messages.
 */

#define  ZMP_STATS_NAME "Zeus:Config/Msg/MsgNetStats.dat"

struct   st_Header
{
	ULONG     id;          // in case of updates in the future, this can be checked.
	time_t    LastUpdate;
	UBYTE     reserved[16];
};

struct   st_Node
{
	BOOL      Msg;         // TRUE = msg area, FALSE = tick/file area.
	ULONG     Number;      // obviously, find corrisponding name from Zeus - but for msg passthrough?  ZMP has a bad passthrough implementation :(
	UWORD     DayItems[7]; // number of messages/files imported during the 24 hour period 6..0 days ago. Max 65k each.
								  // ordered as: [0]=6th day ago, [1]=5th day, [2]=4th day. [3]=3 [4]=2 [5]=1 [6]=0
	ULONG     DayBytes[7]; // bytes of messages/files imported during the 24 hour period 6..0 days ago. Max 4G each.
	ULONG     Total;       // number of messages/files imported since echo's creation.
};

struct   st_NodeMem
{
	struct    MinNode     n;
	BOOL      Msg;         // TRUE = msg area, FALSE = tick/file area.
	ULONG     Number;      // obviously, find corrisponding name from Zeus - but for msg passthrough?  ZMP has a bad passthrough implementation :(
	UWORD     DayItems[7]; // number of messages/files imported during the 24 hour period 6..0 days ago. Max 65k each.
	ULONG     DayBytes[7]; // bytes of messages/files imported during the 24 hour period 6..0 days ago. Max 4G each.
	ULONG     Total;       // number of messages/files imported since echo's creation.
};

#endif /* PLMSG_H */

