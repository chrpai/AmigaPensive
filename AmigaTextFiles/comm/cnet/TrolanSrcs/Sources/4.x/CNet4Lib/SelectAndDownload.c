#include "cnet4lib.h"

UBYTE SelectAndDownload( char *file, UBYTE flags )
{
        cmess.arg1 = (ULONG)file;
        cmess.arg2 = (ULONG)flags;
        CallHost( 39 );
        return( (UBYTE)cmess.result );
}
