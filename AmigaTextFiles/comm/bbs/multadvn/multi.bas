rem the following two lines are for the HiSoft Basic Compiler
REM $EVENT on
REM $OPTION a+,k50,e-,g-,n+,p-,o-,x-,l-,s-,u-,v-,w+,y+

1 DEFLNG a-z:cr$=CHR$(13)+CHR$(10):de$=CHR$(8)+" "+CHR$(8)

ON ERROR GOTO Errors

'2 rtime$="30":uname$="SYSOP":id$="1":lev$="9":bd$="2400":ring$="0"
'3 path$="dh1:metro/doorfiles1/door23/"
'4 test$="YES"

2 CLOSE#2:OPEN "I",#2,"RAM:USERDATA":LINE INPUT#2,rtime$:LINE INPUT#2,uname$
3 LINE INPUT#2,id$:LINE INPUT#2,lev$:LINE INPUT#2,bd$:LINE INPUT#2,ring$
4 LINE INPUT#2,path$:CLOSE#2

5 rtime=VAL(rtime$):id=VAL(id$):lev=VAL(lev$):ring=VAL(ring$):GOSUB modem.o
6 ON TIMER(60) GOSUB t.dec
7 title$="Multi-Adventure Door v1.0   ID:"+STR$(id)+"  "+uname$+"  Lev:"+STR$(lev)
8 WINDOW 1,title$,(0,0)-(617,185),7,-1:WINDOW OUTPUT 1:TIMER ON

SetUp:
  MaxG=40:MaxQ=100:MaxCh=9
  DIM gname$(MaxG),ques$(MaxQ),ch$(MaxQ,MaxCh),res$(MaxQ,MaxCh)
  o$=cr$+cr$+cr$+cr$+SPACE$(28)+"The Multi-Adventure Door"+cr$:GOSUB modem.out
  o$=SPACE$(28)+"   By Mark D. Turner"+cr$:GOSUB modem.out
  o$=SPACE$(28)+"    Copyright 1990"+cr$+cr$+cr$+cr$:GOSUB modem.out
  GOSUB AnyKey

LoadGameNames:
  flnm$=path$+"Menu.Dat"
100 OPEN "i",2,flnm$
  MaxGame=0
  WHILE EOF(2)<>-1
    LINE INPUT #2,temp$
    IF temp$<>"" THEN MaxGame=MaxGame+1:gname$(MaxGame)=temp$
    IF MaxGame=MaxG THEN LoadGameNames2 
  WEND
LoadGameNames2:
CLOSE 2

MainMenu:
  o$=cr$+cr$+cr$+" THE ADVENTURES"+cr$+cr$:GOSUB modem.out
  FOR ct=1 TO MaxGame
    o$=STR$(ct)+". "+gname$(ct)+cr$:GOSUB modem.out
  NEXT ct
  o$=STR$(0)+". Quit"+cr$:GOSUB modem.out
  o$=cr$:GOSUB modem.out
MainMenu2:
  GOSUB timecheck
  o$=" Your choice: ":in=4:il=2:GOSUB modem
  game$=in$:game=VAL(game$)
  IF game<0 OR game>MaxGame THEN MainMenu2
  IF game=0 THEN bye
  again$="YES"
  GOSUB Intro
  GOSUB ReadData
MainMenu3:
  GOSUB MainLoop
  IF again$="YES" THEN MainMenu3
GOTO MainMenu

Intro:
  o$=cr$+cr$+cr$:GOSUB modem.out
  flnm$=path$+"Intro."+game$
200 OPEN "I",2,flnm$
  WHILE EOF(2)<>-1
   LINE INPUT #2,temp$
   o$=temp$+cr$:GOSUB modem.out
  WEND
  CLOSE #2
299 'return here if no Intro file
RETURN

ReadData:
  o$=cr$+"Loading...":GOSUB modem.out
  ERASE ques$,ch$,res$
  DIM ques$(MaxQ),ch$(MaxQ,MaxCh),res$(MaxQ,MaxCh)
  ques=0:ch=0:res=0
  flnm$=path$+"Game."+game$
300 OPEN "i",2,flnm$
ReadData2:
  what$="":mark$=""
ReadData3:
  LINE INPUT #2,temp$
  IF temp$="" THEN ReadData3
  IF LEFT$(temp$,3)="\Q\" THEN what$="Q":ques=ques+1:ch=0:res=0
  IF LEFT$(temp$,3)="\A\" THEN what$="A":ch=ch+1
  IF LEFT$(temp$,3)="\N\" THEN what$="N":res=res+1
  IF LEFT$(temp$,3)="\Y\" THEN what$="Y":res=res+1
  IF LEFT$(temp$,3)="\W\" THEN what$="W":res=res+1
  IF temp$="\END\" THEN ReadData4
  IF what$="Q" OR what$="A" THEN temp$=RIGHT$(temp$,(LEN(temp$)-3))
  IF what$="" THEN mark$=cr$:what$=last$
  IF what$="Q" THEN ques$(ques)=ques$(ques)+mark$+temp$
  IF what$="A" THEN ch$(ques,ch)=ch$(ques,ch)+mark$+temp$
  IF what$="N" OR what$="Y" OR what$="W" THEN res$(ques,res)=res$(ques,res)+mark$+temp$
  last$=what$
  GOTO ReadData2
ReadData4:
  CLOSE #2
RETURN

MainLoop:
  FOR ct=1 TO ques
    o$=cr$+cr$+cr$:GOSUB modem.out
    temp$=ques$(ct):GOSUB SendLines
    o$=cr$:GOSUB modem.out
    FOR ct2=1 TO MaxCh
      IF ch$(ct,ct2)="" THEN hich=ct2-1:GOTO MainLoop2
      temp$=STR$(ct2)+". "+ch$(ct,ct2):what$="ANSWER":GOSUB SendLines
    NEXT ct2
MainLoop2:
    o$=cr$:GOSUB modem.out
MainLoop3:
    o$="Your choice: ":in=0:il=1:GOSUB modem
    ch=VAL(in$)
    IF ch<1 OR ch>hich THEN MainLoop3
    GOSUB CheckResults
    IF again$<>"CONTINUE" THEN RETURN
  NEXT ct
RETURN

CheckResults:
  again$=""
  res$=MID$(res$(ct,ch),2,1)
  reply$=RIGHT$(res$(ct,ch),(LEN(res$(ct,ch))-3))
  o$=cr$:GOSUB modem.out
  temp$=reply$:GOSUB SendLines
  IF res$="N" THEN
    GOSUB timecheck
    o$=cr$+"This game is over!"+cr$:GOSUB modem.out
    o$="Do you wish to try again (y/N)? "
    in=3:il=1:yes=0
    GOSUB modem
    again$=in$
    RETURN
  END IF
  IF res$="Y" THEN again$="CONTINUE":GOSUB AnyKey:RETURN

Winner:
  flnm$=path$+"Winner."+game$
400 OPEN "i",2,flnm$
  WHILE EOF(2)<>-1
    LINE INPUT #2,temp$
    o$=temp$+cr$:GOSUB modem.out
  WEND
  CLOSE 2
499 GOSUB AnyKey
  again$="WINNER"
RETURN

SendLines:
  linect=0
SendLines2:
  IF what$="ANSWER" AND linect>0 THEN temp$="    "+temp$
  pos1=INSTR(temp$,cr$)
  IF pos1>0 THEN
    temp2$=LEFT$(temp$,pos1-1)
    temp$=RIGHT$(temp$,(LEN(temp$)-pos1-1))
    o$=temp2$+cr$:GOSUB modem.out
    linect=linect+1
    GOTO SendLines2
  END IF
  o$=temp$+cr$:GOSUB modem.out
  what$=""
RETURN

AnyKey:
  o$=cr$+"Press any key... ":in=0:il=1:GOSUB modem
  o$=cr$:GOSUB modem.out
RETURN

Errors:
  IF ERL=100 AND ERR=53 THEN o$=flnm$+" is missing.  Sorry!"
  IF ERL=200 AND ERR=53 THEN PRINT "No Intro file found, but that's okay.":RESUME 299
  IF ERL=300 AND ERR=53 THEN o$=flnm$+" is missing.  Sorry!"
  IF ERL=400 AND ERR=53 THEN PRINT "No Winner file found, but that's okay.":RESUME 499
  o$="Error "+STR$(ERR)+" on line "+STR$(ERL)+".  SORRY!"
  GOSUB modem.out
  GOSUB AnyKey
  GOTO bye

bye:
TIMER OFF
IF test$<>"YES" THEN
  KILL "RAM:USERDATA"
  WINDOW CLOSE 1
END IF
GOSUB modem.c:CLEAR:END

modem:
GOSUB modem.out:GOSUB modem.in:RETURN

modem.out:
IF icon=0 THEN
 oo$=o$:lo=LEN(oo$)
 FOR ih=1 TO lo
  IF MID$(oo$,ih,1)=CHR$(13) THEN MID$(oo$,ih,1)=CHR$(32)
 NEXT:PRINT oo$;:oo$=""
END IF
IF ring=1 AND no.out=0 THEN PRINT#1,o$;:o$=""
IF (modout=1 OR cmdmenu=1 OR dismsg=1 OR chat=1) THEN GOSUB modem.in
RETURN
mod.out1:
abort=1:RETURN

modem.in:
idle=TIMER:key$="":in$="":a$="":mi$="":a=0:keyy=0:b=0
WHILE keyy<>1
 key$=INKEY$:IF key$<>"" THEN mod.in1
 IF ring=1 THEN
  cd=PEEKL(12570624&):cd=cd AND 8192
  IF cd<>0 THEN bye
  IF LOC(1) THEN a$=INPUT$(1,1):key$=a$
 END IF
mod.in1:
IF key$=CHR$(19) THEN
 pau$="":paused=TIMER
 WHILE TIMER<paused+45
  pau$=INKEY$
  IF pau$<>"" THEN key$="":pau$="":GOTO mod.in1.1
  IF ring=1 THEN
   IF LOC(1) THEN key$="":pau$="":GOTO mod.in1.1
  END IF
 WEND
 key$="":pau$=""
END IF
mod.in1.1:
IF (modout=1 OR cmdmenu=1 OR dismsg=1 OR chat=1) AND key$="" THEN keyy=1
IF modout=1 AND key$=CHR$(32) THEN mod.out1
IF modout=1 AND key$<>CHR$(32) THEN keyy=1
IF cmdmenu=1 AND key$<>"" THEN cmd=1
IF dismsg=1 AND key$<>"" THEN cmd=1
IF key$="" THEN mod.in2
mod.in1.5:
a=ASC(key$):b=LEN(in$)
IF a=13 THEN mod.in1.6
IF a=129 THEN bye
IF (a=8 OR a=127) AND LEN(in$)>=1 THEN
 in$=LEFT$(in$,b-1):b=b-1:o$=de$:GOSUB modem.out
END IF
IF b=il THEN mod.in2
IF in=0 THEN
 IF a>31 AND a<127 THEN
  IF cmd=1 THEN cmdmenu=0:dismsg=0
  key$=UCASE$(key$):o$=key$+cr$:GOSUB modem.out:in$=key$:keyy=1
 END IF
END IF
mod.in1.6:
IF in=1 AND a>31 AND a<127 THEN o$=key$:GOSUB modem.out:in$=in$+key$
IF in=2 AND a>31 AND a<127 THEN
 key$=UCASE$(key$):o$=key$:GOSUB modem.out:in$=in$+key$
END IF
IF in=3 THEN
 IF a=13 AND yes=1 THEN o$="YES":GOSUB modem.out:in$="YES":keyy=1
 IF a=13 AND yes=0 THEN o$="NO":GOSUB modem.out:in$="NO":keyy=1
 IF a=89 OR a=121 THEN o$="YES"+cr$:GOSUB modem.out:in$="YES":keyy=1
 IF a=78 OR a=110 THEN o$="NO"+cr$:GOSUB modem.out:in$="NO":keyy=1
END IF
IF in=4 AND a>47 AND a<58 THEN o$=key$:GOSUB modem.out:in$=in$+key$

IF a=13 THEN o$=cr$:GOSUB modem.out:keyy=1
mod.in2:
IF TIMER>idle+300 THEN
 IF sysop=1 AND logon=0 AND TIMER>idle+600 THEN bye
 IF lev>7 AND TIMER>idle+600 THEN bye
 IF lev<8 THEN bye
END IF
mod.in2.0:
WEND

IF ring=1 THEN
 IF LOC(1) THEN mi$=INPUT$(LOF(1),1):mi$=""
END IF
mod.in2.1:
RETURN

time1:
t.t=FRE(""):time1$=TIME$:t.t=VAL(LEFT$(time1$,2))
IF t.t>11 THEN a$=" PM"
IF t.t<12 THEN a$=" AM"
IF t.t=0 THEN t.t=12:time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6):GOTO timeout
IF t.t<10 THEN time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6):GOTO timeout
IF t.t>21 THEN
 t.t=t.t-12:time1$=RIGHT$(STR$(t.t),2)+RIGHT$(time1$,6):GOTO timeout
END IF
IF t.t>12 THEN t.t=t.t-12:time1$=RIGHT$(STR$(t.t),1)+RIGHT$(time1$,6)
timeout:
time1$=time1$+a$+"  "+DATE$:RETURN

timecheck:
GOSUB time1
IF rtime<=0 THEN
 o$=cr$+cr$+"Sorry, Time Limit Exceeded!":GOSUB modem.out:GOTO bye
END IF
RETURN

t.dec:
rtime=rtime-1
RETURN

delayy:
pause=TIMER:WHILE TIMER<pause+tt:WEND:RETURN

modem.c:
CLOSE#1:RETURN

modem.o:
OPEN "COM1:"+bd$+",n,8,1" AS #1 LEN=2048:RETURN
