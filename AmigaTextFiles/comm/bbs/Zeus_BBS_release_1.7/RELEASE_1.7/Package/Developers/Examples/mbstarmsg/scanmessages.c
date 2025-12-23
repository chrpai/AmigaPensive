
#include "library.h"

ULONG LIBFUNC mbScanMessages( AREG(0) struct MsgScanKey *msk, AREG(1) struct WholeMessage *wm )
{
	struct	WholeMessage	*twm;
	struct	ZArea				*za;
	struct	MsgAreaNode		*man;
	BOOL							 ret = FALSE;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( msk->AreaNumber ) )
	{
		if( man = GetMsgAreaNode( msk->AreaNumber ) )
		{
			ObtainSemaphore( &man->sem );
			
			if( twm = mbLoadMessage( msk->AreaNumber, msk->RecordsLoaded, NULL ) )
			{
				memcpy( wm, twm, sizeof(*wm) );
				FreeMem( twm, sizeof(*twm) );
				
				if( msk->Direction == SA_FORWARDS ) msk->RecordsLoaded++;
				else											msk->RecordsLoaded--;
				
				ret = TRUE;
			}

			ReleaseSemaphore( &man->sem );
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}

long LIBFUNC mbScanMsgHdrs( AREG(0) struct MsgScanKey *msk, AREG(1) struct Msg *m )
{
	struct	WholeMessage	*twm;
	struct	ZArea				*za;
	struct	MsgAreaNode		*man;
	BOOL							 ret = FALSE;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( msk->AreaNumber ) )
	{
		if( man = GetMsgAreaNode( msk->AreaNumber ) )
		{
			ObtainSemaphore( &man->sem );
			
			if( twm = mbLoadMessage( msk->AreaNumber, msk->RecordsLoaded, LM_NOTEXT ) )
			{
				memcpy( m, &twm->wm_Msg, sizeof(*m) );
				FreeMem( twm, sizeof(*twm) );
				
				if( msk->Direction == SA_FORWARDS ) msk->RecordsLoaded++;
				else											msk->RecordsLoaded--;
				
				ret = TRUE;
			}

			ReleaseSemaphore( &man->sem );
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}
