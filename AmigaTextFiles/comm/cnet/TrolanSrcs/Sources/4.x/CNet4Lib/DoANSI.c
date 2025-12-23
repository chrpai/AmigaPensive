#include "cnet4lib.h"

void DoANSI( UBYTE n, USHORT a, USHORT b)
{
	cmess.arg1 = (ULONG)n;
	cmess.arg2 = (ULONG)a;
	cmess.arg3 = (ULONG)b;
	CallHost( 59 );
	return;
}
