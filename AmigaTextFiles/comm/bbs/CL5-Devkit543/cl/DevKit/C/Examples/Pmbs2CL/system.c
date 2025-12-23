//
// Prometheus <-> Connectline Konverter, system.c
// 
// Copyright 1995 by Mathias Mischler
//

#include "all.h"

//
// ConvertPMBSSystem
// 

void ConvertPMBSSystem ( void ) 
{
BPTR file = Open ( PMBSSystemList, MODE_OLDFILE );
char boxname [ 22 ], domain [ 42 ], pmroute [ 22 ], passwort [ 22 ];
char telefon [ 42 ], modeminit [ 22 ];
WORD w;
LONG i;
struct Box box;

	if ( !file )
	{
		PutStr ( "Fehler: Kann Brettliste " PMBSSystemList " nicht öffnen.\n" );
		return;
	}

	CLS_LoadList ();
	CLS_LockList ( TRUE );

	PutStr ( "Scanne Systemliste:\n" );

	while ( FRead ( file, &w, 2, 1 ) > 0 )
	{
		if ( !w )
		{
			while ( ( i = FGetC ( file ) ) == 0 ); // ??? Martin ???
			if ( i < 0 ) break;
		}
	
	  FRead ( file, boxname, 20, 1 );
		FRead ( file, domain, 40, 1 );
		FRead ( file, pmroute, 20, 1 );
		FRead ( file, passwort, 20, 1 );
		FRead ( file, telefon, 40, 1 );
		FRead ( file, modeminit, 20, 1 );
		FRead ( file, &w, 2, 1 ); // Netztyp
		FRead ( file, &w, 2, 1 ); // Packer
		FRead ( file, &w, 2, 1 ); // Protokoll
		FRead ( file, &w, 2, 1 ); // Frei
		FRead ( file, &w, 2, 1 ); // Eilmail
		FRead ( file, &i, 4, 1 ); // Flags

		if ( CLS_FindBox ( boxname ) )
		{
			Printf ( "System %s ist bereits im CL-System eingetragen.\n", boxname );
			continue;
		}

		Printf ( "Trage System %s ein.\n", boxname );

		memset ( &box, 0, sizeof ( struct Box ) ); 
		 
		strcpy ( box.boxname, boxname );
		if ( domain [ 0 ] == '.' ) 
			strcpy ( box.boxtext, domain + 1);
		else
			strcpy ( box.boxtext, domain );
		strcpy ( box.outserver, pmroute );
		strcpy ( box.netzname, "JANUS" );
		strcpy ( box.xprprotosend, "ZMODEM" );
		strcpy ( box.packerin, "LHA" ); 
		strcpy ( box.packerout, "LHA" ); 
		strcpy ( box.telefon, telefon );
		strcpy ( box.password, passwort );
		box.flags = BOX_MAPS | BOX_BACKUP | BOX_EXTPROTO;

		CLS_AddEntry ( &box );
	} 

	Close ( file );

	CLS_UnlockList ();
	CLS_SaveList ();

	PutStr ( "Systemliste übertragen.\n\n" );
}
