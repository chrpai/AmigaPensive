		XDEF 	_ExamineCfg
		XDEF	_Fnd

		section kala,code

_ExamineCfg	Move.L	4(SP),A0
EC_Notyet	Move.L	8(SP),A1
		Tst.B	(A0)
		Beq	EC_Notfnd
		Cmp.B	#';',(A0)
		Beq	EC_Halfdot
		Cmpm.B	(A0)+,(A1)+
		Bne	EC_Notyet
EC_Contsearch	Tst.B	(A1)
		Beq	EC_Fnd
		Cmpm.B	(A0)+,(A1)+
		Beq	EC_Contsearch
		Bra	EC_Notyet
EC_Notfnd	Moveq	#0,D0
		Rts
EC_Fnd		Move.L	A0,D0
		Rts
EC_Halfdot	Cmp.B	#10,(A0)+
		Beq	EC_Notyet
		Tst.B	(A0)
		Beq	EC_Notfnd
		Bra	EC_Halfdot

_Fnd		Move.L	4(SP),A0
		Move.L	8(SP),A1
		Movem.L	A1-A2,-(SP)
		Moveq	#0,D0
		Move.L	A0,A2
.loop		Move.L	A2,A0
		Tst.B	(A1)
		Beq.s	.notfnd
		Move.L	A1,D1
		Cmp.B	#10,(A1)
		Bne.s	.looptro
		Cmp.B	#'#',1(A1)
		Beq.s	.Comment
.looptro	Tst.B	(A0)
		Beq.s	.found
		Move.B	(A1),D0
		Cmp.B	(A0),D0
		Bne.s	.notyet
		Addq.L	#1,A0
		Addq.L	#1,A1
		Bra	.looptro
.notyet		Cmp.B	#96,D0
		Bcs.s	.notyet2
		Sub.B	#32,D0
		Cmp.B	(A0),D0
		Bne.s	.notyet2
		Addq.L	#1,A0
		Addq.L	#1,A1
		Bra	.looptro
.notyet2	Addq.L	#1,A1
		Bra	.loop
.notfnd		Move.L	A1,A0
		Moveq	#0,D0
		Movem.L	(SP)+,A1-A2
		Rts
.found		Move.L	D1,D0
		Movem.L	(SP)+,A1-A2
		Rts
.Comment	Cmp.B	#10,(A1)+
		Bne	.Comment
		Bra	.loop

		END

