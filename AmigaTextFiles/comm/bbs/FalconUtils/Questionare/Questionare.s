;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	Questionare,Code

ExecBase	Equ	4
OpenLibrary	Equ	-552
CloseLibrary	Equ	-414
AllocMem	Equ	-198
FreeMem		Equ	-210

Open		Equ	-30
Close		Equ	-36
Read		Equ	-42
Write		Equ	-48
Input		Equ	-54
Output		Equ	-60
Seek		Equ	-66
Lock		Equ	-84
UnLock		Equ	-90
Examine		Equ	-102

***************************************************************************
Start:
	Bsr	GetUserName
	Bsr	OpenDOS

	Move.L	#Credits,D2
	Move.L	#CreditsLen,D3
	Bsr	WriteText

	Tst.L	UserName
	Bne.S	.NameOK

	Move.L	#NoName,D2
	Move.L	#NoNameLen,D3
	Bsr	WriteText
	Bra	ExitToDos

.NameOK
	Lea	WelcomeName(PC),A0
	Lea	WelcomeEnd(PC),A1
	Bsr	LoadFile

	Lea	QuestionsName(PC),A0
	Bsr	ReadFile
	Tst.L	D0
	Bne	FileError

	Move.L	#DataFileName,D1
	Bsr	OpenFile

	Move.L	#UserName,D2
	Bsr	WriteToDisk

	Move.L	MemStart(PC),A0
	Move.L	A0,CurrentPos
	Add.L	MemSize(PC),A0
	Move.L	A0,MemEnd	

***************************************
MainLoop:
	Move.L	CurrentPos(PC),A0
	Move.L	MemEnd(PC),A1
	Cmp.L	A1,A0
	Bge.S	NoMoreQs
	
MoreQs:
	Moveq	#0,D3
	Move.L	A0,D2
.Loop
 	Cmp.B	#0,(A0)+
	Beq	.Found

	Cmp.L	A1,A0
	Bge.S	NoMoreQs

	Addq	#1,D3
	Bra.S	.Loop

.Found
	Move.L	A0,CurrentPos
	Bsr	WriteText
	Bsr	InputToDisk
	Bra.S	MainLoop

***************************************
NoMoreQs:
	Bsr	CloseFile

	Move.L	MemStart(PC),A1
	Move.L	MemSize(PC),D0
	Move.L	(ExecBase).w,A6
	Jsr	FreeMem(A6)

	Lea	GoodByeName(PC),A0
	Lea	GoodByeEnd(PC),A1
	Bsr	LoadFile

ExitToDos:
	Bsr	CloseDOS
	Moveq	#0,D0
	Rts

***************************************************************************
OpenDOS:
	Move.L	(ExecBase).w,A6
	Lea	DosLibrary(PC),A1
	Moveq	#0,D1
	Jsr	OpenLibrary(A6)
	Move.L	D0,DosBase

	Move.L	DosBase(PC),A6
	Jsr	Output(A6)
	Move.L	D0,OutputHandle

	Move.L	DosBase(PC),A6
	Jsr	Input(A6)
	Move.L	D0,InputHandle
	Rts

***************************************
CloseDOS:
	Move.L	(ExecBase).w,A6
	Move.L	DosBase(PC),A1
	Jsr	CloseLibrary(A6)
	Rts

***************************************
OpenFile:
	Move.L	DosBase(PC),A6
	Move.L	#1004,D2
	Jsr	Open(A6)
	Move.L	D0,FileHandle
	Beq	Error

	Move.L	FileHandle(PC),D1
	Moveq.L	#0,D2
	Moveq	#1,D3
	Jsr	Seek(A6)

	Moveq	#0,D0
	Rts

Error:
	Moveq	#-1,D0
	Rts

***************************************
CloseFile:
	Move.L	DosBase(PC),A6
	Move.L	FileHandle(PC),D1
	Jsr	Close(A6)
	Rts

***************************************
InputToDisk:
	Bsr	ClearBuffer
	Bsr	ReadText
	Move.L	#KeyBoardBuffer,D2
	Bsr	WriteToDisk
	Rts

***************************************
WriteToDisk:
	Move.L	D2,A0
	Moveq	#0,D3
	Moveq	#79,D0
.loop
	Cmp.B	#0,(A0)+
	Beq	.found
	Addq	#1,D3
	DBra	D0,.loop
	Moveq	#-1,D0
	Rts

.found
	Move.L	DosBase(PC),A6
	Move.L	FileHandle(PC),D1
	Jsr	Write(A6)
	Moveq	#0,D0
	Rts

***************************************
ClearBuffer:
	Lea	KeyBoardBuffer(PC),A0
	Moveq	#19,D0
	Moveq	#0,D1
.loop
	Move.L	D1,(A0)+
	DBra	D0,.loop
	Rts

***************************************
ReadText:
	Move.L	DosBase(PC),A6
	Move.L	InputHandle(PC),D1
	Move.L	#KeyBoardBuffer,D2
	Moveq	#79,D3
	Jsr	Read(A6)
	Rts

***************************************
WriteText:
	Move.L	DosBase(PC),A6
	Move.L	OutputHandle(PC),D1
	Jsr	Write(A6)
	Rts

***************************************
GetUserName:
	Cmp.B	#1,D0
	Beq.S	.NoUser
	Cmp.B	#0,D0
	Beq.S	.NoUser

	Cmp.B	#"Y",(A0)
	Beq.S	.IBMOK
	Not.B	IBMFlag
.IBMOK	
	Subq	#3,D0
	Addq	#1,A0

	Lea	UserName(PC),A1
	Move.B	#10,(A1)+
	Move.B	#10,(A1)+
	Move.L	#'----',(A1)+
	Move.B	#10,(A1)+

.loop
	Cmp.B	#0,(A0)
	Beq.S	.endloop	
	Move.B	(A0)+,(A1)+
	DBra	D0,.loop
.endloop
	Move.B	#10,(A1)+
	Move.B	#10,(A1)+
.NoUser
	Rts

***************************************
LoadFile:
	Tst.B	IBMFlag
	Bne.S	.BBS
	
	Bsr	ReadFile
	Tst.L	D0
	Bne.S	.BBS
	Bra	.TypeIt
.BBS	
	Move.B	#"b",(A1)	
	Bsr	ReadFile
	Tst.L	D0
	Bne.S	.Error

.TypeIt
	Move.L	MemStart(PC),D2
	Move.L	MemSize(PC),D3
	Bsr.S	WriteText
	
	Move.L	MemStart(PC),A1
	Move.L	MemSize(PC),D0
	Move.L	(ExecBase).w,A6
	Jsr	FreeMem(A6)
.Error
	Rts

***************************************
ReadFile:
	Move.L	#0,MemSize
	Move.L	#0,MemStart

*******************
	Moveq	#1,D4
	Lea	FileNamePtr(PC),A1
	Move.L	A0,(A1)

*******************
	Move.L	DosBase(PC),A6
	Move.L	FileNamePtr(PC),D1
	Moveq	#-2,D2
	Jsr	Lock(A6)
	Lea	FileLock(PC),A0
	Move.L	D0,(A0)
	Beq	.Error

*******************
	Move.L	FileLock(PC),D1
	Lea	FileInfo(PC),A0
	Move.L	A0,D2
	Jsr	Examine(A6)

*******************
	Move.L	FileInfo+124(PC),D0
	Lea	MemSize(PC),A0
	Move.L	D0,(A0)

*******************
	Move.L	FileLock(PC),D1
	Jsr	UnLock(A6)

*******************
	Move.L	FileNamePtr(PC),D1
	Move.L	#1005,D2
	Jsr	Open(A6)
	Lea	LoadHandle(PC),A0
	Move.L	D0,(A0)
	Beq	.Error

*******************
	Moveq	#2,D4
	Move.L	(ExecBase).W,A6
	Move.L	MemSize(PC),D0
	Moveq	#0,D1
	Jsr	AllocMem(A6)
	Tst.L	D0
	Beq	.AllocError
	Lea	MemStart(PC),A0
	Move.L	D0,(A0)

*******************
	Moveq	#3,D4
	Move.L	DosBase(PC),A6
	Move.L	LoadHandle(PC),D1
	Move.L	MemStart(PC),D2
	Move.L	FileInfo+124(PC),D3
	Jsr	Read(A6)
	Tst.L	D0
	Beq.S	.LoadError

*******************
	Move.L	FileInfo+124(PC),D1
	Cmp.L	D0,D1
	Bne.S	.LoadError

*******************
	Move.L	DosBase(PC),A6
	Move.L	LoadHandle(PC),D1
	Jsr	Close(A6)
	Moveq	#0,D4
.Error
	Move.L	D4,D0
	Rts

*******************
.LoadError
	Move.L	(ExecBase).W,A6
	Move.L	MemStart(PC),A1
	Move.L	MemSize(PC),D0
	Jsr	FreeMem(A6)

.AllocError
	Move.L	DosBase(PC),A6
	Move.L	LoadHandle(PC),D1
	Jsr	Close(A6)

	Move.L	D4,D0
	Rts

***************************************
FileError:
	Cmp.B	#3,D0
	Bne.S	.NotRead
	Move.L	#ReadError,D2
	Moveq	#ReadErrorLength,D3
	Bra.S	.TypeError

.NotRead
	Cmp.B	#2,D0
	Bne.S	.NotNoMem
	Move.L	#MemError,D2
	Moveq	#MemErrorLength,D3
	Bra.S	.TypeError

.NotNoMem
	Move.L	#LoadError,D2
	Moveq	#LoadErrorLength,D3

.TypeError
	Bsr	WriteText
	Bra	ExitToDos

***************************************************************************
***************************************************************************
***************************************************************************
Version:
	Dc.B	0,'$VER: QUESTIONARE 1.05 (22.05.92)',0
Credits:
	Dc.B	10,'[0;37m  Questionare V1.05',10
	Dc.B	'[35m  (c) 1992  David Dustin / KAOS Productions',10,10,0
CreditsLen	Equ	*-Credits

***************************************
NoName:
	DC.B	'[0mNo Arguments were supplied.  Please check and try again!',10,0
NoNameLen	Equ	*-NoName

***************************************
LoadError:
	DC.B	'[0;35mThere was a problem finding the questions file.  Contact the sysop.',10,0
LoadErrorLength	Equ	*-LoadError

***************************************
MemError:
	DC.B	'[0;35mThere is not enough memory for the questions file.  Call for the sysop.',10,0
MemErrorLength	Equ	*-MemError

***************************************
ReadError:
	Dc.B	'[0;35mThere was a problem loading the questions file.  Contact the sysop.',10,0
ReadErrorLength	Equ	*-ReadError

***************************************************************************
IBMFlag:
	Dc.B	0
DosLibrary:
	Dc.B	'dos.library',0

WelcomeName:
	Dc.B	'QUESTIONS/Welcome.gbs',0
WelcomeEnd	Equ	*-4

GoodByeName:
	Dc.B	'QUESTIONS/Goodbye.gbs',0
GoodByeEnd	Equ	*-4

QuestionsName:
	Dc.B	'QUESTIONS/Questions.qst',0

DataFileName:
	Dc.B	'Questions.dat',0
	Even

***************************************
CurrentPos:
	Dc.L	0
DosBase:
	Dc.L	0
InputHandle:
	Dc.L	0
OutputHandle:
	Dc.L	0
FileHandle:
	Dc.L	0
FileLock:
	Dc.L	0
LoadHandle:
	Dc.L	0
FileNamePtr:
	Dc.L	0
MemEnd:
	Dc.L	0
MemSize:
	Dc.L	0
MemStart:
	Dc.L	0

***************************************
	CNop	0,4
FileInfo:
	Ds.B	260
KeyBoardBuffer:
	Ds.B	80
UserName:
	Ds.B	80
