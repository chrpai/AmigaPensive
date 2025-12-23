//
// Editcmd-Kommando für Connectline, editcmd.c
//
// Copyright 1995 by Mathias Mischler
//

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cl/clonline.h>
#include <cl/cl_prefs.h>
#include <cl/clutil.h>
#include <cl/shell.h>

// Version String

char version[] = { "$VER: Editcmd 1.1 " __AMIGADATE__ };

char cmdstr [ 128 ];
APTR cmdnol, mycmdh;
struct clshell_cmd *mycmd;

//
// ask_what()
//

void ask_what ( void )
{
	PutMSG ("MSG_WHICH");
	CLONL_BlueRead ( cmdstr, "", 40, TYPE_ASCII, FALSE );
}

//
// cmd_del()
//

void cmd_del ( void )
{
	CLU_NOL_DeleteEntry ( mycmd );
	CLP_SetPrefs ( SHELL_PREFSID, SHELL_PREFSSUB, &cmdnol, 4, NULL );
}

//
// cmd_edit()
//

void cmd_edit ( void )
{
Mask *mask = NULL;
char titel [ 72 ];
char cmd_typ [ 12 ], cmd_log [ 3 ], cmd_ser [ 12 ], cmd_prio [ 4 ], cmd_logprio [ 4 ];

	CLONL_SPrintf ( titel, MSG ("MSG_TITEL"), CLU_NOL_GetEntryName ( mycmdh ) );

	if ( mycmd->type == CLSC_DOS ) strcpy ( cmd_typ, "DOS" );
	else                           strcpy ( cmd_typ, "ALIAS" );

	if ( mycmd->flg_log ) strcpy ( cmd_log, "J" );
	else                  strcpy ( cmd_log, "N" );

	switch ( mycmd->flg_ser )
	{
		case 0 :
			strcpy ( cmd_ser, "STDIO" );
			break;
		case 1 :
			strcpy ( cmd_ser, "FREI" );
			break;
		case 2 :
			strcpy ( cmd_ser, "SCHLIESSEN" );
			break;
	}

	CLONL_SPrintf ( cmd_prio, "%ld", mycmd->minpri );

	CLONL_SPrintf ( cmd_logprio, "%ld", mycmd->log_pri );

	CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_TYP"),     cmd_typ, 10);
	CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_EXEC"),    mycmd->exec, 60);
	CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_HELP"),    mycmd->helpfilenode, 60);
	CLONL_AddGadget ( &mask, TYPE_ASCII,  MSG("MSG_SERIAL"),  cmd_ser, 10);
	CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_PRIO"),    cmd_prio, 2);
	CLONL_AddGadget ( &mask, TYPE_BOOL,   MSG("MSG_LOG"),     cmd_log, 1);
	CLONL_AddGadget ( &mask, TYPE_NUMBER, MSG("MSG_LOGPRIO"), cmd_logprio, 2);

	if ( CLONL_InputMask ( mask, titel ) )
	{
	struct Shortmenu *mymenu;
	char dummy;

		if ( strcmp ( cmd_typ, "ALIAS" ) == 0 ) mycmd->type = CLSC_ALIAS;
		else                                    mycmd->type = CLSC_DOS;

		if ( cmd_log [ 0 ] == 'J' ) mycmd->flg_log = 1;
		else                        mycmd->flg_log = 0;

		if      ( strcmp ( cmd_ser, "STDIO" )       == 0 ) mycmd->flg_ser = 0;
		else if ( strcmp ( cmd_ser, "FREI" )        == 0 ) mycmd->flg_ser = 1;
		else if ( strcmp ( cmd_ser, "SCHLIESSEN" )  == 0 ) mycmd->flg_ser = 2;

		mycmd->minpri = atoi ( cmd_prio );

		mycmd->log_pri = atoi ( cmd_logprio );

		CLONL_GotoXY ( 1, 11 );

		mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_SAVE"), MSG("MSG_SAVEHOT")[ 0 ] );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"), MSG("MSG_QUITHOT")[ 0 ] );
		dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );

		if ( dummy == 'S' )
		{
			CLP_SetPrefs ( SHELL_PREFSID, SHELL_PREFSSUB, &cmdnol, 4, NULL );
		}
	}
}

int main(int argc, char *argv[])
{
struct Shortmenu *mymenu;
BOOL quit = FALSE;

	if ( !CLONL_Open("Online_Editcmd.clcat")) { PutStr("Can't init ANSI-System.\n"); return 1; }

	while ( !quit)
	{
	char dummy;

		CLONL_DrawTitle();
		mymenu = CLONL_AddShortMenu ( NULL,   MSG("MSG_EDIT"), MSG("MSG_EDITHOT")[ 0 ] );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_NEU"),  MSG("MSG_NEUHOT") [ 0 ] );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_DEL"),  MSG("MSG_DELHOT") [ 0 ] );
		mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"), MSG("MSG_QUITHOT")[ 0 ] );
		dummy = CLONL_ShortMenuSelect ( mymenu ); CLONL_FreeShortMenu ( mymenu );

		if ( dummy == 0 || dummy == 'Q' )
		{
			quit = TRUE;
		}
		else
		{
			ask_what();
			if ( strlen ( cmdstr ) == 0 ) break;

		  if ( !CLP_GetPrefs ( SHELL_PREFSID, SHELL_PREFSSUB, &cmdnol, 4, NULL ) ) { PutStr ("Can't get command-prefs.\n"); break; }

			if ( dummy == 'N' )
			{
 				mycmd = malloc ( sizeof ( struct clshell_cmd ) );
				strcpy ( mycmd->exec, "CONNECTLINE:commands/" );
				mycmd->type = CLSC_DOS;
				mycmd->minpri = 99;
				mycmd->flg_log = FALSE;
				mycmd->log_pri = 6;
				mycmd->flg_ser = 0;
				strcpy ( mycmd->helpfilenode, "" );
				CLU_NOL_AddEntry ( cmdnol, cmdstr, mycmd, NOLIM_INSERT_SORTED, NULL);
				dummy = 'E';
			}

			if ( !( mycmdh = CLU_NOL_FindEntry ( cmdnol, cmdstr ) ) )
			{
				CLONL_Printf ( MSG ("MSG_UNKNOWN"), cmdstr );
				Delay ( 60 );
			}
			else
			{
				mycmd = CLU_NOL_GetEntryData ( mycmdh );
	
				switch ( dummy )
				{
					case 'E':
						cmd_edit();
						break;
					case 'L':
						cmd_del();
						break;
				}
			}
	 	  CLU_NOL_Delete ( cmdnol );
		}
	}

	PutStr ("\n");

	CLONL_Close();
}
