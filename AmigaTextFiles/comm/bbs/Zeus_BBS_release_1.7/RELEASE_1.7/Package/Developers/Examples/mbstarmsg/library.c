
#define MAIN

#include "library.h"

UBYTE						 *cfgname = "Zeus:Config/Msg/StarMsg.dat";

void DosErr( UBYTE *fmt, ... )
{
	va_list	args;
	UBYTE		buf[256];

	if( IoErr() )
	{
		va_start( args, fmt );

		ELVPrintf( 'D', fmt, args );

		va_end( args );

		if( DOSBase )
		{
			if( Fault( IoErr(), NULL, buf, 256 ) )
			{
				ELPrintf( 'D', "mbstarmsg: Dos Error (%ld) %s", IoErr(), buf );
			}
			else if( IoErr() )
			{
				ELPrintf( 'D', "mbstarmsg: Dos Error (%ld)", IoErr() );
			}
		}
	}
}

struct ZArea *FindArea( ULONG area )
{
	struct	ZArea		*za;

	za = (struct ZArea *)GetHead( &AreaList );

	while( za && za->area != area ) za = (struct ZArea *)GetSucc( za );

	return( za );
}

struct SIdx *FindIdx( ULONG area )
{
	struct	SIdx		*si;
	struct	SIdxN		*sn;
	ULONG					 i;

	if( idxs )
	{
		si = idxs;

		for( i = 0 ; i < inum ; i++ )
		{
			if( si->idx.Area == area ) return( si );
			
			si++;
		}

		sn = (struct SIdxN *)GetHead( &IdxList );

		while( sn )
		{
			if( sn->idx.idx.Area == area ) return( &sn->idx );
			
			sn = (struct SIdxN *)GetSucc( sn );
		}
	}

	return( NULL );
}

void RemDupeArea( ULONG number )
{
	struct	ZArea		*za;

	if( za = FindArea( number ) )
	{
		Remove( (struct Node *)za );
		FreeMem( za, sizeof(*za) );
	}
}

int LIBFUNC mbInitBase( AREG(0) struct msgarea *area )
{
	struct	ZArea		*za;
	struct	SIdx		*idx;
	struct	SIdxN		*in;
	int					 ret = FALSE;

	if( Shared )
	{
		ObtainSemaphore( &AreaSem );

		RemDupeArea( area->number );

		if( za = AllocMem( sizeof(*za), MEMF_CLEAR | MEMF_PUBLIC ) )
		{
			if( ( idx = FindIdx( area->number ) ) == NULL )
			{
				if( in = AllocMem( sizeof(*in), MEMF_CLEAR | MEMF_PUBLIC ) )
				{
					if( Seek( ifh, NULL, OFFSET_END ) != -1 )
					{
						in->idx.pos      = ipos;
						in->idx.idx.Area = area->number;

						if( Write( ifh, idx, sizeof(*idx) ) == sizeof(*idx) )
						{
							ipos += sizeof(struct SIdx);
							idx = &in->idx;
							AddTail( &IdxList, (struct Node *)in );
						}
						else
						{
							FreeMem( in, sizeof(*in) );
						}
					}
					else
					{
						FreeMem( in, sizeof(*in) );
					}
				}
				else
				{
				}
			}

			if( idx )
			{
				UBYTE		fn[256];
				BPTR		lock;

				sprintf( fn, "%s%s", Shared->Paths->Mail, area->tagname );

				if( ( lock = Lock( fn, ACCESS_READ ) ) == NULL )
				{
					lock = CreateDir( fn );
				}

				if( lock )
				{
					UnLock( lock );

					za->ma   = area;
					za->area = area->number;
					za->idx  = idx;
					za->idx->idx.Area = area->number;

					if( za->idx->idx.High && !za->idx->idx.Low ) za->idx->idx.Low = 1;

					AddTail( &AreaList, (struct Node *)za );

					ret = TRUE;
				}
				else
				{
					FreeMem( za, sizeof(*za) );
				}
			}
			else
			{
				FreeMem( za, sizeof(*za) );
			}
		}
		ReleaseSemaphore( &AreaSem );
	}

	return( ret );
}

BOOL LIBFUNC mbInitialise( AREG(0) struct MsgGlobals *gl )
{
	struct	FileInfoBlock		*fib;
	BPTR								 cfg;
	BOOL								 ret = FALSE;
	LONG								 dres;

	if( Shared = GetShared() )
	{
		DOSBase      = Shared->DOSBase;
		PipeUtilBase = Shared->PipeUtilBase;
		UtilityBase  = Shared->UtilityBase;
		msgglobals   = gl;

		sprintf( Tear, "\r--- Zeus BBS %ld.%ld\r", Shared->ZVer, Shared->ZRev );
		sprintf( idxname, "%sStarMsg.idx", Shared->Paths->Mail );

		fmt = SM_FTSC;

		if( cfg = Open( cfgname, MODE_OLDFILE ) )
		{
			if( Read( cfg, &fmt, 4 ) != 4 ) fmt = SM_FTSC;

			Close( cfg );
		}

		if( ifh = Open( idxname, MODE_OLDFILE ) )
		{
			if( fib = AllocDosObject( DOS_FIB, NULL ) )
			{
				if( ExamineFH( ifh, fib ) )
				{
					if( fib->fib_DirEntryType < 0 )
					{
						if( fib->fib_Size )
						{
							if( idxs = AllocMem( fib->fib_Size, MEMF_PUBLIC ) )
							{
								isize = fib->fib_Size;
								inum  = isize / sizeof(struct SIdx);

								dres = Read( ifh, idxs, fib->fib_Size );

								if( dres == fib->fib_Size )
								{
									struct	SIdx		*si;
									ULONG					 i;

									for( ipos = 0, i = 0, si = idxs ; i < inum ; i++, si++, ipos += sizeof(struct SIdx) )
									{
										si->pos = ipos;
									}
										
									ret = TRUE;
								}
								else if( dres == -1 )
								{
									DosErr( "mbstarmsg: Error reading from index file" );
									ret = FALSE;
								}
								else
								{
									DosErr( "mbstarmsg: Short read from index file" );
									ret = FALSE;
								}

								if( !ret )
								{
									FreeMem( idxs, fib->fib_Size );
									idxs = NULL;
								}
							}
						}
						else
						{
							ret = TRUE;
						}
					}
				}
				FreeDosObject( DOS_FIB, fib );
			}

			if( !ret )
			{
				Close( ifh );
				ifh = NULL;
			}
		}
		else if( ifh = Open( idxname, MODE_NEWFILE ) )
		{
			ret = TRUE;
		}
		else
		{
			DosErr( "mbstarmsg: Error opening index file" );
		}
	}

	return( ret );
}

void LIBFUNC mbQuit( void )
{
	struct	ZArea		*za;
	struct	SIdxN		*sn;

	ObtainSemaphore( &AreaSem );

	while( za = (struct ZArea *)RemHead( &AreaList ) ) FreeMem( za, sizeof(*za) );

	while( sn = (struct SIdxN *)RemHead( &IdxList  ) ) FreeMem( sn, sizeof(*sn) );

	ReleaseSemaphore( &AreaSem );

	if( idxs )
	{
		FreeMem( idxs, isize );
		idxs = NULL;
	}

	if( ifh )
	{
		Close( ifh );
		ifh = NULL;
	}
}

int LIBFUNC mbGetInfo( AREG(0) struct MsgInfo *info )
{
	strcpy( info->Name, "StarMsg v1.1" );
	strcpy( info->Info, "*.MSG format" );
	strcpy( info->Author, "(c)1994,95 Alex & Nick" );

	return( 1 );
}

int LIBFUNC __UserLibInit( void )
{
	if( PipelineBase = OpenLibrary( "zeus.library", 0 ) )
	{
		NewList( &AreaList );
		NewList( &IdxList );

		InitSemaphore( &AreaSem );

		return( 0 );
	}
	return( 1 );
}

void LIBFUNC __UserLibCleanup( void )
{
	if( Shared ) mbQuit();

	CloseLibrary( PipelineBase );
}

ULONG LIBFUNC mbCustom1( AREG(0) ULONG newfmt )
{
	ULONG		old = fmt;

	fmt = newfmt;

	return( old );
}

APTR LIBFUNC mbCustom2( AREG(0) APTR Summat )
{
	return( NULL );
}
