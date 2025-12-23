/* $VER: ArcAlpha.rexx 8.3 (18.11.94)
© 1990-94 Richard Lee Stockton - FREELY DISTRIBUTABLE
archives user defined alphafilelist into file in users email
receives its arguments from rexxDoors/Make_BrowseList.rexx
*/

CALL TIME('R')

SIGNAL ON ERROR
SIGNAL ON SYNTAX
OPTIONS FAILAT 999999

PARSE ARG name lastbrowse alphaflag libflag numlist
lastbrowse=STRIP(lastbrowse)

IF ~DATATYPE(lastbrowse,'N') | name='' | numlist='' THEN
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
libpath=WORD(lynes.8,1)
IF ~EXISTS(libpath) THEN
  DO
    SAY libpath 'does not exist!'
    CALL GETOUT(23)
  END
testchar=RIGHT(libpath,1)
IF testchar~='/' & testchar~=':' THEN libpath=libpath'/'
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

DO i=1
  IF GETCLIP('BBS_ALPHA'i)='' THEN  /* info clip for external STOP */
    DO
      CALL SETCLIP('BBS_ALPHA'i,name)
      clipnum=i
      LEAVE i
    END
END

CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,scratch'/ArcAlpha.STDOUT'clipnum,'W')
SAY STRIP(SUBSTR(SOURCELINE(1),3))
SAY

CALL PRAGMA('P',-3)         /* lower the priority of this task */

extension=WORD(lynes.32,1)
arccom=lynes.33
compos=POS('/*',lynes.33)
IF compos>0 THEN lynes.33=LEFT(lynes.33,compos-1)
arccom=STRIP(lynes.33)
IF LEFT(extension,1)~='.' THEN
  DO
    extension='.lzh'
    arccom='lharc -m m'
  END

filepath=bbspath'EmailFiles/'name
CALL MAKEDIR(filepath)
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
alphaname=filepath'/BBBBS_'lastm

dirs.=''
x=OPEN(f,bbspath'Lists/Libraries','R')
IF x=0 THEN CALL GETOUT(24)
DO i=1
  line=READLN(f)
  IF line='END' | EOF(f) THEN LEAVE i
  num=WORD(line,1)
  IF ~DATATYPE(num,'W') THEN ITERATE i
  dirs.num=WORD(line,2)
END
CALL CLOSE(f)

filelist.=''
filelist.0=0
IF alphaflag='D' THEN
  DO
    x=OPEN(f,bbspath'Lists/Files','R')
    IF x=0 THEN CALL GETOUT(24)
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN LEAVE i
      num=WORD(line,1)
      IF ~DATATYPE(num,'W') THEN ITERATE i
      filelist.num=WORD(line,3)
      IF num>filelist.0 THEN filelist.0=num
    END
    CALL CLOSE(f)
    alphalist.=''
    IF ~listOPEN(f,bbspath'Lists/Files.ALPHA','R') THEN CALL GETOUT(29)
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN LEAVE i
      alphalist.i=line
      num=WORD(line,3)
      filelist.num.0=i
    END
    CALL CLOSE(f)
    alphalist.0=i-1
  END
x=OPEN(a,alphaname,'W')
IF x=0 THEN CALL GETOUT(25)
title='=' bbsname
IF alphaflag='A' THEN title=title 'alphabetical'
ELSE title=title 'newest to oldest'
title=title', single-line file descriptions'
CALL WRITELN(a,title)
CALL WRITELN(a,'= Custom archived for' name'  'DATE() TIME('C'))
IF libflag='A' THEN
  DO
    CALL WRITELN(a,'')
    CALL WRITELN(a,'Filename          Bytes File# Library         KeyWords')
    CALL WRITELN(a,LEFT('=',77,'='))
  END
CALL CLOSE(a)

x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
IF x=0 THEN CALL GETOUT(28)
CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm)
CALL WRITELN(f,' From: BBBBS')
CALL WRITELN(f,'   To: 'name)
CALL WRITELN(f,' Subj: AlphaList')
CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
CALL WRITELN(f,LEFT('=',75,'='))
CALL WRITELN(f,'Here are the single line file descriptions you requested.')
CALL WRITELN(f,'The collection process was interrupted.')
CALL CLOSE(f)

IF alphaflag='A' THEN
  DO
    IF libflag='L' THEN
      DO
        x=OPEN(a,alphaname,'A')
        IF x=0 THEN RETURN
        DO i=1 TO WORDS(numlist)
          dnum=WORD(numlist,i)
          CALL WRITELN(a,'')
          IF dirs.dnum='' THEN
            DO
              line='Library' dnum 'does not exist!'
              CALL WRITELN(a,line)
              ITERATE i
            END
          x=OPEN(f,libpath||dirs.dnum'/.'STRIP(LEFT(dirs.dnum,15)),'R')
          IF x=0 THEN
            DO
              line='Library' dnum dirs.dnum 'filelist failed to open for reading!'
              CALL WRITELN(a,line)
              ITERATE i
            END
          DO j=1
            line=READLN(f)
            IF EOF(f) THEN LEAVE j
            CALL WRITELN(a,line)
          END
          CALL CLOSE(f)
        END
        CALL CLOSE(a)
      END
    ELSE CALL write_alist(numlist)
  END
ELSE
  DO
    IF libflag='L' THEN
      DO i=1 TO WORDS(numlist)
        CALL write_dlist(WORD(numlist,i))
      END
    ELSE CALL write_dlist(numlist)
  END

IF WORD(STATEF(alphaname),2)<40 THEN CALL GETOUT(26)
CALL PRAGMA('P',0)         /* normal priority */
ADDRESS COMMAND arccom alphaname||extension alphaname
IF ~EXISTS(alphaname||extension) THEN CALL GETOUT(27)
CALL PRAGMA('P',-3)
x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
IF x=0 THEN CALL GETOUT(28)
CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm||extension)
CALL WRITELN(f,' From: BBBBS')
CALL WRITELN(f,'   To: 'name)
CALL WRITELN(f,' Subj: AlphaList')
CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
CALL WRITELN(f,LEFT('=',75,'='))
CALL WRITELN(f,'Here are the single line file descriptions you requested.')
t=(.5+TIME('E'))%1
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
temp=temp 'to compile this list.'
CALL WRITELN(f,temp)
CALL CLOSE(f)

newmess='Your archived alphabetical filelist is waiting in Email.'
IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  DO
    oldmess=GETCLIP('BBS_MESSAGE')
    IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
    CALL SETCLIP('BBS_MESSAGE',oldmess||newmess)
  END
IF GETCLIP('BBS_LOCAL')=name THEN CALL SETCLIP('BBS_LOCAL_MSG',newmess)
CALL GETOUT(0)
EXIT


write_alist:
ARG nlist
nlist=STRIP(nlist)
x=OPEN(a,alphaname,'A')
IF x=0 THEN RETURN
count=0
IF ~listOPEN(f,bbspath'Lists/Files.ALPHA','R') THEN
  DO
    CALL CLOSE(f)
    CALL CLOSE(a)
    RETURN
  END
CALL CLOSE(f)
ADDRESS COMMAND 'COPY' bbspath'Lists/Files.ALPHA' bbspath'Lists/Files.ALPHA.arcalpha'
IF ~listOPEN(f,bbspath'Lists/Files.ALPHA.arcalpha','R') THEN
  DO
    CALL CLOSE(f)
    CALL CLOSE(a)
    RETURN
  END
DO j=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE j
  IF j//5=0 THEN
    DO
      t=GETCLIP('BBS_STOP_ALPHA'clipnum)
      IF t~='' THEN CALL cleanup(t)
    END
  IF WORD(line,3)<=lastbrowse THEN ITERATE j
  IF FIND(nlist,WORD(line,4))=0 THEN ITERATE j
  CALL WRITELN(a,line)
  count=count+1
END
CALL CLOSE(f)
CALL WRITELN(a,' 'count' files.')
CALL WRITELN(a,'')
CALL CLOSE(a)
CALL DELETE(bbspath'Lists/Files.ALPHA.arcalpha')
RETURN


write_dlist:
ARG nlist
nlist=STRIP(nlist)
x=OPEN(a,alphaname,'A')
IF x=0 THEN RETURN
IF libflag='L' THEN
  DO
    CALL WRITELN(a,'')
    CALL WRITELN(a,'File Library' nlist)
    CALL WRITELN(a,'Filename          Bytes File# Library         KeyWords')
    CALL WRITELN(a,LEFT('=',77,'='))
  END
count=0
DO k=filelist.0 TO 1 BY -1
  IF filelist.k='' THEN ITERATE k
  IF k//5=0 THEN
    DO
      t=GETCLIP('BBS_STOP_ALPHA'clipnum)
      IF t~='' THEN CALL cleanup(t)
    END
  IF k<=lastbrowse THEN LEAVE k
  j=filelist.k.0
  IF ~DATATYPE(j,'W') THEN ITERATE k
  IF FIND(nlist,WORD(alphalist.j,4))=0 THEN ITERATE k
  CALL WRITELN(a,alphalist.j)
  count=count+1
END
CALL WRITELN(a,' 'count' files.')
CALL WRITELN(a,'')
CALL CLOSE(a)
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


cleanup:
ARG t2 .
CALL CLOSE(a)
SAY 'User aborted!'
IF t2='DELETE' THEN
  DO
    CALL DELETE(alphaname)
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


ERROR:
SYNTAX:
GETOUT:
ARG errorout 
CALL SETCLIP('BBS_BROWSE')
CALL SETCLIP('BBS_ALPHA'clipnum)
CALL SETCLIP('BBS_STOP_ALPHA'clipnum)
IF errorout>0 THEN SAY 'ArcAlpha error' errorout'!'
EXIT(errorout)


/* ArcAlpha.rexx */
