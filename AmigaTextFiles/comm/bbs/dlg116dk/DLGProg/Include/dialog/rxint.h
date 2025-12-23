/***************************************************************************

rxint.h - definitions for the interface between DLGRx and DLG

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

struct RxQuery
{
  struct Message mess;     /* The exec message structure */
  struct USER_DATA *user;  /* User data structure        */
  struct Ram_File *ram;    /* Online user info           */
  char *command;           /* ARexx command              */
  LONG rcode;              /* Result code                */
  char *rstr;              /* Result string              */
};
