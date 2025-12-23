/* $VER: bbsFriends.rexx 8.3 (18.1.95)
copyright © 1994-95 Richard Lee Stockton
BBBBS email alias handler
FREELY DISTRIBUTABLE
Thanks to John Ruckart for this idea and much of this code
*/

SIGNAL ON BREAK_C

ARG name colorflag .

IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END
ELSE
  DO
    CR=''
    frombb=0
  END

def='[0m'
pen3='[33m'
IF colorflag=0 THEN
  DO
    def=''
    pen3=''
  END
lineup='1B'x'M'

namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
bbspath=GETCLIP('BBS_path')
aliasexclude='SYSOP BYE OFF FL QUICK'
CALL loadFriends()

ch=''
DO WHILE ch~='Q'
  SAY CR
  SAY pen3||LEFT('=',75,'=')def||CR
  SAY CENTER('F R I E N D S - L I S T',75)||CR
  SAY CR
  SAY CENTER('A L I A S   E D I T O R',75)||CR
  SAY pen3||LEFT('=',75,'=')def||CR
  SAY CR
  SAY '                           'pen3'W - 'def'What is the Friends List? 'CR
  SAY '                           'pen3'A - 'def'Add an Alias 'CR
  SAY '                           'pen3'D - 'def'Delete an Alias 'CR
  SAY '                           'pen3'V - 'def'View my Aliases 'CR
  SAY '                           'pen3'Q - 'def'Quit to previous menu'CR
  SAY CR
  ch=getinput(1 1 pen3'Enter Choice > 'def)
  SELECT
    WHEN ch='W' THEN CALL whatFriends()
    WHEN ch='A' THEN CALL addalias()
    WHEN ch='D' THEN CALL delalias()
    WHEN ch='V' THEN CALL viewalias()
    WHEN ch='Q' THEN CALL saveFriends()
    OTHERWISE SAY 'No such command'CR
  END
END
EXIT


loadFriends:
CALL MAKEDIR(bbspath'Friends')
alias.=''
alias.0=0
realname.=''
CALL CLOSE(f)
IF OPEN(f,bbspath'Friends/'name,'R')=0 THEN RETURN 1
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  alias.i=WORD(line,1)
  realname.i=WORD(line,2)
END
alias.0=i-1
CALL CLOSE(f)
RETURN 1


saveFriends:
frn=bbspath'Friends/'name
IF alias.0<1 THEN
  DO
    CALL DELETE(frn)
    RETURN
  END
CALL OPEN(f,frn,'W')
DO i=1 TO alias.0
  CALL WRITELN(f,alias.i'  'realname.i)
END
CALL CLOSE(f)
RETURN


whatFriends:
IF OPEN(f,bbspath'Information/BBBBS.Friends','R')=0 THEN RETURN
SAY CR
DO i=1
  SAY READLN(f)||CR
  IF EOF(f) THEN LEAVE i
END
CALL CLOSE(f)
CALL getinput(1 1 pen3'                          RETURN=Continue 'def)
SAY CR
RETURN


addalias:
match=0
username=getinput(1 0 pen3'Enter Users Email Name > 'def)
username=cleanstring(1':'username)
IF username='' THEN RETURN
IF ~EXISTS(bbspath'Users/'username) THEN 
 DO
  SAY 'Username not found'CR
  RETURN
 END 
newalias=getinput(1 0 pen3'Enter an Alias for'def' 'username def'> ')
IF newalias='' THEN RETURN
IF alias.0>0 THEN
  DO i=1 TO alias.0
   IF UPPER(alias.i)=UPPER(newalias) THEN match=1
  END
IF FIND(aliasexclude,UPPER(newalias))>0 THEN match=2
IF match=0 THEN 
  DO 
   alias.0=alias.0+1
   num=alias.0
   alias.num=newalias
   realname.num=username
   SAY alias.num 'alias as ' realname.num 'added'CR
  END
ELSE IF match=1 THEN SAY 'Alias 'newalias' already exists'CR
ELSE SAY newalias ' is a reserved name'CR
RETURN


delalias:
match=0
dalias=getinput(1 0 pen3'Enter Alias to Delete > 'def)
dalias=UPPER(WORD(dalias,1))
IF alias.0>0 THEN
  DO i=1 TO alias.0
   IF UPPER(alias.i)=UPPER(dalias) THEN 
    DO 
     match=1
     num=i
     LEAVE i
    END
  END
IF match=1 THEN 
 DO
  IF getinput(1 1 'Really Delete 'dalias'? (Ny) > ')='Y' THEN
   DO
    DO i=num TO alias.0
     j=i+1
     alias.i=alias.j
     realname.i=realname.j
    END
    alias.0=alias.0-1
   END
 END
ELSE SAY dalias' not Found.'CR
RETURN


viewalias:
IF alias.0>0 THEN
DO i=1 TO alias.0
 SAY RIGHT(alias.i,22) 'is' realname.i||CR
END
ELSE SAY 'No Aliases assigned'CR
CALL getinput(1 1 pen3'                          RETURN=Continue 'def)
SAY CR
RETURN


getinput:
PARSE ARG upflag' 'oneflag' 'pline
CALL checkdcd()
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
inarg=cleanstring(0':'inarg)
RETURN inarg


checkdcd:
IF ~frombb THEN RETURN
dcd
IF RC=0 THEN
  DO
    DO dcds=1 TO 3  /* 5 second delay */
      CALL DELAY(50)
      dcd
      IF RC~=0 THEN RETURN
    END
    dcd
    IF RC=0 THEN EXIT
  END
RETURN


cleanstring:
PARSE ARG nflag':'cstr
IF nflag=1 THEN
  DO
    cstr=COMPRESS(cstr,"'`")
    cstr=TRANSLATE(cstr,,namemask)
    cstr=SPACE(cstr,1,'_')
    RETURN cstr
  END
bot=XRANGE(,'1F'x)
cstr=strip_ansi(cstr)
top=XRANGE('7F'x)
cstr=COMPRESS(cstr,bot||top)
IF nflag=0 THEN cstr=STRIP(cstr)
RETURN cstr


strip_ansi:
PARSE ARG aline 
n=POS('1B'x,aline)
DO WHILE n>0
  DO k=2
    IF DATATYPE(SUBSTR(aline,n+k,1),'M') | (n+k+1)>LENGTH(aline) THEN
      leave k
  END
  aline=DELSTR(aline,n,k+1)
  n=POS('1B'x,aline)
END
RETURN aline


BREAK_C:
EXIT

/* bbsFriends.rexx */
