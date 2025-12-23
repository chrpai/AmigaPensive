* GetFile *****************************************************************
*
* Examines a file, and returns a pointer to it's handle and info
*
*INPUTS
*    D1 = Name/Path of file to examine
*
*RESULTS
*    D0 = Handle of file
*    D1 = Pointer to FileInfo Buffer
*
***************************************************************************
GetFile:
	Movem.L	D2/A6,-(SP)
	Move.L	D1,GetNameData
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

	Move.L	GetNameData(PC),D1
	Move.L	#MODE_READWRITE,D2
	CALL	Open
	Tst.L	D0
	Beq.S	.NoFile
	Move.L	#ReadFileInfo,D1
	Movem.L	(SP)+,D2/A6
	Rts

.NoFile
	Moveq	#0,D0
	Moveq	#0,D1
	Movem.L	(SP)+,D2/A6
	Rts

GetNameData:	Dc.L	0
