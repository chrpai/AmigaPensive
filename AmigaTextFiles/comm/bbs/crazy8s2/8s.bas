REM ******************************************************************

REM Program- Crazy 8s
REM    Date- 5 June 1991
REM  Author- Peter Deane
REM
REM Version- 2.0
REM
REM  (Recompiled 23 Nov 94 to run under OzMetro)

REM ******************************************************************

ON ERROR GOTO woops
TIMER ON
ON TIMER (60) GOSUB RtimeDec

' Set things so you can DO things!
CLEAR ,20000&
CLEAR
DEFLNG a-z
DEFINT k
cr$=CHR$(13)+CHR$(10)
de$=CHR$(8)+" "+CHR$(8)
null$=CHR$(0)
esc$=CHR$(27)
DIM Scores$(15,3)

TestIt$="notreallyno"

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
  tpc$="250"
  id$="999"
  lev$="9"
  ring$="0"
  ring%=0
  bd$="2400"
  PRINT "Enter Your Name ";
  in%=2:il%=25:GOSUB Modem.in
  if in$="" THEN in$="NAMELESS SWINE"
  uname$=in$
  path$=""
END IF


'convert to short integers
rtime%=VAL(tpc$)
id%=VAL(id$)
lev%=VAL(lev$)
ring%=VAL(ring$)
Baud=VAL(bd$)
Title$="Crazy 8's (by PD) - User # "+id$+" ["+uname$+"]  Lev: "+lev$
IF ring%=1 THEN
  title$=title$+" Baud: "+bd$
ELSE
  title$=title$+" Baud: Local Login"
END IF


'open modem
IF ring%=1 THEN
  OPEN "COM1:"+bd$+",n,8,1" AS #1 LEN=4096
  GOSUB delay1
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
GOSUB ClearScreen
MPrint cr$+"Welcome to the Crazy 8s Door"
MPrint cr$+cr$+"(Version 2.0  - 23 November 1994)"
MPrint cr$+cr$+cr$+"Original Author Unknown"
MPrint cr$+cr$+"Converted to a Metro Door by Peter Deane"
MPrint cr$+cr$+"Inquestor BBS, NSW, Australia."+cr$
GOSUB TimeCheck


' Grab some items from the Metro.config
' flnm$="s:Metro.config"
' OPEN flnm$ FOR INPUT AS #2
'   LINE INPUT #2, BBSName$
'   LINE INPUT #2, garb$
'   LINE INPUT #2, BBSPath$
'   LINE INPUT #2, MAILPath$
'   LINE INPUT #2, UDPath$
'   FOR k=1 TO 19
'     LINE INPUT #2, garb$
'   NEXT
'   LINE INPUT #2, SYSOPName$
' CLOSE #2
' BBSPath$=BBSPath$+"BBSFiles/"
' MAILPath$=MAILPath$+"Mailfiles/"
' UDPath$=UDPath$+"UDFiles/"


'set config variables

GOSUB SetAnsi 
GOSUB Speakcheck
GOSUB ClearScreen
GOSUB About

REM    Eights       a card game      <- this is about the only orig rem
'      ------        14-Jan-85          stmnt. I left it in for nostalgia!

DIM c(52)
DIM acard$(26)
DIM s$(32)
DIM v(10)
DIM p(10)
DIM s(5)


REM ******************************************************************
TopMenu:
REM ******************************************************************

GOSUB MenuPrint ' Print it up all the time now it's hot-keyed :-)
                ' The '?' is ridiculous - any key other than Q P I A S prints
                ' the menu, anyway! 
GOSUB TimeCheck
Mcol 36
MPrint cr$+Time2$+"  "
MCol 33
MPrint " (Time Left:"+STR$(rtime%)+")"
MCol 31
MPrint "  Options: <P>-<I>-<A>-<S>-<Q> [?=Menu] "
in%=0:il%=1
GOSUB modem.in

CommandBranch:
GOSUB TimeCheck

IF in$="Q" THEN GOTO FinishUp
IF in$="P" THEN GOSUB PlaySection
IF in$="I" THEN GOSUB Instruct
IF in$="A" THEN GOSUB About
IF in$="S" THEN GOSUB Scores

GOTO TopMenu



REM ******************************************************************
PlaySection:
REM ******************************************************************

'Reset v array (ie scores)

FOR p1=1 TO 8
  v(p1)=0 
NEXT p1
hndno=0

GOSUB ClearScreen
MCol 37
MPrint "       There are two sections of this game: Competition & Practice"+cr$
MCol 35
MPrint cr$+"       In the Comp Section, you must play 5 games straight against 3"
MPrint cr$+"       opponents. But your results WILL be eligible to go into the"
MPrint cr$+"       Best Scores Table."+cr$
MCol 33
MPrint cr$+"       The Practice Section allows you to play one game against any"
MPrint cr$+"       number of opponents, but you will NOT be able to make it into"
MPrint cr$+"       the Best Scores Table."+cr$
MCol 31
MPrint cr$+"       WARNING: If you QUIT at any time during the 5 Comp games, you "
MPrint cr$+"       will NOT be eligible to make the Best Scores Table."+cr$

WhichGame:
MCol 37
MPrint cr$+cr$+"                <"
MCol 31
MPrint "P"
MCol 37
MPrint ">"
MCol 32
MPrint "ractice Section"
MCol 37
MPrint "    <"
MCol 31
MPrint "C"
MCol 37
MPrint ">"
MCol 32
MPrint "ompetition Section "+cr$
in%=0:il%=1:GOSUB Modem.in
IF in$<>"P" AND in$<>"C" THEN WhichGame
GOSUB ClearScreen

IF in$="P" THEN
  CompGame=0
  MPrint "You have chosen to use the Practice Section"+cr$
  NoPlayersCheck:
  MCol 31
  MPrint cr$+cr$+cr$+"Including you, how many players (2, 3 or 4) ? "
  in%=0:il%=1:GOSUB modem.in
  MPrint in$
  k=VAL(in$)
  IF k<2 OR k>4 THEN NoPlayersCheck:
  N=k
  GOSUB Main
  RETURN
ELSE
  CompGame=1
  MPrint "Starting the Competition Match. Good Luck!"+cr$+cr$
  GOSUB Delay1

  N=4             'This is the # players .. see!  (not my code)

  GOSUB Main
  IF QuitGameFlag=1 THEN    ' This Quit Game flag was needed - sorry.
                            ' this is for the best scores table - 
                            ' if they quit a game, they don't make it....
    QuitGameFlag=0
    RETURN                  ' <=---- See..
  END IF
END IF

GamesLoop:

IF hndno<5 THEN
  GOSUB TimeCheck
  MCol 33
  MPrint cr$+cr$+"You have"+STR$(rtime%)+" minutes remaining."
  MCol 35
  Matt 3
  MPrint cr$+cr$

  'Smart-Alec Comments Section
  ' ... Working Storage Division....  
  ' 01  INDEX   PIC 99.
  ' 
  ' hangon, this is BASIC, you fool!
  '
  Index=(6-HndNo)*v(1)

  IF Index<10              THEN MPrint "So, you think you're doing pretty well, eh, Bucko?"
  IF Index<20 AND Index>9  THEN MPrint "Don't be so cocky, you still haven't played 5 games!"
  IF Index<30 AND Index>19 THEN Mprint "The computer will always beat inferior humans. Beware!"
  IF Index<40 AND Index>29 THEN MPrint "Average play from an average player. Zzzzzzzz!"
  IF Index<50 AND Index>39 THEN MPrint "I suggest you read the instructions again!"
  IF Index<60 AND Index>49 THEN MPrint "Say, would you like to borrow my spectacles?"
  IF Index<70 AND Index>59 THEN MPrint "You don't stand an iceberg's chance in Hell of doing much good!"
  IF Index >69             THEN MPrint "Huh! My grandmother plays better than you, and she can't see!"

  Matt 0
  MCol 36
  MPrint cr$+cr$+"Continue Competition Games (Y/n)? "
  MCol 37
  in%=3:il%=1:YES=1:GOSUB Modem.in
  IF in$="NO" THEN
    MCol 36
    MPrint cr$+cr$+"Okay, but you aren't eligible to go into the"
    MPrint cr$+"Best Scores Table for that woeful effort!"+cr$
    GOSUB AnyKey
    RETURN
  END IF
  GOSUB Main
  IF QuitGameFlag=1 THEN
    QuitGameFlag=0
    RETURN
  END IF
  GOTO GamesLoop
END IF

GOSUB TimeCheck
GOSUB DateCheck

MCol 33
MPrint cr$+cr$+cr$+"Wow, so you've played the five hands. Let's see if you made the"
MPrint cr$+"Best Scores Table..."
GOSUB Delay1
GOSUB Delay1

flnm$=path$+"8s.scores"
OPEN flnm$ FOR INPUT AS #2
FOR K=1 TO 15
  FOR L=1 TO 3
    LINE INPUT #2, Scores$(k,l)
  NEXT
NEXT
CLOSE #2

IF v(1) < VAL(Scores$(15,2)) THEN
  MCol 36

  IF v(1)=0 THEN
    MPrint cr$+cr$+"WOW! A Perfect Score!!!! Well Done. Writing the new file..."
  ELSE
    MPrint cr$+cr$+"WOW! You DID make it! Better save out the new file..."
  END IF  
  GOSUB Delay1

  Scores$(15,1)=Uname$
  Scores$(15,2)=STR$(v(1))
  Scores$(15,3)=Date2$

  FOR k=15 to 2 STEP -1
    IF VAL(scores$(k,2))<VAL(scores$(k-1,2)) THEN
      SWAP scores$(k,1),scores$(k-1,1)
      SWAP scores$(k,2),scores$(k-1,2)
      SWAP scores$(k,3),scores$(k-1,3)
    END IF
  NEXT k

  flnm$=path$+"8s.scores"
  OPEN flnm$ FOR OUTPUT AS #2
  FOR K=1 TO 15
    FOR L=1 TO 3
      PRINT #2, Scores$(K,L)
    NEXT
  NEXT
  CLOSE #2

  IconKill$=flnm$+".info"
  KILL IconKill$
  GOSUB Scores

ELSE
  MCol 36
  MPrint cr$+cr$+"Nah, sorry, you haven't made it. Maybe next time!"
  GOSUB AnyKey
END IF

RETURN


REM ******************************************************************
main:
REM ******************************************************************

LastCard$=" Nothing at all!"
OldLastCard$=LastCard$

a=0
f7=0
hndno=hndno+1
GOSUB ClearScreen
R=52 
Mcol 32
MPrint cr$+"  Shuffling..."
 
D$="SHCD"
acard$=" A 2 3 4 5 6 7 8 910 J Q K"
s$="Spades  Hearts  Clubs   Diamonds"
D=1
 
FOR i=1 TO 52 
  c(i)=0
NEXT i
MPrint "..."

FOR p1=1 TO 8
  p(p1)=6
NEXT p1
MPrint "..."
   
FOR l0=1 TO N
  MPrint "..."
  FOR l1=1 TO 6
    GOSUB deal
    c((y-1)*4+x)=l0
  NEXT l1
NEXT l0

MPrint cr$
GOSUB showhand
GOSUB deal

IF y=1 THEN a=1
IF y=7 THEN f7=1
MCol 35
MPrint cr$+"Start --->"
spk$="start .":GOSUB speak

GOSUB PrintCard
spk$=MID$(acard$,y*2-1,2)+" of "+MID$(s$,x*8-7,8):GOSUB speak
LastCard$=CardToPrint$

610
s1=x 
s2=y

640
D=D+1
q1=0
q2=0
IF D > N THEN D=D-N
IF s2<> 1 OR a=0 THEN GOTO 730
MCol 33
MPrint cr$+"Hand #"+STR$(D)+":"
MCol 32
MPrint "   skipped."
spk$="hand "+STR$(D)+" skipped.":GOSUB speak
a=0
GOTO 640

730
IF D=1 THEN GOTO 1270
x=1 

750
y=1

760
IF c((y-1)*4+x) <> D THEN GOTO 880
IF s2<>7 THEN GOTO 830
IF y=7 THEN GOTO 810
IF f7=0 THEN GOTO 830
GOTO 880

810
f7=f7+1
GOTO 960

830
IF x=s1 OR y=s2 THEN GOTO 930
IF y<> 8 THEN GOTO 880
q1=x
q2=y

880
y=y+1
IF y<=13 THEN GOTO 760
x=x+1
IF x<=4 THEN GOTO 750
x=q1
y=q2
GOTO 970

930
IF y=7 THEN f7=1

960
q1=0
q2=0

970
MCol 33
MPrint cr$+"Hand #"+STR$(D)+": "
IF x<>0 THEN GOTO 1040
MCol 31
MPrint "   Draws "
GOSUB draw
IF m>9 THEN GOTO 1020
MPrint STR$(m)+" "
spk$="hand "+STR$(D)+" draws "+STR$(m)+".":GOSUB speak
GOTO 640

1020
MPrint STR$(m)
GOTO 640

1040
spk$="hand "+STR$(D) : GOSUB speak
OldLastCard$=LastCard$
GOSUB PrintCard
LastCard$=CardToPrint$
spk$=MID$(acard$,y*2-1,2)+" of "+MID$(s$,x*8-7,8):GOSUB speak

1050
c((y-1)*4+x)=-1
IF q1<>0 AND D<>1 THEN GOTO 1080
IF y<>8 OR D=1 THEN GOTO 1210

1080
FOR i=1 TO 5
  s(i)=0
NEXT i
FOR x1=1 TO 4
  FOR y1=1 TO 13
    IF c((y1-1)*4+x1)=D THEN s(x1)=s(x1)+1
  NEXT y1
NEXT x1

FOR x1=1 TO 4
  IF s(x1) <= s(5) THEN GOTO 1190
  s(5)=s(x1)
  x=x1

1190
NEXT x1

MCol 31
MPrint cr$+"    Call:    "+MID$(s$,x*8-7,8)
LastCard$=LastCard$+" Called: "+MID$(s$,x*8-7,8)
spk$="call " : GOSUB speak
spk$="      "+MID$(s$,x*8-7,8):GOSUB speak

1210
p(D)=p(D)-1
IF p(D)=0 THEN GOTO 2380
IF y<> 1 THEN GOTO 610
a=1
GOTO 610

1270
REM input

GOSUB TimeCheck
MCol 31
MPrint cr$
MPrint cr$+"+-----------------------------+"
MPrint cr$+"| Tap a key to play your turn |"
MPrint cr$+"+-----------------------------+"+cr$
in%=0:il%=1:GOSUB modem.in
GOSUB ClearScreen
GOSUB showhand
MCol 35
MPrint cr$+"Your play:"
spk$="your play":GOSUB speak

1280
GOSUB getinp

IF k=ASC("Z") THEN 
  MCol 33
  MPrint cr$+"This is hand #"+STR$(hndno)+"."
  MCol 31
  MPrint cr$+"Are you really sure you want to quit (y/N)? "
  MCol 37
  in%=3:il%=1:YES=0:GOSUB Modem.in
  IF in$="YES" THEN
    QuitGameFlag=1
    IF CompGame=1 THEN
      MCol 36
      MPrint cr$+cr$+"Okay, but you aren't eligible to go into the"
      MPrint cr$+"Best Scores Table for that woeful effort!"+cr$
    END IF 
    GOTO QuitFromGame
  END IF
  GOTO 1270
END IF

IF k<> ASC("D") THEN GOTO 1360
MCol 31
MPrint "   Draws "
GOSUB draw
MPrint STR$(m)
IF m<10 THEN MPrint " "
spk$="you draw "+STR$(m):GOSUB speak
GOTO 640

1360
IF k<> ASC("1") THEN GOTO 1400
MCol 31
MPrint cr$+"Enter '0' ONLY for a Ten."
MCol 35
MPrint cr$+"Try Again:"
GOTO 1280

1400
l0=0

1410
l0=l0+1
IF k=ASC(MID$(acard$,l0*2,1)) THEN GOTO 1670
IF l0<13 THEN GOTO 1410

1435
MCol 31
MPrint cr$+"Enter Card Number or Z to Quit."
MCol 32
MPrint cr$+"--Huh?-- :"
GOTO 1280

1670
y=l0 
MCol 36
MPrint MID$(acard$,y*2-1,2)+" "

OldLastCard$=MID$(acard$,y*2-1,2)+" "
GOSUB getinp
l0=0

1690
l0=l0+1
IF k= ASC(MID$(D$,l0,1)) THEN GOTO 1730
IF l0<4 THEN GOTO 1690
GOTO 1435

1730
x=l0
MCol 36
MPrint MID$(s$,x*8-7,8)
OldLastCard$=OldLastCard$+MID$(s$,x*8-7,8)
spk$=MID$(acard$,y*2-1,2)+" of "+MID$(s$,x*8-7,8) :GOSUB speak
IF c((y-1)*4+x)=1 THEN GOTO 1790
MCol 33
MPrint cr$+cr$+"Play your own cards!!"+cr$
spk$="that card is not in your hand." : GOSUB speak
GOTO 1270

1790
IF x=s1 OR y=s2 THEN GOTO 1840
IF y=8 THEN GOTO 1840
Mcol 33
MPrint cr$+cr$+"Follow suit or rank!!"+cr$
spk$="follow sute or number" : GOSUB speak
GOTO 1270

1840
IF s2=7 THEN GOTO 1980
IF y=7 THEN GOTO 1990

1880
LastCard$=OldLastCard$
IF y<>8 THEN GOTO 1050
c((y-1)*4+x)=-1
MCol 31
MPrint cr$+"     Call:   "
spk$="call ": GOSUB speak

1910
GOSUB getinp
l0=0

1920
l0=l0+1
IF k=ASC(MID$(D$,l0,1)) THEN GOTO 1960
IF l0<4 THEN GOTO 1920
GOTO 1910

1960
x=l0
MCol 36
MPrint MID$(s$,x*8-7,8)
LastCard$=OldLastCard$+" Called: "+MID$(s$,x*8-7,8)
spk$="      "+ MID$(s$,x*8-7,8) : GOSUB speak
GOTO 1210

1980
IF y<>7 THEN 2010

1990
f7=f7+1
LastCard$=OldLastCard$
GOTO 1050

2010
IF f7=0 THEN GOTO 1880
MCol 33
MPrint cr$+cr$+"Play a '7' or Draw!!!!"+cr$
spk$="Play a 7 or draw." : GOSUB speak
GOTO 1270


REM ******************************************************************
PrintCard:
REM ******************************************************************

MCol 36
CardToPrint$=MID$(acard$,y*2-1,2)+" "+MID$(s$,x*8-7,8)
MPrint CardToPrint$
RETURN
                         

REM ******************************************************************
Draw:
REM ******************************************************************

m=f7*3
IF m<1 THEN m=1
m1=m
FOR t=1 TO m1
  GOSUB deal
  IF m<> m1 THEN GOTO 2160
  c((y-1)*4+x)=D
2160
NEXT t
p(D)=p(D)+m
f7=0
RETURN
      
REM ******************************************************************
Deal:
REM ******************************************************************

IF R<>0 THEN GOTO 2320
FOR x1=1 TO 52
  IF c(x1)<>-1 THEN GOTO 2280
  c(x1)=0
  R=R+1
2280
NEXT x1

c((s2-1)*4+s1)=-1
R=R-1
IF R>0 THEN GOTO 2320
m=m-1
RETURN
   
2320
x= INT(RND*4+1)
z= INT(RND*4)
y= INT(RND*13+1)
IF c((y-1)*4+x)<>0 THEN GOTO 2320
c((y-1)*4+x)=-1
R=R-1
RETURN


REM ****************************************************************** 
2380
REM ****************************************************************** 

MCol 33
MPrint cr$+cr$+"A Winner .................................................. "+cr$
spk$="a winner:":GOSUB speak
MCol 35
MPrint cr$+"***************************"
MPrint cr$+"    Hand "+STR$(D)+" Wins!"
MPrint cr$+"***************************"+cr$+cr$
GOSUB AnyKey
GOSUB ClearScreen

FOR z= 1 TO N
  R=0
  IF z=1 THEN GOTO 2450
  MCol 31
  MPrint cr$+cr$+"Hand #"+STR$(z)+": "+cr$
  MCol 35
  MPrint "-----------"
  GOTO 2460

2450
  MCol 31
  MPrint cr$+cr$+"Your Hand: "+cr$
  MCol 35
  MPrint "-----------"

2460
  FOR x=1 TO 4
    FOR y=1 TO 13
      IF c((y-1)*4+x) <> z THEN GOTO 2570
      MPrint cr$
      GOSUB PrintCard
      R=R+1
      IF y=1 THEN R=R+2
      IF y=7 THEN R=R+9
      IF y=8 THEN R=R+4
2570
    NEXT y
  NEXT x

  Mcol 35
  MPrint cr$+"-----------"+cr$
  
  IF R<>1 THEN GOTO 2595
  MCol 33
  MPrint "  1 Point   "+cr$+cr$
  GOTO 2605

2595
  IF R<10 THEN MPrint " "
  MCol 33
  MPrint STR$(R)+" Points  "+cr$+cr$

2605
  v(z)=v(z)+R
  GOSUB AnyKey
NEXT z

GOSUB ClearScreen
  
IF hndno=1 THEN GOTO 2680
MCol 32
MPrint cr$+cr$+"After"+STR$(hndno)+" hands..."
GOTO 2689

2680
MCol 32
MPrint cr$+cr$+"After"+STR$(hndno)+" hand..."

2689
MCol 35
MPrint cr$+"----------------"
FOR z=1 TO N
  IF z>1 THEN GOTO 2740
  MCol 31
  MPrint cr$+"Your hand:"
  GOTO 2745

2740
  MCol 31
  MPrint cr$+"Hand #"+STR$(z)+": "

2745
  IF v(z)<100 THEN MPrint " "
  IF v(z)<10 THEN MPrint " "
  MCol 36
  MPrint STR$(v(z))
NEXT z
MPrint cr$+cr$

QuitFromGame:
GOSUB AnyKey
RETURN  'Back to the PlaySection


REM ****************************************************************** 
Showhand:
REM ******************************************************************

MCol 37
MPrint cr$+"Hand:    #Cards"
MCol 31
MPrint cr$+"---------------"

FOR x=1 TO N 
  IF x=1 THEN
    MCol 32
    MPrint cr$+" You    : "
  ELSE
    MCol 32
    MPrint cr$+"Hand "+STR$(x)+" : "
  END IF

  IF p(x)<10 THEN MPrint " "
  MCol 36
  MPrint STR$(p(x))
NEXT x

MPrint cr$
MCol 37
MPrint cr$+" Your Hand "
MCol 31
MPrint cr$+"-----------"

FOR x=1 TO 4
  FOR y=1 TO 13
    IF c((y-1)*4+x)=1 THEN 
      MPrint cr$
      GOSUB PrintCard 
    END IF 
  NEXT y 
NEXT x 

MCol 35
MPrint cr$+cr$+"Last Card:"
MCol 33
MPrint LastCard$

4080
RETURN
                                          

REM ******************************************************************
Getinp:
REM ******************************************************************

GOSUB TimeCheck
in%=0:il%=1
GOSUB modem.in
k=ASC(in$)
RETURN
   

REM ******************************************************************
Scores:
REM ******************************************************************

GOSUB ClearScreen
flnm$=path$+"8s.scores"
OPEN flnm$ FOR INPUT AS #2
FOR K=1 TO 15
  FOR L=1 TO 3
    LINE INPUT #2, Scores$(k,l)
  NEXT
NEXT
CLOSE #2

MCol 35
MPrint "   ษออออออออออออออออออออออออออออออออออออออออป"+cr$
MCol 35
MPrint "   บ"
MCol 37
MPrint "         Crazy 8s Best Scores           "
MCol 35
MPrint "บ"+cr$
MPrint "   ศออออออออออออออออออออออออออออออออออออออออผ"+cr$+cr$

FOR K=1 TO 15
  MCol 36
  Kay$=RIGHT$(STR$(k),LEN(STR$(k))-1)
  Kay$="("+Kay$+")"
  IF k<10 THEN Kay$=" "+Kay$
  MPrint Kay$+"  "
  MCol 32
  MPrint Scores$(k,1)+SPACE$(26-LEN(Scores$(k,1)))
  MCol 33
  MPrint Scores$(k,2)+SPACE$(8-LEN(Scores$(k,2)))
  MCol 31
  MPrint Scores$(k,3)+cr$
NEXT

GOSUB AnyKey
RETURN


REM ******************************************************************
speak:
REM ******************************************************************

IF ring%=0 AND TalkToMe=1 THEN

  IF MID$(spk$,7,1)="H" THEN MID$(spk$,7)="harts   "
  IF MID$(spk$,7,1)="D" THEN MID$(spk$,7)="di-monds"
  temp$=spk$
  IF MID$(spk$,1,1)=" " AND  MID$(spk$,2,1)="A" THEN spk$="ace of "+RIGHT$(temp$,8)
  IF MID$(spk$,2,1)="J" THEN spk$="jack of "+ RIGHT$(temp$,8)
  IF MID$(spk$,2,1)="K" THEN spk$="king of "+ RIGHT$(temp$,8)
  IF MID$(spk$,2,1)="Q" THEN spk$="queen of "+ RIGHT$(temp$,8)
  IF LEFT$(spk$,1)="1" THEN spk$="ten of "+RIGHT$(temp$,8)
  SAY TRANSLATE$(spk$)

END IF
RETURN




REM ****************************************************************** 
MainTitle:
REM ****************************************************************** 

' This bit just prints out the pretty Crazy 8s letters...

MCol 35
MPrint "     ษออออออออป                                               "
MCol 37
MPrint "ษออออออออป"+cr$

MCol 35
MPrint "     บ        บ     "
MCol 32
MPrint "ษออออ                                     "
MCol 37
MPrint "บ        บ"+cr$

MCol 35
MPrint "     บ             "
MCol 32
MPrint "หผ                         "
MCol 36
MPrint "บ     บ        "
MCol 37
MPrint " บ        บ "+cr$

MCol 35
MPrint "     บ             "
MCol 32
MPrint "บ       "
MCol 31
MPrint " อออป             "
MCol 36
MPrint " บ     บ         "
MCol 37
MPrint "ศป      ษผ"+cr$

MCol 35
MPrint "     บ             "
MCol 32
MPrint "บ           "
MCol 31
MPrint "บ             "
MCol 36
MPrint " บ     บ          "
MCol 37
MPrint "ฬออออออน"+cr$
MCol 35
MPrint "     บ             "
MCol 32 
MPrint "บ       "
MCol 31
MPrint "ษอออน             "
MCol 36
MPrint " ศอออออน         "
MCol 37
MPrint "ษผ      ศป"+cr$

MCol 35
MPrint "     บ                     "
MCol 31
MPrint "บ   บ   "
MCol 33
MPrint " อออออป          "
MCol 36
MPrint "บ         "
MCol 37
MPrint "บ        บ  "
MCol 34
MPrint "ษออ"+cr$

MCol 35
MPrint "     บ        บ            "
MCol 31
MPrint "ศอออส        "
MCol 33
MPrint "ษผ          "
MCol 36
MPrint "บ         "
MCol 37
MPrint "บ        บ  "
MCol 34
MPrint "ศอป"+cr$

MCol 35
MPrint "     ศออออออออผ                      "
MCol 33
MPrint "ษออผ     "
Mcol 36
MPrint " อออออผ         "
MCol 37
MPrint "ศออออออออผ  "
MCol 34
MPrint "ออผ"+cr$
MCol 33
MPrint "                                    ษผ"+cr$
MPrint "                                    ศอออออ"+cr$+cr$

RETURN


REM ****************************************************************** 
Instruct:
REM ****************************************************************** 

GOSUB InstructHeader
MCol 35
MPrint cr$+"  If you have played the commercial game 'UNO' before, you will have some"
MPrint cr$+"  idea of how this game works. It is played with a standard 52-card deck, and"
MPrint cr$+"  players take it in turns to lead a card, with the object of getting rid of"
MPrint cr$+"  all their cards."+cr$
MCol 36
MPrint cr$+"  A lead is based on the last card played. You must follow either the suit"
MPrint cr$+"  of the last card or the number. (EG if the last card was the 3 Diamonds, a"
MPrint cr$+"  a legal play is any Diamond or any 3)."+cr$
MCol 33
MPrint cr$+"  There are several special cards, however. If a 7 is played, the next player"
MPrint cr$+"  MUST play another 7, or draw 3 cards. (If the next player CAN play a 7,"
MPrint cr$+"  then the next player after that must play a 7 or draw 6 cards - the amount"
MPrint cr$+"  of cards to be drawn is cumulative)."+cr$
MCol 31
MPrint cr$+"  An 8 is 'CRAZY' and can be played AT ANY TIME (except where a 7 is needed)"
MPrint cr$+"  Playing an 8 also allows you to call the suit that the next lead will be"
MPrint cr$+"  played in. You can make the suit anything you want."+cr$
GOSUB AnyKey
GOSUB InstructHeader
MCol 35
MPrint cr$+"  If an Ace is played, the next player's turn will be skipped."+cr$
MCol 36
MPrint cr$+"  If you don't have a legal play, you must draw from the deck. To do this,"
MPrint cr$+"  press 'D' for Draw."+cr$
MCol 33
MPrint cr$+"  Enter your moves by pressing the NUMBER of the card you wish to play,"
MPrint cr$+"  followed by the first letter of the suit. To enter a 10, use the '0' key"
MPrint cr$+"  ONLY. To enter an Ace, Jack, Queen or King, enter A, J, Q or K."+cr$
MCol 31
MPrint cr$+"  All keypresses are hot-keyed, so there is no need to press <return> after"
MPrint cr$+"  entering the figures. If you wish to change your mind halfway through"
MPrint cr$+"  entering a card, input a nonsensical value for the suit (eg 'L' or 'X')."+cr$
Mcol 35
MPrint cr$+"  You can quit the game by entering a 'Z' as the number of the card to play."+cr$
GOSUB AnyKey
GOSUB InstructHeader
MCol 36
MPrint cr$+"  There are now two types of gameplay possible: Competition and Practice."
MPrint cr$+"  In the Competition section, you must play 5 complete hands against 3"
MPrint cr$+"  opponents, and your scores are eligible for the Best Scores Table. If you"
MPrint cr$+"  quit before completing either the hand, or the 5 hands, you will NOT be"
MPrint cr$+"  eligible for the Best Scores table at all."+cr$
MCol 33
MPrint cr$+"  In the Practice section, you can play between 1 and 3 opponents, but your"
MPrint cr$+"  scores will not count anywhere except between you and the computer!"+cr$
MCol 31
MPrint cr$+"  At the end of each round, your remaining cards are totalled and you score"
MPrint cr$+"  penalty points for them. A 7 is worth 10 points, an 8 worth 5, an Ace is"
MPrint cr$+"  worth 3, and all the others are worth 1 point each."+cr$
MCol 35
MPrint cr$+"  Naturally, the LOWER your score, the better you are going to do on the"
MPrint cr$+"  Best Scores Table."+cr$
GOSUB AnyKey
RETURN

REM ****************************************************************** 
InstructHeader:
REM ****************************************************************** 

GOSUB ClearScreen
MCol 32
MPrint "  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป"+cr$
MCol 32
MPrint "  บ   "
MCol 31
MPrint "C  "
MCol 32
MPrint "r  "
MCol 31
MPrint "a  "
MCol 34
MPrint "z  "
MCol 35
MPrint "y      "
MCol 37
MPrint "8  "
MCol 36
MPrint "s      "
MCol 31
MPrint "I  "
MCol 32
MPrint "n  "
MCol 33
MPrint "s  "
MCol 34
MPrint "t  "
MCol 35
MPrint "r  "
MCol 36
MPrint "u  "
MCol 37
MPrint "c  "
MCol 31
MPrint "t  "
MCol 32
MPrint "i  "
MCol 33
MPrint "o  "
MCol 34
MPrint "n  "
MCol 35
MPrint "s   "
MCol 32
MPrint " บ"+cr$
MCol 32
MPrint "  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ "+cr$
RETURN


REM ****************************************************************** 
MenuPrint:
REM ****************************************************************** 

GOSUB ClearScreen

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     บ               "
MCol 31
MPrint "C  "
MCol 33
MPrint "r  "
MCol 32
MPrint "a  "
MCol 35
MPrint "z  "
MCol 36
MPrint "y    "    
MCol 37
MPrint "8  "
MCol 31
MPrint "s     "
MCol 33
MPrint "M  "
MCol 32
MPrint "E  "
MCol 35
MPrint "N  "
MCol 36
MPrint "U               "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 32
MPrint "     "
MCol 34
MPrint "วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     "
MPrint "บ                                                                  บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 32
MPrint "     "
MCol 34
MPrint "บ      "
MCol 31
MPrint "<"
MCol 32
MPrint "P"
MCol 31
MPrint ">"
MCol 32
MPrint "lay a Game   "
MCol 35
MPrint "Play either a Competition or Practice Game. "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     "
MPrint "บ      "
MCol 31
MPrint "<"
MCol 32
MPrint "A"
MCol 31
MPrint ">"
MCol 32
MPrint "bout         "
MCol 35
MPrint "In true Amiga form, prints useless info.    "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     บ      "
MCol 31
MPrint "<"
MCol 32
MPrint "I"
MCol 31
MPrint ">"
MCol 32
MPrint "nstructions  "
MCol 35
MPrint "How to play the game (well, mostly).        "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     บ      "
MCol 31
MPrint "<"
MCol 32
MPrint "S"
MCol 31
MPrint ">"
MCol 32
MPrint "cores        "
MCol 35
MPrint "Best scorers - 5 games against 3 opponents. "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     บ      "
MCol 31
MPrint "<"
MCol 32
MPrint "Q"
MCol 31
MPrint ">"
MCol 32
MPrint "uit          "
MCol 35
MPrint "Okay, Okay, I can take a hint.              "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     บ      "
MCol 31
MPrint "<"
MCol 32
MPrint "?"
MCol 31
MPrint ">"
MCol 32
MPrint "=Menu        "
MCol 35
MPrint "Print this very screen up.                  "
MCol 34
MPrint "บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     บ                                                                  บ "
MCol 32
MPrint "ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 34
MPrint "     ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ"
MCol 32
MPrint " ฒฒ"+cr$

GOSUB CheckHotKey
IF GotOne=1 THEN GOTO MenuPrintBack

MCol 32
MPrint "       ฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒฒ"+cr$+cr$

MenuPrintBack:

IF GotOne=0 THEN RETURN ELSE RETURN CommandBranch



REM ****************************************************************** 
About:
REM ****************************************************************** 

GOSUB ClearScreen
GOSUB MainTitle

MPrint cr$
Mcol 33
MPrint "           Original AmigaBASIC Author Unknown"+cr$+cr$
MPrint "                  "
Mcol 31
MPrint "Converted to a Metro Door by "
Mcol 35
MPrint "Peter Deane"+cr$+cr$
MPrint "                            "
Mcol 32
MPrint "Inquestor BBS, NSW, "
Mcol 34
MPrint "Australia "
Mcol 33
MPrint "(011-61-49-72-1647)"+cr$+cr$

GOSUB AnyKey
GOSUB ClearScreen
RETURN


REM ******************************************************************
Speakcheck:
REM ******************************************************************

IF ring%=0 THEN
  GOSUB ClearScreen
  MCol 34
  MPrint cr$+cr$+"(Available on the local console only)"
  MCol 35
  MPrint cr$+cr$+"Do you want me to talk to you (y/N)? "
  in%=3:il%=1:yes=0
  GOSUB modem.in
  TalkToMe=0
  IF in$="YES" THEN TalkToMe=1
END IF

 'forget it if a remote call...

RETURN 


REM ******************************************************************
Woops:
REM ******************************************************************

Matt 0

IF ERR<>99 THEN
 Mprint cr$+cr$+"Oh Dear! We have had a fatal error occur."
 Mprint cr$+cr$+"The Error Number was:- "+STR$(ERR)
 IF ERR=53 THEN
   MPrint cr$+cr$+"This was a File-Not-Found error. This file SHOULD have been"
   MPrint cr$+"available to the program, and was '"+flnm$+"'"
 END IF 
 Mprint cr$+"There will be a note made of this for the sysop, but please"
 Mprint cr$+"remind him by leaving him a message and let him know"
 Mprint cr$+"what you were doing just before the crash."
 GOSUB AnyKey
END IF

errflnm$=path$+"Crazy8s.errors!"
OPEN errflnm$ FOR APPEND AS # 65
PRINT #65, ""
PRINT #65, "Fatal Error Occured At ";TIME$;" on ";DATE$
PRINT #65, "Error #";STR$(ERR);" Last Line #";STR$(ERL)
PRINT #65, ""
IF ERR=99 THEN
  PRINT #65, UName$; " dropped carrier."
  PRINT #65, ""
END IF
IF ERR=53 THEN
  PRINT #65, "File Not Found. Looking for: ";flnm$
  PRINT #65, ""
END IF
CLOSE #65
GOTO Bye


REM ******************************************************************
FinishUp:
REM ******************************************************************

GOSUB ClearScreen

MCol 31
MPrint "                   G  O  O  D  B  Y  E         F  R  O  M :"+cr$+cr$

GOSUB MainTitle

Mcol 33
MPrint cr$+"If you like this door, then leave a message to your Sysop asking"+cr$
MPrint "him to support the shareware concept and REGISTER the program soon."+cr$
GOSUB Delay1
GOSUB Delay1
Mcol 36
Mprint cr$+"The Crazy 8s Door was written by Peter Deane"+cr$
MPrint "Inquestor BBS, Newcastle, NSW, Australia"+cr$+cr$
Mcol 31
MPrint "011-61-49-72-1647 from the USA   (049) 72-1647 from Australia"+cr$
GOSUB Delay1
GOSUB Delay1
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
  Mcolour 35,40
  Mprint cr$+cr$+"ANSI now "
  MColour 31,40
  MPrint "ON!"+cr$
ELSE 
  ansi%=0
  Mprint cr$+cr$+"ANSI now OFF!"
END IF
RETURN


'--------------- MISC USEFUL FUNCTIONS

REM ******************************************************************
AnyKey:
REM ******************************************************************

Matt 0
Mprint cr$+"Tap Any Key To Continue..."
GOSUB TimeCheck
in%=0:il%=1
GOSUB Modem.in
RETURN


REM ******************************************************************
ClearScreen:
REM ******************************************************************    
                                                  
GOSUB TimeCheck
 Mprint cr$+cr$+CHR$(12)
 'Just in case the chr$(12) doesn't work, a few crs
 CLS
 Matt 0
RETURN


'--------------- The Metro Clock!


REM ******************************************************************
TimeCheck:
REM ******************************************************************

time1$=TIME$
t.t=VAL(LEFT$(time1$,2))

IF t.t>11 THEN a$=" PM"
IF t.t<12 THEN a$=" AM"
IF t.t=0 THEN
  t.t=12
  time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6)
ELSEIF t.t<10 THEN
  time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6)
ELSEIF t.t>21 THEN
  t.t=t.t-12
  time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6)
ELSEIF t.t>12 THEN 
  t.t=t.t-12
  time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6)
END IF
Time2$=time1$+a$

IF Rtime% >0 THEN 
  RETURN
ELSE
  Mprint cr$+cr$+"Sorry, Time Limit Exceeded!"
  GOTO Bye
END IF

REM ******************************************************************
delay1:
REM ******************************************************************
pause&=TIMER
WHILE TIMER<pause&+1
WEND
RETURN


REM ******************************************************************
DateCheck:
REM ******************************************************************

'Should call this with GOSUB TimeCheck, GOSUB DateCheck
'Then the strings Time2$ and Date2$ will be the ones to use in the prog

temp$=DATE$
DateTemp1$=MID$(temp$,4,2)
DateTemp3$=RIGHT$(temp$,2)
month%=VAL(LEFT$(temp$,2))
IF month%=1 THEN month$="Jan"
IF month%=2 THEN month$="Feb"
IF month%=3 THEN month$="Mar"
IF month%=4 THEN month$="Apr"
IF month%=5 THEN month$="May"
IF month%=6 THEN month$="Jun"
IF month%=7 THEN month$="Jul"
IF month%=8 THEN month$="Aug"
IF month%=9 THEN month$="Sep"
IF month%=10 THEN month$="Oct"
IF month%=11 THEN month$="Nov"
IF month%=12 THEN month$="Dec"
Date2$=DateTemp1$+"-"+month$+"-"+DateTemp3$
RETURN


REM ******************************************************************
RtimeDec:
REM ******************************************************************
Rtime% = RTime%-1
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


REM ******************************************************************
CheckHotKey:
REM ******************************************************************

GotOne=0
Keyin$=INKEY$
a=asc(keyin$)
IF a=0 AND ring%=1 THEN
  cd=PEEKL(12570624&)
  cd=cd AND 8192
  IF cd<>0 THEN Error 99
  IF LOC(1)>0 THEN Keyin$=INPUT$(1,1)
END IF

IF Keyin$="" THEN RETURN
Keyin$=UCASE$(Keyin$)
a=ASC(Keyin$)

IF a=80 THEN GotOne=1:in$="P":RETURN
IF a=73 THEN GotOne=1:in$="I":RETURN
IF a=65 THEN GotOne=1:in$="A":RETURN
IF a=81 THEN GotOne=1:in$="Q":RETURN
IF a=83 THEN GotOne=1:in$="S":RETURN

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
