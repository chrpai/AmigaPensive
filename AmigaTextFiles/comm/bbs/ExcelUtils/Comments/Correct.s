*********************************************************************
Correct:	Lea	ItemDataName(PC),A0
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

	Move.L	_CommentStart(PC),A2
	Move.L	_ItemStart(PC),A4
	Move.L	_ItemSize(PC),D4
	Divu.W	#_IDEntrySize,D4
	Subq	#1,D4
	Moveq	#1,D5		<----- Position counter

.Checkloop	Cmp.L	#0,_IDFSize(A4)
	Beq.S	.NoEntry

	Move.L	D5,_IDCommentStart(A4)

	Moveq	#0,D1

.FindEndloop	Move.B	(A2)+,D1
	Addq	#1,D5
	Cmp.W	#1,D1
	Bgt.S	.FindEndloop

	Lea	_IDEntrySize(A4),A4
.NoEntry	DBra	D4,.Checkloop

	Move.L	_ItemStart(PC),A4
	Move.L	#0,_IDCommentStart(A4)

	Lea	ItemDataName(PC),A0
	Move.L	_FileMarker(PC),A1
	Bsr	CopyData
	Move.B	#0,(A1)

	Lea	FileArea(PC),A1
	Move.L	A1,D1
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

.FailedIOpen	Move.L	_CommentStart(PC),A1
	Move.L	_CommentSize(PC),D0
	CALLPCEXEC	FreeMem

.FailedComment	Move.L	_ItemStart(PC),A1
	Move.L	_ItemSize(PC),D0
	CALLPCEXEC	FreeMem

.FailedItem	Rts
