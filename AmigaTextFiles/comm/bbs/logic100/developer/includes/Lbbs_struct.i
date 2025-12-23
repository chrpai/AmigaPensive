
**
**	$VER: logicbbs.i 1.0 (26.02.95)
**
**	Userdata and Messagebase structures for LogicBBS
**
**	(C) Copyright 1995 ParCon Software
**	      All Rights Reserved
**


**
** Messagebase related information
**


   IFND EXEC_LISTS_I
   include "exec/lists.i"
   ENDC


 STRUCTURE	MSGHEAD,0
    ULONG	Letter_FreeSpace
    ULONG	Letter_First
    ULONG	Letter_Low
    ULONG	Letter_high
    LABEL	MSGHEAD_SIZE



 STRUCTURE	LMESSAGE,0
    ULONG	Letter_Number
    ULONG	Letter_Status

* MSF stands for MessageStatusFlag
        BITDEF	MSF,Deleted,0
        BITDEF	MSF,Touch,1
        BITDEF	MSF,ALL,2
        BITDEF	MSF,EALL,3
        BITDEF	MSF,EALL_Cosysop,4
        BITDEF	MSF,EALL__Usertype,5		;Not implemented yet
        BITDEF	MSF,Private,6
        BITDEF	MSF,PrivateCosys,7
        BITDEF	MSF,PrivateSysop,8
        BITDEF	MSF,Forward,9
        BITDEF	MSF,NotifySender,10		;*Removed*
        BITDEF	MSF,Received,11
        BITDEF	MSF,Reply,12
        BITDEF	MSF,Attachfile,13
        BITDEF	MSF,SystemMsg,14
    ULONG	Letter_Keep
    ULONG	Sender_Notify
    ULONG	Reply_Ref_User
    ULONG	Reply_Ref_UserNo
    ULONG	Reply_Ref_MailNo
    ULONG	Forwarded_ref
    ULONG	Letter_SenderNo
    UWORD	Letter_Date
    UWORD	Letter_Time
    UWORD	Letter_RecDate
    UWORD	Letter_RecTime
    STRUCT	Letter_From,32
    STRUCT	Letter_To,32
    STRUCT	Letter_Subject,52
    LABEL	LMESSAGE_SIZE



**
** Conference data information
**

 STRUCTURE	USER_CFDATA,0
    ULONG	LMsg_ReadNoConf
    ULONG	LMsg_AutoNoConf
    ULONG	Msg_PostedConf
    ULONG	Upload_BytesConf
    ULONG	Download_BytesConf
    ULONG	Upload_FilesConf
    ULONG	Download_FilesConf
    UWORD	RatioConf
    ULONG	DownloadByteLimitConf
    ULONG	DownloadBytesTodayConf
    UWORD	AccessLevelConf
    UWORD	ForwardMailToDateConf
    ULONG	ForwardMailRefConf
    BYTE	ScanNewMailHere
    BYTE	ScanNewFilesHere
    UBYTE	ScreenLinesConf
    UBYTE	ScreenWidthConf
    BYTE	ScreenClearConf
    BYTE	Available_CHATOLMConf
    UBYTE	Transfer_ProtocolConf
    UBYTE	RatioTypeConf
    BYTE	UserInfoBitsConf
    BYTE	UserInfoBits2Conf
    UWORD	ConfDate






**
** Userdata information
**

	STRUCTURE	UserData,0
	   ULONG	UserNumber
	   UWORD	UserStatus
_UserDeleted	equ	0
_UserActive	equ	1
	   STRUCT	UserName,31
	   STRUCT	Location,31
	   STRUCT	Ph_International,4
	   STRUCT	Phone_1_Voice,12
	   STRUCT	Phone_2_DataFax,12
	   STRUCT	Phone_3_Celluar,12
	   STRUCT	UserPassword1,12
	   STRUCT	UserPassword2,12
	   STRUCT	Fidonet_Password,21
	   STRUCT	UserType,31
	   STRUCT	ConfName,32
	   UWORD	AccessLevel
	   ULONG	ConfAccess_1_32
	   ULONG	ConfAccess_33_64
	   ULONG	ConfAccess_65_96
	   ULONG	ConfAccess_97_128
	   ULONG	ConfAccess_129_160
	   ULONG	ConfAccess_161_192
	   UWORD	ForwardMailToDate
	   ULONG	ForwardMailRef
	   ULONG	Upload_Bytes
	   ULONG	Download_Bytes
	   ULONG	Upload_Files
	   ULONG	Download_Files
	   UWORD	Ratio
	   ULONG	DownloadBytelimit
	   ULONG	DownloadBytesToday
	   UWORD	Timelimit_Day
	   UWORD	TimeUsed_ThisDay
	   ULONG	TimeUsed_All
	   UWORD	ChatLimit
	   UWORD	ChatUsed_ThisDay
	   ULONG	ChatUsed_All
	   ULONG	ReservedInfo_1
	   ULONG	ReservedInfo_2
	   ULONG	LMsg_ReadNo
	   ULONG	LMsg_AutoNo
	   ULONG	Msg_Posted
	   UWORD	FirstLogon_Date
	   UWORD	FirstLogon_Time
	   UWORD	LastCalled_Date
	   UWORD	LastCalled_Time
	   UWORD	BirthDay
	   UBYTE	ComputerType
	   BYTE		ReservedInfo_3
	   ULONG	Calls_All
	   UWORD	Calls_ThisDay
	   UWORD	CPS_TopUpload
	   UWORD	CPS_TopDownload
	   ULONG	BaudRate
	   UBYTE	ScreenTXT_Type
	   UBYTE	ScreenEditor
	   UBYTE	ZoomMailType
	   UBYTE	ScreenLines
	   UBYTE	ScreenWidth
	   UBYTE	ScreenClear
	   UBYTE	Available_CHATOLM
	   UBYTE	Transfer_Protocol
	   UBYTE	RatioType
	   UBYTE	UserInfoBits
	       BITDEF	UIF,Expertmode,0
	       BITDEF	UIF,MailForward,1
	   UBYTE	TRACKUSER_ULB
	   UBYTE	TRACKUSER_DLB
	   ULONG	OLDTRACK_ULB
	   ULONG	OLDTRACK_ULF
	   ULONG	OLDTRACK_DLB
	   ULONG	OLDTRACK_DLF
	   UWORD	OLDTRACK_RATIO
	   UBYTE	OLDTRACK_RATIOTP
	   UBYTE	ReservedInfo_4
	   UWORD	DAYSPAID
	   UWORD	TRACKINGDAYS
	   UWORD	MaxFreeBulls
	   UWORD	PaidBulls
	   UWORD	BullsStay
	   LABEL	USERDATA_SIZE



bbsstatus
Node_Occupied		equ	-1
Node_TimeOut		equ	-2
Node_LC			equ	-3
Node_FatalError		equ	-4
Node_Down		equ	-5
Node_PtrError		equ	-6



NodeStatus_ID			;(identifiers)
NODE_CLOSED		equ	0
NODE_LOGGING_ON		equ	1
NODE_LOGOFF		equ	2
NODE_RESET		equ	3
NODE_MAILEVENT		equ	4
NODE_WAITING		equ	5
NODE_NODELOCAL		equ	6
NODE_NewUser		equ	7
NODE_IDLE		equ	8
NODE_CONNECTION		equ	9
NODE_LOCAL		equ	10
NODE_DOWNLOAD		equ	11
NODE_UPLOAD		equ	12
NODE_NewFiles		equ	13
NODE_ZippySearch	equ	14
NODE_STATUS		equ	15
NODE_RWMail		equ	16
NODE_NewMail		equ	17
NODE_COMMENTSYSOP	equ	18
NODE_Bulletins		equ	19
NODE_Viewing		equ	20
NODE_Sysoping		equ	21
NODE_EDITUSER		equ	22
NODE_PageSysop		equ	23
NODE_ChatSysop		equ	24
NODE_Multichat		equ	25
NODE_LCFiles		equ	26
NODE_SERIALERROR	equ	27
NODE_WhoDoor		equ	28
NODE_JoinConf		equ	29
NODE_Online_Msg		equ	30
NODE_InADoor		equ	31
NODE_DirectUL		equ	32
NODE_DirectDL		equ	33
NODE_TERMINATE		equ	34
NODE_INITERROR		equ	35
NODE_SCRINITERROR	equ	36
NODE_ExtName		equ	37
