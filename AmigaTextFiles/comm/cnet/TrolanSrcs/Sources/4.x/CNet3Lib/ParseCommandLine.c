#include "cnet3lib.h"

void ParseCommandLine( UBYTE numargs )
{
        cmess.arg1 = (ULONG)numargs;
        CallHost( 25 );
}
