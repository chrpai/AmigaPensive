
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include	<stdarg.h>

#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/pipeline.h>
#include <proto/pipeutil.h>
#include <proto/utility.h>
#include <plfile.h>
#include <plmsg.h>
#include <libraries/pipeutil.h>
#include <nodes.h>

#ifdef MAIN
#define global
#else
#define global extern
#endif

#define	MF_PRIVATE		0x0100
#define	MF_CRASH			0x0200
#define	MF_RECEIVED		0x0400
#define	MF_SENT			0x0800
#define	MF_FILEATTACH	0x1000
#define	MF_INTRANSIT	0x2000
#define	MF_ORPHAN		0x4000
#define	MF_KILLSENT		0x8000
#define	MF_LOCAL			0x0001
#define	MF_HOLD4PU		0x0002
#define	MF_UNUSED		0x0004
#define	MF_FILEREQ		0x0008
#define	MF_RETRECREQ	0x0010
#define	MF_ISRETREC		0x0020
#define	MF_AUDITREQ		0x0040
#define	MF_FILEUPREQ	0x00800

#define	SM_FTSC		(0)
#define	SM_FOOZLE	(1)
#define	SM_TRAPTOSS	(2)

struct	SIdx
{
	struct	MsgIdx		idx;
	ULONG						pos;
};

struct	SIdxN
{
	struct	MinNode		n;
	struct	SIdx			idx;
};

typedef	struct	SignalSemaphore	SigSem;

global	struct	DosLibrary		*DOSBase;
global	struct	Library			*PipelineBase;
global	struct	Library			*PipeUtilBase;
global	struct	MsgGlobals		*msgglobals;
global	struct	SharedData		*Shared;
global	struct	List			 	 AreaList, IdxList;
global	struct	SignalSemaphore AreaSem;
global	struct	SIdx				*idxs;
global	ULONG							 isize, inum, ipos, fmt;
global	UBYTE							 Tear[256], idxname[256];
global	BPTR							 ifh;

#define	LIBFUNC	__asm __saveds
#define	AREG(x)	register __a##x
#define	DREG(x)	register __d##x

struct	ZArea
{
	struct	MinNode		 n;
	struct	msgarea		*ma;
	ULONG						 area;
	struct	SIdx			*idx;
};

int		__asm	__saveds  mbInitBase 	( register __a0 struct msgarea * ) ;
BOOL
		__asm 	__saveds  mbInitialise 	( register __a0 struct MsgGlobals * ) ;
int		__asm	__saveds  mbGetInfo 		( register __a0 struct MsgInfo * ) ;
void		__asm	__saveds  mbQuit		( void ) ;

int				  FindPosition		( BPTR, ULONG, ULONG, ULONG ) ;
void				  MakeWaitingMail	( struct Msg * ) ;

struct WholeMessage * 	LIBFUNC mbLoadMessage 	( register __d0 int, register __d1 ULONG, register __d2 ULONG ) ;
signed int 		LIBFUNC mbSaveMessage 	( register __a0 struct WriteMsgStruct * ) ;
struct MsgScanKey * 	LIBFUNC mbInitMsgScanKey 	( register __a0 struct ScanInfo * ) ;
void			LIBFUNC mbCloseMsgScanKey 	( register __a0 struct MsgScanKey * ) ;
ULONG 			LIBFUNC mbScanMessages	( register __a0 struct MsgScanKey *, register __a1 struct WholeMessage * ) ;
signed int		LIBFUNC mbEditMessage 	( register __a0 struct Msg *, register __a1 unsigned char * ) ;
signed int		LIBFUNC mbDeleteMessage 	( register __d0 int, register __d1 ULONG ) ;
struct MsgIdx * 	LIBFUNC mbGetPointers 	( register __d0 ULONG ) ;
int			LIBFUNC mbSetPointers		( register __a0 struct MsgIdx * ) ;
ULONG			LIBFUNC mbGetNumberOfMessages ( void ) ;

struct ZArea *FindArea( ULONG );

struct	MsgHdr
{
	UBYTE		From[36];
	UBYTE		To[36];
	UBYTE		Subject[72];
	UBYTE		Date[20];
	UWORD		TimesRead;
	UWORD		DestNode, OrigNode;
	UWORD		Cost;
	UWORD		OrigNet, DestNet;
	UWORD		DestZone, OrigZone;
	UWORD		DestPoint, OrigPoint;
	UWORD		ReplyTo;
	UWORD		Attr;
	UWORD		NextReply;
	UBYTE		MsgTxt[0];
};

struct	FMsgId
{
	UWORD		Zone;
	UWORD		Net;
	UWORD		Node;
	UWORD		Point;
	ULONG		Data;
};

struct	FMsgHdr
{
	UBYTE		From[36];
	UBYTE		To[36];
	UBYTE		Subject[72];
	UBYTE		Date[20];
	UWORD		TimesRead;
	UWORD		DestNode, OrigNode;
	UWORD		Cost;
	UWORD		OrigNet, DestNet;
	ULONG		Ignored;
	ULONG		TimeStamp;
	UWORD		DestZone;
	UWORD		Attr;
	UWORD		DestPoint;
	struct	FMsgId	ReplyTo, NextReply;
	UBYTE		MsgTxt[0];
};

/* Utils.c */

void StripFido( UBYTE * );
time_t ParseTime( UBYTE * );
void ParseAddr( UBYTE *s, struct addr *a );
UWORD Fido2Zeus( UWORD flags );
UWORD Zeus2Fido( UWORD flags );
