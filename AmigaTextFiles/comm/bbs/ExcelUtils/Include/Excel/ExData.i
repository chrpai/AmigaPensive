; Data required for Extended Access limits

 STRUCTURE ExData,0				
	LONG	exed_Period			; Start of the period

	LONG	exed_Period_Up_Bytes		; Bytes uploaded in this period
	LONG	exed_Period_Down_Bytes		; Bytes downloaded
	WORD	exed_Period_Up_Files		; Files uploaded
	WORD	exed_Period_Down_Files		; Files downloaded
	WORD	exed_Period_Calls		; Calls
	WORD	exed_Period_Minutes		; Time used
	WORD	exed_Period_Charges		; Charges racked up in this per
	WORD	exed_Period_Posts		; Items posted
	WORD	exed_Period_Feedback		; Feedback left in this per
	WORD	exed_Period_TB_Withdraw		; Time taken out this per
	WORD	exed_Period_TB_Deposit		; Time deposited this per

	LONG	exed_Period_Interuser_Chats	; interuser chats
	LONG	exed_Period_Interuser_Mins	; interuser
	LONG	exed_Period_Conference_Entry	; conference entries
	LONG	exed_Period_Conference_Lines	; conference lines sent
	LONG	exed_Period_Conference_Mins	; conference mins 
	LONG	exed_Period_InterUserMessages	; # of im's sent

	LONG	exed_FIDO_Credits		; Users Fido Balance

	LONG	exed_Mail_Forward		; forward ID# 
	LONG	exed_Mail_Forward_Serial	; forward SER#

	STRUCT	exed_expansion,4*5
	LABEL exed_SIZE
