
#include "library.h"

signed int LIBFUNC mbSaveMessage( AREG(0) struct WriteMsgStruct *wms )
{
	struct	MsgAreaNode		*man;
	struct	ZArea				*za;
	struct	FMsgHdr			*fh;
	struct	MsgHdr			*mh;
	UBYTE							 fn[256];
	int							 res = 0;
	BPTR							 file;
	LONG							 ret;
	UWORD							 BaseType;
	BOOL							 wres;
	time_t						 t;

	if( fh = AllocMem( sizeof(*fh), MEMF_CLEAR | MEMF_PUBLIC ) )
	{
		mh = (struct MsgHdr *)fh;

		strncpy( mh->From,    wms->From,    35 );
		strncpy( mh->To,      wms->To,		35 );
		strncpy( mh->Subject, wms->Subject, 71 );

		if( wms->OriginalTime )
			t = wms->OriginalTime;
		else
			t = Time( NULL );

		if( t )
		{
			struct	tm		tm;

			GMTime( t, &tm );

			if( fmt == SM_TRAPTOSS )
				strftime( mh->Date, 20, "%a %d %b %y %H:%M", &tm );
			else
				strftime( mh->Date, 20, "%d %b %y  %H:%M:%S", &tm );
		}

		if( fmt == SM_FOOZLE ) mh->TimesRead = 0x465A;

		mh->DestNode	= wms->DestAddress.Node;
		mh->OrigNode	= wms->FromAddress.Node;

		mh->DestNet		= wms->DestAddress.Net;
		mh->OrigNet		= wms->FromAddress.Net;

		if( fmt == SM_FOOZLE )
		{
			fh->TimeStamp = t;
			fh->DestZone  = wms->DestAddress.Zone;
			fh->Attr      = Zeus2Fido( wms->Flags );
			fh->DestPoint = wms->DestAddress.Point;
		}
		else
		{
			mh->DestZone	= wms->DestAddress.Zone;
			mh->OrigZone	= wms->FromAddress.Zone;

			mh->DestPoint	= wms->DestAddress.Point;
			mh->OrigPoint	= wms->FromAddress.Point;

			mh->ReplyTo		= wms->ReplyTo;
			mh->Attr       = Zeus2Fido( wms->Flags );
		}

		ObtainSemaphore( &AreaSem );

		if( za = FindArea( wms->MessageArea ) )
		{
			if( man = GetMsgAreaNode( wms->MessageArea ) )
			{
				ObtainSemaphore( &man->sem );

				BaseType = man->area->basetype;
					
				sprintf( fn, "%s%s/%lu.Msg", Shared->Paths->Mail, za->ma->tagname, za->idx->idx.High + 1 );
					
				if( file = Open( fn, MODE_NEWFILE ) )
				{
					wres = FALSE;

					if( fmt == SM_FOOZLE )
					{
						if( Write( file, fh, sizeof(*fh) ) == sizeof(*fh) ) wres = TRUE;
					}
					else
					{
						if( Write( file, mh, sizeof(*mh) ) == sizeof(*mh) ) wres = TRUE;
					}

					if( wres )
					{
						if( wms->FullText )
						{
							if( wms->FullTextLength )
								ret = Write( file, wms->FullText, wms->FullTextLength );
							else
								ret = Write( file, wms->FullText, strlen( wms->FullText ) );
						}
						else
						{
							if( wms->FidoText && ( BaseType == BT_NETMAIL || BaseType == BT_ECHOMAIL ) )
							{
								Write( file, wms->FidoText, strlen( wms->FidoText ) );
							}
								
							if( wms->Text )
							{
								Write( file, wms->Text, strlen( wms->Text ) );
							}
								
							if( BaseType == BT_ECHOMAIL )
							{
								long		len;

								Write( file, Tear, strlen( Tear ) );
									
								len = strlen( wms->Origin );

								if( len )
								{
									Write( file, wms->Origin, len );
									
									if( wms->Origin[ len - 1 ] != '\r' && wms->Origin[ len - 1 ] != '\n' )
									{
										Write( file, "\r", 1 );
									}
								}

								if( wms->SeenBys )
								{
									Write( file, wms->SeenBys, strlen( wms->SeenBys ) );
								}
							}
						}
							
						Write( file, "\0", 1 );
					}
					Close( file );
				}
					
				res = ++za->idx->idx.High;

				if( !za->idx->idx.Low ) za->idx->idx.Low = 1;

				ReleaseSemaphore( &man->sem );
			}
		}

		ReleaseSemaphore( &AreaSem );

		if( !wms->NoPointerUpdate && res > 0 )
		{
			if( !mbSetPointers( &za->idx->idx ) ) res = -7;
		}
		FreeMem( fh, sizeof(*fh) );
	}

	return ( res ) ;
}
