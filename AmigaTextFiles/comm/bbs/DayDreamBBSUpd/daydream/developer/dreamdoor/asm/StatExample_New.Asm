;.-------------------------------------------------------------------------.
;|STATUS DOOR EXAMPLE IN ASM FOR DAYDREAM WRITTEN BY BIG-BANG! 28-NOV-1996!|
;|FOR FURTHER HELP.. CONTACT ME AT MY BOARD OPEN 24H/DAY: +46-(0)18-469960!|
;|OR FIND ME ON IRC! (/WHOIS BIGBANG)! THIS WAS WRITTEN USING ASM-ONE 1.29!|
;`-------------------------------------------------------------------------'

;Comments:
;
; - Use equates. Makes life easier.
; - Work_Buffer & Conv_Buffer MUST be allocated
;   aswell if door should be residentable.
;
; I changed all the -xxx(a6) jumps to equates
; Line 143:       Changed adda to add.
; Line 226 & 227: Changed Blk.b to Dcb.b
; Splitted up the design into more rows, as GenAm(DevPac) does not support
; lines longer than 256 bytes.
; Changed dp_curruser to dp_CurrUser and structures to Structures.
;

;-----------------÷[ The Variables, Includes & Structures! ]÷----------------÷]

		Incdir	'Include:'		;Directory of include files!
		Include 'DreamDoor_lib.i'	;DreamDoor library equates.
		Include	'Daydream.i'		;Include this file.
		Include	'Exec/Exec.i'		;And this one too!
		Include	'Exec/Exec_lib.i'	;And this one too!

		STRUCT		Structures,dp_SIZEOF	;Structures Size from
		LABEL		Variables_SIZEOF	;Daydream.I etc!

;--------------------÷[ Just get the damn arguments! ]÷----------------------÷]

Run:		Movea.L	a0,a4			;Store command line parameter!
		Move.L	d0,d4			;The %N entry that is!

;-----------------÷[ Allocate required mem for structures! ]÷----------------÷]

Init:		Movea.L	4.w,a6				;Exec library base!
		Move.L	#Variables_SIZEOF,d0		;Bytes to Allocate!

		Move.L	#MEMF_PUBLIC!MEMF_CLEAR,d1	;Flags for allocation!
		Jsr	_LVOAllocMem(a6)		;AllocMem()..

		Beq.W	ByeBye			;If failed to AllocMem()..
		Movea.L	d0,a5			;Store memory pointer in a5.
		Lea	DayLib(pc),a1		;Library to open -> a1.
		Moveq	#5,d0			;Atleast version 5 of lib!
		Jsr	_LVOOpenLibrary(a6)	;OpenLib()..
		Beq.W	ByeBye			;If failed to OpenLib()..
		Move.L	d0,DayLib_Base		;Store the library base.
		Clr.B	-1(a4,d4.l)		;Clear character in %N arg!
		Tst.B	(a4)			;Check if %N was entered.
		Beq.W	ByeBye			;If %N wasn't entered.

;-------------÷[ Initialize door & Get structure pointers! ]÷----------------÷]

InitDoor:	Movea.L	DayLib_Base(pc),a6	;Get dreamdoor base.
		Movea.L	a4,a0			;The damn parameter in a0!
		Jsr	_LVOInitDoor(a6)	;Initdoor()..
		Beq.W	ByeBye			;If Failed to InitDoor()..
		Move.L	d0,Day_DIF		;Store the daydream DIF.
		Lea	Structures(a5),a0	;The daydream structures.
		Jsr	_LVOInquirePointers(a6)	;InquirePointers()..
		Move.L	Day_DIF(pc),d0		;Update daydream DIF in d0.

;-------------------------÷[ The main program code! ]÷-----------------------÷]

Main:		Jsr	_LVOHideCursor(a6)		;HideCursor()..
		Move.L	Day_DIF(pc),d0		;Update daydream DIF in d0!
		Lea	Activity(pc),a0		;String to change activity to!
		Jsr	_LVOChangeActivity(a6)		;ChangeActivity()..
		Lea	Design(pc),a0		;The Door design by BigBang!
		Bsr.W	Sendit			;Put the shit on screen.

		Lea	Position(pc),a3		;Position ASCII/ANSI string!
		Moveq	#USER_REALNAME,d7	;To get current user realname.
		Bsr.W	DoIt			;Get it and send it.

		Move.B	#"7",2(a3)		;Change position string!
		Moveq	#USER_HANDLE,d7		;To get current user handle!
		Bsr.W	DoIt			;Get the shit and print it!

		Move.B	#"8",2(a3)		;Change [7;xxH -> [8;xxH!
		Moveq	#USER_ORGANIZATION,d7	;To get user organisation!
		Bsr.W	DoIt			;Get it and show it on screen!

		Move.B	#"9",2(a3)		;Change position again.
		Moveq	#USER_ZIPCITY,d7	;Add 78 to address further down
		Bsr.W	DoIt			;Send user Zip/City to DD.

		Lea	Co_Position(pc),a3	;New position string.
		Moveq	#USER_VOICEPHONE,d7	;Move in structures to Voice.
		Bsr.W	DoIt			;Send user Voice Phone Number!

		Lea	Position(pc),a3		;Update first position string!
		Move.W	#"55",4(a3)		;Change X position of it.
		Move.B	#USER_COMPUTERMODEL,d7	;To get user computer.
		Move.B	#"6",2(a3)		;Change Y position of it.
		Bsr.B	DoIt			;Send user computer to status.

		Move.B	#"7",2(a3)		;You should know by now!
		Move.B	#USER_SECURITYLEVEL,d7	;To get user security.
		Moveq	#24,d2			;Set a flag for DoIt routine!
		Bsr.B	DoIt			;Get it but don't sendit.
		Move.B	Work_Buffer(pc),d1	;Move security in HEX to d1.
		Moveq	#3,d6			;Set flag for Convert routine!
		Bsr.W	Convert			;Convert and send security.

		Move.B	#"8",2(a3)		;Position string change.
		Move.B	#USER_SCREENLENGTH,d7	;Point to User screen lenght!
		Moveq	#24,d2			;Set a flag for DoIt routine!
		Bsr.B	DoIt			;Get it but don't send.
		Move.B	Work_Buffer(pc),d1	;Move read screen lenght -> d1
		Moveq	#3,d6			;Set flag for Convert routine!
		Bsr.W	Convert			;Convert HEX->ASCII and send!

		Move.B	#"9",2(a3)		;You know what, right!?
		Move.B	#USER_DAILYTIMELIMIT,d7	;Point at user daily time.
		Moveq	#24,d2			;Set a flag for DoIt routine!
		Bsr.B	DoIt			;Get it but don't send it.
		Move.W	Work_Buffer(pc),d1	;Move HEX daily time to d1.
		Moveq	#0,d6			;Clear convert routine flag!
		Bsr.B	Convert			;Convert and send daily time!

		Lea	Co_Position(pc),a3	;New position string.
		Move.W	#"55",5(a3)		;Change new position.
		Move.W	#USER_TIMEREMAINING,d7	;To point at time left today.
		Moveq	#24,d2			;Set a flag for DoIt routine!
		Bsr.B	DoIt			;Get HEX but don't send yet.
		Move.W	Work_Buffer(pc),d1	;Move HEX, for conversion.
		Bsr.B	Convert			;Convert to ASCII and send.

		Lea	LogString(pc),a0	;String to log pointer in a0!
		Move.L	Day_DIF(pc),d0		;Yepp, do just this now!
		Jsr	_LVOWriteLog(a6)	;WriteLog()..
		Bra.B	EndIt			;Go down and end the door.

;----------------------÷[ Format string %s and send it! ]÷-------------------÷]

DoIt:		Move.L	dp_CurrUser+Structures(a5),d0	;Current user data!

		Add.L	d7,d0			;Go forward in structure data!
		Lea	Privet(pc),a0		;String to Format %s code in!
		Movem.L	d0-d7/a6,-(sp)		;Push some registers on stack!
		Movea.L	4.w,a6			;Get exec.library base again.
		Lea	Work_Buffer(pc),a4	;Where to place fixed string!
		Movea.L	SP,a1			;Stackpointer, to be safe.
		Lea	.PutChar(pc),a2		;Transfer character routine.
		Jsr	_LVORawDoFmt(a6)	;RawDoFmt()..
		Movem.L	(sp)+,d0-d7/a6		;Pop some registers off stack!
		Bra.B	.Continue		;And continue further down.
.PutChar	Move.B	d0,(a4)+		;Transfer a byte to buffer.
		Rts				;Go back again!
.Continue	Movea.L	a3,a0			;Position string pointer to a0!
		Bsr.W	Sendit			;Send string in a0.
		Lea	Work_Buffer(pc),a0	;The read user data into a0!
		Clr.B	15(a0)			;Make it maximum 16 characters!
		Cmpi.B	#24,d2			;Check for a flag in d2.
		Beq.B	.Out			;If d2 contains 24! (Flag set)
		Bsr.B	Sendit			;Send contents of a0 to DD!
.Out		Moveq	#0,d2			;Clear data reg.2 (The Flag)
		Rts				;Get the fuck back in there!

;---------------------÷[ Convert HEX to ASCII and send! ]÷-------------------÷]

Convert:	Lea	Conv_Buffer(pc),a0	;Buffer to place convert in.
		Bsr.B	.Decimal4		;Go convert the shit!
		Lea	Conv_Buffer(pc),a0	;Converted HEX into a0 again!
		Bsr.B	Sendit			;Send HEX converted to ASCII.
		Rts				;Return back from subroutine!

.Decimal4	Cmpi.B	#3,d6			;Check for a flag in d6.
		Beq.B	.3			;If flag is set, branch here!
		Divu	#1000,d1		;Devide HEX with 1000.
		Bsr.B	.Digit			;Fix & add first digit 2 buff!
.3		Divu	#100,d1			;Devide HEX with 100.
		Bsr.B	.Digit			;Fix and add second digit.
		Divu	#10,d1			;Devide HEX with 10.
		Bsr.B	.Digit			;Fix and add third digit.
		Divu	#1,d1			;Devide HEX with 1.
		Bsr.B	.Digit			;Fix and add fourth digit.
		Rts				;Return back again.

.Digit		Addi.B	#$30,d1			;Add 30 HEX to devided digit!
		Move.B	d1,(a0)+		;Move character into buffer.
		Clr.B	d1			;Clear byte contents of d1.
		Swap	d1			;Swap d1 high bits with low.
		Rts				;And go back yet again.

;-------------------------÷[ End the door routine! ]÷------------------------÷]

EndIt:		Lea	EndPosition(pc),a0	;Position of cursor in the end!
		Bsr.B	Sendit			;Make it so!
		Move.L	Day_DIF(pc),d0		;You know! If you don't, BASIC!
		Jsr	_LVOShowCursor(a6)	;ShowCursor()..
		Move.L	Day_DIF(pc),d0		;Update daydream DIF in d0!
		Jsr	_LVOCloseDoor(a6)	;CloseDoor()..

;------------------------÷[ Hasta la' vista.. baby! ]÷-----------------------÷]

ByeBye:		Movea.L	4.w,a6			;Update exec.library base.
		Tst.L	DayLib_Base		;Is dreamdoor.library open?
		Beq.B	.1			;If it ain't open, branch here!
		Movea.L	DayLib_Base(pc),a1	;If it is open, close it.
		Jsr	_LVOCloseLibrary(a6)	;CloseLibrary()..
.1		Tst.L	(a5)			;Check if memory is allocated.
		Beq.B	.2			;If no memory is allocated.
		Movea.L	a5,a1			;What memory to free?
		Move.L	#Variables_SIZEOF,d0	;How much memory to free?
		Jsr	_LVOFreeMem(a6)		;FreeMem()..
.2		Moveq	#0,d0			;No special return codes.
		Rts				;Back to reality.

;------------------------÷[ Send the string routine! ]÷----------------------÷]

Sendit:		Move.L	Day_DIF(pc),d0		;Update daydream DIF in d0.
		Jsr	_LVOSendString(a6)	;Sendstring()..
		Rts				;Return from this subroutine!

;----------------------÷[ A little strings and shit! ]÷----------------------÷]

		Cnop	0,4
DayLib_Base:	Dc.L	0,0			;Long space for Dreamdoor.lib!
Day_DIF:	Dc.L	0,0			;Long space for daydream DIF.
Work_Buffer:	Dcb.B	45,0			;A buffer on 45 bytes.
Conv_Buffer:	Dcb.B	6,0			;A buffer on 6 bytes.
		Even				;Align to even addresses.

DayLib:		Dc.B	'dreamdoor.library',0		;Library to open name.
Activity:	Dc.B	'Checking UserStatus!',0	;User Activity string.
		Even
LogString:	Dc.B	'%H checked user status at %T!',0	;LogString!
		Even
Privet:		Dc.B	'%s!',0				;String for RawDoFmt()..
		Even
Position:	Dc.B	27,'[6;23H',27,'[36m',0		;Position string.
Co_Position:	Dc.B	27,'[10;23H',27,'[36m',0	;New position string.
EndPosition:	Dc.B	27,'[14;0H',0			;Ending position.
		Even

Design:		Dc.B	12,10,'   ',27,'[35m___.-----------------------------------------------------------.____',10,' __)     ',27,'[36mSTATUS DOOR FOR DAYDREAM',27,'[37m!  ',27,'[36mV',27,'[37m1',27,'[36m.',27,'[37mOO  ',27,'[36m(',27,'[37mC',27
		Dc.b	'[36m) 1996 BY BIG',27,'[37m-',27,'[36mBANG',27,'[37m!      ',27,'[35m(___',10
		Dc.B	' \________ __ _',27,'[4;65H_  ______/',10,'    |',27,'[5;71H|',10,'    |  ',27,'[37mREAL NAME',27,'[35m....:                 ',27,'[37mUSER COMPUTER',27,'[35m.:                 |',10
		Dc.B	'    |  ',27,'[37mHANDLE',27,'[35m.......:                 ',27,'[37mUSER SECURITY',27,'[35m.:                 |',10,'    |  ',27,'[37mORGANISATION',27,'[35m.:                 ',27
		Dc.b	'[37mSCREEN LENGHT',27,'[35m.:                 |',10
		Dc.B	'    |  ',27,'[37mZIP',27,'[36m/',27,'[37mCITY',27,'[35m.....:                 ',27,'[37mDAILY TIME',27,'[35m....:                 |',10,'    |  ',27,'[37mVOICE PHONE',27,'[35m..:                 ',27
		Dc.b	'[37mTIME LEFT',27,'[35m.....:                 |',10
		Dc.B	'    l_',27,'[11;70H_|',10,'      \_____________________________________________________________/',10,0
		Even
