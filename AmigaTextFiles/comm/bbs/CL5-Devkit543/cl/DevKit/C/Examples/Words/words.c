//
// Words-Kommando für Connectline, words.c
//
// Copyright 1995 by Mathias Mischler
//

#define HighscoreFilename "CONNECTLINE:Prefs/CL-Words.score"
#define WordsFilename     "CONNECTLINE:Prefs/CL-Words.words"

#define MaxWords 2048		// Old fashing non-dynamical array
#define MaxMask  2047

#define Startline 4			// First line of Playfield
#define Endline 20			// Last line of Playfield
#define Numberlines 16 	
#define Inputline 21		// Line for Input

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cl/clonline.h>

// Version String

static char version[] = { "$VER: Words 1.2 " __AMIGADATE__ };

static long numberwords;

//
// Draw_Highscore ()
//

static void Draw_Highscore ( void )
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
// Sortin_Score()
//

static void Sortin_Score ( ulong score, char *name )
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

static char *words [ MaxWords ];

//
// GetWords
//

static void GetWords ( void )
{
BPTR file;
char line [ 82 ];
long i = 0;

	numberwords = 0;

	file = Open ( WordsFilename, MODE_OLDFILE );

	if  ( !file )
		return;

	while ( FGets ( file, line, 80 ) )
	{
		if ( line [ strlen ( line ) - 1 ] = 10 )
			line [ strlen ( line ) - 1 ] = 0;
		words [ i ] = malloc ( strlen ( line ) + 1 );
		if ( !words [ i ] )
		{
			PutStr ( "Error getting memory.\n" );
			continue;
		}
		strcpy ( words [ i ], line );
		numberwords ++;
		i++;
		if ( i >= MaxWords ) 
		{
			PutStr ( "Error field too small.\n" );
			break;
		}
	}

	words [ i ] = NULL;

	Close ( file );
}

struct WORD
{
	char *word;
	long line;
};


//
// Spiel
//

static long Spiel ( void )
{
long score = 0;
long x, i, j;
struct WORD word [ Numberlines ];
char myword [ 80 ];
long pause = 20, delay;
long newword = 10, newdelay;
long harder = 0;
bool leave = FALSE;
long lifes = 3;

  CLONL_DrawTitle ();
	
	CLONL_GotoXY ( 1, Inputline ); 

	strcpy ( myword, "" );
	for ( i = 0; i < Numberlines; i ++ )
	{
		word[ i ].word = NULL;
		word[ i ].line = 0;
	}
	newdelay = 0;
	
	while ( !leave )
	{
		if ( newdelay <= 0 )
		{
			newdelay = newword;

			j = rand ();
			if ( j ) j %= numberwords;

			i = 70 - strlen ( words [ j ] );
			x = rand ();
			if ( x ) x %= i;

			CLONL_GotoXY ( x, Startline ); 
			CLONL_Color ( COLOR_White, COLOR_Black, STYLE_Normal );

			for ( i = 0; i < Numberlines; i++ )
			{
				if ( word[ i ].word == NULL )
				{
					word[ i ].word = words [ j ];
					word[ i ].line = Startline;
					PutStr ( word[ i ].word );
					break;
				}
			}
		}
		else
		{
			newdelay --;
		}

		delay = pause ;

		while ( !leave && delay >= 0 )
		{
			Delay ( 1 );
			if ( CLONL_IsKeyPressed () )
			{
				static long key;
				key = CLONL_GetKey ();
				switch ( key )
				{
					case CHAR_BS:
						myword [ strlen ( myword ) - 1 ] = 0;
						break;

					case CHAR_CR:
						CLONL_GotoXY ( 2, Inputline ); 
						PutStr ( myword );
						CLONL_FlushOutput ();
						for ( i = 0; i < Numberlines; i++ )
						{
							if ( word[ i ].word && myword [ 0 ] && stricmp ( word[ i ].word, myword ) == 0 )
							{
								CLONL_GotoXY ( 1, word[ i ].line );
								CLONL_DeleteEOL ();
								word[ i ].word = NULL;
								word[ i ].line = 0;
								newdelay = 0; // new word... hehe !

								score += 50;
								harder ++ ;
								if ( harder > 10 ) 
								{
									if ( newword > 1 ) newword--;	
									harder = 0;
								}
							}
						}
						strcpy ( myword, "" );
						break;

					case CHAR_EOF:
					case CHAR_BREAK:
						leave = TRUE;
						break;

					default:
						myword [ strlen ( myword ) + 1 ] = 0;
						myword [ strlen ( myword ) ] = key;
						break;
				}
			}
			delay --;
		}

		score ++;		

		// Scroll & Draw new
		CLONL_GotoXY ( 1, Inputline ); 
		CLONL_DeleteEOL ();

		CLONL_GotoXY ( 1, Startline - 1 ); 
		CLONL_Printf ( MSG ("MSG_SCORE"), score );

		CLONL_GotoXY ( 1, Endline ); 
		CLONL_DeleteEOL ();

		CLONL_GotoXY ( 1, Startline ); 
		CLONL_InsertLine ();

		for ( i = 0; i < Numberlines; i ++ )
		{
			if ( word[ i ].line )
			{
				word[ i ].line ++;
				if ( word[ i ].line >= Endline )	// Lost
				{
					word[ i ].word = NULL;
					word[ i ].line = 0;
					lifes --;
					if ( lifes <= 0 )
					{
						leave = TRUE;
						CLONL_GotoXY ( 25, Startline + Numberlines / 2 );
						CLONL_Color ( COLOR_Red, COLOR_Black, STYLE_Bold );
						PutStr ( "-G-A-M-E---O-V-E-R-" );	
					}
				}
			}
		}

		CLONL_GotoXY ( 70, Inputline ); 
		CLONL_Color ( COLOR_Blue, COLOR_Black, STYLE_Bold );
		for ( i = 0; i < lifes; i++ )
			PutStr ( "*" );
		CLONL_GotoXY ( 1, Inputline ); 
		CLONL_Color ( COLOR_Green, COLOR_Black, STYLE_Bold );
		PutStr ( ">" ); 
		CLONL_Color ( COLOR_Yellow, COLOR_Black, STYLE_Bold );
		PutStr ( myword );

		CLONL_FlushOutput ();
	}

	Delay ( 60 );

	return score;
}

//
// main
//

int main ( int argc, char *argv[] )
{
struct Shortmenu *mymenu;
char key;
BOOL quit = FALSE;
long score;

	GetWords ();
	if ( !words ) 
	{
		PutStr ( "Error loading words...\nInform System-Operator!\n" );
		return 0;
	}

	if ( !CLONL_Open ( "Online_Words.clcat" ) ) { PutStr ( "Can't init ANSI-System.\n" ); return 20; }

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
