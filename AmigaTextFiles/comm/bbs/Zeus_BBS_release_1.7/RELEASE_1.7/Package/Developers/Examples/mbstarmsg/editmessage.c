
#include "library.h"

signed int LIBFUNC mbEditMessage( AREG(0) struct Msg *m, AREG(1) unsigned char *Text )
{
	struct	WholeMessage	*wm;
	struct	MsgAreaNode		*man;
	struct	FMsgHdr			*fh;
	struct	MsgHdr			*mh;
	struct	ZArea				*za;
	struct	tm					 tm;
	UBYTE							 fn[256];
	LONG							 len;
	BPTR							 file;
	BOOL							 wres;
	int							 ret = 0;

	if( !m ) return ( -1 ) ;

	ObtainSemaphore( &AreaSem );

	if( za = FindArea( m->MsgSection ) )
	{
		if( fh = AllocMem( sizeof(*fh), MEMF_CLEAR | MEMF_PUBLIC ) )
		{
			mh = (struct MsgHdr *)fh;

			wm = NULL;

			if( !Text )
			{
				if( wm = LoadMessage( m->MsgSection, m->Number, NULL ) )
				{
					Text = wm->wm_Text;
				}
				else
				{
					ReleaseSemaphore( &AreaSem );
					return( -1 );
				}
			}
			
			strcpy( mh->From,		m->From );
			strcpy( mh->To,		m->To );
			strcpy( mh->Subject, m->Subject );
			
			GMTime( m->Date, &tm );

			if( fmt == SM_TRAPTOSS )
				strftime( mh->Date, 20, "%a %d %b %y %H:%M", &tm );
			else
				strftime( mh->Date, 20, "%d %b %y  %H:%M:%S", &tm );

			if( fmt == SM_FOOZLE ) mh->TimesRead = 0x465A;

			mh->DestNode	= m->DestAddress.Node;
			mh->OrigNode	= m->FromAddress.Node;

			mh->DestNet		= m->DestAddress.Net;
			mh->OrigNet		= m->FromAddress.Net;

			if( fmt == SM_FOOZLE )
			{
				fh->TimeStamp = m->Date;
				fh->DestZone  = m->DestAddress.Zone;
				fh->Attr      = Zeus2Fido( m->Flags );
				fh->DestPoint = m->DestAddress.Point;
			}
			else
			{
				mh->DestZone	= m->DestAddress.Zone;
				mh->OrigZone	= m->FromAddress.Zone;

				mh->DestPoint	= m->DestAddress.Point;
				mh->OrigPoint	= m->FromAddress.Point;

				mh->ReplyTo		= m->ReplyTo;
				mh->Attr       = Zeus2Fido( m->Flags );
				mh->NextReply	= m->NextReply;
			}

			if( man = GetMsgAreaNode( m->MsgSection ) )
			{
				ObtainSemaphore( &man->sem );

				sprintf( fn, "%s%s/%lu.Msg", Shared->Paths->Mail, za->ma->tagname, m->Number );

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
						len = strlen( Text ) + 1;
						
						if( Write( file, Text, len ) == len )
						{
							ret = 1;
						}
					}
					Close( file );
				}
				ReleaseSemaphore( &man->sem );
			}

			if( wm ) FreeMessage( wm );
			
			FreeMem( fh, sizeof(*fh) );
		}
	}

	ReleaseSemaphore( &AreaSem );

	return( ret );
}
