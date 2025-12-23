struct Library *WorldWideBase;
struct Library *RexxSysBase;

BOOL panic=FALSE;

void ABBEREXXCommand(BYTE *,UWORD,BYTE *,char *,char **,char **,long *);
void SystemCommand(BYTE *,UWORD,BYTE *,char *,char **,char **,long *);
void EmailCommand(BYTE *,UWORD,BYTE *);
void MessageBaseCommand(BYTE *,UWORD,BYTE *);
void FileBaseCommand(BYTE *,UWORD,BYTE *);

BOOL GetNextArea(BYTE *,BYTE *);
BOOL GetPreviousArea(BYTE *,BYTE *);

struct Command {
	UBYTE cmd_Group;
	BYTE *cmd_Name;
	UWORD cmd_ID;
};

enum {
	GROUP_ABBEREXX=1,
	GROUP_System,
	GROUP_Email,
	GROUP_MessageBase,
	GROUP_FileBase
};

enum {
	ABBEREXX_BBSIdentify=1,
	ABBEREXX_CheckAbort,
	ABBEREXX_CLS,
	ABBEREXX_GetCarrier,
	ABBEREXX_GetChar,
	ABBEREXX_GetUser,
	ABBEREXX_HangUp,
	ABBEREXX_IReady,
	ABBEREXX_MayGetChar,
	ABBEREXX_NewLine,
	ABBEREXX_Print,
	ABBEREXX_Prompt,
	ABBEREXX_Query,
	ABBEREXX_Receive,
	ABBEREXX_Send,
	ABBEREXX_SendFile,
	ABBEREXX_SetNodeLocation,
	ABBEREXX_SysopLog
};

enum {
	SYS_ANSIDetect=1,
	SYS_ChatRequest,
	SYS_Delay,
	SYS_DoLogoff,
	SYS_DoLogon,
	SYS_DosCommand,
	SYS_EditPrefs,
	SYS_EditUser,
	SYS_Logon,
	SYS_PageUser,
	SYS_ReadNews,
	SYS_Register,
	SYS_RunMenu,
	SYS_ShowStats,
	SYS_StartTimer,
	SYS_StopTimer,
	SYS_UserEditor,
	SYS_UserList,
	SYS_ValidateNewUsers,
	SYS_WhoIsOnline
};

enum {
	EMAIL_Next=1,
	EMAIL_Current,
	EMAIL_Previous,
	EMAIL_JumpTo,
	EMAIL_List,
	EMAIL_Write,
	EMAIL_ReplyTo,
	EMAIL_Delete,
	EMAIL_Feedback,
	EMAIL_ShowNew
};

enum {
	MSG_InitGlobal=1,
	MSG_EditSearchPatterns,
	MSG_JoinArea,
	MSG_ChangeArea,
	MSG_ParentArea,
	MSG_NextArea,
	MSG_PreviousArea,
	MSG_NextUnread,
	MSG_Current,
	MSG_NextThread,
	MSG_PreviousThread,
	MSG_JumpToThread,
	MSG_ListThreads,
	MSG_NextResponse,
	MSG_PreviousResponse,
	MSG_JumpToResponse,
	MSG_ListResponses,
	MSG_SearchNext,
	MSG_Catchup,
	MSG_Write,
	MSG_ReplyTo,
	MSG_MailReplyTo,
	MSG_Kill,
	MSG_EditAreaDescription
};

enum {
	FILE_InitGlobal=1,
	FILE_EditSearchPatterns,
	FILE_JoinArea,
	FILE_ChangeArea,
	FILE_ParentArea,
	FILE_NextArea,
	FILE_PreviousArea,
	FILE_Next,
	FILE_Current,
	FILE_Previous,
	FILE_Jump,
	FILE_List,
	FILE_SearchNext,
	FILE_Catchup,
	FILE_Upload,
	FILE_Download,
	FILE_ClearTagList,
	FILE_EditTagList,
	FILE_Tag,
	FILE_ViewArchive,
	FILE_ReadTextFile,
	FILE_UploadFromDisk,
	FILE_Kill,
	FILE_Move,
	FILE_EditDescription,
	FILE_EditAreaDescription
};

struct Command cmds[]={
	{ GROUP_ABBEREXX,"BBSIDENTIFY",ABBEREXX_BBSIdentify },
	{ GROUP_ABBEREXX,"CHECKABORT",ABBEREXX_CheckAbort },
	{ GROUP_ABBEREXX,"CLS",ABBEREXX_CLS },
	{ GROUP_ABBEREXX,"GETCARRIER",ABBEREXX_GetCarrier },
	{ GROUP_ABBEREXX,"GETCHAR",ABBEREXX_GetChar },
	{ GROUP_ABBEREXX,"GETUSER",ABBEREXX_GetUser },
	{ GROUP_ABBEREXX,"HANGUP",ABBEREXX_HangUp },
	{ GROUP_ABBEREXX,"IREADY",ABBEREXX_IReady },
	{ GROUP_ABBEREXX,"CHECKIO",ABBEREXX_IReady },
	{ GROUP_ABBEREXX,"MAYGETCHAR",ABBEREXX_MayGetChar },
	{ GROUP_ABBEREXX,"NEWLINE",ABBEREXX_NewLine },
	{ GROUP_ABBEREXX,"PRINT",ABBEREXX_Print },
	{ GROUP_ABBEREXX,"PROMPT",ABBEREXX_Prompt },
	{ GROUP_ABBEREXX,"QUERY",ABBEREXX_Query },
	{ GROUP_ABBEREXX,"RECEIVE",ABBEREXX_Receive },
	{ GROUP_ABBEREXX,"SEND",ABBEREXX_Send },
	{ GROUP_ABBEREXX,"SENDSTRING",ABBEREXX_Send },
	{ GROUP_ABBEREXX,"SENDFILE",ABBEREXX_SendFile },
	{ GROUP_ABBEREXX,"SETNODELOCATION",ABBEREXX_SetNodeLocation },
	{ GROUP_ABBEREXX,"SYSOPLOG",ABBEREXX_SysopLog },
	{ GROUP_ABBEREXX,"LOGENTRY",ABBEREXX_SysopLog },

	{ GROUP_System,"ANSIDETECT",SYS_ANSIDetect },
	{ GROUP_System,"CHATREQUEST",SYS_ChatRequest },
	{ GROUP_System,"DELAY",SYS_Delay },
	{ GROUP_System,"DOLOGOFF",SYS_DoLogoff },
	{ GROUP_System,"DOLOGON",SYS_DoLogon },
	{ GROUP_System,"DOSCOMMAND",SYS_DosCommand },
	{ GROUP_System,"EDITPREFS",SYS_EditPrefs },
	{ GROUP_System,"EDITUSER",SYS_EditUser },
	{ GROUP_System,"LOGON",SYS_Logon },
	{ GROUP_System,"PAGEUSER",SYS_PageUser },
	{ GROUP_System,"READNEWS",SYS_ReadNews },
	{ GROUP_System,"REGISTER",SYS_Register },
	{ GROUP_System,"RUNMENU",SYS_RunMenu },
	{ GROUP_System,"SHOWSTATS",SYS_ShowStats },
	{ GROUP_System,"STARTTIMER",SYS_StartTimer },
	{ GROUP_System,"STOPTIMER",SYS_StopTimer },
	{ GROUP_System,"USEREDITOR",SYS_UserEditor },
	{ GROUP_System,"USERLIST",SYS_UserList },
	{ GROUP_System,"VALIDATENEWUSERS",SYS_ValidateNewUsers },
	{ GROUP_System,"WHOISONLINE",SYS_WhoIsOnline },

	{ GROUP_Email,"NEXTEMAIL",EMAIL_Next },
	{ GROUP_Email,"CURRENTEMAIL",EMAIL_Current },
	{ GROUP_Email,"PREVIOUSEMAIL",EMAIL_Previous },
	{ GROUP_Email,"JUMPTOEMAIL",EMAIL_JumpTo },
	{ GROUP_Email,"LISTEMAIL",EMAIL_List },
	{ GROUP_Email,"WRITEEMAIL",EMAIL_Write },
	{ GROUP_Email,"REPLYTOEMAIL",EMAIL_ReplyTo },
	{ GROUP_Email,"DELETEEMAIL",EMAIL_Delete },
	{ GROUP_Email,"FEEDBACK",EMAIL_Feedback },
	{ GROUP_Email,"SHOWNEWEMAIL",EMAIL_ShowNew },

	{ GROUP_MessageBase,"INITMESSAGEGLOBAL",MSG_InitGlobal },
	{ GROUP_MessageBase,"EDITMESSAGESEARCHPATTERNS",MSG_EditSearchPatterns },
	{ GROUP_MessageBase,"JOINMESSAGEAREA",MSG_JoinArea },
	{ GROUP_MessageBase,"CHANGEMESSAGEAREA",MSG_ChangeArea },
	{ GROUP_MessageBase,"PARENTMESSAGEAREA",MSG_ParentArea },
	{ GROUP_MessageBase,"NEXTMESSAGEAREA",MSG_NextArea },
	{ GROUP_MessageBase,"PREVIOUSMESSAGEAREA",MSG_PreviousArea },
	{ GROUP_MessageBase,"NEXTUNREADMESSAGE",MSG_NextUnread },
	{ GROUP_MessageBase,"CURRENTMESSAGE",MSG_Current },
	{ GROUP_MessageBase,"NEXTTHREAD",MSG_NextThread },
	{ GROUP_MessageBase,"PREVIOUSTHREAD",MSG_PreviousThread },
	{ GROUP_MessageBase,"JUMPTOTHREAD",MSG_JumpToThread },
	{ GROUP_MessageBase,"LISTTHREADS",MSG_ListThreads },
	{ GROUP_MessageBase,"NEXTRESPONSE",MSG_NextResponse },
	{ GROUP_MessageBase,"PREVIOUSRESPONSE",MSG_PreviousResponse },
	{ GROUP_MessageBase,"JUMPTORESPONSE",MSG_JumpToResponse },
	{ GROUP_MessageBase,"LISTRESPONSES",MSG_ListResponses },
	{ GROUP_MessageBase,"SEARCHNEXTMESSAGE",MSG_SearchNext },
	{ GROUP_MessageBase,"CATCHUPMESSAGES",MSG_Catchup },
	{ GROUP_MessageBase,"WRITEMESSAGE",MSG_Write },
	{ GROUP_MessageBase,"REPLYTOMESSAGE",MSG_ReplyTo },
	{ GROUP_MessageBase,"MAILREPLYTOMESSAGE",MSG_MailReplyTo },
	{ GROUP_MessageBase,"KILLMESSAGE",MSG_Kill },
	{ GROUP_MessageBase,"EDITMESSAGEAREADESCRIPTION",MSG_EditAreaDescription },

	{ GROUP_FileBase,"INITFILEGLOBAL",FILE_InitGlobal },
	{ GROUP_FileBase,"EDITFILESEARCHPATTERNS",FILE_EditSearchPatterns },
	{ GROUP_FileBase,"JOINFILEAREA",FILE_JoinArea },
	{ GROUP_FileBase,"CHANGEFILEAREA",FILE_ChangeArea },
	{ GROUP_FileBase,"PARENTFILEAREA",FILE_ParentArea },
	{ GROUP_FileBase,"NEXTFILEAREA",FILE_NextArea },
	{ GROUP_FileBase,"PREVIOUSFILEAREA",FILE_PreviousArea },
	{ GROUP_FileBase,"NEXTFILE",FILE_Next },
	{ GROUP_FileBase,"CURRENTFILE",FILE_Current },
	{ GROUP_FileBase,"PREVIOUSFILE",FILE_Previous },
	{ GROUP_FileBase,"JUMPTOFILE",FILE_Jump },
	{ GROUP_FileBase,"LISTFILES",FILE_List },
	{ GROUP_FileBase,"SEARCHNEXTFILE",FILE_SearchNext },
	{ GROUP_FileBase,"CATCHUPFILES",FILE_Catchup },
	{ GROUP_FileBase,"UPLOAD",FILE_Upload },
	{ GROUP_FileBase,"DOWNLOAD",FILE_Download },
	{ GROUP_FileBase,"CLEARFILETAGLIST",FILE_ClearTagList },
	{ GROUP_FileBase,"EDITFILETAGLIST",FILE_EditTagList },
	{ GROUP_FileBase,"TAGFILE",FILE_Tag },
	{ GROUP_FileBase,"VIEWARCHIVE",FILE_ViewArchive },
	{ GROUP_FileBase,"READTEXTFILE",FILE_ReadTextFile },
	{ GROUP_FileBase,"UPLOADFROMDISK",FILE_UploadFromDisk },
	{ GROUP_FileBase,"KILLFILE",FILE_Kill },
	{ GROUP_FileBase,"MOVEFILE",FILE_Move },
	{ GROUP_FileBase,"EDITFILEDESCRIPTION",FILE_EditDescription },
	{ GROUP_FileBase,"EDITFILEAREADESCRIPTION",FILE_EditAreaDescription },

	{ NULL,NULL,NULL }
};

/* email */
ULONG email_current;
ULONG email_promptcurrent;
ULONG email_prompthigh;

/* message bases */
struct List *message_global;
BYTE message_fullpath[256];
BYTE message_path[256];
BYTE message_area[33];
BYTE message_group[21];
ULONG message_current;
BYTE message_read[256];
ULONG message_currentthread;
ULONG message_promptlowthread;
ULONG message_prompthighthread;
ULONG message_promptcurrentresponse;
ULONG message_prompthighresponse;
BYTE message_frompattern[41];
BYTE message_topattern[41];
BYTE message_subjectpattern[41];
BYTE message_textpattern[41];

/* file bases */
struct List *file_global;
struct List *file_taglist;
BYTE file_fullpath[256];
BYTE file_path[256];
BYTE file_area[33];
BYTE file_group[41];
ULONG file_current;
ULONG file_high;
ULONG file_promptlow;
ULONG file_prompthigh;
BYTE file_filepattern[41];
BYTE file_descriptionpattern[41];
BYTE file_uploaderpattern[41];
