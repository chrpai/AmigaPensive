***************************************************************************
* eclipse.library/FindData
*
*
* Hunts through memory starting at A1, for the null terminated string in A0
*
*INPUTS
*    A0 = Source data   (Null terminated)
*    A1 = Data to search
*
*RESULTS
*    D0 =
*         0 found
*        -1 not found
*    A0 = Pointer to start of found string
*    A1 = Pointer to end of found string
*
***************************************************************************
FindData:
	Movem.L	A2/A3,-(SP)
.FindDataLoop
	Moveq	#0,D0
	Moveq	#0,D1
	Move.B	(A0),D0
	Bsr.S	ChangeCase
	Move.B	D0,D1
	Move.B	(A1)+,D0
	Tst.B	D0
	Beq.S	.Exit
	Bsr.S	ChangeCase
	Cmp.B	D0,D1
	Bne.S	.FindDataLoop

	Lea	1(A0),A2
	Move.L	A1,A3
.FindLoop
	Move.B	(A2)+,D0
	Tst.B	D0
	Beq.S	.Found
	Bsr.S	ChangeCase
	Move.B	D0,D1
	Move.B	(A3)+,D0
	Bsr.S	ChangeCase
	Cmp.B	D0,D1
	Bne.S	.FindDataLoop
	Bra.S	.FindLoop
.Exit
	Moveq	#-1,D0
	Movem.L	(SP)+,A2/A3
	Rts
.Found
	Lea	-1(A1),A0
	Move.L	A3,A1
	Moveq	#0,D0
	Movem.L	(SP)+,A2/A3
	Rts
