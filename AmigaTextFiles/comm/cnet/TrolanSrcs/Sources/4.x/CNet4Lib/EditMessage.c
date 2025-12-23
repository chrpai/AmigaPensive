#include "cnet4lib.h"

void EditMessage( char *file )
{
        cmess.arg1 = (ULONG) file;
        CallHost( 28 );
}
