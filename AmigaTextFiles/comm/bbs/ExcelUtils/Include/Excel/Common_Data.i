 STRUCTURE Common_Data,0
	APTR	excd_Int_Data ; *ID		; System Private
	LONG	excd_Version			; Version # of this structure

	BYTE	excd_CHL			; last character return
	BYTE	excd_KludgeFill00		; 0=unknown, 1=con, 2=ser

	APTR	excd_Ex				; Points to language file
	APTR	excd_Input_Buffer		; Points to the Input Buffer

	BOOL	excd_ANSI			; Is the user in ANSI mode?
	LONG	excd_Baud			; Baud rate the user is at
	LONG	excd_Login_Time			; Time user logged in

	APTR	excd_UserInfo ; *U_Data		; User Data
	APTR	excd_SUserInfo ; *SU_Data	; Scratch UserData structure

	LONG	excd_User_ID			; The users ID on this node

	BOOL	excd_IPLResponse		; used after an IPL '\?'

	APTR	excd_FileInfoBlock ; *fib	; File Info Block

	LONG	excd_Info_Bits			; REFER TO NODE_ defines*/
	LONG	excd_MoreInfo_Bits		; REFER TO MNODE_ defines

	APTR	excd_List ; *Editor		; Points to the editor
	LONG	excd_VisEdReturn		; 0=abort, 1=save, 2=editor
	LONG	excd_MaxEditorLines		; The max lines available to enter 

	APTR	excd_System_Data ; *Sys		; System Data Structure

	APTR	excd_ParseVal			; The parse val after a command

	LONG	excd_MouseX			; Coordinates from our rastport
	LONG	excd_MouseY

	APTR	exdc_List ; *QUOTEs		; Quoted lines, if any

	BOOL	excd_PrintMessage		; if NO abort message (space)
	LONG	excd_MaxLinesEntered		; The total # of lines entered

	LONG	excd_Ext_Flags			; Refer to EXT_ defines

	LONG	excd_Cur_Msg			; private for Vised's
	LONG	excd_Lo_Msg
	LONG	excd_Hi_Msg
	LONG	excd_TotalResponses

	APTR	excd_MiscString			; Pointer to buffer
	LABEL excd_SIZE
