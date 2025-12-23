//
// Brett2Dir Excludes all binary entrys in a directory, main.c
//
// Copyright 1995 by Mathias Mischer
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dostags.h>
#include <cl/clutil.h>
#include <cl/clb.h>
#include <cl/cl_msg.h>

void CopyMsg ( char *sourcedir, char *sourcename, char *destname, char *destdir, char *comment )
{
char str [ 256 ], source [ 256 ], dest [ 256 ];

	// Build source/dest filenames with path
	strcpy ( source, sourcedir );	AddPart ( source, sourcename, 255 );
	strcpy ( dest, destdir );	AddPart ( dest, destname, 255 );

	// Build AmigaOS Commands
	sprintf ( str, "c:COPY \"%s\" \"%s\"", source, dest );
	System ( str, NULL );
//Printf ( "%s\n", str );

	if ( comment && *comment )
	{
		sprintf ( str, "c:FILENOTE \"%s\" \"%s\"", dest, comment );
		System ( str, NULL );
//	Printf ( "%s\n", str );
	}
}

void PrintBinIndex ( APTR brett, APTR index, char *dir )
{
long num, i;
char *path;
char *subject, *shortcomment, *filename, storagefilename [ 256 ];
long flgbin;

	// Get Board Path
	CLB_Get ( brett, CLBA_PATH, &path, TAG_DONE );

	// Get Number of Messages
	CLMSG_Get ( index, CLMSGA_NUMBEROFMSGS, &num, TAG_DONE );
	Printf ( "%ld Nachrichten: (%s)\n", num, path );

	// Goto through all Messages
	for ( i = 0; i < num; i++ )
	{
		// Goto Message
		if ( CLMSG_Move ( index, CLMSG_MOVE_GOTO, i ) )
			continue;

		// Get Message Data
		CLMSG_Get ( index, 
			CLMSGA_SUBJECT, &subject, 
			CLMSGA_FILENAME, &filename,
			CLMSGA_SHORTCOMMENT, &shortcomment,
			CLMSGA_FLG_BIN, &flgbin,
			CLMSGA_STORAGE_FILENAME, &storagefilename,
			TAG_DONE );

		// Not binary ? Continue.
		if ( !flgbin ) 
			continue;

		// Print Out Info for User.
		Printf ( "%03ld - %s %-25.25s %s (%s)\n",
			i,
			flgbin ? "BIN" : "TXT", 
			subject,
			FilePart ( storagefilename ),
			filename 
			); 
	
		// Msg in Dir kopieren
		CopyMsg ( path, FilePart ( storagefilename ), filename, dir, shortcomment );
 	}
}

int main ( int argc, char **argv )
{
APTR brett = NULL;
APTR index = NULL;

	PutStr ( "\nBrett2Dir, Copyright 1995 by Mathias Mischler.\n\n");
	if ( argc < 3 )
	{
		Printf ( "Aufruf: %s <BRETT/NAME> <Pfad>\n", *argv );
		return 10;
	}
	CLB_LoadList();

	// Find Board
	brett = CLB_FindBrett( argv[1] );
	if ( !brett )
	{
		Printf ( "Fehler: Brett %s ist nicht eingetragen.\n", argv[1] );
		return 20;
	}

	// Lock Index, no change during
	index = CLMSG_GetIndexTags ( brett, CLMFT_WRITE, TRUE, TAG_DONE ); 
	if ( !index )
	{
		Printf ( "Fehler: Kann Index von Brett %s nicht lesen.\n", argv[1] );
		return 20;
	}

	PrintBinIndex ( brett, index, argv[2] );

	// Give Index Free, again 
	if ( index )
		CLMSG_FreeIndex ( index );
}
