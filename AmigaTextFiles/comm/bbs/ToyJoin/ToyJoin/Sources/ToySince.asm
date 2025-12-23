;===[ Includes & Offsets ]=========================================================

			incdir	"include:"
			include	"exec/exec.i"
			include	"dos/dos.i"
			include	"dos/datetime.i"
			include	"libraries/daydream.i"
			include	"offsets/exec_lib.i"
			include	"offsets/dos_lib.i"
			include	"offsets/dreamdoor_lib.i"

;===[ Constants ]==================================================================

DOS_Version		=37
DreamDoor_Version	=5
Max_Conferences		=64

;===[ Structures & Variables ]=====================================================

			STRUCTURE	News,0
			UWORD		NEWS_USERSLOT
			STRUCT		NEWS_LASTLOGIN,ds_SIZEOF
			STRUCT		NEWS_FILES,Max_Conferences*2
			STRUCT		NEWS_BYTES,Max_Conferences*4
			STRUCT		NEWS_FUTURE,102
			LABEL		News_SIZEOF

			STRUCTURE	Variables,0
			APTR		DOS_Base
			APTR		DreamDoor_Base
			APTR		DreamDoor_Pointer
			APTR		Node
			APTR		Datafile_Buffer
			ULONG		Datafile_Length
			ULONG		User_Pointer
			ULONG		Users
			ULONG		New_User
			APTR		Configuration_Filename
			APTR		Output_Filename
			APTR		Configuration_Buffer
			ULONG		Configuration_Length
			APTR		Output_Buffer
			STRUCT		Structures,dp_SIZEOF
			STRUCT		New_Buffer,News_SIZEOF
			STRUCT		Door_Parameters,200
			STRUCT		Format_Buffer,300
			STRUCT		Stack_Buffer,600
			STRUCT		Datetime_Buffer,dat_SIZEOF
			STRUCT		Date_String,LEN_DATSTRING
			STRUCT		Outputname_Buffer,200
			LABEL		Variables_SIZEOF

;===[ Startup ]====================================================================

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
			move.l	a4,Node(a5)
	
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
			bne	Init_Door
			move.l	DOS_Base(a5),a6
			move.l	#DreamDoor_Error,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Close_DOS

Init_Door:		move.l	DreamDoor_Base(a5),a6
			move.l	a4,a0
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

;===[ Load Datafile ]==============================================================

Open_Datafile:		move.l	DOS_Base(a5),a6
			move.l	#Data_Filename,d1
			move.l	#MODE_READWRITE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			bne	Get_Datafile_Length
			bra	Close_Door
Get_Datafile_Length:	move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_END,d3
			jsr	_LVOSeek(a6)
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_BEGINNING,d3
			jsr	_LVOSeek(a6)
			move.l	d0,d5
Allocate_Datafile:	move.l	4.w,a6
			add.l	#News_SIZEOF,d0
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			move.l	d0,d6
			bne	Read_Datafile
			move.l	DOS_Base(a5),a6
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Close_Door
Read_Datafile:		move.l	DOS_Base(a5),a6
			move.l	d4,d1
			move.l	d6,d2
			move.l	d5,d3
			jsr	_LVORead(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)
			move.l	d6,Datafile_Buffer(a5)
			move.l	d5,Datafile_Length(a5)
			add.l	#News_SIZEOF,Datafile_Length(a5)

			move.l	dp_CurrUser+Structures(a5),a0
			add.l	#USER_ACCOUNT_ID,a0
			moveq	#0,d1
			move.w	(a0),d1

			move.l	d6,a2
			move.l	d5,d2
			beq	Add_New
			divu.w	#News_SIZEOF,d2
			move.l	d2,Users(a5)
			subq.l	#1,d2

Find_User:		cmp.w	(a2),d1
			beq	Found_User
			add.l	#News_SIZEOF,a2
			dbf	d2,Find_User
Add_New:		move.w	d1,(a2)
			move.l	#1,New_User(a5)
			add.l	#1,Users(a5)
Found_User:		move.l	a2,User_Pointer(a5)

;===[ Get Online Parameters ]======================================================

			move.l	dp_DoorParams+Structures(a5),a0
			tst.b	(a0)
			beq	Open_Uploadlog
			lea	Door_Parameters(a5),a1
Copy_Parameters:	move.b	(a0)+,(a1)+
			bne	Copy_Parameters

			lea	Door_Parameters(a5),a0
			move.l	a0,Configuration_Filename(a5)
Find_Space_Character:	tst.b	(a0)
			beq	No_Output_Parameter
			cmp.b	#" ",(a0)+
			bne	Find_Space_Character
			subq.l	#1,a0
			clr.b	(a0)+

			lea	Outputname_Buffer(a5),a1
			move.l	a1,Output_Filename(a5)
Make_New:		move.b	(a0)+,(a1)+
			bne	Make_New
			subq.l	#1,a1
			move.b	#".",(a1)+
			move.l	Node(a5),a0
			move.b	(a0)+,(a1)+
			move.b	(a0)+,(a1)+
			move.b	(a0)+,(a1)+

			bra	Open_Configuration

No_Output_Parameter:	lea	Online_Usage,a0
			jsr	Send_String
			bra	Update_Datafile

;===[ Load Configuration ]=========================================================

Open_Configuration:	move.l	Configuration_Filename(a5),d1
			move.l	#MODE_OLDFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			bne	Get_Configuration_Size
			bra	Update_Datafile
Get_Configuration_Size:	move.l	d0,d4
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_END,d3
			jsr	_LVOSeek(a6)
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_BEGINNING,d3
			jsr	_LVOSeek(a6)
			move.l	d0,d5
			bne	Allocate_Configuration
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Update_Datafile
Allocate_Configuration:	move.l	4.w,a6
			move.l	d0,d3
			mulu.w	#4,d5
			move.l	d5,d0
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			move.l	d0,d6
			bne	Read_Configuration
			move.l	DOS_Base(a5),a6
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Update_Datafile
Read_Configuration:	move.l	DOS_Base(a5),a6
			move.l	d4,d1
			move.l	d6,d2
			jsr	_LVORead(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)
			move.l	d5,Configuration_Length(a5)
			move.l	d6,Configuration_Buffer(a5)
			add.l	d3,d6
			addq.l	#2,d6
			move.l	d6,Output_Buffer(a5)

;===[ Make Output ]================================================================

			move.l	DOS_Base(a5),a6
			move.l	User_Pointer(a5),a1
			add.l	#NEWS_LASTLOGIN,a1
			lea	Datetime_Buffer(a5),a2
			move.l	(a1)+,(a2)
			move.l	(a1)+,4(a2)
			move.l	(a1)+,8(a2)
			lea	Date_String(a5),a1
			move.l	a1,dat_StrDate(a2)
			move.l	a2,d1
			jsr	_LVODateToStr(a6)
			tst.l	8(a2)
			bne	Prepare_Scan
			lea	Firstcall_Text,a0
			lea	Date_String(a5),a1
Copy_Firstcall:		move.b	(a0)+,(a1)+
			bne	Copy_Firstcall

Prepare_Scan:		move.l	Configuration_Buffer(a5),a1
			lea	Date_String(a5),a2
			move.l	Output_Buffer(a5),a3
			lea	Stack_Buffer(a5),a4
			move.l	User_Pointer(a5),d3
			add.l	#NEWS_FILES,d3
			move.l	User_Pointer(a5),d4
			add.l	#NEWS_BYTES,d4

Scan_Configuration:	tst.b	(a1)
			beq	Create_Output
			cmp.b	#"%",(a1)
			beq	Found_Control
			move.b	(a1)+,(a3)+
			bra	Scan_Configuration
Found_Control:		cmp.b	#"D",1(a1)
			beq	Found_Date
			cmp.b	#"B",2(a1)
			beq	Found_Bytes
			cmp.b	#"B",3(a1)
			beq	Found_Bytes
			cmp.b	#"K",2(a1)
			beq	Found_Kilobytes
			cmp.b	#"K",3(a1)
			beq	Found_Kilobytes
			cmp.b	#"M",2(a1)
			beq	Found_Megabytes
			cmp.b	#"M",3(a1)
			beq	Found_Megabytes
			cmp.b	#"F",2(a1)
			beq	Found_Files
			cmp.b	#"F",3(a1)
			beq	Found_Files
			move.b	(a1)+,(a3)+
			bra	Scan_Configuration

Found_Date:		move.w	#"%s",(a3)+
			move.l	a2,(a4)+
			addq.l	#2,a1
			bra	Scan_Configuration

Found_Files:		move.l	#"%4.4",(a3)+
			move.w	#"ld",(a3)+
			addq.l	#1,a1
			moveq	#0,d0
			move.l	a1,a0
			jsr	String_To_Number
			move.l	a0,a1
			tst.l	d0
			beq	Free_Configuration
			subq.b	#1,d0
			mulu.w	#2,d0
			move.l	d3,a0
			moveq	#0,d1
			move.w	(a0,d0),d1
			move.l	d1,(a4)+
			bra	Scan_Configuration

Found_Bytes:		move.l	#"%8.8",(a3)+
			move.w	#"ld",(a3)+
			addq.l	#1,a1
			moveq	#0,d0
			move.l	a1,a0
			jsr	String_To_Number
			move.l	a0,a1
			tst.l	d0
			beq	Free_Configuration
			subq.b	#1,d0
			mulu.w	#4,d0
			move.l	d4,a0
			move.l	(a0,d0),(a4)+
			bra	Scan_Configuration

Found_Kilobytes:	move.l	#"%5.5",(a3)+
			move.w	#"ld",(a3)+
			addq.l	#1,a1
			moveq	#0,d0
			move.l	a1,a0
			jsr	String_To_Number
			move.l	a0,a1
			tst.l	d0
			beq	Free_Configuration
			subq.b	#1,d0
			mulu.w	#4,d0
			move.l	d4,a0
			move.l	(a0,d0),(a4)+
			bra	Scan_Configuration

Found_Megabytes:	move.l	#"%2.2",(a3)+
			move.w	#"ld",(a3)+
			move.b	#".",(a3)+
			move.l	#"%1.1",(a3)+
			move.w	#"ld",(a3)+
			addq.l	#1,a1
			moveq	#0,d0
			move.l	a1,a0
			jsr	String_To_Number
			move.l	a0,a1
			tst.l	d0
			beq	Free_Configuration
			subq.b	#1,d0
			mulu.w	#4,d0
			move.l	d4,a0
			move.l	(a0,d0),d0
			divu.w	#1024,d0
			and.l	#$ffff,d0
			divu.w	#1024,d0
			move.l	d0,d1
			and.l	#$ffff,d0
			move.l	d0,(a4)+
			swap	d1
			and.l	#$ffff,d1
			move.l	d1,(a4)+
			bra	Scan_Configuration

Create_Output:		move.l	DOS_Base(a5),a6
			move.l	Output_Filename(a5),d1
			move.l	#MODE_NEWFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			beq	Free_Configuration
			move.l	d4,d1
			move.l	Output_Buffer(a5),d2
			lea	Stack_Buffer(a5),a0
			move.l	a0,d3
			jsr	_LVOVFPrintf(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)

Free_Configuration:	move.l	4.w,a6
			move.l	Configuration_Buffer(a5),a1
			move.l	Configuration_Length(a5),d0
			jsr	_LVOFreeMem(a6)

			bra	Update_Datafile

;===[ Load Uploadlogfile ]=========================================================

Open_Uploadlog:		move.l	DOS_Base(a5),a6
			move.l	#Uploadlog_Filename,d1
			move.l	#MODE_OLDFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			beq	Update_Datafile
Get_Uploadlog_Length:	move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_END,d3
			jsr	_LVOSeek(a6)
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_BEGINNING,d3
			jsr	_LVOSeek(a6)
			move.l	d0,d5
			bne	Allocate_Uploadlog
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Update_Datafile
Allocate_Uploadlog:	move.l	4.w,a6
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			move.l	d0,d6
			bne	Read_Uploadlog
			move.l	DOS_Base(a5),a6
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Update_Datafile
Read_Uploadlog:		move.l	DOS_Base(a5),a6
			move.l	d4,d1
			move.l	d6,d2
			move.l	d5,d3
			jsr	_LVORead(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)

			move.l	d6,a3
			move.l	d5,d3

			move.l	Datafile_Buffer(a5),a4

Add_Files_Loop:		sub.l	#50,d3
			bmi	Update_Uploadlog

			move.l	a3,a1
			add.l	d3,a1
			tst.b	47(a1)
			bne	Update_Uploadlog
			move.b	#1,47(a1)

			move.l	Users(a5),d2
			subq.l	#1,d2

			move.l	a4,a2

Add_To_Users:		move.l	a2,a0
			add.l	#NEWS_FILES,a0
			moveq	#0,d1
			move.b	45(a1),d1
			subq.l	#1,d1
			mulu.w	#2,d1
			add.w	#1,(a0,d1)

			move.l	a2,a0
			add.l	#NEWS_BYTES,a0
			moveq	#0,d1
			move.b	45(a1),d1
			subq.l	#1,d1
			mulu.w	#4,d1
			move.l	34(a1),d0
			add.l	d0,(a0,d1)

			add.l	#News_SIZEOF,a2
			dbf	d2,Add_To_Users
			bra	Add_Files_Loop

Update_Uploadlog:	move.l	DOS_Base(a5),a6
			move.l	#Uploadlog_Filename,d1
			move.l	#MODE_NEWFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			beq	Free_Uploadlog
			move.l	d4,d1
			move.l	d6,d2
			move.l	d5,d3
			jsr	_LVOWrite(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)

Update_Current_User:	move.l	User_Pointer(a5),d1
			add.l	#NEWS_LASTLOGIN,d1
			jsr	_LVODateStamp(a6)

			move.l	User_Pointer(a5),a0
			add.l	#NEWS_FILES,a0
			move.l	#Max_Conferences-1,d0
Clear_Files:		clr.w	(a0)+
			dbf	d0,Clear_Files

			move.l	User_Pointer(a5),a0
			add.l	#NEWS_BYTES,a0
			move.l	#Max_Conferences-1,d0
Clear_Bytes:		clr.l	(a0)+
			dbf	d0,Clear_Bytes

Free_Uploadlog:		move.l	4.w,a6
			move.l	d6,a1
			move.l	d5,d0
			jsr	_LVOFreeMem(a6)

			bra	Update_Datafile

;===[ Cleanup ]====================================================================

Update_Datafile:	move.l	DOS_Base(a5),a6
			move.l	#Data_Filename,d1
			move.l	#MODE_NEWFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			beq	Free_Datafile
			move.l	d4,d1
			move.l	Datafile_Buffer(a5),d2
			move.l	Datafile_Length(a5),d3
			tst.l	New_User(a5)
			bne	Add_User
			sub.l	#News_SIZEOF,d3
Add_User:		jsr	_LVOWrite(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)

Free_Datafile:		move.l	4.w,a6
			move.l	Datafile_Buffer(a5),a1
			move.l	Datafile_Length(a5),d0
			beq	Close_Door
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

;===[ Sub-Programs ]===============================================================

Send_String:		movem.l	d0-d7,-(sp)
			move.l	4.w,a6
			move.l	sp,a1
			lea	Put_Character,a2
			lea	Format_Buffer(a5),a3
			jsr	_LVORawDoFmt(a6)
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Format_Buffer(a5),a0
			jsr	_LVOSendString(a6)
			movem.l	(sp)+,d0-d7
			rts

Put_Character:		move.b	d0,(a3)+
			rts

String_To_Number:	moveq	#0,d0
			moveq	#0,d1
Take_Next_Number:	move.b	(a0)+,d1
			cmp.b	#"0",d1
			bmi	Numbers_Taken
			cmp.b	#"9",d1
			bhi	Numbers_Taken
			sub.b	#"0",d1
			move.l	d0,d2
			asl.l	#3,d0
			add.l	d2,d0
			add.l	d2,d0
			add.l	d1,d0
			bra	Take_Next_Number
Numbers_Taken:		rts

;===[ Strings ]====================================================================

DOS_Library:		dc.b	"dos.library",0
DreamDoor_Library:	dc.b	"dreamdoor.library",0
DreamDoor_Error:	dc.b	"This door requires dreamdoor.library v5.0 or higher.",10,0
DreamPort_Error:	dc.b	"DayDream's doorport is not open!",10,0
Door_Usage:		dc.b	"USAGE: ToySince <NODE>",10,0
Online_Usage:		dc.b	"USAGE: ToySince <CONFIGFILE> <OUTPUTFILE>",10,0
Data_Filename:		dc.b	"DayDream:Data/ToySince.DAT",0
Uploadlog_Filename:	dc.b	"DayDream:Logfiles/Uploadlog.DAT",0
Firstcall_Text:		dc.b	"xx-xxx-xx",0
Version_String:		dc.b	"FLID","$VER: ToySince v1.0 by Joo/Toytools [01-Apr-97]",0

