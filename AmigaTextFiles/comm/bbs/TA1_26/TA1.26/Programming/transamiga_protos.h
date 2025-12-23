#ifndef  CLIB_TRANSAMIGA_PROTOS_H
#define  CLIB_TRANSAMIGA_PROTOS_H
/*
**	$Filename: clib/transamiga_protos.h $
**  $Release: v1.11 $
**  $Revision: 2.2 $
**  $Date: 92/03/19 $
**
**	TransAmiga prototypes.
**
**  (C) Copyright 1991, 1992 Timothy J. Aston
**	    All Rights Reserved
*/
#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef  TRANSAMIGA_H
#include <pd/transamiga.h>
#endif

/* TransAmiga library function definitions.  Most of these functions are
 * private, so don't bother with them, only use those that are documented.
 */

LONG RexxReserved();
VOID OutStr(char *str);
VOID LogEntry(char *symbol, char *str, char *logfile);
LONG FreeDriveSpace(char *dev);
int NewBBSLine(struct BBSLine *line);
VOID CloseBBSLine(struct BBSLine *line);
ULONG BBSLineInfo(int linenum);
VOID SetSerParams(struct BBSLine *line, ULONG baud);
int CheckCarrier(struct BBSLine *line);
VOID SerWrite(struct BBSLine *line, char *ch);
LONG SerReadData(struct BBSLine *line, char *ch);
LONG SerReadChar(struct BBSLine *line, char *ch);
VOID SerFlush(struct BBSLine *line);
VOID ConWrite(struct BBSLine *line, char *ch);
VOID ConQueueRead(struct BBSLine *line, char *ch);
int ConCheckRead(struct BBSLine *line);
VOID ConAbortRead(struct BBSLine *line);
int CheckLock(int what);
VOID DropDTR(struct BBSLine *line);
VOID RaiseDTR(struct BBSLine *line);
int StartRexx(char *macroname, char *portname, struct MsgPort *replyport);
int ParseRexxMsg(struct BBSLine *line,struct RexxMsg *rmp, char *rxargs);
ULONG CreateArgStr(char *str);
int DosCmd(char *cmd, char *outfile);
VOID SerWriteLen(struct BBSLine *line, char *ch, ULONG len);
VOID ConWriteLen(struct BBSLine *line, char *ch, ULONG len);
ULONG SerQuery(struct BBSLine *line);
VOID ANSIWrite(struct BBSLine *line, UBYTE *ch);
VOID PostNodeMsg(struct BBSLine *line, int linenum, char *msg);
VOID MenuCommSeq(struct BBSLine *line);
VOID OnlineMenus(struct BBSLine *line);
VOID WaitingMenus(struct BBSLine *line);
VOID SetScreenTitle(struct Screen *scr, UBYTE *scrtitle);
long RVIGetVar(struct RexxMsg *msg, char *var, VOID *val);
long RVISetVar(struct RexxMsg *msg, char *var, VOID *val);
VOID FixString(char *ch);
int TextReadStr(char *src, char *dest, long *l);
int AboutWin(struct BBSLine *line, UBYTE *regstr, UBYTE *portname);
int BBSCmd(LONG linenum, char *cmd, char *buf);
int SaveMsgText(char *infile, char *outfile, LONG msgnum, LONG type, LONG marg, struct MsgHeader *header);
struct RexxMsg * WaitRexxMsg(struct MsgPort *rp);
LONG ObtainLine(LONG linenum, struct BBSLine *linebuf);
VOID ReleaseLine(struct BBSLine *line);
VOID LockAccess(int what);
VOID UnLockAccess(int what);
VOID UpdateCursor(struct BBSLine *line);
VOID StripANSI(UBYTE *s, UBYTE *d);
ULONG GetTimer(LONG dummy);

#endif