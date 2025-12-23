#ifndef  CLIB_WWBBS_PROTOS_H
#define  CLIB_WWBBS_PROTOS_H

/*
**  $VER: wwbbs_protos.h 3.0 (1.4.94)
*/

ULONG GetConfig(struct TagItem *);
ULONG GetConfigTags(Tag,...);
ULONG SetConfig(struct TagItem *);
ULONG SetConfigTags(Tag,...);
BOOL AddConfig(struct TagItem *);
BOOL AddConfigTags(Tag,...);
BOOL RemConfig(struct TagItem *);
BOOL RemConfigTags(Tag,...);

ULONG GetStatus(struct TagItem *);
ULONG GetStatusTags(Tag,...);
ULONG SetStatus(struct TagItem *);
ULONG SetStatusTags(Tag,...);
BOOL AddStatus(struct TagItem *);
BOOL AddStatusTags(Tag,...);
BOOL RemStatus(struct TagItem *);
BOOL RemStatusTags(Tag,...);

ULONG GetUser(struct TagItem *);
ULONG GetUserTags(Tag,...);
ULONG SetUser(struct TagItem *);
ULONG SetUserTags(Tag,...);
BOOL AddUser(struct TagItem *);
BOOL AddUserTags(Tag,...);
BOOL RemUser(struct TagItem *);
BOOL RemUserTags(Tag,...);
BOOL LoadUser(struct TagItem *);
BOOL LoadUserTags(Tag,...);
VOID UnLoadUser(struct TagItem *);
VOID UnLoadUserTags(Tag,...);

APTR OpenMessageGroup(BYTE *,BYTE *,LONG);
void CloseMessageGroup(APTR);
ULONG GetMessage(APTR,struct TagItem *);
ULONG GetMessageTags(APTR,Tag,...);
ULONG SetMessage(APTR,struct TagItem *);
ULONG SetMessageTags(APTR,Tag,...);
ULONG AddMessage(APTR,struct TagItem *);
ULONG AddMessageTags(APTR,Tag,...);
BOOL RemMessage(APTR,struct TagItem *);
BOOL RemMessageTags(APTR,Tag,...);
ULONG GetNextMessage(APTR,ULONG);
ULONG GetPreviousMessage(APTR,ULONG);

APTR OpenFileGroup(BYTE *,BYTE *,LONG);
void CloseFileGroup(APTR);
BOOL GetFileGroupDirectory(APTR,BYTE *,LONG);
ULONG GetFile(APTR,struct TagItem *);
ULONG GetFileTags(APTR,Tag,...);
ULONG SetFile(APTR,struct TagItem *);
ULONG SetFileTags(APTR,Tag,...);
ULONG AddFile(APTR,struct TagItem *);
ULONG AddFileTags(APTR,Tag,...);
ULONG RemFile(APTR,struct TagItem *);
ULONG RemFileTags(APTR,Tag,...);
ULONG GetNextFile(APTR,ULONG);
ULONG GetPreviousFile(APTR,ULONG);

ULONG NodeCommand(BYTE *,UBYTE,struct TagItem *);
ULONG NodeCommandTags(BYTE *,UBYTE,Tag,...);
ULONG ConsoleCommand(BYTE *,UBYTE,struct TagItem *);
ULONG ConsoleCommandTags(BYTE *,UBYTE,Tag,...);
ULONG SerialCommand(BYTE *,UBYTE,struct TagItem *);
ULONG SerialCommandTags(BYTE *,UBYTE,Tag,...);
ULONG TimerCommand(BYTE *,UBYTE,struct TagItem *);
ULONG TimerCommandTags(BYTE *,UBYTE,Tag,...);

void SetUpdatePort(struct MsgPort *);
void SendUpdate(UBYTE,ULONG);

void LogEntry(BYTE *,BYTE *,BYTE *);
BOOL IsRange(BYTE *,ULONG);

BOOL Ask(BYTE *,BOOL);
BOOL GetLine(BYTE *,UWORD,ULONG);
void ShowText(BYTE *);
BOOL RunEditor(BYTE *);
BOOL Pager(BYTE *,UWORD);

#endif
