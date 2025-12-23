#ifndef NOVIA_MISC_H
#define NOVIA_MISC_H
#define NOVIA_MISC_H_VERSION "$VER: 0.07 (09.06.1999)"
// (c) Copyright 1996-1999 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.
//V0.04 03.08.1998
//V0.05 11.08.1998
//V0.06 13.02.1999
//V0.07 09.06.1999

#define CMD_TYPE_INVALID		0
#define CMD_TYPE_INTERNAL		1
#define CMD_TYPE_DOS			2
#define CMD_TYPE_NOViA			3
#define CMD_TYPE_AREXX_NOVIA	4
#define CMD_TYPE_AREXX_CNET		5

#define __aligned
#define __asm
//#define __regargs
#define __saveds
//#define __stdargs
#define REG(r)    register __ ## r
#define GNUCREG(r)
#define SAVEDS
#define ASM
#define REGARGS
#define STDARGS
#define ALIGNED
/*
struct command {
	char 	name[32];		// 32
	char	comment[32];	// 64
	ULONG id;				// 68
	ULONG	accountlevel;	// 72
	ULONG	type;				// 76
	ULONG	reserverd;		// 80
	char	*filename;
	char	*args;
};

struct ext_command
{
	ext_command	*ln_Succ;
	ext_command	*ln_Pred;
	ULONG		id;				// same as command
	SHORT		type;			// viewer, soundplayer, archiver...
	char		Filename[128];
	char		Args[32];
};

struct int_command
{
	ULONG	id;
	SHORT	type;
	SHORT	reserved;
	ULONG	reserved2;
	char	name[32];
};
*/
struct mcicode
{
	UWORD number;    		// 2
	char  type;				// 3
	char  name[21];			// 24
	ULONG length;			// 28
	ULONG offset;			// 32
};

struct IOBuffer
{
	Node *ln_Succ;			// 4
	Node *ln_Pred;			// 8
	APTR data;				// 12 address of iodata
	ULONG data_len;			// 16 len of data
};

struct MenuEntry
{
	Node *ln_Succ;			// 4
	Node *ln_Pred;			// 8
	UWORD	menuNumber;		// 10
	UWORD	itemNumber;		// 12
};

struct MyMemEntry
{
	Node *ln_Succ;			// 4
	Node *ln_Pred;			// 8
	APTR address;			// 12
	ULONG length;			// 16
	char text[80];			// 96
};

struct MyMemList
{
	struct Node *	lh_Head;		// 4
	struct Node *	lh_Tail;		// 8
	struct Node *	lh_TailPred;	// 12
	ULONG			entrys;			// 16
};

struct HisEntry
{
	struct HisEntry *ln_Succ;
	struct HisEntry *ln_Pred;
	char buffer[80];
};

struct STR_Group						// New in V0.04
{
	char **table;
	UWORD entrys;
	UWORD maxid;
};

struct command_table					// New in V0.05
{
	ULONG  entrys;						// real entrys
	ULONG  tablesize;					// allocated entrys
	struct command **cmd;				// commandtable
};

struct rexxstring
{
	ULONG	IDNumber;
	ULONG	offset;
	UWORD	type;
	UWORD	length;
};

#define DATETYPE_DATE		0
#define DATETYPE_SPACE		1
#define DATETYPE_MONTH		2
#define DATETYPE_DAILY		3

#define MULTIPLICATOR_SECS	1
#define MULTIPLICATOR_MINS	60
#define MULTIPLICATOR_HOURS	3600
#define MULTIPLICATOR_DAYS	86400

struct EventRequest
{
	struct IORequest	tr_node;
	struct timeval		tr_time;
	APTR				event;			// pointer to event
};


struct Event							// New in V0.06
{
	struct Event	*ln_Succ;			// 4
	struct Event	*ln_Pred;			// 8
	ULONG			IDNumber;			// 12
	ULONG			IPNumber;			// 16
	char			Name[43];			// 59
	UBYTE			DateType;			// 60
	struct Date		BeginDate;			// 72		start execute Date/begin of interval (Time)
	struct Date		EndDate;			// 84		end of execute/end of interval (Time)
	struct Date		LastEvent;			// 96
	struct Date		NextEvent;			// 108
	ULONG			IntervalTime;		// 112
	UBYTE			UseInterval;		// 113
	UBYTE			weekday[7];			// 120
	UBYTE			month[12];			// 132
	UBYTE			Holiday;			// 133
	UBYTE			UseWeekday;			// 134
	UBYTE			Running;			// 135		TRUE = IORequest sended, FALSE = configuration ERROR;
	UBYTE			init;				// 136		event is correct init
	ULONG			CommandType;		// 140
	ULONG			EventType;			// 144
	UWORD			RunType;			// 146
	UBYTE			Program[80];		// 226
	UBYTE			Status;				// 227
	UBYTE			ClosePort;			// 228
	char			buf[100];			// 328 Display buffer for gui
	ULONG			Multiplicator;		// 332 Intervalmultiplicator 
	EventRequest	*eventreq;			// 336 pointer of EventRequest;
	UBYTE			res[68];			// 400
};

#endif


