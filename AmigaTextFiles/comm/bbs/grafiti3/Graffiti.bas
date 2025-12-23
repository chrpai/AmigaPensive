1 DEFLNG a-z:cr$=CHR$(13)+CHR$(10):de$=CHR$(8)+" "+CHR$(8)

ON ERROR GOTO Errors

'2 rtime$="30":uname$="SYSOP":id$="1":lev$="9":bd$="2400":ring$="0"
'3 path$="dh1:metro/doorfiles1/door16/"
'4 test$="Y"

2 CLOSE#2:OPEN "I",#2,"RAM:USERDATA":LINE INPUT#2,rtime$:LINE INPUT#2,uname$
3 LINE INPUT#2,id$:LINE INPUT#2,lev$:LINE INPUT#2,bd$:LINE INPUT#2,ring$
4 LINE INPUT#2,path$:CLOSE#2

5 rtime%=VAL(rtime$):id%=VAL(id$):lev%=VAL(lev$):ring%=VAL(ring$):GOSUB modem.o
6 ON TIMER(60) GOSUB t.dec
7 Title$="Graffiti Wall v3.0   ID:"+STR$(id%)+"  "+uname$+"  Lev:"+STR$(lev%)
  SCREEN 1,640,200,4,2
8 WINDOW 1,Title$,(0,0)-(631,185),6,1:WINDOW OUTPUT 1:TIMER ON
  PALETTE 0,0,0,0
  PALETTE 1,.93,.2,0
  PALETTE 2,.33,.87,0
  PALETTE 3,1,1,.13
  PALETTE 4,.4,.6,1
  PALETTE 5,.8,0,.93
  PALETTE 6,.47,.87,1
  PALETTE 7,1,1,1
  LIBRARY "exec.library"
  DECLARE FUNCTION OpenDevice% LIBRARY
  DECLARE FUNCTION AllocMem& LIBRARY
  DECLARE FUNCTION AllocSignal% LIBRARY
  DECLARE FUNCTION FindTask& LIBRARY
  DECLARE FUNCTION DoIO& LIBRARY
  ansi%=1
  icon%=0
  cr$=CHR$(13)+CHR$(10)
  de$=CHR$(8)+" "+CHR$(8)
  esc$=CHR$(27)
  ansi0$=esc$+"[0m"
  ansi1$=esc$+"[31m"
  ansi2$=esc$+"[32m"
  ansi3$=esc$+"[33m"
  ansi4$=esc$+"[34m"
  ansi5$=esc$+"[35m"
  ansi6$=esc$+"[36m"
  ansi7$=esc$+"[37m"

WantAnsi:
  o$=cr$+"Do you want ANSI (y/N)? ":in%=3:il%=1:GOSUB modem
  IF in$="YES" THEN ansi%=1 ELSE ansi%=0:PALETTE 1,1,1,1:CLS

'========================  start your program here  =======================
DIM ln$(99), wallname$(30), total%(30), wallsec%(30)

Title:
  o$=cr$+cr$+cr$+cr$:GOSUB modem.out
  pa%=1:o$=SPACE$(29)+"+---------------------+"+cr$:GOSUB modem.out
  o$=SPACE$(29)+"|":GOSUB modem.out
  pa%=6:o$="  THE GRAFFITI WALL  ":GOSUB modem.out
  pa%=1:o$="|"+cr$:GOSUB modem.out
  o$=SPACE$(29)+"|":GOSUB modem.out
  pa%=6:o$="          by         ":GOSUB modem.out
  pa%=1:o$="|"+cr$:GOSUB modem.out
  o$=SPACE$(29)+"|":GOSUB modem.out
  pa%=6:o$="    Mark D. Turner   ":GOSUB modem.out
  pa%=1:o$="|"+cr$:GOSUB modem.out
  pa%=1:o$=SPACE$(29)+"+---------------------+"+cr$:GOSUB modem.out
  o$=cr$+cr$+cr$+cr$+cr$+cr$:GOSUB modem.out

Config:
  flnm$=path$+"Config.Dat"
  walls%=0
900 OPEN "I",#2,flnm$
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
999 CLOSE #2
  IF maxlines%>99 THEN maxlines%=99
  IF maxlines%<1 THEN maxlines%=1

  GOSUB AnyKey  
  o$=cr$+cr$+cr$:GOSUB modem.out

MainLoop:
  pa%=1:o$=cr$+cr$+cr$+cr$+"=- THE GRAFFITI WALL -="+cr$+cr$:GOSUB modem.out
  pa%=7:o$="     <R>":GOSUB modem.out
  pa%=6:o$="ead"+cr$:GOSUB modem.out
  pa%=7:o$="     <W>":GOSUB modem.out
  pa%=6:o$="rite"+cr$:GOSUB modem.out
  IF lev%=mainman% THEN
    pa%=7:o$="     <M>":GOSUB modem.out
    pa%=6:o$="aintenance"+cr$:GOSUB modem.out
  END IF
  pa%=7:o$="     <Q>":GOSUB modem.out
  pa%=6:o$="uit"+cr$+cr$:GOSUB modem.out
  pa%=3:o$="     Your choice: ":in%=0:il%=1:GOSUB modem
  CH$=in$
  IF CH$="R" THEN GOSUB ReadWall
  IF CH$="W" THEN GOSUB WriteWall
  IF CH$="Q" THEN Quit
  IF CH$="M" AND lev%=>mainman% THEN
    o$=cr$+cr$+cr$
    GOSUB modem.out
    GOSUB Maintenance
  END IF
GOTO MainLoop

ReadWall:
2000 '
  GOSUB WhichWall
  IF wall%=0 THEN RETURN
  pa%=1:o$=cr$+cr$+cr$+cr$+"WRITINGS ON THE GRAFFITI WALL"+cr$+cr$:GOSUB modem.out
  flnm$=path$+"Graffiti-"+wall$+".Dat"
  linect%=0:skip%=0:go$="C":msg%=0
  OPEN "I",#2,flnm$
  WHILE EOF(2)<>-1
    GOSUB ReadIt
    IF go$<>"S" THEN o$=cr$:GOSUB modem.out:linect%=linect%+1
    IF sysop$="YES" AND go$<>"S" THEN pa%=2:o$=who$+":"+cr$:GOSUB modem.out:linect%=linect%+1
    IF go$<>"S" THEN
      pa%=7:o$=temp$+cr$:GOSUB modem.out:linect%=linect%+1
      FOR ct%=2 TO length%
        pa%=7:o$=ln$(ct%)+cr$:GOSUB modem.out:linect%=linect%+1
      NEXT ct%
    END IF
    msg%=msg%+1
    IF skip%=msg%+1 THEN go$="C"
    IF linect%>15 THEN linect%=0:GOSUB GoOrStop:IF go$="Q" THEN ReadWall5
  WEND
  GOSUB AnyKey
ReadWall5:
  CLOSE #2
2999 '
RETURN

WhichWall:
  wall$="1"
  o$=cr$+cr$:GOSUB modem.out
  FOR wallct%=1 TO walls%
    IF lev%>=wallsec%(wallct%) AND wallname$(wallct%)<>"DELETED" THEN
      temp$=STR$(wallct%)
      temp$=RIGHT$(temp$,LEN(temp$)-1)
      pa%=7:o$="     "+temp$+"  ":GOSUB modem.out
      pa%=6:o$=wallname$(wallct%)+cr$:GOSUB modem.out
    END IF
  NEXT wallct%
WhichWall2:
  pa%=3:o$=cr$+"     Which wall? ":in%=4:il%=2:GOSUB modem
  wall$=in$
  wall%=VAL(wall$)
  IF wall%<0 OR wall%>walls% THEN WhichWall2
  IF lev%<wallsec%(wall%) THEN WhichWall2
RETURN

WriteWall:
  GOSUB WhichWall
  IF wall%=0 THEN RETURN
  flnm$=path$+"Graffiti-"+wall$+".Dat"
  GOSUB Text.Ed
RETURN

Text.Ed:
  GOSUB TimeCheck
  IF car%>0 THEN
    GOTO text9
  END IF
  FOR sc%=0 TO maxlines%
    ln$(sc%)=""
  NEXT sc%
  in%=5
  il%=1
  ln%=0
  in$=""
  cs$="Maximum number of lines"+STR$(maxlines%)+".  75 characters per line."
  GOSUB center
  pa%=4
  o$=cr$+cr$+lcs$
  GOSUB modem.out
  cs$="Enter a Carriage Return on a blank line to exit Editor."
  GOSUB center
  pa%=5
  o$=cr$+cr$+lcs$+cr$
  GOSUB modem.out
  pa%=0
  GOSUB aprt
  text1:
  IF in$=CHR$(13) OR in$=CHR$(10) OR in$=CHR$(32) THEN
    in$=""
  END IF
  ln%=ln%+1
  IF ln%<10 THEN
    o$=cr$+STR$(ln%)+": "+in$
  ELSE
    o$=cr$+RIGHT$(STR$(ln%),2)+": "+in$
  END IF
  GOSUB modem.out
  ln$(ln%)=ln$(ln%)+in$
  text2:
  GOSUB modem.in
  IF car%>0 THEN
    GOTO text9
  END IF
  IF in$=CHR$(13) AND LEN(ln$(ln%))=0 THEN
    ln%=ln%-1
    GOTO text5
  END IF
  IF in$=CHR$(13) THEN
    GOTO text4
  END IF
  ln$(ln%)=ln$(ln%)+in$
  IF LEN(ln$(ln%))<75 THEN
    GOTO text2
  END IF
  in$=""
  i%=LEN(ln$(ln%))
  string1$=ln$(ln%)
  string2$=" "
  GOSUB match0
  IF match%=0 THEN
    GOTO text4
  END IF
  text3:
  aw$=MID$(ln$(ln%),i%,1)
  IF aw$=" " AND LEN(ln$(ln%))=75 THEN
    in$=" "
    i%=i%-1
    ln$(ln%)=LEFT$(ln$(ln%),i%)
    o$=de$
    GOSUB modem.out
    GOTO text4
  END IF
  IF aw$<>" " THEN
    in$=aw$+in$
    i%=i%-1
    ln$(ln%)=LEFT$(ln$(ln%),i%)
    o$=de$
    GOSUB modem.out
    GOTO text3
  END IF
  IF aw$=" " THEN
    i%=i%-1
    ln$(ln%)=LEFT$(ln$(ln%),i%)
    o$=de$
    GOSUB modem.out
  END IF
  text4:
'  IF ln%=maxlines%-3 THEN
'    pa%=3
'    o$=cr$+"Only 3 lines left."
'    GOSUB modem.out
'  END IF
'  IF ln%=maxlines%-1 THEN
'    pa%=3
'    o$=cr$+"This is your last line."
'    GOSUB modem.out
'  END IF
  pa%=0
  GOSUB aprt
  IF ln%<maxlines% THEN
    GOTO text1
  END IF
  text5:
  pa%=3:o$=cr$+cr$+"[S]ave - [R]esume - [E]dit - [D]elete - [I]nsert - [C]enter"
  o$=o$+" - [L]ist - [A]bort"+cr$+cr$+"Enter Command: "
  in%=0
  il%=1
  GOSUB modem

  IF in$="S" THEN
    ln$(1)=uname$+"/"+STR$(ln%)+"/"+ln$(1)
    OPEN "A",#2,flnm$
      FOR ct%=1 TO ln%
        PRINT#2,ln$(ct%)
      NEXT ct%
    CLOSE #2
    total%(wall%)=total%(wall%)+1
    GOSUB UpdateConfig
    pa%=5:o$=cr$+"Thank you!"+cr$:GOSUB modem.out
    RETURN
  END IF

  IF in$="R" THEN
    in$=""
    in%=5
    il%=1
    GOTO text4
  END IF

  IF in$="E" THEN
    text5.2:
    pa%=3
    o$=cr$+"Enter Line # To Edit: "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      GOTO text5
    ELSE
      eline%=VAL(in$)
    END IF
    IF eline%<1 OR eline%>ln% THEN
      GOTO text5.2
    END IF
    IF eline%<10 THEN
      o$=cr$+" "+STR$(eline%)+": "+ln$(eline%)
      GOSUB modem
    END IF
    IF eline%>9 THEN
      o$=cr$+RIGHT$(STR$(eline%),2)+": "+ln$(eline%)
      GOSUB modem
    END IF
    text5.21:
    o$=cr$+"Enter New Line"+cr$+"  : "
    in%=1
    il%=75
    GOSUB modem
    IF b%=0 THEN
      pa%=3
      o$=cr$+"No Change!"
      GOSUB modem.out
      pa%=0
      GOSUB aprt
      GOTO text5
    END IF
    ln$(eline%)=in$
    GOTO text5
  END IF

  IF in$="D" THEN
    text5.3:
    pa%=3
    o$=cr$+"Start Delete At Line #: "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      GOTO text5
    ELSE
      sline%=VAL(in$)
    END IF
    IF sline%<1 OR sline%>ln% THEN
      GOTO text5.3
    END IF
    text5.31:
    pa%=3
    o$=cr$+"End Delete At Line #: "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      GOTO text5
    ELSE
      eline%=VAL(in$)
    END IF
    IF eline%<sline% OR eline%>ln% THEN
      GOTO text5.31
    END IF
    dline%=eline%-sline%+1
    FOR qm%=sline% TO ln%
      ln$(qm%)=""
      ln$(qm%)=ln$(qm%+dline%)
    NEXT qm%
    ln%=ln%-dline%
    GOTO text5
  END IF

  IF in$="C" THEN
    text5.4:
    pa%=3
    o$=cr$+"Enter Line # To Center: "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      GOTO text5
    ELSE
      cline%=VAL(in$)
    END IF
    IF cline%<1 OR cline%>ln% THEN
      GOTO text5.4
    END IF
    cs$=ln$(cline%)
    GOSUB center
    ln$(cline%)=lcs$
    GOTO text5
  END IF

  IF in$="L" THEN
    text5.5:
    pa%=3
    o$=cr$+"Start List At Line # (Return = 1): "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      sline%=1
      GOTO text5.51
    END IF
    sline%=VAL(in$)
    IF sline%<1 OR sline%>ln% THEN
      GOTO text5.5
    END IF
    text5.51:
    pa%=3
    o$=cr$+"End List At Line # (Return = To End): "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      eline%=ln%
      GOTO text5.52
    END IF
    eline%=VAL(in$)
    IF eline%<sline% OR eline%>ln% THEN
      GOTO text5.51
    END IF
    text5.52:
    o$=cr$
    GOSUB modem.out
    FOR qm%=sline% TO eline%
      IF qm%<10 THEN
        o$=cr$+" "+STR$(qm%)+": "+ln$(qm%)
        GOSUB modem.out
      END IF
      IF qm%>9 THEN
        o$=cr$+RIGHT$(STR$(qm%),2)+": "+ln$(qm%)
        GOSUB modem.out
      END IF
    NEXT qm%
    GOTO text5
  END IF

  IF in$="I" THEN
    text5.6:
    pa%=3
    o$=cr$+"Enter Line # To Insert: "
    in%=4
    il%=2
    GOSUB modem
    pa%=0
    GOSUB aprt
    IF b%=0 THEN
      GOTO text5
    ELSE
      iline%=VAL(in$)
    END IF
    IF iline%<1 OR iline%>ln% THEN
      GOTO text5.6
    END IF
    ln%=ln%+1
    IF ln%>maxlines%-1 THEN
      pa%=3
      o$=cr$+"Sorry, Message Is Full."
      GOSUB modem.out
      pa%=0
      GOSUB aprt
      GOTO text5
    END IF
    FOR qm%=ln% TO iline%+1 STEP -1
      ln$(qm%)=ln$(qm%-1)
    NEXT qm%
    ln$(iline%)=""
    eline%=iline%
    GOTO text5.21
  END IF

  IF in$="A" THEN
    pa%=0
    GOSUB aprt
    pa%=3
    o$=cr$+"Are You Sure You Want To Abort? (N/y) "
    yes%=0
    in%=3
    il%=1
    GOSUB modem
    in%=0
    pa%=0
    GOSUB aprt
    IF in$="NO" THEN
      GOTO text5
    END IF
    FOR sc%=0 TO maxlines%
      ln$(sc%)=""
    NEXT sc%
    ln%=0
    pa%=3
    o$=cr$+"Message Aborted!"
    GOSUB modem.out
    pa%=0
    GOSUB aprt
    GOTO text9
  END IF
  GOTO text5
  text9:
RETURN

header:
  cs$="CTRL S To PAUSE  -  ANY KEY To RESUME  -  SPACE BAR To ABORT"
  GOSUB center
  pa%=3
  o$=cr$+cr$+lcs$+cr$+cr$
  GOSUB modem.out
  pa%=0
  GOSUB aprt
RETURN

center:
  lcs%=LEN(cs$)
  lcs$=SPACE$(38-lcs%/2)+cs$
RETURN

UpdateConfig:
  OPEN "o",#2,path$+"Config.Dat"
    PRINT #2,mainman%
    PRINT #2,maxlines%
    FOR ct%=1 TO walls%
      PRINT #2,wallname$(ct%)
      PRINT #2,wallsec%(ct%)
      PRINT #2,total%(ct%)
    NEXT ct%
  CLOSE #2
RETURN

Quit:
  pa%=5:o$=cr$+"Come again soon!"+cr$+cr$:GOSUB modem.out
GOTO BYE

Maintenance:
  sysop$="YES"
  pa%=1:o$=cr$+cr$+cr$+"      =- MAINTENANCE -="+cr$+cr$:GOSUB modem.out
  pa%=7:o$="     <D>":GOSUB modem.out
  pa%=6:o$="elete"+cr$:GOSUB modem.out
  pa%=7:o$="     <W>":GOSUB modem.out
  pa%=6:o$="ho wrote it?"+cr$:GOSUB modem.out
  pa%=7:o$="     <E>":GOSUB modem.out
  pa%=6:o$="xit Maintenance"+cr$:GOSUB modem.out
  pa%=3:o$=cr$+"     Your choice: ":in%=0:il%=1:GOSUB modem
  CH$=in$
  IF CH$="D" THEN GOSUB DeleteMsg
  IF CH$="W" THEN sysop$="YES":GOSUB ReadWall
  IF CH$="E" THEN sysop$="NO":o$=cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$+cr$:GOSUB modem.out:RETURN
  GOTO Maintenance
RETURN

DeleteMsg:
6000 '
  GOSUB WhichWall
  flnm$=path$+"Graffiti-"+wall$+".Dat"
  flnm2$=path$+"Graffiti.Temp"
  OPEN "i",#2,flnm$
  OPEN "o",#3,flnm2$
  WHILE EOF(2)<>-1
    GOSUB ReadIt
    o$=cr$:GOSUB modem.out
    pa%=2:o$=who$+":"+cr$:GOSUB modem.out
    pa%=7:o$=temp$+cr$:GOSUB modem.out
    FOR ct%=2 TO length%
      pa%=7:o$=ln$(ct%)+cr$:GOSUB modem.out
    NEXT ct%
    pa%=3:o$=cr$+"Do you want to delete this (y/N)? ":in%=3:il%=1:yes=0:GOSUB modem
    IF in$="NO" THEN
      FOR ct%=1 TO length%
        PRINT #3,ln$(ct%)
      NEXT ct%
    ELSE
      total%(wall%)=total%(wall%)-1
    END IF
  WEND
  CLOSE #2
  CLOSE #3
  KILL flnm$
  NAME flnm2$ AS flnm$
  GOSUB UpdateConfig
6999 '
RETURN

ReadIt:
  LINE INPUT #2, ln$(1)
  temp$=ln$(1)
  POS1=INSTR(temp$,"/")
  POS2=INSTR((POS1+1),temp$,"/")
  who$=LEFT$(temp$,(POS1-1))
  length%=VAL(MID$(temp$,(POS1+1),(POS2-POS1-1)))
  temp$=RIGHT$(temp$,(LEN(temp$)-POS2))
  FOR ct%=2 TO length%
    LINE INPUT #2, ln$(ct%)
  NEXT ct%
RETURN

GoOrStop:
  pa%=3:o$=cr$+"Message"+STR$(msg%)+" of"+STR$(total%(wall%))+"...  <C>ontinue, <S>kip or <Q>uit: ":in%=0:il%=1:GOSUB modem
  go$=in$
  IF go$<>"C" AND go$<>"S" AND go$<>"Q" THEN go$="C"
GoOrStop2:
  IF go$="S" THEN o$="Skip to message #":in%=4:il%=5:GOSUB modem
  skip%=VAL(in$)
  IF go$="S" AND skip%<=msg% OR skip%>total%(wall%) THEN GoOrStop2
  IF skip%=msg%+1 THEN go$="C"
RETURN

AnyKey:
  pa%=5:o$=cr$+"Press any key... ":in%=0:il%=1:GOSUB modem
  o$=cr$:GOSUB modem.out
RETURN

Errors:
  ON ERROR GOTO Errors
  IF ERL=900 THEN RESUME 999
  IF ERL>=2000 AND ERL <2999 THEN RESUME 2999
  IF ERL>=6000 AND ERL <6999 THEN RESUME 6999
  pa%=1:o$=cr$+cr$+"Ouch!  A program error!"+cr$:GOSUB modem.out
  GOSUB AnyKey
GOTO BYE

'=======================  end your program here  ==========================

printme:
  GOSUB aprt
  conprint o$
RETURN

match0:
  match%=INSTR(string1$,string2$)
RETURN

aprt:
  IF pa%=9 THEN
    GOTO aprt1
  END IF
  IF pa%=0 THEN
    ansip$=ansi0$
    IF icon%=0 THEN
      conprint ansip$+esc$+"[37;40m"
    END IF
    pa%=9
  END IF
  IF pa%=1 THEN ansip$=ansi1$
  IF pa%=2 THEN ansip$=ansi2$
  IF pa%=3 THEN ansip$=ansi3$
  IF pa%=4 THEN ansip$=ansi4$
  IF pa%=5 THEN ansip$=ansi5$
  IF pa%=6 THEN ansip$=ansi6$
  IF pa%=7 THEN ansip$=ansi7$
  IF ansi%=1 AND ring%=1 AND no.out=0 THEN
    PRINT #1,ansip$;
  END IF
  IF icon%=0 AND pa%>0 AND pa%<8 THEN
    conprint ansip$
  END IF
  aprt1:
RETURN

BYE:
IF test$<>"Y" THEN
  TIMER OFF
  KILL "RAM:USERDATA"
  WINDOW CLOSE 1
  GOSUB modem.c
  CLEAR
END IF
END

modem:
  GOSUB modem.out:GOSUB modem.in
RETURN

modem.out:
  IF ansi%=1 THEN GOSUB aprt
  IF icon%=0 THEN
    IF ansi%=1 THEN
      GOSUB printme
    ELSE
      oo$=o$:lo%=LEN(oo$)
      FOR ih%=1 TO lo%
        IF MID$(oo$,ih%,1)=CHR$(13) THEN MID$(oo$,ih%,1)=CHR$(32)
      NEXT:PRINT oo$;:oo$=""
    END IF
  END IF
  IF ring%=1 AND no.out%=0 THEN PRINT#1,o$;:o$=""
  IF (modout%=1 OR cmdmenu%=1 OR dismsg%=1 OR chat%=1) THEN GOSUB modem.in
RETURN

mod.out1:
  o$="":abort%=1
RETURN

modem.in:
  idle&=TIMER:key$="":in$="":a$="":mi$="":a%=0:keyy%=0:b%=0
WHILE keyy%<>1
  key$=INKEY$:IF key$<>"" THEN mod.in1
  IF ring%=1 THEN
    cd&=PEEKL(12570624&):cd&=cd& AND 8192
    IF cd&<>0 THEN BYE
    IF LOC(1) THEN a$=INPUT$(1,1):key$=a$
  END IF
mod.in1:
  IF key$=CHR$(19) THEN
    pau$="":paused%=TIMER
    WHILE TIMER<paused%+45
      pau$=INKEY$
      IF pau$<>"" THEN key$="":pau$="":GOTO mod.in1.1
      IF ring%=1 THEN
        IF LOC(1) THEN key$="":pau$="":GOTO mod.in1.1
      END IF
    WEND
    key$="":pau$=""
  END IF
mod.in1.1:
  IF (modout%=1 OR cmdmenu%=1 OR dismsg%=1 OR chat%=1) AND key$="" THEN keyy%=1
  IF modout%=1 AND key$=CHR$(32) THEN mod.out1
  IF modout%=1 AND key$<>CHR$(32) THEN keyy%=1
  IF cmdmenu%=1 AND key$<>"" THEN cmd%=1
  IF dismsg%=1 AND key$<>"" THEN cmd%=1
  IF key$="" THEN mod.in2
mod.in1.5:
  a%=ASC(key$):b%=LEN(in$)
  IF a%=13 THEN mod.in1.6
  IF a%=129 THEN BYE
  IF (a%=8 OR a%=127) AND LEN(in$)>=1 THEN
    in$=LEFT$(in$,b%-1):b%=b%-1:o$=de$:GOSUB modem.out
  END IF
  IF b%=il% THEN mod.in2
  IF in%=0 THEN
    IF a%>31 AND a%<127 THEN
    IF cmd%=1 THEN cmdmenu%=0:dismsg%=0
      key$=UCASE$(key$):o$=key$+cr$:GOSUB modem.out:in$=key$:keyy%=1
    END IF
  END IF
mod.in1.6:
  IF in%=1 AND a%>31 AND a%<127 THEN o$=key$:GOSUB modem.out:in$=in$+key$
  IF in%=2 AND a%>31 AND a%<127 THEN
    key$=UCASE$(key$):o$=key$:GOSUB modem.out:in$=in$+key$
  END IF
  IF in%=3 THEN
    IF a%=13 AND yes%=1 THEN o$="YES":GOSUB modem.out:in$="YES":keyy%=1
    IF a%=13 AND yes%=0 THEN o$="NO":GOSUB modem.out:in$="NO":keyy%=1
    IF a%=89 OR a%=121 THEN o$="YES"+cr$:GOSUB modem.out:in$="YES":keyy%=1
    IF a%=78 OR a%=110 THEN o$="NO"+cr$:GOSUB modem.out:in$="NO":keyy%=1
  END IF
  IF in%=4 AND a%>47 AND a%<58 THEN o$=key$:GOSUB modem.out:in$=in$+key$
  IF in%=5 THEN
    IF (a%=8 OR a%=127) AND LEN(ln$(ln%))>=1 THEN
      ln$(ln%)=LEFT$(ln$(ln%),LEN(ln$(ln%))-1)
      o$=de$
      GOSUB modem.out
      keyy%=1
    END IF
    IF a%=13 THEN
      in$=key$
      keyy%=1
    END IF
    IF a%=9 THEN
      o$="     "
      GOSUB modem.out
      in$="     "
      keyy%=1
    END IF
    IF a%=185 AND ansi%=1 THEN
      o$=esc$+"[30m"
      GOSUB modem.out
      in$=esc$+"[30m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=178 AND ansi%=1 THEN
      o$=esc$+"[31m"
      GOSUB modem.out
      in$=esc$+"[31m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=179 AND ansi%=1 THEN
      o$=esc$+"[32m"
      GOSUB modem.out
      in$=esc$+"[32m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=162 AND ansi%=1 THEN
      o$=esc$+"[33m"
      GOSUB modem.out
      in$=esc$+"[33m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=188 AND ansi%=1 THEN
      o$=esc$+"[34m"
      GOSUB modem.out
      in$=esc$+"[34m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=189 AND ansi%=1 THEN
      o$=esc$+"[35m"
      GOSUB modem.out
      in$=esc$+"[35m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=190 AND ansi%=1 THEN
      o$=esc$+"[36m"
      GOSUB modem.out
      in$=esc$+"[36m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=183 AND ansi%=1 THEN
      o$=esc$+"[37m"
      GOSUB modem.out
      in$=esc$+"[37m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=171 AND ansi%=1 THEN
      o$=esc$+"[7m"
      GOSUB modem.out
      in$=esc$+"[7m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%=187 AND ansi%=1 THEN
      o$=esc$+"[0m"
      GOSUB modem.out
      in$=esc$+"[0m"
      keyy%=1
      GOTO mod.in1.7
    END IF
    IF a%>26 AND a%<127 THEN
      o$=key$
      GOSUB modem.out
      in$=key$
      keyy%=1
    END IF
  END IF

mod.in1.7:
  IF a%=13 THEN
    IF (in%<>5 AND in%<>7) THEN
      o$=cr$
      GOSUB modem.out
      keyy%=1
    END IF
  END IF

mod.in2:
  IF TIMER>idle&+300 THEN
    IF sysop%=1 AND logon%=0 AND TIMER>idle&+600 THEN BYE
    IF lev%>7 AND TIMER>idle&+600 THEN BYE
    IF lev%<8 THEN BYE
  END IF

mod.in2.0:
WEND
  IF ring%=1 THEN
   IF LOC(1) THEN mi$=INPUT$(LOF(1),1):mi$=""
  END IF

mod.in2.1:
RETURN

time1:
  t.t%=0:time1$=TIME$:t.t%=VAL(LEFT$(time1$,2))
  IF t.t%>11 THEN a$=" PM"
  IF t.t%<12 THEN a$=" AM"
  IF t.t%=0 THEN t.t%=12:time1$=RIGHT$(STR$(t.t%),2)+RIGHT$(time1$,6):GOTO timeout
  IF t.t%<10 THEN time1$=RIGHT$(STR$(t.t%),1)+RIGHT$(time1$,6):GOTO timeout
  IF t.t%>21 THEN
    t.t%=t.t%-12:time1$=RIGHT$(STR$(t.t%),2)+RIGHT$(time1$,6):GOTO timeout
  END IF
  IF t.t%>12 THEN t.t%=t.t%-12:time1$=RIGHT$(STR$(t.t%),1)+RIGHT$(time1$,6)
timeout:
  time1$=time1$+a$+"  "+DATE$
RETURN

TimeCheck:
  GOSUB time1
  IF rtime%<=0 THEN
    o$=cr$+cr$+"Sorry, Time Limit Exceeded!":GOSUB modem.out:GOTO BYE
  END IF
RETURN

t.dec:
  rtime%=rtime%-1
RETURN

delayy:
  pause%=TIMER
  WHILE TIMER<pause%+tt%
  WEND
RETURN

modem.c:
  CLOSE#1
RETURN

modem.o:
  OPEN "COM1:"+bd$+",n,8,1" AS #1 LEN=2048
RETURN

SUB conprint (tex$) STATIC
   SHARED c.io&
   IF c.io&=0 THEN : systemon
   POKEL c.io& +36,LEN(tex$)
   POKEL c.io& +40,SADD(tex$)
   e&=DoIO&(c.io&)
END SUB

SUB systemoff STATIC
   SHARED c.io&
   closeconsole c.io&
END SUB

SUB systemon STATIC
   SHARED c.io&,c.c$
   openconsole c.io&
   POKEW c.io&+28,3
END SUB

SUB openconsole (result&) STATIC
   createport "basic.con",0,c.port&
   IF c.port&=0 THEN ERROR 255
   createstdio c.port&,c.io&
   POKEL c.io&+36,124
   POKEL c.io&+40,WINDOW(7)
   dev$="console.device"+CHR$(0)
   c.error%=OpenDevice%(SADD(dev$),0,c.io&,0)
   IF c.error%<>0 THEN ERROR 255
   result&=c.io&
END SUB

SUB closeconsole (io&) STATIC
   port&=PEEKL(io&+14)
   CALL closedevice(io&)
   removeport port&
   removestdio io&
END SUB

SUB createstdio (port&,result&) STATIC
   opt&=2^16
   result&=AllocMem&(48,opt&)
   IF result&=0 THEN ERROR 7
   POKE result&+8,5
   POKEL result&+14,port&
   POKEW result&+18,50
END SUB

SUB removestdio (io&) STATIC
   IF io&<>0 THEN
      CALL freemem(io&,48)
   END IF
END SUB

SUB createport (port$,pri%,result&) STATIC
   opt&=2^16
   byte&=38+LEN(port$)
   port&=AllocMem&(byte&,opt&)
   IF port&=0 THEN ERROR 7
   POKEW port&,byte&
   port&=port&+2
   sigbit% = AllocSignal%(-1)
   IF sigbit%= -1 THEN
      CALL freemem(port&,byte&)
      ERROR 7
   END IF
   sigtask&=FindTask& (0)
   POKE port&+8,4
   POKE port&+ 9,pri%
   POKEL port&+10,port&+34
   POKE port&+15,sigbit%
   POKEL port&+16,sigtask&
   POKEL port&+20,port&+24
   POKEL port&+28,port&+20
   FOR lop% =1 TO LEN(port$)
      char%=ASC(MID$(port$,lop%,1))
      POKE port&+33+lop%,char%
   NEXT lop%
   CALL addport(port&)
   result&=port&
END SUB

SUB removeport (port&) STATIC
   byte&=PEEKW(port&-2)
   sigbit%=PEEK(port&+15)
   CALL remport(port&)
   CALL freesignal(sigbit%)
   CALL freemem(port&-2,byte&)
END SUB
