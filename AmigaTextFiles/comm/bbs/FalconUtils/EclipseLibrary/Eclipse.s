;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	EclipseLibrary,CODE
	Opt	c+

	Include	MySystem.gs
	Include	Eclipse.i

	Output	Libs:eclipse.library

***************************************************************************
MyPri	Equ	0
Version	Equ	EclipseVer
VersionStr	MACRO
	Dc.B	'35'
	ENDM
Revision	Equ	26
RevisionStr	MACRO
	Dc.B	'26'
	ENDM
LibName	MACRO
	Dc.B	'Eclipse Software Library'
	ENDM
LibDate	MACRO
	Dc.B	'(22.11.1992)'
	ENDM

***************************************************************************
	STRUCTURE	EclipseLib,LIB_SIZE
	ULONG	ML_SysLib
	ULONG	ML_SegList
	UBYTE	ML_Flags
	UBYTE	ML_Pad
	LABEL	EclipseLib_Sizeof

***************************************************************************
Start:
	Moveq	#0,d0
	Rts

***************************************************************************
InitDescript:
	Dc.W	RTC_MATCHWORD
	Dc.L	InitDescript
	Dc.L	EndCode
	Dc.B	RTF_AUTOINIT
	Dc.B	Version
	Dc.B	NT_LIBRARY
	Dc.B	MyPri
	Dc.L	MyName
	Dc.L	IdString
	Dc.L	Init

MyName:	EclipseName
IdString:
	LibName
	Dc.B	' '
	VersionStr
	Dc.B	'.'
	RevisionStr
	Dc.B	'  '
	LibDate
	Dc.B	13,10,0
	Even

***************************************************************************
Init:	Dc.L	EclipseLib_Sizeof
	Dc.L	FuncTable
	Dc.L	DataTable
	Dc.L	InitRoutine

***************************************************************************
FuncTable:	Dc.L	LibOpen
	Dc.L	LibClose
	Dc.L	Expunge
	Dc.L	Null

***************************************
	Dc.L	ChangeCase
	Dc.L	ConvertDecimal
	Dc.L	ConvertToDecimal

	Dc.L	FindData
	Dc.L	CopyData
	Dc.L	CopyDataExclude

	Dc.L	CompareStrings

	Dc.L	ConvertFalconDate
	Dc.L	FindUser
	Dc.L	GetConfigEntry
	Dc.L	LogTag

	Dc.L	OutputText

	Dc.L	Date
	Dc.L	LongDay
	Dc.L	ShortDay
	Dc.L	Time12
	Dc.L	Time24

	Dc.L	Divu32Bit
	Dc.L	Mulu32Bit

	Dc.L	ReadFile
	Dc.L	GetFile
	Dc.L	GetFileSize

	Dc.L	-1

***************************************************************************
DataTable:	INITBYTE	LH_TYPE,NT_LIBRARY
	INITLONG	LN_NAME,MyName
	INITBYTE	LIB_FLAGS,LIBF_SUMUSED!LIBF_CHANGED
	INITWORD	LIB_VERSION,Version
	INITWORD	LIB_REVISION,Revision
	INITLONG	LIB_IDSTRING,IdString
	Dc.L	0

***************************************************************************
InitRoutine:	Move.L	A5,-(SP)
	Move.L	D0,A5
	Move.L	A6,ML_SysLib(A5)
	Move.L	A0,ML_SegList(A5)

	Move.L	A5,D0
	Move.L	(SP)+,A5
	Movem.L	D0-7/A0-6,-(SP)
	Lea	DOSLibrary(PC),A1
	Moveq	#0,D0
	CALLEXEC	OpenLibrary
	Move.L	D0,_DOSBase
	Movem.L	(SP)+,D0-7/A0-6
	Rts

***************************************
LibOpen:
	Addq.W	#1,LIB_OPENCNT(A6)
	Bclr	#LIBB_DELEXP,ML_Flags(A6)
	Move.L	A6,D0
	Rts

***************************************
LibClose:
	Moveq	#0,D0
	Subq.W	#1,LIB_OPENCNT(A6)
	Bne.S	1$
	Btst	#LIBB_DELEXP,ML_Flags(A6)
	Beq.S	1$
	Bsr	Expunge
1$:
	Rts

***************************************
Expunge:
	Movem.L	D2/A5/A6,-(SP)
	Move.L	A6,A5
	Move.L	ML_SysLib(A5),A6
	Tst.W	LIB_OPENCNT(A5)
	Beq	1$
	Bset	#LIBB_DELEXP,ML_Flags(A5)
	Moveq	#0,D0
	Bra.S	Expunge_End
1$:
	Move.L	ML_SegList(A5),D2
	Move.L	A5,A1
	CALLLIB	_LVORemove

	Moveq	#0,d0
	Move.L	A5,A1
	Move.W	LIB_NEGSIZE(A5),D0
	Sub.L	D0,A1
	Add.W	LIB_POSSIZE(A5),D0
	CALLLIB	_LVOFreeMem

	Move.L	D2,D0
Expunge_End:
	Movem.L	(SP)+,D2/A5/A6
	Rts

***************************************
Null:
	Moveq	#0,D0
	Rts

***************************************************************************
	Include	Routines/ChangeCase.s
	Include	Routines/CompareStrings.s
	Include	Routines/ConvertDecimal.s
	Include	Routines/ConvertFalconDate.s	;missing routine
	Include	Routines/ConvertToDecimal.s
	Include	Routines/CopyData.s
	Include	Routines/CopyDataExclude.s
	Include	Routines/Date.s
	Include	Routines/Divu32Bit.s	;missing routine
	Include	Routines/FindData.s
	Include	Routines/FindUser.s	;missing routine
	Include	Routines/GetConfigEntry.s	;missing routine
	Include	Routines/GetFile.s
	Include	Routines/GetFileSize.s
	Include	Routines/LogTag.s	;missing routine
	Include	Routines/LongDay.s
	Include	Routines/Mulu32Bit.s	;missing routine
	Include	Routines/OutputText.s	;missing routine
	Include	Routines/ReadFile.s
	Include	Routines/ShortDay.s
	Include	Routines/Time12.s
	Include	Routines/Time24.s

DOSLibrary:	DOSNAME
	Even
_DOSBase:	Dc.L	0

EndCode:
