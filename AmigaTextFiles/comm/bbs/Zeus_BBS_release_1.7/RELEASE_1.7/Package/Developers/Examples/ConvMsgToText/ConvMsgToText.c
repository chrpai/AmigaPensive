
#include "Header.h"

struct	Library			*PipelineBase;
struct	Library			*PipeUtilBase;

BPTR				 fH;

LONG				 args[NUMARGS];

void
Header ( void )
{
	PutStr ( "Zeus Bulletin Board System ©1992-1995 Alex May & Nick Loman\n" ) ;
	PutStr ( "Message Data Output Utility - Written by Nick Loman\n\n" ) ;
}

void
Usage ( void )
{
	PutStr ( "Usage: ConvMsgToText NUMBERS/A,AREA/N/A,FILENAME,NOHEAD/S,NOTEXT/S,NOKLUDGES/S\n\n" ) ;
	PutStr ( "   NUMBERS   - Message number(s) to output. Ranges allowed\n" ) ;
	PutStr ( "   AREA      - Area number messages are in\n" ) ;
	PutStr ( "   FILENAME  - File to output messages to (default 'Messages.Txt')\n" ) ;
	PutStr ( "   NOHEAD    - Do not output the message header\n" ) ;
	PutStr ( "   NOTEXT    - Do not output the message text\n" ) ;
	PutStr ( "   NOKLUDGES - Do not output FidoNet kludge lines\n\n" ) ;
}

void
main ( int argc, char **argv )
{
	struct	RDArgs		*Args;
	struct	parsekey	 Key;
	char			 FileName[256] = "Messages.Txt";

	memset ( args, '\0', sizeof ( LONG ) * NUMARGS ) ;
	Header ( ) ;

	if ( Args = ReadArgs ( "NUMBERS/A,AREA/N/A,FILENAME,NOHEAD/S,NOTEXT/S,NOKLUDGES/S", args, NULL ) )
	{
		if ( args[NOHEAD] && args[NOTEXT] )
		{
			Usage ( ) ;
			FreeArgs( Args ) ;
			exit ( 0 ) ;
		}

		if ( PipelineBase = OpenLibrary ( "zeus.library", 0 ) )
		{
			if ( PipeUtilBase = OpenLibrary ( "zmf.library", 0 ) )
			{
				if ( IsPlMsgThere ( ) )
				{
					if ( (char *) args [ FILENAME ] )
					{
						strcpy ( FileName, (char *) args [ FILENAME ] ) ;
					}
					if ( fH = Open ( FileName, MODE_READWRITE ) ) 
					{
						/* Introduce ourselves */
						Printf ( "\nOutputting to File \"%s\".\n", FileName ) ;
						Seek ( fH, 0, OFFSET_END ) ;

						/* Set off Parse Routines */
						InitParse ( &Key, (char *) args [ NUMBERS ] ) ;

						ParseNum ( &Key, RangeFunc, (ULONG) args [ AREA ] ) ;
						Close ( fH ) ;

						Printf ( "Finished.\n" ) ;
					}
				}
				else
					PutStr ("MsgControl is not running!\n");
			}
			else
				PutStr ( "Could not open zmf.library!\n" ) ;
		}
		else
			PutStr ( "Could not open zeus.library!\n" ) ;
		FreeArgs  ( Args ) ;
	}
	else
		Usage ( ) ;
}

int
__saveds
RangeFunc ( ULONG Num, ULONG *Area )
{
	struct	WholeMessage	*msg;
	ULONG			 flag = NULL;

	/* Load in Message */

	if ( args[NOKLUDGES] ) flag = LM_NOFIDOINFO;

	if ( msg = LoadMessage ( *Area, Num, flag ) ) 
	{
		/* Write out Message */
		WriteMessage ( msg ) ;
		/* Free Message again */
		FreeMessage ( msg ) ;
	}
	else
		Printf ( "\nCouldn't Read Message #%ld, from Area #%ld.\n", Num, *Area ) ;
	return ( TRUE ) ;
}

void
WriteMessage ( struct WholeMessage *msg )
{
	char			 Buffer [ 256 ] ;
	char			 DateStr [ 40 ] ;
	struct	msgarea		*area;
	struct	tm		*date;

	if ( ! args[NOHEAD] )
	{
		/* Write out Message Header! */

		date = localtime ( &msg->wm_Msg.Date ) ;

		/* Use standard date string */

		Write ( fH, (char *) "\n\n", 1 ) ;
		strftime ( DateStr, 40, "%d-%b-%y  %H:%M:%S", date ) ;
		sprintf ( Buffer, " From: %-40s Date: %s\n", msg->wm_Msg.From, DateStr ) ;
		Write ( fH, Buffer, strlen ( Buffer ) ) ;
		sprintf ( Buffer, "   To: %-40s Msg#: %ld\n", msg->wm_Msg.To, msg->wm_Msg.Number ) ;
		Write ( fH, Buffer, strlen ( Buffer ) ) ;
		sprintf ( Buffer, "Subj.: %s\n", msg->wm_Msg.Subject ) ;
		Write ( fH, Buffer, strlen ( Buffer ) ) ;
	
		/* Get msgarea details */
		if ( area = GetMsgAreaData ( msg->wm_Msg.MsgSection ) ) 
		{
			if ( area->basetype != BT_LOCAL )
			{
				sprintf ( Buffer, " Addr: %d:%d/%d.%d\n", msg->wm_Msg.FromAddress.Zone, msg->wm_Msg.FromAddress.Net, msg->wm_Msg.FromAddress.Node, msg->wm_Msg.FromAddress.Point ) ;
				if ( msg->wm_Msg.FromAddress.Domain[0] )
				{
					strcat ( Buffer, "@" ) ;
					strcat ( Buffer, msg->wm_Msg.FromAddress.Domain ) ;
				}
				Write ( fH, Buffer, strlen ( Buffer ) ) ;
			}
			if ( area->basetype == BT_NETMAIL )
			{
				sprintf ( Buffer, " Dest: %d:%d/%d.%d\n", msg->wm_Msg.DestAddress.Zone, msg->wm_Msg.DestAddress.Net, msg->wm_Msg.DestAddress.Node, msg->wm_Msg.DestAddress.Point ) ;
				if ( msg->wm_Msg.DestAddress.Domain[0] )
				{
					strcat ( Buffer, "@" ) ;
					strcat ( Buffer, msg->wm_Msg.DestAddress.Domain ) ;
				}
				Write ( fH, Buffer, strlen ( Buffer ) ) ;
			}
			sprintf ( Buffer, " Area: %ld - %s\n", area->number, area->name ) ;
			Write ( fH, Buffer, strlen ( Buffer ) ) ;
		}
	}
	if ( ! args[NOTEXT] && ! args[NOHEAD] )
	{
		WriteStr ( fH, 78, "-" ) ;
		Write ( fH, (char *) "\n\n", 2 ) ;
	}

	if ( ! args[NOTEXT] )
		WriteText ( msg->wm_Text ) ;

	if ( ! args[NOTEXT] && args[NOHEAD] )
	{
		Write ( fH, (char *) "\n\n", 2 ) ;
	}
}

void
WriteStr ( BPTR FileHandle, ULONG Number, char *Str )
{
	int i;

	for ( i = 0 ; i <= Number ; i++ )
	{
		Write ( FileHandle, Str, 1 ) ;
	}
}

void
WriteText ( char *Buffer )
{
	char *cp = Buffer;

	/* Format the text, quickly, replacing ^A kludges, and converting
	   '\r's to '\n's */

	while ( *cp != '\0' )
	{
		if ( *cp == 0x0D )
			*cp = 0x0A;
		if ( *cp == 0x01 )
			*cp = '^';
		cp++;
	}
	/* Line wrap the text to 80 characters */
	LineWrap ( Buffer, 80 ) ;
	/* Dump to disk */
	Write ( fH, Buffer, strlen ( Buffer ) ) ;
}