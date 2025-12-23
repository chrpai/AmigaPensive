* CompareStrings **********************************************************
*
* Compares two strings (Case insensitive)
*
*INPUTS
*    A0 = String 1
*    A1 = String 2
*
*RESULTS
*    D0 =
*         0  String 1 = String 2
*        -1  String 1 ~= String 2
*
***************************************************************************
CompareStrings:
	Move.B	(A0)+,D0
	Tst.B	D0
	Beq.S	.found
	Bsr.S	ChangeCase
	Move.B	D0,D1
	Move.B	(A1)+,D0
	Bsr.S	ChangeCase
	Cmp.B	D0,D1
	Bne.S	.no
	Tst.B	D0
	Bne.S	CompareStrings
.found
	Moveq	#0,D0
	Rts
.no
	Moveq	#-1,D0
	Rts
