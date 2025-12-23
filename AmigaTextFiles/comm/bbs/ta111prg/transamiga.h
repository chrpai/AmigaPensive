#ifndef TRANSAMIGA_H
#define TRANSAMIGA_H
/*
**  $Filename: transamiga.h $
**  $Release: v1.11 $
**  $Revision: 2.2 $
**  $Date: 92/03/19 $
**
**  TransAmiga BBS custom library
**
**  (C) Copyright 1991-1993 Timothy J. Aston
**	    All Rights Reserved
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef	EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

#ifndef	DEVICES_SERIAL_H
#include <devices/serial.h>
#endif

#ifndef	GRAPHICS_SPRITE_H
#include <graphics/sprite.h>
#endif

#ifndef	INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif

#ifndef	INTUITION_SCREENS_H
#include <intuition/screens.h>
#endif

#define TRANSAMIGA_NAME		"transamiga.library"
#define TRANSAMIGA_VERSION   0L

/* The library base structure.  At the moment, this is strictly private,
 * althought some parts may be made public later on.
 */

struct TransBase {
	struct Library tr_Lib;
	ULONG tr_SegList;
	WORD tr_Sprite;
	struct SimpleSprite tr_Cursor;
	BOOL tr_MsgLock;
	BOOL tr_FileLock;
	ULONG tr_SemCount;
};

/* Will eventually be filled in by SaveMsgText()
 */

struct NetAddress {
	UWORD na_Zone;
	UWORD na_Net;
	UWORD na_Node;
	UWORD na_Point;
	UWORD na_PointNet;
	UBYTE na_Domain[20];
};

#define ATTR_PRIV 1L
#define ATTR_CRASH 2L
#define ATTR_RECEIVED 4L
#define ATTR_FATTACH 16L
#define ATTR_KILL 128L
#define ATTR_HOLD 512L

struct MsgHeader {
	UBYTE mh_FromUser[36];
	UBYTE mh_ToUser[36];
	UBYTE mh_Subject[72];
	UBYTE mh_DateTime[20];
	UBYTE mh_MsgID[30];
	struct NetAddress mh_OrigNode;
	struct NetAddress mh_DestNode;
	LONG mh_Attrs;
	LONG mh_PrevReply;
	LONG mh_NextReply;
};

/* Message mode values for SaveMsgText()
 */

#define QUOTE_TYPE		1L		/* Message text should be displayed in quoted form */
#define ASCII_TYPE		2L		/* Normal display with no ANSI */
#define ANSI_TYPE		4L		/* Normal display with ANSI */
#define SHOW_KLUDGE		8L		/* Display kludge lines */
#define TEXTONLY_TYPE  16L		/* Show body only, no header or threading */

/* This is strictly private, DO NOT USE IT!
 */

struct ANSITerminal {
	UWORD at_FColour;
	UWORD at_BColour;
	WORD at_X,at_Y;
	WORD at_SX,at_SY;
	ULONG at_Style;							/* PRIVATE */
	UWORD at_Flag;							/* PRIVATE */
	UWORD at_Values[3];						/* PRIVATE */
	UWORD at_Bright;						/* PRIVATE */
	UWORD at_ValPtr;						/* PRIVATE */
	BOOL at_InANSI;							/* PRIVATE */
	UBYTE at_Work[90];
};

/* This is the meat and potatoes.  Be careful, as there is a mixture of
 * public fields, read-only fields, and private fields.  If you are
 * starting up a new line, then you must set the bl_Number and bl_Window
 * fields, and optionally bl_SerDev, bl_SerUnit, bl_Screen, bl_Planes,
 * bl_Name, bl_Status, bl_Message, bl_FontName and bl_FontSize, BEFORE you
 * call NewBBSLine().  You may only change the bl_Status and bl_Comment
 * fields after this has been called, or if you recieved this structure with
 * BBSLineInfo().  You can also change bl_CallerType, but under normal
 * circumstances, only the programme that created the line should do so.
 */

struct BBSLine {
	ULONG bl_Number;						/* The number associated with this line */
	char *bl_Name;							/* The "name" of the user of this line */
	char *bl_Status;						/* Current status of this line */
	char *bl_Message;						/* PRIVATE */
	ULONG bl_SerUnit;						/* Serial device unit */
	char *bl_SerDev;						/* Serial device name, NULL for no serial I/O */
	struct IOExtSer *bl_SerRead;			/* Serial I/O read request */
	struct IOExtSer *bl_SerWrite;			/* Serial I/O write request */
	struct IOStdReq *bl_ConRead;			/* Console I/O read request */
	struct IOStdReq *bl_ConWrite;			/* Console I/O write request */
	struct Window *bl_Window;				/* Window for console output */
	struct Screen *bl_Screen;				/* Custom screen used (optional) */
	struct SignalSemaphore *bl_UserSem;		/* PRIVATE */
	struct SignalSemaphore *bl_MsgPSem;		/* PRIVATE */
	char *bl_FontName;						/* The name of the font to use */
	ULONG bl_FontSize;						/* The size of the font to use */
	struct TextFont *bl_TextFont;			/* Of the font in the window */
	BYTE bl_SReadOpen;						/* PRIVATE */
	BYTE bl_SWriteOpen;						/* PRIVATE */
	struct SignalSemaphore *bl_FileSem;		/* PRIVATE */
	struct SignalSemaphore *bl_CallerSem;	/* PRIVATE */
	struct SignalSemaphore *bl_LReadSem;	/* PRIVATE */
	BOOL bl_ConPending;						/* PRIVATE */
	struct BBSLine *bl_OtherLine;			/* PRIVATE */
	ULONG bl_Nothing;						/* PRIVATE */
	WORD bl_Planes;							/* If you set bl_Screen, SET THIS! */
	WORD bl_Sprite;							/* PRIVATE */
	struct SimpleSprite bl_Cursor;			/* PRIVATE */
	struct ANSITerminal bl_Term;			/* PRIVATE */
	BOOL bl_ReceiveMsgs;					/* Willing to receive node messages */
	ULONG bl_Expansion[60]; 				/* Lots of room for expansion */
};

/* Error values returned by NewBBSLine() and ObtainLine()
 */

#define ERR_LINEINUSE 1
#define ERR_NOREADPORT 2
#define ERR_NOREADIO 3
#define ERR_NOSERDEVREAD 4
#define ERR_NOWRITEPORT 2
#define ERR_NOWRITEIO 3
#define ERR_NOSERDEVWRITE 4
#define ERR_NOWINDOW 5
#define ERR_NOCONPORT 6
#define ERR_NOCONIO 7
#define ERR_NOCONDEV 8
#define ERR_LINENOTFOUND 9
#define ERR_SERDEVLOCKED 10

/* Values passed to CheckLock()
 */
#define MSGAREAS 1
#define FILEAREAS 2

#endif
