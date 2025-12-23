#include "cnet4lib.h"

void ReadMessagePoint( char *a, long b )
{
        cmess.arg1 = (ULONG) a;
        cmess.arg2 = (ULONG) b;
        CallHost( 27 );
}
