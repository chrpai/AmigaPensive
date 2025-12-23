;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	Survey,Code
	Opt	C+,D+
	Include	mysystem.gs
	Include	library/eclipse.i

	Output	Work:Survey

VERSION	MACRO
	Dc.B	'Survey V1.00'
	ENDM
DATE	MACRO
	Dc.B	'29.10.92'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

***************************************************************************
* Notes:
*	1.00	First version.  Rewrite of Questionare
*
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
	Beq	Exit
	
	Move.B	(A0)+,Ident
	Lea	_UserName,A1
	CALLECLIPSE	CopyData

********************************
	Lea	PrefName(PC),A0
	Bsr	.GetFile
	Lea	SPath(PC),A0
	Bsr	.GetFile
	Move.B	#'V',SPath
	Lea	ENVPath(PC),A0
	Bsr	.GetFile
		
	Moveq	#0,D1
	Move.L	#NoPrefText,D2
	Moveq	#NoPrefTextLen,D3
	CALL	OutputText
	Bra	Exit

.GetFile	Moveq	#0,D0
	Lea	_PrefStart,A1
	Lea	_PrefSize,A2
	CALL	ReadFile
	Tst.L	D0
	Beq.S	FileOk
	Rts

******************************
FileOk:	Move.L	_PrefStart,_PrefPos
	CALLDOS	Output
	Move.L	D0,_OutputHandle
	
	Move.B	#10,CreditReturn
	Move.L	_OutputHandle,D1
	Move.L	#CreditText,D2
	Move.L	#CreditTextLen,D3
	CALL	Write

******************************
DoWelcome:	Bsr	GetNextEntry

	Bsr	LoadTempFile
	
	Tst.L	D0
	Bne.S	DoQuestions
	
	Move.L	_OutputHandle,D1
	Move.L	_TempFileStart,D2
	Move.L	_TempFileSize,D3
	CALLDOS	Write

	Bsr	FreeTempFile

******************************
DoQuestions:	Bsr	GetNextEntry

	Bsr	LoadTempFile

	Tst.L	D0
	Beq.S	.GotQuestions
	
	Move.L	_OutputHandle,D1
	Move.L	#NoQuestions,D2
	Moveq	#NoQuestionsLen,D3
	CALLDOS	Write
	Bra	UnLoadPrefs

.GotQuestions	Bsr	GetNextEntry

	Move.L	#_FileName,D1
	Move.L	#MODE_READWRITE,D2
	CALLDOS	Open
	Tst.L	D0
	Bne.S	.GotAnswerFile

	Move.L	_OutputHandle,D1
	Move.L	#NoAnswer,D2
	Moveq	#NoAnswerLen,D3
	CALLDOS	Write
	Bsr	FreeTempFile
	Bra	UnLoadPrefs

.GotAnswerFile	Move.L	D0,_AnswerHandle
	Move.L	#_DateStamp,D1
	CALLDOS	DateStamp

	Move.L	_AnswerHandle,D1
	Move.L	#_UserName,D2
	Moveq	#0,D3
	CALLECLIPSE	OutputText

	Move.B	#'	',_DateString

	Lea	_DateString+1,A0
	Lea	_DateStamp,A1
	CALL	Time12
	
	Move.B	#' ',(A0)+
	Move.B	#' ',(A0)+

	Lea	_DateStamp,A1
	CALL	LongDay
	Move.B	#',',(A0)+
	Move.B	#' ',(A0)+
	
	Lea	_DateStamp,A1
	CALL	Date

	Move.L	_AnswerHandle,D1
	Move.L	#_DateString,D2
	Moveq	#0,D3
	CALL	OutputText


	Move.L	_AnswerHandle,D1
	CALLDOS	Close
	Bsr	FreeTempFile

******************************
DoGoodbye:	Bsr	GetNextEntry

	Bsr	LoadTempFile
	
	Tst.L	D0
	Bne.S	UnLoadPrefs
	
	Move.L	_OutputHandle,D1
	Move.L	_TempFileStart,D2
	Move.L	_TempFileSize,D3
	CALLDOS	Write

	Bsr	FreeTempFile

******************************
UnLoadPrefs:	Move.L	_PrefStart,A1
	Move.L	_PrefSize,D0
	CALLEXEC	FreeMem

Exit:	Move.L	_ECLIPSEBase,A1
	CALLEXEC	CloseLibrary

NoEclipseLib:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary

NoDOSLib:	Moveq	#0,D0
	Move.L	_SaveStack,SP
	Rts

***************************************************************************
***************************************************************************
fromWorkbench:	Lea	pr_MsgPort(A4),A0
	CALLEXEC	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,A1
	CALL	Forbid
	CALL	ReplyMsg
	Move.L	_SaveStack,SP
	Moveq	#0,D0
	Rts

***************************************************************************
GetNextEntry:	Move.L	_PrefPos,A0
	Lea	_FileName,A1
	Moveq	#127,D0
	CALLECLIPSE	CopyData
	Move.B	#0,(A1)
	Move.L	A0,_PrefPos
	Rts

***************************************************************************
LoadTempFile:	Moveq	#0,D0
	Lea	_FileName,A0
	Lea	_TempFileStart,A1
	Lea	_TempFileSize,A2
	CALLECLIPSE	ReadFile
	Rts

***************************************************************************
FreeTempFile:	Move.L	_TempFileStart,A1
	Move.L	_TempFileSize,D0
	CALLEXEC	FreeMem
	Rts

***************************************************************************
***************************************************************************
***************************************************************************
VerString:	Dc.B	0,'$VER: '
CreditText:	VERSION
	Dc.B	' ('
	DATE
	Dc.B	') '
CreditReturn:	Dc.B	' '
	AUTHOR
	Dc.B	10,10
CreditTextLen	Equ	*-CreditText

******************************
DOSLibrary:	DOSNAME
ECLIPSELibrary:	EclipseName

******************************
ENVPath:	Dc.B	'EN'
SPath:	Dc.B	'S:'
PrefName:	Dc.B	'Survey_'
Ident:	Dc.B	' .prefs',0

******************************
NoPrefText:	Dc.B	10,'The survey preference file couldn''t be found/loaded.',10
NoPrefTextLen	Equ	*-NoPrefText
NoQuestions:	Dc.B	10,'I couldn''t find the file with the questions....',10
NoQuestionsLen	Equ	*-NoQuestions
NoAnswer:	Dc.B	10,'I couldn''t open the answer file....',10
NoAnswerLen	Equ	*-NoQuestions
	Even

***************************************************************************
	Section	DataSpace,BSS
_DOSBase:	Ds.L	1
_ECLIPSEBase:	Ds.L	1
_OutputHandle:	Ds.L	1
_SaveStack:	Ds.L	1
_DateStamp:	Ds.L	3

_PrefStart:	Ds.L	1
_PrefSize:	Ds.L	1
_PrefPos:	Ds.L	1

_TempFileStart:	Ds.L	1
_TempFileSize:	Ds.L	1
_TempFilePos:	Ds.L	1

_AnswerHandle:	Ds.L	1

_UserName:	Ds.B	40
_DateString:	Ds.B	30
_FileName:	Ds.B	128
