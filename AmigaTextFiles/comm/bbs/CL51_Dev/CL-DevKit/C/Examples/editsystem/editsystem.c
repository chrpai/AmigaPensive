//
// Editsystem-Kommando für Connectline, editsystem.c
//
// Copyright 1995 by Mathias Mischler
//
// + Kopie anfertigen und darin arbeiten.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cl/clonline.h>
#include <cl/cl_prefs.h>
#include <cl/cls.h>

// Version String

char version[] = { "$VER: Editsystem 1.1 " __AMIGADATE__ };

char boxstr [ 42 ];
struct Box *mybox;
struct Box *membox;

//
// ask_what()
//

void ask_what ( void )
{
	PutMSG ("MSG_WHICH");
	CLONL_BlueRead ( boxstr, "", 40, TYPE_ASCII, FALSE );
}

//
// box_del()
//

void box_del ( void )
{
	CLS_DeleteEntry ( membox );
}

//
// cmd_edit()
//

void box_edit ( void )
{
struct Shortmenu *mymenu;
char dummy;
Mask *mask = NULL;
char titel [ 72 ];
char e_route_grundkosten [ 10 ], e_eil_grundkosten [ 10 ], e_modeminit [ 4 ];
char e_priority [ 10 ], e_eilkosten [ 10 ], e_batchsize [ 10 ], e_routekosten [ 10 ];
char e_maxpm [ 12 ];

char e_noeil [ 2 ], e_norout [ 2 ], e_maps [ 2 ], e_autoeintrag [ 2 ];
char e_backup [ 2 ], e_extproto [ 2 ];

	CLONL_SPrintf ( titel, MSG ("MSG_TITEL"), mybox->boxname );

	if ( mybox->flags & BOX_NOEIL )       strcpy ( e_noeil,       "J" ); else strcpy ( e_noeil,       "N" );
	if ( mybox->flags & BOX_NOROUT )      strcpy ( e_norout,      "J" ); else strcpy ( e_norout,      "N" );
	if ( mybox->flags & BOX_MAPS )        strcpy ( e_maps,        "J" ); else strcpy ( e_maps,        "N" );
	if ( mybox->flags & BOX_AUTOEINTRAG ) strcpy ( e_autoeintrag, "J" ); else strcpy ( e_autoeintrag, "N" );
	if ( mybox->flags & BOX_BACKUP )      strcpy ( e_backup,      "J" ); else strcpy ( e_backup,      "N" );
	if ( mybox->flags & BOX_EXTPROTO )    strcpy ( e_extproto,    "J" ); else strcpy ( e_extproto,    "N" );

	CLONL_SPrintf ( e_route_grundkosten, "%ld", mybox->rout_grundkosten );
	CLONL_SPrintf ( e_eil_grundkosten,   "%ld", mybox->eil_grundkosten );
	CLONL_SPrintf ( e_modeminit,         "%ld", mybox->modeminit );
	CLONL_SPrintf ( e_priority,          "%ld", mybox->priority );
	CLONL_SPrintf ( e_eilkosten,         "%ld", mybox->eilkosten );
	CLONL_SPrintf ( e_routekosten,       "%ld", mybox->routkosten );
	CLONL_SPrintf ( e_batchsize,         "%ld", mybox->batchsize );
	CLONL_SPrintf ( e_maxpm,             "%ld", mybox->maxpm );

	while ( 1 )
	{
		CLONL_DrawTitle();

		mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_DATEN"),         'D' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_EINSTELLUNGEN"), 'E' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_GRUPPEN"),       'G' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_SAVE"),          'S' );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"),          'Q' );
		dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );

		if ( dummy == 0 || dummy == 'Q' ) return;

		if ( dummy == 'S' )
		{
			memcpy ( membox, mybox, sizeof ( struct Box ) );
			CLS_SaveList ();
			return ;
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
						if ( CLP_GetGroup ( mybox->groups, groupc ) ) PutStr ("[7m");
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
							if ( dummy == 'M' ) CLP_SetGroup ( mybox->groups, count );
							if ( dummy == 'L' ) CLP_ClrGroup ( mybox->groups, count );
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

		mask = NULL;

		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_DOMAIN"),      mybox->boxtext,      40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_OUTSERVER"),   mybox->outserver,     40);
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_NETZNAME"),    mybox->netzname,      28);
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_ROUTGKOSTEN"), e_route_grundkosten,  4 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_EILGKOSTEN"),  e_eil_grundkosten,    4 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_MODEMINIT"),   e_modeminit,          2 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_XPRSEND"),     mybox->xprprotosend, 10 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PACKERIN"),    mybox->packerin,     40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PACKEROUT"),   mybox->packerout,    40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_POINTUSER"),   mybox->pointuser,    40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_TELEFON"),     mybox->telefon,      60 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_BOXINIT"),     mybox->boxinit,      40 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_PRIORITY"),    e_priority,           2 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_PASSWORD"),    mybox->password,     40 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_ROUTKOSTEN"), 	e_routekosten,        4 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_EILKOSTEN"),  	e_eilkosten,          4 );
		if ( dummy == 'D' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_BATCHSIZE"),   e_batchsize,          4 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_MAXPM"),  	    e_maxpm,             10 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_NOEIL"),       e_noeil,              1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_NOROUT"),      e_norout,             1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_MAPS"),        e_maps,               1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_AUTOEINTRAG"), e_autoeintrag,        1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_BACKUP"),      e_backup,             1 );
		if ( dummy == 'E' ) CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_EXTPROTO"),    e_extproto,           1 );
		
		if ( CLONL_InputMask ( mask, titel ) )
		{
			mybox->rout_grundkosten  = atoi ( e_route_grundkosten );
			mybox->eil_grundkosten   = atoi ( e_eil_grundkosten );
			mybox->modeminit         = atoi ( e_modeminit );
			mybox->priority          = atoi ( e_priority );
			mybox->eilkosten         = atoi ( e_eilkosten );
			mybox->routkosten        = atoi ( e_routekosten );
			mybox->batchsize         = atoi ( e_batchsize );
			mybox->maxpm             = atoi ( e_maxpm );

			if ( e_noeil       [ 0 ] == 'J' ) mybox->flags |= BOX_NOEIL;       else mybox->flags &= ~BOX_NOEIL; 
			if ( e_norout      [ 0 ] == 'J' ) mybox->flags |= BOX_NOROUT;      else mybox->flags &= ~BOX_NOROUT;
			if ( e_maps        [ 0 ] == 'J' ) mybox->flags |= BOX_MAPS;        else mybox->flags &= ~BOX_MAPS;
			if ( e_autoeintrag [ 0 ] == 'J' ) mybox->flags |= BOX_AUTOEINTRAG; else mybox->flags &= ~BOX_AUTOEINTRAG;
			if ( e_backup      [ 0 ] == 'J' ) mybox->flags |= BOX_BACKUP;      else mybox->flags &= ~BOX_BACKUP;
			if ( e_extproto    [ 0 ] == 'J' ) mybox->flags |= BOX_EXTPROTO;    else mybox->flags &= ~BOX_EXTPROTO;   
		}
		else
		{
			return;
		}
	}
}

int main(int argc, char *argv[])
{
struct Shortmenu *mymenu;
BOOL quit = FALSE;

	if ( !CLONL_Open("Online_Editsystem.clcat")) { PutStr("Can't init ANSI-System.\n"); return 1; }

	CLS_LockList ( TRUE );

	mybox = malloc ( sizeof ( struct Box ) );

	while ( !quit)
	{
	char dummy;

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
			if ( strlen ( boxstr ) == 0 ) break;

			if ( dummy == 'N' )
			{
 				membox = malloc ( sizeof ( struct Box ) );
				memset ( membox, 0, sizeof ( struct Box ) );
				strcpy ( membox->boxname, boxstr );
				strcpy ( membox->boxtext, "do.main" );
				strcpy ( membox->outserver, boxstr );
				strcpy ( membox->netzname, boxstr );
				membox->valid_ports = -1;
				strcpy ( membox->xprprotosend, "ZMODEM" );
				strcpy ( membox->packerin, "LHA" );
				strcpy ( membox->packerout, "LHA" );
				membox->flags = BOX_MAPS & BOX_BACKUP & BOX_EXTPROTO;
				CLS_AddEntry ( membox );
				dummy = 'E';
			}
			else
			if ( ! ( membox = CLS_FindBox ( boxstr ) ) )
			{
				CLONL_Printf ( MSG ("MSG_UNKNOWN"), boxstr );
				Delay ( 60 );
				continue;
			}
			memcpy ( mybox, membox, sizeof ( struct Box ) );
			switch ( dummy )
			{
				case 'E':
					box_edit();
					break;
				case 'L':
					box_del();
					break;
			}
		}
	}

	PutStr ("\n");

	CLS_UnlockList ();

	free ( mybox );

	CLONL_Close();
}
