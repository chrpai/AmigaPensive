/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_IO_H
#include <exec/io.h>
#endif

#define RMCONTROL "tptrm.control"

/* The message structure */

struct RMMessage
{
  struct Message mess;  /* The exec message structure                   */
  char type;            /* The type/error field                         */
  char *port;           /* The port this message pertains to            */
  USHORT area;          /* Number of the area                           */
  UBYTE flags;          /* Miscellaneous flags                          */
  char *passwd;         /* Password for lock                            */
  char *newpasswd;      /* New password for transferring lock           */
  char *reason;         /* Reason for lock                              */
  char priority;        /* Priority of lock                             */
  char *breakcommand;   /* Command to be executed to release the lock   */
  void *dataptr;        /* General purpose data pointer                 */
};


/* Message types */

#define ACTIVATEPORT    1     /* Activate a port                  */
#define DEACTIVATEPORT  2     /* Deactivate a port                */
#define LOCKPORT        3     /* Lock a port                      */
#define FREEPORT        4     /* Free a port lock                 */
#define GETPORTINFO     5     /* Get port information             */
#define FREEPORTINFO    6     /* Free port information            */
#define TRANSFER        8     /* Transfer a port lock             */
#define LISTPORTS       9     /* List active ports                */
#define LOCKAREA        10    /* Lock an area                     */
#define BORROWAREA      11    /* Borrow an area                   */
#define FREEAREA        12    /* Free an area lock                */
#define ENTERAREA       13    /* Enter an area                    */
#define LEAVEAREA       14    /* Leave an area                    */
#define GETAREAINFO     15    /* Get area information             */
#define FREEAREAINFO    16    /* Free area information            */
#define LOADLANG        17    /* Load a language file             */
#define GETLANG         18    /* Get language information         */
#define LOCKRESOURCE    19    /* Lock a misc resource             */
#define FREERESOURCE    20    /* Free a misc resource             */

#define LOCKUSERS       22    /* Not used                         */
#define FREEUSERS       23    /* Not used                         */
#define LOCKMENU        24    /* Lock a menu                      */
#define FREEMENU        25    /* Free a menu                      */
#define GETRESREPORT    26    /* Get a resource report            */
#define FREERESREPORT   27    /* Free a resource report           */
#define PURGEMENU       28    /* Purge a menu                     */

#define RMEXIT          255   /* Shut down the resource manager   */

/* Errors returned in the type field */

#define RMNOERR       0    /* No error                */
#define RMBAD        -1    /* Bad message type        */
#define PORTACTIVE   -2    /* Port already active     */
#define BADPORT      -3    /* Port not active         */
#define NOTLOCKED    -4    /* Resource not locked     */
#define NORM         -5    /* TPTRM not active        */
#define RMNOMEM      -6    /* Out of memory error     */
#define AREAINUSE    -7    /* Area has users in it    */
#define BADAREA      -8    /* Area not active         */
#define AREAEMPTY    -9    /* Area has no users in it */
#define NOLANG       -10   /* Language doesn't exist  */
#define BADRES       -11   /* Resource not active     */
#define RESINUSE     -12   /* Resource is in use      */
#define NOLOADMENU   -13   /* Unable to load menu     */


/* Area flags */

#define MSGLOCK   0  /* Message area               */
#define READLOCK  0  /* Read lock                  */
#define FILELOCK  1  /* File area                  */
#define PENDLOCK  2  /* Wait for a lock            */
#define ENTERPEND 2  /* Pend lock                  */
#define WRITELOCK 4  /* Write lock                 */
#define QUICKLOCK 8  /* "Borrow" instead of "lock" */


/* Misc node */

struct MiscNode
{
  struct Node node;        /* Node structure                */
  struct List activelocks; /* Locks currently active        */
  UBYTE type;              /* The type of lock active       */
  struct List locklist;    /* List of pending locks         */
  UBYTE users;		         /* Number of users (for areas)   */
};


/* Node types (in MiscNode.node.ln_Type) */

#define NODE_MISC   1   /* Miscellaneous node   */
#define NODE_PORT   2   /* Port node            */
#define NODE_MAREA  3   /* Message area node    */
#define NODE_FAREA  4   /* File area node       */
#define NODE_MENU   5   /* Menu node            */

struct MiscLock
{
  struct Node node;     /* Node structure for queueing            */
  struct RMMessage *rm; /* Message structure for returning lock   */
  char *reason;         /* Reason for lock                        */
  UBYTE type;           /* Type of lock                           */
};


/* Language structure */

struct LangStruct
{
  char name[21];     /* Name of language           */
  char **strings;    /* Language strings           */
  short numstrings;  /* Number of language strings */
};


/* Language node */

struct LangNode
{
  struct Node node;     /* Node structure for linking          */
  struct LangStruct ls; /* Language structure                  */
  short opencount;      /* Number of ports using this language */
};


/* Port node */

struct PortNode
{
  struct MiscNode mn;      /* Misc. node structure                      */
  char *bgcommand;         /* Command to run when no locks are active   */
  struct LangNode *ln;     /* Language for the port                     */
  struct MenuNode *menu;   /* The current menu for the port             */
  BYTE  custnum;           /* The number of the custom menu             */
  char *custnov;           /* Custom menu for NOVICE                    */
  char *custint;           /* Custom menu for INTERMEDIATE              */
};


/* Port lock */

struct PortLock
{
  struct MiscLock ml;   /* Misc. lock structure                         */
  char *breakcommand;   /* Command to be executed to release the lock   */
};

#define AreaLock MiscLock


/* Area node */

struct AreaNode
{
  struct MiscNode mn;   /* Misc. node structure */
  USHORT area;          /* Number of the area   */
};


/* Port info structure (for GetPortInfo()) */

struct PortInfo
{
  char *port;           /* Name of port                  */
  char *passwd;         /* Password port is locked with  */
  char *reason;         /* Reason for lock               */
  char priority;        /* Priority of lock              */
  char *breakcommand;   /* Command to break lock         */
};


/* Area info structure (for GetAreaInfo()) */

struct DLGAreaInfo
{
  USHORT area;    /* Number of area                */
  char *passwd;   /* Password area is locked with  */
  char *reason;   /* Reason for lock               */
  char priority;  /* Priority of lock              */
  UBYTE users;    /* Number of users in area       */
};


/* An individual command associated with a menu entry */

struct BatchEntry
{
  char *program;           /* Command to execute                     */
  USHORT function;         /* Function number for builtin            */
  UBYTE functype;          /* 0=internal  1=external                 */
  UBYTE type;              /* Type of command                        */
  UBYTE llev,ulev;         /* User level required                    */
  UBYTE flags;             /* Flags (see tse.h)                      */
  char *action;            /* Activity string association with entry */
  SHORT rate;              /* Cost/min                               */
  SHORT maxtime;           /* Max minutes in menu entry              */
  char priority;           /* Task priority                          */
  struct BatchEntry *next; /* Next entry in the list                 */
};


/* The main head node for a compiled menu structure */

struct CompiledMenuEntry
{
  char command;               /* Menu Letter                                  */
  char *description;          /* Command description                          */
  char *helpfile;             /* Help file for entry                          */
  UBYTE visibility;           /* Visibility of entry                          */
  UBYTE logvalue;             /* Log entry value                              */
  SHORT cost;                 /* Base cost                                    */
  struct BatchEntry *blist;   /* List of commands associated with the entry   */
};


/* Menu node */

struct MenuNode
{
  struct MiscNode mn;                  /* Misc Node                        */
  UBYTE numentries;                    /* Number of menu entries           */
  char *name;                          /* Name of menu                     */
  char columns;                        /* Number of columns (1-8)          */
  char *prompt;                        /* Prompt for menu                  */
  UBYTE numentry;                      /* Entry to use if number is typed  */
  char *action;                        /* Action string when idle at menu  */
  struct CompiledMenuEntry *entries;   /* The menu entries                 */
};


/* Menu stuff */

struct MenuStuff        /* Returned from LockMenu */
{
  char *name;              /* Name of menu - provided by caller   */
  struct MenuNode *mun;    /* Menu node                           */
  UBYTE custnum;           /* The number of the custom menu       */
  char *custnov;           /* Custom menu for NOVICE              */
  char *custint;           /* Custom menu for INTERMEDIATE        */
};


/* Resource report structure */

struct ResRepNode
{
  struct Node node;           /* Node structure for queuing    */
  char *bgcommand;            /* Background command (for Port) */
  char *language;             /* Loaded language (for Port)    */
  char *menu;                 /* Current menu (for Port)       */
  UBYTE users;                /* Number of users (for Area)    */
  struct List *activelocks;   /* List of active locks          */
  struct List *pendinglocks;  /* List of pending locks         */
};


/* Lock report structure */

struct LockRepNode
{
  struct Node node;     /* Node structure for queuing                */
  char *reason;         /* Reason for lock                           */
  UBYTE type;           /* Type of lock                              */
  char *breakcommand;   /* Break command to release lock (for Port)  */
};

