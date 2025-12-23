
#include "library.h"

BOOL LoadMsg( struct ZArea *za, struct WholeMessage *wm, UBYTE *txt, ULONG flags, ULONG num )
{
	struct	MsgHdr		*mh;
	struct	FMsgHdr		*fmh;
	ULONG						 mlen;

	mh  = (struct MsgHdr  *)txt;
	fmh = (struct FMsgHdr *)txt;

	wm->wm_Msg.MsgSection = za->area;
	wm->wm_Msg.Number     = num;

	strncpy( wm->wm_Msg.From,    mh->From,    35 );
	strncpy( wm->wm_Msg.To,      mh->To,      35 );
	strncpy( wm->wm_Msg.Subject, mh->Subject, 71 );

	wm->wm_Msg.Date = ParseTime( mh->Date );

	if( mh->TimesRead == 0x465a )
	{
		/* Foozle type message */

		wm->wm_Msg.FromAddress.Zone  = za->ma->address.Zone;
		wm->wm_Msg.FromAddress.Net   = fmh->OrigNet;
		wm->wm_Msg.FromAddress.Node  = fmh->OrigNode;
		wm->wm_Msg.FromAddress.Point = za->ma->address.Point;

		wm->wm_Msg.DestAddress.Zone  = fmh->DestZone;
		wm->wm_Msg.DestAddress.Net   = fmh->DestNet;
		wm->wm_Msg.DestAddress.Node  = fmh->DestNode;
		wm->wm_Msg.DestAddress.Point = fmh->DestPoint;

		wm->wm_Msg.Flags      = Fido2Zeus( fmh->Attr );
		wm->wm_Msg.ArriveDate = fmh->TimeStamp;

		txt = fmh->MsgTxt;
	}
	else
	{
		/* Standard FTSC1 message */

		wm->wm_Msg.FromAddress.Zone  = mh->OrigZone;
		wm->wm_Msg.FromAddress.Net   = mh->OrigNet;
		wm->wm_Msg.FromAddress.Node  = mh->OrigNode;
		wm->wm_Msg.FromAddress.Point = mh->OrigPoint;

		wm->wm_Msg.DestAddress.Zone  = mh->DestZone;
		wm->wm_Msg.DestAddress.Net   = mh->DestNet;
		wm->wm_Msg.DestAddress.Node  = mh->DestNode;
		wm->wm_Msg.DestAddress.Point = mh->DestPoint;

		wm->wm_Msg.ReplyTo    = mh->ReplyTo;
		wm->wm_Msg.NextReply  = mh->NextReply;
		wm->wm_Msg.Flags      = Fido2Zeus( fmh->Attr );
		
		wm->wm_Msg.ArriveDate = Time( NULL );
		
		txt = mh->MsgTxt;
	}

	if( flags & LM_NOTEXT ) return( TRUE );

	if( flags & LM_NOFIDOINFO )
	{
		StripFido( txt );
	}

	if( mlen = strlen( txt ) )
	{
		if( wm->wm_Text = AllocMem( mlen + 1, MEMF_PUBLIC ) )
		{
			memcpy( wm->wm_Text, txt, mlen + 1 );
			
			wm->wm_TextLength = mlen + 1;
			
			wm->wm_Msg.TextLength = mlen;
			
			return( TRUE );
		}
	}

	return( FALSE );
}

struct WholeMessage * LIBFUNC mbLoadMessage( DREG(0) int Area,
														   DREG(1) ULONG Number,
															DREG(2) ULONG Flags )
{
	struct	FileInfoBlock	*fib;
	struct	WholeMessage	*wm;
	struct	MsgAreaNode		*man;
	struct	ZArea				*za;
	UBYTE							*buf;
	BPTR							 fh, lock;
	UBYTE							 fn[256];
	BOOL							 succ = FALSE;

	wm = NULL;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( Area ) )
	{
		if( Number >= za->idx->idx.Low && Number <= za->idx->idx.High )
		{
			if( man = GetMsgAreaNode( Area ) )
			{
				ObtainSemaphore( &man->sem );
				
				sprintf( fn, "%s%s/%lu.Msg", Shared->Paths->Mail, za->ma->tagname, Number );

				if( lock = Lock( fn, ACCESS_READ ) )
				{
					if( wm = AllocMem( sizeof(*wm), MEMF_CLEAR | MEMF_PUBLIC ) )
					{
						if( fib = AllocDosObject( DOS_FIB, NULL ) )
						{
							if( Examine( lock, fib ) )
							{
								if( fib->fib_DirEntryType < 0 && fib->fib_Size )
								{
									if( buf = AllocMem( fib->fib_Size + 1, MEMF_PUBLIC ) )
									{
										if( fh = OpenFromLock( lock ) )
										{
											lock = NULL;

											if( Read( fh, buf, fib->fib_Size ) == fib->fib_Size )
											{
												buf[ fib->fib_Size ] = 0x00;
												
												if( LoadMsg( za, wm, buf, Flags, Number ) )
												{
													succ = TRUE;
												}
											}
											Close( fh );
										}
										FreeMem( buf, fib->fib_Size + 1 );
									}
								}
							}
							FreeDosObject( DOS_FIB, fib );
						}
						if( !succ )
						{
							FreeMem( wm, sizeof(*wm) );
							wm = NULL;
						}
					}
					if( lock ) UnLock( lock );
				}
				ReleaseSemaphore( &man->sem );
			}
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( wm );
}
