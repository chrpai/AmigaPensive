* ConvertToDecimal ********************************************************
*
* Converts a ASCII string into a word
*
*INPUTS
*    A0 = Pointer to string holding number to be converted
*
*RESULTS
*    D0 = Number (word)
*
***************************************************************************
ConvertToDecimal:
	Move.L	D2,-(SP)
	Moveq	#3,D0
	Moveq	#0,D1
	Moveq	#0,D2
.NumberLoop
	Move.B	(A0)+,D1
	Cmp.B	#'0',D1
	Blt.S	.AddEnd
	Cmp.B	#'9',D1
	Bgt.S	.AddEnd
	Lsl.L	#4,D2
	Sub.B	#48,D1
	Add.L	D1,D2
	DBra	D0,.NumberLoop
.AddEnd
	Moveq	#0,D0
	Tst.L	D2
	Beq.S	.Exit
	Moveq	#0,D1

	Move.W	D2,D1
	And.W	#$F000,D1
	Lsr.L	#8,D1
	Lsr.L	#4,D1
	Mulu.W	#1000,D1
	Add.W	D1,D0
	Move.W	D2,D1
	And.W	#$0F00,D1
	Lsr.L	#8,D1
	Mulu.W	#100,D1
	Add.W	D1,D0
	Move.W	D2,D1
	And.W	#$00F0,D1
	Lsr.L	#4,D1
	Mulu.W	#10,D1
	Add.W	D1,D0
	Move.W	D2,D1
	And.W	#$000F,D1
	Add.W	D1,D0
.Exit
	Move.L	(SP)+,D2
	Rts
