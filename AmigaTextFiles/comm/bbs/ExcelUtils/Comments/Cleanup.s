*********************************************************************
Cleanup:	Lea	ItemDataName(PC),A0
	Move.L	_FileMarker(PC),A1
	Bsr	CopyData
	Move.B	#0,(A1)

	Lea	FileArea(PC),A0
	Lea	_ItemStart(PC),A1
	Lea	_ItemSize(PC),A2
	Bsr	LoadFile
	Bne	.FailedItem

	Lea	CommentsName(PC),A0
	Move.L	_FileMarker(PC),A1
	Bsr	CopyData
	Move.B	#0,(A1)

	Lea	FileArea(PC),A0
	Lea	_CommentStart(PC),A1
	Lea	_CommentSize(PC),A2
	Bsr	LoadFile
	Bne	.FailedComment

	Move.L	#FileArea,D1
	Move.L	#MODE_NEWFILE,D2
	CALLPCDOS	Open
	Move.L	D0,_CommentHandle
	Beq	.FailedWOpen

	Move.L	_ItemStart(PC),A4
	Move.L	_ItemSize(PC),D4
	Divu.W	#_IDEntrySize,D4
	Subq	#1,D4
	Moveq	#0,D5		<----- Position counter

.Checkloop	Cmp.L	#0,_IDFSize(A4)
	Beq.S	.NoEntry

	Move.L	_CommentStart(PC),A2
	Add.L	_IDCommentStart(A4),A2
	Move.L	A2,A3

	Move.L	D5,_IDCommentStart(A4)

	Moveq	#0,D1

.FindEndloop	Move.B	(A3)+,D1
	Cmp.W	#1,D1
	Bgt.S	.FindEndloop
	Sub.L	A2,A3
	Move.L	A3,D3
	Move.L	A2,D2
	Addq	#1,D3
	Add.L	D3,D5		<---Position counter
	Move.L	_CommentHandle(PC),D1
	CALLPCDOS	Write

	Lea	_IDEntrySize(A4),A4
.NoEntry	DBra	D4,.Checkloop

	Lea	ItemDataName(PC),A0
	Move.L	_FileMarker(PC),A1
	Bsr	CopyData
	Move.B	#0,(A1)

	Move.L	#FileArea,D1
	Move.L	#MODE_NEWFILE,D2
	CALLPCDOS	Open
	Move.L	D0,_ItemHandle
	Beq.S	.FailedIOpen
	Move.L	D0,D1
	Move.L	_ItemStart(PC),D2
	Move.L	_ItemSize(PC),D3
	CALLPCDOS	Write

	Move.L	_ItemHandle(PC),D1
	CALLPCDOS	Close

.FailedIOpen	Move.L	_CommentHandle(PC),D1
	CALLPCDOS	Close

.FailedWOpen	Move.L	_CommentStart(PC),A1
	Move.L	_CommentSize(PC),D0
	CALLPCEXEC	FreeMem

.FailedComment	Move.L	_ItemStart(PC),A1
	Move.L	_ItemSize(PC),D0
	CALLPCEXEC	FreeMem

.FailedItem	Rts
