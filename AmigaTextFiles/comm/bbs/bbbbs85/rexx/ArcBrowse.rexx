/* $VER: ArcBrowse.rexx 8.3 (18.11.94)
© 1990-94 Richard Lee Stockton - FREELY DISTRIBUTABLE
archives user defined browselist into file in users email it receives
its arguments from rexxDoors/Make_BrowseList.rexx or bbsQUICKOUT.rexx
*/

CALL TIME('R')

SIGNAL ON ERROR
SIGNAL ON SYNTAX
SIGNAL ON BREAK_C
OPTIONS FAILAT 999999

PARSE ARG name lastbrowse alphaflag libflag liblist 
lastbrowse=STRIP(lastbrowse)

IF ~DATATYPE(lastbrowse,'N') | name='' | alphaflag='' | libflag='' | liblist='' THEN
  CALL GETOUT(20)

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    CALL GETOUT(21)
  END
lynes.=''
DO i=1 TO 33
  lynes.i=READLN(f)
END
CALL CLOSE(f)

compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname = STRIP(lynes.1)

sysop   = WORD(lynes.2,1)

bbspath = WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    CALL GETOUT(22)
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

DO i=1
  IF GETCLIP('BBS_BROWSE'i)='' THEN  /* info clip for external STOP */
    DO
      CALL SETCLIP('BBS_BROWSE'i,name)
      CALL SETCLIP('BBS_STOP_BROWSE'i)
      clipnum=i
      LEAVE i
    END
END

CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,scratch'/ArcBrowse.'clipnum,'W')
SAY STRIP(SUBSTR(SOURCELINE(1),3))
SAY

CALL PRAGMA('P',-4)         /* lower the priority of this task */

extension=WORD(lynes.32,1)
compos=POS('/*',lynes.33)
IF compos>0 THEN lynes.33=LEFT(lynes.33,compos-1)
arccom=STRIP(lynes.33)
IF LEFT(extension,1)~='.' THEN
  DO
    extension='.lzh'
    arccom='lha -m m'
  END

filepath=bbspath'EmailFiles/'name
CALL MAKEDIR(filepath)
IF GETCLIP('BBS_'name)~='' THEN browsename=filepath'/FileList'
ELSE
  DO
    x=OPEN(f,bbspath'Numbers/LastMail','R')
    IF x=0 THEN
      DO
        CALL DELAY(100)
        x=OPEN(f,bbspath'Numbers/LastMail','R')
      END
    IF x=0 THEN lastm=1
    ELSE lastm=READLN(f)+1
    CALL CLOSE(f)
    ADDRESS COMMAND 'ECHO >'bbspath'Numbers/LastMail 'lastm
    browsename=filepath'/BBBBS_'lastm
  END
filelist.=''
IF ~listOPEN(c,bbspath'Lists/Files','R') THEN CALL GETOUT(23)
DO i=1
  line=READLN(c)
  IF EOF(c) THEN LEAVE i
  indx=WORD(line,1)
  IF ~DATATYPE(indx,'W') THEN ITERATE i
  IF indx<=lastbrowse THEN ITERATE i
  filelist.indx=line
END
CALL CLOSE(c)
lastfile=indx
IF OPEN(c,bbspath'Numbers/LastFile','R')~=0 THEN
  DO
    lastfile=READLN(c)
    CALL CLOSE(c)
  END
oldt=0
count=0
archives=1
CALL open_browse()

IF libflag='A' THEN CALL noteloop(1)
ELSE IF libflag='L' THEN
  DO j=1 TO WORDS(liblist)
    test1=WORD(liblist,j)
    CALL noteloop(0)
  END

IF WORD(STATEF(browsename),2)<80 THEN CALL GETOUT(26)
IF GETCLIP('BBS_'name)~='' THEN CALL GETOUT(0)
CALL write_msg()
CALL GETOUT(0)
EXIT



add_note:
ARG addname
count=count+1
IF count//5=0 THEN
  DO
    t=GETCLIP('BBS_STOP_BROWSE'clipnum)
    IF t~='' THEN CALL cleanup(t)
  END
finfo=STATEF(browsename)
IF WORD(finfo,2)>1400000 THEN
  DO
    x=OPEN(a,browsename,'A')
    IF x=0 THEN CALL GETOUT(33)
    CALL WRITELN(a,'')
    CALL WRITELN(a,CENTER('=== continued in part' archives+1'. ===',75))
    CALL CLOSE(a)
    CALL DELAY(50)
    IF GETCLIP('BBS_'name)~='' THEN
      DO
        IF EXISTS('FileList') THEN CALL RENAME('FileList','List1')
        browsename=filepath'/List'archives+1
      END
    ELSE
      DO
        CALL write_msg()
        x=OPEN(f,bbspath'Numbers/LastMail','R')
        IF x~=0 THEN lastm=READLN(f)+1
        ELSE lastm=lastm+1
        CALL CLOSE(f)
        ADDRESS COMMAND 'echo >'bbspath'Numbers/LastMail 'lastm
        browsename=filepath'/BBBBS_'lastm
      END
    archives=archives+1
    CALL open_browse()
  END
x=OPEN(a,browsename,'A')
IF x=0 THEN CALL GETOUT(30)
CALL WRITELN(a,'=====')
x=OPEN(b,addname,'R')
IF x=0 THEN CALL WRITELN(a,addname 'failed to open for reading!')
ELSE
  DO
    data=READCH(b,65000)
    CALL CLOSE(b)
    CALL WRITECH(a,data)
  END
CALL WRITELN(a,'')
CALL CLOSE(a)
RETURN


write_msg:
IF GETCLIP('BBS_'name)~='' THEN RETURN
CALL PRAGMA('P',0)         /* normal priority */
ADDRESS COMMAND arccom browsename||extension browsename
IF ~EXISTS(browsename||extension) THEN CALL GETOUT(27)
CALL PRAGMA('P',-4)
x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
IF x=0 THEN CALL GETOUT(28)
CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm||extension)
CALL WRITELN(f,' From: BBBBS')
CALL WRITELN(f,'   To: 'name)
temp='BrowseList'
IF WORD(finfo,2)>1400000 THEN temp=temp', part' archives
CALL WRITELN(f,' Subj:' temp)
CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
CALL WRITELN(f,LEFT('=',75,'='))
temp='Here are the file descriptions you requested'
IF WORD(finfo,2)>1400000 THEN temp=temp', part' archives
CALL WRITELN(f,temp'.')
t=(.5+TIME('E'))%1
t=t-oldt
oldt=t+oldt
min=t%60
hrs=min%60
min=min//60
sec=t//60
temp='It took'
IF hrs>1 THEN temp=temp hrs 'hours'
ELSE IF hrs=1 THEN temp=temp '1 hour'
IF min>1 THEN temp=temp min 'minutes'
ELSE IF min=1 THEN temp=temp '1 minute'
IF sec=1 THEN temp=temp '1 second'
ELSE IF sec>0 THEN temp=temp sec 'seconds'
temp=temp 'to compile these descriptions.'
CALL WRITELN(f,temp)
CALL CLOSE(f)
newmess='Your archived filelist is waiting in Email.'
IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  DO
    oldmess=GETCLIP('BBS_MESSAGE')
    IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
    CALL SETCLIP('BBS_MESSAGE',oldmess||newmess)
  END
IF GETCLIP('BBS_LOCAL')=name THEN CALL SETCLIP('BBS_LOCAL_MSG',newmess)
RETURN


open_browse:
x=OPEN(a,browsename,'W')
IF x=0 THEN CALL GETOUT(25)
IF alphaflag='A' THEN temp='alphabetical'
ELSE temp='aged'
title='=' bbsname temp 'file descriptions'
IF libflag='L' THEN title=title 'by library'
IF archives>1 THEN title=title', part' archives
CALL WRITELN(a,title)
CALL WRITELN(a,'= Custom archived for' name'  'DATE() TIME('C'))
CALL WRITELN(a,'')
CALL CLOSE(a)
IF GETCLIP('BBS_'name)='' THEN
  DO
    x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
    IF x=0 THEN CALL GETOUT(28)
    CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm)
    CALL WRITELN(f,' From: BBBBS')
    CALL WRITELN(f,'   To: 'name)
    CALL WRITELN(f,' Subj: BrowseList')
    CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
    CALL WRITELN(f,LEFT('=',75,'='))
    CALL WRITELN(f,'Here are the file descriptions you requested.')
    CALL WRITELN(f,'The collection process was interrupted.')
    CALL CLOSE(f)
  END
RETURN


listOPEN:
PARSE ARG fh,listfile,flag
DO i=0 TO 59 WHILE OPEN(fh,listfile,flag)=0
  IF i//4=0 THEN SAY 'Waiting' (60-i)*5 'more seconds for' listfile 'to become available...'
  CALL DELAY(250)
END
IF i>59 THEN
  DO
    SAY '*** unable to access' listfile 'list.'
    RETURN 0
  END
RETURN 1


noteloop:
ARG nflag .
IF alphaflag='A' THEN
  DO
    IF ~listOPEN(c,bbspath'Lists/Files.ALPHA','R') THEN GETOUT(24)
    CALL CLOSE(c)
    ADDRESS COMMAND 'COPY' bbspath'Lists/Files.ALPHA' bbspath'Lists/Files.ALPHA.arcbrowse'
    IF ~listOPEN(c,bbspath'Lists/Files.ALPHA.arcbrowse','R') THEN GETOUT(24)
    DO i=1
      line=READLN(c)
      IF EOF(c) THEN LEAVE i
      k=WORD(line,3)
      IF ~DATATYPE(k,'W') THEN ITERATE i
      IF filelist.k='' | lastbrowse>=k THEN ITERATE i
      lname=WORD(filelist.k,2)
      fname=WORD(filelist.k,3)
      IF nflag THEN
        DO
          IF FIND(UPPER(liblist),UPPER(lname))=0 THEN ITERATE i
        END
      ELSE IF UPPER(test1)~=UPPER(lname) THEN ITERATE i
      CALL add_note(bbspath'FileNotes/'lname'/'fname)
    END
    CALL CLOSE(c)
    CALL DELETE(bbspath'Lists/Files.ALPHA.arcbrowse')
  END
ELSE
  DO i=lastfile TO lastbrowse+1 BY -1
    IF filelist.i='' THEN ITERATE i
    lname=WORD(filelist.i,2)
    fname=WORD(filelist.i,3)
    IF nflag THEN
      DO
        IF FIND(UPPER(liblist),UPPER(lname))=0 THEN ITERATE i
      END
    ELSE IF UPPER(test1)~=UPPER(lname) THEN ITERATE i
    CALL add_note(bbspath'FileNotes/'lname'/'fname)
  END
RETURN


cleanup:
ARG t2 .
SAY 'User aborted!'
IF t2='DELETE' THEN
  DO
    IF GETCLIP('BBS_'name)~='' THEN
      DO
        CALL DELETE(bbspath'EmailFiles/'name'/FileList')
        ADDRESS COMMAND 'c:delete' bbspath'EmailFiles/'name'/List#?'
        x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'A')
        IF x~=0 THEN
          DO
            CALL WRITELN(f,'')
            CALL WRITELN(f,'*** This process aborted' DATE() 'at' TIME('C') '***')
            CALL WRITELN(f,'*** No data file exists, not even a partial one. ***')
            CALL CLOSE(f)
          END
      END
    ELSE ADDRESS COMMAND 'c:delete' LEFT(browsename,LENGTH(browsename)-4)'#?'
  END
CALL GETOUT(0)
RETURN


ERROR:
SYNTAX:
BREAK_C:
GETOUT:
ARG errorout
CALL SETCLIP('BBS_BROWSE')
CALL SETCLIP('BBS_BROWSE'clipnum)
CALL SETCLIP('BBS_STOP_BROWSE'clipnum)
IF errorout>0 THEN SAY 'ArcBrowse error' errorout'!'
EXIT(errorout)


/* ArcBrowse.rexx */
