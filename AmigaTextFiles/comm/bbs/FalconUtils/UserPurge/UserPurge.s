;Released as public domain 28-Jan-1996 by the author Dave Dustin

	Section	UserPurge,Code
	Opt	C+
	Include	mysystem.gs
	Include	eclipse.i
	Include	workbench/icon_lib.i

	Output	UserPurge

VERSION	MACRO
	Dc.B	'Falcon CBCS UserPurge V1.15'
	ENDM
DATE	MACRO
	Dc.B	'6.12.92'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

***************************************************************************
* Notes:
*	1.01 - Added LogTag Comment
*	1.02 - Changed LogTag Comment
*	1.03 - Optimized
*	1.04 - Changed Logtag comment to include number
*	       of users removed.
*	1.05 - Added info text (command line usage)
*	     - Check for purge to 0 days
*	1.10 - Added full workbench support (DAYS, USERBASE)
*	1.11 - Fixed Removed text bug.  Wasn't taking into account
*	       users that were deleted
*	     - Removed info text
*	1.12 - Added Purge log
*	     - Added exlude from purge
*	1.13 - Added Black-list file
*	     - Fixed bug if there was no Exclude file
*	1.14 - Removes AppFile entry when user is purged
*	     - Fixed bug with the purge-log, if the user had been
*	       deleted using Falcon it would produce an wrong entry
*	     - Now reports the number of users killed in PurgeLog
*	1.15 - Changed log file output slightly
*
***************************************************************************
Start:	Move.L	D0,_ArgSize
	Move.L	A0,_ArgStart
	Move.L	SP,_SaveStack

	Lea	DataSpace(PC),A0
	Moveq	#DataSpaceLen,D0
	Moveq	#0,D1
.loop	Move.B	D1,(A0)+
	DBra	D0,.loop

	Lea	DataSpace2,A0
	Move.L	#DataSpaceLen2,D0
.loop2	Move.B	D1,(A0)+
	DBra	D0,.loop2

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Beq	fromWorkbench

	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Move.L	D0,_DOSBase
	Beq	NoDos
	
	Lea	ECLIPSELibrary(PC),A1
	Moveq	#EclipseVer,D0
	CALL	OpenLibrary
	Move.L	D0,_ECLIPSEBase
	Beq	ExitToDos

	Move.L	_ArgStart(PC),A0
	Move.L	_ArgSize(PC),D0

	Cmp.B	#1,D0
	Beq.S	.NoNewDays
	Cmp.B	#0,D0
	Beq.S	.NoNewDays
	Cmp.B	#'?',(A0)
	Beq	.NoNewDays

	CALLECLIPSE	ConvertToDecimal
	Tst.L	D0
	Beq	.NoNewDays
	Move.L	D0,DaysOld

.NoNewDays
WBRestart:	Tst.L	FileName
	Bne.S	.WBFile

	Lea	UserBaseName(PC),A0
	Bsr	GetFile
	Tst.L	D0
	Beq.S	.Loaded

	Lea	UserBaseKey(PC),A0
	Lea	FileName,A1
	CALLECLIPSE	GetConfigEntry
	Tst.L	D0
	Bne	Exit

	Move.L	#FileName,D1
	CALLDOS	FilePart
	Move.L	D0,A1
	Lea	UserBaseName(PC),A0
	Moveq	#8,D0
	CALLECLIPSE	CopyData
	
	Bra.S	.OtherName

.WBFile	Lea	FileName,A0
	Bsr	GetFile
	Tst.L	D0
	Bne	Exit

.OtherName	Move.L	#FileName,NameToUse

.Loaded	Move.L	NameToUse(PC),A0
	Bsr	MakeBackup
	Tst.L	D0
	Beq	Error

	Move.L	#CurrentStamp,D1
	CALL	DateStamp

	Move.L	NameToUse(PC),D1
	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Tst.L	D0
	Beq	Error
	Move.L	D0,UserBaseHandle

	Lea	ExcludeStart(PC),A4
	Bsr	GetConfigFile
	Lea	BadStart(PC),A4
	Bsr	GetConfigFile
	Bsr	StartPurgeLog
	Bsr	OpenAppFile

	Move.L	UserBaseStart(PC),A0
	Move.L	A0,CurrentPos
	Add.L	UserBaseSize(PC),A0
	Move.L	A0,UserBaseEnd

***************************************
MainLoop:	Move.L	#SIGBREAKF_CTRL_C,D1
	Moveq	#0,D0
	CALLEXEC	SetSignal
	Btst.L	#SIGBREAKB_CTRL_C,D0
	Beq.S	.CarryOn
	Moveq	#0,D1
	Move.L	#Break,D2
	Moveq	#Breaklen,D3
	CALLECLIPSE	OutputText
	Bra	CloseDown

.CarryOn	Move.L	CurrentPos(PC),A0
	Tst.B	(A0)
	Beq.S	.UserKilled

	Move.L	BadStart(PC),A1
	CALLECLIPSE	FindData
	Tst.L	D0
	Beq.S	.KillUser

	Move.L	CurrentPos(PC),A0
	Move.L	124(A0),D0
	Lea	UserLastCall(PC),A0
	CALLECLIPSE	ConvertFalconDate

	Move.L	CurrentStamp(PC),D0
	Sub.L	UserLastCall(PC),D0
	Move.L	DaysOld(PC),D1
	Cmp.L	D1,D0
	Blt.S	.UserOk

.UserKilled	Move.L	CurrentPos(PC),A0
	Move.L	ExcludeStart(PC),A1
	CALLECLIPSE	FindData
	Tst.L	D0
	Beq.S	.UserOk

.KillUser	Addq.L	#1,UsersKilled
	Bsr	DoPurgeLog
	Bra.S	.NextUser

.UserOk	Move.L	UserBaseHandle(PC),D1
	Move.L	CurrentPos(PC),D2
	Move.L	#512,D3
	CALLDOS	Write

	Bsr	WriteAppEntry

.NextUser	Add.L	#512,CurrentPos
	Move.L	UserBaseEnd(PC),D0
	Move.L	CurrentPos(PC),D1
	Sub.L	D1,D0
	Tst.L	D0
	Bne	MainLoop

CloseDown:	Move.L	UserBaseHandle(PC),D1
	CALLDOS	Close

	Move.L	DaysOld(PC),D0
	Moveq	#2,D1
	Lea	DaysText(PC),A0
	CALLECLIPSE	ConvertDecimal

	Move.L	UsersKilled(PC),D0
	Cmp.L	#1,D0
	Bne.S	.Notone
	Move.B	#0,UsersTxt+5
	
.Notone	Moveq	#3,D1
	Lea	UsersText(PC),A0
	CALLECLIPSE	ConvertDecimal

	Lea	UsersTxt(PC),A0
	Moveq	#6,D0
	CALL	CopyData
	
	Lea	TrimText(PC),A0
	CALL	LogTag

	Bsr	CloseAppFile
	Bsr	ClosePurgeLog

	Move.L	ExcludeStart(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoExclude
	Move.L	ExcludeSize(PC),D0
	CALLEXEC	FreeMem
.NoExclude
	Move.L	BadStart(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoBad
	Move.L	BadSize(PC),D0
	CALLEXEC	FreeMem
.NoBad

Error:	Move.L	UserBaseStart(PC),A1
	Move.L	UserBaseSize(PC),D0
	CALLEXEC	FreeMem

***************************************
Exit:	Move.L	_ECLIPSEBase(PC),A1
	CALLEXEC	CloseLibrary

ExitToDos:	Move.L	_DOSBase(PC),A1
	CALLEXEC	CloseLibrary

NoDos:	Tst.L	_WBMessage
	Beq.S	.NoWB

	Move.L	_IconBase(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoIcon
	CALLEXEC	CloseLibrary

.NoIcon	CALLEXEC	Forbid
	Move.L	_WBMessage(PC),A1
	CALL	ReplyMsg

.NoWB	Moveq	#0,D0
	Move.L	_SaveStack(PC),SP
	Rts

***************************************************************************
fromWorkbench:	Lea	pr_MsgPort(A4),A0
	CALLEXEC	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,_WBMessage

***************************************
	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Move.L	D0,_DOSBase
	Beq.S	NoDos
	
	Lea	ECLIPSELibrary(PC),A1
	Moveq	#EclipseVer,D0
	CALL	OpenLibrary
	Move.L	D0,_ECLIPSEBase
	Beq.S	ExitToDos

	Lea	ICONLibrary(PC),A1
	Moveq	#0,D0
	CALLEXEC	OpenLibrary
	Move.L	D0,_IconBase
	Beq	Exit

***************************************
	Move.L	_WBMessage(PC),A0
	Move.L	$24(A0),A0
	Beq	Exit

	Move.L	(A0),D1
	CALLDOS	CurrentDir

	Move.L	_WBMessage(PC),A0
	Move.L	$24(A0),A0
	Move.L	4(A0),A0

	Move.L	_IconBase(PC),A6
	CALLICON	GetDiskObject
	Beq	ExitToDos

	Move.L	D0,A1
	Move.L	$36(A1),A1
	Move.l	A1,_ToolTypes

***************************************
	Move.L	A1,A0
	Lea	DataBase(PC),A1
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoUsers
	Move.L	D0,A0
	Lea	FileName,A1
	Moveq	#108,D0
	CALLECLIPSE	CopyData

.NoUsers	Move.L	_ToolTypes(PC),A0
	Lea	Days(PC),A1
	CALLICON	FindToolType
	Tst.L	D0
	Beq.S	.NoDays
	Move.L	D0,A0
	CALLECLIPSE	ConvertToDecimal
	Tst.L	D0
	Beq.S	.NoDays
	Move.L	D0,DaysOld

.NoDays	Bra	WBRestart

***************************************************************************
GetFile:	Lea	UserBaseStart(PC),A1
	Lea	UserBaseSize(PC),A2
	Moveq	#0,D0
	CALLECLIPSE	ReadFile
	Rts

***************************************************************************
ClearRemoved:	Lea	RemovedText,A0
	Moveq	#79,D0
	Moveq	#' ',D1

.loop	Move.B	D1,(A0)+
	DBra	D0,.loop
	Rts

***************************************************************************
StartPurgeLog:	Lea	FileName2,A1
	Lea	PurgeLog(PC),A0
	Moveq	#13,D0
	CALLECLIPSE	CopyData
	Move.B	#0,(A1)
	
	Move.L	#FileName2,D1
	Move.L	#MODE_READWRITE,D2
	CALLDOS	Open
	Move.L	D0,_LogHandle
	Beq.S	.NoLog

	Move.L	_LogHandle(PC),D1
	Moveq	#0,D2
	Moveq	#OFFSET_END,D3
	CALL	Seek

	Bsr.S	ClearRemoved

	Lea	RemovedText,A0
	Lea	CurrentStamp(PC),A1
	CALLECLIPSE	LongDay
	Move.B	#',',(A1)+
	Move.B	#' ',(A1)+

	Move.L	A1,A0
	Lea	CurrentStamp(PC),A1
	CALL	Date
	Move.B	#10,(A1)+
	Move.B	#0,(A1)

	Move.L	_LogHandle(PC),D1
	Move.L	#RemovedText,D2
	Moveq	#0,D3
	CALL	OutputText
.NoLog	Rts

***************************************
ClosePurgeLog:	Move.L	_LogHandle(PC),D1
	Tst.L	D1
	Beq.S	.NoLog

	Move.L	#RemoveText+2,D2
	Moveq	#10,D3
	CALLDOS	Write

	Move.L	_LogHandle(PC),D1
	Move.L	#UsersText,D2
	Moveq	#9,D3
	CALL	Write

	Move.L	_LogHandle(PC),D1
	Move.L	#Rets,D2
	Moveq	#2,D3
	CALL	Write
	
	Move.L	_LogHandle(PC),D1
	CALL	Close
.NoLog	Rts

***************************************
DoPurgeLog:	Cmp.L	#0,_LogHandle
	Beq.S	.NoLog
	Bsr	ClearRemoved

	Lea	RemoveText(PC),A0
	Lea	RemovedText,A1
	Moveq	#12,D0
	CALLECLIPSE	CopyData

	Move.L	CurrentPos(PC),A0
	Moveq	#40,D0

	Cmp.B	#0,(A0)
	Bne.S	.UserNameOk
	Lea	DeletedUser(PC),A0
	Move.L	#DeletedLen,D0

.UserNameOk	CALL	CopyData
	Move.B	#10,(A1)+
	Move.B	#0,(A1)

	Move.L	_LogHandle,D1
	Move.L	#RemovedText,D2
	Moveq	#0,D3
	CALL	OutputText
.NoLog	Rts

***************************************************************************
OpenAppFile:	Lea	AppFileKey(PC),A0
	Lea	AppFileName,A1
	CALLECLIPSE	GetConfigEntry
	Tst.L	D0
	Bne.S	NoAppData

	Lea	AppFileName,A0
	Moveq	#0,D0
	Lea	AppFileStart,A1
	Lea	AppFileSize,A2
	CALL	ReadFile
	Tst.L	D0
	Bne.S	NoAppData

	Lea	AppFileName,A0
	Bsr	MakeBackup

	Move.L	#AppFileName,D1
	Move.L	#MODE_NEWFILE,D2
	CALLDOS	Open
	Tst.L	D0
	Beq	.ClearAppData
	Move.L	D0,AppFileHandle
	Rts

.ClearAppData	Move.L	AppFileStart(PC),A1
	Move.L	AppFileSize(PC),D0
	CALLEXEC	FreeMem

NoAppData:	Move.L	#0,AppFileStart
	Move.L	#0,AppFileSize
	Rts

***************************************
CloseAppFile:	Move.L	AppFileHandle(PC),D1
	Tst.L	D1
	Beq.S	NoAppData

	CALLDOS	Close

	Move.L	AppFileStart(PC),A1
	Move.L	AppFileSize(PC),D0
	CALLEXEC	FreeMem

	Bra.S	NoAppData

***************************************
WriteAppEntry:	Move.L	AppFileStart(PC),A1
	Cmp.L	#0,A1
	Beq.S	.NoUser
	Move.L	CurrentPos(PC),A0
	CALLECLIPSE	FindData
	Tst.L	D0
	Bne.S	.NoUser

	Move.L	A0,A1
	Lea	-10(A0),A0
	Move.L	A0,AppEntryStart
	Lea	NameText(PC),A0
	CALL	FindData
	Beq.S	.GotNextEntry

	Move.L	AppFileStart(PC),A0
	Add.L	AppFileSize(PC),A0

.GotNextEntry	Move.L	A0,D3
	Move.L	AppEntryStart(PC),D0
	Sub.L	D0,D3

	Move.L	AppFileHandle(PC),D1
	Move.L	AppEntryStart(PC),D2
	CALLDOS	Write

.NoUser	Rts

***************************************************************************
MakeBackup:	Move.L	NameToUse(PC),-(SP)
	Move.L	A0,NameToUse

	Move.L	NameToUse(PC),A0
	Lea	FileName2,A1
	Move.L	#139,D0
	CALLECLIPSE	CopyData
	Subq	#3,A1
	Move.B	#'B',(A1)+
	Move.B	#'A',(A1)+
	Move.B	#'K',(A1)+
	Move.B	#0,(A1)

	Move.L	#FileName2,D1
	Moveq	#ACCESS_READ,D2
	CALLDOS	Lock
	Tst.L	D0
	Beq.S	.NoBackup
	Move.L	D0,D1
	CALL	UnLock

	Move.L	#FileName2,D1
	CALL	DeleteFile

.NoBackup	Move.L	NameToUse(PC),D1
	Move.L	#FileName2,D2
	CALL	Rename
	Move.L	(SP)+,NameToUse
	Rts

***************************************************************************
GetConfigFile:	Lea	12(A4),A0
	Bsr.S	.LoadFile
	Tst.L	D0
	Beq.S	.GotConfig

	Lea	10(A4),A0
	Bsr.S	.LoadFile
	Tst.L	D0
	Beq.S	.GotConfig

	Lea	8(A4),A0
	Move.B	#'V',10(A4)
	Bsr.S	.LoadFile
	Tst.L	D0
	Beq.S	.GotConfig

	Moveq	#-1,D0
	Rts

.LoadFile	Moveq	#0,D0
	Move.L	A4,A1
	Lea	4(A4),A2
	CALLECLIPSE	ReadFile
	Rts

.GotConfig	Move.L	(A4),A0
	Add.L	4(A4),A0
	Move.B	#0,-1(A0)
	Moveq	#0,D0
	Rts

***************************************************************************
***************************************************************************
***************************************************************************
VerString:	Dc.B	0,'$VER: '
	VERSION
	Dc.B	' ('
	DATE
	Dc.B	') '
	AUTHOR
	Dc.B	0

***************************************
DOSLibrary:	DOSNAME
ECLIPSELibrary:	EclipseName
ICONLibrary:	ICONNAME

PurgeLog:	Dc.B	'UserPurge.log',0
UserBaseName:	Dc.B	'User.BBS',0

UserBaseKey:	Dc.B	'USERDIR',0
AppFileKey:	Dc.B	'APPFILE',0

DataBase:	Dc.B	'USERBASE',0
Days:	Dc.B	'DAYS',0

NameText:	Dc.B	'Name:     ',0

RemoveText:	Dc.B	'    Removed '
DeletedUser:	Dc.B	'an already deleted user'
DeletedLen	Equ	*-DeletedUser
TrimText:	Dc.B	'Trimed Userbase to '
DaysText:	Dc.B	'   days, removing '
UsersText:	Dc.B	'         ',0
UsersTxt:	Dc.B	' users'

Break:	Dc.B	10,'What''s wrong?'
Rets:	Dc.B	10
Breaklen	Equ	*-Break
	Dc.B	10
	Even

***************************************
ExcludeStart:	Dc.L	0
ExcludeSize:	Dc.L	0
	Dc.B	'ENS:UserPurge.exclude',0
	Even

BadStart:	Dc.L	0
BadSize:	Dc.L	0
	Dc.B	'ENS:UserPurge.bad',0
	Even

***************************************
_ArgSize:	Dc.L	0
_ArgStart:	Dc.L	0
_SaveStack:	Dc.L	0

NameToUse:	Dc.L	UserBaseName
DaysOld:	Dc.L	30

DataSpace:
_DOSBase:	Dc.L	0
_ECLIPSEBase:	Dc.L	0
_IconBase:	Dc.L	0

_WBMessage:	Dc.L	0
_ToolTypes:	Dc.L	0

_LogHandle:	Dc.L	0

AppFileStart:	Dc.L	0
AppFileSize:	Dc.L	0
AppFileHandle:	Dc.L	0
AppEntryStart:	Dc.L	0

UserBaseStart:	Dc.L	0
UserBaseSize:	Dc.L	0
UserBaseEnd:	Dc.L	0
UserBaseHandle:	Dc.L	0

CurrentPos:	Dc.L	0

UsersKilled:	Dc.L	0

CurrentStamp:	Ds.L	3
UserLastCall:	Ds.L	3

DataSpaceLen	Equ	*-DataSpace

***************************************
	Section	DataSpace,BSS
DataSpace2:

AppFileName:	Ds.B	80
RemovedText:	Ds.B	80

FileName:	Ds.B	140
FileName2:	Ds.B	140

DataSpaceLen2	Equ	*-DataSpace2
