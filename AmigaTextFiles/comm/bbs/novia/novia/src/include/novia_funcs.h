#ifndef NOVIA_FUNCS_H
#define NOVIA_FUNCS_H
#define NOVIA_FUNCS_H_VERSION "$VER: 0.01 (08.05.1998)"

#ifndef NOVIA_TYPES_H
#include <novia/novia_types.h>
#endif

#ifndef NOVIA_GLOBAL_H
#include <novia/novia_global.h>
#endif

// novia_io_main.c

extern ULONG Conread(char *string, char len,ULONG sigs);
extern ULONG WaitSig(ULONG bitmask);
extern BOOL SendMsg(MsgPort *port, APTR data, ULONG data_len, ULONG msg_type, ULONG cmd_type);
extern void Writeio2(APTR data, ULONG len);

// novia_serial.c

extern BOOL SN_Open();
extern void SN_Close();
extern BOOL SN_CheckNetworkStatus();
extern LONG SN_Send(APTR data, ULONG len);
extern ULONG SN_Recv(char *string, char len, ULONG BreakBitMask);
extern void SN_HangingUP();
extern void SN_WaitConnect();


// novia_global.c

extern void GetArg(char *string);
extern void FreeAllMem();
extern void ClearMemQuick(APTR start,ULONG len);
extern void ClearMem(APTR start,ULONG len);
extern void ItemToViewItem(APTR itemaddress,ViewItem *viewitem);
extern BOOL ArgToLong(const char *string,ULONG *from,ULONG *to);
extern BOOL DelItem(const char *path,ULONG start,ULONG end);
extern BOOL AddItem(struct ItemHeader *itemheader,const char *path);
extern void NewShell();
extern void CloseShell();
extern ULONG StrToIP(char *string);
extern void IPToStr(char *string,ULONG IPNumber);
extern void ClearList(struct List *list);
extern BOOL ChangeWhere(char *where);
extern void SelectFromTo(UBYTE *table, ULONG tablesize, char *string);

// novia_main.c

extern long GetCommandID(char *string);
extern char StdCommand(ULONG cmdid);

// novia_user.c

extern void ShowUserList();
extern void SaveUserList(UserList *ulist);
extern void LoadUserList(UserList *ulist);
extern void Termsettings(UserData *udata,char type);  // 0 - Show 1- Edit
extern void UserEdit(ULONG user);

// novia_main.c

extern void SaveMainPortConfig( MainPortConfig *pointer);
extern void CloseIt( char error_code, char *error_text);
extern struct MainPortConfig *LoadMainPortConfig(MainPortConfig *pointer);
extern LONG init_maindata();
extern void StartPort(); 		// Prototype
extern BOOL CloseClientPort(LONG PortID);
extern void LoadClientList();
extern void SaveClientList();
extern struct PortData *NewClientPort();
extern void SaveClientConfig( ClientConfig *cconfig);

extern void MyFreeVec( APTR address,  char *text);
extern APTR MyAllocVec( ULONG size,  ULONG flags,  char *text);
extern void LoadMCICodes(ULONG *numberofmci, struct mcicode *mcicmd);
extern BOOL LoadMemberList(char *filename, List *ulist);
extern BOOL SaveMemberList(char *filename, List *ulist);
extern void RepairUserList( UserList *ulist);
extern void LoadStringTable(struct STR_Group *strgroup, UWORD strgroups);
extern void RemoveStringList();
extern struct PortData *FindClient(ULONG PortID);

// novia_visualeditor.c

extern ULONG VisualEditor(char *filename, APTR data, ULONG flags);



#endif