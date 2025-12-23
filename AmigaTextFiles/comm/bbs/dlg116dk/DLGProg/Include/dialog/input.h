/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* Combined user/ram structure for passing to DLGQuery */

struct UserInfo
{
  struct USER_DATA *User;
  struct Ram_File *Ram;
};

/* Structure for passing to DLGQuery */

struct Query
{
  char *prompt;      /* The prompt to be displayed                      */
  char *template;    /* Template for input, or array if in guess mode   */
  char *string;      /* String to put the input in                      */
  char *defstring;   /* Default string, or current item in guess mode   */
  char *valid;       /* String containing valid input characters        */
  USHORT length;     /* Max number of characters to put in string       */
  USHORT typelength; /* Max chars typeable, or num items in guess mode  */
  ULONG flags;       /* Flags (see below)                               */
};

/* Flags for DLGQuery */

#define QUERY_UPCASE    1L    /* Convert string to upper case              */
#define QUERY_HIDDEN    2L    /* Display characters as '#'                 */
#define QUERY_CAPITAL   4L    /* Capitalize words                          */
#define QUERY_GUESS     8L    /* Use "guess" mode                          */
#define QUERY_NODEFEDT  16L   /* Don't display default to be edited        */
#define QUERY_NOSTACK   32L   /* Don't interpret ';' as a stack delimiter  */
