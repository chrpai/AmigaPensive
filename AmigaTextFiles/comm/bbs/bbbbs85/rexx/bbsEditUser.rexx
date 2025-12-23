/* $VER: bbsEditUser.rexx 8.5 (1.7.95)
 * Copyright © 1995 Richard Lee Stockton
 * Edit userfile or conference access
 * FREELY DISTRIBUTABLE
*/

CALL TIME('R')
OPTIONS RESULTS

SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
SIGNAL ON FAILURE
SIGNAL ON SYNTAX

ARG maxtime name .
IF maxtime='' THEN SAY 'Run this from within bbsMan.rexx!'

def='[0m'
pen3='[33m'
pen6='[36m'
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
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
CALL config()
CALL loaddata()
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
IF colorflag=0 THEN
  DO
    def=''
    pen3=''
    pen6=''
    bak2=''
  END
text.=''
text.1='   Full Name'
text.2='      Street'
text.3='City, ST Zip'
text.4=' Voice Phone'
text.5='    Password'
text.6='    Protocol'
text.7='LinesPerPage'
text.8=' Preferences'
text.9='    Computer'
text.10='   Interests'
text.11='Session Time'
text.12='FirstSession'
text.13='Last Session'
text.14='      UpLoad'
text.15='    Download'
text.16='   Last File'
text.17='Ratio  Email'
text.18='    Winnings'
text.19='       Usage'
text.20='       Level'
text.21='Exclude DIRS'
text.22='   Msgs Read'
text.23='   Msgs Writ'
text.24=' Marked Msgs'
text.25='Marked Files'
text.26='QUICKexclude'
text.27=' CBV numbers'

IF level>0 THEN
  IF getinput(1 1 'Change ['pen3'U'def']ser data or ['pen3'M'def']essage conference access (mU) > ')='M' THEN
    DO
      CALL postuser('Conference Access')
      SAY CR
      SAY pen3'     - Message Conference Access -'def||CR
      SAY '[O]ff turns all message conferences OFF.'CR
      SAY '[R]eset lets you Reset to ''x'' number of messages back.'CR
      SAY 'Set the last message read by you in ALL message conferences'CR
      temp=getinput(1 1 ' ['pen3'F'def']irst  ['pen3'L'def']ast  ['pen3'O'def']ff  ['pen3'R'def']eset  ['pen3'Q'def']uit  (florQ) > ')
      IF POS(temp,'FLOR')=0 THEN EXIT 0
      back=0
      IF temp='R' THEN
        back=getnumber('Set each conference pointer back how many messages?')
      SAY 'Resetting...'lineup||CR
      data.22=''
      DO i=1 TO level
        IF temp='F' THEN num=0
        ELSE IF temp='O' THEN num=-1
        ELSE
          DO
            num=countcheck('Numbers/LastMessage'i 0)-back
            IF num<1 THEN num=0
          END
        data.22=data.22 num
      END
      CALL savedata()
      EXIT 1
    END
CALL postuser('User File')
new=0
change=0
edata.=''
edname=name
DO i=0 TO data.0
  edata.i=data.i
END
num=1
DO WHILE num~='' | edname~=name
  IF num='' | LEFT(num,1)='Q' THEN
    DO
      IF change THEN
        DO
          CALL setdata()
          CALL savedata()
          change=0
        END
      IF new THEN
        DO
          data.=''
          DO i=0 TO edata.0
            data.i=edata.i
          END
          name=edname
          new=0
        END
      CALL setdata()
    END
  maxnum=10
  IF edata.20>sysoplevel THEN maxnum=20
  IF edata.20=99 THEN maxnum=27
  SAY bak2' 'name' 'def||CR
  maxlines=21
  IF maxnum=10 THEN maxlines=20
  DO i=1 TO maxlines
    IF i=5 & name~=edname & edata.20<99 THEN ITERATE
    SAY RIGHT(i,2)||pen3 text.i||def':' data.i||CR
  END
  IF edata.20>sysoplevel THEN
    DO
      line=LEFT(' ',50)
      IF name=edname THEN line=line'NEW = Change User.'
      line=pen3||line||def||lineup
      SAY line||CR
    END
  num=getinput(1 0 'Select Line Number To Edit: ')
  IF num='NEW' & edata.20>sysoplevel & edname=name THEN    /* select a new user */
    DO
      new=1
      IF change THEN
        DO
          CALL setdata()
          CALL savedata()
        END
      change=0
      nufile=bbspath'Lists/NEW_USERS'
      IF OPEN(f,nufile,'R')~=0 THEN
        DO i=1
          SAY READLN(f)||CR
          IF EOF(f) THEN LEAVE i
        END
      CALL CLOSE(f)
      savename=name
      name=getinput(1 0 'New User Name: 'def)
      name=cleanstring(1':'name)
      name=check_alias(name)
      IF loaddata()=0 THEN
        DO
          SAY bbspath'USERS/'name 'failed to open!'CR
          name=savename
        END
      ELSE IF data.20>=edata.20 THEN
        DO
          SAY 'Can''t Edit!' pen3||name def'has an equal or higher level than thee.'
          name=savename
          CALL loaddata()
        END
    END
  ELSE IF DATATYPE(num,'W') & num>0 THEN
    DO
      IF num>maxnum THEN
        DO
          SAY CR
          SAY pen3'You are not authorized to change that information!'def||CR
          SAY CR
        END
      ELSE
        DO dummy=1 TO 1
          IF num=8 THEN
            DO
              SAY CR
              SAY 'Use spaces to separate options.'CR
              SAY 'If the option word is in line 8, it is ON.'CR
              SAY 'Valid Options:'CR
              SAY '        CLEAR  clears screen between pages.'CR
              SAY '        COLOR  turns ANSI color codes ON.'CR
              SAY '        MENU   combines all main commands into 1 menu.'CR
              SAY '        MENUS  splits main commands into 3 menus.'CR
              SAY '        PHONE  makes your phone number public.'CR
              SAY '        QUICK  activates offline options. See bbsQUICK.DOC'CR
              SAY '        STREET makes your street address public.'CR
              SAY '        TERSE  skips some of the logon procedures.'CR
              SAY CR
            END
          line=RIGHT(num,2)||pen3 text.num||def': '
          SAY line||data.num||CR
          temp=getinput(0 0 line)
          IF temp='' THEN
            DO
              IF num=1 | num=4 | num=5 | num=6 | num=7 THEN LEAVE dummy
              IF num=11 | num=12 | num=13 | num=20 THEN LEAVE dummy
            END
          IF num=5 | num=8 THEN temp=UPPER(temp)
          IF num=20 & DATATYPE(temp,'W') & temp>=edata.20 THEN
            temp=data.20
          IF edata.20>sysoplevel & name~=edname THEN line2=name' '
          ELSE line2=''
          IF num=21 & name=edname & edata.20<99 THEN LEAVE dummy
          line=text.num':' data.num pen6'CHANGED TO'def temp
          CALL send2log(line2||line)
          data.num=temp
          SAY line||CR
          SAY CR
          change=1
        END
    END
END
IF change THEN
  DO
    CALL setdata()
    CALL savedata()
  END
EXIT 0


/* functions */

savedata:
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
IF OPEN(f,bbspath'USERS/'name,'W')=0 THEN RETURN
IF data.0<27 THEN data.0=27
DO i=1 TO data.0
  CALL WRITELN(f,data.i)
END
CALL CLOSE(f)
SAY 'User' name 'has been updated.'CR
RETURN


loaddata:
IF name='' THEN RETURN 0
IF OPEN(f,bbspath'USERS/'name,'R')=0 THEN RETURN 0
data.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  data.i=line
END
data.0=i-1
CALL CLOSE(f)

setdata:
level=data.20
password=data.5
IF ~DATATYPE(data.7,'W') THEN data.7=20
IF data.7<5 THEN data.7=5
linesperpage=data.7
IF ~frombb THEN linesperpage=20
IF FIND(UPPER(data.8),'TERSE')>0 THEN terseflag=1
ELSE terseflag=0
IF FIND(UPPER(data.8),'COLOR')>0 THEN colorflag=1
ELSE colorflag=0
IF FIND(UPPER(data.8),'CLEAR')>0 THEN clr='0C'x
ELSE clr=''
RETURN 1


check_alias:
ARG ali .
IF ~DATATYPE(Friends.0,'W') THEN RETURN ali
DO ii=1 TO Friends.0
  IF UPPER(WORD(Friends.ii,1))=ali THEN RETURN WORD(Friends.ii,2)
END
RETURN ali


postuser:
IF ~frombb | ~SHOW('P','BBSPOST') THEN RETURN
PARSE ARG parg 
ptext=GETCLIP('BBSPOST4')
IF WORDS(ptext)>4 THEN ptext=LEFT(ptext,WORDINDEX(ptext,5)-1)
ptext=STRIP(ptext)
ptext=CENTER(ptext'   bbsEditUser:' parg,74)
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
IF waitchar='Q' THEN RETURN 1
RETURN 0


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
IF nflag=2 THEN bot=COMPRESS(bot,'1B'x)
ELSE cstr=strip_ansi(cstr)
top=XRANGE('7F'x)
cstr=COMPRESS(cstr,bot||top)
IF nflag=0 THEN cstr=STRIP(cstr)
RETURN cstr


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


countcheck:
PARSE ARG fname' 'cknum .
fname=bbspath||fname
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
RETURN inarg


getnumber:
PARSE ARG tprompt
tnum=getinput(1 0 '  'tprompt' > ')
mask=COMPRESS(XRANGE(),'0123456789')
tnum=COMPRESS(tnum,mask)
IF ~DATATYPE(tnum,'W') THEN tnum=0
tnum=tnum%1
IF tnum>0 & tnum<10 THEN tnum='0'tnum
RETURN tnum


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
IF OPEN(f,arg,'R')=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS_TEXT/CONFIG.BBS are both missing!'CR
    EXIT 20
  END
lynes.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  lynes.i=line
END
CALL CLOSE(f)
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'CR
    EXIT 1
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
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
CALL WRITELN('log','bbsEditUser:' sendline)
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
  IF level>sysoplevel | ~frombb THEN SAY 'bbsEditUser:' lin.er||CR
  IF frombb THEN CALL send2log(lin.er)
END
EXIT 1

/* bbsEditUser.rexx */
