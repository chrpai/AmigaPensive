/* $VER: bbsUSER.rexx 8.3 (18.11.94)
counts upload/download statistics
copyright 1990-94 Richard Lee Stockton 
      - FREELY DISTRIBUTABLE -
*/

HOW_MANY=10
CALL PRAGMA('P',-2)
SIGNAL ON ERROR
SIGNAL ON SYNTAX
SIGNAL ON FAILURE
NUMERIC DIGITS 14

CALL TIME('R')
PARSE ARG inarg .
test=COMPRESS(UPPER(inarg),' -')
IF test='?' | test='H' | test='USAGE' | test='HELP' THEN
  DO
    SAY 'Usage: rx bbsUSER [filename or CLI]'
    SAY
    SAY 'CLI writes to the console.'
    SAY 'If a filename or the letters CLI are not included,'
    SAY 'writes to a file called Information/STATS.USER'
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
    CALL OPEN(STDOUT,scratch'/bbsUSER.STDOUT','W')
    SAY SUBSTR(SOURCELINE(1),10)
  END

counts.=0
DO j=1 TO HOW_MANY
  counts.LOW.j=999999999
END
counts.AGE_YOUNGEST=99999999
ten=0
thirty=0
ninty=0
sixmo=0
ndate=DATE('I')
userlist=SHOWDIR(bbspath'Users','F')
x=FIND(userlist,sysop)
IF x>0 THEN userlist=DELWORD(userlist,x,1)
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
  IF j<23 THEN ITERATE i
  callcount=WORD(lynes.19,6)
  counts.CALLS=counts.CALLS+callcount
  DO j=HOW_MANY TO 1 BY -1 WHILE callcount>counts.CALLS.j
    jj=j+1
    counts.CALLS.jj=counts.CALLS.j
    counts.ID.CALLS.jj=counts.ID.CALLS.j
    counts.CALLS.j=callcount
    counts.ID.CALLS.j=thisuser
  END
  ontime=WORD(lynes.19,1)*60+WORD(lynes.19,3)
  counts.MINUTES=counts.MINUTES+ontime
  DO j=HOW_MANY TO 1 BY -1 WHILE ontime>counts.ON_MINS.j
    jj=j+1
    counts.ON_MINS.jj=counts.ON_MINS.j
    counts.ID.ON_MINS.jj=counts.ID.ON_MINS.j
    counts.ON_MINS.j=ontime
    counts.ID.ON_MINS.j=thisuser
  END
  IF WORDS(lynes.12)>3 & DATATYPE(WORD(lynes.12,4),'N') THEN
    DO
      counts.AGE_INDEX=counts.AGE_INDEX+1
      age=0
      IF UPPER(WORD(lynes.12,3))='AGE:' THEN
        DO
          age=WORD(lynes.12,4)
          counts.AGE_TOTAL=counts.AGE_TOTAL+age
        END
      ELSE IF UPPER(WORD(lynes.12,3))='BIRTHDAY:' THEN
        DO
          age=LEFT(DATE('S'),4)-LEFT(WORD(lynes.12,4),4)
          counts.AGE_TOTAL=counts.AGE_TOTAL+age
          IF SUBSTR(DATE('S'),5,2)<SUBSTR(WORD(lynes.12,4),5,2) THEN
            DO
              counts.AGE_TOTAL=counts.AGE_TOTAL-1
              age=age-1
            END
        END
      IF age>0 & age<counts.AGE_YOUNGEST THEN counts.AGE_YOUNGEST=age
      IF age>counts.AGE_OLDEST THEN counts.AGE_OLDEST=age
    END
  idate=DATE('I',STRIP(WORD(lynes.13,1)),'S')
  days=ndate-idate
  IF days<11 THEN ten=ten+1
  IF days<31 THEN thirty=thirty+1
  IF days<91 THEN ninty=ninty+1
  IF days<183 THEN sixmo=sixmo+1
  upfiles=0
  upbytes=1
  dnfiles=0
  dnbytes=1
  IF WORDS(lynes.14)>3 THEN
    DO
      upfiles=WORD(lynes.14,1)
      upbytes=WORD(lynes.14,3)
    END
  IF WORDS(lynes.15)>3 THEN
    DO
      dnfiles=WORD(lynes.15,1)
      dnbytes=WORD(lynes.15,3)
    END
  IF upbytes<1 THEN upbytes=1
  counts.UP_FILES=counts.UP_FILES+upfiles
  counts.UP_BYTES=counts.UP_BYTES+upbytes
  counts.DN_FILES=counts.DN_FILES+dnfiles
  counts.DN_BYTES=counts.DN_BYTES+dnbytes
  DO j=HOW_MANY TO 1 BY -1 WHILE upfiles>counts.HIGH.UP_FILES.j
    jj=j+1
    counts.HIGH.UP_FILES.jj=counts.HIGH.UP_FILES.j
    counts.ID.UP_FILES.jj=counts.ID.UP_FILES.j
    counts.HIGH.UP_FILES.j=upfiles
    counts.ID.UP_FILES.j=thisuser
  END
  DO j=HOW_MANY TO 1 BY -1 WHILE upbytes>counts.HIGH.UP_BYTES.j
    jj=j+1
    counts.HIGH.UP_BYTES.jj=counts.HIGH.UP_BYTES.j
    counts.ID.UP_BYTES.jj=counts.ID.UP_BYTES.j
    counts.HIGH.UP_BYTES.j=upbytes
    counts.ID.UP_BYTES.j=thisuser
  END
  DO j=HOW_MANY TO 1 BY -1 WHILE dnfiles>counts.HIGH.DN_FILES.j
    jj=j+1
    counts.HIGH.DN_FILES.jj=counts.HIGH.DN_FILES.j
    counts.ID.DN_FILES.jj=counts.ID.DN_FILES.j
    counts.HIGH.DN_FILES.j=dnfiles
    counts.ID.DN_FILES.j=thisuser
  END
  DO j=HOW_MANY TO 1 BY -1 WHILE dnbytes>counts.HIGH.DN_BYTES.j
    jj=j+1
    counts.HIGH.DN_BYTES.jj=counts.HIGH.DN_BYTES.j
    counts.ID.DN_BYTES.jj=counts.ID.DN_BYTES.j
    counts.HIGH.DN_BYTES.j=dnbytes
    counts.ID.DN_BYTES.j=thisuser
  END
  IF (dnbytes=1)&(upbytes=1) THEN NOP
  ELSE
    DO j=HOW_MANY TO 1 BY -1 WHILE (dnbytes/upbytes)<counts.LOW.j
      jj=j+1
      counts.LOW.jj=counts.LOW.j
      counts.ID.LOW.jj=counts.ID.LOW.j
      counts.LOW.j=dnbytes/upbytes
      counts.ID.LOW.j=thisuser
    END
  DO j=HOW_MANY TO 1 BY -1 WHILE (dnbytes/upbytes)>counts.HIGH.j
    jj=j+1
    counts.HIGH.jj=counts.HIGH.j
    counts.ID.HIGH.jj=counts.ID.HIGH.j
    counts.HIGH.j=dnbytes/upbytes
    counts.ID.HIGH.j=thisuser
  END
  totwrit=0
  DO j=1 TO 99
    thisnum=WORD(lynes.23,j)
    IF DATATYPE(thisnum,'N') THEN totwrit=totwrit+thisnum
  END
  DO j=HOW_MANY TO 1 BY -1 WHILE totwrit>counts.HIGH.MSGS.j
    jj=j+1
    counts.HIGH.MSGS.jj=counts.HIGH.MSGS.j
    counts.ID.MSGS.jj=counts.ID.MSGS.j
    counts.HIGH.MSGS.j=totwrit
    counts.ID.MSGS.j=thisuser
  END
END


/* OUTPUT */

IF inarg='' THEN inarg=bbspath'Information/STATS.USER'
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
SAY CENTER(bbsname 'USER statistics through' DATE() 'at' TIME('C'),75)
SAY LEFT('-',75,'-')
SAY CENTER('Does not include sysop.',75)
SAY LEFT('-',75,'-')
SAY
IF counts.AGE_INDEX>0 THEN
  SAY CENTER('Of' WORDS(userlist) 'total users,' counts.AGE_INDEX 'gave their ages.',75)
  SAY CENTER('The youngest is' counts.AGE_YOUNGEST'    The oldest is' counts.AGE_OLDEST'    The average is' TRUNC(counts.AGE_TOTAL/counts.AGE_INDEX+.005,2),75)
SAY
SAY RIGHT(comma(counts.UP_BYTES),15) 'bytes in' RIGHT(comma(counts.UP_FILES),7) 'files have been uploaded by these users.'
SAY RIGHT(comma(counts.DN_BYTES),15) 'bytes in' RIGHT(comma(counts.DN_FILES),7) 'files have been downloaded by these users.'
SAY
SAY
SAY 'Most public  messages:' RIGHT(comma(counts.HIGH.MSGS.1),14) counts.ID.MSGS.1
DO j=2 TO HOW_MANY
  IF counts.HIGH.MSGS.j>0 THEN
    SAY RIGHT(comma(counts.HIGH.MSGS.j),37) counts.ID.MSGS.j
END
SAY
SAY 'Most files   uploaded:' RIGHT(comma(counts.HIGH.UP_FILES.1),14) counts.ID.UP_FILES.1
DO j=2 TO HOW_MANY
  IF counts.HIGH.UP_FILES.j>0 THEN
    SAY RIGHT(comma(counts.HIGH.UP_FILES.j),37) counts.ID.UP_FILES.j
END
SAY
SAY 'Most bytes   uploaded:' RIGHT(comma(counts.HIGH.UP_BYTES.1),14) counts.ID.UP_BYTES.1
DO j=2 TO HOW_MANY
  IF counts.HIGH.UP_BYTES.j>0 THEN
    SAY RIGHT(comma(counts.HIGH.UP_BYTES.j),37) counts.ID.UP_BYTES.j
END
SAY
SAY 'Most files downloaded:' RIGHT(comma(counts.HIGH.DN_FILES.1),14) counts.ID.DN_FILES.1
DO j=2 TO HOW_MANY
  IF counts.HIGH.DN_FILES.j>0 THEN
    SAY RIGHT(comma(counts.HIGH.DN_FILES.j),37) counts.ID.DN_FILES.j
END
SAY
SAY 'Most bytes downloaded:' RIGHT(comma(counts.HIGH.DN_BYTES.1),14) counts.ID.DN_BYTES.1
DO j=2 TO HOW_MANY
  IF counts.HIGH.DN_BYTES.j>0 THEN
    SAY RIGHT(comma(counts.HIGH.DN_BYTES.j),37) counts.ID.DN_BYTES.j
END
SAY
SAY
SAY 'Best  (lowest)  ratio, downloads divided by uploads:'
DO j=1 TO HOW_MANY
  IF counts.LOW.j<999999999 THEN
    SAY RIGHT(comma(counts.LOW.j%1)||RIGHT(TRUNC(counts.LOW.j+.000005,5),6),37) counts.ID.LOW.j
END
SAY
SAY 'Worst (highest) ratio, downloads divided by uploads:'
DO j=1 TO HOW_MANY
  IF counts.HIGH.j>0 THEN
    SAY RIGHT(comma(counts.HIGH.j%1),37) counts.ID.HIGH.j
END
SAY
SAY 'Most Calls To' bbsname':'
DO j=1 TO HOW_MANY
  IF counts.CALLS.j>0 THEN
    SAY RIGHT(comma(counts.CALLS.j%1),37) counts.ID.CALLS.j
END
SAY
SAY 'Most Time Spent On' bbsname':'
DO j=1 TO HOW_MANY
  IF counts.ON_MINS.j>0 THEN
    DO
      temp=comma(counts.ON_MINS.j%60) 'hours'
      temp2=counts.ON_MINS.j//60
      IF temp2<10 THEN temp2='0'temp2
      temp=RIGHT(temp temp2 'minutes',37)
      SAY temp counts.ID.ON_MINS.j
    END
END
SAY
SAY RIGHT(comma(counts.MINUTES),37) 'minutes.'
SAY RIGHT(comma(counts.CALLS),37) 'calls.'
SAY RIGHT('----------',39)
SAY RIGHT(TRUNC(.05+counts.MINUTES/counts.CALLS,1),39) 'minutes per call.'
SAY
users=WORDS(userlist)
SAY RIGHT(comma(users),7) 'users.'
SAY RIGHT(comma(sixmo),7)',' RIGHT(((sixmo*100)/users)%1,2)'%, have called in the last 6 months.'
SAY RIGHT(comma(ninty),7)',' RIGHT(((ninty*100)/users)%1,2)'%, have called in the last 90 days.'
SAY RIGHT(comma(thirty),7)',' RIGHT(((thirty*100)/users)%1,2)'%, have called in the last 30 days.'
SAY RIGHT(comma(ten),7)',' RIGHT(((ten*100)/users)%1,2)'%, have called in the last 10 days.'
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


comma: PROCEDURE
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
SAY thisuser
SAY
CALL SETCLIP('BBS_STAT')
EXIT;

/* end of bbsUSER.rexx */
