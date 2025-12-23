****************************************************************************
*                        SentBy for Logicbbs                              *
 *                       -------------------                             *
  *                    Written by  Michael Pendec                       *
   *                Copyright 1995 ParCon Software                     *
    *******************************************************************

;Source is originally written for AsmOne and will be compiled
; correctly with AsmOne. But other compilers may work just as well.


		INCDIR	source:includes/
		include	"lvo3.0/exec_lib.i"
		include	"lvo3.0/dos_lib.i"
		include	"lvo3.0/LBBS_Lvo_lib.i"

WriteTextScreen	MACRO
		move.l	MyNodePtr(pc),d1
		lea	\1(A4),a0
		move.l	a0,d2
		moveq	#0,d3
		CALLLBB	Write
		tst.l	d0
		bmi.w	FreeNode
		ENDM

GetInput		MACRO
		move.l	MyNodePtr(pc),d1
		lea	\1(A4),a0
		move.l	a0,d2
		move.l	#\2,d3
		CALLLBB	Read
		tst.l	d0
		bmi.w	FreeNode
		ENDM
GetInputNoCR	MACRO
		move.l	MyNodePtr(pc),d1
		lea	\1(A4),a0
		move.l	a0,d2
		move.l	#\2,d3
		CALLLBB	ReadNoCR
		tst.l	d0
		bmi.w	FreeNode
		ENDM

		basereg	Mya4ptr,a4

		section	SentBy,code

j		movem.l	d0-a6,-(A7)
		lea	mya4ptr,a4
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
		move.l	d7,d1
		beq.w	NoNode
		CALLLBB	AllocNode
		tst.l	d0
		bmi.w	Nonode
		move.l	d0,MyNodePtr(A4)


		move.l	MyNodePtr(A4),d1
		move.l	#buffer,d2
		moveq	#0,d3
		CALLLBB	GetMainLine
		bsr.w	TransMainline

		cmp.l	#'FCHE',buffer(a4)
		bne.w	WriteNewFileDesciption


		WriteTextScreen	MyText

		bsr.w	GetUsersSentByLine
		tst.l	d0
		bne.w	.nosentby
		tst.b	SentByString(a4)
		beq.w	.nosentby
		pea	SentByString(a4)
		move.w	#' ',-(a7)
		pea	sentbyLine(a4)
		pea	buffer(a4)
		bsr.w	Insert
		add.w	#14,a7
		lea	Directoryfile(A4),a0
		move.l	a0,d1
		move.l	#1005,d2
		CALLDOS	Open
		move.l	d0,-(A7)
		move.l	d0,d1
		moveq	#0,d2
		moveq	#1,d3
		CALLDOS	Seek
		lea	buffer(a4),a0
		move.l	a0,d2
		moveq	#-1,d3
.e		addq.l	#1,d3
		tst.b	(a0)+
		bne.s	.e
		move.l	(a7),d1
		CALLDOS	Write
		move.l	(A7)+,d1
		CALLDOS	Close

		WriteTextScreen	SentByLineAdded
		bra.s	FreeNode
.nosentby		WriteTextScreen	SentByLineError
FreeNode
.e444		move.l	MyNodePtr(A4),d1
		CALLLBB	FreeNode
NoNode		move.l	_LbbsBase(A4),a1
		CALLEXEC	CloseLibrary
exitall		move.l	OldStack(a4),a7
		movem.l	(a7)+,d0-a6
		rts


GetUsersSentByLine
		move.l	MyNodePtr(A4),d1
		CALLLBB	SlotNumber
		move.l	d0,-(a7)
		pea	SentByDirectory(a4)
		pea	buffer(a4)
		bsr.w	Insert
		add.w	#12,a7
		lea	buffer(a4),a0
		move.l	a0,d1
		move.l	#1005,d2
		CALLDOS	Open
		tst.l	d0
		beq.b	nosentby
		move.l	d0,d7
		lea	SentByString(a4),a0
		move.l	a0,d2
		move.l	#98,d3
		move.l	d0,d1
		CALLDOS	Read
		move.l	d7,d1
		CALLDOS	Close
		moveq	#0,d0
		dc.w	$c40
nosentby		moveq	#1,d0
		rts

PutUsersSentByLine	move.l	MyNodePtr(A4),d1
		CALLLBB	SlotNumber
		move.l	d0,-(a7)
		pea	SentByDirectory(a4)
		pea	buffer(a4)
		bsr.w	Insert
		add.w	#12,a7
		lea	buffer(a4),a0
		move.l	a0,d1
		move.l	#1006,d2
		CALLDOS	Open
		tst.l	d0
		beq.b	.nosentby
		move.l	d0,d7
		lea	SentByString(a4),a0
		move.l	a0,d2
		moveq	#-1,d3
.e		addq.l	#1,d3
		tst.b	(a0)+
		bne.s	.e
		move.l	d0,d1
		CALLDOS	Write
		move.l	d7,d1
		CALLDOS	Close
		moveq	#0,d0
		dc.w	$c40
.nosentby		moveq	#1,d0
		rts

WriteNewFileDesciption
		WriteTextScreen	MyText1

		bsr.w	GetUsersSentByLine
		tst.l	d0
		bne.w	.EnterNew
		WriteTextScreen	FoundSentBy
		WriteTextScreen	SentByString
		WriteTextScreen	UseThis
		move.l	MyNodePtr(A4),d1
		moveq	#0,d2
		CALLLBB	ReadOne
		move.l	d0,-(A7)
		move.b	d0,buffer(a4)
		clr.b	buffer+1(a4)
		WriteTextScreen	buffer
		move.l	(a7)+,d0
		bclr	#5,d0
		cmp.b	#'N',d0
		bne.w	FreeNode
.EnterNew		WriteTextScreen	EnterNewSentby
		GetInput	SentByString,44
		tst.b	SentByString(a4)
		beq.w	FreeNode
		bsr.w	PutUsersSentByLine
		bra.w	FreeNode

MyText
 dc.b 10
 dc.b $1b,'[4;33;44m SeNTBy LiNe aDDeR FoR ® LoGiCBBS ',$1b,'[0m',10,0
MyText1
 dc.b 10
 dc.b $1b,'[4;33;44m  SeNTBy LiNe MaKeR/CHaNGeR FoR ® LoGiCBBS  ',$1b,'[0m',10,0
 even

EnterNewSentby	dc.b	10
		dc.b	10,$1b,'[36mPLeaSe eNTeR youR PeRSoNaL SeNTBy ',$1b,'[35m(',$1b,'[36m44 CHARS',$1b,'[35m)',$1b,'[0m',10,10
		dc.b	$1b,'[33m············································',$1b,'[0m',$1b,'[44D',0
FoundSentBy	dc.b	10
		dc.b	$1b,'[36mU HaVe aLLReaDy a PeRSoNaL SeNTBy ',$1b,'[35m(',$1b,'[36m44 CHARS',$1b,'[35m)',$1b,'[0m',10,10,0
UseThis		dc.b	10
		dc.b	10,$1b,'[36mWaNNa KeeP iT? ',$1b,'[35m(',$1b,'[36mY/n',$1b,'[35m)',$1b,'[0m',0

SentByLineAdded	dc.b	10,$1b,'[36mSeNTBy LiNe aDDeD',$1b,'[0m',10,0
SentByLineError	dc.b	10,$1b,'[33mNO',$1b,'[36m SeNTBy LiNe FoUND, TyPe ',$1b,'[33;44m SENT ',$1b,'[36;40m To MaKe oNe',10,0
SentByDirectory	dc.b	'DOORS:LBBS_Sentby/Sentby.%ld',0
SentByLine	dc.b	'%33Y%-44s',10,0
		even

TransMainline	lea	buffer(A4),a0
.e		cmp.b	#' ',(a0)+
		beq.s	.chk
		tst.b	(a0)
		bne.s	.e
		bra.s	.getall
.chk		tst.b	(a0)
		beq.s	.getall
		cmp.b	#' ',(a0)
		beq.s	.e
		lea	CheckedFilename(A4),a1
.ee		cmp.b	#' ',(a0)
		beq.s	.chk1
		move.b	(a0)+,(A1)+
		bne.s	.ee
		bra.s	.getall1
.chk1		addq.w	#1,a0
		clr.b	(a1)
		cmp.b	#' ',(a0)
		beq.s	.chk1
		tst.b	(a0)
		beq.s	.getall1
		lea	Directoryfile(A4),a1
.e1e		cmp.b	#' ',(a0)
		beq.s	.chka1
		move.b	(a0)+,(A1)+
		bne.s	.e1e
.chka1		moveq	#2,d0
		rts
.getall1		moveq	#1,d0
		dc.w	$c40
.getall		moveq	#0,d0
		rts

INSERT		lea	Workinsert(A4),a5
		MOVEM.L	4(A7),A2-A3
		move.b	#' ',(a5)
		LEA	12(A7),A0
		MOVE.L	A0,-(A7)
C660:		MOVE.B	(A3)+,D0
		BEQ.S	C66E
		CMP.B	#'%',D0
		BEQ.S	C674
C66A:		MOVE.B	D0,(A2)+
		BRA.S	C660
C66E:		ADDQ.W	#4,SP
		MOVE.B	D0,(A2)+
		RTS
C674:		lea	2(a5),a1
		CLR.W	D3
		cmp.b	#'%',(a3)
		beq.s	C66A
		cmp.b	#' ',(a3)
		beq.s	C66A
		cmp.b	#9,(a3)
		beq.s	C66A
		cmp.b	#10,(a3)
		beq.s	C66A
		CMP.B	#'@',(A3)
		BNE.S	.r1
		BSET	#4,D3
		ADDQ.W	#1,A3
.r1		CMP.B	#',',(A3)
		BNE.S	.r
		BSET	#3,D3
		ADDQ.W	#1,A3

.r		CMP.B	#'-',(A3)
		BNE.S	C686
		BSET	#0,D3
		ADDQ.W	#1,A3

C686:		CMP.B	#'0',(A3)
		BNE.S	C690
		BSET	#1,D3
C690:
		BSR.W	C768
		MOVE.W	D0,D6
		moveq	#0,d5
		CMP.B	#'.',(A3)
		BNE.S	C6A6
		ADDQ.W	#1,A3
		BSR.W	C768
		MOVE.W	D0,D5
C6A6		CMP.B	#'Y',(A3)
		BNE.S	.gh
		ADDQ.W	#1,A3
		BSR.S	C6D2
		MOVE.B	D4,(A5)
		clr.b	2(a5)
		BRA.B	C70A
.gh		CMP.B	#'u',(A3)
		BNE.S	.l
		BSET	#5,D3
		BSET	#2,D3
		ADDQ.W	#1,A3
		BSR.S	C6D0
		BSR.W	CONVtoDEC
		BRA.B	C70A
.l		CMP.B	#'l',(A3)
		BNE.S	C6B2
		BSET	#2,D3
		ADDQ.W	#1,A3
C6B2:		MOVE.B	(A3)+,D0
		CMP.B	#'d',D0
		BNE.S	C6C2
		BSR.S	C6D0
		BSR.W	CONVtoDEC
		BRA.B	C70A
C6C2:		CMP.B	#'x',D0
		BNE.S	C6F0
		BSR.S	C6D0
		BSR.W	C808
		BRA.B	C70A
C6D0:		BTST	#2,D3
		BNE.S	C6E4
C6D2		MOVE.L	4(A7),A0
		MOVE.W	(A0)+,D4
		MOVE.L	A0,4(A7)
		EXT.L	D4
		RTS
C6E4:		MOVE.L	4(A7),A0
		MOVE.L	(A0)+,D4
		MOVE.L	A0,4(A7)
		RTS
C6F0		CMP.B	#'s',D0
		BNE.S	C6FE
		MOVE.L	(A7),A0
		MOVE.L	(A0)+,A1
		MOVE.L	A0,(A7)
		BRA.S	C710
C6FE:		CMP.B	#'c',D0
		BNE.W	C66A
		BSR.S	C6D2
		MOVE.B	D4,(A1)+
		CLR.B	(A1)
C70A:		lea	2(a5),a1
C710:		MOVE.L	A1,A0
		BSR.S	C75A
		TST.W	D5
		BEQ.S	C71C
		CMP.W	D5,D2
		BHI.S	C71E

C71C		MOVE.W	D2,D5
C71E		btst	#4,d3
		beq.s	C71C1
		move.w	d6,d5
		sub.w	d2,d6
		bpl.s	C724
		CLR.W	D6
		BRA.S	C724
C71C1		SUB.W	D5,D6
		BPL.S	C724
		CLR.W	D6
C724:		BTST	#0,D3
		BNE.S	C730
		BSR.S	C742
		BRA.S	C730
C72E		tst.b	(a1)
		beq.s	C731
		MOVE.B	(A1)+,(A2)+
C730		DBRA	D5,C72E
C731		clr.b	(A2)
		BTST	#0,D3
		BEQ.W	C660
		BSR.S	C742
		BRA.W	C660
C742:		MOVE.B	(a5),D2
		BTST	#1,D3
		BEQ.S	C754
		MOVE.B	#$30,D2
		BRA.S	C754
C752:		MOVE.B	D2,(A2)+
C754:		DBRA	D6,C752
		RTS
C75A:		MOVEQ	#-1,D2
C75C:		TST.B	(A0)+
		DBEQ	D2,C75C
		NEG.L	D2
		SUBQ.W	#1,D2
		RTS
C768:		moveq	#0,d0
		moveq	#0,d2
C76C:		MOVE.B	(A3)+,D2
		CMP.B	#$30,D2
		BCS.S	C78C
		CMP.B	#$39,D2
		BHI.S	C78C
		ADD.L	D0,D0
		MOVE.L	D0,D1
		lsl.l	#2,d0
		ADD.L	D1,D0
		SUB.B	#$30,D2
		ADD.L	D2,D0
		BRA.S	C76C
C78C:		SUBQ.W	#1,A3
		RTS


CONVtoDEC	btst	#5,d3
		beq.s	CONVtoDEC1
		tst.l	d4
		beq.s	.l55
		moveq	#2,d7
		LEA	L7D0(PC),A0
		moveq	#0,d5
.l1		subq.l	#1,d7
		MOVE.L	(A0)+,D1
		BEQ.S	.l55
		move.l	d4,d0
		beq.s	.l44
		bsr.w	_divu
		move.l	d1,d4
.l44		btst	#3,d3
		beq.s	.t
		tst.w	d7
		bne.s	.t
		moveq	#3,d7
		tst.w	d5
		beq.s	.t
		MOVE.B	#',',(A1)+
.t		tst.w	d0
		BNE.S	.l2
		TST.W	D5
		BEQ.S	.l1
.l2		MOVEQ	#-1,D5
		ADD.B	#$30,D0
		MOVE.B	D0,(A1)+
		clr.b	(A1)
		BRA.S	.l1
.l55		ADD.B	#$30,D4
		MOVE.B	D4,(A1)+
		clr.b	(A1)
		RTS

CONVtoDEC1	moveq	#2,d7
		TST.L	D4
		BEQ.S	C7C6
		BMI.S	C79A
		NEG.L	D4
		BRA.S	C79E
C79A		MOVE.B	#'-',(A1)+
C79E		LEA	L7D0(PC),A0
		CLR.W	D1
C7A4:		subq.l	#1,d7
		MOVE.L	(A0)+,D2
		BEQ.S	C7C6
		MOVEQ	#-1,D0
C7AA:		ADD.L	D2,D4
		DBGT	D0,C7AA
		SUB.L	D2,D4
		btst	#3,d3
		beq.s	.t
		tst.w	d7
		bne.s	.t
		moveq	#3,d7
		tst.w	d1
		beq.s	.t
		MOVE.B	#',',(A1)+
.t		ADDQ.W	#1,D0
		BNE.S	C7BA
		TST.W	D1
		BEQ.S	C7A4
C7BA:		MOVEQ	#-1,D1
		NEG.B	D0
		ADD.B	#$30,D0
		MOVE.B	D0,(A1)+
		BRA.S	C7A4
C7C6:		NEG.B	D4
		ADD.B	#$30,D4
		MOVE.B	D4,(A1)+
		clr.b	(A1)
		RTS

L7D0:		dc.l	1000000000
		dc.l	100000000	;,
		dc.l	10000000
		dc.l	1000000
		dc.l	100000		;,
		dc.l	10000
		dc.l	1000	
		dc.l	100		;,
		dc.l	10
		dc.l	0


C808:		TST.L	D4
		BEQ.S	C7C6
		CLR.W	D1
		BTST	#2,D3
		BNE.S	C81A
		MOVEQ	#3,D2
		SWAP	D4
		BRA.S	C81C
C81A:		MOVEQ	#7,D2
C81C:		ROL.L	#4,D4
		MOVE.B	D4,D0
		AND.B	#$F,D0
		BNE.S	C82A
		TST.W	D1
		BEQ.S	C83E
C82A:		MOVEQ	#-1,D1
		CMP.B	#9,D0
		BHI.S	C838
		ADD.B	#$30,D0
		BRA.S	C83C
C838:		ADD.B	#$37,D0
C83C:		MOVE.B	D0,(A1)+
C83E:		DBRA	D2,C81C
		clr.b	(a1)
		RTS

; D0 = D0 * D1
_mulu	movem.l	D1-D3,-(SP)
	move.w	D1,D2
	mulu	D0,D2
	move.l	D1,D3
	swap	D3
	mulu	D0,D3
	swap	D3
	clr.w	D3
	add.l	D3,D2
	swap	D0
	mulu	D1,D0
	swap	D0
	clr.w	D0
	add.l	D2,D0
	movem.l	(SP)+,D1-D3
	rts


; D0.L = D0.L / D1.L
_divs	movem.l	D1/D4,-(SP)
	tst.l	d0
	beq.s	divs_exit
	clr.l	D4
	tst.l	D0
	bpl.s	.v1
	neg.l	D0
	addq.w	#1,D4
.v1	tst.l	D1
	bpl.s	.v2
	neg.l	D1
	eor.w	#1,D4
.v2	bsr.s	comdivide
divs_chksign	tst.w	D4
	beq.s	divs_exit
	neg.l	D0
divs_exit	movem.l	(SP)+,D1/D4
	tst.l	D0
	rts

; D0.L = D0.L MOD D1.L signed
_mods	movem.l	D1/D4,-(SP)
	clr.l	D4
	tst.l	D0
	bpl.s	.v1
	neg.l	D0
	addq.w	#1,D4
.v1	tst.l	D1
	bpl.s	.b2
	neg.l	D1
.b2	bsr.s	comdivide
	move.l	D1,D0
	bra.s	divs_chksign

; D0.L = D0.L MOD D1.L unsigned
_modu	move.l	D1,-(SP)
	bsr.s	comdivide
	move.l	D1,D0
	move.l	(SP)+,D1
	tst.l	D0
	rts

; D0.L = D0.L / D1.L unsigned
_divu	tst.l	d0
	beq.s	.ww
	bsr.s	comdivide
	tst.l	D0
	rts
.ww	moveq	#0,d0
	moveq	#0,d1
	rts

comdivide	movem.l	D2/D3,-(SP)
	swap	D1
	tst.w	D1
	bne.s	.hardldv
	swap	D1
	move.w	D1,D3
	move.w	D0,D2
	clr.w	D0
	swap	D0
	divu	D3,D0
	move.l	D0,D1
	swap	D0
	move.w	D2,D1
	divu	D3,D1
	move.w	D1,D0
	clr.w	D1
	swap	D1
	movem.l	(SP)+,D2/D3
	rts

.hardldv	swap	D1
	move.l	D1,D3
	move.l	D0,D1
	clr.w	D1
	swap	D1
	swap	D0
	clr.w	D0
	moveq	#16-1,D2
.loop	add.l	D0,D0
	addx.l	D1,D1
	cmp.l	D1,D3
	bhi.s	.b1
	sub.l	D3,D1
	addq.w	#1,D0
.b1	dbra	D2,.loop
	movem.l	(SP)+,D2/D3
	rts



Get_Number	moveq	#0,d7
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
		bsr.w	_Mulu
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
OldStack	ds.l	1
MyNodePtr	ds.l	1
_LbbsBase	ds.l	1
_DosBase	ds.l	1
CheckedFilename	ds.b	300
Directoryfile	ds.b	300
SentByString	ds.b	100
Workinsert	ds.b	400
buffer		ds.b	1000
