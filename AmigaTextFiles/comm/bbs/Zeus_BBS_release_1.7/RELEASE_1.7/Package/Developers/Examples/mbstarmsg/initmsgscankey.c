
#include "library.h"

struct MsgScanKey * LIBFUNC mbInitMsgScanKey( AREG(0) struct ScanInfo *si )
{
	struct	MsgScanKey		*key;
	struct	ZArea				*za;

	if( key = AllocMem( sizeof(*key), MEMF_CLEAR | MEMF_PUBLIC ) )
	{
		if( za = FindArea( si->Area ) )
		{
			key->AreaNumber = si->Area;
			key->Direction  = si->Direction;

			if( si->StartNumber )
			{
				     if( si->StartNumber < za->idx->idx.Low  ) key->RecordsLoaded = za->idx->idx.Low;
				else if( si->StartNumber > za->idx->idx.High ) key->RecordsLoaded = za->idx->idx.High;
				else														  key->RecordsLoaded = si->StartNumber;
			}

			return ( key ) ;
		}
		FreeMem( key, sizeof(*key) );
	}

	return( NULL );
}

void LIBFUNC mbCloseMsgScanKey( AREG(0) struct MsgScanKey *key )
{
	FreeMem( key, sizeof(*key) );
}
