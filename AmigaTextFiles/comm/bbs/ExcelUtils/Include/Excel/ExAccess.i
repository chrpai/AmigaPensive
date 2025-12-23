 STRUCTURE ExAccess,0
	LONG	exea_AccBits			; 32 Access Flag Bits
	LONG	exea_More_AccBits		; 32 more Flag Bits

	LONG	exea_Private_Area		; How many bytes allowed in a
						; users priv dir area at logoff

	WORD	exea_Bad_Logins			; Number of bad logins
	WORD	exea_Max_TB_Credits		; time allowed in the timebank
	WORD	exea_Time_Idle			; Idle time allowed

	WORD	exea_Call_Minutes		; Minutes per call
	WORD	exea_Call_Feedback		; Feedback per call
	WORD	exea_Call_Posts			; Posts per call
	WORD	exea_Call_TB_Withdraw		; How much time allowed per call
	WORD	exea_Call_TB_Deposit		; How much time allowed per call
	WORD	exea_Call_Upload_Files		; Uploaded files per call
	WORD	exea_Call_Download_Files	; Downloaded files per call
	LONG	exea_Call_Upload_Bytes		; Uploaded bytes per call
	LONG	exea_Call_Download_Bytes	; Downloaded bytes per call

	BYTE	exea_Period_Type		; Period type, 0=none, 1=daily,
						; 2 = effective between first call
						; of a period and period's len
	BYTE	exea_KludgeFill00

	WORD	exea_Period_Length		; How many hours between periods
	WORD	exea_Period_Calls		; Calls per period
	WORD	exea_Period_Minutes		; Minutes Per Period
	WORD	exea_Period_Feedback		; Pieced of feedback per period
	WORD	exea_Period_Posts		; Items posted per period
	WORD	exea_Period_Charges		;  rack up
	WORD	exea_Period_TB_Deposit		; deposit per period
	WORD	exea_Period_TB_Withdraw		; withdraw per period
	WORD	exea_Period_Upload_Files	; Uploaded Files per period
	WORD	exea_Period_Download_Files	; Downloaded Files per period
	LONG	exea_Period_Upload_Bytes	; Uploaded Bytes per period
	LONG	exea_Period_Download_Bytes	; Downloaded per period

	BYTE	exea_Download_Byte_Ratio	; Users Download Ratio (BYTES)
	BYTE	exea_Download_File_Ratio	; Users Download Ratio (FILES)

	WORD	exea_Expire_Days		; Inactive days allowed

	LONG	exea_FIDO_Credits		; Users MAX Fido Balance

	LONG	exea_Editor_Lines		; # of lines a user is allowed

	LONG	exea_Period_Interuser_Chats	; # of interuser chats allowed
	LONG	exea_Period_Interuser_Mins	; # of interuser mins allowed
	LONG	exea_Period_Conference_Entry	; # of conference entries
	LONG	exea_Period_Conference_Lines	; # of conference lines sent
	LONG	exea_Period_Conference_Mins	; # of conference lines 
	LONG	exea_Period_InterUserMessages	; # of im's user can send

	LONG	exea_Call_Interuser_Chats	; # of interuser chats allowed
	LONG	exea_Call_Interuser_Mins	; # of interuser mins allowed
	LONG	exea_Call_Conference_Entry	; # of conference entries
	LONG	exea_Call_Conference_Lines	; # of conference lines sent
	LONG	exea_Call_Conference_Mins	; # of conference mins 
	LONG	exea_Call_InterUserMessages	; # of im's user can send

	LONG	exea_Max_ActionCmds		; # of personal action cmds
	STRUCT	exea_expansion,4*4
	LABEL exea_SIZE
