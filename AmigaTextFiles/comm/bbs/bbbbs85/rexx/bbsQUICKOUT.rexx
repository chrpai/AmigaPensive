/* $VER: bbsQUICKOUT.rexx 8.3 (22.11.94)
Copyright © 1994 Richard Lee Stockton
FREELY DISTRIBUTABLE AS LONG AS THIS NOTICE REMAINS
Gathers and archives all NEW mail, messages, and file descriptions
into an emailfile called QUICK_n.lha for later download.
*/

CR='0D'x
LF='0A'x

SIGNAL ON ERROR
SIGNAL ON SYNTAX
SIGNAL ON FAILURE
OPTIONS FAILAT 666

PARSE ARG name level lastbrowse sincedate excludelist 

bps=GETCLIP('BBS_QUICKOUT_BAUD')
CALL SETCLIP('BBS_QUICKOUT_BAUD')

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    CALL GETOUT(20)
  END
lynes.=''
DO i=1 TO 25
  lynes.i=READLN(f)
END
CALL CLOSE(f)
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
IF name='' THEN name=sysop
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

/* wait up to 5 minutes for QUICKIN to finish processing */

DO i=1 TO 100 WHILE GETCLIP('BBS_'name)='QUICKIN'
  CALL DELAY(150)
END

/* Should only be one QUICKOUT process at a time per user */

IF GETCLIP('BBS_'name)='QUICK' THEN EXIT

sendline=RIGHT(TIME('C'),7) LEFT(DATE(),6)' OUT' name
ADDRESS AREXX bbsLog99.rexx 'QUICK' sendline
x=countcheck('Numbers/QUICKOUT' 0)
CALL countcheck('Numbers/QUICKOUT' x+1)

CALL CLOSE(STDOUT)
IF OPEN(STDOUT,scratch'/QUICKOUT.STDOUT','W')=0 THEN
  DO i=1 WHILE OPEN(STDOUT,scratch'/QUICKOUT.STDOUT.'i,'W')=0
  END
SAY STRIP(SUBSTR(SOURCELINE(1),10))
SAY name DATE() TIME('C')
SAY
CALL PRAGMA('P',-1)
CALL TIME('R')
CALL SETCLIP('BBS_'name,'QUICK')
DO i=1
  IF GETCLIP('BBS_QUICKOUT'i)='' THEN  /* info clip for external STOP */
    DO
      CALL SETCLIP('BBS_QUICKOUT'i,name)
      clipnum=i
      LEAVE i
    END
END
CALL MAKEDIR(bbspath'EmailFiles/'name)
IF level='' THEN
  DO
    level=99
    x=OPEN(f,bbspath'Users/'name,'R')
    IF x=0 THEN EXIT 22
    data.=''
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN LEAVE i
      data.i=line
    END
    data.0=i-1
    CALL CLOSE(f)
    city=docity(data.3)
    data.13=DATE('S')'  'TIME('C')
    lastbrowse=WORD(data.16,1)
    sincedate=WORD(data.16,2)
    IF FIND(noquick,'FILELIST')=0 THEN
      data.16=countcheck('Numbers/LastFile' 0) DATE('S') TIME()
    excludelist=data.21
    IF FIND(noquick,'MESSAGES')=0 THEN
      DO
        CALL SETCLIP('BBS_'name'_22',data.22)
        temp=''
        DO i=1 TO level
          IF WORD(data.22,i)=-1 THEN temp=STRIP(temp -1)
          ELSE temp=STRIP(temp countcheck('Numbers/LastMessage'i 0))
        END
        data.22=temp
      END
    noquick=UPPER(data.26)
    x=OPEN(f,bbspath'Users/'name,'W')
    IF x~=0 THEN
      DO i=1 TO data.0
        CALL WRITELN(f,data.i)
      END
    CALL CLOSE(f)
  END
ELSE
  DO
    city=GETCLIP('BBS_city')
    CALL SETCLIP('BBS_city')
    noquick=UPPER(GETCLIP('BBS_'name'_26'))
    CALL SETCLIP('BBS_'name'_26')
  END

IF ~EXISTS(bbspath'Users/'name) THEN CALL GETOUT(21)
CALL check_abort()

IF FIND(noquick,'MESSAGES')=0 THEN CALL ArcMsgs.rexx(name)
CALL check_abort()

x=OPEN(f,bbspath'Numbers/LastMail','R')
IF x=0 THEN CALL GETOUT(27)
lastm=READLN(f)+1
CALL CLOSE(f)
ADDRESS COMMAND 'ECHO >'bbspath'Numbers/LastMail 'lastm


/* Make an ID file for the archive(s) */

x=OPEN(f,bbspath'EmailFiles/'name'/.ID','W')
IF x=0 THEN CALL GETOUT(22)
CALL WRITELN(f,' USER:' name)
CALL WRITELN(f,' CITY:' city)
CALL WRITELN(f,'  BBS:' bbsname)
CALL WRITELN(f,'SYSOP:' sysop)
CALL WRITELN(f,' DATE:' TIME('C') DATE())
CALL WRITELN(f,' KEYS:' lastm level sysoplevel TIME('E'))
CALL WRITELN(f,'')
CALL CLOSE(f)
comm='CD' bbspath'EmailFiles/'name LF 'lha -2amN m'
comm=comm 'QUICK_'lastm'.lha .ID'
SAY comm
ADDRESS COMMAND comm


/* add Messages, Libraries, and Conferences to archive */

frn=bbspath'Friends/'name
IF EXISTS(frn) THEN
  ADDRESS COMMAND 'c:copy' frn bbspath'EmailFiles/'name'/Friends'

CALL PRAGMA('D',bbspath'EmailFiles/'name)
nlist=''
IF EXISTS('Conferences') THEN nlist='Conferences'
IF EXISTS('Libraries') THEN nlist=STRIP(nlist 'Libraries')
IF EXISTS('Friends') THEN nlist=STRIP(nlist 'Friends')
IF EXISTS('Marked') THEN nlist=STRIP(nlist 'Marked')
CALL strip_ansi(bbspath'EmailFiles/'name nlist)
IF EXISTS('Messages') THEN nlist=STRIP(nlist 'Messages')
comm='CD' bbspath'EmailFiles/'name LF 'lha -2amN m'
comm=comm 'QUICK_'lastm'.lha'
SAY comm
SAY nlist
ADDRESS COMMAND comm nlist
CALL check_abort()


/* gather and archive email and emailfiles */

mailfiles=''
email=SHOWDIR(bbspath'Email/'name)
DO i=1 TO WORDS(email)
  x=OPEN(f,bbspath'Email/'name'/'WORD(email,i),'R')
  IF x=0 THEN ITERATE i
  line=READLN(f)
  CALL CLOSE(f)
  file=WORD(line,4)
  IF file='' | ~EXISTS(bbspath'EmailFiles/'name'/'file) THEN ITERATE i
  IF LEFT(UPPER(file),6)='QUICK_' THEN ITERATE i
  IF WORD(STATEF(bbspath'EmailFiles/'name'/'file),2)>0 THEN
    DO
      mailfiles=STRIP(mailfiles 'EmailFiles/'name'/'file)
      xdl=bbspath'EmailFiles/'name'/'file'.xdl'
      IF EXISTS(xdl) THEN
        CALL RENAME(xdl,bbspath'EmailFiles/'name'/QUICK_'lastm'.lha.xdl')
    END
END

CALL check_abort()
IF mailfiles~='' THEN
  DO
    comm='CD' bbspath LF 'lha -2axmN m'
    comm=comm 'EmailFiles/'name'/QUICK_'lastm'.lha' mailfiles
    SAY comm
    ADDRESS COMMAND comm
  END
SAY

CALL check_abort()
IF email~='' THEN
  DO
    CALL strip_ansi(bbspath'Email/'name email)
    comm='CD' bbspath LF 'lha -2axmN m'
    comm=comm 'EmailFiles/'name'/QUICK_'lastm'.lha Email/'name'/#?'
    SAY comm
    ADDRESS COMMAND comm
  END
SAY


/* Gather WELCOME(s), UNTIL(s), GOODBYE and all
   new Information files into "Notices" drawer */

CALL check_abort()
CALL MAKEDIR(bbspath'EmailFiles/'name'/Notices')

ulist=''
IF FIND(noquick,'HELLO')=0 & EXISTS(bbspath'BBS_TEXT/HELLO') THEN
  ulist='HELLO'
IF FIND(noquick,'WELCOME')=0 & EXISTS(bbspath'BBS_TEXT/WELCOME') THEN
  ulist=STRIP(ulist 'WELCOME')
arg='WELCOME.'RIGHT(DATE('S'),4)
IF EXISTS(bbspath'BBS_TEXT/'arg) THEN ulist=STRIP(ulist arg)
arg='WELCOME.'LEFT(DATE('W'),3)
IF EXISTS(bbspath'BBS_TEXT/'arg) THEN ulist=STRIP(ulist arg)
IF FIND(noquick,'GOODBYE')=0 & EXISTS(bbspath'BBS_TEXT/GOODBYE') THEN
  ulist=STRIP(ulist 'GOODBYE')
IF FIND(noquick,'BAUD.'bps)=0 & EXISTS(bbspath'BBS_TEXT/BAUD.'bps) THEN
  ulist=STRIP(ulist 'BAUD.'bps)
untils.=''
IF FileList(bbspath'BBS_TEXT/UNTIL.*',untils)>0 THEN
  DO ui=1 TO untils.0
    IF RIGHT(untils.ui,8)<DATE('S') THEN ITERATE ui
    ELSE ulist=STRIP(ulist untils.ui)
  END
DROP untils.
levels.=''
IF FileList(bbspath'BBS_TEXT/LEVEL.*',levels)>0 THEN
  DO ui=1 TO levels.0
    p=LASTPOS('.',levels.ui)
    x=SUBSTR(levels.ui,p+1)
    PARSE VAR x lo'-'hi .
    IF ~DATATYPE(lo,'W') | ~DATATYPE(hi,'W') THEN ITERATE ui
    IF lo>level | hi<level THEN ITERATE ui
    ulist=STRIP(ulist levels.ui)
  END
DROP levels.

CALL check_abort()
DO i=1 TO WORDS(ulist)
  comm='CD' bbspath'BBS_TEXT'LF'copy'
  comm=comm WORD(ulist,i) bbspath'EmailFiles/'name'/Notices'
  ADDRESS COMMAND comm
END

CALL check_abort()
ulist=''
dm=DATE(,sincedate,'S')
PARSE VAR dm da' 'mo' 'yr .
yr=RIGHT(yr,2)
sincedate=da'-'mo'-'yr
arg=bbspath'Information'
ADDRESS COMMAND 'C:LIST >'scratch'/infolist' arg 'NOHEAD DATES SINCE' sincedate
IF WORD(STATEF(scratch'/infolist'),2)>3 THEN
  DO
    x=OPEN(f,scratch'/infolist','R')
    IF x=0 THEN SAY scratch'infolist failed to open for reading!'
    ELSE
      DO i=1
        line=READLN(f)
        IF EOF(f) THEN LEAVE i
        IF LEFT(line,1)=':' THEN ITERATE i
        fyle=WORD(line,1)
        IF FIND(noquick,UPPER(fyle))>0 THEN ITERATE i
        ulist=STRIP(ulist fyle)
        readcount=STATEF(bbspath'Information/'fyle)
        readcount=WORD(readcount,8)
        IF ~DATATYPE(readcount,'W') THEN readcount=0
        ADDRESS COMMAND 'C:filenote' bbspath'Information/'fyle readcount+1
      END
    CALL CLOSE(f)
  END

CALL check_abort()
DO i=1 TO WORDS(ulist)
  comm='CD' bbspath'Information'LF'copy'
  comm=comm WORD(ulist,i) bbspath'EmailFiles/'name'/Notices'
  ADDRESS COMMAND comm
END

CALL check_abort()
IF FIND(noquick,'STATS.BBS')=0 THEN CALL bbsSTATS.rexx(name 0)
IF FIND(noquick,'HOURLY')=0 THEN CALL ShowHourly.rexx(name 99 0)

CALL check_abort()
IF level>=sysoplevel THEN
  DO
    ADDRESS COMMAND 'info >'bbspath'EmailFiles/'name'/Notices/Info_Devs'
    ADDRESS COMMAND 'rxset >'bbspath'EmailFiles/'name'/Notices/Info_Clips'
    ADDRESS COMMAND 'avail >'bbspath'EmailFiles/'name'/Notices/Info_Memory'
    temp=bbspath'Lists/NEW_USERS'
    IF EXISTS(temp) THEN
      ADDRESS COMMAND 'copy' temp bbspath'EmailFiles/'name'/Notices'
    temp=bbspath'Lists/CBV_USERS'
    IF EXISTS(temp) THEN
      ADDRESS COMMAND 'copy' temp bbspath'EmailFiles/'name'/Notices'
  END

CALL check_abort()
IF WORDS(bbspath'EmailFiles/'name'/Notices')>0 THEN
  DO
    temp=bbspath'EmailFiles/'name'/Notices'
    CALL strip_ansi(temp SHOWDIR(temp))
    comm='CD' bbspath'EmailFiles/'name||LF'lha -2axmN m'
    comm=comm bbspath'EmailFiles/'name'/QUICK_'lastm'.lha Notices/#?'
    SAY comm
    ADDRESS COMMAND comm
  END


/* archive NEW file descriptions by date */

CALL check_abort()
IF FIND(noquick,'FILELIST')=0 THEN
  DO
    x=OPEN(f,bbspath'Lists/Libraries','R')
    IF x=0 THEN
      DO
        SAY 'Libraries list did not open!'
        CALL GETOUT(26)
      END
    libs.=''
    liblist=''
    DO i=1
      line=READLN(f)
      IF EOF(f) | line='END' THEN LEAVE i
      num=WORD(line,1)
      lib=WORD(line,2)
      IF DATATYPE(num,'N') THEN
        DO
          num=num%1
          IF num>0 & num<=level THEN
            DO
              IF FIND(UPPER(excludelist),UPPER(lib))=0 THEN
                liblist=STRIP(liblist lib)
            END
        END
    END
    CALL CLOSE(f)
    CALL ArcBrowse.rexx(name lastbrowse 'D A' liblist)
  END


/* add FileList to archive */

CALL check_abort()
IF EXISTS('FileList') THEN
  DO
    comm='CD' bbspath'EmailFiles/'name LF 'lha -2amN m'
    comm=comm 'QUICK_'lastm'.lha FileList'
    SAY comm
    ADDRESS COMMAND comm
  END
ELSE IF EXISTS('List1') THEN
  DO i=1 WHILE EXISTS('List'i)
    comm='CD' bbspath'EmailFiles/'name LF 'lha -2amN m'
    comm=comm 'QUICK_'lastm'.lha List'i
    SAY comm
    ADDRESS COMMAND comm
  END
CALL check_abort()


/* If user is still online, write email and signal */

IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  DO
    x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
    IF x=0 THEN CALL GETOUT(26)
    CALL WRITELN(f,' Mail: 'lastm'   FILE: QUICK_'lastm'.lha')
    CALL WRITELN(f,' From: BBBBS')
    CALL WRITELN(f,'   To: 'name)
    CALL WRITELN(f,' Subj: BBS activity since your last call.')
    CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
    CALL WRITELN(f,LEFT('=',75,'='))
    CALL WRITELN(f,'Here is the QUICK archive you requested.')
    CALL CLOSE(f)
    oldmess=GETCLIP('BBS_MESSAGE')
    IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
    newmess='Your QUICK archive is waiting in Email.'
    CALL SETCLIP('BBS_MESSAGE',oldmess||newmess)
  END
SAY 'QUICKOUT archive for' name 'sucessfully completed at' TIME('C')
temp=''
secs=TIME('E')
mins=secs%60
hrs=mins%60
secs=(secs//60)%1
mins=mins//60
IF hrs=1 THEN temp='1 hour'
ELSE IF hrs>0 THEN temp=hrs 'hours'
IF mins=1 THEN temp=temp '1 minute'
ELSE IF mins>0 THEN temp=temp mins 'minutes'
IF secs=1 THEN temp=temp '1 second'
ELSE IF secs>0 THEN temp=temp secs 'seconds'
temp=STRIP(temp)
tstamp=hrs':'
IF mins<10 THEN tstamp=tstamp'0'
tstamp=tstamp||mins':'
IF secs<10 THEN tstamp=tstamp'0'
tstamp=LEFT(tstamp||secs,9)
bytes=WORD(STATEF(bbspath'EmailFiles/'name'/QUICK_'lastm'.lha'),2)
SAY ' -' temp 'to process this' bytes 'byte archive - '
SAY
sendline=RIGHT(TIME('C'),7) LEFT(DATE(),6)' OUT Elapsed:'
sendline=sendline tstamp||RIGHT(bytes,8) 'bytes for' name
ADDRESS AREXX bbsLog99.rexx 'QUICK' sendline
CALL GETOUT(0)
EXIT


GETOUT:
ARG err 
IF err>0 THEN SAY 'Error:' err'  'ERRORTEXT(RC) 'RC='RC'  LINE#='SIGL
ERROR:
SYNTAX:
FAILURE:
IF RC>0 THEN SAY 'RC='RC'  SIGL='SIGL
IF GETCLIP('BBS_'name)='QUICK' THEN CALL SETCLIP('BBS_'name)
CALL SETCLIP('BBS_QUICKOUT'clipnum)
CALL DELETE(bbspath'EmailFiles/'name'/Notices')
EXIT err


check_abort:
t=GETCLIP('BBS_STOP_QUICKOUT'clipnum)
IF t='' THEN RETURN
CALL SETCLIP('BBS_STOP_QUICKOUT'clipnum)
SAY 'Aborted at' TIME('C')
IF t='DELETE' THEN
  DO
    CALL DELETE(bbspath'EmailFiles/'name'/QUICK_'lastm'.lha')
    ADDRESS COMMAND 'c:delete' bbspath'EmailFiles/'name'/Notices ALL'
  END
sendline=RIGHT(TIME('C'),7) LEFT(DATE(),6)' OUT' name
sendline=sendline 'ABORTED!'
ADDRESS AREXX bbsLog99.rexx 'QUICK' sendline
CALL GETOUT(0)
RETURN


strip_ansi:
PARSE ARG path tlist 
IF tlist='' THEN RETURN
olddir=PRAGMA('D',path)
DO j=1 TO WORDS(tlist)
  data.=''
  changed=0
  x=OPEN(f,WORD(tlist,j),'R')
  IF x=0 THEN
    DO
      SAY WORD(tlist,j) 'failed to open to read!'
      ITERATE j
    END
  DO i=1
    line=READLN(f)
    IF EOF(f) THEN LEAVE i
    n=POS('1B'x,line)
    DO WHILE n>0
      DO k=2
        IF DATATYPE(SUBSTR(line,n+k,1),M) | (n+k+1)>LENGTH(line) THEN
          leave k
      END
      line=DELSTR(line,n,k+1)
      n=POS('1B'x,line)
      changed=1
    END
    data.i=line
  END
  data.0=i-1
  CALL CLOSE(f)
  IF changed=0 THEN ITERATE j
  CALL DELAY(50)
  x=OPEN(f,WORD(tlist,j),'W')
  IF x=0 THEN
    DO
      SAY WORD(tlist,j) 'failed to open to write!'
      ITERATE j
    END
  DO i=1 TO data.0
    CALL WRITELN(f,data.i)
  END
  CALL CLOSE(f)
END
CALL PRAGMA('D',olddir)
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

/* bbsQUICKOUT.rexx */
