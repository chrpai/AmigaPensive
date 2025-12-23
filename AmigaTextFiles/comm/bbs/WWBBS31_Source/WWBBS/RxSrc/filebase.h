struct FileGlobalNode {
	struct Node fgn_Node;
	BYTE fgn_Name[41];
	ULONG fgn_High;
};

struct FileTagNode {
	struct Node ftn_Node;
	BYTE ftn_Path[256];
	BYTE ftn_Area[33];
	ULONG ftn_ID;
	BYTE ftn_Name[256];
	ULONG ftn_Size;
};

struct FileTagData {
	BYTE ftd_Path[256];
	BYTE ftd_Area[33];
	ULONG ftd_ID;
	BYTE ftd_Name[256];
	ULONG ftd_Size;
};

void InitFileGlobal(BYTE *);
void ChangeFileDisplay(BYTE *);
void EditFileSearchPatterns(BYTE *);

struct List *OpenFileGlobal(BYTE *);
void CloseFileGlobal(struct List *);
BOOL AddFileGlobal(struct List *,BYTE *,ULONG);
BOOL DeleteFileGlobal(struct List *,BYTE *);
ULONG GetFileGlobalHigh(struct List *,BYTE *);
BOOL SetFileGlobalHigh(struct List *,BYTE *,ULONG);
struct List *OpenFileTagList(BYTE *);
void CloseFileTagList(struct List *);
BOOL AddFileTag(struct List *,BYTE *,BYTE *,ULONG,BYTE *,ULONG);
BOOL DeleteFileTag(struct List *,BYTE *);
BYTE *GetFileTagPath(struct List *,BYTE *);
BYTE *GetFileTagArea(struct List *,BYTE *);
ULONG GetFileTagID(struct List *,BYTE *);
ULONG GetFileTagSize(struct List *,BYTE *);
int SelectFileArea(BYTE *,UBYTE,BYTE *);
void GetFileDescriptions(BYTE *);
void ProcessLogFile(BYTE *);
void FileUpdateVars(BYTE *);

void JoinFileArea(BYTE *);
void ChangeFileArea(BYTE *);
void ParentFileArea(BYTE *);
void NextFileArea(BYTE *);
void PreviousFileArea(BYTE *);
BOOL DisplayArea(BYTE *,APTR);
void SetFileDoorVariables(BYTE *);

void NextFile(BYTE *);
void CurrentFile(BYTE *);
void PreviousFile(BYTE *);
void JumptoFile(BYTE *);
void ListFiles(BYTE *);
void SearchNext(BYTE *);
void CatchupFiles(BYTE *);
BOOL DisplayFile(BYTE *,APTR);

void ViewArchive(BYTE *);
void ReadTextFile(BYTE *);

void ClearFileTagList(BYTE *);
void EditFileTagList(BYTE *);
void TagFile(BYTE *);

void Upload(BYTE *);
void Download(BYTE *);
ULONG CreateFileList(BYTE *);
void DeleteFileList(BYTE *);
void DoTransfer(BYTE *,BYTE *,BYTE *);

void UploadFromDisk(BYTE *);
void KillFile(BYTE *);
void MoveFile(BYTE *);
void EditFileDescription(BYTE *);
