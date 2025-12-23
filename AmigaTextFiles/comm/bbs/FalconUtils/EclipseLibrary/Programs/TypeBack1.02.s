;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	TypeBack,Code
	Opt	C+
	Include	mysystem.gs
	Include	library/eclipse.i

	Output	Work:TypeBack

VERSION	MACRO
	Dc.B	'TypeBack V1.02'
	ENDM
DATE	MACRO
	Dc.B	'27 September 92'
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
	Beq	ExitToDos
	Move.L	D0,_ECLIPSEBase

	Movem.L	(SP)+,D0/A0

	Cmp.B	#1,D0
	Beq	Exit
	Cmp.B	#0,D0
	Beq	Exit
	Cmp.B	#'?',(A0)
	Beq	Exit
	
	Lea	FileName,A1
	CALLECLIPSE	CopyData

	Lea	FileName,A0
	Lea	_FileStart,A1
	Lea	_FileSize,A2
	Moveq	#0,D0
	CALL	ReadFile
	Tst.L	D0
	Bne	Exit

	Move.L	_FileStart,A0
	Add.L	_FileSize,A0
	Move.L	A0,_CurrentPos

MainLoop:	Move.L	_CurrentPos,A0
	Subq	#1,A0
	Moveq	#0,D3

.findloop	Addq	#1,D3
	Cmp.B	#10,-1(A0)
	Beq.s	.foundend
	Cmp.B	#13,-1(A0)
	Beq.s	.foundend
	Subq	#1,A0
	Move.L	_FileStart,A1
	Cmp.L	A0,A1
	Bne.S	.findloop
	Addq	#1,D3

.foundend	Move.L	A0,_CurrentPos
	Moveq	#0,D1
	Move.L	A0,D2
	CALLECLIPSE	OutputText

	Move.L	#SIGBREAKF_CTRL_C,D1
	Moveq	#0,D0
	CALLEXEC	SetSignal
	Btst.L	#SIGBREAKB_CTRL_C,D0
	Bne.S	Close

	Move.L	_CurrentPos,A0
	Move.L	_FileStart,A1
	Cmp.L	A0,A1
	Bne.S	MainLoop
	
Close:	Move.L	_FileStart,A1
	Move.L	_FileSize,D0
	CALLEXEC	FreeMem

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
FileName:	Ds.B	140
_FileStart:	Ds.L	1
_FileSize:	Ds.L	1
_CurrentPos:	Ds.L	1
_ECLIPSEBase:	Ds.L	1
_SaveStack:	Ds.L	1
