 STRUCTURE Comm_Message,0
	STRUCT	excm_MN,MN_SIZE		; message
	LONG	excm_Command
	LONG	excm_Pass1
	LONG	excm_Pass2
	LONG	excm_Pass3
	LONG	excm_Pass4
	LONG	excm_Pass5
	LONG	excm_Pass6
	LONG	excm_Result1
	LONG	excm_Result2
	LABEL excm_SIZE
