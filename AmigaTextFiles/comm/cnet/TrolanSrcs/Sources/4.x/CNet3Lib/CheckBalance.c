#include "cnet3lib.h"

UBYTE CheckBalance( short n, short a )
{
        cmess.arg1 = (ULONG)n;
        cmess.arg2 = (ULONG)a;
        CallHost( 14 );
        return( (UBYTE)cmess.result );
}
