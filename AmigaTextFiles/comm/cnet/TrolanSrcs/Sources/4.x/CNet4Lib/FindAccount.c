#include "cnet4lib.h"

#ifdef CNET4
short FindAccount( char *a, struct UserData *b, UBYTE c )
#else
short FindAccount( char *a, struct UserData *b )
#endif
{
        cmess.arg1 = (ULONG)a;
        cmess.arg2 = (ULONG)b;
#ifdef CNET4
        cmess.arg3 = (ULONG)c;
#endif
        CallHost( 20 );
        return( (short)cmess.result );
}
