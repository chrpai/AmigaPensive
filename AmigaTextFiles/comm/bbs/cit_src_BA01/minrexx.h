/*
 *   Includes for minrexx.c; please refer to that file for
 *   further documentation.
 */
#include <proto/all.h>
#include <string.h>
#include <rexx/rxslib.h>
#include <proto/rexxsyslib.h>
#include <clib/rexxsyslib_protos.h>
#include <pragmas/rexxsyslib_pragmas.h>

/*
 *   This is the list of functions we can access.  (Cheap forward
 *   declarations, too.)
 */
long upRexxPort(char *, struct rexxCommandList *, char *,
           void (*)(struct RexxMsg *, struct rexxCommandList *, char *)) ;
void dnRexxPort(void) ;
void dispRexxPort(void) ;
struct RexxMsg *sendRexxCmd(char *, void (*)(struct RexxMsg *),
           STRPTR, STRPTR, STRPTR) ;
struct RexxMsg *syncRexxCmd(char *, struct RexxMsg *msg) ;
struct RexxMsg *asyncRexxCmd(char *) ;
void replyRexxCmd(struct RexxMsg *, long, long, char *) ;
void DontReply(void) ;
/*
 *   Maximum messages that can be pending, and the return codes
 *   for two bad situations.
 */
#define MAXRXOUTSTANDING (300)
#define RXERRORIMGONE (100)
#define RXERRORNOCMD (30)
/*
 *   This is the association list you build up (statically or
 *   dynamically) that should be terminated with an entry with
 *   NULL for the name . . .
 */
struct rexxCommandList {
   char *name ;
   void (*userdata)(struct RexxMsg *, char *) ;
} ;
