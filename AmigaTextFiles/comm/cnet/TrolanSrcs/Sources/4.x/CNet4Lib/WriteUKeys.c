#include "cnet4lib.h"

BYTE WriteUKeys( BYTE getsem )
{
	CallHost( 58 );
	return( (BYTE)cmess.result );
}
