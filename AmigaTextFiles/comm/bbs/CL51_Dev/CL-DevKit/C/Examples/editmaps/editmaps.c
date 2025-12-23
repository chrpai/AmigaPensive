//
// EditMAPS, editmaps.c
//
// Copyright 1996 by Mathias Mischler, All Rights Reserved
//

char version[] = { "$VER: EditMAPS 0.1 " __AMIGADATE__ };

#include <stdlib.h>
#include <string.h>
#include <cl/clutil.h>
#include <cl/clonline.h>
#include <cl/cl_prefs.h>
#include <cl/cl_maps.h>

APTR maps_prefs, maps_handle;
struct maps_cmd *maps_cmd;

static char tp[] = "M+*-";

void MAPS_Edit( void )
{
char maps_minpri[ 3 ], maps_access[ 2 ], titel[ 82 ];
Mask *mask = NULL;

	CLONL_SPrintf( titel, MSG( "MSG_TITEL" ), CLU_NOL_GetEntryName( maps_handle ) );
	maps_access[ 0 ] = tp[ maps_cmd->user_cmd ];
	maps_access[ 1 ] = 0;
	CLONL_SPrintf( maps_minpri, "%ld", maps_cmd->minpri );

	CLONL_AddGadget( &mask, TYPE_ASCII, MSG( "MSG_EXEC" ), maps_cmd->exec, 60 );
	CLONL_AddGadget( &mask, TYPE_NUMBER, MSG( "MSG_MINPRI" ), maps_minpri, 2 );
	CLONL_AddGadget( &mask, TYPE_ASCII, MSG( "MSG_ACCESS" ), maps_access, 1 );

	if( CLONL_InputMask( mask, titel ) )
	{
	char *c = strchr( tp, maps_access[ 0 ] ), in;
	struct Shortmenu *mymenu;

		if( c )
			maps_cmd->user_cmd = c - tp;
		maps_cmd->minpri = atoi( maps_minpri );

		CLONL_GotoXY ( 1, 7 );

		mymenu = CLONL_AddShortMenu( NULL, &MSG( "MSG_SAVE" )[ 1 ], MSG( "MSG_SAVE" )[ 0 ] );
		mymenu = CLONL_AddShortMenu( mymenu, &MSG( "MSG_QUIT" )[ 1 ], MSG( "MSG_QUIT" )[ 0 ] );
		in = CLONL_ShortMenuSelect( mymenu );
		CLONL_FreeShortMenu( mymenu );

		if( in == MSG( "MSG_SAVE" )[ 0 ] )
			CLP_SetPrefs( MAPS_PREFSID, MAPS_PREFSSUB, &maps_prefs, 4, NULL );
	}
}

void main( void )
{
BOOL quit = FALSE;

	if( !CLONL_Open( "Online_EditMAPS.clcat") )
	{
		PutStr( "Can't init ANSI-System.\n" );
		return;
	}

	CLP_GetPrefs( MAPS_PREFSID, MAPS_PREFSSUB, &maps_prefs, 4, NULL );
	if( !maps_prefs )
	{
		PutStr( "Can't load MAPS-Preferences.\n" );
		return;
	}

	while( !quit )
	{
	struct Shortmenu *menu;
	char in;

		CLONL_DrawTitle();
		menu = CLONL_AddShortMenu( NULL, &MSG( "MSG_EDIT" )[ 1 ], MSG( "MSG_EDIT" )[ 0 ] );
		menu = CLONL_AddShortMenu( menu, &MSG( "MSG_NEU" )[ 1 ], MSG( "MSG_NEU" )[ 0 ] );
		menu = CLONL_AddShortMenu( menu, &MSG( "MSG_DEL" )[ 1 ], MSG( "MSG_DEL" )[ 0 ] );
		menu = CLONL_AddShortMenu( menu, &MSG( "MSG_LIST" )[ 1 ], MSG( "MSG_LIST" )[ 0 ] );
		menu = CLONL_AddShortMenu( menu, &MSG( "MSG_QUIT" )[ 1 ], MSG( "MSG_QUIT" )[ 0 ] );
		in = CLONL_ShortMenuSelect( menu );
		CLONL_FreeShortMenu( menu );

		if( !in || in == MSG( "MSG_QUIT" )[ 0 ] )
		{
			quit = TRUE;
			continue;
		}

		if( in == MSG( "MSG_LIST" )[ 0 ] )
		{
			maps_handle = CLU_NOL_GetFirstEntry( maps_prefs );
			PutStr( "\n" );

			while( maps_handle )
			{
			char *maps_name;

				maps_cmd = CLU_NOL_GetEntryData( maps_handle );
				maps_name = CLU_NOL_GetEntryName( maps_handle );

				Printf( "%-20.20s %2ld %lc %-50.50s\n",
					maps_name,
					maps_cmd->minpri,
					tp[ maps_cmd->user_cmd ],
					maps_cmd->exec
				);
				maps_handle = CLU_NOL_GetNextEntry( maps_handle );
			}
			CLONL_Wait();
			continue;
		}
		else
		{
		char maps_name[ 42 ];

			PutMSG( "MSG_WHICH" );
			CLONL_BlueRead( maps_name, "", 40, TYPE_ASCII, FALSE );

			if( in == MSG( "MSG_NEU" )[ 0 ] )
			{
 				maps_cmd = AllocMem( sizeof( *maps_cmd ), MEMF_CLEAR );
				strcpy( maps_cmd->exec, "CONNECTLINE:MapsCommands/" );
				maps_cmd->minpri = 99;
				maps_cmd->user_cmd = MAPSACCESS_NONE;
				in = MSG( "MSG_EDIT" )[ 0 ];
				CLU_NOL_AddEntry( maps_prefs, maps_name, maps_cmd, NOLIM_INSERT_SORTED, NULL );
				FreeMem( maps_cmd, sizeof( *maps_cmd ) );
			}

			maps_handle = CLU_NOL_FindEntry( maps_prefs, maps_name );
			if( !maps_handle )
			{
				CLONL_Printf( MSG( "MSG_UNKNOWN" ), maps_name );
				CLONL_Wait();
				continue;
			}

			maps_cmd = CLU_NOL_GetEntryData( maps_handle );

			if( in == MSG( "MSG_DEL" )[ 0 ] )
			{
				CLU_NOL_DeleteEntry( maps_handle );
				CLP_SetPrefs( MAPS_PREFSID, MAPS_PREFSSUB, &maps_prefs, 4, NULL );
			}
			else
			if( in == MSG( "MSG_EDIT" )[ 0 ] )
				MAPS_Edit();	
		}
	}

	PutStr( "\n" );
	CLU_NOL_Delete( maps_prefs );
	CLONL_Close();
}
