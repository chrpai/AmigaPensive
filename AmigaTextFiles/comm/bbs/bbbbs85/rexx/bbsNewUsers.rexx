/* $VER: bbsNewUsers.rexx 8.4 (25.1.95)
copyright © 1990-95 Richard Lee Stockton
BBBBS offline New User Validator
FREELY DISTRIBUTABLE
*/

CALL TIME('R')

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

title.=''
title.1='BBBBS Users'
title.2='Version 8.4'
title.3='25-Nov-94'

ARG name level colorflag maxtime .

def='[0m'
pen3='[33m'
IF colorflag=0 THEN
  DO
    def=''
    pen3=''
  END
lineup='1B'x'M'
CR=''
IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END
ELSE frombb=0

figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'CR
    EXIT
  END
lynes.=''
DO i=1 TO 40
  lynes.i=READLN(f)
END
CALL CLOSE(f)
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
sysop=WORD(lynes.2,1)
compos=POS('/*',lynes.3)
IF compos>0 THEN lynes.3=LEFT(lynes.3,compos-1)
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'CR
    EXIT
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
back=WORD(lynes.40,1)
IF ~DATATYPE(back,'W') THEN back=999999

minlev=0
maxlev=99
templist=''
uname=''
newufile=bbspath'Lists/NEW_USERS'
IF EXISTS(newufile) THEN
  DO
    IF getinput(1 1 'Latest New Users Only? (nY) > ')~='N' THEN
      DO
        IF readlines(newufile 1)=0 THEN
          DO i=2 TO lynes.0
            templist=STRIP(templist WORD(lynes.i,3))
          END
      END
    ELSE newufile=''
  END
ELSE newufile=''
IF newufile='' THEN
  DO
    minlev=getinput(1 0 'Minimum level? (0) > ')
    maxlev=getinput(1 0 'Maximum level? (99) > ')
    IF ~DATATYPE(minlev,'W') THEN minlev=0
    IF ~DATATYPE(maxlev,'W') THEN maxlev=99
    IF minlev<0 | minlev>99 THEN minlev=0
    IF maxlev<0 | maxlev>99 THEN maxlev=99
    templist=SHOWDIR(bbspath'Users')
  END
DO levi=1 TO WORDS(templist)
  userfile=bbspath'Users/'WORD(templist,levi)
  IF readlines(userfile 1)=1 THEN ITERATE levi
  lt=WORD(lynes.20,1)
  IF ~DATATYPE(lt,'W') THEN lt=0
  IF lt<minlev | lt>maxlev THEN ITERATE levi
  line=lt WORD(templist,levi)
  SAY line||CR
  IF newufile='' THEN
    IF levi//20=0 THEN
      IF getinput(1 1 pen3'  Q=QUIT   RETURN=CONTINUE 'def)='Q' THEN LEAVE levi
  IF newufile~='' | lt<10 THEN
    DO
      DO levj=1 TO 12
        SAY pen3'  'lynes.levj||def||CR
      END
      SAY pen3'  'lynes.19||def||CR
    END
  ELSE ITERATE levi
  lcom=''
  IF lt<10 THEN lcom='['pen3'A'def']dd  '
  lcom=lcom'['pen3'K'def']ill  ['pen3'R'def']ename  ['pen3'S'def']kip this user?'
  IF lt<10 THEN lcom=lcom' (Akrs) > '
  ELSE lcom=lcom '(krS) > '
  lcom=getinput(1 1 lcom)
  IF lcom='K' THEN
    DO
      IF level>sysoplevel THEN CALL killuser(WORD(templist,levi))
    END
  ELSE IF lcom='R' THEN
    DO
      newname=''
      IF ChangeUserName.rexx(WORD(templist,levi))=0 THEN
        DO
          newname=GETCLIP('BBS_newname')
          CALL SETCLIP('BBS_newname')
        END
      IF newname~='' & newname~=WORD(templist,levi) THEN
        DO
          temp=WORDINDEX(templist,levi+1)
          rtemp=''
          IF temp>0 THEN rtemp=SUBSTR(templist,temp)
          temp=WORDINDEX(templist,levi)
          templist=''
          IF temp>2 THEN templist=STRIP(LEFT(templist,temp-1))
          templist=STRIP(templist newname rtemp)
        END
      levi=levi-1
      CALL SETCLIP('BBS_newname')
    END
  ELSE IF lcom~='S' & lt<10 THEN
    DO
      IF OPEN(f,bbspath'BBS_TEXT/DEF.MEMBER','R')=0 THEN
        SAY 'You need a default member file in BBS_TEXT!  ( BBS_TEXT/DEF.MEMBER )'CR
      ELSE
        DO
          DO lvi=1 TO 22
            line=READLN(f)
            IF lvi=11 THEN lynes.11=line
            IF lvi=20 THEN lynes.20=line
            IF lvi=21 THEN lynes.21=line
          END
          lynes.22=line
          CALL CLOSE(f)
          IF back<999999 THEN
            DO
              SAY CR
              lynes.22=''
              lynes.23=''
              IF DATATYPE(lynes.20,'W') THEN
                DO
                  SAY 'Setting message counters to last' back 'messages in each conference...'CR
                  DO i=1 TO lynes.20
                    num=countcheck(bbspath'Numbers/LastMessage'i 0)-back
                    IF num<0 | msg.i.0<back THEN num=0
                    lynes.22=lynes.22 num
                    lynes.23=lynes.23 0
                  END
                END
              ELSE SAY 'Bad default level in BBS_TEXT/DEF.MEMBER file!'CR
              SAY 'Setting file counter to last file uploaded...'CR
              lynes.16=countcheck(bbspath'Numbers/LastFile' 0)
              lynes.16=lynes.16 DATE('S') TIME()
            END
          lynes.0=27
          CALL savelines(userfile)
          SAY lynes.20 WORD(templist,levi) 'has been made a member.'CR
        END
    END
  IF lcom~='K' & newufile~='' THEN
    DO
      nlt=getinput(1 0 lynes.20 'Enter new level or blank for no change. > ')
      IF DATATYPE(nlt,'W') THEN
        DO
          lynes.20=nlt
          CALL savelines(userfile)
        END
      CALL writenew()
    END
END
IF newufile~='' & EXISTS(newufile) THEN
  IF getinput(1 1 'Delete NEW_USERS file? (nY) > ')~='N' THEN
    CALL DELETE(newufile)
IF EXISTS(bbspath'Lists/CBV_USERS') THEN
  IF getinput(1 1 'Delete CBV_USERS file? (nY) > ')~='N' THEN
    CALL DELETE(bbspath'Lists/CBV_USERS')
EXIT


writenew:
toname=WORD(templist,levi)
IF getinput(1 1 'Write' toname 'an email message? (nY) > ')~='N' THEN
  DO
    replysubj=''
    IF EXISTS(bbspath'BBS_TEXT/EMAIL_WELCOME') THEN
      IF getinput(1 1 'Use default welcome? (nY) > ')~='N' THEN replysubj='|@NEW@|'
    comm=STRIP(name maxtime-TRUNC(TIME('E')) 'MAIL' toname . 0 0 replysubj)
    CALL bbsWrite.rexx(comm)
  END
RETURN


killuser:
ARG narg
IF STRIP(narg)='' THEN LEAVE loop
CALL bbsKillUser.rexx(narg)
RETURN


readlines:
CALL CLOSE(f)
PARSE ARG tempname readstart .
IF OPEN(f,tempname,'R')=0 THEN RETURN 1
IF readstart<2 THEN lynes.=''
DO ri=readstart
  line=READLN(f)
  IF EOF(f) THEN BREAK
  lynes.ri=line
END
lynes.0=ri-1
CALL CLOSE(f)
DO ri=lynes.0 TO 0 BY -1 WHILE LENGTH(lynes.ri)=0 | LEFT(UPPER(lynes.ri),2)='/E' | LEFT(UPPER(lynes.ri),2)='/S'
END
lynes.0=ri
RETURN 0


savelines:
PARSE ARG tempname .
IF OPEN(f,tempname,'W')=0 THEN
  DO
    SAY '***' tempname 'failed to open for saving!'CR
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


countcheck:
PARSE ARG fname' 'cknum .
IF ~EXISTS(fname) THEN
  DO
    IF cknum=0 THEN RETURN 0
    IF OPEN(f,fname,'W')=0 THEN RETURN 0
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN cknum
  END
IF OPEN(f,fname,'R')=0 THEN
  DO
    CALL DELAY(99)
    IF OPEN(f,fname,'R')=0 THEN RETURN cknum
  END
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'W') THEN retval=0
IF ~DATATYPE(cknum,'W') THEN cknum=0
IF retval<cknum THEN
  DO
    IF OPEN(f,fname,'W')~=0 THEN
      DO
        CALL WRITELN(f,cknum)
        CALL CLOSE(f)
        RETURN cknum
      END
  END
RETURN retval


getinput:
PARSE ARG upflag' 'oneflag' 'pline
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
RETURN inarg

/* bbsNewUsers.rexx */
