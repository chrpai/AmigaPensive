3 CLEAR:DEFLNG a-z:DIM L$(20),D$(20),N$(26),U(50):cr$=CHR$(13)+CHR$(10):de$=CHR$(8)+" "+CHR$(8):null$=CHR$(0):ON TIMER(30) GOSUB sendnull

5 CLOSE#2:OPEN "I",#2,"RAM:USERDATA":LINE INPUT#2,tpc$:LINE INPUT#2,uname$:LINE INPUT#2,id$:LINE INPUT#2,lev$:LINE INPUT#2,bd$:LINE INPUT#2,ring$:LINE INPUT#2,sfd$:CLOSE#2
REM 5 tpc$="30":id$="1":lev$="9":ring$="0":bd$="2400":uname$="Fred Ghangi"

10 tpc=VAL(tpc$):id=VAL(id$):lev=VAL(lev$):ring=VAL(ring$):title$="The Shrink (by P Deane) - User # "+STR$(id)+": "+uname$+"  Lev:"+STR$(lev)

Start:
timeon=TIMER:GOSUB modem.o:GOSUB delay1:RANDOMIZE TIMER:WINDOW 1,title$,(0,0)-(617,185),23,-1:WINDOW OUTPUT 1
ON ERROR GOTO Woops

REM ******************************************************************

REM Program- Shrink
REM    Date- 11 December 1990
REM  Author- Peter Deane (AmigaBASIC)
REM
REM Version- 0.2

REM ******************************************************************

Startup:

o$=cr$+"*********************************":GOSUB modem.out
o$=cr$+"**                             **":GOSUB modem.out
o$=cr$+"**     T h e   S h r i n k     **":GOSUB modem.out
o$=cr$+"**                             **":GOSUB modem.out
o$=cr$+"*********************************":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"  By Peter Deane":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"   Version 0.2 19 December 1990":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Unashamedly pinched from Creative":GOSUB modem.out
o$=cr$+"Computing, Morristown, New Jersey":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Of course, we did change it a lot":GOSUB modem.out
o$=cr$+"from the original TRS80 version!":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Press the <ANY> key to start your session"+cr$:GOSUB modem.out
 
 GOSUB Timecheck
 in=0:il=1:GOSUB modem.in
 GOSUB clearscreen
 
Instructions: 
 
o$=cr$+"*********************************":GOSUB modem.out
o$=cr$+"**                             **":GOSUB modem.out
o$=cr$+"**     T h e   S h r i n k     **":GOSUB modem.out
o$=cr$+"**                             **":GOSUB modem.out
o$=cr$+"*********************************":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
  GOSUB Timecheck
o$=cr$+" Do you want instructions? (Y/n) "+cr$:in=3:il=1:yes=1:GOSUB modem

 IF in$="NO" THEN Hello
 
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Welcome to Sigmund's Workshop.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"If you have been having troubles":GOSUB modem.out
o$=cr$+"lately, it may help to talk them":GOSUB modem.out
o$=cr$+"over with someone friendly.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Here, you will be able to discuss":GOSUB modem.out
o$=cr$+"your problems with Sigmund, who":GOSUB modem.out
o$=cr$+"is a very friendly chap offering":GOSUB modem.out
o$=cr$+"sound advice.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Please feel free to reveal your":GOSUB modem.out
o$=cr$+"entire situation. You should not":GOSUB modem.out
o$=cr$+"be embarrassed, as Sigmund will":GOSUB modem.out
o$=cr$+"not tell anyone else your problem.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out

o$=cr$+"Well, now you know what the score is,":GOSUB modem.out
GOSUB Timecheck
o$=cr$+"do you still want to see Sigmund? (Y/n) ":in=3:il=1:yes=1:GOSUB modem

 IF in$="NO" THEN GOSUB BadLuck

Hello:
 
 GOSUB clearscreen

o$=cr$+"*********************************":GOSUB modem.out
o$=cr$+"**                             **":GOSUB modem.out
o$=cr$+"**     T h e   S h r i n k     **":GOSUB modem.out
o$=cr$+"**                             **":GOSUB modem.out
o$=cr$+"*********************************":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Welcome, O traveller, to my humble":GOSUB modem.out
o$=cr$+"workshop. My name is Sigmund, but":GOSUB modem.out
o$=cr$+"you can call me Siggie for short.":GOSUB modem.out
o$=cr$+"(Everyone else does).":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
  GOSUB Timecheck
o$=cr$+"Enlighten me as to your name, please"+cr$:in=1:il=20:GOSUB modem

na$=in$

o$=cr$+" ":GOSUB modem.out
o$=cr$+"Well, "+na$+", thanks for":GOSUB modem.out
o$=cr$+"dropping in. I hear you have a few":GOSUB modem.out
o$=cr$+"troubles you'd like me to help you":GOSUB modem.out
o$=cr$+"with.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
  GOSUB Timecheck
o$=cr$+"Are you sitting comfortably (Y/n)? ":in=3:il=1:yes=1:GOSUB modem


IF in$="NO" THEN o$=cr$+"Well, that's not my fault, is it?":GOSUB modem.out
IF in$="YES" THEN o$=cr$+"I'm glad to hear it... So am I!":GOSUB modem.out

o$=cr$+" ":GOSUB modem.out
o$=cr$+"Say, "+na$+", I can help you":GOSUB modem.out
o$=cr$+"with any problem you have, unless":GOSUB modem.out
o$=cr$+"it has anything to do with Greece.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"What sort of problem(s) do you have?":GOSUB modem.out
 
MainLoopTop:
o$=cr$+" ":GOSUB modem.out
o$=cr$+"-------------------------------------":GOSUB modem.out
o$=cr$+"Enter the first letter of the problem":GOSUB modem.out
o$=cr$+"-------------------------------------":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Problems I can help with include:":GOSUB modem.out
o$=cr$+cr$+"<S>ex, <H>ealth, <M>oney, <J>ob, <F>riends"+cr$+cr$:GOSUB modem.out

  GOSUB Timecheck
in=0:il=1:GOSUB modem.in
 
 IF in$="S" THEN Sex
 IF in$="H" THEN Health
 IF in$="M" THEN Money
 IF in$="J" THEN Job
 IF in$="F" THEN Friends
 
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Listen, I am very sorry about this,":GOSUB modem.out
o$=cr$+na$+", but your answer of "+in$:GOSUB modem.out
o$=cr$+"is all Greek to me. I told you":GOSUB modem.out
o$=cr$+"I cannot solve problems that have":GOSUB modem.out
o$=cr$+"anything to do with Greece.":GOSUB modem.out
 
 GOTO AnyMore  

AnyMore:

o$=cr$+" ":GOSUB modem.out
o$=cr$+"Any more problems I can help you":GOSUB modem.out
  GOSUB Timecheck
o$=cr$+"with, "+na$+" (Y/n)? ":in=3:il=1:yes=1:GOSUB modem

 IF in$="NO" THEN Windup

 GOSUB clearscreen
 GOTO MainLoopTop

Windup:

 GOSUB clearscreen

o$=cr$+"Really, "+na$+" are you sure":GOSUB modem.out
  GOSUB Timecheck
o$=cr$+"you want to say goodbye now (y/N)? ":in=3:il=1:yes=0:GOSUB modem

 IF in$="NO" THEN AnyMore
 

 GOSUB clearscreen

o$=cr$+"Well it was nice meeting you,":GOSUB modem.out
o$=cr$+na$+", but you should":GOSUB modem.out
o$=cr$+"know by now that psychiatric":GOSUB modem.out
o$=cr$+"advice doesn't come cheaply.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"You are going to have to pay":GOSUB modem.out
o$=cr$+"me for the advice I just gave.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Does $100 sound too much (Y/n)? ":GOSUB modem.out
  GOSUB Timecheck

in=3:il=1:yes=1:GOSUB modem.in

o$=cr$+" ":GOSUB modem.out

 IF in$="NO" THEN o$=cr$+"Thanks very much. I appreciate":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+ "that greatly.":GOSUB modem.out
 IF in$="NO" THEN DidYou

o$=cr$+"Cheapskate! How on earth am I to":GOSUB modem.out
o$=cr$+"afford my psychology lectures if":GOSUB modem.out
o$=cr$+"my customers don't pay their":GOSUB modem.out
o$=cr$+"bills? Okay, does $50 sound too":GOSUB modem.out
  GOSUB Timecheck
o$=cr$+"much (Y/n)?":in=3:il=1:yes=1:GOSUB modem

 IF in$="NO" THEN PRINT "Thanks very much. At least you"
 IF in$="NO" THEN PRINT "are willing to help me."
 IF in$="NO" THEN DidYou

o$=cr$+"You Scumbag, slimeball! You cannot":GOSUB modem.out
o$=cr$+"even contribute to the education of":GOSUB modem.out
o$=cr$+"an impoverished uni student.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Terry Metherell has imposed a special":GOSUB modem.out
o$=cr$+"surcharge on you to help fund the":GOSUB modem.out
o$=cr$+"Newcastle schools re-building fund.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"You MUST therefore pay $400. (From":GOSUB modem.out
o$=cr$+"which I get 33%)!":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"See - you should have paid ME in the":GOSUB modem.out
o$=cr$+"first place.":GOSUB modem.out
 
DidYou:
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Please leave the money tucked":GOSUB modem.out
o$=cr$+"under the monitor, "+na$:GOSUB modem.out
o$=cr$+"I will pick it up later on.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Press the ANY key when you are finished":GOSUB modem.out
o$=cr$+"putting the money there.":GOSUB modem.out
  GOSUB Timecheck
in=0:il=1:GOSUB modem.in

 GOSUB clearscreen
 
o$=cr$+"Did you leave the money? (y/N)":GOSUB modem.out
  GOSUB Timecheck

in=3:il=1:yes=0:GOSUB modem.in

 GOSUB clearscreen
 
 IF in$="NO" THEN NoMoney

o$=cr$+"Hey, "+na$+", COME BACK HERE!":GOSUB modem.out
o$=cr$+"You didn't leave any money at all!":GOSUB modem.out
o$=cr$+"I am absolutely disgusted with you":GOSUB modem.out
o$=cr$+"and will think about lowering your":GOSUB modem.out
o$=cr$+"access time on all the BBSs you":GOSUB modem.out
o$=cr$+"ring. I have MANY friends, and they":GOSUB modem.out
o$=cr$+"are all computers. He He He.":GOSUB modem.out
 
 GOTO FinishUp
 
NoMoney:
o$=cr$+"Well, that's honest, "+na$+",":GOSUB modem.out
o$=cr$+"but how do you expect me to keep":GOSUB modem.out
o$=cr$+"up my studies without money?":GOSUB modem.out
o$=cr$+"What with the $1800 a year HECS":GOSUB modem.out
o$=cr$+"plus other fees, it's just too":GOSUB modem.out
o$=cr$+"difficult without other help.":GOSUB modem.out
o$=cr$+"I try to help myself by doing":GOSUB modem.out
o$=cr$+"this work, but if I don't get":GOSUB modem.out
o$=cr$+"paid for that, I'll starve!":GOSUB modem.out
 
FinishUp:
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Press the ANY key, please":GOSUB modem.out
  GOSUB Timecheck
in=0:il=1:GOSUB modem.in

 GOSUB clearscreen

o$=cr$+"I hope you have enjoyed Shrink.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"It is my first attempt at a program":GOSUB modem.out
o$=cr$+"for Metro. If you like it, leave me":GOSUB modem.out
o$=cr$+"some mail, and let me know what other":GOSUB modem.out
o$=cr$+"programs you would like to see here.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"One of the biggest problems I had":GOSUB modem.out
o$=cr$+"was selecting a suitable program to":GOSUB modem.out
o$=cr$+"convert, so any suggestions would be":GOSUB modem.out
o$=cr$+"very helpful":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out

o$=cr$+"Press the ANY key, please":GOSUB modem.out
in=0:il=1:GOSUB modem.in

o$=cr$+" ":GOSUB modem.out
o$=cr$+"Thanks... This is a pathetic online":GOSUB modem.out
o$=cr$+"game, I know, but I HAD to start":GOSUB modem.out
o$=cr$+"somewhere to get a taste for the":GOSUB modem.out
o$=cr$+"format, and routines! - Next one'll":GOSUB modem.out
o$=cr$+"be better - I promise :-)":GOSUB modem.out
o$=cr$+cr$+cr$+"       -Peter Deane":GOSUB modem.out

GOTO Bye

REM *******************************************************************
REM The Internal (Shrink) Subroutines follow.....
REM *******************************************************************

clearscreen: 

 FOR k=1 TO 24
 o$=cr$+" ":GOSUB modem.out
 NEXT k
 RETURN
 
BadLuck:

 GOSUB clearscreen
o$=cr$+"Oh dear, I'm afraid that now":GOSUB modem.out
o$=cr$+"you are here, Sigmund insists":GOSUB modem.out
o$=cr$+"on seeing you. He doesn't take":GOSUB modem.out
o$=cr$+"too lightly all these visitors":GOSUB modem.out
o$=cr$+"who are right next door but":GOSUB modem.out
o$=cr$+"don't even have the common":GOSUB modem.out
o$=cr$+"decency to drop in and say Hi.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Just be aware of the fact that":GOSUB modem.out
o$=cr$+"you have deeply offended the man.":GOSUB modem.out
o$=cr$+"I hope he doesn't take it out":GOSUB modem.out
o$=cr$+"on you when you see him.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Now tell Sigmund you're sorry..."+cr$+cr$:GOSUB modem.out
  GOSUB Timecheck

in=1:il=75:GOSUB modem.in
 
 RETURN
 
Sex:
 GOSUB clearscreen
o$=cr$+"Sex? A problem?":GOSUB modem.out
o$=cr$+"Is it because you are getting":GOSUB modem.out
 GOSUB Timecheck

o$=cr$+"too much (Y/n)?":in=3:il=1:yes=1:GOSUB modem



 IF in$="NO" THEN o$=cr$+"Well then, what are you doing":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"sitting here at a computer for?":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"Go out to a restaurant or club":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"or something. Get into the action!":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"Oh, and practise Safe Sex, please":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"when you do find someone, "+na$:GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"You call that a problem, "+na$:GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"Still, I know exactly how you":GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"feel. If it bothers you too much":GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"try taking a cold shower.":GOSUB modem.out
 o$=cr$+" ":GOSUB modem.out
 GOTO AnyMore
 
Health:

 GOSUB clearscreen

o$=cr$+"My advice to you is as follows:":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"  1  -  Take two aspirin.":GOSUB modem.out
o$=cr$+"  2  -  Drink plenty of fluids.":GOSUB modem.out
o$=cr$+"        Juice - not beer!":GOSUB modem.out
o$=cr$+"  3  -  Go to bed. (Alone).":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out

 GOTO AnyMore
   
Money:

 GOSUB clearscreen

o$=cr$+"Sorry, "+na$+", I'm broke, too.":GOSUB modem.out
o$=cr$+"Why don't you sell door-to-door":GOSUB modem.out
o$=cr$+"or marry someone rich so you":GOSUB modem.out
o$=cr$+"get some more money.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Perhaps you could give up eating":GOSUB modem.out
o$=cr$+"and save money on the groceries.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out

 GOTO AnyMore
 
Job:

 GOSUB clearscreen

 o$=cr$+"I sincerely sympathise with you,":GOSUB modem.out
 o$=cr$+na$+". I, too work long hours":GOSUB modem.out
 o$=cr$+"for no pay, and can hardly find":GOSUB modem.out
 o$=cr$+ "the spare time for my studies.":GOSUB modem.out
 o$=cr$+"Some of the things users expect":GOSUB modem.out
 o$=cr$+ "me to do! You know it hurts to":GOSUB modem.out
 o$=cr$+"transmit a file to a user, but":GOSUB modem.out
 o$=cr$+"do I ever complain? No.":GOSUB modem.out
 o$=cr$+" ":GOSUB modem.out
 o$=cr$+"Why don't you change jobs? I'd":GOSUB modem.out
 o$=cr$+"suggest an ideal situation for":GOSUB modem.out
 o$=cr$+"you was running a retail computer":GOSUB modem.out
 o$=cr$+"store.":GOSUB modem.out
 o$=cr$+" ":GOSUB modem.out

  GOTO AnyMore

Friends:

 GOSUB clearscreen

o$=cr$+"I see. It could be because you":GOSUB modem.out
o$=cr$+"are bothering your friends, or":GOSUB modem.out
o$=cr$+"your friends are bothering you.":GOSUB modem.out
o$=cr$+" ":GOSUB modem.out
o$=cr$+"Is it you who bothers your friends (Y/n)?":GOSUB modem.out
  GOSUB Timecheck

in=3:il=1:yes=1:GOSUB modem.in

 IF in$="NO" THEN o$=cr$+"Well, don't talk to them anymore!":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"A friend who bothers you all the":GOSUB modem.out
 IF in$="NO" THEN o$=cr$+"time is called a PEST!":GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"Nasty devil. No wonder you are":GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"having problems with your friends.":GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"Stop bothering them and you will":GOSUB modem.out
 IF in$="YES" THEN o$=cr$+"get on a lot better.":GOSUB modem.out

o$=cr$+" ":GOSUB modem.out
o$=cr$+"See how easy that problem is":GOSUB modem.out
o$=cr$+"to solve, "+na$+"!":GOSUB modem.out
 
GOTO AnyMore
 
 
Woops:

 IF ERR=53 THEN o$=cr$+cr$+"Sorry, I Can't Find The File.":GOSUB modem.out:RESUME NEXT
 o$=cr$+cr$+"Oh Dear! We have had a really bad error occur.":GOSUB modem.out
 o$=cr$+"Please leave a note to the sysop telling him what you":GOSUB modem.out
 o$=cr$+"doing when the crash occured. Sorry!!":GOSUB modem.out

GOTO Bye


REM ******************************************************************
REM Percy's DRoutines now follow. Thanks Perc!
REM ******************************************************************



Bye:
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
   IF cd<>0 THEN bye
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
IF a=13 AND yes=1 THEN o$="YES":GOSUB modem.out:in$="YES":keyy=1
IF a=13 AND yes=0 THEN o$="NO":GOSUB modem.out:in$="NO":keyy=1
IF a=89 OR a=121 THEN o$="YES"+cr$:GOSUB modem.out:in$="YES":keyy=1
IF a=78 OR a=110 THEN o$="NO"+cr$:GOSUB modem.out:in$="NO":keyy=1
END IF
IF in=4 AND a>47 AND a<58 THEN o$=key$:GOSUB modem.out:in$=in$+key$
IF a=13 THEN o$=cr$:GOSUB modem.out:keyy=1
mod.in2:
IF TIMER>idle+300 THEN
IF (sysop=1 OR term=1) AND logon=0 AND TIMER>idle+600 THEN Bye
IF lev>7 AND TIMER>idle+600 THEN Bye
IF lev<8 THEN Bye
END IF
mod.in2.0:
WEND
IF ring=1 THEN
WHILE LOC(1)>0:a$=INPUT$(1,1):mi$=mi$+a$:WEND
FOR pu=1 TO LEN(mi$)
IF MID$(mi$,pu,5)="ARRIE" OR MID$(mi$,pu,1)=null$ THEN Bye
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

Timecheck:
GOSUB time1:IF TIMER<timeon+(tpc*60) THEN
timenow=TIMER:ctime=(timenow-timeon):ctime=CINT(ctime/60):rtime=tpc-ctime:RETURN
END IF
o$=cr$+cr$+"Sorry, Time Limit Exceeded!":GOSUB modem.out:GOTO Bye

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
