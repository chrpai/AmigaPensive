;Released as public domain 27-Jan-1996 by the author Renze de Ruiter

**********************************************************
*                                                        *
* Callers.s ver 3.01 by Renze de Ruiter/Eclipse Software *
*                                                        *
*         Copyright (c) 1994 All Rights Reserved         *
*                                                        *
*                                                        *
* Dave's comment: I did a lot of fixes, and I hate this  *
*                 this program now!!                     *
**********************************************************

	Section	Callers,CODE
	opt	C+,D-

	include	mysystem.gs
	include	dos/rdargs.i
	Include	excel/excel.i

*******************************************************************************
Start:	Move.L	$4.w,A6
	Move.L	A6,_EXECBase

	lea	_DOSName,A1
	moveq	#37,D0
	CALL	OpenLibrary
	move.l	D0,_DOSBase
	beq	no37

********************************
	Lea	_CustomRDArgs,A0
	Move.L	#_ExtHelp,RDA_ExtHelp(A0)	;pointer to extended help text
	Move.L	A0,D3		;my custom RDArgs structure

	Move.L	#_Template,D1
	Move.L	#_MyArgs,D2
	CALLDOS	ReadArgs
	Move.L	D0,_RdArgs
	Beq	noargs

********************************
	Bsr	Readkey

	Tst.B	_KeyOK
	Beq.S	.reg_ok
	Move.L	#_Unregmsg,D1
	CALL	PutStr		;faster than VPrintf
	Move.L	#250,D1
	CALL	Delay
.reg_ok
********************************
	Move.L	#10,_MaxEntries		;default number of lines
	Tst.B	_KeyOK		;keyfile protection
	Bne.S	.maxok		;keyfile protection

	Move.L	_Max,A0
	Cmp.L	#0,A0
	Beq.S	.maxok
	Cmp.L	#0,(A0)
	Beq.S	.maxok
	Move.L	(A0),_MaxEntries
.maxok
********************************
	move.l	#_Logotable,D2
	move.l	#_ANSILogo,D1
	tst.l	_Noansi
	beq.S	.drawlogo
	move.l	#_ASCIILogo,D1
.drawlogo	CALLDOS	VPrintf

********************************
	Move.L	_LogFile,D2
	Bne.S	.lnameprovided

	Move.L	#_SDName,D1
	Move.L	#MODE_OLDFILE,D2
	CALL	Open
	Move.L	D0,D5
	Beq	noargs

	Move.L	D0,D1
	Move.L	#_SystemData,D2
	Move.L	#exsd_SIZE,D3
	CALL	Read

	Move.L	D5,D1
	CALL	Close

	Move.L	#_FileName,D1
	Move.L	#_SystemData+exsd_Path+128,D2
	Move.L	#130,D3
	CALL	AddPart

	Move.L	#_Logname,D2
.lnameprovided	Move.L	#_FileName,D1
	Move.L	#130,D3
	CALL	AddPart

	move.l	#_FileName,D1
	move.l	#MODE_OLDFILE,D2
	CALL	Open
	move.l	D0,_File
	beq	noargs

	move.l	D0,D1
	moveq	#0,D2
	moveq	#OFFSET_END,D3
	CALL	Seek
	cmp.l	#-1,D0
	beq	badlogfile

	move.l	_File,D1
	moveq	#0,D2
	moveq	#OFFSET_BEGINNING,D3
	CALL	Seek
	addq	#1,D0
	move.l	D0,_Length

	Addq	#2,D0		;prevent memmunging
	move.l	#MEMF_ANY|MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	move.l	D0,_Memptr
	beq	badlogfile
	Addq.L	#2,_Memptr

	move.l	_File,D1
	move.l	_Memptr,D2
	move.l	_Length,D3
	CALLDOS	Read

*******************************************************************************
	move.l	_Memptr,A1
	Moveq	#0,D2
.countloop	lea	_Separator,A0
	bsr	FindData
	tst.l	D0
	bne.s	.countdone
	Addq	#1,D2
	Bra.s	.countloop
.countdone	move.L	D2,_LogEntries

	tst.l	D2
	beq	Notablemem

********************************
	Move.l	_LogEntries,D0
	lsl	#2,D0
	move.l	D0,D2
	Addq	#4,D0
	move.l	#MEMF_ANY|MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	move.l	D0,_Offsets
	beq	Notablemem
	Add.L	D0,D2
	Move.L	D2,_Endoftable

********************************
	move.l	_Memptr,A1
	Move.L	_Offsets,A2
	Move.L	_LogEntries,D3
	subq	#2,D3

tableloop:	lea	_Separator,A0
	bsr	FindData
	tst.l	D0
	bne.s	tabledone

	lea	4(A2),A3
	move.l	A2,A4
	Move.L	D3,D0
.shiftloop	move.l	(A3)+,(A4)+
	dbra	D0,.shiftloop

	move.l	A0,(A4)
	move.b	#0,-1(A0)
	bra.s	tableloop

tabledone:	Move.L	_LogEntries,D0
	Move.L	D0,_Entries
	Move.L	_MaxEntries,D1
	Cmp.L	D0,D1
	Bgt.S	.Okdo
	Move.L	_MaxEntries,_Entries
.Okdo
********************************
	move.l	#_ANSITable,D1
	tst.l	_Noansi
	beq.s	.drawtable
	move.l	#_ASCIITable,D1
.drawtable	CALLDOS	PutStr		;VPrintf replacement

********************************
	move.l	_User,A2
	lea	_Strbuffer,A1
.concatloop	move.l	(A2)+,A0
	cmp.l	#0,A0
	beq.s	.endconcat
.copyloop	move.b	(A0)+,D0
	beq.s	.copyloopdone
	move.b	D0,(A1)+
	bra.s	.copyloop

.copyloopdone	move.b	#' ',(A1)+
	bra.s	.concatloop

.endconcat	move.b	#0,-1(A1)

*******************************************************************************
	Tst.L	_Hidefile
	Beq	.noHFile

	Move.L	_Hidefile,D1
	move.l	#MODE_OLDFILE,D2
	CALL	Open
	move.l	D0,_HFile
	beq	.noHFile

	move.l	D0,D1
	moveq	#0,D2
	moveq	#OFFSET_END,D3
	CALL	Seek
	cmp.l	#-1,D0
	beq	.badHFile

	move.l	_HFile,D1
	moveq	#0,D2
	moveq	#OFFSET_BEGINNING,D3
	CALL	Seek
	addq	#1,D0
	move.l	D0,_HFileSize

	Addq	#1,D0
	move.l	#MEMF_ANY|MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	move.l	D0,_HFileStart
	beq.S	.badHFile

	move.l	_HFile,D1
	move.l	_HFileStart,D2
	move.l	_HFileSize,D3
	CALLDOS	Read

.badHFile	Move.L	_HFile,D1
	CALL	Close
.noHFile		
*******************************************************************************
	move.l	_Endoftable,A5
	move.L	_Entries,D6
	subq	#1,D6

mainloop:	subq	#4,A5
	Bsr	ClearData

	tst.L	(A5)
	beq	closedown

********************************
	lea	FindAlias,A0
	bsr	FindString
	tst.l	D0
	bne.s	mainloop

	moveq	#25,D1
	lea	_Alias,A2

.aliasloop	move.b	(A1)+,D0
	cmp.b	#'(',D0
	beq.s	.aliasskip
	cmp.b	#' ',D0
	bne.s	.aliasspace
	cmp.b	#' ',(A1)
	beq.s	.aliasend
.aliasspace	move.b	D0,(A2)+
	dbra	D1,.aliasloop
.aliasend	addq	#1,A2
.aliasskip	subq	#1,A2
	move.b	#0,(A2)

	Move.L	_Hide,A0
	Tst.B	(A0)
	Beq.S	.skiphide
	Lea	_Alias,A1
	Bsr	CompareStrings
	Tst.L	D0
	Beq.S	mainloop
.skiphide
********************************
	Move.L	_HideID,A0
	Cmp.L	#0,A0
	Beq.S	.noID
	Move.L	(A0),D3

	lea	FindID,A0
	bsr	FindString
	tst.l	D0
	bne.s	.noID

	move.l	A1,D1
	move.l	#_UserID,D2
	CALLDOS	StrToLong

	Move.L	_UserID,D4
	Sub.L	D4,D3
	Beq	mainloop
.noID

********************************
	Move.L	_HFileStart,A1
	Cmp.L	#0,A1
	Beq.S	.noHFile

	Lea	_Alias,A2
	Lea	_HideAliasBuf,A0
	Move.L	A0,A3
	Moveq	#26,D0
.HFileloop	Move.B	(A2)+,D1
	Cmp.B	#0,D1
	Beq.S	.HFloopend
	Move.B	D1,(A3)+
	DBra	D0,.HFileloop

.HFloopend	Move.B	#10,(A3)+
	Move.B	#0,(A3)+
	Bsr	FindData
	Tst.L	D0
	Beq	mainloop
.noHFile
********************************
	lea	FindDate,A0
	bsr	FindString
	tst.l	D0
	bne	mainloop
	addq	#4,A1

	lea	_Calldate,A2
	move.b	(A1)+,(A2)+
	move.b	(A1)+,D0
	cmp.b	#'-',D0
	beq.s	.dateskip
	move.b	D0,(A2)+
	addq	#1,A1
.dateskip	move.b	#' ',(A2)+
	move.b	(A1)+,(A2)+
	move.b	(A1)+,(A2)+
	move.b	(A1)+,(A2)+
	move.b	#0,(A2)

********************************
	addq	#6,A1
	lea	_Calltime,A2
	moveq	#6,D1
.timeloop	move.b	(A1)+,D0
	cmp.b	#'>',D0
	beq.s	.timeskip
	move.b	D0,(A2)+
	dbra	D1,.timeloop
.timeskip	move.b	#0,(A2)

********************************
	lea	FindBaud,A0
	bsr	FindString
	tst.l	D0
	bne.s	.nobaud

	moveq	#4,D1
	lea	_Baudrate,A2

.baudloop	move.b	(A1)+,D0
	cmp.b	#' ',D0
	beq.s	.baudskip
	move.b	D0,(A2)+
	dbra	D1,.baudloop
.baudskip	move.b	#0,(A2)
	bra.s	.notlocal
.nobaud
********************************
	lea	SysopLogin,A0
	bsr	FindString
	tst.l	D0
	bne.s	.notsysop
	tst.b	_KeyOK		;keyfile protection
	bne.s	.local_nokey		;keyfile protection
	tst.l	_Localoff
	bne	mainloop
.local_nokey	move.l	#LocalBaud,_Baud
	bra.s	.notlocal

.notsysop	lea	LocalLogin,A0
	bsr	FindString
	tst.l	D0
	bne.s	.notlocal
	tst.b	_KeyOK		;keyfile protection
	bne.s	.local_nokey2		;keyfile protection
	tst.l	_Localoff
	bne	mainloop
.local_nokey2	move.l	#LocalBaud,_Baud
.notlocal
********************************
	lea	FindTimeon,A0
	bsr	FindString
	tst.l	D0
	bne	mainloop

.timeonloop	move.b	-(A0),D0
	cmp.b	#'(',D0
	bne.s	.timeonloop
	addq	#1,A0
	move.l	A0,D1
	move.l	#_Timeon,D2
	CALLDOS	StrToLong

********************************
	Tst.L	_Timeonline
	Beq.S	.onlineskip
	Move.L	_Timeonline,A0
	Move.L	(A0),D0
	Move.L	_Timeon,D1
	Sub.L	D1,D0
	Bgt	mainloop
.onlineskip
********************************
	lea	FindDownload,A0
	bsr	FindString
	tst.l	D0
	bne.s	.nodownload

	move.l	A1,D1
	move.l	#_Downloads,D2
	CALL	StrToLong
.nodownload
********************************
	lea	FindUpload,A0
	bsr	FindString
	tst.l	D0
	bne.s	.noupload

	move.l	A1,D1
	move.l	#_Uploads,D2
	CALL	StrToLong
.noupload
********************************
	moveq	#0,D4
	Move.L	(A5),A1
.postloop	lea	FindPosts,A0
	bsr	FindData
	tst.l	D0
	bne.s	.postdone
	addq	#1,D4
	bra.s	.postloop
.postdone	move.l	D4,_Posts

*******************************************************************************
display:
	Move.L	_Timeon,D0
	Add.L	D0,_TotalTime
	Move.L	_Posts,D0
	Add.L	D0,_TotalPosts
	Move.L	_Uploads,D0
	Add.L	D0,_TotalUpload
	Move.L	_Downloads,D0
	Add.L	D0,_TotalDownload

	lea	_Strbuffer,A0
	Cmp.B	#0,(A0)
	Beq.S	.nohilite
	lea	_Alias,A1
	bsr	CompareStrings
	tst.l	D0
	bne.s	.nohilite

	move.l	#_ANSIHLine,D1
	tst.l	_Noansi
	beq.s	.drawline
	move.l	#_ASCIIHLine,D1
	bra.s	.drawline

.nohilite	move.l	#_ANSILine,D1
	tst.l	_Noansi
	beq.s	.drawline
	move.l	#_ASCIILine,D1

.drawline	move.l	#_Vartable,D2
	CALLDOS	VPrintf

********************************
	Move.L	#SIGBREAKF_CTRL_C,D1	;check to see if we got a break_c message
	Moveq	#0,D0
	CALLEXEC	SetSignal
	Btst.L	#SIGBREAKB_CTRL_C,D0
	Beq.S	.nobreak

	move.l	#Break,D1		;say what we are doing
	CALLDOS	PutStr		;replacing VPrintf
	Move.B	#-1,Break
	Bra.s	closedown
.nobreak
********************************
	dbra	D6,mainloop

*******************************************************************************
closedown:	Move.l	_LogEntries,D0
	lsl	#2,D0
	Addq	#4,D0
	move.l	_Offsets,A1
	CALLEXEC	FreeMem

	Move.L	_HFileSize,D0
	Beq.S	.noHFile
	Move.L	_HFileStart,A1
	CALL	FreeMem
.noHFile
********************************
Notablemem:	Cmp.B	#-1,Break		;don't draw bottom line if a break_c was received
	Beq.S	.skiptail

	Tst.L	_Nototals
	Beq.S	.noNT
	move.l	#_ANSINTTail,D1
	tst.l	_Noansi
	beq.s	.drawtail
	move.l	#_ASCIINTTail,D1
	Bra.S	.drawtail

.noNT	Move.L	#_TotalTime,D2
	move.l	#_ANSITail,D1
	tst.l	_Noansi
	beq.s	.drawtail
	move.l	#_ASCIITail,D1
.drawtail	CALLDOS	VPrintf		;replacing VPrintf
.skiptail
********************************
	move.l	_Memptr,A1
	Subq.L	#2,A1		;prevent memmunging
	move.l	_Length,D0
	Addq	#2,D0		;get correct length
	CALLEXEC	FreeMem

********************************
badlogfile:	move.l	_File,D1
	CALLDOS	Close

noargs:	move.l	_RdArgs,D1
	CALL	FreeArgs

********************************
	move.l	_DOSBase,A1
	CALLEXEC	CloseLibrary

no37:	moveq	#0,D0
	rts

*******************************************************************************
ClearData:	Move.B	#0,_Calldate
	Move.B	#0,_Calltime
	Move.B	#0,_Alias
	Move.B	#0,_Baudrate
	Move.B	#0,_HideAliasBuf
	Move.L	#_Baudrate,_Baud
	Move.L	#0,_Timeon
	Move.L	#0,_Posts
	Move.L	#0,_Uploads
	Move.L	#0,_Downloads
	Rts

*******************************************************************************
FindString:	move.L	(a5),A1
	include	routines/finddata.s

	include	routines/comparestrings.s
	include	routines/changecase.s

	include	routines/keycode.s

*******************************************************************************
	Section	CallersData,DATA

_DOSName:	DOSNAME
_Version:	dc.b	'$VER: Callers 3.01 (10.8.94) (c)1994 Renze de Ruiter/Eclipse Software',0

_Keyname:	dc.b	'L:Callers.key',0

_SDName:	dc.b	'SystemData',0
_Logname:	dc.b	'Logs/'
_IDLabel:	dc.b	'Callers',0

_ExtHelp:	Dc.B	10
	Dc.B	'LOGFILE    - Filename of log file to view',10
	Dc.B	'HIDE       - Name of a user to hide from the calls list',10
	Dc.B	'HIDEID     - ID number of user to hide from the calls list',10
	Dc.B	'HIDEFILE   - Pointer to a filename of users to be hidden during listing',10
	Dc.B	'MAX        - Maximum number of entries to be displayed',10
	Dc.B	'TIMEONLINE - Only list users online for more than x minutes',10
	Dc.B	'LOCALOFF   - Hide LOCAL calls',10
	Dc.B	'NOANSI     - Use ASCII display, with no ANSI colours',10
	Dc.B	'NOTOTALS   - Don''t print the totals at the bottom of the list',10
	Dc.B	'USER       - Name to be highlighted if it appears in the list.  Normally',10
	Dc.B	'             the name of the current user online',10,10

_Template:	dc.b	'LF=LOGFILE/K,H=HIDE/K,HID=HIDEID/K/N,HF=HIDEFILE/K,M=MAX/K/N,T=TIMEONLINE/K/N,L=LOCALOFF/S,N=NOANSI/S,NT=NOTOTALS/S,USER/M',0

_Unregmsg:	dc.b	'Unregistered version. Delaying for 5 seconds.',10,10,0
Break:	Dc.B	27,'[0m...BREAK',10,0

_ASCIILogo:	dc.b	' +---------------------------+',10
	dc.b	' | C A L L E R S   V 3 . 0 1 |',10
	dc.b	' | (c) 1994 Eclipse Software | Reg: %s (%s) #%03ld',10
	dc.b	' +---------------------------+',10,10,0

_ANSILogo:	dc.b	' ',27,'[1;33;44m C A L L E R S   V 3 . 0 1 ',27,'[0;37m',10
	dc.b	' ',27,'[1;36;44m (c) 1994 Eclipse Software ',27,'[0;37m Reg: %s (%s) #%03ld',27,'[0m',10,10,0

_ASCIITable:	dc.b	' |========|=========|===========================|=======|=====|=====|====|====|',10
	dc.b	' | Date   | Time    | Alias                     | Speed | Min | Msg | UL | DL |',10
	dc.b	' |--------|---------|---------------------------|-------|-----|-----|----|----|',10,0

_ANSITable:	dc.b	27,'[1;34m ษออออออออัอออออออออัอออออออออออออออออออออออออออัอออออออหอออออัอออออัออออัออออป',10
	dc.b	27,'[1;34m บ ',27,'[37mDate   ',27,'[34mณ ',27,'[37mTime    ',27,'[34mณ ',27,'[37mAlias                     ',27,'[34mณ ',27,'[37mSpeed '
	dc.b	27,'[34mบ ',27,'[37mMin ',27,'[34mณ ',27,'[37mMsg ',27,'[34mณ ',27,'[37mUL ',27,'[34mณ ',27,'[37mDL ',27,'[34mบ',10
	dc.b	27,'[1;34m วฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤืฤฤฤฤฤลฤฤฤฤฤลฤฤฤฤลฤฤฤฤถ',10,0

_ASCIILine:	dc.b	' | %-6.6s | %7.7s | %-25.25s | %5.5s | %3.3ld | %3.3ld | %2.2ld | %2.2ld |',10,0
_ASCIIHLine:	dc.b	' | %-6.6s | %7.7s |[%-25.25s]| %5.5s | %3.3ld | %3.3ld | %2.2ld | %2.2ld |',10,0
_ANSILine:	dc.b	27,'[1;34m บ ',27,'[0;35m%-6.6s ',27,'[1;34mณ ',27,'[35m%7.7s ',27,'[34mณ ',27,'[33m%-25.25s ',27,'[34mณ ',27,'[32m%5.5s ',27,'[34mบ '
	dc.b	27,'[0;36m%3.3ld ',27,'[1;34mณ ',27,'[0;31m%3.3ld ',27,'[1;34mณ ',27,'[31m%2.2ld ',27,'[34mณ ',27,'[31m%2.2ld ',27,'[34mบ',10,0
_ANSIHLine:	dc.b	27,'[1;34m บ ',27,'[0;35m%-6.6s ',27,'[1;34mณ ',27,'[35m%7.7s ',27,'[34mณ',27,'[33;41m %-25.25s ',27,'[34;40mณ ',27,'[32m%5.5s '
	dc.b	27,'[34mบ ',27,'[0;36m%3.3ld ',27,'[1;34mณ ',27,'[0;31m%3.3ld ',27,'[1;34mณ ',27,'[31m%2.2ld ',27,'[34mณ ',27,'[31m%2.2ld ',27,'[34mบ',10,0

_ASCIINTTail:	dc.b	' |========|=========|===========================|=======|=====|=====|====|====|',10,0

_ASCIITail:	dc.b	' |========|=========|===========================|=======|=====|=====|====|====|',10
	dc.b	'                                                        | %3.3ld | %3.3ld | %2.2ld | %2.2ld |',10
	dc.b	'                                                        |=====================|',10,0

_ANSINTTail:	dc.b	27,'[1;34m ศออออออออฯอออออออออฯอออออออออออออออออออออออออออฯอออออออสอออออฯอออออฯออออฯออออผ',27,'[0;37m',10,0

_ANSITail:	dc.b	27,'[1;34m ศออออออออฯอออออออออฯอออออออออออออออออออออออออออฯอออออออฮอออออุอออออุออออุออออน',27,'[0;37m',10
	dc.b	27,'[1;34m                                                        บ',27,'[0;36m %3.3ld ',27,'[1;34mณ',27,'[0;31m %3.3ld ',27,'[1;34mณ',27,'[31m %2.2ld ',27,'[34mณ',27,'[31m %2.2ld ',27,'[34mบ',27,'[0;37m',10
	dc.b	27,'[1;34m                                                        ศอออออฯอออออฯออออฯออออผ',27,'[0;37m',10,0
_KeyOK:	dc.b	-1
_BBS:	dc.b	'Unregistered',0
	ds.b	40-(*-_BBS)
_Separator:	dc.b	'\q1------',0
FindBaud:	dc.b	'Login: ',0
FindDate:	dc.b	'   <',0
FindAlias:	dc.b	'User: ',0
FindTimeon:	dc.b	' elapsed)',0
FindDownload:	dc.b	'Download: ',0
FindUpload:	dc.b	'Upload  : ',0
FindPosts:	dc.b	' message ',0
FindID:	dc.b	' ID#: ',0
SysopLogin:	dc.b	'SysOp Login',0
LocalLogin:	dc.b	'Local Login',0
LocalBaud:	dc.b	'LOCAL',0
	even

_Vartable:	dc.l	_Calldate
	dc.l	_Calltime
	dc.l	_Alias
_Baud:	dc.l	_Baudrate
_Timeon:	dc.l	0
_Posts:	dc.l	0
_Uploads:	dc.l	0
_Downloads:	dc.l	0
	dc.l	0

_Logotable:	dc.l	_Sysop
	dc.l	_BBS
_Serial:	dc.l	0
	dc.l	0

*******************************************************************************
	Section	CallersBSS,BSS
_DOSBase:	ds.l	1
_EXECBase:	ds.l	1
_File:	ds.l	1

_Keybuffer:	ds.b	96
_Strbuffer:	ds.b	200
_FileName:	ds.b	130

_MyArgs:
_LogFile:	ds.l	1
_Hide:	ds.l	1
_HideID:	ds.l	1
_Hidefile:	ds.l	1
_Max:	ds.l	1
_Timeonline:	ds.l	1
_Localoff:	ds.l	1
_Noansi:	ds.l	1
_Nototals:	ds.l	1
_User:	ds.l	2

_Sysop:	ds.b	40
_ID:	ds.b	12

_RdArgs:	ds.l	1

_Calldate:	ds.b	8
_Calltime:	ds.b	8
_Alias:	ds.b	26
_HideAliasBuf:	ds.b	28
_Baudrate:	ds.b	6

_TotalTime:	Ds.L	1
_TotalPosts:	Ds.L	1
_TotalUpload:	Ds.L	1
_TotalDownload:	Ds.L	2

_Length:	ds.l	1
_Memptr:	ds.l	1
_Offsets:	ds.l	1
_Endoftable:	ds.l	1
_MaxEntries:	ds.l	1
_Entries:	ds.l	1
_LogEntries:	ds.l	1
_UserID:	ds.l	1

_HFile:	ds.l	1
_HFileStart:	ds.l	1
_HFileSize:	ds.l	1

_CustomRDArgs:	Ds.B	RDA_SIZEOF
_SystemData:	Ds.B	exsd_SIZE
	END


Program History:
V3.00 (12/7/94) RDR 	Done
      (13/7/94) DRD 	Fixed & Finished
                    	Added TIMEONLINE function
                    	Added CTRL_C abort function
      (14/7/94) DRD 	Added keyfile protection routines back in
                    	Added Extended help for commandline
                    	Added Totals for Time, Msgs's, Uploads & Downloads
                    	Added HIDEID option to hide users via their UserID's
                    	Added HIDEFILE option to hide a list of users
                    	Added NOTOTALS option to remove totals at bottom of list
V3.01 (10/8/94) DRD 	Added LOGFILE option, to specify the logfile to read
                    	Added use of SystemData file
