/* $VER: bbsULDL.rexx 8.3 (18.11.94)
- counts upload/download statistics -
copyright 1990-94 Richard Lee Stockton
       - FREELY DISTRIBUTABLE -
*/

/* exclude = dirs or files excluded from the "most popular file" list */
/* This is a space separated list. files need to be  dir/filename  */
exclude=''
exclude=UPPER(exclude)

CALL TIME('R')
HOW_MANY=25
CALL PRAGMA('P',-2)
SIGNAL ON ERROR
SIGNAL ON SYNTAX
SIGNAL ON FAILURE
NUMERIC DIGITS 14

inarg=''
PARSE ARG inarg .
test=COMPRESS(UPPER(inarg),' -')
IF test='?' | test='H' | test='USAGE' | test='HELP' THEN
  DO
    SAY 'Usage: rx bbsULDL [filename or CLI]'
    SAY
    SAY 'CLI writes to the console.'
    SAY 'If a filename or the letters CLI are not included,'
    SAY 'writes to a file called Information/STATS.ULDL'
  END

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
    EXIT(20)
  END

CALL SETCLIP('BBS_STAT','ON')
lynes.=''
DO i=1 TO 25
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
    CALL SETCLIP('BBS_STAT')
    EXIT(20)
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
IF WORD(lunes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'

IF inarg='' THEN
  DO
    CALL CLOSE(STDOUT)
    CALL OPEN(STDOUT,scratch'/bbsULDL.STDOUT','W')
    SAY SUBSTR(SOURCELINE(1),10)
  END
counts.=0
popular.=0
largest.=0
namelist=''
dir.=''
x=OPEN(f,bbspath'Lists/Libraries','R')
IF x=0 THEN
  DO
    SAY 'Could not open' bbspath'Lists/Libraries!'
    CALL SETCLIP('BBS_STAT')
    EXIT(20)
  END
DO i=1
  line=READLN(f)
  IF EOF(f) | UPPER(line)='END' THEN LEAVE i
  num=WORD(line,1)
  IF DATATYPE(num,'W') THEN dir.num=WORD(line,2)
END
CALL CLOSE(f)

totfiles=0
DO h=1 TO 99
  IF dir.h='' THEN ITERATE h
  fyles=SHOWDIR(bbspath'FileNotes/'dir.h)
  totfiles=totfiles+WORDS(fyles)
  DO i=1 TO WORDS(fyles)
    fname=WORD(fyles,i)
    filenote=bbspath'FileNotes/'dir.h'/'fname
    x=OPEN(f,filenote,'R')
    IF x=0 THEN
      DO
        SAY filenote 'did not open!'
        ITERATE i
      END
    line1=READLN(f)
    line2=READLN(f)
    line3=READLN(f)
    CALL CLOSE(f)
    fnum=WORD(line1,2)
    size=WORD(line2,4)
    dls=WORD(line2,7)
    name=WORD(line3,2)
    IF name~=UPPER(sysop) THEN
      IF FIND(namelist,name)=0 THEN namelist=namelist name
    counts.name.BYTES=counts.name.BYTES+size
    counts.TOTAL.BYTES=counts.TOTAL.BYTES+size
    counts.name.ULOADS=counts.name.ULOADS+1
    counts.TOTAL.DLOADS=counts.TOTAL.DLOADS+dls
    IF dls>popular.HOW_MANY THEN
      DO
        IF FIND(exclude,UPPER(dir.h))>0 | FIND(exclude,UPPER(dir.h'/'fname)) THEN ITERATE i
        line=RIGHT(dls,4) LEFT(fname,22) RIGHT(fnum,5) LEFT(dir.h,18) LEFT(name,20)
        popular.HOW_MANY=dls
        popular.HOW_MANY.0=line
        DO j=HOW_MANY-1 TO 1 BY -1
          jj=j+1
          IF popular.jj<=popular.j THEN LEAVE j
          temp=popular.jj
          temp0=popular.jj.0
          popular.jj=popular.j
          popular.jj.0=popular.j.0
          popular.j=temp
          popular.j.0=temp0
        END
      END
  END
END

DO nameindex=1 TO WORDS(namelist)
  name=WORD(namelist,nameindex)
  IF counts.name.BYTES>largest.HOW_MANY THEN
    DO
      line=LEFT(name,20) LEFT(' ',21) RIGHT(comma(counts.name.BYTES),11) 'bytes'RIGHT(comma(counts.name.ULOADS),5) 'files'
      largest.HOW_MANY=counts.name.BYTES
      largest.HOW_MANY.0=line
      DO j=HOW_MANY-1 TO 1 BY -1
        jj=j+1
        IF largest.jj<=largest.j THEN LEAVE j
        temp=largest.jj
        temp0=largest.jj.0
        largest.jj=largest.j
        largest.jj.0=largest.j.0
        largest.j=temp
        largest.j.0=temp0
      END
    END
END


/* OUTPUT */

IF inarg='' THEN inarg=bbspath'Information/STATS.ULDL'
IF UPPER(inarg)~='CLI' THEN
  DO
    CALL CLOSE(STDOUT)
    CALL OPEN(STDOUT,inarg,'W')
    IF x=0 THEN
      DO
        SAY inarg 'would not open for writing!'
        CALL SETCLIP('BBS_STAT')
        EXIT(20)
      END
  END

SAY LEFT('-',75,'-')
SAY TRIM(CENTER('Upload/Download statistics through' DATE() 'at' TIME('C'),75))
SAY TRIM(CENTER(bbsname,75))
SAY TRIM(CENTER(comma(counts.TOTAL.BYTES) 'bytes in' comma(totfiles) 'files uploaded by' comma(WORDS(namelist)) 'of' comma(WORDS(SHOWDIR(bbspath'Users'))) 'current users.',75))
SAY LEFT('-',75,'-')

SAY
SAY
SAY CENTER('Top' HOW_MANY 'favorite files from' comma(counts.TOTAL.DLOADS) 'total downloads',75)
SAY LEFT('-',75,'-')
SAY '  DLoads' LEFT('Filename',22) 'File#' LEFT('Library',18) LEFT('Uploader',20)
SAY LEFT('-',75,'-')
DO i=1 TO HOW_MANY
  IF popular.i=0 THEN LEAVE i
  SAY RIGHT(i,2)'.' popular.i.0
END

SAY
SAY
SAY CENTER('Top' HOW_MANY 'uploaders of files currently in the libraries (excludes sysop)',75)
SAY LEFT('-',75,'-')
DO i=1 TO HOW_MANY
  IF largest.i=0 THEN LEAVE i
  x=OPEN(f,bbspath'Users/'STRIP(WORD(largest.i.0,1)),'R')
  IF x=0 THEN ITERATE i
  CALL READLN(f)
  CALL READLN(f)
  city=READLN(f)
  CALL CLOSE(f)
  city=TRANSLATE(city,'     ','+-.*/')
  DO j=WORDS(city) TO 1 BY -1
    IF DATATYPE(WORD(city,j),'N') THEN city=STRIP(DELWORD(city,j,1))
    IF UPPER(WORD(city,j))='USA' THEN city=STRIP(DELWORD(city,j,1))
  END
  city=SPACE(city,1)
  SAY RIGHT(i,2)'.' OVERLAY(city,largest.i.0,22,21)
END
SAY
t=(.5+TIME('E'))%1
min=t%60
hrs=min%60
min=min//60
sec=t//60
temp=' - It took'
IF hrs>1 THEN temp=temp hrs 'hours'
ELSE IF hrs=1 THEN temp=temp '1 hour'
IF min>1 THEN temp=temp min 'minutes'
ELSE IF min=1 THEN temp=temp '1 minute'
temp=temp sec 'seconds to compile this report. -'
SAY temp
SAY
CALL SETCLIP('BBS_STAT')
EXIT;


comma:
ARG num .
dgt=LENGTH(num)
numtext=''
IF dgt>3 THEN numtext=','RIGHT(num,3)
IF dgt>6 THEN numtext=','LEFT(RIGHT(num,6),3)||numtext
IF dgt>9 THEN
  DO
    numtext=','LEFT(RIGHT(num,9),3)||numtext
    numtext=LEFT(num,dgt-9)||numtext
  END
ELSE IF dgt>6 THEN numtext=LEFT(num,dgt-6)||numtext
ELSE IF dgt>3 THEN numtext=LEFT(num,dgt-3)||numtext
ELSE numtext=num
RETURN(numtext)


SYNTAX:
FAILURE:
ERROR:
SAY 'Line:' SIGL ERRORTEXT(RC)
SAY
SAY line1
SAY line2
SAY line3
CALL SETCLIP('BBS_STAT')
EXIT;

/* end of uldl.rexx */
