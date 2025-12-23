#include "cnet4lib.h"

char WaitForInput( long mics )
{
        cmess.arg1 = (ULONG) mics;
        CallHost( 31 );
        return( (char)cmess.result );
}
