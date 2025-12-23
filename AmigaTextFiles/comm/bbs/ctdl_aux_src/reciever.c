/*
** Debug Server: Citadel Aux:
**
*/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <proto/all.h>
#include <pragmas/dos_pragmas.h>
#include <pragmas/exec_pragmas.h>
#include "aux-debug.h"

int debugmode = FALSE;   /* extra output with TRUE*/
FILE *op      = NULL;    /* file save output option */
void main(void);

void main()
  {
  struct MsgPort *port;
  long port_signal, break_signals, signals;
  char *ptr;
/* open the port */
  printf("... Creating Port:%s\n",AUXDEBUGPORT);
  port = CreatePort(AUXDEBUGPORT,0);
  if( port == NULL )
    {
    printf("Cannot create port of name:%s\n",AUXDEBUGPORT);
    };
  port_signal    = 1 <<  port->mp_SigBit;  /* server port */
  break_signals  = SIGBREAKF_CTRL_F;       /* toggle file save option */
  break_signals |= SIGBREAKF_CTRL_C;
  break_signals |= SIGBREAKF_CTRL_D;       /* turn on DEBUG INFO */
  break_signals |= SIGBREAKF_CTRL_E;       /* Exit               */

/* process until a control-E to exit */

  FOREVER
    {
    signals = Wait(port_signal | break_signals);
    if( signals & port_signal )                   /* debug messages first */
      {
      struct debugmsg *msg;
      while( msg = (struct debugmsg *)GetMsg(port) )
        {
        if( debugmode || msg->flag )
          {
          if( debugmode )
            {
            for(ptr = msg->msg; *ptr ; ptr++)if( *ptr < 20 || *ptr >126)*ptr='.';
            printf("%s %d %d %d\n",msg->msg, msg->a, msg->b, msg->c);
            if( op != NULL )fprintf(op,"%s %d %d %d\n",msg->msg, msg->a, msg->b, msg->c);
            }
          else
            {
            printf("%s",msg->msg);
            if( op != NULL )fprintf(op,"%s",msg->msg);
            };
          };
        FreeMem(msg->msg, strlen(msg->msg));
        FreeMem(msg, sizeof(struct debugmsg));
        };
      };
    if( signals & SIGBREAKF_CTRL_D )
      {
      debugmode = ~debugmode;
      printf("Got Control_D, debug is %s\n",(debugmode) ? "on" : "off");
      if( op != NULL )fprintf(op,"Got Control_D, debug is %s\n",(debugmode) ? "on" : "off");
      };
    if( signals & SIGBREAKF_CTRL_E )
      {
      printf("Got Control_E, exiting\n");
      break;
      };
    if( signals & SIGBREAKF_CTRL_F )
      {
      printf("Got Control_F, File save option is turned ");
      if( op != NULL )fprintf(op,"Got Control_F, File save option is turned off");
      if( op == NULL )
        {
        printf("on...\n");
        op = fopen("save.aux","ab+");
        if( op == NULL )
          {
          printf(" Error:  save.aux could not be opened!\n");
          };
        }
      else  /* already logging, turn it off */
        {
        printf("off...\n");
        fclose(op);
        op = NULL;
        };
      };
    };
  if(  port )DeletePort(port);    /* remove the server port */
  if( op != NULL)fclose(op);
  }
