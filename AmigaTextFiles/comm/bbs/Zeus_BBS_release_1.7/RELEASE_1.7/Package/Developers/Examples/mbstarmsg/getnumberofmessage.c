
#include "library.h"

ULONG LIBFUNC mbGetNumberOfMessages( void )
{
	struct	ZArea		*za;
	ULONG					 Number = 0;

	ObtainSemaphore( &AreaSem );

	za = (struct ZArea *)GetHead( &AreaList );

	while( za )
	{
		if( za->idx->idx.Low ) Number += ( za->idx->idx.High - za->idx->idx.Low + 1 );
		
		za = (struct ZArea *)GetSucc( (struct Node *)za );
	}

	ReleaseSemaphore( &AreaSem );

	return( Number );
}
