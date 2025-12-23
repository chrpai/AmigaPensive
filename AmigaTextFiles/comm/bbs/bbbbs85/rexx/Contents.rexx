/*          $VER: Contents.rexx 8.5 (3.6.95)
copyright 1990-95 Richard Lee Stockton  FREELY DISTRIBUTABLE

     Required in C: directory   arc dms lha lzx unarj unzip zoo
*/

PARSE ARG argname emptyflag .
IF emptyflag~=1 THEN CALL DELETE('RAM:CONTENTS')
endtest=UPPER(RIGHT(argname,4))
arcomm=''
IF endtest='.ARC' THEN arcomm='c:arc'
IF endtest='.ZOO' THEN arcomm='c:zoo'
IF endtest='.LZX' THEN arcomm='c:lzx'
IF arcomm~=''     THEN arcomm=arcomm 'v' argname
ELSE IF endtest='.ARJ' THEN arcomm='c:unarj l' argname
ELSE IF endtest='.DMS' THEN arcomm='c:dms VIEW' argname
ELSE IF endtest='.ZIP' THEN arcomm='c:unzip -v' argname
ELSE IF endtest='.LHA' | endtest='.LZH' | endtest='.RUN' THEN
  arcomm='c:lha -N v' argname
arcomm='CD RAM:' || '0A'x || arcomm
IF emptyflag~=1 THEN
  DO
    CALL CLOSE(STDOUT)
    CALL OPEN(STDOUT,'RAM:CONTENTS','W')
  END
ADDRESS COMMAND arcomm
IF emptyflag~=1 THEN
  DO
    CALL CLOSE(STDOUT)
    IF WORD(STATEF('RAM:CONTENTS'),2)=0 THEN CALL Contents.rexx(argname 1)
  END

/* Contents.rexx */
