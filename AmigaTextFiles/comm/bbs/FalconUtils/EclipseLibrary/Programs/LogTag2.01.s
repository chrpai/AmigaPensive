;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	LogTag,Code
	Opt	C+
	Include	mysystem.gs
	Include	library/eclipse.i

	Output	Work:LogTag

VERSION	MACRO
	Dc.B	'Falcon CBCS Logtag V2.01'
	ENDM
DATE	MACRO
	Dc.B	'22 September 92'
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

	Lea	ECLIPSELibrary(PC),A1
	Moveq	#EclipseVer,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq.S	ExitToDos
	Move.L	D0,_ECLIPSEBase

	Movem.L	(SP)+,D0/A0

	Cmp.B	#1,D0
	Beq.S	Exit
	Cmp.B	#0,D0
	Beq.S	Exit
	Cmp.B	#'?',(A0)
	Beq.S	Exit

	Lea	CommentLine,A1
	CALLECLIPSE	CopyData

	Lea	CommentLine,A0
	CALL	LogTag

Exit:	Move.L	_ECLIPSEBase,A1
	CALLEXEC	CloseLibrary

ExitToDos:	Moveq	#0,D0
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
	Even

***************************************
	Section	DataSpace,BSS
CommentLine:	Ds.B	100
_ECLIPSEBase:	Ds.L	1
_SaveStack:	Ds.L	1
