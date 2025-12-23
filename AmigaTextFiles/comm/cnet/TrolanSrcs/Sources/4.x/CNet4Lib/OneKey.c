#include "cnet4lib.h"

char OneKey( void )
{
        CallHost( 3 );
        return( (char)cmess.result );   /* returns key pressed */
}
