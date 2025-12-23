/* $VER: bbsWrite.rexx 8.4 (26.1.95)
copyright © 1994-95 Richard Lee Stockton
BBBBS write & setup email or message
FREELY DISTRIBUTABLE
*/

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

OPTIONS RESULTS

SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
SIGNAL ON FAILURE
SIGNAL ON SYNTAX

PARSE ARG name maxtime edtype toname orig msgnum msgdir subj 

CALL TIME('R')
IF toname='.' THEN toname=''
IF orig='.' THEN orig=''
subj=STRIP(subj)
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
lineup='1B'x'M'
CR=''
IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END
ELSE frombb=0
courtesy=''
thechosen.=''
CALL config()
IF ~loaddata() THEN
  DO
    SAY 'Userfile' name 'failed to open for reading!'CR
    EXIT 0
  END
SAY '                    'CR
def='[0m'
pen3='[33m'
bak2='[42m'
IF colorflag=0 THEN
  DO
    def=''
    pen3=''
    bak2=''
  END
Friends.=''
IF OPEN(f,bbspath'Friends/'name,'R')~=0 THEN
  DO
    DO i=1
      Friends.i=READLN(f)
      IF EOF(f) THEN LEAVE i
    END
    Friends.0=i
    CALL CLOSE(f)
  END
IF edtype='MAIL' THEN lastwrit=countcheck(bbspath'Numbers/LastMail 0')
ELSE 
  DO
    IF OPEN(f,bbspath'Lists/Conferences','R')~=0 THEN
      DO
        msg.=''
        DO i=1
          line=READLN(f)
          IF line='END' | EOF(f) THEN LEAVE i
          num=WORD(line,1)
          IF DATATYPE(num,'W') THEN msg.num=WORD(line,2)
          IF num=msgdir THEN LEAVE i
        END
        CALL CLOSE(f)
      END
    IF edtype='MSG' THEN
      DO
        IF msgdir=0 THEN
          IF areaselect() THEN EXIT 0
      END
    lastwrit=countcheck(bbspath'Numbers/LastMessage'msgdir 0)
    IF edinfo(msgpath||msgdir'.txt',msg.msgdir,'Public Message Conference') THEN
      EXIT 0
  END
IF toname='' THEN
  DO
    IF edtype='MAIL' THEN
      DO
        CALL selectchosen(1 pen3'Send PRIVATE' edtype lastwrit+1 'To: 'def)
        toname=thechosen.1
      END
    ELSE toname=getinput(1 0 pen3'Post A PUBLIC Message To: 'def)
  END
toname=SPACE(toname,1,'_')
toname=cleanstring(1':'toname)
toname=check_alias(toname)
IF toname='' | FIND(exclusion,toname)>0 THEN
  DO
    IF toname='' & edtype='MSG' THEN toname='ALL'
    ELSE toname=sysop
    SAY pen3'*** Re-Addressed to'def toname||CR
  END
IF toname~='ALL' THEN
  DO
    IF toname='BBBBS' THEN toname=sysop
    IF ~EXISTS(bbspath'Users/'toname) THEN
      DO
        IF courtesy='' THEN CALL loadcourtesy()
        IF FIND(courtesy,toname)=0 THEN
          DO
            SAY CR
            SAY bak2' 'toname' is not on the user list! 'def||CR
            IF edtype='MAIL' THEN EXIT 0
            ELSE IF getinput(1 1 'Do you want to use it anyway? (nY) > ')='N' THEN EXIT 0
          END
      END
  END
IF frombb THEN
  DO
    IF toname=sysop THEN CALL sound('FEEDBACK')
    ELSE CALL sound('MESSAGE')
  END
IF edtype='MAIL' THEN
  DO
    CALL MAKEDIR(bbspath'EMail/'toname)
    mailname=bbspath'EMail/'toname'/'name'.'lastwrit+1
  END
ELSE
  DO
    CALL MAKEDIR(msgpath||msgdir)
    mailname=msgpath||msgdir'/'lastwrit+1
  END
lynes.=''
lynes.0=6
IF edtype='MAIL' THEN lynes.1=' Mail:' lastwrit+1  /* FILE: filename */
ELSE lynes.1='  Msg:' lastwrit+1          /* Msg: MSG# REPLY # # ... */
lynes.2=' From:' name
IF city~='' THEN lynes.2=lynes.2' - 'city
lynes.3='   To:' toname                       /*  To: toname   MSG # */
IF edtype='MAIL' THEN
  DO
    IF OPEN(f,bbspath||'Users/'toname,'R')~=0 THEN
      DO
        CALL READLN(f)
        CALL READLN(f)
        temp=READLN(f)
        CALL CLOSE(f)
        temp=docity(temp)
        IF temp~='' THEN lynes.3=lynes.3' - 'temp
      END
    IF subj='|@NEW@|' THEN
      DO
        CALL readlines(bbspath'BBS_TEXT/EMAIL_WELCOME' 7)
        subj='Welcome to' bbsname
      END
  END
IF edtype='REPLY' THEN
  DO
    SAY pen3'Subj:'def subj||CR
    temp=getinput(0 0 'Change the current subject? (Ny) > ')
    IF LENGTH(temp)>3 THEN subj=temp
    ELSE IF LEFT(UPPER(temp),1)='Y' THEN subj=''
  END
IF subj='' THEN
  DO
    SAY pen3'Enter the'def 'Subject' pen3'of this message (1 line).'def||CR
    subj=getinput(0 0 pen3': 'def)
    IF level>sysoplevel THEN
      DO
        temp=bbspath'BBS_TEXT/SUBJ.'WORD(subj,1)
        IF EXISTS(temp) THEN
          IF getinput(1 1 'Use SUBJ.'WORD(subj,1)'? (nY) > ')~='N' THEN
            CALL readlines(temp 7)
      END
  END
IF LENGTH(subj)>66 THEN subj=LEFT(subj,66)
IF subj='' THEN subj='?'
lynes.4=' Subj:' subj
lynes.5=' Date:' DATE('W') DATE()'  'TIME('C')
IF edtype~='MAIL' THEN lynes.5=LEFT(lynes.5,39) 'Conference:' msg.msgdir
lynes.6=LEFT('',74,'=')
IF edtype='REPLY' THEN lynes.3=lynes.3'  MSG 'msgnum
DO i=1 TO lynes.0
  SAY lynes.i||CR
END
IF frombb THEN temp='File'
ELSE temp='LOCAL'
CALL writebuffer(scratch'/Message'temp)
CALL DELETE(mailname)
IF savelines(mailname) THEN EXIT 0
CALL seelines(1)
IF thechosen.0='' THEN
  DO
    thechosen.0=1
    thechosen.1=toname
  END
carbons=thechosen.0+1
DO FOREVER
  IF thechosen.0>=carbons THEN
    DO
      junk='Copies To:'
      DO cci=carbons TO thechosen.0
        junk=junk thechosen.cci
      END
      SAY junk||CR
    END
  pline=''
  IF edtype='MAIL' THEN pline='['pen3'C'def']opies'
  pline=STRIP(pline '['pen3'E'def']dit ['pen3'K'def']ill ['pen3'R'def']ead')
  pline=pline '['pen3'U'def']pload-Text ['pen3'S'def']end' edtype'? (ekrSu) 'def
  junk=getinput(1 1 pline)
  IF junk='E' THEN
    DO
      IF level>sysoplevel THEN firstedit=1
      ELSE firstedit=7
      IF bbsEd.rexx(firstedit mailname name TRUNC(maxtime-TIME('E'))-28)=2 THEN EXIT 0
      junk='R'
    END
  ELSE IF edtype='MAIL' & junk='C' THEN
    DO
      CALL selectchosen(carbons pen3'Carbon Copies To: 'def)
      junk='R'
    END
  ELSE IF junk='K' THEN
    DO
      IF DELETE(mailname)=1 THEN SAY edtype 'DELETED.'CR
      EXIT 0
    END
  ELSE IF junk='U' THEN
    DO
      CALL txup(mailname)
      junk='R'
    END
  IF junk='R' THEN
    DO
      CALL readlines(mailname 1)
      CALL seelines(1)
      nonstop=0
    END
  ELSE BREAK
END
IF edtype='MAIL' THEN
  DO
    IF orig~='' & toname~='' THEN
      DO
        IF toname=sysop THEN junk='Y'
        ELSE junk=getinput(1 1 'Attach original mail from' toname'? (nY) > ')
        IF junk~='N' THEN
          IF ~readlines(orig 1) THEN CALL savelines(mailname)
      END
    junk=getinput(1 1 pen3'Attach a file to this message? (Ny) > 'def)
    IF junk='Y' THEN
      DO
        savearg=arg
        arg=''
        IF frombb THEN arg=getinput(0 0 'Filename: ')
        curdir=PRAGMA('D')
        CALL MAKEDIR(bbspath'EmailFiles/'toname)
        CALL setdir(bbspath'EmailFiles/'toname)
        DO WHILE uload()=2
        END
        IF WORD(STATEF(bbspath'EmailFiles/'toname'/'arg),2)>1 THEN
          DO
            CALL readlines(mailname 1)
            IF arg~='' THEN lynes.1=lynes.1'  FILE: 'arg
            CALL setdir(curdir)
            CALL DELETE(mailname)
            CALL savelines(mailname)
          END
        ELSE
          DO
            CALL DELETE(bbspath'EmailFiles/'toname'/'arg)
            SAY pen3'*** Upload failed! ***'def||CR
          END
        arg=savearg
      END
    totmail=WORD(data.17,2)
    IF ~DATATYPE(totmail,'W') THEN totmail=1
    ELSE totmail=totmail+1
    data.17=WORD(data.17,1)'  'totmail'  'WORD(data.17,3)
  END
CALL readlines(mailname 1)
DO ui=1 TO thechosen.0
  IF thechosen.ui='' THEN ITERATE ui
  IF ui>1 THEN
    DO
      CALL MAKEDIR(bbspath'Email/'thechosen.ui)
      newname=bbspath'Email/'thechosen.ui'/'name'.'lastwrit+1
      IF ui<carbons THEN lynes.3='   To:' thechosen.ui
      ELSE
        DO
          lynes.1=lynes.1'  (Carbon Copy)'
          lynes.3='   To:' thechosen.1
        END
      CALL savelines(newname)
      IF WORDS(lynes.1)>3 & EXISTS(bbspath'EmailFiles/'thechosen.1'/'WORD(lynes.1,4)) THEN
        DO
          CALL MAKEDIR(bbspath'EmailFiles/'thechosen.ui)
          ADDRESS COMMAND 'C:COPY' bbspath'EmailFiles/'thechosen.1'/'WORD(lynes.1,4) bbspath'EmailFiles/'thechosen.ui
          line2='Copied' WORD(lynes.1,4)
          SAY line2 'to the' thechosen.ui 'file area.'CR
          CALL send2log(line2)
        END
    END
  line=edtype':'lastwrit+1 'at' TIME('C') 'to' thechosen.ui
  IF edtype~='MAIL' THEN
    DO
      IF EXISTS(bbspath'Users/'thechosen.ui) THEN
        CALL msgmark(thechosen.ui msgdir lastwrit+1)
      line=line 'in' msg.msgdir
    END
  CALL send2log(line)
  line=edtype 'Sent To' thechosen.ui
  l2='in the' msg.msgdir 'conference.'
  IF edtype~='MAIL' THEN line=line l2
  IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=thechosen.ui THEN
    DO
      oldmess=GETCLIP('BBS_MESSAGE')
      IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
      IF edtype='MAIL' THEN l3='You have new Email.'
      ELSE l3='There is a new message addressed to you' l2
      CALL SETCLIP('BBS_MESSAGE',oldmess||l3)
    END
  SAY line||CR
END
IF edtype='MAIL' THEN CALL countcheck(bbspath'Numbers/LastMail' lastwrit+1)
ELSE CALL countcheck(bbspath'Numbers/LastMessage'msgdir lastwrit+1)
CALL setdir(libpath||dirs.1)
CALL savedata()
EXIT 1


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


edinfo:
PARSE ARG t1,t2,t3
IF level<sysoplevel THEN RETURN 0
IF getinput(1 1 'Edit the'pen3 t2 def||t3 'info file? (Ny) > ')='Y' THEN
  DO
    IF ~EXISTS(t) THEN
      DO
        IF OPEN(f,t1,'W')~=0 THEN
          DO
            CALL WRITELN(f,TRIM(CENTER('***'pen3 t2 def||t3 '***',75)))
            CALL WRITELN(f,LEFT('',75,'='))
            CALL CLOSE(f)
            CALL DELAY(28)
          END
      END
    CALL bbsEd.rexx(1 t1 name TRUNC(maxtime-TIME('E'))-28)
    RETURN 1
  END
RETURN 0


areaselect:
SAY pen3||LEFT('-',75,'-')||def||CR
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
DROP smsg.
msgs.0=count%3
IF (count//3)>0 THEN msgs.0=msgs.0+1
DO i=1 TO msgs.0
  DO j=1 TO 2
    k=i+j*msgs.0
    IF k<=count THEN msgs.i=msgs.i msgs.k
  END
END
DO i=1 TO msgs.0
  SAY msgs.i||CR
  IF i//linesperpage=0 & i<msgs.0 THEN CALL waiting()
END
temp=getinput(1 0 pen3'Select Message Conference: 'def)
IF ~DATATYPE(temp,'W') | temp<1 | temp>level | FIND(data.21,temp)>0 THEN RETURN 1
IF msg.temp='' THEN RETURN 1
msgdir=temp
RETURN 0


selectchosen:
PARSE ARG startat selectline
IF startat<2 THEN thechosen.=''
line='Enter list of comma separated user names'
IF level>sysoplevel THEN line=line 'or ALL'
SAY line||CR
thechosen.startat=getinput(1 0 selectline' ')
IF STRIP(thechosen.startat)='' THEN RETURN 1
thechosen.startat=SPACE(thechosen.startat,1,'_')
thechosen.0=startat
IF level>sysoplevel & thechosen.startat='ALL' THEN
  thechosen.startat=SHOWDIR(bbspath'Users','F',',')
IF POS(',',thechosen.startat)>0 THEN
  DO
    temp=TRANSLATE(thechosen.startat,' ',',')
    thechosen.0=thechosen.0+WORDS(temp)-1
    DO ei=1 TO WORDS(temp)
      eii=startat+ei-1
      thechosen.eii=STRIP(WORD(temp,ei))
    END
  END
DO ei=startat TO thechosen.0
  thechosen.ei=check_alias(thechosen.ei)
  DO WHILE ~EXISTS(bbspath'Users/'thechosen.ei)
    IF thechosen.ei~='' THEN
      DO
        IF FIND(exclusion,thechosen.ei)>0 | thechosen.ei='BBBBS' THEN
          DO
            thechosen.ei=sysop
            ITERATE ei
          END
        IF courtesy='' THEN CALL loadcourtesy()
        IF FIND(courtesy,thechosen.ei)>0 THEN ITERATE ei
      END
    SAY thechosen.ei 'not found! Enter that name again or press RETURN.'CR
    thechosen.ei=getinput(1 0 pen3||selectline' 'def)
    IF thechosen.ei='' THEN
      DO
        IF getinput(1 1 'Do you want to see the list of current users? (Ny) > ')='Y' THEN
          IF readlines(bbspath'Lists/USERS' 1)=0 THEN CALL seelines(1)
        ITERATE ei
      END
    thechosen.ei=SPACE(thechosen.ei,1,'_')
  END
END
RETURN 0


loadcourtesy:
IF EXISTS(bbspath'Lists/Courtesy') THEN
  DO
    IF OPEN(f,bbspath'Lists/Courtesy','R') THEN
      DO
        SAY 'Checking Courtesy List...'CR
        DO i=1
          line=READLN(f)
          IF EOF(f) THEN BREAK
          line=cleanstring(1':'line)
          courtesy=courtesy line
        END
        CALL CLOSE(f)
      END
  END
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


seelines:
DO i=1 TO lynes.0
  IF LEFT(lynes.i,2)=': ' & WORDS(lynes.i)=2 THEN ITERATE i
  ELSE IF LEFT(lynes.i,10)='Directory ' | LEFT(lynes.i,5)='=====' THEN
    SAY pen3||lynes.i||def||CR
  ELSE SAY lynes.i||CR
  IF i//linesperpage=0 & i<lynes.0 THEN
    IF waiting2() THEN LEAVE i
END
nonstop=0
RETURN


readlines:
CALL CLOSE(f)
PARSE ARG tempname readstart .
IF OPEN(f,tempname,'R')=0 THEN RETURN 1
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


savelines:
PARSE ARG tempname .
IF EXISTS(tempname) & edtype='MAIL' THEN
  DO
    ok=OPEN(f,tempname,'A')
    IF ok~=0 THEN CALL WRITELN(f,LEFT('',74,'^'))
  END
ELSE ok=OPEN(f,tempname,'W')
IF ok=0 THEN
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


setdir:
PARSE ARG tempdir
CALL PRAGMA('D',STRIP(tempdir))
directory=PRAGMA('D')
IF frombb THEN Data directory
slash=LASTPOS('/',directory)
IF slash=0 THEN slash=LASTPOS(':',directory)
plaindir=directory
IF slash>0 THEN plaindir=SUBSTR(plaindir,slash+1)
RETURN


config:
arg='s:CONFIG.BBS'
IF ~EXISTS(arg) THEN arg='BBS:BBS_TEXT/CONFIG.BBS'
IF readlines(arg 1) THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS_TEXT/CONFIG.BBS are both missing!'CR
    EXIT 0
  END
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
exclusion=STRIP(lynes.3)
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'CR
    EXIT 0
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
msgpath=WORD(lynes.7,1)
IF ~EXISTS(msgpath) THEN
  DO
    SAY msgpath 'does not exist!'CR
    SIGNAL DONE2
  END
testchar=RIGHT(msgpath,1)
IF testchar~='/' & testchar~=':' THEN msgpath=msgpath'/'
msgpath=msgpath'MSG'
SYSTEM_SPACE_LIMIT=WORD(lynes.12,1)
bbsprefs.=''
DO i=16 TO 41
  j=i-15
  bbsprefs.j=STRIP(WORD(lynes.i,1))
END
spellpath=WORD(lynes.9,1)
IF bbsprefs.5 & ~EXISTS(spellpath) THEN
  DO
    SAY spellpath 'does not exist!'CR
    bbsprefs.5=0
  END
IF bbsprefs.10 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'
CALL MAKEDIR(scratch)
RETURN


writebuffer:
PARSE ARG bufname .
IF frombb THEN Capture OFF
CALL DELETE(bufname)
startnum=lynes.0+1
SAY 'Type 'pen3'/E'def' or 'pen3'/S'def' on a new line to Exit and Save.'CR
IF EXISTS(bufname) THEN
  DO
    CALL DELAY(56)
    CALL DELETE(bufname)
    CALL DELAY(56)
  END
IF frombb THEN
  DO
    CaptWrap 74
    Send pen3
    Capture bufname
    Send def
    TimeOut 120
    DO bufloop=1
      Wait '/E,/S,RING,NO CARRIER'
      Status 'L'
      test=LEFT(UPPER(cleanstring(0':'RESULT)),2)
      CALL checkdcd()
      IF test='/E' | test='/S' | test='/X' THEN LEAVE bufloop
    END
    IF test~='/X' THEN Send '\b\b'pen3
    Capture OFF
    CALL checkdcd()
    TimeOut maxidle
    SAY def||CR
    CALL readlines(bufname startnum)
    CALL wrapbuf(startnum)
    QUEUE CR
  END
ELSE
  DO
    DO bufloop=startnum
      PARSE PULL line
      IF LEFT(UPPER(STRIP(line)),2)='/E' | LEFT(UPPER(STRIP(line)),2)='/S' THEN
        LEAVE bufloop
      lynes.bufloop=line
    END
    lynes.0=bufloop-1
    CALL wrapbuf(startnum)
    CALL DELETE(bufname)
    CALL savelines(bufname)
    SAY
  END
RETURN


wrapbuf:
ARG startnum .
CALL cleanline(1)
SAY pen3'Wordwrapping...'def||CR
lynes.startnum=TRANSLATE(lynes.startnum,' ','09'x)
lynes.startnum=cleanstring(2':'lynes.startnum)
DO wi=startnum WHILE wi<=lynes.0
  wj=wi+1
  lynes.wj=TRANSLATE(lynes.wj,' ','09'x)
  lynes.wj=cleanstring(2':'lynes.wj)
  IF LENGTH(lynes.wi)>75 THEN
    DO
      testchar=''
      IF lynes.wj~='' THEN testchar=LEFT(lynes.wj,1)
      IF testchar=' ' | testchar='.' | testchar=':' THEN
        DO
          DO wjj=lynes.0 TO wi+1 BY -1
            wk=wjj+1
            lynes.wk=lynes.wjj
          END
          lynes.wj=''
          lynes.0=lynes.0+1
        END
      DO wl=WORDS(lynes.wi) TO 1 BY -1 WHILE LENGTH(lynes.wi)>74
        IF WORDS(lynes.wi)=1 THEN
          lynes.wi=LEFT(lynes.wi,74) SUBSTR(lynes.wi,75)
        lynes.wj=WORD(lynes.wi,wl) lynes.wj
        lynes.wi=STRIP(DELWORD(lynes.wi,wl,1))
      END
    END
END
RETURN


txup:
PARSE ARG uparg .
IF frombb THEN
  DO
    SAY 'Ready to append' pen3'TEXT ONLY'def 'using'pen3 protocol||def||CR
    pline='Are you SURE your file is un-compressed text? (Ny) > '
    IF getinput(1 1 pline)~='Y' THEN RETURN
  END
savearg=arg
arg='Upload'
arg2='tempfile1'
IF frombb THEN arg=arg'File'
ELSE
  DO
    arg=arg'LOCAL'
    arg2=arg2'LOCAL'
  END
curdir=PRAGMA('D')
CALL setdir(scratch)
CALL DELETE(arg)
CALL DELETE(arg2)
IF uload()=0 THEN
  DO
    ADDRESS COMMAND 'C:copy' uparg scratch'/'arg2 'CLONE'
    CALL DELETE(uparg)
    ADDRESS COMMAND 'C:join' scratch'/'arg2 PRAGMA('D')'/'arg 'AS' uparg
  END
CALL readlines(uparg 1)
CALL setdir(curdir)
arg=savearg
RETURN


chpro:
arg=UPPER(LEFT(arg,1))
IF(arg='') THEN
  DO
    SAY CR
    SAY '['pen3'W'def']- WXModem'CR
    SAY '['pen3'X'def']- XModem-CRC'CR
    SAY '['pen3'K'def']- XModem-1K'CR
    SAY '['pen3'Y'def']- YModem'CR
    SAY '['pen3'G'def']- YModem-G'CR
    SAY '['pen3'Z'def']- ZModem'CR
    SAY CR
    arg=getinput(1 0 STRIP(protocol) '> ')
 END
IF LEFT(UPPER(arg),1)='A' THEN arg='Z'
Set arg
Status Transfer
protocol=STRIP(RESULT)
SAY protocol||CR
RETURN


uload:
CALL bbsspace(12)
SAY CR
IF bbsk<1 THEN
  DO
    line='Upload area is full!'
    CALL send2log(line)
    SAY pen3||line||def||CR
    RETURN 1
  END
IF frombb THEN
  DO
    checkproto='T'
    targ=arg
    DO WHILE checkproto='T'
      arg=''
      SAY CR
      SAY 'Library:'pen3 plaindir def'  Filename:'pen3 targ def'  Protocol:'pen3 protocol||def||CR
      pline=' ['pen3'Q'def']uit ['pen3'T'def']ransfer-protocol'
      pline=pline '['pen3'U'def']pload (qtU) > '
      checkproto=getinput(1 1 pline)
      IF checkproto='Q' THEN RETURN 1
      IF checkproto='T' THEN CALL chpro()
    END
    arg=targ
    CALL sound('UPLOAD')
    uploadtime=TIME('E')
    CALL checktime()
    SAY 'Starting' protocol 'transfer.  Press' pen3'Esc'def 'to abort.'CR
    DownLoad arg
    IF RC>0 THEN RETURN 2
    IF bbsXferStats.baud(14 arg colorflag protocol) THEN RETURN 2
    rbytes=WORD(STATEF(arg),2)
    IF rbytes<1 THEN
      DO
        CALL DELETE(arg)
        RETURN 2
      END
    temp=''
    DO WHILE temp~='N' & temp~='Y'
      temp=getinput(1 1 'Received' rbytes 'bytes. Was your upload successful? (ny) > ')
    END
    IF temp='N' THEN RETURN 2
  END
ELSE
  DO
    frompath=GETCLIP('BBS_frompath')
    IF frompath='' THEN frompath='RAM:'
    fdir=''
    DO loop=1
      fromfile=GetFile(150,36,frompath,'',' Select File to Upload ')
      IF fromfile='' THEN RETURN 1
      IF EXISTS(fromfile) THEN LEAVE loop
      SAY
      SAY fromfile 'does not exist!'
    END
    ADDRESS COMMAND 'C:COPY' fromfile PRAGMA('D') 'CLONE'
    rbytes=WORD(STATEF(fromfile),2)
    x=LASTPOS('/',fromfile)
    IF x=0 THEN x=POS(':',fromfile)
    IF x>0 THEN
      DO
        arg=SUBSTR(fromfile,x+1)
        fdir=LEFT(fromfile,x)
        IF RIGHT(fdir,1)='/' THEN fdir=LEFT(fdir,x-1)
        CALL SETCLIP('BBS_frompath',fdir)
      END
    ELSE arg=fromfile
  END
IF TestArc.rexx(PRAGMA('D')'/'arg)>0 THEN
  DO
    SAY CR
    SAY pen3'***'def arg pen3'failed archive check!'def||CR
    SAY CR
    temp=getinput(1 1 'Do you believe the archive checker made a mistake? (Ny) > ')
    IF temp~='Y' THEN
      DO
        CALL DELETE(arg)
        SAY CR
        RETURN 2
      END
  END
IF ~frombb THEN RETURN 0
CALL bytes2user(14 rbytes)
ADDRESS AREXX bbsNewFile.rexx name PRAGMA('D')'/'arg
IF bbsprefs.9 & name~=sysop THEN
  DO
    newufile=bbspath'EMail/'sysop'/NEW_FILES'
    IF EXISTS(newufile) THEN ok=OPEN(f,newufile,'A')
    ELSE
      DO
        ok=OPEN(f,newufile,'W')
        IF ok~=0 THEN CALL WRITELN(f,'*** New Files ***') 
      END
    IF ok~=0 THEN CALL WRITELN(f,name 'uploaded' plaindir'/'arg'  'DATE() TIME())
    CALL CLOSE(f)
  END
RETURN 0


bytes2user:
PARSE ARG indx bytes .
tfiles=WORD(data.indx,1)
tbytes=WORD(data.indx,3)
IF ~DATATYPE(tfiles,'W') THEN tfiles=0
IF ~DATATYPE(tbytes,'W') THEN tbytes=0
tbytes=tbytes+bytes
tfiles=tfiles+1
IF tfiles>1 THEN data.indx=tfiles 'files' tbytes 'bytes.'
ELSE data.indx='1 file' bytes 'bytes.'
data.indx=data.indx DATE()
CALL savedata(0)
RETURN


bbsspace:
ARG tabspace .
ADDRESS COMMAND 'C:info >'scratch'/infout' bbsdevice
ok=OPEN(f,scratch'/infout','R')
IF ok=0 THEN RETURN 20
line=READLN(f)
line=READLN(f)
line=READLN(f)
line=READLN(f)
CALL CLOSE(f)
IF tabspace<14 THEN SAY CR
bbsk=WORD(line,4)
IF ~DATATYPE(bbsk,'N') THEN
  DO
    line=bbsdevice 'is not an info compatible device!'
    CALL send2log(line)
    SAY pen3||line||def||CR
    bbsk=0
    RETURN
  END
bbsk=bbsk*512-SYSTEM_SPACE_LIMIT
IF bbsk<1 THEN bbsk=0
SAY RIGHT(comma(bbsk),tabspace) 'bytes available for uploads.'CR
RETURN


comma: PROCEDURE
ARG num .
t=''
x=POS('.',num)
IF x>0 THEN t=SUBSTR(num,x)
num=num%1
dgt=LENGTH(num)
numtext=''
IF dgt>3 THEN numtext=','RIGHT(num,3)
IF dgt>6 THEN numtext=','LEFT(RIGHT(num,6),3)||numtext
IF dgt>9 THEN numtext=','LEFT(RIGHT(num,9),3)||numtext
IF dgt>12 THEN
  DO
    numtext=','LEFT(RIGHT(num,12),3)||numtext
    numtext=LEFT(num,dgt-12)||numtext
  END
ELSE IF dgt>9 THEN numtext=LEFT(num,dgt-9)||numtext
ELSE IF dgt>6 THEN numtext=LEFT(num,dgt-6)||numtext
ELSE IF dgt>3 THEN numtext=LEFT(num,dgt-3)||numtext
ELSE numtext=num
RETURN numtext||t


loaddata:
IF name='' THEN RETURN 0
IF OPEN(f,bbspath'USERS/'name,'R')=0 THEN RETURN 0
data.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN BREAK
  data.i=line
END
data.0=i-1
CALL CLOSE(f)
city=docity(data.3)
protocol=data.6
IF ~DATATYPE(data.7,'W') | data.7<5 THEN data.7=20
linesperpage=data.7
IF ~frombb THEN linesperpage=20
IF FIND(UPPER(data.8),'COLOR')>0 THEN colorflag=1
ELSE colorflag=0
clr=''
IF FIND(UPPER(data.8),'CLEAR')>0 THEN clr='0C'x
level=data.20
RETURN 1


savedata:
IF OPEN(f,bbspath'USERS/'name,'W')=0 THEN RETURN
IF data.0<27 THEN data.0=27
DO i=1 TO data.0
  CALL WRITELN(f,data.i)
END
CALL CLOSE(f)
SAY 'User' name 'has been updated.'CR
RETURN


sound:
ARG snd 
IF bbsprefs.13=1 THEN RETURN
ADDRESS AREXX bbsSounds.rexx bbspath'Sounds/' snd 
RETURN


check_alias:
ARG ali .
IF ~DATATYPE(Friends.0,'W') THEN RETURN ali
DO ii=1 TO Friends.0
  IF UPPER(WORD(Friends.ii,1))=ali THEN RETURN WORD(Friends.ii,2)
END
RETURN ali


docity:
PARSE ARG citi
citi=TRANSLATE(citi,'          ','+-.,*/()<>')
DO i=WORDS(citi) TO 1 BY -1
  IF DATATYPE(WORD(citi,i),'N') THEN citi=STRIP(DELWORD(citi,i,1))
  IF UPPER(WORD(citi,i))='USA' THEN citi=STRIP(DELWORD(citi,i,1))
END
citi=SPACE(citi,1)
RETURN STRIP(citi)


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
CALL WRITELN('log','bbsWrite:' sendline)
CALL CLOSE('log')
RETURN


checktime:
IF ~frombb THEN RETURN
IF TIME('E')>maxtime THEN EXIT
IF TIME('E')>(maxtime-120) THEN SAY '*** Less than 2 minutes left! ***'CR
MSG RIGHT(' ',66-LENGTH(name)) '1B'x'M'||'[30m'||'[41m'||' 'name' level 'level' '||'[0m'
CALL checkdcd()
RETURN


cleanline:
ARG lflag .
IF nonstop=0 & clr~='' THEN
  DO
    Send clr
    RETURN
  END
IF colorflag~=1 & lflag=1 THEN RETURN
cline=lineup||LEFT(' ',78)
IF lflag=1 THEN cline=cline||lineup
SAY cline||CR
RETURN


getinput:
PARSE ARG upflag' 'oneflag' 'pline
CALL checkdcd()
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
inarg=cleanstring(0':'inarg)
RETURN inarg


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


countcheck:
PARSE ARG fname' 'cknum .
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
    IF RC=0 THEN EXIT 0
  END
xmsg=GETCLIP('BBS_MESSAGE')
Capture
IF RC=0 & xmsg~='' THEN
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


BREAK_E:
i=999999
ri=999999
wi=999999
RETURN


BREAK_C:
EXIT 0


FAILURE:
SYNTAX:
lin.1='[36m'ERRORTEXT(RC)'[0m'
lin.2=SIGL-1     SOURCELINE(SIGL-1)
lin.3=SIGL '[36m'SOURCELINE(SIGL)'[0m'
lin.4=SIGL+1     SOURCELINE(SIGL+1)
DO er=1 TO 4
  IF level>sysoplevel | ~frombb THEN SAY 'bbsWrite:' lin.er||CR
  IF frombb THEN CALL send2log(lin.er)
END
EXIT 0

/* bbsWrite.rexx */
