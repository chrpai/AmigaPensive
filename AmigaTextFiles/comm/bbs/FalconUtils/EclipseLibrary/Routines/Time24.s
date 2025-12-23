* Time24 ******************************************************************
*
* Converts a DateStamp into an ASCII string containing the time of the date.
* In 24 hour mode
*
*INPUTS
*    A0 = Pointer to buffer for Time String.  5 bytes long
*    A1 = Pointer to a DOS DateStamp (3 Longwords)
*
*RESULTS
*    A0 = Enf of Time string (after last charcter)
*
***************************************************************************
Time24:
	Move.L	4(A1),D0

	Divu.W	#60,D0
	Swap	D0
	Clr.W	D0
	Swap	D0

	Divu	#10,D0
	Add.B	#'0',D0
	Move.B	D0,(A0)+
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
	Rts
