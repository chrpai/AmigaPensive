*****************************************************************************
* This code and it's design are (c) 1993 by David Dustin / ECLIPSE Software *
*---------------------------------------------------------------------------*
*                                                                           *
* Any modifications and subsequent release of this program must be sent to  *
* me (David).  As I have released this code to the public, I CANNOT be held *
* responsable for ANYTHING that happens while using this program.           *
*                                                                           *
*****************************************************************************
	Section	TrimLog,Code
	Opt	C+
	Include	mysystem.gs

	Output	TrimLog

VERSION	MACRO
	Dc.B	'TrimLog V1.02'
	ENDM
DATE	MACRO
	Dc.B	'28.1.93'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

DEBUG	Equ	0	;remove ReadArgs checking

*****************************************************************************
* Notes:
*	1.00 - Initial Release
*	1.01 - Fixed error with write to disk routine
*	1.02 - Added normal TrimFile routines, so it's two programs
*	       in one
*
*****************************************************************************
Start:	Move.L	SP,_SaveStack

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

	CALLDOS	Output
	Move.L	D0,_OutputHandle

***************************************
	IFEQ	DEBUG
	Move.L	#ArgumentTemp,D1
	Move.L	#_Arguments,D2
	Moveq	#0,D3
	CALL	ReadArgs
	Tst.L	D0
	Beq	Exit
	Move.L	D0,_Argument

	Move.L	_Arguments,D1
	ENDC

	IFNE	DEBUG
	Move.L	#FileName,D1
	ENDC

***************************************
	Move.L	#MODE_OLDFILE,D2
	CALL	Open
	Move.L	D0,_FileHandle
	Tst.L	D0
	Bne.S	.GotFile
	Lea	NoFileError(PC),A0
	Bsr	DoErrorMessage
	Bra	Exit2

.GotFile	Move.L	D0,D1
	Move.L	#_FileInfo,D2
	CALL	ExamineFH

	Lea	_FileInfo,A0
	Move.L	fib_Size(A0),D0
	Tst.L	D0
	Beq	.FileError
	Move.L	D0,_FileSize
	Addq	#2,D0
	Move.L	#MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Bne.S	.GotMem

.FileError	Move.L	_FileHandle,D1
	CALLDOS	Close
	Lea	NoMemError(PC),A0
	Bsr	DoErrorMessage
	Bra	Exit2

.GotMem	Addq	#1,D0
	Move.L	D0,_FileStart

	Move.L	_FileHandle,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALLDOS	Read
	Tst.L	D0
	Bne	.ReadOk
	
	Move.L	_FileHandle,D1
	CALL	Close
	Lea	NoFileError(PC),A0
	Bsr	DoErrorMessage
	Bra	CloseDown

.ReadOk	Move.L	_FileHandle,D1
	CALL	Close

***************************************
	Tst.L	_Line
	Beq.S	.NoLine
	Move.B	#100,Entries
	
.NoLine	Move.L	_Entries,A0
	Move.L	(A0),D0
	Beq.S	.NoChange
	Move.B	D0,Entries

.NoChange	Move.L	#TempFile,D1
	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Move.L	D0,_OutputFile
	Tst.L	D0
	Beq.S	Main

	Move.L	D0,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALL	Write

	Move.L	_OutputFile,D1
	CALL	Close

*****************************************************************************
Main:	Tst.L	_Line
	Beq	TrimCalls

***************************************
	Move.L	_FileStart,A0
	Add.L	_FileSize,A0
	Move.L	_FileSize,D0
	Subq	#1,D0
	Addq.B	#1,Entries

.FindLoop	Cmp.B	#10,-(A0)
	Beq.S	.foundone
.LoopAgain	DBra	D0,.FindLoop
	Bra	CloseDown

.foundone	Subq.B	#1,Entries
	Tst.B	Entries
	Bne.s	.LoopAgain
	Addq.L	#1,A0
	Move.L	A0,_EntriesTable

	IFEQ	DEBUG
	Move.L	_Arguments,D1
	ENDC

	IFNE	DEBUG
	Move.L	#FileName,D1
	ENDC

	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Move.L	D0,_OutputFile
	Tst.L	D0
	Bne.S	.OpenOk

	Lea	NoOutputError(PC),A0
	Bsr	DoErrorMessage
	Bra	CloseDown

.OpenOk	Move.L	D0,D1
	Move.L	_EntriesTable,D2
	Move.L	D2,D4
	Move.L	_FileStart,D0
	Sub.L	D0,D4
	Move.L	_FileSize,D3
	Sub.L	D4,D3
	CALL	Write

	Move.L	_OutputFile,D1
	CALL	Close
	Bra	CloseDown
	

***************************************
TrimCalls:	Move.L	_FileStart,A1
	Lea	_EntriesTable,A4
	Moveq	#0,D2

.MainLoop	Lea	SearchString(PC),A0
	Bsr	.FindData
	Tst.L	D0
	Bne.S	.FinishedSearch
	Addq	#1,D2
	Move.L	A0,(A4)+
	Move.L	A1,A0
	Bra.S	.MainLoop

.FinishedSearch	Moveq	#0,D0
	Move.B	Entries(PC),D0
	Cmp.L	D0,D2
	Ble	CloseDown

	Sub.L	D0,D2
	Mulu	#4,D2
	Move.L	D2,D5
	
	IFEQ	DEBUG
	Move.L	_Arguments,D1
	ENDC

	IFNE	DEBUG
	Move.L	#FileName,D1
	ENDC

	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Move.L	D0,_OutputFile
	Tst.L	D0
	Bne.S	.OpenOk

	Lea	NoOutputError(PC),A0
	Bsr	DoErrorMessage
	Bra	CloseDown

.OpenOk	Move.L	D0,D1
	Lea	_EntriesTable,A0
	Move.L	(A0,D5.w),D2
	Move.L	D2,D4
	Move.L	_FileStart,D0
	Sub.L	D0,D4
	Move.L	_FileSize,D3
	Sub.L	D4,D3
	CALL	Write

	Move.L	_OutputFile,D1
	CALL	Close
	Bra.S	CloseDown

***************************************
.FindData	Moveq	#0,D0
	Moveq	#0,D1
	Move.B	(A0),D0
	Bsr.S	.Upper
	Move.B	D0,D1
	Move.B	(A1)+,D0
	Tst.B	D0
	Beq.S	.Exit
	Bsr.S	.Upper
	Cmp.B	D0,D1
	Bne.S	.FindData

	Lea	1(A0),A2
	Move.L	A1,A3

.findloop	Move.B	(A2)+,D0
	Tst.B	D0
	Beq.S	.Found
	Bsr.S	.Upper
	Move.B	D0,D1
	Move.B	(A3)+,D0
	Bsr.S	.Upper
	Cmp.B	D0,D1
	Bne.S	.FindData
	Bra.S	.findloop

.Exit	Moveq	#-1,D0
	Rts

.Found	Lea	-1(A1),A0
	Move.L	A3,A1
	Moveq	#0,D0
	Rts

***************************************
.Upper	Cmp.B	#'a',D0
	Blo.S	.exit
	Cmp.B	#'z',D0
	Bls.S	.change
	Cmp.B	#$E0,D0
	Blo.S	.exit
	Cmp.B	#$FD,D0
	Bhi.S	.exit
	Cmp.B	#$F7,D0
	Beq.S	.exit
.change	Sub.B	#32,D0
.exit	Rts

*****************************************************************************
CloseDown:	Move.L	_FileStart,A1
	Subq	#1,A1
	Move.L	_FileSize,D0
	Addq	#2,D0
	CALLEXEC	FreeMem

Exit2:
	IFEQ	DEBUG
	Move.L	_Argument,D1
	CALLDOS	FreeArgs
	ENDC

Exit:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary
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

****************************************************************************
DoErrorMessage:	Move.L	A0,D2
	Moveq	#0,D3
.CalcLoop	Addq.L	#1,D3
	Cmp.B	#0,(A0)+
	Bne.S	.CalcLoop

	Move.L	_OutputHandle,D1
	CALLDOS	Write
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
ArgumentTemp:	Dc.B	'FILENAME/A,ENTRIES/N,L=LINE/S',0
SearchString:	Dc.B	'\n2\',0
TempFile:	Dc.B	'T:TrimLog.temp',0
NoFileError:	Dc.B	'Error reading file',10,0
NoOutputError:	Dc.B	'Could not write file to disk.',10,0
NoMemError:	Dc.B	'No memory to load file',10,0
	IFNE	DEBUG
FileName:	Dc.B	'Work:Callers',0
	ENDC
Entries:	Dc.B	10
	Even

*****************************************************************************
	Section	DataSpace,BSS
_SaveStack:	Ds.L	1

DataStart:
_EntriesTable:	Ds.L	256

_FileStart:	Ds.L	1
_FileSize:	Ds.L	1
_FileHandle:	Ds.L	1

_OutputHandle:	Ds.L	1
_OutputFile:	Ds.L	1

_Arguments:	Ds.L	1
_Entries:	Ds.L	1
_Line:	Ds.L	2

_Argument:	Ds.L	1
_DOSBase:	Ds.L	1

	CNOP	0,4
_FileInfo:	Ds.B	260

DataSize	Equ	*-DataStart
