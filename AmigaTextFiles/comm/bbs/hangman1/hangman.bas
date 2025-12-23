3 CLEAR:DEFLNG a-z:DIM L$(20),D$(20),N$(26),U(50):cr$=CHR$(13)+CHR$(10):de$=CHR$(8)+" "+CHR$(8):null$=CHR$(0):ON TIMER(30) GOSUB sendnull
5 CLOSE#2:OPEN "I",#2,"RAM:USERDATA":LINE INPUT#2,tpc$:LINE INPUT#2,uname$:LINE INPUT#2,id$:LINE INPUT#2,lev$:LINE INPUT#2,bd$:LINE INPUT#2,ring$:LINE INPUT#2,sfd$:CLOSE#2
10 tpc=VAL(tpc$):id=VAL(id$):lev=VAL(lev$):ring=VAL(ring$):title$="HANGMAN     ID:"+STR$(id)+"  "+uname$+"  Lev:"+STR$(lev)
31 timeon=TIMER:GOSUB modem.o:GOSUB delay1:RANDOMIZE TIMER:WINDOW 1,title$,(0,0)-(617,185),7,-1:WINDOW OUTPUT 1
35 o$=cr$+cr$+cr$+"Welcome To Hangman! "+uname$:GOSUB modem.out
40 C=1:N=50
50 FOR I=1 TO 20: D$(I)="-":NEXT I:M=0
60 FOR I=1 TO 26:N$(I)="":NEXT I
95 IF C<N THEN 100
97 o$=cr$+cr$+"YOU DID ALL THE WORDS!!":GOSUB modem.out:GOTO bye
100 Q=INT(N*RND(1))+1
110 IF U(Q)=1 THEN 100
115 U(Q)=1:C=C+1:RESTORE:T1=0
150 FOR I=1 TO Q:READ aA$:NEXT I
160 L=LEN(aA$):FOR I=1 TO LEN(aA$):L$(I)=MID$(aA$,I,1):NEXT I
170 o$=cr$+"HERE ARE THE LETTERS YOU USED:"+cr$:GOSUB modem.out
180 FOR I=1 TO 26:o$=N$(I):GOSUB modem.out:IF N$(I+1)="" THEN 200
190 o$=",":GOSUB modem.out:NEXT I
200 o$=cr$+cr$:GOSUB modem.out:FOR I=1 TO L:o$=D$(I):GOSUB modem.out:NEXT I
210 o$=cr$+"Enter A Letter: ":in=0:il=1:GOSUB modem:G$=in$:R=0
220 FOR I=1 TO 26:IF N$(I)="" THEN 250
230 IF G$=N$(I) THEN o$=cr$+"YOU GUESSED THAT LETTER BEFORE!"+cr$:GOSUB modem.out:GOTO 170
240 NEXT I:o$=cr$+cr$+"PROGRAM ERROR.":GOTO bye
250 N$(I)=G$:T1=T1+1
260 FOR I=1 TO L:IF L$(I)=G$ THEN 280
270 NEXT I:IF R=0 THEN 290
275 GOTO 300
280 D$(I)=G$:R=R+1:GOTO 270
290 M=M+1:GOTO 400
300 FOR I=1 TO L:IF D$(I)="-" THEN 320
310 NEXT I:GOTO 390
320 o$=cr$:GOSUB modem.out:FOR I=1 TO L:o$=D$(I):GOSUB modem.out:NEXT I
330 o$=cr$+cr$+"WHAT DO YOU THINK THIS WORD IS? ":in=2:il=25:GOSUB modem:bB$=in$
340 IF aA$=bB$ THEN 360
350 o$=cr$+"WRONG. TRY ANOTHER LETTER."+cr$:GOSUB modem.out:GOTO 170
360 o$=cr$+cr$+"RIGHT!! IT TOOK YOU "+STR$(T1)+" GUESSES!":GOSUB modem.out
370 GOSUB timecheck:o$=cr$+cr$+"DO YOU WANT ANOTHER WORD (Y/n) ":yes=1:in=3:il=1:GOSUB modem:W$=in$:IF W$="YES" THEN 50
380 o$=cr$+cr$+"IT'S BEEN FUN! BYE FOR NOW.":GOSUB modem.out:GOTO bye
390 o$=cr$+cr$+"YOU FOUND THE WORD!":GOSUB modem.out:GOTO 370
400 o$=cr$+"SORRY, THAT LETTER ISN'T IN THE WORD.":GOSUB modem.out
410 ON M GOTO 415,420,425,430,435,440,445,450,455,460
415 o$=cr$+cr$+"FIRST, WE DRAW A HEAD"+cr$:GOSUB modem.out:file$=sfd$+"hanghead":GOTO 470
420 o$=cr$+cr$+"NOW WE DRAW A BODY."+cr$:GOSUB modem.out:file$=sfd$+"hangbody":GOTO 470
425 o$=cr$+cr$+"NEXT WE DRAW AN ARM."+cr$:GOSUB modem.out:file$=sfd$+"hang1arm":GOTO 470
430 o$=cr$+cr$+"THIS TIME IT'S THE OTHER ARM."+cr$:GOSUB modem.out:file$=sfd$+"hang2arm":GOTO 470
435 o$=cr$+cr$+"NOW,LET'S DRAW THE RIGHT LEG."+cr$:GOSUB modem.out:file$=sfd$+"hangrleg":GOTO 470
440 o$=cr$+cr$+"THIS TIME WE DRAW THE LEFT LEG."+cr$:GOSUB modem.out:file$=sfd$+"hanglleg":GOTO 470
445 o$=cr$+cr$+"NOW WE PUT UP A HAND."+cr$:GOSUB modem.out:file$=sfd$+"hang1hand":GOTO 470
450 o$=cr$+cr$+"NEXT THE OTHER HAND."+cr$:GOSUB modem.out:file$=sfd$+"hang2hand":GOTO 470
455 o$=cr$+cr$+"NOW WE DRAW ONE FOOT"+cr$:GOSUB modem.out:file$=sfd$+"hang1foot":GOTO 470
460 o$=cr$+cr$+"HERE'S THE OTHER FOOT -- YOU'RE HUNG!!"+cr$:GOSUB modem.out:file$=sfd$+"hang2foot"
470 ON ERROR GOTO 490
472 CLOSE#2:OPEN "I",#2,file$,4096:WHILE NOT EOF(2):LINE INPUT#2,o$:o$=o$+cr$:GOSUB modem.out:WEND:CLOSE#2
478 ON ERROR GOTO 0
480 GOTO 590
490 IF ERR=53 THEN o$=cr$+cr$+"Sorry, I Can't Find The File.":GOSUB modem.out:RESUME 478
590 IF M<>10 THEN 170
600 o$=cr$+cr$+"SORRY,YOU LOSE.  THE WORD WAS: "+aA$:GOSUB modem.out
610 o$=cr$+cr$+"YOU MISSED THAT ONE.":GOSUB modem.out:GOTO 370
700 DATA "GUM","SIN","FOR","CRY","LUG","BUY","FLY"
710 DATA "UGLY","EACH","FROM","WORK","TALK","WITH","SELF"
720 DATA "PIZZA","THING","TANDY","FIEND","ELBOW","FAULT","DIRTY"
730 DATA "BUDGET","SPIRIT","QUAINT","MAIDEN","ESCORT","PICKAX"
740 DATA "EXAMPLE","TENSION","QUININE","KIDNEY","REPLICA","SLEEPER"
750 DATA "TRIANGLE","KANGAROO","MAHOGANY","SERGEANT","SEQUENCE"
760 DATA "MOUSTACHE","DANGEROUS","SCIENTIST","DIFFERENT","QUIESCENT"
770 DATA "MAGISTRATE","ERRONEOUSLY","LOUDSPEAKER","PHYTOTOXIC"
780 DATA "MATRIMONIAL","PARASYMPATHOMIMETIC","THIGMOTROPISM"
990 PRINT "BYE NOW..."

bye:
o$=cr$+cr$+"Hope You Enjoyed Hangman!":GOSUB modem.out:GOSUB delay1
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
WHILE LOC(1)>0:a$=INPUT$(1,1):IF a$=null$ THEN bye
key$=a$:GOTO mod.in1
WEND
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
IF a=13 AND yes=1 THEN o$="YES":GOSUB modem.out:in$="YES":keyy=1
IF a=13 AND yes=0 THEN o$="NO":GOSUB modem.out:in$="NO":keyy=1
IF a=89 OR a=121 THEN o$="YES"+cr$:GOSUB modem.out:in$="YES":keyy=1
IF a=78 OR a=110 THEN o$="NO"+cr$:GOSUB modem.out:in$="NO":keyy=1
END IF
IF in=4 AND a>47 AND a<58 THEN o$=key$:GOSUB modem.out:in$=in$+key$
IF a=13 THEN o$=cr$:GOSUB modem.out:keyy=1
mod.in2:
IF TIMER>idle+300 THEN
IF (sysop=1 OR term=1) AND logon=0 AND TIMER>idle+600 THEN bye
IF lev>7 AND TIMER>idle+600 THEN bye
IF lev<8 THEN bye
END IF
mod.in2.0:
WEND
IF ring=1 THEN
WHILE LOC(1)>0:a$=INPUT$(1,1):mi$=mi$+a$:WEND
FOR pu=1 TO LEN(mi$)
IF MID$(mi$,pu,5)="ARRIE" OR MID$(mi$,pu,1)=null$ THEN bye
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
o$=cr$+cr$+"Sorry, Time Limit Exceeded!":GOSUB modem.out:GOTO bye

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
