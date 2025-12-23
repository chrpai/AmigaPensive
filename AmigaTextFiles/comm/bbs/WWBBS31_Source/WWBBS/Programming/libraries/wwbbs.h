#ifndef LIBRARIES_WWBBS_H
#define LIBRARIES_WWBBS_H

/*
**  $VER: wwbbs.h 3.0 (1.4.94)
*/

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

/*
** Config
*/

/* Global */

#define CFGTAG_GLOBAL ((ULONG)(1L<<15))

/* Get/Set/Add/Rem */
#define CFGTAG_Path TAG_USER+CFGTAG_GLOBAL+1
#define CFGTAG_Name TAG_USER+CFGTAG_GLOBAL+2

/* Get/Add */
#define CFGTAG_Type TAG_USER+CFGTAG_GLOBAL+3

/* Get */
#define CFGTAG_Exists TAG_USER+CFGTAG_GLOBAL+4
#define CFGTAG_Parent TAG_USER+CFGTAG_GLOBAL+5
#define CFGTAG_Child TAG_USER+CFGTAG_GLOBAL+6
#define CFGTAG_GetFirst TAG_USER+CFGTAG_GLOBAL+7
#define CFGTAG_GetLast TAG_USER+CFGTAG_GLOBAL+8
#define CFGTAG_GetNext TAG_USER+CFGTAG_GLOBAL+9
#define CFGTAG_GetPrevious TAG_USER+CFGTAG_GLOBAL+10
#define CFGTAG_Next TAG_USER+CFGTAG_GLOBAL+11
#define CFGTAG_NextReturn TAG_USER+CFGTAG_GLOBAL+12

/* Set */
#define CFGTAG_NewName TAG_USER+CFGTAG_GLOBAL+13
#define CFGTAG_MoveUp TAG_USER+CFGTAG_GLOBAL+14
#define CFGTAG_MoveDown TAG_USER+CFGTAG_GLOBAL+15
#define CFGTAG_ForceSave TAG_USER+CFGTAG_GLOBAL+16

/* Set/Add/Rem */
#define CFGTAG_DontSave TAG_USER+CFGTAG_GLOBAL+17


/* Local */
#define AGTAG_AccessLevel TAG_USER+1
#define AGTAG_SessionTimeLimit TAG_USER+2
#define AGTAG_InactivityTimeLimit TAG_USER+3
#define AGTAG_RatioByte TAG_USER+4
#define AGTAG_RatioFile TAG_USER+5
#define AGTAG_Days TAG_USER+6

#define ARCTAG_Extension TAG_USER+1
#define ARCTAG_AddCommand TAG_USER+2
#define ARCTAG_ExtractCommand TAG_USER+3
#define ARCTAG_ViewCommand TAG_USER+4

/*#define VARTAG_String TAG_USER+1*/

#define EDTAG_Type TAG_USER+1
#define EDTAG_Command TAG_USER+2
#define EDTAG_Stack TAG_USER+3

#define FBTAG_AccessRange TAG_USER+1
#define FBTAG_Group TAG_USER+2
#define FBTAG_Directory TAG_USER+3
#define FBTAG_DiskSpaceRequired TAG_USER+4
#define FBTAG_Days TAG_USER+5
#define FBTAG_ReadOnly TAG_USER+6
#define FBTAG_AutoJoin TAG_USER+7

#define MNTAG_AccessRange TAG_USER+1
#define MNTAG_Character TAG_USER+2
#define MNTAG_Default TAG_USER+3
#define MNTAG_Number TAG_USER+4
#define MNTAG_Prompt TAG_USER+5
#define MNTAG_Command TAG_USER+6

#define MBTAG_AccessRange TAG_USER+1
#define MBTAG_Group TAG_USER+2
#define MBTAG_QuoteHeader TAG_USER+3
#define MBTAG_ReadOnly TAG_USER+4
#define MBTAG_AutoJoin TAG_USER+5
#define MBTAG_Days TAG_USER+6

#define NWSTAG_AccessRange TAG_USER+1
#define NWSTAG_File TAG_USER+2

#define NDTAG_AccessRange TAG_USER+1
#define NDTAG_Type TAG_USER+2
#define NDTAG_Device TAG_USER+3
#define NDTAG_Unit TAG_USER+4
#define NDTAG_BufferSize TAG_USER+5
#define NDTAG_Baud TAG_USER+6
#define NDTAG_Handshaking TAG_USER+7
#define NDTAG_AutoBaud TAG_USER+8
#define NDTAG_FrontEnd TAG_USER+9
#define NDTAG_Command TAG_USER+10

#define PRTAG_SendCommand TAG_USER+1
#define PRTAG_ReceiveCommand TAG_USER+2
#define PRTAG_Batch TAG_USER+3
#define PRTAG_Bidirectional TAG_USER+4

/*#define SYSTAG_DisplayMode TAG_USER+1*/
/*#define SYSTAG_Font TAG_USER+2*/
#define SYSTAG_InputColor TAG_USER+3
#define SYSTAG_HeaderColor TAG_USER+4
#define SYSTAG_OutputColor TAG_USER+5
#define SYSTAG_PromptColor TAG_USER+6
#define SYSTAG_SystemColor TAG_USER+7
#define SYSTAG_OnlyRealNames TAG_USER+8
#define SYSTAG_FileIDDizExtractor TAG_USER+9

enum {
CFGTYP_Config=0,
CFGTYP_AccessGroup,
CFGTYP_Archiver,
DUMMY_DoorVariable,
CFGTYP_Editor,
CFGTYP_FileBase,
CFGTYP_Menu,
CFGTYP_MessageBase,
CFGTYP_News,
CFGTYP_Node,
CFGTYP_Protocol,
CFGTYP_System
};

enum {
EDTYP_Standard=0,
EDTYP_Visual,
EDTYP_Graphical
};

enum {
NDTYP_Local=0,
NDTYP_Remote
};

/*
** Status
*/

/* Global */
#define STTAG_GLOBAL ((ULONG)(1L<<15))

/* Get/Set/Add/Rem */
#define STTAG_Name TAG_USER+STTAG_GLOBAL+1

/* Get */
#define STTAG_Exists TAG_USER+STTAG_GLOBAL+2
#define STTAG_Next TAG_USER+STTAG_GLOBAL+3
#define STTAG_NextReturn TAG_USER+STTAG_GLOBAL+4

/* Local */
#define STTAG_UserName TAG_USER+1
#define STTAG_Location TAG_USER+2
#define STTAG_Prompt TAG_USER+3
#define STTAG_Stack TAG_USER+4
#define STTAG_TimeRemaining TAG_USER+5
#define STTAG_TimeUsed TAG_USER+6
#define STTAG_InactivityTimeLimit TAG_USER+7
#define STTAG_Panic TAG_USER+8
#define STTAG_Local TAG_USER+9
#define STTAG_ConsoleOpen TAG_USER+10
#define STTAG_ANSI TAG_USER+11
#define STTAG_PageReceived TAG_USER+12
#define STTAG_UserMonitor TAG_USER+13

/*
** Users
*/

/* Global */
#define USRTAG_GLOBAL ((ULONG)(1L<<15))

/* Get/Set/Rem/Load/UnLoad */
#define USRTAG_Name TAG_USER+USRTAG_GLOBAL+1

/* Get/Set/Add */
#define USRTAG_UserName TAG_USER+USRTAG_GLOBAL+2
#define USRTAG_RealName TAG_USER+USRTAG_GLOBAL+3
#define USRTAG_UUCPName TAG_USER+USRTAG_GLOBAL+4

/* Get */
#define USRTAG_Exists TAG_USER+USRTAG_GLOBAL+5
#define USRTAG_Next TAG_USER+USRTAG_GLOBAL+6
#define USRTAG_NextReturn TAG_USER+USRTAG_GLOBAL+7

/* Set */
#define USRTAG_ForceSave TAG_USER+USRTAG_GLOBAL+8

/* Set/Add/Rem */
#define USRTAG_DontSave TAG_USER+USRTAG_GLOBAL+9

/* Local */
#define USRTAG_Password TAG_USER+101
#define USRTAG_Address TAG_USER+102
#define USRTAG_City TAG_USER+103
#define USRTAG_State TAG_USER+104
#define USRTAG_Zip TAG_USER+105
#define USRTAG_Country TAG_USER+106
#define USRTAG_PhoneCode TAG_USER+107
#define USRTAG_PhoneNumber TAG_USER+108
#define USRTAG_BirthDay TAG_USER+109
#define USRTAG_BirthMonth TAG_USER+110
#define USRTAG_BirthYear TAG_USER+111

#define USRTAG_AccessLevel TAG_USER+201
#define USRTAG_SessionTimeLimit TAG_USER+202
#define USRTAG_InactivityTimeLimit TAG_USER+203
#define USRTAG_RatioByte TAG_USER+204
#define USRTAG_RatioFile TAG_USER+205
#define USRTAG_Days TAG_USER+206

#define USRTAG_ScreenWidth TAG_USER+301
#define USRTAG_ScreenHeight TAG_USER+302
#define USRTAG_LogonStack TAG_USER+303
#define USRTAG_Editor TAG_USER+304
#define USRTAG_Protocol TAG_USER+305
#define USRTAG_Archiver TAG_USER+306
#define USRTAG_HotKeys TAG_USER+307
#define USRTAG_Menus TAG_USER+308
/*#define USRTAG_MufflePages TAG_USER+309*/
#define USRTAG_UserMonitor TAG_USER+310
/*#define USRTAG_ChatRequest TAG_USER+311*/

#define USRTAG_Sessions TAG_USER+401
#define USRTAG_EmailRead TAG_USER+402
#define USRTAG_EmailWritten TAG_USER+403
#define USRTAG_MessagesRead TAG_USER+404
#define USRTAG_MessagesWritten TAG_USER+405
#define USRTAG_FilesUploaded TAG_USER+406
#define USRTAG_FilesDownloaded TAG_USER+407
#define USRTAG_FilesCredits TAG_USER+408
#define USRTAG_KilosUploaded TAG_USER+409
#define USRTAG_KilosDownloaded TAG_USER+410
#define USRTAG_KilosCredits TAG_USER+411
#define USRTAG_LastOn TAG_USER+412
#define USRTAG_TimeUsed TAG_USER+413

/*
** Messages
*/

/* Global */

#define MSGTAG_GLOBAL ((ULONG)(1L<<15))

/* Get/Set/Rem */
#define MSGTAG_ID TAG_USER+MSGTAG_GLOBAL+1

/* Get */
#define MSGTAG_Exists TAG_USER+MSGTAG_GLOBAL+2

/* Set */
#define MSGTAG_ForceSave TAG_USER+MSGTAG_GLOBAL+3

/* Set/Add/Rem */
#define MSGTAG_DontSave TAG_USER+MSGTAG_GLOBAL+4

/* Add */
#define MSGTAG_ReplyTo TAG_USER+MSGTAG_GLOBAL+5

/* Local */
#define MSGTAG_Thread TAG_USER+1
#define MSGTAG_From TAG_USER+2
#define MSGTAG_To TAG_USER+3
#define MSGTAG_Subject TAG_USER+4
#define MSGTAG_Date TAG_USER+5
#define MSGTAG_Text TAG_USER+6

/*
** Files
*/

/* Global */

#define FILTAG_GLOBAL ((ULONG)(1L<<15))

/* Get/Set/Rem */
#define FILTAG_ID TAG_USER+FILTAG_GLOBAL+1

/* Get */
#define FILTAG_Exists TAG_USER+FILTAG_GLOBAL+2

/* Set */
#define FILTAG_ForceSave TAG_USER+FILTAG_GLOBAL+3

/* Set/Add/Rem */
#define FILTAG_DontSave TAG_USER+FILTAG_GLOBAL+4

/* Local */
#define FILTAG_Name TAG_USER+1
#define FILTAG_Uploader TAG_USER+2
#define FILTAG_Size TAG_USER+3
#define FILTAG_Date TAG_USER+4
#define FILTAG_Downloads TAG_USER+5
#define FILTAG_LastDownloadDate TAG_USER+6
#define FILTAG_Description TAG_USER+7


/*
** Command Messages
*/

/* Node */

enum {
	NDCMD_Quit=0,
	NDCMD_Session,
	NDCMD_Panic,
	NDCMD_Page
};

#define NDSESSIONTAG_Baud TAG_USER+1

#define NDPAGETAG_FromNode TAG_USER+1
#define NDPAGETAG_Text TAG_USER+2

/* Console */

enum {
	CONCMD_Quit=0,
	CONCMD_Open,
	CONCMD_Close,
	CONCMD_Read,
	CONCMD_Write,
	CONCMD_Wait,
	CONCMD_Abort,
	CONCMD_Query
};

#define CONWRITETAG_Buffer TAG_USER+1
#define CONWRITETAG_Size TAG_USER+2

#define CONWAITTAG_WaitMask TAG_USER+1
#define CONWAITTAG_WaitMaskRet TAG_USER+2

/* Serial */

enum {
	SERCMD_Quit=0,
	SERCMD_Open,
	SERCMD_Close,
	SERCMD_Read,
	SERCMD_Write,
	SERCMD_Wait,
	SERCMD_Abort,
	SERCMD_SetBaud,
	SERCMD_Carrier,
	SERCMD_Query
};

#define SERREADTAG_Buffer TAG_USER+1
#define SERREADTAG_Size TAG_USER+2

#define SERWRITETAG_Buffer TAG_USER+1
#define SERWRITETAG_Size TAG_USER+2

#define SERWAITTAG_WaitMask TAG_USER+1
#define SERWAITTAG_WaitMaskRet TAG_USER+2
#define SERWAITTAG_Buffer TAG_USER+3
#define SERWAITTAG_Size TAG_USER+4

#define SERSETBAUDTAG_ResetBaud TAG_USER+1
#define SERSETBAUDTAG_NewBaud TAG_USER+2

/* Timer */

enum {
	TMRCMD_Quit=0,
	TMRCMD_Start,
	TMRCMD_Stop
};

#define TMRSTARTTAG_TimeRemaining TAG_USER+1
#define TMRSTARTTAG_TimeUsed TAG_USER+2

/* Update */

enum {
	UPDCMD_UpdateStatus=1,
	UPDCMD_StartupNode,
	UPDCMD_KillNode
};

/* Lib */
#define GLFLG_Edit (1<<0)
#define GLFLG_NoEcho (1<<1)
#define GLFLG_NoEmpty (1<<2)
#define GLFLG_Format (1<<3)
#define GLFLG_ToLower (1<<4)
#define GLFLG_ToUpper (1<<5)
#define GLFLG_Chars (1<<6)
#define GLFLG_Digits (1<<7)
#define GLFLG_NoChars (1<<8)
#define GLFLG_NoDigits (1<<9)
#define GLFLG_NoSpaces (1<<10)
#define GLFLG_NoPunct (1<<11)
#define GLFLG_BeginChar (1<<12)
#define GLFLG_BeginDigit (1<<13)

#endif
