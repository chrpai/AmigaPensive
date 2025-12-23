
*-------------------------------------------*
*  QuickLogin (C)1996 DayDream Development
*
*  Written as an example to doorcoding for
*  DayDream in Assembler.
*
*  Can be made resident.
*
*  Assembles fine with DevPac, SNMA, PhxAss
*  and Asm-One. Beware that the escape codes
*  are stripped if loaded into Asm-One.
*
*-------------------------------------------*
	Incdir	Include2:
	Include	DayDream.i
	Include	libraries/DreamDoor_lib.i
	Incdir	Include:
	Include	exec/memory.i
	Include	exec/types.i
	Include	exec/exec_lib.i
	Include	dos/dos_lib.i
*-------------------------------------------*
	STRUCTURE	Globals,0
	ULONG		cmdlin
	APTR		DreamDoorBase
	APTR		DosBase
	APTR		dif
	LABEL		MemSize

QuickLogin:
		Move.L	A0,A4			;Store commandline parameters
		Move.L	D0,D4

		Move.L	$4.w,A6
		Move.L	#MemSize,D0
		Move.L	#MEMF_PUBLIC!MEMF_CLEAR,D1
		Jsr	_LVOAllocMem(A6) 	;Allocate memory for variables.
		Move.L	D0,A5
		Tst.L	D0
		Beq	.Quit

		Clr.B	-1(A4,D4.L)
		Move.L	A4,cmdlin(A5)

		Lea.l	_dreamname(Pc),a1	; Open dreamdoor.library V5+
		MoveQ	#5,d0
		Move.l	$4.w,a6
		Jsr	_LVOOpenLibrary(a6)
		Tst.l	d0
		Beq	.Quit
		Move.l	d0,DreamDoorBase(a5)

		Lea.l	_dosname(Pc),a1		; Open dos.library
		MoveQ	#36,d0
		Move.l	$4.w,a6
		Jsr	_LVOOpenLibrary(a6)
		Tst.l	d0
		Beq	.Quit
		Move.l	d0,DosBase(a5)


		Move.l	cmdlin(a5),a0			; Commandline
		Move.l	DreamDoorBase(a5),a6		; DDLibBase
		Jsr	_LVOInitDoor(a6)		; InitDoor()
		Tst.l	d0				; Success ?
		Beq	.Quit				; Nope, QUIT
		Move.l	d0,dif(a5)			; Store result.


		Lea.l	_Selecting(Pc),a0
		Move.l	dif(a5),d0
		Move.l	DreamDoorBase(a5),a6		; DDLibBase
		Jsr	_LVOChangeActivity(a6)		; ChangeActivity()

		Move.l	DreamDoorBase(a5),a6		; DDLibBase
		Move.l	dif(a5),d0
		Jsr	_LVOHideCursor(a6)		; HideCursor()

		Move.l	dif(a5),d0			; DIFace
		Lea.l	_Prompt(Pc),a0			; String to send
		Move.l	DreamDoorBase(a5),a6		; DDLibBase
		Jsr	_LVOSendString(a6)		; SendString()

		Move.l	dif(a5),d0			; DIFace
		Move.l	#HOTKEYF_ADDLF,d1		; Flags
		Move.l	DreamDoorBase(a5),a6
		Jsr	_LVOGetKey(a6)
		Tst.l	d0				; Carrier lost ?
		Beq	.Quit

		Lea.l	NormalName(Pc),a0		; Normal is default.
		Cmp.b	#'q',d0
		Bne.s	.NoQuick
		Lea.l	QuickName(Pc),a0		; Use quick login
.NoQuick:	Cmp.b	#'s',d0
		Bne.s	.NoShort
		Lea.l	ShortName(Pc),a0		; Use short login
.NoShort:
		Move.l	a0,-(sp)
		Move.l	dif(a5),d0
		Move.l	DreamDoorBase(a5),a6		; DDLibBase
		Jsr	_LVOChangeActivity(a6)		; ChangeActivity()
		Move.l	(sp)+,a0

		Move.l	dif(a5),d0
		Move.l	#DDF_ADDEXTENSION!DDF_SHOWERROR,d1
		Move.l	DreamDoorBase(a5),a6
		Jsr	_LVODisplayFile(a6)		; Display File.		

		Move.l	DreamDoorBase(a5),a6		; DDLibBase
		Move.l	dif(a5),d0
		Jsr	_LVOShowCursor(a6)		; ShowCursor()

.Quit:		Cmp.l	#0,a5				; Was memalloc ok ?
		Beq	.NoMem

		Tst.l	dif(a5)				; Did we get a link ?
		Bne.s	.NoError0
		Move.l	#_ErrorTxt2,d1
		Bsr	.DoError
.NoError0:
		Tst.l	dif(a5)
		Beq	.NoDif
		Move.l	dif(a5),d0
		Move.l	DreamDoorBase(a5),a6
		Jsr	_LVOCloseDoor(a6)		; Close door.
		Clr.l	dif(a5)
.NoDif:
		Tst.l	DreamDoorBase(a5)
		Bne.s	.NoError1
		Move.l	#_ErrorTxt1,d1
		Bsr	.DoError
.NoError1:
		Move.l	DreamDoorBase(a5),a1
		Cmp.l	#0,a1
		Beq	.NoDDLib
		Move.l	$4.w,a6
		Jsr	_LVOCloseLibrary(a6)		; Close ddlib.
		Clr.l	DreamDoorBase(a5)
.NoDDLib:
		Move.l	DosBase(a5),a1
		Cmp.l	#0,a1
		Beq	.NoDos
		Move.l	$4.w,a6
		Jsr	_LVOCloseLibrary(a6)		; Close dos.library
.NoDos:
		Move.l	a5,a1
		Move.l	#MemSize,d0
		Move.l	$4.w,a6
		Jsr	_LVOFreeMem(a6)			; Free variable space.

.NoMem:		MoveQ	#0,d0
		Rts


.DoError:	Move.l	DosBase(a5),a6
		Cmp.l	#0,a6
		Beq	.NoDosL
		Jsr	_LVOPutStr(a6)
.NoDosL:	Rts


NormalName:	dc.b	'NormalLogin',0
QuickName:	dc.b	'QuickLogin',0
ShortName:	dc.b	'ShortLogin',0
_Selecting:	dc.b	'Selecting Login Mode..',0
_Prompt:	dc.b	"[36m([35mN[36m)[0mormal[33m, [36m([35mS[36m)[0mhortened [33mor [36m([35mQ[36m)[0muick Login [36m([35mN[36m/[35ms[36m/[35mq[36m)[0m? ",0
_ErrorTxt1:	dc.b	'Could not open dreamdoor.library V5+',10,0
_ErrorTxt2:	dc.b	'This program requires Windows 3.11+',10,0
_dreamname:	dc.b	'dreamdoor.library',0
_dosname:	dc.b	'dos.library',0
