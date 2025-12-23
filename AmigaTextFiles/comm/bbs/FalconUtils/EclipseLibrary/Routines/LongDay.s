* LongDay *****************************************************************
*
* Converts a DateStamp into an ASCII string containing the day of the date.
*
*INPUTS
*    A0 = Pointer to buffer for DayString.  8 bytes long
*    A1 = Pointer to a DOS DateStamp (3 Longwords)
*
*RESULTS
*    A1 = End of Day String (after last character)
*
***************************************************************************
LongDay:
	Move.L	(A1),D0
	Divu	#7,D0
	Clr.W	D0
	Swap	D0

	Move.L	A0,D1
	Lea	DayNames(PC),A1
	Mulu	#4,D0
	Add.L	D0,A1
	Move.L	(A1),A0
	Move.L	D1,A1
	Moveq	#8,D0
	Bsr	CopyData
	Rts

DayNames:	Dc.L	Sunday
	Dc.L	Monday
	Dc.L	Tuesday
	Dc.L	Wednsday
	Dc.L	Thursday
	Dc.L	Friday
	Dc.L	Saturday

Sunday:	Dc.B	'Sunday',0
Monday:	Dc.B	'Monday',0
Tuesday:	Dc.B	'Tuesday',0
Wednsday:	Dc.B	'Wednsday',0
Thursday:	Dc.B	'Thursday',0
Friday:	Dc.B	'Friday',0
Saturday:	Dc.B	'Saturday',0
	Even
