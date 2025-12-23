/*
// clonline.library, Version 5.0
//
// Copyright 1995 by Mathias Mischler
//
// Definitons
*/

#ifndef CLONLINE_H
#define CLONLINE_H

/* Some more includes, that there is no need of others in YOUR main */

#include <time.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <cl/types.h>
#include <cl/lists.h>
#include <cl/cl_user.h>
#include <cl/UserShell.h>

/* Pragma Definitions */

#pragma libcall CLOnlineBase CLONL_Open 1e 801
#pragma libcall CLOnlineBase CLONL_Close 24 00
#pragma libcall CLOnlineBase CLONL_FlushInput 2a 00
#pragma libcall CLOnlineBase CLONL_FlushOutput 30 00
#pragma libcall CLOnlineBase CLONL_UpdateUser 36 00
#pragma libcall CLOnlineBase CLONL_Normal 3c 00
#pragma libcall CLOnlineBase CLONL_DeleteEOL 42 00
#pragma libcall CLOnlineBase CLONL_DeleteLine 48 00
#pragma libcall CLOnlineBase CLONL_InsertLine 4e 00
#pragma libcall CLOnlineBase CLONL_BlueMode 54 00
#pragma libcall CLOnlineBase CLONL_GotoXY 5a 1002
#pragma libcall CLOnlineBase CLONL_Color 60 21003
#pragma libcall CLOnlineBase CLONL_ClearScreen 66 00
#pragma libcall CLOnlineBase CLONL_BlueRead 6c 2109805
#pragma libcall CLOnlineBase CLONL_Wait 72 00
#pragma libcall CLOnlineBase CLONL_DrawTitle 78 00
#pragma libcall CLOnlineBase CLONL_Edit 7e 801
#pragma libcall CLOnlineBase CLONL_RequesterSelect a8 190804
#pragma libcall CLOnlineBase CLONL_GetLocale ae 00
#pragma libcall CLOnlineBase CLONL_GetCatalog b4 00
#pragma libcall CLOnlineBase CLONL_GetUserShell ba 00
#pragma libcall CLOnlineBase CLONL_IsTTY c0 00
#pragma libcall CLOnlineBase CLONL_GetString c6 801
#pragma libcall CLOnlineBase CLONL_VPrintf cc 9802
#pragma tagcall CLOnlineBase CLONL_Printf cc 9802
#pragma libcall CLOnlineBase CLONL_VSPrintf d2 A9803
#pragma tagcall CLOnlineBase CLONL_SPrintf d2 A9803
#pragma libcall CLOnlineBase CLONL_DoCommand d8 9802
#pragma libcall CLOnlineBase CLONL_SendMail de DBA9805
#pragma libcall CLOnlineBase CLONL_GetKey e4 00
#pragma libcall CLOnlineBase CLONL_FreeRange f0 801
#pragma libcall CLOnlineBase CLONL_ParseExpression f6 0802
#pragma libcall CLOnlineBase CLONL_Transfer fc 9802
#pragma libcall CLOnlineBase CLONL_GetUserFullName 102 801
#pragma libcall CLOnlineBase CLONL_GetNameVal 108 801
#pragma libcall CLOnlineBase CLONL_IsKeyPressed 10e 00
#pragma libcall CLOnlineBase CLONL_Continue 114 00
#pragma libcall CLOnlineBase CLONL_AddShortMenu 11a 09803
#pragma libcall CLOnlineBase CLONL_FreeShortMenu 120 801
#pragma libcall CLOnlineBase CLONL_ShortMenuSelect 126 801
#pragma libcall CLOnlineBase CLONL_TimeToStr 12c 8002
#pragma libcall CLOnlineBase CLONL_StrToTime 132 9802
#pragma libcall CLOnlineBase CLONL_AddGadget 138 1A90805
#pragma libcall CLOnlineBase CLONL_GetGadget 13e 9802
#pragma libcall CLOnlineBase CLONL_InputMask 144 9802
#pragma libcall CLOnlineBase CLONL_ShortMenuSelectVertical 14a 801
#pragma libcall CLOnlineBase CLONL_XFer 150 0802
#pragma libcall CLOnlineBase CLONL_FreeXFerList 156 801
#pragma libcall CLOnlineBase CLONL_SetObscure 15c 001
#pragma libcall CLOnlineBase CLONL_PrintFile 162 801

/* Defs for "CLOnlineBase = OpenLibrary ( CLONL_NAME, CLONL_VERSION )" */

extern struct Library *CLOnlineBase;

#define CLONL_NAME "CONNECTLINE:libs/clonline.library"
#define CLONL_VERSION 5

// Definition of a ANSI-Select Entry

struct EntryNode
{
	struct MinNode n;	// Exec MinNodes
	char	 data[80];	// String to display
	BOOL	 bold;
	BOOL	 inverse;
};

// Colour Definitions for CLONL_Color Command

#define COLOR_Black  0
#define COLOR_Red    1
#define COLOR_Green  2
#define COLOR_Yellow 3
#define COLOR_Blue   4
#define COLOR_Pink   5
#define COLOR_Cyan   6
#define COLOR_White  7

// Style Definitions for CLONL_Color Command

#define STYLE_Normal     0
#define STYLE_Bold       1
#define STYLE_Kursiv     3
#define STYLE_Underlined 4
#define STYLE_Inverse    7

// Definitions of types for CLONL_BlueRead()

#define TYPE_ASCII  0
#define TYPE_NUMBER 1
#define TYPE_BOOL   2
#define TYPE_SECRET 3

// Definitions for CLONL_GetKey ()

#define CHAR_CTRLA 1
#define CHAR_CTRLB 2
#define CHAR_CTRLC 3
#define CHAR_CTRLD 4
#define CHAR_CTRLE 5
#define CHAR_CTRLF 6
#define CHAR_BS    8
#define CHAR_TAB   9
#define CHAR_LF  	 10
#define CHAR_CR    13
#define CHAR_CTRLK 11
#define CHAR_CTRLN 14
#define CHAR_CTRLR 18
#define CHAR_CTRLX 24
#define CHAR_CTRLZ 26
#define CHAR_ESC   27
#define CHAR_DEL   127
#define CHAR_CSI   155

#define CHAR_BREAK 1024
#define CHAR_UP    1025
#define CHAR_DOWN  1026
#define CHAR_RIGHT 1027
#define CHAR_LEFT  1028
#define CHAR_EOF   1029

// Definitons for Mode for ANSI-Requester

#define MODE_SELECT 0 // To select multiple entrys by marking
#define MODE_CHOOSE 1 // To choose ONE entry by cursor left
#define MODE_INHALT 2 // Special - internal

// Defines for easyer textoutput

#define MSG(txt) CLONL_GetString(txt)
#define PutMSG(txt) CLONL_Printf(MSG(txt))

//
// Definition of range for CLONL_FreeRange() and CLONL_ParseExpression()
//

struct Range
{
	unsigned long from;
	unsigned long to;
	struct Range *next;
};

//
// Definition of Shortmenu for CLONL_AddShortMenu, CLONL_ShortMenuSelect(), CLONL_FreeShortMenu
//

struct Shortmenu
{
	char entry [ 80 ];
	char hotkey;
	struct Shortmenu *next;
};

//
// Definition for one Mask Entry for CLONL_AddGadget, CLONL_GetGagdet, CLONL_InputMask()
//

struct MaskNode
{
	struct MinNode n;
	ulong  MaskType;
	char   Identifier[40];
	char  *Buffer;
	ulong  BufferSize;
};

typedef struct MaskNode MaskNode;

// Mask-List Definition

typedef struct MinList Mask;

BOOL  CLONL_Open ( char *catalogname );
void  CLONL_Close ( void );
void  CLONL_FlushInput ( void );
void  CLONL_FlushOutput ( void );
void  CLONL_UpdateUser( void );
void  CLONL_Normal ( void );
void  CLONL_DeleteEOL ( void );
void  CLONL_DeleteLine ( void );
void  CLONL_InsertLine ( void );
void  CLONL_BlueMode ( void );
void  CLONL_GotoXY ( ulong x, ulong y );
void  CLONL_Color ( ulong col, ulong bcol, ulong style );
void  CLONL_ClearScreen ( void );
char *CLONL_BlueRead ( char *str, char *def, ulong len, ulong type, BOOL maske );
BOOL  CLONL_Wait ( void );
void  CLONL_DrawTitle ( void );
BOOL  CLONL_Edit ( char *filename );
struct EntryNode *CLONL_RequesterSelect ( struct MinList *entrys, ulong pos, char *titel, ulong MODE );
void *CLONL_GetLocale ( void );
APTR CLONL_GetCatalog ( void );
struct CLUserShell *CLONL_GetUserShell ( void );
BOOL CLONL_IsTTY ( void );
char *CLONL_GetString ( char *entry );
void CLONL_VPrintf ( char *format, APTR ctl );
void CLONL_Printf ( char *format, ... );
void CLONL_VSPrintf ( char *to, char *format, APTR ctl );
void CLONL_SPrintf ( char *to, char *format, ... );
long CLONL_DoCommand ( char *cmd, char *param);
ulong CLONL_SendMail ( char *abs, char *emp, char *bet, char *filename, char *bezid );
long CLONL_GetKey ( void );
void CLONL_FreeRange ( struct Range *myrange );
struct Range *CLONL_ParseExpression ( char *parsepattern, ulong maximum );
long CLONL_Transfer ( char *filename, char *sendfilename );
char *CLONL_GetUserFullName ( char *name );
long CLONL_GetNameVal ( char *name );
BOOL CLONL_IsKeyPressed ( void );
BOOL CLONL_Continue ( void );
struct Shortmenu *CLONL_AddShortMenu ( struct Shortmenu *menu, char *entry, char hotkey );
void CLONL_FreeShortMenu ( struct Shortmenu *menu );
char CLONL_ShortMenuSelect ( struct Shortmenu *menu );
char *CLONL_TimeToStr ( time_t time, char *str );
time_t CLONL_StrToTime ( char *str, time_t *time );
void CLONL_AddGadget ( Mask ** mask, ulong masktype, char *identifier, char *buffer, ulong buffersize );
MaskNode *CLONL_GetGadget ( Mask * mask, char *identifier );
BOOL CLONL_InputMask ( Mask * mask, char *title );
char CLONL_ShortMenuSelectVertical ( struct Shortmenu *menu );
struct List *CLONL_XFer ( struct List *sendlist, ULONG recflag );
void CLONL_FreeXFerList ( struct List *reclist );
void CLONL_SetObscure ( int isobscure );
BOOL CLONL_PrintFile( char *filename );

#endif
