#include "cnet3lib.h"

void PutText( char *text )
{
        cmess.arg1 = (ULONG)text;       /* text to print                */
        CallHost( 1 );
}
