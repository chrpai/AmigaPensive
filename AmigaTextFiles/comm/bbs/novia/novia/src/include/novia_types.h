#ifndef NOVIA_TYPES_H
#define NOVIA_TYPES_H
#define NOVIA_TYPES_H_VERSION "$VER: 0.07 (09.10.1999)"
// (c) Copyright 1996-1999 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

// V0.06 (10.06.1998) (c) copyright 1996-1998 by Thorsten Gehler
/* V0.07 (09.10.1999) (c) copyright 1996-1999 by Thorsten Gehler

	- LOWLEVEL COMMANDS ADDED
*/

#define NOVIA_CLIENT			1
#define NOVIA_MAIN				2

#define CLIENT					1
#define MAIN					2

/*** get string/long/line etc. flags ***/

#define ESC_UP 					1
#define ESC_DOWN 				2
#define CURSER_BREAK 			4
#define BUFFERED				8
#define TAB_FORWARD				16
#define TAB_BACKWARD			32
#define TAB						(TAB_FORWARD|TAB_BACKWARD)
#define INPUT_TYPE_STRING		64
#define INPUT_TYPE_NUMBER		128
#define INPUT_TYPE_IP			256

/***************************************/

#define MAXARGLEN				50
#define MAXARGS					10

/*** AskKey flags ***/

#define FLAG_NO					1
#define FLAG_YES				2
#define FLAG_ALL				4
#define FLAG_CANCEL				8
#define FLAG_QUIT				16

/*** AskKey results ***/

#define RESULT_YES				1
#define RESULT_NO				2
#define RESULT_ALL				3
#define RESULT_CANCEL			4
#define RESULT_QUIT				5



/*** VDE - Type 1 ***/

#define VDE_BYTE				1
#define VDE_WORD				2
#define VDE_LONG				4
#define VDE_STRING				5
#define VDE_DATE				6
#define VDE_BOOL				7
#define VDE_ITEMSORT			8
#define VDE_FILEACTION			9
#define VDE_SHOWNAME			10
#define VDE_PURGE				11
#define VDE_GENDER				12
#define VDE_NETWORK				13
#define VDE_BUFFERTYPE			14
#define VDE_SEX					20
#define VDE_LANGUAGE			21
#define VDE_TEXTTYPE			22
#define VDE_MOREMODE			23
#define VDE_TIMEFORMAT			24
#define VDE_DATEFORMAT			25
#define VDE_TERMLINEFEED 		26
#define VDE_TERMTABS			27
#define VDE_TERMANSI			28
#define VDE_TERMCOLOR			29
#define VDE_TERMTYPE			30
#define VDE_COMPTYPE			31
#define VDE_HELPLEVEL			32
#define VDE_PROTOCOL			33
#define VDE_PAUSING				34
#define VDE_YANKHIDE			35
#define VDE_AUTOHIDE			36
#define VDE_SCREENMODE			37
#define VDE_FONT				38
#define VDE_IP					39

#define VDE_NULL				0

/* Type 2*/
#define VDE_EDITMEMBERS			200
#define	VDE_TEXTONLY			248
#define	VDE_READONLY			249
#define	VDE_READWRITE			250
#define	VDE_LOAD				251
#define VDE_SAVE				252
#define VDE_QUIT				253
#define VDE_KILL				254
#define VDE_SUBMENU				255

/* Type of VDE */
#define VDEID_USER				1
#define VDEID_SUBBOARD			2
#define VDEID_SUBDIR			3
#define VDEID_VOTE				4
#define VDEID_MAINPORTCONFIG	5
#define VDEID_CLIENTCONFIG		6
#define VDEID_NRC				7
#define VDEID_MEMBER			20

/* General Types */


#define TYPE_BYTE				1
#define TYPE_WORD				2
#define TYPE_LONG				4
#define TYPE_STRING				5
#define TYPE_DATE				6
#define TYPE_BOOL				7
#define TYPE_ITEMSORT			8
#define TYPE_FILEACTION			9
#define TYPE_SHOWNAME			10
#define TYPE_PURGE				11
#define TYPE_GENDER				12
#define TYPE_NETWORK			13
#define TYPE_BUFFERTYPE			14
#define TYPE_SEX				20
#define TYPE_LANGUAGE			21
#define TYPE_TEXTTYPE			22
#define TYPE_MOREMODE			23
#define TYPE_TIMEFORMAT			24
#define TYPE_DATEFORMAT			25
#define TYPE_TERMLINEFEED		26
#define TYPE_TERMTABS			27
#define TYPE_TERMANSI			28
#define TYPE_TERMCOLOR			29
#define TYPE_TERMTYPE			30
#define TYPE_COMPTYPE			31
#define TYPE_HELPLEVEL			32
#define TYPE_PROTOCOL			33
#define TYPE_PAUSING			34
#define TYPE_YANKHIDE			35
#define TYPE_AUTOHIDE			36
#define TYPE_YANKARC			37
#define TYPE_YANKMETHOD			38
#define TYPE_DEFARC				39
#define TYPE_USERNUMBER			40

#define TYPE_NULL				0

// V0.07 Low Level Commands

#define LOWLEVEL_CMD_MAKE_FILESYSTEM	20001
#define LOWLEVEL_CMD_MAKE_ROOT			20002
#define LOWLEVEL_CMD_ADD_DIRECTORY		20003
#define LOWLEVEL_CMD_ADD_LINK				20004
#define LOWLEVEL_CMD_ADD_COMMAND			20005


#define CMD_LOGIN				1
#define CMD_LOGOFF				2
#define CMD_RELOGIN				3
#define CMD_USERINFO			4
#define CMD_STATUS				5
#define CMD_VIEWNEW				6
#define CMD_VIEWFEEDBACK		7

#define CMD_QUIT				8
#define CMD_BROWSE				9
#define CMD_NEXT				10
#define CMD_BACK				11
#define CMD_PREVIEW				12
#define CMD_LIST				13
#define CMD_SAVE				14
#define CMD_YES					15
#define CMD_NO					16

#define CMD_WRITEFILE			17
#define CMD_DELFILE				18
#define CMD_EDITFILE			19
#define CMD_COPYFILE			20

#define CMD_EDITACCOUNT			21
#define CMD_EDITGROUP			22
#define CMD_ACCOUNT_DEL			23
#define CMD_EDITACCOUNTCOMMENT	24

#define CMD_OPENLOG				25
#define CMD_CLOSELOG			26
#define CMD_ACTIFITY_MONITOR	27
#define CMD_LOG_CALL			28
#define CMD_LOG_TRANSFER		29
#define CMD_LOG_OPEN			30
#define CMD_LOG_AMAINT			31

#define CMD_SETPORTPRI			32
#define CMD_PORTOPEN			33
#define CMD_PORTCLOSE			34
#define CMD_REMAINDER			35

#define CMD_OLM					36
#define CMD_MEGAOLM				37
#define CMD_FILEOLM				38
#define CMD_CHAT				39
#define CMD_SYSOPCHAT			40

#define CMD_SENDMAIL			41
#define CMD_READMAIL			42
#define CMD_EDITMAIL			43
#define CMD_FORWARDMAIL			44
#define CMD_FEEDBACK			45

#define CMD_SETTING_TERM		46
#define CMD_SETTING_USER		47
#define CMD_SETTING_GLOBAL		48
#define CMD_SETTING_MISC		49
#define CMD_SETTING_PREFS		50
#define CMD_SETTING_COSTS		51

#define CMD_DOWNLOAD_DIRECT		52
#define CMD_DOWNLOAD_SELECT		53

#define CMD_HELP				54
#define CMD_TIME				55
#define CMD_INFO				56
#define CMD_WHO					57
#define CMD WHOIS				58

#define CMD_NEWUSER				59
#define CMD_CHAGEPW				60
#define CMD_PORTMONITOR			61
#define CMD_ACTIVITYGRAPH		62
#define CMD_STATISTIC			63
#define CMD_HIDE				64
#define CMD_MUFFLE				65
#define CMD_SLEEP				66
#define CMD_WHY					68
#define CMD_IDENTIFY			69
#define CMD_EDITFINGER			70
#define CMD_COSTS				71
#define CMD_XPERT				72
#define CMD_EDITCOMMENT			73
#define CMD_PARTYKAL			74
#define CMD_STERNUL				75
#define CMD_TOPTOOLS			76
#define CMD_NEWS				77
#define CMD_DOWNLOAD			78
#define CMD_SHELL				79
#define CMD_PING				80
#define CMD_VOTE				81
#define CMD_CALLBACK			82
#define CMD_DIALOUT				83
#define CMD_TERMINAL			84
#define CMD_SCANNEW				85
#define CMD_YANK				86
#define CMD_READNEW				87
#define CMD_DELNEW				88
#define CMD_YANKGLOBAL			89
#define CMD_YANKLOCAL			90
#define CMD_READGLOBAL			91
#define CMD_VIEWFILE			92
#define CMD_EXAMINE				93
#define CMD_SETFLAGS			94
#define CMD_GRAB				95
#define CMD_JOIN				96
#define CMD_DIR					97
#define CMD_SCAN				98
#define CMD_UPLOAD				99
#define CMD_VALIDATE			101
#define CMD_ADOPT_ORPHANS		102
#define CMD_EDITENTRYMSG		103
#define CMD_ATRIBUTE			104
#define CMD_TRANSFORM			105
#define CMD_CD					106
#define CMD_PARRENT				107
#define CMD_MEMBERS				108
#define CMD_SELECT				109
#define CMD_POST				110
#define CMD_ADD					111
#define CMD_DROP				112
#define CMD_CONTINUE			113

#define CMD_SUBBOARD_NEW		114
#define CMD_SUBBOARD_EDIT		115
#define CMD_SUBBOARD_DEL		116

#define CMD_MAILVERIFY			117
#define CMD_SHOWUSERLIST		118

#define CMD_GO					119
#define CMD_BASE				121
#define CMD_NEWPORT				122
#define CMD_CLOSEPORT			123

#define CMD_RESETBBS			124
#define CMD_PORTARG				125
#define CMD_PORTREMOVE			126

#define CMD_EDITCONFIG			127
#define CMD_EDITCLIENT			128
#define CMD_CLOSEGUI			129
#define CMD_LOADGUI				130
#define CMD_DEL					131
#define CMD_EDIT				132
#define CMD_MOVE				133

#define CMD_REPLY				134
#define CMD_RESPONSE			135

#define CMD_OPENCHAT			140
#define CMD_TEXT				150
#define CMD_REPAIR				151
#define CMD_ADDMEMBER			152
#define CMD_DELMEMBER			153
#define CMD_ADDGROUP			154
#define CMD_DELGROUP			155
#define CMD_LISTMEMBERS			156
#define CMD_CONFIG				157
#define CMD_VIEWSELECTLIST		158
#define CMD_KILL				160
#define CMD_RENAME				161

#define MSG_GETMAINPORT			1
#define MSG_GETLOCALPORT		2
#define MSG_INITPORT			3
#define MSG_PORT_REMOVE			4			// Mainport beginn to closing port.
#define MSG_PORT_OPEN			5
#define MSG_PORT_OFFLINE		6
#define MSG_PORT_CLOSE			7			// Port close himselve
#define MSG_PORT_LOCK			9
#define MSG_PORT_PING			10
#define MSG_PORT_CHANGE			11
#define MSG_PORT_LOSTCARRIER	12			// New in V0.06
#define MSG_PORT_LOGOFF			13			// New in V0.06
#define MSG_PORT_CID			14			// New in V0.06 ConnectID (for Telnet logins)
#define MSG_PORT_RING			15

#define MSG_PORT_TOFRONT		50
#define MSG_PORT_CLOSEWINDOW	51
#define MSG_PORT_LOCKCONSULE	52
#define MSG_ABOUT				53

#define MSG_CHAT_BREAK			60

#define MSG_PING				70

#define MSG_SYSTEMINFO			100
#define MSG_COMMAND				101

#define MSG_TEXT				200

#define MSG_RETURN_ERROR		1000
#define MSG_RETURN_TIMEOUT		1001

#define MSG_REPLY				10000
#define MSG_REPLY_PORTCLOSED	10001	
#define MSG_REPLY_INITPORT		10002
#define MSG_REPLY_PORTOFFLINE	10005	
#define MSG_REPLY_INITPORT_FAIL	10010

#define MSG_REPLY_PORTCLOSEFAILAT	100050
#define MSG_REPLY_PORTCLOSEOK		100100

#define MSG_NREXX_START			20000
#define MSG_NREXX_END			20010
#define MSG_NREXX_OK			20020
#define MSG_NREXX_FAILAT		20050

#define MAX_NETWORK				7

#define NETWORKTYPE_LOCAL		0			// Port are running on the same machine
#define NETWORKTYPE_SERIAL		1			// Port are running on a other machine with a serial-cable
#define NETWORKTYPE_PARALLEL	2			// Port are running on a other machine with a parallel-cable
#define NETWORKTYPE_TCP			3			// Port are running on a other machine with TCP-IP connection
#define NETWORKTYPE_ENVOY		4			// Port are running on a other machine with EnvoyNetware
#define NETWORKTYPE_IPX			5			// Port are running on a other machine with NovellNetware
#define NETWORKTYPE_MSN			6			// Port are runninf on a other machine with Microsoft network.
#define NETWORKTYPE_NET			7			// Port are running on a other machine with a other network

#define PORT_TYPE_LOCAL			0			// Port are running on the same machine
#define PORT_TYPE_NETSER		1			// Port are running on a other machine with a serial-cable
#define PORT_TYPE_NETPAR		2			// Port are running on a other machine with a parallel-cable
#define PORT_TYPE_NETTCPIP		3			// Port are running on a other machine with TCP-IP connection
#define PORT_TYPE_ENVOY			4			// Port are running on a other machine with EnvoyNetware
#define PORT_TYPE_NETNOVELL		5			// Port are running on a other machine with NovellNetware
#define PORT_TYPE_NETMICROSOFT	6			// Port are runninf on a other machine with Microsoft network.
#define PORT_TYPE_NET			7			// Port are running on a other machine with a other network

#define PORT_STATUS_WAITLOAD	-1			// No calls, no systemmaintance
#define PORT_STATUS_CLOSE		0			// No calls, no systemmaintance
#define PORT_STATUS_OFFLINE		1			// port is closed when a systemmaintance are running
#define PORT_STATUS_WAITCALL	2			// Waiting for a call
#define PORT_STATUS_CONNECT 	3			// Call detected, waiting for carrier.
#define PORT_STATUS_LOGIN		4			// Starting Login. Carrier detected.
#define PORT_STATUS_ONLINE		5			// User is online.

#define MAINPORT				1
#define CLIENTPORT				2

#define VISUALEDIT_FILE				1
#define VISUALEDIT_EXITIFNOTEXIT 2
#define VISUALEDIT_READONLY		4
#define VISUALEDIT_FROMADDRESS	8

#define ERROR_OUT_OF_MEMORY						1000
#define ERROR_INVALID_OBJECT_NAME				1001
#define ERROR_INVALID_OBJECT_TYPE				1002
#define ERROR_NOT_A_NOVIA_DOS_PROGRAMM			1003
#define ERROR_CANT_START_FROM_MAINTASK_ONLY	1004
#define ERROR_ACCESS_DENIED						1005
#define ERROR_GUI_NOT_OPENED						1006
#define ERROR_GUI_ALREADY_OPENED					1007
#define ERROR_X_ENVELOPE_TO_NOT_FOUND			1008
#define ERROR_USER_NOT_FOUND						1009

#endif // NOVIA_TYPES_H