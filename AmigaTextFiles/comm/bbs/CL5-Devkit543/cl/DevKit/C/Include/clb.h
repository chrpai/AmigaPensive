#ifndef _CL_CLB_H
#define _CL_CLB_H

/*
**
** clboard.library v16.2 definitions
**
*/

/*
** SAS/C pragmas 
*/

#ifndef _NO_PRAG
extern struct Library *CLBBase;
#pragma libcall CLBBase CLB_LoadList 1E 0
#pragma libcall CLBBase CLB_SaveList 24 0
#pragma libcall CLBBase CLB_GetEntry 2A 801
#pragma libcall CLBBase CLB_FindEntry 30 801
#pragma libcall CLBBase CLB_DeleteEntry 36 801
#pragma libcall CLBBase CLB_AddEntry 3C 801
#pragma libcall CLBBase CLB_LockList 42 001
#pragma libcall CLBBase CLB_UnlockList 48 0
#pragma libcall CLBBase CLB_LastEntry 4E 801
#pragma libcall CLBBase CLB_ValidateUser 54 9802
#pragma libcall CLBBase CLB_ValidateBox 5A 9802
#pragma libcall CLBBase CLB_TestBox 60 9802
#pragma libcall CLBBase CLB_AddBox 66 9802
#pragma libcall CLBBase CLB_DelBox 6C 9802
#pragma libcall CLBBase CLB_GetBox 72 09803
#pragma libcall CLBBase CLB_FindBrett 78 801
#pragma libcall CLBBase CLB_FindNewsgroup 7E 801
#pragma libcall CLBBase CLB_ClearBrettNetdist 84 801
#pragma libcall CLBBase CLB_GetNumEntries 8A 0
#pragma libcall CLBBase CLB_MakeAutoEntry 90 A9803
#pragma libcall CLBBase CLB_NG2BrettName 96 9802
#pragma libcall CLBBase CLB_Reserved1 9C 0
#pragma libcall CLBBase CLB_GetAutoEntry a2 001
#pragma libcall CLBBase CLB_DelAutoEntry a8 801
#pragma libcall CLBBase CLB_AddAutoEntry ae 801
#pragma libcall CLBBase CLB_FindAutoEntry b4 801
#pragma libcall CLBBase CLB_DelBoxEverywhere ba 801
#pragma libcall CLBBase CLB_GetA c0 9802
#pragma tagcall CLBBase CLB_Get c0 9802
#pragma libcall CLBBase CLB_SetA c6 9802
#pragma tagcall CLBBase CLB_Set c6 9802
#pragma libcall CLBBase CLB_NewObjectA cc 90803
#pragma tagcall CLBBase CLB_NewObject cc 90803
#pragma libcall CLBBase CLB_IsModified d2 0
#pragma libcall CLBBase CLB_NumAutoEntryBoards d8 0
#pragma libcall CLBBase CLB_FindFTSArea de 801
#pragma libcall CLBBase CLB_NewTestBox e4 9802
#pragma libcall CLBBase CLB_NewAddBox ea 9802
#pragma libcall CLBBase CLB_NewDelBox f0 9802
#pragma libcall CLBBase CLB_NewGetBox f6 0802
#pragma libcall CLBBase CLB_GetBoard_Relative fc 0BA9805
#pragma libcall CLBBase CLB_FindBoard_Prefix 102 0A9804
#pragma libcall CLBBase CLB_FindBoard_Next 108 0A9804
#pragma libcall CLBBase CLB_FindBoard_Prev 10e 0A9804
#pragma libcall CLBBase CLB_Debug 114 0
#pragma libcall CLBBase CLB_ValidateNets 11a 0
#endif

#define CLB_NAME "clboard.library"
#define CLB_VERSION 16

/*
** Board is a private handle. Use CLB_Get()/CLB_Set()
** to access data items.
*/
typedef APTR Board;

/*
** ANSI prototypes 
*/
#ifndef _NO_PROTO
int CLB_LoadList( void );
int CLB_SaveList( void );
Board CLB_GetEntry( Board );
void CLB_DeleteEntry( Board );
void CLB_LockList( int readwrite );
void CLB_UnlockList( void );
Board CLB_LastEntry( Board );
long CLB_ValidateUser( Board, struct User* );
long CLB_ValidateBox( Board, struct Box* );
long CLB_TestBox( Board, STRPTR boxname );
long CLB_AddBox( Board, STRPTR boxname );
long CLB_DelBox( Board, STRPTR boxname );
long CLB_GetBox( Board, STRPTR boxnamebuffer, ULONG index );
Board CLB_FindBrett( STRPTR boardname );
Board CLB_FindNewsgroup( STRPTR newsgroupname );
void CLB_ClearBrettNetdist( Board );
long CLB_GetNumEntries( void );
/* Internal */
Board CLB_MakeAutoEntry( char*, char*, char* );
ULONG CLB_NG2BrettName( STRPTR newsgroupname, STRPTR brettname );
void CLB_Reserved1( void );
Board CLB_GetAutoEntry( ULONG index );
Board CLB_FindAutoEntry( STRPTR name );
Board CLB_AddAutoEntry( APTR newautoentry );
Board CLB_DelAutoEntry( APTR autoentry );
void CLB_DelBoxEverywhere( STRPTR boxname );
long CLB_IsModified( void );
long CLB_NumAutoEntryBoards( void );
Board CLB_FindFTSArea( STRPTR areaname );
long CLB_NewTestBox( Board, struct Box *box );
long CLB_NewAddBox( Board, struct Box *box );
long CLB_NewDelBox( Board, struct Box *box );
struct Box *CLB_NewGetBox( Board, ULONG index );
void CLB_Debug( void );
int CLB_ValidateNets( void );

/*
**  ONLY USE THESE FUNCTIONS TO ACCESS BOARD DATA!
*/

Board CLB_NewObjectA( STRPTR boardname, ULONG autoentry, struct TagItem *vals );
Board CLB_NewObject( STRPTR boardname, ULONG autoentry, ... );
void CLB_GetA( Board brett, struct TagItem *vals );
void CLB_Get( Board brett, ... );
void CLB_SetA( Board brett, struct TagItem *vals );
void CLB_Set( Board brett, ... );

/*
**	Misc Support Stuff
*/

ULONG CLB_FindBoard_Prefix(
	struct User *user,			/* optional */
	STRPTR prefix,
	STRPTR dstname,				/* optional */
	size_t dstlen );

ULONG CLB_FindBoard_Next(
	struct User *user,			/* optional */
	STRPTR current,
	STRPTR dstname,				/* optional */
	size_t dstlen );

ULONG CLB_FindBoard_Prev(
	struct User *user,			/* optional */
	STRPTR current,
	STRPTR dstname,				/* optional */
	size_t dstlen );

ULONG CLB_GetBoard_Relative(
	struct User *user,			/* optional */
	char *currboard,
	char *path,
	char *dst,
	size_t dstlen );

#endif

/*
**  Board Attributes
*/

#define CLBA_Dummy			(TAG_USER+0x711)
#define CLBA_NAME 			(CLBA_Dummy+1)	/* STRPTR boardname */
#define CLBA_NGNAME 		(CLBA_Dummy+2)	/* STRPTR newsgroupname */
#define CLBA_PATH 			(CLBA_Dummy+3)	/* STRPTR dos path */
#define CLBA_ADMIN			(CLBA_Dummy+4)	/* STRPTR administrative user */
#define CLBA_GROUP			(CLBA_Dummy+5)	/* UBYTE[16] group flags */
#define CLBA_LASTUPDATE		(CLBA_Dummy+6)	/* time_t last write */
#define CLBA_HOLD_KB		(CLBA_Dummy+7)	/* ULONG maxkb */
#define CLBA_HOLD_DAYS		(CLBA_Dummy+8)	/* ULONG maxdays */
#define CLBA_PRI			(CLBA_Dummy+9)	/* ULONG access priority */
#define CLBA_ABOUT			(CLBA_Dummy+14)	/* STRPTR infoline */
#define CLBA_FTSNAME		(CLBA_Dummy+15)	/* STRPTR fidoname */
#define CLBA_CENSOR			(CLBA_Dummy+16)	/* STRPTR censor */
#define CLBA_WRITEPRI		(CLBA_Dummy+17)	/* ULONG write prority */
#define CLBA_MAXNET			(CLBA_Dummy+18)	/* ULONG maximum msg size. 0 = no limit */

#define CLBA_NEWFILEDATE	(CLBA_Dummy+24)	/* time_t internal for NewFiles! */
#define CLBA_NETCNT			(CLBA_Dummy+25) // V16: number of entries in net list


/*
**  The following attributes are boolean, but are stored as
**  a long integer containing either TRUE or FALSE.
*/

#define CLBA_NO_BIN			(CLBA_Dummy+10)	/* LONGBOOL no binaries allowed */
#define CLBA_NO_TEXT		(CLBA_Dummy+11)	/* LONGBOOL no text allowed */
#define CLBA_MAPS_ALLOWED	(CLBA_Dummy+12)	/* LONGBOOL maps access allowed */
#define CLBA_AUTOENTRY		(CLBA_Dummy+13)	/* LONGBOOL board was created automatically */
#define CLBA_REALNAME		(CLBA_Dummy+19)	/* LONGBOOL realname required */
#define CLBA_DLRATIO		(CLBA_Dummy+20)	/* LONGBOOL download ratio active */
#define CLBA_AUTOREMOVE		(CLBA_Dummy+21)	/* LONGBOOL auto remove */

#define CLBA_EXTERNAL		(CLBA_Dummy+22)	/* LONGBOOL board contents are stored externally */
#define CLBA_ROUTEONLY		(CLBA_Dummy+23)	/* LONGBOOL this board is route-only */

#define CLBA_NOSCAN			(CLBA_Dummy+26) // LONGBOOL don't rescan date on every access (only EXTERNAL)

//
//	Return-Values from CLB_ValidateUser()
//

#define CLBVU_NONE	0
#define CLBVU_ACCESS 1
#define CLBVU_CENSORED 2
#define CLBVU_IS_ADMIN 4
#define CLBVU_IS_CENSOR 8

#endif
