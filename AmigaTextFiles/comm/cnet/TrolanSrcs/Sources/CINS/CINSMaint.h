/*
** CINSMaint Header file
** by Kelly Cochran
** Last Updated: Tue Sep 16 00:50:45 1997
*/

#ifndef CINSMAINT_H
#define CINSMAINT_H

struct UDirEnt
{
   char  UUCP   [ 12 ]; /*  0 */    /* UUCP id                             */
   BOOL  Found;         /* 12 */    /* Did we have a match here?           */
   UBYTE none   [  3 ]; /* 13 */    /* Spacer to make it a multiple of 4   */
};                      /* 16 */

#endif
