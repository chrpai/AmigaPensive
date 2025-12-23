
REM ******************************************************************

REM Program       Corral (A metro door)
REM    Date       17 April 1991
REM Version       3.0
REM Recompiled    23 Nov 94 (slight change to not use Metro.config
REM               also removed the CHR$(7) printing which was causing
REM               some problems on some systems using MaxsBeep)
REM
REM  Author       Peter Francis Deane
REM
REM ******************************************************************

ON ERROR GOTO woops
CLEAR
DEFLNG A-Z
cr$=CHR$(13)+CHR$(10)
de$=CHR$(8)+" "+CHR$(8)
esc$=CHR$(27)
null$=CHR$(0)
' bell$=CHR$(7)

TestIt$="NOTONYOURLIFE"

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
  tpc$="30"
  id$="1"
  lev$="4"
  ring$="0"
  bd$="2400"
  uname$="MIKHAIL GORBACHEV"
  path$=""
END IF

'convert to short integers
tpc%=VAL(tpc$)
id%=VAL(id$)
lev%=VAL(lev$)
ring%=VAL(ring$)
Baud=VAL(bd$)
Title$="Corral! (by PD) - User #"+id$+" ["+uname$+"]  Lev:"+lev$
IF ring%=1 THEN
  title$=title$+" Baud: "+bd$
ELSE
  title$=title$+" Baud: Local Login"
END IF

'open modem
IF ring%=1 THEN
  TIMER ON
  OPEN "COM1:"+bd$+",n,8,1" AS #1 LEN=4096
  GOSUB delay1
END IF

'HouseKeeping
timeon=TIMER
RANDOMIZE TIMER
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
REM *******************************************************************
Startup:
REM *******************************************************************

GOSUB ClearScreen
GOSUB TimeCheck

MPrint cr$+"          *********************************"
MPrint cr$+"          **                             **"
MPrint cr$+"          **            Corral           **"
MPrint cr$+"          **                             **"
MPrint cr$+"          *********************************"+cr$+cr$
MPrint cr$+"            By Peter Deane, Inquestor BBS."+cr$
MPrint cr$+"              Newcastle, NSW, Australia."
MPrint cr$+"              Version 3.0  (23 Nov 1994)"+cr$
MPrint cr$+"     Original TRS-80 version by Colin Keay (1979)"+cr$
MPrint cr$+cr$

GOSUB SetAnsi
'Get the Corral Data Setup. This bit makes it easy/hard Pretty Hard now

DIM A(21)
DIM S(2,9)
DATA -1,0,1,1,2,2,1,1,0,-1
DATA 0,1,2,3,4,3,2,1,0,0
FOR I=1 TO 2
  FOR j=0 TO 9
    READ S(I,j)
  NEXT
NEXT

'Load Top 10 Scores
DIM score$(10,3)
flnm$=path$+"Corral.scores"

OPEN flnm$ FOR INPUT AS 2
  FOR j=1 TO 10
    FOR k=1 TO 3
      LINE INPUT #2, score$(j,k)
    NEXT k
  NEXT j
CLOSE #2:

'
' Grab some items from the Metro.config
' flnm$="s:Metro.config"
' OPEN flnm$ FOR INPUT AS #2
'  LINE INPUT #2, BBSName$
'  LINE INPUT #2, garb$
'  LINE INPUT #2, BBSPath$
'  LINE INPUT #2, MAILPath$
'  LINE INPUT #2, UDPath$
'  FOR k=1 TO 19
'    LINE INPUT #2, garb$
'  NEXT
'  LINE INPUT #2, SYSOPName$
' CLOSE #2
'
' now NONE of these are used in this door!  Sysopname was, but
' we now just say 'The Sysop' instead of the personal touch!
'
' BBSPath$=BBSPath$+"BBSFiles/"
' MAILPath$=MAILPath$+"Mailfiles/"
' UDPath$=UDPath$+"UDFiles/"
'

REM *******************************************************************
TOPMENU:
REM *******************************************************************
GOSUB ClearScreen
in$=""
MPrint cr$+cr$+cr$
Matt 1
MCol 32
MPrint cr$+"  *********************************"
MPrint cr$+"  **                             **"
MPrint cr$+"  **            "
MCol 33
MPrint "Corral"
MCol 32
MPrint "           **"
MPrint cr$+"  **                             **"
MPrint cr$+"  *********************************"
MPrint cr$+cr$
Matt 0
MCol 31
Matt 3
MPrint cr$+"Hey, Bucko, Here's whatchy'all can do:"
Matt 0
MPrint cr$+cr$
Mcol 36
MPrint cr$+"<G>et the gen on the thing (Read docs)"
Mcol 35
MPrint cr$+"<R>ide 'em, Cowboy! (Play)"
Mcol 33
MPrint cr$+"<C>ause an internal program error"
MCol 32
MPrint cr$+"<S>ee the Top CowPersons list"
MCol 37
MPrint cr$+"<Q>uit the whole kit 'n' k'boodle right now"
MCol 31
Matt 3
MPrint cr$+cr$+cr$+"Okay, what's it to be "+uname$+"? "
Matt 0
in%=0:il%=1:GOSUB modem.in

IF in$="G" THEN GOSUB Instructions
IF in$="I" THEN GOSUB Instructions
IF in$="R" THEN GOSUB Play
IF in$="C" THEN GOSUB Crash
IF in$="S" THEN GOSUB Scores
IF in$="Q" THEN GOTO Quit
IF in$="P" THEN GOSUB Play

GOTO TOPMENU

REM *******************************************************************
REM The Play Modules are here
REM *******************************************************************

REM *******************************************************************
Instructions:
REM *******************************************************************

GOSUB ClearScreen
Matt 1
MCol 33
MPrint "                  *********************************"
MPrint cr$+"                  **     Corral Instructions     **"
MPrint cr$+"                  *********************************"+cr$
Matt 0
MCol 36
MPrint cr$+"Corral is a very simple, but addictive game where you are the cowboy"
MPrint cr$+"(please excuse the sexism), and you have to catch your horse."
MCol 35
MPrint cr$+cr$+"You are represented by a 'C' and the horse by a 'H'. You can move"
MPrint cr$+"toward your horse from between 1 & 5 steps at a time, but beware!"
MPrint cr$+"Harry (the horse) is quite temperamental. If you move up on him too"
MPrint cr$+"quickly (more than half the separation) he will BOLT. He will also"
MPrint cr$+"tend to bolt if you pin him up against the rails."+cr$
MCol 36
MPrint cr$+"Furthermore, at times, he will lash out and KICK you, thus rendering"
MPrint cr$+"you unconscious for a few moves, while he cavorts away, gleefully."+cr$
MCol 35
MPrint cr$+"You CAN win the game, and if you do, you'll be added to a high score"
MPrint cr$+"table, which you can call up from the game menu (how do you think I"
MPrint cr$+"tested it?)! It's pretty hard, but by no means impossible."
MCol 32
MPrint cr$+cr$+"                        - Peter Deane."
GOSUB AnyKey
RETURN


REM *******************************************************************
Play:
REM *******************************************************************

GOSUB timecheck
GOSUB ClearScreen
Matt 1
MCol 32
MPrint cr$+"      *********************************"
MPrint cr$+"      **                             **"
MPrint cr$+"      **     "
MCol 33
MPrint "Corral Game Board"
MCol 32
MPrint "       **"
MPrint cr$+"      **                             **"
MPrint cr$+"      *********************************"
Matt 0
MPrint cr$+cr$

250 C=1:l=1:k=0:m=0:n=0:GOSUB 640
260 IF R>5 THEN Q=-Q
270 H=13+Q:GOSUB 650
280 T=2+P
290 B$="        "
300 FOR j=1 TO 21:A(j)=32:NEXT
310 A(C)=67:A(H)=72

REM Figure out output string
320 MCol 32
IF LEN(STR$(n))=1 THEN MPrint "  "+STR$(n)+SPACE$(8)
IF LEN(STR$(n))=2 THEN MPrint " "+STR$(n)+SPACE$(8)
IF LEN(STR$(n))>2 THEN MPrint STR$(n)+SPACE$(8)
MCol 31
MPrint "|"
MCol 36
330 FOR j=1 TO 21:MPrint CHR$(A(j)):NEXT j
MCol 31
340 MPrint "|"
MCol 33
MPrint SPACE$(8)+B$
350 X=ABS(H-C):l=SGN(H-C)
360 n=n+1: IF k>0 THEN 590
370 IF n>99 THEN 790

GOSUB timecheck

380 MCol 35
MPrint "Move? "
in%=0:il%=1:GOSUB modem.in
D=VAL(in$)
MPrint cr$
IF in$="Q" THEN 790   'At last! You can Quit!
390 IF D>0 AND D<6 THEN 410
400 MCol 31 : MPrint SPACE$(11)+"| ** Illegal  Move ** |"+SPACE$(16):GOTO 380
410 E=C+l*D:IF E<1 OR E>21 THEN 400
420 C=E:GOSUB 640
430 G=P:H=H+l*G:GOSUB 650
440 IF X<2*D AND D>1 THEN 490
450 IF H>1 AND H<20 THEN 530
460 GOSUB 640
470 IF R>2 THEN 530
480 IF X>7 THEN 290
490 G=9+2*P:H=H-l*G: l=-l: GOSUB 650
500 IF ABS(H-C)>1 THEN 520
510 H=H-3*l: GOSUB 650
520 B$="BOLTED  ":GOTO 300
530 IF ABS(H-C)>2 THEN 290
540 GOSUB 640
550 IF R>3 THEN 620
560 GOSUB 640
570 k=P+2: m=m+1: H=H-5*l: GOSUB 650
580 B$="KICKED  ":GOTO 300
590 IF m>T THEN 680
600 k=k-1: MPrint cr$:GOSUB 640
610 H=H+l*(P+1): GOSUB 650: GOTO 290
620 IF H=C THEN 700
630 GOTO 290
640 R=INT(10*RND): P=S(1,R): Q=S(2,R): RETURN 
650 IF H<1 THEN H=1
660 IF H>21 THEN H=21
670 RETURN
680 MCol 32 : MPrint cr$+cr$+cr$+"Sheesh! Those kicks landed you in hospital!"
690 MPrint cr$+"I hope you Get Well Soon.": GOSUB AnyKey:GOTO GameOver
700 FOR j=1 TO 21:A(j)=32:NEXT:A(C)=35

MCol 32
IF LEN(STR$(n))=1 THEN MPrint "  "+STR$(n)+SPACE$(8)
IF LEN(STR$(n))=2 THEN MPrint " "+STR$(n)+SPACE$(8)
IF LEN(STR$(n))>2 THEN MPrint STR$(n)+SPACE$(8)
MCol 31
MPrint "|"
MCol 36
FOR j=1 TO 21:MPrint CHR$(A(j)):NEXT j
MCol 31
730 MPrint "|"
MCol 33
MPrint SPACE$(8)+B$

740 MCol 32 : MPrint cr$+cr$+cr$+"Yipeee! Got Him! Now let's see if you can do it in fewer moves!"
750 GOSUB AnyKey: GOSUB CheckHi
760 GOTO GameOver
790 MCol 32 : MPrint cr$+cr$+cr$+"Sheesh! You'd be better as the camp cook!"
800 GOSUB AnyKey
810 GOTO GameOver

GameOver:
RETURN

REM *******************************************************************
Crash:
REM *******************************************************************

REM Not sure why I did this, but it's interesting!
REM and was a bit of a muck around with the filehandling...

GOSUB timecheck
GOSUB ClearScreen

FOR k=1 TO 20
  MPrint cr$
  Matt 0
  FOR l=1 TO 75
loop2:
    fred=RND*255:IF fred<32 THEN loop2
    MPrint CHR$(fred)
  NEXT
NEXT
MCol 31
Matt 1
MPrint cr$+cr$+"Oh, no! You shouldn't have done that!"
Matt 0
MCol 36
MPrint cr$+cr$+"Now you've done severe damage to the High Scores file. So be it. Your attempt"
MPrint cr$+"to sabotage the system has been noted, and the Sysop will now probably reduce"
MPrint cr$+"your access time."+cr$+cr$
GOSUB AnyKey

flnm$=path$+"corral.swine"
GOSUB TimeCheck
GOSUB DateCheck
CLOSE #3:OPEN flnm$ FOR APPEND AS 3
PRINT# 3,DATE2$,SPACE$(3),uname$
CLOSE #3

GOSUB timecheck
MCol 35
MPrint cr$+"Would you like to see the list of potential saboteurs? (Y/n)"
in%=3:il%=1:YES=1:GOSUB modem.in
IF in$="NO" THEN GOTO SabOver

GOSUB ClearScreen
Matt 1
MCol 32
MPrint cr$+"      *********************************"
MPrint cr$+"      **                             **"
MPrint cr$+"      **      "
MCol 33
MPrint "Corral Saboteurs!"
MCol 32
MPrint "      **"
MPrint cr$+"      **                             **"
MPrint cr$+"      *********************************"
MPrint cr$+cr$
Matt 0
MCol 35
OPEN flnm$ FOR INPUT AS 3
count=0
WHILE NOT EOF(3)
count=count+1
  LINE INPUT #3, o$
  MPrint cr$+o$
  IF (count MOD 15)=0 THEN 
    GOSUB timecheck
    MCol 37
    MPrint cr$+cr$+"More (Y/n)?"
    in%=5:il%=1:YES=1:GOSUB modem.in
    MPrint de$+de$+de$+de$+de$+de$+de$+de$+de$+de$+de$
    MCol 35
    IF in$="NO" THEN lab2
  END IF
WEND

lab2:
CLOSE #3
SabOver:
GOSUB AnyKey
RETURN


REM *******************************************************************
Scores:
REM *******************************************************************

GOSUB ClearScreen
Matt 1
MCol 32
MPrint "      *********************************"
MPrint cr$+"      **                             **"
MPrint cr$+"      **     "
MCol 33
MPrint "Corral High Scores"
MCol 32
MPrint "      **"
MPrint cr$+"      **                             **"
MPrint cr$+"      *********************************"+cr$
Matt 0
MCol 37
MPrint cr$+"-----------------  -----  -------------------------"
MCol 31
MPrint cr$+"Date               Moves  CowPerson"
MCol 37
MPrint cr$+"-----------------  -----  -------------------------"

FOR k=1 TO 10
  spc1=19-LEN(score$(k,3))
  IF VAL(Score$(k,1))<10 THEN Spc1=Spc1+1
  MCol 35
  MPrint cr$+score$(k,3)+SPACE$(spc1)
  MCol 33
  MPrint score$(k,1)+SPACE$(5)
  MCol 36
  MPrint score$(k,2)
NEXT k

GOSUB AnyKey
RETURN

REM ******************************************************************
Quit:
REM ******************************************************************

Gosub ClearScreen
MCol 35
MPrint "Hope you enjoyed this door!"+cr$
Mcol 32
MPrint cr$+"If you did, then leave a message to the sysop asking him"+cr$
MPrint "to support the shareware concept and register the program."+cr$
Mcol 36
GOSUB Delay1
GOSUB Delay1
Mprint cr$+"The Corral Door was written by Peter Deane"+cr$
MPrint "Inquestor BBS, Newcastle, NSW, Australia"+cr$+cr$
Mcol 31
MPrint "011-61-49-72-1647 from the USA"+cr$
MPrint "(049) 72-1647 from Australia"+cr$
GOSUB Delay1
GOSUB Delay1
GOTO Bye



REM *******************************************************************
REM The Internal (Corral) Subroutines follow.....
REM *******************************************************************

REM *******************************************************************
CheckHi:
REM *******************************************************************

MCol 37
MPrint cr$+"You took "+STR$(n)+" moves"
IF n>=VAL(score$(10,1)) THEN
  MPrint cr$+cr$+"Bad luck. You didn't make the high score list."
  GOSUB AnyKey
  RETURN
END IF
MPrint cr$+cr$+"Hey! Whad'ya know? You're in the Top Ten!"
GOSUB AnyKey

GOSUB DateCheck
score$(10,1)=STR$(n)
score$(10,2)=uname$
score$(10,3)=DATE2$

REM This sort routine pinched from Mark Turner! Thanks, Mark!

FOR ct=10 TO 2 STEP -1
  IF VAL(score$(ct,1))<VAL(score$(ct-1,1)) THEN
    SWAP score$(ct,1),score$(ct-1,1)
    SWAP score$(ct,2),score$(ct-1,2)
    SWAP score$(ct,3),score$(ct-1,3)
  END IF
NEXT ct
GOSUB SaveHi
GOSUB Scores
RETURN

REM *******************************************************************
SaveHi:
REM *******************************************************************

flnm$=path$+"corral.scores"
CLOSE #2
OPEN flnm$ FOR OUTPUT AS 2
FOR j=1 TO 10
  FOR k=1 TO 3
   temp$=score$(j,k)
   PRINT #2, temp$
  NEXT
NEXT
CLOSE #2
RETURN


REM *******************************************************************
AnyKey:
REM *******************************************************************

Matt 3
MCol 31
MPrint cr$+cr$+"Please press the <ANY> key to continue"
in%=0:il%=1:GOSUB modem.in
Matt 0
MPrint cr$+cr$
RETURN


REM ******************************************************************
ClearScreen:
REM ******************************************************************    
                                                  
GOSUB TimeCheck
Matt 0
MPrint cr$+cr$+cr$+cr$+CHR$(12) 
CLS
RETURN


REM ******************************************************************
SetAnsi:
REM ******************************************************************

Matt 0
Mprint cr$+cr$+"Do you want ANSI colours (Y/n)? "
in%=3:il%=1:yes=1:GOSUB modem.in

IF in$="YES" THEN
  ansi%=1
  Mcolour 35,40
  Mprint cr$+cr$+"ANSI now "
  MColour 31,40
  MPrint "ON!"+cr$
ELSE 
  ansi%=0
  Mprint cr$+cr$+"ANSI now OFF!"
END IF
RETURN

 
REM *******************************************************************
Woops:
REM *******************************************************************

MPrint cr$+cr$+"Oh Dear! We have had a fatal error occur."
MPrint cr$+cr$+"The Error Number was:- "+STR$(ERR)
MPrint cr$+"There will be a note made of this for the sysop, but please"
MPrint cr$+"remind him by leaving him a message and let him know"
MPrint cr$+"what you were doing just before the crash."

flnm$=path$+"Door.errors!"
OPEN flnm$ FOR APPEND AS # 65
PRINT #65, ""
PRINT #65, "Fatal Error Occured At ";TIME$;" on ";DATE$
PRINT #65, "Error #";STR$(ERR);" Last Line #";STR$(ERL)
PRINT #65, ""
CLOSE #65
GOSUB AnyKey
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
IF TIMER<timeon+(tpc%*60) THEN
  timenow=TIMER
  ctime=(timenow-timeon)
  ctime=CINT(ctime/60)
  rtime=tpc%-ctime
  RETURN
END IF
Mprint cr$+cr$+"Sorry, Time Limit Exceeded!"
GOTO Bye


REM ******************************************************************
delay1:
REM ******************************************************************
pause&=TIMER
WHILE TIMER<pause&+2
WEND
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



'--------------- SUB PROGRAMS

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


