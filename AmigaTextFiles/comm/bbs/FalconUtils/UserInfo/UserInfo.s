;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	UserInfo,Code
	Opt	C+
	Include	mysystem.gs
	Include	eclipse.i
	Include	libraries/reqtools.i
	Include	libraries/reqtools_lib.i
	Output	work:UserInfo

VERSION	MACRO
	Dc.B	'Falcon CBCS UserInfo V2.22'
	ENDM
DATE	MACRO
	Dc.B	'24.12.92'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

*****************************************************************************
* Notes:
*	2.01 - Optimized
*	       Now uses Eclipse.library
*	2.02 - Changed search paramaters
*	2.02b- Now needs only one keypress
*	       Dos V37+ only
*	2.03 - Added AppFile support
*	       Config option added
*	2.04 - Cleaned up a little bit
*	     - Provided a SysOp mode for AppFile
*	2.05 - Added comment feild on the name
*	2.06 - Corrected error with scanned entry
*	2.07 - Fixed bug if all config options are not
*	       selected.  Would not read correct line.
*	2.08 - Fixed bug with SysOp info mode.
*	2.09 - Added password to SysOp info mode
*	     - Added extra path for comments file
*	2.09b- Fixed same bug as was fixed in v2.06
*	2.10 - Modified config file handling
*	2.11 - ReWrote infomation output routine.  Now uses
*	       Exec's RawDoFmt()
*	2.20 - Added ReqTools option (-r)
*	     - Usable from WB as well (ReqTools)
*	     - Made password entry invisible (defualt ON)
*	     - Log output is now optional (defualt ON)
*	2.21 - Added backspace to hidden console password entry
*	2.21b- Fixed small bug with single key-press reader
*	     - Fixed error checking routine that prevented
*	       ReqTools from being used :(
*	2.22 - Fixed bug with lengths of info
*
*****************************************************************************
Start:	Move.L	D0,_ArgSize
	Move.L	A0,_ArgStart
	Move.L	SP,_SaveStack

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Bne.S	fromCLI

	Lea	pr_MsgPort(A4),A0
	CALL	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,_WBMessage
	Move.B	#1,UseReqs

fromCLI:	Lea	DataStart,A0
	Move.L	#DataSize-1,D0

.ClearDataSpace	Move.B	#0,(A0)+
	DBra	D0,.ClearDataSpace

	Move.L	#'NNNN',Display
	Move.B	#0,ProgramEnd

	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoDOS
	Move.L	D0,_DOSBase

	Lea	ECLIPSELibrary(PC),A1
	Moveq	#EclipseVer,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	ExitToDos
	Move.L	D0,_ECLIPSEBase

	Lea	RTLibrary(PC),A1
	Moveq	#38,D0
	CALL	OpenLibrary
	Move.L	D0,_ReqToolsBase
	Tst.L	D0
	Bne.S	.RTOk
	Move.B	#0,UseReqs
	Tst.L	_WBMessage
	Bne	ExitToEclipse

.RTOk	Bsr	GetScreen
	Bsr	LoadConfig

	Tst.B	UseReqs
	Bne.S	.UseReq

	CALLDOS	Input
	Move.L	D0,_InputHandle
	CALL	Output
	Move.L	D0,_OutputHandle
	
	Move.L	_ArgStart,A0
	Move.L	_ArgSize,D0

	Cmp.B	#'?',(A0)
	Beq	Exit
	Cmp.B	#1,D0
	Beq	Exit
	Cmp.B	#0,D0
	Beq	Exit

	Lea	UserName,A1
	CALLECLIPSE	CopyData

	Cmp.B	#0,UserName
	Beq	Exit

	Cmp.W	#'-r',UserName
	Bne.S	.NoReq

	Tst.L	_ReqToolsBase
	Beq	Exit

.UseReq	Move.L	#0,UserName
	Move.B	#1,UseReqs

	Move.L	_ReqToolsBase,A6
	Lea	UserName,A1
	Moveq	#39,D0
	Lea	ProgramName(PC),A2
	Sub.L	A3,A3
	Lea	GetNameTags(PC),A0
	Jsr	_LVOrtGetStringA(A6)
	Tst.L	D0
	Beq	Exit
	
.NoReq	Lea	UserName,A0
	Cmp.B	#'@',(A0)
	Bne	.NotSysOp
	
	Lea	UserName,A0
	Addq	#1,A0
	Lea	UserName,A1
	Moveq	#39,D0
	CALLECLIPSE	CopyData
	Move.B	#0,(A1)

	Tst.B	UserName
	Beq	Exit

	Move.B	HidePassword,D0
	Bsr	ChangeCase
	Move.B	D0,HidePassword
	Cmp.B	#'N',D0
	Bne.S	.NoHide
	Move.L	#FALSE,GetPassTags+4

.NoHide	Tst.B	UseReqs
	Beq.S	.NoPassReq

	Move.L	_ReqToolsBase,A6
	Lea	PassReadBuffer,A1
	Moveq	#19,D0
	Lea	ProgramName(PC),A2
	Sub.L	A3,A3
	Lea	GetPassTags(PC),A0
	Jsr	_LVOrtGetStringA(A6)
	Tst.L	D0
	Beq	.NotSysOp

	Bra	.PassError

.NoPassReq	Move.L	_OutputHandle,D1
	Move.L	#AskPass,D2
	Moveq	#27,D3
	CALLDOS	Write

	Cmp.B	#'N',HidePassword
	Beq	.NormalEntry
	
.DoHiddenPass	Move.L	_InputHandle,D1
	Moveq	#1,D2
	CALL	SetMode

	Lea	PassReadBuffer,A5

.GetPassLoop	Move.L	_InputHandle,D1
	Move.L	#ReadBuffer,D2
	Moveq	#1,D3
	CALL	Read
	Addq.B	#1,PassCount
	
	Move.B	ReadBuffer,D0
	Cmp.B	#10,D0
	Beq.S	.PassFinished
	Cmp.B	#13,D0
	Beq.S	.PassFinished

	Cmp.B	#8,D0
	Bne.S	.NotBackSpace

	Tst.B	PassCount
	Beq.S	.GetPassLoop

	Subq.B	#1,PassCount
	Subq	#1,A5
	Move.L	_OutputHandle,D1
	Move.L	#BackSpace,D2
	Moveq	#3,D3
	CALL	Write
	Bra.S	.GetPassLoop
		
.NotBackSpace	Move.B	D0,(A5)+
	Cmp.B	#20,PassCount
	Bge.S	.PassFinished

	Move.L	_OutputHandle,D1
	Move.L	#Hash,D2
	Moveq	#1,D3
	CALL	Write
	Bra.S	.GetPassLoop
	
.PassFinished	Move.B	#0,(A5)+
	Bsr	DoCR
	Move.L	_InputHandle,D1
	Moveq	#0,D2
	CALL	SetMode
	Bra.S	.PassEntered

.NormalEntry	Move.L	_InputHandle,D1
	Move.L	#PassReadBuffer,D2
	Moveq	#20,D3
	CALL	Read

.PassEntered	Bsr	DoCR

.PassError	Cmp.B	#0,PasswordBuffer
	Beq.S	.NotSysOp

	Lea	PasswordBuffer,A0
	Lea	PassReadBuffer,A1
	CALLECLIPSE	CompareStrings
	Tst.L	D0
	Beq.S	.IsSysop
	
	Sub.L	#$40404040,MyPass
	Sub.L	#$40404040,MyPass+4

	Lea	MyPass,A0
	Lea	PassReadBuffer,A1
	CALL	CompareStrings
	Tst.L	D0
	Bne.S	.NotSysOp

.IsSysop	Move.L	#'YYYY',Display
			Move.L	#'YYYY',UserPassword
.NotSysOp	Move.L	#UserFile,D1
	CALLECLIPSE	GetFile
	Tst.L	D0
	Beq	Exit

	Move.L	D0,_UserHandle
	Move.L	D1,A0
	Move.L	fib_Size(A0),_UserBaseSize

	Tst.B	UseReqs
	Bne.S	.NoChange
	Move.L	_InputHandle,D1
	Moveq	#1,D2
	CALLDOS	SetMode

.NoChange	Move.B	NoLog,D0
	Bsr	ChangeCase
	Move.B	D0,NoLog

	Cmp.B	#'Y',NoLog
	Beq.S	.NoLogging
	Lea	LookedText(PC),A0
	Lea	ScanSpace,A1
	Moveq	#LookedLen,D0
	CALLECLIPSE	CopyData

	Lea	ScanSpace,A0
	CALL	LogTag
.NoLogging
*****************************************************************************
MainLoop:	Addq.L	#1,UserCounter

	Move.L	_UserHandle,D1
	Move.L	#UserData,D2
	Move.L	#512,D3
	CALLDOS	Read
	Tst.L	D0
	Beq.S	NoUserError

	Lea	UserName,A0
	Lea	UserData,A1
	CALLECLIPSE	FindData
	Tst.L	D0
	Beq.S	FoundPos

NextUser:	Add.L	#512,FilePos
	Move.L	FilePos,D0
	Move.L	_UserBaseSize,D1
	Cmp.L	D0,D1
	Beq.S	NoUserError
	Bra.S	MainLoop

NoUserError:	Tst.B	UseReqs
	Bne.S	.NoUser
	Move.L	_OutputHandle,D1
	Move.L	#NoUser,D2
	Moveq	#NoUserLen,D3
	CALLDOS	Write
.NoUser	Bra	Error

FoundPos:	Tst.B	UseReqs
	Beq.S	.NoReq

	Move.L	_ReqToolsBase,A6
	Lea	OkTags(PC),A0
	Lea	LookedText(PC),A1
	Lea	OkGadgets(PC),A2
	Lea	ScanData(PC),A4
	Sub.L	A3,A3
	Jsr	_LVOrtEZRequestA(A6)
	Cmp.L	#0,D0
	Beq.S	NextUser
	Cmp.L	#2,D0
	Beq	AbortExit
	Bra.S	TypeInfo

.NoReq	Lea	Found(PC),A0
	Lea	FoundData(PC),A1
	Bsr	MyPrintf

	Move.L	_InputHandle,D1
	Move.L	#ReadBuffer,D2
	Moveq	#1,D3
	CALLDOS	Read

	Tst.L	D0
	Beq	Error

	Move.L	_OutputHandle,D1
	Move.L	#ReadBuffer,D2
	Moveq	#2,D3
	CALLDOS	Write

	Move.B	ReadBuffer,D0
	Bsr	ChangeCase
	Cmp.B	#'N',D0
	Beq	NextUser
	Cmp.B	#'A',D0
	Beq	AbortExit

*****************************************************************************
HOMEADD	Equ	0
AGE	Equ	HOMEADD+80
PHONE	Equ	AGE+10
COMPUTER	Equ	PHONE+20
LASTCALLED	Equ	COMPUTER+80
ACCESS	Equ	LASTCALLED+30
NAME	Equ	ACCESS+10
DATASIZE	Equ	NAME+80

TypeInfo:	Bsr	GetAppFile

	Move.L	#DATASIZE,D0
	Move.L	#MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Beq	NoDataMem
	Move.L	D0,_DataSpace

	Move.L	#2000,D0
	Move.L	#MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Beq	NoDataMem2
	Move.L	D0,_DataSpace2

;username
	Lea	UserData,A0
	Move.L	_DataSpace,A1
	Lea	NAME(A1),A1
	Move.L	A1,Name
	Moveq	#36,D0
	CALLECLIPSE	CopyData
	
	Bsr	GetComment

;access level
	Lea	UserData+135,A0
	Moveq	#0,D0
	Move.B	(A0),D0
	Lsl.L	#2,D0
	Lea	Levels(PC),A1
	Move.L	(A1,D0.w),A0
	Move.L	_DataSpace,A1
	Lea	ACCESS(A1),A1
	Move.L	A1,Lvl
	Moveq	#8,D0
	CALL	CopyData

;Last Date
	Lea	UserData+124,A0
	Moveq	#0,D0
	Move.L	(A0),D0
	Lea	DateSpace,A0
	CALL	ConvertFalconDate

	Move.L	_DataSpace,A0
	Lea	LASTCALLED(A0),A0
	Move.L	A0,Called
	Lea	DateSpace,A1
	CALL	Time12
	Move.B	#' ',(A0)+
	Move.B	#' ',(A0)+

	Lea	DateSpace,A1
	CALL	ShortDay
	Move.B	#',',(A0)+
	Move.B	#' ',(A0)+

	Lea	DateSpace,A1
	CALL	Date

;address
	Lea	Display,A0
	Cmp.B	#'Y',(A0)
	Bne.S	.NoAddress
	Move.L	UserAppData,A0
	Move.L	_DataSpace,A1
	Move.L	A1,Address
	Moveq	#60,D0
	CALL	CopyData

.NoAddress

;phone number
	Bsr	SkipALine
	Lea	Display,A0
	Cmp.B	#'Y',1(A0)
	Bne.S	.NoPhone
	Move.L	UserAppData,A0
	Move.L	_DataSpace,A1
	Lea	PHONE(A1),A1
	Move.L	A1,Phone
	Moveq	#20,D0
	CALL	CopyData
.NoPhone

;age
	Bsr	SkipALine
	Lea	Display,A0
	Cmp.B	#'Y',2(A0)
	Bne.S	.NoAge
	Move.L	UserAppData,A0
	Move.L	_DataSpace,A1
	Lea	AGE(A1),A1
	Move.L	A1,Age
	Moveq	#2,D0
	CALL	CopyData
.NoAge

;computer type
	Bsr	SkipALine
	Lea	Display,A0
	Cmp.B	#'Y',3(A0)
	Bne.S	.NoComputer
	Move.L	UserAppData,A0
	Move.L	_DataSpace,A1
	Lea	COMPUTER(A1),A1
	Move.L	A1,Computer
	Moveq	#60,D0
	CALL	CopyData

.NoComputer
	Move.L	UserCounter,_FormatData
	Lea	UserData+118,A0
	Move.W	(A0)+,Time+2
	Move.W	(A0)+,Calls+2
	Move.W	(A0),Msgs+2
	Lea	UserData+150,A0
	Move.L	(A0),Down
	Addq	#8,A0
	Move.L	(A0),Up

	Move.B	UserPassword,D0
	Bsr	ChangeCase
	Cmp.B	#'Y',D0
	Bne.S	.NoPassword
	Move.L	#UserData+36,Password
	
.NoPassword	Bsr	OutputInfo
	Bsr	DoCR
	Bsr	DoCR

*****************************************************************************
	Cmp.B	#'Y',NoLog
	Beq.S	.NoLogging

	Lea	ViewText(PC),A0
	Lea	ViewSpace,A1
	Moveq	#7,D0
	CALLECLIPSE	CopyData

	Lea	ViewSpace,A0
	CALL	LogTag
.NoLogging

AbortExit:	Move.L	_DataSpace2,A1
	Move.L	#2000,D0
	CALLEXEC	FreeMem

NoDataMem2:	Move.L	_DataSpace,A1
	Move.L	#DATASIZE,D0
	CALLEXEC	FreeMem
	
NoDataMem:	Move.L	_AppStart,A1
	Cmp.L	#0,A1
	Beq.S	Error
	Move.L	_AppSize,D0
	CALLEXEC	FreeMem

Error:	Tst.B	UseReqs
	Bne.S	.NoReq
	Move.L	_InputHandle,D1
	Moveq	#0,D2
	CALLDOS	SetMode

.NoReq	Move.L	_UserHandle,D1
	CALLDOS	Close

Exit:	Move.L	_ReqToolsBase,A1
	Cmp.L	#0,A1
	Beq.S	ExitToEclipse
	CALLEXEC	CloseLibrary

ExitToEclipse:	Move.L	_ECLIPSEBase,A1
	Cmp.L	#0,A1
	Beq.S	ExitToDos
	CALLEXEC	CloseLibrary

ExitToDos:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary

NoDOS:	Move.L	_WBMessage,A1
	Cmp.L	#0,A1
	Beq.S	.NoWB
	CALLEXEC	Forbid
	CALL	ReplyMsg

.NoWB	Moveq	#0,D0
	Move.L	_SaveStack,SP
	Rts

*****************************************************************************
LoadConfig:	Lea	ConfigName+4,A0
	Bsr.S	.Load
	Beq.S	.Loaded

	Lea	ConfigName,A0
	Bsr.S	.Load
	Bne.S	.Exit

.Loaded	Lea	ConfigData,A0
	Move.L	_ConfigStart,A1
	Move.L	_ConfigSize,D0
	Bsr.S	GetMyConfig

	Move.L	_ConfigStart,A1
	Move.L	_ConfigSize,D0
	CALLEXEC	FreeMem

.Exit	Rts

.Load	Moveq	#0,D0
	Lea	_ConfigStart,A1
	Lea	_ConfigSize,A2
	CALLECLIPSE	ReadFile
	Tst.L	D0
	Rts

*****************************************************************************
; A0	Pointer to config entries data table
;		.l    Config name
;		.l    Space for data
;		.w    Max size for data
; A1	Pointer to config data
; D0	Size of config data

LinkSize	Equ	12
C_Entries	Equ	-12
C_DataStart	Equ	-8
C_DataSize	Equ	-4

GetMyConfig:	Link	A5,#-LinkSize
	Move.L	A0,C_Entries(A5)
	Move.L	A1,C_DataStart(A5)
	Ext.L	D0
	Move.L	D0,C_DataSize(A5)

.GetEntries	Move.L	C_Entries(A5),A1
	Move.L	(A1)+,A0
	Cmp.L	#0,A0
	Beq.S	.End
	Move.L	(A1)+,-(SP)
	Move.W	(A1)+,-(SP)
	Move.L	A1,C_Entries(A5)
	Bsr.S	.FindData
	Move.L	A1,A0
	Move.W	(SP)+,D1
	Move.L	(SP)+,A1
	Tst.L	D0
	Bne.S	.GetEntries
	Cmp.B	#'=',(A0)
	Bne.S	.GetEntries
	Addq	#1,A0
	Move.L	D1,D0
	Bsr.S	.CopyData
	Move.B	#0,(A1)
	Bra.S	.GetEntries
	
.End	Unlk	A5
	Moveq	#0,D0
	Rts

.FindData	Move.L	C_DataStart(A5),A1
	Move.L	A1,A2
	Add.L	C_DataSize(A5),A2
.FindDataLoop	Moveq	#0,D0
	Moveq	#0,D1
	Move.B	(A0),D0
	Bsr.S	ChangeCase
	Move.B	D0,D1
	Move.B	(A1)+,D0
	Tst.B	D0
	Beq.S	.Exit
	Cmp.L	A2,A1
	Bge.S	.Exit
	Bsr.S	ChangeCase
	Cmp.B	D0,D1
	Bne.S	.FindDataLoop

	Lea	1(A0),A3
	Move.L	A1,A4
.findloop	Move.B	(A3)+,D0
	Tst.B	D0
	Beq.S	.Found
	Bsr.S	ChangeCase
	Move.B	D0,D1
	Move.B	(A4)+,D0
	Cmp.L	A2,A4
	Bge.S	.Exit
	Bsr.S	ChangeCase
	Cmp.B	D0,D1
	Bne.S	.FindDataLoop
	Bra.S	.findloop
.Exit	Moveq	#-1,D0
	Rts
.Found	Lea	-1(A1),A0
	Move.L	A4,A1
	Moveq	#0,D0
	Rts

.CopyData	Subq	#1,D0
.loop	Move.B	(A0)+,D1
	Cmp.B	#0,D1
	Beq.S	.stop
	Cmp.B	#10,D1
	Beq.S	.stop
	Cmp.B	#'"',D1
	Beq.S	.skip
	Move.B	D1,(A1)+
.skip	DBra	D0,.loop
.stop	Rts

*****************************************************************************
ChangeCase:	Cmp.B	#'a',D0
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
GetComment:	Move.L	A1,-(SP)
	Lea	CommentFile,A0
	Moveq	#0,D0
	Lea	_CommentStart,A1
	Lea	_CommentSize,A2
	CALLECLIPSE	ReadFile
	Bne.S	.Exit

	Lea	UserData,A0
	Move.L	_CommentStart,A1
	CALL	FindData
	Tst.L	D0
	Bne.S	.NoName
	Move.L	A1,A0
	Move.L	(SP),A1
	Moveq	#40,D0
	CALL	CopyData

.NoName	Move.L	_CommentStart,A1
	Move.L	_CommentSize,D0
	CALLEXEC	FreeMem

.Exit	Move.L	_ECLIPSEBase,A6
	Move.L	(SP)+,D0
	Rts

*****************************************************************************
GetAppFile:	Lea	AppFile,A0
	Moveq	#0,D0
	Lea	_AppStart,A1
	Lea	_AppSize,A2
	CALLECLIPSE	ReadFile
	Tst.L	D0
	Bne.S	ClearAppData

	Lea	UserData,A0
	Move.L	_AppStart,A1
	CALL	FindData
	Tst.L	D0
	Bne.S	.ClearAppFile
	Move.L	A0,UserAppData

	Bsr.S	SkipALine
	Bsr.S	SkipALine

	Rts

.ClearAppFile	Move.L	_AppStart,A1
	Move.L	_AppSize,D0
	CALLEXEC	FreeMem
	Move.L	#0,_AppStart
	Move.L	#0,_AppSize

ClearAppData:	Move.L	#'NNNN',Display
	Rts

*****************************************************************************
SkipALine:	Move.L	UserAppData,A0
.Skiploop	Cmp.B	#0,(A0)
	Beq.S	.Exit
	Cmp.B	#10,(A0)+
	Bne.S	.Skiploop

	Lea	10(A0),A0
.Exit	Move.L	A0,UserAppData
	Rts

*****************************************************************************
DoCR:	Tst.B	UseReqs
	Bne.S	.Exit
	Move.L	_OutputHandle,D1
	Move.L	#Ret,D2
	Moveq	#1,D3
	CALLDOS	Write
.Exit	Rts

*****************************************************************************
OutputInfo:	Tst.B	UseReqs
	Beq.S	.NoReq

	Move.L	_ReqToolsBase,A6
	Lea	InfoTags(PC),A0
	Lea	UsrNmb+2(PC),A1
	Lea	InfoGadgets(PC),A2
	Lea	_FormatData(PC),A4
	Sub.L	A3,A3
	Jsr	_LVOrtEZRequestA(A6)
	Rts

.NoReq	Lea	UsrNmb(PC),A0
	Lea	_FormatData(PC),A1

	Move.L	_DataSpace2,A3
	Lea	.CopyChar(PC),A2
	CALLEXEC	RawDoFmt

	Move.L	_DataSpace2,A0
	Bsr.S	CalcLen

	Move.L	_OutputHandle,D1
	Move.L	_DataSpace2,D2
	CALLDOS	Write
	Rts

.CopyChar	Move.B	D0,(A3)+
	Rts

CalcLen:	Moveq	#-1,D3
.FindEnd	Addq.L	#1,D3
	Cmp.B	#0,(A0)+
	Bne.S	.FindEnd
	Rts

*****************************************************************************
MyPrintf:	Lea	_FormatSpace,A3
	Moveq	#31,D0
.ClearLoop	Move.L	#0,(A3)+
	DBra	D0,.ClearLoop
	
	Lea	_FormatSpace,A3
	Lea	.CopyChar(PC),A2
	CALLEXEC	RawDoFmt

	Lea	_FormatSpace,A0
	Bsr.S	CalcLen

	Move.L	_OutputHandle,D1
	Move.L	#_FormatSpace,D2
	CALLDOS	Write
	Rts

.CopyChar	Move.B	D0,(A3)+
	Rts

*****************************************************************************
GetScreen:	Lea	IntuiLibrary(PC),A1
	Moveq	#37,D0
	CALLEXEC	OpenLibrary
	Tst.L	D0
	Beq.S	.NoIntui
	Move.L	D0,A1

	Move.L	ib_ActiveScreen(A1),D0
	Move.L	#RT_Screen,ScrTag1
	Move.L	D0,ScrTag1+4
	Move.L	#RT_Screen,ScrTag2
	Move.L	D0,ScrTag2+4
	Move.L	#RT_Screen,ScrTag3
	Move.L	D0,ScrTag3+4
	Move.L	#RT_Screen,ScrTag4
	Move.L	D0,ScrTag4+4

	CALL	CloseLibrary
.NoIntui	Rts

*****************************************************************************
*****************************************************************************
*****************************************************************************
VerString:	Dc.B	0,'$VER: '
ProgramName:	VERSION
ProgramEnd:	Dc.B	' ('
	DATE
	Dc.B	') '
	AUTHOR
	Dc.B	0

***************************************
DOSLibrary:	DOSNAME
ECLIPSELibrary:	EclipseName
RTLibrary:	REQTOOLSNAME
IntuiLibrary:	Dc.B	'intuition.library',0

ConfigName:	Dc.B	'ENV:UserInfo.prefs',0

_User:	Dc.B	'USERFILE',0
_App:	Dc.B	'APPFILE',0
_Comment:	Dc.B	'COMMENTFILE',0
_Display:	Dc.B	'DISPLAY',0
_Password:	Dc.B	'PASSWORD',0
_NoLog:	Dc.B	'NOLOG',0
_HidePassword:	Dc.B	'HIDEPASSWORD',0
_UserPassword:	Dc.B	'USERPASSWORD',0

ViewText:	Dc.B	'Viewed ',0

Twit:	Dc.B	'Twit',0
Disgrace:	Dc.B	'Disgrace',0
Normal:	Dc.B	'Normal',0
Privil:	Dc.B	'Privil',0
Assist:	Dc.B	'Assist',0
Sysop:	Dc.B	'Sysop',0

AskPass:	Dc.B	'Please enter the password: ',0

NoUser:	Dc.B	10,10,'I''m sorry, but there is no user of that name on this BBS.',10
	Dc.B	'Please try a different name, or check on a different BBS.',10,10
NoUserLen	Equ	*-NoUser

LookedText:	Dc.B	'Scanning for user:  '
LookedLen	Equ	*-LookedText
	Dc.B	'%s',10
Found:	Dc.B	"Is '%s' the person you were after? ",0

***************************************
UsrNmb:	Dc.B	10,10,'UserNumber:        %ld',10
	Dc.B	'UserName:          %s',10,10
	Dc.B	'Calling from:      %s',10,10
	Dc.B	'Password:          %s',10,10
	Dc.B	'Home Address:      %s',10
	Dc.B	'Age:               %s',10
	Dc.B	'Phone Number:      %s',10,10
	Dc.B	'Computer Type:     %s',10,10
	Dc.B	'Last Called:       %s',10
	Dc.B	'Access Level:      %s',10,10
	Dc.B	'Calls made:        %ld',10
	Dc.B	'Messages posted:   %ld',10
	Dc.B	'Total UpLoaded:    %ldk',10
	Dc.B	'Total DownLoaded:  %ldk',10
	Dc.B	'Total Time Online: %ld mins',10,0

***************************************
GetNameText:	Dc.B	'Name of user to look for?',0
OkGadgets:	Dc.B	'_Yes|_Abort|_No',0
InfoGadgets:	Dc.B	'_Continue',0
	Even

***************************************
MyPass:	Dc.B	$8F,$B6,$A5,$B2,$B2,$A9,$A4,$A5,$00
ReadBuffer:	Dc.B	0
Ret:	Dc.B	10
NotAvail:	Dc.B	'<not available>',0
Hash:	Dc.B	'#'
BackSpace:	Dc.B	8,' ',8
	Even

Levels:	Dc.L	Twit,Disgrace,Normal,Privil,Assist,Sysop

_FormatData:	Dc.L	0
Name:	Dc.L	0	;username
	Dc.L	UserData+60	;calling from
Password:	Dc.L	NotAvail	;users password
Address:	Dc.L	NotAvail	;home address
Age:	Dc.L	NotAvail	;age
Phone:	Dc.L	NotAvail	;phone number
Computer:	Dc.L	NotAvail	;computer type
Called:	Dc.L	0	;last Called
Lvl:	Dc.L	0	;access level
Calls:	Dc.L	0	;calls made
Msgs:	Dc.L	0	;messages posted
Up:	Dc.L	0	;uploaded
Down:	Dc.L	0	;downloaded
Time:	Dc.L	0	;time online
	Dc.L	0	

ScanData:	Dc.L	UserName
FoundData:	Dc.L	UserData,0

ConfigData:	Dc.L	_User,UserFile
	Dc.W	80
	Dc.L	_App,AppFile
	Dc.W	80
	Dc.L	_Comment,CommentFile
	Dc.W	80
	Dc.L	_Display,Display
	Dc.W	4
	Dc.L	_NoLog,NoLog
	Dc.W	4
	Dc.L	_HidePassword,HidePassword
	Dc.W	4
	Dc.L	_UserPassword,UserPassword
	Dc.W	4
	Dc.L	_Password,PasswordBuffer
	Dc.W	40
	Dc.L	0

GetNameTags:	Dc.L	RTGS_TextFmt,GetNameText
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTGS_Flags,GSREQF_CENTERTEXT
ScrTag1:	Dc.L	RT_ScreenToFront,TRUE
	Dc.L	TAG_DONE

GetPassTags:	Dc.L	RTGS_Invisible,TRUE
	Dc.L	RTGS_TextFmt,AskPass
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTGS_Flags,GSREQF_CENTERTEXT
ScrTag2:	Dc.L	RT_ScreenToFront,TRUE
	Dc.L	TAG_DONE

OkTags:	Dc.L	RTEZ_ReqTitle,ProgramName
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RT_Underscore,'_'
ScrTag3:	Dc.L	RT_ScreenToFront,TRUE
	Dc.L	TAG_DONE

InfoTags:	Dc.L	RT_Underscore,'_'
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTEZ_ReqTitle,ProgramName
ScrTag4:	Dc.L	RT_ScreenToFront,TRUE
	Dc.L	TAG_DONE

*****************************************************************************
	Section	DataSpace,BSS
_ArgSize:	Ds.L	1
_ArgStart:	Ds.L	1
_SaveStack:	Ds.L	1
_WBMessage:	Ds.L	1

DataStart:
_DOSBase:	Ds.L	1
_ECLIPSEBase:	Ds.L	1
_ReqToolsBase:	Ds.L	1

_InputHandle:	Ds.L	1
_OutputHandle:	Ds.L	1

_AppSize:	Ds.L	1
_AppStart:	Ds.L	1
_ConfigStart:	Ds.L	1
_ConfigSize:	Ds.L	1
_CommentStart:	Ds.L	1
_CommentSize:	Ds.L	1
_DataSpace:	Ds.L	1
_DataSpace2:	Ds.L	1

_UserBaseSize:	Ds.L	1
_UserHandle:	Ds.L	1

UserCounter:	Ds.L	1
UserAppData:	Ds.L	1

FilePos:	Ds.L	1
DateSpace:	Ds.L	3

PassReadBuffer:	Ds.B	10
_FormatSpace:	Ds.B	128
ScanSpace:	Ds.B	LookedLen
UserName:	Ds.B	40

Display:	Ds.B	4
NoLog:	Ds.B	4
HidePassword:	Ds.B	4
UserPassword:	Ds.B	4
PasswordBuffer:	Ds.B	40
UserFile:	Ds.B	80
AppFile:	Ds.B	80
CommentFile:	Ds.B	80

PassCount:	Ds.W	1
UseReqs:	Ds.B	1
ViewSpace:	Ds.B	7
UserData:	Ds.B	512
DataSize	Equ	*-DataStart
