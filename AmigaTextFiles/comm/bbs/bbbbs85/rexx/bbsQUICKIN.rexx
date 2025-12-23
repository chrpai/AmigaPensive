/* $VER: bbsQUICKIN.rexx 8.3 (23.12.94)
Copyright © 1993-94 Richard Lee Stockton
FREELY DISTRIBUTABLE AS LONG AS THIS NOTICE REMAINS 

Processes archive "QUICKIN.lha" in user's emailfiles, which
should have been made by bbsQUICK.rexx, the offline reader.
Handles incoming mail, messages, downloads, and uploads.
Also handles some sysop and super-sysop offline functions.
*/

CR='0D'x
LF='0A'x
SIGNAL ON ERROR
SIGNAL ON SYNTAX
SIGNAL ON BREAK_C
SIGNAL ON FAILURE

ARG name level sysoplevel accessflag .

fromcli=0
figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    CALL GETOUT(20)
  END
lynes.=''
DO i=1 TO 31
  lynes.i=READLN(f)
END
CALL CLOSE(f)
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
calls=WORD(lynes.31,1)
IF name='' THEN
  DO
    bbspath=WORD(lynes.6,1)
    IF RIGHT(bbspath,1)~=':' & RIGHT(bbspath,1)~='/' THEN bbspath=bbspath'/'
    msgpath=WORD(lynes.7,1)
    IF RIGHT(msgpath,1)~=':' & RIGHT(msgpath,1)~='/' THEN msgpath=msgpath'/'
    libpath=WORD(lynes.8,1)
    IF RIGHT(libpath,1)~=':' & RIGHT(libpath,1)~='/' THEN libpath=libpath'/'
    name=sysop
    IF ~EXISTS(bbspath'EmailFiles/'name'/QUICKIN.lha') THEN
      DO
        SAY bbspath'EmailFiles/'name'/QUICKIN.lha does not exist!'
        CALL GETOUT(21)
      END
    level=99
    sysoplevel=99
    accessflag=0
    fromcli=1
  END
ELSE
  DO
    bbspath=GETCLIP('BBS_path')
    msgpath=GETCLIP('BBS_msgpath')
    libpath=GETCLIP('BBS_libpath')
  END
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

/* Wait 10 mins for QUICKOUT process (started by this user) to finish */

DO i=1 TO 100 WHILE GETCLIP('BBS_'name)='QUICK'
  CALL DELAY(300)
END

/* Only one QUICKIN process at a time per user */

IF GETCLIP('BBS_'name)='QUICKIN' THEN EXIT

sendline=RIGHT(TIME('C'),7) LEFT(DATE(),6)'  IN' name
ADDRESS AREXX bbsLog99.rexx 'QUICK' sendline
x=countcheck('Numbers/QUICKIN' 0)
CALL countcheck('Numbers/QUICKIN' x+1)

CALL PRAGMA('P',-1)

CALL TIME('R')
CALL SETCLIP('BBS_'name,'QUICKIN')
DO i=1 WHILE GETCLIP('BBS_QUICK_WAIT')~='' | GETCLIP('BBS_UPLOAD')~=''
  CALL DELAY(500)                 /* wait for main filesaves to complete */
  IF TIME('E')>42000 THEN LEAVE i /* don't wait forever */
END
DO i=1
  IF GETCLIP('BBS_QUICKIN'i)='' THEN  /* info clip for external STOP */
    DO
      CALL SETCLIP('BBS_QUICKIN'i,name)
      clipnum=i
      LEAVE i
    END
END
arcfile=bbspath'Emailfiles/'name'/QUICKIN.lha'
savefiles=0
upfiles=-1
upbytes=0
upmail=0
upmsg=''
a.=''
a.0=0

lastm=get_last(bbspath'Numbers/LastMail')
CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,bbspath'Email/'name'/BBBBS.'lastm,'W')
SAY ' Mail: 'lastm
SAY ' From: BBBBS'
SAY '   To: 'name
SAY ' Subj: QUICKIN Report'
SAY ' Date: 'DATE('W') DATE() TIME('C')
SAY LEFT('=',75,'=')
SAY 'Here is the log of your QUICKIN file processing.'
SAY STRIP(SUBSTR(SOURCELINE(1),10))
SAY

ADDRESS COMMAND 'lha -q t' arcfile
IF RC>0 THEN
  DO
    SAY 'QUICKIN archive is corrupt!  Aborting...'
    SIGNAL DONE
  END

CALL MAKEDIR(scratch'/QUICK')
CALL PRAGMA('D',scratch'/QUICK')
ADDRESS COMMAND 'CD' scratch'/QUICK' LF 'lha -mN x' arcfile
SAY

CALL check_abort()
CALL do_file_requests()
CALL check_abort()
CALL do_file_deletes()

DO i=.001 TO .999 BY .001
  CALL check_abort()
  hdr=RIGHT(i,3)'.HDR'
  IF ~EXISTS(hdr) THEN ITERATE i
  txt=RIGHT(i,3)'.TXT'
  x=OPEN(f,hdr,'R')
  IF x=0 THEN
    DO
      SAY hdr 'failed to open for reading.'
      ITERATE i
    END
  hdr.=''
  DO j=1 TO 6
    hdr.j=READLN(f)
  END
  CALL CLOSE(f)
  IF LEFT(hdr.1,6)='File: ' THEN CALL do_file()
  ELSE IF LEFT(hdr.1,6)='  Msg:' THEN CALL do_msg()
  ELSE IF LEFT(hdr.1,6)=' Mail:' THEN CALL do_mail()
  ELSE
    DO
      SAY
      SAY hdr 'is an unknown header type!'
      DO j=1 TO 6
        SAY hdr.j
        SAY
      END
      ITERATE i
    END
  IF WORDS(SHOWDIR(scratch'/QUICK','F'))=0 THEN LEAVE i
END

IF savefiles=1 THEN
  DO
    IF listOPEN(a,bbspath'Lists/Files','W') THEN
      DO
        DO i=1 TO f.0
          IF f.i~='' THEN CALL WRITELN(a,i f.i)
        END
        CALL CLOSE(a)
        SAY 'Updated Lists/Files'
      END
    CALL checkliblists()
    IF ~listOPEN(a,bbspath'Lists/Files.ALPHA','R') THEN
      DO
        SAY 'Could not open Files.ALPHA for reading!'
        CALL GETOUT(22)
      END
    CALL CLOSE(a)
    CALL RENAME(bbspath'Lists/Files.ALPHA',bbspath'Lists/Files.ALPHA.quickin')
    IF OPEN(f,bbspath'Lists/Files.ALPHA.quickin','R')=0 THEN
      DO
        SAY bbspath'Lists/Files.ALPHA.quickin failed to open for reading!'
        CALL GETOUT(23)
      END
    IF listOPEN(a,bbspath'Lists/Files.ALPHA','W') THEN
      DO
        IF a.0>1 THEN CALL QSORT(1,a.0,a)
        acount=1
        DO i=1 TO f.0
          line=READLN(f)
          num=WORD(line,3)
          IF f.num~='' THEN
            DO
              DO WHILE a.acount~='' & UPPER(a.acount)<UPPER(line)
                CALL WRITELN(a,a.acount)
                CALL updateliblists(a.acount)
                acount=acount+1
              END
              acc=acount-1
              IF num~=WORD(a.acc,3) & num~=WORD(a.acount,3) THEN
                DO
                  CALL WRITELN(a,line)
                  CALL updateliblists(line)
                END
            END
        END
        CALL CLOSE(f)
        CALL CLOSE(a)
        SAY 'Updated Lists/Files.ALPHA'
        CALL DELETE(bbspath'Lists/Files.ALPHA.quickin')
        CALL closeliblists()
        CALL bbsALPHA.rexx(f.0 'lha lha >'scratch'/junk -2a m')
      END
    IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localfiles',2)
    IF SHOW('P','BBBBS_LOCAL') THEN CALL SETCLIP('BBS_mainfiles',2)
  END

DROP a. f. dirs.

CALL check_abort()

IF EXISTS(scratch'/QUICK/Files') THEN
  ADDRESS COMMAND 'delete' scratch'/QUICK/Files ALL QUIET'

IF EXISTS(scratch'/QUICK/Information') THEN
  DO
    IF level=99 THEN
      ADDRESS COMMAND 'copy' scratch'/QUICK/Information/#?' bbspath'Information'
    ADDRESS COMMAND 'delete' scratch'/QUICK/Information ALL QUIET'
  END
IF EXISTS(scratch'/QUICK/BBS_TEXT') THEN
  DO
    IF level=99 THEN
      ADDRESS COMMAND 'copy' scratch'/QUICK/BBS_TEXT/#?' bbspath'BBS_TEXT'
    ADDRESS COMMAND 'delete' scratch'/QUICK/BBS_TEXT ALL QUIET'
  END
IF EXISTS(scratch'/QUICK/rexxDoors') THEN
  DO
    IF level=99 THEN
      ADDRESS COMMAND 'copy' scratch'/QUICK/rexxDoors/#?' bbspath'rexxDoors ALL'
    ADDRESS COMMAND 'delete' scratch'/QUICK/rexxDoors ALL QUIET'
  END
IF EXISTS(scratch'/QUICK/REXX') THEN
  DO
    IF level=99 THEN
      ADDRESS COMMAND 'copy' scratch'/QUICK/REXX/#? REXX:'
    ADDRESS COMMAND 'delete' scratch'/QUICK/REXX ALL QUIET'
  END
IF EXISTS(scratch'/QUICK/S') THEN
  DO
    IF level=99 THEN
      ADDRESS COMMAND 'copy' scratch'/QUICK/S/#? S:'
    ADDRESS COMMAND 'delete' scratch'/QUICK/S ALL QUIET'
  END
IF EXISTS(scratch'/QUICK/C') THEN
  DO
    IF level=99 THEN
      ADDRESS COMMAND 'copy' scratch'/QUICK/C/#? C:'
    ADDRESS COMMAND 'delete' scratch'/QUICK/C ALL QUIET'
  END
CALL check_abort()
IF EXISTS(scratch'/QUICK/MSG') THEN
  DO
    IF level=99 THEN
      DO
        d=SHOWDIR(scratch'/QUICK/MSG','F')
        DO i=1 TO WORDS(d)
          msg=WORD(d,i)
          PARSE VAR msg 'MSG'conf'.'msgnum
          IF DATATYPE(conf,'W') & DATATYPE(msgnum,'W') THEN
            DO
              newname=msgpath'MSG'conf'/'msgnum
              IF EXISTS(newname) THEN
                DO
                  SAY newname 'already exists!'
                  ITERATE i
                END
              x=OPEN(f,scratch'/QUICK/MSG/'msg,'R')
              IF x=0 THEN ITERATE i
              a=READCH(f,65000)
              CALL CLOSE(f)
              a='!!'SUBSTR(a,3)
              x=OPEN(f,newname,'W')
              IF x=0 THEN ITERATE i
              CALL WRITECH(f,a)
              CALL CLOSE(f)
              SAY 'Un-deleted message' msgnum 'in conference' conf
            END
        END
      END
    ADDRESS COMMAND 'delete' scratch'/QUICK/MSG ALL QUIET'
  END
SAY

CALL check_abort()

IF EXISTS('Super') THEN
  DO
    IF level=99 & EXISTS('Super/Super.rexx') THEN
      DO
        CALL PRAGMA('D','Super')
        SAY 'running Super.rexx...'
        CALL Super.rexx()
        CALL PRAGMA('D','/')
        SAY
      END
    CALL DELETE('Super/Super.rexx')
    CALL DELETE('Super')
  END

d=SHOWDIR(scratch'/QUICK','F')
IF d~='' THEN
  DO
    SAY
    SAY 'Unable to process the following files.'
    SAY
    DO i=1 TO WORDS(d)
      SAY
      dname=WORD(d,i)
      SAY 'Filename:' dname
      x=OPEN(f,scratch'/QUICK/'dname,'R')
      IF x=0 THEN
        DO
         SAY dname 'failed to open for reading!'
          ITERATE i
        END
      stuff=READCH(f,65000)
      CALL CLOSE(f)
      CALL WRITECH(STDOUT,stuff)
      CALL DELETE(scratch'/QUICK/'dname)
    END
  END

DONE:
abytes=WORD(STATEF(arcfile),2)
CALL DELETE(arcfile)

IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  DO
    oldmess=GETCLIP('BBS_MESSAGE')
    IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
    newmess='Your QUICKIN archive has been processed. A report is waiting in Email.'
    CALL SETCLIP('BBS_MESSAGE',oldmess||newmess)
    IF upfiles>0 | upmail>0 | upmsg~='' THEN
      DO
        CALL SETCLIP(name'_UPDATE',upfiles upbytes upmail upmsg)
        upfiles=0
        upbytes=0
        upmail=0
        upmsg=''
      END
  END

IF upfiles>0 | upmail>0 | upmsg~='' THEN
  DO
    x=OPEN(f,bbspath'Users/'name,'R')
    IF x~=0 THEN
      DO
        data.=''
        DO i=1
          line=READLN(f)
          IF EOF(f) THEN LEAVE i
          data.i=line
        END
        CALL CLOSE(f)
        data.0=i-1
        IF upfiles>0 THEN
          DO
            files=WORD(data.14,1)
            bytes=WORD(data.14,3)
            IF DATATYPE(files,'W') THEN upfiles=upfiles+files
            IF ~DATATYPE(bytes,'W') THEN bytes=upbytes
            ELSE IF fromcli THEN
              DO
                bytes=bytes+upbytes
                files=files+1
              END
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
            data.23=''
            DO i=1 TO level
              msg=WORD(temp,i)
              IF ~DATATYPE(msg,'W') THEN msg=0
              DO j=1 TO WORDS(upmsg)
                IF WORD(upmsg,j)=i THEN msg=msg+1
              END
              data.23=data.23 msg
            END
          END
        x=OPEN(f,bbspath'Users/'name,'W')
        IF x~=0 THEN
          DO
            DO i=1 TO data.0
              CALL WRITELN(f,data.i)
            END
            CALL CLOSE(f)
            SAY 'User file' name 'updated.'
          END
      END
  END

SAY 'QUICKIN archive for' name 'sucessfully processed at' TIME('C')
temp=''
secs=TIME('E')
mins=secs%60
hrs=mins%60
secs=(secs//60)%1
mins=mins//60
IF hrs=1 THEN temp='1 hour'
ELSE IF hrs>0 THEN temp=temp hrs 'hours'
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
SAY ' - It took' temp 'to process this' abytes 'byte archive -'
sendline=RIGHT(TIME('C'),7) LEFT(DATE(),6)'  IN Elapsed:'
sendline=sendline tstamp||RIGHT(abytes,8) 'bytes from' name
ADDRESS AREXX bbsLog99.rexx 'QUICK' sendline
CALL GETOUT(0)
EXIT


/* Functions */


check_abort:
t=GETCLIP('BBS_STOP_QUICKIN'clipnum)
IF t='' THEN RETURN
CALL SETCLIP('BBS_STOP_QUICKIN'clipnum)
SAY 'Aborted at' TIME('C')
IF t='DELETE' THEN
  DO
    CALL DELETE(arcfile)
    ADDRESS COMMAND 'c:delete' scratch'/QUICK/#? ALL'
  END
sendline=RIGHT(TIME('C'),7) LEFT(DATE(),6)'  IN' name
sendline=sendline 'ABORTED!'
ADDRESS AREXX bbsLog99.rexx 'QUICK' sendline
CALL GETOUT(0)
RETURN


do_file:
PARSE VAR hdr.1 'File:' filenum .'KeyWords: 'keywords 
filename=WORD(hdr.2,2)
IF ~DATATYPE(filenum,'W') & ~EXISTS('Files/'filename) THEN
  DO
    SAY filename 'did not arrive with the QUICKIN archive!'
    RETURN
  END
toname=WORD(hdr.3,2)
lib=WORD(hdr.3,9)
IF load_files() THEN RETURN
IF DATATYPE(filenum,'W') THEN
  DO
    IF f.filenum='' THEN
      DO
        SAY 'File number' filenum '['lib'/'filename'] does not exist!'
        RETURN
      END
    PARSE VAR f.filenum oldlib' 'oldname .
    IF lib='^' THEN lib=oldlib
    IF filename='^' THEN filename=oldname
  END
DO ii=1 TO level
  IF UPPER(lib)=UPPER(dirs.ii) THEN LEAVE ii
END
IF ii>level THEN
  DO
    SAY 'Unknown File Library:' lib 'for' filename
    IF sysoplibnum=0 | DATATYPE(filenum,'W') THEN RETURN
    SAY 'Placing' filename 'in Sysops library.'
    lib='Sysops'
    libnum=sysoplibnum
  END
ELSE libnum=ii
IF DATATYPE(filenum,'W') THEN
  DO
    IF sysoplevel>level THEN RETURN
    subpath=oldlib'/'oldname
    finfo=STATEF(bbspath'FileNotes/'subpath)
    x=OPEN(f,bbspath'FileNotes/'subpath,'R')
    IF x=0 THEN
      DO
        SAY bbspath'FileNotes/'subpath 'failed to open for reading!'
        RETURN
      END
    note.=''
    DO ii=1 WHILE ~EOF(f)
      note.ii=READLN(f)
    END
    CALL CLOSE(f)
    note.0=ii
    IF note.ii='' THEN note.0=ii-1
    IF UPPER(lib)~=UPPER(oldlib) | UPPER(filename)~=UPPER(oldname) THEN
      DO
        IF EXISTS(libpath||subpath) THEN
          DO
            ADDRESS COMMAND 'copy' libpath||subpath libpath||lib'/'filename
            CALL DELETE(libpath||subpath)
          END
        comm='copy' bbspath'FileNotes/'subpath
        comm=comm bbspath'FileNotes/'lib'/'filename 'CLONE'
        ADDRESS COMMAND comm
        CALL DELETE(bbspath'FileNotes/'subpath)
        IF UPPER(lib)~=UPPER(oldlib) THEN
          DO
            PARSE VAR note.3 front'Lib:' .
            note.3=front'Lib:' lib
            SAY '  Moved:' filename 'from' oldlib 'to' lib 'library.'
          END
        IF UPPER(filename)~=UPPER(oldname) THEN
          DO
            PARSE VAR note.2 'Name:' . 'Size: 'back
            note.2='Name:' LEFT(filename,27)' Size: 'back
            SAY 'Renamed:' oldname 'to' filename 'in the' lib 'library.'
          END
        f.filenum=lib filename
        CALL DELETE(libpath||lib'/.'STRIP(LEFT(lib,15)))
      END
    IF keywords~='^' THEN
      DO
        PARSE VAR note.1 front'KeyWords:' . 
        note.1=front'KeyWords:' keywords
        SAY 'Changed: KeyWords for' lib'/'filename'.'
      END
    IF toname~=WORD(note.3,2) & toname~='^' THEN
      DO
        PARSE VAR note.3 'From: 'fromname back
        note.3='From:' toname back
        SAY 'Changed: Uploader of' lib'/'filename 'from' fromname 'to' toname'.'
      END
    num=a.0+1
    a.0=num
    PARSE VAR note.1 . 'KeyWords: 'keywords
    a.num=LEFT(filename,22-LENGTH(WORD(note.2,4)))
    a.num=a.num WORD(note.2,4) RIGHT(filenum,5)
    a.num=a.num RIGHT(libnum,2) LEFT(lib,12)
    a.num=a.num STRIP(LEFT(STRIP(keywords),32))
    DO ii=1 TO 4
      hdr.ii=note.ii
    END
    IF EXISTS(txt) THEN
      SAY 'Changed long file description for' lib'/'filename
    ELSE IF note.0>4 THEN
      DO
        x=OPEN(f,txt,'W')
        IF x=0 THEN SAY txt 'failed to open for writing!'
        ELSE
          DO ii=5 TO note.0
            CALL WRITELN(f,note.ii)
          END
        CALL CLOSE(f)
        CALL DELAY(14)
      END
    CALL write_msg(4,bbspath'FileNotes/'lib'/'filename)
  END
ELSE
  DO
    DO ii=1 TO f.0
      IF UPPER(WORD(f.ii,2))=UPPER(filename) THEN
        DO
          SAY filename 'is already here, in the' WORD(f.ii,1) 'library.'
          RETURN
        END
    END
    lastf=get_last(bbspath'Numbers/LastFile')
    IF accessflag & sysoplevel>level THEN lib='Sysops'
    ADDRESS COMMAND 'copy' scratch'/QUICK/Files/'filename libpath||lib
    hdr.1='File:' lastf SUBSTR(hdr.1,13)
    hdr.3='From: 'LEFT(name,27)' Date: 'DATE() TIME('C')'  Lib: 'lib
    CALL write_msg(4,bbspath'FileNotes/'lib'/'filename)
    f.lastf=lib filename
    f.0=lastf
    PARSE VAR hdr.1 . 'KeyWords: 'keywords
    num=a.0+1
    a.0=num
    a.num=LEFT(filename,22-LENGTH(WORD(hdr.2,4)))
    a.num=a.num WORD(hdr.2,4) RIGHT(lastf,5)
    a.num=a.num RIGHT(libnum,2) LEFT(lib,12)
    a.num=a.num STRIP(LEFT(STRIP(keywords),32))
    upfiles=upfiles+1
    upbytes=upbytes+WORD(a.num,2)
    newf=bbspath'EMail/'sysop'/NEW_FILES'
    IF EXISTS(newf) THEN ok=OPEN(f,newf,'A')
    ELSE
      DO
        ok=OPEN(f,newf,'W')
        IF ok~=0 THEN CALL WRITELN(f,'*** New Files ***') 
      END
    IF ok~=0 THEN CALL WRITELN(f,name 'uploaded' lib'/'filename'  'DATE() TIME() 'via QUICK')
    CALL CLOSE(f)
    CALL DELETE(libpath||lib'/.'STRIP(LEFT(lib,15)))
    SAY 'Uploaded' filename 'as file' lastf 'in the' lib 'library.'
    IF WORD(lynes.24,1)='1' THEN CALL bbsNewFile.rexx(name libpath||lib'/'filename)
  END
savefiles=1
RETURN


do_msg:
IF ~EXISTS(txt) THEN
  DO
    SAY txt 'does not exist!'
    RETURN
  END
toname=WORD(hdr.3,2)
conf=WORD(hdr.5,8)
IF conf.0~='' THEN CALL load_conf()
DO ii=1 TO level
  IF UPPER(conf)=UPPER(conf.ii) THEN LEAVE ii
END
IF ii>level THEN
  DO
    SAY 'Unknown Message Conference:' conf
    RETURN
  END
confnum=ii
lastm=get_last(bbspath'Numbers/LastMessage'confnum)
hdr.1='  Msg:' lastm
hdr.5=' Date:' DATE('W') DATE() TIME('C')
hdr.5=LEFT(hdr.5,39) 'Conference:' conf
replynum=WORD(hdr.3,4)
IF DATATYPE(replynum,'W') THEN
  DO
    x=OPEN(f,msgpath'MSG'confnum'/'replynum,'R')
    IF x~=0 THEN
      DO
        data.=''
        DO ii=1 WHILE ~EOF(f)
          data.ii=READLN(f)
        END
        CALL CLOSE(f)
        data.0=ii
        IF data.ii='' THEN data.0=ii-1
        IF WORDS(data.1)>3 THEN data.1=data.1 lastm
        ELSE data.1=data.1'   Reply' lastm
        x=OPEN(f,msgpath'MSG'confnum'/'replynum,'W')
        IF x~=0 THEN
          DO ii=1 TO data.0
            CALL WRITELN(f,data.ii)
          END
        CALL CLOSE(f)
      END
  END
IF write_msg(6,msgpath'MSG'confnum'/'lastm) THEN
  DO
    SAY 'Sent message' lastm 'to' toname 'in the' conf 'conference.'
    upmsg=STRIP(upmsg confnum)
  END
IF ~EXISTS(bbspath'Users/'toname) THEN RETURN
x=OPEN(f,bbspath'Users/'toname,'R')
IF x=0 THEN
  DO
    SAY bbspath'Users/'toname 'failed to open for reading.'
    RETURN
  END
data.=''
DO ii=1 WHILE ~EOF(f)
  data.ii=READLN(f)
END
CALL CLOSE(f)
data.0=ii-1
data.24=data.24 confnum'/'lastm
x=OPEN(f,bbspath'Users/'toname,'W')
IF x=0 THEN
  DO
    SAY bbspath'Users/'toname 'failed to open for writing.'
    RETURN
  END
DO ii=1 TO data.0
  CALL WRITELN(f,data.ii)
END
CALL CLOSE(f)
RETURN


do_mail:
IF ~EXISTS(txt) THEN
  DO
    SAY txt 'does not exist!'
    RETURN
  END
toname=WORD(hdr.3,2)
mailpath=bbspath'Email/'toname
IF ~EXISTS(mailpath) THEN
  DO
    SAY mailpath 'does not exist, or failed to open!  Unable to send mail.'
    RETURN
  END
lastm=get_last(bbspath'Numbers/LastMail')
PARSE VAR hdr.1 . 'FILE: 'emailfile .
hdr.1=' Mail:' lastm
IF emailfile~='' & EXISTS(scratch'/QUICK/Files/'emailfile) THEN
  hdr.1=hdr.1'    FILE:' emailfile
hdr.5=' Date: 'DATE('W') DATE() TIME('C')
IF write_msg(6,mailpath'/'name'.'lastm) THEN SAY 'Sent email' lastm 'to' toname'.'
IF emailfile~='' & EXISTS(scratch'/QUICK/Files/'emailfile) THEN
  DO
    mailfilepath=bbspath'EmailFiles/'toname
    CALL MAKEDIR(mailfilepath)
    ADDRESS COMMAND 'copy' scratch'/QUICK/Files/'emailfile mailfilepath
    IF WORD(lynes.24,1)='1' THEN
      CALL bbsNewFile.rexx(name scratch'/QUICK/Files/'emailfile)
    hdr.1=hdr.1'    FILE:' emailfile
    SAY '...with attached file;' emailfile
  END
upmail=upmail+1
RETURN


write_msg:
PARSE ARG hdrstop,ofile
data=''
x=OPEN(f,txt,'R')
IF x=0 THEN
  DO
    IF hdrstop=6 THEN
      DO
        SAY txt 'failed to open for reading.'
        RETURN 0
      END
  END
ELSE data=READCH(f,65000)
CALL CLOSE(f)
x=OPEN(f,ofile,'W')
IF x=0 THEN
  DO
    SAY ofile 'failed to open for writing.'
    RETURN 0
  END
DO ii=1 TO hdrstop
  CALL WRITELN(f,hdr.ii)
END
IF data~='' THEN CALL WRITECH(f,data)
CALL CLOSE(f)
CALL DELETE(hdr)
CALL DELETE(txt)
RETURN 1


load_files:
IF DATATYPE(f.0,'W') THEN RETURN 0
CALL load_libs()
f.=''
IF ~listOPEN(a,bbspath'Lists/Files','R') THEN RETURN 1
f.0=0
DO ii=1 WHILE ~EOF(a)
  line=READLN(a)
  num=WORD(line,1)
  IF DATATYPE(num,'W') THEN
    DO
      f.num=WORD(line,2) WORD(line,3)
      IF num>f.0 THEN f.0=num
    END
END
CALL CLOSE(a)
RETURN 0


do_file_requests:
fr='File_Requests'
IF EXISTS(fr) THEN
  DO
    x=OPEN(f,fr,'R')
    IF x=0 THEN
      DO
        SAY fr 'failed to open for reading!'
        RETURN
      END
    fr.=''
    DO i=1 WHILE ~EOF(f)
      fr.i=READLN(f)
    END
    CALL CLOSE(f)
    fr.0=i
    IF fr.i='' THEN fr.0=i-1
    IF load_files() THEN RETURN
    selected=''
    DO i=1 TO fr.0
      num=fr.i
      IF DATATYPE(num,'W') THEN
        DO
          IF f.num='' THEN
            DO
              SAY 'File Number' num 'does not exist!'
              ITERATE i
            END
          subpath=WORD(f.num,1)'/'WORD(f.num,2)
          finfo=STATEF(bbspath'FileNotes/'subpath)
          IF finfo~='' THEN
            DO
              x=OPEN(f,bbspath'FileNotes/'subpath,'R')
              IF x=0 THEN
                DO
                  SAY bbspath'FileNotes/'subpath 'failed to open for reading!'
                  ITERATE i
                END
              note.=''
              DO j=1 WHILE ~EOF(f)
                note.j=READLN(f)
              END
              CALL CLOSE(f)
              note.0=j
              IF note.j='' THEN note.0=j-1
              PARSE VAR note.2 line'Downloads: 'num .
              IF DATATYPE(num,'W') THEN
                DO
                  note.2=line'Downloads:' num+1
                  x=OPEN(f,bbspath'FileNotes/'subpath,'W')
                  IF x=0 THEN
                    DO
                      SAY bbspath'FileNotes/'subpath 'failed to open for updating!'
                      ITERATE i
                    END
                  DO j=1 TO note.0
                    CALL WRITELN(f,note.j)
                  END
                  CALL CLOSE(f)
                END
              ELSE
                DO
                  SAY 'Unable to PARSE filenote' subpath 'for download count.'
                  SAY note.2
                END
            END
          IF EXISTS(libpath||subpath) THEN
            selected=STRIP(selected libpath||subpath)
          ELSE IF WORDS(finfo)>7 THEN
            DO
              comment=SUBSTR(finfo,WORDINDEX(finfo,8))
              IF EXISTS(comment) THEN selected=STRIP(comment selected)
            END
          ELSE SAY subpath 'is missing!  Unable to archive for download.'
        END
      ELSE IF fr.i~='' THEN
        SAY 'Unknown file request!' fr.i  'Unable to archive for download.'
    END
    SAY
    SAY 'Selected file request list:'
    DO i=1 TO WORDS(selected)
      SAY WORD(selected,i)
    END
    SAY
    ADDRESS AREXX bbsArcExt.rexx name selected
    CALL DELETE(fr)
    SAY 'File Requests have been passed to the file archiver...'
    SAY
    CALL DELAY(100)
  END
RETURN


do_file_deletes:
fr='File_Deletes'
IF ~EXISTS(fr) THEN RETURN
x=OPEN(f,fr,'R')
IF x=0 THEN
  DO
    SAY fr 'failed to open for reading!'
    RETURN
  END
fr.=''
DO i=1 WHILE ~EOF(f)
  fr.i=READLN(f)
  dels=STRIP(dels fr.i)
END
CALL CLOSE(f)
fr.0=i
IF fr.i='' THEN fr.0=i-1
IF load_files() THEN RETURN
DO i=1 TO fr.0
  num=fr.i
  IF DATATYPE(num,'W') THEN
    DO
      IF f.num='' THEN
        DO
          SAY 'File Number' num 'does not exist to delete!'
          ITERATE i
        END
      dlib=WORD(f.num,2)
      subpath=WORD(f.num,1)'/'dlib
      finfo=STATEF(bbspath'FileNotes/'subpath)
      IF sysoplevel>level THEN
        DO
          x=OPEN(f,bbspath'FileNotes/'subpath,'R')
          IF x=0 THEN
            DO
              SAY bbspath'FileNotes/'subpath 'failed to open for reading!'
              ITERATE i
            END
          CALL READLN(f)
          CALL READLN(f)
          from=WORD(READLN(f),2)
          CALL CLOSE(f)
          IF name~=from THEN
            DO
              SAY subpath 'not deleted.'
              SAY 'You may only delete files you have uploaded yourself.'
              ITERATE i
            END
        END
      IF EXISTS(libpath||subpath) THEN
        DO
          IF DELETE(libpath||subpath)=0 THEN
            SAY 'Failed to delete' libpath||subpath
        END
      ELSE IF WORDS(finfo)>7 THEN
        DO
          comment=SUBSTR(finfo,WORDINDEX(finfo,8))
          IF EXISTS(comment) THEN
            IF DELETE(comment)=0 THEN SAY 'Failed to delete' comment
        END
      ELSE SAY subpath 'is missing!  Unable to delete.'
      IF DELETE(bbspath'FileNotes/'subpath)=0 THEN
        SAY 'Failed to delete' bbspath'FileNotes/'subpath
      f.num=''
      savefiles=1
      CALL DELETE(libpath||dlib'/.'STRIP(LEFT(dlib,15)))
      SAY 'Deleted' subpath', file number' num'.'
    END
  ELSE IF STRIP(fr.i)~='' THEN SAY 'Unknown delete request!' fr.i
END
SAY
CALL DELETE(fr)
RETURN


load_libs:
sysoplibnum=0
dirs.=''
IF ~listOPEN(a,bbspath'Lists/Libraries','R') THEN CALL GETOUT(24)
DO ii=1
  line=READLN(a)
  IF EOF(a) | line='END' THEN LEAVE ii
  num=WORD(line,1)
  IF DATATYPE(num,'W') THEN
    DO
      num=num%1
      IF num>0 & num<100 THEN
        DO
          dirs.num=WORD(line,2)
          IF UPPER(dirs.num)='SYSOPS' THEN sysopslibnum=num
        END
    END
END
CALL CLOSE(a)
RETURN


load_conf:
conf.=''
IF ~listOPEN(a,bbspath'Lists/Conferences','R') THEN CALL GETOUT(25)
conf.=''
DO ii=1
  line=READLN(a)
  IF EOF(a) | line='END' THEN LEAVE ii
  num=WORD(line,1)
  IF DATATYPE(num,'W') THEN
    DO
      num=num%1
      IF num>0 & num<100 THEN conf.num=WORD(line,2)
    END
END
CALL CLOSE(a)
RETURN


do_user_change:
uc='User_Changes'
IF ~EXISTS(uc) THEN RETURN
x=OPEN(f,uc,'R')
IF x=0 THEN
  DO
    SAY uc 'failed to open for reading!'
    RETURN
  END
uc.=''
DO i=1 WHILE ~EOF(f)
  uc.i=READLN(f)
END
CALL CLOSE(f)
uc.0=i
IF uc.i='' THEN uc.0=i-1
DO i=1 TO uc.0
  PARSE VAR uc.i uname mins lev exc
  x=OPEN(f,bbspath'Users/'uname,'R')
  IF x=0 THEN
    DO
      SAY uname 'does not appear to be a member of' bbsname
      ITERATE i
    END
  DO j=1
    line=READLN(f)
    IF EOF(f) THEN LEAVE j
    data.j=line
  END
  CALL CLOSE(f)
  data.0=j-1
  IF DATATYPE(mins,'W') THEN data.11=mins 'minutes' calls 'more calls today.'
  IF DATATYPE(lev,'W') THEN data.20=lev
  IF STRIP(exc)~='' THEN data.21=exc
  x=OPEN(f,bbspath'Users/'uname,'W')
  IF x=0 THEN
    DO
      SAY uname 'did not open for writing!'
      ITERATE i
    END
  DO j=1 TO data.0
    CALL WRITELN(f,data.j)
  END
  CALL CLOSE(f)
END
CALL DELETE(uc)
RETURN


get_last:
PARSE ARG statname
x=OPEN(f,statname,'R')
IF x=0 THEN
  DO
    SAY statname 'failed to open for reading!'
    RETURN 0
  END
last=READLN(f)
CALL CLOSE(f)
IF DATATYPE(last,'W') THEN last=last+1
ELSE RETURN 0
OPTIONS FAILAT 20
ADDRESS COMMAND 'ECHO >'statname last
RETURN last


listOPEN:
PARSE ARG fh,listfile,flag
CALL CLOSE(fh)
DO i=0 TO 59 WHILE OPEN(fh,listfile,flag)=0
  IF i//4=0 THEN SAY 'Waiting' (60-i)*5 'more seconds for' listfile 'to become available...'
  CALL DELAY(250)
END
IF i>59 THEN
  DO
    line='*** unable to access' listfile 'list.'
    SAY line
    RETURN 0
  END
RETURN 1


checkliblists:
SAY 'Checking individual library filelists...'
liblist=''
lastlib=0
cnt.=0
DO i=1 TO 99
  IF dirs.i='' THEN ITERATE i
  IF ~EXISTS(libpath||dirs.i'/.'STRIP(LEFT(dirs.i,15))) THEN liblist=liblist i
END
liblist=STRIP(liblist)
DO j=1 TO WORDS(liblist)
  tt=WORD(liblist,j)
  CALL MAKEDIR(libpath||dirs.tt)
  lf=libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15))
  flg='W'
  IF EXISTS(libpath||dirs.tt'.txt') THEN
    DO
      ADDRESS COMMAND 'COPY' libpath||dirs.tt'.txt' lf
      flg='A'
    END
  IF listOPEN(f,lf,flg)=0 THEN ITERATE j
  IF flg='A' THEN CALL WRITELN(f,'')
  CALL WRITELN(f,'Filename          Bytes File# Library         KeyWords')
  CALL WRITELN(f,LEFT('=',77,'='))
  CALL CLOSE(f)
END
RETURN


updateliblists:
PARSE ARG alpha 
x=FIND(liblist,WORD(alpha,4))
IF x=0 THEN RETURN
tt=WORD(liblist,x)
IF tt~=lastlib THEN
  DO
    CALL CLOSE(b)
    lastlib=tt
    IF OPEN(b,libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15)),'A')=0 THEN
      DO
        lastlib=0
        RETURN
      END
  END
CALL WRITELN(b,alpha)
cnt.tt=cnt.tt+1
RETURN


closeliblists:
CALL CLOSE(b)
DO i=1 TO WORDS(liblist)
  tt=WORD(liblist,i)
  dname='.'STRIP(LEFT(dirs.tt,15))
  SAY ' 'dname
  x=OPEN(f,libpath||dirs.tt'/'dname,'A')
  IF x~=0 THEN
    DO
      CALL WRITELN(f,LEFT('-',77,'-'))
      temp='file'
      IF cnt.tt>1 THEN temp=temp's'
      temp=cnt.tt temp'.  Last updated' DATE() 'at' TIME('C')
      temp=temp RIGHT(bbsname,76-LENGTH(temp))
      CALL WRITELN(f,temp)
      CALL CLOSE(f)
    END
  CALL MAKEDIR(bbspath'FileNotes/'dirs.tt)
  fnote=bbspath'FileNotes/'dirs.tt'/'dname
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
  finfo=STATEF(libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15)))
  bt=WORD(finfo,2)
  dl=WORD(lynes.2,7)
  IF ~DATATYPE(dl,'W') THEN dl=0
  lynes.1='File: 'LEFT(tt,5)' KeyWords: alphabetical files list CONTENTS'
  lynes.2='Name: 'LEFT(dname,27)' Size:' bt 'bytes  Downloads:' dl
  lynes.3='From: 'LEFT('BBBBS',27)' Date: 'DATE() TIME('C')'  Lib: 'dirs.tt
  lynes.4=LEFT('',74,'=')
  IF lynes.5='' THEN
    lynes.5='Up to the minute alphabetical filelist of the' dirs.tt 'library.'
  IF OPEN(f,fnote,'W')~=0 THEN
    DO
      DO k=1 TO lynes.0
        CALL WRITELN(f,lynes.k)
      END
      CALL CLOSE(f)
      SAY LEFT(' ',LENGTH(dname)+2)'1B'x'Mupdated.'
    END
END
RETURN


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


GETOUT:
ARG err 
IF err>0 THEN SAY 'Error:' err'  RC='RC'  SIGL='SIGL

ERROR:
SYNTAX:
BREAK_C:
FAILURE:
IF RC>0 THEN SAY 'RC='RC'  SIGL='SIGL
IF GETCLIP('BBS_'name)='QUICKIN' THEN CALL SETCLIP('BBS_'name)
CALL SETCLIP('BBS_QUICKIN'clipnum)
EXIT err

/* bbsQUICKIN.rexx */
