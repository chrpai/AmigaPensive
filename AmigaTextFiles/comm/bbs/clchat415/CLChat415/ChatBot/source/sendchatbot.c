#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>

#include "rev.h"

struct DosLibrary *DOSBase;

char version[] = { "$VER: SendChatBot " VERTAG };

void __saveds __entry( void )
{
	struct RDArgs *rda;
	struct args {
		char *botid;
		char *txt;
	} args;

	DOSBase = OldOpenLibrary( "dos.library" );

	rda = ReadArgs( "BOTID/A,TEXT/A/F", &args, NULL );
	if( rda )
	{
		struct ipcmsg {
			struct Message m;
			char data[ 256 ];
		} im;
		char servername[ 128 ];
		struct MsgPort *sp;

		strcpy( servername, "CHATBOT_" );
		strcat( servername, args.botid );
		strupr( servername );

		if( sp = FindPort( servername ) )
		{
			strcpy( im.data, args.txt );
			im.m.mn_ReplyPort = CreateMsgPort();
			PutMsg( sp, &im );
			WaitPort( im.m.mn_ReplyPort );
			GetMsg( im.m.mn_ReplyPort );
			DeleteMsgPort( im.m.mn_ReplyPort );
		}
		else
			Printf( "SendChatBot: No Bot with ID \"%s\" found!\n", args.botid );
		FreeArgs( rda );
	}
	else
		PrintFault( IoErr(), "SendChatBot" );
	CloseLibrary( DOSBase );
}
