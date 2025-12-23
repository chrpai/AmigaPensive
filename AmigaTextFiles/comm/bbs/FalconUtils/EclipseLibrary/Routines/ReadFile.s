* ReadFile ****************************************************************
*
* Allocates a block of memory, and loads a file into it
*
*INPUTS
*    D0 = Type of memory to use (MEMF_xxxx)
*    A0 = Pointer to sring containing file name
*    A1 = Pointer to LongWord to contain start of allocated block
*    A2 = Pointer to LongWord to contain size of allocated block
*
*RESULTS
*    D0 =
*         0 = No Error
*         1 = Error Reading the file
*         2 = Not enough memory to load file
*         3 = The file could not be found/opened
*
***************************************************************************
ReadFile:
	Movem.L	D2/D3/D4/A2/A6,-(SP)
	Move.L	A1,MemStart
	Move.L	A2,MemSize
	Move.L	D0,MemType

	Moveq	#1,D4
	Move.L	A0,D1

	Bsr	GetFile
	Tst.L	D0
	Beq	.Error
	Move.L	D0,ReadFileHandle
	Move.L	D1,A0
	Move.L	124(A0),D0
	Move.L	MemSize(PC),A0
	Move.L	D0,(A0)

	Moveq	#2,D4
	Move.L	(A0),D0
	Move.L	MemType(PC),D1
	CALLEXEC	AllocMem
	Tst.L	D0
	Beq.S	.AllocError
	Move.L	MemStart(PC),A0
	Move.L	D0,(A0)

	Moveq	#3,D4
	Move.L	ReadFileHandle(PC),D1
	Move.L	MemStart(PC),A0
	Move.L	(A0),D2
	Move.L	MemSize(PC),A0
	Move.L	(A0),D3
	CALLDOS	Read
	Tst.L	D0
	Beq.S	.LoadError

	Move.L	ReadFileHandle(PC),D1
	CALLDOS	Close
	Moveq	#0,D4
.Error
	Move.L	D4,D0
	Movem.L	(SP)+,D2/D3/D4/A2/A6
	Rts

.LoadError
	Move.L	MemStart(PC),A0
	Move.L	(A0),A1
	Move.L	MemSize(PC),A0
	Move.L	(A0),D0
	CALLEXEC	FreeMem

.AllocError
	Move.L	ReadFileHandle(PC),D1
	CALLDOS	Close

	Move.L	D4,D0
	Movem.L	(SP)+,D2/D3/D4/A2/A6
	Rts

ReadFileHandle:	Dc.L	0
MemSize:	Dc.L	0
MemStart:	Dc.L	0
MemType:	Dc.L	0
	CNOP	0,4
ReadFileInfo:	Ds.B	260
