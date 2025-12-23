#include "cnet4lib.h"

UBYTE ReadFile( char *path, UBYTE flags )
{
        cmess.arg1 = (ULONG)path;
        cmess.arg2 = (ULONG)flags;          /* 1 = print File Not Found */
        CallHost( 6 );
        return( (UBYTE)cmess.result );      /* returns FALSE if File Not Found */
}
