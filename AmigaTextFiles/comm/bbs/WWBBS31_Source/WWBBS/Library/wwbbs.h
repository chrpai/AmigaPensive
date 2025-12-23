#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <graphics/modeid.h>
#include <graphics/text.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/utility.h>

#include <libraries/wwbbs.h>

#include "wwbbs.library_rev.h"

char *ver=VERSTAG;

struct ExecBase *SysBase;
struct DosLibrary *DOSBase;
struct Library *UtilityBase;

struct MsgPort *UpdatePort;
struct SignalSemaphore UpdateSemaphore;

struct List ConfigList;
struct SignalSemaphore ConfigSemaphore;
struct List StatusList;
struct SignalSemaphore StatusSemaphore;
struct List UserList;
struct SignalSemaphore UserSemaphore;
struct List MessageGroupList;
struct SignalSemaphore MessageGroupSemaphore;
struct List FileGroupList;
struct SignalSemaphore FileGroupSemaphore;

char *ConfigFile="WWBBS:WWBBS.config";
char *UserIndexFile="WWBBS:WWBBS.userindex";
char *UserInfoFile="WWBBS:WWBBS.userinfo";
char *UserAccessFile="WWBBS:WWBBS.useraccess";
char *UserPrefsFile="WWBBS:WWBBS.userprefs";
char *UserStatsFile="WWBBS:WWBBS.userstats";

/* config.c */
__asm ULONG GetConfig(register __a0 struct TagItem *);
ULONG GetConfigTags(Tag,...);
__asm ULONG SetConfig(register __a0 struct TagItem *);
ULONG SetConfigTags(Tag,...);
__asm BOOL AddConfig(register __a0 struct TagItem *);
BOOL AddConfigTags(Tag,...);
__asm BOOL RemConfig(register __a0 struct TagItem *);
BOOL RemConfigTags(Tag,...);

/* config_lib.c */
struct Node *GetConfigNode(BYTE *,BYTE *,LONG);
void FreeConfigNode(struct Node *);
ULONG GetConfigFields(struct Node *,struct TagItem *);
ULONG SetConfigFields(struct Node *,struct TagItem *);
void LoadConfig(BYTE *);
void SaveConfig(BYTE *);
void SaveConfigList(BPTR,struct List *);
void SaveConfigNode(BPTR,struct Node *);
void SaveConfigField(BPTR,LONG,UWORD,APTR);
void UnLoadConfig(void);
void UnLoadConfigList(struct List *);

/* status.c */
__asm ULONG GetStatus(register __a0 struct TagItem *);
ULONG GetStatusTags(Tag,...);
__asm ULONG SetStatus(register __a0 struct TagItem *);
ULONG SetStatusTags(Tag,...);
__asm BOOL AddStatus(register __a0 struct TagItem *);
BOOL AddStatusTags(Tag,...);
__asm BOOL RemStatus(register __a0 struct TagItem *);
BOOL RemStatusTags(Tag,...);

/* status_lib.c */
struct Node *GetStatusNode(BYTE *,LONG);
void FreeStatusNode(struct Node *);
ULONG GetStatusFields(struct Node *,struct TagItem *);
ULONG SetStatusFields(struct Node *,struct TagItem *);
void UnLoadStatus(void);

/* users.c */
__asm ULONG GetUser(register __a0 struct TagItem *);
ULONG GetUserTags(Tag,...);
__asm ULONG SetUser(register __a0 struct TagItem *);
ULONG SetUserTags(Tag,...);
__asm BOOL AddUser(register __a0 struct TagItem *);
BOOL AddUserTags(Tag,...);
__asm BOOL RemUser(register __a0 struct TagItem *);
BOOL RemUserTags(Tag,...);
__asm BOOL LoadUser(register __a0 struct TagItem *);
BOOL LoadUserTags(Tag,...);
__asm VOID UnLoadUser(register __a0 struct TagItem *);
VOID UnLoadUserTags(Tag,...);

/* users_lib.c */
struct Node *GetUserNode(BYTE *,LONG);
void FreeUserNode(struct Node *);
ULONG GetUserInfoFields(struct Node *,struct TagItem *);
ULONG SetUserInfoFields(struct Node *,struct TagItem *);
ULONG GetUserAccessFields(struct Node *,struct TagItem *);
ULONG SetUserAccessFields(struct Node *,struct TagItem *);
ULONG GetUserPrefsFields(struct Node *,struct TagItem *);
ULONG SetUserPrefsFields(struct Node *,struct TagItem *);
ULONG GetUserStatsFields(struct Node *,struct TagItem *);
ULONG SetUserStatsFields(struct Node *,struct TagItem *);
struct Node *FindUser(BYTE *);
void LoadUserIndex(BYTE *);
void SaveUserIndex(BYTE *);
void UnLoadUserIndex(void);
BOOL LoadUserData(BYTE *,struct Node *,ULONG);
void SaveUserData(BYTE *,ULONG);
void UnLoadUserData(struct Node *,ULONG);
ULONG SaveUserField(BPTR,LONG,UWORD,APTR);
BOOL IsTag(Tag *,struct TagItem *);

/* messages.c */
__asm APTR OpenMessageGroup(register __a0 BYTE *,register __a1 BYTE *,register __d0 LONG);
__asm void CloseMessageGroup(register __a0 struct MessageGroupNode *);
__asm ULONG GetMessage(register __a0 struct MessageGroupNode *,register __a1 struct TagItem *);
ULONG GetMessageTags(struct MessageGroupNode *,Tag,...);
__asm ULONG SetMessage(register __a0 struct MessageGroupNode *,register __a1 struct TagItem *);
ULONG SetMessageTags(struct MessageGroupNode *,Tag,...);
__asm ULONG AddMessage(register __a0 struct MessageGroupNode *,register __a1 struct TagItem *);
ULONG AddMessageTags(struct MessageGroupNode *,Tag,...);
__asm BOOL RemMessage(register __a0 struct MessageGroupNode *,register __a1 struct TagItem *);
BOOL RemMessageTags(struct MessageGroupNode *,Tag,...);
__asm ULONG GetNextMessage(register __a0 struct MessageGroupNode *,register __d0 ULONG);
__asm ULONG GetPreviousMessage(register __a0 struct MessageGroupNode *,register __d0 ULONG);

/* messages_lib.c */
struct MessageNode *GetMessageNode(struct MessageGroupNode *,ULONG);
ULONG GetMessageFields(struct MessageNode *,struct TagItem *);
ULONG SetMessageFields(struct MessageNode *,struct TagItem *);
BOOL LoadMessageGroup(BYTE *);
BOOL SaveMessageGroup(struct MessageGroupNode *);
void FlushMessageGroups(void);
void SaveMessageField(BPTR,LONG,APTR,UWORD);
void SaveMessageString(BPTR,LONG,STRPTR);

/* files.c */
__asm APTR OpenFileGroup(register __a0 BYTE *,register __a1 BYTE *,register __d0 LONG);
__asm void CloseFileGroup(register __a0 struct FileGroupNode *);
__asm BOOL GetFileGroupDirectory(register __a0 struct FileGroupNode *,register __a1 BYTE *,register __d0 LONG);
__asm ULONG GetFile(register __a0 struct FileGroupNode *,register __a1 struct TagItem *);
ULONG GetFileTags(struct FileGroupNode *,Tag,...);
__asm ULONG SetFile(register __a0 struct FileGroupNode *,register __a1 struct TagItem *);
ULONG SetFileTags(struct FileGroupNode *,Tag,...);
__asm ULONG AddFile(register __a0 struct FileGroupNode *,register __a1 struct TagItem *);
ULONG AddFileTags(struct FileGroupNode *,Tag,...);
__asm BOOL RemFile(register __a0 struct FileGroupNode *,register __a1 struct TagItem *);
BOOL RemFileTags(struct FileGroupNode *,Tag,...);
__asm ULONG GetNextFile(register __a0 struct FileGroupNode *,register __d0 ULONG);
__asm ULONG GetPreviousFile(register __a0 struct FileGroupNode *,register __d0 ULONG);

/* files_lib.c */
struct FileNode *GetFileNode(struct FileGroupNode *,ULONG);
ULONG GetFileFields(struct FileNode *,struct TagItem *);
ULONG SetFileFields(struct FileNode *,struct TagItem *);
BOOL LoadFileGroup(BYTE *,BYTE *);
BOOL SaveFileGroup(struct FileGroupNode *);
void FlushFileGroups(void);
void SaveFileField(BPTR,LONG,APTR,UWORD);
void SaveFileString(BPTR,LONG,STRPTR);

/* commands.c */
__asm ULONG NodeCommand(register __a0 BYTE *,register __d0 UBYTE,register __a1 struct TagItem *);
ULONG NodeCommandTags(BYTE *,UBYTE,Tag,...);
__asm ULONG ConsoleCommand(register __a0 BYTE *,register __d0 UBYTE,register __a1 struct TagItem *);
ULONG ConsoleCommandTags(BYTE *,UBYTE,Tag,...);
__asm ULONG SerialCommand(register __a0 BYTE *,register __d0 UBYTE,register __a1 struct TagItem *);
ULONG SerialCommandTags(BYTE *,UBYTE,Tag,...);
__asm ULONG TimerCommand(register __a0 BYTE *,register __d0 UBYTE,register __a1 struct TagItem *);
ULONG TimerCommandTags(BYTE *,UBYTE,Tag,...);

/* commands_lib.c */
ULONG DoCommandMessage(BYTE *,BYTE *,UBYTE,struct TagItem *);
ULONG SendCommandMessage(BYTE *,BYTE *,UBYTE,struct TagItem *,ULONG,ULONG *,UBYTE,BOOL);

/* update.c */
__asm void SetUpdatePort(register __a0 struct MsgPort *);
__asm void SendUpdate(register __d0 UBYTE,register __a0 BYTE *);

/* wwbbs_lib.c */
__asm void LogEntry(register __a0 BYTE *,register __a1 BYTE *,register __a2 BYTE *);
__asm BOOL IsRange(register __a0 BYTE *,register __d0 ULONG);
