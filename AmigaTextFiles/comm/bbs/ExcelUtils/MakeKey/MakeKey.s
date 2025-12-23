;Released as public domain 28-Jan-1996 by the author Dave Dustin
*****************************************************************************
* This code and it's design are (c) 1993 by David Dustin / ECLIPSE Software *
*---------------------------------------------------------------------------*
*                                                                           *
* Any modifications and subsequent release of this program must be sent to  *
* me (David).  As I have released this code to the public, I CANNOT be held *
* responsable for ANYTHING that happens while using this program.           *
*                                                                           *
*****************************************************************************
	Section	MakeKey,Code
	Opt	C+
	Include	mysystem.gs
	Include	misc/easystart.i

	Output	Makekey

VERSION	MACRO
	Dc.B	'MakeKey V1.0'
	ENDM
DATE	MACRO
	Dc.B	'9.3.93'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

*****************************************************************************
* Notes:
*	1.00 - Initial Release
*
*****************************************************************************
Start:	Move.L	SP,_SaveStack

	Lea	DataStart,A1
	Move.L	#DataSize-1,D1

.ClearDataSpace	Move.B	#0,(A1)+
	DBra	D1,.ClearDataSpace

	Move.L	D0,_ArgSize
	Move.L	A0,_ArgStart

***************************************
	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALLEXEC	OpenLibrary
	Tst.L	D0
	Beq	NoDOS
	Move.L	D0,_DOSBase

	Move.L	#Con,D1
	Move.L	#MODE_NEWFILE,D2
	CALLDOS	Open
	Move.L	D0,_TextHandle

***************************************
	Lea	Title(PC),A0
	Bsr	TypeMessage

	Cmp.L	#0,_ArgSize
	Beq.S	.GetFile
	Cmp.L	#1,_ArgSize
	Bne.S	.GotFile

.GetFile	Lea	AskFile(PC),A0
	Bsr	TypeMessage
	Move.L	_TextHandle,D1
	Move.L	#_FileName,D2
	Moveq.L	#120,D3
	CALL	Read
	Cmp.B	#1,D0
	Ble	CloseDown

	Lea	_FileName,A0
	Add.L	D0,A0
	Move.B	#0,-1(A0)
	Bra.S	.LoadFile

.GotFile	Move.L	_ArgStart,A0
	Lea	_FileName,A1
	Move.L	_ArgSize,D0
	Subq	#2,D0
.GetLoop	Move.B	(A0)+,(A1)+
	DBra	D0,.GetLoop
	
***************************************
.LoadFile	Move.L	#_FileName,D1
	Tst.L	D1
	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Move.L	D0,_FileHandle

*****************************************************************************
Main:	Lea	AskSys(PC),A0
	Bsr	TypeMessage
	Move.L	_TextHandle,D1
	Move.L	#_SysOp,D2
	Moveq.L	#40,D3
	CALL	Read
	Cmp.B	#40,D0
	Bge	CloseDown
	Cmp.B	#1,D0
	Ble	CloseDown

	Lea	_SysOp,A0
	Moveq	#39,D1
	Moveq	#47,D2
.loop1	Move.B	(A0),D0
	Add.B	D2,D0
	Addq	#1,D2
	Move.B	D0,(A0)+
	DBra	D1,.loop1

***************************************
	Lea	AskBBS(PC),A0
	Bsr	TypeMessage

	Move.L	_TextHandle,D1
	Move.L	#_BBS,D2
	Moveq.L	#40,D3
	CALL	Read
	Cmp.B	#40,D0
	Bge	CloseDown

	Lea	_BBS,A0
	Moveq	#39,D1
	Moveq	#96,D2
.loop2	Move.B	(A0),D0
	Sub.B	D2,D0
	Subq	#1,D2
	Move.B	D0,(A0)+
	DBra	D1,.loop2

***************************************
	Lea	AskProgram(PC),A0
	Bsr	TypeMessage
	Move.L	_TextHandle,D1
	Move.L	#_ProgramID,D2
	Moveq.L	#14,D3
	CALL	Read
	Cmp.B	#14,D0
	Bge.S	CloseDown

	Lea	_ProgramID,A0
	Moveq	#11,D1
.loop3	Move.B	(A0),D0
	Add.B	#40,D0
	Move.B	D0,(A0)+
	DBra	D1,.loop3

***************************************
	Lea	AskSerial(PC),A0
	Bsr	TypeMessage
	Move.L	_TextHandle,D1
	Move.L	#_SerialNum,D2
	Moveq.L	#6,D3
	CALL	Read
	Cmp.B	#6,D0
	Bge.S	CloseDown

	Move.L	#_SerialNum,D1
	Move.L	#_Serial,D2
	CALL	StrToLong
	Move.L	_Serial,D0
	Not.W	D0
	Move.W	D0,_Serial

***************************************
	Lea	_SysOp,A0
	Moveq	#0,D0
	Moveq	#0,D1
	Moveq	#79,D2
.checkloop	Move.B	(A0)+,D0
	Add.L	D0,D1
	DBra	D2,.checkloop
	Move.W	D1,_CheckSum	

***************************************
	Move.L	_FileHandle,D1
	Move.L	#_SysOp,D2
	Moveq	#96,D3
	CALL	Write

*****************************************************************************
CloseDown:	Move.L	_FileHandle,D1
	CALL	Close

	Move.L	_TextHandle,D1
	CALL	Close

Exit:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary
NoDOS:	Moveq	#0,D0
	Move.L	_SaveStack,SP
	Rts

****************************************************************************
TypeMessage:	Move.L	A0,D2
	Moveq	#0,D3
.CalcLoop	Addq.L	#1,D3
	Cmp.B	#0,(A0)+
	Bne.S	.CalcLoop
	Subq	#1,D3
	Move.L	_TextHandle,D1
	CALL	Write
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
Con:	Dc.B	'CONSOLE:',0
Title:	Dc.B	27,'[1m'
	VERSION
	Dc.B	27,'[0m  © 1993  Eclipse Software',10,10,0
AskFile:	Dc.B	'Name to save key as: ',0
AskSys:	Dc.B	'SysOp''s name (40 chars): ',0
AskBBS:	Dc.B	'BBS''s name (40 chars): ',0
AskSerial:	Dc.B	'Serial Number (xxxx): ',0
AskProgram:	Dc.B	'ProgramID (12 chars): ',0
	Even

*****************************************************************************
	Section	DataSpace,BSS
_SaveStack:	Ds.L	1

DataStart:
_FileHandle:	Ds.L	1
_ArgSize:	Ds.L	1
_ArgStart:	Ds.L	1

_TextHandle:	Ds.L	1
_DOSBase:	Ds.L	1

_SysOp:	Ds.B	40
_BBS:	Ds.B	40
_Serial:	Ds.W	1
_CheckSum:	Ds.W	1
_ProgramID:	Ds.B	14
_SerialNum:	Ds.B	6
_FileName:	Ds.B	120
DataSize	Equ	*-DataStart
