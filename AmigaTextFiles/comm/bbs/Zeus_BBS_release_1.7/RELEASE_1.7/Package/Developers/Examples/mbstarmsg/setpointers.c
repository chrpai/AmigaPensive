
#include "library.h"

int LIBFUNC mbSetPointers( AREG(0) struct MsgIdx *idx )
{
	struct	ZArea		*za;
	int					 ret = NULL;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( idx->Area ) )
	{
		memcpy( &za->idx->idx, idx, sizeof(*idx) );

		if( !za->idx->idx.Low && za->idx->idx.High ) za->idx->idx.Low = 1;
		if( !za->idx->idx.High && za->idx->idx.Low ) za->idx->idx.High = za->idx->idx.Low;
		if( za->idx->idx.Low > za->idx->idx.High ) za->idx->idx.Low = za->idx->idx.High;
		if( za->idx->idx.HighSentByNet > za->idx->idx.High ) za->idx->idx.HighSentByNet = za->idx->idx.High;
		if( za->idx->idx.HSBInterNet > za->idx->idx.High ) za->idx->idx.HSBInterNet = za->idx->idx.High;

		if( Seek( ifh, za->idx->pos, OFFSET_BEGINNING ) != -1 )
		{
			if( Write( ifh, za->idx, sizeof(struct SIdx) ) == sizeof(struct SIdx) )
			{
				ret = TRUE;
			}
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}
