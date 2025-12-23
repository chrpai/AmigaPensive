REM ******************************************************************

REM Program- NIM
REM    Date- 4 January 1990
REM  Author- Peter Francis Deane
REM
REM Version- 0.8

REM ******************************************************************


CLEAR ,50000&
3 CLEAR:DEFLNG b-z:DEFSNG a:DIM L$(20),D$(20),N$(26),U(50):cr$=CHR$(13)+CHR$(10):de$=CHR$(8)+" "+CHR$(8):null$=CHR$(0):esc$=CHR$(27):ON TIMER(30) GOSUB sendnull


5 CLOSE#2:OPEN "I",#2,"RAM:USERDATA":LINE INPUT#2,tpc$:LINE INPUT#2,uname$:LINE INPUT#2,id$:LINE INPUT#2,lev$:LINE INPUT#2,bd$:LINE INPUT#2,ring$:LINE INPUT#2,path$:CLOSE#2
REM 5 tpc$="30":id$="1":lev$="9":ring$="0":bd$="2400":uname$="PETER DEANE":path$="WB:Basic/Metro/"

10 tpc=VAL(tpc$):id=VAL(id$):lev=VAL(lev$):ring=VAL(ring$):TITLE$="NIM V0.9 - User: "+uname$+" (#"+STR$(id)+") Lev:"+STR$(lev)+"  Baud:"+bd$


REM ******************************************************************
Start:
REM ******************************************************************

timeon=TIMER:GOSUB modem.o:GOSUB delay1:RANDOMIZE TIMER:WINDOW 1,TITLE$,(0,0)-(617,185),23,-1:WINDOW OUTPUT 1

ON ERROR GOTO Woops

DIM ansi$(10)

o$=cr$+"NIM - a two-player game for Metro BBS":GOSUB modem.out
o$=cr$+"By Peter Deane (Inquestor BBS - Waratah, NSW, Australia)":GOSUB modem.out
o$=cr$+cr$+"Version 0.9"+cr$:GOSUB modem.out

GOSUB AnsiCheck
GOSUB IBMCheck
o$=cr$+cr$+cr$+"Setting up. Won't take a sec..":GOSUB modem.out
DIM a$(8)
DIM b$(8)

  ' a$(0)=Winner Index
  ' a$(1)=Player 1 ID
  ' a$(2)=Player 1 Name
  ' a$(3)=Player 2 ID
  ' a$(4)=Player 2 Name
  ' a$(5)=Date of Last Go
  ' a$(6)=Whose next go (ID#) - 
  '       OR IF game over - the ID of the winner.
  ' a$(7)=How many objects left!
  ' a$(8)=Game Status:
  '
  '       0= Game Waiting  1= 1 player signed up
  '       2= IN PROGRESS   3= Over, but still displaying (set this in config)
  
REM read in config

flnm$=path$+"nim.config"
o$=".....":GOSUB modem.out

OPEN flnm$ FOR INPUT AS 2
  LINE INPUT #2, maxgames$
  maxgames=VAL(maxgames$)
  LINE INPUT #2, password$
  password$=UCASE$(password$)
  LINE INPUT #2, displaytimes$
  displaytimes=VAL(displaytimes$)
CLOSE #2

o$=".....":GOSUB modem.out
DIM player(maxgames)

REM Have a look for games to clear, and update the counters...

GOSUB opengames
FOR J=1 TO 99 
GET #2, J

REM An inactive game!
IF VAL(a$(8))=3 THEN
     FOR k=0 TO 8
       b$(k)=a$(k)
     NEXT
     adead=VAL(b$(0))
     IF adead>displaytimes THEN
       b$(0)="0"  
       b$(8)="0"
     ELSE   
       adead=adead+1
       b$(0)=STR$(adead)
     END IF
     FOR k=0 TO 8
       LSET a$(k)=b$(k)
     NEXT k
     PUT #2,J
  END IF
NEXT J
CLOSE #2
o$=".....":GOSUB modem.out

GOSUB CheckNoGames
o$=".....":GOSUB modem.out

GOSUB About
GOSUB ClearScreen
o$=cr$+ansi$(6)+"You are involved in"+STR$(nogames)+" games.":GOSUB modem.out

REM ******************************************************************
topmenu:
REM ******************************************************************

in$=""
GOSUB TimeCheck
o$=cr$+cr$+cr$:GOSUB modem.out
o$=cr$+ansi$(1)+TIME$+ansi$(5)+" - Options: <P>-<J>-<B>-<S>-<Y>-<V>-<I>-<A>-<C>-<M>-<Q> [?=Menu]: ":GOSUB modem.out
in=0:il=1:GOSUB modem.in

IF in$="P" THEN GOSUB PlayGame
IF in$="J" THEN GOSUB join
IF in$="B" THEN GOSUB begin
IF in$="S" THEN GOSUB status
IF in$="Y" THEN GOSUB YourStat
IF in$="V" THEN GOSUB View
IF in$="I" THEN GOSUB instruct
IF in$="A" THEN GOSUB About
IF in$="C" THEN GOSUB ChangeSetup
IF in$="M" THEN GOSUB Maintenance
IF in$="Q" THEN GOTO quit
IF in$="?" THEN GOSUB PrintMenu

GOTO topmenu

REM ******************************************************************
PlayGame:
REM ******************************************************************

GOSUB ClearScreen
GOSUB opengames
o$=cr$+ansi$(5)+"It is your turn in these games:-":GOSUB modem.out
o$=cr$+cr$+ansi$(3)+"Game  Opponent                   Date Last Move  Objects Left":GOSUB modem.out
o$=cr$+ansi$(1)+"~~~~  ~~~~~~~~                   ~~~~~~~~~~~~~~  ~~~~~~~~~~~~":GOSUB modem.out
count=0

FOR k=1 TO maxgames
player(k)=0
NEXT

FOR k=1 TO 99
  GET #2, k
  IF VAL(a$(8))<>2 THEN NahNextOne
  IF VAL(a$(1))=id THEN InIt
  IF VAL(a$(3))=id THEN InIt
  GOTO NahNextOne
  
InIt:
IF VAL(a$(6))<>id THEN NahNextOne

  count=count+1
  IF count>maxgames THEN NahNextOne
  player(count)=k
  o$=cr$+ansi$(2)+STR$(k)
  IF k<10 THEN o$=o$+" "
  o$=o$+"   "+ansi$(5)
  IF VAL(a$(1))=id THEN o$=o$+a$(4)+SPACE$(27-LEN(a$(4)))
  IF VAL(a$(3))=id THEN o$=o$+a$(2)+SPACE$(27-LEN(a$(2)))
  o$=o$+ansi$(6)+a$(5)+"      "+ansi$(3)+a$(7)
  GOSUB modem.out
   
NahNextOne:
NEXT

CLOSE #2

total=0

FOR k=1 TO maxgames
  total=total+player(k)
NEXT

IF total=0 THEN o$=cr$+cr$+ansi$(2)+"Oh dear, it's not your turn in ANY of the games!":GOSUB modem.out:GOTO FinishPlay
o$=cr$+cr$+ansi$(1)+"Which game shall it be? (0 to Abort) ":GOSUB modem.out
in=4:il=2:GOSUB modem.in

IF VAL(in$)<1 THEN GOTO FinishPlay

FOR k=1 TO maxgames
  IF VAL(in$)=player(k) THEN Playing=VAL(in$):GOTO ValidGame
NEXT
o$=cr$+cr$+ansi$(2)+"Sorry, it's not your turn in game #"+in$:GOSUB modem.out
GOTO PlayGame

ValidGame:
  
GOSUB ClearScreen
GOSUB opengames

GET #2,Playing
NoLeft=VAL(a$(7))

flnm$=path$+"GameImgs/Nim"+STR$(NoLeft)
IF ansi=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
IF  ibm=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
GOSUB fileout

HowMany:
o$=cr$+cr$+ansi$(5)+"There are "+STR$(NoLeft)+" objects remaining.":GOSUB modem.out
o$=cr$+cr$+ansi$(7)+"Remove how many objects (1-3)? ":GOSUB modem.out
in=4:il=1:GOSUB modem.in
Remove=VAL(in$)
IF Remove<1 OR Remove>3 THEN HowMany
NoLeft=NoLeft-Remove
IF NoLeft<1 THEN GOTO LoseGame
IF NoLeft>1 THEN GOTO NextTurn

REM WON!!

flnm$=path$+"Nim.Won"
IF ansi=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
IF  ibm=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
GOSUB fileout

LSET a$(0)="1"
LSET a$(5)=DATE$
IF VAL(a$(1))=id THEN LSET a$(6)=a$(1)
IF VAL(a$(3))=id THEN LSET a$(6)=a$(3)
LSET a$(7)="0"
LSET a$(8)="3"

PUT #2,Playing
GOTO FinishPlay

NextTurn:
GOSUB ClearScreen

flnm$=path$+"GameImgs/Nim"+STR$(NoLeft)
IF ansi=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
IF  ibm=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
GOSUB fileout
o$=cr$+cr$+ansi$(5)+"There are NOW"+STR$(NoLeft)+" objects remaining.":GOSUB modem.out
o$=cr$+cr$+ansi$(7)+"Now, all you gotta do is wait for the other guy to play":GOSUB modem.out
LSET a$(5)=DATE$
IF VAL(a$(1))=id THEN LSET a$(6)=a$(3)
IF VAL(a$(3))=id THEN LSET a$(6)=a$(1)
LSET a$(7)=STR$(NoLeft)

PUT #2, Playing
GOTO FinishPlay

LoseGame:
GOSUB ClearScreen
flnm$=path$+"Nim.Lost"
IF ansi=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
IF  ibm=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
GOSUB fileout

LSET a$(0)="1"
LSET a$(5)=DATE$
IF VAL(a$(1))=id THEN LSET a$(6)=a$(3)
IF VAL(a$(3))=id THEN LSET a$(6)=a$(1)
LSET a$(7)="0"
LSET a$(8)="3"

PUT #2,Playing

FinishPlay:
CLOSE #2
GOSUB AnyKey

RETURN

REM ******************************************************************
join:
REM ******************************************************************

o$=cr$+"Okay! One moment...":GOSUB modem.out
GOSUB CheckNoGames
IF nogames >= maxgames THEN
  o$=cr$+cr$+cr$+ansi$(1)+"You are already active in"+STR$(nogames)+" games.":GOSUB modem.out
  o$=cr$+cr$+ansi$(3)+"Sorry, but you can't join in any more!!!":GOSUB modem.out
  GOSUB AnyKey
  RETURN
END IF

GOSUB opengames
GOSUB ClearScreen
o$=cr$+ansi$(5)+"Games waiting on other players:-":GOSUB modem.out
o$=cr$+cr$+ansi$(1)+"Game Opponent                   Started":GOSUB modem.out
o$=cr$+ansi$(2)+"~~~~ ~~~~~~~                    ~~~~~~~~~":GOSUB modem.out
k2!=1
FOR k=1 TO 99
  IF INT(k2!/20)=k2!/20 THEN
  o$=cr$+cr$+ansi$(7)+"More (Y/n)? ":in=3:il=1:YES=1:GOSUB modem
  IF in$="NO" THEN makesure
END IF

GET #2,k
IF VAL(a$(8))=1 THEN 
  o$=cr$+ansi$(2)+STR$(k)
  IF k<10 THEN o$=o$+" "
  o$=o$+"  "
  o$=o$+ansi$(3)+a$(2)+SPACE$(27-LEN(a$(2)))+ansi$(6)+a$(5)
  GOSUB modem.out
  k2!=k2!+1
END IF
NEXT k

makesure:
o$=cr$+cr$+ansi$(1)+"Join Which Game Number? (0 to Exit) ":in=4:il=2:GOSUB modem
joinup=VAL(in$)
IF joinup=0 THEN CLOSE#2:RETURN

GET #2,joinup
IF VAL(a$(8))=1 THEN 
  REM a game with 1 player waiting
  IF VAL(a$(1))=id THEN o$=cr$+cr$+ansi$(1)+"Sorry, you can't play yourself!":GOSUB modem.out:GOTO makesure
  LSET a$(3)=STR$(id)
  LSET a$(4)=uname$
  LSET a$(5)=DATE$
  LSET a$(6)=STR$(id)
  LSET a$(7)=STR$(17)
  LSET a$(8)="2"

  PUT #2, joinup
  o$=cr$+ansi$(3)+"You have now joined game"+STR$(joinup)+" and can now have the first move!":GOSUB modem.out
  CLOSE #2
  RETURN  
ELSE
  o$=cr$+ansi$(3)+"Sorry, that game is not available to join!":GOSUB modem.out
  GOTO makesure
END IF

REM ******************************************************************
begin:
REM ******************************************************************

GOSUB CheckNoGames
o$=cr$+cr$+ansi$(5)+"You are involved in"+STR$(nogames)+" games.":GOSUB modem.out
o$=cr$+cr$+ansi$(1)+"Are you SURE you wish to start a new game (Y/n)? ":GOSUB modem.out
in=3:il=1:YES=1:GOSUB modem.in
IF in$="NO" THEN notsure  'returns you straight off

IF nogames >= maxgames THEN
 o$=cr$+cr$+cr$+ansi$(1)+"You are already active in"+STR$(maxgames)+" games.":GOSUB modem.out
 o$=cr$+cr$+ansi$(3)+"Sorry, but you can't join in any more!!!":GOSUB modem.out
 GOSUB AnyKey
 RETURN
END IF

GOSUB opengames
count=0

findgame:
count=count+1
IF count=99 THEN 
 o$=cr$+cr$+cr$+ansi$(1)+"There are already 99 active games.":GOSUB modem.out
 o$=cr$+cr$+ansi$(3)+"Sorry, but you can't start any more!!!":GOSUB modem.out
 GOSUB AnyKey
 RETURN
END IF

GET #2, count
IF VAL(a$(8))<>0 THEN findgame
o$=cr$+cr$+ansi$(6)+"You have now commenced game #"+STR$(count):GOSUB modem.out
o$=cr$+cr$+ansi$(5)+"Let's hope someone else joins in soon!":GOSUB modem.out

LSET a$(0)="0"
LSET a$(1)=STR$(id)
LSET a$(2)=uname$
LSET a$(3)="0"
LSET a$(4)=" "
LSET a$(5)=DATE$
LSET a$(6)="0"
LSET a$(7)="17"
LSET a$(8)="1"

PUT #2,count
CLOSE #2
notsure:
RETURN

REM ******************************************************************
status:
REM ******************************************************************

GOSUB ClearScreen
o$=cr$+ansi$(2)+"OVERALL NIM GAMES STATUS:"+cr$:GOSUB modem.out
o$=cr$+ansi$(1)+" #   Player 1                   Player 2                   Date Last Go":GOSUB modem.out
o$=cr$+ansi$(3)+"~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~":GOSUB modem.out

GOSUB opengames
count!=0
FOR k=1 TO 99
  GET #2,k
  IF VAL(a$(8))<1 THEN NoInterest
  count!=count!+1
  o$=cr$+ansi$(1)+STR$(k)
  IF k<10 THEN o$=o$+" "
  o$=o$+ansi$(2)+"  "+a$(2)+SPACE$(27-LEN(a$(2)))+ansi$(3)+a$(4)+SPACE$(27-LEN(a$(4)))+ansi$(6)+a$(5)
  GOSUB modem.out
  o$=cr$+ansi$(5)

  IF VAL(a$(8))=1 THEN o$=o$+"     Game WAITING for another player to join."
  IF VAL(a$(8))=2 THEN
    o$=o$+"     Game in PROGRESS, waiting for "
    IF VAL(a$(1))=VAL(a$(6)) THEN o$=o$+a$(2)
    IF VAL(a$(3))=VAL(a$(6)) THEN o$=o$+a$(4)
  END IF
  IF VAL(a$(8))=3 THEN
    o$=o$+"     Game WON by "
    IF VAL(a$(1))=VAL(a$(6)) THEN o$=o$+a$(2)
    IF VAL(a$(3))=VAL(a$(6)) THEN o$=o$+a$(4)
  END IF
  
  o$=o$+cr$:GOSUB modem.out
  IF count!/6=INT(count!/6) THEN
    o$=cr$+ansi$(7)+"More (Y/n)? ":GOSUB modem.out
    in=3:il=1:YES=1:GOSUB modem.in
    IF in$="NO" THEN NoMore
    o$=cr$+cr$+cr$+ansi$(1)+" #   Player 1                   Player 2                   Date Last Go":GOSUB modem.out
    o$=cr$+ansi$(3)+"~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~":GOSUB modem.out
  END IF
  
NoInterest:
NEXT

NoMore:
GOSUB AnyKey
CLOSE #2
RETURN

REM ******************************************************************
YourStat:
REM ******************************************************************

GOSUB ClearScreen
o$=cr$+ansi$(2)+"THE GAMES YOU ARE INVOLVED IN: "+cr$:GOSUB modem.out

o$=cr$+ansi$(1)+" #   Player 1                   Player 2                   Date Last Go":GOSUB modem.out
o$=cr$+ansi$(3)+"~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~":GOSUB modem.out

GOSUB opengames
count!=0
FOR k=1 TO 99
  GET #2,k
  IF VAL(a$(8))<1 THEN NotInterested
  IF VAL(a$(1))<>id AND VAL(a$(3))<>id THEN NotInterested
  count!=count!+1
  o$=cr$+ansi$(1)+STR$(k)
  IF k<10 THEN o$=o$+" "
  o$=o$+ansi$(2)+"  "+a$(2)+SPACE$(27-LEN(a$(2)))+ansi$(3)+a$(4)+SPACE$(27-LEN(a$(4)))+ansi$(6)+a$(5)
  GOSUB modem.out
  o$=cr$+ansi$(5)

  IF VAL(a$(8))=1 THEN o$=o$+"     Game WAITING for another player to join."
  IF VAL(a$(8))=2 THEN
    o$=o$+"     Game in PROGRESS, waiting for "
    IF VAL(a$(1))=VAL(a$(6)) THEN o$=o$+a$(2)
    IF VAL(a$(3))=VAL(a$(6)) THEN o$=o$+a$(4)
  END IF
  IF VAL(a$(8))=3 THEN
    o$=o$+"     Game WON by "
    IF VAL(a$(1))=VAL(a$(6)) THEN o$=o$+a$(2)
    IF VAL(a$(3))=VAL(a$(6)) THEN o$=o$+a$(4)
  END IF
  
  o$=o$+cr$:GOSUB modem.out
  IF count!/6=INT(count!/6) THEN
    o$=cr$+ansi$(7)+"More (Y/n)? ":GOSUB modem.out
    in=3:il=1:YES=1:GOSUB modem.in
    IF in$="NO" THEN NoMore2
    o$=cr$+cr$+cr$+ansi$(1)+" #   Player 1                   Player 2                   Date Last Go":GOSUB modem.out
    o$=cr$+ansi$(3)+"~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~":GOSUB modem.out
  END IF
  
NotInterested:
NEXT

NoMore2:
GOSUB AnyKey
CLOSE #2

RETURN

REM ******************************************************************
View:
REM ******************************************************************

GOSUB ClearScreen
GOSUB opengames

ViewTop:
o$=cr$+cr$+ansi$(1)+"Which game number would you like to view (0 to exit)? ":in=4:il=2:GOSUB modem
ViewGame=VAL(in$)
IF ViewGame<1 THEN FinView
GET #2,ViewGame
IF VAL(a$(8))<>2 THEN
  o$=cr$+ansi$(2)+"Sorry, that game is not in progress at the moment."+cr$:GOSUB modem.out
  GOTO ViewTop
END IF

GOSUB ClearScreen
GET #2,ViewGame
NoLeft=VAL(a$(7))
flnm$=path$+"GameImgs/Nim"+STR$(NoLeft)
IF ansi=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
IF  ibm=1 THEN flnm$=flnm$+".1" ELSE flnm$=flnm$+".0"
GOSUB fileout

o$=cr$+cr$+ansi$(7)+"Game"+STR$(ViewGame)+":"+cr$+"~~~~~~~":GOSUB modem.out
o$=cr$+ansi$(3)+a$(2)+" Vs         "+ansi$(6)+a$(4):GOSUB modem.out
o$=cr$+cr$+ansi$(5)+"There are "+STR$(NoLeft)+" objects remaining.":GOSUB modem.out
o$=cr$+ansi$(2)+"The last go was made on "+a$(5)+"."
IF VAL(a$(6))=VAL(a$(1)) THEN o$=cr$+ansi$(1)+"It is waiting on "+a$(2):GOSUB modem.out   
IF VAL(a$(6))=VAL(a$(3)) THEN o$=cr$+ansi$(1)+"It is waiting on "+a$(4):GOSUB modem.out   
GOSUB AnyKey

FinView:
CLOSE #2
RETURN

REM ******************************************************************
instruct:
REM ******************************************************************

GOSUB ClearScreen
IF ansi=0 AND ibm=0 THEN flnm$=path$+"nim.inst.0.0"
IF ansi=0 AND ibm=1 THEN flnm$=path$+"nim.inst.0.1"
IF ansi=1 AND ibm=0 THEN flnm$=path$+"nim.inst.1.0"
IF ansi=1 AND ibm=1 THEN flnm$=path$+"nim.inst.1.1"
flnm$=flnm$+".1"
GOSUB fileout
GOSUB AnyKey
GOSUB ClearScreen

IF ansi=0 AND ibm=0 THEN flnm$=path$+"nim.inst.0.0"
IF ansi=0 AND ibm=1 THEN flnm$=path$+"nim.inst.0.1"
IF ansi=1 AND ibm=0 THEN flnm$=path$+"nim.inst.1.0"
IF ansi=1 AND ibm=1 THEN flnm$=path$+"nim.inst.1.1"
flnm$=flnm$+".2"
GOSUB fileout
GOSUB AnyKey
GOSUB ClearScreen

RETURN

REM ******************************************************************
About:
REM ******************************************************************

GOSUB ClearScreen
IF ansi=0 AND ibm=0 THEN flnm$=path$+"nim.about.0.0"
IF ansi=0 AND ibm=1 THEN flnm$=path$+"nim.about.0.1"
IF ansi=1 AND ibm=0 THEN flnm$=path$+"nim.about.1.0"
IF ansi=1 AND ibm=1 THEN flnm$=path$+"nim.about.1.1"
GOSUB fileout
GOSUB AnyKey

RETURN

REM ******************************************************************
PrintMenu:
REM ******************************************************************

GOSUB ClearScreen
IF ansi=0 AND ibm=0 THEN flnm$=path$+"nim.menu.0.0"
IF ansi=0 AND ibm=1 THEN flnm$=path$+"nim.menu.0.1"
IF ansi=1 AND ibm=0 THEN flnm$=path$+"nim.menu.1.0"
IF ansi=1 AND ibm=1 THEN flnm$=path$+"nim.menu.1.1"
GOSUB fileout

RETURN

REM ******************************************************************
ChangeSetup:
REM ******************************************************************

GOSUB ClearScreen
GOSUB AnsiCheck
GOSUB IBMCheck
RETURN

REM ******************************************************************
Maintenance:
REM ******************************************************************

GOSUB ClearScreen
o$=cr$+"Sysop Maintenance":GOSUB modem.out
o$=cr$+cr$+"This allows you to reset any of the games.":GOSUB modem.out
o$=cr$+cr$+"Enter the maintenance password: ":GOSUB modem.out
in=2:il=30:GOSUB modem.in
IF in$<>password$ THEN o$=cr$+cr$+"Password wrong!!":GOSUB modem.out:GOTO FinMaint
IF lev<9 THEN o$=cr$+cr$+"SHAME ON YOU! You're not the Sysop, anyway!":GOSUB modem.out:GOTO FinMaint

' you're in!

GOSUB opengames

Maintloop:
o$=cr$+"Which game to reset? (0 to Exit) ":GOSUB modem.out
in=4:il=2:GOSUB modem.in
ResetGame=VAL(in$)
IF ResetGame<1 THEN FinMaint
GET #2,ResetGame

o$=cr$+cr$+a$(2)+" Vs "+a$(4)+" Last Go: "+a$(5):GOSUB modem.out
o$=cr$+cr$+"Shall we reset this one (y/N)? ":in=3:il=1:YES=0:GOSUB modem
IF in$<> "YES" THEN Maintloop
o$=cr$+cr$+"Are you absolutely sure (Y/n)? ":in=3:il=1:YES=1:GOSUB modem
IF in$<> "YES" THEN Maintloop

LSET a$(0)="0"
LSET a$(1)="0"
LSET a$(2)=" "
LSET a$(3)="0"
LSET a$(4)=" "
LSET a$(5)="Never"
LSET a$(6)="0"
LSET a$(7)="17"
LSET a$(8)="0"

PUT #2,ResetGame

o$=cr$+cr$+"Okay, that game has been cleared!":GOSUB modem.out

GOTO Maintloop

FinMaint:

CLOSE #2
RETURN

REM ******************************************************************
quit:
REM ******************************************************************

GOSUB ClearScreen

IF ansi=0 AND ibm=0 THEN flnm$=path$+"nim.bye.0.0"
IF ansi=0 AND ibm=1 THEN flnm$=path$+"nim.bye.0.1"
IF ansi=1 AND ibm=0 THEN flnm$=path$+"nim.bye.1.0"
IF ansi=1 AND ibm=1 THEN flnm$=path$+"nim.bye.1.1"

GOSUB fileout
GOSUB AnyKey

GOTO BYE

'
'
REM Big Trouble if you get here!

GOTO BYE

REM ******************************************************************
REM Program Subroutines follow...
REM ******************************************************************


REM ******************************************************************
opengames:
REM ******************************************************************

flnm$=path$+"nim.games"

OPEN flnm$ AS #2 LEN=83
FIELD #2, 4 AS a$(0), 5 AS a$(1), 25 AS a$(2), 5 AS a$(3),25 AS a$(4), 10 AS a$(5), 5 AS a$(6), 3 AS a$(7), 1 AS a$(8)
RETURN

REM ******************************************************************
CheckNoGames:
REM ******************************************************************

REM Search for games the player is involved in:
nogames=0

GOSUB opengames
FOR J=1 TO 99

GET #2,J
IF (VAL(a$(1))=id OR VAL(a$(3))=id) THEN
 IF (VAL(a$(8))=2 OR VAL(a$(8))=1) THEN
  nogames=nogames+1
 END IF
END IF

NEXT J

CLOSE #2
RETURN



REM ******************************************************************
Woops:
REM ******************************************************************

 IF ERR=53 THEN
  o$=cr$+cr$+"Sorry, I Can't Find The File I Need!":GOSUB modem.out
  o$=cr$+"The sysop has not setup his directory properly,":GOSUB modem.out
  o$=cr$+"but a note has been made of which file was missing.":GOSUB modem.out
  o$=cr$+"Humble Apologies, but I think it safer if we now quit!":GOSUB modem.out
  eflnm$=path$+"Nim.Errors"
  OPEN eflnm$ FOR APPEND AS 6
  prob$="Couldn't find "+flnm$+" at line #"+STR$(ERL)+" at "+TIME$+" on "+DATE$
  PRINT #6,prob$
  CLOSE #6
  GOTO BYE
 END IF

 o$=cr$+cr$+"Oh Dear! We have had a really bad error occur.":GOSUB modem.out
 o$=cr$+"There has been a note made of the error in the Nim directory.":GOSUB modem.out
 o$=cr$+"Sorry!!  Nim will now quit.":GOSUB modem.out
  eflnm$=path$+"Nim.Errors"
  OPEN eflnm$ FOR OUTPUT AS 6
  prob$="Error "+STR$(ERR)+" at LINE #"+STR$(ERL)+" at "+TIME$+" ON "+DATE$
  PRINT #6,prob$
  CLOSE #6
GOTO BYE

REM ******************************************************************
REM My DRoutines now follow
REM ******************************************************************

fileout:

CLOSE #9
OPEN flnm$ FOR INPUT AS 9
  WHILE EOF(9)<>-1
    LINE INPUT #9, o$
    o$=cr$+o$
    GOSUB modem.out
  WEND
CLOSE #9
RETURN

REM ******************************************************************
AnyKey:
REM ******************************************************************

GOSUB TimeCheck

o$=cr$+cr$+ansi$(7)+"                               >>> Tap a key <<< "+cr$:GOSUB modem.out
GOSUB timecheck
in=0:il=1
GOSUB modem.in
RETURN
                                                           
REM ******************************************************************
AnsiCheck:
REM ******************************************************************

o$=cr$+cr$+"Do you want ANSI colours (Y/n)? ":GOSUB modem.out
in=3:il=3:YES=1:GOSUB modem.in
IF in$="YES" THEN ansi=1 ELSE ansi=0

IF ansi=1 THEN   
    ansi$(0)=esc$+"[30m"
    ansi$(1)=esc$+"[31m"
    ansi$(2)=esc$+"[32m"
    ansi$(3)=esc$+"[33m"
    ansi$(4)=esc$+"[34m"
    ansi$(5)=esc$+"[35m"
    ansi$(6)=esc$+"[36m"
    ansi$(7)=esc$+"[37m"
    ansi$(8)=CHR$(12)
    ansi$(9)=CHR$(0)        :REM SPARE
    ansi$(10)=esc$+"[0m"
END IF

IF ansi<>1 THEN  
    ansi$(0)=""
    ansi$(1)=""       :REM If you don't want ANSI
    ansi$(2)=""       :REM You get NUTHINK!! :-)
    ansi$(3)=""
    ansi$(4)=""
    ansi$(5)=""
    ansi$(6)=""
    ansi$(7)=""
    ansi$(8)=""
    ansi$(9)=""
    ansi$(10)=""
END IF

RETURN


REM ******************************************************************
IBMCheck:
REM ******************************************************************

o$=cr$+cr$+"Do you want IBM Graphics Characters (Y/n)? ":GOSUB modem.out
in=3:il=3:YES=1:GOSUB modem.in
IF in$="YES" THEN ibm=1 ELSE ibm=0

RETURN


REM ******************************************************************
ClearScreen:
REM ******************************************************************    
                                                  
GOSUB timecheck

IF ansi=0 AND ibm=0 THEN
 o$=cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+c$+cr$+cr$+cr$
 GOSUB modem.out
END IF

IF ansi=0 AND ibm=1 THEN
 o$=CHR$(12)
 GOSUB modem.out
 CLS
END IF

IF ansi=1 AND ibm=1 THEN
 o$=CHR$(12)
 GOSUB modem.out
 CLS
END IF

IF ansi=1 AND ibm=0 THEN
 o$=CHR$(12)
 GOSUB modem.out
 CLS
END IF

RETURN



REM ******************************************************************
REM Percy's DRoutines now follow. Thanks Perc!
REM ******************************************************************


BYE:
o$=cr$+cr$+"Hope to see you back soon!":GOSUB modem.out:GOSUB delay1
KILL "RAM:USERDATA":WINDOW CLOSE 1:GOSUB modem.c:CLEAR:END

modem:
GOSUB modem.out:GOSUB modem.in:RETURN

modem.out:
PRINT o$;:IF ring=1 AND no.out=0 THEN PRINT#1,o$;
o$="":RETURN

modem.in:
idle=TIMER:key$="":in$="":a$="":mi$="":a=0:keyy=0:b=0
WHILE keyy<>1
key$=INKEY$:IF key$<>"" THEN mod.in1
 IF ring=1 THEN
   cd=PEEKL(12570624&):cd=cd AND 8192
   IF cd<>0 THEN BYE
   IF LOC(1) THEN a$=INPUT$(1,1):key$=a$
   END IF

mod.in1:
IF key$=CHR$(19) THEN
pause=TIMER:hold=1:WHILE hold<>0
IF LOC(1)>0 THEN key$=INPUT$(1,1) ELSE key$=INKEY$
IF key$=CHR$(17) OR key$=CHR$(13) THEN hold=0
IF TIMER>pause+30 THEN key$=CHR$(17):hold=0
WEND
END IF
mod.in1.1:
IF key$="" THEN mod.in2
mod.in1.5:
a=ASC(key$):b=LEN(in$):IF a=13 THEN mod.in1.6
IF (a=8 OR a=127) AND LEN(in$)>=1 THEN in$=LEFT$(in$,b-1):b=b-1:o$=de$:GOSUB modem.out
IF b=il THEN mod.in2
IF in=0 THEN
IF a>31 AND a<127 THEN
key$=UCASE$(key$):o$=key$+cr$:GOSUB modem.out:in$=key$:keyy=1
END IF
END IF
mod.in1.6:
IF in=1 AND a>31 AND a<127 THEN o$=key$:GOSUB modem.out:in$=in$+key$
IF in=2 AND a>31 AND a<127 THEN key$=UCASE$(key$):o$=key$:GOSUB modem.out:in$=in$+key$
IF in=3 THEN
IF a=13 AND YES=1 THEN o$="YES":GOSUB modem.out:in$="YES":keyy=1
IF a=13 AND YES=0 THEN o$="NO":GOSUB modem.out:in$="NO":keyy=1
IF a=89 OR a=121 THEN o$="YES"+cr$:GOSUB modem.out:in$="YES":keyy=1
IF a=78 OR a=110 THEN o$="NO"+cr$:GOSUB modem.out:in$="NO":keyy=1
END IF
IF in=4 AND a>47 AND a<58 THEN o$=key$:GOSUB modem.out:in$=in$+key$
IF a=13 THEN o$=cr$:GOSUB modem.out:keyy=1
mod.in2:
IF TIMER>idle+300 THEN
IF (Sysop=1 OR term=1) AND logon=0 AND TIMER>idle+600 THEN BYE
IF lev>7 AND TIMER>idle+600 THEN BYE
IF lev<8 THEN BYE
END IF
mod.in2.0:
WEND
IF ring=1 THEN
WHILE LOC(1)>0:a$=INPUT$(1,1):mi$=mi$+a$:WEND
FOR pu=1 TO LEN(mi$)
IF MID$(mi$,pu,5)="ARRIE" OR MID$(mi$,pu,1)=null$ THEN BYE
NEXT
END IF
mod.in2.1:
RETURN

time1:
t.t=FRE(0):time1$=TIME$:t.t=VAL(LEFT$(time1$,2))
IF t.t>11 THEN a$=" PM"
IF t.t<12 THEN a$=" AM"
IF t.t=0 THEN t.t=12:time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6):GOTO timeout
IF t.t<10 THEN time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6):GOTO timeout
IF t.t>21 THEN t.t=t.t-12:time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6):GOTO timeout
IF t.t>12 THEN t.t=t.t-12:time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6)
timeout:
time1$=time1$+a$+"  "+DATE$:RETURN

timecheck:
GOSUB time1:IF TIMER<timeon+(tpc*60) THEN
timenow=TIMER:ctime=(timenow-timeon):ctime=CINT(ctime/60):rtime=tpc-ctime:RETURN
END IF
o$=cr$+cr$+cr$+"Sorry, Call Time Limit Exceeded! Call Back Later, Please!":GOSUB modem.out:GOTO BYE

delay1:
pause=TIMER:WHILE TIMER<pause+2:WEND:RETURN

modem.c:
IF ring=1 THEN TIMER OFF
GOSUB delay1:CLOSE#1:RETURN

modem.o:
CLOSE#1:OPEN "COM1:"+bd$+",n,8,1" AS #1 LEN=4096:GOSUB delay1:IF ring=1 THEN TIMER ON
RETURN

sendnull:
PRINT#1,null$;:RETURN
