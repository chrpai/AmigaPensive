/*
**	cl_misc.h - defines various preferences IDs (mostly internal)
*/

/*
** Temporary files 
*/

#define CLTEMP_PREFSID MAKE_ID('T','M','P','F')
#define CLTEMP_SUBID_FILENAME (TAGT_STRING|0)
#define CLTEMP_SUBID_MAXSIZE (TAGT_LONG|0)

/*
** Msg base
*/

#define CLMSGDB_PREFSID MAKE_ID('M','S','G','B')
#define CLMSGDB_SUBID_BOARD (TAGT_STRING|0)
#define CLMSGDB_SUBID_USER (TAGT_STRING|1)
#define CLMSGDB_SUBID_EXCLUDE_TEXT (TAGT_STRING|2)
#define CLMSGDB_SUBID_EXCLUDE_BIN (TAGT_STRING|3)

/*
** Offline-Editor
*/

#define CLEDIT_PREFSID MAKE_ID('E','D','I','T')
#define CLEDIT_SUBID_OFFLINEED (TAGT_STRING|0)

/*
** Userantrag
*/

#define CLUSER_PREFSID MAKE_ID('U','S','R','A')
#define CLUSER_SUBID_FILTERMASK 	(TAGT_STRING|0)
#define CLUSER_SUBID_USERUSER 		(TAGT_STRING|1)
#define CLUSER_SUBID_AUTOUSERNAME 	(TAGT_LONG|2)

/*
** Server
*/

#define CLSERVER_PREFSID MAKE_ID('S','E','R','V')
#define CLSERVER_SUBID_LOGTICK (TAGT_LONG|0)

/*
** IP
*/

#define CLIP_PREFSID MAKE_ID('T','C','P','I')
#define CLIP_SUBID_NUMTELNET (TAGT_LONG|0)	// Defaults to 4 (Number of Telnet-Ports)
