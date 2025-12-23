* Date ********************************************************************
*
* Converts a DateStamp into an ASCII string containing the date.
*
*INPUTS
*    A0 = Pointer to buffer for Time String.  9 bytes long
*    A1 = Pointer to a DOS DateStamp (3 Longwords)
*
*RESULTS
*    A1 = End of string (character after last number)
*
***************************************************************************
Date:
	Movem.L	D2/D3,-(SP)
	Move.L	A0,_TempDatePoint
	Lea	TempDateSpace(PC),A0
	Move.L	(A1),D0
	Addq	#1,D0
	Moveq	#2,D1
	Moveq	#78,D2
.Loop
	Move.L	#365,D3
	And.B	#3,D1
	Bne.S	.NoLeap
	Addq	#1,D3
.NoLeap
	Cmp.L	D3,D0
	Ble.S	.YearFound
	Sub.L	D3,D0
	Addq	#1,D2
	Addq	#1,D1
	Bra.S	.Loop
.YearFound
	Cmp.W	#100,D2
	Blt.S	.NoNewCent
	Sub.W	#100,D2
.NoNewCent
	Divu	#10,D2
	Add.B	#'0',D2
	Move.B	D2,7(A0)
	Swap	D2
	Add.B	#'0',D2
	Move.B	D2,8(A0)

	Lea	NormalMonths(PC),A1
	And.W	#3,D1
	Bne.S	.NoLeap2
	Move.W	#29,2(A1)
.NoLeap2
	Moveq	#0,D1
.Loop2
	Cmp.W	(A1),D0
	Ble.S	.MonthFound
	Sub.W	(A1)+,D0
	Addq	#1,D1
	Bra.S	.Loop2
.MonthFound
	Lea	MonthNames(PC),A1
	Mulu	#3,D1
	Add.L	D1,A1
	Move.B	(A1)+,3(A0)
	Move.B	(A1)+,4(A0)
	Move.B	(A1),5(A0)
	Move.B	#' ',6(A0)

	Divu	#10,D0
	Add.B	#'0',D0
	Cmp.B	#'0',D0
	Bne.S	.Not0
	Move.B	#' ',D0
.Not0
	Move.B	D0,(A0)
	Swap	D0
	Add.B	#'0',D0
	Move.B	D0,1(A0)
	Move.B	#' ',2(A0)
	Movem.L	(SP)+,D2/D3
	Lea	TempDateSpace(PC),A0
	Move.L	_TempDatePoint(PC),A1
	Cmp.B	#' ',(A0)
	Bne.s	.CopyOk
	Addq	#1,A0
.CopyOk
	Moveq	#10,D0
	Moveq	#0,D1
	Bsr	CopyDataExclude
	Rts

TempDateSpace:	Ds.B	10
MonthNames:	Dc.B	'JanFebMarAprMayJunJulAugSepOctNovDec'
	Even
NormalMonths:	Dc.W	31,28,31,30,31,30,31,31,30,31,30,31
_TempDatePoint:	Dc.L	0
