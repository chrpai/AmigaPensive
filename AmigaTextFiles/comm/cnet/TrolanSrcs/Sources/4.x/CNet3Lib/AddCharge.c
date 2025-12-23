#include "cnet3lib.h"

UBYTE AddCharge( short n, short a )
{
        cmess.arg1 = (ULONG)n;
        cmess.arg2 = (ULONG)a;
        CallHost( 13 );
        return( (UBYTE)cmess.result );
}
