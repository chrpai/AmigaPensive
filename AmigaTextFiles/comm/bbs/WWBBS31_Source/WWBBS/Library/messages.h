struct MessageGroupNode {
	struct Node mgn_Node;
	struct SignalSemaphore mgn_Semaphore;
	BYTE mgn_Name[41];
	struct List mgn_Messages;
	ULONG mgn_MessageCount;
	ULONG mgn_ThreadCount;
	struct SignalSemaphore mgn_UsageCountSemaphore;
	UWORD mgn_UsageCount;
};

struct MessageNode {
	struct Node mn_Node;
	ULONG mn_ID;
	ULONG mn_Thread;
	BYTE mn_From[33];
	BYTE mn_To[33];
	BYTE mn_Subject[65];
	struct DateStamp mn_Date;
	BYTE *mn_Text;
};

/* message commands */

#define MSGCMD_Begin 1
#define MSGCMD_Field 2
#define MSGCMD_End   3


/* field types */

#define MSGFLD_ID 1
#define MSGFLD_Thread 2
#define MSGFLD_From 3
#define MSGFLD_To 4
#define MSGFLD_Subject 5
#define MSGFLD_Date 6
#define MSGFLD_Text 7


/* tag attrs */
Tag msg_get_tags[]={ MSGTAG_ID,MSGTAG_Exists,TAG_END };
Tag msg_set_tags[]={ MSGTAG_ID,MSGTAG_ForceSave,MSGTAG_DontSave,TAG_END };
Tag msg_add_tags[]={ MSGTAG_DontSave,MSGTAG_ReplyTo,TAG_END };
Tag msg_rem_tags[]={ MSGTAG_ID,MSGTAG_DontSave,TAG_END };
Tag msg_tags[]={ MSGTAG_Thread,MSGTAG_From,MSGTAG_To,MSGTAG_Subject,MSGTAG_Date,MSGTAG_Text,TAG_END };
