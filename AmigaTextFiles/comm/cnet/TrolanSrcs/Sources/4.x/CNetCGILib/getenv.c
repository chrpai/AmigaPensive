/*
 * Use the AmigaDOS two-level ENV variables.
 */
#include <ctype.h>
#include <proto/dos.h>

char *
getenv (const char *name)
{
    static char value[256] ;

    if (GetVar ((char *)name,value,256,0L) == -1) return NULL;
    return value;
}
