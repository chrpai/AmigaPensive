/* $VER: bbsProfiles.rexx 8.3 (21.12.94)
Copyright © 1994 Richard Lee Stockton
BBBBS Profiles manager
FREELY DISTRIBUTABLE
*/

OPTIONS RESULTS
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

PARSE ARG name level sysoplevel linesperpage colorflag maxtime bbspath .

CALL TIME('R')

def='[0m'
pen3='[33m'
bak2='[42m'
IF colorflag=0 THEN
  DO
    def=''
    pen3=''
    bak2=''
  END
CR=''
frombb=0
IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END
prodir=bbspath'Profiles'
CALL MAKEDIR(prodir)
pros=SHOWDIR(prodir)
protxt=bbspath'BBS_TEXT/PROFILES'
CALL showtext(protxt 1)
DO lupe=1
  SAY CR
  SAY '       1. Edit 'name'''s user Profile'CR
  SAY '       2. View a User Profile'CR
  SAY '       3. Search User Profiles'CR
  SAY '       4. Browse User Profiles'CR
  SAY CR
  temp=getinput(1 1 'Enter Selection Number > ')
  IF temp=1 THEN
    DO
      lynes.=''
      IF EXISTS(prodir'/'name) THEN
        DO
          IF readlines(prodir'/'name 1)~=0 THEN ITERATE lupe
          CALL DELETE(prodir'/'name)
        END
      ELSE lynes.0=3
      lynes.1=name
      lynes.2='Profile Last Updated:' DATE('W') DATE() TIME('C')
      lynes.3=LEFT('=',74,'=')
      IF savelines(prodir'/'name)~=0 THEN
        DO
          line='Profile for' name 'failed to save!'
          SAY line||CR
          CALL send2log(line)
          ITERATE lupe
        END
      edtype=''
      CALL bbsEd.rexx(4 prodir'/'name name TRUNC(maxtime-TIME('E'))-28)
      IF readlines(prodir'/'name 1)~=0 THEN CALL DELETE(prodir'/'name)
      IF lynes.0<4 THEN CALL DELETE(prodir'/'name)
      pros=SHOWDIR(prodir)
    END
  ELSE IF temp=2 THEN
    DO pf=1
      totpros=WORDS(pros)
      DO pfl=1 TO totpros BY 3
        pfl2=pfl+1
        pfl3=pfl+2
        pfline=pen3||RIGHT(pfl,3)||def LEFT(WORD(pros,pfl),21)
        IF pfl2<=totpros THEN
          pfline=pfline pen3||RIGHT(pfl2,3)||def LEFT(WORD(pros,pfl2),21)
        IF pfl3<=totpros THEN
          pfline=pfline pen3||RIGHT(pfl3,3)||def LEFT(WORD(pros,pfl3),21)
        SAY pfline||CR
        IF nonstop~=1 & ((pfl3%3)//linesperpage)=0 & pfl<totpros THEN
          IF waiting(2) THEN LEAVE pfl
      END
      emnum=getinput(1 0 pen3'Select User Profile Number > 'def)
      IF DATATYPE(emnum,'W') & emnum>0 & emnum<=totpros THEN
        DO
          tmp=WORD(pros,emnum)
          IF level>sysoplevel THEN
            DO
              CALL bbsEd.rexx(1 prodir'/'tmp name TRUNC(maxtime-TIME('E'))-28)
              IF readlines(prodir'/'tmp 1)~=0 THEN CALL DELETE(prodir'/'tmp)
              IF lynes.0<4 THEN CALL DELETE(prodir'/'tmp)
              pros=SHOWDIR(prodir)
            END
          ELSE CALL showtext(prodir'/'tmp 1)
        END
      ELSE LEAVE pf
    END
  ELSE IF temp=3 | temp=4 THEN
    DO
      searcharg=''
      nonstop=0
      IF temp=3 THEN
        DO
          searcharg=STRIP(getinput(0 0 'Enter Search Phrase > '))
          IF searcharg='' THEN ITERATE lupe
        END
      DO ui=1 TO WORDS(pros)
        pro=prodir'/'WORD(pros,ui)
        IF temp=3 THEN
          IF textsearch(pro searcharg)=0 THEN ITERATE ui
        SAY CR
        CALL readlines(pro 1)
        IF nonstop=1 THEN rnonstop=1
        ELSE rnonstop=0
        CALL seelines(2)
        IF rnonstop THEN nonstop=1
        ELSE IF waiting2()=1 THEN LEAVE ui
        SAY CR
        SAY CR
      END
    END
  ELSE IF temp='' | LEFT(temp,1)='Q' THEN LEAVE lupe
END
EXIT


textsearch:
ARG sfile' 'sarg
IF sarg='' THEN RETURN 0
x=OPEN(f,sfile,'R')
IF x=0 THEN RETURN 0
stemp=UPPER(READCH(f,65000))
CALL CLOSE(f)
retflag=0
IF POS(sarg,stemp)>0 THEN retflag=1
RETURN retflag


showtext:
PARSE ARG arg .
IF EXISTS(arg) THEN
  DO
    CALL readlines(arg 1)
    CALL seelines(1)
    nonstop=0
    CALL waiting()
  END
RETURN


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for reading!'
SAY line||CR
RETURN 0


seelines:
DO i=1 TO lynes.0
  SAY lynes.i||def||CR
  IF i//linesperpage=0 THEN
    IF waiting2() THEN LEAVE i
END
nonstop=0
RETURN


readlines:
CALL CLOSE(f)
PARSE ARG tempname readstart .
IF ~readopen(tempname) THEN RETURN 1
IF readstart<2 THEN lynes.=''
DO ri=readstart
  line=READLN(f)
  IF EOF(f) THEN BREAK
  lynes.ri=line
END
lynes.0=ri-1
CALL CLOSE(f)
DO ri=lynes.0 TO 0 BY -1 WHILE LENGTH(lynes.ri)=0 | LEFT(UPPER(lynes.ri),1)='/'
END
lynes.0=ri
RETURN 0


savelines:
PARSE ARG tempname .
ok=OPEN(f,tempname,'W')
IF ok=0 THEN
  DO
    SAY '***' tempname 'failed to open for saving!'CR
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


waiting:
CALL checktime()
IF waitchar='Q' THEN
  DO
    waitchar=''
    RETURN
  END
waitchar=''
IF nonstop=1 THEN RETURN
OPTIONS PROMPT pen3'                       RETURN=Continue  'def
PULL waitchar
RETURN


waiting2:
CALL checktime()
IF nonstop=1 THEN RETURN 0
waitchar=getinput(1 1 pen3'   Q=Quit   N=Non-Stop   RETURN=Continue  'def)
IF waitchar='N' THEN
  DO
    nonstop=1
    SAY pen3'To EXIT non-stop scrolling of text, press CTRL-E        'def||CR
    SAY CR
    CALL DELAY(100)
    waitchar=''
  END
IF waitchar='Q' THEN RETURN 1
RETURN 0


getinput:
PARSE ARG upflag' 'oneflag' 'pline
CALL checktime()
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
RETURN inarg


checktime:
IF ~frombb THEN RETURN
IF TIME('E')>maxtime THEN EXIT 0
IF TIME('E')>(maxtime-120) THEN SAY '*** Less than 2 minutes left! ***'CR
MSG RIGHT(' ',66-LENGTH(name)) '1B'x'M'||'[30m'||'[41m'||' 'name' level 'level' '||'[0m'
CALL checkdcd()
RETURN


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
    IF RC=0 THEN EXIT 0
  END
xmsg=GETCLIP('BBS_MESSAGE')
IF xmsg~='' THEN
  DO
    SAY CR
    SAY bak2' Message From BBBBS: 'def||CR
    SAY xmsg||CR
    SAY CR
    CALL SETCLIP('BBS_MESSAGE')
    CALL waiting()
  END
IF POS('G',GETCLIP('BBS_COMMAND'))>0 THEN EXIT
RETURN


send2log:
PARSE ARG sendline
IF ~frombb THEN RETURN
logfile=bbspath'Logs/log.'DATE('S')
fl='W'
IF EXISTS(logfile) THEN fl='A'
IF ~OPEN('log',logfile,fl) THEN
  DO
    IF ~OPEN('log',logfile,fl) THEN
      DO
        SAY 'failed to open log file'
        RETURN
     END
  END
CALL WRITELN('log','bbsProfiles:' sendline)
CALL CLOSE('log')
RETURN


BREAK_E:
i=999999
ri=999999
wi=999999
RETURN


BREAK_C:
EXIT

/* bbsProfiles.rexx */
