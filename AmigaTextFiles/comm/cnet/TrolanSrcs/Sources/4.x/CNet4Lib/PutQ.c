#include "cnet4lib.h"

UBYTE PutQ( char *a )
{
        PutText( a );
        return (UBYTE)(z->MCIcreg[0][0]=='1') ;
}
