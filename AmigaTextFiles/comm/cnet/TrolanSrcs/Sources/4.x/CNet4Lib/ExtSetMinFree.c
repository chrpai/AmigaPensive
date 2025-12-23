#include "cnet4lib.h"

void ExtSetMinFree( long free )
{
        cmess.arg1 = (ULONG)free;
        CallHost( 42 );
}
