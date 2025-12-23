/* $VER: 8.1 bbsArcExt.rexx (17.9.94)
copyright 1990-94 Richard Lee Stockton FREELY DISTRIBUTABLE

Each archive should be no larger than 800,000 bytes
All combined, the uncompressed files must be less than SIZE_LIMIT
*/
SIZE_LIMIT=5000000   /* 5 megs */

LF='0A'x
PARSE ARG name selected 
IF selected='' THEN EXIT
bbspath=GETCLIP('BBS_path')
CALL CLOSE(STDOUT)
CALL OPEN(STDOUT,'RAM:ArcExt.STDOUT','W')

bytes=0
tbytes=0
s.=''
s.0=1
lasti=0
DO i=1 TO WORDS(selected)
  info=STATEF(WORD(selected,i))
  IF info='' THEN ITERATE i
  bytes=bytes+WORD(info,2)
  tbytes=tbytes+WORD(info,2)
  IF tbytes>SIZE_LIMIT THEN LEAVE i
  IF bytes>800000 & (i-lasti)>1 THEN
    DO
      n=s.0
      IF bytes>999999 & WORDS(s.n)>1 THEN i=i-1
      s.0=s.0+1
      lasti=i
      IF bytes>999999 & WORDS(s.n)>1 THEN
        DO
          bytes=0
          ITERATE i
        END
      bytes=0
    END
  n=s.0
  s.n=STRIP(s.n WORD(selected,i))
END

fdir=bbspath'EmailFiles/'name
CALL MAKEDIR(fdir)

x=OPEN(f,bbspath'Numbers/LastMail','R')
IF x=0 THEN EXIT 25
lastm=READLN(f)
CALL CLOSE(f)
IF DATATYPE(lastm,'W') THEN lastm=lastm+1
ELSE lastm=1

DO i=1 TO s.0
  ADDRESS COMMAND 'ECHO >'bbspath'Numbers/LastMail 'lastm
  x=OPEN(f,'RAM:ArcList','W')
  IF x=0 THEN EXIT 26
  CALL WRITELN(f,s.i) /* list in a file can be of unlimited length */
  CALL CLOSE(f)
  OPTIONS FAILAT 999
  comm='LhA -2amNrZ a' fdir'/BBBBS_'lastm'.lha @RAM:ArcList'
  ADDRESS COMMAND comm
  temp=WORD(s.i,WORDS(s.i))
  dev=LEFT(temp,POS(':',temp)-1)
  x=OPEN(f,fdir'/BBBBS_'lastm'.lha.xdl','W')
  IF x=0 THEN EXIT 27
  CALL WRITELN(f,dev)
  CALL WRITELN(f,WORDS(s.i))
  CALL CLOSE(f)
  x=OPEN(f,bbspath'Email/'name'/BBBBS.'lastm,'W')
  IF x=0 THEN EXIT 28
  CALL WRITELN(f,' Mail: 'lastm'   FILE: BBBBS_'lastm'.lha')
  CALL WRITELN(f,' From: BBBBS')
  CALL WRITELN(f,'   To: 'name)
  temp=' Subj: Archive of selected external device files'
  IF s.0>1 THEN temp=temp', part' i 'of' s.0'.'
  CALL WRITELN(f,temp)
  CALL WRITELN(f,' Date: 'DATE('W') DATE() TIME('C'))
  CALL WRITELN(f,LEFT('=',75,'='))
  CALL WRITELN(f,'Here is the archive of files you requested.')
  DO j=1 TO WORDS(s.i)
    CALL WRITELN(f,WORD(s.i,j))
  END
  IF i=s.0 & tbytes>SIZE_LIMIT THEN
    CALL WRITELN(f,SIZE_LIMIT 'byte size limit exceeded! Other files ignored.')
  CALL CLOSE(f)
  lastm=lastm+1
END

newmess='Your file archive is waiting in Email.'
IF GETCLIP('BBS_level')~='' & WORD(GETCLIP('BBS_lastcaller'),1)=name THEN
  DO
    oldmess=GETCLIP('BBS_MESSAGE')
    IF oldmess~='' THEN oldmess=oldmess||'0D0A'x
    CALL SETCLIP('BBS_MESSAGE',oldmess||newmess)
  END
IF GETCLIP('BBS_LOCAL')=name THEN CALL SETCLIP('BBS_LOCAL_MSG',newmess)

/* bbsArcExt.rexx */
