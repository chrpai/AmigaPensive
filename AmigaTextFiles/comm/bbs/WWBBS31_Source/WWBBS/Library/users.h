struct UserNode {
	struct Node un_Node;
	struct SignalSemaphore un_Semaphore;
	BYTE un_UserName[33];
	BYTE un_RealName[31];
	BYTE un_UUCPName[9];
	struct TagItem *un_Offsets;
	struct TagItem *un_Pointers;
};

struct OffsetStruct {
	ULONG os_Type;
	ULONG os_Offset;
};

#define UNTAG_UserInfo TAG_USER+1

struct UserInfo {
	BYTE ui_Password[11];
	BYTE ui_Address[31];
	BYTE ui_City[21];
	BYTE ui_State[21];
	BYTE ui_Zip[11];
	BYTE ui_Country[4];
	BYTE ui_PhoneCode[7];
	BYTE ui_PhoneNumber[9];
	UBYTE ui_BirthDay;
	UBYTE ui_BirthMonth;
	UWORD ui_BirthYear;
};

#define UNTAG_UserAccess TAG_USER+2

struct UserAccess {
	UBYTE ua_AccessLevel;
	UWORD ua_SessionTimeLimit;
	UWORD ua_InactivityTimeLimit;
	UBYTE ua_RatioByte;
	UBYTE ua_RatioFile;
	ULONG ua_Days;
};

#define UNTAG_UserPrefs TAG_USER+3

struct UserPrefs {
	UBYTE up_ScreenWidth;
	UBYTE up_ScreenHeight;
	BYTE up_LogonStack[21];
	BYTE up_Editor[33];
	BYTE up_Protocol[33];
	BYTE up_Archiver[33];
	ULONG up_Flags;
};

#define UPFLG_HotKeys (1<<0)
#define UPFLG_NoMenus (1<<1)
/*#define UPFLG_MufflePages (1<<2)*/
#define UPFLG_NoUserMonitor (1<<3)
/*#define UPFLG_NoChatRequest (1<<4)*/

#define UNTAG_UserStats TAG_USER+4

struct UserStats {
	ULONG us_Sessions;
	ULONG us_EmailRead;
	ULONG us_EmailWritten;
	ULONG us_MessagesRead;
	ULONG us_MessagesWritten;
	ULONG us_FilesUploaded;
	ULONG us_FilesDownloaded;
	ULONG us_FilesCredits;
	ULONG us_KilosUploaded;
	ULONG us_KilosDownloaded;
	ULONG us_KilosCredits;
	struct DateStamp us_LastOn;
	UWORD us_TimeUsed;
};

Tag usernode_tags[]={ UNTAG_UserInfo,UNTAG_UserAccess,UNTAG_UserPrefs,UNTAG_UserStats,TAG_END };


/* file commands */

#define USRCMD_Begin 1
#define USRCMD_Field 2
#define USRCMD_End 3


/* field types */

#define USRFLD_UserName 1
#define USRFLD_RealName 2
#define USRFLD_UUCPName 3
#define USRFLD_Offset   4

#define USRFLD_Password 1
#define USRFLD_Address 2
#define USRFLD_City 3
#define USRFLD_State 4
#define USRFLD_Zip 5
#define USRFLD_Country 6
#define USRFLD_PhoneCode 7
#define USRFLD_PhoneNumber 8
#define USRFLD_BirthDay 9
#define USRFLD_BirthMonth 10
#define USRFLD_BirthYear 11

#define USRFLD_AccessLevel 1
#define USRFLD_SessionTimeLimit 2
#define USRFLD_InactivityTimeLimit 3
#define USRFLD_RatioByte 4
#define USRFLD_RatioFile 5
#define USRFLD_Days 6

#define USRFLD_ScreenWidth 1
#define USRFLD_ScreenHeight 2
#define USRFLD_LogonStack 3
#define USRFLD_Editor 4
#define USRFLD_Protocol 5
#define USRFLD_Archiver 6
#define USRFLD_Flags 7

#define USRFLD_Sessions 1
#define USRFLD_EmailRead 2
#define USRFLD_EmailWritten 3
#define USRFLD_MessagesRead 4
#define USRFLD_MessagesWritten 5
#define USRFLD_FilesUploaded 6
#define USRFLD_FilesDownloaded 7
#define USRFLD_FilesCredits 8
#define USRFLD_KilosUploaded 9
#define USRFLD_KilosDownloaded 10
#define USRFLD_KilosCredits 11
#define USRFLD_LastOn 12
#define USRFLD_TimeUsed 13

/* tag attrs */
Tag usr_get_tags[]={ USRTAG_Name,USRTAG_UserName,USRTAG_RealName,USRTAG_UUCPName,USRTAG_Exists,USRTAG_Next,USRTAG_NextReturn,TAG_END };
Tag usr_set_tags[]={ USRTAG_Name,USRTAG_UserName,USRTAG_RealName,USRTAG_UUCPName,USRTAG_ForceSave,USRTAG_DontSave,TAG_END };
Tag usr_add_tags[]={ USRTAG_UserName,USRTAG_RealName,USRTAG_UUCPName,USRTAG_DontSave,TAG_END };
Tag usr_rem_tags[]={ USRTAG_Name,USRTAG_DontSave,TAG_END };
Tag usr_load_tags[]={ USRTAG_Name,TAG_END };
Tag usr_unload_tags[]={ USRTAG_Name,TAG_END };

Tag usrinfo_tags[]={ USRTAG_Password,USRTAG_Address,USRTAG_City,USRTAG_State,USRTAG_Zip,USRTAG_Country,USRTAG_PhoneCode,USRTAG_PhoneNumber,USRTAG_BirthDay,USRTAG_BirthMonth,USRTAG_BirthYear,TAG_END };
Tag usraccess_tags[]={ USRTAG_AccessLevel,USRTAG_SessionTimeLimit,USRTAG_InactivityTimeLimit,USRTAG_RatioByte,USRTAG_RatioFile,USRTAG_Days,TAG_END };
Tag usrprefs_tags[]={ USRTAG_ScreenWidth,USRTAG_ScreenHeight,USRTAG_LogonStack,USRTAG_Editor,USRTAG_Protocol,USRTAG_Archiver,USRTAG_HotKeys,USRTAG_Menus,USRTAG_UserMonitor,TAG_END };
Tag usrstats_tags[]={ USRTAG_Sessions,USRTAG_EmailRead,USRTAG_EmailWritten,USRTAG_MessagesRead,USRTAG_MessagesWritten,USRTAG_FilesUploaded,USRTAG_FilesDownloaded,USRTAG_FilesCredits,USRTAG_KilosUploaded,USRTAG_KilosDownloaded,USRTAG_KilosCredits,USRTAG_LastOn,USRTAG_TimeUsed,TAG_END };
