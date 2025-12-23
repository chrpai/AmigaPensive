;Released as public domain 28-Jan-1996 by the author Dave Dustin
	Section	Cleanup,Code
	Opt	C+;,D+

	Include	mysystem.gs
	Include	excel/itemdata.i
;--------------------------------------------------------------------------
Start:	Move.L	$4.w,A6
	Move.L	A6,_EXECBase

	Lea	DOSLib(PC),A1
	Moveq	#36,D0
	CALL	OpenLibrary
	Move.L	D0,_DOSBase
	Beq	.NoDOS

	Move.L	#ArgumentTemp,D1
	Move.L	#_Cleanup,D2
	Moveq	#0,D3
	CALLPCDOS	ReadArgs
	Beq	.NoArguments
	Move.L	D0,_Argument

	Move.L	#Title,D1
	CALLPCDOS	PutStr

	Tst.L	_MBase
	Beq.S	.UseFBase
	Move.B	#'M',FBaseName
	Move.L	#770,_FPathOffset

.UseFBase	Tst.L	_Correct
	Bne.S	.Main
	Tst.L	_Cleanup
	Bne.S	.Main
	
	Move.L	#Info,D1
	CALLPCDOS	PutStr
	Bra	.NoSystemData
;------main
.Main	Move.L	#ItemDataName,D1
	Moveq	#ACCESS_READ,D2
	CALLPCDOS	Lock
	Tst.L	D0
	Beq.S	.NoLocal

	Move.L	D0,D1
	CALL	UnLock

	Move.L	#Local,D1
	CALL	PutStr

	Tst.L	_Correct
	Beq.S	.LClean
	Move.L	#CorrectionTxt,D1
	CALL	PutStr
	Bsr	Correct
	Bra.S	.LocalDone
.LClean	Move.L	#CleanupTxt,D1
	CALL	PutStr
	Bsr	Cleanup
.LocalDone	Move.L	#Complete,D1
	CALLPCDOS	PutStr
	Bra	.NoSystemData

.NoLocal	Lea	SysDataName(PC),A0
	Lea	_DataStart(PC),A1
	Lea	_DataSize(PC),A2
	Bsr	LoadFile
	Bne	.NoSystemData

	Move.L	_DataStart(PC),A0	;data
	Lea	386(A0),A0
	Lea	FBase(PC),A1
	Bsr	CopyData
	Lea	FBaseName(PC),A0
	Bsr	CopyData
	Move.B	#0,(A1)

	Move.L	_DataStart(PC),A0	;files
	Add.L	_FPathOffset,A0
	Lea	FileArea(PC),A1
	Bsr	CopyData
	Move.L	A1,_AreaMarker

	Lea	FBase(PC),A0
	Lea	_FBaseStart(PC),A1
	Lea	_FBaseSize(PC),A2
	Bsr	LoadFile
	Bne	.NoFBase

	Move.L	#CommentTxt,D1
	CALLPCDOS	PutStr

	Move.L	#CleanupTxt,D1
	Tst.L	_Correct
	Beq.S	.TypeClean
	Move.L	#CorrectionTxt,D1

.TypeClean	CALL	PutStr

	Move.L	#InProgress,D1
	CALL	PutStr

	Move.L	_FBaseStart(PC),A5
	Move.L	_FBaseSize(PC),D7
	Divu.W	#332,D7
	Subq	#1,D7

.Processloop	Move.B	177(A5),D1	;get identity flag (I hope :)

	Cmp.B	#$20,D1	;check to see if it's a normal area
	Beq.S	.Area

	Cmp.B	#$04,D1	;check to see if it's a usenet area
	Beq.S	.Usenet

	Cmp.B	#$10,D1	;check to see if it's a directory
	Beq.S	.SubDir

	Move.L	#Tab,D1
	CALLPCDOS	PutStr
	Move.L	A5,D1
	CALL	PutStr
	Move.L	#Unknown,D1
	CALL	PutStr
	Bra	.SkipIt
.Usenet
	Move.L	#Tab,D1
	CALLPCDOS	PutStr
	Move.L	A5,D1
	CALL	PutStr
	Move.L	#Usenet,D1
	CALL	PutStr
	Bra.S	.SkipIt

.SubDir	Move.L	#Bold,D1
	CALLPCDOS	PutStr
	Move.L	A5,D1
	CALL	PutStr
	Move.L	#BoldOff,D1
	CALL	PutStr
	Bra.S	.SkipIt

.Area	Lea	31(A5),A0
	Move.L	_AreaMarker(PC),A1
	Bsr	CopyData
	Move.B	#'/',(A1)+
	Move.L	A1,_FileMarker

	Move.L	#Tab,D1
	CALLPCDOS	PutStr
	Move.L	A5,D1
	CALL	PutStr

	Tst.L	_Correct
	Beq.S	.Clean
	Bsr	Correct
	Bra.S	.Done
.Clean	Bsr.S	Cleanup

.Done	Move.L	#Cr,D1
	CALLPCDOS	PutStr

.SkipIt	Move.L	#SIGBREAKF_CTRL_C,D1
	Moveq	#0,D0
	CALLPCEXEC	SetSignal
	Btst.L	#SIGBREAKB_CTRL_C,D0
	Beq.S	.NoBreak

	Move.L	#Break,D1
	CALLPCDOS	PutStr
	Bra.S	.Closedown

.NoBreak	Lea	332(A5),A5
	DBra	D7,.Processloop

;------closedown
.Closedown	Move.L	_FBaseStart(PC),A1
	Move.L	_FBaseSize(PC),D0
	CALLPCEXEC	FreeMem

.NoFBase	Move.L	_DataStart(PC),A1
	Move.L	_DataSize(PC),D0
	CALLPCEXEC	FreeMem

.NoSystemData	Move.L	_Argument(PC),D1
	CALLPCDOS	FreeArgs

.NoArguments	Move.L	_DOSBase(PC),A1
	CALLPCEXEC	CloseLibrary

.NoDOS	Moveq	#0,D0
	Rts

;--------------------------------------------------------------------------
	Include	Cleanup.s
	Include	Correct.s
	Include	LoadFile.s
	Include	CopyData.s
;--------------------------------------------------------------------------
Version:	Dc.B	0,'$VER: CleanupComments 1.1 (13.2.94)',0
DOSLib:	DOSNAME
ArgumentTemp:	Dc.B	'CLEANUP/S,CORRECT/S,MBASE/S',0
SysDataName:	Dc.B	'SystemData',0
FBaseName:	Dc.B	'FBase.dat',0
ItemDataName:	Dc.B	'_itemdata',0
CommentsName:	Dc.B	'_comments',0
Title:	Dc.B	10,'EXCELSIOR! BBS File Comments Cleanup v1.1',10,'© Copyright 1994 Eclipse Software, All Rights Reserved.',10,10,0
Info:	Dc.B	'You MUST read the manual before attempting to use this program!',10,0
Tab:	Dc.B	'   ',0
Bold:	Dc.B	10,$9B,'1m',0
BoldOff:	Dc.B	$9B,'0m'
Cr:	Dc.B	10,0
Usenet:	Dc.B	'   [UNET area]',10,0
Unknown:	Dc.B	'   [Unknown entry type]',10,0
CommentTxt:	Dc.B	'Comment ',0
Local:	Dc.B	'Local ',0
CorrectionTxt:	Dc.B	'Correction ',0
CleanupTxt:	Dc.B	'Cleanup ',0
InProgress:	Dc.B	'in progress',10,10,0
Complete:	Dc.B	'complete',10,0
Break:	Dc.B	'***Break',10,0
	Even

_DOSBase:	Dc.L	0
_EXECBase:	Dc.L	0

_Argument:	Dc.L	0
_Cleanup:	Dc.L	0
_Correct:	Dc.L	0
_MBase:	Dc.L	0,0

_DataStart:	Dc.L	0
_DataSize:	Dc.L	0

_FBaseStart:	Dc.L	0
_FBaseSize:	Dc.L	0
_FPathOffset:	Dc.L	898

_ItemStart:	Dc.L	0
_ItemSize:	Dc.L	0
_ItemHandle:	Dc.L	0

_CommentStart:	Dc.L	0
_CommentSize:	Dc.L	0
_CommentHandle:	Dc.L	0

_AreaMarker:	Dc.L	0
_FileMarker:	Dc.L	FileArea

FBase:	Ds.B	140
FileArea:	Ds.B	170
