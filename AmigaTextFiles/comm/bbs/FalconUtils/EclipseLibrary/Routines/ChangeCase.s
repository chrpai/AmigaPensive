* ChangeCase **************************************************************
*
* Converts a byte into Uppercase
*
*INPUTS
*    D0 = Byte to convert
*
*RESULTS
*    D0 = Uppercase version of byte supplied
*
***************************************************************************
ChangeCase:
	Cmp.B	#'a',D0
	Blo.S	.exit
	Cmp.B	#'z',D0
	Bls.S	.change
	Cmp.B	#$E0,D0
	Blo.S	.exit
	Cmp.B	#$FD,D0
	Bhi.S	.exit
	Cmp.B	#$F7,D0
	Beq.S	.exit
.change
	Sub.B	#32,D0
.exit
	Rts
