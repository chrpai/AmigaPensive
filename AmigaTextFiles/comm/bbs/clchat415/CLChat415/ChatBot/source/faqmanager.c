#include <proto/dos.h>
#include <stdio.h>
#include <string.h>

//
//	Usage: FAQ dir VIEW|ADD|DEL [topic] [addtext|delfaq nr]
//

#define INVCHAR ":/ \"\\"

#include "rev.h"
char version[] = { "$VER: CLChat-ChatBot-Faqmanager " VERTAG };

static int exists( char *path )
{
	BPTR l = Lock( path, SHARED_LOCK );
	if( l )
		UnLock( l );
	return( (int)l);
}

__aligned struct FileInfoBlock fib;

int main( int argc, char **argv )
{
	char faqdir[ 256 ];
	char buffer[ 256 ];
	int c, d, rc;
	BPTR l, f;

	if( argc < 3 )
		exit( 20 );

	strcpy( faqdir, argv[ 1 ] );

	if( !exists( faqdir ) )
	{
		Printf( "Error; FAQ directory \"%s\" doesn't exist!\n", faqdir );
		return( 20 );
	}

	if( !stricmp( argv[ 2 ], "ADD" ) )
	{
		if( argc != 5 )
		{
			PutStr( "USAGE: AddFAQ topic \"text\"\n" );
			exit( 5 );
		}

		if( strpbrk( argv[ 3 ], INVCHAR ) )
		{
			PutStr( "Invalid topic name!\n" );
			exit( 5 );
		}

		Printf( "Adding FAQ on topic %s...\n", argv[ 3 ] );

		AddPart( faqdir, argv[ 3 ], 256 );
		UnLock( CreateDir( faqdir ) );

		for( c = 0; c < 32; c++ )
		{
			sprintf( buffer, "%s/FAQ.%ld", faqdir, c );
			if( !exists( buffer ) )
			{
				BPTR f = Open( buffer, MODE_NEWFILE );
				if( f )
				{
					Write( f, argv[ 4 ], strlen( argv[ 4 ] ) );
					Close( f );
					Printf( "FAQ on topic %s saved.\n", argv[ 3 ] );
					exit( 0 );
				}
			}
		}
		Printf( "Sorry, can't save faq, maximum number reached.\n" );
	}
	else if( !stricmp( argv[ 2 ], "DEL" ) )
	{
		if( argc != 5 )
		{
			PutStr( "USAGE: DelFAQ topic number\n" );
			exit( 5 );
		}
		AddPart( faqdir, argv[ 3 ], 256 );
		UnLock( CreateDir( faqdir ) );

		c = atoi( argv[ 4 ] );
		sprintf( buffer, "%s/FAQ.%ld", faqdir, c - 1 );
		if( !exists( buffer ) )
		{
			Printf( "There is no FAQ #%ld on topic %s!\n", c, argv[ 3 ] );
			exit( 0 );
		}
		DeleteFile( buffer );
		Printf( "FAQ #%ld on topic %s deleted.\n", c, argv[ 3 ] );
		DeleteFile( faqdir );
	}
	else
	{
		if( argc < 4 )
		{
			BPTR l = Lock( faqdir, SHARED_LOCK );
			Printf( "I have FAQs on the following topics:\n" );
			c = 0;

			Examine( l, &fib );
			while( ExNext( l, &fib ) )
			{
				if( c++ )
					PutStr( ", " );
				Printf( "%s", fib.fib_FileName );
			}
			UnLock( l );
			Printf( "\n" );
			exit( 0 );
		}

		AddPart( faqdir, argv[ 3 ], 256 );
		l = Lock( faqdir, SHARED_LOCK );
		c = 0;
		if( l )
		{
			Examine( l, &fib );
			while( ExNext( l, &fib ) )
				c++;
		}

		if( !l || !c )
		{
			Printf( "Sorry, there is no FAQ available on topic \"%s\".\n", argv[ 3 ] );
			UnLock( l );
			exit( 5 );
		}

		Printf( "I have %ld FAQs on topic \"%s\":\n", c, argv[ 3 ] );

		for( d = 1, c = 0; c < 32; c++ )
		{
			sprintf( buffer, "%s/FAQ.%ld", faqdir, c );
			f = Open( buffer, MODE_OLDFILE );
			if( f )
			{
				rc = Read( f, buffer, 256 );
				buffer[ rc ] = 0;
				Printf( "(%ld) %s\n", d++, buffer );
				Close( f );
			}
		}
		UnLock( l );
	}
}
