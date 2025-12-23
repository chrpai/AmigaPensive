* CopyDataExclude *********************************************************
*
* Copys a string from one buffer to another while excluding a byte
*
*INPUTS
*    A0 = String to copy
*    A1 = Buffer to copy into
*    D0 = Number of characters to copy
*    D1 = Character to exclude
*
*RESULTS
*    A0 = End of string copied
*    A1 = End of buffer to be copied to (after last character)
*
*NOTES
*    Will not copy quote (") marks.  Will stop copying if a CR (ASCII 10)
*    or a Null (ASCII 0) is found in the source string.
*
***************************************************************************
CopyDataExclude:
	Move.L	D2,-(SP)
	Moveq	#0,D2
	Subq	#1,D0
.loop
	Move.B	(A0)+,D2
	Cmp.B	#0,D2
	Beq.S	.stop
	Cmp.B	#10,D2
	Beq.S	.stop
	Cmp.B	#'"',D2
	Beq.S	.skip
	Cmp.B	D1,D2
	Beq.S	.skip
	Move.B	D2,(A1)+
.skip
	DBra	D0,.loop
.stop
	Move.L	(SP)+,D2
	Rts
