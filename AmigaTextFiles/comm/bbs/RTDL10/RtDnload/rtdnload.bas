'RtDnload by Peter Zelezny.

'1-sep-95

REM $OPTION Y

LIBRARY "exec.library"
LIBRARY "reqtools.library"

DECLARE FUNCTION AllocMem& LIBRARY
DECLARE FUNCTION FreeMem& LIBRARY
DECLARE FUNCTION FindTask& LIBRARY
DECLARE FUNCTION AllocSignal& LIBRARY
DECLARE FUNCTION AddPort& LIBRARY
DECLARE FUNCTION Forbid& LIBRARY
DECLARE FUNCTION Permit& LIBRARY
DECLARE FUNCTION FindPort& LIBRARY
DECLARE FUNCTION PutMsg& LIBRARY
DECLARE FUNCTION WaitPort& LIBRARY
DECLARE FUNCTION GetMsg& LIBRARY
DECLARE FUNCTION RemPort& LIBRARY
DECLARE FUNCTION FreeSignal& LIBRARY
DECLARE FUNCTION OpenLibrary& LIBRARY
DECLARE SUB CloseLibrary& LIBRARY

DECLARE FUNCTION rtAllocRequestA& LIBRARY
DECLARE FUNCTION rtFileRequestA& LIBRARY
DECLARE FUNCTION rtChangeReqAttrA& LIBRARY
DECLARE SUB rtFreeRequest& LIBRARY

ON ERROR GOTO error.handler

DIM SHARED PortAddress&, TaskAddr&, Dummy%, MsgPortName$, MsgPortName2$
DIM SHARED Sig%, ControlPort&, ErrCode%, Arg1&, Arg2&, Reply&
DIM SHARED i&, j%, Flag%, esc$, a%, L&, NameMemAddr&

e$=CHR$(27)+"["
clear$=e$+"0m"+e$+"2J"+e$+"1H"
cr$=CHR$(13)
q$=CHR$(34)
prob$="Unknown Error!"

IF COMMAND$="" THEN ? "Door control port not located": end

port$=COMMAND$
MsgPortName$="DoorControl"+COMMAND$+CHR$(0)
MsgPortName2$="DoorReply"+COMMAND$+CHR$(0)

CALL GetPort

IF ControlPort&=0 THEN end
IF ErrCode% <> 0 THEN GOTO Exitt

'######################################################################
'Your Programme goes in here!!!!!
'######################################################################

 PS clear$+cr$+cr$+"SysOp is selecting a file to send..."+cr$+cr$
 GOSUB FINDSCREEN

 startdir$ = CURDIR$
  
 fr&=rtAllocRequestA&(0&,VARPTR(frtags&(0)))
 TAGLIST VARPTR(frtags&(0)),50&,startdir$,0
 x=rtChangeReqAttrA&(fr&,VARPTR(frtags&(0)))
 title&=SADD("Choose file to send..."+CHR$(0))
 filename&=SADD(STRING$(129,0))
 TAGLIST VARPTR(t&(0)), 7&, maxscr&, 41&, 800&, 42&, "Send",0

 IF fr& THEN
	IF rtFileRequestA&(fr&,filename&,title&,VARPTR(t&(0))) THEN
		fil$ = PEEK$(filename&)
		dir$ = PEEK$(PEEKL(fr&+16&))
	ELSE
		PS "Requestor failed."+cr$+cr$+"%Z"
		GOTO exitt
	END IF
	rtFreeRequest fr&
 ELSE
 	PS "Can't allocate memory."+cr$+cr$+"%Z"
	GOTO exitt
 END IF

 IF UCASE$(dir$)="RAM DISK:" THEN dir$="Ram:"
 IF RIGHT$(dir$,1)<>":" AND RIGHT$(dir$,1)<>"/" THEN dir$=dir$+"/"
 fildir$=dir$+fil$
 
 IF FEXISTS(fildir$) THEN
 	OPEN "I",#1,fildir$
 	size$=STR$(LOF(1))
 	CLOSE 1
 	PS clear$+cr$+cr$
 	PS e$+"1;35mFile: "+e$+"36m"+fil$+"   "
 	PS e$+"35mSize:"+e$+"36m"+size$+e$+"0m"+cr$+cr$
 	PS "Press [ESC] to abort -  Any other key to start."
 	hotkey k$
 	IF k$=CHR$(27) THEN exitt
 	dfunc 124,100,fildir$
 ELSE
 	PS cr$+cr$+"File not found."+cr$+cr$+"%Z"
 END IF
 GOTO exitt
 
FINDSCREEN:
 IntBase& = OpenLibrary&(SADD("intuition.library"),34&)
 scr& = PEEKL(IntBase&+60%)
 p$=port$
 IF p$="0" THEN p$="1"
 DO
	nextscreen&	= PEEKL(scr&)
	title&		= PEEKL(scr&+22%)
	stitle$		= PEEK$(title&)
	IF LEFT$(stitle$,16)=p$+": M A X's BBS V" THEN
		maxscr&=scr&
		EXIT LOOP
	END IF
	scr&=nextscreen&
	IF scr&=0 THEN EXIT LOOP
 LOOP
 CloseLibrary& IntBase&
 RETURN

'######################################################################
'And ends here
'######################################################################

Exitt:
CALL FreePort
LIBRARY CLOSE
SYSTEM

Error.Handler:
PS cr$+e$+"31mError at line: "+STR$(ERL)+cr$+cr$
PS "Please notify %a!!"+cr$+cr$+"%Z"
GOTO exitt

SUB getport STATIC
PortAddress&=AllocMem&(140&,&H10001)
IF PortAddress&=0 THEN
  PRINT "Couldn't allocate the memory!"
  ErrCode%=2
  GOTO Out
END IF  
TaskAddr&=FindTask&(0)
POKEL PortAddress&+16,TaskAddr&
Sig% = AllocSignal&(-1)
IF Sig%<0 THEN
  ErrCode%=3
  GOTO Out
END IF
POKE PortAddress&+8,4
POKEL PortAddress&+10,SADD(MsgPortName2$)
POKE PortAddress&+15,Sig%
POKE PortAddress&+42,5
POKEW PortAddress&+52,106
POKEL PortAddress&+48,PortAddress&
Reply&=AddPort&(PortAddress&)
Dummy%=Forbid&
ControlPort&=FindPort&(SADD(MsgPortName$))
Dummy%=Permit&
Out:
END SUB

SUB FreePort STATIC
On ErrCode% goto Sig1,Sig2,Sig3,Sig4
CALL GetMsgPrt (Arg1&,Arg2&)
POKEW Arg2&,20
Reply&=PutMsg&(ControlPort&,Arg1&)
Pause:
Reply&=WaitPort&(PortAddress&)
Reply&=GetMsg&(PortAddress&)
IF Reply&=0 THEN GOTO Pause
Sig4:
Dummy%=RemPort&(PortAddress&)
Dummy%=PEEK(PortAddress&+15)
Dummy%=FreeSignal&(Dummy%)
Sig3:
Dummy%=FreeMem(PortAddress&,140&)
Sig2:
Sig1:
END SUB

SUB GetMsgPrt(Arg1&, Arg2&) STATIC
Arg1&=PortAddress&+34
Arg2&=PortAddress&+54
POKEL Arg2&+2,0
END SUB

SUB PS(St$) STATIC
CALL GetMsgPrt (Arg1&, Arg2&)
POKEW Arg2&,1       'command number
POKEW Arg2&+2,0     'terminating null
FOR i&=1 TO LEN(St$)
  POKE Arg2&+3+i&,ASC(MID$(St$,i&,1))   'put the string in
NEXT
POKE Arg2&+3+i&,0
CALL PutWaitMsg
END SUB

SUB PutWaitMsg STATIC
LOCAL Temp&, Locn&, Tempp&
Reply&=PutMsg&(ControlPort&,Arg1&)
Pause1:
Reply&=WaitPort&(PortAddress&)
Reply&=GetMsg&(PortAddress&)
IF Reply&=0 THEN GOTO Pause1
Tempp&=PEEKW(Reply&+24&+80&)
IF Tempp&<>0 THEN GOTO Exitt                'lost carrier
END SUB

SUB DFunc (f%,e%,file$) STATIC
CALL GetMsgPrt(Arg1&,Arg2&)
POKEW Arg2&,f%
POKEW Arg2&+2,e%
if file$<>""
  FOR i&=1 TO LEN(file$)
    POKE Arg2&+3+i&,ASC(MID$(file$,i&,1))
  NEXT
  POKE Arg2&+3+i&,0
end if
CALL PutWaitMsg
END SUB

SUB hotkey (K$) STATIC
CALL GetMsgPrt (Arg1&, Arg2&)
POKEW Arg2&,8       'command number
POKEW Arg2&+2,0     'terminating null
POKE Arg2&+3+i&,0
CALL PutWaitMsg
K$=CHR$(PEEK(Arg2&+4))
END SUB
