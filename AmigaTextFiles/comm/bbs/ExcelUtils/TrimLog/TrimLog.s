;Released as public domain 28-Jan-1996 by the author Dave Dustin
*****************************************************************************
* This code and it's design are (c) 1994 by David Dustin / ECLIPSE Software *
*---------------------------------------------------------------------------*
*                                                                           *
* Any modifications and subsequent release of this program must be sent to  *
* me (David).  As I have released this code to the public, I CANNOT be held *
* responsable for ANYTHING that happens while using this program.           *
*                                                                           *
*****************************************************************************
	Section	TrimLog,Code
	Opt	C+,D-
	Include	mysystem.gs

	Output	TrimLog

*****************************************************************************
* Notes:
*	1.00 - Initial Release
*	1.01 - Fixed error with write to disk routine
*	1.02 - Added normal TrimFile routines, so it's two programs
*	       in one
*	1.03 - Changed for newstyle LogEntries
*	     - Added Filtered Log option
*	1.04 - Sycom changed the logs again.
*	     - Filtered Log removed, because of new log style
*	1.05 - Changed the log search string again
*	     - Added varable string option
*	1.1  - Rewrote major sections
*	     - Improved speed, memory usage
*	     - Fixed bugs
*	1.11 - Fixed STUPID error with variable size (B instead of W)
*	1.20 - Major upgrade.  Changed how the damn thing works
*	1.20a- Bug fix.  DBra loop was crashing out
*	1.21 - Prevented 0 as valid ENTRIES count
*	     - Lost over 200 bytes of size
*
*****************************************************************************
Start:	Move.L	$4.W,A6
	Move.L	A6,_EXECBase

	Sub.L	A1,A1
	CALL	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Beq	fromWorkbench

	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	CloseDown
	Move.L	D0,_DOSBase

***************************************
	Move.L	#ArgTemplate,D1
	Move.L	#_Args,D2
	Moveq	#0,D3
	CALLPCDOS	ReadArgs
	Move.L	D0,_RDArgs
	Beq	CloseDown

***************************************
	Moveq	#10,D0
	Tst.L	_Line
	Beq.S	.NoLine
	Moveq	#100,D0

.NoLine	Move.L	_Entries(PC),A0
	Cmp.L	#0,A0
	Beq.S	.NoEntries
	Move.L	(A0),D0
	Cmp.L	#1,D0
	Blt	CloseDown
.NoEntries	Move.L	D0,_TotalEntries

***************************************
	Move.L	_FileName(PC),D1
	Move.L	#MODE_OLDFILE,D2
	CALLPCDOS	Open
	Move.L	D0,_FileHandle
	Bne.S	.GotFile

.BadFile	Move.L	_FileName(PC),_ErrorPointer
	Move.L	#NoFileError,D1
	Move.L	#_ErrorPointer,D2
	CALL	VPrintf
	Bra	CloseDown

.GotFile	Move.L	D0,D1
	Moveq	#0,D2
	Moveq	#OFFSET_END,D3
	CALL	Seek
	Cmp.L	#-1,D0
	Beq.S	.BadFile

	Move.L	_FileHandle(PC),D1
	Moveq	#0,D2
	Moveq	#OFFSET_BEGINNING,D3
	CALL	Seek
	Move.L	D0,_FileSize
	Beq.S	.BadFile

***************************************
	Move.L	#MEMF_CLEAR,D1
	CALLPCEXEC	AllocMem
	Move.L	D0,_FileStart
	Bne.S	.GotMem

	Move.L	_FileSize(PC),_ErrorPointer
	Move.L	#NoMemError,D1
	Move.L	#_ErrorPointer,D2
	CALLPCDOS	VPrintf
	Bra	CloseDown

***************************************
.GotMem	Move.L	_FileHandle(PC),D1
	Move.L	_FileStart(PC),D2
	Move.L	_FileSize(PC),D3
	CALLPCDOS	Read
	Tst.L	D0
	Beq	.BadFile

	Move.L	_FileHandle(PC),D1
	CALL	Close
	Move.L	#0,_FileHandle

	Move.L	_FileStart(PC),A0
	Move.L	_FileSize(PC),D0
	Add.L	D0,A0
	Move.L	A0,_FileEnd

*****************************************************************************
Main:	Tst.L	_Line
	Beq	TrimCalls

***************************************
TrimLines:	Move.L	_FileEnd(PC),A0
	Move.L	_FileSize(PC),D0
	Move.L	_FileStart(PC),A4

	Move.L	_TotalEntries(PC),D3
	Subq	#1,D0
	Subq	#1,A0

.FindLoop	Cmp.L	A4,A0
	Ble	CloseDown
	Cmp.B	#10,-(A0)
	Beq.S	.foundone
.LoopAgain	Subq.L	#1,D0
	Bne.S	.FindLoop
	Bra	CloseDown

.foundone	Subq.L	#1,D3
	Bne.S	.FindLoop

******************************************************************************
TrimFile:	Addq.L	#1,A0
	Move.L	_FileEnd(PC),A1
	Sub.L	A0,A1
	Move.L	A1,D5
	Move.L	A0,D4

	Move.L	_FileName(PC),D1
	Move.L	#MODE_NEWFILE,D2
	CALLPCDOS	Open
	Move.L	D0,_FileHandle
	Beq.S	.BadSave

	Move.L	_FileHandle(PC),D1

	Move.L	D4,D2
	Move.L	D5,D3
	CALL	Write
	Tst.L	D0
	Bne	CloseDown

.BadSave	Move.L	#NoOutputError,D1
	Move.L	#_ErrorPointer,D2
	CALL	VPrintf
	Bra	CloseDown

******************************************************************************
TrimCalls:	Move.L	_String(PC),A5
	Cmp.L	#0,A5
	Bne.S	.NewString
	Lea	SearchString(PC),A5

.NewString	Move.L	_FileStart(PC),A4
	Move.L	_TotalEntries(PC),D4
	Move.L	_FileEnd(PC),A1
	Moveq	#0,D2

.MainLoop	Move.L	A5,A0
	Bsr.S	.FindData
	Tst.L	D0
	Bne.S	CloseDown
	Subq	#1,D4
	Bne.S	.MainLoop

.FinishedSearch	Move.L	A1,A0
	Bra.S	TrimFile

***************************************
.FindData	Moveq	#0,D0
	Moveq	#0,D1
	Move.B	(A0),D0
	Bsr.S	.Upper
	Move.B	D0,D1

	Cmp.L	A4,A1
	Ble.S	.Exit

	Move.B	-(A1),D0
	Tst.B	D0
	Beq.S	.Exit
	Bsr.S	.Upper
	Cmp.B	D0,D1
	Bne.S	.FindData

	Move.L	A0,A2
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

.Found	Subq	#1,A1
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
CloseDown:	Move.L	_FileStart(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoMemory
	Move.L	_FileSize(PC),D0
	CALLPCEXEC	FreeMem
.NoMemory
	Move.L	_FileHandle(PC),D1
	Tst.L	D1
	Beq.S	.NoFile
	CALLPCDOS	Close
.NoFile
	Move.L	_RDArgs(PC),D1
	Tst.L	D1
	Beq.S	.NoArgs
	CALLPCDOS	FreeArgs
.NoArgs
	Move.L	_DOSBase(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoDOS
	CALLPCEXEC	CloseLibrary
.NoDOS
	Moveq	#0,D0
	Rts

*****************************************************************************
fromWorkbench:	Lea	pr_MsgPort(A4),A0
	CALL	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,A1
	CALL	Forbid
	CALL	ReplyMsg
	Rts

*****************************************************************************
VerString:	Dc.B	0,'$VER: Trimlog 1.21 (5.9.94) by David Dustin / Eclipse Software',0

***************************************
DOSLibrary:	DOSNAME
ArgTemplate:	Dc.B	'FILENAME/A,ENTRIES/N,S=STRING/K,L=LINE/S',0
SearchString:	Dc.B	'\q1----',0
NoFileError:	Dc.B	'Error accessing file ''%s''.',10,0
NoOutputError:	Dc.B	'Unable to write file to disk.  Please check file integrity.',10,0
NoMemError:	Dc.B	'Unable to allocate %ld bytes to load file.',10,0
	Even

***************************************
_FileStart:	Dc.L	0
_FileSize:	Dc.L	0
_FileHandle:	Dc.L	0
_FileEnd:	Dc.L	0

_TotalEntries:	Dc.L	0

_Args:
_FileName:	Dc.L	0
_Entries:	Dc.L	0
_String:	Dc.L	0
_Line:	Dc.L	0,0

_ErrorPointer:	Dc.L	0,0

_RDArgs:	Dc.L	0

_EXECBase:	Dc.L	0
_DOSBase:	Dc.L	0
