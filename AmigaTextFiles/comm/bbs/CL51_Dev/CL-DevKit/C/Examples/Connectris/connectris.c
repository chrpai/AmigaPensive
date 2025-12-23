//
// Connectris-Kommando für Connectline
//
// Copyright 1995 by Mathias Mischler
//

#define HighscoreFilename "CONNECTLINE:prefs/CL-Tris.score"
#define Firstline 4
#define Begin 20
#define Width 15
#define Length 17
#define NextX 45
#define NextY 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cl/clonline.h>

// Version String

char version[] = { "$VER: Connectris 1.2 " __AMIGADATE__ };

//
// Draw_Highscore ()
//

void Draw_Highscore ( void )
{
BPTR lock, file;
char spieler [ 10 ][ 40 ];
ulong punkte [ 10 ];
ulong count;
char line [ 82 ];

	lock = Lock ( HighscoreFilename, ACCESS_READ );
	if ( lock )
	{
		UnLock ( lock );
		if ( file = Open ( HighscoreFilename, MODE_OLDFILE ) )
		{
			for ( count = 0; count < 10; count ++ )
			{
				FGets ( file, line, 80 );
				line [ strlen ( line ) - 1 ] = 0; // LF entfernen
				sscanf ( line, "%d %s", &punkte [ count ], spieler [ count ] );
			}
			Close ( file );
		}
		else
		{
			PutStr ("Can't open highscore-file.\n");
			return;
		}
	}
	else
	{
		if ( file = Open ( HighscoreFilename, MODE_NEWFILE ) )
		{
			for ( count = 0; count < 10; count ++ )
			{
				punkte [ count ] = 0;
				strcpy ( spieler [ count ], "???" );
				FPuts ( file, "0 ???\n" );
			}
			Close ( file );
		}
		else
		{
			PutStr ("Can't open highscore-file.\n");
			return;
		}
	}

	PutMSG ("MSG_HIGHSCORE");
	for ( count = 0; count < 10; count ++ )
	{
		CLONL_Printf ( MSG ("MSG_HIGHSCOREFORMAT"), punkte [ count ], spieler [ count ] );
	}

	PutStr ("\n");
}

//
// Draw_Playfield ()
//

void Draw_Playfield ( void )
{
ulong count;

	CLONL_Color ( COLOR_Blue, COLOR_White, STYLE_Inverse );
	for ( count = Firstline; count < Firstline + Length; count ++ )
	{
		CLONL_GotoXY ( Begin - 1,     count ); FPutC ( Output(), '|' );
		CLONL_GotoXY ( Begin + Width, count ); FPutC ( Output(), '|' );
	}
	CLONL_GotoXY ( Begin - 1, Firstline + Length );
	FPutC ( Output(), '+' );
	for ( count = 0; count < Width; count ++ )
	{
		FPutC ( Output(), '-' );
	}
	FPutC ( Output(), '+' );
	CLONL_FlushOutput ();
}

char teile [] =
{
	0, 0,  0, 1,  1, 1,  1, 0, // Quader
	0, 0,  0, 1,  1, 1,  1, 2, //
  1, 0,  1, 1,  0, 1,  0, 2, //
	0, 0,  0, 1,  0, 2,  0, 3, // Langer Vierer
	0, 0,  0, 1,  1, 1,  0, 2, // Dreieck
  0, 0,  1, 0,  1, 1,  1, 2, //
  0, 0,  1, 0,  0, 1,  0, 2, //

	0, 0,  0, 1,  1, 1,  1, 0, // Quader
	0, 0,  1, 0, -1, 1,  0, 1, //
  0, 0, -1, 0,  0, 1,  1, 1, //
 -1, 1,  0, 1,  1, 1,  2, 1, // Langer Vierer
 -1, 0,  0, 0,  1, 0,  0, 1, // Dreieck
  1, 0, -1, 1,  0, 1,  1, 1, //
 -1, 0,  0, 0,  1, 0,  1, 1, //

	0, 0,  0, 1,  1, 1,  1, 0, // Quader
	0, 0,  0, 1,  1, 1,  1, 2, //
  1, 0,  1, 1,  0, 1,  0, 2, //
	0, 0,  0, 1,  0, 2,  0, 3, // Langer Vierer
 -1, 1,  0, 0,  0, 1,  0, 2, // Dreieck
  0, 0,  0, 1,  0, 2,  1, 2, //
  0, 0,  0, 1, -1, 2,  0, 2, //

	0, 0,  0, 1,  1, 1,  1, 0, // Quader
	0, 0,  1, 0, -1, 1,  0, 1, //
  0, 0, -1, 0,  0, 1,  1, 1, //
 -1, 1,  0, 1,  1, 1,  2, 1, // Langer Vierer
 -1, 1,  0, 1,  1, 1,  0, 0, // Dreieck
 -1, 0,  0, 0,  1, 0, -1, 1, //
 -1, 0, -1, 1,  0, 1,  1, 1, //
};

char playfield [ 80 ][ Length + Firstline + 2 ];

//
// Draw_Teil ()
//

void Draw_Teil ( ulong teil, ulong x, ulong y, ulong dreh, BOOL write )
{
char *p;

	p = teile + ( 8 * ( teil - 1 )) + ( dreh * 7 * 8 );
	CLONL_Color ( teil, 0, STYLE_Inverse );
	CLONL_GotoXY ( x + *(p + 0) , y + *( p + 1) ); FPutC ( Output(), '+' );
	CLONL_GotoXY ( x + *(p + 2) , y + *( p + 3) ); FPutC ( Output(), '+' );
	CLONL_GotoXY ( x + *(p + 4) , y + *( p + 5) ); FPutC ( Output(), '+' );
	CLONL_GotoXY ( x + *(p + 6) , y + *( p + 7) ); FPutC ( Output(), '+' );
	PutStr ("[0m"); CLONL_GotoXY ( 1, Firstline );
	CLONL_FlushOutput ();

	if ( write )
	{
		playfield [ x + *(p + 0) ][ y + *(p + 1)] = teil;
		playfield [ x + *(p + 2) ][ y + *(p + 3)] = teil;
		playfield [ x + *(p + 4) ][ y + *(p + 5)] = teil;
		playfield [ x + *(p + 6) ][ y + *(p + 7)] = teil;
	}
}

//
// Delete_Teil ()
//

void Delete_Teil ( ulong teil, ulong x, ulong y, ulong dreh )
{
char *p;

	p = teile + ( 8 * ( teil - 1 )) + ( dreh * 7 * 8 );
	PutStr ("[0m");
	CLONL_GotoXY ( x + *(p + 0) , y + *( p + 1) ); FPutC ( Output(), ' ' );
	CLONL_GotoXY ( x + *(p + 2) , y + *( p + 3) ); FPutC ( Output(), ' ' );
	CLONL_GotoXY ( x + *(p + 4) , y + *( p + 5) ); FPutC ( Output(), ' ' );
	CLONL_GotoXY ( x + *(p + 6) , y + *( p + 7) ); FPutC ( Output(), ' ' );
}

//
// Check_Collision ()
//

BOOL Check_Collision ( ulong teil, ulong x, ulong y, ulong dreh )
{
char *p;

	p = teile + ( 8 * ( teil - 1 )) + ( dreh * 7 * 8 );
	if ( playfield [ x + *(p + 0) ][ y + *(p + 1)] == 0
		&& playfield [ x + *(p + 2) ][ y + *(p + 3)] == 0
		&& playfield [ x + *(p + 4) ][ y + *(p + 5)] == 0
		&& playfield [ x + *(p + 6) ][ y + *(p + 7)] == 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//
// Check_Lines ()
//

ulong Check_Lines ( ulong start )
{
ulong x, y;
ulong intx, inty;
BOOL is;
ulong howmuch;

	howmuch = 0;
	for ( y = start; y < Firstline + Length; y ++ )
	{
		is = TRUE;
		for ( x = Begin; x < ( Begin + Width ); x ++ )
		{
			if ( playfield [ x ][ y ] == 0 ) is = FALSE;
		}
		if ( is )
		{
			howmuch ++; PutStr ("[0m");
			CLONL_GotoXY ( 1, y         ); CLONL_DeleteLine ();
			CLONL_GotoXY ( 1, Firstline ); CLONL_InsertLine ();
			CLONL_Color ( COLOR_Blue, COLOR_White, STYLE_Inverse );
			CLONL_GotoXY ( Begin - 1,     Firstline ); FPutC ( Output(), '|' );
			CLONL_GotoXY ( Begin + Width, Firstline ); FPutC ( Output(), '|' );

			for ( inty = y ; inty > Firstline; inty -- )
			{
				for ( intx = Begin; intx < ( Begin + Width ); intx ++ )
				{
					playfield [ intx ][ inty ] = playfield [ intx ][ inty - 1 ];
				}
			}
			for ( intx = Begin; intx < ( Begin + Width ); intx ++ )
			{
				playfield [ intx ][ Firstline ] = 0;
			}
		}
	}
	return howmuch;
}

//
// Spiel ()
//

ulong Spiel ( void )
{
ulong teil, x, y, delay, count, dreh, nextteil;
ulong oldx, oldy, score, teilcount;
BOOL falling, work, first;
long key;

	CLONL_DrawTitle ();
	Draw_Playfield ();

	score = 0;

	// Spielfeld löschen
	for ( y = 0; y < ( Length + Firstline + 2 ); y ++ )
	{
		for ( x = 0; x < 80; x ++ )
		{
			if ( y == ( Length + Firstline ) )
				playfield [ x ][ y ] = 1;
			else				
				if ( x >= Begin && x < ( Begin + Width ) )
					playfield [ x ][ y ] = 0;
				else
					playfield [ x ][ y ] = 1;
		}
	}
	while ( !( teil = rand () & 7 ) ); // schlechter Zufall, durch time_t ersetzen ?!
	delay = 30;
	teilcount = 0;
	work = TRUE;

	while ( work )
	{
		while ( !( nextteil = rand () & 7 ) ); // schlechter Zufall, durch time_t ersetzen ?!

		CLONL_GotoXY ( NextX, NextY ); PutMSG ("MSG_NEXT");
		CLONL_GotoXY ( 1, Firstline - 1 ); CLONL_Printf ( MSG("MSG_SCORE"), score );
		Draw_Teil ( nextteil, NextX + 1, NextY + 2, 0, FALSE );	

		x = Begin + ( Width / 2 );
		y = Firstline;
		oldx = x; oldy = y;
		falling = TRUE;
		dreh = 0;
		first = TRUE;

		while ( falling )
		{
			if ( Check_Collision ( teil, x, y, dreh ) )
			{
				first = FALSE;
				Draw_Teil ( teil, x, y, dreh, FALSE );
				for ( count = 0; count < delay; count ++ )
				{
					Delay ( 1 );
					if ( CLONL_IsKeyPressed() )
					{
						switch ( CLONL_GetKey () )
						{
							case CHAR_EOF:
							case CHAR_BREAK:
								return score;
								break;
							case CHAR_DOWN:
								score += 1;
								count = delay;
								break;
							case CHAR_LEFT:
								if ( Check_Collision ( teil, x - 1, y, dreh ) )
								{
									Delete_Teil ( teil, x, y, dreh );
									x -= 1;
									Draw_Teil ( teil, x, y, dreh, FALSE );
								}
								break;
							case CHAR_RIGHT:
								if ( Check_Collision ( teil, x + 1, y, dreh ) )
								{
									Delete_Teil ( teil, x, y, dreh );
									x += 1;
									Draw_Teil ( teil, x, y, dreh, FALSE );
								}
								break;
							case ' ':
							case CHAR_UP:
								if ( Check_Collision ( teil, x, y, (dreh + 1) & 3 ) )
								{
									Delete_Teil ( teil, x, y, dreh );
									dreh = (dreh + 1) & 3;
									Draw_Teil ( teil, x, y, dreh, FALSE );
								}
								break;
						}
					}
				}
				Delete_Teil ( teil, x, y, dreh );
				oldx = x; oldy = y;
				y += 1;
			}
			else
			{
				while ( CLONL_IsKeyPressed() )
				{
					key = CLONL_GetKey();
					if ( key == CHAR_BREAK || key == CHAR_EOF ) return score;
				}
				Draw_Teil ( teil, oldx, oldy, dreh, TRUE );
				if ( first ) 
				{
					work = FALSE;
					falling = FALSE;
				}
				else
				{
					teilcount ++;
					if ( teilcount >= 40 )
					{
						if ( delay > 1 ) delay --;
					}
					score += 10;
					CLONL_GotoXY ( NextX, NextY ); CLONL_DeleteEOL ();
					Delete_Teil ( nextteil, NextX + 1, NextY + 2, 0 );	
					score += ( Check_Lines ( oldy ) * 100 );
					falling = FALSE;
					teil = nextteil;
				}
			}
		}
	}
	Delay ( 120 );	
	return score;
}

//
// Sortin_Score()
//

void Sortin_Score ( ulong score, char *name )
{
BPTR file;
char spieler [ 11 ][ 40 ];
ulong punkte [ 11 ];
ulong count, pos;
char line [ 82 ];
BOOL nicht;

	nicht = TRUE;

	if ( file = Open ( HighscoreFilename, MODE_OLDFILE ) )
	{
		pos = 0;
		for ( count = 0; count < 10; count ++ )
		{
			FGets ( file, line, 80 );
			line [ strlen ( line ) - 1 ] = 0; // LF entfernen
			sscanf ( line, "%d %s", &punkte [ pos ], spieler [ pos ] );

			if ( nicht && strcmp ( spieler [ pos ], name ) == 0 && punkte [ pos ] >= score ) break;
			if ( nicht && punkte [ pos ] < score ) 
			{ 
				strcpy ( spieler [ pos + 1 ], spieler [ pos ] );
				punkte [ pos + 1 ] = punkte [ pos ];
				strcpy ( spieler [ pos ], name );
				punkte [ pos ] = score;
				pos ++; 
				nicht = FALSE; 
			}
			if ( strcmp ( spieler [ pos ], name ) == 0 ) pos --;
			pos++;
		}
		Close ( file );
	}
	else
	{
		PutStr ("Can't open highscore-file.\n");
		return;
	}

	if (!nicht)
	{
		if ( file = Open ( HighscoreFilename, MODE_NEWFILE ) )
		{
			for ( count = 0; count < 10; count ++ )
			{
				FPrintf ( file, "%ld %s\n", punkte [ count ], spieler [ count ] );
			}
			Close ( file );
		}
		else
		{
			PutStr ("Can't open highscore-file.\n");
			return;
		}
	}
}

//
// main
//

int main ( int argc, char *argv[] )
{
struct Shortmenu *mymenu;
char key;
BOOL quit = FALSE;
ulong score;

	if ( !CLONL_Open ( "Online_Connectris.clcat" ) ) { PutStr ( "Can't init ANSI-System.\n" ); return 20; }

	if ( CLONL_IsTTY () ) { PutStr ("You need at least VT100 or ANSI Terminal to run.\n" ); goto Ende; }

	mymenu = CLONL_AddShortMenu ( NULL, MSG("MSG_SPIELEN"), MSG("MSG_SPIELENHOT")[ 0 ] );
	mymenu = CLONL_AddShortMenu ( mymenu, MSG("MSG_QUIT"), MSG("MSG_QUITHOT")[ 0 ] );

	while ( !quit )
	{
	  CLONL_DrawTitle ();
		PutStr ("[0m[34mCopyright 1995 by Mathias Mischler\n\n");
		Draw_Highscore ();

		key = CLONL_ShortMenuSelect ( mymenu );

		switch ( key )
		{
			case 'Q':
			case 0:
				quit = TRUE;
				break;
			case 'S':
				srand ( time ( 0 ) );
				score = Spiel ();
				Sortin_Score ( score, CLONL_GetUserShell()->user.Username );
				break;
		}
	}
	
	PutStr ("\n");
	CLONL_FreeShortMenu ( mymenu );
	Ende:
	CLONL_Close ();
}
