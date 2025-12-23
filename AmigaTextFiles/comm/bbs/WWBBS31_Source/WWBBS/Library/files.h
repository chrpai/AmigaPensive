struct FileGroupNode {
	struct Node fgn_Node;
	struct SignalSemaphore fgn_Semaphore;
	BYTE fgn_Name[41];
	BPTR fgn_Directory;
	struct List fgn_Files;
	ULONG fgn_FileCount;
	struct SignalSemaphore fgn_UsageCountSemaphore;
	UWORD fgn_UsageCount;
};

struct FileNode {
	struct Node fn_Node;
	ULONG fn_ID;
	BYTE fn_Name[33];
	BYTE fn_Uploader[33];
	ULONG fn_Size;
	struct DateStamp fn_Date;
	UWORD fn_Downloads;
	struct DateStamp fn_LastDownloadDate;
	BYTE *fn_Description;
};


/* file commands */

#define FILCMD_Begin 1
#define FILCMD_Field 2
#define FILCMD_End   3


/* field types */

#define FILFLD_ID 1
#define FILFLD_Name 2
#define FILFLD_Uploader 3
#define FILFLD_Size 4
#define FILFLD_Date 5
#define FILFLD_Downloads 6
#define FILFLD_LastDownloadDate 7
#define FILFLD_Description 8


/* tag attrs */
Tag file_get_tags[]={ FILTAG_ID,FILTAG_Exists,TAG_END };
Tag file_set_tags[]={ FILTAG_ID,FILTAG_ForceSave,FILTAG_DontSave,TAG_END };
Tag file_add_tags[]={ FILTAG_DontSave,TAG_END };
Tag file_rem_tags[]={ FILTAG_ID,FILTAG_DontSave,TAG_END };
Tag file_tags[]={ FILTAG_Name,FILTAG_Uploader,FILTAG_Size,FILTAG_Date,FILTAG_Downloads,FILTAG_LastDownloadDate,FILTAG_Description,TAG_END };
