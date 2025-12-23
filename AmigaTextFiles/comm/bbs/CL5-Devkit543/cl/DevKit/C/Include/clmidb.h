#ifndef _CL_CLMIDB_H
#define _CL_CLMIDB_H

/*
		clmidb.library
		--------------
		fast MessageID database
*/

#ifndef _NO_PRAG
extern struct Library *CLMidbBase;
#pragma libcall CLMidbBase CLMID_StoreID 1e 9802
#pragma libcall CLMidbBase CLMID_CheckID 24 9802
#pragma libcall CLMidbBase CLMID_CheckStoreID 2a A9803
#pragma libcall CLMidbBase CLMID_Flush 30 0
#pragma libcall CLMidbBase CLMID_CloseDB 36 0
#pragma libcall CLMidbBase CLMID_CrunchIDs 3c 98003
#pragma libcall CLMidbBase CLMID_ShowStat 42 0
#pragma libcall CLMidbBase CLMID_ResetMinMax 48 0
#pragma libcall CLMidbBase CLMID_ShowMap 4e 0
#pragma libcall CLMidbBase CLMID_DumpDB 54 0
#endif

#ifndef _NO_PROTO
void CLMID_StoreID( STRPTR id, STRPTR fromsys );
time_t CLMID_CheckID( STRPTR id, STRPTR wherefrombuff );
time_t CLMID_CheckStoreID( STRPTR id, STRPTR fromsys, STRPTR wherefrombuff );
void CLMID_Flush( void );
long CLMID_CrunchIDs( ULONG days, APTR gaugeobj, APTR appobj );
void CLMID_CloseDB( void );
void CLMID_ShowStat( void );
void CLMID_ResetMinMax( void );
void CLMID_ShowMap( void );
void CLMID_DumpDB( void );
#endif

#define CLMIDB_NAME "clmidb.library"
#define CLMIDB_VERSION 7

/* 
**	PrefsID for preference data items
*/

#define CLMIDB_PREFSID MAKE_ID('C','M','I','D')
#define CLMIDB_SUBID_PATH (1|TAGT_STRING)

/*
**	Error returns from CLMID_CrunchIDs()
*/

#define CLMIDB_CERR_OPENDB 1		/* opendb() failed */
#define CLMIDB_CERR_OPENNEW 2		/* OpenAsync() on new output file failed */
#define CLMIDB_CERR_NOMEM 3			/* No memory for internal buffering */

/*
**	Flags to 'or' into Crunch Days
*/

#define CLMIDB_CRUNCH_HOLDBAK (1L<<30)	/* Hold backup after crunch */

#endif
