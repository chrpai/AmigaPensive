*INPUTS
*	A0	Filename to load
*	A1	Mem start space
*	A2	Mem size space
*
*RESULTS
*	D0	Zero for success or -1 for failure
*
*********************************************************************
LoadFile:	Moveq	#-1,D6		fail at value
	Move.L	A1,A3		swap mem start

	Move.L	A0,D1
	Move.L	#MODE_OLDFILE,D2
	CALLPCDOS	Open
	Move.L	D0,A4
	Tst.L	D0
	Beq.S	.FailedOpening

	Move.L	D0,D1
	Moveq	#0,D2
	Moveq	#OFFSET_END,D3
	CALL	Seek
	Cmp.L	#-1,D0
	Beq.S	.FailedSeek

	Move.L	A4,D1
	Moveq	#0,D2
	Moveq	#OFFSET_BEGINNING,D3
	CALL	Seek
	Cmp.L	#-1,D0
	Beq.S	.FailedSeek
	Move.L	D0,(A2)

	Moveq	#0,D1
	CALLPCEXEC	AllocMem
	Move.L	D0,(A3)
	Tst.L	D0
	Beq.S	.FailedSeek

	Move.L	A4,D1		file
	Move.L	(A3),D2		buffer
	Move.L	(A2),D3		length
	CALLPCDOS	Read
	Move.L	(A2),D1
	Sub.L	D0,D1
	Bne.S	.FailedLoading

	Moveq	#0,D6
	Bra.S	.FailedSeek

.FailedLoading	Move.L	(A3),A1
	Move.L	(A2),D0
	CALLPCEXEC	FreeMem

.FailedSeek	Move.L	A4,D1
	CALLPCDOS	Close

.FailedOpening	Move.L	D6,D0
	Rts
