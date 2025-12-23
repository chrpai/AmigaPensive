/*
** CINS Control Module structure definitions
** by Kelly Cochran
** Last Updated: Tue Sep  9 22:23:14 1997
*/

#include "CINS.h"

struct CINSMainPort
{
   struct MsgPort    cinsport;   /* CINS Messaging Port         */
   long   Users;                 /* # of user's in CINS         */
   long   FTP;                   /* # of active FTPs            */
   char   *cmbegin;              /* Start of mem alloc for cm   */
   char   **cm;                  /* CINS.txt pointer array      */
   char   *cmdbegin;             /* Start of mem alloc for cmd  */
   char   **cmd;                 /* CINS.cmd pointer array      */
   struct Window     *win;       /* CINS Control main window    */
   struct Menu       *menu;      /* CINS Control menus          */
   struct VisualInfo *vi;        /* VisualInfo for CINS' screen */
   struct CINSCfg    cfg;        /* CINS.cfg info               */
};
