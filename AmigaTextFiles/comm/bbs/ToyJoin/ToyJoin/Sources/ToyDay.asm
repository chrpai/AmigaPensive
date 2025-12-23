;===[ Includes & Offsets ]====================================================

			incdir	"include:"
			include	"exec/exec.i"
			include	"dos/dos.i"
			include	"offsets/exec_lib.i"
			include	"offsets/dos_lib.i"

;===[ Constants ]=============================================================

DOS_Version		=37
Max_Conferences		=64

;===[ Structures & Variables ]================================================

			STRUCTURE	Variables,0
			APTR		DOS_Base
			LABEL		Arguments_Array
			ULONG		CONFIGNAME
			ULONG		OUTPUTNAME
			APTR		Arguments_Pointer
			APTR		Configuration_Buffer
			ULONG		Configuration_Length
			ULONG		Configuration_Size
			APTR		Output_Buffer
			APTR		Logfile_Buffer
			ULONG		Logfile_Length
			STRUCT		Date_Stamp,ds_SIZEOF
			STRUCT		Bytes_And_Files,Max_Conferences*4*4
			STRUCT		Stack_Buffer,Max_Conferences*4*4
			LABEL		Variables_SIZEOF

;===[ Startup ]===============================================================

			movem.l	d2-d7/a2-a6,-(sp)

Allocate_Memory:	move.l	4.w,a6
			move.l	#Variables_SIZEOF,d0
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			tst.l	d0
			beq	Exit_Program
			move.l	d0,a5

Open_DOS:		lea	DOS_Library,a1
			moveq	#DOS_Version,d0
			jsr	_LVOOpenLibrary(a6)
			move.l	d0,DOS_Base(a5)
			beq	Free_Memory

;===[ Check Arguments ]=======================================================

Get_Arguments:		move.l	DOS_Base(a5),a6
			move.l	#Arguments_String,d1
			lea	Arguments_Array(a5),a2
			move.l	a2,d2
			moveq	#0,d3
			jsr	_LVOReadArgs(a6)
			move.l	d0,Arguments_Pointer(a5)

Test_Argument_1:	tst.l	(a2)
			bne	Test_Argument_2
			move.l	#Usage_String,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Free_Arguments
Test_Argument_2:	tst.l	4(a2)
			bne	Open_Configuration
			move.l	#Usage_String,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Free_Arguments

;===[ Open Configfile & Uploadlog ]===========================================

Open_Configuration:	move.l	CONFIGNAME(a5),d1
			move.l	#MODE_OLDFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			bne	Get_Configuration_Size
			move.l	CONFIGNAME(a5),d0
			movem.l	d0,-(sp)
			move.l	#Configfile_Error,d1
			move.l	sp,d2
			jsr	_LVOVPrintf(a6)
			movem.l	(sp)+,d0
			bra	Free_Arguments
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
			bra	Free_Arguments
Allocate_Configuration:	move.l	4.w,a6
			move.l	d0,d3
			mulu.w	#3,d5
			move.l	d5,d0
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			move.l	d0,d6
			bne	Read_Configuration
			move.l	DOS_Base(a5),a6
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Free_Arguments
Read_Configuration:	move.l	DOS_Base(a5),a6
			move.l	d4,d1
			move.l	d6,d2
			jsr	_LVORead(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)
			move.l	d5,Configuration_Length(a5)
			move.l	d6,Configuration_Buffer(a5)
			move.l	d3,Configuration_Size(a5)
			add.l	d3,d6
			move.l	d6,Output_Buffer(a5)

Open_Logfile:		move.l	DOS_Base(a5),a6
			move.l	#UploadLog_Filename,d1
			move.l	#MODE_OLDFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			beq	Prepare_Output
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_END,d3
			jsr	_LVOSeek(a6)
			move.l	d4,d1
			moveq	#0,d2
			moveq	#OFFSET_BEGINNING,d3
			jsr	_LVOSeek(a6)
			move.l	d0,d5
			bne	Allocate_Logbuffer
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Prepare_Output
Allocate_Logbuffer:	move.l	4.w,a6
			move.l	#MEMF_PUBLIC!MEMF_CLEAR,d1
			jsr	_LVOAllocMem(a6)
			move.l	d0,d6
			bne	Read_Logfile
			move.l	DOS_Base(a5),a6
			move.l	d4,d1
			jsr	_LVOClose(a6)
			bra	Free_Configuration
Read_Logfile:		move.l	DOS_Base(a5),a6
			move.l	d4,d1
			move.l	d6,d2
			move.l	d5,d3
			jsr	_LVORead(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)
			move.l	d5,Logfile_Length(a5)
			move.l	d6,Logfile_Buffer(a5)
			move.l	d5,d6
			beq	Free_Logfile
			divu.w	#50,d6

;===[ Scan Logfile ]==========================================================

Get_Current_Time:	move.l	DOS_Base(a5),a6
			lea	Date_Stamp(a5),a2
			move.l	a2,d1
			jsr	_LVODateStamp(a6)
			move.l	(a2)+,d5
			mulu.w	#(60*60*24)/2,d5
			add.l	d5,d5

			move.l	Logfile_Buffer(a5),a2
			add.l	Logfile_Length(a5),a2
			lea	Bytes_And_Files(a5),a3
	
Scan_Logfile:		sub.l	#50,a2
			move.l	38(a2),d0
			cmp.l	d5,d0
			bhi	Todays_Upload
			bra	Prepare_Output
Todays_Upload:		moveq	#0,d0
			move.b	45(a2),d0
			move.l	a3,a1
			subq.b	#1,d0
			mulu.w	#8,d0
			add.l	d0,a1
			move.l	34(a2),d0
			add.l	d0,(a1)
			add.l	#1,4(a1)
			subq.l	#1,d6
			tst.l	d6
			beq	Prepare_Output
			bra	Scan_Logfile
			
Prepare_Output:		move.l	Configuration_Buffer(a5),a2
			move.l	Output_Buffer(a5),a3
			move.l	Configuration_Size(a5),d2
			lea	Bytes_And_Files(a5),a4
			lea	Stack_Buffer(a5),a1

Scan_Configuration:	cmp.l	#0,d2
			bhi	Continue_Scan
			clr.b	(a3)
			bra	Make_Report
Continue_Scan:		cmp.b	#"%",(a2)
			beq	Found_Control
Normal_Text:		move.b	(a2)+,(a3)+
			subq.l	#1,d2
			bra	Scan_Configuration
Found_Control:		cmp.b	#"B",2(a2)
			beq	Bytes_1
			cmp.b	#"B",3(a2)
			beq	Bytes_2
			cmp.b	#"K",2(a2)
			beq	Kilobytes_1
			cmp.b	#"K",3(a2)
			beq	Kilobytes_2
			cmp.b	#"M",2(a2)
			beq	Megabytes_1
			cmp.b	#"M",3(a2)
			beq	Megabytes_2
			cmp.b	#"F",2(a2)
			beq	Files_1
			cmp.b	#"F",3(a2)
			beq	Files_2
			bra	Normal_Text
			
Files_1:		move.l	#"%3.l",(a3)+
			move.b	#"d",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	4(a0),(a1)+
			addq.l	#2,a2
			subq.l	#3,d2
			bra	Scan_Configuration
Files_2:		move.l	#"%3.l",(a3)+
			move.b	#"d",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			mulu.w	#10,d0
			move.l	d0,d1
			addq.l	#1,a2
			move.b	(a2),d0
			sub.b	#"0",d0
			add.l	d1,d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	4(a0),(a1)+
			addq.l	#2,a2
			subq.l	#4,d2
			bra	Scan_Configuration
Bytes_1:		move.l	#"%8.l",(a3)+
			move.b	#"d",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	(a0),(a1)+
			addq.l	#2,a2
			subq.l	#3,d2
			bra	Scan_Configuration
Bytes_2:		move.l	#"%8.l",(a3)+
			move.b	#"d",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			mulu.w	#10,d0
			move.l	d0,d1
			addq.l	#1,a2
			move.b	(a2),d0
			sub.b	#"0",d0
			add.l	d1,d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	(a0),(a1)+
			addq.l	#2,a2
			subq.l	#4,d2
			bra	Scan_Configuration
Kilobytes_1:		move.l	#"%5.l",(a3)+
			move.b	#"d",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	(a0),d0
			asr.l	#8,d0
			asr.l	#2,d0
			move.l	d0,(a1)+
			addq.l	#2,a2
			subq.l	#3,d2
			bra	Scan_Configuration
Kilobytes_2:		move.l	#"%5.l",(a3)+
			move.b	#"d",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			mulu.w	#10,d0
			move.l	d0,d1
			addq.l	#1,a2
			move.b	(a2),d0
			sub.b	#"0",d0
			add.l	d1,d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	(a0),d0
			asr.l	#8,d0
			asr.l	#2,d0
			move.l	d0,(a1)+
			addq.l	#2,a2
			subq.l	#4,d2
			bra	Scan_Configuration
Megabytes_1:		move.l	#"%2.2",(a3)+
			move.w	#"ld",(a3)+
			move.b	#".",(a3)+
			move.l	#"%1.1",(a3)+
			move.w	#"ld",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	(a0),d0
			divu.w	#1024,d0
			and.l	#$ffff,d0
			divu.w	#1024,d0
			move.l	d0,d1
			and.l	#$ffff,d0
			move.l	d0,(a1)+
			swap	d1
			and.l	#$ffff,d1
			move.l	d1,(a1)+
			addq.l	#2,a2
			subq.l	#3,d2
			bra	Scan_Configuration
Megabytes_2:		move.l	#"%2.2",(a3)+
			move.w	#"ld",(a3)+
			move.b	#".",(a3)+
			move.l	#"%1.1",(a3)+
			move.w	#"ld",(a3)+
			addq.l	#1,a2
			moveq	#0,d0
			move.b	(a2),d0
			sub.b	#"0",d0
			mulu.w	#10,d0
			move.l	d0,d1
			addq.l	#1,a2
			move.b	(a2),d0
			sub.b	#"0",d0
			add.l	d1,d0
			subq.b	#1,d0
			mulu.w	#8,d0
			move.l	a4,a0
			add.l	d0,a0
			move.l	(a0),d0
			divu.w	#1024,d0
			and.l	#$ffff,d0
			divu.w	#1024,d0
			move.l	d0,d1
			and.l	#$ffff,d0
			move.l	d0,(a1)+
			swap	d1
			and.l	#$ffff,d1
			move.l	d1,(a1)+
			addq.l	#2,a2
			subq.l	#4,d2
			bra	Scan_Configuration

Make_Report:		move.l	OUTPUTNAME(a5),d1
			move.l	#MODE_NEWFILE,d2
			jsr	_LVOOpen(a6)
			move.l	d0,d4
			bne	Write_Report
			move.l	OUTPUTNAME(a5),d0
			movem.l	d0,-(sp)
			move.l	#Outputfile_Error,d1
			move.l	sp,d2
			jsr	_LVOVPrintf(a6)
			movem.l	(sp)+,d0
			bra	Free_Logfile
Write_Report:		move.l	d4,d1
			move.l	Output_Buffer(a5),d2
			lea	Stack_Buffer(a5),a0
			move.l	a0,d3
			jsr	_LVOVFPrintf(a6)
			move.l	d4,d1
			jsr	_LVOClose(a6)

;===[ Cleanup ]===============================================================

Free_Logfile:		move.l	4.w,a6
			move.l	Logfile_Length(a5),d0
			beq	Free_Configuration
			move.l	Logfile_Buffer(a5),a1
			jsr	_LVOFreeMem(a6)

Free_Configuration:	move.l	4.w,a6
			move.l	Configuration_Length(a5),d0
			beq	Free_Arguments
			move.l	Configuration_Buffer(a5),a1
			jsr	_LVOFreeMem(a6)

Free_Arguments:		move.l	DOS_Base(a5),a6
			move.l	Arguments_Pointer(a5),d1
			beq	Close_DOS
			jsr	_LVOFreeArgs(a6)

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

;===[ Strings ]===============================================================

DOS_Library:		dc.b	"dos.library",0
Arguments_String:	dc.b	"CONFIGFILE/A,OUTPUTFILE/A",0
Usage_String:		dc.b	"USAGE: ToyDay <CONFIGFILE> <OUTPUTFILE>",10,0
UploadLog_Filename:	dc.b	"DayDream:Logfiles/UploadLog.DAT",0
Configfile_Error:	dc.b	"ToyDay: Can't open configuration file ",34,"%s",34,"!",10,0
Outputfile_Error:	dc.b	"ToyDay: Can't open output file ",34,"%s",34,"!",10,0
Version_String:		dc.b	"FLID","$VER: ToyDay v1.0 by Joo/Toytools [01-Apr-97]",0

