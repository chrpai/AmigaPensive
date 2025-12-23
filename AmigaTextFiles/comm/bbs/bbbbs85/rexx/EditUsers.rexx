/* EditUsers.rexx  -  writes one word in one line for ALL bbs userfiles */

SAY '*** WARNING ***  This can change ALL user data! Be sure!  *** WARNING ***'

linenumber=''
wordnumber=''
newvalue=''
doit=''

PARSE ARG linenumber wordnumber newvalue doit .
IF ~DATATYPE(linenumber,'N') | ~DATATYPE(wordnumber,'N') THEN
  SIGNAL USAGE
IF UPPER(newvalue)='DOIT' THEN
  DO
    doit='DOIT'
    newvalue=''
  END

IF UPPER(doit)='DOIT' THEN doit=1
ELSE doit=0
userlist=SHOWDIR('BBBBS:Users')
DO i=1 TO WORDS(userlist)
  x=OPEN(f,'BBBBS:Users/'WORD(userlist,i),'R')
  IF x=0 THEN ITERATE i
  lines.=''
  DO j=1
    line=READLN(f)
    IF EOF(f) THEN LEAVE j
    lines.j=line
  END
  CALL CLOSE(f)
  lines.0=j-1
  IF WORDS(lines.linenumber)>=wordnumber THEN
    DO
      windx=WORDINDEX(lines.linenumber,wordnumber)
      lines.linenumber=DELWORD(lines.linenumber,wordnumber,1)
      IF windx=1 THEN lines.linenumber=newvalue'  'STRIP(lines.linenumber)
      ELSE lines.linenumber=INSERT(newvalue' ',lines.linenumber,windx-1)
      IF doit THEN
        DO
          x=OPEN(f,'BBBBS:Users/'WORD(userlist,i),'W')
          IF x=0 THEN ITERATE i
          DO j=1 TO lines.0
            CALL WRITELN(f,STRIP(lines.j))
          END
          CALL CLOSE(f)
        END
    END
  SAY lines.linenumber WORD(userlist,i)
END
EXIT

USAGE:
SAY 'Usage> rx EditUsers linenumber wordnumber newvalue DOIT'
SAY 'Test your selection first, then add the DOIT at the end.'
EXIT
