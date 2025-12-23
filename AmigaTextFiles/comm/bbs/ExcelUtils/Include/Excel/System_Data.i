 STRUCTURE System_Data,0
	STRUCT	exsd_System_Name,41
	STRUCT	exsd_Sysop_Name,21
	STRUCT	exsd_LocalEditor,41		; Local editor
	BYTE	exsd_KludgeFill00

	LONG	exsd_Total_Users		; Total User Records Used
	LONG	exsd_Active_Users		; Total User Still Active
	LONG	exsd_Serial_Number		; Highest Serial Number
	LONG	exsd_Total_Calls		; Total Logged Calls

	LONG	exsd_Base_Serial_Number		; Highest base serial # 

	LONG	exsd_System_Bits		; System attributes 

	STRUCT	exsd_System_Passwd,21		; Private System Password
	STRUCT	exsd_System_Font,32		; Font for BBS to use
	STRUCT	exsd_Net_Address,41		; Network Address
	STRUCT	exsd_Default_Language,21	; not in use
	BYTE	exsd_KludgeFill01
	
	LONG	exsd_Font_Size			; The size of the font

	STRUCT	exsd_ColorSpec,cs_SIZEOF*17	; Sysop Definable Color Specs

	BYTE	exsd_Default_Color
	BYTE	exsd_Default_BarColor

	STRUCT	exsd_Path,15*128		; The System Path's
						; 0 = Data
						; 1 = Text
						; 2 = Mail
						; 3 = MBase
						; 4 = FBase
						; 5 = TBase
						; 6 = PBase
						; 7 = News
						; 8 = UUNEWS:
						; 9 = Main BBS
						; 10 = UUMAIL:

	STRUCT	exsd_Default_Country,4		; Default Country
	STRUCT	exsd_Default_State,3		; Default State
	STRUCT	exsd_Default_AreaCode,4		; Default Area Code
	BYTE	exsd_KludgeFill02

	LONG	exsd_SetUp			;  Setup Date
	LONG	exsd_Reset			;  last reset date

	LONG	exsd_NewUsers			;  newusers waiting
	LONG	exsd_Call_Reset			;  calls since last reset

	LONG	exsd_PostIDs			;  Serial ID of posted items

	LONG	exsd_MaxRooms			; Configured Conference Rooms

	LONG	exsd_Default_EdLines		; Default editor lines

	LONG	exsd_Config			; Config Bits

	WORD	exsd_Feedbacks			; New Feedbacks waiting
	WORD	exsd_Emails			; New emails sent to ID#1 

	LONG	exsd_Total_Files		; Total Files on system 
	LONG	exsd_Total_Posts		; Total Posts on System

	LONG	exsd_FilesSince			; Files up'd since reset
	LONG	exsd_PostsSince			; Posts since rest

	LONG	exsd_CaptureBufferSize		; Size for console buffer

	WORD	exsd_ShellPasswd		; password for the shell
	WORD	exsd_padding			; 2 byte pad

	LONG	exsd_MaxBatchItems
	LONG	exsd_BatchHoldDays

	STRUCT	exsd_Reserved1,5

	STRUCT	exsd_System_Location,31
	STRUCT	exsd_System_Number,31
	BYTE	exsd_KludgeFill03

	LONG	exsd_UserReset
	LONG	exsd_DefaultProto
	LONG	exsd_DefaultHelp
	LONG	exsd_DefaultHeader
	LONG	exsd_flags

	STRUCT	exsd_SysopName,31
	STRUCT	exsd_expansion1,81
	STRUCT	exsd_expansion2,81
	STRUCT	exsd_NetMailPath,81

	LONG	exsd_ScreenFlags		; The Screen bits

	STRUCT	exsd_Translation,24

	LONG	exsd_ExtFlags			; System Ext Flags

	LONG	exsd_CDROMPath			; Path ID# for buffering

	STRUCT	exsd_QWKID,3			; Two-letter QWK packet ID
	BYTE	exsd_buffer

	STRUCT	exsd_QWKIMPORT,8		; System ID for QWK readers
	STRUCT	exsd_BatchPath,16
	WORD	exsd_DefaultScan
	WORD	exsd_DefaultWho

	STRUCT	exsd_DefaultWhoNote,61
	STRUCT	exsd_DefaultMacro,91

	WORD	exsd_LockCode			; Master console lock code

	LONG	exsd_DupeSize			; XMAIL! Dupe Table Msg Size
	LONG	exsd_RemotePasswd
	LONG	exsd_NewUsersSince
	LONG	exsd_OverRidePW
	LABEL exsd_SIZE
