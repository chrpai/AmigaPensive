
#include "library.h"

long LIBFUNC mbMaintenance( DREG(0) long area )
{
	return( TRUE );
}

long LIBFUNC mbTrim( DREG(0) long area )
{
	struct	ZArea			*za;
	BPTR						 cur, old;
	UBYTE						 fn[256];
	long						 i, ret = TRUE;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( area ) )
	{
		if( za->idx->idx.Low && za->idx->idx.High )
		{
			if( za->ma->maxmsgs )
			{
				if( za->idx->idx.High - za->idx->idx.Low >= za->ma->maxmsgs )
				{
					i = za->idx->idx.High - za->ma->maxmsgs;

					sprintf( fn, "%s%s", Shared->Paths->Mail, za->ma->tagname );

					if( cur = Lock( fn, ACCESS_READ ) )
					{
						old = CurrentDir( cur );

						for( ; za->idx->idx.Low <= i ; za->idx->idx.Low++ )
						{
							sprintf( fn, "%lu.MSG", za->idx->idx.Low );
							DeleteFile( fn );
						}

						if( !mbSetPointers( &za->idx->idx ) ) ret = FALSE;

						CurrentDir( old );
						UnLock( cur );
					}
				}
			}
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}

long LIBFUNC mbUpdate( DREG(0) long area )
{
	struct	ZArea			*za;
	BPTR						 cur, old, file;
	UBYTE						 fn[256];
	long						 i, ret = NULL;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( area ) )
	{
		if( za->idx->idx.Low && za->idx->idx.High )
		{
			sprintf( fn, "%s%s", Shared->Paths->Mail, za->ma->tagname );

			if( cur = Lock( fn, ACCESS_READ ) )
			{
				old = CurrentDir( cur );

				i = za->idx->idx.High;

				sprintf( fn, "%lu.MSG", i );

				if( file = Lock( fn, ACCESS_READ ) )
				{
					while( file )
					{
						UnLock( file );
						sprintf( fn, "%lu.MSG", ++i );
						file = Lock( fn, ACCESS_READ );
					}

					za->idx->idx.High = i - 1;

					i = za->idx->idx.Low;

					while( !file )
					{
						sprintf( fn, "%lu.MSG", i++ );
						file = Lock( fn, ACCESS_READ );
					}

					UnLock( file );

					za->idx->idx.Low = i - 1;

					if( mbSetPointers( &za->idx->idx ) ) ret = TRUE;
				}

				CurrentDir( old );
				UnLock( cur );
			}
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}

long LIBFUNC mbRescan( DREG(0) long area )
{
	struct	AnchorPath	*ua;
	struct	ZArea			*za;
	BPTR						 cur, old;
	UBYTE						 fn[256];
	long						 i, rc, ret = NULL;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( area ) )
	{
		if( ua = AllocMem( sizeof(*ua), MEMF_CLEAR | MEMF_PUBLIC ) )
		{
			sprintf( fn, "%s%s", Shared->Paths->Mail, za->ma->tagname );

			if( cur = Lock( fn, ACCESS_READ ) )
			{
				old = CurrentDir( cur );

				MatchFirst( "#?.MSG", ua );

				rc  = IoErr();
				ret = TRUE;

				za->idx->idx.Low  = ~0;
				za->idx->idx.High =  0;

				while( rc == NULL )
				{
					if( ua->ap_Info.fib_DirEntryType < 0 )
					{
						if( i = atol( ua->ap_Info.fib_FileName ) )
						{
							if( i < za->idx->idx.Low  ) za->idx->idx.Low  = i;
							if( i > za->idx->idx.High ) za->idx->idx.High = i;
						}
					}
					MatchNext( ua );
					rc = IoErr();
				}

				MatchEnd( ua );

				if( rc != ERROR_NO_MORE_ENTRIES ) ret = FALSE;

				CurrentDir( old );
				UnLock( cur );
			}
			FreeMem( ua, sizeof(*ua) );
		}

		if( ret )
		{
			if( za->idx->idx.Low == ~0 )
			{
				memset( &za->idx->idx, NULL, sizeof(struct MsgIdx) );
				za->idx->idx.Area = area;
			}

			if( !mbSetPointers( &za->idx->idx ) ) ret = FALSE;
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}

long LIBFUNC mbInitialiseArea( DREG(0) long area )
{
	struct	AnchorPath	*ua;
	struct	ZArea			*za;
	BPTR						 cur, old;
	UBYTE						 fn[256];
	long						 rc, ret = NULL;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( area ) )
	{
		if( ua = AllocMem( sizeof(*ua), MEMF_CLEAR | MEMF_PUBLIC ) )
		{
			sprintf( fn, "%s%s", Shared->Paths->Mail, za->ma->tagname );

			if( cur = Lock( fn, ACCESS_READ ) )
			{
				old = CurrentDir( cur );

				MatchFirst( "#?.MSG", ua );

				rc  = IoErr();
				ret = TRUE;

				while( ret && rc == NULL )
				{
					if( ua->ap_Info.fib_DirEntryType < 0 )
					{
						if( !DeleteFile( ua->ap_Info.fib_FileName ) ) ret = FALSE;
					}
					MatchNext( ua );
					rc = IoErr();
				}

				MatchEnd( ua );

				if( ret && rc != ERROR_NO_MORE_ENTRIES ) ret = FALSE;

				CurrentDir( old );
				UnLock( cur );
			}
			FreeMem( ua, sizeof(*ua) );
		}

		if( ret )
		{
			memset( &za->idx->idx, NULL, sizeof(struct MsgIdx) );
			za->idx->idx.Area = area;

			if( !mbSetPointers( &za->idx->idx ) ) ret = FALSE;
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}

long LIBFUNC mbCreateWaitingMail( DREG(0) long area )
{
	struct	WholeMessage	*wm;
	struct	ZArea				*za;
	long							 ret = TRUE;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( area ) )
	{
		if( za->idx->idx.HighSentByNet > za->idx->idx.High ) za->idx->idx.HighSentByNet = za->idx->idx.High;

		while( za->idx->idx.HighSentByNet < za->idx->idx.High )
		{
			if( za->idx->idx.HighSentByNet < za->idx->idx.Low ) za->idx->idx.HighSentByNet = za->idx->idx.Low - 1;

			za->idx->idx.HighSentByNet++;

			if( wm = mbLoadMessage( area, za->idx->idx.HighSentByNet, LM_NOTEXT ) )
			{
				if( DoesUserExist( wm->wm_Msg.To ) )
				{
					AddWaitingMail( wm->wm_Msg.To, area, za->idx->idx.HighSentByNet );
				}
				FreeMessage( wm );
			}
		}

		if( !mbSetPointers( &za->idx->idx ) ) ret = FALSE;
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}
