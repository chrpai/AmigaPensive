/**
                               vortex.c

 Network "vortex" (aka infinite loop) handling code.


**/
#include "ctdl.h"
#include "math.h"
#include "vortex.h"
#include <dos.h>
/**
                               contents
       InitVortexing()         allocate & initialize buffers
       NotVortex()             checks for vortex possibility
**/


/**
   External variable definitions
**/
extern CONFIG    cfg;   /* Lots an lots of variables    */
extern MessageBuffer   msgBuf;
extern FILE      *netLog;
extern char *READ_ANY, *APPEND_ANY;
extern char *R_W_ANY, *WRITE_ANY;
extern char logNetResults;
extern char netDebug;

/**
 -Vortex Handling-

    Vortex: the phenomenon of messages showing up on other systems more than
 once.  The cause is messages being sent from one system to another and
 eventually getting back to the originating system.  This could be in two
 flavors:

    A -- B -- C -- eventually back to A

    or

    A -- B -- C -- D -- eventually back to B or C...

    See vortex.h for a complete description of the data records.
**/
char VortexHandle = TRUE;   /* default to Vortex Handling */
SYS_FILE vareaname;         /* set to #netarea plus vortex */
SYS_FILE vfilename;         /* full filename of current vortex file */
static struct elist *head;  /* error list */
void                    Fix_Name(char *out, char *in, int flag);
static ROOM_ENTRY_TYPE *Vortex_Find_Node( label room );

void Fix_Name(char *out, char *in, int flag )
  {
  /**
    take the input name and remove all spaces and special characters
    can be used to append one "normalized" string onto another.
    flag == 1 says to supress all special characters, 0 says replace
    them with "x".
  **/
  int i,j;
  char c;
  j = 0;
  for(i=0; i<strlen(in); i++)
    {
    c = in[i];
    if( isspace(c) ) continue;  /* always supress spaces, tabs, ect. */
    if( flag == 1 && !isalnum(c) ) continue;
    if( !isalnum(c) ) c = 'x';
    out[j++] = c;
    };
  out[j] = '\0';
  }

static ROOM_ENTRY_TYPE *Vortex_Find_Node(label room )
  {
  FILE *vfd;
  ROOM_ENTRY_TYPE *vptr;
  int  j;
  strcpy(vfilename,vareaname);
  j = strlen(vfilename);
  vfilename[j++] = '/';
  Fix_Name(&vfilename[j], room, 0);
  vptr = (ROOM_ENTRY_TYPE *)malloc(sizeof(ROOM_ENTRY_TYPE));
  if( vptr == NULL ) return NULL;
  if( (vfd = fopen(vfilename, READ_ANY) ) == NULL )
    {
    /**
      assume it does not exist... create it.
    **/
    memset(vptr, '\0', sizeof(ROOM_ENTRY_TYPE));  /* initialize it */
    }
  else
    {
    /**
      read the entries
    **/
    if( fread(vptr, sizeof(ROOM_ENTRY_TYPE), 1, vfd) != 1 )
      {
      splitF(netLog, "System error with reading %s\n",vfilename);
      free(vptr);
      vptr = NULL;
      };
    fclose(vfd);
    };
  return vptr;
  }

/**
 VortexInit()

 This function initializes vortexing.  Currently, that means making sure
 that the directory in the #NETAREA called "vortex" exists, it will be created
 if not.  Otherwise, nothing is done at this point.
**/

void VortexInit()
  {
  /**
     Initialize the system for vortex checking
     1) create vortex directory if none exits
  **/
  if (!VortexHandle) return ;   /* exit if not doing vortex checking */
  makeSysName(vareaname, "vortex", &cfg.netArea);
  if( access(vareaname, F_OK) != 0 )
    {
    /**
      does not exist, we must create the directory
    **/
    if (mkdir(vareaname) != 0)
      mPrintf("Error creating %s!\n ",vareaname);
    };
  return;
  }

/**
 InitVortexing()

 This is an initialization function which should be called before each
 session of checking messages in from a network session.

 Initialize the error list.
**/
void InitVortexing()
  {
  head = NULL;    /* should already be empty */
  return;         /* nothing else to do */
  }

/**
 NotVortex()

 This checks to see if the msg in msgBuf is vortexing or not.  FALSE is
 returned if the message should be discarded.  TRUE is not a vortex.

 All vortex files are created and updated during this call.

**/

char NotVortex()
  {
  int i;
  char vortex_flag = TRUE;
  ROOM_ENTRY_TYPE *node;  /* the current node */
  char mynode[20], yournode[20];

  /**
    First, we must find the Node Entry that matches the Message
    The function Vortex_Find_Node() will always return a ROOM_ENTRY
    pointer, it will create it if it does not exits.
  **/
  if (!VortexHandle) return TRUE;   /* exit if not doing vortex checking */
  Fix_Name(  mynode, &cfg.codeBuf[cfg.nodeName], 1);
  Fix_Name(yournode, msgBuf.mboname, 1);
  if( strcmp(mynode, yournode) == 0 )
    {
    splitF(netLog, " NotVortex: detected messages from %s\n",msgBuf.mboname);
    splitF(netLog, "            Routed from %s\n",msgBuf.mborig);
    return FALSE;
    };
  if( ( node = Vortex_Find_Node(msgBuf.mbroom) ) == NULL )
    {
    splitF(netLog, " NotVortex: node (%s) bad or no memory\n",msgBuf.mboname);
    return TRUE;
    };
  if( node->index == 0 )
    {
    if( logNetResults)
      {
      splitF(netLog, "Msg for unmonitored room %s, adding it.\n", msgBuf.mbroom);
      };
    }
  else  /* check to see if already in list */
    {
    for( i=0; i < node->index && vortex_flag; i++)
      {
      struct elist *error;
      char *tm;
      MESSAGE_ENTRY_TYPE *mptr = &node->msg_entry[i];
      if( msgBuf.mbsrcId[0] == '\0' ) continue;
      if( mptr->mbsrcId[0]  == '\0' ) continue;
      if( strcmp(mptr->mbsrcId, msgBuf.mbsrcId) != 0 ) continue;
      if( strcmp(mptr->mborig,  msgBuf.mborig ) != 0 ) continue;
      if( (tm = strchr(mptr->mbtime,'\n')) != NULL )*tm ='\0';
      splitF(netLog, "Vortex Detection: Node: %s Room: %s Id: %s Date: %s Time: %s \n"
      , mptr->mborig, msgBuf.mbroom, mptr->mbsrcId, mptr->mbdate, mptr->mbtime);
      splitF(netLog, "   Message Entry: Node: %s SrcId: %s Date: %s Time: %s\n"
      , msgBuf.mborig, msgBuf.mbsrcId, msgBuf.mbdate, msgBuf.mbtime );
      vortex_flag = FALSE;
      error = head;
      while( error )
        {
        if( strcmp(error->room,  msgBuf.mbroom)   == 0 &&
            strcmp(error->nodeid, msgBuf.mborig)  == 0 ) break;
        error = error->next;
        };
      if( error == NULL )
        {
        error = ( struct elist *)malloc(sizeof(struct elist));
        if( error == NULL )
          {
          mPrintf(" Error: No memory for vortex error list\n");
          }
        else
          {
          strcpy(error->room,  msgBuf.mbroom);
          strcpy(error->nodeid, msgBuf.mborig);
          error->next = head;
          head        = error;
          };
        };
      };
    };
  if( vortex_flag == TRUE )
    {
    FILE *vfd;
    MESSAGE_ENTRY_TYPE *mptr = &node->msg_entry[node->next_slot];
    strcpy(mptr->mborig,  msgBuf.mborig);

    if( msgBuf.mbsrcId[0] )
      {
      strcpy(mptr->mbsrcId, msgBuf.mbsrcId);
      }
    else mptr->mbsrcId[0] = '\0';

    strcpy(mptr->mbdate,  msgBuf.mbdate );
    strcpy(mptr->mbtime,  msgBuf.mbtime );
    node->index++;
    if( node->index > MAX_VORTEX_SIZE) node->index = MAX_VORTEX_SIZE;
    node->next_slot = ( node->next_slot + 1 ) % MAX_VORTEX_SIZE;
    if( (vfd = fopen(vfilename, WRITE_ANY) ) == NULL )
      {
      mPrintf("System error with open for write of %s\n",vfilename);
      }
    else
      {
      if( fwrite(node, sizeof(ROOM_ENTRY_TYPE), 1, vfd) != 1 )
        {
        mPrintf("System error with write of %s\n",vfilename);
        };
      fclose(vfd);
      };
    };
  free(node);
  return vortex_flag;
  }

/**
 FinVortexing()

 This function should be called to finish a vortex checking session.
 Nothing to do in this function right now.
**/
void FinVortexing()
  {
  struct elist *error;
  if (!VortexHandle)  return;
  if ( head == NULL ) return;
  strcpy(msgBuf.mbtext, "Vortex attempted involving the following system(s): \n");
  error = head;
  head  = error->next;
  while( error && strlen(msgBuf.mbtext) < 7000  )
    {
    strcat(msgBuf.mbtext, " Node Id: ");
    strcat(msgBuf.mbtext, error->nodeid);
    strcat(msgBuf.mbtext, " Room: ");
    strcat(msgBuf.mbtext, error->room);
    strcat(msgBuf.mbtext, "\n");
    free(error);
    error = head;
    if( error ) head = error->next;
    };
  }
