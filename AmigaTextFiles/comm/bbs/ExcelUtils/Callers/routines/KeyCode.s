;Read keyfile
Readkey:	move.l	#_Keyname,D1
	move.l	#MODE_OLDFILE,D2
	CALLDOS	Open
	move.l	D0,_File
	beq	.fail

	move.l	D0,D1
	move.l	#_Keybuffer,D2
	moveq	#96,D3
	CALL	Read
	Move.L	D0,D3
	move.l	_File,D1
	CALL	Close
	cmp.L	#96,D3
	bne.s	.fail

;CheckValidity of keyfile

	lea	_Keybuffer,A0
	bsr	checkchecksum
	tst.l	D0
	bne.s	.fail

	lea	_Keybuffer,A0
	lea	_Sysop,A1
	bsr.s	decodesysop
	lea	_Keybuffer,A0
	lea	_BBS,A1
	bsr.s	decodebbs
	lea	_Keybuffer,A0
	lea	_ID,A1
	bsr.s	decodeid
	lea	_Keybuffer,A0
	bsr.s	getserial
	move.l	D0,_Serial

	lea	_IDLabel,A0
	lea	_ID,A1
	bsr	CompareStrings
	tst.l	D0
	bne.s	.fail

	move.b	#0,_KeyOK

.fail	Rts


;Decode keyfile

;key=A0	Sysop=a1
decodesysop:	moveq	#47,D1
	moveq	#39,D2
.loopds	move.b	(A0)+,D0
	sub.b	D1,D0
	addq	#1,D1
	cmp.b	#10,D0
	bne.s	.nds
	moveq	#0,D0
.nds	move.b	D0,(A1)+
	dbf	D2,.loopds
	rts

;A0=key	A1=bbs
decodebbs:	moveq	#96,D1
	lea	40(A0),A0
	moveq	#39,D2
.loopdb	move.b	(A0)+,D0
	add.b	D1,D0
	subq	#1,D1
	cmp.b	#10,D0
	bne.s	.ndb
	moveq	#0,D0
.ndb	move.b	D0,(A1)+
	dbf	D2,.loopdb
	rts

;A0=key	A1=id
decodeid:	lea	84(A0),A0
	moveq	#11,D2
.loopdi	move.b	(A0)+,D0
	sub.b	#40,D0
	cmp.b	#10,D0
	bne.s	.ndi
	moveq	#0,D0
.ndi	move.b	D0,(A1)+
	dbf	D2,.loopdi
	rts

;A0=key
getserial:	moveq	#0,D0
	lea	80(A0),A0
	move.w	(A0),D0
	not.w	D0
	rts
	
;A0=key
checkchecksum:	moveq	#0,D0
	moveq	#0,D1
	moveq	#79,D2
.loopchk	move.b	(A0)+,D0
	add.l	D0,D1
	dbf	D2,.loopchk
	move.w	2(A0),D0
	sub.l	D1,D0
	rts
