
#include "library.h"

signed int LIBFUNC mbDeleteMessage( DREG(0) int Area, DREG(1) ULONG Number )
{
	struct	ZArea				*za;
	struct	MsgAreaNode		*man;
	UBYTE							 fn[256];
	int							 ret = FALSE;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( Area ) )
	{
		if( man = GetMsgAreaNode( Area ) )
		{
			ObtainSemaphore( &man->sem );

			sprintf( fn, "%s%s/%lu.MSG", Shared->Paths->Mail, za->ma->tagname, Number );

			if( DeleteFile( fn ) )
			{
				if( za->idx->idx.Low < za->idx->idx.High )
				{
					if( Number == za->idx->idx.Low  ) za->idx->idx.Low++;
					if( Number == za->idx->idx.High ) za->idx->idx.High--;
				}

				ret = TRUE;
			}
			
			ReleaseSemaphore( &man->sem );
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}
