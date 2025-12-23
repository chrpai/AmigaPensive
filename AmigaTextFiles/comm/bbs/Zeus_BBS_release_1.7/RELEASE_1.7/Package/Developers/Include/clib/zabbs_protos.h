/********************************************************************
**                                                                 **
**                 zabbs.library include file                      **
**                                                                 **
**        Copyright © 1995 CeCom. All rights reserved              **
**                                                                 **
********************************************************************/
#ifndef CLIB_ZABBS_PROTOS_H
#define CLIB_ZABBS_PROTOS_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

BYTE ZaAddCatMark(struct User *, ULONG);
BYTE ZaRemCatMark(struct User *, ULONG);

ULONG ZaNextFArea(struct Line *le, UBYTE *cp, ULONG *afl, ULONG a, UBYTE fl);
ULONG ZaPrevFArea(struct Line *le, UBYTE *cp, ULONG *afl, ULONG a, UBYTE fl);

ULONG ZaAccessMFlags(struct Line *, struct User *, ULONG *, ULONG, BOOL);
ULONG ZaNextMsgArea(struct User *, ULONG *, ULONG);
ULONG ZaPrevMsgArea(struct User *, ULONG *, ULONG);

long ZaFAreaAccess(struct User *u, long a);
long ZaCheckFAreaAccess(struct Line *le, struct filearea *fa, long *reqfl, long fl);
long ZaMAreaAccess(struct User *u, long a);
long ZaCheckMAreaAccess(struct Line *le, struct msgarea *ma, long *reqfl, long fl);
long ZaCheckMsgAccess(struct User *u, struct Msg *msg, unsigned long aflg, long flg);

UBYTE *ZaStrStr(UBYTE *, UBYTE *);

BOOL ZaSetPointer(struct User *u, struct HighPointer *newhp);
LONG ZaGetPointer(struct User *u, ULONG a);
LONG ZaCountConference(struct User *u, struct msgarea *ma, struct CountConference *cc, ULONG aflg, ULONG flg);

VOID ZaFreeList(struct List *ls, ULONG sz);
void ZaTranslateString(UBYTE *str, struct TransNode *tn, ULONG len, BOOL in);
struct Archiver *ZaGetArchiver(long);
UBYTE *ZaGetErrorStr(ULONG);

BOOL ZaLoadWindowPos(UBYTE *name, struct ZaWindowPos *zwp, ULONG num);
BOOL ZaSaveWindowPos(UBYTE *name, struct ZaWindowPos *zwp, ULONG num);

#endif
