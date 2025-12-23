#include "cnet3lib.h"

long GetFree( char *s, UBYTE q )
{
        cmess.arg1 = (ULONG)s;
        cmess.arg2 = (ULONG)q;
        CallHost( 19 );
        return( (long)cmess.result );
}
