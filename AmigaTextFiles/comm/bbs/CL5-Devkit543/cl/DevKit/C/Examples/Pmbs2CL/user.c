//
// Prometheus <-> Connectline Konverter, user.c
// 
// Copyright 1995 by Mathias Mischler
//

#include "all.h"

//
// ConvertPMBSUser 
// 

void ConvertPMBSUser ( void )
{
BPTR file = Open ( PMBSUserList, MODE_OLDFILE ), uf;
char ul [ 256 ], fn [ 140 ], l1 [ 256 ], l2 [ 256 ], l3 [ 256 ];
char clup [ 256 ];
ULONG i;
UWORD w;
UBYTE b;
struct User u;
struct UserProfile up;
struct Box *box;

	CLP_GetPrefs ( CLMSGDB_PREFSID, CLMSGDB_SUBID_USER, clup, 256, NULL );

	if ( !file )
	{
		PutStr ( "Fehler: Kann Userliste " PMBSUserList " nicht öffnen.\n" );
		return;
	}

	PutStr ( "Scanne Userdaten:\n" );

	while ( FGets ( file, ul, 255 ) )
	{
		CLU_clnl ( ul );
		if ( CLP_GetUser ( ul, &u, &up ) )
		{
			Printf ( "User %s ist bereits im CL-System eingetragen.\n", ul );
			continue;
		}

		// Userdatenfile laden
		sprintf ( fn, PMBSUserPath "%s/°dat", ul );
		uf = Open ( fn, MODE_OLDFILE );

		if ( !uf )
		{
			Printf ( "Fehler: Kann Userdatenfile %s nicht öffnen.\n", uf );
			continue;
		}

		// CL Strukturen löschen	
		memset ( &u, 0, sizeof ( struct User ) );
		memset ( &up, 0, sizeof ( struct UserProfile ) );

		Printf ( "Trage User %s in CL-System ein.\n", ul );

		// Pointname
		FGets ( uf, l2, 255 ); CLU_clnl ( l2 ); // l2 aufheben für Pointverkettung

		// Username auf 30 Zeichen kürzen und eintragen
		FGets ( uf, l1, 255 ); CLU_clnl ( l1 );
		l1 [ 30 ] = 0;
		strcpy ( u.Username, l1 );

		// Pointverkettung ( Pointname in l2 ) 
		if ( l2 [ 0 ] )
		if ( box = CLS_FindBox ( l2 ) )
		{
			Printf ( "Point: Verbinde User %s mit System %s.\n", ul, l2 );
			CLS_LockList ( TRUE );

			strcpy ( box->pointuser, l1 );

			CLS_UnlockList ();
			CLS_SaveList ();
		}
		else
		{
			Printf ( "Fehler: Kann System %s nicht finden.\n", l2 );
		}

		// Userpfad setzen
		strcpy ( l2, clup ); AddPart ( l2, l1, 255 );
		strcpy ( u.Pfad, l2 );

		// Vorname, Nachname
		FGets ( uf, l1, 255 ); CLU_clnl ( l1 );
		FGets ( uf, l2, 255 ); CLU_clnl ( l2 );
		sprintf ( l3, "%s %s", l1, l2 ); l3 [ 40 ] = 0;
		strcpy ( up.RealName, l3 );

		// Straße, Ort
		FGets ( uf, l1, 255 ); CLU_clnl ( l1 );
		FGets ( uf, l2, 255 ); CLU_clnl ( l2 );
		sprintf ( l3, "%s - %s", l1, l2 ); l3 [ 40 ] = 0;
		strcpy ( up.RealAdr, l3 );

		// Telefon
		FGets ( uf, l1, 255 ); CLU_clnl ( l1 );
		l1 [ 40 ] = 0;
		strcpy ( up.RealTel, l1 );

		// Alter (ignore)
		FGets ( uf, l1, 255 );

		// Mitteilung
		FGets ( uf, l1, 255 ); CLU_clnl ( l1 );
		l1 [ 80 ] = 0;
		strcpy ( up.InfoMessage, l1 );

		// Modem Computer Prompt (ignore)
		FGets ( uf, l1, 255 );
		FGets ( uf, l1, 255 );
		FGets ( uf, l1, 255 );

		// Sysopinfo
		FGets ( uf, l1, 255 ); CLU_clnl ( l1 );
		l1 [ 40 ] = 0;
		strcpy ( up.RealInfo, l1 );

		// Länge der Daten holen
		FRead ( uf, &i, 2, 1 );

		// FirstLogin
		FRead ( uf, &i, 4, 1 );
		up.created = time ( 0 );

		// LastLogin
		FRead ( uf, &i, 4, 1 );

		// Passwortänderung, Passwort
		FRead ( uf, &i, 4, 1 );
		FRead ( uf, &i, 4, 1 );

		// Anrufe
		FRead ( uf, &i, 4, 1 );
		up.NumberOfCalls = i;

		// Flags
		FRead ( uf, &i, 4, 1 );

		// Freiraum
		FRead ( uf, &i, 4, 1 );
		up.FreeKByte = i / 1024;

		// Bytes Up
		FRead ( uf, &i, 4, 1 );
		up.Uploads = i / 1024;

		// Bytes Down
		FRead ( uf, &i, 4, 1 );
		up.Downloads = i / 1024;
		
		// Ascii Up/Down 
		FRead ( uf, &i, 4, 1 );
		FRead ( uf, &i, 4, 1 );

		// Konto
		FRead ( uf, &i, 4, 1 );
		up.AccountTalers = i;

		// Baudrate/LBoxanrufe
		FRead ( uf, &i, 4, 1 );
		FRead ( uf, &i, 4, 1 );

		// Zeitlimit
		FRead ( uf, &w, 2, 1 );
		up.Timeout = w;

		// Restzeit, TopUpCPS, TopDownCPS
		FRead ( uf, &w, 2, 1 );
		FRead ( uf, &w, 2, 1 );
		FRead ( uf, &w, 2, 1 );

		// Emulation, Zeilen
		FRead ( uf, &b, 1, 1 );
		FRead ( uf, &b, 1, 1 );
		up.ShowLines = b;
			
		Close ( uf );

		// Standard Daten setzen
		u.pw [ 0 ] = rand ();	u.pw [ 1 ] = rand ();	u.pw [ 2 ] = rand ();	u.pw [ 3 ] = rand ();
		u.pw [ 4 ] = rand ();	u.pw [ 5 ] = rand ();	u.pw [ 6 ] = rand ();	u.pw [ 7 ] = rand ();
		u.pw [ 8 ] = rand ();	u.pw [ 9 ] = rand ();	u.pw [ 10 ] = rand ();
		u.Passwordval = rand ();

		strcpy ( up.Terminal, "ANSI" );
		strcpy ( up.Language, "DEUTSCH" );
		strcpy ( up.Keyboard, "ISO" );
		up.Flags = USER_AUTO;
		strcpy ( up.StdProto, "ZMODEM" );

		// User speichern
		CLP_AllocateUserlist ( TRUE );
		CLP_SetUser ( &u, &up ); 
		CLP_FreeUserlist ();
	}

	Close ( file );

	PutStr ( "\nDie Passwörter können nicht konvertiert werden.\nBitte im Usereditor nach Userwunsch nachtragen.\n\n" );
	PutStr ( "Empfehlen sie ihren Benutzern, die Onlinebefehle SET und FLAGS aufzurufen,\num ihre Accounts nach ihren Wünschen einzustellen.\n\n" ); 
}

