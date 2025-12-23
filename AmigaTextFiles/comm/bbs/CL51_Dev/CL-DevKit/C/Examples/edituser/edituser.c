//
// Edituser-Kommando für Connectline, edituser.c
//
// Copyright 1995 by Mathias Mischler
//
// + Flags!!!
// + MaxKB/MaxDays/Passwort
// + Gruppen
// + Neue PW Routinen ( 4.8.95 )
//

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cl/clonline.h>
#include <cl/cl_prefs.h>
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#include <cl/cl_misc.h>

// Version String

char version[] = { "$VER: Edituser 1.5 " __AMIGADATE__ };

struct UserProfile userprofile;
struct User user;

char userstr [ 32 ];

//
// ask_what()
//

void ask_what ( void )
{
	PutMSG ("MSG_WHICH");
	CLONL_BlueRead ( userstr, "", 30, TYPE_ASCII, FALSE );
}

//
// user_edit()
//

void user_edit ( void )
{
struct Shortmenu *mymenu;
Mask *mask = NULL;
char titel [ 72 ];
char invisible [ 3 ], network [ 3 ], dontsave [ 3 ], autoentry [ 3 ], autoremove [ 3 ];
char priority [ 4 ], timeout [ 6 ], konto [ 12 ], credit [ 12 ], showlines [ 4 ], freefiles [ 6 ], rate [ 4 ], freekbyte[ 12 ];
char numberofcalls[ 7 ];
long Flags;
char password [ 32 ];
char crunchday [ 6 ], crunchkb [ 6 ];
char dummy;

	while ( 1 )
	{
		CLONL_DrawTitle();

		CLONL_Printf ( MSG("MSG_USERSTR"), user.Username );

		mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_DATEN"),         'D' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_EINSTELLUNGEN"), 'E' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_GRUPPEN"),       'G' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_SAVE"),          'S' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"),          'Q' );
		dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );

		if ( dummy == 0 || dummy == 'Q' ) return;
		if ( dummy == 'S' ) { CLP_SetUser ( &user, &userprofile ); return; }

		if ( dummy == 'G' ) 
		{
		ulong x,y, group, count;
		char gruppen [ 80 ];
		struct Range *myrange, *temprange;

			CLONL_DrawTitle();

			while ( 1 )
			{
				for ( y = 0; y < 16; y++ )
				{
					for ( x = 0; x < 6; x++ )
					{
						group = 1 + y * 6 + x;
						CLONL_GotoXY ( x * 13, 3 + y );
						Printf ( "[0m[36;1m%2ld:[0m[35m", group ); 
						if ( CLP_GetGroup ( &user.Groups, group ) ) PutStr ("[7m");
						PutStr ( CLP_GetGroupName ( group ) );
					}
				}
				PutStr ("\n\n");
				mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_MARKIEREN"), 'M' );
				mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_DEL"),       'L' );
				mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"),      'Q' );
				dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );
				if ( dummy == 0 ) return;
				if ( dummy == 'Q') break;
				PutStr ("[A"); CLONL_DeleteEOL ();
				PutMSG ("MSG_WELCHE");
				CLONL_BlueRead ( gruppen, "", 40, TYPE_ASCII, FALSE );
				PutStr ("[A"); CLONL_DeleteEOL ();
				if ( myrange = CLONL_ParseExpression ( gruppen, 96 ) )
				{
					temprange = myrange;
					while ( temprange )
					{
						for ( count = ( temprange->from ); count <= ( temprange->to ); count++ )
						{
							if ( dummy == 'M' ) CLP_SetGroup ( &user.Groups, count );
							if ( dummy == 'L' ) CLP_ClrGroup ( &user.Groups, count );
						}
						temprange = temprange->next;
					}
					CLONL_FreeRange ( myrange );
				}
				else
				{
					continue;
				}
			}
	  	continue; 
		}

		strcpy ( password, "???" );

		CLONL_SPrintf ( titel, MSG ("MSG_TITEL"), user.Username );

		CLONL_SPrintf ( priority,  "%ld", user.Priority );
		CLONL_SPrintf ( timeout,   "%ld", userprofile.Timeout );
		CLONL_SPrintf ( konto,     "%ld", userprofile.AccountTalers );
		CLONL_SPrintf ( credit,    "%ld", userprofile.CreditTalers );
		CLONL_SPrintf ( showlines, "%ld", userprofile.ShowLines );
		CLONL_SPrintf ( freefiles, "%ld", userprofile.FreeFiles );
		CLONL_SPrintf ( numberofcalls, "%ld", userprofile.NumberOfCalls );
		CLONL_SPrintf ( freekbyte, "%ld", userprofile.FreeKByte );
		CLONL_SPrintf ( rate,      "%ld", userprofile.Rate );
		CLONL_SPrintf ( crunchday, "%ld", userprofile.MaxDays );
		CLONL_SPrintf ( crunchkb,  "%ld", userprofile.MaxKB );

		if ( userprofile.Flags & USER_INVISIBLE) 			strcpy ( invisible,  "J" ); else strcpy ( invisible,  "N" );
		if ( userprofile.Flags & USER_NETWORKENABLED) strcpy ( network,    "J" ); else strcpy ( network,    "N" );
		if ( userprofile.Flags & USER_DONTSAVE) 			strcpy ( dontsave,   "J" ); else strcpy ( dontsave,   "N" );
		if ( userprofile.Flags & USER_AUTO) 					strcpy ( autoentry,  "J" ); else strcpy ( autoentry,  "N" );
		if ( userprofile.Flags & USER_AUTOREMOVE) 		strcpy ( autoremove, "J" ); else strcpy ( autoremove, "N" );

		mask = NULL;

		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_REALNAME"),   userprofile.RealName, 40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_ADDRESSE"),   userprofile.RealAdr, 40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_TELEFON"),    userprofile.RealTel, 40 ); 
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_REALINFO"),   userprofile.RealInfo, 40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_USERINFO"),   userprofile.InfoMessage, 40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_LEVEL"),      priority, 2 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PASSWORT"),   password, 30 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_TERMINAL"),   userprofile.Terminal, 10 ); 
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_LANGUAGE"),   userprofile.Language, 10 ); 
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_KEYBOARD"),   userprofile.Keyboard, 10 ); 
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PROTOKOLL"),  userprofile.StdProto, 40 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_CRUNCHDAYS"), crunchday, 4 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_CRUNCHKB"),   crunchkb, 4 ); 
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_TIMEOUT"),    timeout, 4 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_KONTO"),      konto, 10 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_CREDIT"),     credit, 10 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_LINES"),      showlines, 2 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_FREEFILES"),  freefiles, 4 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_NUMBEROFCALLS"),  numberofcalls, 5 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_FREEKBYTE"),  freekbyte, 10 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_RATIO"),      rate, 2 ); 
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_INVISIBLE"),  invisible, 1 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_NETWORK"),    network, 1 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_DONTSAVE"),   dontsave, 1 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_AUTO"),       autoentry, 1 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_AUTOREMOVE"), autoremove, 1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_VERTRETER"),  userprofile.CopyTo, 60 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PFAD"),       user.Pfad, 70 );

		if ( CLONL_InputMask ( mask, titel ) )
		{
			user.Priority             = atoi ( priority );
			userprofile.Timeout       = atoi ( timeout );
			userprofile.AccountTalers = atoi ( konto );
			userprofile.CreditTalers  = atoi ( credit );
			userprofile.ShowLines     = atoi ( showlines );
			userprofile.FreeFiles     = atoi ( freefiles );
			userprofile.NumberOfCalls = atoi ( numberofcalls );
			userprofile.FreeKByte     = atoi ( freekbyte );
			userprofile.Rate          = atoi ( rate );

			Flags = 0;
			if ( invisible[0]  == 'J') Flags |= USER_INVISIBLE;
			if ( network[0]    == 'J') Flags |= USER_NETWORKENABLED;
			if ( dontsave[0]   == 'J') Flags |= USER_DONTSAVE;
			if ( autoentry[0]  == 'J') Flags |= USER_AUTO;
			if ( autoremove[0] == 'J') Flags |= USER_AUTOREMOVE;
			userprofile.Flags = Flags;

			if ( strcmp ( password, "???" ) != 0 )
			{
				CLP_SetPassword ( &user, password );
//			int count;
//		  	for ( count = 0; count < strlen ( password ); count++ ) password [ count ] = toupper ( password [ count ] );
//				user.Password = CLONL_GetNameVal ( password );
			}
		}
		else
		{
			return;
		}
	}
}

//
// user_del()
//

void user_del ( void )
{
	CLP_DelUser ( userstr, TRUE );
}

//
// main()
//

int main(int argc, char *argv[])
{
struct Shortmenu *mymenu;
BOOL quit = FALSE;
char dummy;

	if ( !CLONL_Open("Online_Edituser.clcat")) { PutStr("Can't init ANSI-System.\n"); return 1; }

	while ( !quit )
	{
		CLONL_DrawTitle();
		mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_EDIT"), 'E' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_NEU"),  'N' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_DEL"),  'L' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"), 'Q' );
		dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );

		if ( dummy == 0 || dummy == 'Q' )
		{
			quit = TRUE;
		}
		else
		{
			ask_what();
			if ( strlen ( userstr ) == 0 ) break;

			if ( dummy == 'E' )
			{
				if ( !CLP_GetUser ( userstr, &user, &userprofile) ) 
				{ 
					CLONL_Printf ( MSG("MSG_NOUSER"), userstr ); 
					Delay ( 60 );
					dummy = ' ';
				}
			} else
			if ( dummy == 'N' )
			{
				strcpy ( user.Username, userstr );
				user.Priority = 0;

		    CLP_GetPrefs ( CLMSGDB_PREFSID, CLMSGDB_SUBID_USER, &user.Pfad, 255, NULL ); strcat ( user.Pfad, userstr );
				CLP_SetPassword ( &user, "" );
				strcpy ( userprofile.RealName, "" );
				strcpy ( userprofile.RealAdr, " - " );
				strcpy ( userprofile.RealInfo, "Neuer User" );
				strcpy ( userprofile.InfoMessage, "" );
				strcpy ( userprofile.CopyTo, "" );
				userprofile.Schreibschutz = 0;
				strcpy ( userprofile.Terminal, "ANSI" );
				strcpy ( userprofile.Language, "DEUTSCH" );
				strcpy ( userprofile.Keyboard, "ISO" );
				userprofile.LastLogin = 0;
				userprofile.NumberOfCalls = 0;
				userprofile.Timeout = 0;
				userprofile.AccountTalers = 0;
				userprofile.CreditTalers = 0;
				userprofile.ShowLines = 20;
				userprofile.Flags = 0;
				userprofile.UserTarif = 0;
				strcpy ( userprofile.StdProto, "ZMODEM" );
				userprofile.InhaltFlags = 0;
				userprofile.LesenFlags = 0;
				userprofile.MaxKB = 100;
				userprofile.MaxDays = 31;
				userprofile.LastCall = 0;
				userprofile.Uploads = 0;
				userprofile.Downloads = 0;
				userprofile.FreeFiles = 0;
				userprofile.FreeKByte = 0;
				userprofile.Rate = 0;
				memset ( user.Groups, 0, 16 );
				dummy = 'E';
			}

			switch ( dummy )
			{
				case 'E':
					user_edit();
					break;
				case 'L':
					{
					char dummy;
						CLONL_Printf ( MSG("MSG_DODELETE"), userstr ); 
						mymenu = CLONL_AddShortMenu ( NULL, MSG("MSG_DEL"),  'L' );
						mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"), 'Q' );
						dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );
						if( dummy == 'L' )					
							user_del();
					}
					break;
			}
		}
	}

	PutStr ("\n");

	CLONL_Close();
}
