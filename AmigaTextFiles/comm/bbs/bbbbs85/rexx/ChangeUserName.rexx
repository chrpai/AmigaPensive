/* $VER: ChangeUserName.rexx 8.3 (6.11.94)
 * copyright 1991-93 Richard Lee Stockton
 * FREELY DISTRIBUTABLE
*/

SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
IF ADDRESS()='BAUD' THEN CR='0D'x
ELSE CR=''

PARSE ARG OLD_NAME .

IF STRIP(OLD_NAME)='' THEN OLD_NAME=GETCLIP('BBS_oldname')
IF STRIP(OLD_NAME)='' THEN
  DO
    SAY '  Usage:  rx  ChangeUserName  OLD_USER_NAME'
    SAY 'Example:  rx  ChangeUserName  THE_MAD_HACKER'
    CALL BYE(10)
  END
OLD_NAME=TRANSLATE(STRIP(UPPER(OLD_NAME)),'_',' ')
OLD_NAME=COMPRESS(OLD_NAME,'.')

bbspath=GETCLIP('BBS_path')
IF bbspath='' THEN
  DO
    figarg='s:CONFIG.BBS'
    IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
    x=OPEN(f,figarg,'R')
    IF x=0 THEN
      DO
        SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
        EXIT(20)
      END
    lynes.=''
    DO i=1 TO 6
      lynes.i=READLN(f)
    END
    CALL CLOSE(f)
    bbspath=WORD(lynes.6,1)
    IF bbspath='' | ~EXISTS(bbspath) THEN
      DO
        SAY 'The bbspath, 'bbspath', does not exist!'
        CALL BYE(1);
      END
  END

/* load first line of userfile */

SAY OLD_NAME
x=OPEN(f,bbspath'Users/'OLD_NAME,'R')
IF x=0 THEN EXIT(2);
fullname=READLN(f)
CALL CLOSE(f);


/* let operator enter new name */

SAY 'Full Name:' fullname||CR
OPTIONS PROMPT 'User Name: '
PULL NEW_NAME
IF STRIP(NEW_NAME)='' THEN NEW_NAME=UPPER(fullname)
NEW_NAME=cleanstring(NEW_NAME)
IF EXISTS(bbspath'Users/'NEW_NAME) THEN
  DO
    SAY NEW_NAME 'already exists!'CR
    CALL BYE(3);
  END

OPTIONS PROMPT 'Rename' OLD_NAME 'to' NEW_NAME '? (y/N) > '
PULL temp
IF UPPER(LEFT(temp,1))~='Y' THEN EXIT(0)


/* rename */

SAY 'Renaming User files...'CR
CALL RENAME(bbspath'Users/'OLD_NAME,bbspath'Users/'NEW_NAME)
CALL RENAME(bbspath'Email/'OLD_NAME,bbspath'Email/'NEW_NAME)
CALL RENAME(bbspath'EmailFiles/'OLD_NAME,bbspath'EmailFiles/'NEW_NAME)

OPTIONS PROMPT 'Re-Address Email FROM' OLD_NAME'? (y/N) > '
PULL temp
IF UPPER(LEFT(temp,1))='Y' THEN
  DO
    SAY 'Re-Addressing EMail...'CR
    emailist=SHOWDIR(bbspath'Email','D')
    DO i=1 TO WORDS(emailist)
      user=WORD(emailist,i)
      email=SHOWDIR(bbspath'Email/'user,'F')
      DO j=1 TO WORDS(email)
        mail=WORD(email,j)
        IF POS(OLD_NAME,mail)>0 THEN
          DO
            x=OPEN(f,bbspath'EMail/'user'/'mail,'R')
            IF x=0 THEN
              DO
                SAY user'/'mail 'failed to open for reading!'CR
                ITERATE j
              END
            lynes.=''
            DO k=1
              line=READLN(f)
              IF EOF(f) THEN LEAVE k
              lynes.k=line
            END
            CALL CLOSE(f)
            lynes.0=k-1
            PARSE VAR lynes.2 ltemp junk rtemp
            lynes.2=' 'ltemp NEW_NAME||rtemp
            PARSE VAR mail junk'.'num
            CALL DELETE(bbspath'EMail/'user'/'mail)
            mail=NEW_NAME'.'num
            x=OPEN(f,bbspath'EMail/'user'/'mail,'W')
            IF x=0 THEN
              DO
                SAY user'/'mail 'failed to open for writing!'CR
                ITERATE j
              END
            DO k=1 TO lynes.0
              CALL WRITELN(f,lynes.k)
            END
            CALL CLOSE(f)
          END
      END
    END
  END

CALL DELETE(bbspath'Lists/Users')
CALL SETCLIP('BBS_newname',NEW_NAME)
IF GETCLIP('BBS_oldname')='' THEN
  DO
    IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localusers',1)
    IF SHOW('P','BBBBS_LOCAL') THEN CALL SETCLIP('BBS_mainusers',1)
  END
ELSE CALL SETCLIP('BBS_oldname')
IF ARG()>0 THEN SAY 'Done!'
CALL BYE(0);


BYE:
ARG byearg
EXIT(byearg);


BREAK_C:
BREAK_E:
SAY '*** CTRL BREAK'CR
EXIT(5);


cleanstring:
PARSE ARG cstr
bot=TRIM(XRANGE(,' '))
top=XRANGE('7F'x)
bot=bot||XRANGE('!','@')'[\]`~{:}'
cstr=TRANSLATE(UPPER(cstr),' ','_')
cstr=COMPRESS(cstr,bot||top)
cstr=STRIP(cstr)
cstr=SPACE(cstr,1,'_')
RETURN cstr



/*********  THESE NEXT TWO ROUTINES NOT YET IMPLEMENTED!  *********/


/* search filenotes for OLD_NAME and replace with NEW_NAME */
do_filenotes:
OPTIONS PROMPT 'Search filenotes and replace' OLD_NAME 'with' NEW_NAME '? '
PULL temp
IF UPPER(LEFT(temp,1))~='Y' THEN CALL BYE(1)
  DO
  END
RETURN


/* search message bases for OLD_NAME and replace with NEW_NAME */
do_msgs:
OPTIONS PROMPT 'Search message bases and replace' OLD_NAME 'with' NEW_NAME '? '
PULL temp
IF UPPER(LEFT(temp,1))~='Y' THEN CALl BYE(1);
RETURN

/* ChangeUserName.rexx */
