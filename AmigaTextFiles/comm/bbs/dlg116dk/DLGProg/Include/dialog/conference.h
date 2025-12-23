/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_IO_H
#include <exec/io.h>
#endif

#define CONFCONTROL "tptconf.control"

/* Message types */

#define CREATECONF   1  /* Create a new conference             */
#define JOINCONF     2  /* Join an existing conference         */
#define LEAVECONF    3  /* Leave a conference                  */
#define LISTCONF     4  /* List available conferences          */
#define CONFMSG      5  /* Send a message to a conference      */
#define CONFREPLY    6  /* Reply to message                    */
#define KILLCONF     7  /* Terminate a conference              */
#define CONFEXIT     8  /* Shut down the conference handler    */
#define FREECONF     9  /* Free memory obtained with LEAVECONF */

/* Errors (returned in the type field) */

#define FNOERR       0  /* No error                            */
#define NOACCESS     -1 /* User has no access to conference    */
#define TOOSLOW      -2 /* Baud rate too slow for conference   */
#define BADTYPE      -3 /* Bad message type                    */
#define BADCONF      -4 /* Bad conference area                 */
#define CONFEXISTS   -5 /* Conference already exists           */

/* The message structure */

struct ConfMessage
{
  struct Message mess;        /* The exec message structure                */
  char type;                  /* The type of the message                   */
  char *username;             /* The sender's name                         */
  char *topvt;                /* The name of the recipient if private      */
  struct Conference *conf;    /* The conference to be used                 */
  int baud;                   /* The sender's baud or minimum baud         */
  unsigned char user_level;   /* The sender's user level or min user level */
  void *dataptr;              /* Multi-purpose pointer                     */
};

/* The Conference structure */

struct Conference
{
  char name[21];                 /* The name of the conference                */
  unsigned char user_level;      /* The level required to use the confernce   */
  int minbaud;                   /* The baudrate of the conference            */
  struct UserList *users;        /* The list of users on the conference       */
  struct Conference *nextconf;   /* The next conference in the list           */
};

/* The user list structure */

struct UserList
{
  char name[36];              /* The name of the user       */
  struct MsgPort *port;       /* The user's message port    */
  struct UserList *nextuser;  /* The next user in the list  */
};

/* The message data structure */

struct MsgData
{
  char *buffer;                  /* The text of the message       */
  unsigned char outcount;        /* The number of pending writes  */
  struct ConfMessage *origmess;  /* The original message          */
};
