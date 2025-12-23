
#include "library.h"

void LIBFUNC mbQuit( void )
{
	struct	ZArea		*za;

	ObtainSemaphore( &AreaSem );

	while( za = (struct ZArea *)RemHead( &AreaList ) )
	{
		FreeMem( za, sizeof(*za) );
	}

	ReleaseSemaphore( &AreaSem );
}
