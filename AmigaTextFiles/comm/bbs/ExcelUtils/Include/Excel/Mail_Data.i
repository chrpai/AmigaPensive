;  Mail Structure
 STRUCTURE Mail_Data,0
	LONG	exmd_Date_Sent			; Date Letter Was Sent
	LONG	exmd_Date_Expire		; Date Letter/Post expires

	LONG	exmd_To_ID			; The Recievers ID
	LONG	exmd_To_Serial_Num		; The Recievers Serial Number
	STRUCT	exmd_To_Alias,61		; The Recievers Alias
	BYTE	exmd_KludgeFill00

	LONG	exmd_From_ID			; The Senders User ID
	LONG	exmd_From_Serial_Num		; Senders Serial Number
	STRUCT	exmd_From_Alias,101		; Senders Alias
	BYTE	exmd_KludgeFill01

	LONG	exmd_Next_Letter
	LONG	exmd_Last_Letter

	WORD	exmd_Mess_Flags			; All The Flags For Mail

	STRUCT	exmd_Subject,61			; The Message Subject
	BYTE	exmd_KludgeFill02

	STRUCT	exmd_CarbonCopies,4*75		; Carbon Copies

	STRUCT	exmd_UUCPDate,128		; Date For UUCP Messages
	LABEL exmd_SIZE
