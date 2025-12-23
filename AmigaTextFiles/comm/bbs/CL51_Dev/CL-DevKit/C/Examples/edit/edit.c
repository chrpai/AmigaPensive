//
// Edit-Kommando, edit.c
//
// Copyright 1995 by Mathias Mischler
//

// Connectline Support Includes

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <dos/dostags.h>
#include <cl/clonline.h>
typedef APTR HH; /* Headerhandle */
#include <cl/cl_msg.h>
#include <cl/clb.h>
#include <cl/cl_userflags.h>

// Version String
char version[] = { "$VER: Edit 1.1 " __AMIGADATE__ };

BOOL IsBin ( APTR index )
{
long isbin;

	CLMSG_Get ( index, CLMSGA_FLG_BIN, &isbin, TAG_DONE );
	return (BOOL) isbin;
}

// Print Header

BOOL PrintHeader ( APTR index )
{
ulong number, readcount, bodylen;
//ulong flagbin, flaghold, flagdel, flagcensored, flagnoratio;
char *subject, *from, *filename, *shortcomment, *msgid;
time_t incomdate, senddate;
//char  flags [ 10 ];
char  incomdatestr [ 20 ];
char  senddatestr [ 20 ];
char  sizestr [ 40 ];
struct tm mytime;

	CLMSG_Get ( index, 
		CLMSGA_NUMBER,       &number, 
		CLMSGA_SUBJECT,      &subject,
		CLMSGA_FILENAME,     &filename,
		CLMSGA_SHORTCOMMENT, &shortcomment,
		CLMSGA_FROM,				 &from,
		CLMSGA_MSGID,        &msgid,
		CLMSGA_INCOMDATE,    &incomdate,
		CLMSGA_SENDDATE,     &senddate,
		CLMSGA_READCOUNT,    &readcount,
//	CLMSGA_FLG_BIN,      &flagbin,
//	CLMSGA_FLG_HOLD,     &flaghold,
//	CLMSGA_FLG_DEL,      &flagdel,
//	CLMSGA_FLG_CENSORED, &flagcensored,
//	CLMSGA_FLG_NO_RATIO, &flagnoratio,
		CLMSGA_BODY_LEN,		 &bodylen,
		TAG_DONE );
/*
	strcpy ( flags, "T...." );
	if ( flagbin      ) flags[0] = 'B';
	if ( flaghold     ) flags[1] = 'H';
	if ( flagdel      ) flags[2] = 'D';
	if ( flagcensored ) flags[3] = 'C';
	if ( flagnoratio  ) flags[4] = '*';
*/
	mytime = * gmtime ( &incomdate );
	CLONL_SPrintf ( incomdatestr, "%02ld.%02ld.%04ld %02ld:%02ld", mytime.tm_mday, mytime.tm_mon+1, mytime.tm_year + 1900, mytime.tm_hour, mytime.tm_min );
	mytime = * gmtime ( &senddate );
	CLONL_SPrintf ( senddatestr, "%02ld.%02ld.%04ld %02ld:%02ld", mytime.tm_mday, mytime.tm_mon+1, mytime.tm_year + 1900, mytime.tm_hour, mytime.tm_min );

	if ( bodylen > 99999 )
		if ( ((bodylen + 512) / 1024) > 9999 )
			CLONL_SPrintf ( sizestr, "%ldmB (%ld Bytes)", ( bodylen + (512 * 1024) ) / (1024*1024), bodylen );
		else
			CLONL_SPrintf ( sizestr, "%ldkB (%ld Bytes)", ( bodylen + 512 ) / 1024, bodylen );
	else
		CLONL_SPrintf ( sizestr, "%ld Bytes", bodylen );

	CLONL_Printf ( MSG("MSG_NUMMER"),    number ); 
	if ( ( !IsBin ( index ) ) || ( strcmp ( subject, filename ) != 0 ) )
		CLONL_Printf ( MSG("MSG_BETREFF"),   subject );
	if ( IsBin ( index ) && ( strlen (filename) > 0 ) )
		CLONL_Printf ( MSG("MSG_FILENAME"),  filename );
	CLONL_Printf ( MSG("MSG_ABSENDER"),  from );
	if ( strlen (shortcomment) > 0 )
		CLONL_Printf ( MSG("MSG_KOMMENTAR"), shortcomment );
//CLONL_Printf ( MSG("MSG_MSGID"),     msgid ); 
	CLONL_Printf ( MSG("MSG_GELESEN"),   readcount ); 
//CLONL_Printf ( MSG("MSG_FLAGS"),     flags ); 
	CLONL_Printf ( MSG("MSG_SENDDATE"),  senddatestr ); 
//CLONL_Printf ( MSG("MSG_INCOMDATE"), incomdatestr ); 
	CLONL_Printf ( MSG("MSG_LENGTH"),    sizestr ); 
	PutMSG ("MSG_TRENNUNG"); 
	return TRUE;
}

// Edit

void Edit ( APTR index )
{
char *shortcomment, *subject, *filename;
char puffer [ 1024 ];

	CLMSG_Get ( index, 
		CLMSGA_SUBJECT,      &subject,
		CLMSGA_FILENAME,     &filename,
		CLMSGA_SHORTCOMMENT, &shortcomment, 
		TAG_DONE );

/*
	PutMSG("MSG_EDITBETREFF"); CLONL_FlushOutput();
	strcpy ( puffer, subject ); puffer [ 59 ] = 0;
	CLONL_BlueRead ( puffer, puffer, 59, TYPE_ASCII, FALSE );
	CLMSG_Set ( index, CLMSGA_SUBJECT, puffer, TAG_DONE );

	PutMSG("MSG_EDITFILENAME"); CLONL_FlushOutput();
	strcpy ( puffer, filename ); puffer [ 59 ] = 0;
	CLONL_BlueRead ( puffer, puffer, 59, TYPE_ASCII, FALSE );
	CLMSG_Set ( index, CLMSGA_FILENAME, puffer, TAG_DONE );
*/
	PutMSG("MSG_EDITKOMMENTAR"); CLONL_FlushOutput();
	strcpy ( puffer, shortcomment ); puffer [ 59 ] = 0;
	CLONL_BlueRead ( puffer, puffer, 59, TYPE_ASCII, FALSE );
	CLMSG_Set ( index, CLMSGA_SHORTCOMMENT, puffer, TAG_DONE );
}

// PrintMsgRange

void PrintMsgRange ( APTR index, struct Range *myrange )
{
ulong count;

	while ( myrange )
	{
		for ( count = (myrange->from); count <= (myrange->to); count++ )
		{
			CLMSG_Move ( index, CLMSG_MOVE_GOTO, count );
			CLONL_DrawTitle();
			PrintHeader ( index );
			PutStr ("\n");
			Edit ( index );
			PutStr ("\n");
		}
		myrange = myrange -> next;
	}
}

//
// main
//

int main(int argc, char *argv[])
{
APTR index;
APTR board;
ulong count, numberall;
char eingabe [ 256 ];
struct Range *myrange;
char *argument;

	if ( !CLONL_Open ("Online_Edit.clcat") ) { PutStr("Can't init ANSI-System.\n"); return 1; }
	strcpy ( eingabe, "");

	CLONL_DrawTitle();

	board = CLB_FindBrett ( CLONL_GetUserShell()->currentboard );

	if ( ( board && (CLB_ValidateUser ( board, &CLONL_GetUserShell()->user ) & CLBVU_IS_ADMIN ) ) 
		|| ( strlen ( CLONL_GetUserShell()->currentboard ) == 0 ) 
		|| ( strcmp ( CLONL_GetUserShell()->currentboard, CLONL_GetUserShell()->user.Username) == 0 )
		|| ( CLONL_GetUserShell()->user.Priority >= 90 ) )
	{
		if ( board || ( strlen ( CLONL_GetUserShell()->currentboard ) == 0 ) || ( strcmp ( CLONL_GetUserShell()->currentboard, CLONL_GetUserShell()->user.Username) == 0 ) )
		{
			if ( !board )
				index = CLMSG_GetIndexTags ( (APTR) &CLONL_GetUserShell()->user,
				CLMFT_USER, TRUE, CLMFT_WRITE, TRUE, TAG_DONE );
			else
				index = CLMSG_GetIndexTags ( board, CLMFT_WRITE, TRUE, TAG_DONE );
			CLMSG_Get ( index, CLMSGA_NUMBEROFMSGS, &numberall, TAG_DONE );

			if ( argc > 1 )
			{
				argument = argv [ 1 ];

				if ( strcmp ( argument, "*" ) == 0 )
				{
					if ( numberall > 0 )
					{
						myrange = CLONL_ParseExpression ( "0-", numberall - 1 );
						PrintMsgRange ( index, myrange );
						CLONL_FreeRange ( myrange );
					}
				}	else
				{
					strcpy ( eingabe, "" );
					for ( count = 1; count < argc; count ++ )
					{
						strcat ( eingabe, argv [ count ] );
						strcat ( eingabe, " ");
					}
					if ( numberall > 0 )
					{
						myrange = CLONL_ParseExpression ( eingabe, numberall - 1 );
						PrintMsgRange ( index, myrange );
						CLONL_FreeRange ( myrange );
					}
				}
			}
			CLMSG_FreeIndex ( index );
		}
	}
	else
	{
		PutMSG ("MSG_NOADMIN");
	}

	CLONL_Close();
}
