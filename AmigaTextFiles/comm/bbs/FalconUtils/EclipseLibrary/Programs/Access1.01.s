;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	Access,Code
	Opt	C+
	Include	mysystem.gs
	Include	library/eclipse.i

	Output	Work:Access

VERSION	MACRO
	Dc.B	'Access V1.01'
	ENDM
DATE	MACRO
	Dc.B	'4.10.92'
	ENDM
AUTHOR	MACRO
	Dc.B	'(c) 1992  David Dustin'
	ENDM

***************************************************************************
* Notes:
***************************************************************************
Start:	Move.L	SP,_SaveStack
	Movem.L	D0/A0,-(SP)

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Beq	fromWorkbench

	Move.L	#5,_Error
	Lea	ECLIPSELibrary(PC),A1
	Moveq	#EclipseVer,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	ExitToDos
	Move.L	D0,_ECLIPSEBase

	Movem.L	(SP)+,D0/A0

	Cmp.B	#1,D0
	Beq	Exit
	Cmp.B	#0,D0
	Beq	Exit
	Cmp.B	#'?',(A0)
	Beq	Exit
	
	Move.B	(A0),DatNameNum
	Move.B	(A0)+,NoNameNum

	Lea	Name,A1
	CALLECLIPSE	CopyData

	Lea	DatName(PC),A0
	Lea	_FileStart,A1
	Lea	_FileSize,A2
	Moveq	#0,D0
	CALL	ReadFile
	Tst.L	D0
	Bne.S	Deny

	Lea	Name,A0
	Move.L	_FileStart,A1
	CALL	FindData
	
	Tst.L	D0
	Bne.S	.Nope
	Move.L	#0,_Error
	Bra.S	Close

.Nope	Move.L	_FileStart,A1
	Move.L	_FileSize,D0
	CALLEXEC	FreeMem

Deny:	Lea	NoName(PC),A0
	Lea	_FileStart,A1
	Lea	_FileSize,A2
	Moveq	#0,D0
	CALLECLIPSE	ReadFile
	Tst.L	D0
	Bne.S	Exit
	
	Moveq	#0,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALL	OutputText

Close:	Move.L	_FileStart,A1
	Move.L	_FileSize,D0
	CALLEXEC	FreeMem

Exit:	Move.L	_ECLIPSEBase,A1
	CALLEXEC	CloseLibrary

ExitToDos:	Move.L	_Error,D0
	Move.L	_SaveStack,SP
	Rts

***************************************************************************
fromWorkbench:	Lea	pr_MsgPort(A4),A0
	CALLEXEC	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,A1
	Movem.L	(SP)+,D0/A0

	CALL	Forbid
	CALL	ReplyMsg
	Move.L	_SaveStack,SP
	Rts

***************************************************************************
***************************************************************************
***************************************************************************
VerString:	Dc.B	0,'$VER: '
	VERSION
	Dc.B	' ('
	DATE
	Dc.B	')',0
ECLIPSELibrary:	EclipseName
DatName:	Dc.B	'Access'
DatNameNum:	Dc.B	'x.dat',0
NoName:	Dc.B	'Access'
NoNameNum:	Dc.B	'x.deny',0
	Even

***************************************
	Section	DataSpace,BSS
Name:	Ds.B	40
_Error:	Ds.L	1
_FileStart:	Ds.L	1
_FileSize:	Ds.L	1
_ECLIPSEBase:	Ds.L	1
_SaveStack:	Ds.L	1
