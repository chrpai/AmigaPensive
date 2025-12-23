
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	<proto/exec.h>
#include	<proto/dos.h>
#include	<proto/pipeline.h>

#include	<exec/memory.h>

struct	Library		*PipelineBase;
struct	SharedData	*Shared;
UBYTE						 Prefix[100];

UBYTE						*version = "$VER: SendMsg 1.0 (25.07.95)";

BOOL SendMsg( long line, UBYTE *msg )
{
	BPTR		 file;
	UBYTE		*buff;
	UBYTE		*ptr, *bptr;
	UWORD		 len = strlen( msg );
	BOOL		 ret = TRUE;

	if( buff = AllocMem( len + 100, MEMF_ANY ) )
	{
		if( *Prefix )
		{
			strcpy( buff, Prefix );
			bptr  = buff;
			bptr += strlen( buff );
		}
		else bptr = buff;

		if( *msg == '@' )
		{
			if( file = Open( ++msg, MODE_OLDFILE ) )
			{
				while( ret && FGets( file, bptr, 99 ) )
				{
					ptr = &buff[ strlen( buff ) - 1 ];
				
					while( *ptr == 0x0a ) *ptr-- = 0x00;

					ret = SendLineMsg( line, buff, FALSE );
				}
				Close( file );
			}
		}
		else
		{
			strcpy( bptr, msg );
			ret = SendLineMsg( line, buff, FALSE );
		}
		FreeMem( buff, len + 100 );
	}

	return( ret );
}

void main( int argc, char **argv )
{
	long		i, j, line;

	PutStr( ZEUS_COPYRIGHT "\n" );
	Printf( "Zeus Line Notifier - Written by Alex May\n\n" );

	if( argc > 2 )
	{
		memset( Prefix, NULL, sizeof(Prefix) );
		
		i = 1;
		
		if( !stricmp( argv[1], "PREFIX" ) )
		{
			strncpy( Prefix, argv[2], 100 );
			i = 3;
		}

		if( PipelineBase = OpenLibrary( "zeus.library", 37 ) )
		{
			if( Shared = GetShared() )
			{
				for( ; i < argc ; i++ )
				{
					if( i % 2 )
					{
						if( *argv[i] == '*' ) line = -1;
						else						 line = atol( argv[i] );
					}
					else
					{
						if( line == -1 )
						{
							for( j = 0 ; j <= Shared->HiLine ; j++ )
							{
								if( SendMsg( j, argv[i] ) )
									Printf( "Sent message to line %ld\n", j );
								else
									Printf( "Couldn't send message to line %ld\n", j );
							}
						}
						else
						{
							if( GetLine( line ) )
							{
								if( SendMsg( line, argv[i] ) )
									Printf( "Sent message to line %ld\n", line );
								else
									Printf( "Couldn't send message to line %ld\n", line );
							}
							else
							{
								Printf( "Non-existant line number: %ld\n", line );
							}
						}
					}
				}
			}
			CloseLibrary( PipelineBase );
		}
	}
	else
	{
		PutStr( "Usage: SendMsg PREFIX,LINE/N/A,MESSAGE/A [LINE/N,MESSAGE] [...]\n\n" );

		PutStr( "   PREFIX  - A string to prefix all messages with\n"
				  "   LINE    - The number of the line to send the message to\n"
		        "   MESSAGE - A string to send to the user online\n\n"
		        "You can use '*' for a line number to send the same message to all lines.\n"
		        "If the first character of the message is '@' then the rest of the argument\n"
		        "   is a filename and will be sent line by line to the user\n\n" );
	}
}
