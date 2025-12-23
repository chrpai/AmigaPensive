* ShortDay ****************************************************************
*
* Converts a DateStamp into an ASCII string containing the day of the date.
*
*INPUTS
*    A0 = Pointer to buffer for DayString.  3 bytes long
*    A1 = Pointer to a DOS DateStamp (3 Longwords)
*
*RESULTS
*    A0 = End of Day string (after last character)
*
***************************************************************************
ShortDay:
	Move.L	(A1),D0
	Divu	#7,D0
	Clr.W	D0
	Swap	D0

	Lea	ShortDayNames(PC),A1
	Mulu	#3,D0
	Add.L	D0,A1
	Move.B	(A1)+,(A0)+
	Move.B	(A1)+,(A0)+
	Move.B	(A1),(A0)+
	Rts

ShortDayNames:	Dc.B	'SunMonTueWedThuFriSat'
	Even
