/* ALWAYS change the base port to something other than previously used */
/* It is a good idea to use an alpha-NUMERIC port name in case someone */
/* else decides to use the same "BaseName" as you want to use          */

#define KEY_CHECK (BOOL) z->KB1 != KB2

#define QUIET 0   /* not currently used */
#define VERBOSE 1 /* not currently used */

#define UserMsg_Event 0x01    /* flagged when message from a remote port is received */
#define UserKey_Event 0x02    /* flagged when there is input waiting in the keyboard buffer */
#define UserTimer_Event 0x04  /* flagged when a timeout event (approx. every 3/8 second) occurs */
#define ReplyMsg_Event 0x08   /* flagged when an NT_REPLYMSG is encountered */
#define Rexx_Event 0x10       /* flagged when an AREXX event/command has has been received */
#define UserBreak_Event 0x20  /* flagged when CTRL-C is received or "BREAK" is executed on this process */


struct PortVars
  {
  /* this is where you declare all SHARED variables.                        */
  /* anything that you want another port to access must be declared here    */
  /* otherwise the remote port will not be able to access this port's data  */
  /*                                                                        */
  /* REMEMBER!: Shared variables CAN be changed by other ports.  Be VERY    */
  /*            careful when using shared data!  Never assume it being what */
  /*            you set it to if another port has access to alter it.       */

  struct PortData *z;
  LONG testvar;
  };

typedef struct CNMPMsg {
  struct Message Msg;
  int            CNMP_Type;  /* type of message - see CNMP_* #defines, above */
  int            CNMP_Arg;
  int            FromPort;
  LONG           Var1;       /* general purpose data variable - NOT a pointer! */
  LONG           Var2;       /* gen purpose data var 2 */
  LONG           Var3;       /* gen purpose data var 3 */

  void           *Data1;   /* malloc'd variable - use malloc() and then CopyMem() or strcpy() your data to the returned memory location and then set Data1 equal to the memory location returned by malloc() */
  LONG           Data1Len; /* allocated length of Data1 */
  void           *Data2;   /* malloc'd variable - use malloc() and then CopyMem() or strcpy() your data to the returned memory location and then set Data2 equal to the memory location returned by malloc() */
  LONG           Data2Len; /* allocated length of Data2 */
  void           *Data3;   /* straight pointer - DO NOT malloc() data for this pointer or it will not be freed by the default setup.  If you malloc() memory for this pointer, it will never get free()'d */
  LONG           Data3Len; /* allocated length of Data3 */
                           /* Data1, Data2 and Data3 are data pointers/flags/message variables for the remote port(s) to access */
                           /* data in these pointers/variables MAY be changed by the remote port at any time so use extreme     */
                           /* caution with pointer variables. */
                           /* Note: the *DataX variables may be typecast */
                           /* to "look like" any pointer variable type you need */

  BOOL           Recd;    /* *** this variable CNetMultiPort private - do not alter directly! */
  struct CNMPMsg *NextMsg;   /* *** This variable private - do not monkey with it! */
};


/* CNMP_Types -  types currently supported */
#define CNMP_ADDPORT    300      /* tell remote ports that you've entered */
                                 /* set Data1 = MyCNMPName */
#define CNMP_REMOVEPORT 301      /* tell remote ports to remove your port from their linked lists */
#define CNMP_VARSINIT   302      /* use this to setup shared variables between ports. */
                                 /* remember to use CNMP_VARSREMOVE to tell a remote port it is not to use data anymore */
#define CNMP_VARSREMOVE 303      /* use this to remove variables from remote port variable lists */
#define CNMP_TEXTECHO   304      /* used as an OLM-type method to send text */
#define CNMP_WAITNOTIFY 306      /* Tell the remote port not to process     */
                                 /* anymore msgs from our port until we send*/
                                 /* CNMP_RESUMENOTIFY                       */
                                 /* this entails GETTING the message and    */
                                 /* storing it in the linked list but       */
                                 /* not to process any of them.             */
#define CNMP_RESUMENOTIFY 307    /* resume processing of port's messages */
#define CNMP_REQUESTNOTIFY 308   /* Tells the remote port to enable message checking again */
#define CNMP_ENTRYCONFIRM 309    /* Port sends this CNMP_Type if it wants to ask another port if it is allowed to enter */
#define CNMP_ENTRYPERMIT 310     /* Port is allowed entry - sent to remote port that has requested confirmation of entry */
                                 /* If CNMP_REQUESTCONFIRM is sent and CNMP_PORTPERMIT is received as a reply */
                                 /* CNMP_ADDPORT should be sent to tell the remote port to add this port to it's list */
#define CNMP_ENTRYREFUSE 311     /* Port is refused entry - sent to remote port that has requested confirmation of entry */
#define CNMP_FLUSHALL 312        /* We have been told to clear/free all messages that have piled up */
                                 /* ie. call the function FreeUserMsgs()                            */
#define CNMP_KICKPORT 313        /* Send this message to a port to force it to exit/quit */
                                 /* this does NOT dump the user from the CNet port but exits the CNMP Program. */
#define CNMP_DUMPUSER 314        /* send this message to force the user's exit from CNMP AND to dumnp him from the CNet port. */
#define CNMP_STATUS   315        /* send this message to send status info to the user */


/* note: CNMP_ENTRY* types are not _required_.  CNMP_ADDPORT can be sent
 *       directly if no port permission checking is desired
 *       there is an advantage to using port permissions in that CRASHED
 *       or busy ports will not be recognized until they reply, thus
 *       memory savings will be realized.
 */

/* also note that not all defined message types are implemented as MetalSoft */
/* wishes that this implementation be as flexible as possible. If you want   */
/* a new message type to be supported, simply #define a message type for it  */
/* and add the handling for it in the while() loop that contains the comment */
/* "MAIN_CNMP_LOOP".                                                         */
/* PLEASE try to use numbers from 500 up.  0-499 are reserved for current and*/
/* future expansion as version 1.0 of CNetMultiPort should be compatible with*/
/* all future version.  Please note that expansion MAY render this point NULL*/
/* but every attempt will be made to keep all I/O 100% compatible.           */

/* user CNMP_* defines start here from 500 up */
#define CNMP_DISPLAY_TV 500  /* CNMP_DISPLAY_TV is  an example of what you  */
                             /* can add on your own.  Do a search for       */
                             /* "CNMP_DISPLAY_TV" to see what exactly is    */
                             /* required for you to set up your own message */
                             /* type.                                       */


/* all remote ports with CNMP message ports open are contained in a linked list */
/* one RemotePort structure exists for each port   */
struct RemotePort
  {
  int PortNum;                  /* number of the CNet port */
  char PortName[100];           /* DO NOT CHANGE! required to be 100 for compatibility */
  struct MsgPort *PortPtr;      /* pointer to the MsgPort */
  BOOL MsgsLocked;              /* This will be TRUE if we've received a CNMP_WAITNOTIFY from this remote port */
                                /* and it must be set FALSE when we receive CNMP_RESUMENOTIFY */
  struct PortVars *PortVars;    /* pointer to declared variables */
  struct RemotePort *NextPort;
  };



struct CnmpCluster
  {
  struct SignalSemaphore   *cc_SEM;          /* semaphore for locking this cluster */
  char                     CNMP_BASEPORT[30];
  char                     cc_MyPortName[30];
  struct Process           *cc_myproc;
  struct timerequest       *cc_iotr;         /* initialized timerequest
                                                structure */
  BOOL                     cc_iotpending;    /* true if timerequest pending */
  struct MsgPort           *cc_iosink;       /* message port for timerequest
                                                replies */
  ULONG  cc_ExtPortMask;                     /* remote port message mask */
  ULONG  cc_TimerMask;                       /* timer.device message mask */
  ULONG  cc_UserMask;                        /* for defining your own signal bits to "wait()" on */
  struct RemotePort        *cc_firstport;    /* First CNMPort in list */
  struct RemotePort        *cc_myport;       /* My CNMPort */
  struct PortVars          *cc_portvars;     /* PortVars for current port */
  struct CNMPMsg           *cc_msglist;      /* PRIVATE -> linked list of
                                                messages */
  struct CNMPMsg           *cc_cnmpmsg;      /* allocated message structure for
                                                quick message sending */
  BOOL                     cc_ignoremsgs;    /* set to true if CNMP is to ignore
                                                and NOT file received msgs.
                                                It DOES, however, ReplyMsg()
                                                them */
  struct MsgPort           *cc_msgport;      /* the actual message port that was
                                                opened for sending/receiving
                                                msgs */
  int                      cc_totalcnmports; /* the number of current CNMP ports
                                                open that are sharing this
                                                CNMP_BASEPORT */
  };
