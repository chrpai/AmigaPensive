/*Released as public domain 28-Jan-1996 by the author Dave Dustin*/

/* UserList V2.41
 *
 * © 1993 David Dustin / Eclipse Software
 *
 *
 * This utility provides a Excelsior! BBS sysop with a way to view the
 * list of users on their system, without the system needing to be run.
 *
 * History:
 *	1.0		Written in assembly for Falcon CBCS
 *	2.0		Written in E for Excelsior! BBS
 *	2.1		Added checking for deleted users
 *			Added first/last arguments
 *			Added access argument
 *	2.2		Added higher argument
 *	2.21	Major bug fixed.  Nothing typed unless H or ACCESS was present
 *			Fixed minor error with number of users checked.  One too many
 *	2.3		Updated code for Excelsior! 0.979
 *			Added extend option
 *			Added match options for main information
 *  2.4		Added Keyfile protection
 *  2.41	Added check for Data/UserData as defualt if no file is specified
 */
 
OPT OSVERSION=37
MODULE	'workbench/startup'
MODULE	'ReqTools'
MODULE	'libraries/ReqTools'

DEF fname[120]:STRING,first,last,extend,wantacc,acchigh=TRUE,wantcalls,callhigh=TRUE
DEF malias[31]:STRING,mname[31]:STRING,mcomp[31]:STRING,mcity[26]:STRING
DEF myargs:PTR TO LONG,freq:PTR TO rtfilerequester,rdargs,tstring[20]:STRING
DEF error,adr,len,flen=NIL,alive,highest,loopcount,usercount,usernumber=1
DEF alias,name,computer,access,city
DEF phone,calls
DEF keyfilehandle,keyok=FALSE,keyfile[96]:STRING,sysop[40]:STRING,serial

PROC main()
	myargs:=[0,0,0,0,0,0,0,0,0,0]
	IF rdargs:=ReadArgs('FILENAME,FIRST/K/N,LAST/K/N,E=EXTEND/S,ACCESS/K,CALLS/K,A=ALIAS/K,N=NAME/K,C=COMPUTER/K,P=PLACE/K',myargs,NIL)
		StrCopy(fname,myargs[0],ALL)
		first:=Long(myargs[1])
		last:=Long(myargs[2])
		extend:=myargs[3]
		StrCopy(tstring,myargs[4],ALL);parserange(0)
		StrCopy(tstring,myargs[5],ALL);parserange(1)
		StrCopy(malias,myargs[6],ALL);UpperStr(malias)
		StrCopy(mname,myargs[7],ALL);UpperStr(mname)
		StrCopy(mcomp,myargs[8],ALL);UpperStr(mcomp)
		StrCopy(mcity,myargs[9],ALL);UpperStr(mcity)
		FreeArgs(rdargs)
	ENDIF

	
	IF fname[0]=0 THEN rtfilereq()
	IF fname[0]<>0
	IF readfile()<>TRUE
		SELECT error
			CASE 1
				WriteF('Unable to open \a\s\a\n',fname)
			CASE 2
				WriteF('Unable to allocate \d bytes for \a\s\a\n',flen,fname)
			CASE 3
				WriteF('Only read \d bytes of \d from \a\s\a\n',len,flen,fname)
			CASE 4
				WriteF('\a\s\as size doesn\at match a normal 0.979+ UserData file.\n',fname)
			DEFAULT
				WriteF('Fatal Error\n')
		ENDSELECT
	ELSE
		WriteF('[1mUserList V2.41[0m\n©1993 David Dustin/Eclipse Software\n')
		IF (keyfilehandle:=Open('L:EclipseExcel.key',OLDFILE))<>0
			Read(keyfilehandle,keyfile,96)
			IF checkchecksum(keyfile)=TRUE
				decodesysop(keyfile,sysop)
				serial:=getserial(keyfile)
				keyok:=TRUE
			ENDIF
		ENDIF

		WriteF('Registered to:  ')
		IF keyok=TRUE
			WriteF('\s (\d)\n\n',sysop,serial)
		ELSE
			WriteF('unregistered.\n\n')
			Delay(250)
		ENDIF

		IF (extend<>0) AND (keyok=FALSE) THEN WriteF('[1mSorry, EXTEND option is disabled until you register![0m\n\n')
		highest:=Div(len,1268)
		IF (last=0) OR (last>highest) THEN last:=highest
		IF (first<=0) OR (first>highest) OR (first>last) THEN first:=0
		IF first<>0
			adr:=adr+(Mul(first-1,1268))
			last:=last-first
			usernumber:=first
		ENDIF
		WriteF('ACC# Alias               Real Name            Comp   Location          Acc\n')
		WriteF('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n')
			FOR loopcount:=0 TO last-1
					alive:=Long(adr+836)
					MOVE.L	alive,D0
					BTST	#9,D0
					BEQ		isdead

					alias:=adr+172
					IF (malias[0]<>0) AND (compare(alias,malias)=FALSE) THEN JUMP isdead

					name:=adr+203
					IF (mname[0]<>0) AND (compare(name,mname)=FALSE) THEN JUMP isdead

					computer:=adr+397
					IF (mcomp[0]<>0) AND (compare(computer,mcomp)=FALSE) THEN JUMP isdead

					city:=adr+330
					IF (mcity[0]<>0) AND (compare(city,mcity)=FALSE) THEN JUMP isdead

					access:=Char(adr+16)
					IF (acchigh=FALSE) AND (wantacc<>0)
						IF (access<wantacc) THEN JUMP isdead
					ELSEIF (wantacc<>0)
						IF (access<>wantacc) THEN JUMP isdead
					ENDIF

					calls:=Long(adr+968)
					IF (callhigh=FALSE) AND (wantcalls<>0)
						IF (calls<wantcalls) THEN JUMP isdead
					ELSEIF (wantcalls<>0)
						IF (calls<>wantcalls) THEN JUMP isdead
					ENDIF

					WriteF('\r\d[3]  \l\s[19] \s[20] \s[5]  \s[17]  \r\d[2]\n',usernumber,alias,name,computer,city,access)
					INC usercount
					IF extend=0 THEN JUMP isdead
					IF keyok=TRUE
						phone:=adr+234
						WriteF('Ph: \l\s[18]   Number of calls: \d[4]\n\n',phone,calls)
					ENDIF
isdead:			adr:=adr+1268
				INC usernumber
				IF CtrlC()=TRUE
					WriteF('\n...break UserList')
					loopcount:=last
				ENDIF
			ENDFOR
			WriteF('\n\d active users listed.\n\n',usercount)
		ENDIF
	ENDIF
ENDPROC

PROC rtfilereq()
	DEF reqname[120]:STRING
	DEF req,filetags
	filetags:=[0]
	IF FileLength('Data/UserData')<>-1
		StrCopy(fname,'Data/UserData',ALL)
		RETURN
	ENDIF
	IF reqtoolsbase:=OpenLibrary('reqtools.library',37)
		IF req:=RtAllocRequestA(0,0)
			freq:=req
			fname[0]:=0
			reqname:='UserData\0'
			IF (RtFileRequestA(req,reqname,'Select UserData file...',filetags))=FALSE THEN RETURN
			StrCopy(fname,freq.dir,ALL)
			AddPart(fname,reqname,120)
			RtFreeRequest(req)
			CloseLibrary(reqtoolsbase)
		ENDIF
	ELSE
		WriteF('Could not open reqtools.library V37+\n')
		fname[0]:=0
	ENDIF
ENDPROC

PROC readfile()
	DEF a,handle
	IF (flen:=FileLength(fname))=-1 THEN RETURN error:=1
	IF (adr:=New(flen))=NIL THEN RETURN error:=2
	IF (handle:=Open(fname,OLDFILE))=NIL THEN RETURN error:=1
	IF (len:=Read(handle,adr,flen))=NIL THEN RETURN error:=3
	Close(handle)
	IF len<>flen THEN RETURN error:=3
	IF (a:=Mod(flen,1268)) THEN RETURN error:=4
ENDPROC TRUE

PROC compare(data1,data2)
	DEF	tempstr[31]:STRING
		StrCopy(tempstr,data1,ALL)
		UpperStr(tempstr)
		IF (InStr(tempstr,data2,0))=-1 THEN RETURN FALSE
ENDPROC TRUE

PROC parserange(mode)
	DEF	tcount,a,number,flag,linelen
	IF (linelen:=StrLen(tstring))<>0
		FOR tcount:=0 TO linelen
			a:=Char(tstring+tcount)
			IF (a=72) OR (a=104)
				flag:=FALSE
				JUMP finishedparse
			ENDIF
		ENDFOR
finishedparse:
		number:=Val(tstring,NIL)
		IF mode=0
			wantacc:=number
			acchigh:=flag
		ELSE
			wantcalls:=number
			callhigh:=flag
		ENDIF
	ENDIF
	tstring[0]:=0
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

PROC getserial(key)
DEF serial
	MOVEQ	#0,D0
	MOVE.L	key,A0
	LEA		80(A0),A0
	MOVE.W	(A0),D0
	NOT.W	D0
	MOVE.L	D0,serial
ENDPROC serial

CHAR '$VER: UserList v2.41 (17.3.93)  ©1993 David Dustin / Eclipse Software'
