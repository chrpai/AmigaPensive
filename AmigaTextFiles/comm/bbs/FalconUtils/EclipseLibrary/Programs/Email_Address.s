;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	EMailAddress,Code
	Opt	C+
	Include	mysystem.gs
	Include	Library/Eclipse.i

	Output	Work:Address

VERSION	MACRO
	Dc.B	'EMail Address V1.00'
	ENDM
DATE	MACRO
	Dc.B	'20.10.92'
	ENDM
AUTHOR	MACRO
	Dc.B	'(c) 1992  David Dustin'
	ENDM

***************************************************************************
* Notes:
***************************************************************************
Start:	Move.L	SP,_SaveStack

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Bne	fromCLI

	Lea	pr_MsgPort(A4),A0
	CALL	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,_WBMessage

fromCLI	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoDOSLib
	Move.L	D0,_DOSBase

	Lea	ECLIPSELibrary(PC),A1
	Moveq	#0,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoECLIPSELib
	Move.L	D0,_ECLIPSEBase
	
	Move.L	#WindowName,D1
	Move.L	#MODE_READWRITE,D2
	CALLDOS	Open
	Tst.L	D0
	Beq.S	Exit
	Move.L	D0,_WindowHandle
	
	Lea	FileName(PC),A0
	Moveq	#0,D0
	Lea	_FileStart,A1
	Lea	_FileSize,A2
	CALLECLIPSE	ReadFile
	Tst.L	D0
	Bne.S	NoFile
	
	Move.L	_WindowHandle,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALL	OutputText

	Move.L	_WindowHandle,D1
	Move.L	#FileName,D2
	Moveq	#1,D3
	CALLDOS	Read

	Move.L	_FileStart,A1
	Move.L	_FileSize,D0
	CALLEXEC	FreeMem

NoFile:	Move.L	_WindowHandle,D1
	CALLDOS	Close
	
Exit:	Move.L	_ECLIPSEBase,A1
	CALLEXEC	CloseLibrary

NoECLIPSELib:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary

NoDOSLib:	Move.L	_WBMessage,A1
	Cmp.L	#0,A1
	Beq.S	NoWB

	CALLEXEC	Forbid
	CALL	ReplyMsg

NoWB:	Moveq	#0,D0
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
WindowName:	Dc.B	'RAW:0/20/640/200/'
	VERSION
	Dc.B	0	
FileName:	Dc.B	'S:Addresses',0
	Even


***************************************
	Section	DataSpace,BSS
_DOSBase:	Ds.L	1
_ECLIPSEBase:	Ds.L	1
_WBMessage	Ds.L	1
_SaveStack:	Ds.L	1
_WindowHandle:	Ds.L	1
_FileStart:	Ds.L	1
_FileSize:	Ds.L	1
