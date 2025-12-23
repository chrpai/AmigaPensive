/* $VER: bbsSTATS.rexx 8.4 (25.1.95)
copyright © 1993-95 Richard Lee Stockton
    - FREELY DISTRIBUTABLE -
*/

SIGNAL ON BREAK_C
NUMERIC DIGITS 14

PARSE SOURCE . . . prg .
ADDRESS AREXX Increment.rexx prg

CR=''
def='[0m'
pen2='[32m'
pen3='[33m'
lineup='1B'x'M'

PARSE ARG name colorflag nonstop emailonline grand grand2 filesonline users .

IF colorflag~=1 THEN
  DO
    def=''
    pen2=''
    pen3=''
  END
IF ADDRESS()='BAUD' THEN CR='0D'x
IF filesonline='' THEN
  DO
    x=STRIP(GETCLIP('BBS_statsarg'))
    CALL SETCLIP('BBS_statsarg')
    IF x='' THEN
      DO
        filesonline='?'
        IF emailonline='' THEN emailonline='?'
      END
    ELSE PARSE VAR x emailonline grand grand2 filesonline .
  END
IF grand='GRAND' | grand='' THEN grand='?'

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    EXIT 21
  END
lynes.=''
DO i=1 TO 33
  lynes.i=READLN(f)
END
CALL CLOSE(f)

compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname = STRIP(lynes.1)
sysop=WORD(lynes.2,1)
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    EXIT 22
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
SYSTEM_SPACE_LIMIT=WORD(lynes.12,1)
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

x=OPEN(f,bbspath'Users/'name,'R')
IF x=0 THEN
  DO
    CALL DELAY(150)
    x=OPEN(f,bbspath'Users/'name,'R')
    IF x=0 THEN
      DO
        SAY name 'user file is missing!'
        EXIT 23
      END
  END
data.=''
DO i=1 TO 25
  data.i=READLN(f)
END
CALL CLOSE(f)
lpp=data.7%1
IF CR='' THEN lpp=20
level=data.20%1
lastread.=0

IF nonstop='' THEN
  DO
    nonstop=1
    CALL CLOSE(STDOUT)
    CALL OPEN(STDOUT,bbspath'EmailFiles/'name'/Notices/STATS.BBS','W')
  END

x=GETCLIP('BBS_'name'_22')
IF x~='' THEN
  DO
    data.22=x
    CALL SETCLIP('BBS_'name'_22')
  END

SAY GETCLIP('BBS_copyright')||CR
CALL SETCLIP('BBS_copyright')

IF grand2='GRAND2' | grand2='' THEN
  DO
    grand2=0
    DO i=1 TO 99
      grand2=grand2+countcheck('Numbers/LastMessage'i 0)
    END
  END

prevcaller=GETCLIP('BBS_prevcaller')
IF prevcaller~='' THEN
  DO
    SAY 'The previous'pen3 bbsname def'user was:'CR
    SAY ' 'prevcaller||CR
    SAY '  logged off at:' GETCLIP('BBS_userlogoff')||CR
    SAY pen3'Last disconnect:'def GETCLIP('BBS_disconnect')||CR
  END
SAY CR
day1='01'
IF readopen(bbspath'Numbers/FirstLogon') THEN
  DO
    line=READLN(f)
    CALL CLOSE(f)
    SAY 'The First Logon to'pen3 bbsname def'was' line'.'CR
    PARSE VAR line .' 'day1'-'.
  END
IF day1<10 & LENGTH(day1)<2 THEN day1='0'day1
SAY '     Your sysop is' pen3||sysop||def||CR
SAY CR
usagelist=SHOWDIR(bbspath'Usage','F')
tempnum=FIND(usagelist,'USER.LOG')
IF tempnum>0 THEN usagelist=DELWORD(usagelist,tempnum,1)
usagelist=sortnumbers(usagelist)
SAY pen3'             - Total BBS Usage -'def||CR
DO i=1 TO WORDS(usagelist)
  dateclip=STRIP(WORD(usagelist,i))
  usageclip=countcheck('Usage/'dateclip 0)
  usageclp=usageclip%60 usageclip//60
  mtime=30*23*60  /* we guess 1 hour a day for various maintenance */
  IF i=1 THEN
    DO
      mtime=mtime-(day1-1)*23*60
      day1=dateclip||day1
    END
  ELSE IF LEFT(DATE('S'),6)=dateclip THEN mtime=WORD(DATE(),1)*23*60
  dateclip=dateclip'01'
  line=RIGHT(DATE('M',dateclip,'S'),10) WORD(DATE(,dateclip,'S'),3)':'
  line=line RIGHT(WORD(usageclp,1),3) 'hours' RIGHT(WORD(usageclp,2),2)
  line=line 'minutes  = ' RIGHT(((usageclip*100)/mtime+.5)%1,2)'% usage.'
  SAY line||CR
  IF (i+12)//(lpp-3)=0 THEN
    IF waiting2() THEN LEAVE i
END
cmin=countcheck('Numbers/Minutes' 0)
chr=cmin%60
cmin=cmin//60
hrz=chr
daz=1
IF hrz<1 THEN hrz=1
IF day1>19900101 THEN
  DO
    daz=1+DATE('I')-DATE('I',day1,'S')
    hrz=daz*24
  END
SAY CR
SAY '     Total Connect Time Since First Logon [all users]:'CR
SAY RIGHT(comma(chr),20) 'hours' RIGHT(cmin,2) 'minutes  = ' RIGHT(((chr*100)/hrz+.5)%1,2)'% usage.'CR
calls=countcheck('Numbers/Calls' 0)
IF calls<1 THEN calls=1
SAY RIGHT(comma(calls),20) 'completed calls.' RIGHT(TRUNC((calls/daz)+.05,1),8) 'calls per day.'CR
SAY CR
calz.=''
CALL FileList(bbspath'Numbers/Calls?*',calz,'F','N')
calz.0.0=0
calz.0.1=0
calz.0.2=0
DO i=1 TO calz.0
  calz.i.0=countcheck('Numbers/'calz.i 0)
  calz.i.2=SUBSTR(calz.i,6)
  calz.0.0=calz.0.0+calz.i.0
  calz.i.1=countcheck('Numbers/Minutes'calz.i.2 0)
  calz.0.1=calz.0.1+calz.i.1
END
SAY LEFT('',19) pen2'baudrate   calls  minutes'def||CR
SAY LEFT('',19) pen2'--------  ------  -------'def||CR
used=''
DO j=1 TO calz.0
  hi=0
  DO i=1 TO calz.0
    IF calz.i.2>calz.hi.2 & FIND(used,calz.i.2)=0 THEN hi=i
  END
  IF hi>0 THEN
    DO
      used=used calz.hi.2
      temp=RIGHT(calz.hi.2,27) RIGHT(TRUNC(((calz.hi.0*100)/calz.0.0+.005),2),7)'%'
      temp=temp                RIGHT(TRUNC(((calz.hi.1*100)/calz.0.1+.005),2),7)'%'
      SAY temp||CR
    END
END
SAY CR
CALL waiting2()
IF waitchar='Q' THEN RETURN
CALL bbsspace(15)
SAY CR
IF extdevs~='' THEN CALL showxdevs()
SAY RIGHT(comma(countcheck('Numbers/Bytes.DownLoad' 0)),15) 'bytes  in' RIGHT(comma(countcheck('Numbers/Files.DownLoad' 0)),7) 'files downloaded.'CR
SAY CR
SAY RIGHT(comma(countcheck('Numbers/Bytes.UpLoad' 0)),15) 'bytes uploaded.'CR
SAY RIGHT(comma(filesonline),15) 'online of' RIGHT(comma(countcheck('Numbers/LastFile' 0)),7) 'public files.'CR
IF emailonline<0 THEN CALL countmail()
SAY RIGHT(comma(emailonline),15) 'online of' RIGHT(comma(countcheck('Numbers/LastMail' 0)),7) 'private messages.'CR
SAY RIGHT(comma(grand),15) 'online of' RIGHT(comma(grand2),7) 'public messages.'CR
IF users<1 THEN
  DO
    IF OPEN(f,bbspath'Lists/USERS','R')=0 THEN
      users=WORDS(SHOWDIR(bbspath'USERS','F'))
    ELSE
      DO
        users=0
        DO i=1
          dat.i=READCH(f,65000)
          IF EOF(f) THEN LEAVE i
          users=users+WORDS(dat.i)
        END
        CALL CLOSE(f)
      END
  END
SAY RIGHT(comma(users),15) 'active of' RIGHT(comma(countcheck('Numbers/Users' 0)),7) 'user applications.'CR
SAY RIGHT(comma(countcheck('Numbers/QUICKIN' 0)),15) 'QUICKIN &' RIGHT(comma(countcheck('Numbers/QUICKOUT' 0)),7) 'QUICKOUT files processed.'CR
SAY CR
SAY 'Your access level is 'level'  -  minimum sysop level is' sysoplevel||CR
SAY CR
SAY '  You Have'CR
totmail=WORD(data.17,2)
IF ~DATATYPE(totmail,'W') THEN totmail=0
totmsg=0
DO ti=1 TO level
  temp=WORD(data.23,ti)
  IF DATATYPE(temp,'W') THEN totmsg=totmsg+WORD(data.23,ti)
END
SAY '   Written' RIGHT(comma(totmsg),14)'  public &' RIGHT(comma(totmail),8)'  private messages.'CR
totfiles=WORD(data.14,1)
IF ~DATATYPE(totfiles,'W') THEN totfiles=0
totbytes=WORD(data.14,3)
IF ~DATATYPE(totbytes,'W') THEN totbytes=0
SAY '  Uploaded' RIGHT(comma(totbytes),14)'  bytes in' RIGHT(comma(totfiles),8)'  files.'CR
totfiles=WORD(data.15,1)
IF ~DATATYPE(totfiles,'W') THEN totfiles=0
totbytes=WORD(data.15,3)
IF ~DATATYPE(totbytes,'W') THEN totbytes=0
SAY 'Downloaded' RIGHT(comma(totbytes),14)'  bytes in' RIGHT(comma(totfiles),8)'  files.'CR
PARSE VAR data.19 dhour' hours 'dmin' minutes in 'calls .
IF ~DATATYPE(dhour,'W') THEN dhour=0
IF ~DATATYPE(dmin,'W') THEN dmin=0
IF ~DATATYPE(calls,'W') THEN calls=0
SAY '..and been on' bbsname dhour 'hours' dmin+TIME('E')%60 'minutes in' calls+1 'calls.'CR

BREAK_C:
EXIT


waiting2:
IF nonstop=1 THEN RETURN 0
waitchar=getinput(1 1 pen3'   Q=Quit   N=Non-Stop   RETURN=Continue  'def)
IF waitchar='N' THEN
  DO
    nonstop=1
    waitchar=''
  END
CALL cleanline(1)
IF waitchar='Q' THEN RETURN 1
RETURN 0


cleanline:
ARG lflag .
IF colorflag~=1 & lflag=1 THEN RETURN
cline=lineup||LEFT(' ',78)
IF lflag=1 THEN cline=cline||lineup
SAY cline||CR
RETURN


getinput:
PARSE ARG upflag' 'oneflag' 'pline
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
RETURN inarg


countcheck:
PARSE ARG fname' 'cknum .
fname=bbspath||fname
IF ~EXISTS(fname) THEN
  DO
    IF cknum=0 THEN RETURN 0
    IF ~writeopen(fname) THEN RETURN 0
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN cknum
  END
IF ~readopen(fname) THEN RETURN cknum
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'W') THEN retval=0
IF ~DATATYPE(cknum,'W') THEN cknum=0
IF retval<cknum THEN
  DO
    IF writeopen(fname) THEN
      DO
        CALL WRITELN(f,cknum)
        CALL CLOSE(f)
        RETURN cknum
      END
  END
RETURN retval


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
SAY fname 'failed to open for reading!'CR
RETURN 0


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok~=0 THEN RETURN 1
SAY fname 'failed to open for writing!'CR
RETURN 0


comma: PROCEDURE
ARG num .
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
RETURN numtext


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
    SAY pen3||line||def||CR
    bbsk=0
    RETURN
  END
bbsk=bbsk*512-SYSTEM_SPACE_LIMIT
IF bbsk<1 THEN bbsk=0
SAY RIGHT(comma(bbsk),tabspace) 'bytes available for uploads.'CR
RETURN


showxdevs: PROCEDURE EXPOSE bbspath pen3 def CR 
CALL FileList(bbspath'Numbers/Files.X.*',xfiles,'F','N')
IF xfiles.0>1 THEN CALL QSORT(1,xfiles.0,xfiles)
DO i=1 TO xfiles.0
  ii=LASTPOS('FILES.X.',UPPER(xfiles.i))+8
  temp=SUBSTR(xfiles.i,ii)
  SAY RIGHT(comma(countcheck('Numbers/Bytes.X.'temp 0)),15) 'bytes  in' RIGHT(comma(countcheck('Numbers/Files.X.'temp 0)),7)' files downloaded from' pen3||temp||def||CR
END
SAY LEFT('-',74,'-')||CR
RETURN


countmail:
SAY '   Counting online email...'lineup||CR
emailonline=0
DO ti=1 TO WORDS(userlist)
  emailonline=emailonline+WORDS(SHOWDIR(bbspath'Email/'WORD(userlist,ti)))
END
RETURN

/* bbsSTATS.rexx */
