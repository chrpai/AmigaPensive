#include "cnet3lib.h"

int EnterText( char firstchar, short maxchars, short perline, short maxlines )
{
        cmess.arg1 = (ULONG)firstchar;
        cmess.arg2 = (ULONG)maxchars;
        cmess.arg3 = (ULONG)perline;
        cmess.arg4 = (ULONG)maxlines;
        CallHost( 15 );
        return( (int)cmess.result );
}
