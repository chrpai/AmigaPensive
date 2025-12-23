/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* User access in message areas - MSG: */

struct Msg_Log
{
  char   Name[36];  /* Name of user (sorted)                   */
  long   High_Mess; /* Users last message read                 */
  UBYTE  Priv_Flag; /* This is used internally                 */
  char   uflag;     /* upgraded priv flags                     */
  char   dflag;     /* downgraded priv flags                   */
  char   special;   /* the user is not purgable from the area  */
};

/* Priv flags */

#define Enter_Priv      1
#define Kill_Priv       2
#define Hurl_Priv       4
#define Forward_Priv    8
#define Special_ReEdit  32
#define Special_Kill    64
#define Sysop_Access    64
#define Re_Edit         128

/* Message area structure - MSG:Area.bbs    FILE:Area.bbs */
/* DLGConfig:SIGS/<SIGNAME>.(FILE.MSG) */

struct Msg_Area
{
  char   Name[34];        /* text describing message area                  */
  SHORT  AADef;           /* this is unused                                */
  SHORT  Capacity;        /* number of messages allowed in area            */
  SHORT  Number;          /* number of the message area                    */
  char   Flag;            /* represents the type of area                   */
  SHORT  UpperLimit;      /* the renumber trigger value                    */
  unsigned char llevel;   /* lowest level that can gain access             */
  unsigned char ulevel;   /* highest level that can gain access            */
  char   path[8];         /* used only in file areas (alternate file path) */
  char   processor[12];   /* batchfile used to process the text            */
  char   origin[75];      /* custom origin line for this area              */
  unsigned char uwrite;   /* highest level that can write                  */
  unsigned char lwrite;   /* lowest level that can write                   */
  unsigned char ukill;    /* highest level that can kill                   */
  unsigned char lkill;    /* lowest level that can kill                    */
  unsigned char uforward; /* highest level that can forward                */
  unsigned char lforward; /* lowest level that can forward                 */
  unsigned char ucopy;    /* highest level that can copy                   */
  unsigned char lcopy;    /* lowest level that can copy                    */
  unsigned char uedit;    /* highest level that can edit                   */
  unsigned char ledit;    /* lowest level that can edit                    */
  unsigned char usysop;   /* highest level that can have sysop access      */
  unsigned char lsysop;   /* lowest level that can have sysop access       */
  char          forceset; /* not used                                      */
  unsigned char loadcharset; /* not used                                   */
  unsigned char savecharset; /* alternate character set for this area      */
  char unused[33];           /* not used                                   */
};

/* Number to use for private area */

#define PVTAREA     -1

/* Message area flags (types) */

#define AUTO_ACCESS_AREA   1  /* auto access                   */
#define HIDE_SEENBY        2  /* seenby lines will be hidden   */
#define HANDLES_AREA       4  /* handles used                  */
#define ECHO_AREA          8  /* echomail area                 */
#define NETMAIL_AREA       16 /* netmail area                  */
#define NEWSGROUP_AREA     32 /* newsgroup area                */
#define SIGNATURE_AREA     64 /* signature used in this area   */


/* Message header structure - First part of MSG:<number>/<number>.msg
   This header is followed by a null-terminated body of text */

struct Msg_Header
{
  char    From[36];         /* sender of the message                       */
  char    To[36];           /* receiver of message                         */
  unsigned char Title[72];  /* title of message                            */
  char    Date[20];         /* string containing creation date of message  */
  SHORT   TimesRead;        /* used for message type                       */
  SHORT   DestNode;         /* fidonet destination node                    */
  SHORT   OrigNode;         /* fidonet originating node                    */
  SHORT   Cost;             /* fidonet cost field                          */
  SHORT   OrigNet;          /* fidonet originating net                     */
  SHORT   DestNet;          /* fidonet originating node                    */
  char    Fill[8];          /* not used                                    */
  SHORT   ReplyTo;          /* backward reply link                         */
  SHORT   Attribute;        /* fidonet attribute field                     */
  SHORT   NextReply;        /* forward reply link                          */
};


/* Message Types */
#define PRI_MSG      0  /* private message         */
#define PUB_MSG      1  /* public message          */
#define PRINET_MSG   2  /* private netmail message */
#define PUBNET_MSG   3  /* public netmail message  */
#define ECHO_MSG     4  /* echomail message        */
#define UUCP_MSG     5  /* uucp message            */
#define CC_MSG       6  /* carbon copy message     */
#define FILE_DESC    7  /* file description        */
#define SIGNATURE    8  /* signature               */
#define BULLETIN     9  /* bulletin                */
#define TEXT         10 /* text file               */
#define USENET_MSG   11 /* usenet message          */
#define NEWS_MSG     12 /* newsgroup message       */


/* Message header bits for FidoNet */
#define Private            1
#define Crash              2
#define Received           4
#define Sent               8
#define FileAttached       16
#define InTransit          32
#define Orphan             64
#define KillSent           128
#define Local              256
#define HoldForPickup      512
#define FFileRequest       2048
#define ReturnRectRequest  4096
#define IsReturnRect       8192
#define AuditRequest       16384
#define FileUpdateReq      32768


/* Structure for calling DispMsg() */

struct MsgDisplay
{
  USHORT  area;       /* current area number                                     */
  char   *transarray; /* character set translation array or NULL                 */
  char   *passwd;     /* password for doing area locking                         */
  char   *filename;   /* path/filename of the message to display                 */
  USHORT *screenpos;  /* address of current screen position                      */
  LONG    findex;     /* size of the message header                              */
  char   *breakbuf;   /* null terminated string of characters that can be used   */
                      /*  to break the message body display                      */
  ULONG   flags;      /* flags see following defines                             */
};

/* MsgDisplay flags */
#define MSG_STRIPSB  1L
#define MSG_MSGAREA  2L
#define MSG_FILEAREA 4L

/* Structure for calling SendPublicMsg() and SendRawMsg() */

struct MsgStruct
{
  struct Msg_Header *header;    /* address of message header structure  */
  struct Msg_Header *repheader; /* address of header being replied to   */
  unsigned char *body;          /* buffer containing the body text      */
  USHORT replyto;               /* message number we are replying to    */
  struct Msg_Area *areainfo;    /* address of areainfo structure        */
  long flags;                   /* flags; see following defines         */
};

/* MsgStruct flags */
#define MSG_NOORIGIN    1L


/* Filename User:First_Last/WaitingMail.dat

   This file MUST always be alphabetically sorted on the first field
   A record must be added when a public messages in entered to a user
   on the system.  A record must be deleted when that user READS or PACKS
   the message, does anything that will reset the recieved bit of the message
   header, OR when the message is deleted from the system.  See AddStruct() and
   DeleteStruct() */

struct WaitingMail
{
  char  msgid[12];     /* format:    area:message   eg:  0001:00123        */
  char  from [36];     /*                                ^^^^^^^^^^        */
  char  subject[72];   /* Must be padded with ZEROS in this exact format   */
  SHORT areanum;
  char  areaname[34];
  SHORT messagenum;
};


/* SIGs */

struct SIG_Def
{
  unsigned char number; /* number representing the sig               */
  char name[36];        /* text string representing name of sig      */
  char shortname[11];   /* short name of sig (for prompt purposes)   */
  unsigned char  level; /* level required to join sig                */
  char sigop[36];       /* leader of the sig                         */
  char status;          /* not used                                  */
  char filler[40];      /* not used                                  */
};

