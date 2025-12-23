/* $VER: 8.4 bbsHourly.rexx (25.1.95)
copyright 1990-95 Richard Lee Stockton FREELY DISTRIBUTABLE
keeps track of minutes of use per hour
*/

thishour=0
prevhour=0
prevhour2=0
prevhour3=0

PARSE ARG hour min hours mins hourly_dir .
mins=mins+hours*60

CALL MAKEDIR(hourly_dir)
IF ~EXISTS(hourly_dir'/Hour') THEN
  CALL countcheck(hourly_dir'/Hour' TIME('H'))
IF ~EXISTS(hourly_dir'/Start') THEN
  CALL countcheck(hourly_dir'/Start' DATE('I'))

IF min>=mins THEN thishour=mins
ELSE
  DO
    thishour=min
    prevhour=mins-min
    IF prevhour>60 THEN
      DO
        prevhour2=prevhour-60
        prevhour=60
        IF prevhour2>60 THEN
          DO
            prevhour3=prevhour2-60
            prevhour2=60
          END
      END
  END

IF hour=0 THEN
  DO
    prev=23
    prev2=22
    prev3=21
  END
ELSE IF hour=1 THEN
  DO
    prev=0
    prev2=23
    prev3=22
  END
ELSE IF hour=2 THEN
  DO
    prev=1
    prev2=0
    prev3=23
  END
ELSE
  DO
    prev=hour-1
    prev2=hour-2
    prev3=hour-3
  END

thishour=thishour+countcheck(hourly_dir'/'hour 0)
CALL countcheck(hourly_dir'/'hour thishour)

IF prevhour>0 THEN
  DO
    prevhour=prevhour+countcheck(hourly_dir'/'prev 0)
    CALL countcheck(hourly_dir'/'prev prevhour)
  END

IF prevhour2>0 THEN
  DO
    prevhour2=prevhour2+countcheck(hourly_dir'/'prev2 0)
    CALL countcheck(hourly_dir'/'prev2 prevhour2)
  END

IF prevhour3>0 THEN
  DO
    prevhour3=prevhour3+countcheck(hourly_dir'/'prev3 0)
    CALL countcheck(hourly_dir'/'prev3 prevhour3)
  END

EXIT


countcheck:
PARSE ARG fname' 'cknum .
IF ~EXISTS(fname) THEN
  DO
    IF ~writeopen(fname) THEN RETURN(0)
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN(cknum)
  END
IF ~readopen(fname) THEN RETURN(cknum)
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'N') THEN retval=0
IF ~DATATYPE(cknum,'N') THEN cknum=0
IF retval<cknum THEN
  DO
    IF writeopen(fname) THEN
      DO
        CALL WRITELN(f,cknum)
        CALL CLOSE(f)
        RETURN(cknum)
      END
  END
RETURN(retval)


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN(1)
line=fname 'failed to open for reading!'
SAY line
RETURN(0)


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok~=0 THEN RETURN(1)
line=fname 'failed to open for writing!'
SAY line
RETURN(0)

/* bbsHourly.rexx */
