*****************************************************************************
* This code and it's design are (c) 1993 by David Dustin / ECLIPSE Software *
*---------------------------------------------------------------------------*
*                                                                           *
* Any modifications and subsequent release of this program must be sent to  *
* me (David).  As I have released this code to the public, I CANNOT be held *
* responsable for ANYTHING that happens while using this program.           *
*                                                                           *
*****************************************************************************
	Section	IPLRead,Code
	Opt	C+,D+
	Include	mysystem.gs

	Output	Work:IPLRead

VERSION	MACRO
	Dc.B	'IPLRead V1.1'
	ENDM
DATE	MACRO
	Dc.B	'1.2.93'
	ENDM
AUTHOR	MACRO
	Dc.B	'by David Dustin'
	ENDM

DEBUG	Equ	0	;remove ReadArgs checking

*****************************************************************************
* Notes:
*	1.00 - Initial Release
*	1.01 - Fixed error with NewLines and BackSpaces.  Would type
*	       far too many due to an error in the number conversion
*	     - Improved conversion routines
*	     - Added support for more than single digit numbers for 
*	       all IPL codes that support them
*	     - Increased translation buffer size (now 4 x original)
*	1.02 - Corrected error with copy loops.
*	     - Changed to buffered write
*	     - Added TO option, so ANSI files can be saved
*	     - Changed from CSI code to <esc>[ (for easier editing)
*	     - Added CTRL-C break option
*	1.03 - Corrected error with \z option
*	     - Now allows \ on their own to work correctly
*	1.04 - Should hopefully always work now.  Used to crap out,
*	       and only type the first line of the text file
*	     - Removed ANSI-codes for Word-Wrap
*	1.05 - Still wasn't working, so I changed it some more
*	     - Added two more IPL Codes (\v@ & \V#)
*	1.10 - Re-wrote write routine.
*
*****************************************************************************
Start:	Move.L	SP,_SaveStack

	Sub.L	A1,A1
	CALLEXEC	FindTask
	Move.L	D0,A4
	Tst.L	pr_CLI(A4)
	Beq	fromWorkbench

	Lea	DataStart,A0
	Move.L	#DataSize-1,D0

.ClearDataSpace	Move.B	#0,(A0)+
	DBra	D0,.ClearDataSpace

	Lea	DOSLibrary(PC),A1
	Moveq	#37,D0
	CALL	OpenLibrary
	Tst.L	D0
	Beq	NoDOS
	Move.L	D0,_DOSBase

	CALLDOS	Output
	Move.L	D0,_OutputHandle

	IFEQ	DEBUG
	Move.L	#ArgumentTemp,D1
	Move.L	#_Arguments,D2
	Moveq	#0,D3
	CALL	ReadArgs
	Tst.L	D0
	Beq	Exit
	Move.L	D0,_Argument

	Move.L	_Arguments,D1
	ENDC

	IFNE	DEBUG
	Move.L	#FileName,D1
	ENDC

***************************************
	Move.L	#MODE_OLDFILE,D2
	CALL	Open
	Move.L	D0,_FileHandle
	Tst.L	D0
	Bne.S	.GotFile
	Lea	NoFileError(PC),A0
	Bsr	DoErrorMessage
	Bra	Exit2

.GotFile	Move.L	D0,D1
	Move.L	#_FileInfo,D2
	CALL	ExamineFH

	Lea	_FileInfo,A0
	Move.L	fib_Size(A0),D0
	Tst.L	D0
	Beq	.FileError
	Move.L	D0,_FileSize
	Addq	#2,D0
	Move.L	#MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Bne.S	.GotMem

.FileError	Move.L	_FileHandle,D1
	CALLDOS	Close
	Lea	NoMemError(PC),A0
	Bsr	DoErrorMessage
	Bra	Exit2

.GotMem	Addq	#1,D0
	Move.L	D0,_FileStart

	Move.L	_FileHandle,D1
	Move.L	_FileStart,D2
	Move.L	_FileSize,D3
	CALLDOS	Read
	Tst.L	D0
	Bne	.ReadOk
	
	Move.L	_FileHandle,D1
	CALL	Close
	Lea	FileReadError(PC),A0
	Bsr	DoErrorMessage
	Bra	CloseDown

.ReadOk	Move.L	_FileHandle,D1
	CALL	Close

***************************************
	Move.L	_To,D1
	Tst.L	D1
	Beq.S	.NoFileOutput
	
	Move.L	#MODE_NEWFILE,D2
	CALL	Open
	Tst.L	D0
	Bne.S	.GotOutput

	Lea	NoOutputError(PC),A0
	Bsr	DoErrorMessage
	Bra	NoLock

.GotOutput	Move.L	D0,_OutputFile
.NoFileOutput	Move.L	_FileSize,D0
	Lsl.L	#2,D0
	Move.L	D0,_TextSize

	Move.L	#MEMF_CLEAR,D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Beq	NoLock
	Move.L	D0,_TextStart

*****************************************************************************
Main:	Move.L	_FileStart,A0
	Move.L	_TextStart,A1
	Move.L	_FileSize,D0
	Subq	#1,D0
.CopyLoop	Moveq	#0,D1
	Move.B	(A0)+,D1
	Beq.S	.CopyFinished
	Cmp.B	#'\',D1
	Beq	.GotIPLCode
.CopyIt	Move.B	D1,(A1)+
.Copied	DBra	D0,.CopyLoop

***************************************
.CopyFinished	Move.L	_TextStart,A2
.PrintLoop	Lea	_CopyBuffer,A3
.CalcLoop2	Move.B	(A2)+,D4
	Beq.S	.PrintLine
	Move.B	D4,(A3)+
	Cmp.B	#10,D4
	Bne.S	.CalcLoop2
.PrintLine	Move.B	#0,(A3)
	Move.L	_OutputFile,D1
	Tst.L	D1
	Bne.S	.GotFile
	Move.L	_OutputHandle,D1
.GotFile	Moveq	#0,D3
	Move.L	#_CopyBuffer,D2
	CALLDOS	VFPrintf

	Move.L	#SIGBREAKF_CTRL_C,D1
	Moveq	#0,D0
	CALLEXEC	SetSignal
	Btst.L	#SIGBREAKB_CTRL_C,D0
	Beq.S	.NoBreak

	Lea	BreakError(PC),A0
	Bsr	DoErrorMessage
	Bra	CloseDown

.NoBreak	Tst.B	D4
	Bne.S	.PrintLoop
	Bra	CloseDown

***************************************
.GotIPLCode	Move.B	(A0)+,D1
	Bsr	.Upper
	Cmp.B	#'\',D1
	Beq.S	.CopyIt

	Tst.L	_Filter
	Bne	.SkipAll

	Cmp.B	#'X',D1
	Beq	.CopyFinished

	Cmp.B	#'C',D1	;foreground colour
	Bne.S	.NotColour
	Moveq	#'3',D2
	Bra	.AnsiColour

.NotColour	Cmp.B	#'Z',D1	;background colour
	Bne.S	.NotBGColour
	Moveq	#'4',D2
	Bra	.AnsiColour

.NotBGColour	Cmp.B	#'B',D1	;bell (displaybeep)
	Bne.S	.NoBell
	Moveq	#'',D2
	Bra	.DoNumber

.NoBell	Cmp.B	#'O',D1	;bold text
	Bne.S	.NotBold
	Move.B	(A0)+,D1
	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+
	Cmp.B	#'0',D1
	Beq.S	.Off
	Move.B	#'1',(A1)+
	Bra.S	.Dom
.Off	Move.B	#'2',(A1)+
	Move.B	#'2',(A1)+
.Dom	Move.B	#'m',(A1)+
	Bra	.Copied

.NotBold	Cmp.B	#'U',D1	;underscored text
	Bne.S	.NotUnderline
	Moveq	#'4',D2
	Bra	.DoAnsiCode

.NotUnderline	Cmp.B	#'R',D1	;reverse text
	Bne.S	.NotReverse
	Moveq	#'7',D2
	Bra	.DoAnsiCode

.NotReverse	Cmp.B	#'H',D1	;backspaces
	Bne.S	.NotBackSpaces
	Moveq	#8,D2
	Bra	.DoNumber

.NotBackSpaces	Cmp.B	#'N',D1	;newlines
	Bne.S	.NotNewLines
	Moveq	#10,D2
	Bra	.DoNumber

.NotNewLines	Cmp.B	#'^',D1	;cursor up
	Bne.S	.NotCursorUp
	Moveq	#'A',D2
	Bra	.CopyNumber

.NotCursorUp	Cmp.B	#'!',D1	;cursor down
	Bne.S	.NotCursorDown
	Moveq	#'B',D2
	Bra	.CopyNumber

.NotCursorDown	Cmp.B	#'<',D1	;cursor left
	Bne.S	.NotCursorLeft
	Moveq	#'D',D2
	Bra	.CopyNumber

.NotCursorLeft	Cmp.B	#'>',D1	;cursor right
	Bne.S	.NotCursorRight
	Moveq	#'C',D2
	Bra	.CopyNumber

.NotCursorRight	Cmp.B	#'F',D1	;reset screen position
	Bne.S	.NotReset
	Move.B	(A0)+,D1
	Cmp.B	#'1',D1
	Beq.S	.Clear
	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+
	Move.B	#'0',(A1)+
	Move.B	#';',(A1)+
	Move.B	#'0',(A1)+
	Move.B	#'H',(A1)+
	Bra	.Copied
.Clear	Move.B	#'',(A1)+
	Bra	.Copied

.NotReset	Cmp.B	#'Q',D1	;set settings
	Bne.S	.NotSettings
	Move.B	(A0)+,D1
	Cmp.B	#'0',D1
	Beq.S	.NoNewSettings
	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+
	Move.B	#'0',(A1)+
	Move.B	#';',(A1)+
	Move.B	#'3',(A1)+
	Move.B	#'9',(A1)+
	Move.B	#'m',(A1)+
.NoNewSettings	Bra	.Copied

.NotSettings	Cmp.B	#'V',D1	;user variables
	Bne.S	.NotVar
	Move.B	(A0)+,D1
	Cmp.B	#'9',D1
	Bgt.S	.IsLetter
	Sub.B	#48,D1
	Bra.S	.CalcVar
.IsLetter	Bsr	.Upper
	Cmp.B	#'P',D1
	Bgt.S	.ExtraVars
	Sub.B	#55,D1
.CalcVar	Lsl.L	#1,D1
	Lea	Variables(PC),A2
	Lea	Var0(PC),A3
	Add.W	(A2,D1.w),A3
.CopyVarLoop	Move.B	(A3)+,D1
	Cmp.B	#0,D1
	Beq.S	.VarFinished
	Move.B	D1,(A1)+
	Bra.S	.CopyVarLoop

.VarFinished	Bra	.Copied

.ExtraVars	Cmp.B	#'#',D1
	Bne.S	.NotVARUUCP
	Lea	VarUU(PC),A3
	Bra.S	.CopyVarLoop
.NotVARUUCP	Cmp.B	#'@',D1
	Bne.S	.NotVar
	Lea	Var@(PC),A3
	Bra.S	.CopyVarLoop
	
.NotVar
.SkipAll	Move.B	(A0)+,D1
	Bra	.Copied

***************************************
.AnsiColour	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+
	Move.B	D2,(A1)+
	Move.B	(A0)+,(A1)+
	Move.B	#'m',(A1)+
	Bra	.Copied

***************************************
.DoAnsiCode	Move.B	(A0)+,D1
	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+
	Cmp.B	#'0',D1
	Bne.S	.On
	Move.B	#'2',(A1)+
.On	Move.B	D2,(A1)+
	Move.B	#'m',(A1)+
	Bra	.Copied

***************************************
.DoNumber	Move.B	(A0)+,D1
	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+

.DoNumLoop	Sub.B	#48,D1
	Move.B	(A0),D3
	Cmp.B	#'0',D3
	Blt.S	.NotNum
	Cmp.B	#'9',D3
	Bgt.S	.NotNum
	Addq	#1,A0
	Sub.B	#48,D3
	Mulu	#10,D1
	Add.L	D3,D1
	Bra.S	.DoNumLoop
.NotNum	Subq	#1,D1
.NumLoop	Move.B	D2,(A1)+
	DBra	D1,.NumLoop
	Bra	.Copied

***************************************
.CopyNumber	Move.B	(A0)+,D1
	Move.B	#'',(A1)+
	Move.B	#'[',(A1)+

.CopyNumLoop	Move.B	D1,(A1)+
	Move.B	(A0),D1
	Cmp.B	#'0',D1
	Blt.S	.NotNum2
	Cmp.B	#'9',D1
	Bgt.S	.NotNum2
	Addq	#1,A0
	Bra.S	.CopyNumLoop
.NotNum2	Move.B	D2,(A1)+
	Bra	.Copied

***************************************
.Upper	Cmp.B	#'a',D1
	Blo.S	.exit
	Cmp.B	#'z',D1
	Bls.S	.change
	Cmp.B	#$E0,D1
	Blo.S	.exit
	Cmp.B	#$FD,D1
	Bhi.S	.exit
	Cmp.B	#$F7,D1
	Beq.S	.exit
.change	Sub.B	#32,D1
.exit	Rts

*****************************************************************************
CloseDown:	Move.L	_TextStart,A1
	Move.L	_TextSize,D0
	CALLEXEC	FreeMem

NoLock:	Move.L	_OutputFile,D1
	Tst.L	D1
	Beq.S	.NoFile
	CALLDOS	Close
.NoFile	Move.L	_FileStart,A1
	Subq.L	#1,A1
	Move.L	_FileSize,D0
	Addq	#2,D0
	CALLEXEC	FreeMem

Exit2:
	IFEQ	DEBUG
	Move.L	_Argument,D1
	CALLDOS	FreeArgs
	ENDC

Exit:	Move.L	_DOSBase,A1
	CALLEXEC	CloseLibrary
NoDOS:	Moveq	#0,D0
	Move.L	_SaveStack,SP
	Rts

*****************************************************************************
fromWorkbench:	Lea	pr_MsgPort(A4),A0
	CALLEXEC	WaitPort
	Lea	pr_MsgPort(A4),A0
	CALL	GetMsg
	Move.L	D0,A1

	CALL	Forbid
	CALL	ReplyMsg
	Move.L	_SaveStack,SP
	Rts

****************************************************************************
DoErrorMessage:	Move.L	A0,D2
	Moveq	#0,D3
.CalcLoop	Addq.L	#1,D3
	Cmp.B	#0,(A0)+
	Bne.S	.CalcLoop

	Move.L	_OutputHandle,D1
	CALLDOS	Write
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
ArgumentTemp:	Dc.B	'FILENAME/A,TO/K,F=FILTER/S',0
NoFileError:	Dc.B	'Could not find file',10,0
FileReadError:	Dc.B	'Error reading file',10,0
NoOutputError:	Dc.B	'Could not open output file',10,0
NoMemError:	Dc.B	'No memory to load file',10,0
BreakError:	Dc.B	'...Break IPLRead',10,0
CharSpace:	Dc.B	0
Var0:	Dc.B	'<Date & Time>',0
Var1	Equ	*-Var0
	Dc.B	'<Last Call>',0
Var2	Equ	*-Var0
	Dc.B	'<Alias>',0
Var3	Equ	*-Var0
	Dc.B	'<Real Name>',0
Var4	Equ	*-Var0
	Dc.B	'<Phone Number>',0
Var5	Equ	*-Var0
	Dc.B	'<Password>',0
Var6	Equ	*-Var0
	Dc.B	'<Access>',0
Var7	Equ	*-Var0
	Dc.B	'<Time of day>',0
Var8	Equ	*-Var0
	Dc.B	'<System Calls>',0
Var9	Equ	*-Var0
	Dc.B	'<Street Address>',0
Vara	Equ	*-Var0
	Dc.B	'<City & State>',0
Varb	Equ	*-Var0
	Dc.B	'<Post Code>',0
Varc	Equ	*-Var0
	Dc.B	'<Remaining Time>',0
Vard	Equ	*-Var0
	Dc.B	'<Users Calls>',0
Vare	Equ	*-Var0
	Dc.B	'<Dl''d Files>',0
Varf	Equ	*-Var0
	Dc.B	'<Ul''d Files>',0
Varg	Equ	*-Var0
	Dc.B	'<Dl''d Bytes>',0
Varh	Equ	*-Var0
	Dc.B	'<Ul''d Bytes>',0
Vari	Equ	*-Var0
	Dc.B	'<# of Posts>',0
Varj	Equ	*-Var0
	Dc.B	' ',0
Vark	Equ	*-Var0
	Dc.B	'<File credits>',0
Varl	Equ	*-Var0
	Dc.B	'<Byte credits>',0
Varm	Equ	*-Var0
	Dc.B	'<First name>',0
Varn	Equ	*-Var0
	Dc.B	'<Last name>',0
Varo	Equ	*-Var0
	Dc.B	'<SubBrd number>',0
Varp	Equ	*-Var0
	Dc.B	'<SubBrd name>',0
Var@:	Dc.B	'<BBS Name>',0
VarUU:	Dc.B	'<UUCP Address>',0	;ment to be #
	IFNE	DEBUG
FileName:	Dc.B	'Excel:ex.lastuser',0
	ENDC
	Even

Variables:	Dc.W	0,Var1,Var2,Var3,Var4,Var5,Var6,Var7,Var8
	Dc.W	Var9,Vara,Varb,Varc,Vard,Vare,Varf,Varg,Varh
	Dc.W	Vari,Varj,Vark,Varl,Varm,Varn,Varo,Varp,0

*****************************************************************************
	Section	DataSpace,BSS
_SaveStack:	Ds.L	1

DataStart:
_FileStart:	Ds.L	1
_FileSize:	Ds.L	1
_FileHandle:	Ds.L	1

_TextStart:	Ds.L	1
_TextSize:	Ds.L	1
_Arguments:	Ds.L	1
_To:	Ds.L	1
_Filter:	Ds.L	2

_OutputHandle:	Ds.L	1
_OutputFile:	Ds.L	1
_Argument:	Ds.L	1
_DOSBase:	Ds.L	1
	CNOP	0,4
_FileInfo:	Ds.B	260
_CopyBuffer:	Ds.B	1024
DataSize	Equ	*-DataStart
