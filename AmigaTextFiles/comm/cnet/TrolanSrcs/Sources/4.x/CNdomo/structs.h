/*
** Definitions for CNdomo
** by Kelly Cochran
** Last updated: Sat Oct 17 20:23:38 1998
*/

#ifndef _CNDOMO_H
#define _CNDOMO_H

#define MAINDIR "Mail:CNdomo"
#define LISTDIR MAINDIR"/Lists"

#define MIN(a, b)  (((a)<(b))?(a):(b))
#define MAX(a, b)  (((a)>(b))?(a):(b))

struct ListEntry
{
   char         le_Name    [  32 ]; /* Name of list                */
   char         le_From    [ 128 ]; /* From address                */
   char         le_ReplyTo [ 128 ]; /* Reply-to address            */
   char         le_ErrorTo [ 128 ]; /* Errors-to addr              */
   char         le_Owner   [ 128 ]; /* List owner address          */
   char         le_Passwd  [  16 ]; /* List admin password         */
   ULONG        le_Number;          /* High article #              */
   ULONG        le_Flags;           /* See below                   */
   ULONG        le_DigestSize;      /* Max digest size             */
   ULONG        le_CurrentSize;     /* Current pending digest size */
   ULONG        le_DigestNum;       /* Current digest number       */
   ULONG        le_LastDigest;      /* Last article digested       */
   ULONG        le_MaxPostSize;     /* Largest post allowed        */
   struct List *le_Users;           /* list of users               */
   struct List *le_Posters;         /* list of posters             */
   struct List *le_Pending;         /* Pending subscribers         */
   struct List *le_Banned;          /* Banned users                */
};

struct ListEntryNode
{
   struct Node      le_Node;
   struct ListEntry le;
};

/* ListEntry.le_Flags */

#define LEF_CLOSED      (1<<0)   /* Only allow posts from list    */
#define LEF_MODERATED   (1<<1)   /* Moderate list?                */
#define LEF_DIGEST      (1<<2)   /* Digestify?                    */
#define LEF_CONFIRM     (1<<3)   /* Require confirmation to join  */
#define LEF_HANDLEADMIN (1<<4)   /* Check for admin requests      */
#define LEF_PRIVATE     (1<<5)   /* Do not show in LISTS command  */
#define LEF_PREFIXLIST  (1<<6)   /* Add [listname] to subject     */
#define LEF_SETLISTFROM (1<<7)   /* Set from to be from list      */
#define LEF_SENDSPOOL   (1<<8)   /* Send mail to list immediately */
#define LEF_ARCHIVE     (1<<9)   /* Archive mail items            */

/* Error codes */

#define CND_ERR_NONE      0
#define CND_ERR_TOOLARGE  1
#define CND_ERR_NOTONLIST 2
#define CND_ERR_BANNED    3
#define CND_ERR_BASPASS   4
#define CND_ERR_NOTADMIN  5

struct UserEntry
{
   char         ue_From [ 128 ];  /* From address             */
   char         ue_Key  [  16 ];  /* password for join/modify */
   struct List *ue_Lists;         /* Lists subscribed to      */
};

struct UserEntryNode
{
   struct Node      ue_Node;
   struct UserEntry ue;
};

struct UserListEntry
{
   struct Node       ule_Node;
   char              ule_Name [ 32 ]; /* Name of the list subscribed to */
   struct ListEntry *ule_LEntry;      /* ListEntry for list             */
};

struct MailItem
{
   char FullFrom [ 256 ];
   char MessageID[ 128 ];
   char From     [  64 ];
   char ReplyTo  [  64 ];
   char To       [  64 ];
   char Date     [  64 ];
   char Subject  [  64 ];
   char Org      [  64 ];
   struct List *RcvHdrs;          /* Received: headers    */
   struct List *OtherHdrs;        /* other keeper headers */
   LONG Flags;
   LONG Size;
   APTR Message;
#define MIF_LISTINSUBJECT (1<<0) /* Listname in subject already? */
#define MIF_LOOP          (1<<1) /* Loop detected?               */
#define MIF_APPROVED      (1<<2) /* Approved                     */
};

struct CNdomoCmd
{
   char *name;          /* Name of command   */
   int  (*func)(void);  /* Function to call  */
   int  min_args;
   int  max_args;
   int  domo_also;      /* Command available from main CNdomo addr? */
#define CND_DA_YES   1
#define CND_DA_NO    2
#define CND_DA_REQ   3
};

struct MailHeaderClass
{
   char *name;
   int   type;
};

enum
{
   HDR_FROM = 1    ,
   HDR_TO          ,
   HDR_DATE        ,
   HDR_SUBJECT     ,
   HDR_REPLYTO     ,
   HDR_ORGANIZATION,
   HDR_APPROVED    ,
   HDR_XLISTSERVER ,
   HDR_MESSAGEID   ,
   HDR_RECEIVED    ,
   HDR_OTHER       ,
};

#endif /* _CNDOMO_H */
