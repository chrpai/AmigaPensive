//
// Inhalt-Kommando, inhalt.c
//
// Copyright © 1995-1996 by Mathias Mischler
//
// + Realname NUR anzeigen, wenn vorhanden.
// + Neue Nachrichten bold.
// + BUG: Wenn KEIN Inhalt, checken. (9.7.95)
// + Graphicmodus
// + Merken!! ,) (12.7.95)
// + BUG: mytime (struct tm) ist POINTER! (13.7.95)
// + Brett mit nur gelöschten Nachrichten liefert bei i# bugs. (14.7.95)
// + Bei leeren Bretter Dummy erzeugen ?! (kein Dummy, NULL wird verarbeitet). (14.7.95)
// + Cursor bei I # direkt auf Neue. (14.7.95)
// + Inhalt rückwärts (neue zuerst). (17.7.95)
// + Inhalt.txt ausgeben. (17.7.95)
// + Brettverwalter/Zensor ausgeben. (18.7.95)
// + Inhalt **, I ***, etc. wird als I * behandelt (14.9.95)
// + bei "I" MOVE_NEW von CLB_MOVE verwenden... sollte schneller gehen.(12.11.95)
// + Inhalt.txt auch bei "i #" ausgeben. (18.11.95)
// - Inhalt # <Range>.
//

// Connectline Support Includes

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dos/dostags.h>
#include <cl/clutil.h>
#include <cl/clonline.h>
#include <cl/cl_msg.h>
#include <cl/clb.h>
#include <cl/cl_userflags.h>

// Version String
char version[] = { "$VER: Inhalt 2.4 " __AMIGADATE__ };

//
// Inhalt.txt - Filename für Infos zum Inhalt
//

#define InhaltFilename "Inhalt.txt"

static char *GetInhaltForm ( APTR index, char *deststr, BOOL onlynew );

//
// Interaktive Graphische Brettauswahl, ShowGfx()
//

static long ShowGfx ( APTR *index, ulong numberall, BOOL backwards )
{
char   buffer [ 512 ], *ausgabe, titel [ 256 ];
time_t writetime, lastcall;
struct MinList indexlist;
struct EntryNode *en, *en2;
long count;
BOOL quit;
long ret_val;
ulong pos;
char param [ 2048 ], one [ 10 ];
BOOL found;
BOOL suchneue;

	// Existiert ein Index
	if ( !index ) return 1;

	// Init
	ret_val = 0;
	CLONL_DrawTitle();
	PutMSG ("MSG_ACTUAL");

	// Datum für neue Nachrichten	
	lastcall = CLONL_GetUserShell() -> userprofile.LastLogin;
	NEWLIST ( &indexlist );

	found = FALSE;
	pos = 0; 
	suchneue = TRUE;

	if ( backwards )
	{
		pos = 0;
		for ( count = numberall - 1; count >= 0; count-- )
		{
			CLMSG_Move ( *index, CLMSG_MOVE_GOTO, count );
			ausgabe = GetInhaltForm ( *index, buffer, FALSE );

			if ( ausgabe )
			{
				if ( !( en = malloc ( sizeof ( struct EntryNode ) ) ) ) { PutStr("Can't get memory.\n"); break; }
				strcpy ( en->data, ausgabe );
				CLMSG_Get ( *index, CLMSGA_INCOMDATE, &writetime, TAG_DONE );
				en->bold = ( lastcall <= writetime );
				en->inverse = 0;
				ADDTAIL ( &indexlist, en );
				found = TRUE;
			}
		}
	}
	else
	{
		for ( count = 0; count < numberall; count++ )
		{
			CLMSG_Move ( *index, CLMSG_MOVE_GOTO, count );
			ausgabe = GetInhaltForm ( *index, buffer, FALSE );

			if ( ausgabe )
			{
				if ( !( en = malloc ( sizeof ( struct EntryNode ) ) ) ) { PutStr("Can't get memory.\n"); break; }
				strcpy ( en->data, ausgabe );
				CLMSG_Get ( *index, CLMSGA_INCOMDATE, &writetime, TAG_DONE );
				en->bold = ( lastcall <= writetime );
				if ( suchneue && ( lastcall <= writetime ) ) 
				{
					pos = count; // Auf neue setzen;
					suchneue = FALSE;
				}
				en->inverse = 0;
				ADDTAIL ( &indexlist, en );
				found = TRUE;
			}
		}
	}

	if ( *index ) 
	{
		CLMSG_FreeIndex ( *index );
		*index = NULL; // Freigeben !!
	}

//if ( found )
//{
		quit = FALSE;

		while (!quit)
		{
			strcpy ( titel, MSG("MSG_INHALTAUSWAHL") );
			if ( !found )
			{
				strcat( titel, "¤" );
				strcat( titel, MSG("MSG_NOMSGSTATUS") );
			}
			en = CLONL_RequesterSelect ( &indexlist, pos, titel, MODE_INHALT );

			if ( en == NULL )
			{
				ret_val = 0;
				quit = TRUE;
			} else
			if ( en == (struct EntryNode *) -1 )
			{
				ret_val = 1;
				quit = TRUE;
			} else
			if ( en )
			{
				// ALTE position bestimmen
				pos = 0;
				en2 = FIRSTNODE ( &indexlist );
				while ( NEXTNODE ( en2 ) )
				{
					if ( en2 == en ) break;
					en2 = NEXTNODE ( en2 ); pos++;
				}

				// Parameterstring basteln
				strcpy ( param, "" );

				en2 = FIRSTNODE ( &indexlist );
				while ( NEXTNODE ( en2 ) )
				{
					if ( en2->inverse )
					{
						one [ 0 ] = en2->data [ 0 ];
						one [ 1 ] = en2->data [ 1 ];
						one [ 2 ] = en2->data [ 2 ];
						one [ 3 ] = en2->data [ 3 ];
						one [ 4 ] = en2->data [ 4 ];
						one [ 5 ] = 0;
						strcat ( param, one );
						strcat ( param, " " );
					}
					en2 = NEXTNODE ( en2 );
				}
				if ( strlen ( param ) > 0 )
				{
					param [ strlen ( param ) - 1 ] = 0; // Komma hinten weg.
				}
				else
				{
					one [ 0 ] = en->data [ 0 ];
					one [ 1 ] = en->data [ 1 ];
					one [ 2 ] = en->data [ 2 ];
					one [ 3 ] = en->data [ 3 ];
					one [ 4 ] = en->data [ 4 ];
					one [ 5 ] = 0;
					strcpy ( param, one );
				}

//			Printf( "Param: %s.\n", param );

				switch ( titel [ 0 ] )
				{
					case 'L':
						CLONL_DoCommand ( "LESEN", param );
						// Einträge löschen
						en2 = FIRSTNODE ( &indexlist );
						while ( NEXTNODE ( en2 ) )
						{
							en2->inverse = 0;
							en2 = NEXTNODE ( en2 );
						}
						break;
					case 'S':
						CLONL_DoCommand ( "SENDEN", "" );
						break;
					case 'M':
						CLONL_DoCommand ( "MERKEN", param );
						// Einträge löschen
						en2 = FIRSTNODE ( &indexlist );
						while ( NEXTNODE ( en2 ) )
						{
							en2->inverse = 0;
							en2 = NEXTNODE ( en2 );
						}
						break;
					case 'R':
						CLONL_DoCommand ( "LOESCHEN", param );
						ret_val = 2; // Again !!
						quit = TRUE;
						// Einträge löschen
						en2 = FIRSTNODE ( &indexlist );
						while ( NEXTNODE ( en2 ) )
						{
							en2->inverse = 0;
							en2 = NEXTNODE ( en2 );
						}
						break;
				}
			}
		}

//		while ( en = REMHEAD ( &indexlist ) )
//			free ( en );

//}

	CLONL_GotoXY ( 1, CLONL_GetUserShell()->userprofile.ShowLines + 1);

	return ret_val;
}

static char *GetInhaltForm ( APTR index, char *deststr, BOOL onlynew )
{
ulong number, readcount, bodylen;
ulong flagbin, flaghold, flagdel, flagcensored, flagnoratio;
char *subject, *from;
time_t incomdate;
char  flags [ 6 ];
char  date  [ 6 ];
char  absender [ 130 ], *absp, *abs2p;
char  sizestr [ 10 ];
time_t lastcall;
struct tm *mytime;

	lastcall = CLONL_GetUserShell()->userprofile.LastLogin;

	CLMSG_Get ( index, 
		CLMSGA_NUMBER,       &number, 
		CLMSGA_SUBJECT,      &subject,
		CLMSGA_FROM,				 &from,
		CLMSGA_INCOMDATE,    &incomdate,
		CLMSGA_READCOUNT,    &readcount,
		CLMSGA_FLG_BIN,      &flagbin,
		CLMSGA_FLG_HOLD,     &flaghold,
		CLMSGA_FLG_DEL,      &flagdel,
		CLMSGA_FLG_CENSORED, &flagcensored,
		CLMSGA_FLG_NO_RATIO, &flagnoratio,
		CLMSGA_BODY_LEN,		 &bodylen,
		TAG_DONE );

	if ( bodylen > 99999 )
	{
		if ( ((bodylen + 512) / 1024) > 9999 )
		{	
			CLONL_SPrintf ( sizestr, "%ldm", ( bodylen + (512 * 1024) ) / (1024*1024) );
		}
		else
		{	
			CLONL_SPrintf ( sizestr, "%ldk", ( bodylen + 512 ) / 1024 );
		}
	}
	else
	{
		CLONL_SPrintf ( sizestr, "%ld", bodylen );
	}

	strcpy ( absender, from ); // Realname extrahieren, wenn vorhanden
	absp = absender;
	while ( *absp && ( *absp != '(' ) ) absp++;
	if ( *absp )
	{
		absp++;
		abs2p = absp;
		while ( *abs2p && ( *abs2p != ')' ) ) abs2p++;
		*abs2p = 0;
	}
	else
	{
		absp = absender;
	}

	mytime = gmtime ( &incomdate );
	CLONL_SPrintf ( date, "%02ld.%02ld", mytime->tm_mday, mytime->tm_mon + 1);

	strcpy ( flags, "T...." );
	if ( flagbin      ) flags[0] = 'B';
	if ( flaghold     ) flags[1] = 'H';
	if ( flagdel      ) flags[2] = 'D';
	if ( flagcensored ) flags[3] = 'C';
	if ( flagnoratio  ) flags[4] = '*';

	if ( ((!flagdel) || (CLONL_GetUserShell()->userprofile.InhaltFlags & USERI_SHOWDELETE))
 		&& ((!onlynew) || (lastcall <= incomdate)) )
	{
		CLONL_SPrintf ( deststr, MSG ("MSG_INHALTFORMAT"), number, date, flags, sizestr, absp, subject );
	}
	else
	{
		return NULL;
	}
	return deststr;
}

static long PrintMsgActual ( APTR index, BOOL onlynew )
{
char  commentstr [ 1025 ], *commentp, *commentnextp;
long  commentlen;
time_t lastcall;
time_t incomdate;
char *comment, *ausgabe;

	if ( !index ) return 1;

	lastcall = CLONL_GetUserShell()->userprofile.LastLogin;

	ausgabe = GetInhaltForm ( index, commentstr, onlynew ); // commenstr wird ledigl. als puffer verwendet!

	if ( ausgabe )
	{
		CLMSG_Get ( index, 
			CLMSGA_SHORTCOMMENT, &comment,
			CLMSGA_INCOMDATE,    &incomdate,
			TAG_DONE );

		if ( lastcall <= incomdate )
		{
			CLONL_Color ( COLOR_Yellow, COLOR_Black, STYLE_Bold );
		}
		else
		{
			CLONL_Color ( COLOR_Yellow, COLOR_Black, 0 );
		}

		PutStr ( ausgabe );

		if ( comment && comment [ 0 ] )
		{
			commentlen = atoi ( MSG ("MSG_COMMENTSIZE") );
			strcpy ( commentstr, comment );
			commentstr [ strlen ( commentstr ) + 1 ] = 0; // Ende mit 00 markieren
			commentp = commentstr;
			while ( *commentp )
			{
				if ( strlen ( commentp ) < commentlen )
					commentnextp = commentp + strlen ( commentp );
				else
				{
					commentnextp = commentp + commentlen - 1;
					while ( (commentnextp > commentp) && (*commentnextp != ' ')) commentnextp--;
					if ( commentnextp == commentp )
					{
						commentnextp = commentp + commentlen;
					}
					else
					{
						*commentnextp = 0; 
						commentnextp ++;
					}
				}

				CLONL_Printf ( MSG ("MSG_COMMENT"), commentp );
				commentp = commentnextp;
			}
		}
	}

	if ( CheckSignal( SIGBREAKF_CTRL_C) ) 
		return 20;
	return 0;
}

static long PrintMsgRange ( APTR index, struct Range *myrange, BOOL onlynew )
{
ulong count;
long reterror;
long min = -1;

	if ( !index ) return 1;

	if ( onlynew )
	{
		if ( !CLMSG_Move( index, CLMSG_MOVE_NEWER, CLONL_GetUserShell()->userprofile.LastLogin ) )
			CLMSG_Get( index, CLMSGA_NUMBER, &min, TAG_DONE );
		if ( min < 0 )
			return 0; // No newer mail entrys
	}

	reterror = 0;

	while ( myrange )
	{
		for ( count = (myrange->from); count <= (myrange->to); count++ )
		{
			if( onlynew && count < min ) continue; 
			CLMSG_Move ( index, CLMSG_MOVE_GOTO, count );
			reterror |= PrintMsgActual ( index, onlynew );
			if ( reterror ) return reterror;
		}
		myrange = myrange -> next;
	}
	return reterror;
}

static long PrintMsgRangeBack ( APTR index, struct Range *myrange, BOOL onlynew )
{
long count;
long reterror;
long min = -1;

	if ( !index ) return 1;

	if ( onlynew )
	{
		if ( !CLMSG_Move( index, CLMSG_MOVE_NEWER, CLONL_GetUserShell()->userprofile.LastLogin ) )
			CLMSG_Get( index, CLMSGA_NUMBER, &min, TAG_DONE );
		if ( min < 0 )
			return 0; // No newer mail entrys
	}

	reterror = 0;

	if ( myrange && myrange->next ) 
	{
		if ( reterror |= PrintMsgRangeBack ( index, myrange->next, onlynew ) )
			return reterror;
	}

	if ( myrange )
		for ( count = (myrange->to); count >= (long)(myrange->from); count-- )
		{
			if( onlynew && count < min ) continue; 
			CLMSG_Move ( index, CLMSG_MOVE_GOTO, count );
			reterror |= PrintMsgActual ( index, onlynew );
			if ( reterror ) return reterror;
		}

	return reterror;
}

//
// PrintInfo
// Inhalt.txt im normalen Modus ausgeben
//

static BOOL PrintInfo ( APTR board )
{
char *path;
char mypath [ 256 ];
char *censor, *admin;
long dlratio;
BOOL leerzeile;

	if ( board )
	{
		CLB_Get ( board, 
							CLBA_PATH, &path, 
							CLBA_ADMIN, &admin, 
							CLBA_CENSOR, &censor, 
							CLBA_DLRATIO, &dlratio, 
							TAG_DONE );

		leerzeile = FALSE;
		if ( admin && admin [ 0 ] )
		{
			CLONL_Printf ( MSG("MSG_BOARDADMIN"), admin );
			leerzeile = TRUE;
		}
		if ( censor && censor [ 0 ] )
		{
			CLONL_Printf ( MSG("MSG_BOARDCENSOR"), censor );
			leerzeile = TRUE;
		}
		if ( !dlratio )
		{
			PutMSG ("MSG_BOARDRATIO");
			leerzeile = TRUE;
		}
		if ( leerzeile ) 
		{
			PutStr ("\n");
		}
		strncpy ( mypath, path, 256 );
		AddPart ( mypath, InhaltFilename, 256 );
	}
	else
	{
		strncpy ( mypath, CLONL_GetUserShell()->user.Pfad, 256 );
		AddPart ( mypath, InhaltFilename, 256 );
	}
	CLONL_PrintFile( mypath );
	return TRUE;
}

//
// Inhalt.txt für interaktiven Modus ausgeben
//

static BOOL PrintInfoGfx ( APTR board )
{
char *path;
char mypath [ 256 ];

	if ( board )
	{
		CLB_Get ( board, CLBA_PATH, &path, TAG_DONE );
		strncpy ( mypath, path, 255 );
		AddPart ( mypath, InhaltFilename, 255 );
	}
	else
	{
		strncpy ( mypath, CLONL_GetUserShell()->user.Pfad, 255 );
		AddPart ( mypath, InhaltFilename, 255 );
	}
	if( CLU_Exists( mypath ) )
	{
		CLONL_DrawTitle();
		CLONL_PrintFile( mypath );	
		CLONL_Wait();
	}
	return TRUE;
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
int ret_val;
bool again;

	if ( !CLONL_Open ("Online_Inhalt.clcat") ) { PutStr("Can't init ANSI-System.\n"); return 1; }

	do
	{

	again = FALSE;

	strcpy ( eingabe, "");
	ret_val = 0;

	// Test auf PM (siehe CLMFT_USER)
	board = CLB_FindBrett ( CLONL_GetUserShell()->currentboard );

	if ( board || !CLONL_GetUserShell()->currentboard [ 0 ] || ( stricmp ( CLONL_GetUserShell()->currentboard, CLONL_GetUserShell()->user.Username) == 0 ) )
	{
		if ( !board )
			index = CLMSG_GetIndexTags ( (APTR) &CLONL_GetUserShell()->user,
				CLMFT_USER, TRUE, TAG_DONE );
		else
			index = CLMSG_GetIndexTags ( board, TAG_DONE );

		if ( !index ) goto ende;

		CLMSG_Get ( index, CLMSGA_NUMBEROFMSGS, &numberall, TAG_DONE );

		if ( argc > 1 )
		{
			argument = argv [ 1 ];

			if ( !CLONL_IsTTY() && strcmp ( argument, "#" ) == 0 )
			{
				PrintInfoGfx ( board );
				ret_val = ShowGfx ( &index, numberall, (BOOL) CLONL_GetUserShell()->userprofile.InhaltFlags & USERI_BACKWARDS );
				if ( ret_val == 2 ) again = TRUE;
			} else
			if ( argument [ 0 ] == '*' )
			{
				CLONL_DrawTitle();
				PrintInfo ( board );
				PutMSG ( "MSG_INHALTWHATIS" );
				if ( numberall > 0 )
				{
					myrange = CLONL_ParseExpression ( "0-", numberall - 1 );
					if ( CLONL_GetUserShell()->userprofile.InhaltFlags & USERI_BACKWARDS )
						PrintMsgRangeBack ( index, myrange, FALSE );
					else
						PrintMsgRange ( index, myrange, FALSE );
					CLONL_FreeRange ( myrange );
				}
				else
				{	
					PutMSG ("MSG_NOMSG");
				}
			}	else
			if ( (strcmp ( argument, "n" ) == 0) || (strcmp ( argument, "N" ) == 0) )
			{
				PrintInfo ( board );
				PutMSG ( "MSG_INHALTWHATIS" );
				if ( numberall > 0 )
				{
					myrange = CLONL_ParseExpression ( "0-", numberall - 1 );
					if ( CLONL_GetUserShell()->userprofile.InhaltFlags & USERI_BACKWARDS )
						PrintMsgRangeBack ( index, myrange, TRUE );
					else
						PrintMsgRange ( index, myrange, TRUE );
					CLONL_FreeRange ( myrange );
				}
				else
				{
					PutMSG ("MSG_NOMSG");
				}
			}	else
			{
				CLONL_DrawTitle();
				PrintInfo ( board );
				PutMSG ( "MSG_INHALTWHATIS" );
				strcpy ( eingabe, "" );
				for ( count = 1; count < argc; count ++ )
				{
					strcat ( eingabe, argv [ count ] );
					strcat ( eingabe, " ");
				}
				if ( numberall > 0 )
				{
					myrange = CLONL_ParseExpression ( eingabe, numberall - 1 );
					if ( CLONL_GetUserShell()->userprofile.InhaltFlags & USERI_BACKWARDS )
						PrintMsgRangeBack ( index, myrange, FALSE );
					else
						PrintMsgRange ( index, myrange, FALSE );
					CLONL_FreeRange ( myrange );
				}
				else
				{
					PutMSG ("MSG_NOMSG");
				}
			}
		}
		else
		{
			CLONL_DrawTitle();
			PrintInfo ( board );
			PutMSG ( "MSG_INHALTWHATIS" );
			if ( numberall > 0 )
			{
				myrange = CLONL_ParseExpression ( "0-", numberall - 1 );
				if ( CLONL_GetUserShell()->userprofile.InhaltFlags & USERI_BACKWARDS )
					PrintMsgRangeBack ( index, myrange, TRUE );
				else
					PrintMsgRange ( index, myrange, TRUE );
				CLONL_FreeRange ( myrange );
			}
			else
			{
				PutMSG ("MSG_NOMSG");
			}
		}
		if ( index ) CLMSG_FreeIndex ( index );
	}
	else
	{
		PutMSG("MSG_NOBOARD");
	}

	}
	while ( again );

	ende:

	CLONL_Close();
	return ret_val;
}
