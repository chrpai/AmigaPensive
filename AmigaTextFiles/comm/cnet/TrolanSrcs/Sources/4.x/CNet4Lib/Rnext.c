#include "cnet4lib.h"

UBYTE Rnext( void )
{
        CallHost( 24 );
        return( (UBYTE)cmess.result );
}
