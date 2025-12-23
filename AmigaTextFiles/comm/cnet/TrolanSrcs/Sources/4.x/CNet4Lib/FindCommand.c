#include "cnet4lib.h"

short FindCommand( short num )
{
        cmess.arg1 = (ULONG) num;
        CallHost( 26 );
        return( (short)cmess.result );
}
