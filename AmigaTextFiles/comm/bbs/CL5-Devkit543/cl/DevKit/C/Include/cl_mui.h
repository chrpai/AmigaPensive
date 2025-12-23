#ifndef _CL_MUI_H
#define _CL_MUI_H

#include <libraries/mui.h>
#include <proto/muimaster.h>

/*
 *      Connectline 13.0 "cl_mui.library"
 *      and custom class definitions.
 */

extern struct Library *CLMUIBase;

/*
 *  SAS/C specific pragmas
 */

#pragma libcall CLMUIBase CLMUI_NewObjectA 1e 9802
#pragma tagcall CLMUIBase CLMUI_NewObject 1e 9802
#pragma libcall CLMUIBase CLMUI_ErrorRequest 24 0
#pragma libcall CLMUIBase CLMUI_SCF_String 2a 0802
#pragma libcall CLMUIBase CLMUI_SCF_KeyString 30 10803
#pragma libcall CLMUIBase CLMUI_SCF_CheckMark 36 001
#pragma libcall CLMUIBase CLMUI_SCF_KeyCheckMark 3c 1002
#pragma libcall CLMUIBase CLMUI_SCF_SimpleButton 42 801
#pragma libcall CLMUIBase CLMUI_SCF_KeyButton 48 0802
#pragma libcall CLMUIBase CLMUI_SCF_Cycle 4e 801
#pragma libcall CLMUIBase CLMUI_SCF_KeyCycle 54 0802
#pragma libcall CLMUIBase CLMUI_SCF_Radio 5a 9802
#pragma libcall CLMUIBase CLMUI_SCF_KeyRadio 60 09803
#pragma libcall CLMUIBase CLMUI_SCF_Slider 66 21003
#pragma libcall CLMUIBase CLMUI_SCF_KeySlider 6c 321004
#pragma libcall CLMUIBase CLMUI_SCF_PopButton 72 001
#pragma libcall CLMUIBase CLMUI_SCF_Label 78 801
#pragma libcall CLMUIBase CLMUI_SCF_Label1 7e 801
#pragma libcall CLMUIBase CLMUI_SCF_Label2 84 801
#pragma libcall CLMUIBase CLMUI_SCF_LLabel 8a 801
#pragma libcall CLMUIBase CLMUI_SCF_LLabel1 90 801
#pragma libcall CLMUIBase CLMUI_SCF_LLabel2 96 801
#pragma libcall CLMUIBase CLMUI_SCF_KeyLabel 9c 0802
#pragma libcall CLMUIBase CLMUI_SCF_KeyLabel1 a2 0802
#pragma libcall CLMUIBase CLMUI_SCF_KeyLabel2 a8 0802
#pragma libcall CLMUIBase CLMUI_SCF_KeyLLabel ae 0802
#pragma libcall CLMUIBase CLMUI_SCF_KeyLLabel1 b4 0802
#pragma libcall CLMUIBase CLMUI_SCF_KeyLLabel2 ba 0802
#pragma libcall CLMUIBase CLMUI_Info c0 801
#pragma libcall CLMUIBase CLMUI_Boards2lv c6 0802
#pragma libcall CLMUIBase CLMUI_Links2lv cc 801
#pragma libcall CLMUIBase CLMUI_lv2Gruppen d2 09803
#pragma libcall CLMUIBase CLMUI_Gruppen2lv d8 9802
#pragma libcall CLMUIBase CLMUI_lv2Verteiler de 09803
#pragma libcall CLMUIBase CLMUI_Verteiler2lv e4 9802
#pragma libcall CLMUIBase CLMUI_FindIn_lv ea 9802
#pragma libcall CLMUIBase CLMUI_FindDel f0 9802
#pragma libcall CLMUIBase CLMUI_Copy_lv_Active f6 9802
#pragma libcall CLMUIBase CLMUI_str2lv fc 9802
#pragma libcall CLMUIBase CLMUI_WildcardSelect 102 09803
#pragma libcall CLMUIBase CLMUI_CheckmarkExcludeA 108 801
#pragma tagcall CLMUIBase CLMUI_CheckmarkExclude 108 801
#pragma libcall CLMUIBase CLMUI_SetCycleChainA 10e 9802
#pragma tagcall CLMUIBase CLMUI_SetCycleChain 10e 9802
#pragma libcall CLMUIBase CLMUI_HelpFileName 114 9802
#pragma libcall CLMUIBase CLMUI_TickDelay 11a 0802
#pragma libcall CLMUIBase CLMUI_DisableA 120 8002
#pragma tagcall CLMUIBase CLMUI_Disable 120 8002
#pragma libcall CLMUIBase CLMUI_SCF_Set 126 10803
#pragma libcall CLMUIBase CLMUI_SCF_Get 12c 90803
#pragma libcall CLMUIBase CLMUI_SCF_SetString 132 9802
#pragma libcall CLMUIBase CLMUI_SCF_GetString 138 9802
#pragma libcall CLMUIBase CLMUI_SCF_SetText 13e 9802
#pragma libcall CLMUIBase CLMUI_SCF_SetInteger 144 0802
#pragma libcall CLMUIBase CLMUI_SCF_GetInteger 14a 9802
#pragma libcall CLMUIBase CLMUI_SCF_SetCycle 150 0802
#pragma libcall CLMUIBase CLMUI_SCF_GetCycle 156 9802
#pragma libcall CLMUIBase CLMUI_SCF_SetCheckmark 15c 0802
#pragma libcall CLMUIBase CLMUI_SCF_GetCheckmark 162 9802
#pragma libcall CLMUIBase CLMUI_SetKeyID 168 801
#pragma libcall CLMUIBase CLMUI_SetDoubleStart 16e 9802
#pragma libcall CLMUIBase CLMUI_MakeButtonSendID 174 0802
#pragma libcall CLMUIBase CLMUI_KeyTimedout 17a 0
#pragma libcall CLMUIBase CLMUI_KeyNoPermission 180 801

/*
 *   ANSI Prototypes
 */

APTR CLMUI_NewObjectA( STRPTR classname, struct TagItem *tags );
APTR CLMUI_NewObject( STRPTR classname, ... );
LONG CLMUI_ErrorRequest( void );
APTR CLMUI_SCF_String( STRPTR, ULONG );
APTR CLMUI_SCF_KeyString( STRPTR, ULONG, ULONG );
APTR CLMUI_SCF_CheckMark( ULONG );
APTR CLMUI_SCF_KeyCheckMark( ULONG, ULONG );
APTR CLMUI_SCF_SimpleButton( STRPTR );
APTR CLMUI_SCF_KeyButton( STRPTR, ULONG );
APTR CLMUI_SCF_Cycle( STRPTR * );
APTR CLMUI_SCF_KeyCycle( STRPTR *, ULONG );
APTR CLMUI_SCF_Radio( STRPTR, STRPTR * );
APTR CLMUI_SCF_KeyRadio( STRPTR, STRPTR *, ULONG );
APTR CLMUI_SCF_Slider( ULONG, ULONG, ULONG );
APTR CLMUI_SCF_KeySlider( ULONG, ULONG, ULONG, ULONG );
APTR CLMUI_SCF_PopButton( ULONG );
APTR CLMUI_SCF_Label( STRPTR );
APTR CLMUI_SCF_Label1( STRPTR );
APTR CLMUI_SCF_Label2( STRPTR );
APTR CLMUI_SCF_LLabel( STRPTR );
APTR CLMUI_SCF_LLabel1( STRPTR );
APTR CLMUI_SCF_LLabel2( STRPTR );
APTR CLMUI_SCF_KeyLabel( STRPTR, ULONG );
APTR CLMUI_SCF_KeyLabel1( STRPTR, ULONG );
APTR CLMUI_SCF_KeyLabel2( STRPTR, ULONG );
APTR CLMUI_SCF_KeyLLabel( STRPTR, ULONG );
APTR CLMUI_SCF_KeyLLabel1( STRPTR, ULONG );
APTR CLMUI_SCF_KeyLLabel2( STRPTR, ULONG );
void CLMUI_Info( APTR applicationobject );

void CLMUI_Boards2lv(APTR , BOOL );
void CLMUI_Links2lv(APTR );
void CLMUI_lv2Gruppen(APTR , char *, ULONG );
void CLMUI_Gruppen2lv(char * , APTR );
void CLMUI_lv2Verteiler(APTR , struct Brett *, ULONG );
void CLMUI_Verteiler2lv(struct Brett * , APTR );
long CLMUI_FindIn_lv(APTR , char * );
void CLMUI_FindDel(APTR , char * );
void CLMUI_Copy_lv_Active(APTR , APTR );
void CLMUI_str2lv(APTR , APTR );
void CLMUI_WildcardSelect(APTR , APTR , LONG );
void CLMUI_CheckmarkExcludeA( APTR * );
void CLMUI_CheckmarkExclude(...);

void CLMUI_SetCycleChainA( APTR window, APTR *objlist );
void CLMUI_SetCycleChain( APTR window, ... );

void CLMUI_HelpFileName( STRPTR appname, STRPTR to );

void CLMUI_TickDelay( APTR appobject, LONG ticks );

void CLMUI_DisableA( LONG state, APTR *objlist );
void CLMUI_Disable( LONG state, ... );

void CLMUI_SCF_Set( APTR obj, ULONG attr, ULONG val );
void CLMUI_SCF_Get( APTR obj, ULONG attr, ULONG *valptr );
void CLMUI_SCF_SetString( APTR obj, STRPTR val );
void CLMUI_SCF_GetString( APTR obj, STRPTR *valptr );
void CLMUI_SCF_SetText( APTR obj, STRPTR val );
void CLMUI_SCF_SetInteger( APTR obj, ULONG val );
void CLMUI_SCF_GetInteger( APTR obj, ULONG *valptr );
void CLMUI_SCF_SetCycle( APTR obj, ULONG val );
void CLMUI_SCF_GetCycle( APTR obj, ULONG *valptr );
void CLMUI_SCF_SetCheckmark( APTR obj, ULONG val );
void CLMUI_SCF_GetCheckmark( APTR obj, ULONG *valptr );

void CLMUI_SetKeyID( APTR );
void CLMUI_SetDoubleStart( APTR app, APTR win );
void CLMUI_MakeButtonSendID( APTR but, LONG id );

void CLMUI_KeyTimedout( void );
void CLMUI_KeyNoPermission( STRPTR subsystem );

/*
 *  Redefine MUI macros
 */

//#ifdef _CLMUI_MACROFUNC
#undef String
#define String CLMUI_SCF_String
#undef KeyString
#define KeyString CLMUI_SCF_KeyString
#undef CheckMark
#define CheckMark CLMUI_SCF_CheckMark
#undef KeyCheckMark
#define KeyCheckMark CLMUI_SCF_KeyCheckMark
#undef SimpleButton
#define SimpleButton CLMUI_SCF_SimpleButton
#undef KeyButton
#define KeyButton CLMUI_SCF_KeyButton
#undef Cycle
#define Cycle CLMUI_SCF_Cycle
#undef KeyCycle
#define KeyCycle CLMUI_SCF_KeyCycle
#undef Radio
#define Radio CLMUI_SCF_Radio
#undef KeyRadio
#define KeyRadio CLMUI_SCF_KeyRadio
#undef Slider
#define Slider CLMUI_SCF_Slider
#undef KeySlider
#define KeySlider CLMUI_SCF_KeySlider
#undef PopButton
#define PopButton CLMUI_SCF_PopButton
#undef Label
#define Label CLMUI_SCF_Label
#undef Label1
#define Label1 CLMUI_SCF_Label1
#undef Label2
#define Label2 CLMUI_SCF_Label2
#undef LLabel
#define LLabel CLMUI_SCF_LLabel
#undef LLabel1
#define LLabel1 CLMUI_SCF_LLabel1
#undef LLabel2
#define LLabel2 CLMUI_SCF_LLabel2
#undef KeyLabel
#define KeyLabel CLMUI_SCF_KeyLabel
#undef KeyLabel1
#define KeyLabel1 CLMUI_SCF_KeyLabel1
#undef KeyLabel2
#define KeyLabel2 CLMUI_SCF_KeyLabel2
#undef KeyLLabel
#define KeyLLabel CLMUI_SCF_KeyLLabel
#undef KeyLLabel1
#define KeyLLabel1 CLMUI_SCF_KeyLLabel1
#undef KeyLLabel2
#define KeyLLabel2 CLMUI_SCF_KeyLLabel2

// Set/Get stuff

#undef set
#undef get
#undef setcycle
#undef setstring
#undef setcheckmark

#define set(o,a,v) CLMUI_SCF_Set(o,a,(ULONG)(v))
#define get(o,a,v) CLMUI_SCF_Get(o,a,(ULONG*)(v))
#define setstring(o,v) CLMUI_SCF_SetString(o,v)
#define getstring(o,v) CLMUI_SCF_GetString(o,v)
#define settext(o,v) CLMUI_SCF_SetText(o,v)
#define setinteger(o,v) CLMUI_SCF_SetInteger(o,v)
#define getinteger(o,v) CLMUI_SCF_GetInteger(o,v)
#define setcycle(o,v) CLMUI_SCF_SetCycle(o,v)
#define getcycle(o,v) CLMUI_SCF_GetCycle(o,v)
#define setcheckmark(o,v) CLMUI_SCF_SetCheckmark(o,v)
#define getcheckmark(o,v) CLMUI_SCF_GetCheckmark(o,v)

//#endif

/******************************************************************/
/* Misc */
/******************************************************************/

/*
 *      This magic value ends a CLMUI_SetCycleChain
 *      object list.
 */

#define CLMUI_SCCEND ((APTR)-1)


/********************************************************************
 ********* Custom Classes *******************************************
 ********************************************************************/

#define CLMUIA_TB (TAG_USER|(222<<16))

/*
 * Console 1.1
 * -----------
 *
 * Subclass of Areaclass, allows integrating of console device
 * in a MUI friendly manner.
 *
 */

#define CLC_CONSOLE "cl_console.clmui"

/*
 *  Attributes
 */

#define CLMUIA_Console_IOWrite                          (CLMUIA_TB+66)  // [I.G] Console write request
#define CLMUIA_Console_IOWritePort                      (CLMUIA_TB+68)  // [I.G] Console write msgport
#define CLMUIA_Console_Query                            (CLMUIA_TB+71)  // [..G] number of chars avaible in input buffer
#define CLMUIA_Console_ReadChar                         (CLMUIA_TB+73)  // [..G] read current char
#define CLMUIA_Console_Cursor                           (CLMUIA_TB+75)  // [ISG] cursor?

/*
 *  New Methods
 */

#define CLMUIM_Console_Write                            (CLMUIA_TB+70)  // New Method: Write Text [buffer,len]
#define CLMUIM_Console_Read                             (CLMUIA_TB+72)  // New Method: Read text [buffer,len]

/*
 *  Macros
 */

#define CLConsoleObject CLMUI_NewObject(CLC_CONSOLE


/*************************************************************************/

/*
 * PopstringList 3.1
 * -----------------
 *
 * A subclass of MUI's PopObjectClass. Creates a PopupObject
 * with properly set up String, Button and a Popup Listview.
 * Possible choices are given as a NULL terminated array of
 * STRPTRs or a magic value designating a User, Board or
 * System list.
 */

#define CLC_POPSTRINGLIST "cl_popstringlist.clmui"

/*
 *  Attributes
 */

#define CLMUIA_Popstringlist_MaxLen             (CLMUIA_TB+1)   // [I..] length of string object (40)
#define CLMUIA_Popstringlist_ButtonImage        (CLMUIA_TB+2)   // [I..] ImageID of button to generate (MUII_PopUp)
#define CLMUIA_Popstringlist_Contents           (CLMUIA_TB+3)   // [ISG] string contents (Empty)
#define CLMUIA_Popstringlist_Entries            (CLMUIA_TB+4)   // [I..] NULL terminated array of popup selections or a magic value (Empty)
#define CLMUIA_Popstringlist_StringObject       (CLMUIA_TB+5)   // [I.G] (APTR) embedded String Object
#define CLMUIA_Popstringlist_Integer            (CLMUIA_TB+6)   // [ISG] string contents, integer form (Empty)
#define CLMUIA_Popstringlist_FilterHook         (CLMUIA_TB+7)   // [I..] filter hook for Use/Board/Syslist

/*
 *  Magic values fuer CLMUIA_Popstringlist_Entries
 */

#define CLMUIV_Popstringlist_Userlist           (-1)                    // Show Userlist
#define CLMUIV_Popstringlist_Boardlist          (-2)                    // Show Boardlist
#define CLMUIV_Popstringlist_UserBoardlist      (-3)                    // Show User and Boardlist
#define CLMUIV_Popstringlist_Systemlist         (-4)                    // Show Systemlist

/*
 *  Your filter hook is called with a pointer to your hook
 *  in A0, one of the values below in A1 (read: A1 contains
 *  the value, not a pointer to it!) and the hook object
 *  in A2. If the hook returns TRUE, the object is added
 *  to the list.
 *
 *  [Note: Some older documentation had A1 / A2 exchanged
 *   in above description!]
 */

#define CLMUIV_Popstringlist_HookObjectUser   (0)                       // Hook object is struct User
#define CLMUIV_Popstringlist_HookObjectBoard  (1)                       // Hook object is struct Board
#define CLMUIV_Popstringlist_HookObjectSystem (2)                       // Hook object is struct System

/*
 *  Macros
 */

#define CLPopstringlistObject CLMUI_NewObject(CLC_POPSTRINGLIST
#define CLPopstringlist(default,len,array) \
        CLPopstringlistObject, \
                CLMUIA_Popstringlist_Contents,  default, \
                CLMUIA_Popstringlist_MaxLen,    len,\
                CLMUIA_Popstringlist_Entries,   array,\
                End

#define CLPopstringlistM(default,list) \
        CLPopstringlistObject, \
                CLMUIA_Popstringlist_Contents,  default, \
                CLMUIA_Popstringlist_MaxLen,    80,\
                CLMUIA_Popstringlist_Entries,   list,\
                End

/*************************************************************************/

/*
 * Groupselect 1.0
 * -----------------
 *
 * A subclass of MUI's Listview class. Allows group selection.
 */

#define CLC_GROUPSELECT "cl_groupselect.clmui"

/*
 *  Attributes
 */

#define CLMUIA_Groupselect_Groups	(CLMUIA_TB+9487)	// [ISG] Group array


/*
 * Integer 1.0
 * -----------------
 *
 * A subclass of MUI's String class. Allows setting min & max values
 * with automatic range checking, and allows notifications on
 * MUIA_String_Integer
 */

#define CLC_INTEGER "cl_integer.clmui"

/*
 *  Attributes
 */

#define CLMUIA_Integer_Min	(CLMUIA_TB+7731)	// [I..] Minimum value (defaults to 0)
#define CLMUIA_Integer_Max	(CLMUIA_TB+7732)	// [I..] Maximum value (defaults to MAXINT)

/*
 *  Macros
 */

#define CLInteger(current,min,max) CLMUI_NewObject(CLC_INTEGER,MUIA_String_Integer,current,CLMUIA_Integer_Min,min,CLMUIA_Integer_Max,max,StringFrame,NULL)
#define CLIntegerNR(current) CLMUI_NewObject(CLC_INTEGER,MUIA_String_Integer,current,StringFrame,NULL)

// Popimage bei Button

#define PopImageKeyButton(control)\
        ImageObject,\
                ImageButtonFrame,\
                MUIA_InputMode        , MUIV_InputMode_RelVerify,\
                MUIA_Image_Spec       , MUII_PopUp,\
                MUIA_Image_FreeVert   , TRUE,\
                MUIA_Background       , MUII_ButtonBack,\
                MUIA_ShowSelState     , FALSE,\
                MUIA_ControlChar      , control,\
                End

#endif
