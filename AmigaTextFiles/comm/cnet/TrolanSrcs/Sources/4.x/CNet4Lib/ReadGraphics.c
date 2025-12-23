#include "cnet4lib.h"

UBYTE ReadGraphics( char *path, char flags )
{
        cmess.arg1 = (ULONG)path;
        cmess.arg2 = (ULONG)flags;      /* 1 = print File Not Found     */
        CallHost( 9 );
        return( (UBYTE)cmess.result );          /* FALSE if File Not Found      */
}
