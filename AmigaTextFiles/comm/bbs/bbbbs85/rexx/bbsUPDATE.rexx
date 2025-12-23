/* $VER: Update.rexx 8.3 (12.12.94) */

CALL TIME('R')
SAY
SAY SUBSTR(SOURCELINE(1),4)
SAY
SAY 'This macro updates the file libraries for ALL previous'
SAY 'versions of BBBBS to work with BBBBS version 8.0 and above.'
SAY 'It will abort before trying to update twice, so ''no worries''.'
SAY
SAY 'It will renumber the first 101 files by shuffling them into'
SAY 'blank spots in the filelist, or adding to the end if that is'
SAY 'necessary. It then saves the filelist and calls BuildALPHA.rexx.'
SAY
OPTIONS PROMPT 'Continue? (nY) > '
PULL junk
IF LEFT(junk,1)='N' THEN
  DO
    OPTIONS PROMPT 'Not renumbering is probably not a good idea, are you sure? (Ny) > '
    PULL junk
    IF LEFT(junk,1)='Y' THEN EXIT
  END
figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
IF readopen(figarg)=0 THEN
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

bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    EXIT
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'

libpath=WORD(lynes.8,1)
IF ~EXISTS(libpath) THEN
  DO
    SAY libpath 'does not exist!'
    EXIT
  END
testchar=RIGHT(libpath,1)
IF testchar~='/' & testchar~=':' THEN libpath=libpath'/'

dirs.=''
IF readopen(bbspath'Lists/Libraries') THEN
  DO
    SAY 'Loading Library list...'
    DO i=1
      line=READLN(f)
      IF line='END' | EOF(f) THEN LEAVE i
      num=WORD(line,1)
      IF DATATYPE(num,'W') THEN dirs.num=STRIP(WORD(line,2))
    END
    CALL CLOSE(f)
  END
ELSE EXIT

file.=''
file.0=0
gottadoit=0
IF readopen(bbspath'Lists/Files') THEN
  DO
    SAY 'Loading Current Files list...'
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN LEAVE i
      num=WORD(line,1)
      IF DATATYPE(num,'W') THEN
        DO
          IF gottadoit=0 THEN
            DO
              IF num>100 THEN
                DO
                  SAY 'This file system has already been updated for BBBBS 8.0'
                  SAY
                  EXIT
                END
            END
          file.num=STRIP(WORD(line,2) WORD(line,3))
          IF num>file.0 THEN file.0=num
          IF num<100 THEN
            IF LEFT(WORD(file.num,2),1)~='.' THEN gottadoit=1 
        END
    END
    CALL CLOSE(f)
  END
ELSE EXIT
SAY

oldtop=file.0
SAY 'Renumbering files...'
lastfill=102
DO i=101 TO 1 BY -1
  IF file.i='' THEN ITERATE i
  DO j=lastfill WHILE file.j~=''
  END
  lastfill=j
  IF i=1 THEN j=101
  file.j=file.i
  fnote=bbspath'FileNotes/'WORD(file.j,1)'/'WORD(file.j,2)
  IF readopen(fnote) THEN
    DO
      lynes.=''
      DO k=1
        line=READLN(f)
        IF EOF(f) THEN LEAVE k
        lynes.k=line
      END
      CALL CLOSE(f)
      lynes.0=k-1
      lynes.1=OVERLAY(j,lynes.1,7,5)
      CALL DELAY(28)
      IF writeopen(fnote) THEN
        DO
          DO k=1 TO lynes.0
            CALL WRITELN(f,lynes.k)
          END
          CALL CLOSE(f)
          file.i=''
          SAY 'Renumbered file' i 'to' j WORD(file.j,1)'/'WORD(file.j,2)
        END
      IF j>file.0 THEN file.0=j
    END
END
SAY
IF file.0>oldtop THEN
  DO
    IF readopen(bbspath'Numbers/LastFile') THEN
      DO
        num=READLN(f)
        CALL CLOSE(f)
        CALL DELAY(50)
        IF num<file.0 THEN
          DO
            SAY 'LastFile number reset to' file.0
            IF writeopen(bbspath'Numbers/LastFile') THEN CALL WRITELN(f,file.0)
            CALL CLOSE(f)
          END
      END
  END
SAY
SAY 'Rebuilding all filelists...'
SAY
CALL BuildALPHA.rexx()
SAY
SAY 'The' bbsname 'file libraries'
SAY 'have been updated for use with BBBBS version 8.0 and above.'
SAY
EXIT


/* subroutines */

readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for reading!'
SAY line
RETURN 0


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for writing!'
SAY line
RETURN 0

/* UpdateBBBBS.rexx */
