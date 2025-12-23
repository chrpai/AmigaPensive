 STRUCTURE AccessGroup_Data,0
	STRUCT	exagd_ExAccess,exea_SIZE

	STRUCT	exagd_Access_Title,31		; The Title
	BYTE	exagd_KludgeFill00

	LONG	exagd_User_Info_Bits		; Data For More Prompt, Etc

	STRUCT	exagd_ExData,exed_SIZE

	LONG	exagd_AccBits			; 32 Access Flag Bits
	LONG	exagd_More_AccBits		; 32 More Flag Bits

	BYTE	exagd_ReducedAccess		; Access to reduce to
	BYTE	exagd_KludgeFill01

	LONG	exagd_ReductionDate		; Date to reduce access

	LONG	exagd_Up_Bytes			; Bytes Uploaded
	LONG	exagd_Up_MBytes			; MBytes Uploaded
	LONG	exagd_Down_Bytes		; Bytes Downloaded
	LONG	exagd_Down_MBytes		; MBytes Downloaded
	LONG	exagd_Up_Files			; Files Uploaded
	LONG	exagd_Down_Files		; Files Downloaded
	LONG	exagd_Posts			; Items posted
	LONG	exagd_Mail			; How many letters sent
	LONG	exagd_Mail_Recieved		; Letters this user has recieved
	LONG	exagd_Call_Number		; How many calls to the system

	LONG	exagd_Balance			; How much has the user charged

	LONG	exagd_Credit_Bytes		; UD byte credits
	LONG	exagd_Credit_Files		; UD file credits
	LONG	exagd_Credit_Time		; Timebank credits

	LONG	exagd_Last_Baud			; last baud rate

	LONG	exagd_Bad_Logins		; # of failed attempts

	LONG	exagd_Free_Bytes		; initial file credits
	LONG	exagd_Free_Files		; initial byte credits

	LONG	exagd_MaxCommentLines
	LONG	exagd_expansion,4*4
	LABEL exagd_SIZE
