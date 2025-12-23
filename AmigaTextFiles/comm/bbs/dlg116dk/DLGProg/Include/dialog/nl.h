/***************************************************************************/
/*						Traplist.library::Master Header File							*/
/*-------------------------------------------------------------------------*/
/*						  © Copyright 1990-93 by Martin J. Laubach					*/
/*									All rights reserved										*/
/*																									*/
/*					Inspired by structures by Juergen Hermann							*/
/*																									*/
/***************************************************************************/
/*
 * $Header: Susan:nl/RCS/nl.h,v 1.31 93/09/09 00:40:20 mjl Exp $
 */
/*-------------------------------------------------------------------------*/
#ifndef NL_NL_H
#define NL_NL_H 1

#define TRAPLIST_NAME	"traplist.library"
#define TRAPLIST_VER		5

/*-------------------------------------------------------------------------*/
#define NL_VERBATIM 0x8000		  /* Don't translate phone numbers in search */
               /* A NLFind() with that flag will give you the phone number */
               /* as it is listed in the nodelist 									*/

#define NL_PARSE_INVALID_CHAR		-1		         /* Returned by NLParseAddr */
#define NL_PARSE_INVALID_NUMBER	-2

/*-------------------------------------------------------------------------*/
#include "nltags.h"

/*-------------------------------------------------------------------------*/
/* Use these macros in NLEnumNode() */

#define IS_ZONE(ad,re)	 (ad->Zone == ad->Net && ad->Net == re && !ad->Node && !ad->Point)
#define IS_NET(ad,re)	 (!ad->Node && !ad->Point)
#define IS_NODE(ad,re)	 (ad->Node && !ad->Point)
#define IS_POINT(ad,re)	 (ad->Point)
#define IS_REGION(ad,re) (!ad->Node && !ad->Point && re == ad->Net)
#define IS_HUB(ad,hub)   (!ad->Point && ad->Node == hub)

/*-------------------------------------------------------------------------*/
typedef struct
	{
	UWORD Zone;
	UWORD Net;
	UWORD Node;
	UWORD Point;
	} Addr;

/*-------------------------------------------------------------------------*/
/* This structure is returned by NLFind() and NLIndexFind() */

typedef struct
	{
	Addr Node;						  /* node address */
	ULONG Reserved0;

	UWORD Region;					  /* region (0 if none) */
	UWORD Hub;						  /* node # of this node's HUB (0 if none) */
	char *System;					  /* node name */
	char *Sysop;					  /* sysop name */
	char *Phone;					  /* phone number */
	char *City;     				  /* city and state */
	char *Passwd;					  /* password */
	char *Flags;					  /* nodelist flags */
	LONG Cost;						  /* cost */
	UWORD BaudRate;				  /* baud rate */
	LONG ID; 						  /* unique ID (offset into nodeindex) */

	ULONG Reserved1;             /* New for version 5 */
	ULONG Reserved2;             /* For compatibility with nodelist.library */
	ULONG Reserved3;

	UBYTE Type;                  /* Entry type */
	UBYTE Pad0;
	} NodeDesc;

/*-------------------------------------------------------------------------*/
#define NL_ENTRY_NODE	0
#define NL_ENTRY_HOST	1
#define NL_ENTRY_HOLD	2
#define NL_ENTRY_HUB		3
#define NL_ENTRY_PVT		4
#define NL_ENTRY_POINT	5
#define NL_ENTRY_ZONE	6
#define NL_ENTRY_REGION	7
#define NL_ENTRY_DOWN	8
#define NL_ENTRY_MAX		9

/*-------------------------------------------------------------------------*/
#ifndef NL_PRIVATE

typedef void *Extra;
typedef void *NodeList;

#ifdef __STDC__

void LibArexx(void);

NodeList NLOpen(char *, ULONG);
void NLClose(NodeList);
NodeDesc *NLFind(NodeList, Addr *, ULONG);
int NLAddrComp(Addr *, Addr *);
void NLFreeNode(NodeDesc *);
NodeDesc *NLIndexFind(NodeList, Addr *, ULONG);

Extra ExtraOpen(char *);
void *ExtraFromNL(NodeList);
void ExtraClose(Extra);
void *ExtraFind(Extra, Addr *addr, ULONG, ULONG *);
void *ExtraFindLock(Extra, Addr *addr, ULONG, ULONG *);
BOOL ExtraPut(Extra, Addr *, ULONG, int, void *);
BOOL ExtraChange(Extra, Addr *, ULONG, ULONG, void *);
BOOL ExtraDel(Extra, void *);
BOOL ExtraFlush(Extra);
void ExtraLock(Extra);
void ExtraUnlock(Extra);
LONG ExtraEnumNode(Extra, BOOL (*)(Addr *, void *), void *);
LONG ExtraEnumTag(Extra, Addr *, BOOL (*)(void *, ULONG, ULONG, void *), void *);

LONG NLEnumNode(NodeList, LONG, Addr *, BOOL (*)(Addr *, ULONG, void *), void *);

LONG NLParseAddr(Addr *, char *, Addr *);

BOOL NLSubmitNewIndex(NodeList, char *, char *);

void ExtraLockTag(Extra, void *);
void ExtraUnlockTag(Extra, void *);

void ExtraTagDate(Extra, void *, struct DateStamp *);

char **NLGetNodelistNames(NodeList);
void NLFreeNodelistNames(NodeList, char **);

#endif /* STDC */

#if defined(SASC) || defined(LATTICE)
/*pragma libcall NodelistBase LibArexx 1E 9802*/
#pragma libcall NodelistBase NLOpen 24 0802
#pragma libcall NodelistBase NLClose 2A 801
#pragma libcall NodelistBase NLFind 30 09803
#pragma libcall NodelistBase NLFreeNode 36 801
#pragma libcall NodelistBase NLAddrComp 3C 9802
#pragma libcall NodelistBase NLIndexFind 42 09803
#pragma libcall NodelistBase ExtraFromNL 48 801
#pragma libcall NodelistBase ExtraOpen 4E 801
#pragma libcall NodelistBase ExtraClose 54 801
#pragma libcall NodelistBase ExtraFind 5A A09804
#pragma libcall NodelistBase ExtraPut 60 A109805
#pragma libcall NodelistBase ExtraDel 66 9802
/*pragma libcall NodelistBase ObsoleteOldIndexFind 6C 09803*/
#pragma libcall NodelistBase ExtraFlush 72 801
#pragma libcall NodelistBase ExtraLock 78 801
#pragma libcall NodelistBase ExtraUnlock 7E 801
#pragma libcall NodelistBase ExtraEnumNode 84 A9803
#pragma libcall NodelistBase ExtraChanged 8A 801
#pragma libcall NodelistBase ExtraEnumTag 90 BA9804
#pragma libcall NodelistBase NLEnumNode 96 BA90805
#pragma libcall NodelistBase NLParseAddr 9C A9803
/*pragma libcall NodelistBase NLSubmitNewIndex A2 A9803*/
#pragma libcall NodelistBase ExtraLockTag A8 9802
#pragma libcall NodelistBase ExtraUnlockTag AE 9802
#pragma libcall NodelistBase ExtraChange B4 A109805
#pragma libcall NodelistBase ExtraFindLock BA A09804
/*pragma libcall NodelistBase NLNodelistLibrary0 C0 0*/
/*pragma libcall NodelistBase NLNodelistLibrary1 C6 0*/
#pragma libcall NodelistBase ExtraTagDate CC A9803
#pragma libcall NodelistBase NLGetNodelistNames D2 801
#pragma libcall NodelistBase NLFreeNodelistNames D8 9802
#endif /* SASC */
#endif /* PRIVATE */
#endif /* NL.H */

