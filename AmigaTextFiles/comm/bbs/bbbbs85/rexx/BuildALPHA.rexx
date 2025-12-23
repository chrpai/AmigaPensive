/* $VER: BuildALPHA.rexx 8.5 (5.6.95)
copyright 1990-95 Richard Lee Stockton
       FREELY DISTRIBUTABLE

Build the main filelist "Lists/Files", and alphabetically
sorted filelist "Lists/Files.ALPHA" from the filenotes.

KeyWords field (if empty) is built from the long description (if any).
Bytes field is corrected (if necessary) to match real filesize.

If COLLECT is passed as an argument, will collect files that have
no file description and construct descriptions for them using the
filecomment of the file (if any) for note data. Note that the
filecomment may then be blanked so it won't interfere with displays.

If NOFILEBUILD is passed as an argument, only the alpha list is built.
You may NOT use both flags at once.
*/

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN CALL GETOUT(666)

ARG baflags .

IF baflags='?' THEN
  DO
    DO i=1 TO 14
      SAY SOURCELINE(i)
    END
    EXIT
  END
CALL OPENPORT('BUILDALPHA')

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    EXIT(20)
  END
lynes.=''
DO i=1 TO 8
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
    SAY 'The bbspath, 'bbspath', does not exist!'
    EXIT(21)
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
libpath = WORD(lynes.8,1)
IF ~EXISTS(libpath) THEN
  DO
    SAY 'The library path, 'libpath', does not exist!'
    EXIT(22)
  END
testchar=RIGHT(libpath,1)
IF testchar~='/' & testchar~=':' THEN libpath=libpath'/'

IF SHOW('P','BBBBS') THEN CALL PRAGMA('P',-2)
etime=TIME('R')

IF baflags='COLLECT' THEN
  DO
    OPTIONS PROMPT 'Shall I clear each filecomment after using in the description? (Yn) > '
    PULL clearflag .
    clearflag=LEFT(STRIP(clearflag),1)
    SAY 'Searching for files with no file descriptions...'
    filelibs=SHOWDIR(libpath,'D')
    DO i=1 TO WORDS(filelibs)
      thislib=WORD(filelibs,i)
      realfiles=SHOWDIR(libpath||thislib)
      DO j=1 TO WORDS(realfiles)
        thisfile=WORD(realfiles,j)
        IF ~EXISTS(bbspath'FileNotes/'thislib'/'thisfile) THEN CALL makenote
      END
    END
    IF ~DATATYPE(lastfile,'N') THEN
      DO
        SAY 'All files in the libraries have descriptions.'
        EXIT
      END
  END

x=OPEN(f,bbspath'Lists/Libraries','R')
IF x=0 THEN CALL GETOUT(24);
libs.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  indx=WORD(line,1)
  IF ~DATATYPE(indx,'W') THEN ITERATE i
  libs.indx=WORD(line,2)
END
CALL CLOSE(f)

f.=''
cnt.=0
latest=0
IF ~listOPEN(a,bbspath'Lists/Files.ALPHA.alpha','W') THEN CALL GETOUT(26)
IF baflags='NOFILEBUILD' THEN SAY 'Rebuilding Files.ALPHA...'
ELSE SAY 'Rebuilding Filelists...'
DO i=1 TO 99
  IF libs.i='' THEN ITERATE i
  CALL MAKEDIR(libpath||libs.i)
  notes=SHOWDIR(bbspath'FileNotes/'libs.i,'F')
  DO j=1 TO WORDS(notes)
    note=WORD(notes,j)
    file=libpath||libs.i'/'note
    IF ~EXISTS(file) THEN
      DO
        temp=WORD(STATEF(bbspath'FileNotes/'libs.i'/'note),8)
        IF temp~='' THEN file=temp
        IF ~EXISTS(file) THEN SAY file 'does not exist.'
      END
    IF OPEN(f,bbspath'FileNotes/'libs.i'/'note,'R')=0 THEN
      DO
        CALL CLOSE(f)
        IF EXISTS(file) THEN
          SAY 'Could not open file description for' libs.i'/'note
        ELSE SAY file 'does not exist.'
        ITERATE j
      END
    line=READLN(f)
    PARSE VAR line 'File:' filenum 'KeyWords:' keywords
    IF ~DATATYPE(filenum,'W') THEN
      DO
        CALL CLOSE(f)
        SAY '*** Bad file description!' libs.i'/'note
        SAY line
        ITERATE j
      END
    filenum=filenum%1
    line=READLN(f)
    name=WORD(line,2)
    notebytes=WORD(line,4)
    IF LENGTH(keywords)<3 THEN
      DO
        CALL READLN(f)
        CALL READLN(f)
        line=READLN(f)
        CALL CLOSE(f)
        IF line~='' THEN
          DO
            keywords=STRIP(LEFT(line,32))
            CALL fixkeywords()
          END
      END
    ELSE CALL CLOSE(f)
    IF POS('1B'x,keywords)>0 THEN
      DO
        keywords=strip_ansi(keywords)
        CALL fixkeywords()
      END
    bytes=0
    IF EXISTS(file) THEN
      DO
        bytes=WORD(STATEF(file),2)
        IF bytes~=notebytes THEN CALL fixbytes()
      END
    ELSE bytes=notebytes
    IF UPPER(name)~=UPPER(note) THEN CALL fixname()
    IF f.filenum~='' THEN
      DO
        SAY 'Duplicate file numbers! 'filenum f.filenum '&' libs.i name
        IF OPEN(na,bbspath'Numbers/LastFile','R')~=0 THEN
          DO
            newnum=READLN(na)
            CALL CLOSE(na)
            IF DATATYPE(newnum,'W') THEN
              DO
                filenum=newnum+1
                CALL fixnumber()
                IF baflags='NOFILEBUILD' THEN
                  DO
                    IF OPEN(na,bbspath'BBS_TEXT/Files','A')~=0 THEN
                      DO
                        CALL WRITELN(na,filenum libs.i note)
                        CALL CLOSE(na)
                      END
                  END
                IF OPEN(na,bbspath'Numbers/LastFile','W')~=0 THEN
                  DO
                    CALL WRITELN(na,filenum)
                    CALL CLOSE(na)
                  END
              END
          END
      END
    IF baflags~='NOFILEBUILD' THEN f.filenum=libs.i name
    IF filenum>latest THEN latest=filenum
    alpha=LEFT(name,22-LENGTH(bytes)) bytes RIGHT(filenum,5) RIGHT(i,2)
    alpha=alpha LEFT(libs.i,12) TRIM(LEFT(STRIP(keywords),32))
    CALL WRITELN(a,alpha)
    cnt.i=cnt.i+1
  END
  lf1=libpath||libs.i'/.'STRIP(LEFT(libs.i,15))
  CALL DELETE(lf1)
  flg='W'
  IF EXISTS(libpath||libs.i'.txt') THEN
    DO
      ADDRESS COMMAND 'COPY' libpath||libs.i'.txt' lf1
      flg='A'
    END
  IF listOPEN(f,lf1,flg)=0 THEN ITERATE j
  IF flg='A' THEN CALL WRITELN(f,'')
  CALL WRITELN(f,'Filename          Bytes File# Library         KeyWords')
  CALL WRITELN(f,LEFT('=',77,'='))
  CALL CLOSE(f)
END
CALL CLOSE(a)

CALL SETCLIP('BBS_UPLOAD','NO')

IF baflags~='NOFILEBUILD' THEN
  DO
    IF ~listOPEN(f,bbspath'Lists/Files','W') THEN CALL GETOUT(27)
    CALL SETCLIP('BBS_UPLOAD','NO')
    SAY 'Saving Lists/Files...'
    DO i=1 TO latest
      IF f.i='' THEN ITERATE i
      CALL WRITELN(f,i f.i)
    END
    CALL CLOSE(f)
  END
IF EXISTS('c:shellsort') THEN
  DO
    SAY 'Sorting...'
    temp=bbspath'Lists/Files.ALPHA.alpha'
    temp='c:shellsort -c' temp temp
    ADDRESS COMMAND temp
    CALL DELAY(28)
  END
a.=''
IF ~listOPEN(f,bbspath'Lists/Files.ALPHA.alpha','R') THEN CALL GETOUT(28)
SAY 'Loading Lists/Files.ALPHA.alpha...'
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  a.i=line
END
CALL CLOSE(f)
a.0=i-1
IF ~EXISTS('c:shellsort') THEN
  DO
    SAY 'Sorting...'
    CALL QSORT(1,a.0,a)
  END
IF ~listOPEN(f,bbspath'Lists/Files.ALPHA','W') THEN CALL GETOUT(29)
CALL DELETE(bbspath'Lists/Files.ALPHA.alpha')
SAY 'Saving Lists/Files.ALPHA and individual library lists...'
DO i=1 TO a.0
  IF a.i='' THEN ITERATE i
  CALL WRITELN(f,a.i)
  lnum=WORD(a.i,4)
  IF ~DATATYPE(lnum,'W') THEN ITERATE i
  flg='W'
  lf=libpath||libs.lnum'/.'STRIP(LEFT(libs.lnum,15))
  IF EXISTS(lf) THEN flg='A'
  IF ~OPEN(a,lf,flg) THEN ITERATE i
  CALL WRITELN(a,a.i)
  CALL CLOSE(a)
END
CALL CLOSE(f)

CALL SETCLIP('BBS_UPLOAD')

SAY 'Adding taglines to individual lists...'
DO tt=1 TO 99
  IF libs.tt='' THEN ITERATE tt
  dname='.'STRIP(LEFT(libs.tt,15))
  x=OPEN(f,libpath||libs.tt'/'dname,'A')
  IF x~=0 THEN
    DO
      CALL WRITELN(f,LEFT('-',77,'-'))
      temp='file'
      IF cnt.tt~=1 THEN temp=temp's'
      temp=cnt.tt temp'.  Last updated' DATE() 'at' TIME('C')
      temp=temp RIGHT(bbsname,76-LENGTH(temp))
      CALL WRITELN(f,temp)
      CALL CLOSE(f)
    END
  CALL MAKEDIR(bbspath'FileNotes/'libs.tt)
  fnote=bbspath'FileNotes/'libs.tt'/'dname
  lynes.=''
  lynes.0=5
  x=OPEN(f,fnote,'R')
  IF x~=0 THEN
    DO
      DO k=1
        line=READLN(f)
        IF EOF(f) THEN LEAVE k
        lynes.k=line
      END
      CALL CLOSE(f)
      lynes.0=k-1
    END
  finfo=STATEF(libpath||libs.tt'/.'STRIP(LEFT(libs.tt,15)))
  bt=WORD(finfo,2)
  dl=WORD(lynes.2,7)
  IF ~DATATYPE(dl,'W') THEN dl=0
  lynes.1='File: 'LEFT(tt,5)' KeyWords: alphabetical files list CONTENTS'
  lynes.2='Name: 'LEFT(dname,27)' Size:' bt 'bytes  Downloads:' dl
  lynes.3='From: 'LEFT('BBBBS',27)' Date: 'DATE() TIME('C')'  Lib: 'libs.tt
  lynes.4=LEFT('',74,'=')
  IF lynes.5='' THEN
    lynes.5='Up to the minute alphabetical filelist of the' libs.tt 'library.'
  IF OPEN(f,fnote,'W')~=0 THEN
    DO
      DO k=1 TO lynes.0
        CALL WRITELN(f,lynes.k)
      END
      CALL CLOSE(f)
    END
END

IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localfiles',2)
IF SHOW('P','BBBBS_LOCAL') THEN CALL SETCLIP('BBS_mainfiles',2)
CALL GETOUT(0)
EXIT


/* sub-routines */

GETOUT:
ARG outnum .
IF outnum>0 THEN SAY 'Error number' outnum'.  Refer to BuildALPHA.rexx'
mins=TIME('E')%60
secs=TRUNC(TIME('E')//60)
IF secs<10 THEN secs='0'secs
SAY 'Elapsed time file building =' mins':'secs
CALL SETCLIP('BBS_UPLOAD')
EXIT(outnum);


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


fixnumber:
SAY 'Adding' libs.i name 'to the filelists as file number' filenum'...'
x=OPEN(f,bbspath'FileNotes/'libs.i'/'note,'R')
IF x=0 THEN
  DO
    SAY note 'failed to open for reading! (fixing filenumber)'
    RETURN
  END
text.=''
DO ii=1
  x=READLN(F)
  IF EOF(f) THEN LEAVE ii
  text.ii=x
END
CALL CLOSE(f)
text.0=ii-1
text.1=OVERLAY(filenum,text.1,7,5)
CALL DELAY(50)
x=OPEN(f,bbspath'FileNotes/'libs.i'/'note,'W')
IF x=0 THEN
  DO
    SAY note 'failed to open for writing! (fixing filenumber)'
    RETURN
  END
DO ii=1 TO text.0
  CALL WRITELN(f,text.ii)
END
CALL CLOSE(f)
RETURN


fixname:
SAY 'Name: field does not match actual filename!'
SAY 'Changing Name: 'name' in file description to 'note
x=OPEN(f,bbspath'FileNotes/'libs.i'/'note,'R')
IF x=0 THEN
  DO
    SAY note 'failed to open for reading! (fixing name)'
    RETURN
  END
text.=''
DO ii=1
  x=READLN(F)
  IF EOF(f) THEN LEAVE ii
  text.ii=x
END
CALL CLOSE(f)
text.0=ii-1
text.2=OVERLAY(note,text.2,7,27)
CALL DELAY(50)
x=OPEN(f,bbspath'FileNotes/'libs.i'/'note,'W')
IF x=0 THEN
  DO
    SAY note 'failed to open for writing! (fixing name)'
    RETURN
  END
DO ii=1 TO text.0
  CALL WRITELN(f,text.ii)
END
CALL CLOSE(f)
name=note
RETURN


fixkeywords:
SAY 'Fixing keywords field for' libs.i'/'note
fixfile=bbspath'FileNotes/'libs.i'/'note
finfo=STATEF(fixfile)
comment=''
IF WORDS(finfo)>7 THEN comment=STRIP(SUBSTR(finfo,WORDINDEX(finfo,8)))
fxlines.=''
x=OPEN(kf,fixfile,'R')
IF x=0 THEN
  DO
    SAY 'Failed to open' fixfile 'for reading! (keywords adjustment)'
    RETURN
  END
DO fx=1
  line=READLN(kf)
  IF EOF(kf) THEN LEAVE fx
  fxlines.fx=line
END
CALL CLOSE(kf)
fxlines.0=fx-1
fxlines.1='File:' LEFT(WORD(fxlines.1,2),5) 'KeyWords:' keywords
x=OPEN(kf,fixfile,'W')
IF x=0 THEN
  DO
    SAY 'Failed to open' fixfile 'for writing! (keywords adjustment)'
    RETURN
  END
DO fx=1 TO fxlines.0
  CALL WRITELN(kf,fxlines.fx)
END
CALL CLOSE(kf)
IF comment~='' THEN ADDRESS COMMAND 'filenote' fixfile comment
RETURN


fixbytes:
SAY 'Fixing bytes field for' libs.i'/'note
fixfile=bbspath'FileNotes/'libs.i'/'note
finfo=STATEF(fixfile)
comment=''
IF WORDS(finfo)>7 THEN comment=STRIP(SUBSTR(finfo,WORDINDEX(finfo,8)))
fxlines.=''
x=OPEN(f,fixfile,'R')
IF x=0 THEN
  DO
    SAY 'Failed to open' fixfile 'for reading! (byte adjustment)'
    RETURN
  END
DO fx=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE fx
  fxlines.fx=line
END
CALL CLOSE(f)
fxlines.0=fx-1
fxlines.2=OVERLAY(bytes,fxlines.2,WORDINDEX(fxlines.2,4),LENGTH(bytes)+1)
x=OPEN(f,fixfile,'W')
IF x=0 THEN
  DO
    SAY 'Failed to open' fixfile 'for writing! (byte adjustment)'
    RETURN
  END
DO fx=1 TO fxlines.0
  CALL WRITELN(f,fxlines.fx)
END
CALL CLOSE(f)
IF comment~='' THEN ADDRESS COMMAND 'filenote' fixfile comment
RETURN


makenote:
CALL MAKEDIR(bbspath'FileNotes/'thislib)
x=OPEN(f,bbspath'FileNotes/'thislib'/'thisfile,'W')
IF x=0 THEN
  DO
    SAY bbspath'FileNotes/'thislib'/'thisfile 'failed to open for writing!'
    RETURN
  END
x=OPEN(ff,bbspath'Numbers/LastFile','R')
IF x~=0 THEN lastfile=READLN(ff)+1
ELSE lastfile=1
CALL CLOSE(ff)
ADDRESS COMMAND 'ECHO >'bbspath'Numbers/LastFile 'lastfile
finfo=STATEF(libpath||thislib'/'thisfile)
bytes=WORD(finfo,2)
comment=''
IF WORDS(finfo)>7 THEN comment=STRIP(SUBSTR(finfo,WORDINDEX(finfo,8)))
CALL WRITELN(f,'File: 'LEFT(lastfile,5)' KeyWords:' STRIP(LEFT(comment,32)))
CALL WRITELN(f,'Name: 'LEFT(thisfile,27)' Size: 'bytes' bytes   Downloads: 0')
CALL WRITELN(f,'From: 'LEFT(sysop,27)' Date: 'DATE() TIME('C')'  Lib: 'thislib)
CALL WRITELN(f,LEFT('',74,'='))
IF LENGTH(comment)>31 THEN CALL WRITELN(f,comment)
CALL CLOSE(f)
IF comment~='' & clearflag~='N' THEN
  ADDRESS COMMAND 'filenote' libpath||thislib'/'thisfile '22'x||'22'x
SAY 'Added' thisfile 'to the' thislib 'library.'
RETURN


listOPEN:
PARSE ARG fh,listfile,flag
DO li=0 TO 59 WHILE OPEN(fh,listfile,flag)=0
  IF li//4=0 THEN SAY 'Waiting' (60-li)*5 'more seconds for' listfile 'to become available...'
  CALL DELAY(250)
END
IF li>59 THEN
  DO
    SAY '*** unable to access' listfile 'list.'
    RETURN 0
  END
RETURN 1


/* BuildALPHA.rexx */
