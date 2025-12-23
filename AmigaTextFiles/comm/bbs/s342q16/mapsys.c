/*
 *				mapsys.c
 *
 */

/*
 *				history
 *
 * 89Feb02 HAW  Created.
 */

#define SYSTEM_DEPENDENT
#include "ctdl.h"
#include "stdarg.h"
/*
 *				Contents
 *
 *	ToFile()		Gets stuff to a file.
 */

int putRouteChar(int c);
int (*ToFileWork)(int c) = putRouteChar;

/*
 * ToFile()
 *
 * Stuff stuff to a file and append a 0.
 */
void ToFile(char *format, ...)
{
    char garp[MAXTEXT];
    va_list argptr;
    int i;

    va_start(argptr, format);
    vsprintf(garp, format, argptr);
    va_end(argptr);
    for (i = 0; garp[i]; i++) (*ToFileWork)(garp[i]);
    (*ToFileWork)(0);
}
