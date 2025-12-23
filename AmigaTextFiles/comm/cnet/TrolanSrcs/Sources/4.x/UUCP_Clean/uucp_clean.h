/*
*  UUCP_Clean Header file
*  by Kelly Cochran
*/

struct UDirEnt
{
   char  UUCP   [ 12 ]; /*  0 */    /* UUCP id                             */
   BOOL  Found;         /* 12 */    /* Did we have a match here?           */
   UBYTE none   [  3 ]; /* 13 */    /* Spacer to make it a multiple of 4   */
};                      /* 16 */
