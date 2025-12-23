/*         $VER: bbsREPORT.rexx 8.5 (25.4.95)
copyright 1991-95 Richard Lee Stockton FREELY DISTRIBUTABLE
  
               - reports user inactivity -
       - any user who hasn't called for 365 days -
   - less than 3 calls with last older than 270 days -
       - only 1 call made more than 180 days ago -
     - CHANGE THESE LIMITS IF YOU DON'T LIKE THEM! -
         - also will report filelist problems -
*/

HOW_MANY=100    /* how many old files to put on report */
MIN_DAYS=30     /* don't report files younger than this (in days) */

PARSE SOURCE . . . prg .
ADDRESS AREXX Increment.rexx prg

CALL TIME('R')
least.=999999999999
CALL PRAGMA('P',-2)
SIGNAL ON ERROR
SIGNAL ON SYNTAX
SIGNAL ON FAILURE
NUMERIC DIGITS 14

ARG name what2do 
temp=STRIP(WORD(what2do,WORDS(what2do)))
IF DATATYPE(temp,'N') THEN SMALLEST=temp
ELSE SMALLEST=0

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
sysop   = UPPER(WORD(lynes.2,1))
bbspath = WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    EXIT(20)
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'

libpath            = WORD(lynes.8,1)
IF ~EXISTS(libpath) THEN
  DO
    MSG libpath 'does not exist!'
    EXIT(20)
  END
testchar=RIGHT(libpath,1)
IF testchar~='/' & testchar~=':' THEN libpath=libpath'/'


IF FIND(what2do,'USERS')=0 THEN SIGNAL FILEREPORT

inarg=bbspath'EMail/'name'/INACTIVE_USERS'
CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,inarg,'W')
IF x=0 THEN
  DO
    SAY inarg 'would not open for writing!'
    EXIT(20)
  END

userlist=SHOWDIR(bbspath'Users','F')
x=FIND(userlist,sysop)
IF x>0 THEN userlist=DELWORD(userlist,x,1)
SAY LEFT('USER_NAME',28)'days last_session calls location'
ndate=DATE('I')
ten=0
thirty=0
ninty=0
sixmo=0
DO i=1 TO WORDS(userlist)
  thisuser=WORD(userlist,i)
  x=OPEN(f,bbspath'Users/'thisuser,'R')
  IF x=0 THEN
    DO
      SAY thisuser 'userfile would not open!'
      ITERATE i
    END
  lynes.=''
  DO j=1 TO 23
    line=READLN(f)
    IF EOF(f) THEN LEAVE j
    lynes.j=line
  END
  CALL CLOSE(f)
  idate=0
  add2list=0
  IF DATATYPE(WORD(lynes.13,1),'N') THEN
    DO
      idate=DATE('I',STRIP(WORD(lynes.13,1)),'S')
      age=ndate-idate
      IF age<11 THEN ten=ten+1
      IF age<31 THEN thirty=thirty+1
      IF age<91 THEN ninty=ninty+1
      IF age<183 THEN sixmo=sixmo+1
      IF FIND(lynes.21,'VIP_USER')>0 THEN NOP
      ELSE IF (idate+365)<ndate THEN add2list=365
      ELSE IF WORD(lynes.19,6)<4 & (idate+270)<ndate THEN add2list=270
      ELSE IF WORD(lynes.19,6)<2 & (idate+180)<ndate THEN add2list=180
      IF add2list>0 THEN
        DO
          line=LEFT(thisuser,28) RIGHT(add2list,3)'  'DATE(,WORD(lynes.13,1),'S')
          line=line'  'WORD(lynes.19,6)'  'lynes.3
          SAY line
        END
    END
  ELSE SAY 'Bad Date!' thisuser lynes.13
END
SAY
SAY RIGHT(WORDS(userlist),5) 'users.'
SAY RIGHT(sixmo,5) 'of these users have called within the last 6 months.'
SAY RIGHT(ninty,5) 'of these users have called within the last 90 days.'
SAY RIGHT(thirty,5) 'of these users have called within the last 30 days.'
SAY RIGHT(ten,5) 'of these users have called within the last 10 days.'
SAY
t=(.5+TIME('E'))%1
min=t%60
hrs=min%60
min=min//60
secs=t//60
temp=' - It took'
IF hrs>1 THEN temp=temp hrs 'hours'
ELSE IF hrs=1 THEN temp=temp '1 hour'
IF min>1 THEN temp=temp min 'minutes'
ELSE IF min=1 THEN temp=temp '1 minute'
IF secs=1 THEN temp=temp '1 second'
ELSE IF secs>0 THEN temp=temp secs 'seconds'
temp=temp 'to compile this report -'
SAY temp
SAY
IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  CALL SETCLIP('BBS_MESSAGE','INACTIVE_USERS report is waiting in Email.')
CALL TIME('R')



FILEREPORT:
IF FIND(what2do,'FILES')=0 THEN EXIT(0)

inarg=bbspath'EMail/'name'/FILELISTS_REPORT'
CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,inarg,'W')
IF x=0 THEN
  DO
    SAY inarg 'would not open for writing!'
    EXIT(20)
  END

SAY ' - FileReport -' DATE('W')'  'DATE()'  'TIME('C')
SAY LEFT('-',75,'-')

dirs.='0000000'
IF readopen(bbspath'Lists/Libraries') THEN
  DO
    DO i=1
      line=READLN(f)
      IF line='END' | EOF(f) THEN LEAVE i
      num=WORD(line,1)
      IF DATATYPE(num,'N') THEN dirs.num=STRIP(WORD(line,2))
    END
    CALL CLOSE(f)
  END

problem=0
IF readopen(bbspath'Lists/Files') THEN
  DO
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN BREAK
      listfile=libpath||COMPRESS(WORD(line,2)'/'WORD(line,3),' ')
      IF EXISTS(listfile) THEN ITERATE i
      link=STATEF(bbspath'FileNotes/'COMPRESS(WORD(line,2)'/'WORD(line,3)))
      link=WORD(link,8)
      realfile=listfile
      IF link~='' THEN realfile=link
      IF EXISTS(realfile) THEN ITERATE i
      temp=listfile
      IF temp~=realfile THEN temp=temp '('realfile')'
      SAY temp 'does not exist!'
      problem=1
    END
    CALL CLOSE(f)
    devs.0=di
  END
IF ~problem THEN SAY 'All files in the master list exist as links or in the libraries.'
ELSE SAY 'The above files in the master list are not available for downloading!'
SAY i-1 'files listed in ''Lists/Files''.'
xlist='.JPG .GIF .LHA .LZH .ARC .ZOO .ZIP .RUN .EXE .PAK .LHW .LZX'
xlist=xlist '.ARJ .WRP .ZOM .DMS .CPT .SEA .SIT .TXT .MPG .JPE JPEG'
k=0
kk=0
devs.=0
devlist=UPPER(LEFT(libpath,POS(':',libpath)-1))
DO i=1 TO 99
  IF dirs.i='' | dirs.i='0000000' THEN ITERATE i
  realfiles=SHOWDIR(libpath||dirs.i)
  k=k+WORDS(realfiles)
  DO j=1 TO WORDS(realfiles)
    DO
      file=dirs.i'/'STRIP(WORD(realfiles,j))
      IF ~EXISTS(bbspath'FileNotes/'file) THEN
        SAY 'No file description for' file WORD(STATEF(libpath||file),2) 'bytes.'
    END
  END
  notes=SHOWDIR(bbspath'FileNotes/'dirs.i)
  DO j=1 TO WORDS(notes)
    thisnote=bbspath'FileNotes/'dirs.i'/'STRIP(WORD(notes,j))
    x=OPEN(f,thisnote,'R')
    IF x=0 THEN ITERATE j
    line1=READLN(f)
    line2=READLN(f)
    line3=READLN(f)
    CALL CLOSE(f)
    kk=kk+1
    di=1
    IF ~EXISTS(libpath||dirs.i'/'STRIP(WORD(notes,j))) THEN
      DO
        link=WORD(STATEF(thisnote),8)
        IF link='' THEN ITERATE j
        ELSE
          DO
            IF EXISTS(link) THEN
              DO
                k=k+1
                x=POS(':',link)
                IF x>2 THEN
                  DO
                    dev=UPPER(LEFT(link,x-1))
                    di=FIND(devlist,dev)
                    IF di=0 THEN
                      DO
                        devlist=devlist dev
                        di=WORDS(devlist)
                      END
                  END
              END
            ELSE ITERATE j
          END
      END
    filenum=WORD(line1,2)
    IF FIND(xlist,UPPER(RIGHT(thisnote,4)))=0 & LEFT(WORD(notes,j),1)~='.' THEN
      SAY 'File' filenum WORD(notes,j) 'in' dirs.i 'may be unarchived.'
    nname=WORD(line2,2)
    size=WORD(line2,4)
    dlds=WORD(line2,7)
    dirs.i.1=dirs.i.1+1      /*  files this dir */
    dirs.i.2=dirs.i.2+size   /*  bytes this dir */
    dirs.i.3=dirs.i.3+dlds   /* dloads this dir */
    devs.di.1=devs.di.1+1    /*  files this device */
    devs.di.2=devs.di.2+size /*  bytes this device */
    devs.di.3=devs.di.3+dlds /* dloads this device */
    IF size<SMALLEST | SMALLEST<0 THEN ITERATE j
    uldate=norm2seq(SUBWORD(line3,4,3))
    IF uldate<19780000 | uldate>99991231 THEN
      DO
        SAY 'Bad Date! 'SUBWORD(line3,4,3) thisnote
        ITERATE j
      END
    IF DATE('I',uldate,'S')>(DATE('I')-MIN_DAYS) THEN ITERATE j
    DO jj=HOW_MANY TO 1 BY -1
      IF dlds>least.jj THEN ITERATE j
      IF dlds=least.jj & uldate>least.jj.0 THEN ITERATE j
      jjj=jj+1
      least.jjj=least.jj
      least.jjj.0=least.jj.0
      least.jjj.1=least.jj.1
      least.jj=dlds
      least.jj.0=uldate
      least.jj.1=RIGHT(size,8) RIGHT(filenum,6) LEFT(dirs.i,16) nname
    END
  END
END

SAY kk 'filenotes.'
SAY k 'actual files in libraries.'

IF readopen(bbspath'Lists/Files.ALPHA') THEN
  DO
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN BREAK
    END
    CALL CLOSE(f)
  END
SAY i-1 'files listed in ''Lists/Files.ALPHA''.'
SAY
IF SMALLEST~<0 THEN
  DO
    SAY ' - The 'HOW_MANY' least downloaded, oldest files greater -'
    SAY ' - than' comma(SMALLEST) 'byte(s) in the current libraries -'
    SAY RIGHT('dls',5) LEFT('Date',11) RIGHT('Bytes',8) 'File #' LEFT('Library',16) 'Filename'
    SAY LEFT('-',62,'-')
    DO i=1 TO HOW_MANY
      IF least.i>99991231 THEN ITERATE i
      SAY RIGHT(least.i,5) DATE(,least.i.0,'S') least.i.1
    END
    SAY
  END
SAY ' - LIBRARY STATISTICS -'
SAY RIGHT('Lib#',4) LEFT('Library Name',16) RIGHT('Files',5) RIGHT('Bytes',14) RIGHT('Downloads',9) RIGHT('AverageSize',11) RIGHT('DL/file',7)
SAY LEFT('-',72,'-')
tots.=0
DO i=1 TO 99
  IF dirs.i='' | dirs.i=0 THEN ITERATE i
  DO j=1 TO 3
    tots.j=tots.j+dirs.i.j
  END
  temp=dirs.i.1
  IF temp<1 THEN temp=1
  SAY RIGHT(i,4) LEFT(dirs.i,16) RIGHT(comma(dirs.i.1),5) RIGHT(comma(dirs.i.2),14) RIGHT(comma(dirs.i.3),9) RIGHT(comma((.5+dirs.i.2/temp)%1),11) RIGHT(TRUNC(dirs.i.3/temp,2),7)
END
SAY
SAY LEFT('',21) RIGHT('Files',5) RIGHT('Bytes',14) RIGHT('Downloads',9) RIGHT('AverageSize',11) RIGHT('DL/file',7)
SAY LEFT('-',72,'-')
x=WORDS(devlist)
IF x>1 THEN
  DO
    DO i=1 TO x
      temp=devs.i.1
      IF temp<1 THEN temp=1
      SAY RIGHT(WORD(devlist,i)':',21) RIGHT(comma(devs.i.1),5) RIGHT(comma(devs.i.2),14) RIGHT(comma(devs.i.3),9) RIGHT(comma((.5+devs.i.2/temp)%1),11) RIGHT(TRUNC(devs.i.3/temp,2),7)
    END
    SAY LEFT('-',72,'-')
  END
temp=tots.1
IF temp<1 THEN temp=1
SAY RIGHT('TOTALS:',21) RIGHT(comma(tots.1),5) RIGHT(comma(tots.2),14) RIGHT(comma(tots.3),9) RIGHT(comma((.5+tots.2/temp)%1),11) RIGHT(TRUNC(tots.3/temp,2),7)
SAY
t=(.5+TIME('E'))%1
min=t%60
hrs=min%60
min=min//60
secs=t//60
temp=' - It took'
IF hrs>1 THEN temp=temp hrs 'hours'
ELSE IF hrs=1 THEN temp=temp '1 hour'
IF min>1 THEN temp=temp min 'minutes'
ELSE IF min=1 THEN temp=temp '1 minute'
IF secs=1 THEN temp=temp '1 second'
ELSE IF secs>0 THEN temp=temp secs 'seconds'
temp=temp 'to compile this report -'
SAY temp
SAY
IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  CALL SETCLIP('BBS_MESSAGE','FILELISTS_REPORT is waiting in Email.')
EXIT(0);


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN(1)
line=fname 'failed to open for reading!'
SAY line
RETURN(0)


norm2seq:     /* takes '3 Jan 1990' and turns it into '19900103' */
ARG da' 'mo' 'yr .
da=da/1
mo=STRIP(mo)
yr=yr/1
IF da<10 THEN da='0'da
SELECT
  WHEN mo='JAN' THEN mo='01'
  WHEN mo='FEB' THEN mo='02'
  WHEN mo='MAR' THEN mo='03'
  WHEN mo='APR' THEN mo='04'
  WHEN mo='MAY' THEN mo='05'
  WHEN mo='JUN' THEN mo='06'
  WHEN mo='JUL' THEN mo='07'
  WHEN mo='AUG' THEN mo='08'
  WHEN mo='SEP' THEN mo='09'
  WHEN mo='OCT' THEN mo='10'
  WHEN mo='NOV' THEN mo='11'
  WHEN mo='DEC' THEN mo='12'
  OTHERWISE RETURN(-1);
END
RETURN(yr||mo||da);


comma: PROCEDURE
ARG num .
num=num%1
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
EXIT;

/* end of bbsREPORT.rexx */
