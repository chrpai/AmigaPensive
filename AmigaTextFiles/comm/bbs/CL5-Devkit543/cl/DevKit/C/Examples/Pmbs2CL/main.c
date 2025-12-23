//
// Prometheus <-> Connectline Konverter, main.c
// 
// Copyright 1995 by Mathias Mischler
//

#define Version "1.1 " __AMIGADATE__
char VERSTR[] = "$VER: PMBS->Connectline-Konverter " Version;

#include "all.h"

void main ( int argc, char **argv )
{
	PutStr ( "\n*** Prometheus -> Connectline Konverter " Version " ***\nCopyright 1995 by Mathias Mischler.\n\n" );
	PutStr ( "Bei Problemen kann das Programm beliebig oft gestartet werden.\nEs werden keinerlei Daten doppelt eingetragen.\n\n" );

	ConvertPMBSSystem ();
	ConvertPMBSUser ();
	ConvertPMBSBoard ();

	PutStr ( "Fertig. Viel Spaﬂ mit Connectline!\n" );
}
