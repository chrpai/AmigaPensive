/* $VER: ArcMsgs.rexx 8.3 (18.11.94)
© 1990-94 Richard Lee Stockton - FREELY DISTRIBUTABLE
archives unread conference messages into file in users email
*/

SIGNAL ON BREAK_C
SIGNAL ON ERROR
SIGNAL ON SYNTAX
OPTIONS FAILAT 999999

PARSE ARG name' 'single_dir' '.
IF STRIP(single_dir)='' THEN single_dir=0
IF name='' THEN CALL GETOUT(20)

CALL TIME('R')

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    CALL GETOUT(21)
  END
data.=''
DO i=1 TO 33
  data.i=READLN(f)
END
CALL CLOSE(f)

compos=POS('/*',data.1)
IF compos>0 THEN data.1=LEFT(data.1,compos-1)
bbsname = STRIP(data.1)
sysop   = WORD(data.2,1)
bbspath = WORD(data.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    CALL GETOUT(22)
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
msgpath = WORD(data.7,1)
IF ~EXISTS(msgpath) THEN
  DO
    SAY msgpath 'does not exist!'
    CALL GETOUT(23)
  END
testchar=RIGHT(msgpath,1)
IF testchar~='/' & testchar~=':' THEN msgpath=msgpath'/'
msgpath=msgpath'MSG'
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'
extension=WORD(data.32,1)
arccom=data.33
compos=POS('/*',data.33)
IF compos>0 THEN data.33=LEFT(data.33,compos-1)
arccom=STRIP(data.33)
IF LEFT(extension,1)~='.' THEN
  DO
    extension='.lzh'
    arccom='lharc -m m'
  END

CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,scratch'/ArcMsgs.STDOUT','W')
SAY STRIP(SUBSTR(SOURCELINE(1),3))
SAY

DO i=1
  IF GETCLIP('BBS_MSGS'i)='' THEN  /* info clip for external STOP */
    DO
      CALL SETCLIP('BBS_MSGS'i,name)
      CALL SETCLIP('BBS_STOP_MSGS'i)
      clipnum=i
      LEAVE i
    END
END

x=OPEN(f,bbspath'Users/'name,'R')
IF x=0 THEN
  DO
    CALL DELAY(150)
    x=OPEN(f,bbspath'Users/'name,'R')
    IF x=0 THEN
      DO
        SAY name 'user file is missing!'
        CALL GETOUT(24)
      END
  END
data.=''
DO i=1 TO 25
  data.i=READLN(f)
END
CALL CLOSE(f)
level=data.20%1
lastread.=0

x=GETCLIP('BBS_'name'_22')
IF x~='' THEN
  DO
    data.22=x
    CALL SETCLIP('BBS_'name'_22')
  END

msg.=''
IF readopen(bbspath'Lists/Conferences') THEN
  DO
    DO i=1
      line=READLN(f)
      IF line='END' THEN BREAK
      IF EOF(f) THEN BREAK
      num=WORD(line,1)
      IF DATATYPE(num,'N') THEN msg.num=WORD(line,2)
    END
    CALL CLOSE(f)
  END

CALL SETCLIP('BBS_MSGS')
CALL PRAGMA('P',-2)         /* lower the priority of this task */

oldt=0
count=0
lastm=0
archives=1
CALL open_new()

filepath=bbspath'EmailFiles/'name
CALL MAKEDIR(filepath)
arcname=filepath'/BBBBS_'lastm
IF GETCLIP('BBS_'name)~='' THEN arcname=filepath'/Messages'
CALL open_arc()
CALL newmsgs()
CALL CLOSE(a)
CALL DELAY(28)
IF WORD(STATEF(arcname),2)<80 THEN CALL GETOUT(24)
CALL write_msg()
CALL GETOUT(0)
EXIT



/* Functions */


write_msg:
IF GETCLIP('BBS_'name)~='' THEN RETURN
CALL PRAGMA('P',0)         /* normal priority */
ADDRESS COMMAND arccom arcname||extension arcname
IF ~EXISTS(arcname||extension) THEN CALL GETOUT(27)
CALL PRAGMA('P',-2)
x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
IF x=0 THEN CALL GETOUT(26)
CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm||extension)
CALL WRITELN(f,' From: BBBBS')
CALL WRITELN(f,'   To: 'name)
CALL WRITELN(f,' Subj: 'subj)
CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
CALL WRITELN(f,LEFT('=',75,'='))
temp='Here are the archived new messages you requested'
IF WORD(finfo,2)>1400000 THEN temp=temp', part' archives-1
CALL WRITELN(f,temp)
t=(.5+TIME('E'))%1
t=t-oldt
oldt=t+oldt
min=t%60
hrs=min%60
min=min//60
sec=t//60
temp=' - It took'
IF hrs>1 THEN temp=temp hrs 'hours'
ELSE IF hrs=1 THEN temp=temp '1 hour'
IF min>1 THEN temp=temp min 'minutes'
ELSE IF min=1 THEN temp=temp '1 minute'
IF sec=1 THEN temp=temp '1 second'
ELSE IF sec>0 THEN temp=temp sec 'seconds'
temp=temp 'to compile these messages -'
CALL WRITELN(f,temp)
CALL CLOSE(f)
newmess='Your archived messages are waiting in Email.'
IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  DO
    oldmess=GETCLIP('BBS_MESSAGE')
    IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
    CALL SETCLIP('BBS_MESSAGE',oldmess||newmess)
  END
IF GETCLIP('BBS_LOCAL')=name THEN CALL SETCLIP('BBS_LOCAL_MSG',newmess)
RETURN


open_new:
subj='All New Conference Messages'
IF single_dir>0 THEN subj=msg.single_dir' conference messages'
IF WORD(finfo,2)>1400000 THEN subj=subj', part' archives
IF GETCLIP('BBS_'name)='' THEN
  DO
    x=OPEN(f,bbspath'Numbers/LastMail','R')
    IF x=0 THEN
      DO
        CALL DELAY(100)
        x=OPEN(f,bbspath'Numbers/LastMail','R')
      END
    IF x=0 THEN
      DO
        IF lastm=0 THEN lastm=1
        ELSE lastm=lastm+1
      END
    ELSE lastm=READLN(f)+1
    CALL CLOSE(f)
    ADDRESS COMMAND 'ECHO >'bbspath'Numbers/LastMail 'lastm
    x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
    IF x=0 THEN CALL GETOUT(26)
    CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm)
    CALL WRITELN(f,' From: BBBBS')
    CALL WRITELN(f,'   To: 'name)
    CALL WRITELN(f,' Subj: 'subj)
    CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
    CALL WRITELN(f,LEFT('=',75,'='))
    CALL WRITELN(f,'Here are the new messages you requested.')
    CALL WRITELN(f,'The collection process was interrupted.')
    CALL CLOSE(f)
  END
RETURN


countcheck:
PARSE ARG fname' '.
IF ~readopen(fname) THEN RETURN(cknum)
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'N') THEN retval=0
RETURN(retval)


newmsgs:
IF single_dir>0 THEN
  DO
    msgdir=single_dir
    CALL readmsg()
    RETURN
  END
CALL WRITELN(a,'Scanning all Conferences for new messages..')
DO newi=1 TO level
  IF msg.newi='' THEN ITERATE newi
  msgdir=newi
  CALL readmsg()
END
RETURN


readmsg:
IF msg.msgdir='' | FIND(data.21,msgdir)>0 THEN RETURN;   /* sysop excluded */
IF WORD(data.22,msgdir)=-1 THEN RETURN;  /*  user excluded */
IF DATATYPE(WORD(data.22,msgdir),'N') THEN
  lastread.msgdir=WORD(data.22,msgdir)
IF STRIP(SHOWDIR(msgpath||msgdir))='' THEN RETURN
lstwrt=countcheck(bbspath'Numbers/LastMessage'msgdir 0)
frstwrt=countcheck(bbspath'Numbers/FirstMessage'msgdir 0)
temp=''
IF lastread.msgdir>=lstwrt THEN
  DO
    lastread.msgdir=lstwrt
    RETURN
  END
CALL WRITELN(a,'Entering' msg.msgdir 'Message Conference..')
IF OPEN(c,msgpath||msgdir'.txt','R')~=0 THEN
  DO i=1
    line=READLN(c)
    IF EOF(c) THEN LEAVE i
    CALL WRITELN(a,line)
  END
CALL CLOSE(c)
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
CALL countcheck(bbspath'Numbers/FirstMessage'msgdir firstmess)
msgstatus=1
DO msgloop=1
  lastreadnum=lastread.msgdir
  DO WHILE msglist.lastreadnum=0 & lastreadnum<lstwrt
    lastreadnum=lastreadnum+1
  END
  lastread.msgdir=lastreadnum
  IF lastreadnum=lstwrt & msglist.lstwrt=0 THEN RETURN
  DO mess=lastread.msgdir TO lstwrt+1
    IF msglist.mess~=msgstatus THEN ITERATE mess
    IF msgstatus>1 THEN CALL WRITELN(a,'Following the thread, level' msgstatus-1'.')
    msglist.mess=0
    arg=dirname'/'mess
    IF ~EXISTS(arg) THEN
      DO
        CALL WRITELN(a,'Message number' mess 'is missing.')
        ITERATE mess
      END
    IF ~readopen(arg) THEN ITERATE mess
    firstline  = READLN(f)
    secondline = READLN(f)
    thirdline  = READLN(f)
    forthline  = READLN(f)
    CALL CLOSE(f)
    IF WORDS(firstline)>2 THEN  /* if replies, change their num to >1 */
      DO
        thread=SUBSTR(firstline,WORDINDEX(firstline,4))
        DO tindx=1 TO WORDS(thread)
          test=WORD(thread,tindx)
          IF msglist.test~=0 THEN msglist.test=msgstatus+1
        END
      END
    CALL add_msg(arg)
    IF thread~='' THEN
       DO
         thread=''
         msgstatus=msgstatus+1
       END
  END
  IF msgstatus>1 THEN msgstatus=msgstatus-1
END
RETURN


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN(1)
SAY fname 'failed to open for reading!'
RETURN(0)


open_arc:
CALL CLOSE(a)
x=OPEN(a,arcname,'W')
IF x=0 THEN CALL GETOUT(30)
temp='= Custom archived for' name
IF WORD(finfo,2)>1400000 THEN temp=temp', part' archives
CALL WRITELN(a,temp)
CALL WRITELN(a,'=' bbsname  'conference messages to' DATE('W') DATE() TIME('C'))
CALL WRITELN(a,'')
RETURN


add_msg:
ARG addname .
count=count+1
IF count//5=0 THEN
  DO
    t=GETCLIP('BBS_STOP_MSGS'clipnum)
    IF t~='' THEN CALL cleanup(t)
  END
finfo=STATEF(arcname)
IF WORD(finfo,2)>1400000 THEN
  DO
    archives=archives+1
    CALL WRITELN(a,CENTER('=== continued in part' archives '. ===',75))
    IF GETCLIP('BBS_'name)~='' THEN
      DO
        IF EXISTS('Messages') THEN CALL RENAME('Messages','Mess1')
        arcname=filepath'/Mess'archives
      END
    ELSE
      DO
        CALL write_msg()
        x=OPEN(f,bbspath'Numbers/LastMail','R')
        IF x~=0 THEN lastm=READLN(f)+1
        ELSE lastm=lastm+1
        CALL CLOSE(f)
        ADDRESS COMMAND 'echo >'bbspath'Numbers/LastMail 'lastm
        arcname=filepath'/BBBBS_'lastm
        CALL open_new()
      END
    CALL open_arc()
  END
x=OPEN(b,addname,'R')
IF x=0 THEN SAY addname 'failed to open for reading!'
ELSE
  DO
    data=READCH(b,65000)
    CALL CLOSE(b)
    CALL WRITECH(a,data)
  END
CALL WRITELN(a,'')
CALL WRITELN(a,'')
RETURN


cleanup:
ARG t2 .
CALL CLOSE(a)
SAY 'User aborted!'
IF t2='DELETE' THEN
  DO
    IF GETCLIP('BBS_'name)~='' THEN temp=bbspath'EmailFiles/'name'/Mess'
    ELSE temp=LEFT(arcname,LENGTH(arcname)-4)
    ADDRESS COMMAND 'c:delete' temp'#?'
    x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'A')
    IF x~=0 THEN
      DO
        CALL WRITELN(f,'')
        CALL WRITELN(f,'*** This process aborted' DATE() 'at' TIME('C') '***')
        CALL WRITELN(f,'*** No data file exists, not even a partial one. ***')
        CALL CLOSE(f)
      END
  END
CALL GETOUT(0)
RETURN


BREAK_C:
SAY 'BREAK_C at line' SIGL
CALL GETOUT(1)


GETOUT:
ARG errorout
ERROR:
SYNTAX:
IF RC='RC' THEN RC=0
IF errorout>0 | RC>0 THEN SAY 'Error:' errorout'  RC='RC'  SIGL='SIGL
CALL SETCLIP('BBS_MSGS')
CALL SETCLIP('BBS_MSGS'clipnum)
CALL SETCLIP('BBS_STOP_MSGS'clipnum)
EXIT errorout

/* end of ArcMsgs.rexx */
