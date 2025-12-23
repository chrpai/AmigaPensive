
/*
 * FAMEMsgScanModify v1.0
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * $VER: FAMEMsgScanModify.c v1.0
 *
 * Code by: David 'Strider' Wettig
 * E-Mail:  David.Wettig@fhtw-berlin.de
 * URL:     http://www.trsi.de/inno/strider/index.html
 *
 * Copyright note: FAME and all it's contents like programs,
 *                 tools, doors, documents, texts, sources,
 *                 standards like the FIM (FAME door port standard),
 *                 the FAMEToolPort and all depending things,
 *                 the FAMECoProc standard and (data) file formats
 *                 and this source code you're reading now
 *                 are copyrighted © 1993 - 1998 by David Wettig.
 *
 * Used tab size: 2
 */

/*
 * Includes:
 */

#include <stdio.h>
#include <strings.h>
#include <proto/exec.h>
#include <dos/stdio.h>
#include <FAME/FAME.h>

/*
 * Disable Lattice CTRL-C handling:
 */

#ifdef LATTICE
int CXBRK(void) {return(0);}
int chkabort(void) {return(0);}
#endif

/*
 * Define our version string for the "version" command:
 */

char *VerStr = { "$VER: FAMEMsgScanModify 1.0 "__AMIGADATE__ };

/*
 * Prototypes:
 */

void ShutDown( long );

/*
 * No CON window please, quit immediately.
 */

void __autoopenfail(void) { _XCEXIT(0);}

/*
 * Variables for ReadArgs and Template.
 */

char 			ArgStr[]					= "CONFNR/K/N,ALLCONFS/S,USERNR/K/N,ALLUSERS/S,NEWPOINTER/K/N";
long			ArgArray[]				= { 0L, 0L, 0L, 0L, 0L },
					ConfNr						= -1,
					UserNr						= -1,
					NewPointer				= 0;
char			Buffer[512];

/*
 * Global variables:
 */

struct FAMELibrary	*FAMEBase			= NULL;
struct FAMESystem		*FAMESystem		= NULL;
struct FAMEConf			*FAMEConf			= NULL;
struct FAMEUserConf	*FAMEUserConf	= NULL;
BPTR								FileHandle		= NULL;

/*
 * void main( void );
 *
 * main() entry point:
 */

void main( void )
{
	struct  RDArgs *rda	= NULL;

	Printf("\nFAMEMsgScanModify v1.0 "__AMIGADATE__" by David 'Strider' Wettig\n\nFAMEMsgScanModify allows you to modify FAME Message Base scan pointers.\n\n");

	/*
	 * Open FAME.library.
	 */

	if( ! ( FAMEBase = (struct FAMELibrary *)	OpenLibrary( FAMENAME, 6L )))
	{
		ShutDown( 1 );
	}

	/*
	 * Use ReadArgs() for arguments.
	 */

	if( rda = ReadArgs( ArgStr, ArgArray, NULL ))
	{
		/*
		 * Read the Conf number.
		 */

		if( ArgArray[ 0 ] )
		{
			ConfNr = *(long *)ArgArray[ 0 ];
		}

		/*
		 * All Conferences?
		 */

		if( ArgArray[ 1 ] )
		{
			ConfNr = 0;
		}

		/*
		 * Read the User number.
		 */

		if( ArgArray[ 2 ] )
		{
			UserNr = *(long *)ArgArray[ 2 ];
		}

		/*
		 * All Users?
		 */

		if( ArgArray[ 3 ] )
		{
			UserNr = 0;
		}

		/*
		 * Read the NewPointer
		 */

		if( ArgArray[ 4 ] )
		{
			NewPointer = *(long *)ArgArray[ 4 ];
		}

		FreeArgs( rda );
	}
	else
	{
		ShutDown( 2 );
	}

	if( ConfNr < 0 || UserNr < 0 || NewPointer < 0 )
	{
		ShutDown( 2 );
	}

	Printf( "FAMEMsgScanModify will do the following:\n\nConference(s) where MsgBase scan pointers will be modified: ");

	if( ConfNr )
	{
		Printf( "%ld", ConfNr );
	}
	else
	{
		Printf( "ALL Conferences" );
	}

	Printf( "\nFollowing User(s) will be modified: " );

	if( UserNr )
	{
		Printf( "%ld", ConfNr );
	}
	else
	{
		Printf( "ALL Users" );
	}

	Printf( "\nMsg scan pointer(s) will be set to: %ld\n\nMake sure your FAME BBS is not running now!\n\nDo you want to continue (y/N)? ", NewPointer );
	Flush( Output( ));
	Flush( Input( ));

	if( ! gets( Buffer ))
	{
		ShutDown( 3 );
	}

	if( Buffer[ 0 ] == 'y' || Buffer[ 0 ] == 'Y' )
	{
		Printf( "\n\nNow working...\n\n" );

		if( ! ( FAMESystem		= AllocVec( sizeof( struct FAMESystem ), MEMF_PUBLIC | MEMF_CLEAR )))
		{
			ShutDown( 5 );
		}

		if( ! ( FAMEConf			= AllocVec( sizeof( struct FAMEConf ), MEMF_PUBLIC | MEMF_CLEAR )))
		{
			ShutDown( 6 );
		}

		if( ! ( FAMEUserConf	= AllocVec( sizeof( struct FAMEUserConf ), MEMF_PUBLIC | MEMF_CLEAR )))
		{
			ShutDown( 7 );
		}

		if( FileHandle = Open( "FAME:Settings/System.dat", MODE_OLDFILE ))
		{
			if( DOSTRUE != Seek( FileHandle, 61, OFFSET_BEGINNING ))
			{
				if( sizeof( struct FAMESystem ) == Read( FileHandle, FAMESystem, sizeof( struct FAMESystem )))
				{
					long	StartPosition,
								EndPosition;

					Close( FileHandle );
					FileHandle = NULL;

					Printf( "System.dat read.\n" );

					if( ConfNr )
					{
						StartPosition	= ConfNr;
						EndPosition		= ConfNr;
					}
					else
					{
						StartPosition	= 1;
						EndPosition		= FAMESystem -> NumberOfConfs;
					}

					for( StartPosition ; StartPosition <= EndPosition ; StartPosition ++ )
					{
						FAMESPrintf( Buffer, "FAME:Settings/Conferences/Conf%ld.dat", StartPosition );

						Printf( "Working on Conf: %s\n", Buffer );

						if( FileHandle = Open( Buffer, MODE_OLDFILE ))
						{
							if( DOSTRUE != Seek( FileHandle, 61, OFFSET_BEGINNING ))
							{
								if( sizeof( struct FAMEConf ) == Read( FileHandle, FAMEConf, sizeof( struct FAMEConf )))
								{
									Close( FileHandle );
									FileHandle = NULL;

									strcpy( Buffer, FAMEConf -> ConfLocation );
									AddPart( Buffer, "UserCnf.data", 512 );

									if( FileHandle = Open( Buffer, MODE_OLDFILE ))
									{
										if( DOSTRUE != Seek( FileHandle, 61, OFFSET_BEGINNING ))
										{
											FAMESPrintf( Buffer, "User: %ld", UserNr );

											/*
											 * Skip to a specific User entry?
											 */

											if( UserNr )
											{
												if( DOSTRUE == Seek( FileHandle, sizeof( struct FAMEUserConf ) * ( UserNr - 1 ), OFFSET_CURRENT ))
												{
													ShutDown( 12 );
												}
											}

											Printf( "Working on User: " );

											/*
											 * Read User entries.
											 */

											while( sizeof( struct FAMEUserConf ) == Read( FileHandle, FAMEUserConf, sizeof( struct FAMEUserConf )))
											{
												Printf( "%ld ", FAMEUserConf -> UserNumber );

												/*
												 * That's all we have to change.
												 */

												FAMEUserConf -> MsgScanned = NewPointer;

												/*
												 * Seek back to write back.
												 */

												if( DOSTRUE == Seek( FileHandle, - sizeof( struct FAMEUserConf ), OFFSET_CURRENT ))
												{
													ShutDown( 12 );
												}

												/*
												 * Write User back.
												 */

												if( ENDSTREAMCH == Write( FileHandle, FAMEUserConf, sizeof( struct FAMEUserConf )))
												{
													ShutDown( 14 );
												}

												/*
												 * Only one User.
												 */

												if( UserNr )
												{
													break;
												}
											}

											Printf( "\n" );
										}
										else
										{
											ShutDown( 12 );
										}

										Close( FileHandle );
										FileHandle = NULL;
									}
									else
									{
										ShutDown( 11 );
									}
								}
								else
								{
									ShutDown( 13 );
								}
							}
							else
							{
								ShutDown( 12 );
							}
						}
						else
						{
							ShutDown( 11 );
						}

						Printf( "\n" );
					}
				}
				else
				{
					ShutDown( 10 );
				}
			}
			else
			{
				ShutDown( 9 );
			}
		}
		else
		{
			ShutDown( 8 );
		}
	}
	else
	{
		ShutDown( 4 );
	}

	/*
	 * Finito:
	 */

	ShutDown( 0 );

} // void main( void );

/*
 * void ShutDown( long ErrCode );
 *
 * Clean up code.
 */

void ShutDown( long ErrCode )
{
	long TheIoErr = 0L;

	if( FileHandle )
	{
		Close( FileHandle );
	}

	if( FAMEUserConf )
	{
		FreeVec( FAMEUserConf );
	}

	if( FAMEConf )
	{
		FreeVec( FAMEConf );
	}

	if( FAMESystem )
	{
		FreeVec( FAMESystem );
	}

	if( FAMEBase )
	{
		CloseLibrary( (struct Library *)FAMEBase );
	}

	switch( ErrCode )
	{
		case 1:
			Printf( "Can't open FAME.library 6!\n" );

			TheIoErr	= ERROR_INVALID_RESIDENT_LIBRARY ;
			ErrCode		= RETURN_FAIL;
		break;
		case 2:
			Printf( "Argument error!\n" );

			TheIoErr	= ERROR_REQUIRED_ARG_MISSING ;
			ErrCode		= RETURN_ERROR;
		break;
		case 3:
			Printf( "Nothing to read!\n" );

			TheIoErr	= ERROR_OBJECT_NOT_FOUND;
			ErrCode		= RETURN_WARN;
		break;
		case 4:
			Printf( "User abort!\n" );

			TheIoErr	= 0L;
			ErrCode		= RETURN_OK;
		break;
		case 5:
			Printf( "Error allocating FAMESystem structure!\n" );

			TheIoErr	= ERROR_NO_FREE_STORE;
			ErrCode		= RETURN_FAIL;
		break;
		case 6:
			Printf( "Error allocating FAMEConf structure!\n" );

			TheIoErr	= ERROR_NO_FREE_STORE;
			ErrCode		= RETURN_FAIL;
		break;
		case 7:
			Printf( "Error allocating FAMEUserConf structure!\n" );

			TheIoErr	= ERROR_NO_FREE_STORE;
			ErrCode		= RETURN_FAIL;
		break;
		case 8:
			Printf( "Error opening FAME:Settings/System.dat!\n" );

			TheIoErr	= ERROR_OBJECT_NOT_FOUND;
			ErrCode		= RETURN_ERROR;
		break;
		case 9:
			Printf( "Error seeking in System.dat!\n" );

			TheIoErr	= ERROR_SEEK_ERROR;
			ErrCode		= RETURN_ERROR;
		break;
		case 10:
			Printf( "Error reading FAMESystem structure!\n" );

			TheIoErr	= ERROR_OBJECT_WRONG_TYPE;
			ErrCode		= RETURN_ERROR;
		break;
		case 11:
			Printf( "Error opening %s!\n", Buffer );

			TheIoErr	= ERROR_OBJECT_NOT_FOUND;
			ErrCode		= RETURN_ERROR;
		break;
		case 12:
			Printf( "Error seeking in %s!\n", Buffer );

			TheIoErr	= ERROR_SEEK_ERROR;
			ErrCode		= RETURN_ERROR;
		break;
		case 13:
			Printf( "Error reading %s!\n", Buffer );

			TheIoErr	= ERROR_OBJECT_WRONG_TYPE;
			ErrCode		= RETURN_ERROR;
		break;
		case 14:
			Printf( "Error writing %s!\n", Buffer );

			TheIoErr	= ERROR_OBJECT_WRONG_TYPE;
			ErrCode		= RETURN_ERROR;
		break;
		default:
			Printf( "FAMEMsgScanModify ended!\n" );

			TheIoErr	= 0L;
			ErrCode		= RETURN_OK;
		break;
	}

	Printf("\n");

	SetIoErr( TheIoErr );

	PrintFault(IoErr(),"FAMEMsgScanModify");

	exit( ErrCode );

} // void ShutDown( long ErrCode );

