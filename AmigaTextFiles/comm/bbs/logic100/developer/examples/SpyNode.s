****************************************************************************
*                        SpyNode for Logicbbs                             *
 *                       --------------------                            *
  *                    Written by  Michael Pendec                       *
   *                Copyright 1995 ParCon Software                     *
    *******************************************************************

;Source is originally written for AsmOne and will be compiled
; correctly with AsmOne. But other compilers may work just as well.


		INCDIR	"source:includes/"
		include	"lvo3.0/exec_lib.i"
		include	"lvo3.0/dos_lib.i"
		include	"lvo3.0/LBBS_Lvo_lib.i"

WriteTextScreen	MACRO
		move.l	MyNodePtr(pc),d1
		lea	\1(A4),a0
		move.l	a0,d2
		moveq	#0,d3
		moveq	#0,d4
		CALLLBB	SendTextConsole
		tst.l	d0
		bmi.w	FreeNode
		ENDM

WriteTextA0	MACRO
		move.l	MyNodePtr(pc),d1
		move.l	a0,d2
		moveq	#0,d3
		moveq	#0,d4
		CALLLBB	SendTextConsole
		tst.l	d0
		bmi.w	FreeNode
		ENDM

		basereg	Mya4ptr,a4

		section	SpyNode,code

j		movem.l	d0-a6,-(A7)
		lea	mya4ptr,a4
		lea	bss1(a4),a2
		lea	bss2(a4),a3
.e		clr.w	(a2)+
		cmp.l	a3,a2
		bne.s	.e
		move.l	a7,OldStack(a4)
		pea	(a0)
		lea	doslib(a4),a1
		moveq	#0,d0
		CALLEXEC	OpenLibrary
		move.l	d0,_DosBase(a4)
		lea	LibraryName(a4),a1
		moveq	#0,d0
		CALLEXEC	OpenLibrary
		move.l	(a7)+,a0
		tst.l	d0
		beq.w	exitall
		move.l	d0,_LbbsBase(A4)

		bsr.w	Get_Number
		move.l	a0,TypeMe(a4)

		move.l	d7,d1
		beq.w	NoNode
		CALLLBB	SpyNode
		tst.l	d0
		bmi.w	Nonode
		move.l	d0,MyNodePtr(A4)

		move.l	TypeMe(a4),a0
		WriteTextA0


FreeNode
.e444		move.l	MyNodePtr(A4),d1
		CALLLBB	FreeSpy
NoNode		move.l	_LbbsBase(A4),a1
		CALLEXEC	CloseLibrary
exitall		move.l	OldStack(a4),a7
		movem.l	(a7)+,d0-a6
		rts

Get_Number		moveq	#0,d7
		moveq	#0,d6
.t2		moveq	#0,d0
		move.b	(a0)+,d0
		beq.s	.a
		cmp.w	#$20,d0
		beq.s	.t2
		cmp.w	#9,d0
		beq.s	.t2
		cmp.w	#10,d0
		beq.s	.r4
		cmp.w	#'-',d0
		bne.s	.r2a
		moveq	#1,d6
.r3		moveq	#0,d0
		move.b	(a0)+,d0
.r2a		cmp.w	#' ',d0
		beq.s	.r5
		cmp.w	#9,d0
		beq.s	.l1
		cmp.w	#';',d0
		beq.s	.r5
		cmp.w	#10,d0
		beq.s	.r4
		cmp.w	#'0',d0
		blt.s	.r4
		cmp.w	#'9',d0
		bgt.s	.r4
.r2		move.l	d0,-(A7)
		moveq	#10,d1
		move.l	d7,d0
		mulu	d1,d0
		move.l	d0,d7
		move.l	(a7)+,d0
		and.w	#$f,d0
		add.l	d0,d7
		bra.s	.r3
.r4		tst.w	d6
		beq.s	.l1
		neg.l	d7
.l1		moveq	#0,d0
		rts
.a		moveq	#1,d0
		rts
.r5		bra.s	.r4

Mya4ptr
Doslib		dc.b	'dos.library',0
LibraryName	dc.b	'lbbs.library',0
		even
bss1
TypeMe			ds.l	1
OldStack		ds.l	1
MyNodePtr		ds.l	1
_LbbsBase		ds.l	1
_DosBase		ds.l	1
bss2
