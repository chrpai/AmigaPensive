 STRUCTURE UserInfo,0
	LONG	exui_Serial_Number		; Unique number for each acct

	LONG	exui_LastPvt_Entry		; Last time in personal area

	LONG	exui_LastCall			; Time/date of first call
	LONG	exui_FirstCall			; Time/date of last call

	BYTE	exui_AccessGroup
	BYTE	exui_ReducedAccess

	LONG	exuiReductionDate

	STRUCT	exui_ExAccess,exea_SIZE

	STRUCT	exui_Alias,31
	STRUCT	exui_Real_Name,31
	STRUCT	exui_Phone,21
	STRUCT	exui_WorkPhone,21
	STRUCT	exui_Street,31
	STRUCT	exui_Mail_Route,15
	STRUCT	exui_Country,4
	STRUCT	exui_State,4
	STRUCT	exui_City,26
	STRUCT	exui_Comments,41
	STRUCT	exui_Computer,31
	STRUCT	exui_Net_Alias,41		; Net Address for Usenet
	STRUCT	exui_Macro,4*91			; The New Macro Key Settings
	BYTE	exui_KludgeFill00

	WORD	exui_Passwd

	LONG	exui_User_Info_Bits		; refer to EI_ defines

	BYTE	exui_Term_Type			; Terminal Type
						; 1 = ASCII   2 = AMIGA
	BYTE	exui_Term_Max_Col		; Number of Columns
	BYTE	exui_Term_Max_Lines		; Number of Lines
	BYTE	exui_Default_Proto		; Default UD Protocal
	BYTE	exui_Scan_Prefs			; The scan preferences for scanning

	BYTE	exui_Time_Zone			; Time zone offset

	STRUCT	exui_ExData,exed_SIZE

	LONG	exui_Up_Bytes			; Bytes Uploaded
	LONG	exui_Up_MBytes			; MBytes uploaded
	LONG	exui_Down_Bytes			; Bytes Downloaded
	LONG	exui_Down_MBytes		; MBytes downloaded
	LONG	exui_Up_Files			; Files Uploaded
	LONG	exui_Down_Files			; Files Downloaded

	LONG	exui_Posts			; Items posted
	LONG	exui_Mail			; How many letters sent
	LONG	exui_Mail_Received		; Letters this user has recieved

	LONG	exui_Call_Number		; How many calls to the system

	LONG	exui_Balance			; How much has the user charged

	LONG	exui_Credit_Bytes		; UD byte credits
	LONG	exui_Credit_Files		; UD file credits
	LONG	exui_Credit_Time		; Timebank credits

	LONG	exui_Last_Baud			; last baud rate
	LONG	exui_Bad_Logins			; # of failed attempts

	LONG	exui_FreeFiles			; Files that are free!
	LONG	exui_FreeBytes			; Free bytes!

	STRUCT	exui_WhoNote,61			; Info to other users in WHO

	BYTE	exui_Header_Prefs		; message headers prefs

	LONG	exui_User_ID			; The User's ID#

	LONG	exui_ScreenClear		; Clears screen reading

	LONG	exui_WhoPrefs			; Default Who Listing

	STRUCT	exui_BirthDate,exbd_SIZE

	LONG	exui_MBase_Flags
	LONG	exui_FBase_Flags
	LONG	exui_PBase_Flags

	BYTE	exui_BatchMethod
	STRUCT	exui_Organization,81

	LONG	exui_Language
	LONG	exui_MailCounter		; Private

	WORD	exui_TextColor
	WORD	exui_BarColor

	LONG	exui_MaxCommentLines
	STRUCT	exui_expansion,4*17
	LABEL exui_SIZE
