/* $VER: TestArc.rexx 8.5 (15.4.95)
copyright 1990-95 Richard Lee Stockton
       FREELY DISTRIBUTABLE

     Required in C: directory:
    arc dms lha lzx unarj unzip zoo
*/

OPTIONS FAILAT 999
CR='0D'x
LF='0A'x

PARSE ARG argname .
endtest=UPPER(RIGHT(argname,4))
CALL DELETE('RAM:file_id.diz')

arcomm=''
IF endtest='.ARC' THEN arcomm='c:arc >RAM:TestArc.txt t'
ELSE IF endtest='.ARJ' THEN arcomm='c:unarj >RAM:TestArc.txt t'
ELSE IF endtest='.DMS' THEN arcomm='c:dms >RAM:TestArc.txt VIEW'
ELSE IF endtest='.ZOO' THEN arcomm='c:zoo >RAM:TestArc.txt -t'
ELSE IF endtest='.ZIP' THEN arcomm='c:unzip >RAM:TestArc.txt -t'
ELSE IF endtest='.LZX' THEN arcomm='c:lzx >RAM:TestArc.txt t'
ELSE IF endtest='.LHA' | endtest='.LZH' THEN
  arcomm='c:lha >RAM:TestArc.txt -N t'

IF arcomm='' THEN EXIT 0
IF ~EXISTS(WORD(arcomm,1)) THEN
  DO
    SAY 'Can''t check archive integrity,' WORD(arcomm,1) 'does not exist!'CR
    EXIT 0
  END

SAY 'Testing archive integrity...'CR
ADDRESS COMMAND arcomm argname
err=RC
IF err>0 THEN
  DO
    CALL DELAY(100)
    IF OPEN(f,'RAM:TestArc.txt','R')~=0 THEN
      DO i=1 WHILE ~EOF(f)
        SAY READLN(f)||CR
      END
    CALL CLOSE(f)
    EXIT err
  END

arcomm=''
IF endtest='.ARC' THEN arcomm='c:arc >RAM:TestArc.txt e'
ELSE IF endtest='.ARJ' THEN arcomm='c:unarj >RAM:TestArc.txt e'
ELSE IF endtest='.ZOO' THEN arcomm='c:zoo >RAM:TestArc.txt -e'
ELSE IF endtest='.ZIP' THEN arcomm='c:unzip >RAM:TestArc.txt -j'
ELSE IF endtest='.LZX' THEN arcomm='c:lzx >RAM:TestArc.txt e'
ELSE IF endtest='.LHA' | endtest='.LZH' THEN
  arcomm='c:lha >TestArc.txt -Nx0 e'

IF arcomm='' | ~EXISTS(WORD(arcomm,1)) THEN EXIT 0
ADDRESS COMMAND 'CD RAM:'LF||arcomm argname 'file_id.diz'
SAY ' - Archive tested OK -'CR
EXIT 0

/* TestArc.rexx */
