#include "cnet3lib.h"

long ConferenceWait( short a )
{
        cmess.arg1 = (ULONG) a;
        CallHost( 16 );
        return( (long)cmess.result );
}
