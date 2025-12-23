/* $VER: bbsALPHA.rexx 8.1 (20.9.94)
copyright © 1991-94 Richard Lee Stockton
        FREELY DISTRIBUTABLE
*/

arcfile=''
arcfile='Text/FileList' /* delete this line for no archive of filelist */
libfile=''
libfile='Text/FilesByLib' /* delete this line for no filelist by lib */

arcfile2='FileList.txt'
libfile2='FilesByLib.txt'
IF arcfile='' & libfile='' THEN SIGNAL GETOUT

bbspath=GETCLIP('BBS_path')
libpath=GETCLIP('BBS_libpath')
libpath2=libpath
bbsname=GETCLIP('BBS_bbsname')

PARSE ARG totfiles extension arccom

IF extension='' THEN
  DO
    extension='.lzh'
    arccom='lharc >ram:junk -m m'
  END
ELSE extension='.'extension

IF arcfile~='' THEN
  DO
    arcfile=arcfile||extension
    SAY 'Updating' arcfile'...'
    CALL DELETE(libpath||arcfile)
    IF RIGHT(libpath,1)='/' THEN libpath2=LEFT(libpath,LENGTH(libpath)-1)
    x=OPEN(f,'RAM:header','W')
    IF x=0 THEN SIGNAL GETOUT
    CALL WRITELN(f,bbsname '- Complete alphabetical filelist')
    CALL WRITELN(f,totfiles 'files. - Compiled' DATE() 'at' TIME('C'))
    CALL WRITELN(f,'')
    CALL WRITELN(f,'Filename          Bytes File# Library         KeyWords')
    CALL CLOSE(f)
    CALL DELAY(99)
    ADDRESS COMMAND 'JOIN RAM:header' bbspath'Lists/Files.ALPHA' 'AS' libpath||arcfile2
    ADDRESS COMMAND 'CD' libpath2'0A'x||arccom arcfile arcfile2
    CALL DELETE('ram:junk')
    lynes.=''
    sz=WORD(STATEF(libpath||arcfile),2)
    x=OPEN(f,bbspath'FileNotes/'arcfile,'R')
    IF x=0 THEN
      DO
        lib=LEFT(arcfile,POS('/',arcfile)-1)
        lynes.1='File: 101  KeyWords: full alphabetical CONTENTS list'
        lynes.2='Name: 'LEFT(libfile2,27)' Size:' sz 'bytes  Downloads: 0'
        lynes.3='From: 'LEFT('BBBBS',27)' Date: 'DATE() TIME('C')'  Lib:' lib
        lynes.4=LEFT('',74,'=')
        lynes.0=5
        CALL SETCLIP('BBS_101',arcfile)
      END
    ELSE
      DO
        DO i=1
          line=READLN(f)
          IF EOF(f) THEN LEAVE i
          lynes.i=line
        END
        lynes.0=i-1
        CALL CLOSE(f)
        lynes.2=OVERLAY(sz 'bytes',lynes.2,41)
        lynes.3=OVERLAY(DATE() TIME('C')' ',lynes.3,41)
      END
    lynes.5='Complete up to the minute alphabetical filelist.' totfiles 'files.'
    x=OPEN(f,bbspath'FileNotes/'arcfile,'W')
    IF x=0 THEN SIGNAL GETOUT
    DO i=1 TO lynes.0
      CALL WRITELN(f,lynes.i)
    END
    CALL CLOSE(f)
  END

IF libfile~='' THEN
  DO
    libfile=libfile||extension
    IF OPEN(f,bbspath'Lists/Libraries','R')~=0 THEN
      DO
        j=0
        dirs.=''
        DO i=1
          line=READLN(f)
          IF line='END' | EOF(f) THEN LEAVE i
          num=WORD(line,1)
          IF DATATYPE(num,'W') THEN
            DO
             j=j+1
             dirs.j=WORD(line,2)
            END
        END
        CALL CLOSE(f)
        dirs.0=j
        CALL QSORT(1,dirs.0,dirs)
      END
    ELSE
      DO
        SAY 'Lists/Libraries failed to open!  Aborting...'
        SIGNAL GETOUT
      END
    CALL DELETE(libpath||libfile)
    libpath2=libpath
    IF RIGHT(libpath,1)='/' THEN libpath2=LEFT(libpath,LENGTH(libpath)-1)
    x=OPEN(a,libpath||libfile2,'W')
    IF x=0 THEN SIGNAL GETOUT
    SAY 'Constructing FilesByLib list...'
    CALL WRITELN(a,bbsname '- Complete alphabetical filelist, sorted by library')
    CALL WRITELN(a,totfiles 'files. - Compiled' DATE() 'at' TIME('C'))
    DO i=1 TO dirs.0
      CALL WRITELN(a,'')
      x=OPEN(f,libpath||dirs.i'/.'STRIP(LEFT(dirs.i,15)),'R')
      IF x=0 THEN
        DO
          SAY libpath||dirs.i'/.'STRIP(LEFT(dirs.i,15)) 'failed to open!'
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
    SAY 'Updating' libfile'...'
    comm='CD' libpath2'0A'x||arccom libfile libfile2
    ADDRESS COMMAND comm
    CALL DELETE('ram:junk')
    CALL DELETE(libpath||libfile2)
    lynes.=''
    sz=WORD(STATEF(libpath||libfile),2)
    x=OPEN(f,bbspath'FileNotes/'libfile,'R')
    IF x=0 THEN
      DO
        lib=LEFT(libfile,POS('/',libfile)-1)
        lynes.1='File: 100  KeyWords: alphabetical CONTENTS by library'
        lynes.2='Name: 'LEFT(libfile2,27)' Size:' sz 'bytes  Downloads: 0'
        lynes.3='From: 'LEFT('BBBBS',27)' Date: 'DATE() TIME('C')'  Lib:' lib
        lynes.4=LEFT('',74,'=')
        lynes.0=5
        CALL SETCLIP('BBS_100',libfile)
      END
    ELSE
      DO
        DO i=1
          line=READLN(f)
          IF EOF(f) THEN LEAVE i
          lynes.i=line
        END
        lynes.0=i-1
        CALL CLOSE(f)
        lynes.2=OVERLAY(sz 'bytes',lynes.2,41)
        lynes.3=OVERLAY(DATE() TIME('C')' ',lynes.3,41)
      END
    lynes.5='Up to the minute alphabetical filelist, sorted by library.' totfiles 'files.'
    x=OPEN(f,bbspath'FileNotes/'libfile,'W')
    IF x=0 THEN SIGNAL GETOUT
    DO i=1 TO lynes.0
      CALL WRITELN(f,lynes.i)
    END
    CALL CLOSE(f)
  END

GETOUT:
CALL SETCLIP('BBS_maint')
EXIT
RETURN

/* bbsALPHA.rexx */
