ON ERROR GOTO Errors

Title:
  CLS
  PRINT 
  PRINT "The Graffiti Wall Setup Program"
  PRINT "          Version 3.0"
  PRINT 

GetStuff:
  GOSUB Config
  INPUT "Security level required for access to Maintenance Menu";mainman%
GetStuff2:
  INPUT "Maximum lines per message (1-99)";maxlines%
  IF maxlines%<1 OR maxlines%>99 THEN GetStuff2

MainMenu:
  PRINT :PRINT :PRINT :PRINT 
  GOSUB DisplayWalls
  PRINT 
  INPUT "<E>dit, <A>dd, <D>elete, <Q>uit: ";dowhat$
  dowhat$=UCASE$(dowhat$)
  IF dowhat$<>"E" AND dowhat$<>"A" AND dowhat$<>"D" AND dowhat$<>"Q" THEN MainMenu
  IF dowhat$="E" THEN GOSUB EditWall:dowhat$=""
  IF dowhat$="A" THEN GOSUB AddWall:dowhat$=""
  IF dowhat$="D" THEN GOSUB DeleteWall:dowhat$=""
  IF dowhat$="Q" THEN Quit
GOTO MainMenu  

EditWall:
  PRINT:PRINT
  PRINT "-=  EDIT A WALL  =-"
  GOSUB PickWall
  LINE INPUT "New name: "; wallname$(wall%)
  INPUT "New security level: ";wallsec%(wall%)
RETURN

AddWall:
  PRINT:PRINT 
  PRINT "-=  ADD A WALL  =-"
  LINE INPUT "Name of wall: ";temp$
  INPUT "Security level: ";temp%
  FOR ct%=1 TO 30
    IF wallname$(ct%)="DELETED" OR wallname$(ct%)="" THEN
      IF ct%>walls% THEN walls%=walls%+1
      wallname$(ct%)=temp$
      wallsec%(ct%)=temp%
      GOTO AddWall2
    END IF
  NEXT ct%
AddWall2:
RETURN

DeleteWall:
  PRINT:PRINT 
  PRINT "-=  DELETE A WALL  =-"
  GOSUB PickWall
  IF wall%=0 THEN RETURN
  INPUT "Are you sure (y/N)";temp$
  temp$=UCASE$(temp$)
  IF temp$="Y" THEN
    wallname$(wall%)="DELETED"
    wallsec%(wall%)=9
  END IF 
RETURN

Quit:
  GOSUB CountEm
  GOSUB SaveConfig
  PRINT
  PRINT "Finished!"
END

DisplayWalls:
  PRINT:PRINT 
  PRINT "  #  LEV    WALL NAME"
  FOR ct%=1 TO walls%
    IF wallname$(ct%)<>"DELETED" THEN
      IF ct%<10 THEN PRINT " ";
      PRINT ct%;" ";wallsec%(ct%);"    ";wallname$(ct%)
    END IF
  NEXT ct%
RETURN

PickWall:
  GOSUB DisplayWalls
  INPUT "Your choice: ";wall%
  IF wall%<0 OR wall%>walls% THEN PickWall
RETURN

Config:
  DIM wallname$(30), total%(30)
  flnm$=path$+"Config.Dat"
  walls%=0
100 OPEN "i",#2,flnm$
  INPUT #2,mainman%
  INPUT #2,maxlines%
  WHILE EOF(2)<>-1
    LINE INPUT #2,temp$
    INPUT #2,temp%
    INPUT #2,temp2%
    IF temp$<>"" THEN
      walls%=walls%+1
      wallname$(walls%)=temp$
      wallsec%(walls%)=temp%
      total%(walls%)=temp2%
    END IF
  WEND
199 CLOSE #2
RETURN

SaveConfig:
  flnm$="Config.Dat"
  OPEN "o",#2,flnm$
    PRINT #2,mainman%
    PRINT #2,maxlines%
    FOR ct%=1 TO walls%
      PRINT #2,wallname$(ct%)
      PRINT #2,wallsec%(ct%)
      PRINT #2,total%(ct%)
    NEXT ct%
  CLOSE #2
RETURN

CountEm:
  PRINT "Counting existing messages..."
  FOR wallct%=1 TO walls%
    temp$=STR$(wallct%)
    temp$=RIGHT$(temp$,LEN(temp$)-1)
    flnm$=path$+"Graffiti-"+temp$+".Dat"
    total%(wallct%)=0
    GOSUB ReadEm
  NEXT wallct%
RETURN

ReadEm:
900 OPEN "I",#2,flnm$
  WHILE EOF(2)<>-1
    GOSUB ReadMsgs
    total%(wallct%)=total%(wallct%)+1
  WEND
  CLOSE #2
999 '
RETURN

ReadMsgs:
  LINE INPUT #2, temp$(1)
  temp$=temp$(1)
  pos1%=INSTR(temp$,"/")
  pos2%=INSTR((pos1%+1),temp$,"/")
  who$=LEFT$(temp$,(pos1%-1))
  length%=VAL(MID$(temp$,(pos1%+1),(pos2%-pos1%-1)))
  temp$=RIGHT$(temp$,(LEN(temp$)-pos2%))
  FOR ct%=2 TO length%
    LINE INPUT #2, temp$(ct%)
  NEXT ct%
RETURN

Errors:
  IF ERL=100 AND ERR=62 THEN RESUME 199
  IF ERL=100 AND ERR=53 THEN PRINT "Warning, no CONFIG.DAT was found.":PRINT:RESUME 199
  IF ERL=900 AND ERR=53 THEN PRINT flnm$;" not found.  No messages.":RESUME 999
  PRINT
  PRINT "Error";ERR;" on line";ERL
STOP
  
