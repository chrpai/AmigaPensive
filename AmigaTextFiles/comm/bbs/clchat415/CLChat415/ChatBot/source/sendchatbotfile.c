#define __USE_SYSBASE
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dostags.h>
#include <bsdsocket.h>
#include <string.h>

#include "rev.h"

char version[] = { "$VER: SendChatBotFile " VERTAG };

__aligned struct FileInfoBlock fib;

//	sprintf() replacement
static UWORD fmtfunc[] = { 0x16c0, 0x4e75 };
void __stdargs sprintf( char *to, char *fmt, ... )
{
	RawDoFmt( fmt, &fmt + 1, (APTR)fmtfunc, to );
}

int __stdargs main( char *dummy )
{
	struct RDArgs *rda;
	struct args {
		char *botid;
		char *nick;
		char *file;
		char *dcccmd;
	} args;

	args.dcccmd = "DCC.SEND.AS225";
	rda = ReadArgs( "BOTID/A,NICK/A,FILENAME/A,DCCCMD", &args, NULL );
	if( rda )
	{
		struct ipcmsg {
			struct Message m;
			char data[ 256 ];
		} im;
		char servername[ 128 ];
		char dcccmd[ 256 ];
		struct MsgPort *sp;
		BPTR l;
		int port = 1200 + ( time( NULL ) % 2000 );
		
		l = Lock( args.file, SHARED_LOCK );
		if( !l )
		{
			Printf( "SendChatBotFile: file %s not found\n", args.file );
			FreeArgs( rda );
			return( 10 );
		}
		Examine( l, &fib );
		UnLock( l );

		sprintf( dcccmd, "RUN %s ORIGINATE NICK %s %s PORT %lu SIZE %lu",
			args.dcccmd,
			args.nick,
			args.file,
			port,
			fib.fib_Size
		);

		Printf( "starting \"%s\"\n", dcccmd );
		SystemTags( dcccmd,
			TAG_DONE 
		);

		strcpy( servername, "CHATBOT_" );
		strcat( servername, args.botid );
		strupr( servername );

		sprintf( im.data, "/QMSG %s \001DCC SEND %s %lu %lu %lu\001",
			args.nick,
			FilePart( args.file ),
			gethostid(),
			port,
			fib.fib_Size
		);

		if( sp = FindPort( servername ) )
		{
			im.m.mn_ReplyPort = CreateMsgPort();
			PutMsg( sp, &im );
			WaitPort( im.m.mn_ReplyPort );
			GetMsg( im.m.mn_ReplyPort );
			DeleteMsgPort( im.m.mn_ReplyPort );
		}
		else
			Printf( "SendChatBotFile: No Bot with ID \"%s\" found!\n", args.botid );
		FreeArgs( rda );
	}
	else
		PrintFault( IoErr(), "SendChatBot" );
	return( 0 );
}
