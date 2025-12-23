/* UserCheck.rexx 6.0 (9.7.93)
   Checks user files for duplicate phone numbers.
*/

CALL TIME('R')
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

p.=''
mask=COMPRESS(XRANGE(),'0123456789')
users=SHOWDIR(bbspath'Users')
DO i=1 TO WORDS(users)
  user=WORD(users,i)
  x=OPEN(f,bbspath'Users/'user,'R')
  IF x=0 THEN ITERATE i
  DO j=1 TO 4
    line=READLN(f)
  END
  CALL CLOSE(f)
  ph=RIGHT(COMPRESS(line,mask),7)
  IF ~DATATYPE(ph,'W') THEN
    DO
      SAY 'No phone number for' user
      ITERATE i
    END
  IF p.ph='' THEN p.ph=user
  ELSE SAY RIGHT(user,25) 'has same phone number as' p.ph
END
SAY 'Elapsed:' TIME('E')

/* UserCheck.rexx */
