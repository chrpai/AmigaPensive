/*
 * Interface: online.lib, allgemeiner Online-Kommando-Header
 *
 * $RCSfile$
 * $Author$
 * $Revision$
 * $Date$
 * $State$
 * $Locker$
 *
 * $Log$
 */

#ifndef _cl_onlinelib_H
#define _cl_onlinelib_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <dos/dos.h>
#include <dos/rdargs.h>
#include <libraries/locale.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>

#include <CL/Types.h>
#include <CL/cl_prefs.h>
#include <CL/clutil.h>
#include <CL/cl_text.h>
#include <CL/UserShell.h>


/*
 * Zeiger auf UserShell-Struktur (Runtime-Environment)
 */

extern CLUserShell *__clusershell;
#define shell __clusershell


/*
 * Automatisch geöffneter CL-Text-Katalog,
 * Utility-Makros dazu
 */

extern struct Locale		*__cmd_locale;
extern APTR					__cmd_catalog;
extern APTR					__shell_catalog;

/* utility macros */
#define MSG( txt, def )		CLTXT_GetString( __cmd_catalog, txt, def )
#define SHMSG( txt, def )	CLTXT_GetString( __shell_catalog, txt, def )

/* these use CLTXT token replacement functions... */
APTR onl_sprintf( STRPTR to, STRPTR format, ... );
APTR onl_printf( STRPTR format, ... );


/*
 * Support-Funktionen
 */

#define IsUserPM( user, brett ) \
	( brett && user && ( !*brett || Stricmp( brett, user->Username ) == 0 ) )


/*
 * Range-Support
 */

typedef struct cl_rangenode cl_rangenode;

struct cl_rangenode
{
	cl_rangenode	*next;
	ulong			from, to;
};

cl_rangenode *Text2Range( STRPTR *arg );
cl_rangenode *FindRangeNode( cl_rangenode *head, ulong id );


/*
 * Signal-Handling
 */

extern ulong __signals;
#define CHECK_BREAK		(__signals = CheckSignal( \
	SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_D ))
#define BREAK_C			(__signals & SIGBREAKF_CTRL_C)
#define BREAK_D			(__signals & SIGBREAKF_CTRL_D)


/*
 * Standard-Texte
 */

#define TXT_NOMEM SHMSG( "NoMem", "$(BOLD)Out of memory!$(NORMAL)\n" )
#define TXT_BREAK SHMSG( "BreakDetected", "$(BOLD)[BREAK]$(NORMAL)\n" )


#endif
