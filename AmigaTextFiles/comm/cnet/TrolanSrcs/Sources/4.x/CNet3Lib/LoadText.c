#include "cnet3lib.h"

void LoadText( BPTR fh )
{
        cmess.arg1 = (ULONG) fh;
        CallHost( 29 );
}
