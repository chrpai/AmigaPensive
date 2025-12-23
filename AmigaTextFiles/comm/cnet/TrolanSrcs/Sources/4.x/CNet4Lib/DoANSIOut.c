#include "cnet4lib.h"

void DoANSIOut( UBYTE n)
{
	cmess.arg1 = (ULONG)n;
	CallHost( 60 );
	return;
}
