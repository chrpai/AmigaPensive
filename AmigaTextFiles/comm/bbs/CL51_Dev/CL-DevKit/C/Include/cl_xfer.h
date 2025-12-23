#ifndef _CL_XFER_H
#define _CL_XFER_H

/*
**   cl_xfer.h 5.0
**   -------------
**
**   Connectline File Transfer Protocol Driver definitions
**  
*/

#include <time.h>
#include <cl/cl_prefs.h>

extern struct Library *CLXFERBase;

#ifndef _NO_PRAGMAS
#pragma libcall CLXFERBase CLXFER_Info 1e 0
#pragma libcall CLXFERBase CLXFER_FreeList 24 801
#pragma libcall CLXFERBase CLXFER_ResetPrefs 2a 0
#pragma libcall CLXFERBase CLXFER_Transfer 30 BA98005
#pragma tagcall CLXFERBase CLXFER_TransferTags 30 BA98005
#pragma libcall CLXFERBase CLXFER_AllocateSendfilelist 36 801
#pragma tagcall CLXFERBase CLXFER_AllocateSendfilelistTags 36 801

#pragma libcall CLXFERBase CLXFER_SER_Open 3c 943210807
#pragma tagcall CLXFERBase CLXFER_SER_OpenTags 3c 943210807
#pragma libcall CLXFERBase CLXFER_SER_Close 42 801
#pragma libcall CLXFERBase CLXFER_SER_SetParams 48 3210805
#pragma libcall CLXFERBase CLXFER_SER_Write 4e 09803
#pragma libcall CLXFERBase CLXFER_SER_Query 54 801
#pragma libcall CLXFERBase CLXFER_SER_CD 5a 801
#pragma libcall CLXFERBase CLXFER_SER_Clear 60 801
#pragma libcall CLXFERBase CLXFER_SER_Flush 66 801
#pragma libcall CLXFERBase CLXFER_SER_Inhibit 6c 0802
#pragma libcall CLXFERBase CLXFER_SER_WaitMask 72 801
#pragma libcall CLXFERBase CLXFER_SER_Read 78 A2109806
#pragma libcall CLXFERBase CLXFER_SER_DropDTR 7e 0802
#pragma libcall CLXFERBase CLXFER_SER_TempClose 84 0802
#pragma libcall CLXFERBase CLXFER_SER_SendModemCmd 8a 109804
#pragma libcall CLXFERBase CLXFER_SER_Converse 90 BA910806
#pragma tagcall CLXFERBase CLXFER_SER_ConverseTags 90 BA910806
#pragma libcall CLXFERBase CLXFER_SER_SetCheckCD 96 0802
#pragma libcall CLXFERBase CLXFER_SER_Gets 9c A109805
#pragma libcall CLXFERBase CLXFER_SER_GetLineRate a2 801
#pragma libcall CLXFERBase CLXFER_SER_SetLineRate a8 0802
#pragma libcall CLXFERBase CLXFER_SER_Transfer ae BA9804
#pragma tagcall CLXFERBase CLXFER_SER_TransferTags ae BA9804
#pragma libcall CLXFERBase CLXFER_SER_Port_Hangup b4 9802
#pragma libcall CLXFERBase CLXFER_SER_Port_Dial ba 10BA9806
#pragma libcall CLXFERBase CLXFER_SER_SetConObject c0 9802
#pragma libcall CLXFERBase CLXFER_SER_SetConWriteObject c6 9802
#pragma libcall CLXFERBase CLXFER_GetProtoList cc 0
#pragma libcall CLXFERBase CLXFER_GetProtoNameIndex d2 801
#pragma libcall CLXFERBase CLXFER_GetProtoComment d8 801

struct List * CLXFER_Info( void );
void CLXFER_FreeList( struct List *list );
void CLXFER_ResetPrefs( void );
struct List * CLXFER_Transfer( STRPTR protoname, struct IOExtSer *ioread, struct IOExtSer *iowrite, struct List *sendfilelist, struct TagItem *tags );
struct List * CLXFER_TransferTags( STRPTR protoname, struct IOExtSer *ioread, struct IOExtSer *iowrite, struct List *sendfilelist, ... );
struct List * CLXFER_AllocateSendfilelist( STRPTR *files );
struct List * CLXFER_AllocateSendfilelistTags( ... );

void CLXFER_SER_Close( struct clxfer_serialhandle * );
struct clxfer_serialhandle * CLXFER_SER_Open( char *, ULONG, ULONG, ULONG, ULONG, ULONG, struct TagItem * );
struct clxfer_serialhandle * CLXFER_SER_OpenTags( char *, ULONG, ULONG, ULONG, ULONG, ULONG, ... );
LONG CLXFER_SER_SetParams( struct clxfer_serialhandle *, ULONG, ULONG, ULONG, ULONG );
LONG CLXFER_SER_Write( struct clxfer_serialhandle *, APTR, LONG );
LONG CLXFER_SER_Query( struct clxfer_serialhandle * );
LONG CLXFER_SER_CD( struct clxfer_serialhandle * );
void CLXFER_SER_Clear( struct clxfer_serialhandle * );
void CLXFER_SER_Flush( struct clxfer_serialhandle * );
LONG CLXFER_SER_Inhibit( struct clxfer_serialhandle *, ULONG );
LONG CLXFER_SER_TempClose( struct clxfer_serialhandle *, ULONG );
ULONG CLXFER_SER_WaitMask( struct clxfer_serialhandle * );
LONG CLXFER_SER_Read( struct clxfer_serialhandle *, UBYTE *, ULONG, LONG, ULONG, ULONG * );
LONG CLXFER_SER_DropDTR( struct clxfer_serialhandle *, ULONG );
LONG CLXFER_SER_SendModemCmd( struct clxfer_serialhandle *, STRPTR, ULONG, ULONG );
LONG CLXFER_SER_Converse( struct clxfer_serialhandle *, ULONG timeout, ULONG modemdelay, APTR muiapp, ULONG *checkabortflag, APTR converselist );
LONG CLXFER_SER_ConverseTags( struct clxfer_serialhandle *, ULONG timeout, ULONG modemdelay, APTR muiapp, ULONG *checkabortflag, ... );
void CLXFER_SER_SetCheckCD( struct clxfer_serialhandle *, ULONG checkcarrier );
ULONG CLXFER_SER_GetLineRate( struct clxfer_serialhandle * );
ULONG CLXFER_SER_SetLineRate( struct clxfer_serialhandle *, ULONG linerate );
LONG CLXFER_SER_Gets( struct clxfer_serialhandle *, STRPTR, ULONG, ULONG, APTR );
struct List * CLXFER_SER_Transfer( STRPTR protoname, struct clxfer_serialhandle *, struct List *sendfilelist, struct TagItem *tags );
struct List * CLXFER_SER_TransferTags( STRPTR protoname, struct clxfer_serialhandle *, struct List *sendfilelist, ... );

ULONG CLXFER_SER_Port_Hangup( struct clxfer_serialhandle *, struct PortInfo *portinfo );
ULONG CLXFER_SER_Port_Dial( struct clxfer_serialhandle *, struct PortInfo *portinfo, STRPTR phone, STRPTR modemmsgbuffer, APTR muiapp, ULONG *abortflagptr );

void CLXFER_SER_SetConObject( struct clxfer_serialhandle *, APTR clmuiconobj );
void CLXFER_SER_SetConWriteObject( struct clxfer_serialhandle *, APTR clmuiconobj );

STRPTR * CLXFER_GetProtoList( void );
LONG CLXFER_GetProtoNameIndex( STRPTR protoname );
STRPTR CLXFER_GetProtoComment( STRPTR protoname );

#endif

//
// Eine Exec-Liste mit diesen Nodes beschreibt die
// vorhanden Protokollbibliotheken.
//

typedef struct clxfer_info {
  struct Node    n;       		// Exec-List-Node
  char        name[ 30 ];   	// Name
  char        desc[ 128 ];  	// Beschreibung
  ULONG        flags;     		// s.u.
  char		  template[ 80 ];	// Parameter-Template
} clxfer_info;

#define CLXFERF_BIDIR  1        // Bidirektionales Protokoll
#define CLXFERF_RESUME 2        // Resume möglich

//
// Eine Exec-Liste mit diesen Nodes beschreibt die
// zu verschickenden Files.
//

typedef struct clxfer_sendfile {
  struct MinNode n;         	// Exec-Node-Verkettung
  UBYTE      filename[ 256 ];  	// Voller Name des Files
  ULONG      offset;       		// Offset in der Datei. Default 0
  ULONG      len;        		// zu sendende Länge. Bei 0 komplette Datei
  UBYTE      sendname[ 32 ];   	// Name der Datei auf der Gegenseite 
  int        success;      		// Datei erfolgreich übertragen?
  time_t     time;        		// benötigte Zeit
  ULONG      userdata;			// For use by user
} clxfer_sendfile;

//
// Eine Exec-Liste mit folgenden Nodes gibt CLXFER_Transfer()
// zurück. Ist diese Liste leer, wurden keine Dateien empfangen.
// 

typedef struct clxfer_recfile {
  struct MinNode     n;     	// Exec-Node-Verkettung
  struct FileInfoBlock  fib;    // Kopie des FileInfoBlocks
  int           success;  		// Übertragung erfolgreich
  time_t         time;    		// benötigte Zeit
} clxfer_recfile;


//
// Tags für CLXFER_Transfer()
//

#define CLXFER_TAGBASE (TAG_USER+0x19749)
#define CLXFER_RecDir (CLXFER_TAGBASE+1)  // STRPTR: Verzeichnis, in das
                      // empfangen werden soll
#define CLXFER_DoRec (CLXFER_TAGBASE+2)   // BOOL: Auch empfangen.
                      // Führt bei unidirektionalen
                      // Protokollen zu zwei
                      // Durchgängen: Erst Senden,
                      // dann empfangen.
#define CLXFER_RecFirst (CLXFER_TAGBASE+3) // BOOL: Erst empfangen, dann
                      // senden. Ignoriert bei
                      // Bi-Protokollen.
#define CLXFER_CheckCD (CLXFER_TAGBASE+4)  // BOOL: Carrier prüfen.
                      // Default TRUE!
#define CLXFER_MUIApp (CLXFER_TAGBASE+5)  // MUI-Application-Objekt.
#define CLXFER_Hook (CLXFER_TAGBASE+6)   // HOOK: Wird für MUI-Ids
                      // und Signals aufgerufen.
#define CLXFER_Signals (CLXFER_TAGBASE+7)  // ULONG: App-Signale.
#define CLXFER_Args (CLXFER_TAGBASE+8)   // STRPTR: zusätzliche
                      // Argumente.
#define CLXFER_MUIWinID (CLXFER_TAGBASE+9) // MUIA_Window_ID für
                      // Transfer-Fenster.
#define CLXFER_LineBaud (CLXFER_TAGBASE+10) // Real line baud rate (as opposed to serial DTE rate)

#define CLXFER_Username (CLXFER_TAGBASE+11) // Username

//
// Serial-Support-Funktionen
//
// This structure is *PRIVATE* -- touch it and die!
//

typedef struct clxfer_serialhandle {
	struct IOExtSer  *readreq, *writereq;
	struct	timerequest *timereq;
	struct MsgPort   *readport, *writeport, *timerport;
	STRPTR       		devicename;
	ULONG        		deviceunit;
	ULONG        		serflags;
	ULONG				serbaud;
	ULONG				serrbuf;
	ULONG				linerate;
	APTR				conobj;			/* cl_console.mui object */
	APTR				conwriteobj;	/* cl_console.mui object */

	UBYTE				readbuffer[ 1 ];
	UBYTE				timeropen;
	UBYTE				seropen;
	UBYTE				readq, timeq;
	UBYTE				checkcd;	/* Flag -- automatic carrier check */
	UBYTE				inhibit, closed;

} clxfer_serialhandle;

typedef struct clxfer_converse {
	STRPTR		waitforwhat;		/* String to wait for (max. 127 chars). NULL terminates array */
	ULONG		action;				/* Action to take if what appeared */
	APTR		actiondata;			/* Data for action */
} clxfer_converse;

#define	CLXFE_SERCONVACT_EXIT		0 	/* Exit conversation, return data */
#define CLXFE_SERCONVACT_SEND		1	/* Send data as string */
#define CLXFE_SERCONVACT_SENDMODEM	2	/* Send data as Modem command */

#define CLXFE_SERCONVFLG_CASE		(1L<<31)	/* Flag: "what" is case-sensitive */
#define CLXFE_SERCONVFLG_CR			(1L<<30)	/* Flag: append CR on SEND */
#define CLXFE_SERCONVFLG_WAIT		(1L<<29)	/* Flag: react only after 1 second pause */

//
//	Flags for CLXFER_SER_Gets() (ored into maxlen)
//

#define CLXFER_GETS_ECHO (1L<<30)
#define CLXFER_GETS_ECHO_SECRET (1L<<29)


//
//	Tags für CLXFER_SER_OpenHandle()
//

/*
**	Error returns
*/

#define CLXFE_SER_INHIBITED (-2)		/* Channel is inhibited */
#define CLXFE_SER_NOCARRIER	(-3)		/* Carrier lost */
#define CLXFE_SER_TEMPCLOSED (-4)		/* Channel is temporarly closed */
#define CLXFE_SER_DTRDROPREOPEN (-5)	/* Reopen after DTR Drop failed! */

//
// Prefs
//

#define CLXFER_PREFSID MAKE_ID('O','X','F','E')
#define CLXFER_SUBID_SETTINGS (TAGT_NOL|0)

typedef struct clxfer_prefs {
  char        name[ 12 ];     // Name für den User, keine Leerzeichen
  char        comment[ 64 ];   // Kommentar für den User
  char        driverlib[ 20 ];  // clxfer_#?
  char        args[ 256 ];    // special args
} clxfer_prefs;

#endif
