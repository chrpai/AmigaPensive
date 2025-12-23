#include "cnet4lib.h"

void SetDoing( char *what )
{
        cmess.arg1 = (ULONG)what;
        CallHost( 7 );
}
