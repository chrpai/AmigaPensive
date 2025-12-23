#ifndef  CLIB_DDDOOR_PROTOS_H
#define  CLIB_DDDOOR_PROTOS_H
/*
**	$Filename: clib/dd_protos.h $
**	$Release: V1.00 $
**	$Revision: 1.0 $
**	$Date: 96/10/10 $
**
**	C prototypes. For use with 32 bit integers only.
**
*/
#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef  LIBRARIES_DDDOOR_H
#include <libraries/dddoor.h>
#endif

struct DIFace *InitDoor(char *);
void CloseDoor(struct DIFace *);
void SendString(struct DIFace *, char *);
int Prompt(struct DIFace *, char *, unsigned long, unsigned long, char *);
void InquirePointers(struct DIFace *, struct DDPointers *);
void DisplayFile(struct DIFace *, char *, unsigned long);
int YesNo(struct DIFace *, unsigned long);
ULONG ScanUserbase(struct DIFace *, char *);
void UpdateStructures(struct DIFace *);
void XprSend(struct DIFace *, char *, char *);
void XprReceive(struct DIFace *, char *);
ULONG GetLrp(struct DIFace *, unsigned long, unsigned long);
void SetLrp(struct DIFace *, unsigned long, unsigned long, unsigned long);
ULONG GetKey(struct DIFace *, unsigned long);
ULONG ScanFileDirs(struct DIFace *, unsigned long, char *, void *, APTR);
ULONG FlagFile(struct DIFace *, char *, char *, unsigned long, unsigned long);
void Disconnect(struct DIFace *);
void DDCommand(struct DIFace *, char *);
void ResetIdle(struct DIFace *);
void Relogin(struct DIFace *);
void JoinConference(struct DIFace *, unsigned long, unsigned long);
void ChangeMsgBase(struct DIFace *, unsigned long, unsigned long);
void IntCommand(struct DIFace *, char *);
ULONG CheckKey(struct DIFace *);
void LineEditor(struct DIFace *, char *);
void FifoExecute(struct DIFace *, char *);
void WriteLog(struct DIFace *, char *);
void CleanTemp(struct DIFace *);
ULONG FetchKey(struct DIFace *, unsigned long, unsigned long);
void ChangeActivity(struct DIFace *, char *);
struct DayDream_Conference *GetConf(struct DayDream_Conference *, unsigned long);
struct DayDream_MsgBase *GetBase(struct DayDream_Conference *, unsigned long, unsigned long);
APTR IsAway(struct DIFace *);
ULONG SendMessage(struct DIFace *, ULONG, APTR);
ULONG SendMsgToAll(struct DIFace *, APTR);
void SetSerial(struct DIFace *, ULONG);
void ChatMode(struct DIFace *);
void Pause(struct DIFace *);
void ReCountFiles(struct DIFace *);
void SetRemainingTime(struct DIFace *, LONG);
void HideCursor(struct DIFace *);
void ShowCursor(struct DIFace *);
void GetDDString(struct DIFace *, ULONG);
struct DayDream_User *UpdateUserRec(struct DIFace *, struct DayDream_User *);
ULONG SetAccPreset(struct DIFace *, ULONG, struct DayDream_User *);
ULONG SearchUser(struct DIFace *, ULONG, char *, ULONG, struct DayDream_User *);
#endif	 /* CLIB_DD_PROTOS_H */
