struct ConfigNode {
	struct Node cn_Node;
	struct SignalSemaphore cn_Semaphore;
	UBYTE cn_Type;
	struct Node *cn_Parent;
	struct List *cn_Child;
	BYTE cn_Name[33];
};


/* file commands */

#define CFGCMD_Begin  1
#define CFGCMD_Child  2
#define CFGCMD_End    3
#define CFGCMD_Field  4
#define CFGCMD_Parent 5


/* field types */

#define CFGFLD_Name 0

#define AGFLD_AccessLevel 1
#define AGFLD_SessionTimeLimit 3
#define AGFLD_InactivityTimeLimit 4
#define AGFLD_RatioByte 5
#define AGFLD_RatioFile 6
#define AGFLD_Days 7

#define ARCFLD_Extension 1
#define ARCFLD_AddCommand 2
#define ARCFLD_ExtractCommand 3
#define ARCFLD_ViewCommand 4

#define EDFLD_Type 1
#define EDFLD_Command 2
#define EDFLD_Stack 3

#define FBFLD_AccessRange 1
#define FBFLD_Group 2
#define FBFLD_Directory 3
#define FBFLD_DiskSpaceRequired 4
#define FBFLD_Days 5
#define FBFLD_Flags 6

#define MNFLD_AccessRange 1
#define MNFLD_Character 2
#define MNFLD_Default 3
#define MNFLD_Number 4
#define MNFLD_Buffer 5

#define MBFLD_AccessRange 1
/*#define MBFLD_Group 2*/
#define MBFLD_QuoteHeader 3
#define MBFLD_Flags 4
#define MBFLD_Days 5
#define MBFLD_Group 6

#define NWSFLD_AccessRange 1
#define NWSFLD_File 2

#define NDFLD_AccessRange 1
#define NDFLD_Type 2
#define NDFLD_Device 3
#define NDFLD_Unit 4
#define NDFLD_BufferSize 5
#define NDFLD_Baud 6
#define NDFLD_Flags 7
#define NDFLD_Command 8

#define PRFLD_SendCommand 1
#define PRFLD_ReceiveCommand 2
#define PRFLD_Flags 3

/*#define SYSFLD_DisplayMode 1*/
/*#define SYSFLD_FontName 2*/
/*#define SYSFLD_FontSize 3*/
/*#define SYSFLD_FontFlags 4*/
#define SYSFLD_InputColor 5
#define SYSFLD_HeaderColor 6
#define SYSFLD_OutputColor 7
#define SYSFLD_PromptColor 8
#define SYSFLD_SystemColor 9
/*#define SYSFLD_LogDays 10*/
/*#define SYSFLD_OfflineDirectory 11*/
#define SYSFLD_Flags 12
#define SYSFLD_FileIDDizExtractor 13


/* structures */

struct AccessGroupNode {
	struct ConfigNode agn_Node;
	UBYTE agn_AccessLevel;
	UWORD agn_SessionTimeLimit;
	UWORD agn_InactivityTimeLimit;
	UBYTE agn_RatioByte;
	UBYTE agn_RatioFile;
	ULONG agn_Days;
};

struct ArchiverNode {
	struct ConfigNode an_Node;
	BYTE an_Extension[4];
	BYTE an_AddCommand[256];
	BYTE an_ExtractCommand[256];
	BYTE an_ViewCommand[256];
};

struct EditorNode {
	struct ConfigNode en_Node;
	UBYTE en_Type;
	BYTE en_Command[256];
	ULONG en_Stack;
};

struct FileBaseNode {
	struct ConfigNode fbn_Node;
	BYTE fbn_AccessRange[21];
	BYTE fbn_Group[41];
	BYTE fbn_Directory[256];
	ULONG fbn_DiskSpaceRequired;
	ULONG fbn_Days;
	ULONG fbn_Flags;
};

#define FBFLG_ReadOnly (1<<0)
#define FBFLG_AutoJoin (1<<1)

struct MenuNode {
	struct ConfigNode mn_Node;
	BYTE mn_AccessRange[21];
	BYTE mn_Character;
	BYTE mn_Default;
	BYTE mn_Number;
	BYTE mn_Buffer[64]; /* prompt/cmd */
};

struct MessageBaseNode {
	struct ConfigNode mbn_Node;
	BYTE mbn_AccessRange[21];
	BYTE mbn_Group[21];
	BYTE mbn_QuoteHeader[81];
	ULONG mbn_Days;
	ULONG mbn_Flags;
};

#define MBFLG_ReadOnly (1<<0)
#define MBFLG_AutoJoin (1<<1)

struct NewsNode {
	struct ConfigNode nn_Node;
	BYTE nn_AccessRange[21];
	BYTE nn_File[256];
};

struct NodeNode {
	struct ConfigNode nn_Node;
	BYTE nn_AccessRange[21];
	UBYTE nn_Type;
	BYTE nn_Device[33];
	ULONG nn_Unit;
	ULONG nn_BufferSize;
	ULONG nn_Baud;
	ULONG nn_Flags;
	BYTE nn_Command[256];
};

#define NDFLG_Handshaking (1<<0)
#define NDFLG_AutoBaud (1<<1)
#define NDFLG_FrontEnd (1<<2)

struct ProtocolNode {
	struct ConfigNode pn_Node;
	BYTE pn_SendCommand[256];
	BYTE pn_ReceiveCommand[256];
	ULONG pn_Flags;
};

#define PRFLG_Batch (1<<0)
#define PRFLG_Bidirectional (1<<1)

struct SystemNode {
	struct ConfigNode sn_Node;
	ULONG sn_DisplayMode;
	BYTE sn_FontName[33];
	UWORD sn_FontSize;
	UBYTE sn_FontFlags;
	BYTE sn_InputColor[81];
	BYTE sn_HeaderColor[81];
	BYTE sn_OutputColor[81];
	BYTE sn_PromptColor[81];
	BYTE sn_SystemColor[81];
/*	ULONG sn_LogDays;*/
/*	BYTE sn_OfflineDirectory[256];*/
	BYTE sn_FileIDDizExtractor[81];
	ULONG sn_Flags;
};

#define SYSFLG_OnlyRealNames (1<<0)

/* tag attrs */

Tag cfg_get_tags[]={ CFGTAG_Path,CFGTAG_Name,CFGTAG_Type,CFGTAG_Parent,CFGTAG_Child,CFGTAG_GetFirst,CFGTAG_GetLast,CFGTAG_GetNext,CFGTAG_GetPrevious,CFGTAG_Next,CFGTAG_NextReturn,TAG_END };
Tag cfg_set_tags[]={ CFGTAG_Path,CFGTAG_Name,CFGTAG_NewName,CFGTAG_MoveUp,CFGTAG_MoveDown,CFGTAG_ForceSave,CFGTAG_DontSave,TAG_END };
Tag cfg_add_tags[]={ CFGTAG_Path,CFGTAG_Name,CFGTAG_Type,CFGTAG_DontSave,TAG_END };
Tag cfg_rem_tags[]={ CFGTAG_Path,CFGTAG_Name,CFGTAG_DontSave,TAG_END };

Tag accessgroup_tags[]={ AGTAG_AccessLevel,AGTAG_SessionTimeLimit,AGTAG_InactivityTimeLimit,AGTAG_RatioByte,AGTAG_RatioFile,AGTAG_Days,TAG_END };
Tag archiver_tags[]={ ARCTAG_Extension,ARCTAG_AddCommand,ARCTAG_ExtractCommand,ARCTAG_ViewCommand,TAG_END };
Tag editor_tags[]={ EDTAG_Type,EDTAG_Command,EDTAG_Stack,TAG_END };
Tag filebase_tags[]={ FBTAG_AccessRange,FBTAG_Group,FBTAG_Directory,FBTAG_DiskSpaceRequired,FBTAG_Days,FBTAG_ReadOnly,FBTAG_AutoJoin,TAG_END };
Tag menu_tags[]={ MNTAG_AccessRange,MNTAG_Character,MNTAG_Default,MNTAG_Number,MNTAG_Prompt,MNTAG_Command,TAG_END };
Tag messagebase_tags[]={ MBTAG_AccessRange,MBTAG_Group,MBTAG_QuoteHeader,MBTAG_Days,MBTAG_ReadOnly,MBTAG_AutoJoin,TAG_END };
Tag news_tags[]={ NWSTAG_AccessRange,NWSTAG_File,TAG_END };
Tag node_tags[]={ NDTAG_AccessRange,NDTAG_Type,NDTAG_Device,NDTAG_Unit,NDTAG_BufferSize,NDTAG_Baud,NDTAG_Handshaking,NDTAG_AutoBaud,NDTAG_FrontEnd,NDTAG_Command,TAG_END };
Tag protocol_tags[]={ PRTAG_SendCommand,PRTAG_ReceiveCommand,PRTAG_Batch,PRTAG_Bidirectional,TAG_END };
Tag system_tags[]={ SYSTAG_InputColor,SYSTAG_HeaderColor,SYSTAG_OutputColor,SYSTAG_PromptColor,SYSTAG_SystemColor,SYSTAG_OnlyRealNames,SYSTAG_FileIDDizExtractor,TAG_END };
