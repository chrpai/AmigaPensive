
#include "library.h"

struct MsgIdx * LIBFUNC mbGetPointers( DREG(0) ULONG Area )
{
	struct	ZArea		*za;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( Area ) )
	{
		ReleaseSemaphore ( &AreaSem );

		return( &za->idx->idx );
	}

	ReleaseSemaphore( &AreaSem );

	return( NULL );
}
