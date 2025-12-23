/* $VER: bbsMail.rexx 8.5 (18.2.95)
 * Copyright © 1994-95 Richard Lee Stockton
 * BBBBS mail reader/sender with optional file attach
 * FREELY DISTRIBUTABLE
 * Thanks to Matt English for "SendFile.rexx"
 * Thanks to John Ruckart for additional "detail work".
*/

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

CALL TIME('R')
title.=''
title.1='bbsMail for BBBBS'
title.2='Version 8.5'
title.3='18-Feb-95'

OPTIONS RESULTS
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
SIGNAL ON FAILURE
SIGNAL ON SYNTAX

ARG maxtime name pw 
IF ~DATATYPE(maxtime,'N') THEN maxtime=6000

def='[0m'
pen3='[33m'
bak2='[42m'
lineup='1B'x'M'
CR=''
frombb=0
IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END

changed=0
emailonline=0
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
topath='RAM:'
CALL config()
IF name='' THEN
  DO
    OPTIONS PROMPT ' Are you 'sysop'? (Yn) > '
    PULL answer
    IF answer='N' THEN
      DO
        SAY CR
        OPTIONS PROMPT ' Please enter your name > '
       PULL name
        name=cleanstring(name)
        IF name='' THEN EXIT 1 
        IF ~EXISTS(bbspath'Users/'name) THEN
          DO
            SAY name 'does not exist!'CR
            EXIT 1
          END
      END
    ELSE name=sysop
  END
userfile=bbspath'Users/'name
CALL OPEN(f,userfile,'R')
data.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  data.i=line
END
CALL CLOSE(f)
data.0=i-1
password=data.5
linesperpage=data.7
IF ~frombb THEN linesperpage=20
colorflag=1
IF frombb & FIND(data.8,'COLOR')=0 THEN
  DO
    def=''
    pen3=''
    bak2=''
    colorflag=0
  END
clr=''
IF frombb & FIND(data.8,'CLEAR')>0 THEN clr='0C'x
level=data.20

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
          EXIT 1
        END
      passprompt='Incorrect.  Password: ' /* ask again */
    END
    SAY
    SAY' OK, 'name' here we go....'
    SAY
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
IF level>sysoplevel & ~frombb THEN
  DO
    CALL showtext(bbspath'Lists/NEW_USERS')
    CALL showtext(bbspath'Lists/CBV_USERS')
  END
exitarg=0
lm='Loading Module...'lineup||CR
folk.=''
replysubj=''
IF ARG()=0 THEN
  DO
    SAY
    DO i=1 TO 3
      SAY CENTER(title.i,74)
    END
    SAY
  END
CALL readmail()

DONE:
IF emailonline~=0 THEN
  DO
    x=GETCLIP('BBS_email')
    IF ~DATATYPE(x,'N') THEN x=0
    IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_email',x+emailonline)
    IF SHOW('P','BBBBS_LOCAL') THEN CALL SETCLIP('LOCAL_email',x+emailonline)
  END
EXIT exitarg


readmail:
arg=''
x=GETCLIP('BBSMAIL_ARG')
IF x~='' THEN
  DO
    CALL SETCLIP('BBSMAIL_ARG')
    fromenu=WORD(x,1)
    IF WORDS(x)>1 THEN arg=SUBSTR(x,WORDINDEX(x,2))
  END
IF fromenu~=0 THEN fromenu=1
replysubj=''
IF fromenu THEN
  DO
    CALL postuser('Menu')
    temp=UPPER(arg)
    arg=''
    IF temp~='F' & temp~='T' & temp~='W' THEN
      DO
        SAY '                  'lineup||CR
        line='Find Email ['pen3'F'def']rom You ['pen3'T'def']o You or ['pen3'W'def']rite New Email or ['pen3'Q'def']uit (fqTw) > 'def
        temp=getinput(1 1 line)
        CALL cleanline(0)
        IF temp='' THEN temp='T'
      END
    IF temp='W' THEN
      DO
        CALL postuser('Write')
        SAY lm
        CALL bbsWrite.rexx(name maxtime-TRUNC(TIME('E')) 'MAIL' . . 0 0)
        RETURN
      END
    ELSE IF temp='F' THEN
      DO
        CALL postuser('From')
        firsteditline=0
        picklist.=''
        picklist.0=0
        IF getinput(1 1 'Check ALL users? (nY) > ')='N' THEN
          DO
            picklist.1=getinput(1 0 'Check EMail From' name 'To Who? > ')
            picklist.1=SPACE(STRIP(UPPER(picklist.1)),1,'_')
            picklist.1=COMPRESS(picklist.1,'.,:/*#?^ ')
            IF picklist.1='' THEN RETURN
            IF ~EXISTS(bbspath'Users/'picklist.1) THEN
              DO
                SAY '***'pen3 picklist.1 def'does not exist!'||CR
                picklist.0=0
                RETURN
              END
            fmaillist=SHOWDIR(bbspath'EMail/'picklist.1)
            DO ej=1 TO WORDS(fmaillist)
              ejname=WORD(fmaillist,ej)
              uname=ejname
              dot=LASTPOS('.',uname)
              IF dot>2 THEN uname=LEFT(uname,dot-1)
              IF uname=name THEN
                DO
                  arg=bbspath'EMail/'picklist.1'/'ejname
                  IF EXISTS(arg) THEN
                    DO
                      pklst=picklist.0+1
                      picklist.pklst=picklist.1
                      picklist.pklst.0=ejname
                      picklist.0=pklst
                    END
                END
            END
          END
        ELSE
          DO
            SAY 'Loading userlist...'lineup||CR
            userlist=SHOWDIR(bbspath'Users')
            users=WORDS(userlist)
            SAY pen3'Scanning'def users pen3'email directories...'def||CR
            SAY pen3' - To ABORT, press CTRL-E -'def||CR
            DO wi=1 TO users
              CALL busywait(60 wi users)
              fmaillist=SHOWDIR(bbspath'EMail/'WORD(userlist,wi))
              DO ej=1 TO WORDS(fmaillist)
                ejname=WORD(fmaillist,ej)
                uname=ejname
                dot=LASTPOS('.',uname)
                IF dot>2 THEN uname=LEFT(uname,dot-1)
                IF uname=name THEN
                  DO
                    arg=bbspath'EMail/'WORD(userlist,wi)'/'ejname
                    IF EXISTS(arg) THEN
                      DO
                        pklst=picklist.0+1
                        picklist.pklst=WORD(userlist,wi)
                        picklist.pklst.0=ejname
                        picklist.0=pklst
                      END
                  END
              END
              IF wi=999999 THEN RETURN
            END
          END
        CALL busywait(4 0)
        IF picklist.0=0 THEN SAY lineup'No Email FROM you was found.                  'CR
        ELSE
          DO
            SAY pen3'You have the following Email pending:'def||CR
            pickcheck=1
            DO WHILE pickcheck~=0
              pickcheck=pickfromlist()
              IF pickcheck~=0 THEN
                DO
                  firsteditline=5
                  IF level>sysoplevel THEN firsteditline=1
                  CALL bbsEd.rexx(firsteditline bbspath'Email/'picklist.pickcheck'/'picklist.pickcheck.0 name TRUNC(maxtime-TIME('E'))-28)
                  IF ~EXISTS(bbspath'Email/'picklist.pickcheck'/'picklist.pickcheck.0) THEN
                    picklist.pickcheck='- KILLED -'
                END
            END
          END
        RETURN
      END
    ELSE IF temp='T' THEN BREAK
    ELSE RETURN
  END
ELSE IF GETCLIP('BBS_'name)~='' THEN RETURN
sl=1
menuflag=1
CALL MAKEDIR(bbspath'EMail/'name)
mailist=sortnumbers(SHOWDIR(bbspath'Email/'name))
IF frombb THEN
  DO
    sl=GETCLIP('BBSMAIL_letter')
    CALL SETCLIP('BBSMAIL_letter')
    IF sl~='' THEN
      DO
        DO i=1 TO WORDS(mailist)
          IF UPPER(WORD(mailist,i))~=UPPER(sl) THEN ITERATE i
          sl=i
          LEAVE i
        END
      END
    IF ~DATATYPE(sl,'W') THEN sl=1
    ELSE menuflag=0
  END
IF menuflag THEN SAY 'Checking your mailbox...'CR
CALL postuser('Reading')
nomail=1
IF WORDS(mailist)=0 THEN
  DO
    SAY lineup'Your mailbox is empty.  'CR
    SAY CR
    RETURN
  END
IF menuflag THEN
  DO
    line=WORDS(mailist)
    IF line>1 THEN line=line 'letters'
    ELSE line=line 'letter'
    line=line 'waiting.'
    SAY line||CR
    DO ii=1 TO WORDS(mailist)
      SAY 'Email:' pen3||WORD(mailist,ii)||def||CR
    END
  END
IF ~fromenu & menuflag THEN
  IF getinput(1 1 'Read your private mail now? (nY) > ')='N' THEN RETURN
onename=''
IF menuflag & WORDS(mailist)>3 THEN
  DO
    IF getinput(1 1 'Read all private mail? (nY) > ')='N' THEN
      DO
        onename=getinput(1 0 'Read ONLY private mail from? > ')
        onename=SPACE(STRIP(UPPER(onename)),1,'_')
        onename=COMPRESS(onename,'.,:/*#?^ ')
        IF onename='' THEN RETURN
        IF ~EXISTS(bbspath'Users/'onename) & picklist.1~='BBBBS' THEN
          DO
            SAY '***'pen3 onename def'does not exist!'||CR
            RETURN
          END
      END
  END
DO letter=sl TO WORDS(mailist)
  readname=WORD(mailist,letter)
  uname=readname
  dot=LASTPOS('.',uname)
  IF dot>2 THEN uname=LEFT(uname,dot-1)
  IF onename~='' & onename~=uname THEN ITERATE letter
  arg=bbspath'Email/'name'/'readname        /* user has mail! */
  IF readlines(arg 1) THEN ITERATE letter
  delnum=WORD(lynes.1,2)
  CALL seelines(1)
  nomail=0
  nonstop=0
  mailfile=''
  IF UPPER(WORD(lynes.1,3))='FILE:' THEN mailfile=WORD(lynes.1,4)
  IF mailfile~='' & readname~='NEW_FILES' & readname~='FILELISTS_REPORT' & readname~='INACTIVE_USERS' & LEFT(readname,3)~='MSG' THEN
    DO
      IF LEFT(RIGHT(mailfile,4),1)~='.' & LEFT(readname,6)='BBBBS.' THEN
        DO
          SAY CR
          SAY pen3'The attached file is unarchived and may be incomplete.'CR
          SAY 'If the archiver is still building this file, downloading will fail.'def||CR
          IF getinput(1 1 'Do you want to try to download it anyway? (Ny) > ')~='Y' THEN ITERATE letter
          SAY CR
        END
      curdir=PRAGMA('D')
      CALL setdir(bbspath'EmailFiles/'name)
      filesize=WORD(STATEF(mailfile),2)
      IF menuflag THEN
        IF getinput(1 1 ' Attached file:' pen3||mailfile||def 'is' pen3||filesize||def 'bytes.  Download now? (nY) > ')~='N' THEN
          DO
            mf=bbspath'EmailFiles/'name'/'mailfile
            IF frombb THEN
              DO
                CALL SETCLIP('BBSMAIL_letter',readname)
                exitarg=mf
                SIGNAL DONE
              END
            topath=GETCLIP('BBS_topath')
            IF topath='' THEN topath=GETCLIP('BBS_frompath')
            todir=GetFile(150,36,topath,'',' Destination ',,'NOFILES')
            IF RIGHT(todir,1)='/' THEN todir=LEFT(todir,LENGTH(todir)-1)
            IF WORD(STATEF(todir),1)='DIR' THEN
              DO
                IF todir~=topath THEN CALL SETCLIP('BBS_topath',todir)
                SAY 'Copying' mailfile 'to' todir '...'
                ADDRESS COMMAND 'COPY' mf todir 'CLONE'
              END
            ELSE SAY pen3'*'def||todir||pen3'* is not a valid directory!'def
          END
      menuflag=1
      CALL setdir(curdir)
    END
  IF readname~='NEW_FILES' & readname~='FILELISTS_REPORT' & readname~='INACTIVE_USERS' & LEFT(readname,3)~='MSG' & LEFT(readname,6)~='BBBBS.' THEN
    DO
      tempchar='A'
      DO WHILE tempchar='A'
        tempchar=getinput(1 1 '['pen3'A'def']gain  ['pen3'C'def']ontinue  ['pen3'R'def']eply (acR) > ')
        IF tempchar='' THEN tempchar='R'
        IF tempchar='A' THEN CALL seelines(1)
      END
      IF tempchar='R' THEN
        DO
          IF WORDS(lynes.4)<2 THEN replysubj='NONE'
          ELSE replysubj=SUBSTR(lynes.4,WORDINDEX(lynes.4,2))
          SAY lm
          IF bbsWrite.rexx(name maxtime-TRUNC(TIME('E')) 'MAIL' uname bbspath'Email/'name'/'readname 0 0 replysubj)~=0 THEN
            emailonline=emailonline+1
          replysubj=''
        END
    END
  IF LEFT(readname,6)~='BBBBS.' & level>0 THEN
    DO
      tempchar='A'
      DO WHILE tempchar='A'
        tempchar=getinput(1 1 'Forward mail from'pen3 uname def'to other users? (aNy) > ')
        IF tempchar='A' THEN CALL seelines(1)
      END
      IF tempchar='Y' THEN
        DO
          IF getfolk(1 pen3'Forward Email To: 'def)=0 THEN
            DO ei=1 TO folk.0 WHILE folk.ei~=''
              CALL MAKEDIR(bbspath'EMail/'folk.ei)
              forwardarg=bbspath'Email/'folk.ei'/'readname
              ADDRESS COMMAND 'C:COPY' bbspath'Email/'name'/'readname forwardarg
              CALL readlines(forwardarg 1)
              lynes.1=lynes.1'  Forwarded to you by' name TIME('C') DATE()
              CALL DELETE(forwardarg)
              CALL savelines(forwardarg)
              IF WORDS(lynes.2)>3 THEN
                DO
                  forname=bbspath'EmailFiles/'name'/'WORD(lynes.2,4)
                  IF EXISTS(forname) THEN
                    DO
                      CALL MAKEDIR(bbspath'EmailFiles/'folk.ei)
                      ADDRESS COMMAND 'C:COPY' forname bbspath'EmailFiles/'folk.ei
                    END
                END
              line='Mail' pen3||readname||def 'forwarded to' pen3||folk.ei||def
              emailonline=emailonline+1
              CALL send2log(line)
              SAY line||CR
            END
        END
    END
  tempchar=''
  tempstr='Delete the email ('pen3||delnum||def') from'pen3 uname def'that you just read?'
  IF mailfile='' THEN tempchar=getinput(1 1 tempstr '(nqY) > ')
  ELSE
    DO WHILE tempchar~='N' & tempchar~='Q' & tempchar~='Y'
      tempchar=getinput(1 1 tempstr '(nqy) > ')
    END
  IF tempchar='Q' THEN
    DO
      IF getinput(1 1 'Quit reading your Email? (Ny) > ')='Y' THEN
        DO
          readname=''
          uname=''
          RETURN
        END
    END
  ELSE IF tempchar~='N' THEN
    DO
      dirname=bbspath'Email/'name'/'
      nodelete=0
      IF bbsprefs.14=1 & name~=sysop & uname~=sysop & WORD(lynes.2,2)~='BBBBS' & WORD(lynes.2,2)~=sysop & WORD(lynes.3,2)~=sysop THEN
        nodelete=1
      IF nodelete THEN
        ADDRESS COMMAND 'C:Copy' dirname||readname bbspath'Email/'sysop
      ELSE emailonline=emailonline-1
      CALL DELETE(dirname||readname)
      tempstr='Old email'
      IF mailfile~='' & readname~='NEW_FILES' & readname~='FILELISTS_REPORT' & readname~='INACTIVE_USERS' & EXISTS(bbspath'EmailFiles/'name'/'mailfile) THEN
        DO
          IF nodelete THEN
            ADDRESS COMMAND 'C:Copy' bbspath'EmailFiles/'name'/'mailfile bbspath'EmailFiles/'sysop
          CALL DELETE(bbspath'EmailFiles/'name'/'mailfile)
          CALL DELETE(bbspath'EmailFiles/'name'/'mailfile'.xdl')
          tempstr=tempstr 'and attached file'
        END
      tempstr=tempstr 'deleted. Thank you for keeping a clean BBS!'
      SAY tempstr||CR
      IF tempchar='Q' THEN
        IF getinput(1 1 'Quit reading your Email? (Ny) > ')='Y' THEN
          DO
            readname=''
            uname=''
            RETURN
          END
    END
  ELSE IF LEFT(readname,3)='MSG' & level>sysoplevel THEN
    DO
      ii=LEFT(readname,POS('.',readname)-1)
      ii=SUBSTR(ii,4)%1
      IF getinput(1 1 'Move this message back to the' WORD(lynes.5,8) 'conference? (nY) > 'def)~='N' THEN
        DO
          temp=TRANSLATE(readname,'/','.')
          temp=SUBSTR(temp,4)
          lynes.1='!!'STRIP(lynes.1)
          edtype=''
          CALL savelines(msgpath||temp)
          CALL DELETE(bbspath'Email/'name'/'readname)
        END
    END
  ELSE IF LEFT(readname,3)~='MSG' & readname~='NEW_FILES' & readname~='FILELISTS_REPORT' & readname~='INACTIVE_USERS' THEN
    DO
      arg=bbspath'Email/'name'/'readname
      CALL readlines(arg 1)
      IF WORDS(lynes.5)<7 THEN
        DO
          lynes.5=lynes.5'  (Rcvd)' DATE('W') DATE() TIME('C')
          CALL DELETE(arg)
          CALL savelines(arg)
          SAY 'Email has been marked as received.'CR
        END
    END
  CALL checktime()
  readname=''
  uname=''
  arg=''
END
IF nomail THEN
  DO
    SAY 'No mail was found.'CR
    CALL waiting()
  END
CALL setdir(libpath||dirs.1)
RETURN


sortnumbers: PROCEDURE
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
RETURN STRIP(newlist)


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for reading!'
SAY line||CR
RETURN 0


seelines:
DO i=1 TO lynes.0
  SAY lynes.i||def||CR
  IF i//linesperpage=0 THEN
    IF waiting2() THEN LEAVE i
END
nonstop=0
RETURN


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
DO ri=lynes.0 TO 0 BY -1 WHILE LENGTH(lynes.ri)=0 | LEFT(UPPER(lynes.ri),1)='/'
END
lynes.0=ri
RETURN 0


savelines:
PARSE ARG tempname .
ok=OPEN(f,tempname,'W')
IF ok=0 THEN
  DO
    SAY '***' tempname 'failed to open for saving!'CR
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


getfolk:
PARSE ARG startnum selectline
SAY 'Enter a list of comma separated user names'CR
folk.startnum=getinput(1 0 selectline' ')
IF STRIP(folk.startnum)='' THEN RETURN 1
folk.startnum=SPACE(folk.startnum,1,'_')
folk.0=startnum
IF POS(',',folk.startnum)>0 THEN
  DO
    temp=TRANSLATE(folk.startnum,' ',',')
    folk.0=folk.0+WORDS(temp)
    DO ei=startnum TO startnum+WORDS(temp)
      folk.ei=STRIP(WORD(temp,ei))
      IF LEFT(folk.ei,1)='_' THEN
        folk.ei=SUBSTR(folk.ei,2)
    END
  END
DO ei=1 TO folk.0
  folk.ei=check_alias(folk.ei)
  DO WHILE ~EXISTS(bbspath'Users/'folk.ei)
    SAY folk.ei 'not found! Enter that name again or press RETURN.'CR
    folk.ei=getinput(1 0 pen3||selectline' 'def)
    folk.ei=cleanstring(folk.ei)
  END
END
RETURN 0


pickfromlist:
DO pfl=1 TO picklist.0 BY 3
  pfl2=pfl+1
  pfl3=pfl+2
  pfline=pen3||RIGHT(pfl,3)||def LEFT(picklist.pfl,21)
  IF picklist.pfl2~='' THEN
    pfline=pfline pen3||RIGHT(pfl2,3)||def LEFT(picklist.pfl2,21)
  IF picklist.pfl3~='' THEN
    pfline=pfline pen3||RIGHT(pfl3,3)||def LEFT(picklist.pfl3,21)
  SAY pfline||CR
END
emnum=getinput(1 0 pen3'Select Email Number > 'def)
IF ~DATATYPE(emnum,'W') | emnum<1 | emnum>picklist.0 THEN RETURN 0
RETURN emnum


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


cleanstring:
PARSE ARG cstr
cstr=TRANSLATE(cstr,,namemask)
cstr=SPACE(cstr,1,'_')
RETURN cstr


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


check_alias:
ARG ali .
IF ~DATATYPE(Friends.0,'W') THEN RETURN ali
DO ii=1 TO Friends.0
  IF UPPER(WORD(Friends.ii,1))=ali THEN RETURN WORD(Friends.ii,2)
END
RETURN ali


showtext:
PARSE ARG arg .
IF EXISTS(arg) THEN
  DO
    IF readlines(arg 1) THEN RETURN
    CALL seelines(1)
    nonstop=0
    CALL waiting()
  END
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


postuser:
IF ~frombb | ~SHOW('P','BBSPOST') THEN RETURN
PARSE ARG parg 
ptext=GETCLIP('BBSPOST4')
IF WORDS(ptext)>4 THEN ptext=LEFT(ptext,WORDINDEX(ptext,5)-1)
ptext=STRIP(ptext)
ptext=CENTER(ptext'   EMail:' parg,74)
CALL SETCLIP('BBSPOST4',ptext)
ADDRESS BBSPOST 'UPDATE'
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
OPTIONS PROMPT pen3'                       RETURN=Continue  'def
PULL waitchar
CALL cleanline(1)
RETURN


waiting2:
CALL checktime()
IF nonstop=1 THEN RETURN 0
waitchar=getinput(1 1 pen3'   Q=Quit   N=Non-Stop   RETURN=Continue  'def)
IF waitchar='N' THEN
  DO
    nonstop=1
    SAY pen3'To EXIT non-stop scrolling of text, press CTRL-E        'def||CR
    SAY CR
    CALL DELAY(100)
    waitchar=''
  END
CALL cleanline(1)
IF waitchar='Q' THEN RETURN 1
RETURN 0


getinput:
PARSE ARG upflag' 'oneflag' 'pline
CALL checktime()
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
RETURN inarg


checktime:
IF ~frombb THEN RETURN
IF TIME('E')>maxtime THEN EXIT 0
IF TIME('E')>(maxtime-120) THEN SAY '*** Less than 2 minutes left! ***'CR
MSG RIGHT(' ',66-LENGTH(name)) '1B'x'M'||'[30m'||'[41m'||' 'name' level 'level' '||'[0m'
CALL checkdcd()
RETURN


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
IF xmsg~='' THEN
  DO
    SAY CR
    SAY bak2' Message From BBBBS: 'def||CR
    SAY xmsg||CR
    SAY CR
    CALL SETCLIP('BBS_MESSAGE')
    CALL waiting()
  END
IF POS('G',GETCLIP('BBS_COMMAND'))>0 THEN EXIT
RETURN


config:
arg='s:CONFIG.BBS'
IF ~EXISTS(arg) THEN arg='BBS:BBS_TEXT/CONFIG.BBS'
IF readlines(arg 1) THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS_TEXT/CONFIG.BBS are both missing!'CR
    EXIT 1
  END
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
CALL SETCLIP('BBS_bbsname',bbsname)
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
    EXIT 1
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
msgpath=WORD(lynes.7,1)
IF ~EXISTS(msgpath) THEN
  DO
    SAY msgpath 'does not exist!'CR
    EXIT 1
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


send2log:
PARSE ARG sendline
IF ~frombb THEN RETURN
logfile=bbspath'Logs/log.'DATE('S')
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
CALL WRITELN('log','bbsMail:' sendline)
CALL CLOSE('log')
RETURN


BREAK_E:
i=999999
ri=999999
wi=999999
RETURN


BREAK_C:
EXIT 1


FAILURE:
SYNTAX:
lin.1='[36m'ERRORTEXT(RC)'[0m'
lin.2=SIGL-1     SOURCELINE(SIGL-1)
lin.3=SIGL '[36m'SOURCELINE(SIGL)'[0m'
lin.4=SIGL+1     SOURCELINE(SIGL+1)
DO er=1 TO 4
  IF level>sysoplevel | ~frombb THEN SAY 'bbsMail:' lin.er||CR
  IF frombb THEN CALL send2log(lin.er)
END
EXIT 1

/* bbsMail.rexx */
