#include "cnet3lib.h"

UBYTE ReadAccount( short id, struct UserData *user )
{
        cmess.arg1 = (ULONG)id;
        cmess.arg2 = (ULONG)user;
        CallHost( 11 );
        return( (UBYTE)cmess.result );
}
