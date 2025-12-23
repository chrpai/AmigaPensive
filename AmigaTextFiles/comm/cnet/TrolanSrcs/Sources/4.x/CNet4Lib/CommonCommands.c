#include "cnet4lib.h"

long CommonCommands( void )
{
        CallHost( 5 );
        return( (long)cmess.result );
}
