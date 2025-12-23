*****************************************************************************
*****************************************************************************
* AMOS MAXS DOORHANDLER, WRITTEN BY ARI TSIRONIS, V0.20, 1-5-94             *
*****************************************************************************
*****************************************************************************


*****************************************************************************
* EQUATES:  EXTENSION NUMBER=16  SIZE OF DoorMsg = 106                      *   
*****************************************************************************

ExtNb		    equ	     16-1
MsgLength   equ      106


*****************************************************************************
* INCLUDES                                                                  *   
*****************************************************************************

            Incdir   dh0:devpac/include/
		        Include  exec/exec.i
		        Include  exec/exec_lib.i
            Include  exec/ports.i 

            Incdir   AMOS:Extension_src/
            Include  _Equ.s
		        RsSet	   DataLong
            Include  _Pointe.s
		        Include  _CEqu.s
            Include  _WEqu.s
            Include  _LEqu.s


*****************************************************************************
* MACROS                                                                    *   
*****************************************************************************


;FOR ACCESSING OUR EXTENSION DATA ZONE FROM WITHIN THE FUNCTIONS

Dlea		    MACRO
		        move.l	 ExtAdr+ExtNb*16(a5),\2
		        add.w	   #\1-MB,\2
		        ENDM

Dload		    MACRO
		        move.l	 ExtAdr+ExtNb*16(a5),\1
		        ENDM


; MACRO FOR SENDING A MESSAGE TO MAXS DOOR PORT

SendDoorMsg MACRO
            move.l   d0,a0                  
            Dlea     DoorMsg,a1
            CALLEXEC PutMsg                   ; PutMsg(MAXDoorPort,DoorMsg)

            move.l   MyReplyPort-MB(a2),a0
            CALLEXEC WaitPort                 ; WaitPort(MyReplyPort)

            move.l   MyReplyPort-MB(a2),a0
            CALLEXEC GetMsg                   ; GetMsg(MyReplyPort)
            ENDM


*****************************************************************************
* EXTENSION SPECIFIC POINTERS                                               *   
*****************************************************************************

		        RsReset

Start		    dc.l	   C_Tk-C_Off               ; Pointer to Token List     
		        dc.l	   C_Lib-C_Tk               ; Pointer to first Library Function
		        dc.l	   C_Title-C_Lib            ; Pointer to Title
		        dc.l	   C_End-C_Title            ; Pointer to End of Extension
		        dc.w	   0


*****************************************************************************
* OFFSETS TO FUNCTIONS                                                      *
*****************************************************************************

C_Off       dc.w (L1-L0)/2,(L2-L1)/2,(L3-L2)/2,(L4-L3)/2
            dc.w (L5-L4)/2,(L6-L5)/2,(L7-L6)/2,(L8-L7)/2
            dc.w (L9-L8)/2,(L10-L9)/2,(L11-L10)/2,(L12-L11)/2
          	dc.w (L13-L12)/2,(L14-L13)/2,(L15-L14)/2,(L16-L15)/2
           	dc.w (L17-L16)/2,(L18-L17)/2,(L19-L18)/2,(L20-L19)/2
          	dc.w (L21-L20)/2,(L22-L21)/2,(L23-L22)/2,(L24-L23)/2
          	dc.w (L25-L24)/2


*****************************************************************************
* TOKENS                                                                    *
*****************************************************************************

C_Tk		    dc.w 	   1,0
		        dc.b 	   $80,-1


; USAGE: RESULT = M_PortOpen(NODE_NUMBER)

		        dc.w 	   -1,L_PortOpen
		        dc.b 	   "m_portope","n"+$80,"00",-1


; USAGE: M_PortClose

            dc.w     L_PortClose,-1
            dc.b     "m_portclos","e"+$80,"I",-1
        

; USAGE: RESULT = M_BBSText(TEXT$)

            dc.w     -1,L_BBSText
            dc.b     "m_bbstex","t"+$80,"02",-1


; USAGE: RESULT = M_LocalText(TEXT$)

            dc.w     -1,L_LocalText
            dc.b     "m_localtex","t"+$80,"02",-1


; USAGE: RESULT = M_ModemChar(CHAR)

            dc.w     -1,L_ModemChar
            dc.b     "m_modemcha","r"+$80,"00",-1


; USAGE: RESULT = M_ScreenChar(CHAR)

            dc.w     -1,L_ScreenChar
            dc.b     "m_screencha","r"+$80,"00",-1


; USAGE: RESULT = M_BBSChar(CHAR)

            dc.w     -1,L_BBSChar
            dc.b     "m_bbscha","r"+$80,"00",-1


; USAGE: RESULT = M_PromptText(TEXT$,INPUT$)

            dc.w     -1,L_PromptText
            dc.b     "m_prompttex","t"+$80,"02,2",-1


; USAGE: RESULT = M_SPromptText(TEXT$,INPUT$)

            dc.w     -1,L_SPromptText
            dc.b     "m_sprompttex","t"+$80,"02,2",-1


; USAGE: RESULT = M_GetChar(TEXT$,CHAR$)

            dc.w     -1,L_GetChar
            dc.b     "m_getcha","r"+$80,"02,2",-1


; USAGE: RESULT = M_TwitUser

		        dc.w 	   -1,L_TwitUser
		        dc.b 	   "m_twituse","r"+$80,"0",-1


; USAGE: RESULT = M_ShowFile(FILE$)

            dc.w     -1,L_ShowFile
            dc.b     "m_showfil","e"+$80,"02",-1


; USAGE: RESULT = M_CheckFile(FILE$)

            dc.w     -1,L_CheckFile
            dc.b     "m_checkfil","e"+$80,"02",-1


; USAGE: RESULT = M_EditFile(FILE$)

            dc.w     -1,L_EditFile
            dc.b     "m_editfil","e"+$80,"02",-1


; USAGE: RESULT = M_GetUserNum(TYPE)

            dc.w     -1,L_GetUserNum
            dc.b     "m_getusernu","m"+$80,"00",-1


; USAGE: RESULT = M_GetUserStr(TYPE,USER$)

            dc.w     -1,L_GetUserStr
            dc.b     "m_getuserst","r"+$80,"00,2",-1


; USAGE: RESULT = M_NewAccess(ACCESS)

            dc.w     -1,L_NewAccess
            dc.b     "m_newacces","s"+$80,"00",-1


; USAGE: RESULT = M_AddTime(TIME)

            dc.w     -1,L_AddTime
            dc.b     "m_addtim","e"+$80,"00",-1


; USAGE: RESULT = M_DoFunction(FUNC,EXTRA,PATH$)

            dc.w     -1,L_DoFunction
            dc.b     "m_dofunctio","n"+$80,"00,0,2",-1


; USAGE: RESULT = M_ChangeUserData(TYPE,DATA)

            dc.w     -1,L_ChangeUserData
            dc.b     "m_changeuserdat","a"+$80,"00,0",-1


; USAGE: RESULT = M_GetKey

            dc.w     -1,L_GetKey
            dc.b     "m_getke","y"+$80,"0",-1

		        dc.w 	   0


*****************************************************************************
* START LIBRARY, PARAMETERS ARE PASSED In (a3), RESULTS ARE RETURNED IN d3  *
* FOR RETURNING RESULTS d2=0,1,2 FOR LONG, FLOAT, STRING RESPECTIVELY       *
* STRING FORMAT = XX,YY,"ZZZZZ" WHERE XX,YY, ARE WORD LENGTH OF STRING      *
*****************************************************************************

C_Lib


*****************************************************************************
* COLD START                                                                *
*****************************************************************************

L0	        
            movem.l	 a3-a6,-(sp)


; STORE THE ADDRESS OF THE EXTENSION DATA ZONE

	          lea	     MB(pc),a3
	          move.l	 a3,ExtAdr+ExtNb*16(a5)


; STORE THE ADDRESS OF THE ROUTINE CALLED BY DEFAULT OR RUN

	          lea	     L0_Default(pc),a0
          	move.l	 a0,ExtAdr+ExtNb*16+4(a5)


; STORE THE ADDRESS OF THE END ROUTINE

          	lea	     L0_End(pc),a0
          	move.l	 a0,ExtAdr+ExtNb*16+8(a5)
          	movem.l	 (sp)+,a3-a6

          	moveq	   #ExtNb,d0		            ; NO ERRORS
           	rts


; SCREEN RESET

L0_Default:
            rts


; QUIT

L0_End:
            rts


*****************************************************************************
* EXTENSION SPECIFIC VARIABLES.  HAS TO BE WITHIN L1-L0 SO THAT L0 CAN      *
* ACCESS THE DATA WITHOUT USING Dlea AND Dload,                             *
*****************************************************************************

            RsReset

            Rdata                             ; STOPS SEARCHES FOR Rjmp etc!
          
MB:

DoorMsg     ds.l     1                        ; NODE:    struct Node *ln_Succ
            ds.l     1                        ; NODE:    struct Node *ln_Pred
Type        ds.b     1                        ; NODE:    UBYTE ln_Type
Pri         ds.b     1                        ; NODE:    BYTE ln_Pri
            ds.l     1                        ; NODE:    char *ln_Name
ReplyPort   ds.l     1                        ; MSGPORT: MsgPort *mn_ReplyPort
Length      ds.w     1                        ; MSG:     UWORD mm_Length
Command     ds.w     1                        ; DOORMSG: WORD Command
Data        ds.w     1                        ; DOORMSG: WORD Data
String      ds.b     80                       ; DOORMSG: char String[80]
Carrier     ds.w     1                        ; DOORMSG: WORD Carrier


MAXDoorPort ds.l     1
MyReplyPort ds.l     1

MAXDoorName dc.b     "DoorControl",0,0
            even
           

*****************************************************************************
* OPENS DOOR CONTROL PORT  USAGE: RESULT = M_PortOpen(NODE_NUMBER)          *
* RESULT = 0  IF THERE'S AN ERROR                                           *
*****************************************************************************

L_PortOpen  equ      1
L1
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER


; ERASE OUR DoorMsg STRUCTURE
           
            Dlea     DoorMsg,a0               ; POINT TO START OF DoorMsg
            move.w   #MsgLength,d0
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra

L1_EraseLoop:  
            clr.b    (a0)+
            dbra     d0,L1_EraseLoop
           

; ERASE OUR MSG PORT POINTERS

            clr.l    MAXDoorPort-MB(a2)
            clr.l    MyReplyPort-MB(a2)


; GET THE NODE NUMBER, AND CHANGE IT INTO AN ASCII NUMBER 0-9

            move.l   (a3)+,d0
            add.l    #48,d0           


; MAKE A CONTROL PORT NAME, BASED ON THE NODE NUMBER

            move.b   d0,11+MAXDoorName-MB(a2)            


; d0 = FindPort(MAXDoorName)

            Dlea     MAXDoorName,a1
            CALLEXEC Forbid
            CALLEXEC FindPort
            CALLEXEC Permit

            move.l   d0,MAXDoorPort-MB(a2)            
            beq.s    L1_InitError


; d0 = CreatPort(NULL,NULL)

            clr.l    -(sp)               
            clr.l	   -(sp)                   

	          moveq	   #0,d0                    ; CreatePort
	          Rbsr	   L_Amiga.Lib

            addq.l	 #8,sp                    ; CLEANUP STACK

            move.l   d0,MyReplyPort-MB(a2)  
            beq.s    L1_InitError 


; INITIALISE OUR DOOR MESSAGE STRUCTURE

            move.l   d0,ReplyPort-MB(a2)      ; DoorMsg.ReplyPort = MyReplyPort
            move.b   #NT_MESSAGE,Type-MB(a2)  ; DoorMsg.Type = NT_MESSAGE
            move.w   #MsgLength,Length-MB(a2) ; DoorMsg.Length = sizeof(DoorMsg)


L1_InitError:
            moveq.l  #0,d2                    ; RETURN IS A LONG
            move.l   d0,d3                    ; RETURN d0

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* CLOSES DOOR CONTROL PORT  USAGE: M_PortClose                              *
*****************************************************************************

L_PortClose equ      2
L2          
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER

            move.w   #0,Data-MB(a2)           ; Data = 0  
            move.w   #20,Command-MB(a2)       ; Command = 20  ie End


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L2_TryReply              ; IS MAXDoorPort = NULL?

            SendDoorMsg


; DeletePort(MyReplyPort)

L2_TryReply:
            move.l   MyReplyPort-MB(a2),d0
            beq.s    L2_NoPort   

            move.l   d0,-(sp)                 ; MyReplyReport

            moveq	#1,d0                       ; DeletePort
          	Rbsr	L_Amiga.Lib

            addq.l	 #4,sp                    ; CLEANUP STACK

L2_NoPort:
            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* OUTPUTS TEXT TO BOTH LOCAL AND MODEM  USAGE: RESULT = M_BBSText(TEXT$)    *
*****************************************************************************

L_BBSText   equ      3		
L3	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.w   #0,Data-MB(a2)           ; Data = 0  ie NO CR
            move.w   #1,Command-MB(a2)        ; Command = 1  ie SendMessage


; COPY TEXT$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO TEXT$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF TEXT$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L3_StrLoop               ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L3_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY TEXT$ TO String
            dbra     d0,L3_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L3_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L3_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* OUTPUTS TEXT TO LOCAL  USAGE: RESULT = M_LocalText(TEXT$)                 *
*****************************************************************************

L_LocalText equ      4		
L4	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.w   #0,Data-MB(a2)           ; Data = 0  ie NO CR
            move.w   #2,Command-MB(a2)        ; Command = 2  ie LocalMessage


; COPY TEXT$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO TEXT$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF TEXT$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L4_StrLoop               ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L4_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY TEXT$ TO String
            dbra     d0,L4_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L4_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0                       ; ZERO ANY GARBAGE
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L4_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* OUTPUTS CHAR TO MODEM  USAGE: RESULT = M_ModemChar(CHAR)                  *
*****************************************************************************

L_ModemChar equ      5		
L5	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.l   (a3)+,d0                 ; CHAR 
            move.w   d0,Data-MB(a2)           ; Data = CHAR
            move.w   #3,Command-MB(a2)        ; Command = 3  ie PutModemChar


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L5_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L5_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* OUTPUTS CHAR TO SCREEN  USAGE: RESULT = M_ScreenChar(CHAR)                *
*****************************************************************************

L_ScreenChar equ      6		
L6	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.l   (a3)+,d0                 ; CHAR 
            move.w   d0,Data-MB(a2)           ; Data = CHAR
            move.w   #4,Command-MB(a2)        ; Command = 4  ie PutScreenChar


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L6_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L6_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* OUTPUTS CHAR TO BBS  USAGE: RESULT = M_BBSChar(CHAR)                      *
*****************************************************************************

L_BBSChar   equ      7		
L7	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.l   (a3)+,d0                 ; CHAR 
            move.w   d0,Data-MB(a2)           ; Data = CHAR
            move.w   #5,Command-MB(a2)        ; Command = 5  ie PutDoorChar


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L7_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L7_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* PROMPT FOR INPUT$ OUTPUT TEXT$  USAGE: RESULT = M_PromptText(TEXT$,INPUT$)*
*****************************************************************************

L_PromptText equ      8
L8	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           

; SAVE POINTER TO INPUT$, STORED IN (a3) IN REVERSE ORDER 

            move.l   (a3)+,a4                 ; POINTER TO INPUT$
            move.w   (a4)+,d4                 ; LENGTH OF INPUT$ 
            cmpi.w   #79,d4
            ble.s    L8_InputLen
            move.w   #79,d4

L8_InputLen
            move.w   d4,Data-MB(a2)           ; Data = LENGTH OF INPUT$
            move.w   #6,Command-MB(a2)        ; Command = 6  ie Prompt


; COPY TEXT$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO TEXT$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF TEXT$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L8_StrLoop               ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L8_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY TEXT$ TO String
            dbra     d0,L8_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L8_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg

           
; COPY DoorMsg.String INTO INPUT$

            subq.w   #1,d4                    ; d4-1 BECAUSE OF dbra
            Dlea     String,a0                ; POINTER TO DoorMsg.String
            
L8_InpLoop            
            move.b   (a0)+,(a4)+              ; COPY String TO INPUT$
            dbra     d4,L8_InpLoop  

            clr.l    d0                       ; ZERO ANY GARBAGE
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L8_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* STACK INPUT$ OUTPUT TEXT$  USAGE: RESULT = M_SPromptText(TEXT$,INPUT$)    *
*****************************************************************************

L_SPromptText equ      9
L9	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           

; SAVE POINTER TO INPUT$, STORED IN (a3) IN REVERSE ORDER

            move.l   (a3)+,a4                 ; POINTER TO INPUT$
            move.w   (a4)+,d4                 ; LENGTH OF INPUT$ 
            cmpi.w   #79,d4
            ble.s    L9_InputLen
            move.w   #79,d4

L9_InputLen
            move.w   d4,Data-MB(a2)           ; Data = LENGTH OF INPUT$
            move.w   #7,Command-MB(a2)        ; Command = 7  ie SPrompt


; COPY TEXT$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO TEXT$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF TEXT$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L9_StrLoop               ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L9_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY TEXT$ TO String
            dbra     d0,L9_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L9_NoPort                ; IS MAXDoorPort = NULL?

            SendDoorMsg


; COPY DoorMsg.String INTO INPUT$

            subq.w   #1,d4                    ; d4-1 BECAUSE OF dbra
            Dlea     String,a0                ; POINTER TO DoorMsg.String
            
L9_InpLoop            
            move.b   (a0)+,(a4)+              ; COPY String TO INPUT$
            dbra     d4,L9_InpLoop  

            clr.l    d0                       ; ZERO ANY GARBAGE
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L9_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* OUTPUT TEXT$ GET CHAR$  USAGE: RESULT = M_GetChar(TEXT$,CHAR$)            *
*****************************************************************************

L_GetChar   equ      10
L10	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER


; SAVE POINTER TO CHAR$ 

            move.l   (a3)+,a4                 ; POINTER TO CHAR$
            move.w   (a4)+,d4                 ; LENGTH OF CHAR$ 
           
            move.w   #8,Command-MB(a2)        ; Command = 8  ie HotKey


; COPY TEXT$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO TEXT$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF TEXT$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L10_StrLoop              ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L10_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY TEXT$ TO String
            dbra     d0,L10_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L10_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg


; COPY DoorMsg.String INTO CHAR$

            subq.w   #1,d4                    ; d4-1 BECAUSE OF dbne
            Dlea     String,a0                ; POINTER TO DoorMsg.String
            
L10_InpLoop            
            move.b   (a0)+,(a4)+              ; COPY String TO CHAR$, WHILE
            dbne     d4,L10_InpLoop           ; A CHAR!=0 AND <LENGTH OF INPUT$ 

            clr.l    d0                       ; ZERO ANY GARBAGE
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L10_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* TWITS AN IDIOT!  USAGE: RESULT = M_TwitUser                               *
*****************************************************************************

L_TwitUser  equ      11		
L11	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.w   #9,Command-MB(a2)        ; Command = 9  ie TwitUser


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L11_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L11_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* DISPLAYS A ASCII/ANSI FILE  USAGE: RESULT = M_ShowFile(FILE$)             *
*****************************************************************************

L_ShowFile  equ      12		
L12	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.w   #10,Command-MB(a2)       ; Command = 10  ie ShowFile


; COPY FILE$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO FILE$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF FILE$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L12_StrLoop              ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L12_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY FILE$ TO String
            dbra     d0,L12_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L12_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L12_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* CHECKS A FILE TO SEE IF IT'S ONLINE  USAGE: RESULT = M_CheckFile(FILE$)   *
*****************************************************************************

L_CheckFile equ      13		
L13	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.w   #1,Data-MB(a2)           ; Data = 1
            move.w   #11,Command-MB(a2)       ; Command = 11  ie CheckFile


; COPY FILE$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO FILE$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF FILE$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L13_StrLoop              ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L13_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY FILE$ TO String
            dbra     d0,L13_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L13_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE
            cmpi.w   #20,d0
            beq.s    L13_NoPort               ; IF CARRIER DROPPED RETURN
            move.w   Data-MB(a2),d0           ; ELSE RETURN FILE STATUS

L13_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* EDITS A FILE  USAGE: RESULT = M_EditFile(FILE$)                           *
*****************************************************************************

L_EditFile  equ      14		
L14	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.w   #99,Data-MB(a2)          ; Data = 99 MAX LINES
            move.w   #12,Command-MB(a2)       ; Command = 12  ie EditFile


; COPY FILE$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO FILE$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF FILE$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra
            cmpi.w   #78,d0                 
            ble.s    L14_StrLoop              ; IS IT <=78?
            move.w   #78,d0                   ; NO, THEN MAKE IT 78

L14_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY FILE$ TO String
            dbra     d0,L14_StrLoop  

            move.b   #0,(a0)


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L14_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE
            cmpi.w   #20,d0
            beq.s    L14_NoPort               ; IF CARRIER DROPPED RETURN
            move.w   Data-MB(a2),d0           ; ELSE RETURN FILE STATUS

L14_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* GETS USER NUMBER STATS  USAGE: RESULT = M_GetUserNum(TYPE)                *
*****************************************************************************

L_GetUserNum equ      15		
L15	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.l   (a3)+,d0                 ; TYPE
            move.w   d0,Data-MB(a2)           ; Data = TYPE
            move.w   #13,Command-MB(a2)       ; Command = 13  ie GetSNum


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L15_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE
            cmpi.w   #20,d0
            beq.s    L15_NoPort               ; IF CARRIER DROPPED RETURN
            move.w   Data-MB(a2),d0           ; ELSE RETURN DATA FOR TYPE

L15_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* GETS USER STRING STATS  USAGE: RESULT = M_GetUserStr(TYPE,USER$)          *
*****************************************************************************

L_GetUserStr equ      16		
L16	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER

            moveq.l  #0,d0                    ; RETURN 0 FOR AN ERROR


; SAVE POINTER TO USER$, STORED IN (a3) IN REVERSE ORDER

            move.l   (a3)+,a4                 ; POINTER TO USER$
            move.w   (a4)+,d4                 ; LENGTH OF USER$ 
            cmpi.w   #40,d4
            ble.s    L16_NoPort               ; IF LENGTH OF USER$ <= 40
           
            move.l   (a3)+,d0                 ; TYPE
            move.w   d0,Data-MB(a2)           ; Data = TYPE
            move.w   #14,Command-MB(a2)       ; Command = 14  ie GetSVar


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L16_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg


; COPY DoorMsg.String INTO USER$

            subq.w   #1,d4                    ; d4-1 BECAUSE OF dbra
            Dlea     String,a0                ; POINTER TO DoorMsg.String
            
L16_InpLoop            
            move.b   (a0)+,(a4)+              ; COPY String TO INPUT$
            dbra     d4,L16_InpLoop  

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L16_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* CHANGES USER ACCESS LEVEL  USAGE: RESULT = M_NewAccess(ACCESS)            *
*****************************************************************************

L_NewAccess equ      17		
L17	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.l   (a3)+,d0                 ; ACCESS 
            move.w   d0,Data-MB(a2)           ; Data = ACCESS
            move.w   #15,Command-MB(a2)       ; Command = 15  ie NewAccess


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L17_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L17_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* ADDS EXTRA TIME TO USER'S CURRENT ACCOUNT  RESULT = M_AddTime(TIME)       *
*****************************************************************************

L_AddTime   equ      18		
L18	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           
            move.l   (a3)+,d0                 ; TIME 
            move.w   d0,Data-MB(a2)           ; Data = TIME
            move.w   #21,Command-MB(a2)       ; Command = 21  ie NewTime


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L18_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L18_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* DO ANY MAXS FUNCTION  USAGE: RESULT = M_DoFunction(FUNC,EXTRA,PATH$)      *
*****************************************************************************

L_DoFunction equ     19		
L19	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           

; COPY PATH$ INTO DoorMsg.String

            move.l   (a3)+,a1                 ; POINTER TO PATH$
            Dlea     String,a0                ; POINTER TO DoorMsg.String 

            move.w   (a1)+,d0                 ; LENGTH OF PATH$
            subq.w   #1,d0                    ; d0-1 BECAUSE OF dbra

L19_StrLoop:    
            move.b   (a1)+,(a0)+              ; COPY PATH$ TO String
            dbra     d0,L19_StrLoop  

            move.b   #0,(a0)

            
; DoorMsg.Data = EXTRA  DoorMsg.Command = FUNC + 100

            move.l   (a3)+,d1                 ; EXTRA
            move.l   (a3)+,d0                 ; FUNC
            add.l    #100,d0                  ; FUNC + 100

            move.w   d1,Data-MB(a2)           ; Data = EXTRA
            move.w   d0,Command-MB(a2)        ; Command = FUNC + 100


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L19_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L19_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* CHANGE USER DATA  USAGE: RESULT = M_ChangeUserData(TYPE,DATA)             *
*****************************************************************************

L_ChangeUserData   equ   20		
L20	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER
           

; DoorMsg.String = DATA

            move.l   (a3)+,d0                 ; DATA
            Dlea     String,a0                ; POINTER TO DoorMsg.String

            move.l   d0,(a0)                  ; DoorMsg.String = DATA
            

; DoorMsg.Data = TYPE

            move.l   (a3)+,d0                 ; TYPE

            move.w   d0,Data-MB(a2)           ; Data = TYPE
            move.w   #200,Command-MB(a2)      ; Command = 200  ie ChangeUserInt


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L20_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE

L20_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* GETS A KEY INSTANTLY  USAGE: RESULT = M_GetKey                            *
*****************************************************************************

L_GetKey    equ      21		
L21	        
            movem.l  a4-a6,-(sp)

            Dload    a2                       ; EXTENSION DATA ZONE BASE POINTER

            move.w   #201,Command-MB(a2)      ; Command = 201  ie GetKey


; SEND MESSAGE STRUCTURE TO MAXDoorPort AND WAIT FOR A REPLY AT MyReplyPort

            move.l   MAXDoorPort-MB(a2),d0
            beq.s    L21_NoPort               ; IS MAXDoorPort = NULL?

            SendDoorMsg

            clr.l    d0
            move.w   Command-MB(a2),d0        ; RETURN CARRIER STATE
            cmpi.w   #20,d0                   ; IF CARRIER DROPPED RETURN
            beq.s    L21_NoPort               ; ELSE RETURN LOCAL OR REMOTE
            move.w   Data-MB(a2),d0           ; KEYPRESS.
            move.l   #31,d1                   ; REMOTE = 0x8XXXXXXX
            lsl.l    d1,d0                    ; LOCAL  = 0x0XXXXXXX


; POINT TO KEY IN DoorMsg.String

            Dlea     String,a0                ; POINTER TO DoorMsg.String
            or.b     (a0),d0                  ; RETURN 0xRXXXXXKK  KK = KEY

L21_NoPort:
            moveq.l  #0,d2                    ; RETURN IS LONG
            move.l   d0,d3                    ; RETURN d0  

            movem.l  (sp)+,a4-a6
            rts


*****************************************************************************
* RELOCATABLE CreatePort and DeletePort from Amiga.lib                      *
*****************************************************************************

L_Amiga.Lib	equ	22

L22	
	lsl.w	#2,d0
	lea	JJJmp(pc),a0
	jmp	0(a0,d0.w)

JJJmp	
  bra	_CreatePort
	bra	_DeletePort
	bra	_CreateExtIO
	bra	_DeleteExtIO

_CreatePort
	MOVEM.L	D2-7/A2,-(A7)
	MOVE.L	$20(A7),D4
	MOVE.B	$27(A7),D3
	MOVE.L	#$FFFFFFFF,-(A7)
	BSR	L7C7DB14
	MOVE.L	D0,D5
	MOVE.L	D5,D6
	MOVEQ	#-1,D2
	CMP.L	D5,D2
	ADDQ.L	#4,A7
	BNE.S	L7C7D97A
	MOVEQ	#0,D0
	BRA	L7C7D9E6
L7C7D97A	
  MOVE.L	#$10001,-(A7)
	PEA	$22.W
	BSR	L7C7DAD0
	MOVEA.L	D0,A2
	EXG	D7,A2
	TST.L	D7
	EXG	D7,A2
	ADDQ.L	#8,A7
	BNE.S	L7C7D9A4
	MOVE.L	D6,-(A7)
	BSR	L7C7DB28
	MOVEQ	#0,D0
	ADDQ.L	#4,A7
	BRA.S	L7C7D9E6
L7C7D9A4	
  MOVE.L	D4,$A(A2)
	MOVE.B	D3,9(A2)
	MOVE.B	#4,8(A2)
	CLR.B	$E(A2)
	MOVE.B	D6,$F(A2)
	CLR.L	-(A7)
	BSR	L7C7DB00
	MOVE.L	D0,$10(A2)
	TST.L	D4
	ADDQ.L	#4,A7
	BEQ.S	L7C7D9D8
	MOVE.L	A2,-(A7)
	BSR	L7C7DB3C
	ADDQ.L	#4,A7
	BRA.S	L7C7D9E4
L7C7D9D8	
  PEA	$14(A2)
	BSR	_NewList
	ADDQ.L	#4,A7
L7C7D9E4	
  MOVE.L	A2,D0
L7C7D9E6	
  MOVEM.L	(A7)+,D2-7/A2
	RTS

_DeletePort
	MOVEM.L	D2/A2,-(A7)
	MOVEA.L	$C(A7),A2
	TST.L	$A(A2)
	BEQ.S	L7C7DA04
	MOVE.L	A2,-(A7)
	BSR	L7C7DB50
	ADDQ.L	#4,A7
L7C7DA04	
  MOVE.B	#$FF,8(A2)
	MOVEQ	#-1,D2
	MOVE.L	D2,$14(A2)
	MOVEQ	#0,D2
	MOVE.B	$F(A2),D2
	MOVE.L	D2,-(A7)
	BSR	L7C7DB28
	PEA	$22.W
	MOVE.L	A2,-(A7)
	BSR	L7C7DAE8
	LEA	$C(A7),A7
	MOVEM.L	(A7)+,D2/A2
	RTS

_CreateExtIO
	MOVEM.L	D2-4,-(A7)
	MOVE.L	$10(A7),D2
	MOVE.L	$14(A7),D3
	TST.L	D2
	BNE.S	L7C7DA54
	MOVEQ	#0,D0
	BRA.S	L7C7DA82
L7C7DA54	
  MOVE.L	#$10001,-(A7)
	MOVE.L	D3,-(A7)
	BSR	L7C7DAD0
	MOVEA.L	D0,A0
	EXG	D4,A0
	TST.L	D4
	EXG	D4,A0
	ADDQ.L	#8,A7
	BNE.S	L7C7DA72
	MOVEQ	#0,D0
	BRA.S	L7C7DA82
L7C7DA72	
  MOVE.B	#5,8(A0)
	MOVE.W	D3,$12(A0)
	MOVE.L	D2,$E(A0)
	MOVE.L	A0,D0
L7C7DA82	
  MOVEM.L	(A7)+,D2-4
	RTS

_DeleteExtIO
	MOVEM.L	D2-3,-(A7)
	MOVEA.L	$C(A7),A0
	EXG	D3,A0
	TST.L	D3
	EXG	D3,A0
	BEQ	L7C7DABE
	MOVE.B	#$FF,8(A0)
	MOVEQ	#-1,D2
	MOVE.L	D2,$14(A0)
	MOVEQ	#-1,D2
	MOVE.L	D2,$18(A0)
	MOVEQ	#0,D2
	MOVE.W	$12(A0),D2
	MOVE.L	D2,-(A7)
	MOVE.L	A0,-(A7)
	BSR	L7C7DAE8
	ADDQ.L	#8,A7
L7C7DABE	
  MOVEM.L	(A7)+,D2-3
	RTS
L7C7DAD0
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVEM.L	8(A7),D0-1
	JSR	-$C6(A6)
	MOVEA.L	(A7)+,A6
	RTS
L7C7DAE8
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVEA.L	8(A7),A1
	MOVE.L	$C(A7),D0
	JSR	-$D2(A6)
	MOVEA.L	(A7)+,A6
	RTS
L7C7DB00
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVEA.L	8(A7),A1
	JSR	-$126(A6)
	MOVEA.L	(A7)+,A6
	RTS
L7C7DB14
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVE.L	8(A7),D0
	JSR	-$14A(A6)
	MOVEA.L	(A7)+,A6
	RTS
L7C7DB28
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVE.L	8(A7),D0
	JSR	-$150(A6)
	MOVEA.L	(A7)+,A6
	RTS
L7C7DB3C
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVEA.L	8(A7),A1
	JSR	-$162(A6)
	MOVEA.L	(A7)+,A6
	RTS
L7C7DB50
	MOVE.L	A6,-(A7)
	MOVEA.L	$4.w,A6
	MOVEA.L	8(A7),A1
	JSR	-$168(A6)
	MOVEA.L	(A7)+,A6
	RTS

_NewList
	move.l	4(a7),a0
	move.l	a0,(a0)
	addq.l	#4,(a0)
	clr.l	4(a0)
	move.l	a0,8(a0)
	rts


*****************************************************************************
* ERROR MESSAGES                                                            *
*****************************************************************************

L_Custom	  equ	     23

; First routine

L23	        moveq	   #0,d1                
          	moveq	   #ExtNb,d2
           	moveq	   #-1,d3
          	Rjmp	   L_ErrorExt


; Second routine

L24	        moveq	   #0,d1                
          	moveq	   #ExtNb,d2
          	moveq	   #-1,d3
          	Rjmp	   L_ErrorExt
L25


*****************************************************************************
* IDENTIFICATION AND VERSION STRING                                         *
*****************************************************************************

C_Title	    dc.b 	   31,"MAXS AMOS Door Handler, V0.20 Ari Tsironis 01-May-94",0
	          even


*****************************************************************************
* THE END                                                                   *
*****************************************************************************

C_End	      dc.w	   0
