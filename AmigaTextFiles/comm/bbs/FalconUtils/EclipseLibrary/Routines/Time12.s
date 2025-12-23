* Time12 ******************************************************************
*
* Converts a DateStamp into an ASCII string containing the time of the date.
* In 12 hour mode (am/pm)
*
*INPUTS
*    A0 = Pointer to buffer for Time String.  7 bytes long
*    A1 = Pointer to a DOS DateStamp (3 Longwords)
*
*RESULTS
*    A0 = End of Time string (after last character)
*
***************************************************************************
Time12:
	Move.L	4(A1),D0

	Divu.W	#60,D0
	Swap	D0
	Clr.W	D0
	Swap	D0

	Cmp.B	#12,D0
	Blt.S	.NoSub
	Sub.B	#12,D0
	Moveq	#1,D1
.NoSub
	Cmp.B	#0,D0
	Bne.S	.No0
	Moveq	#12,D0
.No0
	Divu	#10,D0
	Add.B	#'0',D0
	Cmp.B	#'0',D0
	Beq.S	.Ok
	Move.B	D0,(A0)+
.Ok
	Swap	D0
	Add.B	#'0',D0
	Move.B	D0,(A0)+

	Move.B	#':',(A0)+

	Move.L	4(A1),D0
	Divu.W	#60,D0
	Clr.W	D0
	Swap	D0

	Divu	#10,D0
	Add.B	#'0',D0
	Move.B	D0,(A0)+
	Swap	D0
	Add.B	#'0',D0
	Move.B	D0,(A0)+

	Move.B	#'a',(A0)+
	Cmp.B	#1,D1
	Bne.S	.morn
	Move.B	#'p',-1(A0)
.morn
	Move.B	#'m',(A0)+
	Rts
