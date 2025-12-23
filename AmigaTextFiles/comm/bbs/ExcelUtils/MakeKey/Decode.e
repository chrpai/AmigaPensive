/*Released as public domain 28-Jan-1996 by the author Dave Dustin*/
/* Test of Decode */

DEF keyfile[96]:STRING,sysop[40]:STRING,bbs[40]:STRING,id[12]:STRING,serial
DEF filehandle

PROC main()
	filehandle:=Open('keyfile',OLDFILE)
	Read(filehandle,keyfile,96)
	Close(filehandle)
	
	decodesysop(keyfile,sysop)
	WriteF('sysop: \s\n',sysop)
	decodebbs(keyfile,bbs)
	WriteF('bbs: \s\n',bbs)
	decodeid(keyfile,id)
	WriteF('id: \s\n',id)
	serial:=getserial(keyfile)
	WriteF('serial#: \d\n',serial)
	IF checkchecksum(keyfile)=TRUE 
		WriteF('checksum routine ok')
	ELSE
		WriteF('checksum routine bad')
	ENDIF
ENDPROC

PROC decodesysop(key,sysop)
DEF loopcount
	MOVE.L	key,A0
	MOVE.L	sysop,A1
	MOVEQ	#47,D1
	FOR loopcount:=0 TO 39
		MOVE.B	(A0)+,D0
		SUB.B	D1,D0
		ADDQ	#1,D1
		CMP.B	#10,D0
		BNE.S	nds
		MOVEQ	#0,D0
nds:	MOVE.B	D0,(A1)+
	ENDFOR
ENDPROC

PROC decodebbs(key,bbs)
DEF loopcount
	MOVE.L	key,A0
	LEA		40(A0),A0
	MOVE.L	bbs,A1
	MOVEQ	#96,D1
	FOR loopcount:=0 TO 39
		MOVE.B	(A0)+,D0
		ADD.B	D1,D0
		SUBQ	#1,D1
		CMP.B	#10,D0
		BNE.S	ndb
		MOVEQ	#0,D0
ndb:	MOVE.B	D0,(A1)+
	ENDFOR
ENDPROC

PROC decodeid(key,id)
DEF loopcount
	MOVE.L	key,A0
	LEA		84(A0),A0
	MOVE.L	id,A1
	FOR loopcount:=0 TO 11
		MOVE.B	(A0)+,D0
		SUB.B	#40,D0
		CMP.B	#10,D0
		BNE.S	ndi
		MOVEQ	#0,D0
ndi:	MOVE.B	D0,(A1)+
	ENDFOR
ENDPROC

PROC getserial(key)
DEF serial
	MOVEQ	#0,D0
	MOVE.L	key,A0
	LEA		80(A0),A0
	MOVE.W	(A0),D0
	NOT.W	D0
	MOVE.L	D0,serial
ENDPROC serial

PROC checkchecksum(key)
DEF	loopcount,a=0,b=0
	MOVE.L	key,A0
	MOVEQ	#0,D0
	MOVEQ	#0,D1
	FOR loopcount:=0 TO 79
		MOVE.B	(A0)+,D0
		ADD.L	D0,D1
	ENDFOR
	MOVE.L	D1,a
	MOVE.W	2(A0),D0
	MOVE.L	D0,b
	IF (a<>b) THEN RETURN FALSE
ENDPROC TRUE
