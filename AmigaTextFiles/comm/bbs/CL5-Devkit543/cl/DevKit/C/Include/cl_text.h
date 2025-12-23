/*
 * Interface: cl_text.library
 *
 * $RCSfile$
 * $Author$
 * $Revision$
 * $Date$
 * $State$
 * $Locker$
 *
 * $Log$
 */

#ifndef _CL_TEXT_H
#define _CL_TEXT_H

#include <libraries/locale.h>

//
// External token replacement list
//

typedef struct CLTXT_TokEntry
{
	char *name;		// Name of token (UPPER CASE ONLY!)
	ULONG type;		// Replacement type
	APTR data;		// Data
} CLTXT_TokEntry;

//
// Possible types for CLTXT_TokEnts
//

#define CLTXT_RT_STRING 0		// Insert Data as a string (max. 1024 chars)
#define CLTXT_RT_INTEGER 1  	// Insert Data with %lD
#define CLTXT_RT_UNSIGNED 2		// Insert Data with %lU
#define CLTXT_RT_HOOK 3			// Call Hook (a0 = Hook, a1 = STRPTR to token data, a2 = STRPTR to buffer (must be zero-terminated, max. 1024 chars!)
#define CLTXT_RT_FILE 4			// Data is file to include (max. 1024 chars!)


// SAS/C pragmas

extern struct Library *CLTXTBase;

#pragma libcall CLTXTBase CLTXT_ReadPrefs 1e 0
#pragma libcall CLTXTBase CLTXT_OpenLocale 24 801
#pragma libcall CLTXTBase CLTXT_SPrintfA 2a BA9804
#pragma libcall CLTXTBase CLTXT_PrintfA 30 A9803
#pragma libcall CLTXTBase CLTXT_FormatDate 36 BA9804
#pragma libcall CLTXTBase CLTXT_Replace 3c BA9804
#pragma libcall CLTXTBase CLTXT_MakeFile 42 BA9804
#pragma libcall CLTXTBase CLTXT_OpenCatalog 48 9802
#pragma libcall CLTXTBase CLTXT_CloseCatalog 4e 801
#pragma libcall CLTXTBase CLTXT_GetString 54 A9803
#pragma libcall CLTXTBase CLTXT_GetLanguageByDomain 5a 801
#pragma libcall CLTXTBase CLTXT_FlushCatalogs 60 0
#pragma libcall CLTXTBase CLTXT_ReplaceAll 66 B0A9805
#pragma libcall CLTXTBase CLTXT_GetLanguageList 6c 0
#pragma libcall CLTXTBase CLTXT_ViewFile 72 B9803

#pragma tagcall CLTXTBase CLTXT_SPrintf 2a BA9804
#pragma tagcall CLTXTBase CLTXT_Printf 30 A9803


// ANSI prototypes

void CLTXT_ReadPrefs( void );
struct Locale *CLTXT_OpenLocale( STRPTR cllangname );
APTR CLTXT_SPrintfA( struct Locale *locale, STRPTR to, STRPTR format, APTR args );
APTR CLTXT_SPrintf( struct Locale *locale, STRPTR to, STRPTR format, ... );
APTR CLTXT_PrintfA( struct Locale *locale, STRPTR format, APTR args );
APTR CLTXT_Printf( struct Locale *locale, STRPTR format, ... );
void CLTXT_FormatDate( struct Locale *locale, STRPTR to, STRPTR format, struct DateStamp *datestamp );
int CLTXT_Replace( struct Locale *locale, STRPTR token, STRPTR to, CLTXT_TokEntry *tokents );
int CLTXT_ReplaceAll( struct Locale *locale, STRPTR src, STRPTR dst, size_t dstlen, CLTXT_TokEntry *tokents );
int CLTXT_MakeFile( STRPTR cllangname, STRPTR textfile, STRPTR tofilename, CLTXT_TokEntry *tokents );
APTR CLTXT_OpenCatalog( STRPTR cllangname, STRPTR catalogname );
void CLTXT_CloseCatalog( APTR clcatalog );
STRPTR CLTXT_GetString( APTR clcatalog, STRPTR stringid, STRPTR defstring );
STRPTR CLTXT_GetLanguageByDomain( STRPTR domainadr );
void CLTXT_FlushCatalogs( void );
STRPTR * CLTXT_GetLanguageList( void );
int CLTXT_ViewFile( STRPTR cllangname, STRPTR textfile, CLTXT_TokEntry *tokents );

#endif
