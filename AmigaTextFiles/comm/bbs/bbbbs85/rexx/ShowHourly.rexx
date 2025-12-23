/* $VER: ShowHourly.rexx 8.4 (25.1.95)
copyright 1993-95 Richard Lee Stockton
      FREELY DISTRIBUTABLE
*/

PARSE SOURCE . . . prg .
ADDRESS AREXX Increment.rexx prg

CR=''
def='[0m'
pen3='[33m'
bbspath=GETCLIP('BBS_path')
PARSE ARG name lpp colorflag nonstop .
IF lpp='' THEN
  DO
    lpp=99
    def=''
    pen3=''
  END
IF colorflag=0 THEN
  DO
    def=''
    pen3=''
  END
IF nonstop='' THEN
  DO
    nonstop=1
    CALL CLOSE(STDOUT)
    CALL OPEN(STDOUT,bbspath'EmailFiles/'name'/Notices/HOURLY','W')
  END
IF nonstop=1 THEN lpp=99
IF ADDRESS()='BAUD' THEN CR='0D'x

hc.=0
hc.24=countcheck(bbspath'Numbers/Hourly/Start' 0)
IF hc.24=0 THEN hc.25=1
ELSE hc.25=1+DATE('I')-hc.24
hc.26=countcheck(bbspath'Numbers/Hourly/Hour' 0)
hc.27=TIME('H')
DO i=0 TO 23
  temp=hc.25
  IF temp>1 & i>hc.27 THEN temp=temp-1
  hc.i=countcheck(bbspath'Numbers/Hourly/'i 0)%temp
END
IF hc.24=0 THEN hc.24=DATE('I')
SAY CR
SAY pen3'        Average minutes per hour of use each day since' DATE(,hc.24,'I')||def||CR
SAY LEFT('-',68,'-')||CR
line=' Hour:  ********10********20********30********40********50********60'
SAY line||CR
DO i=0 TO 23
  IF i=0 THEN temp=12'am'
  ELSE IF i<12 THEN temp=i'am'
  ELSE IF i=12 THEN temp='12pm'
  ELSE temp=i-12'pm'
  SAY RIGHT(temp,5)':  'pen3||LEFT('*',hc.i,'*')||def||CR
  IF i=(lpp-4) THEN
    IF waiting2() THEN LEAVE i
END
SAY line||CR
SAY LEFT('-',68,'-')||CR
EXIT


waiting2:
waitchar=getinput(pen3'   Q=Quit   RETURN=Continue  'def)
IF waitchar='Q' THEN RETURN 1
SAY '1B'x'M'||LEFT(' ',78)||CR||'1B'x'M'
RETURN 0


getinput:
PARSE ARG pline
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
inarg=UPPER(inarg)
inarg=LEFT(inarg,1)
RETURN inarg


countcheck:
PARSE ARG fname' 'cknum .
IF ~EXISTS(fname) THEN
  DO
    IF cknum=0 THEN RETURN 0
    IF ~writeopen(fname) THEN RETURN 0
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN cknum
  END
IF ~readopen(fname) THEN RETURN cknum
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'W') THEN retval=0
IF ~DATATYPE(cknum,'W') THEN cknum=0
IF retval<cknum THEN
  DO
    IF writeopen(fname) THEN
      DO
        CALL WRITELN(f,cknum)
        CALL CLOSE(f)
        RETURN cknum
      END
  END
RETURN retval


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
SAY fname 'failed to open for reading!'CR
RETURN 0


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok~=0 THEN RETURN 1
SAY fname 'failed to open for writing!'CR
RETURN 0


/* ShowHourly.rexx */
