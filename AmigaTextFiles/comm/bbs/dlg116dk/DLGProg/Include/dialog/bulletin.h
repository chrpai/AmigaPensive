/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* Bulleting header - MSG:0/<number>.msg */

struct Bulletin
{
  char    From[36];   /* name of the person writing the bulletin              */
  char    To[36];     /* the To: field of the bulletin                        */
  char    Title[72];  /* Title: field of the bulletin                         */
  char    Date[20];   /* string representing date when bulletin was created   */
  SHORT   Attribute;  /* see defines below                                    */
  long    Expire;     /* manx encoded long of when the bulletin will expire   */
};

#define STACK   1
