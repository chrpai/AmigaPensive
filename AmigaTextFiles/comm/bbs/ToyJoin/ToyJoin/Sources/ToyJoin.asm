;===[ Program Info ]===========================================================
;
;  ToyJoin - The ultimate joinconf-door for DayDream BBS v1.29.
;
;  Version 1.0   [01-Jun-97]
;
;  Programmed by Joo/Toytools using ASM-One v1.29 by T.F.A.
;
;  If you use this source or any parts of it, please credit me!
;
;  The routines are quite basic, well I don't care as long as this works.
;
;===[ Includes & Offsets ]=====================================================

			incdir	"include:"
			include	"exec/exec.i"
			include	"dos/dos.i"
			include	"dos/datetime.i"
			include	"libraries/daydream.i"
			include	"offsets/exec_lib.i"
			include	"offsets/dos_lib.i"
			include	"offsets/dreamdoor_lib.i"

;===[ Constants ]==============================================================

DOS_Version		=37
DreamDoor_Version	=6

Prompt_Buffersize	=10
Path_Buffersize		=150
Format_Buffersize	=500
Temporary_Buffersize	=400

Max_Conferences		=64

Previous_Command	="<"
Next_Command		=">"

Key_UP			=250
Key_DOWN		=251
Key_RIGHT		=252
Key_LEFT		=253
Key_ESC			=27
Key_HELP		=63
Key_ENTER		=13
Key_BACKSPACE		=8
Key_H			=104
Key_I			=105
Key_Q			=113

;===[ Structures & Variables ]=================================================

			STRUCTURE	Conftable,0
			UBYTE		Conf_X
			UBYTE		Conf_Y
			UBYTE		Conf_Number
			UBYTE		Conf_Access
			APTR		Conf_Name
			APTR		Conf_Info
			LABEL		Conftable_SIZEOF

			STRUCTURE	Variables,0
			APTR		DOS_Base
			APTR		DreamDoor_Base
			APTR		DreamDoor_Pointer
			STRUCT		Structures,dp_SIZEOF
			APTR		Node_Number
			ULONG		Access_Level
			ULONG		Display_Mode
			ULONG		Prompt_Command
			ULONG		Mode
			APTR		Configuration_Pointer
			ULONG		Configuration_Size
			UWORD		Conference_Size
			UWORD		Messagebase_Size
			ULONG		Conferences
			ULONG		Possible_Conferences
			APTR		Names_Pointer
			ULONG		Names_Size
			APTR		Infos_Pointer
			ULONG		Infos_Size
			ULONG		Lowest_Possible
			ULONG		Highest_Possible
			ULONG		Row1_Conferences
			ULONG		Row2_Conferences
			ULONG		Highest_Conference
			LABEL		Configuration
			ULONG		HIDE_ALWAYS
			ULONG		RELATIVE_CONFS
			ULONG		USE_TWO_ROWS
			ULONG		HIDE_NOACCESS
			ULONG		USE_HOTKEYS
			ULONG		USE_CUSTOMNAMES
			ULONG		USE_INFOLINES
			ULONG		SKIP_JOINGRAPHICS
			ULONG		SKIP_NOACCESS
			ULONG		CYCLE_CONFERENCES
			ULONG		MENU_TIMEOUT
			ULONG		ROW1_X
			ULONG		ROW1_Y
			ULONG		ROW2_X
			ULONG		ROW2_Y
			ULONG		MENUROW_ACCESS
			ULONG		MENUROW_NOACCESS
			ULONG		SELECTOR_ACCESS
			ULONG		SELECTOR_NOACCESS
			ULONG		COMMANDROW_STRING
			ULONG		INFOROW_STRING
			ULONG		CURSOR_STRING
			ULONG		BACKGROUND_FILE
			ULONG		CONFNAMES_FILE
			ULONG		INFOLINES_FILE
			STRUCT		Path_Buffer,Path_Buffersize
			STRUCT		Version_Buffer,100
			STRUCT		Hide_Table,Max_Conferences+2
			STRUCT		Access_Conferences,Max_Conferences
			STRUCT		Conference_Access,Max_Conferences
			STRUCT		Conference_List,Max_Conferences
			STRUCT		Conference_Table,Max_Conferences*Conftable_SIZEOF
			STRUCT		Prompt_Buffer,Prompt_Buffersize+2
			STRUCT		Format_Buffer,Format_Buffersize
			STRUCT		Temporary_Buffer,Temporary_Buffersize
			LABEL		Variables_SIZEOF

;===[ Startup ]================================================================

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
			bne	Init_Door
			move.l	DOS_Base(a5),a6
			move.l	#DreamDoor_Error,d1
			moveq	#0,d2
			jsr	_LVOVPrintf(a6)
			bra	Close_DOS

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

			move.l	dp_CurrDisplay+Structures(a5),a0
			move.b	DISPLAY_ID(a0),d0
			move.l	d0,Display_Mode(a5)

;===[ Fastcheck Door Parameters ]==============================================

			move.l	dp_DoorParams+Structures(a5),a0
			cmp.b	#"?",(a0)
			bne	Check_Commands

Display_Prompthelp:	move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#DDF_ADDEXTENSION!DDF_CHECKCONF!DDF_CHECKACS,d1
			lea	PromptHelp_Filename,a0
			jsr	_LVODisplayFile(a6)
			bra	Close_Door

Check_Commands:		move.l	dp_DoorCmd+Structures(a5),a0

Check_Prompt_1:		cmp.b	#Previous_Command,(a0)
			bne	Check_Prompt_2
			move.l	#1,Prompt_Command(a5)
			bra	Try_Variable
Check_Prompt_2:		cmp.b	#Next_Command,(a0)
			bne	Check_Prompt_3
			move.l	#2,Prompt_Command(a5)
			bra	Try_Variable
Check_Prompt_3:		move.l	dp_DoorParams+Structures(a5),a0
			tst.b	(a0)
			beq	Get_Configpath
			move.l	#3,Prompt_Command(a5)

;===[ Get Prompt Variable ]====================================================

Try_Variable:		move.l	DOS_Base(a5),a6
			move.l	#Prompt_Filename,d1
			lea	Prompt_Buffer(a5),a2
			move.l	a2,d2
			move.l	#Prompt_Buffersize,d3
			moveq	#0,d4
			jsr	_LVOGetVar(a6)
			tst.l	d0
			bmi	Get_Configpath

Check_1:		cmp.b	#"Y",(a2)+
			bne	Check_2
			move.l	#1,RELATIVE_CONFS(a5)
Check_2:		cmp.b	#"Y",(a2)+
			bne	Check_3
			move.l	#1,SKIP_JOINGRAPHICS(a5)
Check_3:		cmp.b	#"Y",(a2)+
			bne	Check_4
			move.l	#1,SKIP_NOACCESS(a5)
Check_4:		cmp.b	#"Y",(a2)+
			bne	Prepare_Lists
			move.l	#1,CYCLE_CONFERENCES(a5)

Prepare_Lists:		jsr	Count_Conferences
			tst.l	d0
			beq	Close_Door
			move.l	#1,Mode(a5)
			bra	Find_Current_Conf

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
			move.l	d5,d1
			move.l	d6,d2
			move.l	d7,d3
			lea	Config1_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			move.l	d5,d1
			move.l	d6,d2
			lea	Config2_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			move.l	d5,d1
			move.l	d7,d2
			lea	Config3_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			move.l	d6,d1
			move.l	d7,d2
			lea	Config4_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			move.l	d5,d1
			lea	Config5_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			move.l	d6,d1
			lea	Config6_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			move.l	d7,d1
			lea	Config7_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			move.l	a4,d0
			lea	Config8_Filename,a0
			jsr	Format_Configname
			jsr	Load_File
			bne	Handle_Configuration
			lea	Temporary_Buffer(a5),a0
			move.l	a0,d0
			move.l	#FileMissing_Error,a0
			jsr	Send_String
			bra	Close_Door
			
Handle_Configuration:	move.l	d6,Configuration_Pointer(a5)
			move.l	d5,Configuration_Size(a5)
			lea	Configuration(a5),a4
			lea	Configuration_Strings,a3
			move.l	d6,a2
			moveq	#1,d4
			moveq	#1,d3
Handle_Next_Configrow:	move.l	a2,a0
			move.l	a3,a1
			tst.b	(a1)
			beq	Check_Counted
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
			bra	Free_Configuration
Missing_Linefeed:	move.l	#Linefeed_Error,d0
			move.l	d4,d1
			move.l	a3,d2
			lea	Configuration_Error,a0
			jsr	Send_String
			bra	Free_Configuration
Keyword_Found:		move.b	(a1)+,d0
			move.l	a2,a0
			add.l	#20,a0
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
			bra	Free_Configuration
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
			bra	Free_Configuration
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

;===[ Check If Counted ]=======================================================

Check_Counted:		tst.l	Mode(a5)
			beq	Not_Counted
			jsr	Make_Hidetable
			jsr	Recount_Conferences
			tst.l	d0
			bne	Check_Prompt
			bra	Free_Configuration
Not_Counted:		jsr	Make_Hidetable
			jsr	Count_Conferences
			tst.l	d0
			bne	Check_Prompt
			bra	Free_Configuration

Check_Prompt:		tst.l	Prompt_Command(a5)
			beq	Check_Custom_Names
			bra	Find_Current_Conf

;===[ Make Hide Table ]========================================================

Make_Hidetable:		move.l	HIDE_ALWAYS(a5),a0
			tst.l	(a0)
			beq	Table_Ready
			lea	Hide_Table(a5),a1
Hide_Loop:		jsr	String_To_Number
			tst.b	d0
			beq	Table_Ready
			cmp.b	#Max_Conferences,d0
			bhi	Table_Ready
			move.b	#1,(a1,d0)
			bra	Hide_Loop
Table_Ready:		rts

;===[ Count Conferences ]======================================================

Count_Conferences:	move.l	dp_VersionDat+Structures(a5),a0
			move.w	VER_CONFERENCESIZE(a0),Conference_Size(a5)
			move.w	VER_MSGBASESIZE(a0),Messagebase_Size(a5)

Recount_Conferences:	tst.l	RELATIVE_CONFS(a5)
			beq	Load_Tables
			move.l	#1,HIDE_NOACCESS(a5)
			
Load_Tables:		move.l	dp_CurrUser+Structures(a5),a0
			move.l	USER_CONFERENCEACC1(a0),d4
			lea	Hide_Table(a5),a0
			move.l	a0,d6
			lea	Conference_Table(a5),a1
			lea	Conference_Access(a5),a2
			lea	Conference_List(a5),a3
			lea	Access_Conferences(a5),a4
			moveq	#0,d3
			moveq	#1,d7
			move.l	#0,Conferences(a5)
			move.l	#0,Possible_Conferences(a5)
			move.l	#0,Lowest_Possible(a5)
			move.l	#0,Highest_Possible(a5)

Count_Loop:		move.l	dp_Conferences+Structures(a5),a0
			add.l	d3,a0
			cmp.b	#$ff,(a0)
			beq	Check_Amount
			moveq	#0,d1
			move.b	CONF_MSGBASES(a0),d1
			moveq	#0,d2
			move.b	CONF_NUMBER(a0),d2
			move.l	d2,d0
			addq.l	#1,a0
			move.l	a0,Conf_Name(a1)
			subq.l	#1,d0
			btst	d0,d4
			bne	Access_Is_Ok
			tst.l	HIDE_NOACCESS(a5)
			beq	Add_To_List
			bra	Next_Conference
Access_Is_Ok:		move.b	#1,(a2)
			move.b	d2,(a4)+
			move.b	#1,Conf_Access(a1)
			addq.l	#1,d0
			move.l	d6,a0
			tst.b	(a0,d0)
			bne	Next_Conference
			addq.l	#1,Possible_Conferences(a5)
			move.b	d2,(a3)+
			addq.l	#1,Conferences(a5)
			move.b	d2,Conf_Number(a1)
			move.l	d2,Highest_Possible(a5)
			tst.l	RELATIVE_CONFS(a5)
			bne	Store_Relative
			tst.l	Lowest_Possible(a5)
			bne	Next_Conference
			move.l	d2,Lowest_Possible(a5)
			bra	Next_Conference
Store_Relative:		move.l	#1,Lowest_Possible(a5)
			move.l	d7,Highest_Possible(a5)
			move.b	d7,Conf_Number(a1)
			addq.l	#1,d7
			bra	Next_Conference
Add_To_List:		move.b	d2,(a3)+
			addq.l	#1,Conferences(a5)
			move.b	d2,Conf_Number(a1)
Next_Conference:	add.l	#Conftable_SIZEOF,a1
			addq.l	#1,a2
			cmp.b	#Max_Conferences,d2
			bcc	Check_Amount
			cmp.b	#Max_Conferences/2,d2
			bmi	More_Conferences
			move.l	dp_CurrUser+Structures(a5),a0
			move.l	USER_CONFERENCEACC2(a0),d4
More_Conferences:	moveq	#0,d0
			move.w	Messagebase_Size(a5),d0
			mulu.w	d1,d0
			add.w	Conference_Size(a5),d0
			add.l	d0,d3
			bra	Count_Loop
Check_Amount:		tst.l	Possible_Conferences(a5)
			beq	Count_Error
			moveq	#1,d0
			rts

Count_Error:		moveq	#0,d0
			rts

;===[ Handle Prompt Commands ]=================================================

Find_Current_Conf:	moveq	#0,d0
			move.l	dp_CurrConf+Structures(a5),a0
			move.b	(a0),d0
			lea	Access_Conferences(a5),a0
			moveq	#0,d7
Find_Current_Loop:	move.b	(a0)+,d1
			beq	Check_Mode
			cmp.b	d0,d1
			beq	Prompt_Commands
			addq.l	#1,d7
			cmp.b	#Max_Conferences,d7
			bne	Find_Current_Loop
			bra	Check_Mode

Prompt_Commands:	cmp.l	#1,Possible_Conferences(a5)
			beq	Check_Mode

Test_Join_Previous:	cmp.l	#1,Prompt_Command(a5)
			bne	Test_Join_Next
			tst.l	d7
			beq	Test_Cycle_Last
			move.l	#1,d0
			move.l	dp_DoorParams+Structures(a5),a0
			cmp.b	#"0",(a0)
			beq	Join_First
			tst.b	(a0)
			beq	Join_Previous
			jsr	String_To_Number
			tst.l	d0
			beq	Check_Mode
Join_Previous:		lea	Access_Conferences(a5),a0
			sub.l	d0,d7
			bmi	Join_First
			moveq	#0,d1
			move.b	(a0,d7),d1
			bra	Join_Conference
Test_Cycle_First:	tst.l	CYCLE_CONFERENCES(a5)
			beq	Check_Mode
Join_First:		lea	Access_Conferences(a5),a0
			moveq	#0,d1
			move.b	(a0),d1
			bra	Join_Conference

Test_Join_Next:		cmp.l	#2,Prompt_Command(a5)
			bne	Test_Join_Direct
			move.l	Possible_Conferences(a5),d0
			subq.l	#1,d0
			cmp.l	d0,d7
			beq	Test_Cycle_First
			move.l	#1,d0
			move.l	dp_DoorParams+Structures(a5),a0
			cmp.b	#"0",(a0)
			beq	Join_Last
			tst.b	(a0)
			beq	Join_Next
			jsr	String_To_Number
			tst.l	d0
			beq	Check_Mode
Join_Next:		lea	Access_Conferences(a5),a0
			add.l	d0,d7
			cmp.l	Possible_Conferences(a5),d7
			bhi	Join_Last
			moveq	#0,d1
			move.b	(a0,d7),d1
			bra	Join_Conference
Test_Cycle_Last:	tst.l	CYCLE_CONFERENCES(a5)
			beq	Check_Mode
Join_Last:		lea	Access_Conferences(a5),a0
			move.l	Possible_Conferences(a5),d0
			subq.l	#1,d0
			moveq	#0,d1
			move.b	(a0,d0),d1
			bra	Join_Conference

Test_Join_Direct:	move.l	dp_DoorParams+Structures(a5),a0
			tst.b	(a0)
			beq	Check_Mode
			jsr	String_To_Number
			tst.l	d0
			beq	No_Access
			cmp.l	#Max_Conferences+1,d0
			bhi	No_Access
			tst.l	RELATIVE_CONFS(a5)
			bne	Join_Relative
			move.l	d0,d1
			lea	Conference_Access(a5),a0
			subq.l	#1,d0
			tst.b	(a0,d0)
			beq	No_Access
			bra	Join_Conference
Join_Relative:		lea	Access_Conferences(a5),a0
			subq.l	#1,d0
			move.b	(a0,d0),d0
			move.l	d0,d1
			lea	Conference_Access(a5),a0
			subq.l	#1,d0
			tst.b	(a0,d0)
			beq	No_Access
			bra	Join_Conference			
			bra	Check_Mode
No_Access:		tst.l	SKIP_NOACCESS(a5)
			bne	Check_Mode
			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#NoAccess_Error,a0
			jsr	_LVOSendString(a6)
			bra	Close_Door

Join_Conference:	move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#0,d2
			tst.l	SKIP_JOINGRAPHICS(a5)
			beq	No_Skip
			move.l	#JOINCONFF_SKIPTXT,d2
No_Skip:		jsr	_LVOJoinConference(a6)
			bra	Close_Door

Check_Mode:		move.l	#0,Prompt_Command(a5)
			tst.l	Mode(a5)
			beq	Check_Custom_Names
			bra	Get_Configpath

;===[ Load & Handle Custom Names ]=============================================

Check_Custom_Names:	tst.l	USE_CUSTOMNAMES(a5)
			beq	Check_Infolines
			tst.l	CONFNAMES_FILE(a5)
			beq	Check_Infolines

Load_Names_File:	move.l	CONFNAMES_FILE(a5),a0
			jsr	Load_File
			tst.l	d0
			bne	Update_Name_Table
			move.l	CONFNAMES_FILE(a5),d0
			move.l	#FileMissing_Error,a0
			jsr	Send_String
			bra	Free_Configuration

Update_Name_Table:	move.l	d6,a2
			lea	Conference_Table(a5),a3
			moveq	#0,d2

Take_Names:		tst.b	(a2)
			beq	End_Of_Names
			cmp.b	#10,(a2)
			beq	No_Name
			move.l	a2,Conf_Name(a3)
Search_Name_End:	addq.l	#1,a2
			tst.b	(a2)
			beq	End_Of_Names
			cmp.b	#10,(a2)
			bne	Search_Name_End
No_Name:		clr.b	(a2)+
			add.l	#Conftable_SIZEOF,a3
			addq.l	#1,d2
			cmp.l	#Max_Conferences,d2
			bne	Take_Names

End_Of_Names:		move.l	d6,Names_Pointer(a5)
			move.l	d5,Names_Size(a5)

;===[ Load & Handle Infolines ]================================================

Check_Infolines:	tst.l	USE_INFOLINES(a5)
			beq	Change_Activity
			tst.l	INFOLINES_FILE(a5)
			beq	Change_Activity

Load_Infos_File:	move.l	INFOLINES_FILE(a5),a0
			jsr	Load_File
			tst.l	d0
			bne	Make_Info_Table
			move.l	INFOLINES_FILE(a5),d0
			move.l	#FileMissing_Error,a0
			jsr	Send_String
			bra	Free_Names

Make_Info_Table:	move.l	d6,a2
			lea	Conference_Table(a5),a3
			moveq	#0,d2

Take_Infos:		tst.b	(a2)
			beq	End_Of_Infos
			cmp.b	#10,(a2)
			beq	No_Info
			move.l	a2,Conf_Info(a3)
Search_Info_End:	addq.l	#1,a2
			tst.b	(a2)
			beq	End_Of_Infos
			cmp.b	#10,(a2)
			bne	Search_Info_End
No_Info:		clr.b	(a2)+
			add.l	#Conftable_SIZEOF,a3
			addq.l	#1,d2
			cmp.l	#Max_Conferences,d2
			bne	Take_Infos

End_Of_Infos:		move.l	d6,Infos_Pointer(a5)
			move.l	d5,Infos_Size(a5)

;===[ Display Menu Background ]================================================

Change_Activity:	move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	Activity_String,a0
			jsr	_LVOChangeActivity(a6)

Do_Menu:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#DDF_SHOWERROR!DDF_ADDEXTENSION!DDF_CHECKCONF!DDF_CHECKACS,d1
			tst.l	BACKGROUND_FILE(a5)
			beq	Free_Infos
			move.l	BACKGROUND_FILE(a5),a0
			jsr	_LVODisplayFile(a6)

;===[ Draw Menu ]==============================================================

Draw_Menu:		tst.l	USE_TWO_ROWS(a5)
			bne	Draw_Two_Rows

Draw_One_Row:		lea	Conference_List(a5),a3
			lea	Conference_Table(a5),a4
			move.l	ROW1_Y(a5),d4
			move.l	ROW1_X(a5),d5
			move.l	Conferences(a5),d7
			move.l	d7,Row1_Conferences(a5)
			subq.l	#1,d7
			jsr	Draw_Menu_Loop
			bra	Display_Commandrow
			
Draw_Two_Rows:		move.l	Conferences(a5),d0
			divu.w	#2,d0
			move.l	d0,d2
			move.l	d0,d1
			and.l	#$ff,d0
			and.l	#$ff,d1
			swap	d2
			add.b	d2,d0
			move.l	d0,Row1_Conferences(a5)
			move.l	d1,Row2_Conferences(a5)
			lea	Conference_List(a5),a3
			lea	Conference_Table(a5),a4
			move.l	ROW1_Y(a5),d4
			move.l	ROW1_X(a5),d5
			move.l	Row1_Conferences(a5),d7
			subq.l	#1,d7
			jsr	Draw_Menu_Loop
			move.l	ROW2_Y(a5),d4
			move.l	ROW2_X(a5),d5
			move.l	Row2_Conferences(a5),d7
			beq	Display_Commandrow
			subq.l	#1,d7
			jsr	Draw_Menu_Loop
			bra	Display_Commandrow

Draw_Menu_Loop:		moveq	#0,d0
			move.b	(a3)+,d0
			subq.l	#1,d0
			mulu.w	#Conftable_SIZEOF,d0
			move.l	Conf_Name(a4,d0),d3
			moveq	#0,d2
			move.b	Conf_Number(a4,d0),d2
			move.b	d4,Conf_Y(a4,d0)
			move.b	d5,Conf_X(a4,d0)
			move.l	MENUROW_ACCESS(a5),a0
			tst.b	Conf_Access(a4,d0)
			bne	Draw_Menu_Row
			move.l	MENUROW_NOACCESS(a5),a0
Draw_Menu_Row:		move.l	d4,d0
			move.l	d5,d1
			jsr	Send_String
			add.l	#1,d4
			dbf	d7,Draw_Menu_Loop
			rts

;===[ Display Commandrow ]=====================================================

Display_Commandrow:	move.l	COMMANDROW_STRING(a5),a0
			move.l	Lowest_Possible(a5),d0
			move.l	Highest_Possible(a5),d1
			jsr	Send_String

;===[ Handle Menu ]============================================================

			lea	Conference_List(a5),a0
			move.l	Conferences(a5),d0
			subq.l	#1,d0
			move.b	(a0,d0),d0
			move.l	d0,Highest_Conference(a5)

Handle_Menu:		moveq	#0,d0
			move.l	dp_CurrConf+Structures(a5),a0
			move.b	(a0),d0
			lea	Conference_List(a5),a0
			moveq	#0,d7
Find_Current:		move.b	(a0)+,d1
			beq	Not_Found
			cmp.b	d0,d1
			beq	Display_Selector
			addq.l	#1,d7
			cmp.b	#Max_Conferences,d7
			bne	Find_Current
Not_Found:		moveq	#0,d7
			bra	Display_Selector

Restore_Old:		lea	Conference_List(a5),a0
			move.b	(a0,d0),d0
			subq.l	#1,d0
			lea	Conference_Table(a5),a2
			mulu.w	#Conftable_SIZEOF,d0
			add.l	d0,a2
			moveq	#0,d0
			move.b	Conf_Y(a2),d0
			moveq	#0,d1
			move.b	Conf_X(a2),d1
			moveq	#0,d2
			move.b	Conf_Number(a2),d2
			move.l	Conf_Name(a2),d3
			move.l	MENUROW_ACCESS(a5),a0
			tst.b	Conf_Access(a2)
			bne	Restore_Old_Line
			move.l	MENUROW_NOACCESS(a5),a0
Restore_Old_Line:	jsr	Send_String

Display_Selector:	lea	Conference_List(a5),a0
			moveq	#0,d0
			move.b	(a0,d7),d0
			subq.l	#1,d0
			lea	Conference_Table(a5),a2
			mulu.w	#Conftable_SIZEOF,d0
			add.l	d0,a2
			moveq	#0,d0
			move.b	Conf_Y(a2),d0
			moveq	#0,d1
			move.b	Conf_X(a2),d1
			moveq	#0,d2
			move.b	Conf_Number(a2),d2
			move.l	Conf_Name(a2),d3
			move.l	SELECTOR_ACCESS(a5),a0
			tst.b	Conf_Access(a2)
			bne	Write_Selector
			move.l	SELECTOR_NOACCESS(a5),a0
Write_Selector:		jsr	Send_String

Display_Infoline:	move.l	Conf_Info(a2),d0
			beq	Display_Number
			move.l	INFOROW_STRING(a5),a0
			jsr	Send_String

Display_Number:		moveq	#0,d0
			move.b	Conf_Number(a2),d0
			move.l	CURSOR_STRING(a5),a0
			jsr	Send_String

			move.l	#$f,d6

Handle_Keys:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#HOTKEYF_CURSOR,d1
			move.l	MENU_TIMEOUT(a5),d2
			bmi	No_Timeout
			jsr	_LVOFetchKey(a6)
			bra	Check_Result
No_Timeout:		jsr	_LVOGetKey(a6)

Check_Result:		cmp.l	#0,d0
			bhi	Test_Cursor_Up
			bra	Quit

Test_Cursor_Up:		cmp.b	#Key_UP,d0
			bne	Test_Cursor_Down
			cmp.l	#0,d7
			beq	Go_To_Last
			move.l	d7,d0
			subq.l	#1,d7
			bra	Restore_Old
Go_To_Last:		move.l	d7,d0
			move.l	Conferences(a5),d7
			subq.l	#1,d7
			bra	Restore_Old

Test_Cursor_Down:	cmp.b	#Key_DOWN,d0
			bne	Test_Cursor_Left
			move.l	Conferences(a5),d0
			subq.l	#1,d0
			cmp.l	d0,d7
			beq	Go_To_First
			move.l	d7,d0
			addq.l	#1,d7
			bra	Restore_Old
Go_To_First:		move.l	d7,d0
			moveq	#0,d7
			bra	Restore_Old

Test_Cursor_Left:	cmp.b	#Key_LEFT,d0
			bne	Test_Cursor_Right
			tst.l	Row2_Conferences(a5)
			beq	Go_To_First
			move.l	Row1_Conferences(a5),d0
			subq.l	#1,d0
			cmp.l	d0,d7
			bls	Cursor_Right
Cursor_Left:		move.l	d7,d0
			sub.l	Row1_Conferences(a5),d7
			bra	Restore_Old

Test_Cursor_Right:	cmp.b	#Key_RIGHT,d0
			bne	Test_Enter
			tst.l	Row2_Conferences(a5)
			beq	Go_To_Last
			move.l	Row1_Conferences(a5),d0
			subq.l	#1,d0
			cmp.l	d0,d7
			bhi	Cursor_Left
Cursor_Right:		move.l	d7,d0
			add.l	Row1_Conferences(a5),d7
			move.l	Conferences(a5),d1
			subq.l	#1,d1
			cmp.l	d1,d7
			bls	Restore_Old
			move.l	d1,d7
			bra	Restore_Old

Test_Enter:		cmp.w	#Key_ENTER,d0
			bne	Test_Numbers
			cmp.b	#$f,d6
			beq	Selector_Join
			cmp.b	#$ff,d6
			beq	Selector_Join
Number_Join:		move.l	d6,d3
			beq	Handle_Keys
			moveq	#1,d4
			lea	Conference_Table(a5),a0
Find_Real:		move.b	Conf_Number(a0),d0
			cmp.b	d3,d0
			beq	Found_Real
			addq.l	#1,d4
			cmp.l	#Max_Conferences,d4
			beq	Display_Number
			add.l	#Conftable_SIZEOF,a0
			bra	Find_Real
Found_Real:		move.l	d4,d3
			bra	Check_Access
Selector_Join:		lea	Conference_List(a5),a0
			moveq	#0,d3
			move.b	(a0,d7),d3
Check_Access:		move.l	d3,d0
			subq.l	#1,d0
			lea	Conference_Table(a5),a2
			mulu.w	#Conftable_SIZEOF,d0
			add.l	d0,a2
			tst.b	Conf_Access(a2)
			beq	Handle_Keys
Do_Join:		move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	ClearScreen_String,a0
			jsr	_LVOSendString(a6)
			move.l	d3,d1
			moveq	#0,d2
			move.l	DreamDoor_Pointer(a5),d0
			jsr	_LVOJoinConference(a6)
			bra	Free_Infos

Test_Numbers:		cmp.b	#"0",d0
			bmi	Test_Backspace
			cmp.b	#"9",d0
			bhi	Test_Backspace
			sub.b	#"0",d0
			cmp.l	#$ff,d6
			beq	First_Time
			cmp.l	#$f,d6
			bne	Second_Time
			move.l	d0,d6
			lea	Backspaced1_String,a0
			jsr	Send_String
			bra	Handle_Keys
First_Time:		move.l	d0,d6
			lea	Pressed_String,a0
			jsr	Send_String
			bra	Handle_Keys
Second_Time:		move.l	d6,d5
			move.l	#$f,d6
			mulu.w	#10,d5
			add.l	d0,d5
			tst.l	d5
			beq	Display_Number
			move.l	Highest_Conference(a5),d1
			cmp.l	d5,d1
			bcs	Display_Number
			moveq	#1,d4
			lea	Conference_Table(a5),a0
Find_Conference:	move.b	Conf_Number(a0),d0
			cmp.b	d5,d0
			beq	Found_Conference
			addq.l	#1,d4
			cmp.l	#Max_Conferences,d4
			beq	Display_Number
			add.l	#Conftable_SIZEOF,a0
			bra	Find_Conference
Found_Conference:	lea	Conference_List(a5),a0
			move.l	d4,d5
			move.l	d7,d0
			moveq	#0,d4
Find_Place:		move.b	(a0)+,d1
			beq	Display_Number
			cmp.b	d5,d1
			beq	Found_Place
			addq.l	#1,d4
			cmp.l	#Max_Conferences,d4
			beq	Display_Number
			bra	Find_Place
Found_Place:		move.l	d4,d7
			tst.l	USE_HOTKEYS(a5)
			beq	Restore_Old
			lea	Conference_List(a5),a0
			moveq	#0,d3
			move.b	(a0,d7),d3
			move.l	d3,d1
			subq.l	#1,d1
			lea	Conference_Table(a5),a2
			mulu.w	#Conftable_SIZEOF,d1
			add.l	d1,a2
			tst.b	Conf_Access(a2)
			beq	Restore_Old
			bra	Do_Join

Test_Backspace:		cmp.b	#Key_BACKSPACE,d0
			bne	Test_Info_1
			cmp.l	#$ff,d6
			beq	Handle_Keys
			cmp.l	#$f,d6
			bne	Second_Backspace
First_Backspace:	lea	Conference_List(a5),a0
			moveq	#0,d1
			move.b	(a0,d7),d1
			lea	Conference_Table(a5),a0
			subq.l	#1,d1
			mulu.w	#Conftable_SIZEOF,d1
			moveq	#0,d0
			move.b	Conf_Number(a0,d1),d0
			divu.w	#10,d0
			and.l	#$f,d0
			move.l	d0,d6
			lea	Backspaced1_String,a0
			jsr	Send_String
			bra	Handle_Keys
Second_Backspace:	lea	Backspaced2_String,a0
			jsr	Send_String
			move.l	#$ff,d6
			bra	Handle_Keys

Test_Info_1:		cmp.b	#Key_I,d0
			bne	Test_Info_2
			bra	Display_Confhelp
Test_Info_2:		cmp.b	#Key_I-32,d0
			bne	Test_Help_1
Display_Confhelp:	move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#DDF_ADDEXTENSION!DDF_CHECKCONF!DDF_CHECKACS,d1
			lea	ConfHelp_Filename,a0
			jsr	_LVODisplayFile(a6)
			move.l	DreamDoor_Pointer(a5),d0
			moveq	#0,d1
			move.l	#30,d2
			jsr	_LVOFetchKey(a6)
			bra	Do_Menu

Test_Help_1:		cmp.b	#Key_HELP,d0
			bne	Test_Help_2
			bra	Display_Menuhelp
Test_Help_2:		cmp.b	#Key_H,d0
			bne	Test_Help_3
			bra	Display_Menuhelp
Test_Help_3:		cmp.b	#Key_H-32,d0
			bne	Test_Quit_1
Display_Menuhelp:	move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			move.l	#DDF_ADDEXTENSION!DDF_CHECKCONF!DDF_CHECKACS,d1
			lea	MenuHelp_Filename,a0
			jsr	_LVODisplayFile(a6)
			move.l	DreamDoor_Pointer(a5),d0
			moveq	#0,d1
			move.l	#30,d2
			jsr	_LVOFetchKey(a6)
			bra	Do_Menu

Test_Quit_1:		cmp.b	#Key_ESC,d0
			bne	Test_Quit_2
			bra	Quit
Test_Quit_2:		cmp.b	#Key_Q,d0
			bne	Test_Quit_3
			bra	Quit
Test_Quit_3:		cmp.b	#Key_Q-32,d0
			bne	Handle_Keys
Quit:			move.l	DreamDoor_Base(a5),a6
			move.l	DreamDoor_Pointer(a5),d0
			lea	ClearScreen_String,a0
			jsr	_LVOSendString(a6)

;===[ Cleanup ]================================================================

Free_Infos:		move.l	4.w,a6
			move.l	Infos_Size(a5),d0
			beq	Free_Names
			move.l	Infos_Pointer(a5),a1
			jsr	_LVOFreeMem(a6)

Free_Names:		move.l	4.w,a6
			move.l	Names_Size(a5),d0
			beq	Free_Configuration
			move.l	Names_Pointer(a5),a1
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

String_To_Number:	moveq	#0,d0
			moveq	#0,d1
Take_Next_Number:	move.b	(a0)+,d1
			cmp.b	#"0",d1
			bmi	Numbers_Taken
			cmp.b	#"9",d1
			bhi	Numbers_Taken
			sub.b	#"0",d1
			mulu.w	#10,d0
			add.b	d1,d0
			bra	Take_Next_Number
Numbers_Taken:		rts

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

;===[ Strings ]================================================================

CFG_Switch		=0
CFG_Number		=1
CFG_String		=2

Configuration_Strings:	dc.b	"HIDE_ALWAYS",0,	CFG_String
			dc.b	"RELATIVE_CONFS",0,	CFG_Switch
			dc.b	"USE_TWO_ROWS",0,	CFG_Switch
			dc.b	"HIDE_NOACCESS",0,	CFG_Switch
			dc.b	"USE_HOTKEYS",0,	CFG_Switch
			dc.b	"USE_CUSTOMNAMES",0,	CFG_Switch
			dc.b	"USE_INFOLINES",0,	CFG_Switch
			dc.b	"SKIP_JOINGRAPHICS",0,	CFG_Switch
			dc.b	"SKIP_NOACCESS",0,	CFG_Switch
			dc.b	"CYCLE_CONFERENCES",0,	CFG_Switch
			dc.b	"MENU_TIMEOUT",0,	CFG_Number
			dc.b	"ROW1_X_COORDINATE",0,	CFG_Number
			dc.b	"ROW1_Y_COORDINATE",0,	CFG_Number
			dc.b	"ROW2_X_COORDINATE",0,	CFG_Number
			dc.b	"ROW2_Y_COORDINATE",0,	CFG_Number
			dc.b	"MENUROW_ACCESS",0,	CFG_String
			dc.b	"MENUROW_NOACCESS",0,	CFG_String
			dc.b	"SELECTOR_ACCESS",0,	CFG_String
			dc.b	"SELECTOR_NOACCESS",0,	CFG_String
			dc.b	"COMMANDROW_STRING",0,	CFG_String
			dc.b	"INFOROW_STRING",0,	CFG_String
			dc.b	"CURSOR_STRING",0,	CFG_String
			dc.b	"BACKGROUND_FILE",0,	CFG_String
			dc.b	"CONFNAMES_FILE",0,	CFG_String
			dc.b	"INFOLINES_FILE",0,	CFG_String
			dc.b	0

DOS_Library:		dc.b	"dos.library",0
DreamDoor_Library:	dc.b	"dreamdoor.library",0
DreamDoor_Error:	dc.b	"This door requires dreamdoor.library v6.0 or higher.",10,0
DreamPort_Error:	dc.b	"DayDream's doorport is not open!",10,0
Door_Usage:		dc.b	"USAGE: ToyJoin <NODE>",10,0
Prompt_Filename:	dc.b	"ToyJoin",0
Variable_Filename:	dc.b	"DreamUtils",0
Config1_Filename:	dc.b	"%sToyJoin.n%s.dm%ld.ax%03.ld.CFG",0
Config2_Filename:	dc.b	"%sToyJoin.n%s.dm%ld.CFG",0
Config3_Filename:	dc.b	"%sToyJoin.n%s.ax%03.ld.CFG",0
Config4_Filename:	dc.b	"%sToyJoin.dm%ld.ax%03.ld.CFG",0
Config5_Filename:	dc.b	"%sToyJoin.n%s.CFG",0
Config6_Filename:	dc.b	"%sToyJoin.dm%ld.CFG",0
Config7_Filename:	dc.b	"%sToyJoin.ax%03.ld.CFG",0
Config8_Filename:	dc.b	"%sToyJoin.CFG",0
PromptHelp_Filename:	dc.b	"ToyJoin_PromptHelp",0
MenuHelp_Filename:	dc.b	"ToyJoin_MenuHelp",0
ConfHelp_Filename:	dc.b	"ToyJoin_ConfHelp",0
FileMissing_Error:	dc.b	27,"[36mToyJoin error",27,"[32m: ",27,"[36mCan't find file ",27,"[32m",34,27,"[33m%s",27,"[32m",34,27,"[36m!",10,0
Configuration_Error:	dc.b	27,"[36mError in configuration",27,"[32m: ",27,"[33m%s",27,"[36m!",10,27,"[32m(",27,"[36mrow ",27,"[33m%ld",27,"[36m, keyword ",27,"[33m%s",27,"[32m)",10,0
Keyword_Error:		dc.b	"Unknown/missing keyword",0
Switch_Error:		dc.b	"Unknown switch",0
Number_Error:		dc.b	"Bad number",0
Linefeed_Error:		dc.b	"Missing linefeed",0
NoAccess_Error:		dc.b	27,"[36mYou have no access to that conference!",10,0
Activity_String:	dc.b	"ToyJoin",0
Pressed_String:		dc.b	"%1.1ld",0
Backspaced1_String:	dc.b	27,"[2D%1.1ld ",27,"[1D",0
Backspaced2_String:	dc.b	27,"[1D ",27,"[1D",0
ClearScreen_String:	dc.b	12,0
Version_String:		dc.b	"FLID","$VER: ToyJoin v1.0 by Joo/Toytools [01-Jun-97]",0

