* GetFileSize *************************************************************
*
* Examines a file, and returns it's size in bytes
*
*INPUTS
*    D1 = Name/Path of file to examine
*
*RESULTS
*    D0 = Size of file  or  NULL if file couldn't be found
*
***************************************************************************
GetFileSize:
	Movem.L	D2/A6,-(SP)
	Moveq	#ACCESS_READ,D2
	CALLDOS	Lock
	Tst.L	D0
	Beq.S	.NoFile
	Move.L	D0,-(SP)
	Move.L	D0,D1
	Move.L	#ReadFileInfo,D2
	CALL	Examine

	Move.L	(SP)+,D1
	CALL	UnLock

	Lea	ReadFileInfo(PC),A0
	Move.L	124(A0),D0
	Movem.L	(SP)+,D2/A6
	Rts

.NoFile
	Moveq	#0,D0
	Movem.L	(SP)+,D2/A6
	Rts
