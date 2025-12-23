/***************************************************************************

 © copyright 1995-96 by DLG Development
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

#define CRONCONTROL "TPTCRON"

/* CronMessage types */

#define ADDEVENT     1  /* Add an event                           */
#define DELEVENT     2  /* Delete an event                        */
#define LISTEVENTS   3  /* List dynamic events                    */
#define CRONEXIT     4  /* Shut down TPTCron                      */
#define WHENEVENT    5  /* Check the next occurrence of an event  */
#define READFILE     6  /* Read a new CronTab file                */
#define TABLIST      7  /* List CronTab events                    */
#define CHANGEDIR    8  /* Change TPTCron's working directory     */

/* Errors returned in the type field *
 * If the type was DELEVENT, it will *
 * return the number of events that  *
 * actually were deleted.            */

#define CNOERR       0  /* No error                */
#define OUTOFMEM     -1 /* Out of memory error     */
#define BADSYNTAX    -2 /* Bad message type        */
#define NOCRON       -3 /* TPTCron not active      */
#define TABNOTFOUND  -4 /* CronTab not found       */
#define NOEVENTS     -5 /* No events to list       */
#define DIRNOTFOUND  -6 /* New directory not found */


/* Structure of CronTab events */

struct CronEntry
{
  char *min, *hour, *day, *month, *wday, *command;
};


/* Message structure to issue a command to TPTCron */

struct CronMessage
{
  struct Message mess;  /* Exec Message structure                 */
  char type;            /* Message type                           */
  ULONG minutes;        /* Minutes until event                    */
  char *command;        /* Event to add/delete/search for, or     */
                        /* new CronTab file or directory          */
  BPTR ofh;             /* AmigaDOS FileHandle to send output to  */
};


/* Structure of dynamic events */

struct EventNode
{
  ULONG time;                    /* Time event is to be executed  */
  char *command;                 /* Command to be executed        */
  struct EventNode *nextevent;   /* Pointer to next event in list */
};
