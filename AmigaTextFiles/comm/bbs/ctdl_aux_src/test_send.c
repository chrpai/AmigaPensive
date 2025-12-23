/* Special Debug Port routine Version 1.00
*/
#include "stdio.h"
#include "string.h"
#include <libraries/dos.h>
/*#include <ctype.h>*/
#include <devices/console.h>
#include <devices/serial.h>
#include <devices/timer.h>
#include <exec/types.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <exec/tasks.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/libraries.h>
#include <exec/devices.h>
#include <exec/io.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <libraries/dos.h>
#include <libraries/dosextens.h>
#include <libraries/filehandler.h>
#include "proto/all.h"
#include "pragmas/dos_pragmas.h"
#include "pragmas/exec_pragmas.h"
#include "aux-debug.h"

void Aux_Debug(char *, int, int, int);

void main( void );

void main()
  {
  int i,j,k;
  printf("Starting...\n");
  for( k = 0; k< 10;k++) for(j=1; j < k; j++) for(i=2; i< j; i++)
    {
    Delay(10);
    Aux_Debug("Test program",i,j,k);
    };
  printf("Test completed...\n");
  }

void Aux_Debug(msg, int1, int2, int3)
char *msg;             /* the message to send */
int  int1, int2, int3; /* various integer data to send */
  {
  struct MsgPort *port;
  char *mymessage;
  struct debugmsg *temp;
  Forbid();
  port = FindPort(AUXDEBUGPORT);  /* check to see if we have a port...*/
  Permit();
  if( !port )return;  /* if no port, just exit */
  temp      = (struct debugmsg *)AllocMem(sizeof(struct debugmsg),0);
  if(!temp) return;
  mymessage = (char *)AllocMem(strlen(msg)+1,0);  /* dynamic sizing */
  if( mymessage == NULL)
    {
    FreeMem(temp, sizeof(struct debugmsg));
    return;
    };
  temp->msg   = mymessage;
  temp->a     = int1;
  temp->b     = int2;
  temp->c     = int3;
/*
** Setup the Message itself
*/
temp->ipc_Msg.mn_Node.ln_Type = NT_MESSAGE;
temp->ipc_Msg.mn_Length = sizeof(struct debugmsg);
temp->ipc_Msg.mn_ReplyPort = NULL;

/* safe send of a message.  If Port exists, we send the
** packet and forget about it.  If the Port does not exist
** we forget that the message even existed...
*/
  Forbid();
  port = FindPort(AUXDEBUGPORT);
  if( port )
    {
    PutMsg(port,(struct Message *)temp);
    }
  else
    {
    FreeMem(temp->msg,strlen(msg)+1);
    FreeMem(temp, sizeof(struct debugmsg));
    }
  Permit();
/*
** we do not wait for the message reply, in this case there is
** no reply done and all memmory is deallocated in the reciever.
*/
  }
