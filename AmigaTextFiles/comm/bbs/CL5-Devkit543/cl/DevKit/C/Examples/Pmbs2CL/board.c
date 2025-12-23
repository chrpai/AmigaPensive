//
// Prometheus <-> Connectline Konverter, board.c
// 
// Copyright 1995 by Mathias Mischler
//

#include "all.h"

char *fgetsterm ( BPTR file, char *str, ULONG len )
{
char *dest = str;

	if ( !str ) return NULL;
	while ( len && ((*dest) = FGetC ( file )) )
	{
		dest++; len--;
	}
	return str;
}
 

//
// ConvertPMBSBoard
// 

void ConvertPMBSBoard ( void ) 
{
BPTR file = Open ( PMBSBoardList, MODE_OLDFILE );
LONG i;
WORD w;
char brettname [ 42 ], pfad [ 82 ], betreuer [ 22 ];
char kommentar [ 82 ], l1 [ 256 ];
char verteiler [ 1024 ], realbrettname [ 256 ];
UWORD maxupload, rate, maxeintraege, maxalter, maxkb;
ULONG brettflags;
char brettpfad [ 256 ];
APTR board;
char *p, *r;

  CLP_GetPrefs ( CLMSGDB_PREFSID, CLMSGDB_SUBID_BOARD, brettpfad, 255, NULL ); 

	if ( !file )
	{
		PutStr ( "Fehler: Kann Brettliste " PMBSBoardList " nicht öffnen.\n" );
		return;
	}

	CLB_LoadList ();	// Laden, wenn noch nicht vorhanden
	CLB_LockList ( TRUE ); 	// exclusiv reservieren

	// Kennung (?) überspringen
	FRead ( file, &w, 2, 1 );

	PutStr ( "Scanne Brettliste:\n" );

	// Strukturgröße
	while ( FRead ( file, &w, 2, 1 ) > 0 )
	{
		if ( !w )
		{
			while ( ( i = FGetC ( file ) ) == 0 ); // ??? Martin ???
			if ( i < 0 ) break;
		}

		// Feste Brettdaten einlesen
		FRead ( file, brettname, 40, 1 );
		FRead ( file, pfad, 80, 1 );
		FRead ( file, betreuer, 20, 1 );
		FRead ( file, l1, 20, 1 ); // Passwort
		FRead ( file, kommentar, 80, 1 );
		FRead ( file, l1, 62, 1 ); // Gruppen

		// Brettwerte einlesen / ignorieren
		FRead ( file, &i, 4, 1 ); // Letzter Eintrag
		FRead ( file, &i, 4, 1 ); // Akt. Datum

		FRead ( file, &maxupload, 2, 1 );
		FRead ( file, &rate, 2, 1 ); if ( rate == 100 ) rate = 0;
		FRead ( file, &maxeintraege, 2, 1 );
		FRead ( file, &maxalter, 2, 1 ); 
		FRead ( file, &maxkb, 2, 1 );

		FRead ( file, &w, 2, 1 ); // Kopfnummer (?)
		FRead ( file, &brettflags, 4, 1 ); // Brettflags (no doks): maybe later

		fgetsterm ( file, realbrettname, 254 ); // Ganzer Brettname (!)
		fgetsterm ( file, verteiler, 1023 ); // Verteilerstring ( {<vert>'|'} )

		if ( realbrettname [ 0 ] == '/' )
			memmove ( realbrettname, realbrettname + 1, strlen ( realbrettname ) + 1 );

		if ( CLB_FindBrett ( realbrettname ) )
		{
			Printf ( "Brett %s ist bereits im CL-System eingetragen.\n", realbrettname );
			continue;
		}
		else
		{
			// Eintragen
			Printf ( "Trage Brett %s ein.\n", realbrettname );

			// Pfad berechnen
			strcpy ( l1, brettpfad ); AddPart ( l1, realbrettname, 255 );

			board = CLB_NewObject ( realbrettname, FALSE,
				CLBA_PATH, l1,
				CLBA_ADMIN, betreuer,
				CLBA_HOLD_KB, (ULONG) maxkb,
				CLBA_HOLD_DAYS, (ULONG) maxalter,
				CLBA_PRI, 99,
				CLBA_ABOUT, kommentar,
				CLBA_MAPS_ALLOWED, TRUE,
				CLBA_AUTOENTRY, TRUE,
				CLBA_REALNAME, TRUE
				);

			if ( !board ) 
			{
				PutStr ( "Fehler beim Anlegen des Brettes aufgetreten.\nBitte Connectline Preferences überprüfen.\n" );
				continue;
			}

			verteiler [ strlen ( verteiler ) + 1 ] = 0; // Doppelt Terminieren 			
			p = verteiler;
			while ( strlen ( p ) )
			{
				r = strchr ( p, '|' );
				if ( r ) *r = 0;
				CLB_AddBox ( board, p );
				p = r + 1;
			}
		}
	}

	Close ( file );

	CLB_UnlockList ();	// Brettlist freigeben
	CLB_SaveList ();	// Und auch abspeichern, jetzt, wo es wieder geht ;)	

	PutStr ( "Brettliste übertragen.\n\n" );
}
