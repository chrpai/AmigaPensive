/* $VER: bbsMsg.rexx 8.5 (12.3.95)
copyright © 1990-1995 Richard Lee Stockton
BBBBS offline message conference handler
FREELY DISTRIBUTABLE
*/

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

CALL TIME('R')
OPTIONS RESULTS
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
SIGNAL ON FAILURE
SIGNAL ON SYNTAX

PARSE ARG maxtime name pw 
If ~DATATYPE(maxtime,'N') THEN maxtime=6000

title.=''
title.1='BBBBS Message Handler'
title.2='Version 8.4'
title.3='27-Jan-95'

IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END
ELSE
  DO
    CR=''
    frombb=0
  END

CALL config()
clear_marked=0
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
IF name='' THEN
  DO
    OPTIONS PROMPT ' Are you 'sysop'? (Yn) > '
    PULL answer
    IF answer='N' THEN
      DO
        SAY
        OPTIONS PROMPT ' Please enter your name > '
        PULL name
        name=cleanstring('1:'name)
        IF name='' THEN EXIT 
      END
    ELSE name=sysop
  END
IF ~EXISTS(bbspath'Users/'name) THEN
  DO
    SAY name 'does not exist!'
    EXIT
  END
CALL loaddata()
IF pw~=password THEN
  DO
    passprompt=pen3' Please Enter Password: [30m'
    DO tries=1 TO 3
      OPTIONS PROMPT passprompt
      PULL newpassword
      SAY def
      IF(password=newpassword) THEN LEAVE tries; /* correct password */
      IF tries=3 THEN
        DO
          SAY 
          SAY 'Access terminated.'
          SAY '*** Bad password ***' newpassword '***'
          EXIT
        END
      passprompt='Incorrect.  Password: ' /* ask again */
    END
    SAY
    SAY' OK, 'name' here we go....'
    SAY
  END
msg.=''
IF readopen(bbspath'Lists/Conferences') THEN
  DO
    DO i=1
      line=READLN(f)
      IF line='END' THEN BREAK
      IF EOF(f) THEN BREAK
      num=WORD(line,1)
      IF DATATYPE(num,'W') THEN msg.num=WORD(line,2)
    END
    CALL CLOSE(f)
  END
SAY '                        'lineup||CR
CALL sortconferences()
searcharg=GETCLIP('BBSMSG_SEARCH')
IF searcharg~='' THEN
  DO
    CALL SETCLIP('BBSMSG_SEARCH')
    CALL search()
    EXIT
  END
IF ARG()=0 THEN
  DO
    DO i=1 TO 3
      SAY CENTER(title.i,75)
    END
    SAY CR
  END
CALL readmessages()
EXIT


/* sub-routines */

config:
arg='s:CONFIG.BBS'
IF ~EXISTS(arg) THEN arg='BBS:BBS_TEXT/CONFIG.BBS'
IF readlines(arg 1) THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS_TEXT/CONFIG.BBS are both missing!'CR
    EXIT
  END
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
compos=POS('/*',lynes.3)
IF compos>0 THEN lynes.3=LEFT(lynes.3,compos-1)
exclusion=STRIP(lynes.3)
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'CR
    EXIT
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
msgpath=WORD(lynes.7,1)
IF ~EXISTS(msgpath) THEN
  DO
    SAY msgpath 'does not exist!'CR
    EXIT
  END
testchar=RIGHT(msgpath,1)
IF testchar~='/' & testchar~=':' THEN msgpath=msgpath'/'
msgpath=msgpath'MSG'
DO i=16 TO 41
  j=i-15
  bbsprefs.j=STRIP(WORD(lynes.i,1))
END
IF bbsprefs.10 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'
CALL MAKEDIR(scratch)
IF ~DATATYPE(bbsprefs.16,'W') THEN bbsprefs.16=3
RETURN


sortconferences:
count=0
smsg.=''
DO i=1 TO level
  IF msg.i='' THEN ITERATE i
  count=count+1
  smsg.count=msg.i i
END
smsg.0=count
IF count>0 THEN CALL QSort(1,count,smsg)
count=0
msgs.=''
DO i=1 TO smsg.0
  tempnum=WORD(smsg.i,2)
  tempdir=WORD(smsg.i,1)
  IF FIND(data.21,tempnum)=0 THEN
    DO
      string=' '
      IF tempnum<10 THEN string=string' '
      string=string || tempnum'.'
      IF WORD(data.22,tempnum)='' | WORD(data.22,tempnum)>=0 THEN
        string=string LEFT(tempdir,20)
      ELSE string=string pen3'-OFF-'def LEFT(tempdir,14)
      count=count+1
      msgs.count=string
    END
END
msgs.0=count%3
IF (count//3)>0 THEN msgs.0=msgs.0+1
DO i=1 TO msgs.0
  DO j=1 TO 2
    k=i+j*msgs.0
    IF k<=count THEN msgs.i=msgs.i msgs.k
  END
END
RETURN


areaselect:
SAY pen3||LEFT('-',75,'-')||def||CR
DO i=1 TO msgs.0
  SAY msgs.i||CR
  IF i//linesperpage=0 & i<msgs.0 THEN CALL waiting()
END
SAY pen3||LEFT('-',75,'-')||def||CR
temp=getinput(1 0 pen3'Select Message Conference: 'def)
IF ~DATATYPE(temp,'W') | temp<1 | temp>level | FIND(data.21,temp)>0 THEN RETURN 1
IF msg.temp='' THEN RETURN 1
msgdir=temp
RETURN 0


showmarked:
IF WORDS(data.24)<1 THEN
  DO
    SAY 'You have no marked messages at this time.'CR
    RETURN
  END
fline='These unread conference messages have been ['pen3'M'pen6']arked as addressed to you:'
SAY CR
SAY pen6||fline||def||CR
tempkk=data.24
DO i=1 TO WORDS(tempkk)
  tempk=WORD(tempkk,i)
  PARSE VAR tempk kdir'/'kmsg
  line=RIGHT(kmsg,6) 'in the'pen3 msg.kdir def'conference'
  IF EXISTS(msgpath||tempk) THEN SAY line'.'CR
  ELSE
    DO
      SAY line 'is missing.'CR
      mkw=FIND(data.24,tempk)
      data.24=STRIP(DELWORD(data.24,mkw,1))
      CALL savedata(0)
    END
END
SAY CR
RETURN


readmessages:
x=GETCLIP('BBSMSG_ARG')
IF x~='' THEN
  DO
    CALL SETCLIP('BBSMSG_ARG')
    colorflag=WORD(x,1)
    IF WORDS(x)>1 THEN arg=SUBSTR(x,WORDINDEX(x,2))
    ELSE arg=''
  END
searcharg=''
DO mainloop=1
  SAY CR
  PARSE VAR arg temp' 'arg .
  IF DATATYPE(temp,'W') THEN msgdir=temp
  ELSE IF LEFT(UPPER(temp),1)='A' THEN
    DO
      CALL newmsgs()
      arg=''
      RETURN
    END
  ELSE IF LEFT(UPPER(temp),1)='M' THEN
    DO
      CALL readmarked()
      arg=''
      RETURN
    END
  ELSE
    DO
      CALL showmarked()
      SAY 'Select Message Conference By' pen3'Number'def', ['pen3'M'def']arked only or ['pen3'A'def']ll Active'CR
      IF areaselect() THEN
        DO
          IF LEFT(temp,1)='A' THEN CALL newmsgs()
          IF LEFT(temp,1)='M' THEN CALL readmarked()
          RETURN
        END
    END
  pline='['pen3'A'def']rchive ['pen3'S'def']earch ['pen3'T'def']oggle ON/OFF'
  pline=pline '['pen3'P'def']ost ['pen3'R'def']ead ['pen3'Q'def']uit (apqRst) > '
  IF WORD(data.22,msgdir)<0 THEN
    pline='['pen3'T'def']oggle ON/OFF ['pen3'Q'def']uit (qT) > '
  IF arg~='' THEN junk=UPPER(LEFT(arg,1))
  ELSE junk=getinput(1 1 pline)
  IF WORD(data.22,msgdir)<0 & junk~='Q' THEN junk='T'
  IF junk='Q' THEN RETURN
  IF junk='P' THEN
    DO
      CALL bbsWrite.rexx(name maxtime-TRUNC(TIME('E')) 'MSG' . . 0 msgdir)
      ITERATE mainloop
    END
  IF junk='A' THEN
    DO
      SAY CR
      CALL msgcount(msgdir)
      junk=getinput(1 0 pen3'RETURN'def' to archive new msgs, ['pen3'Q'def']uit, or enter starting message number > ')
      IF junk='Q' THEN RETURN
      IF DATATYPE(junk,'W') THEN
        DO
          IF junk>lastmess | junk<1 THEN junk=1
          lastread.msgdir=junk-1
          CALL savedata(1)
        END
      CALL SETCLIP('BBS_MSGS','ON')
      SAY 'Archiving messages in the'pen3 msg.msgdir def'Conference...'CR
      lastread.msgdir=countcheck('LastMessage'msgdir 0)
      CALL send2log('Arc: ArcMsgs.rexx' msg.msgdir)
      ADDRESS AREXX ArcMsgs.rexx name msgdir
      DO WHILE GETCLIP('BBS_MSGS')~=''
        CALL DELAY(14)
      END
      SAY 'When completed, the archive will be attached to email addressed to you.'CR
      CALL savedata(1)
      SAY CR
      RETURN
    END
  IF junk='S' THEN
    DO
      searcharg=''
      searcharg=getinput(0 0 pen3'Search Phrase: 'def)
      IF LENGTH(STRIP(searcharg))=0 THEN RETURN
      searcharg=COMPRESS(searcharg,'*')
      SAY CR
      CALL searchmsgdir()
      SAY CR
      SAY lineup'All messages in the'pen3 msg.msgdir def'Conference have been searched.'CR
      SAY CR
      CALL waiting()
      searcharg=''
      ITERATE mainloop
    END
  IF junk='T' THEN
    DO
      line='Turning the' msg.msgdir 'conference'
      IF WORD(data.22,msgdir)<0 THEN
        DO
          line=line pen3'ON'def'.'
          newdata='0'
        END
      ELSE
        DO
          line=line pen3'OFF'def'.'
          newdata='-1'
        END
      SAY line||CR
      dataloc=WORDINDEX(data.22,msgdir)-1
      data.22=DELWORD(data.22,msgdir,1)
      IF dataloc>0 THEN data.22=INSERT(newdata' ',data.22,dataloc)
      CALL sortconferences()
    END
  CALL readmsg(0)
  CALL savedata(1)
  nonstop=0
  arg=''
END
RETURN


newmsgs:
test=UPPER(LEFT(arg,1))
IF test='' THEN
  test=getinput(1 1 '['pen3'R'def']ead new messages or ['pen3'A'def']rchive for later download. (aR) > ')
IF test='A' THEN
  DO
    CALL SETCLIP('BBS_MSGS','ON')
    SAY CR
    SAY 'Archiving new conference messages...'CR
    CALL send2log('Arc: ArcMsgs.rexx')
    ADDRESS AREXX ArcMsgs.rexx name
    clear_marked=1
    DO i=1 TO level
      IF WORD(data.22,i)~=-1 THEN
        lastread.i=countcheck('LastMessage'i 0)
    END
    DO WHILE GETCLIP('BBS_MSGS')~=''
      CALL DELAY(14)
    END
    SAY 'When completed, the archive will be attached to email addressed to you.'CR
    CALL savedata(1)
    SAY CR
    RETURN
  END
curmsgdir=msgdir
SAY 'Scanning all Conferences for new messages..'CR
DO newi=1 TO level
  IF msg.newi='' THEN ITERATE newi
  msgdir=newi
  CALL readmsg(1)
  IF msgcom='Q' THEN LEAVE newi
END
CALL savedata(1)
msgdir=curmsgdir
nonstop=0
RETURN


search:
IF searcharg='' THEN searcharg=getinput(0 0 pen3'Search Phrase: 'def)
IF LENGTH(STRIP(searcharg))=0 THEN RETURN
searcharg=COMPRESS(searcharg,'*')
IF getinput(1 1 'Search one conference only? (Ny) > ')='Y' THEN
  DO
    IF areaselect() THEN RETURN
    SAY 'Searching' msg.msgdir 'Message Conference for'pen3 searcharg||def'...'
    SAY
    CALL searchmsgdir()
  END
ELSE
  DO
    SAY 'Searching All Public Message Conferences for'pen3 searcharg||def'...'
    SAY
    DO i=1 TO level
      msgdir=i
      IF msg.msgdir='' | FIND(data.21,msgdir)>0 THEN ITERATE i
      CALL searchmsgdir()
      i=msgdir
      IF msgcom='Q' THEN i=999999
    END
  END
searcharg=''
nonstop=0
SAY CR
IF i<999999 THEN SAY lineup'All available items have been searched.                        'CR
SAY CR
CALL waiting()
RETURN


searchmsgdir:
msglist=SHOWDIR(msgpath||msgdir)
IF WORDS(msglist)>0 THEN SAY lineup||RIGHT(msg.msgdir,40)||CR
CALL postuser('SEARCH:' msg.msgdir 'for' searcharg)
qi=WORDS(msglist)
msglist=sortnumbers(msglist)
DO wi=1 TO qi
  CALL busywait(8 wi qi)
  messnum=WORD(msglist,wi)%1
  IF textsearch(msgpath||msgdir'/'messnum searcharg) THEN
    DO
      CALL busywait(4 0)
      savelast=lastread.msgdir
      CALL readmsg(0 messnum)
      lastread.msgdir=savelast
      IF msgcom='Q' THEN RETURN
      CALL busywait(4 1)
    END
END
CALL busywait(4 0)
RETURN


textsearch:
ARG sfile' 'sarg
IF sarg='' THEN RETURN 0
x=OPEN(f,sfile,'R')
IF x=0 THEN RETURN 0
stemp=UPPER(READCH(f,65000))
CALL CLOSE(f)
retflag=0
IF POS(sarg,stemp)>0 THEN retflag=1
RETURN retflag


sortnumbers:
PARSE ARG slist
IF STRIP(slist)='' THEN RETURN ''
sorted.=''
oldest=999999
newest=0
newlist=''
DO si=1 TO WORDS(slist)
  testword=WORD(slist,si)
  IF ~DATATYPE(testword,'W') THEN
    DO
      testpos=LASTPOS('.',testword)
      IF testpos>0 THEN tempnum=SUBSTR(testword,testpos+1)
      ELSE
        DO
          newlist=testword newlist
          ITERATE si
        END
    END
  ELSE tempnum=testword/1
  IF sorted.tempnum='' THEN
    DO
      sorted.tempnum=testword
      sorted.tempnum.0=1
      IF DATATYPE(tempnum,'W') THEN
        DO
          IF tempnum>newest THEN newest=tempnum
          IF tempnum<oldest THEN oldest=tempnum
        END
    END
  ELSE newlist=newlist testword
END
IF oldest~=999999 & newest~=0 THEN
  DO si=oldest TO newest
    IF sorted.si.0=1 THEN newlist=newlist sorted.si
  END
DROP sorted. oldest newest
RETURN STRIP(newlist)


postuser:
IF ~frombb | bbsprefs.12~=1 | ~SHOW('P','BBSPOST') THEN RETURN
PARSE ARG parg 
ptext=GETCLIP('BBSPOST4')
IF WORDS(ptext)>4 THEN ptext=LEFT(ptext,WORDINDEX(ptext,5)-1)
ptext=STRIP(ptext)
ptext=CENTER(ptext'   'parg,74)
CALL SETCLIP('BBSPOST4',ptext)
ADDRESS BBSPOST 'UPDATE'
RETURN


readmsg:
ARG quietflag marknum .
msgcom=''
IF msg.msgdir='' | FIND(data.21,msgdir)>0 THEN RETURN; /* sysop excluded */
IF WORD(data.22,msgdir)=-1 THEN RETURN;                /*  user excluded */
entering='Entering'pen3 msg.msgdir def'Message Conference..'
IF quietflag=0 & marknum='' THEN SAY entering||CR
IF DATATYPE(WORD(data.22,msgdir),'W') THEN
  lastread.msgdir=WORD(data.22,msgdir)
ELSE lastread.msgdir=0
lstwrt=countcheck('LastMessage'msgdir 0)
frstwrt=countcheck('FirstMessage'msgdir 0)
temp=''
IF marknum='' THEN
  DO
    IF lastread.msgdir>=lstwrt | lastread.msgdir<frstwrt THEN
      DO
        CALL msgcount(msgdir)
        IF quietflag=1 & lastread.msgdir=lstwrt THEN RETURN
        IF nonstop=1 THEN temp=lastread.msgdir
        ELSE temp=getinput(1 0 pen3'Enter starting message number > 'def)
        IF temp='' & lastread.msgdir<lstwrt THEN temp=lastread.msgdir
        IF ~DATATYPE(temp,'W') THEN RETURN
        IF temp<frstwrt THEN temp=frstwrt
        IF temp>lstwrt THEN temp=lstwrt
        IF temp<1 THEN temp=1
        lastread.msgdir=temp-1
      END
  END
ELSE lastread.msgdir=marknum-1
CALL postuser('Reading:' msg.msgdir)
IF quietflag=1 THEN SAY entering||CR
t=msgpath||msgdir'.txt'
IF ~terseflag & marknum='' & EXISTS(t) THEN
  DO
    SAY CR
    CALL readlines(t 1)
    saveflag=nonstop
    CALL seelines(1)
    nonstop=saveflag
    SAY CR
  END
dirname=msgpath||msgdir
msglist.=0 /* set read to 0, unread to 1, and reply >=2 */
firstmess=999999
testlist=SHOWDIR(dirname)
DO i=1 TO WORDS(testlist)
  test=WORD(testlist,i)
  IF test>lastread.msgdir THEN msglist.test=1
  IF test<firstmess THEN firstmess=test
END
IF firstmess=999999 THEN firstmess=0
CALL countcheck('FirstMessage'msgdir firstmess)
msgstatus=1
IF temp='' & marknum='' THEN CALL msgcount(msgdir)
late.=''
late.0=0
skipsubj.=''
skipsubj.0=0
DO msgloop=1
  lastreadnum=lastread.msgdir
  DO WHILE msglist.lastreadnum=0 & lastreadnum<lstwrt
    lastreadnum=lastreadnum+1
  END
  lastread.msgdir=lastreadnum
  IF lastreadnum=lstwrt & msglist.lstwrt=0 THEN LEAVE msgloop
  DO mess=lastread.msgdir TO lstwrt+1
    IF marknum~='' THEN
      DO
        IF mess>marknum THEN LEAVE msgloop
        mess=marknum
      END
    IF msglist.mess~=msgstatus THEN ITERATE mess
    IF msgstatus>1 THEN SAY 'Following the thread, level' msgstatus-1'.'CR
    msglist.mess=0
    arg=dirname'/'mess
    IF ~EXISTS(arg) THEN
      DO
        SAY 'Message number' mess 'is missing.'CR
        ITERATE mess
      END
    IF ~readopen(arg) THEN ITERATE mess
    firstline=READLN(f)
    secondline=READLN(f)
    thirdline=READLN(f)
    forthline=READLN(f)
    CALL CLOSE(f)
    CALL killmark(msgdir mess)
    DO skp=1 TO skipsubj.0
      IF forthline=skipsubj.skp THEN ITERATE mess
    END
    IF WORDS(firstline)>2 THEN /* if replies, change their num to >1 */
      DO
        thread=SUBSTR(firstline,WORDINDEX(firstline,4))
        DO tindx=1 TO WORDS(thread)
          test=WORD(thread,tindx)
          IF msglist.test~=0 THEN msglist.test=msgstatus+1
        END
      END
    ELSE thread=''
    savearg=arg
    msgcom='A'
    DO msgloop2=1 WHILE msgcom='A' | msgcom='O'
      CALL readlines(arg 1)
      IF nonstop=1 THEN rnonstop=1
      ELSE rnonstop=0
      CALL seelines(2)
      IF name=WORD(lynes.3,2) THEN
        DO
          IF WORDS(lynes.3)//2=0 THEN
            DO
              lynes.3=lynes.3'  (Rcvd)'
              CALL savelines(arg)
            END
        END
      msgcom=''
      CALL checktime()
      IF rnonstop THEN
        DO
          SAY CR
          nonstop=1
          msgcom=''
        END
      ELSE
        DO
          pline=''
          IF level<=sysoplevel | WORDS(lynes.3)<4 THEN pline='['pen3'A'def']gain'
          IF level>sysoplevel | name=WORD(lynes.2,2) THEN
            pline=pline '['pen3'E'def']dit ['pen3'K'def']ill'
          IF level>sysoplevel THEN pline=pline '['pen3'M'def']ove'
          IF WORDS(lynes.3)>3 THEN pline=pline '['pen3'O'def']riginal'
          pline=pline '['pen3'N'def']onStop ['pen3'R'def']eply'
          IF level=99 THEN pline=pline '['pen3'!'def']'
          pline=pline '['pen3'S'def']kip ['pen3'Q'def']uit ['pen3'?'def']'
          msgcom=getinput(1 0 STRIP(pline)' > ')
          CALL cleanline(0)
        END
      IF DATATYPE(msgcom,'W') & EXISTS(dirname'/'msgcom) THEN
        DO
          arg=dirname'/'msgcom
          IF msgcom>lastread.msgdir THEN lastread.msgdir=msgcom
          msgcom='A'
          ITERATE msgloop2
        END
      ELSE msgcom=LEFT(msgcom,1)
      IF msgcom='Q' THEN LEAVE msgloop
      ELSE IF msgcom='!' & level>sysoplevel THEN
        DO
          CALL DELETE(arg)
          newchar=LEFT(lynes.1,1)
          IF newchar~='!' THEN newchar='!!'
          ELSE newchar='  '
          lynes.1=OVERLAY(newchar,lynes.1,1,2)
          CALL savelines(arg)
          ITERATE msgloop2
        END
      ELSE IF msgcom='A' THEN ITERATE msgloop2
      ELSE IF msgcom='M' & level>sysoplevel THEN
        DO
          prevmsgdir=msgdir
          If ~areaselect() THEN
            DO
              CALL MAKEDIR(msgpath||msgdir)
              himsg=countcheck('LastMessage'msgdir 0)+1
              lynes.1='  Msg:' himsg
              lynes.3='   To:' WORD(lynes.3,2)
              lynes.5=STRIP(DELWORD(lynes.5,8,1)) msg.msgdir
              nlyn=lynes.0+1
              lynes.0=nlyn
              lynes.nlyn=' *** Moved from the' msg.prevmsgdir 'conference ***'
              CALL savelines(msgpath||msgdir'/'himsg)
              CALL countcheck('LastMessage'msgdir himsg)
              CALL msgmark(WORD(lynes.3,2) msgdir himsg)
              CALL readlines(arg 1)
              CALL DELETE(arg)
              CALL DELAY(28)
              lynes.0=7
              lynes.7='*** Moved to the' msg.msgdir 'conference, message #'himsg' ***'
              CALL savelines(arg)
            END
          msgdir=prevmsgdir
          msgcom='A'
        END
      ELSE IF msgcom='N' THEN
        DO
          nonstop=1
          msgcom=''
        END
      ELSE IF msgcom='H' | msgcom='?' THEN
        DO
          SAY pen3' - HELP with the Read Messages commands -'def||CR
          SAY ' RETURN reads the next message in line.'CR
          SAY ' 34 will read message number 34, if it exists in this conference.'CR
          SAY ' A  reads this message Again (in case it scrolled off screen).'CR
          IF level>sysoplevel | name=WORD(lynes.2,2) THEN
            DO
          SAY ' E  puts this message into the online Editor.'CR
          SAY ' K  deletes a message you wrote. you cannot Kill others!'CR
            END
          IF level>sysoplevel THEN
          SAY ' M  move this message to a new conference.'CR
          SAY ' N  displays all new messages without pausing. CTRL-E to Exit!'CR
          SAY ' O  if this message is a reply, will read the Original message.'CR
          SAY ' R  enters the message editor to Reply to this message.'CR
          SAY ' S  allows you to Skip threads or conferences.'CR
        IF level=99 THEN
          SAY ' !  toggles the do-not-purge! flag for this message.'CR
          SAY ' Q  returns to the message menu. (Quit)'CR
          SAY CR
          CALL waiting()
          msgcom='A'
          IF waitchar='Q' THEN LEAVE msgloop
        END
      ELSE IF msgcom='E' THEN
        DO
          IF level>sysoplevel | name=WORD(lynes.2,2) THEN
            DO
              sline=7
              IF level>sysoplevel THEN sline=1
              CALL bbsEd.rexx(sline arg name maxtime)
              msgcom='A'
            END
        END
      ELSE IF msgcom='S' & mess<lstwrt THEN
        DO
          stemp=''
          req='Skip'
          IF WORDS(lynes.1)>2 THEN req=req 'this ['pen3'T'def']hread or'
          ELSE SAY 'There are no replies to this message.'CR
          req=req 'the entire ['pen3'C'def']onference? ('
          IF WORDS(lynes.1)>2 THEN tst='cqt'
          ELSE tst='cq'
          DO WHILE POS(stemp,UPPER(tst))=0
            stemp=getinput(1 1 req||tst') >')
          END
          IF stemp='T' THEN
            DO
              SAY CR
              SAY pen3 forthline||def||CR
              SAY 'Skipping messages associated with this message...'CR
              SAY CR
              DO i=lastread.msgdir TO lstwrt
                IF msglist.i>1 THEN msglist.i=0
              END
              skipsubj.0=skipsubj.0+1
              sksb=skipsubj.0
              skipsubj.sksb=forthline
            END
          ELSE IF stemp='C' THEN
            DO
              SAY pen3'Skipping to the last message in the'def msg.msgdir pen3'conference.'def||CR
              lastread.msgdir=lstwrt-1
              lw=lstwrt-1
              msglist.lw=0
              msglist.lstwrt=1
              LEAVE mess
            END
        END
      ELSE IF msgcom='K' THEN
        DO
          IF level>sysoplevel | name=WORD(lynes.2,2) THEN
            DO
              IF getinput(1 1 'Really delete' arg'? (Ny) > ')='Y' THEN
                DO
                  IF DELETE(arg)=1 THEN
                    SAY pen3||arg||def' has been deleted.'CR
                  msg.msgdir.0=msg.msgdir.0-1
                END
            END
        END
      ELSE IF msgcom='O' THEN   /* go back and read original */
        DO
          IF WORDS(lynes.3)>3 THEN
            DO
              temp=WORD(lynes.3,4)
              arg=dirname'/'temp
            END
          ELSE SAY 'This is the original message.'CR
        END
      ELSE IF msgcom='R' THEN
        DO
          IF thread~='' & marknum='' THEN
            DO
              li='Read the replies to this message before answering? (nY) > '
              IF getinput(1 1 li)~='N' THEN
                DO
                  n=late.0+1
                  late.0=n
                  late.n=msgstatus
                  late.n.0=arg
                  ITERATE msgloop2
                END
            END
          CALL do_reply(mess)
        END
      ELSE IF arg~=savearg THEN /* Continue */
        DO
          msgcom='A'
          arg=savearg
        END
    END
    IF thread~='' THEN
      DO
        thread=''
        msgstatus=msgstatus+1
      END
  END
  IF msgstatus>0 THEN
    DO
      IF msgstatus>1 THEN msgstatus=msgstatus-1
      CALL do_late()
      CALL postuser('Reading:' msg.msgdir)
    END
END
msgstatus=0
CALL do_late()
DROP msglist. skipsubj. late.
IF quietflag~=1 THEN nonstop=0
RETURN


do_late:
DO lt=1 TO late.0
  IF late.lt='' THEN ITERATE lt
  IF msgstatus<=late.lt THEN
    DO
      SAY CR
      SAY bak2' Reviewing message marked for reply...'def||CR
      late.lt=''
      CALL readlines(late.lt.0 1)
      CALL seelines(2)
      ps='Reply to this message? (nY) > '
      IF getinput(1 1 ps)~='N' THEN CALL do_reply(0)
    END
END
RETURN


docity:
PARSE ARG citi
citi=TRANSLATE(citi,'          ','+-.,*/()<>')
DO i=WORDS(citi) TO 1 BY -1
  IF DATATYPE(WORD(citi,i),'N') THEN citi=STRIP(DELWORD(citi,i,1))
  IF UPPER(WORD(citi,i))='USA' THEN citi=STRIP(DELWORD(citi,i,1))
END
citi=SPACE(citi,1)
RETURN STRIP(citi)


do_reply:
ARG mes
CALL postuser('Writing:' msg.msgdir)
msgnum=WORD(lynes.1,2)
toname=WORD(lynes.2,2)
orig=dirname'/'msgnum
subj=STRIP(SUBSTR(lynes.4,7))
IF getinput(1 1 'Should this reply be private for'pen3 toname def'only? (yN) > ')='Y' THEN
  DO
    comm=name maxtime-TIME('E') 'MAIL' toname orig 0 0 subj
    CALL bbsWrite.rexx(comm)
    RETURN
  END
comm=name maxtime-TIME('E') 'REPLY' toname orig msgnum msgdir subj
IF bbsWrite.rexx(comm) THEN
  DO
    IF EXISTS(orig) THEN
      DO
        IF readlines(orig 1) THEN BREAK
        xmsg=countcheck('LastMessage'msgdir mes)
        IF WORDS(lynes.1)>3 THEN lynes.1=lynes.1 xmsg
        ELSE lynes.1=lynes.1'   Reply' xmsg
        CALL DELAY(28)    /* allow 1/2 sec for read to close */
        CALL savelines(orig)
      END
  END
RETURN


msgmark:
PARSE ARG markname markdir markmsg .
IF OPEN(f,bbspath'Users/'markname,'R')=0 THEN RETURN
mlines.=''
DO mi=1
  temp=READLN(f)
  IF EOF(f) THEN LEAVE mi
  mlines.mi=STRIP(temp)
END
CALL CLOSE(f)
mlines.0=mi-1
CALL DELAY(28)
mlines.24=STRIP(mlines.24 markdir'/'markmsg)
IF OPEN(f,bbspath'Users/'markname,'W')=0 THEN RETURN
DO mi=1 TO mlines.0
  CALL WRITELN(f,mlines.mi)
END
CALL CLOSE(f)
RETURN


checktime:
IF ~frombb THEN RETURN
IF TIME('E')>maxtime THEN EXIT
IF TIME('E')>(maxtime-120) THEN SAY '*** Less than 2 minutes left! ***'CR
MSG RIGHT(' ',66-LENGTH(name)) '1B'x'M'||'[30m'||'[41m'||' 'name' level 'level' '||'[0m'
CALL checkdcd()
RETURN


waiting:
CALL checktime()
IF waitchar='Q' THEN
  DO
    waitchar=''
    RETURN
  END
waitchar=''
IF nonstop=1 THEN RETURN
OPTIONS PROMPT pen3'                          RETURN=Continue 'def
PULL waitchar
CALL cleanline(1)
CALL checkdcd()
RETURN


waiting2:
CALL checktime()
IF nonstop=1 THEN RETURN 0
waitchar=getinput(1 1 pen3'   Q=Quit   N=Non-Stop   RETURN=Continue  'def)
IF waitchar='N' THEN
  DO
    nonstop=1
    SAY lineup||pen3'To EXIT non-stop scrolling of text, press CTRL-E        'def||CR
    SAY CR
    CALL DELAY(99)
    waitchar=''
  END
CALL cleanline(1)
CALL checkdcd()
IF waitchar='Q' THEN RETURN 1
RETURN 0


killmark:
PARSE ARG kdir kmsg .
IF data.24='' THEN RETURN
markword=FIND(data.24,kdir'/'kmsg)
IF markword>0 THEN data.24=STRIP(DELWORD(data.24,markword,1))
RETURN


readmarked:
mrknum=WORDS(data.24)
IF mrknum=0 THEN RETURN
SAY 'Reading only messages addressed to you...'CR
mrklist=data.24
msgcom=''
DO rmki=1 TO mrknum WHILE msgcom~='Q'
  tempk=WORD(mrklist,rmki)
  PARSE VAR tempk mkdir'/'mkmsg .
  IF ~EXISTS(msgpath||tempk) THEN
    DO
      CALL killmark(mkdir mkmsg)
      SAY CR
      SAY 'Message number' mkmsg 'in the' msg.mkdir 'conference is missing!'CR
      SAY CR
      ITERATE rmki
    END
  msgdir=mkdir
  savelast=lastread.msgdir
  CALL readmsg(1 mkmsg)
  IF mkmsg>savelast THEN lastread.msgdir=mkmsg
  ELSE lastread.msgdir=savelast
END
CALL savedata(1)
RETURN


msgcount:
ARG countdir .
lastmess=0
totmsgs=0
unred=0
IF ~EXISTS(msgpath||countdir) THEN RETURN
IF STATEF(msgpath||countdir)=msg.countdir.1 THEN totmsgs=msg.countdir.0
ELSE
  DO
    totmsgs=WORDS(SHOWDIR(msgpath||countdir))
    msg.countdir.0=totmsgs
    msg.countdir.1=STATEF(msgpath||countdir)
  END
IF countdir>level | FIND(data.21,i)>0 THEN RETURN
lastread.countdir=WORD(data.22,countdir)
IF ~DATATYPE(lastread.countdir,'W') THEN lastread.countdir=0
lastmess=countcheck('LastMessage'countdir 0)
IF lastread.countdir<0 THEN RETURN
firstmess=countcheck('FirstMessage'countdir 0)
IF lastread.countdir<firstmess THEN lastread.countdir=firstmess-1
IF lastmess>0 THEN
  IF lastread.countdir>=0 THEN
    DO
      IF lastread.countdir<(firstmess-1) THEN lastread.countdir=firstmess-1
      unred=lastmess-lastread.countdir
      IF unred>totmsgs THEN unred=totmsgs
      cline=RIGHT(unred,5) 'new of' RIGHT(lastmess,5) 'messages,'
      cline=cline RIGHT(totmsgs,5) 'still online in' 
      cline=cline RIGHT(countdir,2)',' msg.countdir
      SAY pen6||cline||def||CR
    END
RETURN


searchmsgdir:
msglist=SHOWDIR(msgpath||msgdir)
IF WORDS(msglist)>0 THEN SAY lineup||RIGHT(msg.msgdir,40)||CR
qi=WORDS(msglist)
DO wi=1 TO qi
  CALL busywait(8 wi qi)
  messnum=WORD(msglist,wi)%1
  IF textsearch(msgpath||msgdir'/'messnum searcharg) THEN
    DO
      CALL busywait(4 0)
      savelast=lastread.msgdir
      CALL readmsg(0 messnum)
      lastread.msgdir=savelast
      IF msgcom='Q' THEN RETURN
      CALL busywait(4 1)
    END
END
CALL busywait(4 0)
RETURN


busywait:
ARG bii bi bt 
IF bii>4 & bi//(10*bii)=0 THEN CALL checkdcd()
IF bbsprefs.21=0 THEN RETURN
IF bi<1 THEN
  DO
    CALL WRITECH(STDOUT,'080808'x)
    IF ni<1 & i>999998 & wi>999998 THEN SAY CR
    RETURN
  END
IF bi=1 THEN CALL WRITECH(STDOUT,'   ')
IF bi//(bii%2)~=0 THEN RETURN
b=bi//bii
IF b=0 | b=bii%2 THEN
  DO
    tp=RIGHT((bi*100)%bt,2)'%'
    CALL WRITECH(STDOUT,'080808'x||tp)
  END
RETURN


cleanline:
ARG lflag .
IF nonstop=0 & clr~='' & frombb THEN
  DO
    Send clr
    RETURN
  END
cline=lineup||LEFT(' ',78)
IF lflag=1 THEN cline=cline||lineup
SAY cline||CR
RETURN


countcheck:
PARSE ARG fname' 'cknum .
fname=bbspath'Numbers/'fname
IF ~EXISTS(fname) THEN
  DO
    IF cknum=0 THEN RETURN 0
    IF OPEN(f,fname,'W')=0 THEN RETURN 0
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN cknum
  END
IF OPEN(f,fname,'R')=0 THEN
  DO
    CALL DELAY(99)
    IF OPEN(f,fname,'R')=0 THEN RETURN cknum
  END
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'W') THEN retval=0
IF ~DATATYPE(cknum,'W') THEN cknum=0
IF retval<cknum THEN
  DO
    IF OPEN(f,fname,'W')~=0 THEN
      DO
        CALL WRITELN(f,cknum)
        CALL CLOSE(f)
        RETURN cknum
      END
  END
RETURN retval


getinput:
PARSE ARG upflag' 'oneflag' 'pline
CALL checktime()
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
inarg=cleanstring(0':'inarg)
RETURN inarg


checkdcd:
IF ~frombb THEN RETURN
dcd
IF RC=0 THEN
  DO
    DO dcds=1 TO 3  /* 5 second delay */
      CALL DELAY(50)
      dcd
      IF RC~=0 THEN RETURN
    END
    dcd
    IF RC=0 THEN EXIT
  END
xmsg=GETCLIP('BBS_MESSAGE')
IF xmsg~='' THEN
  DO
    CALL SETCLIP('BBS_MESSAGE')
    SAY CR
    SAY bak2' Message From BBBBS: 'def||CR
    SAY xmsg||CR
    SAY CR
    CALL waiting()
  END
IF POS('G',GETCLIP('BBS_COMMAND'))>0 THEN EXIT
RETURN


cleanstring:
PARSE ARG nflag':'cstr
IF nflag=1 THEN
  DO
    cstr=COMPRESS(cstr,"'`")
    cstr=TRANSLATE(cstr,,namemask)
    cstr=SPACE(cstr,1,'_')
    RETURN cstr
  END
bot=XRANGE(,'1F'x)
IF nflag=2 THEN bot=COMPRESS(bot,'1B'x)  /* ESC for ANSI */
ELSE cstr=strip_ansi(cstr)
top=XRANGE('7F'x)
cstr=COMPRESS(cstr,bot||top)
IF nflag=0 THEN cstr=STRIP(cstr)
RETURN cstr


do_eleven:
ARG am tc at .
data.11=am 'minutes per call,' tc 'calls per day,'
data.11=data.11 at 'more calls today'
RETURN


savedata:
ARG messflag .
IF data.5='' THEN RETURN
SAY 'Updating...             'lineup||CR
temp=GETCLIP(name'_UPDATE')
IF temp~='' THEN
  DO
    CALL SETCLIP(name'_UPDATE')
    PARSE VAR temp upfiles' 'upbytes' 'upmail' 'upmsg
    IF upfiles>0 THEN
      DO
        files=WORD(data.14,1)
        bytes=WORD(data.14,3)
        IF DATATYPE(files,'W') THEN upfiles=upfiles+files
        IF DATATYPE(bytes,'W') THEN bytes=upbytes
        data.14=upfiles 'files' bytes 'bytes.' DATE()
      END
    IF upmail>0 THEN
      DO
        mail=WORD(data.17,2)
        IF DATATYPE(mail,'W') THEN upmail=upmail+mail
        data.17=WORD(data.17,1) upmail WORD(data.17,3)
      END
    IF upmsg~='' THEN
      DO
        temp=data.23
        DO i=1 TO level
          smsg=WORD(temp,i)
          IF ~DATATYPE(smsg,'W') THEN smsg=0
          IF FIND(upmsg,i) THEN smsg=smsg+1
          data.23=data.23 smsg
        END
      END
  END
SIGNAL OFF BREAK_E
IF frombb THEN
  DO
    Status Trans
    data.6=STRIP(RESULT)
  END
IF lastbrowse>0 THEN
  DO
    IF WORDS(data.16)>1 THEN data.16=DELWORD(data.16,1,1)
    ELSE data.16=DATE('S') TIME()
    data.16=lastbrowse data.16
  END
IF messflag THEN
  DO
    userexclude.=0
    DO si=1 TO WORDS(data.22)
      IF WORD(data.22,si)=-1 THEN userexclude.si=1
    END
    data.22=''
    data.23=''
    DO si=1 TO level
      IF ~DATATYPE(lastread.si,'W') THEN lastread.si=0
      IF userexclude.si THEN data.22=data.22 '-1'
      ELSE data.22=data.22 lastread.si
      IF ~DATATYPE(totwrit.si,'W') THEN totwrit.si=0
      data.23=data.23 totwrit.si
    END
  END
IF clear_marked=1 THEN data.24=''
clear_marked=0
IF writeopen(bbspath'USERS/'name)=0 THEN RETURN
IF data.0<27 THEN data.0=27
DO i=1 TO data.0
  CALL WRITELN(f,data.i)
END
CALL CLOSE(f)
SAY 'User' name 'has been updated.'CR
RETURN


loaddata:
IF name='' THEN RETURN 0
IF ~readopen(bbspath'USERS/'name) THEN RETURN 0
data.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN BREAK
  data.i=line
END
data.0=i-1
CALL CLOSE(f)
city=docity(data.3)
winnings=WORD(data.18,1)
IF ~DATATYPE(winnings,'N') THEN winnings=0
IF WORDS(data.16)<3 THEN data.16='0 19900101 00:00:00'
lastbrowse=WORD(data.16,1)
IF ~DATATYPE(lastbrowse,'W') THEN lastbrowse=0
level=data.20
DO i=1 TO level
  lastread.i=WORD(data.22,i)
  IF ~DATATYPE(lastread.i,'W') THEN lastread.i=0
  totwrit.i=WORD(data.23,i)
  IF ~DATATYPE(totwrit.i,'W') THEN totwrit.i=0
END
password=data.5
IF frombb THEN
  DO
    IF data.6='' THEN
      DO
        Status Trans
        data.6=RESULT
      END
    ELSE
      DO
        IF RIGHT(UPPER(data.6),2)='-G' THEN data.6='G'
        IF RIGHT(UPPER(data.6),3)='-1K' THEN data.6='K'
        IF LEFT(UPPER(data.6),1)='A' THEN data.6='Z'
        IF frombb THEN Set UPPER(LEFT(data.6,1))
      END
  END
IF ~DATATYPE(data.7,'W') THEN data.7=20
IF data.7<5 THEN data.7=5
linesperpage=data.7
IF ~frombb THEN linesperpage=20
IF FIND(UPPER(data.8),'TERSE')>0 THEN terseflag=1
ELSE terseflag=0
IF FIND(UPPER(data.8),'COLOR')>0 THEN colorflag=1
ELSE colorflag=0
CALL colors(colorflag)
clr=''
IF frombb & FIND(UPPER(data.8),'CLEAR')>0 THEN clr='0C'x
menu='ALL'
IF FIND(UPPER(data.8),'MENUS')>0 THEN
  DO
    menuflag=1
    menu='MAIN'
  END
ELSE IF FIND(UPPER(data.8),'MENU')>0 THEN menuflag=1
ELSE menuflag=0
IF level=0 THEN menu='NEW'
IF DATATYPE(WORD(data.11,3),'W') THEN
  DO
    PARSE VAR data.11 amins . atimes .
    CALL do_eleven(amins bbsprefs.16 atimes)
  END
data.21=UPPER(data.21)
RETURN 1


savelines:
PARSE ARG tempname .
IF OPEN(f,tempname,'W')=0 THEN
  DO
    line='***' tempname 'failed to open for saving!'
    CALL send2log(line)
    SAY line||CR
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


seelines:
ARG fancy .
DO i=1 TO lynes.0
  IF fancy=0 THEN SAY lynes.i||def||CR
  ELSE
    DO
      IF LEFT(lynes.i,2)=': ' & WORDS(lynes.i)=2 THEN ITERATE i
      ELSE IF LEFT(lynes.i,10)='Directory ' | LEFT(lynes.i,5)='=====' THEN
        SAY pen3||lynes.i||def||CR
      ELSE SAY lynes.i||CR
      IF fancy=2 & colorflag=1 THEN
        DO
          IF searcharg~='' THEN
            DO
              testpos=POS(UPPER(searcharg),UPPER(lynes.i))
              IF testpos>0 THEN
                SAY LEFT(' ',testpos-1)||pen3||lineup||UPPER(searcharg)||def||CR
            END
          IF i=1 THEN
            IF WORD(lynes.1,3)='Reply' THEN
              DO
                testpos=WORDINDEX(lynes.1,3)
                SAY LEFT(' ',testpos-1)||pen3||lineup||SUBSTR(lynes.1,testpos)||def||CR
              END
        END
    END
  IF i//linesperpage=0 & i<lynes.0 THEN
    IF waiting2() THEN LEAVE i
END
nonstop=0
RETURN


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for writing!'
SAY line||CR
CALL send2log(line)
RETURN 0


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for reading!'
SAY line||CR
CALL send2log(line)
RETURN 0


readlines:
CALL CLOSE(f)
PARSE ARG tempname readstart .
IF ~readopen(tempname) THEN RETURN 1
IF readstart<2 THEN lynes.=''
DO ri=readstart
  line=READLN(f)
  IF EOF(f) THEN BREAK
  lynes.ri=line
END
lynes.0=ri-1
CALL CLOSE(f)
DO ri=lynes.0 TO 0 BY -1 WHILE LENGTH(lynes.ri)=0 | LEFT(UPPER(lynes.ri),2)='/E' | LEFT(UPPER(lynes.ri),2)='/S'
END
lynes.0=ri
RETURN 0


strip_ansi:
PARSE ARG aline 
n=POS('1B'x,aline)
DO WHILE n>0
  DO k=2
    IF DATATYPE(SUBSTR(aline,n+k,1),'M') | (n+k+1)>LENGTH(aline) THEN
      leave k
  END
  aline=DELSTR(aline,n,k+1)
  n=POS('1B'x,aline)
END
RETURN aline


send2log:
PARSE ARG sendline
IF ~frombb THEN RETURN
logfile=bbspath'Logs/log.'DATE('S')    /* daily logs */
fl='W'
IF EXISTS(logfile) THEN fl='A'
IF ~OPEN('log',logfile,fl) THEN
  DO
    IF ~OPEN('log',logfile,fl) THEN
      DO
        SAY 'failed to open log file'
        RETURN
     END
  END
CALL WRITELN('log','bbsMsg:' sendline)
CALL CLOSE('log')
RETURN


colors:
ARG onoff
IF onoff THEN
  DO;def='[0m';pen2='[32m';pen3='[33m';pen6='[36m';bak2='[42m';END
ELSE
  DO;def='';pen2='';pen3='';pen6='';bak2='';END
lineup='1B'x'M'
RETURN


BREAK_E:
i=999999
ri=999999
wi=999999
newi=999999
RETURN


BREAK_C:
EXIT


FAILURE:
SYNTAX:
lin.1='[36m'ERRORTEXT(RC)'[0m'
lin.2=SIGL-1     SOURCELINE(SIGL-1)
lin.3=SIGL '[36m'SOURCELINE(SIGL)'[0m'
lin.4=SIGL+1     SOURCELINE(SIGL+1)
DO er=1 TO 4
  IF level>sysoplevel | ~frombb THEN SAY 'bbsMsg:' lin.er||CR
  IF frombb THEN CALL send2log(lin.er)
END
EXIT

/* bbsMsg.rexx */
