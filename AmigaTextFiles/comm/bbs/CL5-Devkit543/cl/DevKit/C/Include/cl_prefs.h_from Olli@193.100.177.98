#ifndef INCLUDE_CL_PREFS_H
#define INCLUDE_CL_PREFS_H

/*
 *   Connectline 5.x cl_prefs.library definitions
 *
 *   $VER: 23.0
 *
 */


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef _CL_CL_USER_H
#include <cl/cl_user.h>
#endif

/* Obsolete, use clutil.library/CLU_OpenLibraryCode()! */
#define CLP_VERSION 22
#define CLP_NAME "CONNECTLINE:Libs/cl_prefs.library"

/*
 *
 * CLPrefsNotify
 *
 * Such a Message is send to your port every
 * time any changes to the selected PrefsID
 * happen
 *
 */

typedef struct CLMsg {
	struct Message m;
	ULONG id;
} CLMsg;

#define MSG_PREFSNOTIFY 1
#define MSGTYPE_PREFSNOTIFY MSG_PREFSNOTIFY

typedef struct CLPrefsNotifyMsg {
	struct CLMsg clpnm_msg;		/* Standard CL Exec Message; DO NEVER EVER ReplyMsg() THIS, USE CLP_ReplyNotify() instead! */
    ULONG clpnm_msgid;			/* set to CLPNM_ID_ */
	ULONG clpnm_prefsID;		/* Prefs ID of changed node */
	ULONG clpnm_subID;			/* SubID of changed node or TAGT_INVALID */
	/* following fields are private! */

} CLPrefsNotifyMsg;

#define CLMessage CLMsg
#define CLPrefsNotifyMessage CLPrefsNotifyMsg

/*
 *  CLPrefsNotifyMsg Message IDs
 */

#define CLPNM_ID_WRITE 0xb1acaf05	/* Item changed */
#define CLPNM_ID_DELETE 0xb1acac61	/* Item deleted */

/*
 *  Prototypes
 */

#ifndef _NO_PROTO
ULONG  	CLP_GetPrefsSize( ULONG, ULONG );
ULONG  	CLP_GetPrefs( ULONG, ULONG, APTR, ULONG, APTR );
ULONG  	CLP_SetPrefs( ULONG, ULONG, APTR, ULONG, APTR );
void  	CLP_DeletePrefs( ULONG, ULONG );
void  	CLP_DeletePrefsID( ULONG );
APTR  	CLP_AddNotify( ULONG , struct MsgPort * );
void  	CLP_RemNotify( APTR );
void  	CLP_ReplyNotify( struct CLPrefsNotifyMsg * );
ULONG  	CLP_NextSubID( ULONG, ULONG );
ULONG  	CLP_PrevSubID( ULONG, ULONG );
ULONG  	CLP_MaxSubID( ULONG );
STRPTR	CLP_GetSystemName( void );
STRPTR	CLP_GetSystemAddress( void );
void	CLP_MakeTempFileName( STRPTR namebuffer, LONG estimated_size );
LONG	CLP_GetUser( STRPTR username, User *userstruct, UserProfile *profilestruct );
LONG	CLP_SetUser( User *userstruct, UserProfile *profilestruct );
LONG	CLP_GetUserByIndex( ULONG index, User *userstruct, UserProfile *profilestruct );
void	CLP_AllocateUserlist( ULONG readwriteflg );
void	CLP_FreeUserlist( void );
LONG	CLP_NextUser( User *userstruct, UserProfile *profilestruct );
LONG	CLP_DelUser( STRPTR username, ULONG delpathflg );
ULONG	CLP_NumUsers( void );
void	CLP_RequestMailerSem( struct Message *replymsg );
void	CLP_FreeMailerSem( void );
void	CLP_RemoveMailerSem( struct Message *replymsg );
ULONG	CLP_PK_FindByStamp( APTR stamp, STRPTR idstore );
ULONG	CLP_PK_Unpack( STRPTR packerid, STRPTR arcname, STRPTR destdir );
ULONG	CLP_PK_PackA( STRPTR packerid, STRPTR arcname, STRPTR *files );
ULONG	CLP_PK_Pack( STRPTR packerid, STRPTR arcname, ... );
void	CLP_MakeSpoolName( STRPTR dest, STRPTR sysname, ULONG prespool );
int		CLP_RenameToSpool( STRPTR prespoolname );
void	CLP_MakeTrashName( STRPTR dest, STRPTR sysname, STRPTR oldfilename );
void	CLP_PK_AllocSem( void );
void	CLP_PK_FreeSem( void );
STRPTR	CLP_GetSystemNameLowerCase( void );
ULONG	CLP_PK_AttemptSem( void );
int		CLP_RenameToSpoolSysname( STRPTR prespoolname, STRPTR newsysname );
struct CLDBInfo * CLP_GetDBInfo( void );
void 	CLP_SaveDBInfo( void );
LONG	CLP_GetNumNewUsers( void );
STRPTR	CLP_GetGroupName( ULONG groupnum );
ULONG	CLP_SetGroup( APTR grouparray, ULONG groupnum );
ULONG	CLP_ClrGroup( APTR grouparray, ULONG groupnum );
ULONG	CLP_GetGroup( APTR grouparray, ULONG groupnum );
void	CLP_SaveGroupNames( void );
struct PortInfo *CLP_GetPort( ULONG portnum );
void	CLP_SavePorts( ULONG numports );
ULONG	CLP_GetNumPorts( void );
void	CLP_RegisterDBViewer( struct Task *sigtask, ULONG sigmask );
void	CLP_PK_RereadPacker( void );
STRPTR *CLP_PK_GetPackerList( void );
int		CLP_CallEditor( STRPTR filename );
ULONG   CLP_IsPasswordSet( struct User *user );
ULONG   CLP_TestPassword( struct User *user, char *passwordtotest );
void    CLP_SetPassword( struct User *user, char *newpassword );
#endif

/*
 *  SAS/C specific pragmas
 */

#ifndef _NO_PRAG
extern struct Library *CLPrefsBase;
#pragma libcall CLPrefsBase CLP_GetPrefs 1e 9281005
#pragma libcall CLPrefsBase CLP_SetPrefs 24 9281005
#pragma libcall CLPrefsBase CLP_GetPrefsSize 2a 1002
#pragma libcall CLPrefsBase CLP_DeletePrefs 30 1002
#pragma libcall CLPrefsBase CLP_DeletePrefsID 36 001
#pragma libcall CLPrefsBase CLP_AddNotify 3c 8002
#pragma libcall CLPrefsBase CLP_RemNotify 42 801
#pragma libcall CLPrefsBase CLP_ReplyNotify 48 801
#pragma libcall CLPrefsBase CLP_NextSubID 4e 0802
#pragma libcall CLPrefsBase CLP_PrevSubID 54 0802
#pragma libcall CLPrefsBase CLP_MaxSubID 5a 001
#pragma libcall CLPrefsBase CLP_GetSystemName 60 0
#pragma libcall CLPrefsBase CLP_GetSystemAddress 66 0
#pragma libcall CLPrefsBase CLP_MakeTempFileName 6c 0802
#pragma libcall CLPrefsBase CLP_GetUser 72 A9803
#pragma libcall CLPrefsBase CLP_GetUserByIndex 78 98003
#pragma libcall CLPrefsBase CLP_SetUser 7e 9802
#pragma libcall CLPrefsBase CLP_DelUser 84 0802
#pragma libcall CLPrefsBase CLP_NextUser 8a 9802
#pragma libcall CLPrefsBase CLP_NumUsers 90 0
#pragma libcall CLPrefsBase CLP_AllocateUserlist 96 001
#pragma libcall CLPrefsBase CLP_FreeUserlist 9c 0
#pragma libcall CLPrefsBase CLP_RequestMailerSem a2 801
#pragma libcall CLPrefsBase CLP_FreeMailerSem a8 0
#pragma libcall CLPrefsBase CLP_RemoveMailerSem ae 801
#pragma libcall CLPrefsBase CLP_PK_FindByStamp b4 9802
#pragma libcall CLPrefsBase CLP_PK_Unpack ba A9803
#pragma libcall CLPrefsBase CLP_PK_PackA c0 A9803
#pragma tagcall CLPrefsBase CLP_PK_Pack c0 A9803
#pragma libcall CLPrefsBase CLP_MakeSpoolName c6 09803
#pragma libcall CLPrefsBase CLP_RenameToSpool cc 801
#pragma libcall CLPrefsBase CLP_MakeTrashName d2 A9803
#pragma libcall CLPrefsBase CLP_PK_AllocSem d8 0
#pragma libcall CLPrefsBase CLP_PK_FreeSem de 0
#pragma libcall CLPrefsBase CLP_GetSystemNameLowerCase e4 0
#pragma libcall CLPrefsBase CLP_PK_AttemptSem ea 0
#pragma libcall CLPrefsBase CLP_RenameToSpoolSysname f0 9802
#pragma libcall CLPrefsBase CLP_GetDBInfo f6 0
#pragma libcall CLPrefsBase CLP_SaveDBInfo fc 0
#pragma libcall CLPrefsBase CLP_GetNumNewUsers 102 0
#pragma libcall CLPrefsBase CLP_GetGroupName 108 001
#pragma libcall CLPrefsBase CLP_SetGroup 10e 0802
#pragma libcall CLPrefsBase CLP_ClrGroup 114 0802
#pragma libcall CLPrefsBase CLP_GetGroup 11a 0802
#pragma libcall CLPrefsBase CLP_SaveGroupNames 120 0
#pragma libcall CLPrefsBase CLP_GetPort 126 001
#pragma libcall CLPrefsBase CLP_SavePorts 12c 001
#pragma libcall CLPrefsBase CLP_GetNumPorts 132 0
#pragma libcall CLPrefsBase CLP_RegisterDBViewer 138 0802
#pragma libcall CLPrefsBase CLP_PK_RereadPacker 13e 0
#pragma libcall CLPrefsBase CLP_PK_GetPackerList 144 0
#pragma libcall CLPrefsBase CLP_CallEditor 14a 801
#pragma libcall CLPrefsBase CLP_IsPasswordSet 156 801
#pragma libcall CLPrefsBase CLP_TestPassword 15c 9802
#pragma libcall CLPrefsBase CLP_SetPassword 162 9802
#endif

/*
 * Tags for supported datatypes
 */

#define TAGT_STANDARD			0				/* Vanilla structure */
#define TAGT_STRING				(1L<<31)		/* ZTS */
#define TAGT_LONG				(1L<<30)		/* Long */
#define TAGT_NOL				(1L<<29)		/* Pointer to Named object list */

#define TAGT_USERMASK			0x1fffffff		/* SubID-Mask */

/*
 *  Used by NextSubID(), PrevSubID(), MaxSubID() or in
 *  CLPrefsNotifyMsgs
 */

#define TAGT_INVALID			((ULONG)~0)		/* Invalid ID */

/*
 *		Return codes from CLP_PK_Unpack()
 *
 */

#define CLP_UNPK_OK 0
#define CLP_UNPK_PACKERNOTFOUND 1		/* Packer unknown */
#define CLP_UNPK_CANTOPENARC 2			/* Can't open archive file */
#define CLP_UNPK_RUNPACKER 3			/* Running packer executable failed */
#define CLP_UNPK_PACKERFAILED 4			/* Packer runs ok, but failed (probably archive corrupt) */
#define CLP_UNPK_NOCLUE 5				/* Can't recognize packer type */
#define CLP_UNPK_INVALIDDEST 6			/* Dest dir is invalid */
#define CLP_UNPK_NOTSUPPORTED 7			/* Unpack not supported */

#define CLP_PACK_OK 0
#define CLP_PACK_PACKERNOTFOUND 1		/* Packer unknown */
#define CLP_PACK_CANTOPENARC 2			/* Can't open archive file */
#define CLP_PACK_RUNPACKER 3			/* Running packer executable failed */
#define CLP_PACK_PACKERFAILED 4			/* Packer runs ok, but failed */
#define CLP_PACK_NOTSUPPORTED 5			/* pack not supported */
#define CLP_PACK_SINGLEFILE 6			/* Not an Archiver  */

/*
 *
 *		CLDBInfo - Informational structure about
 *      Msgs and System calls. Saved as Prefs/cldbinfo.clprefs
 *
 */

struct CLDBInfo {
	ULONG	today;				// Aktuelles Datum

	ULONG	msg_board;			// Board Mails
	ULONG	msg_priv;			// Private Mails
	ULONG	msg_error;			// Error Mails
	ULONG	msg_censored;		// Censored Mails
	ULONG	msg_dupes;			// Killed Dupes
	ULONG	msg_old;			// Killed old mails

	ULONG	calls;
	ULONG	calls_noconnect;
	ULONG	calls_nologin;
	ULONG	calls_netcall;

	ULONG	calls_today;
	ULONG	calls_today_noconnect;
	ULONG	calls_today_nologin;
	ULONG	calls_today_netcall;

	ULONG	calls_telnet, calls_today_telnet;
};

#endif
