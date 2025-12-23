//
// Editbrett-Kommando für Connectline, editbrett.c
//
// Copyright 1995 by Mathias Mischler
//
// + Akt. Brett ist Default-Vorgabe.
//

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cl/clonline.h>
#include <cl/cl_prefs.h>
#include <dos/dostags.h>
#include <cl/clb.h>
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#include <cl/cl_misc.h>

// Version String

char version[] = { "$VER: Editbrett 1.3 " __AMIGADATE__ };

char brettstr [ 128 ];
APTR board;

//
// ask_what()
//

void ask_what ( void )
{
char defboard [ 520 ];

	defboard [ 0 ] = '/';
	strcpy ( defboard+1, CLONL_GetUserShell()->currentboard );

	PutMSG ("MSG_WHICH");
	CLONL_BlueRead ( brettstr, defboard, 48, TYPE_ASCII, FALSE );
	if ( brettstr [ 0 ] == '/' )
	{
		memmove ( brettstr, brettstr + 1, strlen ( brettstr ) + 1 );
	}
}

//
// brett_edit()
//

void brett_edit ( void )
{
struct Shortmenu *mymenu;
Mask *mask;
char dummy;
char titel [ 80 ];

char *path, *admin, *group, *about, *censor;
char e_path [ 128 ], e_admin [ 80 ], e_about [ 80 ], e_censor [ 80 ], e_group [ 17 ];

ulong hold_kb, hold_days, pri, writepri;
char e_hold_kb [ 14 ], e_hold_days [ 14 ], e_pri [ 10 ], e_writepri [ 10 ];

ulong no_bin, no_text, maps_allowed, autoentry, realname, dlratio, autoremove, netcnt;
ulong noscan, routeonly;

char e_no_bin [ 2 ], e_no_text [ 2 ], e_maps_allowed [ 2 ], e_autoentry [ 2 ];
char e_realname [ 2 ], e_dlratio [ 2 ], e_autoremove [ 2 ], e_noscan [ 2 ], e_routeonly [ 2 ];

	CLB_Get ( board,
		CLBA_PATH, &path,
 		CLBA_ADMIN, &admin,
		CLBA_GROUP, &group,
		CLBA_HOLD_KB, &hold_kb,
		CLBA_HOLD_DAYS, &hold_days,
		CLBA_PRI, &pri,
		CLBA_ABOUT, &about,
		CLBA_CENSOR, &censor,
		CLBA_WRITEPRI, &writepri,
		CLBA_NO_BIN, &no_bin,
		CLBA_NO_TEXT, &no_text,
		CLBA_MAPS_ALLOWED, &maps_allowed,
		CLBA_AUTOENTRY, &autoentry,
		CLBA_REALNAME, &realname,
		CLBA_DLRATIO, &dlratio,
		CLBA_AUTOREMOVE, &autoremove,
		CLBA_NOSCAN, &noscan,
		CLBA_ROUTEONLY, &routeonly,
		TAG_DONE );

	CLONL_SPrintf ( e_hold_kb,   "%ld", hold_kb );
	CLONL_SPrintf ( e_hold_days, "%ld", hold_days );
	CLONL_SPrintf ( e_pri,       "%ld", pri );
	CLONL_SPrintf ( e_writepri,  "%ld", writepri );

	if ( path )	strncpy ( e_path, path, 127 );
	else strcpy ( e_path, "" );
	if ( admin ) strncpy ( e_admin, admin, 79 );
	else strcpy ( e_admin, "" );
	if ( about ) strncpy ( e_about, about, 79 );
	else strcpy ( e_about, "" );
	if ( censor ) strncpy ( e_censor, censor, 79 );
	else strcpy ( e_censor, "" );
	memcpy ( e_group, group, 16 );

	if ( no_bin       ) strcpy ( e_no_bin,       "J" ); else strcpy ( e_no_bin,       "N" );
	if ( no_text      ) strcpy ( e_no_text,      "J" ); else strcpy ( e_no_text,      "N" );
	if ( maps_allowed ) strcpy ( e_maps_allowed, "J" ); else strcpy ( e_maps_allowed, "N" );
	if ( autoentry    ) strcpy ( e_autoentry,    "J" ); else strcpy ( e_autoentry,    "N" );
	if ( realname     ) strcpy ( e_realname,     "J" ); else strcpy ( e_realname,     "N" );
	if ( dlratio      ) strcpy ( e_dlratio,      "J" ); else strcpy ( e_dlratio,      "N" );
	if ( autoremove   ) strcpy ( e_autoremove,   "J" ); else strcpy ( e_autoremove,   "N" );
	if ( noscan       ) strcpy ( e_noscan,       "J" ); else strcpy ( e_noscan,       "N" );
	if ( routeonly    ) strcpy ( e_routeonly,    "J" ); else strcpy ( e_routeonly,    "N" );

	while ( 1 )
	{
		CLONL_DrawTitle();

		CLONL_Printf ( MSG("MSG_BRETTSTR"), brettstr );

		mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_DATEN"),         'D' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_EINSTELLUNGEN"), 'E' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_GRUPPEN"),       'G' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_BOXEN"),         'B' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_SAVE"),          'S' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"),          'Q' );
		dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );

		if ( dummy == 0 || dummy == 'Q' ) return;
		if ( dummy == 'S' ) 
		{ 
			CLB_Set ( board,
				CLBA_PATH, e_path,
				CLBA_ADMIN, e_admin,
				CLBA_GROUP, e_group,
				CLBA_HOLD_KB, hold_kb,
				CLBA_HOLD_DAYS, hold_days,
				CLBA_PRI, pri,
				CLBA_ABOUT, e_about,
				CLBA_CENSOR, e_censor,
				CLBA_WRITEPRI, writepri,
				CLBA_NO_BIN, no_bin,
				CLBA_NO_TEXT, no_text,
				CLBA_MAPS_ALLOWED, maps_allowed,
				CLBA_AUTOENTRY, autoentry,
				CLBA_REALNAME, realname,
				CLBA_DLRATIO, dlratio,
				CLBA_AUTOREMOVE, autoremove,
				CLBA_NOSCAN, noscan,
				CLBA_ROUTEONLY, routeonly,
				TAG_DONE );
			CLB_SaveList ();
			return;
		}

		if ( dummy == 'G' ) 
		{
		ulong x,y, groupc, count;
		char gruppen [ 80 ];
		struct Range *myrange, *temprange;

			CLONL_DrawTitle();

			while ( 1 )
			{
				for ( y = 0; y < 16; y++ )
				{
					for ( x = 0; x < 6; x++ )
					{
						groupc = 1 + y * 6 + x;
						CLONL_GotoXY ( x * 13, 3 + y );
						Printf ( "[0m[36;1m%2ld:[0m[35m", groupc ); 
						if ( CLP_GetGroup ( e_group, groupc ) ) PutStr ("[7m");
						PutStr ( CLP_GetGroupName ( groupc ) );
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
							if ( dummy == 'M' ) CLP_SetGroup ( e_group, count );
							if ( dummy == 'L' ) CLP_ClrGroup ( e_group, count );
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

		if ( dummy == 'B' ) 
		{
		char boxstr [ 22 ], aktbox [ 22 ];
		ulong count;
			while ( 1 )
			{
				CLONL_DrawTitle();
				CLB_Get ( board, CLBA_NETCNT, &netcnt, TAG_DONE );
				for ( count = 0; count < netcnt; count ++ )
				{
					CLB_GetBox ( board, aktbox, count );
					CLONL_Printf ( "[35m%s ", aktbox );
				}				
				PutStr ("\n\n");

				mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_NEU"),  'N' );
				mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_DEL"),  'L' );
				mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"), 'Q' );
				dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );
				if ( dummy == 0 ) return;
				if ( dummy == 'Q') break;
				PutStr ("[A"); CLONL_DeleteEOL ();
				PutMSG ("MSG_WELCHEBOX");
				CLONL_BlueRead ( boxstr, "", 20, TYPE_ASCII, FALSE );
				PutStr ("[A"); CLONL_DeleteEOL ();
				if ( dummy == 'N' ) CLB_AddBox ( board, boxstr );
				if ( dummy == 'L' ) CLB_DelBox ( board, boxstr );
			}
			continue;
		}

		CLONL_SPrintf ( titel, MSG ("MSG_TITEL"), brettstr );

		mask = NULL;

		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_ADMIN"),       e_admin,       30 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_CENSOR"),      e_censor,      30 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_HOLDKB"),      e_hold_kb,     10 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_HOLDDAYS"),    e_hold_days,    4 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_PRI"),         e_pri,          2 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_WRITEPRI"),    e_writepri,     2 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_ABOUT"),       e_about,       70 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PATH"),        e_path,        70 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_NOBIN"),       e_no_bin,       1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_NOTEXT"),      e_no_text,      1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_REALNAME"),    e_realname,     1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_DLRATIO"),     e_dlratio,      1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_MAPSALLOWED"), e_maps_allowed, 1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_AUTOENTRY"),   e_autoentry,    1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_AUTOREMOVE"),  e_autoremove,   1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_NOSCAN"),      e_noscan,       1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_ROUTEONLY"),   e_routeonly,    1 );

		if ( CLONL_InputMask ( mask, titel ) )
		{
			if ( e_no_bin       [ 0 ] == 'J' ) no_bin       = 1; else no_bin       = 0;
			if ( e_no_text      [ 0 ] == 'J' ) no_text      = 1; else no_text      = 0;
			if ( e_maps_allowed [ 0 ] == 'J' ) maps_allowed = 1; else maps_allowed = 0;
			if ( e_autoentry    [ 0 ] == 'J' ) autoentry    = 1; else autoentry    = 0;
			if ( e_realname     [ 0 ] == 'J' ) realname     = 1; else realname     = 0;
			if ( e_dlratio      [ 0 ] == 'J' ) dlratio      = 1; else dlratio      = 0;
			if ( e_autoremove   [ 0 ] == 'J' ) autoremove   = 1; else autoremove   = 0;
			if ( e_noscan       [ 0 ] == 'J' ) noscan       = 1; else noscan       = 0;
			if ( e_routeonly    [ 0 ] == 'J' ) routeonly    = 1; else routeonly    = 0;

			hold_kb   = atoi ( e_hold_kb );
			hold_days = atoi ( e_hold_days );
			pri       = atoi ( e_pri );
			writepri  = atoi ( e_writepri );
		}
		else
		{
			return;
		}
	}
}

//
// brett_del()
//

void brett_del ( void )
{
	CLB_DeleteEntry ( board );
	CLB_SaveList ();
}

//
// main()
//

int main(int argc, char *argv[])
{
struct Shortmenu *mymenu;
BOOL quit = FALSE;
char dummy;

	if ( !CLONL_Open("Online_Editbrett.clcat")) { PutStr("Can't init ANSI-System.\n"); return 1; }

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
			if ( strlen ( brettstr ) == 0 ) break;

			if ( dummy == 'N' )
			{
			char pfad [ 255 ];
		    CLP_GetPrefs ( CLMSGDB_PREFSID, CLMSGDB_SUBID_BOARD, pfad, 255, NULL ); 
				strcat ( pfad, brettstr );
				CLB_NewObject ( brettstr, FALSE,
					CLBA_PRI, 99,
					CLBA_PATH, pfad,
					CLBA_ABOUT, (ULONG) &"Neues Brett",
					CLBA_WRITEPRI, 99,
					CLBA_REALNAME, TRUE,
					CLBA_DLRATIO, FALSE,
					CLBA_AUTOREMOVE, FALSE,
					TAG_DONE );
				dummy = 'E';
			}
			if ( dummy == 'E' || dummy == 'L' )
			{
				if ( !( board = CLB_FindBrett ( brettstr ) ) )
				{ 
					CLONL_Printf ( MSG("MSG_NOBRETT"), brettstr ); 
					Delay ( 60 );
					dummy = ' ';
				}
			}

			switch ( dummy )
			{
				case 'E':
					brett_edit();
					break;
				case 'L':
					brett_del();
					break;
			}
		}
	}

	PutStr ("\n");

	CLONL_Close();
} 
