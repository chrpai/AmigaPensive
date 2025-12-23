/*
 * $NAME: cllib.h
 * $VER: 11.0
 *
 */

#ifndef _CL_CLLIB_H
#define _CL_CLLIB_H

#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif

/*
 *  "cl.lib" 2.0 memory support functions
 *
 *  The cl.lib memory support functions consist of a constructor/destructor
 *  pair which initializes and destroy an exec memory pool via clutil.library.
 *  cl_alloc(), cl_free() and cl_strdup() work as their standard counterparts,
 *  just via the pool. Every and all memory left ist automagically freed
 *  upon program termination.
 *
 *  You can modify the pool attributes with three longwords. Default defines
 *  are also contained within "cl.lib", but may be overriden on your behalf.
 *
 *  long __clpoolmemflags	   Default: MEMF_CLEAR
 *  long __clpoolpuddlesize    Default: 8192
 *  long __clpooltresholdsize  Default: 4096
 *
 *  These functions are not reentrant (unless linked with "cres.o")
 *
 *  These functions are just a _very_ thin layer over "clutil.library"
 *  and quite efficient.
 */

void * __regargs cl_alloc( int );     /* allocate some memory from the pool */
void __regargs cl_free( void * );     /* free it afterwards */
char * __regargs cl_strdup( char * ); /* duplicate string */

/*
 * "cl.lib" 4.0 hook support
 *
 * cl_allochook( function_pointer, data ) allocates a hook structure
 * via cl_alloc() and fills out the h_Entry and h_Data fields
 *
 * cl_allochooka4( funcptr ) allocates a hook with automagic
 * A4 saving.
 *
 * cl_allochooka4d( funcptr, APTR ) allocates a hook with automagic
 * A4 saving and fills out h_Data.
 *
 */

struct Hook * __regargs cl_allochook( void*(*)(), APTR );
#define cl_allochook(f,d) cl_allochook((void*(*)())f,(APTR)(d))
struct Hook * __regargs cl_allochooka4( void*(*)());
#define cl_allochooka4(f) cl_allochooka4((void*(*)())f)
struct Hook * __regargs cl_allochooka4d( void*(*)(), APTR );
#define cl_allochooka4d(f,d) cl_allochooka4d((void*(*)())f,(APTR)(d))

/*
 * "cl.lib" 2.0 ReadArgs() template support functions
 *
 * Define your command template by setting
 * char *__clargstemplate = "<ReadArgs-Template>"
 * The args array will automagically be allocated by
 * the lib function.
 *
 * Setting "__clargshelp" to a non-NULL value enables
 * ReadArgs's extend help facility.
 *
 * A ReadArgs() error will cause your application to
 * terminate immediately, printing an errorcode consisting
 * of the CLI program name and the errorstring (via PrintFault())
 *
 * The ReadArgs() function is linked in by
 * referencing "__clargs" inside your code.
 *
 * New for V6: setting __clargsquiet to the integer index
 * of a switch item inside your args list causes your
 * process Input/Output handles to be replaced bei NIL:.
 * This is for easy implementation of QUIET/S-like
 * template options.
 *
 */

/*
extern char * __clargshelp;
extern long __clargsquiet;
extern struct myargs * __clargs;
*/

/*
 * "cl.lib" 3.0 Locale support
 *
 * Define the name of your program's catalog as
 * char *__clcatalogname = "prefs/system.catalog";
 * and dereference the given LocaleInfo structure.
 *
 * The constructor temporarly changes PROGDIR:
 * to point to CONNECTLINE: in order to allow
 * applications to be started from wherever the
 * user wants without worrying about the path.
 *
 * You may set
 * char *__clbuiltinlanguage = "...";
 * to change the builtin language name. Defaults
 * to "Deutsch".
 *
 */

extern struct LocaleInfo __cllocaleinfo;

/*
 * "cl.lib" 5.0 priority support
 *
 * Modify your process' priority using
 * this function. The old priority will
 * be restored upon program exit.
 *
 */

void __regargs cl_setpri( int );

/*
 * "cl.lib" 5.0 pr_WindowPtr support
 *
 * Modify your process' pr_WindowPtr using
 * this function. The old pr_WindowPtr will
 * be restored upon program exit.
 *
 */

void __regargs cl_setprwindowptr( struct Window * );

#define PRWINDOWPTR_QUIET ((struct Window*)-1)

/*
 * "cl.lib" 7.0 logfile support
 *
 * Output log entries via cl_addlog(). The log prefix
 * for your application is defined by setting
 *
 *  char *__cllogid = "YOURAPP";
 *
 */

/*
 * NEW FOR V11: now loglevel is supported!
 */

void __stdargs cl_addlog( LONG loglevel, STRPTR fmt, ... );
void __regargs cl_addlogargs( LONG loglevel, STRPTR, APTR );


/*
 * "cl.lib" 8.0 current directory stack support
 *
 */

BOOL __regargs cl_pushdir( STRPTR );
void __regargs cl_popdir( void );

#endif
