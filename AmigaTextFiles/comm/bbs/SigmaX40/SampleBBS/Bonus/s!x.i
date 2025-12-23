
; S!X Structure for ASM-One + Compatible dONE bY fOX/RMD^SAD^AD!

;===[ User.Data ]=================[ BBS:User.Data or BBS:Node<NodeNr>.user ]===

			rsreset
UD_Name			rs.b	31	;\
UD_Pass			rs.b	9	; \ Includes Trailing $00-Byte
UD_Location		rs.b	30	; /
UD_PhoneNumber		rs.b	14	;/
UD_SlotNumber		rs.w	1
UD_SecStatus		rs.w	1
UD_SecBoard		rs.w	1	; File or Byte-Ratio
UD_SecLibrary		rs.w	1	; Ratio
UD_SecBulletin		rs.w	1	; Compitype
UD_MessagesPosted	rs.w	1
UD_NewSinceDate		rs.l	1
UD_ConfRead1		rs.l	1
UD_ConfRead2		rs.l	1
UD_ConfRead3		rs.l	1
UD_ConfRead4		rs.l	1
UD_ConfRead5		rs.l	1
UD_ConfRead6		rs.l	1
UD_ConfRead7		rs.l	1
UD_ConfRead8		rs.l	1
UD_ConfRead9		rs.l	1
UD_ConferenceAccess	rs.b	10
UD_Uploads		rs.w	1
UD_Downloads		rs.w	1
UD_ConfRJoin		rs.w	1
UD_TimesCalled		rs.w	1
UD_TimeLastOn		rs.l	1
UD_TimeUsed		rs.l	1
UD_TimeLimit		rs.l	1
UD_TimeTotal		rs.l	1
UD_BytesDownload	rs.l	1
UD_BytesUpload		rs.l	1
UD_DailyBytesLimit	rs.l	1
UD_DailyBytesDld	rs.l	1
UD_Expert		rs.w	1
UD_NBytes_Download	rs.l	2	;64-Bit
UD_NBytes_Upload	rs.l	2
UD_NDaily_Bytes_Limit	rs.l	2
UD_NDaily_Bytes_Dld	rs.l	2
UD_Flag1		rs.b	1
UD_Flag2		rs.b	1
UD_Flag3		rs.b	1
UD_EditorType		rs.b	1
UD_BeginLogCall		rs.l	1
UD_Protocol		rs.b	1
UD_UUCPA		rs.b	1	;AnsiUserFlag!
UD_LineLength		rs.b	1
UD_NewUser		rs.b	1
UserData_SIZEOF		rs.b	0

;===[ User.Keys ]=============[ BBS:User.Keys or BBS:Node<NodeNr>.userkeys ]===

			rsreset
UK_UserName		rs.b	31
UK_Pad1			rs.b	1
UK_Number		rs.l	1
UK_NewUser		rs.b	1
UK_Pad2			rs.b	1
UK_UPcps		rs.w	1
UK_DNcps		rs.w	1
UK_UserFlags		rs.w	1
UK_BaudRate		rs.w	1
UK_QFlag		rs.b	1
UK_Who			rs.b	1
UK_MailScan		rs.b	1
UK_Language		rs.b	1
UK_QuietNode		rs.b	1
UK_MaxCalls		rs.b	1
UK_Called		rs.b	1
UK_ReplySkip		rs.b	1
UK_ConfAccounting	rs.b	1
UK_Pad3			rs.b	1
UserKeys_SIZEOF		rs.b	0

;===[ User.Misc ]==========================================[ BBS:User.Misc ]===

			rsreset
UM_Handle		rs.b	31	; Not Used
UM_Pad1			rs.b	1
UM_ConfAxx		rs.l	1	; Not Sure
UM_MiscInfo		rs.l	1	; Not Sure
UM_MsgSentBy		rs.b	76	; Inc. $00
UM_SentBy		rs.b	46	; Inc. $00+Pad
UM_ConfAccounting	rs.b	1
UM_Pad			rs.b	263	; MayBe Used Soon
UserMisc_SIZEOF		rs.b	0

;===[ ConfBase ]========================================[ BBS:Conf/Conf.DB ]===

			rsreset
CB_Handle		rs.b	31	; Not Used
CB_Pad1			rs.b	1
CB_NewSinceDate		rs.l	1
CB_ConfRead		rs.l	1
CB_ConfYM		rs.l	1
CB_Bytes_Download	rs.l	1
CB_Bytes_Upload		rs.l	1
CB_Daily_Bytes_Limit	rs.l	1
CB_Daily_Bytes_Dld	rs.l	1
CB_Uploads		rs.w	1
CB_Downloads		rs.w	1
CB_RatioType		rs.w	1
CB_Ratio		rs.w	1
CB_Messages_Posted	rs.w	1
CB_Access		rs.w	1
CB_Active		rs.w	1
ConfBase_SIZEOF		rs.b	0

;===[ MailHeader ]===========================[ BBS:Conf/MsgBase/MailHeader ]===

			rsreset
MH_Status		rs.b	1	;"P"=Private "R"=Public "D"=Deleted
MH_Pad1			rs.b	1
MH_MsgNumb		rs.l	1
MH_ToName		rs.b	31	;\
MH_FromName		rs.b	31	; > Includes Trailing $00
MH_Subject		rs.b	31	;/
MH_Pad2			rs.b	1
MH_MsgDate		rs.l	1	; Date Posted
MH_Recv			rs.l	1	; Date Recieved
MH_Conference		rs.b	1	; Unused ?
MH_Pad3			rs.b	1
MailHeader_SIZEOF	rs.b	0

;===[ MailStat ]==============================[ BBS:Conf/MsgBase/MailStats ]===

			rsreset
MS_LowestKey 		rs.l	1
MS_HighMsgNum		rs.l	1
MS_LowestNotDel		rs.l	1
MS_Pad			rs.b	6
MS_SIZEOF		rs.b	0

;===[ ConfCfg ]======================================[ BBS:Conferences.dat ]===

			rsreset
CC_ConfNum		rs.l	1
CC_ConfName		rs.b	100	;\
CC_ConfLoc		rs.b	255	; \ Inc. $00
CC_MsgLoc		rs.b	255	; /
CC_PassWord		rs.b	30	;/
CC_SentBy		rs.w	1
CC_MsgShared		rs.w	1
ConfCfg_SIZEOF		rs.b	0

;===[ topcps ]==================================[ BBS:TOPCPS(<ConfNr>).DAT ]===

TC_upCPS		rs.l	1
TC_upNAME		rs.b	40	;Inc.$00
TC_downCPS		rs.l	1
TC_downNAME		rs.b	40	;Inc.$00
topcps_SIZEOF		rs.b	0

;===[ FNodeStatus ]===============================================[ UNUSED ]===

			rsreset
FNS_FileUpload		rs.b	32
FNS_FileDownload	rs.b	32
FNS_FileUpSize		rs.l	1
FNS_FileUpCurr		rs.l	1
FNS_FileDSize		rs.l	1
FNS_FileDCurr		rs.l	1
FNS_CPSUp		rs.l	1	;int
FNS_CPSDl		rs.l	1	;int
FNodeStatus_SIZEOF	rs.b	0

