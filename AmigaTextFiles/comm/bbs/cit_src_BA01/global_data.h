/**
  Global data items.

  In most files, EXTERN is defined to be extern.  In CTDL.C it is
  defined to be NULL.  All global data is part of CTDL.C
**/

EXTERN char   ExitToMsdos;     /* True when time to bring system down  */
EXTERN int    exitValue;
EXTERN char   MeetDisabled;
EXTERN char   ConsolePassword;

EXTERN char *SysVers;
EXTERN int  SystemPort;

EXTERN int  iconify_window;   /* not iconified so printf uses
                                       crash.sys for output in startup     */

EXTERN char TDirBuffer[120];

EXTERN char *R_W_ANY     ;
EXTERN char *READ_ANY    ;
EXTERN char *READ_TEXT   ;
EXTERN char *APPEND_TEXT ;
EXTERN char *APPEND_ANY  ;
EXTERN char *A_C_TEXT    ;
EXTERN char *WRITE_TEXT  ;
EXTERN char *W_R_ANY     ;
EXTERN char *WRITE_ANY   ;


EXTERN char results_debug;  /* debug modem results codes */
EXTERN char results_cid;    /*  collect caller ID info if present */

