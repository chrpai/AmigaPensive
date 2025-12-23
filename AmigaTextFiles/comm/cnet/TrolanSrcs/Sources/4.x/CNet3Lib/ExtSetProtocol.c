#include "cnet3lib.h"

UBYTE ExtSetProtocol( char a )
{
        cmess.arg1 = (ULONG)a;
        CallHost( 43 );
        return( (UBYTE)cmess.result );
}
