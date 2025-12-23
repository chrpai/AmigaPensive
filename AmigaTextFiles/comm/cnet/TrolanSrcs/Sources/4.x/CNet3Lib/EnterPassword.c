#include "cnet3lib.h"

void EnterPassword( UBYTE len )
{
        cmess.arg1 = (ULONG)len;        /* max number of characters */
        CallHost( 4 );
}
