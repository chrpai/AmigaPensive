; Response struct (one for every response)

 STRUCTURE NewRes_Data,0
	LONG	exnd_Date_Sent
	LONG	exnd_Expire_Date

	LONG	exnd_Response_Number		; Which response are we
	LONG	exnd_Next_Response		; Next response on disk
	LONG	exnd_Last_Response		; last response on disk
	LONG	exnd_SNext_Response		; Responses in sequential ordering
	LONG	exnd_SLast_Response		; Last response in sequential ordering
	LONG	exnd_TextStart			; Where message body starts
	LONG	exnd_TextEnd			; Where message body ends

	LONG	exnd_To_ID			; The Receivers ID
	LONG	exnd_To_Serial_Num		; THe receivers serial number
	STRUCT	exnd_To_Alias,31		; The Receivers Alias
	BYTE	exnd_KludgeFill00

	LONG	exnd_From_ID			; The Senders User ID
	LONG	exnd_From_Serial_Num		; Senders Serial Number
	STRUCT	exnd_From_Alias,31		; Senders Alias
	BYTE	exnd_KludgeFill01

	LONG	exnd_Ext_Flags			; Flags for this Post, see POST_ flags

	STRUCT	exnd_MsgID,61
	STRUCT	exnd_FidoDate,41

	STRUCT	exnd_exp,4*10
	LABEL exnd_SIZE
