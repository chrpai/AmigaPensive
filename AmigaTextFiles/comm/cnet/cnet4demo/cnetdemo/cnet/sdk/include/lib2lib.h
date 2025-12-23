
#ifndef CNET_INTERLIB
#define CNET_INTERLIB

// dates.c

__asm __saveds ULONG LIBTime( void );
__asm __saveds void LIBExplodeTime( register __d0 ULONG t, register __a0 struct IsDate *date );
__asm __saveds ULONG LIBImplodeTime( register __a0 struct IsDate *date );
__asm __saveds USHORT LIBDowDate( register __a0 struct IsDate *d );
__asm __saveds void LIBPrintDate(
	register __a0 struct	IsDate *date,
	register __a1 char	*out,
	register __a2 char	**strings,
	register __a3 char	*format );
__asm __saveds void LIBPrintTime(
	register __d0 ULONG	t,
	register __a0 char	*out,
	register __a1 char	**strings,
	register __a2 struct	UserData *user );
__asm __saveds ULONG LIBTimeNowDiff(
	register __d0 prevtime );

#endif
