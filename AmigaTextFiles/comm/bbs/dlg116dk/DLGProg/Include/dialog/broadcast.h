/***************************************************************************

 © copyright 1990-93 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_IO_H
#include <exec/io.h>
#endif

#ifndef DOS_DOSEXTENS_H
#ifdef SAS5
#include <libraries/dosextens.h>
#else
#include <dos/dosextens.h>
#endif
#endif

#define BCCONTROL "tptbc.control"

/* The message structure used to send a message to TPTBC */

struct BCMessage
{
  struct Message mess;  /* The exec message structure    */
  char type;            /* The type/error field          */
  char flags;           /* Flags                         */
  char *ports;          /* The ports to broadcast to     */
  char *buffer;         /* The message to be broadcast   */
};

/* Message types */

#define BCMSG     1  /* Send a broadcast message      */
#define BCPEND    2  /* Pend messages on a port       */
#define BCRESUME  3  /* Resume messages on a port     */
#define BCGET     4  /* Get the next pending message  */
#define BCEXIT    5  /* Not currently supported       */

/* Errors returned in the type field */

#define BCNOERR   0     /* No error                      */
#define NOPORT    -1    /* Port does not exist           */
#define BCBAD     -2    /* Bad message type              */
#define BCNOMEM   -3    /* Out of memory error           */
#define NOBC      -4    /* TPTBC not active              */
#define BCNOMSG   -5    /* No message to get (for BCGET) */

/* Bit definitions for the flag field */

#define BCIMPORTANTMSG  1  /* Message is important and should not pend */


/* Internal structure for holding ports */

struct BCPortNode
{
  struct Node node;     /* Node structure for linking */
  struct List msglist;  /* List of messages           */
  UBYTE status;         /* Status of port             */
};

/* Bit definitions for the status field */

#define BCMSGOUT  1  /* Port has messages out to it         */
#define BCPENDING 2  /* Port is currently pending messages  */


/* My DosPacket/file-handle/message conglomeration */

struct MyPacket
{
  struct Message sp_Msg;
  struct DosPacket sp_Pkt;
  long fhand;
  char port[4];
  UBYTE paused;
  struct BCMsgNode *msgnode;
};

/* Internal structure for holding messages */

struct BCMsgNode
{
  struct Node node;        /* Node structure for linking */
  char *buffer;            /* Message buffer             */
  struct MyPacket *packet; /* Packet for message         */
};

