
#pragma libcall CNet4Base CNetPrintTime		1e	a98004
#pragma libcall CNet4Base CNetTime		24	00
#pragma libcall CNet4Base CNetExplodeTime	2a	8002
#pragma libcall CNet4Base CNetImplodeTime	30	801
#pragma libcall CNet4Base CNetDowDate		36	801
#pragma libcall CNet4Base CNetPrintDate		3c	ba9804
#pragma libcall CNet4Base CNetFindRange		42	910804
#pragma libcall CNet4Base CNetNextRange		48	801
#pragma libcall CNet4Base CNetLongToRange	4e	8002
#pragma libcall CNet4Base CNetRangeToLong	54	801
#pragma libcall CNet4Base CNetTimeNowDiff 5a 001

// dates.c

__asm __saveds void   CNetPrintTime(
	register __d0 ULONG t,
	register __a0 char *out,
	register __a1 char **strings,
	register __a2 struct UserData *user );
__asm __saveds ULONG  CNetTime( void );
__asm __saveds void   CNetExplodeTime(
	register __d0 ULONG t,
	register __a0 struct IsDate *date );
__asm __saveds ULONG  CNetImplodeTime(
	register __a0 struct IsDate *date );
__asm __saveds USHORT CNetDowDate(
	register __a0 struct IsDate *d );
__asm __saveds void   CNetPrintDate(
	register __a0 struct IsDate *date,
	register __a1 char *out,
	register __a2 char **strings,
	register __a3 char *format );
__asm __saveds LONG CNetTimeNowDiff(
	register __d0 ULONG prevtime );

// ranges.c

__asm __saveds long   CNetFindRange(
	register __a0 char *buffer,
	register __d0 long min,
	register __d1 long max,
	register __a1 struct RangeContext *rc );
__asm __saveds long   CNetNextRange(
	register __a0 struct RangeContext *rc );
__asm __saveds void   CNetLongToRange(
	register __d0 long def,
	register __a0 char *where );
__asm __saveds long   CNetRangeToLong(
	register __a0 char *s );
