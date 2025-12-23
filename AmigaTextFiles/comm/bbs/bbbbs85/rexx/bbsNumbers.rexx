/* $VER: bbsNumbers.rexx 8.3 (17.11.94) */

NUMERIC DIGITS 14

CALL SETCLIP('BBS_STATS',1)
bbspath=GETCLIP('BBS_path')
ADDRESS AREXX bbsLog99.rexx 'NUMBERS' RIGHT(TIME('C'),7) DATE()

in.=''
in.0=0
numdat=bbspath'Logs/Numbers.dat'
dailyfile=bbspath'Logs/log.'DATE('S')
IF OPEN(f,numdat,'R')~=0 THEN
  DO
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN LEAVE i
      in.i=line
    END
    in.0=i-1
    CALL CLOSE(f)
  END
CALL FileList(bbspath'Numbers/*',out,'F')
IF OPEN(ff,numdat,'W')=0 THEN
  DO
    CALL SETCLIP('BBS_STATS')
    EXIT
  END
DO i=1 TO out.0
  IF OPEN(f,out.i,'R')=0 THEN ITERATE i
  num=READLN(f)
  CALL CLOSE(f)
  test=UPPER(SUBSTR(out.i,LASTPOS('/',out.i)+1))
  IF test='FIRSTLOGON' | test='LASTBOOT' | test='LASTDEMONBOOT' THEN ITERATE i
  CALL WRITELN(ff,LEFT(num,20) out.i)
  DO j=1 TO in.0
    IF UPPER(WORD(in.j,2))=UPPER(out.i) THEN
      DO
        IF WORD(in.j,1)>num THEN
          DO
            flag='W'
            IF EXISTS(dailyfile) THEN flag='A'
            IF OPEN(f,dailyfile,flag)~=0 THEN
              DO
                line='NUMBERS ERROR!' WORD(in.j,2) WORD(in.j,1) 'is now' num
                CALL WRITELN(f,line)
                CALL CLOSE(f)
                CALL SETCLIP('BBS_ERROR','NUMBERS ERROR, See log.'DATE('S') '!')
                ADDRESS AREXX bbsLog99.rexx 'NUMBERS' RIGHT(TIME('C'),7) DATE() line
              END
          END
        ITERATE i
      END
  END
END
CALL CLOSE(ff)
CALL SETCLIP('BBS_STATS')

/* bbsNumbers.rexx */
