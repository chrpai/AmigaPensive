#ifndef NOVIA_NSL_H
#define NOVIA_NSL_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define NSL_RESULT_FALSE					0
#define NSL_RESULT_TRUE						1

#define NSL_TYPE_NOTDEFINED				0

#define NSL_TYPE_ACCESSGROUP				101		/* structure entry (se) types */
#define NSL_TYPE_ANSICOLORS				102
#define NSL_TYPE_ANSIMODE					103
#define NSL_TYPE_ANSITABS					104
#define NSL_TYPE_ARCMETHOD					105
#define NSL_TYPE_BOOL						106
#define NSL_TYPE_CHARSET					107
#define NSL_TYPE_COMPUTERTYPE				108
#define NSL_TYPE_COUNTRY					109
#define NSL_TYPE_DATE						110
#define NSL_TYPE_DIRECTORYPATH			111
#define NSL_TYPE_EOLSEQUENCE				112
#define NSL_TYPE_GENDER						113
#define NSL_TYPE_HELPLEVEL					114
#define NSL_TYPE_LANGUAGE					115
#define NSL_TYPE_LINEFEEDS					116
#define NSL_TYPE_MOREMODE					117
#define NSL_TYPE_PHONENUMBER				118
#define NSL_TYPE_PHONEVERIFICATION		119
#define NSL_TYPE_PROTOCOL					120
#define NSL_TYPE_STRING						121
#define NSL_TYPE_TIMEFORMAT				122
#define NSL_TYPE_TIMEZONE					123
#define NSL_TYPE_UBYTE						124
#define NSL_TYPE_ULONG						125
#define NSL_TYPE_USERID						126
#define NSL_TYPE_UWORD						127
#define NSL_TYPE_GROUPBOOL					128
#define NSL_TYPE_DATEFORMAT				129
#define NSL_TYPE_FILENAME					130
#define NSL_TYPE_DOMAIN						140
#define NSL_TYPE_INETADDRESS				141
#define NSL_TYPE_INETSERVICEPORT			142
#define NSL_TYPE_INETUSERLIST				143
#define NSL_TYPE_INETSERVERSTATUS		144
#define NSL_TYPE_INETMAILUSERTYPE		145
#define NSL_TYPE_INETUSERNAME				146
#define NSL_TYPE_IPADDRESS					147
#define NSL_TYPE_SCREENMODE				148
#define NSL_TYPE_FONTNAME					149
#define NSL_TYPE_CLIENTLIST				150
#define NSL_TYPE_LOCALMAILSERVERLIST	151
#define NSL_TYPE_POP3SERVERLIST			152
#define NSL_TYPE_EVENTLIST					153
#define NSL_TYPE_USERIP						154
#define NSL_TYPE_ITEMTYPE					155
#define NSL_TYPE_PROTECTION				156

#define NSL_TYPE_NULL						500	/* nsl menu types */
#define NSL_TYPE_LABEL						501
#define NSL_TYPE_MENU						502						
#define NSL_TYPE_COMMENT					503
#define NSL_TYPE_FIELD						504
#define NSL_TYPE_TEXT						505
#define NSL_TYPE_LINK						506
#define NSL_TYPE_RETURN						507
#define NSL_TYPE_PROCEDURE					508
#define NSL_TYPE_ARGSTRING					550
#define NSL_TYPE_STRUCT						600

#define NSLCOMMAND_UNDEFINED				1000	/* nsl commands */
#define NSLCOMMAND_PRINT					1001
#define NSLCOMMAND_EXIT						1002
#define NSLCOMMAND_MENU						1003
#define NSLCOMMAND_JUMP						1004
#define NSLCOMMAND_RETURN					1005
#define NSLCOMMAND_PROCEDURE				1006
#define NSLCOMMAND_IF						1007
#define NSLCOMMAND_THEN						1008
#define NSLCOMMAND_ELSE						1009

#define NSLCOMMAND_LOADUSER				1010
#define NSLCOMMAND_LOADGROUP				1011
#define NSLCOMMAND_LOADMAINCONFIG		1012
#define NSLCOMMAND_SAVEUSER				1013

#define NSLCOMMAND_LOADCOMMANDENTRY		1020
#define NSLCOMMAND_SAVECOMMANDENTRY		1021

#define NSL_MENUTAG_COLS					2000
#define NSL_MENUTAG_AUTOFORMAT			2001

#define NSL_SLID_USER						10000
#define NSL_SLID_MAINCONFIG				10001
#define NSL_SLID_INETCONFIG				10002
#define NSL_SLID_LOCALMAILSERVER			10003
#define NSL_SLID_POP3SERVER				10004
#define NSL_SLID_INETMAILUSER				10005
#define NSL_SLID_COMMANDENTRY				10006
#define NSL_SLID_ITEMENTRY					10007
#define NSL_SLID_MESSAGE					10008
#define NSL_SLID_FILE						10009
#define NSL_SLID_EMAIL						10010
#define NSL_SLID_DIRECTORY					10011

struct StructureList
{
	struct StructureList	*ln_Succ;
	struct StructureList	*ln_Pred;
	char						sl_name[40];
	ULONG						sl_id;
	struct MinList			se_list;
};

struct StructureEntry
{
	struct	StructureEntry *ln_Succ;
	struct	StructureEntry *ln_Pred;
	char		se_name[40];
	ULONG		se_offset;
	ULONG		se_length;
	ULONG		se_type;
};

struct NSLType
{
	char	name[40];
	ULONG	id;
};

struct NSLVariable
{
	struct NSLVariable *ln_Succ;
	struct NSLVariable *ln_Pred;
	char	*name;
	ULONG nsltype;
	struct StructureList *sl;
	ULONG	data;
	ULONG result;
};


struct NSLMenuItem
{
	NSLMenuItem	*ln_Succ;			// 
	NSLMenuItem	*ln_Pred;			// 
	NSLMenuItem *ln_right;			//
	NSLMenuItem *ln_left;			//
	char			*title;				//
	ULONG 		nsltype;				//
	NSLVariable	*var;					// variable
	ULONG			se_offset;			//	Offset in structure
	ULONG			se_length;			// length of 
	ULONG			se_type;				//
	UBYTE			X;						//
	UBYTE			Y;						//
	UBYTE			titlelen;			//
	UBYTE			fieldlen;			//
};

struct NSLMenu
{
	MinList itemlist;
	UWORD colums;
	UWORD	entrys;
	UWORD lines;
	UWORD	abstand;
	char  *title;
	char  *head;
	char  headlength;
	NSLMenuItem *last_item;
	struct NSLVariable *var;
};




#endif










