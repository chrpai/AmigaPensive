struct StatusNode {
	struct Node sn_Node;
	struct SignalSemaphore sn_Semaphore;
	BYTE sn_NodeName[33];
	BYTE sn_UserName[33];
	BYTE sn_Location[33];
	BYTE sn_Prompt[256];
	BYTE sn_Stack[256];
	UWORD sn_TimeRemaining;
	UWORD sn_TimeUsed;
	UWORD sn_InactivityTimeLimit;
	ULONG sn_Flags;
};

#define STFLG_Panic (1<<0)
#define STFLG_Local (1<<1)
#define STFLG_ConsoleOpen (1<<2)
#define STFLG_ANSI (1<<3)
#define STFLG_PageReceived (1<<4)
#define STFLG_UserMonitor (1<<5)

/* tag attrs */
Tag st_get_tags[]={ STTAG_Name,STTAG_Exists,STTAG_Next,STTAG_NextReturn,TAG_END };
Tag st_set_tags[]={ STTAG_Name,TAG_END };
Tag st_add_tags[]={ STTAG_Name,TAG_END };
Tag st_rem_tags[]={ STTAG_Name,TAG_END };

Tag st_tags[]={
	STTAG_UserName,STTAG_Location,STTAG_Prompt,STTAG_Stack,STTAG_TimeRemaining,STTAG_TimeUsed,STTAG_InactivityTimeLimit,STTAG_Panic,STTAG_Local,STTAG_ConsoleOpen,STTAG_ANSI,STTAG_PageReceived,STTAG_UserMonitor,
	TAG_END
};
