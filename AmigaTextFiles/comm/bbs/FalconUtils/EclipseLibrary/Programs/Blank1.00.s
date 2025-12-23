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

	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoDOSLib
	Move.L	D0,_DOSBase
	
	Lea	ECLIPSELibrary(PC),A1
	Moveq	#EclipseVer,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoEclipseLib
	Move.L	D0,_ECLIPSEBase

	Movem.L	(SP)+,D0/A0

	Cmp.B	#1,D0
	Beq	Exit
	Cmp.B	#0,D0
	Beq	Exit
	Cmp.B	#'?',(A0)
	Beq	DoInfo
	

Exit:	Move.L	_ECLIPSEBase,A1
	CALLEXEC	CloseLibrary

NoEclipseLib:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary

NoDOSLib:	Moveq	#0,D0
	Move.L	_SaveStack,SP
	Rts

***************************************************************************
DoInfo:
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
DOSLibrary:	DOSNAME
ECLIPSELibrary:	EclipseName
	Even

***************************************
	Section	DataSpace,BSS
_DOSBase:	Ds.L	1
_ECLIPSEBase:	Ds.L	1
_SaveStack:	Ds.L	1
