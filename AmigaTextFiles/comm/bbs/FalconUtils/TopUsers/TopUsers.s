;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	TopUsers,Code
	Opt	C+
	Include	mysystem.gs
	Include	custom/macros.i
	Include	workbench/icon_lib.i
	Include	libraries/reqtools.i
	Include	libraries/reqtools_lib.i

TITLE	MACRO
	Dc.B	'Falcon CBCS Top Users V2.0b'
	ENDM
DATESTR	MACRO
	Dc.B	'3 November 92'
	ENDM
AUTHOR	MACRO
	Dc.B	'(c) 1992  David Dustin'
	ENDM

***************************************************************************
* Notes:
***************************************************************************
Start:
	Move.L	SP,_StackSave
	Move.L	D0,_ArgSize
	Move.L	A0,_ArgStart

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Beq	fromWorkbench

	Bsr	OpenDOS
	Bsr	GetConfigFile
	Bsr	GetCommandLine

**********************************
WBStart:
	Lea	REQTOOLSLibrary(PC),A1
	Moveq	#38,D0
	CALLEXEC	OpenLibrary
	Tst.L	D0
	Beq	ExitToDos
	Move.L	D0,_ReqToolsBase

ReStart:
	Cmp.B	#5,Option
	Beq.S	.DoAbout
	Tst.B	Option
	Bne.S	.TestFile

	Lea	Welcome(PC),A1
	Lea	WelcomeGads(PC),A2
	Lea	WelcomeTags(PC),A0
	Bsr	DoRequest

	Tst.L	D0
	Beq	ExitToDos
	Cmp.B	#5,D0
	Beq.S	.DoAbout
	Move.B	D0,Option
	Bra	.TestFile

**********************************
.DoAbout
	Lea	AboutText(PC),A1
	Lea	ContinueGad(PC),A2
	Lea	MyReqTags(PC),A0
	Bsr	DoRequest
	Move.B	#0,Option
	Bra.S	ReStart

**********************************
.TestFile
	Tst.L	FileName
	Bne	.NameOk

	Move.L	_ReqToolsBase(PC),A6
	Moveq	#RT_FILEREQ,D0
	Sub.L	A0,A0
	Jsr	_LVOrtAllocRequestA(A6)
	Tst.L	D0
	Beq	ExitToDos
	Move.L	D0,FileAlloc

	Move.L	FileAlloc(PC),A1
	Lea	FReqLoadName(PC),A2
	Lea	LoadTitle(PC),A3
	Lea	LoadFileTags(PC),A0
	Jsr	_LVOrtFileRequestA(A6)
	Tst.L	D0
	Beq	ExitToDos

	Move.L	FileAlloc(PC),A1
	Move.L	rtfi_Dir(A1),A0
	Lea	FileName,A1
	Moveq	#108,D0
	Bsr	CopyData
	Tst.B	FileName
	Beq.S	.NoRoot
	Cmp.B	#':',-1(A1)
	Beq.S	.NoRoot
	Move.B	#'/',(A1)+
.NoRoot
	Lea	FReqLoadName(PC),A0
	Moveq	#108,D0
	Bsr	CopyData
	Move.B	#0,(A1)

	Tst.L	FileAlloc
	Beq.S	.NameOk

	Move.L	FileAlloc(PC),A1
	Move.L	_ReqToolsBase(PC),A6
	Jsr	_LVOrtAllocRequestA(A6)
	Move.L	#0,FileAlloc

.NameOk
	Tst.L	MemStart
	Bne.S	.AlreadyLoaded

	Lea	FileName,A0
	Bsr	ReadFile
	Tst.L	D0
	Bne	FileError
.AlreadyLoaded

**********************************
	Cmp.B	#1,EntriesFlag
	Beq.S	.Ok

	Move.L	_ReqToolsBase(PC),A6
	Lea	NumberofUsers(PC),A1
	Lea	EntriesTitle(PC),A2
	Sub.L	A3,A3
	Lea	NumberTags(PC),A0
	Jsr	_LVOrtGetLongA(A6)
	Tst.L	D0
	Bne.S	.Ok
	Move.L	#10,NumberofUsers
.Ok

**********************************
	Cmp.B	#1,Output
	Beq	.ScreenOnly
	Cmp.B	#2,Output
	Beq	.DoSaveFile
	Cmp.B	#3,Output
	Beq	.Printer

	Lea	OutText(PC),A1
	Lea	OutGads(PC),A2
	Lea	MyReqTags(PC),A0
	Bsr	DoRequest

	Cmp.L	#1,D0
	Beq	.ScreenOnly
	Cmp.L	#3,D0
	Beq	.Printer
	Tst.L	D0
	Beq	Cancel1

**********************************
.DoSaveFile
	Cmp.B	#1,SaveFlag
	Beq	.NoSaveReq
.SaveLoop
	Tst.L	SaveFileAlloc
	Bne.S	.NoSavePath

	Move.L	_ReqToolsBase(PC),A6
	Moveq	#RT_FILEREQ,D0
	Sub.L	A0,A0
	Jsr	_LVOrtAllocRequestA(A6)
	Tst.L	D0
	Beq	Cancel2
	Move.L	D0,SaveFileAlloc

	Tst.L	SavePath
	Beq.S	.NoSavePath

	Move.L	D0,A1
	Lea	NewPathTags(PC),A0
	Move.L	_ReqToolsBase(PC),A6
	Jsr	_LVOrtChangeReqAttrA(A6)
.NoSavePath

	Move.L	_ReqToolsBase(PC),A6
	Move.L	SaveFileAlloc(PC),A1
	Lea	FReqSaveName(PC),A2
	Lea	SaveTitle(PC),A3
	Lea	SaveFileTags(PC),A0
	Jsr	_LVOrtFileRequestA(A6)
	Tst.L	D0
	Beq	.ScreenOnly

	Move.L	SaveFileAlloc(PC),A1
	Move.L	rtfi_Dir(A1),A0
	Lea	SaveFileName,A1
	Moveq	#108,D0
	Bsr	CopyData
	Tst.B	FileName
	Beq.S	.NoRoot2
	Cmp.B	#':',-1(A1)
	Beq.S	.NoRoot2
	Move.B	#'/',(A1)+
.NoRoot2
	Lea	FReqSaveName(PC),A0
	Moveq	#108,D0
	Bsr	CopyData
	Move.B	#0,(A1)
.NoSaveReq

	Move.L	#SaveFileName,D1
	MoveQ	#ACCESS_READ,D2
	CALLDOS	Lock
	Tst.L	D0
	Beq	.OverWrite
	Move.L	D0,D1
	CALLDOS	UnLock

	Cmp.B	#1,SaveOption
	Beq	.OverWrite
	Cmp.B	#2,SaveOption
	Beq.S	.Join

	Lea	ExistsText(PC),A1
	Lea	ExistsGads(PC),A2
	Lea	CancelTags(PC),A0
	Sub.L	A3,A3
	Lea	FormatStrings(PC),A4
	Move.L	_ReqToolsBase(PC),A6
	Jsr	_LVOrtEZRequestA(A6)

	Tst.L	D0
	Beq	Cancel1
	Cmp.L	#1,D0
	Beq.S	.OverWrite
	Cmp.L	#3,D0
	Beq	.SaveLoop

.Join
	Move.B	#2,PrintFlag
	Move.L	#SaveFileName,D1
	Move.L	#MODE_READWRITE,D2
	CALLDOS	Open
	Tst.L	D0
	Beq.S	.NoDataFile
	Move.L	D0,OtherOutputHandle

	Move.L	D0,D1
	Moveq	#0,D2
	Moveq	#OFFSET_END,D3
	CALLDOS	Seek
.NotAgain
	Bra.S	.OtherOutput

.OverWrite
	Move.B	#2,PrintFlag
	Move.L	#SaveFileName,D1
	Bsr	OpenFile
	Tst.L	D0
	Beq.S	.OtherOutput

.NoDataFile
	Move.B	#0,PrintFlag
	Lea	NoFileText(PC),A1
	Lea	ContinueGad(PC),A2
	Lea	ErrorReqTags(PC),A0
	Bsr	DoRequest
	Bra	.ScreenOnly

**********************************
.Printer
	Move.B	#1,PrintFlag
	Move.L	#PrinterName,D1
	Bsr	OpenFile
	Tst.L	D0
	Beq.S	.OtherOutput

.NoPrt
	Move.B	#0,PrintFlag
	Lea	NoPrtText(PC),A1
	Lea	ContinueGad(PC),A2
	Lea	ErrorReqTags(PC),A0
	Bsr	DoRequest
	Bra	.ScreenOnly

**********************************
.OtherOutput
	Cmp.B	#1,AgainFlag
	Beq.S	.Screen

**********************************
	Move.B	#1,AgainFlag
	Move.L	#DateSpace,D1
	CALLDOS	DateStamp

	Bsr	FindDate
	Bsr	FindTime
	Bsr	FindDay

**********************************
	Move.L	#Credits,D2
	Move.L	#CreditsLen,D3
	Move.L	OtherOutputHandle(PC),D1
	CALLDOS	Write
	Tst.L	D0
	Bne.S	.Screen
	Bsr	WriteError

**********************************
.ScreenOnly
	Move.B	#0,PrintFlag
.Screen
	Move.L	MemStart(PC),A0
	Move.L	A0,CurrentPos
	Add.L	MemSize(PC),A0
	Move.L	A0,MemEnd

**********************************
	Cmp.B	#1,HideFlag
	Beq.S	MainLoop

	Move.L	_ReqToolsBase(PC),A6
	Lea	HideBuffer,A1
	Moveq	#39,D0
	Lea	HideTitle(PC),A2
	Sub.L	A3,A3
	Lea	HideTags(PC),A0
	Jsr	_LVOrtGetStringA(A6)

	Tst.L	D0
	Bne.S	MainLoop
	Move.B	#0,HideBuffer

***********************************************************
MainLoop:
	Move.L	UserCounter(PC),D3

	Move.L	CurrentPos(PC),A0
	Tst.B	(A0)
	Beq	.NextUser

	Lea	HideBuffer,A1
	Move.L	CurrentPos(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Beq.S	.NextUser

**********************************
	Move.L	CurrentPos(PC),A0
.Calls
	Cmp.B	#1,Option
	Bne.S	.NotCalls
	Lea	120(A0),A0
	Bsr	Sort
	Bra	.NextUser

.NotCalls
	Cmp.B	#2,Option
	Bne.S	.NotMessages
	Lea	122(A0),A0
	Bsr	Sort
	Bra	.NextUser

.NotMessages
	Cmp.B	#3,Option
	Bne.S	.NotDownload
	Lea	150(A0),A0
	Bsr	Sort2
	Bra	.NextUser

.NotDownload
	Lea	158(A0),A0
	Bsr	Sort2

**********************************
.NextUser
	Addq.L	#1,UserCounter
	Move.L	CurrentPos(PC),A0
	Lea	512(A0),A0
	Move.L	A0,CurrentPos
	Move.L	MemEnd(PC),D0
	Move.L	CurrentPos(PC),D1
	Sub.L	D1,D0
	Tst.L	D0
	Bne	MainLoop

**********************************
	Lea	OptionText(PC),A1
	Moveq	#15,D0
	Cmp.B	#1,Option
	Bne.S	.Notcalls
	Lea	Callstxt,A0
	Bra	.DoCopy
.Notcalls
	Cmp.B	#2,Option
	Bne.S	.NotMess
	Lea	Messagetxt(PC),A0
	Bra.S	.DoCopy
.NotMess
	Cmp.B	#3,Option
	Bne.S	.NotDown
	Lea	Downloadtxt(PC),A0
	Bra.S	.DoCopy
.NotDown
	Lea	Uploadtxt(PC),A0
.DoCopy
	Bsr	CopyData

.NoChange

**********************************
	Move.L	NumberofUsers(PC),D7
	Subq	#1,D7
	Lea	DataSpace,A5
	Lea	DataText(PC),A2
.printloop
	Moveq	#0,D0
	Move.W	(A5),D0
	Mulu	#512,D0
	Move.L	MemStart(PC),A0
	Add.L	D0,A0
	Move.L	A2,A1
	Moveq	#36,D0
	Bsr	CopyData

.calls
	Cmp.B	#2,Option
	Bgt.S	.DoK
	Bsr	WriteNum
	Bra	.DoLoop

.DoK
	Bsr	WriteK

.DoLoop
	Lea	NextEntry(A2),A2
	DBra	D7,.printloop

**********************************
	Move.B	#0,(A2)
	Tst.B	PrintFlag
	Bne.S	.Output
		
	Lea	PrintSpace(PC),A1
	Lea	ContinueGad(PC),A2
	Lea	NormalReqTags(PC),A0
	Bsr	DoRequest
	Bra	Exit

**********************************
.Output
	Lea	PrintSpace(PC),A0
	Move.L	A0,D2
	Moveq	#0,D3
.findloop
	Cmp.B	#0,(A0)+
	Beq.S	.foundit
	Addq	#1,D3
	Bra.S	.findloop
.foundit
	Move.L	OtherOutputHandle(PC),D1
	CALLDOS	Write
	Tst.L	D0
	Bne.S	Exit
	Bsr	WriteError

***********************************************************
Exit:
	Tst.B	PrintFlag
	Beq.S	Cancel1

	Move.L	#ExtraRets,D2
	Moveq	#2,D3
	Move.L	OtherOutputHandle(PC),D1
	CALLDOS	Write
	Bsr	CloseFile

**********************************
Cancel1:
	Cmp.B	#1,AgainOff
	Beq.S	Cancel2
	Cmp.B	#2,AgainOff
	Beq.S	.Again

	Lea	AgainText(PC),A1
	Lea	AgainGads(PC),A2
	Lea	CancelTags(PC),A0
	Bsr	DoRequest

	Tst.L	D0
	Beq.S	Cancel2

**********************************
.Again
	Bsr	ClearForAgain
	Bra	ReStart

**********************************
Cancel2:
	Move.L	MemStart(PC),A1
	Move.L	MemSize(PC),D0
	CALLEXEC	FreeMem

**********************************
ExitToDos:
	Tst.L	SaveFileAlloc
	Beq.S	.NoFileAlloc

	Move.L	SaveFileAlloc(PC),A1
	Move.L	_ReqToolsBase(PC),A6
	Jsr	_LVOrtAllocRequestA(A6)

.NoFileAlloc
	Tst.L	WBMessage
	Beq.S	.NoWB

	Move.L	_IconBase(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoIcon
	CALLEXEC	CloseLibrary

.NoIcon
	CALLEXEC	Forbid
	Move.L	WBMessage(PC),A1
	CALLEXEC	ReplyMsg

**********************************
.NoWB
	Move.L	_ReqToolsBase(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoReq
	CALLEXEC	CloseLibrary

.NoReq
	Move.L	_DOSBase(PC),A1
	CALLEXEC	CloseLibrary
	Moveq	#0,D0
	Move.L	_StackSave(PC),SP
	Rts

***********************************************************
WriteError:
	Lea	WriteErrorText(PC),A1
	Lea	ContinueGad(PC),A2
	Lea	ErrorReqTags(PC),A0
	Bsr	DoRequest
	Bsr	CloseFile
	Move.B	#0,PrintFlag
	Rts

***********************************************************
WriteNum:
	Moveq	#0,D0
	Move.W	2(A5),D0
	Move.L	A2,A0
	Lea	UserNameLen(A0),A0
	Moveq	#4,D1
	Bsr	DoDec
	Addq	#4,A5
	Rts

***********************************************************
WriteK:
	Moveq	#0,D0
	Move.L	2(A5),D0
	Move.L	A2,A0
	Lea	UserNameLen(A0),A0
	Moveq	#7,D1
	Bsr	DoDec
	Move.B	#'K',(A1)+
	Addq	#6,A5
	Rts

***********************************************************
GetConfigFile:
	Lea	ConfigName(PC),A0
	Bsr	ReadFile
	Tst.L	D0
	Bne.S	.NoConfig

	Move.L	MemStart(PC),CfgData
	Bsr	FindConfigData
	Move.L	MemStart(PC),A1
	Move.L	MemSize(PC),D0
	CALLEXEC	FreeMem
	Move.L	#0,MemStart
	Move.L	#0,MemSize
	Move.L	#0,CfgData
.NoConfig
	Rts

***********************************************************
FindConfigData:
	Lea	Database(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoUserBase
	Bsr	KillEqu

	Lea	FileName,A1
	Moveq	#107,D0
	Bsr	CopyData
	Move.B	#0,(A1)
.NoUserBase

**********************************
	Lea	Entries(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoEntries
	Bsr	KillEqu

	Move.L	#10,NumberofUsers
	Moveq	#0,D0
	Move.B	(A0),D0
	Cmp.B	#'2',D0
	Blt.S	.NoEntries
	Cmp.B	#'9',D0
	Bgt.S	.NoEntries

	Sub.B	#'0',D0
	Move.L	D0,NumberofUsers
.NoEntries

**********************************
	Lea	EntriesRequest(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoEntriesReq
	Bsr	KillEqu

	Move.B	#0,EntriesFlag
	Move.L	A0,A1
	Lea	No(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoEntriesReq
	Move.B	#1,EntriesFlag
.NoEntriesReq

**********************************
	Lea	HideUser(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoHide
	Bsr	KillEqu

	Lea	HideBuffer,A1
	Moveq	#39,D0
	Bsr	CopyData
	Move.B	#0,(A1)
.NoHide

**********************************
	Lea	HideRequest(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoHideReq
	Bsr	KillEqu

	Move.B	#0,HideFlag
	Move.L	A0,A1
	Lea	No(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoHideReq
	Move.B	#1,HideFlag
.NoHideReq

**********************************
	Lea	SaveName(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoSave
	Bsr	KillEqu

	Move.L	A0,TempTool
	Lea	SaveFileName,A1
	Move.L	#215,D0
	Bsr	CopyData
	Move.B	#0,(A1)
	Bsr	CopySaveName

	Lea	SaveRequest(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoSave
	Bsr	KillEqu

	Move.B	#0,SaveFlag
	Move.L	A0,A1
	Lea	No(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoSave
	Move.B	#1,SaveFlag
.NoSave

**********************************
	Lea	Again(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoAgain
	Bsr	KillEqu

	Move.B	#0,AgainOff
	Move.L	A0,TempTool
	Move.L	A0,A1
	Lea	Yes(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoYes
	Move.B	#2,AgainOff
	Bra.S	.NoAgain
.NoYes
	Move.L	TempTool(PC),A1
	Lea	No(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoAgain
	Move.B	#1,AgainOff
.NoAgain

**********************************
	Lea	SaveOpts(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoOpts
	Bsr	KillEqu

	Move.B	#0,SaveOption
	Move.L	A0,TempTool
	Move.L	A0,A1
	Lea	OverWrite(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoOverWrite
	Move.B	#1,SaveOption
	Bra	.NoOpts
.NoOverWrite
	Move.L	TempTool(PC),A1
	Lea	Join(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoOpts
	Move.B	#2,SaveOption
.NoOpts

**********************************
	Lea	OutputType(PC),A0
	Move.L	CfgData(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne.S	.NoOutput
	Bsr	KillEqu

	Move.B	#0,Output
	Move.L	A0,TempTool
	Move.L	A0,A1
	Lea	ScreenText(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoScreen
	Move.B	#1,Output
	Bra	.NoOutput
.NoScreen
	Move.L	TempTool(PC),A1
	Lea	File(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoFile
	Move.B	#2,Output
	Bra.S	.NoOutput
.NoFile
	Move.L	TempTool(PC),A1
	Lea	Printer(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoOutput
	Move.B	#3,Output
.NoOutput

**********************************
	Rts

***********************************************************
KillEqu:
	Cmp.B	#'=',(A3)
	Bne.S	.NoKill
	Addq	#1,A3
.NoKill
	Move.L	A3,A0
	Rts

***********************************************************
fromWorkbench:
	Lea	pr_MsgPort(A4),A0
	CALLEXEC	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALLEXEC	GetMsg
	Move.L	D0,WBMessage

	Bsr	OpenDOS

	Lea	ICONLibrary(PC),A1
	Moveq	#0,D0
	CALLEXEC	OpenLibrary
	Move.L	D0,_IconBase
	Beq	ExitToDos

**********************************
	Move.L	WBMessage(PC),A0
	Move.L	$24(A0),A0
	Beq	ExitToDos

	Move.L	(A0),D1
	CALLDOS	CurrentDir

	Move.L	WBMessage(PC),A0
	Move.L	$24(A0),A0
	Move.L	4(A0),A0

	Move.L	_IconBase(PC),A6
	CALLICON	GetDiskObject
	Move.L	D0,DiskObject
	Beq	ExitToDos

	Move.L	D0,A1
	Move.L	$36(A1),A1
	Move.L	A1,ToolTypesPointer

	Bsr	GetConfigFile

**********************************
	Lea	Database(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoDataBase

	Move.L	D0,A0
	Lea	FileName,A1
	Moveq	#107,D0
	Bsr	CopyData
	Move.B	#0,(A1)
.NoDataBase

**********************************
	Lea	Entries(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoEntries

	Move.L	#10,NumberofUsers
	Move.L	D0,A0
	Moveq	#0,D0

	Move.B	(A0),D0
	Cmp.B	#'2',D0
	Blt.S	.NoEntries
	Cmp.B	#'9',D0
	Bgt.S	.NoEntries

	Sub.B	#'0',D0
	Move.L	D0,NumberofUsers
.NoEntries

**********************************
	Lea	EntriesRequest(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoEntriesReq

	Move.B	#0,EntriesFlag
	Move.L	D0,A0
	Lea	No(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoEntriesReq
	Move.B	#1,EntriesFlag
.NoEntriesReq

**********************************
	Lea	HideUser(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq	.NoHide

	Move.L	D0,A0
	Lea	HideBuffer,A1
	Moveq	#39,D0
	Bsr	CopyData
	Move.B	#0,(A1)
.NoHide

**********************************
	Lea	HideRequest(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoHideReq

	Move.B	#0,HideFlag
	Move.L	D0,A0
	Lea	No(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoHideReq
	Move.B	#1,HideFlag
.NoHideReq

**********************************
	Lea	SaveName(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq	.NoSave
	Move.L	D0,TempTool

	Move.L	D0,A0
	Lea	SaveFileName,A1
	Move.L	#215,D0
	Bsr	CopyData
	Move.B	#0,(A1)
	Bsr	CopySaveName

**********************************
	Lea	SaveRequest(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoSave

	Move.B	#0,SaveFlag
	Move.L	D0,A0
	Lea	No(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoSave
	Move.B	#1,SaveFlag
.NoSave

**********************************
	Lea	Again(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoAgain

	Move.B	#0,AgainOff
	Move.L	D0,TempTool
	Move.L	D0,A0
	Lea	Yes(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoYes
	Move.B	#2,AgainOff
	Bra.S	.NoAgain
.NoYes
	Move.L	TempTool(PC),A0
	Lea	No(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoAgain
	Move.B	#1,AgainOff
.NoAgain

**********************************
	Lea	SaveOpts(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoOpts
	Move.L	D0,TempTool
	
	Move.B	#0,SaveOption
	Move.L	D0,A0
	Lea	OverWrite(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoOverWrite
	Move.B	#1,SaveOption
	Bra	.NoOpts
.NoOverWrite
	Move.L	TempTool(PC),A0
	Lea	Join(PC),A1
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoOpts
	Move.B	#2,SaveOption
.NoOpts

**********************************
	Lea	OutputType(PC),A1
	Move.L	ToolTypesPointer(PC),A0
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoOutput
	Move.L	D0,TempTool

	Move.B	#0,Output
	Move.L	D0,A1
	Lea	ScreenText(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoScreen
	Move.B	#1,Output
	Bra	.NoOutput
.NoScreen
	Move.L	TempTool(PC),A1
	Lea	File(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoFile
	Move.B	#2,Output
	Bra.S	.NoOutput
.NoFile
	Move.L	TempTool(PC),A1
	Lea	Printer(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoOutput
	Move.B	#3,Output
.NoOutput

**********************************
	Bra	WBStart

***********************************************************
CopySaveName:
	Move.L	TempTool(PC),A0
.findend
	Cmp.B	#0,(A0)
	Beq.S	.Found
	Cmp.B	#10,(A0)
	Beq.S	.Found
	Addq	#1,A0
	Bra.S	.findend

**********************************
.Found
	Move.L	A0,A1	;end pointer
.findname
	Cmp.B	#'/',(A0)
	Beq.S	.Found2
	Cmp.B	#':',(A0)
	Beq.S	.Found2
	Subq	#1,A0
	Bra.S	.findname

**********************************
.Found2
	Move.L	A0,A2	;name pointer
	Addq	#1,A0	;start of name
	Sub.L	A2,A1
	Move.L	A1,D0
	Lea	FReqSaveName,A1
	Bsr	CopyData
	Move.B	#0,(A1)

**********************************
	Move.L	TempTool(PC),A0
	Sub.L	A0,A2
	Move.L	A2,D0
	Lea	SavePath,A1
	Bsr	CopyData
	Move.B	#0,(A1)
	Rts

***********************************************************
FindData:
	Moveq	#0,D0
	Move.L	D0,D1
	Move.B	(A0),D1
	Bsr	ChangeCase
	Move.B	D1,D0
	Move.B	(A1)+,D1
	Cmp.B	#0,D1
	Beq.S	.Exit
	Bsr	ChangeCase
	Cmp.B	D0,D1
	Bne.S	FindData

**********************************
	Move.L	A0,A2
	Move.L	A1,A3
	Addq	#1,A2
.findloop
	Move.B	(A2)+,D1
	Bsr	ChangeCase
	Cmp.B	#0,D1
	Beq.S	.Found
	Move.B	D1,D0
	Move.B	(A3)+,D1
	Bsr	ChangeCase
	Cmp.B	D0,D1
	Bne.S	FindData
	Bra.S	.findloop

**********************************
.Exit
	Moveq	#-1,D0
	Rts
.Found
	Moveq	#0,D0
	Rts

***********************************************************
FindData2:
	Moveq	#0,D0
	Move.L	D0,D1
	Move.B	(A0)+,D1
	Cmp.B	#0,D1
	Beq.S	.Found
	Bsr	ChangeCase
	Move.B	D1,D0
	Move.B	(A1)+,D1
	Cmp.B	#0,D1
	Beq.S	.Exit
	Cmp.B	#10,D1
	Beq.S	.Exit
	Bsr	ChangeCase
	Cmp.B	D0,D1
	Beq.S	FindData2

**********************************
.Exit
	Moveq	#-1,D0
	Rts
.Found
	Move.L	A1,A3
	Moveq	#0,D0
	Rts
		
***********************************************************
CopyData:
	Cmp.B	#0,(A0)
	Beq.S	.stop
	Cmp.B	#10,(A0)
	Beq.S	.stop
	Move.B	(A0)+,(A1)+
	DBra	D0,CopyData
.stop
	Rts

***********************************************************
OpenDOS:
	Lea	DOSLibrary(PC),A1
	Moveq	#0,D0
	CALLEXEC	OpenLibrary
	Move.L	D0,_DOSBase
	Rts

***********************************************************
OpenFile:
	Move.L	#MODE_NEWFILE,D2
	CALLDOS	Open
	Move.L	D0,OtherOutputHandle
	Beq.S	Error
	Moveq	#0,D0
	Rts

**********************************
Error:
	Moveq	#-1,D0
	Rts

***********************************************************
CloseFile:
	Move.L	OtherOutputHandle(PC),D1
	CALLDOS	Close
	Rts

***********************************************************
GetCommandLine:
	Addq.L 	#2,_ArgSize
	Move.L	_ArgStart(PC),A0
	Move.L	_ArgSize(PC),D0

**********************************
	Cmp.B	#1,D0
	Beq	.NoArgs
	Cmp.B	#0,D0
	Beq	.NoArgs
	Cmp.B	#'?',(A0)
	Bne.S	.ArgOk
	Move.B	#5,Option
	Bra	.NoArgs

**********************************
.ArgOk
	Move.L	_ArgSize(PC),D0
	Moveq	#0,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Beq	.NoArgs
	Move.L	D0,ArgSpaceStart
	
**********************************
	Move.L	_ArgStart(PC),A0
	Move.L	ArgSpaceStart(PC),A1
	Move.L	_ArgSize(PC),D0
	Moveq	#0,D1
.CopyLoop
	Move.B	(A0)+,D1
	Cmp.B	#0,D1
	Beq.S	.CopyDone
	Cmp.B	#10,D1
	Beq.S	.CopyDone
	Cmp.B	#' ',D1
	Beq	.ASpace
	Cmp.B	#'"',D1
	Bne.S	.NotQuote
	Not.B	Quote
	Bra	.NoCopy
.NotQuote
	Move.B	D1,(A1)+
.NoCopy
	DBra	D0,.CopyLoop

**********************************
.CopyDone
	Move.L	ArgSpaceStart(PC),CfgData
	Bsr	FindConfigData
		
**********************************
	Lea	Select(PC),A0
	Move.L	ArgSpaceStart(PC),A1
	Bsr	FindData
	Tst.L	D0
	Bne	.NoSelect
	Bsr	KillEqu

	Move.B	#1,AgainOff
	Move.B	#0,Option

	Move.L	A0,TempTool
	Move.L	A0,A1
	Lea	CalOpt(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoCalls
	Move.B	#1,Option
	Bra	.NoSelect
.NoCalls
	Move.L	TempTool(PC),A1
	Lea	MsgOpt(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoMsg
	Move.B	#2,Option
	Bra	.NoSelect
.NoMsg
	Move.L	TempTool(PC),A1
	Lea	DldOpt(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoDld
	Move.B	#3,Option
	Bra	.NoSelect
.NoDld
	Move.L	TempTool(PC),A1
	Lea	UldOpt(PC),A0
	Bsr	FindData2
	Tst.L	D0
	Bne.S	.NoSelect
	Move.B	#4,Option
.NoSelect

**********************************
	Move.L	ArgSpaceStart(PC),A1
	Move.L	_ArgSize(PC),D0
	CALLEXEC	FreeMem
	Move.L	#0,CfgData

.NoArgs
	Rts

**********************************
.ASpace
	Cmp.B	#0,Quote
	Bne.S	.CopyOk
	Moveq	#10,D1
.CopyOk	
	Move.B	D1,(A1)+
	DBra	D0,.CopyLoop
	Bra	.CopyDone

***********************************************************
ReadFile:
	Move.L	#0,MemSize
	Move.L	#0,MemStart

	Moveq	#1,D4
	Lea	FileNamePtr(PC),A1
	Move.L	A0,(A1)

	Move.L	FileNamePtr(PC),D1
	Moveq	#-2,D2
	CALLDOS	Lock
	Lea	WorkFileLock(PC),A0
	Move.L	D0,(A0)
	Beq	.Error

	Move.L	WorkFileLock(PC),D1
	Lea	FileInfo,A0
	Move.L	A0,D2
	CALLDOS	Examine

	Move.L	FileInfo+124,D0
	Lea	MemSize(PC),A0
	Move.L	D0,(A0)

	Move.L	WorkFileLock(PC),D1
	CALLDOS	UnLock

	Move.L	FileNamePtr(PC),D1
	Move.L	#MODE_OLDFILE,D2
	CALLDOS	Open
	Lea	LoadHandle(PC),A0
	Move.L	D0,(A0)
	Beq.S	.Error

	Moveq	#2,D4
	Move.L	MemSize(PC),D0
	Moveq	#0,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Beq.S	.AllocError
	Lea	MemStart(PC),A0
	Move.L	D0,(A0)

	Moveq	#3,D4
	Move.L	LoadHandle(PC),D1
	Move.L	MemStart(PC),D2
	Move.L	FileInfo+124,D3
	CALLDOS	Read
	Tst.L	D0
	Beq.S	.LoadError

	Move.L	FileInfo+124,D1
	Cmp.L	D0,D1
	Bne.S	.LoadError

	Move.L	LoadHandle(PC),D1
	CALLDOS	Close
	Moveq	#0,D4

**********************************
.Error
	Move.L	D4,D0
	Rts

**********************************
.LoadError
	Move.L	MemStart(PC),A1
	Move.L	MemSize(PC),D0
	CALLEXEC	FreeMem

.AllocError
	Move.L	LoadHandle(PC),D1
	CALLDOS	Close

	Move.L	D4,D0
	Rts

***********************************************************
FileError:
	Cmp.B	#3,D0
	Bne.S	.NotRead
	Lea	ReadError(PC),A1
	Bra.S	.TypeError

.NotRead
	Cmp.B	#2,D0
	Bne.S	.NotNoMem
	Lea	MemError(PC),A1
	Bra.S	.TypeError

.NotNoMem
	Lea	LoadError(PC),A1

.TypeError
	Lea	ContinueGad(PC),A2
	Lea	ErrorReqTags,A0
	Bsr.S	DoRequest
	Bra	ExitToDos

***********************************************************
DoRequest:
	Move.L	_ReqToolsBase(PC),A6
	Move.L	#0,A3
	Move.L	#0,A4
	Jsr	_LVOrtEZRequestA(A6)
	Rts

***********************************************************
DoDec:
	Move.L	A0,A1
	Move.L	D1,D5

	Lea	DecTable(PC),A0

	Move.L	D0,D4
	Moveq	#0,D3
	Moveq	#'0',D0

.loop
	Move.L	(A0)+,D1
	Beq.S	.exit
	Moveq	#'/',D2

.loop2
	Addq.L	#1,D2
	Sub.L	D1,D4
	Bcc.S	.loop2
	Add.L	D1,D4
	Cmp.L	D0,D2
	Beq.S	.loop
	Moveq	#0,D0
	Move.B	D2,(A1)+
	Addq	#1,D3
	Bra.S	.loop

.exit
	Moveq	#'0',D0
	Add.B	D0,D4
	Move.B	D4,(A1)+
	Rts

***********************************************************
Sort:
	Moveq	#0,D1
	Move.W	(A0),D1

	Lea	DataSpace,A0
	Moveq	#9,D0
.findloop
	Moveq	#0,D2
	Addq	#2,A0
	Move.W	(A0)+,D2
	Cmp.W	D2,D1
	Bge.S	.found
	DBra	D0,.findloop
	Rts

**********************************
.found
	Lea	DataSpaceEnd,A0
	Cmp.B	#0,D0
	Beq.S	.nocopy
	subq	#1,D0
.foundloop
	Move.L	-8(A0),-4(A0)
	Subq	#4,A0
	Dbra	D0,.foundloop
.nocopy
	Move.W	D3,-4(A0)
	Move.W	D1,-2(A0)
	Rts

***********************************************************
Sort2:
	Moveq	#0,D1
	Move.L	(A0),D1

	Lea	DataSpace,A0
	Moveq	#9,D0
.findloop
	Moveq	#0,D2
	Addq	#2,A0
	Move.L	(A0)+,D2
	Cmp.L	D2,D1
	Bge.S	.found
	DBra	D0,.findloop
	Rts

**********************************
.found
	Lea	DataSpaceEnd2,A0
	Cmp.B	#0,D0
	Beq.S	.nocopy
	subq	#1,D0
.foundloop
	Move.L	-12(A0),-6(A0)
	Move.W	-8(A0),-2(A0)
	Subq	#6,A0
	Dbra	D0,.foundloop
.nocopy
	Move.W	D3,-6(A0)
	Move.L	D1,-4(A0)
	Rts

***********************************************************
ClearForAgain:
	Move.B	#0,Option

**********************************
	Moveq	#0,D1
	Lea	FileInfo,A0
	Move.L	#BssDataSize-1,D0
	Bsr	.Clear

	Lea	CurrentPos(PC),A0
	Moveq	#MiscDataLen,D0
	Bsr	.Clear

**********************************
	Moveq	#' ',D1
	Lea	DataText(PC),A2
	Moveq	#9,D2
.Semiloop
	Move.L	A2,A0
	Move.L	#NextEntry-2,D0
	Bsr	.Clear
	Add.L	#NextEntry,A2
	DBra	D2,.Semiloop
	Rts

**********************************
.Clear
	Move.B	D1,(A0)+
	DBra	D0,.Clear
	Rts

***********************************************************
ChangeCase:
	Cmp.B	#97,D1
	Blt.S	.Upper
	Sub.w	#32,D1
.Upper
	Rts

***********************************************************
FindDay:
	Move.L	DateSpace(PC),D0
	Divu	#7,D0
	Clr.W	D0
	Swap	D0
	
	Lea	DayNames(PC),A1
	Mulu	#3,D0
	Add.L	D0,A1
	Lea	Daytext(PC),A0
	Move.B	(A1)+,(A0)
	Move.B	(A1)+,1(A0)
	Move.B	(A1)+,2(A0)
	Rts

***********************************************************
FindDate:
	Lea	Datetext(PC),A0
;find the year
	Move.L	DateSpace(PC),D0
	Addq	#1,D0	;start from 1 not 0
	Moveq	#2,D1	;leap counter
	Moveq	#78,D2	;start year
.Loop
	Move.l	#365,D3	;days in a year
	And.B	#3,D1	;test for leap
	Bne.S	.NoLeap
	Addq	#1,D3	;extra day for leap year
.NoLeap
	Cmp.L	D3,D0	;have we run out of days
	Ble.S	.YearFound
	Sub.L	D3,D0	;kill a year
	Addq	#1,D2	;increase year counter
	Addq	#1,D1	;boost leap counter
	Bra.S	.Loop
.YearFound
	Cmp.W	#100,D2	;have we reached year 2000+
	Blt.S	.NoNewCent
	Sub.W	#100,D2	;drop it back to 00
	Move.B	#'2',7(A0)
	Move.B	#'0',8(A0)
.NoNewCent
	Divu	#10,D2
	Add.B	#'0',D2
	Move.B	D2,9(A0)
	Swap	D2
	Add.B	#'0',D2
	Move.B	D2,10(A0)

**********************************
;find the month
	Lea	NormalMonths(PC),A1
	And.W	#3,D1	;is it a leap year
	Bne.S	.NoLeap2
	Move.W	#29,2(A1)	;add a day to feb
.NoLeap2
	Moveq	#0,D1	;clear month counter
.Loop2
	Cmp.W	(A1),D0	;have we enough days left
	Ble.S	.MonthFound
	Sub.W	(A1)+,D0	;subtract number of days
	Addq	#1,D1	;increase month counter
	Bra.S	.Loop2
.MonthFound
	Lea	MonthNames(PC),A1
	Mulu	#3,D1
	Add.L	D1,A1
	Move.B	(A1)+,3(A0)
	Move.B	(A1)+,4(A0)
	Move.B	(A1)+,5(A0)

**********************************
;find the day
	Divu	#10,D0
	Add.B	#'0',D0
	Cmp.B	#'0',D0
	Bne.S	.Not0
	Move.B	#' ',D0
.Not0
	Move.B	D0,(A0)
	Swap	D0
	Add.B	#'0',D0
	Move.B	D0,1(A0)
	Rts

***********************************************************
FindTime:
	Lea	Timetext(PC),A1
	Move.L	DateSpace+4(PC),D0

	Divu.W	#60,D0
	Swap	D0
	Clr.W	D0
	Swap	D0

	Move.L	A1,A0
	Bsr.S	.calcnumber

	Move.L	DateSpace+4(PC),D0
	Divu.W	#60,D0
	Clr.W	D0
	Swap	D0
	
	Lea	3(A1),A0
.calcnumber
	Divu	#10,D0
	Add.B	#'0',D0
	Move.B	D0,(A0)
	Swap	D0
	Add.B	#'0',D0
	Move.B	D0,1(A0)
	Rts

***************************************************************************
* Data and Variables ******************************************************
***************************************************************************
VerString:
	Dc.B	0,'$VER: '
	TITLE
	Dc.B	'  ('
	DATESTR
	Dc.B	')',0

***********************************************************
MyReqTitle:
	TITLE
	Dc.B	0
ErrorTitle:
	TITLE
	Dc.B	' Error',0
LoadTitle:
	Dc.B	'Load User.BBS database',0
SaveTitle:
	Dc.B	'Save information as',0
HideTitle:
	Dc.B	'Hide a user?',0
EntriesTitle:
	Dc.B	'Number of Entries?',0

***********************************************************
Credits:
	TITLE
	Dc.B	10
	AUTHOR
	Dc.B	10,10
	Dc.B	'Current to:   '
Timetext:	Dc.B	'hh:mm  '
Daytext:	Dc.B	'day, '
Datetext:	Dc.B	'dd mmm 19yy',10
CreditsLen	Equ	*-Credits

***********************************************************
Welcome:
	Dc.B	'Please select the option that you would like',10,10
	Dc.B	'Top Callers',10
	Dc.B	'Top Message posters',10
	Dc.B	'Top Downloaders',10
	Dc.B	'Top Uploaders',0
WelcomeGads:
	Dc.B	'_Callers|_Messages|_DownLoaded|_UpLoaded|_About|_Quit',0

***********************************************************
AboutText:
	TITLE
	Dc.B	'  ('
	DATESTR
	Dc.B	')',10,10
	Dc.B	'This program is classified as ShareWare',10
	Dc.B	'which means that it may be freely',10
	Dc.B	'distributed, but may NOT be altered or',10
	Dc.B	'in anyway backward engineered.',10,10
	Dc.B	'Suggested ShareWare fee could be money,',10
	Dc.B	'beer, any software package etc...',10,10,10
	Dc.B	'Contact me at:',10,10
	Dc.B	'David Dustin',10
	Dc.B	'30 Richmond Ave',10
	Dc.B	'Palmerston North',10
	Dc.B	'NEW ZEALAND',10,10
	AUTHOR
	Dc.B	0
ContinueGad:
	Dc.B	'_Continue',0
	
***********************************************************
OutText:
	Dc.B	'Where would you like the output to go?',0
OutGads:
	Dc.B	'_Screen|_File|_Printer|_Cancel',0

***********************************************************
AgainText:
	Dc.B	'Do you want to get another top 10?',0
AgainGads:
	Dc.B	'_Ok|_Nope',0

***********************************************************
ExistsText:
	Dc.B	'The file ''%s'' already exists',10
	Dc.B	'Do you want to?',0
ExistsGads:
	Dc.B	'_OverWrite|_Join|Choose _Again|_Cancel',0

***********************************************************
HideText:
	Dc.B	'If you don''t want a certain name to appear',10
	Dc.B	'in the Top Users list (the SysOp for example)',10
	Dc.b	'then please enter the name that you want hidden',10
	Dc.B	'here, or cancel to include everyone',0

***********************************************************
NumberText:
	Dc.B	'Please enter the number of',10
	Dc.B	'entries that you want to',10
	Dc.B	'appear in the list',0
	
***********************************************************
LoadError:
	Dc.B	'There was a problem finding the User.BBS file.',10
	Dc.B	'Please check what you entered.',0

**********************************
MemError:
	Dc.B	'There is not enough memory to load the User.BBS file.',10
	Dc.B	'Free up some memory and try again.',0

**********************************
ReadError:
	Dc.B	'There was a problem loading the User.BBS file.',10
	Dc.B	'Check to see if it is still intact.',0

**********************************
NoPrtText:
	Dc.B	'There was a problem opening your printer device.',10
	Dc.B	'Check your DEVS: dir and try again.',0

**********************************
NoFileText:
	Dc.B	'There was a problem opening the new data file.',10
	Dc.B	'Please check and try again.',0

**********************************
WriteErrorText:
	Dc.B	'There was a problem trying to save the data out',10
	Dc.B	'to the file/printer.  Please check and try again',0
	Even

***********************************************************
AgainFlag:
	Dc.B	0
AgainOff:
	Dc.B	0
EntriesFlag:
	Dc.B	0
HideFlag:
	Dc.B	0
Option:
	Dc.B	0
Output:
	Dc.B	0
PrintFlag:
	Dc.B	0
SaveFlag:
	Dc.B	0
SaveOption:
	Dc.B	0
Quote:
	Dc.B	0

**********************************
DOSLibrary:
	DOSNAME
ICONLibrary:
	ICONNAME
REQTOOLSLibrary:
	REQTOOLSNAME

**********************************
ConfigName:
	Dc.B	'S:TopUsers.cfg',0
PrinterName:
	Dc.B	'PRT:',0

**********************************
Select:
	Dc.B	'SELECT',0
Database:
	Dc.B	'USERBASE',0
OutputType:
	Dc.B	'OUTPUT',0
Entries:
	Dc.B	'ENTRIES',0
EntriesRequest:
	Dc.B	'ENTRIESREQUEST',0
SaveName:
	Dc.B	'SAVENAME',0
SaveRequest:
	Dc.B	'SAVEREQUEST',0
SaveOpts:
	Dc.B	'SAVEOPTS',0
HideUser:
	Dc.B	'HIDEUSER',0
HideRequest:
	Dc.B	'HIDEREQUEST',0
Again:
	Dc.B	'AGAIN',0

**********************************
CalOpt:
	Dc.B	'CALLS',0
MsgOpt:
	Dc.B	'MESSAGES',0
DldOpt:
	Dc.B	'DOWNLOADED',0
UldOpt:
	Dc.B	'UPLOADED',0
ScreenText:
	Dc.B	'SCREEN',0
File:
	Dc.B	'FILE',0
Printer:
	Dc.B	'PRINTER',0
OverWrite:
	Dc.B	'OVERWRITE',0
Join:
	Dc.B	'JOIN',0
No:
	Dc.B	'NO',0
Yes:
	Dc.B	'YES',0

**********************************
Callstxt:
	Dc.B	'Calls made ',0
Messagetxt:
	Dc.B	'Messages   ',0
Downloadtxt:
	Dc.B	'Downloaded ',0
Uploadtxt:
	Dc.B	'Uploaded   ',0
MonthNames:
	Dc.B	'JanFebMarAprMayJunJulAugSepOctNovDec'
DayNames:
	Dc.B	'SunMonTueWedThuFriSat'
	Even

***********************************************************
PrintSpace:
	Dc.B	10,'Users Name                          '
OptionText:	Dc.B	'Calls made      ',10
	Dc.B	'-------------------------------------------------',10
DataText:
	Dc.B	'                                      '
UserNameLen	Equ	*-DataText
	Dc.B	'          ',10
NextEntry	Equ	*-DataText
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
	Dc.B	'                                      '
	Dc.B	'          ',10
PrintSpaceLen	Equ	*-PrintSpace
	Dc.B	0
ExtraRets:
	Dc.B	10,10
	Even

***********************************************************
NormalMonths:
	Dc.W	31,28,31,30,31,30,31,31,30,31,30,31

***********************************************************
CurrentPos:
	Dc.L	0
DateSpace:
	Ds.L	3
FileNamePtr:
	Dc.L	0
LoadHandle:
	Dc.L	0
OtherOutputHandle:
	Dc.L	0
UserCounter:
	Dc.L	0
WorkFileLock:
	Dc.L	0
MiscDataLen	Equ	*-CurrentPos

**********************************
FormatStrings:
	Dc.L	SaveFileName
	Dc.L	0

**********************************
_ArgStart:
	Dc.L	0
_ArgSize:
	Dc.L	0
_DOSBase:
	Dc.L	0
_IconBase:
	Dc.L	0
_ReqToolsBase:
	Dc.L	0
_StackSave:
	Dc.L	0
ArgSpaceStart:
	Dc.L	0
CfgData:
	Dc.L	0
DiskObject:
	Dc.L	0
FileAlloc:
	Dc.L	0
MemEnd:
	Dc.L	0
MemSize:
	Dc.L	0
MemStart:
	Dc.L	0
NumberofUsers:
	Dc.L	10
SaveFileAlloc:
	Dc.L	0
TempTool:
	Dc.L	0
ToolTypesPointer:
	Dc.L	0
WBMessage:
	Dc.L	0

**********************************
DecTable:
	Dc.L	$3B9ACA00,$05F5E100,$00989680,$000F4240,$000186A0
	Dc.L	$00002710,$000003E8,$00000064,$0000000A,$00000000

***********************************************************
FReqLoadName:
	Dc.B	'User.BBS'
	Ds.B	110-(*-FReqLoadName)
FReqSaveName:
	Ds.B	110

***********************************************************
CancelTags:
	Dc.L	RTEZ_ReqTitle,MyReqTitle
	Dc.L	RTEZ_DefaultResponse,0
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTEZ_Flags,EZREQF_CENTERTEXT+EZREQF_LAMIGAQUAL
	Dc.L	RT_Underscore,'_'
	Dc.L	TAG_DONE
ErrorReqTags:
	Dc.L	RTEZ_ReqTitle,ErrorTitle
	Dc.L	RTEZ_Flags,EZREQF_CENTERTEXT+EZREQF_LAMIGAQUAL
	Dc.L	RT_Underscore,'_'
	Dc.L	TAG_DONE
HideTags:
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTGS_TextFmt,HideText
	Dc.L	RTGS_Flags,GSREQF_CENTERTEXT
	Dc.L	TAG_DONE
LoadFileTags:
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTFI_Flags,FREQF_NOBUFFER
	Dc.L	TAG_DONE
MyReqTags:
	Dc.L	RTEZ_ReqTitle,MyReqTitle
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTEZ_Flags,EZREQF_CENTERTEXT+EZREQF_LAMIGAQUAL
	Dc.L	RT_Underscore,'_'
	Dc.L	TAG_DONE
NewPathTags:
	Dc.L	RTFI_Dir,SavePath
	Dc.L	TAG_DONE
NormalReqTags:
	Dc.L	RTEZ_ReqTitle,MyReqTitle
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTEZ_Flags,EZREQF_LAMIGAQUAL
	Dc.L	RT_Underscore,'_'
	Dc.L	TAG_DONE
NumberTags:
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTGL_TextFmt,NumberText
	Dc.L	RTGL_Min,2
	Dc.L	RTGL_Max,10
	Dc.L	RTGL_Flags,GLREQF_CENTERTEXT
	Dc.L	TAG_DONE
SaveFileTags:
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTFI_Flags,FREQF_NOBUFFER+FREQF_SAVE
	Dc.L	TAG_DONE
WelcomeTags:
	Dc.L	RTEZ_ReqTitle,MyReqTitle
	Dc.L	RTEZ_DefaultResponse,5
	Dc.L	RT_ReqPos,REQPOS_CENTERSCR
	Dc.L	RTEZ_Flags,EZREQF_CENTERTEXT+EZREQF_LAMIGAQUAL
	Dc.L	RT_Underscore,'_'
	Dc.L	TAG_DONE

***************************************************************************
***************************************************************************
	Section	TopUsersSpace,BSS
FileInfo:
	Ds.B	260
SavePath:
	Ds.B	110
DataSpace:
	Ds.L	10
DataSpaceEnd:
	Ds.L	5
DataSpaceEnd2:
BssDataSize	Equ	*-FileInfo

**********************************
HideBuffer:
	Ds.B	42
FileName:
	Ds.B	218
SaveFileName:
	Ds.B	218
