#ifndef _INLINE_DDDOOR_H
#define _INLINE_DDDOOR_H

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#ifndef DDDOOR_BASE_NAME
#define DDDOOR_BASE_NAME DDBase
#endif

#define ChangeActivity(dif, string) \
	LP2NR(0xcc, ChangeActivity, struct DIFace *, dif, d0, char *, string, a0, \
	, DDDOOR_BASE_NAME)

#define ChangeMsgBase(dif, base, flags) \
	LP3NR(0x9c, ChangeMsgBase, struct DIFace *, dif, d0, unsigned long, base, d1, unsigned long, flags, d2, \
	, DDDOOR_BASE_NAME)

#define ChatMode(dif) \
	LP1NR(0xf6, ChatMode, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define CheckKey(dif) \
	LP1(0xa8, ULONG, CheckKey, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define CleanTemp(dif) \
	LP1NR(0xc0, CleanTemp, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define CloseDoor(dif) \
	LP1NR(0x24, CloseDoor, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define DDCommand(dif, command) \
	LP2NR(0x84, DDCommand, struct DIFace *, dif, d0, char *, command, a0, \
	, DDDOOR_BASE_NAME)

#define Disconnect(dif) \
	LP1NR(0x7e, Disconnect, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define DisplayFile(dif, filename, flags) \
	LP3NR(0x3c, DisplayFile, struct DIFace *, dif, d0, char *, filename, a0, unsigned long, flags, d1, \
	, DDDOOR_BASE_NAME)

#define FetchKey(dif, flags, timeout) \
	LP3(0xc6, ULONG, FetchKey, struct DIFace *, dif, d0, unsigned long, flags, d1, unsigned long, timeout, d2, \
	, DDDOOR_BASE_NAME)

#define FifoExecute(dif, command) \
	LP2NR(0xb4, FifoExecute, struct DIFace *, dif, d0, char *, command, a0, \
	, DDDOOR_BASE_NAME)

#define FlagFile(dif, path, file, flags, size) \
	LP5(0x78, ULONG, FlagFile, struct DIFace *, dif, d0, char *, path, a0, char *, file, a1, unsigned long, flags, d1, unsigned long, size, d2, \
	, DDDOOR_BASE_NAME)

#define GetBase(confdat, conf, base) \
	LP3(0xd8, struct DayDream_MsgBase *, GetBase, struct DayDream_Conference *, confdat, a0, unsigned long, conf, d0, unsigned long, base, d1, \
	, DDDOOR_BASE_NAME)

#define GetConf(confdat, conf) \
	LP2(0xd2, struct DayDream_Conference *, GetConf, struct DayDream_Conference *, confdat, a0, unsigned long, conf, d0, \
	, DDDOOR_BASE_NAME)

#define GetDDString(dif, stringnum) \
	LP3(0xc6, ULONG, GetDDString, struct DIFace *, dif, d0, unsigned long, flags, d1, \
	, DDDOOR_BASE_NAME)

#define GetKey(dif, flags) \
	LP2(0x6c, ULONG, GetKey, struct DIFace *, dif, d0, unsigned long, flags, d1, \
	, DDDOOR_BASE_NAME)

#define GetLrp(dif, conf, base) \
	LP3(0x60, ULONG, GetLrp, struct DIFace *, dif, d0, unsigned long, conf, d1, unsigned long, base, d2, \
	, DDDOOR_BASE_NAME)

#define HideCursor(dif) \
	LP1NR(0x10e, HideCursor, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define InitDoor(node) \
	LP1(0x1e, struct DIFace *, InitDoor, char *, node, a0, \
	, DDDOOR_BASE_NAME)

#define InquirePointers(dif, pointers) \
	LP2NR(0x36, InquirePointers, struct DIFace *, dif, d0, struct DDPointers *, pointers, a0, \
	, DDDOOR_BASE_NAME)

#define IntCommand(dif, command) \
	LP2NR(0xa2, IntCommand, struct DIFace *, dif, d0, char *, command, a0, \
	, DDDOOR_BASE_NAME)

#define IsAway(dif) \
	LP1(0xde, APTR, IsAway, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define JoinConference(dif, conf, flags) \
	LP3NR(0x96, JoinConference, struct DIFace *, dif, d0, unsigned long, conf, d1, unsigned long, flags, d2, \
	, DDDOOR_BASE_NAME)

#define LineEditor(dif, file) \
	LP2NR(0xae, LineEditor, struct DIFace *, dif, d0, char *, file, a0, \
	, DDDOOR_BASE_NAME)

#define Pause(dif) \
	LP1NR(0xfc, Pause, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define Prompt(dif, buffer, length, flags, default) \
	LP5(0x30, int, Prompt, struct DIFace *, dif, d0, char *, buffer, a0, unsigned long, length, d1, unsigned long, flags, d2, char *, default, d3, \
	, DDDOOR_BASE_NAME)

#define ReCountFiles(dif) \
	LP1NR(0x102, ReCountFiles, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define Relogin(dif) \
	LP1NR(0x90, Relogin, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define ResetIdle(dif) \
	LP1NR(0x8a, ResetIdle, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define ScanFileDirs(dif, conf, pattern, hook, userdata) \
	LP5(0x72, ULONG, ScanFileDirs, struct DIFace *, dif, d0, unsigned long, conf, d1, char *, pattern, a0, void *, hook, a1, APTR, userdata, a2, \
	, DDDOOR_BASE_NAME)

#define ScanUserbase(dif, name) \
	LP2(0x48, ULONG, ScanUserbase, struct DIFace *, dif, d0, char *, name, a0, \
	, DDDOOR_BASE_NAME)

#define SendMessage(dif, node, message) \
	LP3(0xe4, ULONG, SendMessage, struct DIFace *, dif, d0, ULONG, node, d1, APTR, message, a0, \
	, DDDOOR_BASE_NAME)

#define SendMsgToAll(dif, message) \
	LP2(0xea, ULONG, SendMsgToAll, struct DIFace *, dif, d0, APTR, message, a0, \
	, DDDOOR_BASE_NAME)

#define SendString(dif, string) \
	LP2NR(0x2a, SendString, struct DIFace *, dif, d0, char *, string, a0, \
	, DDDOOR_BASE_NAME)

#define SetLrp(dif, conf, base, lrp) \
	LP4NR(0x66, SetLrp, struct DIFace *, dif, d0, unsigned long, conf, d1, unsigned long, base, d2, unsigned long, lrp, d3, \
	, DDDOOR_BASE_NAME)

#define SetRemainingTime(dif, time) \
	LP2NR(0x108, SetRemainingTime, struct DIFace *, dif, d0, LONG, time, d1, \
	, DDDOOR_BASE_NAME)

#define SetSerial(dif, state) \
	LP2NR(0xf0, SetSerial, struct DIFace *, dif, d0, ULONG, state, d1, \
	, DDDOOR_BASE_NAME)

#define ShowCursor(dif) \
	LP1NR(0x114, ShowCursor, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define UpdateStructures(dif) \
	LP1NR(0x4e, UpdateStructures, struct DIFace *, dif, d0, \
	, DDDOOR_BASE_NAME)

#define WriteLog(dif, string) \
	LP2NR(0xba, WriteLog, struct DIFace *, dif, d0, char *, string, a0, \
	, DDDOOR_BASE_NAME)

#define XprReceive(dif, path) \
	LP2NR(0x5a, XprReceive, struct DIFace *, dif, d0, char *, path, a0, \
	, DDDOOR_BASE_NAME)

#define XprSend(dif, files, defpath) \
	LP3NR(0x54, XprSend, struct DIFace *, dif, d0, char *, files, a0, char *, defpath, a1, \
	, DDDOOR_BASE_NAME)

#define YesNo(dif, default) \
	LP2(0x42, int, YesNo, struct DIFace *, dif, d0, unsigned long, default, d1, \
	, DDDOOR_BASE_NAME)

#define UpdateUserRec(dif) \
	LP2NR(0x120, struct DayDream_User *, UpdateUserRec, struct DIFace *, dif, d0, struct DayDream_User *, usr, d1\
	, DDDOOR_BASE_NAME)

#endif /* _INLINE_DDDOOR_H */
