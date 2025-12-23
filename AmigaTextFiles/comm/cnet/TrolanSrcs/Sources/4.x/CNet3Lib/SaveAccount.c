#include "cnet3lib.h"

UBYTE SaveAccount( struct UserData *user, short id )
{
        cmess.arg1 = (ULONG)user;
        cmess.arg2 = (ULONG)id;
        CallHost( 12 );
        return( (UBYTE)cmess.result );
}
