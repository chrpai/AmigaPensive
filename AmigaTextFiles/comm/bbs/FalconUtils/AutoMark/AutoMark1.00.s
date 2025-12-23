;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	AutoMark,Code
	Opt	C+,D+
	Include	mysystem.gs

	Output	AutoMark

VERSION	MACRO
	Dc.B	'Falcon CBCS AutoMark V1.00'
	ENDM
DATE	MACRO
	Dc.B	'6.12.92'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

*****************************************************************************
* Notes:
*	1.00 - Initial Release
*
*****************************************************************************
Start:	Move.L	A0,_ArgStart
	Move.L	SP,_SaveStack

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Beq	fromWorkbench

	Lea	DataStart,A0
	Move.L	#DataSize-1,D0

.ClearDataSpace	Move.B	#0,(A0)+
	DBra	D0,.ClearDataSpace

	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoDOS
	Move.L	D0,_DOSBase

	Move.L	_ArgStart,D1
	Move.L	#_AreaNumber,D2
	CALLDOS	StrToLong
	Tst.L	D0
	Beq	Exit

	Move.L	_AreaNumber,D0
	Cmp.L	#100,D0
	Bgt	Exit

	Move.L	#FileName,D1
	Move.L	#MODE_OLDFILE,D2
	CALLDOS	Open
	Move.L	D0,_FileHandle
	Beq	Exit

	Move.L	D0,D1
	Move.L	#_FileInfo,D2
	CALL	ExamineFH

	Lea	_FileInfo,A0
	Move.L	fib_Size(A0),D0
	Move.L	D0,_FileSize
	Beq	Error
	Moveq	#0,D1
	CALLEXEC	AllocMem
	Move.L	D0,_FileStart
	Beq	Error
			
	Move.L	_FileHandle,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALLDOS	Read
	Beq	NoLock
	
	Move.L	_FileHandle,D1
	CALL	Close	
	Move.L	#0,_FileHandle

	Move.L	_FileSize,D0
	Divu.W	#512,D0
	Move.W	D0,_NumberOfUsers

	Add.L	#208,_AreaNumber

*****************************************************************************
Main:	Move.L	_FileStart,A0
	Move.L	A0,A1
	Move.L	_AreaNumber,D0
	Moveq	#0,D1
	Moveq	#0,D2
	Move.W	_NumberOfUsers,D2
	Subq	#1,D2
.loop	Move.L	A1,A0
	Add.L	D0,A0
	BSet.B	#0,(A0)
	Lea	512(A1),A1
	DBra	D2,.loop

	Move.L	#FileName2,D1
	CALLDOS	DeleteFile

	Move.L	#FileName,D1
	Move.L	#FileName2,D2
	CALL	Rename
	Tst.L	D0
	Beq.S	NoLock

	Move.L	#FileName,D1
	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Move.L	D0,_FileHandle
	Beq.S	Exit

	Move.L	D0,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALL	Write

*****************************************************************************
NoLock:	Move.L	_FileStart,A1
	Move.L	_FileSize,D0
	CALLEXEC	FreeMem

Error:	Move.L	_FileHandle,D1
	Beq.S	Exit
	CALLDOS	Close

Exit:	Move.L	_DOSBase,A1
	CALL	CloseLibrary
NoDOS:	Moveq	#0,D0
	Move.L	_SaveStack,SP
	Rts

*****************************************************************************
fromWorkbench:	Lea	pr_MsgPort(A4),A0
	CALLEXEC	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,A1

	CALL	Forbid
	CALL	ReplyMsg
	Move.L	_SaveStack,SP
	Rts
	
*****************************************************************************
*****************************************************************************
*****************************************************************************
VerString:	Dc.B	0,'$VER: '
	VERSION
	Dc.B	' ('
	DATE
	Dc.B	') '
	AUTHOR
	Dc.B	0

***************************************
DOSLibrary:	DOSNAME
FileName:	Dc.B	'User.BBS',0
FileName2:	Dc.B	'User.BAK',0
	Even

*****************************************************************************
	Section	DataSpace,BSS
_SaveStack:	Ds.L	1
_ArgStart:	Ds.L	1

DataStart:
_FileInfo:	Ds.B	260

_NumberOfUsers:	Ds.W	1

_FileStart:	Ds.L	1
_FileSize:	Ds.L	1
_FileHandle:	Ds.L	1

_DOSBase:	Ds.L	1
_AreaNumber	Ds.L	1

DataSize	Equ	*-DataStart
