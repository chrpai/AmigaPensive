;===[ Program Info ]===========================================================
;
;  Chatfool - Fakechatter-door for DayDream BBS v1.29.
;
;  Version 1.0   [01-Jul-97]
;
;  Programmed by Joo/Toytools using ASM-One v1.29 by T.F.A.
;
;  If you use this source or any parts of it, please credit me!
;
;  The routines are quite basic, well I don't care as long as this works.
;
;===[ Includes & Offsets ]====================================================

			incdir	"include:"
			include	"exec/exec.i"
			include	"dos/dos.i"
			include	"dos/datetime.i"
			include	"libraries/daydream.i"
			include	"offsets/exec_lib.i"
			include	"offsets/dos_lib.i"
			include	"offsets/dreamdoor_lib.i"

;===[ Constants ]=============================================================

DOS_Version		=37
DreamDoor_Version	=5
Access_Buffersize	=8
Path_Buffersize		=150
Format_Buffersize	=500
Temporary_Buffersize	=1000
Character_Buffersize	=10
Write_Buffersize	=1000

;===[ Structures & Variables ]================================================

			STRUCTURE	Variables,0
			APTR		DOS_Base
			APTR		DreamDoor_Base
			APTR		DreamDoor_Pointer
			APTR		Node_Number
			ULONG		Access_Level
			ULONG		Display_Mode
			ULONG		User_Slot
			APTR		Configuration_Pointer
			ULONG		Configuration_Size
			APTR		Strings_Pointer
			ULONG		Strings_Size
			APTR		Main_Pointer
			ULONG		Main_Counter
			ULONG		Victims
			APTR		Log_Pointer
			ULONG		Characters
			ULONG		User_Linefeeds
			ULONG		User_Value
			ULONG		Force
			LABEL		Configuration1
			ULONG		PERSONALITIES
			LABEL		Configuration2
			ULONG		PERSONALITY
			ULONG		STRINGS_FILE
			ULONG		RANDOM_MODE
			ULONG		TYPING_SPEED
			ULONG		TYPING_VARIETY
			ULONG		ERROR_PERCENT
			ULONG		ERROR_CORRECT
			ULONG		PAUSE_SECONDS
			ULONG		PAUSE_VARIETY
			ULONG		IDLE_COUNT
			ULONG		LOGFILE
			LABEL		Strings
			ULONG		STARTING_STRINGS
			ULONG		STARTING_NUMBER
			ULONG		MAIN_STRINGS
			ULONG		MAIN_NUMBER
			ULONG		SILENCE_STRINGS
			ULONG		SILENCE_NUMBER
			ULONG		ENDING_STRINGS
			ULONG		ENDING_NUMBER
			STRUCT		Datestamp,dat_SIZEOF
			STRUCT		Random,8
			STRUCT		Structures,dp_SIZEOF
			STRUCT		Access_Buffer,Access_Buffersize
			STRUCT		Path_Buffer,Path_Buffersize
			STRUCT		Format_Buffer,Format_Buffersize
			STRUCT		Write_Buffer,Write_Buffersize
			STRUCT		Temporary_Buffer,Temporary_Buffersize
			STRUCT		Character_Buffer,Character_Buffersize
			STRUCT		Handlelower_String,30
			STRUCT		Locationlower_String,30
			STRUCT		Main_Table,100
			STRUCT		Silence_Table,100
			STRUCT		Date_String,LEN_DATSTRING
			STRUCT		Time_String,LEN_DATSTRING
			LABEL		Variables_SIZEOF

;===[ Startup ]===============================================================

			movem.l	d2-d7/a2-a6,-(sp)
			move.l	a0,a4
			move.l	d0,d4

Allocate_Memory:	move.l	4.w,a6
			move.l	#Variables_SIZEOF,d0
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			tst.l	d0
			beq	Exit_Program
			move.l	d0,a5

Open_DOS:		lea	DOS_Library,a1
			move.l	#DOS_Version,d0
			jsr	_LVOOpenLibrary(a6)
			move.l	d0,DOS_Base(a5)
			beq	Free_Memory

			clr.b	-1(a4,d4.l)
			tst.b	(a4)
			bne	Open_DreamDoor
			move.l	DOS_Base(a5),a6
			move.l	#Door_Usage,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Close_DOS

Open_DreamDoor:		lea	DreamDoor_Library,a1
			move.l	#DreamDoor_Version,d0
			jsr	_LVOOpenLibrary(a6)
			move.l	d0,DreamDoor_Base(a5)
			bne	Init_Random
			move.l	DOS_Base(a5),a6
			move.l	#DreamDoor_Error,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Close_DOS

Init_Random:		move.l	DOS_Base(a5),a6
			lea	Datestamp(a5),a2
			move.l	a2,d1
			jsr	_LVODateStamp(a6)
			move.l	8(a2),d0
			move.b	$dff006,d1
			mulu.w	d1,d0
			jsr	Random_Seed

			lea	Date_String(a5),a0
			move.l	a0,dat_StrDate(a2)
			lea	Time_String(a5),a0
			move.l	a0,dat_StrTime(a2)
			move.l	a2,d1
			jsr	_LVODateToStr(a6)

Init_Door:		move.l	DreamDoor_Base(a5),a6
			move.l	a4,a0
			move.l	a4,Node_Number(a5)
			jsr	_LVOInitDoor(a6)
			move.l	d0,DreamDoor_Pointer(a5)
			bne	Get_Structures
			move.l	DOS_Base(a5),a6
			move.l	#DreamPort_Error,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Close_DreamDoor

Get_Structures:		move.l	DreamDoor_Pointer(a5),d0
			lea	Structures(a5),a0
			jsr	_LVOInquirePointers(a6)

			move.l	dp_CurrUser+Structures(a5),a0
			moveq	#0,d0
			move.b	USER_SECURITYLEVEL(a0),d0
			move.l	d0,Access_Level(a5)
			moveq	#0,d0
			move.w	USER_ACCOUNT_ID(a0),d0
			move.l	d0,User_Slot(a5)
			moveq	#0,d0
			move.l	dp_CurrDisplay+Structures(a5),a0
			move.b	DISPLAY_ID(a0),d0
			move.l	d0,Display_Mode(a5)

			move.l	dp_CurrUser+Structures(a5),a0
			add.l	#USER_HANDLE,a0
			lea	Handlelower_String(a5),a1
			jsr	Convert_To_Lowercase

			move.l	dp_CurrUser+Structures(a5),a0
			add.l	#USER_ORGANIZATION,a0
			lea	Locationlower_String(a5),a1
			jsr	Convert_To_Lowercase

;===[ Check Force ]============================================================

			move.l	dp_DoorParams+Structures(a5),a0
			cmp.l	#"FORC",(a0)
			bne	Get_Variable
			cmp.b	#"E",4(a0)
			bne	Get_Variable
			move.l	#1,Force(a5)
			bra	Get_Configpath

;===[ Check SysOp State ]======================================================

Get_Variable:		move.l	DOS_Base(a5),a6
			move.l	#Access_Filename,d1
			lea	Access_Buffer(a5),a0
			move.l	a0,d2
			move.l	#Access_Buffersize,d3
			moveq	#0,d4
			jsr	_LVOGetVar(a6)
			tst.l	d0
			bpl	Compare_Access

			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			jsr	_LVOIsAway(a6)
			cmp.l	#0,d0
			beq	Page_Real
			bra	Get_Configpath

Compare_Access:		lea	Access_Buffer(a5),a0
			jsr	String_To_Number
			move.l	Access_Level(a5),d1
			cmp.l	d0,d1
			bcs	Get_Configpath

;===[ Real Page SysOp ]======================================================

Page_Real:		move.l	DreamDoor_Base(a5),a6
			lea	Temporary_Buffer(a5),a1
			move.b	#"O",(a1)+
			move.l	dp_DoorParams+Structures(a5),a0
			tst.b	(a0)
			beq	Page_SysOp
			move.b	#" ",(a1)+
Copy_Reason:		tst.b	(a0)
			beq	Page_SysOp
			move.b	(a0)+,(a1)+
			bra	Copy_Reason
Page_SysOp:		clr.b	(a1)
			lea	Temporary_Buffer(a5),a0
			move.l	DreamDoor_Pointer(a5),d0
			jsr	_LVOIntCommand(a6)
			bra	Close_Door

;===[ Get Configuration Path ]=================================================

Get_Configpath:		move.l	DOS_Base(a5),a6
			move.l	#Variable_Filename,d1
			lea	Path_Buffer(a5),a0
			move.l	a0,d2
			move.l	#Path_Buffersize,d3
			moveq	#0,d4
			jsr	_LVOGetVar(a6)
			tst.l	d0
			bpl	Load_Configuration
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	FileMissing_Error,a0
			jsr	_LVOSendString(a6)
			bra	Close_Door

;===[ Handle Configuration ]===================================================

Format_Configname:	movem.l	d0-d3,-(sp)
			move.l	4.w,a6
			move.l	sp,a1
			lea	Put_Character,a2
			lea	Temporary_Buffer(a5),a3
			jsr	_LVORawDoFmt(a6)
			movem.l	(sp)+,d0-d3
			lea	Temporary_Buffer(a5),a0
			rts

Load_Configuration:	move.l	Node_Number(a5),d5
			move.l	Display_Mode(a5),d6
			move.l	Access_Level(a5),d7
			lea	Path_Buffer(a5),a4

			move.l	a4,d0
			move.l	User_Slot(a5),d1
			lea	Config1_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d5,d1
			move.l	d6,d2
			move.l	d7,d3
			lea	Config2_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d5,d1
			move.l	d6,d2
			lea	Config3_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d5,d1
			move.l	d7,d2
			lea	Config4_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d6,d1
			move.l	d7,d2
			lea	Config5_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d5,d1
			lea	Config6_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d6,d1
			lea	Config7_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			move.l	d7,d1
			lea	Config8_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			move.l	a4,d0
			lea	Config9_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Headerconfig
			lea	Temporary_Buffer(a5),a0
			move.l	a0,d0
			move.l	#FileMissing_Error,a0
			jsr	Send_String
			bra	Close_Door

Handle_Headerconfig:	move.l	d6,Configuration_Pointer(a5)
			move.l	d5,Configuration_Size(a5)

			move.l	d6,a2
			lea	Configuration1_Strings,a3
			lea	Configuration1(a5),a4
			jsr	Handle_Configuration
			beq	Free_Configuration

			move.l	PERSONALITIES(a5),d0
			jsr	Make_Random
			move.l	d0,d2

			move.l	a2,a0
			jsr	Go_Next_Row
			tst.l	d2
			beq	Get_Personality
			moveq	#0,d1

Find_Personality:	tst.b	(a0)
			beq	Free_Configuration
			cmp.b	#"~",(a0)
			beq	Free_Configuration
			cmp.b	#"+",(a0)+
			bne	Find_Personality
Found_Next:		addq.l	#1,d1
			cmp.l	d2,d1
			bne	Find_Personality

Get_Personality:	move.l	a0,a2
			lea	Configuration2_Strings,a3
			lea	Configuration2(a5),a4
			jsr	Handle_Configuration
			beq	Free_Configuration

;===[ Handle Strings ]========================================================

			move.l	STRINGS_FILE(a5),a0
			jsr	Load_File
			tst.l	d0
			bne	Handle_Strings
			move.l	STRINGS_FILE(a5),d0
			move.l	#FileMissing_Error,a0
			jsr	Send_String
			bra	Free_Configuration

Handle_Strings:		move.l	d6,Strings_Pointer(a5)
			move.l	d5,Strings_Size(a5)

			move.l	d6,a0
			jsr	Go_Next_Row
			move.l	a0,a2

			lea	Strings(a5),a3
			moveq	#1,d1

Store_Stringpointer:	move.l	a2,(a3)+
			moveq	#0,d0
Strings_Loop:		tst.b	(a2)
			beq	Check_Strings
			cmp.l	#"===[",(a2)
			beq	Found_Divider
			cmp.b	#10,(a2)+
			bne	Strings_Loop
			addq.l	#1,d0
			bra	Strings_Loop
Found_Divider:		move.l	d0,(a3)+
			move.l	a2,a0
			jsr	Go_Next_Row
			move.l	a0,a2
			addq.l	#1,d1
			bra	Store_Stringpointer
Check_Strings:		cmp.l	#4,d1
			beq	Check_Reason
			move.l	STRINGS_FILE(a5),d0
			lea	Strings_Error,a0
			jsr	Send_String
			bra	Free_Strings

;===[ Fake Page SysOp ]======================================================

Check_Reason:		lea	Temporary_Buffer(a5),a1
			move.b	#"-",(a1)+
			clr.b	(a1)
			tst.l	Force(a5)
			bne	Update_Datafile
			move.l	dp_DoorParams+Structures(a5),a0
			tst.b	(a0)
			beq	Ask_Reason
			lea	Temporary_Buffer(a5),a1
Copy_Parameters:	move.b	(a0)+,(a1)+
			bne	Copy_Parameters
			bra	Page_Fake

Ask_Reason:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#STR_ps_AskReason,d1
			jsr	_LVOGetDDString(a6)
			move.l	d0,a0
			move.l	DreamDoor_Pointer(a5),d0
			jsr	_LVOSendString(a6)

			move.l	DreamDoor_Pointer(a5),d0
			move.l	#60,d1
			move.l	#0,d2
			lea	Temporary_Buffer(a5),a0
			move.l	#0,a1
			jsr	_LVOPrompt(a6)
			lea	Temporary_Buffer(a5),a0
			tst.b	(a0)
			bne	Page_Fake
			move.l	DreamDoor_Pointer(a5),d0
			lea	Linefeed_String,a0
			jsr	_LVOSendString(a6)
			bra	Free_Strings

Page_Fake:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#STR_PagingSysOp1,d1
			jsr	_LVOGetDDString(a6)
			move.l	d0,a2
			move.l	d0,a0
			jsr	Send_String
			move.l	a2,a0
			jsr	Go_String_End
			subq.l	#1,a0
			move.l	a0,a2

			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Activity1_String,a0
			jsr	_LVOChangeActivity(a6)

			move.l	#10,d0
			jsr	Make_Random
			move.l	d0,d7
			addq.l	#5,d0

Page_Loop:		move.l	DreamDoor_Pointer(a5),d0
			move.l	#HOTKEYF_CURSOR,d1
			move.l	#1,d2
			jsr	_LVOFetchKey(a6)
			cmp.b	#3,d0
			bne	Display_Paging

Abort_Paging:		move.l	DreamDoor_Pointer(a5),d0
			move.l	#STR_PageAborted,d1
			jsr	_LVOGetDDString(a6)
			move.l	d0,a0
			move.l	DreamDoor_Pointer(a5),d0
			jsr	_LVOSendString(a6)
			bra	Free_Strings

Display_Paging:		move.l	DreamDoor_Pointer(a5),d0
			move.l	a2,a0
			jsr	_LVOSendString(a6)
			dbf	d7,Page_Loop

;===[ Load Datafile ]=========================================================

Update_Datafile:	move.l	DOS_Base(a5),a6
			move.l	#Data_Filename,d1
			move.l	#MODE_READWRITE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			beq	Free_Strings
			move.l	d4,d1
			lea	Victims(a5),a0
			move.l	a0,d2
			move.l	#4,d3
			jsr	_LVORead(a6)
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_BEGINNING,d3
			jsr	_LVOSeek(a6)
			addq.l	#1,Victims(a5)
			move.l	d4,d1
			lea	Victims(a5),a0
			move.l	a0,d2
			move.l	#4,d3
			jsr	_LVOWrite(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)
			
;===[ Handle Chatting ]=======================================================

			move.l	DOS_Base(a5),a6
			move.l	LOGFILE(a5),d1
			move.l	#MODE_READWRITE,d2
			jsr	_LVOOpen(a6)
			tst.l	d0
			beq	Free_Strings
			move.l	d0,Log_Pointer(a5)
			move.l	d0,d1
			moveq	#0,d2
			moveq	#OFFSET_END,d3
			jsr	_LVOSeek(a6)
			move.l	Victims(a5),d0
			move.l	dp_CurrUser+Structures(a5),d1
			move.l	d1,d2
			add.l	#USER_HANDLE,d1
			add.l	#USER_ORGANIZATION,d2
			lea	Temporary_Buffer(a5),a0
			move.l	a0,d3
			lea	Date_String(a5),a0
			move.l	a0,d4
			lea	Time_String(a5),a0
			move.l	a0,d5
			move.l	PERSONALITY(a5),d6
			movem.l	d0-d6,-(sp)
			move.l	DOS_Base(a5),a6
			move.l	Log_Pointer(a5),d1
			move.l	#Header_String,d2
			move.l	sp,d3
			jsr	_LVOVFPrintf(a6)
			movem.l	(sp)+,d0-d6
			move.l	Log_Pointer(a5),d1
			moveq	#0,d2
			moveq	#OFFSET_END,d3
			jsr	_LVOSeek(a6)

Start_Chat:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Activity2_String,a0
			jsr	_LVOChangeActivity(a6)

			move.l	DreamDoor_Pointer(a5),d0
			move.l	#DDF_ADDEXTENSION!DDF_CHECKACS,d1
			lea	Startchat_Filename,a0
			jsr	_LVODisplayFile(a6)

			move.l	STARTING_NUMBER(a5),d0
			move.l	STARTING_STRINGS(a5),a0
			move.l	#0,a3
			move.l	d0,d3
			jsr	Get_String
			move.l	#8,d0
			jsr	Make_Random
			move.l	DOS_Base(a5),a6
			move.l	d0,d1
			add.l	#5,d1
			jsr	_LVODelay(a6)
			jsr	Writer

			move.l	MAIN_STRINGS(a5),Main_Pointer(a5)
			move.l	MAIN_NUMBER(a5),Main_Counter(a5)
			
			moveq	#0,d5
			move.l	RANDOM_MODE(a5),d6
			subq.l	#1,d6

Main_Loop:		move.l	#100,d0
			jsr	Make_Random
			move.l	d0,d3
			cmp.l	#65,d3
			bhi	Main_User
			jsr	Make_Linefeeds
			cmp.l	#10,d3
			bhi	Main_User
			jsr	Make_Linefeeds
			cmp.l	#5,d3
			bhi	Main_User
			jsr	Make_Linefeeds

Main_User:		jsr	User
			cmp.l	#1,User_Value(a5)
			beq	Chat_End

			tst.l	User_Linefeeds(a5)
			bne	Main_Idlecheck
			jsr	Make_Linefeeds

Main_Idlecheck:		cmp.l	IDLE_COUNT(a5),d5
			bne	Main_Plot

Main_Silence:		move.l	SILENCE_NUMBER(a5),d0
			move.l	SILENCE_STRINGS(a5),a0
			lea	Silence_Table(a5),a3
			move.l	d0,d3
			jsr	Get_String
			beq	Main_Plot
			jsr	Writer
			moveq	#0,d5
			bra	Main_Loop

Main_Plot:		cmp.l	#-1,RANDOM_MODE(a5)
			bne	Main_Random
			move.l	Main_Pointer(a5),a0
			lea	Temporary_Buffer(a5),a1
			jsr	Copy_String
			move.l	a0,Main_Pointer(a5)
			addq.l	#1,Main_Pointer(a5)
			subq.l	#1,Main_Counter(a5)
			jsr	Writer
			tst.l	Main_Counter(a5)
			beq	Chat_End
			bra	Main_Loop

Main_Random:		move.l	MAIN_NUMBER(a5),d0
			move.l	MAIN_STRINGS(a5),a0
			lea	Main_Table(a5),a3
			move.l	d0,d3
			jsr	Get_String
			beq	Chat_End
			jsr	Writer

			subq.l	#1,Main_Counter(a5)
			tst.l	Main_Counter(a5)
			beq	Chat_End

			dbf	d6,Main_Loop

Chat_End:		jsr	User

			move.l	ENDING_NUMBER(a5),d0
			move.l	ENDING_STRINGS(a5),a0
			move.l	#0,a3
			move.l	d0,d3
			jsr	Get_String
			jsr	Writer

			move.l	DOS_Base(a5),a6
			move.l	Log_Pointer(a5),d1
			jsr	_LVOClose(a6)

			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#DDF_ADDEXTENSION!DDF_CHECKACS,d1
			lea	Endchat_Filename,a0
			jsr	_LVODisplayFile(a6)

			move.l	DreamDoor_Pointer(a5),d0
			lea	Chatfooled_String,a0
			jsr	_LVOWriteLog(a6)

			bra	Free_Strings

;===[ Chat Sub-Programs ]====================================================

SysOp_Color:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	dp_DayDream+Structures(a5),a0
			add.l	#CFG_COLORSYSOP,a0
			jsr	_LVOSendString(a6)
			rts

User_Color:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	dp_DayDream+Structures(a5),a0
			add.l	#CFG_COLORUSER,a0
			jsr	_LVOSendString(a6)
			rts

Get_String:		lea	Temporary_Buffer(a5),a1
			moveq	#0,d2
			jsr	Make_Random
			move.l	d0,d1
			cmp.l	#0,a3
			beq	Find_Row
Check_Table:		tst.b	(a3,d1)
			beq	Table_Ok
			addq.l	#1,d2
			cmp.l	#3000,d2
			beq	Table_Error
			move.l	d3,d0
			jsr	Make_Random
			move.l	d0,d1
			bra	Check_Table
Table_Error:		moveq	#0,d0
			rts
Table_Ok:		move.b	#1,(a3,d1)
Find_Row:		tst.l	d0
			beq	Copy_String
Find_Linefeeds:		tst.b	(a0)
			beq	String_Ready
			cmp.b	#10,(a0)+
			bne	Find_Linefeeds
			subq.l	#1,d0
			bra	Find_Row
Copy_String:		cmp.b	#10,(a0)
			beq	String_Ready
			tst.b	(a0)
			beq	String_Ready
			cmp.w	#"%h",(a0)
			beq	Copy_Handle_Lower
			cmp.w	#"%H",(a0)
			beq	Copy_Handle_Normal
			cmp.w	#"%l",(a0)
			beq	Copy_Location_Lower
			cmp.w	#"%L",(a0)
			beq	Copy_Location_Normal
			move.b	(a0)+,(a1)+
			bra	Copy_String
String_Ready:		move.b	#10,(a1)+
			clr.b	(a1)
			moveq	#1,d0
			rts
Copy_Handle_Lower:	lea	Handlelower_String(a5),a2
			bra	Copy_String_Loop
Copy_Handle_Normal:	move.l	dp_CurrUser+Structures(a5),a2
			add.l	#USER_HANDLE,a2
			bra	Copy_String_Loop
Copy_Location_Lower:	lea	Locationlower_String(a5),a2
			bra	Copy_String_Loop
Copy_Location_Normal:	move.l	dp_CurrUser+Structures(a5),a2
			add.l	#USER_ORGANIZATION,a2
			bra	Copy_String_Loop
Copy_String_Loop:	move.b	(a2)+,(a1)+
			bne	Copy_String_Loop
			addq.l	#2,a0
			subq.l	#1,a1
			bra	Copy_String

Writer:			lea	Write_Buffer(a5),a2
			move.l	#0,Characters(a5)
			move.l	#"CF> ",(a2)+
			add.l	#4,Characters(a5)
			lea	Character_Buffer(a5),a3
			lea	Temporary_Buffer(a5),a4
			moveq	#0,d7
Writer_Loop:		move.b	(a4)+,(a3)
			beq	Writer_End
			cmp.b	#"ÿ",(a3)
			bne	Writer_Error
Check_Delay1:		cmp.b	#"d",(a4)
			bne	Check_Delay2
			add.l	#1,a4
			move.l	#10,d0
			jsr	Make_Random
			move.l	DOS_Base(a5),a6
			move.l	d0,d1
			add.l	#20,d1
			jsr	_LVODelay(a6)
			bra	Writer_Loop
Check_Delay2:		cmp.b	#"D",(a4)
			bne	Writer_Error
			add.l	#1,a4
			move.l	#20,d0
			jsr	Make_Random
			move.l	DOS_Base(a5),a6
			move.l	d0,d1
			add.l	#30,d1
			jsr	_LVODelay(a6)
			bra	Writer_Loop
Writer_Error:		move.l	#100,d0
			jsr	Make_Random
			cmp.l	ERROR_PERCENT(a5),d0
			bhi	Writer_Delay
			move.l	TYPING_VARIETY(a5),d0
			jsr	Make_Random
			lea	Character_Buffer(a5),a3
			move.l	#"z"-"a",d0
			jsr	Make_Random
			add.l	#"a",d0
			move.b	d0,(a3)
			move.l	#100,d0
			jsr	Make_Random
			cmp.l	ERROR_CORRECT(a5),d0
			bhi	Writer_Delay
			move.l	TYPING_VARIETY(a5),d0
			addq.l	#1,d0
			jsr	Make_Random
			move.l	DOS_Base(a5),a6
			move.l	d0,d1
			jsr	_LVODelay(a6)
			move.l	DreamDoor_Base(a5),a6
			jsr	SysOp_Color
			move.l	DreamDoor_Pointer(a5),d0
			move.l	a3,a0
			jsr	_LVOSendString(a6)
			move.l	TYPING_VARIETY(a5),d0
			addq.l	#5,d1
			jsr	Make_Random
			move.l	DOS_Base(a5),a6
			move.l	d0,d1
			add.l	TYPING_SPEED(a5),d1
			jsr	_LVODelay(a6)
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Backspace_String,a0
			jsr	_LVOSendString(a6)
			subq.l	#1,a4
			bra	Writer_Loop
Writer_Delay:		move.l	TYPING_VARIETY(a5),d0
			jsr	Make_Random
			move.l	DOS_Base(a5),a6
			move.l	d0,d1
			add.l	TYPING_SPEED(a5),d1
			jsr	_LVODelay(a6)
Writer_Send:		move.l	DreamDoor_Base(a5),a6
			jsr	SysOp_Color
			move.l	DreamDoor_Pointer(a5),d0
			move.l	a3,a0
			jsr	_LVOSendString(a6)
			move.b	(a3),(a2)+
			addq.l	#1,Characters(a5)
			addq.l	#1,d7
			cmp.l	#Write_Buffersize,Characters(a5)
			bcs	Writer_Rowchange
			move.l	DOS_Base(a5),a6
			move.l	Log_Pointer(a5),d1
			lea	Write_Buffer(a5),a0
			move.l	a0,d2
			move.l	Characters(a5),d3
			jsr	_LVOWrite(a6)
			move.l	#0,Characters(a5)
			lea	Write_Buffer(a5),a2
Writer_Rowchange:	cmp.l	#75,d7
			bne	Writer_Linefeed
			move.b	#10,(a2)+
			move.l	#"    ",(a2)+
			add.l	#5,Characters(a5)
			bra	Writer_Loop
Writer_Linefeed:	cmp.l	#79,d7
			bne	Writer_Loop
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Linefeed_String,a0
			jsr	_LVOSendString(a6)
			bra	Writer_Loop
Writer_End:		move.b	#10,(a2)+
			clr.b	(a2)
			addq.l	#1,Characters(a5)
			move.l	DOS_Base(a5),a6
			move.l	Log_Pointer(a5),d1
			lea	Write_Buffer(a5),a0
			move.l	a0,d2
			move.l	Characters(a5),d3
			jsr	_LVOWrite(a6)
			moveq	#1,d0
			rts

User:			lea	Write_Buffer(a5),a2
			move.l	#0,Characters(a5)
			move.l	DreamDoor_Base(a5),a6
			jsr	User_Color
			lea	Character_Buffer(a5),a3
			moveq	#0,d4
			move.l	#0,User_Linefeeds(a5)
			moveq	#0,d7
User_Fetchkey:		move.l	PAUSE_VARIETY(a5),d0
			jsr	Make_Random
			move.l	d0,d2
			add.l	PAUSE_SECONDS(a5),d2
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#HOTKEYF_CURSOR,d1
			jsr	_LVOFetchKey(a6)
User_Result:		tst.l	d0
			bmi	User_End
			beq	User_Break
			cmp.b	#3,d0
			beq	User_Break
			cmp.b	#8,d0
			beq	User_Backspace
			cmp.b	#13,d0
			beq	User_Linefeed
			move.b	d0,(a3)
			move.b	d0,(a2)+
			move.l	DreamDoor_Pointer(a5),d0
			move.l	a3,a0
			jsr	_LVOSendString(a6)
			addq.l	#1,d4
			addq.l	#1,Characters(a5)
			addq.l	#1,d7
			cmp.l	#Write_Buffersize,Characters(a5)
			bcs	User_Rowchange
			move.l	DOS_Base(a5),a6
			move.l	Log_Pointer(a5),d1
			lea	Write_Buffer(a5),a0
			move.l	a0,d2
			move.l	Characters(a5),d3
			jsr	_LVOWrite(a6)
			move.l	#0,Characters(a5)
			lea	Write_Buffer(a5),a2
User_Rowchange:		cmp.l	#79,d7
			beq	User_Linefeed
			bra	User_Fetchkey
User_Backspace:		cmp.l	#0,d7
			beq	User_Fetchkey
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Backspace_String,a0
			jsr	_LVOSendString(a6)
			subq.l	#1,d7
			subq.l	#1,Characters(a5)
			subq.l	#1,a2
			bra	User_Fetchkey
User_Linefeed:		moveq	#0,d7
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Linefeed_String,a0
			jsr	_LVOSendString(a6)
			move.b	#10,(a2)+
			addq.l	#1,Characters(a5)
			add.l	#1,User_Linefeeds(a5)
			bra	User_Fetchkey
User_Break:		move.l	#1,User_Value(a5)
User_End:		tst.l	User_Linefeeds(a5)
			bne	User_Write
			move.b	#10,(a2)+
			addq.l	#1,Characters(a5)
User_Write:		clr.b	(a2)
			move.l	DOS_Base(a5),a6
			move.l	Log_Pointer(a5),d1
			lea	Write_Buffer(a5),a0
			move.l	a0,d2
			move.l	Characters(a5),d3
			jsr	_LVOWrite(a6)
			cmp.l	#3,d4
			bhi	User_Wrote
			addq.l	#1,d5
			rts
User_Wrote:		moveq	#0,d5
			rts

Make_Linefeeds:		move.l	DOS_Base(a5),a6
			move.l	TYPING_VARIETY(a5),d0
			jsr	Make_Random
			add.l	#7,d0
			move.l	d0,d1
			jsr	_LVODelay(a6)
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Linefeed_String,a0
			jsr	_LVOSendString(a6)
			rts

;===[ Cleanup ]===============================================================

Free_Strings:		move.l	4.w,a6
			move.l	Strings_Size(a5),d0
			beq	Free_Configuration
			move.l	Strings_Pointer(a5),a1
			jsr	_LVOFreeMem(a6)

Free_Configuration:	move.l	4.w,a6
			move.l	Configuration_Size(a5),d0
			beq	Close_Door
			move.l	Configuration_Pointer(a5),a1
			jsr	_LVOFreeMem(a6)

Close_Door:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			jsr	_LVOCloseDoor(a6)

Close_DreamDoor:	move.l	4.w,a6
			move.l	DreamDoor_Base(a5),a1
			jsr	_LVOCloseLibrary(a6)

Close_DOS:		move.l	4.w,a6
			move.l	DOS_Base(a5),a1
			jsr	_LVOCloseLibrary(a6)

Free_Memory:		move.l	4.w,a6
			move.l	a5,a1
			move.l	#Variables_SIZEOF,d0
			jsr	_LVOFreeMem(a6)

Exit_Program:		movem.l	(sp)+,d2-d7/a2-a6
			moveq	#0,d0
			rts

;===[ Sub-Programs ]===========================================================

Load_File:		move.l	DOS_Base(a5),a6
			move.l	a0,d1
			move.l	#MODE_OLDFILE,d2
			jsr	_LVOOpen(a6)
			tst.l	d0
			bne	Get_File_Length
			rts
Get_File_Length:	move.l	d0,d4
			move.l	d4,d1
			moveq	#0,d2
			move.l	#OFFSET_END,d3
			jsr	_LVOSeek(a6)
			move.l	d4,d1
			moveq	#0,d2
			move.l	#OFFSET_BEGINNING,d3
			jsr	_LVOSeek(a6)
			move.l	d0,d5
			bne	Allocate_File_Buffer
			move.l	d4,d1
			jsr	_LVOClose(a6)
			rts
Allocate_File_Buffer:	move.l	4.w,a6
			addq.l	#2,d0
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			move.l	d0,d6
			bne	Read_File
			move.l	DOS_Base(a5),a6
			move.l	d4,d1
			jsr	_LVOClose(a6)
			moveq	#0,d0
			rts
Read_File:		move.l	DOS_Base(a5),a6
			move.l	d4,d1
			move.l	d6,d2
			move.l	d5,d3
			jsr	_LVORead(a6)
Close_File:		move.l	d4,d1
			jsr	_LVOClose(a6)
			addq.l	#2,d5
			moveq	#1,d0
			rts

Handle_Configuration:	moveq	#1,d4
			moveq	#1,d3
Handle_Next_Configrow:	move.l	a2,a0
			move.l	a3,a1
			tst.b	(a1)
			beq	Config_Ok
			cmp.b	#10,(a0)
			beq	Skip_Commentrow
			cmp.b	#";",(a0)
			bne	Compare_Keyword
Skip_Commentrow:	cmp.b	#0,(a2)
			beq	Missing_Linefeed
			cmp.b	#10,(a2)+
			bne	Skip_Commentrow
			addq.l	#1,d4
			bra	Handle_Next_Configrow
Compare_Keyword:	move.b	(a0)+,d0
			beq	Missing_Linefeed
			move.b	(a1)+,d1
			beq	Keyword_Found
			cmp.b	d0,d1
			beq	Compare_Keyword
			move.l	#Keyword_Error,d0
			move.l	d4,d1
			move.l	a3,d2
			lea	Configuration_Error,a0
			jsr	Send_String
			bra	Config_Error
Missing_Linefeed:	move.l	#Linefeed_Error,d0
			move.l	d4,d1
			move.l	a3,d2
			lea	Configuration_Error,a0
			jsr	Send_String
			bra	Config_Error
Keyword_Found:		move.b	(a1)+,d0
			move.l	a2,a0
			add.l	#16,a0
			cmp.b	#CFG_Switch,d0
			beq	Handle_Switch
			cmp.b	#CFG_Number,d0
			beq	Handle_Number
Handle_String:		move.l	a0,(a4)+
			bra	Next_Configrow
Handle_Switch:		move.b	(a0),d0
			beq	Missing_Linefeed
Check_YES:		cmp.b	#"Y",d0
			bne	Check_NO
			move.l	#1,(a4)+
			bra	Next_Configrow
Check_NO:		cmp.b	#"N",d0
			bne	Unknown_Switch
			move.l	#0,(a4)+
			bra	Next_Configrow
Unknown_Switch:		move.l	#Switch_Error,d0
			move.l	d4,d1
			move.l	a3,d2
			lea	Configuration_Error,a0
			jsr	Send_String
			bra	Config_Error
Handle_Number:		moveq	#0,d0
			moveq	#0,d1
			moveq	#0,d2
			cmp.b	#"-",(a0)
			bne	Next_Number
			move.l	#-1,(a4)+
			bra	Next_Configrow
Next_Number:		move.b	(a0)+,d1
			beq	Missing_Linefeed
			cmp.b	#"0",d1
			bmi	Number_Taken
			cmp.b	#"9",d1
			bhi	Number_Taken
			sub.b	#"0",d1
			mulu.w	#10,d0
			add.w	d1,d0
			moveq	#1,d2
			bra	Next_Number
Number_Taken:		tst.l	d2
			beq	Unknown_Number
			move.l	d0,(a4)+
			bra	Next_Configrow
Unknown_Number:		move.l	d1,d2
			move.l	#Number_Error,d0
			move.l	d4,d1
			move.l	a3,d2
			lea	Configuration_Error,a0
			jsr	Send_String
			bra	Config_Error
Next_Configrow:		tst.b	(a2)
			beq	Missing_Linefeed
			cmp.b	#10,(a2)
			beq	Found_Configrow
			addq.l	#1,a2
			bra	Next_Configrow
Found_Configrow:	clr.b	(a2)+
			addq.l	#1,d3
			addq.l	#1,d4
			move.l	a1,a3
			bra	Handle_Next_Configrow
Config_Ok:		moveq	#1,d0
			rts
Config_Error:		moveq	#0,d0
			rts

String_To_Number:	moveq	#0,d0
			moveq	#0,d1
Take_Next_Number:	tst.b	(a0)
			beq	Numbers_Taken
			move.b	(a0)+,d1
			cmp.b	#"0",d1
			bmi	Numbers_Taken
			cmp.b	#"9",d1
			bhi	Numbers_Taken
			sub.b	#"0",d1
			mulu.w	#10,d0
			add.w	d1,d0
			bra	Take_Next_Number
Numbers_Taken:		rts

Convert_To_Lowercase:	move.b	(a0)+,d0
			beq	String_Ended
			cmp.b	#"A",d0
			bmi	Put_To_Buffer
			cmp.b	#"Z",d0
			bhi	Put_To_Buffer
			add.b	#" ",d0
Put_To_Buffer:		move.b	d0,(a1)+
			bra	Convert_To_Lowercase
String_Ended:		clr.b	(a1)
			rts

Go_Next_Row:		tst.b	(a0)
			beq	Next_Not_Found
			cmp.b	#10,(a0)+
			bne	Go_Next_Row
Next_Not_Found:		rts

Go_String_End:		tst.b	(a0)+
			bne	Go_String_End
			subq.l	#1,a0
			rts

Send_String:		movem.l	d0-d7/a2-a4,-(sp)
			move.l	4.w,a6
			move.l	sp,a1
			lea	Put_Character,a2
			lea	Format_Buffer(a5),a3
			jsr	_LVORawDoFmt(a6)
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Format_Buffer(a5),a0
			jsr	_LVOSendString(a6)
			movem.l	(sp)+,d0-d7/a2-a4
			rts

Put_Character:		move.b	d0,(a3)+
			rts

Random_Seed:		add.l	d0,d1
			movem.l	d0/d1,Random(a5)
Long_Random:		movem.l	d2-d3,-(sp)	
			movem.l	Random(a5),d0/d1
			andi.b	#$0e,d0
			ori.b	#$20,d0
			move.l	d0,d2
			move.l	d1,d3
			add.l	d2,d2
			addx.l	d3,d3
			add.l	d2,d0
			addx.l	d3,d1
			swap	d3
			swap	d2
			move.w	d2,d3
			clr.w	d2
			add.l	d2,d0
			addx.l	d3,d1
			movem.l	d0/d1,Random(a5)
			move.l	d1,d0
			movem.l	(sp)+,d2-d3
			rts

Make_Random:		move.w	d2,-(sp)
			move.w	d0,d2
			beq.s	End_Random
			bsr	Long_Random
			clr.w	d0
			swap	d0
			divu.w	d2,d0
			clr.w	d0
			swap	d0
End_Random:		move.w	(sp)+,d2
			rts

;===[ Strings ]===============================================================

CFG_Switch		=0
CFG_Number		=1
CFG_String		=2

Configuration1_Strings:	dc.b	"PERSONALITIES",0,	CFG_Number
			dc.b	0

Configuration2_Strings:	dc.b	"PERSONALITY",0,	CFG_String
			dc.b	"STRINGS_FILE",0,	CFG_String
			dc.b	"RANDOM_MODE",0,	CFG_Number
			dc.b	"TYPING_SPEED",0,	CFG_Number
			dc.b	"TYPING_VARIETY",0,	CFG_Number
			dc.b	"ERROR_PERCENT",0,	CFG_Number
			dc.b	"ERROR_CORRECT",0,	CFG_Number
			dc.b	"PAUSE_SECONDS",0,	CFG_Number
			dc.b	"PAUSE_VARIETY",0,	CFG_Number
			dc.b	"IDLE_COUNT",0,		CFG_Number
			dc.b	"LOGFILE",0,		CFG_String
			dc.b	0

DOS_Library:		dc.b	"dos.library",0
DreamDoor_Library:	dc.b	"dreamdoor.library",0
DreamDoor_Error:	dc.b	"This door requires dreamdoor.library v5 or higher!",10,0
DreamPort_Error:	dc.b	"DayDream's doorport is not open!",10,0
Door_Usage:		dc.b	"USAGE: Chatfool <NODE>",10,0
Access_Filename:	dc.b	"Chatfool",0
Variable_Filename:	dc.b	"DreamUtils",0
Config1_Filename:	dc.b	"%sChatfool.usr%ld.CFG",0
Config2_Filename:	dc.b	"%sChatfool.n%s.dm%ld.ax%03.ld.CFG",0
Config3_Filename:	dc.b	"%sChatfool.n%s.dm%ld.CFG",0
Config4_Filename:	dc.b	"%sChatfool.n%s.ax%03.ld.CFG",0
Config5_Filename:	dc.b	"%sChatfool.dm%ld.ax%03.ld.CFG",0
Config6_Filename:	dc.b	"%sChatfool.n%s.CFG",0
Config7_Filename:	dc.b	"%sChatfool.dm%ld.CFG",0
Config8_Filename:	dc.b	"%sChatfool.ax%03.ld.CFG",0
Config9_Filename:	dc.b	"%sChatfool.CFG",0
Data_Filename:		dc.b	"Daydream:Data/ChatFool.DAT",0
FileMissing_Error:	dc.b	27,"[36mChatfool error",27,"[32m: ",27,"[36mCan't find file ",27,"[32m",34,27,"[33m%s",27,"[32m",34,27,"[36m!",10,0
Configuration_Error:	dc.b	27,"[36mError in configuration",27,"[32m: ",27,"[33m%s",27,"[36m!",10,27,"[32m(",27,"[36mrow ",27,"[33m%ld",27,"[36m, keyword ",27,"[33m%s",27,"[32m)",10,0
Keyword_Error:		dc.b	"Unknown/missing keyword",0
Switch_Error:		dc.b	"Unknown switch",0
Number_Error:		dc.b	"Bad number",0
Linefeed_Error:		dc.b	"Missing linefeed",0
Strings_Error:		dc.b	27,"[36mStrings missing in ",27,"[32m",34,27,"[33m%s",27,"[32m",34,27,"[36m strings file!",10,0
Activity1_String:	dc.b	"Paging Chatfool",0
Activity2_String:	dc.b	"Chatting with Chatfool",0
Startchat_Filename:	dc.b	"Chatfool_On",0
Endchat_Filename:	dc.b	"Chatfool_Off",0
Linefeed_String:	dc.b	10,0
Backspace_String:	dc.b	27,"[1D ",27,"[1D",0
Chatfooled_String:	dc.b	"*** Chatted with Chatfool! ***",10,0
Header_String:		incbin	"A:Chatfool/Chatfool.header"
Version_String:		dc.b	"FLID","$VER: Chatfool v1.0 by Joo/Toytools [01-Jul-97]",0

