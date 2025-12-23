		XDEF 	_ExamineCfg

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

		END

