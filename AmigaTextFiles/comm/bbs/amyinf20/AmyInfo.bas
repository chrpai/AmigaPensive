REM ******************************************************************

REM Program- Amy Info Door
REM    Date- 17 May 1991
REM  Author- Peter Francis Deane
REM
REM Recompiled for OzMetro 23 Nov 94
REM
REM Version- 2.0

REM ******************************************************************


ON ERROR GOTO woops

' Set things so you can DO things!
CLEAR ,30000&
CLEAR
DEFLNG a-z
DEFINT k
cr$=CHR$(13)+CHR$(10)
de$=CHR$(8)+" "+CHR$(8)
null$=CHR$(0)
esc$=CHR$(27)

TestIt$="Nup!"

IF Testit$<>"YES" THEN
  'read ram:userdata in
  OPEN "I",#2,"RAM:USERDATA"
  LINE INPUT#2,tpc$
  LINE INPUT#2,uname$
  LINE INPUT#2,id$
  LINE INPUT#2,lev$
  LINE INPUT#2,bd$
  LINE INPUT#2,ring$
  LINE INPUT#2,path$
  CLOSE#2
ELSE
  'setup dummy variables
  tpc$="60"
  id$="1"
  lev$="4"
  ring$="0"
  bd$="2400"
  uname$="PRESIDENT REAGAN"
  path$=""
END IF


'convert to short integers
tpc%=VAL(tpc$)
rtime%=tpc%
id%=VAL(id$)
lev%=VAL(lev$)
ring%=VAL(ring$)
Baud=VAL(bd$)
Title$="AmyInfo (by PD) - User #"+id$+" ["+uname$+"]  Lev:"+lev$
IF ring%=1 THEN
  title$=title$+" Baud: "+bd$
ELSE
  title$=title$+" Baud: Local Login"
END IF


'open modem
IF ring%=1 THEN
  OPEN "COM1:"+bd$+",n,8,1" AS #1 LEN=4096
  GOSUB delay1
END IF


'HouseKeeping
TIMER ON
ON TIMER(60) GOSUB RtimeDec
timeon=TIMER
RANDOMIZE TIMER
LIBRARY "dos.library"
DECLARE FUNCTION Execute LIBRARY
LIBRARY "graphics.library"
DECLARE FUNCTION SetSoftStyle LIBRARY


'get a local environment
SCREEN 1,640,202,3,2
COLOR 0,1
WINDOW 1,title$,(0,2)-(631,188),0,1
WINDOW OUTPUT 1
PALETTE 0, 0!, 0!, 0!
PALETTE 1, .93, .2,  0!
PALETTE 2, .22, .87, 0!
PALETTE 3, 1!, 1!, .31
PALETTE 4, .1, .4, .8
PALETTE 5, .8, 0!, .93
PALETTE 6, 0!, .93, .93
PALETTE 7, 1!,1!,.93


'First Words
Matt 0
GOSUB ClearScreen
MPrint cr$+"Welcome to the Amy Info Door"
MPrint cr$+cr$+"(Version 2.0 - 23 Nov 1994)"
MPrint cr$+cr$+"Another Quick Hack by Peter Deane."
MPrint cr$+cr$+"This door allows you to run various"
MPrint cr$+"diagnostic programs in order to snoop on" 
MPrint cr$+"the state of the Amiga that this BBS is"
MPrint cr$+"running on."+cr$
GOSUB TimeCheck


BBSPath$="BBS:BBSFiles/"


' Get page length they have set on the BBS
GOSUB UserlogPrep
GET #3,id%
Page%=VAL(dm2$)
IF Page%=0 THEN Page%=22
IF ring%=0 THEN Page%=22
CLOSE #3


'set config variables
Lines%=1
GOSUB SetAnsi 
GOSUB SetPage


'define changing colours
DIM Colour%(4)
Colour%(1)=32
Colour%(2)=35
Colour%(3)=36
Colour%(4)=33
ColourCount=1


REM ******************************************************************
Start:
REM ******************************************************************

1000

'Check config file
flnm$=path$+"AmyInfo.config"
OPEN flnm$ FOR INPUT AS #2
count=0
WHILE EOF(2)<>-1
  LINE INPUT #2,temp$
  count=count+1
  IF (count MOD 5) =1 THEN
    IF LEN(temp$)<>1 THEN ERROR 62
  END IF
WEND
CLOSE #2


'Now we know how many things are there, and the menu looks reasonable
'let's read the thing into an array.
items=INT(count/5)
DIM Choice$(items+2,5)


'Choice$(items, 5):-
' 1 = Keypress   (therefore only 1 character!)
' 2 = Menu Entry printed up
' 3 = Command
' 4 = Arguments (or if none, a blank line)
' 5 = Text printed while processing the command (could be none!)


flnm$=path$+"AmyInfo.config"
OPEN flnm$ FOR INPUT AS #2
FOR j=1 TO items
  FOR k=1 TO 5
    LINE INPUT #2, Choice$(j,k)
  NEXT
NEXT


REM ******************************************************************
TopMenu:
REM ******************************************************************

Matt 0
GOSUB ClearScreen

Matt 3
MCol 31
MPrint cr$+"System Info Available:"+cr$
Matt 0
FOR k=1 TO items
  colourout=(k MOD 4)+1
  MCol Colour%(colourout)
  MPrint cr$+"   "+Choice$(k,2)
NEXT
colourout=(k MOD 4)+1
MCol Colour%(colourout)
MPrint cr$+"   Q - Quit This Door"

GOSUB MainPrompt
in%=0:il%=1:GOSUB Modem.in
IF in$="" THEN TopMenu
IF in$="Q" THEN FinishUp

count=0

Loopback:
  count=count+1
  IF count>items THEN TopMenu
  IF Choice$(count,1)=in$ THEN GOTO ValChoice
GOTO Loopback

ValChoice:
GOSUB ClearScreen
MAtt 1
MCol 31
MPrint "Executing - "+Choice$(count,2)+cr$
Matt 0
MAtt 3
MCol 37
MPrint cr$+Choice$(count,5)+cr$+cr$
Matt 0

cmd$=Choice$(count,3)+" >ram:temp"
IF Choice$(count,4)<>""THEN cmd$=cmd$+" "+Choice$(count,4)
CALL Dosexe(cmd$)

flnm$="ram:temp"
ColourCount=ColourCount+1
ColourOut=(ColourCount MOD 4)+1
MCol Colour%(ColourOut)

lines%=3
GOSUB fileout
GOSUB AnyKey
KILL flnm$

MCol 35
MPrint cr$+cr$+cr$+"Any more snooping on your sysop's Amiga (Y/n)? "
in%=3:il%=1:YES=1:GOSUB Modem.in
IF in$="YES" THEN TopMenu


REM ******************************************************************
FinishUp:
REM ******************************************************************

Gosub ClearScreen
MCol 35
MPrint "Hope you found this door quite interesting!"+cr$
Mcol 32
MPrint cr$+"If you did, then leave a message to the Sysop asking him"+cr$
MPrint "to support the shareware concept and register the program."+cr$
Mcol 36
GOSUB Delay1
GOSUB Delay1
Mprint cr$+"The AmyInfo Door was written by Peter Deane"+cr$
MPrint "Inquestor BBS, Newcastle, NSW, Australia"+cr$+cr$
Mcol 31
MPrint "011-61-49-72-1647 from the USA"+cr$
MPrint "(049) 72-1647 from Australia"+cr$
GOSUB Delay1
GOSUB Delay1
GOTO Bye


REM ******************************************************************
Woops:
REM ******************************************************************

Matt 0
' These first two are needed by the Fileout Subroutine

IF ERR=62 AND ERL=64999 THEN RESUME Finish 'Input past end with a doc.

IF ERR=15 AND ERL=64999 THEN               'string too long!! >32767 chars
   MPrint cr$+cr$+cr$+cr$+"This file is NOT a textfile, it has no carriage returns in it at all!"
   MPrint cr$+cr$+cr$+"Aborting inspection of this file."
   GOSUB AnyKey
   RESUME Finish
END IF    

'With config files, etc, etc, etc...

IF ERR=62 AND ERL=1000 THEN
   MPrint cr$+cr$+cr$+"Config file error! Oh Dear! Better read the docs on how to"
   MPrint cr$+"set up the config file properly!"
   GOSUB AnyKey
END IF

IF ERR=53 THEN MPrint cr$+cr$+"Sorry, I Can't Find The File I Need!":GOSUB AnyKey:RESUME TopMenu

'if all else fails...!!

IF ERR<>99 THEN
 Mprint cr$+cr$+"Oh Dear! We have had a fatal error occur."
 Mprint cr$+cr$+"The Error Number was:- "+STR$(ERR)
 Mprint cr$+"There will be a note made of this for the sysop, but please"
 Mprint cr$+"remind him by leaving a message to the Sysop and let him"
 Mprint cr$+"know what you were doing just before the crash."
 GOSUB AnyKey
END IF

errflnm$=path$+"AmyInfo.errors!"
OPEN errflnm$ FOR APPEND AS # 65
PRINT #65, ""
PRINT #65, "Fatal Error Occured At ";TIME$;" on ";DATE$
PRINT #65, "Error #";STR$(ERR);" Last Line #";STR$(ERL)
PRINT #65, ""
IF ERR=99 THEN
  PRINT #65, UName$; " dropped carrier."
  PRINT #65, ""
END IF
IF ERR=62 THEN
  PRINT #65, ""
  PRINT #65, "Configuration file error. Please check AmyInfo.config!"
  PRINT #65, ""
  CLOSE #65
END IF
CLOSE #65
GOTO Bye


REM ******************************************************************
Bye:
REM ******************************************************************

Matt 0
Mprint cr$+cr$+"Hope to see you back soon!"
WINDOW CLOSE 1
IF Testit$<>"YES" THEN
  KILL "RAM:USERDATA"
END IF
IF ring%=1 THEN TIMER OFF
CLOSE#1
LIBRARY CLOSE
END



'--------------- PARAMETER SETTNGS


REM ******************************************************************
SetAnsi:
REM ******************************************************************

Matt 0
Mprint cr$+cr$+"Do you want ANSI colours (Y/n)? "
in%=3:il%=1:yes=1:GOSUB modem.in

IF in$="YES" THEN
  ansi%=1
  Mcolour 32,40
  Mprint cr$+cr$+"ANSI now "
  MColour 31,40
  MPrint "ON!"+cr$
ELSE 
  ansi%=0
  Mprint cr$+cr$+"ANSI now OFF!"
END IF
RETURN



REM ******************************************************************
SetPage:
REM ******************************************************************

IF ring%<>1 THEN Page%=22

SetPageLoop:

GOSUB ClearScreen
MColour 33,40
IF Page%>=32765 THEN
  Mprint "Page pausing now OFF."
ELSE
  Mprint "Page pausing now set to every"+STR$(Page%)+" lines."
END IF
Mcolour 31,40
Mprint cr$+cr$+"How many lines (<10>-<98> <99>=Off <Return>=No Change)? "
in%=4:il%=2
GOSUB Modem.in

temp=VAL(in$)
IF in$="" THEN RETURN
Mcolour 35,40
IF temp<10 THEN
  Mprint cr$+"That's ridiculous. You MUST have more than"+STR$(temp)+" lines on your terminal!"  
  GOSUB AnyKey
  GOTO SetPageLoop
END IF

IF temp>32 AND temp<99 THEN
  Mprint cr$+"Are you sure you have"+STR$(temp)+" lines on your screen (Y/n)? "
  in$=""
  in%=3:il%=1:YES=1
  GOSUB Modem.in
  IF in$<>"YES" THEN GOTO SetPageLoop
  Page%=Temp
END IF

IF temp=99 THEN
  Mprint cr$+"Turn page pausing off. Are you sure (Y/n)? "
  in$=""
  in%=3:il%=1:YES=1
  GOSUB Modem.in
  IF in$<>"YES" THEN GOTO SetPageLoop
  Temp=32767
END IF  

Page%=Temp
GOTO SetPageLoop

'--------------- MISC USEFUL FUNCTIONS

REM ******************************************************************
AnyKey:
REM ******************************************************************

Matt 0
Mprint cr$+"Any_Key: "
GOSUB TimeCheck
in%=0:il%=1
GOSUB Modem.in
RETURN



REM ******************************************************************
ClearScreen:
REM ******************************************************************    
                                                  
GOSUB TimeCheck
 Mprint cr$+cr$+cr$+cr$+CHR$(12)
 'Just in case the chr$(12) doesn't work, a few crs
 CLS
 Matt 0
RETURN


REM ******************************************************************
MainPrompt:
REM ******************************************************************

GOSUB TimeCheck
GOSUB DateCheck
Matt 3
Mcol 32
MPrint cr$+cr$+Time2$+"   "
MCol 35
MPrint Date2$
MCol 33
MPrint "   (Time Left:"+STR$(rtime%)+")"
MCol 31
MPrint "    Your Choice: "
Matt 0
RETURN


REM ******************************************************************
UserlogPrep:
REM ******************************************************************
    'open and field the userlog.

flnm$=BBSPath$+"userlog"
OPEN flnm$ AS #3 LEN=125
FIELD #3,25 AS un$,10 AS pa$,1 AS le$,25 AS fr$,12 AS ph$,10 AS dl$,6 AS lm$,6 AS lf$,10 AS ind$,3 AS tc$,3 AS td$,1 AS sw$,1 AS pr$,1 AS mo$,5 AS cn$,4 AS dm1$,2 AS dm2$
RETURN


'--------------- The Metro Clock!


REM ******************************************************************
TimeCheck:
REM ******************************************************************

t.t=FRE(0)
time1$=TIME$
t.t=VAL(LEFT$(time1$,2))

IF t.t>11 THEN a$=" PM"
IF t.t<12 THEN a$=" AM"
IF t.t=0 THEN
  t.t=12
  time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6)
  GOTO timeout
END IF
IF t.t<10 THEN
  time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6)
  GOTO timeout
END IF
IF t.t>21 THEN
  t.t=t.t-12
  time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6)
  GOTO timeout
END IF
IF t.t>12 THEN 
  t.t=t.t-12
  time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6)
END IF

Timeout:

Time2$=time1$+a$
IF rtime% >0 THEN RETURN
Mprint cr$+cr$+"Sorry, Time Limit Exceeded!"
GOTO Bye


REM ******************************************************************
delay1:
REM ******************************************************************
pause&=TIMER
WHILE TIMER<pause&+1
WEND
RETURN

REM ******************************************************************
rtimedec:
REM ******************************************************************

Rtime%=rtime%-1
RETURN


REM ******************************************************************
DateCheck:
REM ******************************************************************

'Should call this with GOSUB TimeCheck, GOSUB DateCheck
'Then the strings Time2$ and Date2$ will be the ones to use in the prog

temp$=DATE$
Date2$=RIGHT$(temp$,8)
month%=VAL(LEFT$(temp$,2))
IF month%=1 THEN month$="January"
IF month%=2 THEN month$="February"
IF month%=3 THEN month$="March"
IF month%=4 THEN month$="April"
IF month%=5 THEN month$="May"
IF month%=6 THEN month$="June"
IF month%=7 THEN month$="July"
IF month%=8 THEN month$="August"
IF month%=9 THEN month$="September"
IF month%=10 THEN month$="October"
IF month%=11 THEN month$="November"
IF month%=12 THEN month$="December"
Date2$=month$+Date2$
RETURN


'--------------- Modem receiving


REM ******************************************************************
Modem.in:
REM ******************************************************************

idle&=TIMER
key$=""
in$=""
a$=""
a=0
b=0
keyy=0

WHILE keyy<>1

  a$=INKEY$
  key$=a$
  a=ASC(a$)

'remote caller online and no local key pressed - local always overides remote

  IF ring%=1 AND a=0 THEN
    cd=PEEKL(12570624&)
    cd=cd AND 8192
    IF cd<>0 THEN ERROR 99
    IF LOC(1)>0 THEN
      a$=INPUT$(1,1)
      key$=a$
      a=ASC(a$)
    END IF
  END IF

  IF a=0 THEN mod.in2

  IF a=13 THEN 
    keyy=1
  ELSE
    b=LEN(in$)
    IF (a=8 OR a=127) AND b>0 THEN
      b=b-1
      in$=LEFT$(in$,b)
      Mprint de$
    END IF
    IF b=il% THEN mod.in2

  'Single Uppercase Hotkey
    IF in%=0 THEN
      IF a>31 AND a<127 THEN
        key$=UCASE$(key$)
        Mprint key$
        in$=key$
        keyy=1
      END IF
    END IF

  END IF  '(of a=13)
 
'Upper & Lower case
  IF in%=1 AND a>31 AND a<127 THEN
    Mprint key$
    in$=in$+key$
  END IF

'UPPERCASE only
  IF in%=2 AND a>31 AND a<127 THEN
   key$=UCASE$(key$)
   Mprint key$
   in$=in$+key$
  END IF

'YES/NO normal response (Percy's)
  IF in%=3 THEN
    IF a=13 AND YES=1 THEN
      Mprint "YES"+cr$
      in$="YES"
      keyy=1
    END IF
    IF a=13 AND YES=0 THEN
      Mprint "NO"+cr$
      in$="NO"
      keyy=1
    END IF
    IF a=89 OR a=121 THEN
      Mprint "YES"+cr$
      in$="YES"
      keyy=1
    END IF
    IF a=78 OR a=110 THEN
      Mprint "NO"+cr$
      in$="NO"
      keyy=1
    END IF
  END IF

'Number Input only
  IF in%=4 THEN
    IF a>47 AND a<58 THEN
      Mprint key$
      in$=in$+key$
    END IF
  END IF

'Y/n/= or space with no advancing.
  IF in%=5 THEN
    IF a=13 AND YES=1 THEN
      in$="YES"
      keyy=1
    END IF
    IF a=13 AND YES=0 THEN
      Mprint " NO"+cr$
      in$="NO"
      keyy=1
    END IF
    IF a=89 OR a=121 THEN
      in$="YES"
      keyy=1
    END IF
    IF a=78 OR a=110 THEN
      Mprint " NO"+cr$
      in$="NO"
      keyy=1
    END IF
    IF a=61 THEN
      in$="="
      keyy=1
    END IF
    IF a=32 THEN
      Mprint " QUIT"+cr$
      in$="NO"
      keyy=1
    END IF
  END IF

' add more in%=xxxx here....

mod.in2:

' Check idle timer (10 mins lev 8/9, 5 mins others)
  IF TIMER>idle&+300 THEN
    IF lev%>7 AND TIMER>idle&+600 THEN Bye
    IF lev%<8 THEN Bye
  END IF

mod.in2.0:
WEND  'Back up to about 5 lines below Modem.in:


'Get rid of old keystrokes
IF ring%=1 THEN
  WHILE LOC(1)>0
    a$=INPUT$(1,1)
  WEND
END IF

a$=INKEY$
a$=INKEY$
a$=INKEY$

mod.in2.1:
RETURN



'--------------- Send a whole file with/without page pausing


REM ******************************************************************
Fileout:
REM ******************************************************************

' Usage flnm$="file to send":GOSUB Fileout 
' Paged Output if required, this routine is getting bloody good!

' Variables Used:
'    FStorePage = Temporary Holding of Page length!
'    Page%      =  Page length
'    Lines%     =  How many lines already printed

GOSUB TimeCheck
keyin$=""
KillMore$=de$+de$+de$+de$+de$+de$+de$+de$+de$+de$+de$+de$+de$  '13 of em
FStorePage=Page%
OPEN flnm$ FOR INPUT AS #9 LEN=100
  WHILE EOF(9)<>-1
64999 :  'In case of Input Past End in the file.
    LINE INPUT #9, o$
    Mprint o$+cr$
    keyin$=INKEY$
    IF keyin$<>"" THEN XoffCheck   'local keyboard gets in first!

    IF ring%=1 THEN
      cd=PEEKL(12570624&)
      cd=cd AND 8192
      IF cd<>0 THEN ERROR 99
      IF LOC(1)>0 THEN keyin$=INPUT$(1,1)
    END IF

XoffCheck:
    keyin%=ASC(keyin$)
    IF keyin%=32 THEN GOTO Finish  'if space pressed, finish send. Rem this
                                   'out if you don't want the Abort option.
    IF keyin%=19 OR keyin%=112 OR keyin%=80 THEN
      hold=1
      pause=TIMER+30
      WHILE hold=1
        IF TIMER>=pause THEN hold=0
        IF hold=1 THEN
          IF ring%=1 THEN 
            IF LOC(1)>0 THEN keyin$=INPUT$(1,1)
          ELSE
            keyin$=INKEY$
          END IF
          IF (keyin$="" OR keyin$=CHR$(19) OR UCASE$(keyin$)="P") THEN
            hold=1
          ELSE 
            hold=0
          END IF
        END IF
      WEND

    END IF
    Lines%=Lines%+1
    IF (Lines% MOD Page%)=0 THEN
      GOSUB TimeCheck
      Mprint "More (Y/n/=)?"
      in%=5:YES=1:il%=1:GOSUB Modem.in
      IF in$="NO" THEN GOTO Finish
      Mprint killmore$
      IF in$="=" THEN
        Page%=32767
        Mprint cr$+"Continuous Output [Space]=Abort  [P/Ctrl-S]=Pause  [OtherKey]=Resume"+cr$+cr$
        GOSUB Delay1
      END IF
    END IF
  WEND   ' Back for another line of text

Finish:
Matt 0
Page%=FStorePage
CLOSE #9

RETURN


'--------------- SUB PROGRAMS

REM ******************************************************************
SUB DosExe(cmd$) STATIC   'From Percy very very handy
REM ******************************************************************

cmd$=cmd$+CHR$(0)
CALL Execute(SADD(cmd$),0,0)
END SUB


REM ******************************************************************
SUB Matt (attrib%) STATIC
REM ******************************************************************
'All mine - sets an ANSI attribute over modem, and locally for the
'Bold, Italics, Underlined and Reset. No combo's as of yet.

SHARED ring%

IF attrib%>4 THEN EXIT SUB
IF attrib%=2 THEN EXIT SUB

IF ring%=1 THEN
  att$=STR$(attrib%)
  att$=RIGHT$(att$, LEN(att$)-1)
  PRINT #1, CHR$(27);"[";att$;"m";
END IF

IF attrib%=0 THEN  ' reset
  COLOR 7,0
  CALL SetSoftStyle(WINDOW(8),0,255)
END IF
IF attrib%=1 THEN  'Bold
  CALL SetSoftStyle(WINDOW(8),2,255)
END IF
IF attrib%=3 THEN  'italics
  CALL SetSoftStyle(WINDOW(8),4,255)
END IF
IF attrib%=4 THEN  'underlined
  CALL SetSoftStyle(WINDOW(8),1,255)
END IF
END SUB



REM ******************************************************************
SUB MCol (an%) STATIC  'from CJ's door sorta! Well...
REM ******************************************************************
'Usage: MCol fgcolour
'Just to set the main colour on black (or same background as before, anyway)

SHARED ring% , ansi%

IF ring%=1 AND ansi%=1 THEN
  an$=STR$(an%)
  an$=RIGHT$(an$, LEN(an$)-1)
  PRINT #1, CHR$(27);"[";an$;"m";
END IF
IF ansi%=1 THEN COLOR (an%-30)
END SUB


REM ******************************************************************
REM These Subs based on those in Craig Jackson's Wall Door. Thanks!
REM ******************************************************************

SUB Mprint  (out$) STATIC
    REM -- Outputs a string to screen & modem (if remote call)

    SHARED ring%

    IF ring%=1 THEN
      PRINT #1, out$;
    END IF
    PRINT out$;
END SUB

SUB MColour (mf%,mb%) STATIC
    REM -- Usage: MColour foregroundcolor, backgroundcolor
    REM -- Changes ANSI Over The Modem For User & Locally

    SHARED ring% , ansi%

    IF ring%=1 AND ansi%=1 THEN
        mf$=STR$(mf%)
        mb$=STR$(mb%)
        mb$=RIGHT$(mb$, LEN(mb$)-1)
        mf$=RIGHT$(mf$, LEN(mf$)-1)
        PRINT #1, CHR$(27);"[";mf$;";";mb$;"m";
    END IF
    IF ansi%=1 THEN COLOR (mf%-30),(mb%-40)
END SUB

SUB Mlocate (mx%,my%) STATIC
    REM -- Usage: MLocate ypos, xpos
    REM -- Changes ANSI Cursor Position Over Modem & Locally

    SHARED ring%

    IF ring%=1 THEN
        mx$=STR$(mx%)
        my$=STR$(my%)
        mx$=RIGHT$(mx$,LEN(mx$)-1)
        my$=RIGHT$(my$,LEN(my$)-1)
        PRINT #1, CHR$(27);"[";mx$;";";my$;"H";
    END IF
    LOCATE mx%,my%
END SUB

