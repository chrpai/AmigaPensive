#include "cnet4lib.h"

long CGetFree( char *s, UBYTE q )
{
        cmess.arg1 = (ULONG)s;
        cmess.arg2 = (ULONG)q;
        CallHost( 19 );
        return( (long)cmess.result );
}
