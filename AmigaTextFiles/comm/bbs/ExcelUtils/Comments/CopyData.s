*INPUTS
*    A0 = String to copy
*    A1 = Buffer to copy into
*
*RESULTS
*    A0 = End of string copied
*    A1 = End of buffer to be copied to (after last character)
*
***************************************************************************
CopyData:	Moveq	#0,D0
.loop	Move.B	(A0)+,D0
	Cmp.B	#0,D0
	Beq.S	.stop
	Move.B	D0,(A1)+
	Bra.S	.loop
.stop	Rts
