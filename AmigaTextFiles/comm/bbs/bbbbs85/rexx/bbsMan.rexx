/* $VER: bbsMan.rexx 8.4 (25.1.95)
Copyright © 1994-95 Richard Lee Stockton
Offline (LOCAL) BBBBS Manager
FREELY DISTRIBUTABLE
*/

reader='C:mo'  /* path to YOUR text reader here. I use ppmore */
vers='Version_8.4'

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

NUMERIC DIGITS 14
def='[0m'
pen2='[32m'
pen3='[33m'
lineup='1B'x'M'
figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    EXIT
  END
lynes.=''
DO i=1 TO 40
  lynes.i=READLN(f)
END
CALL CLOSE(f)

compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
compos=POS('/*',lynes.3)
IF compos>0 THEN lynes.3=LEFT(lynes.3,compos-1)
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    EXIT
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
extdevs=''
DO i=1 TO WORDS(lynes.10)
  test=WORD(lynes.10,i)
  IF POS(':',test)=0 THEN ITERATE i
  IF LEFT(test,2)='/*' THEN LEAVE i
  extdevs=STRIP(extdevs test)
END
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

OPTIONS PROMPT' Are you 'sysop'? (Yn) > '
PULL ans
ans=LEFT(STRIP(ans),1)
IF ans='N' THEN
  DO
    SAY
    OPTIONS PROMPT ' Please enter your name > '
    PULL name
    name=cleanstring('1:'name)
    IF name='' THEN EXIT 
    IF ~EXISTS(bbspath'Users/'name) THEN
      DO
        SAY name 'does not exist!'
        EXIT
      END
  END
ELSE name=sysop
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
level=data.20

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
menu.=''
menu.0=vers
menu.1='Doors'
menu.2='Files'
menu.3='Info'
menu.4='Mail'
menu.5='Messages'
menu.6='Users'
tab.0='                   '
tab.2='         '
tab.3='                '
tab.6='             '
CALL bbsspace()
DO FOREVER
  main=0
  CALL header()
  DO i=1 TO 6
    SAY tab.0||i'.' menu.i
  END
  SAY tab.0'Q. Quit'
  main=getinput()
  IF main=1 THEN CALL bbsDoors(3000 name password)
  ELSE IF main=2 THEN
    DO subloop=1
      CALL header()
      SAY tab.2'1. File Manager (Add-Edit-Browse)'
      SAY tab.2'2. Start File Usage Report'
      SAY tab.2'3. External Devices'
      temp=getinput()
      IF temp=1 THEN
        DO
          CALL bbsFile(name password)
          CALL bbsspace()
        END
      ELSE IF temp=2 THEN
        DO
          SAY
          SAY 'Enter -1 (or blank) for no least-downloaded-files report, or'
          OPTIONS PROMPT 'enter smallest least-downloaded filesize to report > '
          PULL filesize
          IF ~DATATYPE(filesize,'W') THEN filesize=-1
          IF filesize<-1 THEN filesize=-1
          SAY
          IF filesize>=0 THEN
            SAY 'Includes least-used files larger than' filesize 'bytes.'
          SAY 'The FILELISTS_REPORT will be left in email for you.'
          SAY
          ADDRESS AREXX bbsREPORT.rexx name 'FILES' filesize
          LEAVE subloop
        END
      ELSE IF temp=3 THEN
        DO
          dl=bbsExtDL.baud(name level 3000 20 1 extdevs)
          IF dl='' THEN BREAK
          topath=GETCLIP('BBS_topath')
          IF topath='' THEN topath=GETCLIP('BBS_frompath')
          todir=GetFile(150,36,topath,'',' Select Directory ',,'NOFILES')
          IF RIGHT(todir,1)='/' THEN todir=LEFT(todir,LENGTH(todir)-1)
          IF WORD(STATEF(todir),1)='DIR' THEN
            DO
              IF todir~=topath THEN CALL SETCLIP('BBS_topath')
              DO i=1 TO WORDS(dl)
                mf=WORD(dl,i)
                SAY 'Copying' mf'...'
                ADDRESS COMMAND 'COPY' mf todir 'CLONE'
              END
            END
          ELSE SAY pen3'*'def||todir||pen3'* is not a valid directory!'def
        END
      ELSE IF temp='Q' | temp='' THEN leave subloop
    END
  ELSE IF main=3 THEN
    DO subloop=1
      CALL header()
      SAY tab.3'1. Daily log'
      SAY tab.3'2. Numbers log'
      SAY tab.3'3. QUICK log'
      SAY tab.3'4. USER log'
      SAY tab.3'5. Information'
      SAY tab.3'6. Scratch:'
      SAY tab.3'7. RAM:'
      SAY tab.3'8. Hourly Usage'
      SAY tab.3'9. BBS Statistics'
      t=''
      temp=getinput()
      IF temp='' | temp='Q' THEN LEAVE subloop
      ELSE IF temp=1 THEN
        DO
          OPTIONS PROMPT '['pen3'RETURN'def']=TODAY, or enter Log Date ('pen3||DATE('S')||def') > '
          PULL t 
          IF t='' THEN t=DATE('S')
          t=bbspath'Logs/log.'t
        END
      ELSE IF temp=2 THEN t=bbspath'Logs/Numbers.log'
      ELSE IF temp=3 THEN t=bbspath'Logs/QUICK.log'
      ELSE IF temp=4 THEN t=bbspath'Usage/USER.LOG'
      ELSE IF temp=5 THEN t=bbspath'Information/'
      ELSE IF temp=6 THEN t=scratch'/'
      ELSE IF temp=7 THEN t='RAM:'
      ELSE IF temp=8 THEN CALL ShowHourly.rexx(name 20 1 0)
      ELSE IF temp=9 THEN CALL bbsSTATS.rexx(name 1 0)
      IF t='' THEN
        DO
          OPTIONS PROMPT pen3'                          Press RETURN 'def
          PULL junk
        END
      ELSE ADDRESS COMMAND reader t
    END
  ELSE IF main=4 THEN
    DO
      CALL SETCLIP('BBSMAIL_ARG','1')
      CALL bbsMail(3000 name password)
      CALL SETCLIP('LOCAL_email')
    END
  ELSE IF main=5 THEN CALL bbsMsg.rexx(3000 name password)
  ELSE IF main=6 THEN
    DO subloop=1
      CALL header()
      SAY tab.6'1. User Info'
      SAY tab.6'2. User Profiles'
      SAY tab.6'3. Friends List'
      SAY tab.6'4. Edit User'
      IF level>sysoplevel THEN
        DO
          SAY tab.6'5. New Users'
          SAY tab.6'6. Change UserName'
          SAY tab.6'7. Start Inactive Users Report'
          SAY tab.6'8. Kill A User'
        END
      temp=getinput()
      IF temp=1 THEN
        CALL bbsOther.rexx(3000 name sysoplevel 1 bbspath bbsname)
      ELSE IF temp=2 THEN CALL bbsProfiles.rexx(name level sysoplevel 20 1 6000 bbspath)
      ELSE IF temp=3 THEN CALL bbsFriends.rexx(name 1)
      ELSE IF temp=4 THEN CALL bbsEditUser.rexx(6000 name)
      ELSE IF temp=5 & level>sysoplevel THEN CALL bbsNewUsers.rexx(name level 1 3000)
      ELSE IF temp=6 & level>sysoplevel THEN
        DO
          SAY
          OPTIONS PROMPT 'Enter Current UserName > '
          PULL oldname
          IF oldname~='' THEN
            DO
              oldname=TRANSLATE(STRIP(oldname),'_',' ')
              IF EXISTS(bbspath'/Users/'name) THEN CALL ChangeUserName(oldname)
              ELSE SAY oldname 'does not have a userfile!'
            END
        END
      ELSE IF temp=7 & level>sysoplevel THEN
        DO
          ADDRESS AREXX bbsREPORT.rexx name 'USERS'
          SAY
          SAY 'INACTIVE_USERS report will be left in email for you.'
          SAY
          LEAVE subloop
        END
      ELSE IF temp=8 & level>sysoplevel THEN CALL bbsKillUser()
      ELSE IF temp='Q' | temp='' THEN leave subloop
    END
  ELSE IF main='Q' THEN EXIT
END
EXIT

cleanstring:
PARSE ARG nflag':'cstr
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
IF nflag=1 THEN
  DO
    cstr=COMPRESS(cstr,"'`")
    cstr=TRANSLATE(cstr,,namemask)
    cstr=SPACE(cstr,1,'_')
    RETURN cstr
  END
bot=XRANGE(,'1F'x)
top=XRANGE('7F'x)
cstr=COMPRESS(cstr,bot||top)
IF nflag=0 THEN cstr=STRIP(cstr)
RETURN cstr


bbsspace:
ADDRESS COMMAND 'C:info >'scratch'/infout' bbsdevice
ok=OPEN(f,scratch'/infout','R')
IF ok=0 THEN RETURN 20
line=READLN(f)
line=READLN(f)
line=READLN(f)
line=READLN(f)
CALL CLOSE(f)
bbsk=WORD(line,4)
IF ~DATATYPE(bbsk,'N') THEN
  DO
    line=bbsdevice 'is not an info compatible device!'
    SAY pen3||line||def||CR
    bbsk=0
    RETURN
  END
IF bbsk<1 THEN bbsk=0
bbsk=bbsk*512
RETURN


header:
SAY def
temp=pen2'BBBBS_Manager'pen3'___'pen2||menu.main
IF main>0 THEN temp=temp'_Menu'
SAY pen3'   'CENTER(temp||pen3,65,'_')||def
SAY pen3'  /                                             \'def
SAY pen3' / 'def||RIGHT(comma(STORAGE()),14) 'bytes available in RAM.       'pen3 '\'def
SAY pen3' \ 'def||RIGHT(comma(bbsk),14) 'bytes available for uploads.  'pen3 '/'def
SAY pen3'  \'RIGHT(name'__',45,'_')'/'def
SAY
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


getinput:
OPTIONS PROMPT LEFT(tab.main,LENGTH(tab.main)-8)||pen3'Select: 'def
PULL inarg
inarg=STRIP(LEFT(inarg,1))
RETURN inarg

/* bbsMan.rexx */
