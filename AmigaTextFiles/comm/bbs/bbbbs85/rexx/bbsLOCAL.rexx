/*               $VER: bbsLOCAL.rexx 8.5 (3.6.95)
bbsLOCAL.rexx 8.5 © 1990-95 Richard Lee Stockton 3 Jun 95 2:02PM
    - FREELY DISTRIBUTABLE AS LONG AS THIS NOTICE REMAINS -

            BBBBS.baud without the BaudBandit stuff
   Will multi-task with BBBBS.baud (within limits, see docs)
 THIS IS THE SYSOP'S VERSION OF BBBBS.baud FOR LOCAL USE ONLY!
*/

copyright.=''
copyright.1=STRIP(SOURCELINE(2))
copyright.2='
Gramma Software 21305-60th Ave West, Mountlake Terrace WA 98043-2009'
copyright.3='
ARexx portions of this software copyright 1990-93 Richard Lee Stockton'
copyright.4='- FREELY DISTRIBUTABLE as long as this notice remains -'

/* If the QuickSortPort not found then try to run setup.rexx */

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT

IF SHOW('P','BBBBS_LOCAL') THEN
  DO
    SAY 'bbsLOCAL.rexx is already running!'
    EXIT 0
  END
CALL OPENPORT('BBBBS_LOCAL')

CALL SETCLIP('BBS_mainfiles')
CALL SETCLIP('BBS_mainusers')

PARSE VERSION . . cpu .
cpu=RIGHT(cpu,2)/10
IF cpu<1 THEN cpu=1

/* BBS Directories (may be created with SETUP option) */
bbs.=''
bbs.1='Information' /* text files from sysop for the user to read */
bbs.6='Scratch'
bbs.7='BBS_HELP'
bbs.8='rexxDoors'
bbs.9='BBS_TEXT'    /* text files for BBS use. WELCOME HELLO, NEW etc. */
bbs.10='FileNotes'
bbs.11='BBS_LIBS'
bbs.12='BBS_MSGS'
bbs.13='Lists'
bbs.14='Numbers'    /* 1st & last messages, mail, files */
bbs.15='Usage'
bbs.16='Logs'
bbs.17='EMailFiles'
bbs.18='EMail'
bbs.19='Users'


/* VARIABLES */

RESET:
bbsprefs.=0 /* start with all prefs OFF */
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')
alpha.=''
lastread.=0
dirnum=1
linesperpage=20
sortuserflag=0
sortalphaflag=0
loadalphaflag=0
savefileflag=0
emailonline=-1
level=0
lastread.=0
totwrit.=0
lastbrowse=0
warnings=0
winnings=0
nonstop=0
libtext=1
newfilesdate=''
newpassword=''
replysubj=''
msgdir=1
menuflag=1
logonflag=1
data.=''
clr=''
lineup='1B'x'M'
lm='Loading Module...'lineup


/* try to trap everything */

SIGNAL ON BREAK_C
OPTIONS RESULTS
OPTIONS FAILAT 999999
SIGNAL ON BREAK_E
SIGNAL ON SYNTAX
SIGNAL ON FAILURE
NUMERIC DIGITS 14


ARG option .

SAY CENTER(copyright.1,75)
CALL config()

IF ~EXISTS(bbspath'Numbers/FirstLogon') THEN
  ADDRESS COMMAND 'C:Date >'bbspath'Numbers/FirstLogon'

SAY CENTER(copyright.2,75)
SAY CENTER(copyright.3,75)
SAY CENTER(copyright.4,75)
SAY


IF option='SETUP' THEN
  DO
    SAY 'Making sure all needed directories are here...'
    DO i=1 TO 20
      IF bbs.i~='' THEN CALL MAKEDIR(bbspath||bbs.i)
    END
  END

CALL colors(1)
msg.=''
IF readopen(bbspath'Lists/Conferences') THEN
  DO
    DO i=1
      line=READLN(f)
      IF line='END' THEN BREAK
      IF EOF(f) THEN BREAK
      num=WORD(line,1)
      IF DATATYPE(num,'W') THEN msg.num=WORD(line,2)
    END
    CALL CLOSE(f)
  END
IF option='SETUP' THEN
  DO
    DO i=1 TO 99
      IF msg.i~='' THEN CALL MAKEDIR(msgpath||i)
    END
  END

courtesy=''
IF EXISTS(bbspath'Lists/Courtesy') THEN
  DO
    IF readopen(bbspath'Lists/Courtesy') THEN
      DO
        DO i=1
          line=READLN(f)
          IF EOF(f) THEN BREAK
          courtesy=courtesy line
        END
        CALL CLOSE(f)
      END
  END

dirs.=''
IF readopen(bbspath'Lists/Libraries') THEN
  DO
    DO i=1
      line=READLN(f)
      IF line='END' | EOF(f) THEN LEAVE i
      num=WORD(line,1)
      IF DATATYPE(num,'W') THEN dirs.num=STRIP(WORD(line,2))
    END
    CALL CLOSE(f)
  END
IF option='SETUP' THEN
  DO
    SAY 'Making sure all file library directories are here...'
    DO i=1 TO 99
      IF dirs.i~='' THEN
        DO
          CALL MAKEDIR(libpath||dirs.i)
          CALL MAKEDIR(bbspath'FileNotes/'dirs.i)
        END
    END
  END

users=0
CALL sortuserlist()

SAY '          The larger the BBS gets, the longer the setup takes...'

CALL loadfiles()
CALL set_grand()
CALL loadalpha(1)


BIG_LOOP:
/** Identify (title) message */
IF EXISTS(bbspath'BBS_TEXT/HELLO') THEN
  DO
    SAY 
    CALL showtext(bbspath'BBS_TEXT/HELLO' 0)
  END
SAY
SAY pen3'Courtesy List:'def
SAY courtesy
SAY


/* Ask for name */
name=''
DO count=1 TO 3
  name=getinput(1 0 'Please enter name: ')
  name=SPACE(name,1,'_')
  IF name='NEW' THEN LEAVE count
  IF name~='' THEN
    DO
      IF EXISTS(bbspath'Users/'name) THEN LEAVE count
      IF EXISTS(bbspath'Morgue/'name'.lha') THEN
        DO
          SAY
          SAY name 'used to a member of this BBS.'
          SAY 'If that is you, you will have to resurrect yourself...'
          IF getinput(1 1 'Resurrect' name'? (Ny) > ')='Y' THEN
            DO
              dd=WORD(STATEF(bbspath'Morgue/'name'.lha'),5)
              dd=DATE(,dd,'I')
              SAY 'Resurrecting a dead user.  Killed' dd '...'
              ADDRESS COMMAND 'CD' bbspath'0A'x||'lha x Morgue/'name'.lha'
              CALL DELETE(bbspath'Morgue/'name'.lha')
              CALL send2log('RESURRECTED:' name 'who was killed' dd)
              LEAVE count
            END
        END
      IF FIND(exclusion,name)>0 THEN
        DO
          SAY 'Sorry, that is a reserved name.'
          name=''
          ITERATE count
        END
      CALL loadcourtesy()
      IF bbsprefs.7>0 | FIND(courtesy,name)>0 THEN
        DO
          SAY
          SAY 'Welcome' name'!'
          SAY 'You will be automatically validated after you enter your user info.'
          SAY
          LEAVE count
        END
    END
  IF count<3 THEN
    DO
      IF STRIP(name)~='' THEN SAY name 'not found.  Please try again.'
      SAY 'New Users enter NEW to apply for validation.'
    END
END
IF count>3 THEN SIGNAL DONE
CALL checkUser()
IF UPPER(WORD(data.12,3))~='BIRTHDAY:' THEN
  DO
    SAY
    SAY 'Please help us out by entering the following information.'
    CALL getbirth()
    SAY '   Thank you!'
  END
CALL checkclips()
city=docity(data.3)

CALL TIME('R')

IF RIGHT(WORD(data.12,4),4)=RIGHT(DATE('S'),4) THEN
  DO
    arg=bbspath'BBS_TEXT/BIRTHDAY'
    IF EXISTS(arg) THEN 
      DO
        SAY
        CALL showtext(arg 1)
      END
    SAY
    SAY '***  Happy Birthday,' pen3||data.1||def', and many more!  ***'
    SAY
  END
SAY 

CALL SETCLIP('BBS_LOCAL',name)
IF EXISTS('rexx:bbs:LOGON.rexx') THEN CALL bbsLOGON.rexx(name level)
CALL sortlibraries()
CALL sortconferences()
IF FIND(data.8,'QUICK')>0 THEN CALL do_quick(0)


/*
Opening Display after logon. Seen by all Users ONCE A DAY. It first
looks for a unique yearly data (ie, WELCOME.0704), then daily data
(ie, WELCOME.Fri), and then a simple, everyday 'WELCOME' datafile.
*/

IF DATE('I')>lastondate THEN
  DO
    SAY
    arg=bbspath'BBS_TEXT/WELCOME.'RIGHT(DATE('S'),4)
    CALL showtext(arg 1)
    SAY
    arg=bbspath'BBS_TEXT/WELCOME.'LEFT(DATE('W'),3)
    CALL showtext(arg 1)
    SAY
    arg=bbspath'BBS_TEXT/WELCOME'
    CALL showtext(arg 1)

/*
Looks for files in the format  LEVEL.low-high, ie "LEVEL.50-80" will only
be seen by users with a level >= 50 and <= 80.
*/

    levels.=''
    IF FileList(bbspath'BBS_TEXT/LEVEL.*',levels)>0 THEN
      DO
        DO ui=1 TO levels.0
          p=LASTPOS('.',levels.ui)
          x=SUBSTR(levels.ui,p+1)
          PARSE VAR x lo'-'hi .
          IF ~DATATYPE(lo,'W') | ~DATATYPE(hi,'W') THEN ITERATE ui
          IF lo>level | hi<level THEN ITERATE ui
          DO
            SAY
            CALL showtext(levels.ui 1)
          END
        END
      END

/*
Looks for format UNTIL.YYYYMMDD ie, "UNTIL.19920514"
Deletes any that are previous to "today"
*/

    untils.=''
    IF FileList(bbspath'BBS_TEXT/UNTIL.*',untils)>0 THEN
      DO
        CALL QSORT(1,untils.0,untils)
        DO ui=1 TO untils.0
          IF RIGHT(untils.ui,8)<DATE('S') THEN CALL DELETE(untils.ui)
          ELSE
            DO
              SAY
              CALL showtext(untils.ui 1)
            END
        END
      END
    DROP untils.
  END

IF bbsprefs.1 & ~terseflag THEN
  DO
    IF doGrin()>3 THEN CALL waiting()
    IF EXISTS(bbspath'rexxDoors/Moon.rexx') THEN CALL Moon.rexx()
    IF EXISTS(bbspath'rexxDoors/Time.rexx') THEN CALL Time.rexx()
    IF FIND(UPPER(SHOWLIST('A')),'TODAY')>0 THEN
      DO
        tf=scratch'/TODAY'
        finfo=STATEF(tf)
        IF WORD(finfo,5)~=DATE('I') THEN
          ADDRESS COMMAND 'C:Today091 >'tf
        IF EXISTS(tf) THEN CALL showtext(tf 0)
      END
    SAY
  END

IF SHOWDIR(bbspath'Email/'name)~='' THEN CALL readmail(0)
ELSE SAY 'Your mailbox is empty.'
IF ~terseflag THEN
  DO
    IF level>sysoplevel THEN
      DO
        lstmail=WORD(data.17,3)
        IF ~DATATYPE(lstmail,'W') THEN lstmail=0
        IF countcheck('LastMail' 0)>lstmail THEN
          IF getinput(1 1 'Check Email? (Ny) > ')='Y' THEN CALL mailreport()
        IF level<99 THEN
          DO
            SAY
            CALL showtext(bbspath'Email/'sysop'/NEW_FILES' 1)
          END
        SAY
        CALL showtext(bbspath'Lists/NEW_USERS' 1)
        CALL showtext(bbspath'Lists/CBV_USERS' 1)
      END
    CALL logonstats()
    CALL newinfo()
  END
CALL showmarked(1)
CALL setdir(libpath||dirs.1)
logonflag=0


/***** MAIN *****/

IF menu~='ALL' THEN menu='MAIN'

RESTART:
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

waitchar=''
string=''
opt=''
IF level<1 THEN menu='NEW'
DO WHILE(opt~='G')
  go=0
  DO WHILE(~go)
    IF waitchar='' | waitchar='?' THEN
      DO
        commands='cghiqsvwxyz!#,'
        IF level>0  THEN commands='abcdefghijlmnoprstuvwxyz!$#&.,+'
        ELSE IF SHOWDIR(bbspath'Email/'name)~='' THEN commands=commands'e'
        IF level>sysoplevel THEN commands=commands'k%^()=;'
        IF level=99 THEN commands=commands'@~'
        commands=commands'?'
        IF menuflag | waitchar='?' | string='?' THEN
          DO
            opt='MENU'
            arg=''
            CALL menus()
          END
        ELSE SAY pen3'COMMANDS:'def commands
      END
    line=''
    line=line||bak2' 'TIME('C')' 'def
    IF menu='ALL' | menu='FILE' THEN
      line=line pen3'FILE_LIBRARY:'plaindir||def
    ELSE IF menu='MSG' THEN line=line pen3'MESSAGES:'def
    ELSE line=line pen3'MAIN:'def
    line=line'  'bbsname
    IF waitchar='' THEN waitchar=getinput(0 0 line' > ')
    PARSE VAR waitchar string' 'arg
    nonstop=0
    string=UPPER(STRIP(string))
    IF clr~='' THEN SAY clr
    x=GETCLIP('BBS_LOCAL_MSG')
    IF x~='' THEN
      DO
        CALL SETCLIP('BBS_LOCAL_MSG')
        SAY
        SAY bak2' Message from BBBBS: 'def
        SAY x
        SAY
        CALL waiting()
      END
    IF string='OFF' | string='BYE' THEN SIGNAL LOGOUT
    IF string='FL' & level>0 THEN CALL bbsFriends.rexx(name colorflag)
    CALL checkalias()
    IF LEFT(string,1)='D' THEN
      IF DATATYPE(SUBSTR(string,2),'W') THEN arg=SUBSTR(string,2) arg
    waitchar=''
    IF DATATYPE(string,'W') THEN
      DO
        IF string>level THEN
          DO
            arg=STRIP(string arg)
            string='D'
          END
        ELSE
          DO
            dirnum=string
            CALL chdir2()
            CALL since()
          END
      END
    IF string='QUICK' & level>0 THEN CALL do_quick(1)
    opt=LEFT(string,1)
    go=1
    IF POS(opt,UPPER(commands))=0 THEN go=0
  END
  t=bbspath'BBS_TEXT/COM.'opt
  IF UPPER(arg)='EDIT' THEN CALL edinfo(t,opt,'Menu Command')
  IF ~terseflag THEN CALL showtext(t 1)
  OPTIONS PROMPT 'Filename: '
  SELECT
    WHEN opt='A' THEN CALL showalpha()
    WHEN opt='B' THEN CALL browse()
    WHEN opt='C' THEN CALL editor(name 3000 'MAIL' sysop . 0 0 'FEEDBACK')
    WHEN opt='D' THEN CALL dload()
    WHEN opt='E' THEN CALL readmail(level>0)
    WHEN opt='F' THEN CALL do_F()
    WHEN opt='H' THEN CALL help('MAIN')
    WHEN opt='I' THEN CALL information()
    WHEN opt='J' THEN CALL jump2rexx()
    WHEN opt='K' THEN CALL killuser()
    WHEN opt='L' THEN CALL list()
    WHEN opt='M' THEN IF menu~='ALL' THEN menu='MSG'
    WHEN opt='N' THEN CALL newfiles()
    WHEN opt='O' THEN CALL otheruser()
    WHEN opt='P' THEN CALL editor(name 3000 'MSG' . . 0 0)
    WHEN opt='R' THEN CALL readmessages()
    WHEN opt='S' THEN CALL bbsSEARCH()
    WHEN opt='U' THEN CALL uload(1)
    WHEN opt='V' THEN CALL showtext(bbspath'Usage/USER.LOG' 1)
    WHEN opt='W' THEN CALL showuserlist()
    WHEN opt='X' THEN CALL switchmenuflag()
    WHEN opt='Y' THEN CALL edituser()
    WHEN opt='Z' THEN CALL counts()
    WHEN opt='~' THEN CALL sysED(1)
    WHEN opt='@' THEN CALL shell()
    WHEN opt='#' THEN CALL switchcolors()
    WHEN opt='$' THEN IF menu='ALL' THEN menu='MAIN'; ELSE menu='ALL'
    WHEN opt='%' THEN CALL editnote()
    WHEN opt='^' THEN CALL readlogs()
    WHEN opt='&' THEN CALL bbsProfiles.rexx(name level sysoplevel 20 1 6000 bbspath)
    WHEN opt=';' THEN CALL changename()
    WHEN opt='(' THEN CALL filereport()
    WHEN opt=')' THEN CALL mailreport()
    WHEN opt='=' THEN CALL levelreport()
    WHEN opt='+' THEN CALL ext_dload()
    WHEN opt='.' THEN menu='MAIN'
    WHEN opt=',' THEN DO;CALL hourly();CALL waiting();END
    WHEN opt='?' & menuflag THEN CALL help('MAIN')
    OTHERWISE NOP
  END
END
SIGNAL LOGOUT
EXIT



/* FUNCTIONS */


do_F:
IF menu='FILE' | menu='ALL' THEN
  DO
    IF STORAGE()<(bbsprefs.15+100000) | GETCLIP('BBS_libs.0')~='' THEN
      DO
        SAY
        SAY 'Sorry! Not enough memory left for background archiving.'
        SAY 'Please try again in 10 minutes or so.'
        SAY
        RETURN
      END
    DO i=0 TO libs.0
      CALL SETCLIP('BBS_libs.'i,libs.i)
    END
    IF Make_BrowseList.baud(name colorflag files.0)=0 THEN
      IF emailonline>=0 THEN emailonline=emailonline+1
    DO i=0 TO libs.0
      CALL SETCLIP('BBS_libs.'i)
    END
  END
ELSE IF menu~='ALL' THEN menu='FILE'
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
IF nflag=2 THEN bot=COMPRESS(bot,'1B'x)  /* ESC for ANSI */
ELSE cstr=strip_ansi(cstr)
top=XRANGE('7F'x)
cstr=COMPRESS(cstr,bot||top)
IF nflag=0 THEN cstr=STRIP(cstr)
RETURN cstr


showtext:
PARSE ARG starg warg .
IF EXISTS(starg'.'colorflag) THEN
  DO
    IF OPEN(f,starg'.'colorflag,'R')=0 THEN RETURN
    t=READCH(f,65000)
    CALL CLOSE(f)
    SAY t
  END
ELSE IF EXISTS(starg) THEN
  DO
    CALL readlines(starg 1)
    IF colorflag=0 THEN CALL strip_lynes()
    CALL seelines(1)
  END
ELSE RETURN
IF warg THEN
  DO
    CALL waiting()
    nonstop=0
  END
RETURN


strip_lynes:
DO i=1 TO lynes.0
  lynes.i=strip_ansi(lynes.i)
END
RETURN


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


doGrin:
IF ~EXISTS(bbspath'rexxDoors/Grin_du_Jour.rexx') THEN RETURN 0
CALL setdir(bbspath'rexxDoors')
temp=Grin_du_Jour.rexx()
SAY
RETURN temp


do_quick:
ARG flag .
IF FIND(UPPER(data.8),'QUICK')=0 THEN
  DO
    SAY
    SAY 'The QUICK option is OFF in your current settings.'
    SAY
    SAY 'Setting the QUICK option to ON will allow you to tell the BBS to'
    SAY 'make a .lha archive of all new bbs activity since your last call.'
    SAY
    SAY 'This archive can then be read (and replied to, and files can be'
    SAY 'uploaded and downloaded) using 'pen3'bbsQUICK.rexx'def', the offline read/reply'
    SAY 'module for BBBBS, which is available here in the file libraries.'
    SAY
    IF getinput(1 1 'Turn the QUICK option ON? (Ny) > ')~='Y' THEN RETURN
    data.8=data.8 'QUICK'
    CALL saveData(0)
  END
ELSE IF flag=1 THEN
  DO
    IF getinput(1 1 'Turn the QUICK option OFF? (Ny) > ')='Y' THEN
      DO 
        temp=data.8
        data.8=''
        DO i=1 TO WORDS(temp)
          IF WORD(temp,i)~='QUICK' THEN data.8=STRIP(data.8 WORD(temp,i))
        END
        ADDRESS COMMAND 'c:delete' bbspath'EmailFiles/'name'/QUICK_#?'
        RETURN
      END
  END
IF getinput(1 1 'Edit your QUICK exclude list? (Ny) > ')='Y' THEN
  DO
    SAY
    SAY 'You may EXCLUDE any of these from your QUICK archives.'
    SAY pen3||LEFT('-',74,'-')||def
    temp=LEFT(' ',7)
    SAY temp'HELLO          - Pre-logon message.'
    SAY temp'WELCOME        - Post-logon message.'
    SAY temp'GOODBYE        - Logoff message.'
    SAY temp'HOURLY         - Average-Minutes-Per-Hour usage graph.'
    SAY temp'STATS.BBS      - Most of the Z command from the main menu.'
    SAY temp'filename       - ANY filename in the Information area.'
    SAY temp'MESSAGES       - New conference messages.'
    SAY temp'FILELIST       - New file descriptions.'
    SAY pen3||LEFT('-',74,'-')||def
    SAY 'Enter a space separated list of what you wish to exclude.'
    SAY pen3'Exclude:'def data.26
    temp=getinput(1 0 pen3'Exclude: 'def)
    IF temp='' & data.26~='' THEN
      DO
        IF getinput(1 1 'Clear the QUICK exclude list? (nY) > ')~='N' THEN
          data.26=''
      END
    ELSE data.26=temp
    temp='Your QUICK archives will exclude'pen3
    IF data.26='' THEN temp=temp 'nothing!'
    ELSE temp=temp data.26
    SAY temp||def
    CALL saveData(0)
    SAY
  END
IF GETCLIP('BBS_'name)~='' THEN
  DO
    SAY
    SAY 'The QUICK routines are still working on your archive...'
    SAY 'Please try again later.'
    SAY
    RETURN
  END
quickdir=bbspath'EmailFiles/'name
CALL MAKEDIR(quickdir)
CALL setdir(quickdir)
qdarg=scratch'/dirlist'
ADDRESS COMMAND 'C:list >'qdarg quickdir'/QUICK_#? DATES'
efiles=UPPER(SHOWDIR(quickdir))
qflag=0
das=0
IF getinput(1 1 'Archive new BBS activity now? (Ny) > ')='Y' THEN
  DO
    das=1
    DO i=1 TO WORDS(efiles)
      qarg=WORD(efiles,qi)
      IF LEFT(qarg,6)='QUICK_' & RIGHT(qarg,4)='.LHA' THEN
        DO
          SAY
          SAY 'There is already a QUICK_xxxxx.LHA file in your mailbox...'
          SAY 'Activity request has been CANCELLED!'
          SAY
          das=0
          LEAVE i
        END
    END
  END
IF das=1 THEN
  DO
    CALL SETCLIP('BBS_city',city)
    CALL SETCLIP('BBS_'name'_26',data.26)
    IF FIND(UPPER(data.26),'STATS.BBS')=0 THEN
      CALL SETCLIP('BBS_statsarg',emailonline grand grand2 files.0)
    IF FIND(UPPER(data.26),'MESSAGES')=0 THEN
      CALL SETCLIP('BBS_'name'_22',data.22)
    CALL MAKEDIR(bbspath'EmailFiles/'name)
    CALL showmarked(0)
    CALL SETCLIP('BBS_QUICKOUT_BAUD',bps)
    ADDRESS AREXX bbsQUICKOUT.rexx name level lastbrowse WORD(data.16,2) data.21
    IF FIND(UPPER(data.26),'MESSAGES')=0 THEN
      DO
        clear_marked=1
        DO i=1 TO level
          IF WORD(data.22,i)~=-1 THEN
            lastread.i=countcheck('LastMessage'i 0)
        END
        SAY
      END
    IF FIND(UPPER(data.26),'FILELIST')=0 THEN
      lastbrowse=countcheck('LastFile' 0)
    newfilesdate=DATE('S') TIME()
    IF writeopen(bbspath'EmailFiles/'name'/Libraries') THEN
      DO
        DO i=1 TO libs.0
          CALL WRITELN(f,libs.i)
        END
        CALL CLOSE(f)
      END
    IF writeopen(bbspath'EmailFiles/'name'/Conferences') THEN
      DO
        DO i=1 TO msgs.0
          CALL WRITELN(f,msgs.i)
        END
        CALL CLOSE(f)
      END
    SAY
    CALL saveData(1)
    qflag=1
  END
IF WORD(STATEF(qdarg),2)>80 THEN
  DO
    CALL showtext(qdarg 0)
    SAY
  END
DO qi=1 TO WORDS(efiles)
  qarg=WORD(efiles,qi)
  IF LEFT(qarg,6)='QUICK_' & RIGHT(qarg,4)='.LHA' THEN
    DO
      SAY qarg 'is' WORD(STATEF(qarg),2) 'bytes.'
      arg=qarg
      DO WHILE dload()=1
      END
      t=''
      DO WHILE t~='N' & t~='Y'
        t=getinput(1 1 'Delete' qarg'? (ny) > ')
      END
      IF t='Y' THEN
        DO
          IF DELETE(quickdir'/'qarg)=1 THEN SAY qarg 'deleted.'
          CALL DELETE(quickdir'/'qarg'.xdl')
          qarg=COMPRESS(UPPER(qarg),'QUICK_.LHA')
          CALL DELETE(bbspath'Email/'name'/BBBBS.'qarg)
        END
    END
END
arg=''
IF getinput(1 1 'Do you have a QUICKIN file to upload? (Ny) > ')='Y' THEN
  DO
    arg='QUICKIN.lha'
    ul=2
    DO WHILE ul=2
      ul=uload(0)
    END
  END
IF EXISTS(bbspath'EmailFiles/'name'/QUICKIN.lha') & level>=sysoplevel THEN
  IF getinput(1 1 'Process your QUICKIN archive [N]ow or at [L]ogoff? (Ln) > ')='N' THEN
    DO
      SAY
      SAY 'Please wait, processing QUICKIN archive...'
      CALL bbsQUICKIN.rexx(name level sysoplevel bbsprefs.6)
      CALL checkclips()
      CALL loaddata()
      SAY
    END
IF getinput(1 1 'Logoff Now? (nY) > ')~='N' THEN
  DO
    IF qflag THEN SAY 'Your archive will be waiting next time you call...'
    SAY
    SIGNAL LOGOUT2
  END
IF qflag THEN
  DO
    SAY
    SAY 'Note: You now have no ''new'' files or messages (they are being archived).'
    SAY
    CALL waiting()
  END
RETURN


killuser:
ARG kname .
IF level<=sysoplevel THEN RETURN
CALL bbsKillUser.rexx(kname)
RETURN


menus:
IF OPEN(f,bbspath'BBS_TEXT/MENU_'menu'.'colorflag,'R')~=0 THEN
  DO
    m=READCH(f,65000)
    CALL CLOSE(f)
    SAY m
    IF level>sysoplevel THEN
      DO
        SAY ' ['pen3'K'def']ill a user      ['pen3'%'def'] edit filenote  ['pen3'='def'] level report'def
        SAY ' ['pen3'^'def'] view BBS logs  ['pen3'('def'] file report    ['pen3';'def'] change username'def
      END
    IF level=99 THEN
      SAY ' ['pen3'~'def'] online editor  ['pen3'@'def'] dos shell      ['pen3')'def'] email report'def
  END
ELSE IF menu='NEW' THEN
  DO
    SAY pen6'     _________________'def
    SAY pen6'  __/  'pen3'New User Menu'pen6'  \___'def
    SAY pen6' |                        |'def
    SAY pen6' |'def'   ['pen3'H'def']elp               'pen6'|'def
    SAY pen6' |'def'   ['pen3'I'def']nformation        'pen6'|'def
    SAY pen6' |'def'   ['pen3'Y'def']our user data     'pen6'|'def
    SAY pen6' |'def'   ['pen3'W'def']ho is here        'pen6'|'def
    SAY pen6' |'def'   ['pen3'S'def']earch user list   'pen6'|'def
    SAY pen6' |'def'   ['pen3'V'def']iew user log      'pen6'|'def
    SAY pen6' |'def'   ['pen3'Z'def'] bbs statistics   'pen6'|'def
    SAY pen6' |'def'   ['pen3','def'] hourly stats     'pen6'|'def
    SAY pen6' |'def'   ['pen3'X'def'] toggle menus     'pen6'|'def
    SAY pen6' |'def'   ['pen3'#'def'] toggle color     'pen6'|'def
    SAY pen6' |'def'   ['pen3'!'def'] YELL for SYSOP   'pen6'|'def
    SAY pen6' |'def'   ['pen3'C'def']omment to SYSOP   'pen6'|'def
    SAY pen6' |'def'   ['pen3'G'def']oodbye (hangup)   'pen6'|'def
    SAY pen6' |________________________|'def
    IF SHOWDIR(bbspath'Email/'name)~='' THEN
      DO
        SAY CR
        SAY 'You have unread EMail waiting!'
        SAY 'Enter E to read ['pen3'E'def']mail.'
      END
  END
ELSE IF menu='MSG' THEN
  DO
    SAY pen6'       ____________'def
    SAY pen6'  ____/  'pen3'Messages'pen6'  \_____'def
    SAY pen6' |                       |'def
    SAY pen6' |'def'   ['pen3'H'def']elp              'pen6'|'def
    SAY pen6' |'def'   ['pen3'P'def']ost messages     'pen6'|'def
    SAY pen6' |'def'   ['pen3'R'def']ead messages     'pen6'|'def
    SAY pen6' |'def'   ['pen3'S'def']earch messages   'pen6'|'def
    SAY pen6' |'def'   ['pen3'E'def']mail (private)   'pen6'|'def
    SAY pen6' |'def'   ['pen3'C'def']omment to SYSOP  'pen6'|'def
    SAY pen6' |'def'   ['pen3'QUICK'def'] options     'pen6'|'def
    SAY pen6' |'def'   ['pen3'FL'def'] Friends List   'pen6'|'def
    SAY pen6' |'def'   ['pen3'!'def'] YELL for SYSOP  'pen6'|'def
    IF(level>sysoplevel) THEN DO
    SAY pen6' |'def'   ['pen3'^'def'] view BBS logs   'pen6'|'def
    SAY pen6' |'def'   ['pen3')'def'] email report    'pen6'|'def
    SAY pen6' |'def'   ['pen3'='def'] level report    'pen6'|'def
    SAY pen6' |'def'   ['pen3';'def'] change username 'pen6'|'def;END
    IF(level=99) THEN DO
    SAY pen6' |'def'   ['pen3'~'def'] online editor   'pen6'|'def
    SAY pen6' |'def'   ['pen3'@'def'] dos shell       'pen6'|'def;END
    SAY pen6' |'def'   ['pen3'F'def']iles menu        'pen6'|'def
    SAY pen6' |'def'   ['pen3'.'def'] main menu       'pen6'|'def
    SAY pen6' |_______________________|'def
  END
ELSE IF menu='FILE' THEN
  DO
    SAY pen6'         _________'def
    SAY pen6'  ______/  'pen3'Files'pen6'  \_______'def
    SAY pen6' |                        |'def
    SAY pen6' |'def'   ['pen3'A'def']lphabetic list    'pen6'|'def
    SAY pen6' |'def'   ['pen3'H'def']elp               'pen6'|'def
    SAY pen6' |'def'   ['pen3'B'def']rowse filenotes   'pen6'|'def
    SAY pen6' |'def'   ['pen3'N'def']ew files list     'pen6'|'def
    SAY pen6' |'def'   ['pen3'L'def']ist by Library    'pen6'|'def
    SAY pen6' |'def'   ['pen3'F'def']ilelist archives  'pen6'|'def
    SAY pen6' |'def'   ['pen3'S'def']earch files       'pen6'|'def
    SAY pen6' |'def'   ['pen3'U'def']pload             'pen6'|'def
    SAY pen6' |'def'   ['pen3'D'def']ownload           'pen6'|'def
    SAY pen6' |'def'   ['pen3'+'def'] Extra Devices    'pen6'|'def
    IF(level>sysoplevel) THEN DO
    SAY pen6' |'def'   ['pen3'K'def']ill a user        'pen6'|'def
    SAY pen6' |'def'   ['pen3'%'def'] edit filenote    'pen6'|'def
    SAY pen6' |'def'   ['pen3'('def'] file report      'pen6'|'def
    SAY pen6' |'def'   ['pen3';'def'] change username  'pen6'|'def;END
    IF(level=99) THEN
    SAY pen6' |'def'   ['pen3'@'def'] dos shell        'pen6'|'def
    SAY pen6' |'def'   ['pen3'M'def']essages menu      'pen6'|'def
    SAY pen6' |'def'   ['pen3'.'def'] main menu        'pen6'|'def
    SAY pen6' |________________________|'def
  END
ELSE IF menu='MAIN' THEN
  DO
    SAY pen6'       _____________'def
    SAY pen6'  ____/  'pen3'Main Menu'pen6'  \_____'def
    SAY pen6' |                        |'def
    SAY pen6' |'def'   ['pen3'H'def']elp               'pen6'|'def
    SAY pen6' |'def'   ['pen3'I'def']nfomation         'pen6'|'def
    SAY pen6' |'def'   ['pen3'J'def']ump to doorways   'pen6'|'def
    SAY pen6' |'def'   ['pen3'Y'def']our user data     'pen6'|'def
    SAY pen6' |'def'   ['pen3'W'def']ho is here list   'pen6'|'def
    SAY pen6' |'def'   ['pen3'S'def']earch userlist    'pen6'|'def
    SAY pen6' |'def'   ['pen3'O'def']ther users info   'pen6'|'def
    SAY pen6' |'def'   ['pen3'V'def']iew user log      'pen6'|'def
    SAY pen6' |'def'   ['pen3'X'def']pert (no menus)   'pen6'|'def
    SAY pen6' |'def'   ['pen3'#'def'] toggle colors    'pen6'|'def
    SAY pen6' |'def'   ['pen3'$'def'] toggle menu(s)   'pen6'|'def
    SAY pen6' |'def'   ['pen3'&'def'] user profiles    'pen6'|'def
    SAY pen6' |'def'   ['pen3'Z'def'] bbs statistics   'pen6'|'def
    SAY pen6' |'def'   ['pen3','def'] hourly stats     'pen6'|'def
    SAY pen6' |'def'   ['pen3'G'def']oodbye (hangup)   'pen6'|'def
    SAY pen6' |'def'   ['pen3'F'def']iles menu         'pen6'|'def
    SAY pen6' |'def'   ['pen3'M'def']essages menu      'pen6'|'def
    SAY pen6' |________________________|'def
  END
ELSE IF menu='ALL' THEN
  DO
    SAY pen6'     __________________________________________________________'def
    SAY pen6'  __/   'pen3'Main Menu            File Menu          Message Menu 'pen6'  \__'def
    SAY pen6' |                                                                |'def
    SAY pen6' |'def' ['pen3'H'def']elp               ['pen3'A'def']lphabetical list  ['pen3'P'def']ost messages      'pen6'|'def
    SAY pen6' |'def' ['pen3'I'def']nformation        ['pen3'B'def']rowse filenotes   ['pen3'R'def']ead messages      'pen6'|'def
    SAY pen6' |'def' ['pen3'Z'def'] bbs statiZtics   ['pen3'L'def']ist by Library    ['pen3'E'def']mail (private)    'pen6'|'def
    SAY pen6' |'def' ['pen3'Y'def']our user data     ['pen3'N'def']ew files          ['pen3'C'def']omment to SYSOP   'pen6'|'def
    SAY pen6' |'def' ['pen3'O'def']ther users info   ['pen3'F'def']ilelist archiver  ['pen3'!'def'] YELL for SYSOP   'pen6'|'def
    SAY pen6' |'def' ['pen3'J'def']ump to doorways   ['pen3'+'def'] Extra Devices    ['pen3'X'def']pert (no menus)   'pen6'|'def
    SAY pen6' |'def' ['pen3'S'def']earch menu        ['pen3'D'def']ownload           ['pen3'$'def'] toggle menu(s)   'pen6'|'def
    SAY pen6' |'def' ['pen3'&'def'] user profiles    ['pen3'U'def']pload             ['pen3'#'def'] toggle colors    'pen6'|'def
    SAY pen6' |'def' ['pen3'V'def']iew user log      ['pen3'T'def']ransfer protocol  ['pen3','def'] hourly stats     'pen6'|'def
    SAY pen6' |'def' ['pen3'G'def']oodbye (logoff)   ['pen3'QUICK'def'] options      ['pen3'FL'def'] Friends List    'pen6'|'def
    IF(level>sysoplevel) THEN DO
    SAY pen6' |'def' ['pen3'K'def']ill a user        ['pen3'%'def'] edit filenote    ['pen3'='def'] level report     'pen6'|'def
    SAY pen6' |'def' ['pen3'^'def'] view BBS logs    ['pen3'('def'] file report      ['pen3';'def'] change username  'pen6'|'def;END
    IF(level=99) THEN
    SAY pen6' |'def' ['pen3'~'def'] online editor    ['pen3'@'def'] dos shell        ['pen3')'def'] email report     'pen6'|'def
    SAY pen6' |________________________________________________________________|'def
  END
SAY
RETURN


help:
ARG helppath .
SAY
SAY 'For more detailed help, use ['pen3'I'def']nformation commmand to read BBBBS.COMMANDS.'
IF helppath='MAIN' THEN
  SAY 'Commands available from the' pen3||menu||def 'menu:'
frontend=bbspath'BBS_HELP/'helppath
backend='.USER'
IF level=0 THEN backend='.NEW'
ELSE IF level=99 THEN backend='.SUPER'
ELSE IF level>sysoplevel THEN backend='.SYSOP'
CALL showtext(frontend||backend 1)
RETURN


waiting:
IF waitchar='Q' THEN
  DO
    waitchar=''
    RETURN
  END
waitchar=''
IF nonstop=1 THEN RETURN
OPTIONS PROMPT pen3'                       RETURN=Continue  'def
PULL waitchar
CALL cleanline(1)
RETURN


waiting2:
IF nonstop=1 THEN RETURN 0
waitchar=getinput(1 1 pen3'   Q=Quit   N=Non-Stop   RETURN=Continue  'def)
IF waitchar='N' THEN
  DO
    nonstop=1
    SAY pen3'To EXIT non-stop scrolling of text, press CTRL-E        'def
    SAY
    CALL DELAY(99)
    waitchar=''
  END
CALL cleanline(1)
IF waitchar='Q' THEN RETURN 1
RETURN 0


busywait:
ARG bii bi bt 
IF bbsprefs.21=0 THEN RETURN
IF bi<1 THEN
  DO
    CALL WRITECH(STDOUT,'080808'x)
    IF ni<1 & i>999998 & wi>999998 THEN SAY
    RETURN
  END
IF bi=1 THEN CALL WRITECH(STDOUT,'   ')
IF bi//(bii%2)~=0 THEN RETURN
b=bi//bii
IF b=0 | b=bii%2 THEN
  DO
    tp=RIGHT((bi*100)%bt,2)'%'
    CALL WRITECH(STDOUT,'080808'x||tp)
  END
RETURN


cleanline:
ARG lflag .
IF nonstop=0 & clr~='' THEN
  DO
    SAY clr
    RETURN
  END
cline=lineup||LEFT(' ',77)
IF lflag=1 THEN cline=cline||lineup
SAY cline
RETURN


getinput:
PARSE ARG upflag' 'oneflag' 'pline
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
RETURN inarg


docity:
PARSE ARG citi
citi=TRANSLATE(citi,'          ','+-.,*/()<>')
DO i=WORDS(citi) TO 1 BY -1
  IF DATATYPE(WORD(citi,i),'N') THEN citi=STRIP(DELWORD(citi,i,1))
  IF UPPER(WORD(citi,i))='USA' THEN citi=STRIP(DELWORD(citi,i,1))
END
citi=SPACE(citi,1)
RETURN STRIP(citi)


setdir:
PARSE ARG tempdir
CALL PRAGMA('D',STRIP(tempdir))
directory=PRAGMA('D')
slash=LASTPOS('/',directory)
IF slash=0 THEN slash=LASTPOS(':',directory)
plaindir=directory
IF slash>0 THEN plaindir=SUBSTR(plaindir,slash+1)
RETURN


config:
arg='s:CONFIG.BBS'
IF ~EXISTS(arg) THEN arg='BBS:BBS_TEXT/CONFIG.BBS'
IF readlines(arg 1) THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS_TEXT/CONFIG.BBS are both missing!'
    SIGNAL DONE2
  END
compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
bbsname=STRIP(lynes.1)
CALL SETCLIP('BBS_bbsname',bbsname)
sysop=WORD(lynes.2,1)
compos=POS('/*',lynes.3)
IF compos>0 THEN lynes.3=LEFT(lynes.3,compos-1)
exclusion=STRIP(lynes.3)
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    SIGNAL DONE2
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
CALL SETCLIP('BBS_path',bbspath)
msgpath=WORD(lynes.7,1)
IF ~EXISTS(msgpath) THEN
  DO
    SAY msgpath 'does not exist!'
    SIGNAL DONE2
  END
testchar=RIGHT(msgpath,1)
IF testchar~='/' & testchar~=':' THEN msgpath=msgpath'/'
CALL SETCLIP('BBS_msgpath',msgpath)
msgpath=msgpath'MSG'
libpath=WORD(lynes.8,1)
IF ~EXISTS(libpath) THEN
  DO
    SAY libpath 'does not exist!'
    SIGNAL DONE2
  END
testchar=RIGHT(libpath,1)
IF testchar~='/' & testchar~=':' THEN libpath=libpath'/'
CALL SETCLIP('BBS_libpath',libpath)
extdevs=''
DO i=1 TO WORDS(lynes.10)
  test=WORD(lynes.10,i)
  IF POS(':',test)=0 THEN ITERATE i
  IF LEFT(test,2)='/*' THEN LEAVE i
  extdevs=STRIP(extdevs test)
END
SYSTEM_MSG_LIMIT=WORD(lynes.11,1)
SYSTEM_SPACE_LIMIT=WORD(lynes.12,1)
maxidle=WORD(lynes.13,1)
maxbps=WORD(lynes.15,1)
IF ~DATATYPE(maxbps,'W') THEN maxbps=2400
CALL SETCLIP('BBS_baud',maxbps)
DO i=16 TO 41
  j=i-15
  bbsprefs.j=STRIP(WORD(lynes.i,1))
END
spellpath=WORD(lynes.9,1)
IF bbsprefs.5 & ~EXISTS(spellpath) THEN
  DO
    SAY spellpath 'does not exist!'
    bbsprefs.5=0
  END
IF bbsprefs.10 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'
CALL MAKEDIR(scratch)
IF ~DATATYPE(bbsprefs.16,'W') THEN bbsprefs.16=3
extension=WORD(lynes.32,1)
arccom=lynes.33
compos=POS('/*',lynes.33)
IF compos>0 THEN lynes.33=LEFT(lynes.33,compos-1)
arccom=STRIP(lynes.33)
IF LEFT(extension,1)~='.' THEN
  DO
    extension='.lzh'
    arccom='lharc -m m'
  END
real=0
IF WORD(lynes.43,1)=1 THEN real=1
RETURN


readlogs:
t=getinput(1 1 'Read [D]aily, [N]umbers, or [Q]uick log? (dnq) > ')
IF t='' THEN RETURN
IF t='D' THEN
  DO
    arg=getinput(1 0 '['pen3'RETURN'def']=TODAY, or enter Log Date ('pen3||DATE('S')||def') > ')
    IF arg='' THEN arg=DATE('S')
    arg=bbspath'Logs/log.'arg
  END
ELSE IF t='N' THEN arg=bbspath'logs/QUICK.log'
ELSE IF t='Q' THEN arg=bbspath'logs/Numbers.log'
ELSE RETURN
CALL showtext(arg 1)
RETURN


loadcourtesy:
IF courtesyflag=0 & courtesy='' & EXISTS(bbspath'Lists/Courtesy') THEN
  DO
    IF readopen(bbspath'Lists/Courtesy') THEN
      DO
        SAY 'Checking Courtesy List...'
        DO i=1
          line=READLN(f)
          IF EOF(f) THEN BREAK
          courtesy=courtesy UPPER(line)
        END
        CALL CLOSE(f)
        MSG ''
        MSG pen3'Courtesy List:'def
        MSG courtesy
      END
  END
RETURN


fileheader:
SAY 'Filename          Bytes File# Library         KeyWords'
SAY pen3||LEFT('=',77,'=')||def
RETURN


showalpha:
libtext=0
IF DATATYPE(arg,'W') THEN
  DO
    dirnum=arg
    arg=''
    test='Y'
    IF chdir2()>0 THEN
      DO
        libtext=1
        RETURN
      END
  END
ELSE
  DO
    test=getinput(1 1 'Show one library only? (Ny) > ')
    IF test='Y' THEN
      DO
        IF chdir()>0 THEN
          DO
            libtext=1
            RETURN
          END
      END
  END

showalpha2:
libtext=1
IF test='Y' THEN
  DO
    lfile=libpath||plaindir'/.'STRIP(LEFT(plaindir,15))
    IF EXISTS(lfile) THEN
      DO
        CALL showtext(lfile 1)
        nonstop=0
        RETURN
      END
    filecount=WORDS(SHOWDIR(bbspath'FileNotes/'plaindir))
  END
ELSE filecount=files.0
SAY '  'filecount 'files.'
CALL fileheader()
count=0
DO wi=1 TO alpha.0
  CALL busywait(60 wi alpha.0)
  IF test='Y' THEN
    DO
      IF count>=filecount THEN LEAVE wi
      IF UPPER(LEFT(plaindir,12))~=UPPER(LEFT(WORD(alpha.wi,5),12)) THEN
        ITERATE wi
    END
  jj=WORD(alpha.wi,4)
  IF jj>level | FIND(data.21,UPPER(dirs.jj))>0 THEN
    ITERATE wi
  CALL busywait(4 0)
  SAY LEFT(alpha.wi,76)
  count=count+1
  IF (count+2)//linesperpage=0 & wi<alpha.0 THEN
    IF waiting2() THEN
      DO
        CALL busywait(4 1)
        LEAVE wi
      END
  CALL busywait(4 1)
END
CALL busywait(4 0)
nonstop=0
IF waitchar~='Q' THEN CALL waiting()
RETURN


otheruser:
SAY lm
CALL bbsOther.rexx(3000 name sysoplevel real bbspath bbsname)
RETURN


changename:
ARG cname
IF level<=sysoplevel THEN RETURN
IF cname='' THEN cname=getinput(1 0 'Current Username (include underscore): ')
IF readlines(bbspath'Users/'cname 1)>0 THEN RETURN
IF WORD(lynes.20,1)>level THEN RETURN
CALL SETCLIP('BBS_oldname',cname)
CALL ChangeUserName.rexx()
ncname=GETCLIP('BBS_newname')
IF name=cname THEN name=ncname
CALL DELETE(bbspath'Lists/USERS')
sortuserflag=1
CALL SETCLIP('BBS_oldname')
CALL SETCLIP('BBS_newname')
RETURN ncname


levelreport:
SAY lm
CALL bbsNewUsers.rexx(name level 1 3000)
RETURN


filereport:
SAY 'Searching for mismatches between files and filenotes...'
DO i=1 TO sysoplevel+1
  IF dirs.i='' THEN ITERATE
  SAY dirs.i'                               'lineup
  rfiles=SHOWDIR(libpath||dirs.i)
  rnotes=SHOWDIR(bbspath'FileNotes/'dirs.i)
  IF WORDS(rfiles)~=WORDS(rnotes) THEN
    DO
      line='Compare files & filenotes in'pen3 dirs.i||def'. '
      DO j=1 TO WORDS(rfiles)
        IF FIND(UPPER(rnotes),UPPER(WORD(rfiles,j)))=0 THEN
          line=line WORD(rfiles,j)
      END
      SAY line
    END
END
SAY '07'x
CALL waiting()
RETURN


mailreport:
SAY 'Checking ALL pending Email...'
SAY pen3' - Use CTRL-E to Exit -'def
SAY
mailrep=SHOWDIR(bbspath'Email','D')
mailfil=SHOWDIR(bbspath'EmailFiles','D')
lastemail=WORD(data.17,3)
IF ~DATATYPE(lastemail,'W') THEN lastemail=0
IF lastemail=countcheck('LastMail' 0) THEN
  DO
    DROP mailrep. mailfil.
    RETURN
  END
mailynes.=''
mk=0
DO mi=1 TO WORDS(mailrep)
  muser=WORD(mailrep,mi)
  IF muser=sysop | muser=name THEN ITERATE mi
  mlist=SHOWDIR(bbspath'Email/'muser)
  IF WORDS(mlist)>0 THEN SAY lineup||RIGHT(muser,40)
  DO mj=1 TO WORDS(mlist)
    fuser=WORD(mlist,mj)
    IF POS(sysop,fuser)>0 THEN ITERATE mj
    IF logonflag=0 THEN
      DO
        mk=mk+1
        mailynes.mk=pen3||LEFT(muser,20) 'from'def LEFT(fuser,20) DATE(,WORD(STATEF(bbspath'Email/'muser'/'fuser),5),'I')
      END
    IF POS(sysop,fuser)=0 & POS(name,fuser)=0 THEN
      DO
        testnum=RIGHT(fuser,LENGTH(fuser)-LASTPOS('.',fuser))
        IF testnum>emailnum THEN emailnum=testnum
        IF testnum>lastemail THEN
          DO
            CALL showtext(bbspath'Email/'muser'/'fuser 1)
            SAY
            SAY
            IF waitchar='Q' THEN LEAVE mi
          END
      END
  END
  IF logonflag=0 & FIND(mailfil,muser)>0 THEN
    DO
      efilelist=SHOWDIR(bbspath'EmailFiles/'muser)
      IF WORDS(efilelist)>0 THEN
        DO
          mk=mk+1
          mailynes.mk=pen3||LEFT(muser,20) 'emailfiles'def efilelist
        END
    END
END
data.17=WORD(data.17,1) WORD(data.17,2) countcheck('LastMail' 0)
IF mk>0 THEN
  DO
    lynes.0=mk
    DO mi=1 TO mk
      lynes.mi=mailynes.mi
    END
    CALL seelines(1)
    nonstop=0
    CALL waiting()
  END
ELSE SAY 'No unseen Email pending.'
DROP mailrep. mailfil. mailynes. mlist
RETURN


jump2rexx:
arg=bbspath'BBS_TEXT/REXXDOORS'
IF EXISTS(arg) THEN CALL showtext(arg 0)
SAY lm
CALL bbsDoors.rexx(3000 name password)
RETURN


sortlibraries:
SAY 'Sorting Libraries...'
count=0
sdirs.=''
DO i=1 TO level
  IF dirs.i='' THEN ITERATE i
  count=count+1
  sdirs.count=dirs.i i
END
sdirs.0=count
IF count>0 THEN CALL QSort(1,count,sdirs)
count=0
libs.=''
DO i=1 TO sdirs.0
  tempnum=WORD(sdirs.i,2)
  tempdir=WORD(sdirs.i,1)
  IF FIND(data.21,UPPER(tempdir))=0 THEN
    DO
      string=' '
      IF tempnum<10 THEN string=string' '
      string=string || tempnum'. 'LEFT(tempdir,14)
      count=count+1
      libs.count=string
    END
END
libs.0=count%4
IF (count//4)>0 THEN libs.0=libs.0+1
DO i=1 TO libs.0
  DO j=1 TO 3
    k=i+j*libs.0
    IF k<=count THEN libs.i=libs.i||libs.k
  END
END
RETURN


sortconferences:
SAY 'Sorting Conferences...'
count=0
smsg.=''
DO i=1 TO level
  IF msg.i='' THEN ITERATE i
  count=count+1
  smsg.count=msg.i i
END
smsg.0=count
IF count>0 THEN CALL QSort(1,count,smsg)
count=0
msgs.=''
DO i=1 TO smsg.0
  tempnum=WORD(smsg.i,2)
  tempdir=WORD(smsg.i,1)
  IF FIND(data.21,tempnum)=0 THEN
    DO
      string=' '
      IF tempnum<10 THEN string=string' '
      string=string || tempnum'.'
      IF WORD(data.22,tempnum)='' | WORD(data.22,tempnum)>=0 THEN
        string=string LEFT(tempdir,20)
      ELSE string=string pen2'-OFF-'def LEFT(tempdir,14)
      count=count+1
      msgs.count=string
    END
END
msgs.0=count%3
IF (count//3)>0 THEN msgs.0=msgs.0+1
DO i=1 TO msgs.0
  DO j=1 TO 2
    k=i+j*msgs.0
    IF k<=count THEN msgs.i=msgs.i msgs.k
  END
END
RETURN


readmessages:
CALL SETCLIP('BBSMSG_ARG',colorflag arg)
CALL bbsMsg.rexx(6000 name password)
SAY lm
CALL loaddata()
CALL checkemail()
RETURN


showmarked:
ARG ff .
IF WORDS(data.24)<1 THEN RETURN
fline='These unread conference messages have been ['pen3'M'pen6']arked as addressed to you:'
IF ff THEN
  DO
    SAY
    SAY pen6||fline||def
  END
tempkk=data.24
DO i=1 TO WORDS(tempkk)
  tempk=WORD(tempkk,i)
  PARSE VAR tempk kdir'/'kmsg
  line=RIGHT(kmsg,6) 'in the'pen3 msg.kdir def'conference'
  IF EXISTS(msgpath||tempk) THEN
    DO
      IF ff THEN SAY line'.'
      ELSE fline=fline'0A'x||line'.'
    END
  ELSE
    DO
      line=line 'is missing.'
      IF ff THEN SAY line
      ELSE fline=fline'0A'x||line
      mkw=FIND(data.24,tempk)
      data.24=STRIP(DELWORD(data.24,mkw,1))
      CALL savedata(0)
    END
END
IF ff THEN
  DO
    CALL waiting()
    SAY
  END
ELSE
  DO
    IF writeopen(bbspath'EmailFiles/'name'/Marked')=0 THEN RETURN
    CALL WRITELN(f,fline)
    CALL CLOSE(f)
  END
RETURN


readmail:
ARG fromenu .
replysubj=''
IF fromenu THEN SAY lm
ELSE arg=''
CALL SETCLIP('BBSMAIL_ARG',fromenu arg)
CALL bbsMail.rexx(3000 name password)
CALL checkemail()
RETURN


checkemail:
x=GETCLIP('LOCAL_email')
CALL SETCLIP('LOCAL_email')
If DATATYPE(x,'W') THEN
  IF emailonline>-1 THEN emailonline=emailonline+x
RETURN


countcheck:
PARSE ARG fname' 'cknum .
fname=bbspath'Numbers/'fname
IF ~EXISTS(fname) THEN
  DO
    IF cknum=0 THEN RETURN 0
    IF ~writeopen(fname) THEN RETURN 0
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN cknum
  END
IF ~readopen(fname) THEN
  DO
    CALL DELAY(99)
    IF ~readopen(fname) THEN RETURN cknum
  END
retval=STRIP(READLN(f))
CALL CLOSE(f)
IF ~DATATYPE(retval,'W') THEN retval=0
IF ~DATATYPE(cknum,'W') THEN cknum=0
IF retval<cknum THEN
  DO
    IF writeopen(fname) THEN
      DO
        CALL WRITELN(f,cknum)
        CALL CLOSE(f)
        RETURN cknum
      END
  END
RETURN retval


sysEd:
IF level<99 THEN RETURN
arg=getinput(0 0 'Textfile To Edit: ')
IF arg='' THEN RETURN
SAY lm
CALL bbsEd.rexx(1 arg name)
CALL checkfilechanges()
RETURN


editor:
PARSE ARG edarg
SAY lm
IF bbsWrite.rexx(edarg)=0 THEN RETURN
IF WORD(edarg,3)='MAIL' THEN
  DO
    IF emailonline>=0 THEN emailonline=emailonline+1
  END
ELSE
  DO
    grand=grand+1
    IF ~DATATYPE(msg.msgdir.0,'W') THEN msg.msgdir.0=1
    ELSE msg.msgdir.0=msg.msgdir.0+1
  END
CALL loaddata()
RETURN


edinfo:
PARSE ARG t1,t2,t3
IF level<sysoplevel THEN RETURN 0
IF getinput(1 1 'Edit the'pen3 t2 def||t3 'info file? (Ny) > ')='Y' THEN
  DO
    IF ~EXISTS(t) THEN
      DO
        IF writeopen(t1)~=0 THEN
          DO
            CALL WRITELN(f,TRIM(CENTER('***'pen3 t2 def||t3 '***',75)))
            CALL WRITELN(f,LEFT('',75,'='))
            CALL CLOSE(f)
            CALL DELAY(28)
          END
      END
    CALL bbsEd.rexx(1 t1 name)
    arg=''
    RETURN 1
  END
RETURN 0


shell:
SAY
olddir=PRAGMA('D')
DO WHILE(UPPER(opt)~='EXIT')
  SAY bak2||TIME('C')||def PRAGMA('D')
  OPTIONS PROMPT pen3'Type EXIT to quit AmigaDOS> 'def
  PARSE PULL opt' 'arg
  IF(UPPER(opt)='CD') THEN CALL setdir(arg)
  ELSE IF EXISTS(opt)~=0 THEN
    DO
      IF LEFT(STATEF(opt),3)='DIR' THEN CALL setdir(opt)
    END
  ELSE IF opt~='' & UPPER(opt)~='EXIT' THEN
    ADDRESS COMMAND opt '<* >*' arg
END
CALL PRAGMA('D',olddir)
RETURN


bbsspace:
ARG tabspace .
ADDRESS COMMAND 'C:info >'scratch'/locinfout' bbsdevice
ok=OPEN(f,scratch'/locinfout','R')
IF ok=0 THEN RETURN 20
line=READLN(f)
line=READLN(f)
line=READLN(f)
line=READLN(f)
CALL CLOSE(f)
IF tabspace<14 THEN SAY 
bbsk=WORD(line,4)
IF ~DATATYPE(bbsk,'N') THEN
  DO
    line=bbsdevice 'is not an info compatible device!'
    SAY pen3||line||def
    bbsk=0
    RETURN
  END
bbsk=bbsk*512-SYSTEM_SPACE_LIMIT
IF bbsk<1 THEN bbsk=0
SAY RIGHT(comma(bbsk),tabspace) 'bytes available for uploads.'
RETURN


comma: PROCEDURE
ARG num .
t=''
x=POS('.',num)
IF x>0 THEN t=SUBSTR(num,x)
num=num%1
dgt=LENGTH(num)
numtext=''
IF dgt>3 THEN numtext=','RIGHT(num,3)
IF dgt>6 THEN numtext=','LEFT(RIGHT(num,6),3)||numtext
IF dgt>9 THEN numtext=','LEFT(RIGHT(num,9),3)||numtext
IF dgt>12 THEN
  DO
    numtext=','LEFT(RIGHT(num,12),3)||numtext
    numtext=LEFT(num,dgt-12)||numtext
  END
ELSE IF dgt>9 THEN numtext=LEFT(num,dgt-9)||numtext
ELSE IF dgt>6 THEN numtext=LEFT(num,dgt-6)||numtext
ELSE IF dgt>3 THEN numtext=LEFT(num,dgt-3)||numtext
ELSE numtext=num
RETURN numtext||t


is_here:
ARG newname 
CALL WRITECH(STDOUT,'Checking filelist')
DO wi=1 TO 99
  IF wi//3=0 THEN CALL WRITECH(STDOUT,'.')
  IF dirs.wi='' THEN ITERATE wi
  IF ~EXISTS(bbspath'FileNotes/'dirs.wi'/'newname) THEN ITERATE wi
  line=pen3'*** File' newname 'already exists here'
  IF wi<=level THEN line=line 'in the' dirs.wi 'library'
  line=line'.'def
  SAY line
  SAY 'Original uploader should ['pen3'K'def']ill the file before uploading the replacement.'
  CALL waiting()
  RETURN 1
END
CALL cleanline(1)
RETURN 0


uload:
ARG frommenu
CALL bbsspace(12)
SAY
IF bbsk<1 THEN
  DO
    SAY pen3'Upload area is full!'def
    RETURN 1
  END
IF ~SHOW('P','BUILDALPHA') THEN CALL SETCLIP('BBS_UPLOAD')
IF frommenu & GETCLIP('BBS_UPLOAD')~='' THEN
  DO
    SAY pen3'Uploading is temporarily suspended while the filelists are rebuilding.'def
    CALL waiting()
    RETURN 1
  END
IF arg='' THEN
  DO
    frompath=GETCLIP('BBS_frompath')
    IF frompath='' THEN frompath='RAM:'
    fdir=''
    DO loop=1
      fromfile=GetFile(150,36,frompath,'',' Select File to Upload ')
      IF fromfile='' THEN RETURN 1
      IF EXISTS(fromfile) THEN LEAVE loop
      SAY
      SAY fromfile 'does not exist!'
    END
    x=LASTPOS('/',fromfile)
    IF x=0 THEN x=POS(':',fromfile)
    IF x>0 THEN
      DO
        arg=SUBSTR(fromfile,x+1)
        fdir=LEFT(fromfile,x)
        IF RIGHT(fdir,1)='/' THEN fdir=LEFT(fdir,x-1)
        CALL SETCLIP('BBS_frompath',fdir)
      END
    ELSE arg=fromfile
  END
ELSE fromfile=PRAGMA('D')'/'arg
arg=COMPRESS(arg,' :/,;|#?*()+[]"{}')  /* be sure no illegals here */
x=LASTPOS('/',fromfile)
IF x=0 THEN x=LASTPOS(':',fromfile)
IF x>0 THEN
  DO
    IF DATATYPE(SUBSTR(fromfile,x+1),'W') THEN
      DO
        SAY 'Whole numbers are not allowed as filenames!'
        CALL waiting()
        RETURN 1
      END
  END
tempnum=LENGTH(arg)-16
DO WHILE tempnum>0 & POS('EMAILFILES',UPPER(PRAGMA('D')))=0
  temp='          'pen3||arg def'is'pen3 tempnum||def
  IF tempnum=1 THEN temp=temp 'character'
  ELSE temp=temp 'characters'
  temp=temp 'too long for a filename.'
  SAY temp  
  arg=getinput(0 0 'Filename: ')
  arg=cleanstring('0:'arg)
  arg=COMPRESS(arg,' :/,;|#?*')
  tempnum=LENGTH(arg)-16
END
IF arg='' THEN RETURN 1
IF frommenu THEN
  DO
    IF is_here(arg) THEN RETURN 1
    IF bbsprefs.6=1 & sysoplevel>level THEN CALL setdir(libpath'Sysops')
    ELSE
      DO
        SAY 'Please select an appropriate library for -' pen3||arg def'-'
        IF chdir()>0 THEN RETURN
      END
  END
ADDRESS COMMAND 'C:COPY' fromfile PRAGMA('D')'/'arg
IF TestArc.rexx(PRAGMA('D')'/'arg)>0 THEN
  DO
    SAY
    SAY pen3'***'def arg pen3'failed archive check!'def
    SAY
    temp=getinput(1 1 'Do you believe the archive checker made a mistake? (Ny) > ')
    IF temp~='Y' THEN
      DO
        CALL DELETE(arg)
        SAY
        RETURN 2
      END
  END
IF POS('EMAILFILES',UPPER(PRAGMA('D')))>0 THEN RETURN 0
DO ui=sysoplevel+2 TO 100
  IF UPPER(dirs.ui)=UPPER(plaindir) THEN RETURN 0
END
IF frommenu THEN
  DO
    DO WHILE editnote(bbspath'FileNotes/'plaindir'/'arg) /* INSIST on a filenote */
    END
    CALL DELETE(libpath||plaindir'/.'STRIP(LEFT(plaindir,15)))
  END
RETURN 0


findfiles:
PARSE ARG ffile .
IF POS('EMAILFILES',UPPER(PRAGMA('D')))>0 THEN RETURN ffile
wi=0
IF DATATYPE(ffile,'W') THEN
  DO
    IF WORDS(files.ffile)<2 THEN RETURN 0
    dirtemp=WORD(files.ffile,1)
    IF finddirnum(dirtemp)>level | FIND(data.21,UPPER(dirtemp))>0 THEN
      DO
        CALL illegal_access()
        RETURN 0
      END
    CALL setdir(libpath||dirtemp)
  END
ELSE IF EXISTS(ffile) THEN
  DO
    IF EXISTS(bbspath'FileNotes/'plaindir'/'ffile) THEN
      DO
        IF readopen(bbspath'FileNotes/'plaindir'/'ffile)~=0 THEN
          DO
            line=READLN(f)
            CALL CLOSE(f)
            ffile=WORD(line,2)
          END
      END
  END
ELSE IF EXISTS(bbspath'Information'ffile) THEN
  RETURN bbspath'Information/'ffile
ELSE
  DO
    nextfilenum=countcheck('LastFile' 0)+1
    CALL busywait(4 1)
    DO ni=nextfilenum TO 0 BY -1
      IF ni<1 THEN
        DO
          CALL busywait(4 0)
          SAY '***' files.0 'filenames scanned,'pen3 ffile def'was not found!'
          RETURN 0
        END
      IF ni>1 THEN CALL busywait(60 ni nextfilenum)
      argtemp=WORD(files.ni,2)
      IF UPPER(argtemp)=UPPER(ffile) THEN
        DO
          dirtemp=WORD(files.ni,1)
          jj=files.ni.0
          IF WORD(alpha.jj,4)>level | FIND(data.21,UPPER(dirtemp))>0 THEN
            DO
              CALL busywait(4 0)
              CALL illegal_access()
              RETURN 0
            END
          ffile=ni
          CALL setdir(libpath||dirtemp)
          LEAVE ni
        END
    END
    CALL busywait(4 0)
  END
IF wi=999999 THEN RETURN 0
ftemp=ffile
IF DATATYPE(ftemp,'W') THEN ftemp=WORD(files.ftemp,2)
IF ~EXISTS(ftemp) THEN
  DO
    finfo=STATEF(bbspath'FileNotes/'plaindir'/'ftemp)
    IF WORDS(finfo)>7 THEN ftemp=WORD(finfo,8)
    IF ~EXISTS(ftemp) THEN
      DO
        IF finfo='' THEN SAY '***'pen3 PRAGMA('D')'/'ftemp def'was not found!'
        ELSE
          DO
            SAY
            SAY '***'pen3 plaindir'/'ftemp def'is not currently available online.'
            SAY 'Please leave email to your sysop'pen3 sysop||def', to receive this file.'
            SAY
          END
        RETURN 0
      END
  END
RETURN ffile


illegal_access:
SAY
SAY '*** You are not authorized to access' ffile'!'
SAY '*** Send Email to' sysop 'to receive a higher level.'
SAY
RETURN


ext_dload:
SAY
arg=bbsExtDL.baud(name level 3000 linesperpage colorflag extdevs)
IF arg~='' THEN SAY 'Sorry, LOCAL mode cannot download from the Extra Devices.'
RETURN


dload:
arg=STRIP(arg data.25)
data.25=''
errorflag=0
curdir=PRAGMA('D')
OPTIONS PROMPT 'File numbers (and/or names): '
IF arg='' THEN PARSE PULL arg  /* no filename given */
IF arg='' THEN RETURN 0
IF findfiles(arg)=0 THEN RETURN 0
arg=TRANSLATE(arg,'     ',':/,;|')
IF WORDS(arg)>1 THEN arg=WORD(arg,1)
IF DATATYPE(arg,'W') THEN
  DO
    CALL setdir(libpath||WORD(files.arg,1))
    arg=WORD(files.arg,2)
  END
IF arg~='' THEN           /* check for filename */
  DO dloadloop=1
    frompath=GETCLIP('BBS_frompath')
    IF frompath='' THEN frompath=libpath'SysOps/'
    notename=bbspath'FileNotes/'plaindir'/'arg
    IF ~EXISTS(arg) THEN
      DO
        finfo=STATEF(notename)
        IF WORDS(finfo)>7 THEN
          DO
            temp=plaindir
            x=lastslash(WORD(finfo,8))
            arg=WORD(x,1)
            CALL setdir(WORD(x,2))
            plaindir=temp
          END
      END
    topath=PRAGMA('D')
    num=LASTPOS('/',arg)
    IF num=0 THEN num=LASTPOS(':',arg)
    IF num>0 THEN
      DO
        topath=LEFT(arg,num)
        arg=SUBSTR(arg,num+1)
      END
    IF RIGHT(topath,1)~=':' & RIGHT(topath,1)~='/' THEN topath=topath'/'
    SAY ' Select Filename to Copy ' topath||arg 'To:'
    tofile=GetFile(150,36,frompath,arg,' Select Destination Name ')
    IF tofile='' THEN
      DO
        errorflag=1
        LEAVE dloadloop
      END
    ADDRESS COMMAND 'C:Copy' topath||arg tofile
    CALL SETCLIP('BBS_frompath',WORD(lastslash(tofile),2))
    IF POS('EMAILFILES',UPPER(PRAGMA('D')))>0 THEN LEAVE dloadloop
    DO di=sysoplevel+2 TO 100
      IF UPPER(dirs.di)=UPPER(plaindir) THEN LEAVE dloadloop
    END
    IF readlines(notename 1) THEN LEAVE dloadloop
    dls=WORD(lynes.2,7)
    IF ~DATATYPE(dls,'W') THEN dls=0
    lynes.2=STRIP(DELWORD(lynes.2,7,1)) dls+1
    finfo=STATEF(notename)
    IF WORDS(finfo)>7 THEN finfo=SUBSTR(finfo,WORDINDEX(finfo,8))
    ELSE finfo=''
    CALL DELETE(notename)
    CALL savelines(notename)
    CALL DELAY(28)
    IF finfo~='' THEN ADDRESS COMMAND 'C:FileNote' notename finfo
    LEAVE dloadloop
  END
CALL setdir(curdir)
IF errorflag THEN SAY pen3'*** Download Failed!'def
RETURN errorflag


lastslash:
PARSE ARG sarg 
sdir=''
slash=LASTPOS('/',sarg)
IF slash>2 THEN sdir=LEFT(sarg,slash-1)
ELSE
  DO
    slash=LASTPOS(':',sarg)
    IF slash>0 THEN sdir=LEFT(sarg,slash)
  END
IF slash>0 THEN sarg=SUBSTR(sarg,slash+1)
RETURN sarg sdir


editnote:
IF arg='' THEN
  DO
    PARSE PULL arg .
    IF arg='' THEN RETURN 0
  END
comment=''
IF ~EXISTS(arg) THEN
  DO
    finfo=STATEF(bbspath'FileNotes/'plaindir'/'arg)
    fromarg=arg
    fromdir=GETCLIP('BBS_frompath')
    IF WORDS(finfo)>7 THEN
      DO
        temp='Y'
        fromdir=WORD(finfo,8)
        fromdir=lastslash(fromdir)
        fromarg=WORD(fromdir,1)
        fromdir=WORD(fromdir,2)
      END
    ELSE
      DO
        IF level<sysoplevel THEN RETURN 0
        temp=getinput(1 1 'Is this file on an another device? (Nqy)')
      END
    IF fromdir='' THEN fromdir='RAM:'
    IF temp='Y' THEN
      DO WHILE comment=''
        comment=GetFile(150,36,fromdir,fromarg,' Select Linked File ')
        IF comment='' THEN RETURN 0
        IF ~EXISTS(comment) THEN comment=''
        ELSE CALL SETCLIP('BBS_frompath',WORD(lastslash(comment),2))
      END
    ELSE IF temp~='N' THEN RETURN 0
  END
IF comment='' THEN
  DO
    arg=findfiles(arg)
    IF arg=0 THEN RETURN 0
    IF DATATYPE(arg,'W') THEN arg=WORD(files.arg,2)
  END
filedir=plaindir
slash=LASTPOS('/',arg)
IF slash=0 THEN slash=LASTPOS(':',arg)
IF slash>0 THEN
  DO
    filedir=LEFT(arg,slash-1)
    filedir=SUBSTR(filedir,5)
    arg=SUBSTR(arg,slash+1)
  END
ELSE filedir=plaindir
CALL MAKEDIR(bbspath'FileNotes/'filedir)
IF ~EXISTS(bbspath'FileNotes/'filedir) THEN
  DO
    SAY pen3'*** Failed to open directory!' filedir||def
    RETURN 0
  END
notename=bbspath'FileNotes/'filedir'/'arg
lynes.=''
filenum=countcheck('LastFile' 0)
IF level>sysoplevel THEN firstedit=1
ELSE firstedit=5
IF EXISTS(notename) THEN
  DO
    IF comment~='' THEN ADDRESS COMMAND 'C:filenote' notename comment
    CALL bbsEd.rexx(firstedit notename name)
    CALL checkfilechanges()
    RETURN 0
  END
IF comment='' THEN filedata=STATEF(libpath||filedir'/'arg)
ELSE filedata=STATEF(comment)
IF filedata='' THEN
  DO
    IF comment='' THEN line=filedir'/'arg
    ELSE line=comment
    SAY line 'does not exist!'
    RETURN 0
  END
bytes=WORD(filedata,2)
filenum=filenum+1
lynes.0=4
lynes.1='File: 'LEFT(filenum,5)' KeyWords:'
lynes.2='Name: 'LEFT(arg,27)' Size: 'bytes' bytes   Downloads: 0'
lynes.3='From: 'LEFT(name,27)' Date: 'DATE() TIME('C')'  Lib: 'filedir
lynes.4=LEFT('',74,'=')
lynes.1=lynes.1 edkeywords(arg filedir)
diz='RAM:file_id.diz'
IF EXISTS(diz) THEN CALL readlines(diz 5)
CALL DELETE(diz)
CALL seelines(1)
edtype=''
CALL writebuffer(scratch'/NoteLOCAL')
IF savelines(notename) THEN RETURN 0
IF comment~='' THEN ADDRESS COMMAND 'C:filenote' notename comment
CALL DELETE(libpath||filedir'/.'STRIP(LEFT(filedir,15)))
fncom='R'
DO WHILE fncom='R'
  CALL seelines(1)
  nonstop=0
  line='['pen3'E'def']dit'
  IF level>sysoplevel THEN line=line '['pen3'K'def']ill'
  line=line '['pen3'R'def']ead ['pen3'S'def']ave'
  IF level>sysoplevel THEN line=line '(ekrS) 'def
  ELSE line=line '(erS) 'def
  fncom=getinput(1 1 line)
  IF fncom='K' & level>sysoplevel THEN
    DO
      SAY 'Killing FileNote..'
      CALL DELETE(notename)
      RETURN 1
    END
  ELSE IF fncom='E' THEN
    DO
      IF bbsEd.rexx(firstedit notename name)>0 THEN RETURN 0
      CALL readlines(notename 1)
      CALL checkfilechanges()
      fncom='R'
    END
  ELSE IF fncom~='R' THEN
    DO
      SAY 'Adjusting filelist...'
      IF filenum<1 THEN filenum=1
      IF GETCLIP('BBS_level')~='' THEN CALL SETCLIP('BBS_localfiles',1)
      CALL countcheck('LastFile' filenum)
      files.0=files.0+1
      newcount=alpha.0+1
      alpha.0=newcount
      files.filenum=plaindir arg
      files.filenum.0=newcount
      libnum=finddirnum(plaindir)
      PARSE VAR lynes.1 . 'KeyWords:' keywords
      alpha.newcount=LEFT(arg,22-LENGTH(WORD(lynes.2,4)))
      alpha.newcount=alpha.newcount WORD(lynes.2,4) RIGHT(filenum,5)
      alpha.newcount=alpha.newcount RIGHT(libnum,2) LEFT(plaindir,12)
      alpha.newcount=alpha.newcount STRIP(LEFT(STRIP(keywords),32))
      IF EXISTS(bbspath'Lists/Files') THEN
        x=listOPEN(f,bbspath'Lists/Files','A')
      ELSE x=listOPEN(f,bbspath'Lists/Files','W')
      IF x=0 THEN
        DO
          SAY '*** Failed to open' bbspath'Lists/Files'
          savefileflag=1
          RETURN 0
        END
      CALL WRITELN(f,filenum files.filenum)
      CALL CLOSE(f)
      IF EXISTS(bbspath'Lists/Files.ALPHA') THEN
        x=listOPEN(f,bbspath'Lists/Files.ALPHA','A')
      ELSE x=listOPEN(f,bbspath'Lists/Files.ALPHA','W')
      IF x=0 THEN
        DO
          SAY '*** Failed to open' bbspath'Lists/Files.ALPHA'
          RETURN 0
        END
      CALL WRITELN(f,alpha.newcount)
      CALL CLOSE(f)
      IF EXISTS('c:shellsort') THEN loadalphaflag=1
      ELSE sortalphaflag=1
    END
END
RETURN 0


checkfilechanges:
x=GETCLIP('BBS_FileChange')
CALL SETCLIP('BBS_FileChange')
DO ii=1 TO WORDS(x)
  fnum=WORD(x,ii)
  keywords=GETCLIP('BBS_Keywords_'fnum)
  CALL SETCLIP('BBS_Keywords_'fnum)
  num=files.fnum.0
  alpha.num=TRIM(OVERLAY(keywords,alpha.num,47,32))
  sortalphaflag=1
END
RETURN


edkeywords:
PARSE ARG kwarg
templine=''
DO WHILE LENGTH(templine)<3
  SAY
  SAY pen3'Please enter a list of keywords (or a condensed description)'def
  SAY pen3'to be used in the alphabetic list and by the search routine.'def
  SAY '    Note that only the first 32 characters will be used.'
  SAY LEFT('',43)'|'LEFT('',31,'=')'|'
  templine=getinput(0 0 ' 'RIGHT(STRIP(RIGHT(kwarg,32)),32) pen3'KeyWords: 'def)
  templine=cleanstring('0:'templine)
  templine=STRIP(LEFT(templine,32))
  SAY
END
RETURN templine


loadfiles:
SAY def
IF ~listOPEN(f,bbspath'Lists/Files','R') THEN RETURN
SAY 'Loading filelist...'
files.=''
files.0=0
DO i=1
  line=READLN(f)
  IF EOF(f) THEN BREAK
  num=WORD(line,1)
  IF DATATYPE(num,'W') THEN
    DO
      IF num<100 THEN
        IF LEFT(WORD(line,3),1)~='.' THEN
          DO
            CALL CLOSE(f)
            SAY
            SAY 'Your filelists need to be renumbered, running bbsUPDATE.rexx...'
            CALL bbsUPDATE.rexx()
            SIGNAL RESET
          END
      files.num=WORD(line,2) WORD(line,3)
    END
END
files.0=i-1
CALL CLOSE(f)
RETURN


savefilelist:
IF level=99 THEN
  IF getinput(1 1 'Update filelists now? (nY) > ')='N' THEN RETURN

savefilelist2:
SIGNAL OFF BREAK_E
CALL savealphalist()
filenum=countcheck('LastFile' 0)
IF filenum<1 THEN
  DO
    IF files.0>0 THEN filenum=files.0
    ELSE RETURN
  END
xarg=bbspath'Lists/Files'
IF ~listOPEN(f,xarg,'W') THEN RETURN
SAY 'Saving filelist...'
savefileflag=0
DO i=1 TO filenum
  IF files.i~='' THEN CALL WRITELN(f,i files.i)
END
CALL CLOSE(f)
IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localfiles',2)
RETURN


loadalpha:
ARG alflag
SAY def
IF alflag THEN CALL checkliblists()
IF liblist='' THEN alflag=0
IF loadalphaflag=1 THEN
  DO
    SAY 'Sorting using fast shellsort...'
    t=bbspath'Lists/Files.ALPHA'
    ADDRESS COMMAND 'c:shellsort -c' t t
  END
IF ~listOPEN(f,bbspath'Lists/Files.ALPHA','R') THEN RETURN
SAY 'Loading the alphabetical filelist...'
loadalphaflag=0
alpha.=''
alpha.0=0
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  fnum=WORD(line,3)
  IF DATATYPE(fnum,'W') THEN
    DO
      alpha.i=line
      files.fnum.0=i
      IF alflag THEN CALL updateliblists()
    END
  ELSE i=i-1
END
CALL CLOSE(f)
tf=bbspath'Lists/Files.ALPHA.add'
IF EXISTS(tf) & ~SHOW('P','BBSFILE') THEN
  IF readopen(tf) THEN
    DO
      DO i=i
        line=READLN(f)
        IF EOF(f) THEN LEAVE i
        fnum=WORD(line,3)
        IF DATATYPE(fnum,'W') THEN
          DO
            alpha.i=line
            files.fnum.0=i
          END
        ELSE i=i-1
        IF alflag THEN CALL updateliblists()
      END
      CALL CLOSE(f)
      CALL DELETE(tf)
      CALL SETCLIP('BBS_resave_local',1)
    END
alpha.0=i-1
IF alflag THEN CALL closeliblists()
DO i=1 TO 99
  IF dirs.i='' THEN ITERATE i
  dname='.'STRIP(LEFT(dirs.i,15))
  IF files.i='' THEN
    DO
      files.i=dirs.i dname
      files.0=files.0+1
    END
  sz=WORD(STATEF(libpath||dirs.i'/'dname),2)
  IF ~DATATYPE(sz,'W') THEN sz=0
  x=files.i.0
  IF ~DATATYPE(x,'W') THEN
    DO
      x=alpha.0+1
      files.i.0=x
      alpha.0=x
      CALL SETCLIP('BBS_resave_local',1)
      CALL DELETE(libpath||dirs.i'/'dname)
    END
  alpha.x=LEFT(dname,22-LENGTH(sz)) sz RIGHT(i,5) RIGHT(i,2)
  alpha.x=alpha.x LEFT(dirs.i,12) 'alphabetical files list CONTENTS'
END
IF GETCLIP('BBS_resave_local')=1 THEN
  DO
    CALL SETCLIP('BBS_resave_local')
    sortalphaflag=1
    CALL savefilelist2()
  END
SAY
RETURN


savealphalist:
SIGNAL OFF BREAK_E
IF GETCLIP('BBS_mainfiles')~='' THEN
  DO
    CALL SETCLIP('BBS_mainfiles')
    CALL loadfiles()
    CALL loadalpha(0)
  END
CALL checkliblists()
IF sortalphaflag=1 THEN
  DO
    SAY 'Alphabetizing' alpha.0 'files...'
    IF alpha.0>0 THEN CALL QSORT(1,alpha.0,alpha)
    DO i=1 TO alpha.0
      fnum=WORD(alpha.i,3)
      files.fnum.0=i
    END
  END
sortalphaflag=0
IF files.100~='' THEN
  DO
    sz=WORD(STATEF(libpath||WORD(files.100,1)'/'WORD(files.100,2)),2)
    IF DATATYPE(sz,'W') THEN
      DO
        anum=files.100.0
        alpha.anum=OVERLAY(RIGHT(sz,7),alpha.anum,17,7)
      END
  END
IF files.101~='' THEN
  DO
    sz=WORD(STATEF(libpath||WORD(files.101,1)'/'WORD(files.101,2)),2)
    IF DATATYPE(sz,'W') THEN
      DO
        anum=files.101.0
        alpha.anum=OVERLAY(RIGHT(sz,7),alpha.anum,17,7)
      END
  END
IF ~listOPEN(f,bbspath'Lists/Files.ALPHA','W') THEN RETURN
SAY 'Saving alphabetical filelists...'
DO i=1 TO alpha.0
  ii=WORD(alpha.i,3)
  IF files.ii='' THEN alpha.i='0 0' ii '100'
  IF LEFT(alpha.i,4)='0 0 ' THEN ITERATE i
  CALL WRITELN(f,alpha.i)
  IF liblist~='' THEN CALL updateliblists()
END
CALL CLOSE(f)
CALL closeliblists()
CALL bbsALPHA.rexx(files.0 SUBSTR(extension,2) arccom)
DO i=0 TO 1
  t=GETCLIP('BBS_10'i)
  IF t='' THEN ITERATE i
  CALL SETCLIP('BBS_10'i)
  num=100+i
  files.num=TRANSLATE(t,,'/')
  files.0=files.0+1
  x=alpha.0+1
  files.num.0=x
  alpha.0=x
  sz=WORD(STATEF(libpath||t),2)
  IF ~DATATYPE(sz,'W') THEN sz=0
  dnum=finddirnum(WORD(files.num,1))
  alpha.x=LEFT(WORD(files.num,2),22-LENGTH(sz)) sz '  'num RIGHT(dnum,2)
  alpha.x=alpha.x LEFT(dirs.dnum,12)
  IF i THEN alpha.x=alpha.x 'alphabetical files list CONTENTS'
  ELSE alpha.x=alpha.x 'alphabetical by library CONTENTS'
  SAY 'Added file' num t 'to the filelists.'
  SAY 'Must now resort and resave.'
  CALL SETCLIP('BBS_resave_local',1)
END
RETURN


listOPEN:
PARSE ARG fh,listfile,flag
DO i=0 TO 59 WHILE OPEN(fh,listfile,flag)=0
  IF i//4=0 THEN SAY 'Waiting' (60-i)*5 'more seconds for' listfile 'to become available...'
  CALL DELAY(250)
END
IF i>59 THEN
  DO
    SAY '*** unable to access' listfile 'list.'
    RETURN 0
  END
RETURN 1


checkliblists:
SAY 'Checking individual library filelists...'
liblist=''
lastlib=0
cnt.=0
DO i=1 TO 99
  IF dirs.i='' THEN ITERATE i
  finfo=STATEF(libpath||dirs.i'/.'STRIP(LEFT(dirs.i,15)))
  IF finfo='' THEN liblist=liblist i
  ELSE
    DO
      sz=WORD(finfo,2)
      num=files.i.0
      IF DATATYPE(num,'W') THEN
        alpha.num=OVERLAY(RIGHT(sz,7),alpha.num,17,7)
    END
END
liblist=STRIP(liblist)
DO j=1 TO WORDS(liblist)
  tt=WORD(liblist,j)
  CALL MAKEDIR(libpath||dirs.tt)
  lf1=libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15))
  flg='W'
  IF EXISTS(libpath||dirs.tt'.txt') THEN
    DO
      ADDRESS COMMAND 'COPY' libpath||dirs.tt'.txt' lf1
      flg='A'
    END
  IF listOPEN(f,lf1,flg)=0 THEN ITERATE j
  IF flg='A' THEN CALL WRITELN(f,'')
  CALL WRITELN(f,'Filename          Bytes File# Library         KeyWords')
  CALL WRITELN(f,LEFT('=',77,'='))
  CALL CLOSE(f)
END
RETURN


updateliblists:
x=FIND(liblist,WORD(alpha.i,4))
IF x=0 THEN RETURN
tt=WORD(liblist,x)
IF tt~=lastlib THEN
  DO
    CALL CLOSE(a)
    lastlib=tt
    x=OPEN(a,libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15)),'A')
    IF x=0 THEN
      DO
        lastlib=0
        RETURN
      END
  END
CALL WRITELN(a,alpha.i)
cnt.tt=cnt.tt+1
RETURN


closeliblists:
CALL CLOSE(a)
DO i=1 TO WORDS(liblist)
  tt=WORD(liblist,i)
  dname='.'STRIP(LEFT(dirs.tt,15))
  SAY ' 'dname
  x=OPEN(f,libpath||dirs.tt'/'dname,'A')
  IF x~=0 THEN
    DO
      CALL WRITELN(f,LEFT('-',77,'-'))
      temp='file'
      IF cnt.tt~=1 THEN temp=temp's'
      temp=cnt.tt temp'.  Last updated' DATE() 'at' TIME('C')
      temp=temp RIGHT(bbsname,76-LENGTH(temp))
      CALL WRITELN(f,temp)
      CALL CLOSE(f)
    END
  CALL MAKEDIR(bbspath'FileNotes/'dirs.tt)
  fnote=bbspath'FileNotes/'dirs.tt'/'dname
  lynes.=''
  lynes.0=5
  x=OPEN(f,fnote,'R')
  IF x=0 THEN CALL SETCLIP('BBS_resave_local',1)
  ELSE
    DO
      DO k=1
        line=READLN(f)
        IF EOF(f) THEN LEAVE k
        lynes.k=line
      END
      CALL CLOSE(f)
      lynes.0=k-1
    END
  finfo=STATEF(libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15)))
  bt=WORD(finfo,2)
  dl=WORD(lynes.2,7)
  IF ~DATATYPE(dl,'W') THEN dl=0
  lynes.1='File: 'LEFT(tt,5)' KeyWords: alphabetical files list CONTENTS'
  lynes.2='Name: 'LEFT(dname,27)' Size:' bt 'bytes  Downloads:' dl
  lynes.3='From: 'LEFT('BBBBS',27)' Date: 'DATE() TIME('C')'  Lib: 'dirs.tt
  lynes.4=LEFT('',74,'=')
  IF lynes.5='' THEN
    lynes.5='Up to the minute alphabetical filelist of the' dirs.tt 'library.'
  IF writeopen(fnote) THEN
    DO
      DO k=1 TO lynes.0
        CALL WRITELN(f,lynes.k)
      END
      CALL CLOSE(f)
      SAY LEFT(' ',LENGTH(dname)+2)'1B'x'Mupdated.'
    END
END
RETURN


viewuser:
SAY 
SAY bak2' 'name' 'def
DO i=1 TO 18
  stuff=data.i
  IF i=13 | i=14 THEN stuff=DATE(,data.i,'S')
  SAY RIGHT(i,2)||pen3 text.i||def':' stuff
END
CALL waiting()
RETURN


edituser:
x=bbsEditUser.rexx(6000 name)
CALL loaddata()
IF x=1 THEN CALL sortconferences()
RETURN


setmsgs:
IF ~DATATYPE(bbsprefs.25,'W') THEN RETURN
data.22=''
data.23=''
SAY
line='Setting message counters to last'
IF bbsprefs.25>1 THEN line=line bbsprefs.25 'messages'
ELSE line=line 'message'
line=line 'in each conference...'
SAY line
DO i=1 TO level
  num=countcheck('LastMessage'i 0)-bbsprefs.25
  IF num<0 | msg.i.0<bbsprefs.25 THEN num=0
  lastread.i=num
  data.22=data.22 num
  data.23=data.23 0
END
SAY 'Setting file counter to last file uploaded...'
lastbrowse=countcheck('LastFile' 0)
newfilesdate=DATE('S') TIME()
RETURN


getnumber:
PARSE ARG tprompt
tnum=getinput(1 0 '  'tprompt' > ')
mask=COMPRESS(XRANGE(),'0123456789')
tnum=COMPRESS(tnum,mask)
IF ~DATATYPE(tnum,'W') THEN tnum=0
tnum=tnum%1
IF tnum>0 & tnum<10 THEN tnum='0'tnum
RETURN tnum


getbirth:
data.12=WORD(data.12,1)'  'WORD(data.12,2)'  Birthday:'
SAY pen3'Birthday Information:'def
month=getnumber('Please enter the MONTH you were born: (1-12)')
day=getnumber('Please enter the DAY   you were born: (1-31)')
year=getnumber('Please enter the YEAR  you were born:       ')
IF year<100 THEN year=year+1900
born=year||month||day
IF born<18750101 | born>(DATE('S')-50000) THEN
  DO
    born=''
    IF getinput(1 1 'Would you rather skip this question? (Ny) > ')~='Y' THEN
      CALL getbirth()
  END
data.12=WORD(data.12,1)'  'WORD(data.12,2)'  'WORD(data.12,3)' 'WORD(born,1)
RETURN


getname:
nonstop=0
CALL showuserlist()
SAY
waitchar='Q'
CALL showtext(bbspath'BBS_TEXT/NEW_USER_NAME' 1)
pline='Your name on'pen3 bbsname def'will be > '
name=getinput(1 0 pline)
name=cleanstring(1':'name)
IF name='' THEN
  DO
    SAY 'No name, no entry.  Bye!'
    SIGNAL DONE
  END
IF EXISTS(bbspath'Users/'name) | FIND(exclusion,name)>0 THEN
  DO
    SAY 'Sorry! That name is taken. Please try again.'
    RETURN 1
  END
IF getinput(1 1 'Your name on'pen3 bbsname def'will be >' name', is that correct? (nY) > ')='N' THEN
  RETURN 1
RETURN 0


/** see if name is in data */

checkUser:
tries=0
IF name='NEW' THEN
  DO
    name=''
    DO WHILE getname()
    END
  END
IF ~EXISTS(bbspath'Users/'name) THEN
  DO
    IF EXISTS(bbspath'BBS_TEXT/NEW') THEN
      DO
        nonstop=0
        CALL showtext(bbspath'BBS_TEXT/NEW' 1)
      END
    SAY
    defile=bbspath'BBS_TEXT/DEF.NEW_USER'
    CALL loadcourtesy()
    wordnum=FIND(courtesy,name)
    IF wordnum>0 THEN
      DO
        SAY name', is on the Courtesy List. You will be granted immediate access.'
        courtesy=STRIP(DELWORD(courtesy,wordnum,1))
        IF writeopen(bbspath'Lists/Courtesy') THEN
          DO
            DO i=1 TO WORDS(courtesy)
              CALL WRITELN(f,WORD(courtesy,i))
            END
            CALL CLOSE(f)
          END
        defile=bbspath'BBS_TEXT/DEF.COURTESY'
      END
    ELSE IF bbsprefs.7=0 THEN SAY name', You have new user access.'
    IF readlines(defile 1) THEN SIGNAL DONE
    data.=''
    data.0=27
    DO i=6 TO 22
      data.i=lynes.i
    END
    data.12=DATE('S')'  'TIME('C')
    data.13=data.12
    lastondate=DATE('I')-1
    lastontime=TIME('C')
    DO i=60 TO 2 BY -1
      SAY RIGHT('- 'i' -',14)
    END
    data.7=getinput(1 0 'What number is now at the top of your screen? > ')
    IF data.7<17 | data.7>75 THEN data.7=20
    SAY 'Please enter the password you would like to use here.'
    data.5=getinput(1 0 'Enter Password: ')
    DO WHILE getinput(1 1 'Your password on' bbsname 'will be :' data.5 ', is that correct? (nY) > ')='N'
      data.5=getinput(1 0 'Enter Password: ')
    END
    IF data.5='' THEN
      DO
        line=name 'refused to enter a password.'
        SIGNAL DONE
      END
    data.1=''
    DO WHILE data.1=''
      data.1=getinput(0 0 '[0mFull (real) Name: ')
      IF data.1='' THEN SAY 'You MUST leave your real name!'
    END
    data.2=getinput(0 0 'Street: ')
    data.3=getinput(0 0 'City, State Zip: ')
    data.4=''
    DO WHILE data.4=''
      data.4=getinput(0 0 'Voice Phone (including areacode): ')
      IF data.4='' THEN
        SAY sysop 'MUST be able to reach you by phone to validate you!'
    END
    CALL getbirth()
    IF bbsprefs.8 THEN
      DO
        newufile=bbspath'Lists/NEW_USERS'
        IF EXISTS(newufile) THEN ok=OPEN(f,newufile,'A')
        ELSE
          DO
            ok=OPEN(f,newufile,'W')
            IF ok~=0 THEN CALL WRITELN(f,'*** New Users ***')
          END
        IF ok~=0 THEN
          DO
            temp=RIGHT(TIME('C'),7) COMPRESS(DATE())
            temp=temp LEFT(name,24)'=' data.1'  'data.4
            CALL WRITELN(f,temp) 
          END
        CALL CLOSE(f)
      END
    data.9=getinput(0 0 'Computer: ')
    data.10=getinput(0 0 'Interests: ')
    test=getinput(1 1 pen3'Do you want other users to see your STREET address? (Ny) > 'def)
    IF test='Y' THEN data.8=data.8 'STREET'
    test=getinput(1 1 pen3'Do you want other users to see your PHONE number? (Ny) > 'def)
    IF test='Y' THEN data.8=data.8 'PHONE'
    IF bbsprefs.7>0 THEN
      DO
        data.20=bbsprefs.7
        CALL do_eleven(60 bbsprefs.16 bbsprefs.16-1)
      END
    SAY
    CALL setdata()
    IF data.20=0 THEN
      SAY 'Thank you, the sysop will give you higher access soon.'
    ELSE CALL setmsgs()
    SAY
    SAY 'Please feel free to leave additional info by using [C]omment.'
    SAY
    CALL saveData(1)
    SAY 'Adding' name 'to the user list...'
    newpassword=data.5
    sortuserflag=1
    temp=countcheck('Users' 0)+1
    CALL countcheck('Users' temp)
    CALL DELETE(bbspath'Lists/USERS')
  END
ELSE
  DO
    IF loaddata()=0 THEN SIGNAL DONE
    city=docity(data.3)
    PARSE VAR data.11 amins . . . ttimes . . . atimes .
    lastondate=DATE('I',WORD(data.13,1),'S')
    lastontime=WORD(data.13,2)
    IF DATE('I')>lastondate | level>=sysoplevel THEN atimes=ttimes
    IF level=99 THEN amins=120
    data.13=DATE('S')'  'TIME()
    CALL do_eleven(amins ttimes atimes-1)
    passprompt='Enter Password: [30m'
    DO tries=1 TO 3
      OPTIONS PROMPT passprompt
      PULL newpassword
      SAY '[0m'
      IF(password=newpassword) THEN LEAVE tries; /* correct password */
      IF tries=3 THEN
        DO
          SAY 
          SAY 'Access terminated.'
          line='*** Bad password ***' newpassword '***'
          SAY line
          SIGNAL OUT2
        END
      SAY lineup'                                 '
      passprompt='Incorrect.  Password: ' /* ask again */
    END
  END
CALL DELAY(14)
SAY 
RETURN


do_eleven:
ARG am tc at .
data.11=am 'minutes per call,' tc 'calls per day,'
data.11=data.11 at 'more calls today'
RETURN


saveData:
ARG messflag .
IF data.5='' THEN RETURN
SAY 'Updating...             'lineup
IF newfilesdate~='' THEN data.16=lastbrowse newfilesdate
ELSE IF lastbrowse>0 THEN
  DO
    IF WORDS(data.16)>1 THEN data.16=DELWORD(data.16,1,1)
    ELSE data.16=DATE('S') TIME()
    data.16=lastbrowse data.16
  END
IF messflag THEN
  DO
    userexclude.=0
    DO si=1 TO WORDS(data.22)
      IF WORD(data.22,si)=-1 THEN userexclude.si=1
    END
    data.22=''
    data.23=''
    DO si=1 TO level
      IF ~DATATYPE(lastread.si,'W') THEN lastread.si=0
      IF userexclude.si THEN data.22=data.22 '-1'
      ELSE data.22=data.22 lastread.si
      IF ~DATATYPE(totwrit.si,'W') THEN totwrit.si=0
      data.23=data.23 totwrit.si
    END
  END
IF writeopen(bbspath'USERS/'name)=0 THEN RETURN
IF data.0<27 THEN data.0=27
DO i=1 TO data.0
  CALL WRITELN(f,data.i)
END
CALL CLOSE(f)
SAY 'User' name 'has been updated.'
RETURN


loaddata:
IF name='' THEN RETURN 0
IF ~readopen(bbspath'USERS/'name) THEN RETURN 0
data.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN BREAK
  data.i=line
END
data.0=i-1
CALL CLOSE(f)
winnings=WORD(data.18,1)
IF ~DATATYPE(winnings,'N') THEN winnings=0

setdata:
IF WORDS(data.16)<3 THEN data.16='0 19900101 00:00:00'
lastbrowse=WORD(data.16,1)
IF ~DATATYPE(lastbrowse,'W') THEN lastbrowse=0
level=data.20
DO i=1 TO level
  lastread.i=WORD(data.22,i)
  IF ~DATATYPE(lastread.i,'W') THEN lastread.i=0
  totwrit.i=WORD(data.23,i)
  IF ~DATATYPE(totwrit.i,'W') THEN totwrit.i=0
END
password=data.5
IF ~DATATYPE(data.7,'W') THEN data.7=20
IF data.7<5 THEN data.7=5
IF FIND(UPPER(data.8),'TERSE')>0 THEN terseflag=1
ELSE terseflag=0
IF FIND(UPPER(data.8),'COLOR')>0 THEN colorflag=1
ELSE colorflag=0
CALL colors(colorflag)
IF FIND(UPPER(data.8),'CLEAR')>0 THEN clr='0C'x
ELSE clr=''
menu='ALL'
IF FIND(UPPER(data.8),'MENUS')>0 THEN
  DO
    menuflag=1
    menu='MAIN'
  END
ELSE IF FIND(UPPER(data.8),'MENU')>0 THEN menuflag=1
ELSE menuflag=0
IF level=0 THEN menu='NEW'
IF DATATYPE(WORD(data.11,3),'W') THEN
  DO
    PARSE VAR data.11 amins . atimes .
    CALL do_eleven(amins bbsprefs.16 atimes)
  END
data.21=UPPER(data.21)
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


switchmenuflag:
IF menuflag=1 THEN
  DO
    menuflag=0
    noff='OFF'
  END
ELSE
  DO
    menuflag=1
    noff='ON'
  END
SAY 'Menus turned' pen3||noff||def'.'
SAY 'To make a permanent change, add or delete MENU(S) from [Y]our userdata item 8.'
RETURN


switchcolors:
IF colorflag=1 THEN
  DO
    colorflag=0
    noff='OFF'
  END
ELSE
  DO
    colorflag=1
    noff='ON'
  END
CALL colors(colorflag)
SAY 'Color turned' pen3||noff||def'.'
SAY 'To make a permanent change, add or delete COLOR from [Y]our userdata item 8.'
RETURN


/* ANSI pen color codes */
colors:
ARG onoff
IF onoff THEN
  DO
    def='[0m';  /* default */
    pen0='[30m';  pen1='[31m';  pen2='[32m';  pen3='[33m'
    pen4='[34m';  pen5='[35m';  pen6='[36m';  pen7='[37m'
    bak0='[40m';  bak1='[41m';  bak2='[42m';  bak3='[43m'
    bak4='[44m';  bak5='[45m';  bak6='[46m';  bak7='[47m'
  END
ELSE
  DO
    pen0=''; pen1=''; pen2=''; pen3=''; pen4=''; pen5=''; pen6=''; pen7=''
    bak0=''; bak1=''; bak2=''; bak3=''; bak4=''; bak5=''; bak6=''; bak7=''
    def='';
  END
RETURN


sortinfofiles:
infolist=SHOWDIR(bbspath'Information')
IF infolist='' THEN
  DO
    SAY 
    SAY pen3'No files are currently in the Information drawer.'def
    SAY 
    RETURN 1
  END
IF ~DATATYPE(sortinfo.0,'W') THEN
  DO
    info.=''
    sortinfo.=''
    info.0=WORDS(infolist)
    DO i=1 TO info.0
      info.i=WORD(infolist,i)
    END
    SAY 'Sorting..'
    IF info.0>0 THEN CALL QSORT(1,info.0,info)
    sortinfo.0=info.0%3
    IF (info.0//3)>0 THEN sortinfo.0=sortinfo.0+1
    DO i=1 TO sortinfo.0
      sortinfo.i=''
      DO j=0 TO 2
        k=i+j*sortinfo.0
        IF k<=info.0 THEN
          DO
            sortinfo.i=sortinfo.i RIGHT(k,3)'.' LEFT(info.k,19)
            infocount=WORD(STATEF(bbspath'Information/'info.k),8)
            sortinfo.i.0=sortinfo.i.0||RIGHT(infocount,5) LEFT(info.k,19)
          END
      END
    END
    SAY lineup'         'lineup
  END
RETURN 0


information:
IF sortinfofiles() THEN RETURN
SAY pen3'These text files are available for reading online...'def
num=1
readcount=-1
DO infoloop=1
  IF num=0 THEN
    DO
      IF readcount~=-1 THEN
        DO
          sortinfo.0=''
          IF sortinfofiles() THEN RETURN
        END
      SAY CENTER('- Number of accesses per file -',75)
    END
  SAY pen3||LEFT('-',75,'-')||def
  DO i=1 TO sortinfo.0
    IF num=0 THEN SAY sortinfo.i.0
    ELSE SAY sortinfo.i
  END
  SAY pen3||LEFT('-',75,'-')||def
  IF num=0 THEN
    DO
      CALL waiting()
      num=1
      ITERATE infoloop
    END
  num=getinput(1 0 pen3'Select Number Of Information File To View. 0=Stats > 'def)
  IF num=0 THEN ITERATE infoloop
  IF ~DATATYPE(num,'W') | num<1 | num>info.0 THEN RETURN
  readcount=STATEF(bbspath'Information/'info.num)
  readbytes=WORD(readcount,2)
  SAY '  'info.num 'is' readbytes 'bytes.'
  SAY 'Loading File...'
  CALL Increment.rexx(bbspath'Information/'info.num)
  CALL readlines(bbspath'Information/'info.num 1)
  CALL cleanline(0)
  SAY '    'lynes.0 'lines.'
  CALL seelines(0)
  IF waitchar~='Q' THEN CALL waiting()
  nonstop=0
END
RETURN


newfiles:
SAY 
test=getinput(1 1 'Show one library only? (Ny) > ')
IF test='Y' THEN
  IF chdir()>0 THEN RETURN
SAY 'Searching for new (un-browsed) files since' DATE(,WORD(data.16,2),'S') 'at' WORD(data.16,3)'...'
lastbrowz=WORD(data.16,1)
lastfile=countcheck('LastFile' 0)

newfiles2:
IF lastbrowz>=lastfile THEN
  DO
    lastbrowz=0
    SAY pen3'No new files. Listing backwards by date from last file uploaded...'def
  END
ELSE newfilesflag=1
j=0
IF test='Y' THEN
  DO
    filecount=WORDS(SHOWDIR(bbspath'FileNotes/'plaindir))-1
    CALL busywait(4 1)
  END
DO ni=lastfile TO lastbrowz+1 BY -1
  IF files.ni~='' THEN
    DO
      IF test='Y' THEN 
        DO
          IF ni>1 THEN CALL busywait(60 ni lastfile-lastbrowz)
          IF j>=filecount THEN LEAVE ni
          IF UPPER(LEFT(WORD(files.ni,1),12))~=UPPER(LEFT(plaindir,12)) THEN
            ITERATE ni
        END
      jj=files.ni.0
      IF WORD(alpha.jj,4)>level | FIND(data.21,UPPER(WORD(files.ni,1)))>0 THEN
        ITERATE ni  /* unauthorized */
      IF test='Y' THEN CALL busywait(4 0)
      j=j+1
      IF j=1 THEN CALL fileheader()
      SAY LEFT(alpha.jj,76)
      IF (j+2)//(linesperpage-1)=0 THEN
        IF waiting2() THEN LEAVE ni
      IF test='Y' THEN CALL busywait(4 1)
    END
END
IF test='Y' THEN CALL busywait(4 0)
IF j//linesperpage~=0 THEN CALL waiting()
IF j=0 & newfilesflag=1 THEN
  DO
    lastbrowz=999999
    newfilesflag=0
    CALL newfiles2()
  END
IF test~='Y' THEN
  DO
    CALL newinfo()
    IF lynes.0>0 THEN CALL waiting()
  END
nonstop=0
RETURN


newinfo:
lynes.=''
lynes.0=0
dm=DATE(,WORD(data.16,2),'S')
PARSE VAR dm da' 'mo' 'yr .
yr=RIGHT(yr,2)
sincedate=da'-'mo'-'yr
startline=1
arg=bbspath'Information'
IF WORD(STATEF(arg),5)>lastondate THEN
  DO
    ADDRESS COMMAND 'C:LIST >'scratch'/locdirlist' arg 'NOHEAD DATES SINCE' sincedate
    IF WORD(STATEF(scratch'/locdirlist'),2)>3 THEN
      DO
        lynes.startline=pen1||bak2' New or Updated Information Files. Enter'def pen3'I'def bak2'from the main menu to read 'def
        CALL readlines(scratch'/locdirlist' startline+1)
      END
  END
arg=bbspath'Profiles'
IF level>0 & WORD(STATEF(arg),5)>lastondate THEN
  DO
    ADDRESS COMMAND 'C:LIST >'scratch'/locdirlist' arg 'NOHEAD DATES SINCE' sincedate
    IF WORD(STATEF(scratch'/locdirlist'),2)>3 THEN
      DO
        startline=lynes.0+2
        lynes.startline=pen1||bak2' New or Updated User Profiles. Enter'def pen3'&'def bak2'from the main menu to read 'def
        CALL readlines(scratch'/locdirlist' startline+1)
      END
  END
arg=bbspath'rexxDoors/Data/Polls'
IF level>0 & WORD(STATEF(arg),5)>lastondate THEN
  DO
    startline=lynes.0+2
    lynes.startline=pen1||bak2' Voting Activity. Enter'def pen3'J'def bak2'from the main menu, then select Polling_Place 'def
    lynes.0=startline
  END
IF logonflag=1 THEN nonstop=1
IF lynes.0>0 THEN CALL seelines(1)
nonstop=0
RETURN


chdir:
string=''
SAY pen3||LEFT('-',75,'-')||def
DO i=1 TO libs.0
  SAY libs.i
END
SAY pen3||LEFT('-',75,'-')||def
dirnum=getinput(1 0 pen3'Select Library Number: 'def)
IF clr~='' THEN SAY clr
IF ~DATATYPE(dirnum,'W') THEN
  DO
    waitchar=dirnum
    RETURN 2
  END

chdir2:
IF dirnum<1 | dirnum>99 THEN
  DO
    waitchar=dirnum
    RETURN 1
  END
IF dirs.dirnum='' THEN
  DO
    SAY pen3'That library number is currently un-assigned.'def
    RETURN 1
  END
IF dirnum>level | FIND(data.21,UPPER(dirs.dirnum))>0 THEN
  DO
    SAY pen3'You do not have authorization for that library!'def
    RETURN 1
  END
td=libpath||dirs.dirnum
CALL MAKEDIR(td)
CALL setdir(td)
IF libtext=0 THEN
  IF EXISTS(td'/.'STRIP(LEFT(dirs.dirnum,15))) THEN RETURN 0
t=libpath||plaindir'.txt'
IF terseflag | ~EXISTS(t) THEN RETURN 0
nonstop=1
SAY
CALL showtext(t 0)
SAY
RETURN 0


since:
dm=DATE(,WORD(data.16,2),'S')
SAY 
SAY 'New files or files moved since' dm
CALL listsince()
CALL showtext(scratch'/locdirlist' 1)
RETURN


listsince:
dm=DATE(,WORD(data.16,2),'S')
PARSE VAR dm da' 'mo' 'yr .
yr=RIGHT(yr,2)
sincedate=da'-'mo'-'yr
ADDRESS COMMAND 'C:list >'scratch'/locdirlist' directory 'DATES SINCE' sincedate
RETURN


list:
onetime=0
IF DATATYPE(arg,'W') THEN onetime=1
ELSE arg=''
DO listloop=1
  IF DATATYPE(arg,'W') THEN
    DO
      dirnum=arg
      arg=''
      IF chdir2()>0 THEN RETURN
      CALL listsimple()
      IF waitchar='Q' | onetime THEN LEAVE listloop
    END
  ELSE IF arg='' THEN
    DO
      libtext=0
      IF chdir()>0 THEN
        DO
          libtext=1
          RETURN
        END
      test='Y'
      CALL showalpha2()
      arg=''
      IF waitchar='Q' THEN waitchar=''
      IF waitchar~='' THEN RETURN
      ITERATE listloop
    END
  ELSE RETURN
END
RETURN


listsimple:
ADDRESS COMMAND 'C:list >'scratch'/locdirlist' directory 'DATES'
IF readlines(scratch'/locdirlist' 1) THEN RETURN
IF lynes.0>3 THEN
  DO
    SAY pen3'Sorting...'def||lineup
    linesave=lynes.1  /* these 4 lines put in to leave dir title at top */
    lynes.1='0'
    IF lynes.0>1 THEN CALL QSORT(1,lynes.0-1,lynes)
    CALL DELAY(14)
    lynes.1=linesave
  END
CALL seelines(1)
nonstop=0
CALL waiting()
RETURN


browse:
curdironly=0
brdir=PRAGMA('D')
brfilenum=1
nonstop=0
IF files.0<1 THEN RETURN
lastfile=countcheck('LastFile' 0)
IF lastfile<1 THEN RETURN
onearg=0
IF arg='' THEN
  DO
    lin='Browsing'
    test=getinput(1 1 'Browse one library only? (Ny) > ')
    IF test='Y' THEN
      DO
        IF chdir()>0 THEN RETURN
        curdironly=1
        lin=lin 'the' pen3||plaindir||def 'library'
        t=libpath||plaindir'.txt'
        IF edinfo(t,plaindir,'File Library') THEN RETURN
      END
    ELSE lin=lin 'all file libraries'
    lin=lin 'backwards from latest file.'
    SAY lin
    SAY
  END
ELSE onearg=1
i=0
IF arg='' | UPPER(arg)='NEW' | UPPER(arg)='ALL' THEN
  DO lastfileloop=1
    IF lastfile<1 THEN RETURN
    arg=WORD(files.lastfile,2)
    brfilenum=lastfile
    IF WORD(files.lastfile,2)~='' THEN LEAVE lastfileloop
    lastfile=lastfile-1
  END
ELSE IF DATATYPE(arg,'W') THEN
  DO
    brfilenum=arg
    arg=WORD(files.arg,2)
    IF arg='' THEN
      DO
        SAY 'File number' brfilenum 'does not exist in the current libraries!'
        RETURN
      END
  END
ELSE
  DO
    IF onearg THEN CALL busywait(4 1)
    DO ni=lastfile TO 1 BY -1
      IF onearg THEN CALL busywait(60 ni lastfile)
      IF UPPER(WORD(files.ni,2))~=UPPER(arg) THEN ITERATE ni
      brfilenum=ni
      CALL busywait(4 0)
      LEAVE ni
    END
    IF ni<1 THEN
      DO
        SAY 'Unable to find a file description for' pen3||arg||def'.'
        RETURN
      END
  END
IF ~curdironly THEN CALL setdir(libpath||WORD(files.brfilenum,1))
savearg=arg
IF brfilenum>lastfile THEN brfilenum=lastfile
newfilesdate=DATE('S') TIME()
DO browseloop=1
  IF curdironly THEN CALL busywait(4 1)
  DO ni=brfilenum TO 0 BY -1
    IF ni=0 THEN LEAVE browseloop
    IF files.ni='' THEN ITERATE ni
    IF onearg THEN
      DO
        CALL busywait(60 ni lastfile)
        IF UPPER(arg)~=UPPER(WORD(files.ni,2)) THEN ITERATE ni
        IF (ni//30)>0 THEN CALL busywait(4 1)
        LEAVE ni
      END
    testdir=UPPER(WORD(files.ni,1))
    IF curdironly & UPPER(plaindir)~=UPPER(testdir) THEN
      DO
        IF ni>lastbrowse THEN lastbrowse=ni
        IF ni>0 THEN CALL busywait(60 ni lastfile)
        ITERATE ni
      END
    IF FIND(data.21,testdir)>0 | finddirnum(testdir)>level THEN
      DO
        IF ni>lastbrowse THEN lastbrowse=ni
        ITERATE ni
      END
    LEAVE ni
  END
  IF curdironly | onearg THEN CALL busywait(4 0)
  onearg=0
  IF ni=0 THEN brfilenum=lastbrowse
  ELSE brfilenum=ni
  argname=WORD(files.brfilenum,2)
  IF argname='' THEN RETURN
  CALL setdir(libpath||WORD(files.brfilenum,1))
  arg=bbspath'FileNotes/'plaindir'/'argname
  CALL readlines(arg 1)
  IF nonstop=1 THEN brostop=1
  ELSE brostop=0
  CALL seelines(1)
  IF brfilenum>lastbrowse THEN lastbrowse=brfilenum
  IF brostop THEN
    DO
      SAY
      nonstop=1
      brfilenum=brfilenum-1
    END
  ELSE
    DO
      line=''
      endtest=UPPER(RIGHT(argname,4))
      IF FIND('.ARC .ARJ .DMS .LZH .LHA .LZX .RUN .ZIP .ZOO',endtest)>0 THEN
        line='['pen3'C'def']ontents ['pen3'D'def']ownload'
      ELSE line='['pen3'D'def']ownload'
      IF level>sysoplevel | name=WORD(lynes.3,2) THEN
        line=line '['pen3'E'def']dit'
      IF level>sysoplevel | name=WORD(lynes.3,2) THEN
        line=line '['pen3'K'def']ill'
      IF level>sysoplevel THEN line=line '['pen3'L'def']ib'
      line=line '['pen3'M'def']ark ['pen3'N'def']on-Stop'
      IF endtest='.TXT' | UPPER(argname)='.'UPPER(STRIP(LEFT(plaindir,15))) THEN
        line=line '['pen3'R'def']ead'
      line=line '['pen3'Q'def']uit ['pen3'?'def'] > '
      brcom=getinput(1 0 line)
      IF DATATYPE(brcom,'W') THEN
        DO
          brfilenum=brcom+1
          IF brfilenum>lastfile THEN brfilenum=lastfile+1
          IF brfilenum<1 THEN brfilenum=1
          SAY
        END
      ELSE brcom=LEFT(brcom,1)
      CALL cleanline(0)
      IF brcom='Q' THEN LEAVE browseloop
      IF brcom='M' THEN
        DO
          wordnum=FIND(data.25,brfilenum)
          IF wordnum=0 THEN
            DO
              data.25=STRIP(data.25 brfilenum)
              SAY lineup||argname 'marked for next download.'
              SAY
            END
          ELSE
            DO
              data.25=STRIP(DELWORD(data.25,wordnum,1))
              SAY argname 'removed from download list.'
            END
        END
      IF brcom='H' | brcom='?' THEN
        DO
          SAY pen3' - HELP with the Browse Files commands -'def
          SAY ' RETURN reads the next file description in line.'
          SAY ' 34 will display the description of file number 34, if it exists.'
          SAY ' C  displays the contents of an archived (arc dms lzh lha zip zoo) file.'
          SAY ' D  displays the download menu.'
          IF level>sysoplevel | name=WORD(lynes.3,2) THEN
            DO
          SAY ' E  puts this file description into the online Editor.'
          SAY ' K  deletes a file you uploaded. you cannot Kill others!'
            END
          IF level>sysoplevel THEN
          SAY ' L  move file and description to new Library and/or rename.'
          SAY ' M  mark/unmark the current file for the next download'
          SAY ' N  displays all descriptions without pausing. CTRL-E to Exit!'
          SAY ' R  displays file as text. - ONLY FILES THAT END IN .TXT -'
          SAY ' Q  returns to the main menu(s). (Quit)'
          SAY
          CALL waiting()
          IF waitchar='Q' THEN LEAVE browseloop
        END
      ELSE IF brcom='L' & level>sysoplevel THEN
        DO
          curdir=PRAGMA('D')
          IF getinput(1 1 'Rename' argname '? (Ny) > ')='Y' THEN
            DO
              newarg=getinput(0 0 'Rename' argname 'to ')
              IF newarg~='' THEN
                DO
                  IF is_here(newarg) THEN ITERATE browseloop
                  IF wi=999999 THEN ITERATE browseloop
                  IF EXISTS(libpath||filedir'/'newarg) THEN
                    DO
                      SAY
                      SAY '***' newarg 'already exists!'
                      SAY
                      ITERATE browseloop
                    END
                  junk=getinput(1 1 'Are you SURE you want to rename' argname 'to' newarg'? (Ny) ')
                  IF junk='Y' THEN
                    DO
                      lynes.2=OVERLAY(newarg,lynes.2,7,25)
                      comment=WORD(STATEF(arg),8)
                      CALL DELETE(arg)
                      arg=bbspath'FileNotes/'plaindir'/'newarg
                      CALL savelines(arg)
                      IF comment='' THEN
                        DO
                          mpath=libpath||plaindir
                          IF RENAME(mpath'/'argname,mpath'/'newarg)=0 THEN
                            SAY 'Rename failed on main file!'
                        END
                      ELSE
                        DO
                          t=LASTPOS('/',comment)
                          IF t=0 THEN t=LASTPOS(':',comment)
                          mpath=LEFT(comment,t-1)
                          IF RENAME(comment,mpath'/'newarg)=1 THEN
                            ADDRESS COMMAND 'C:FileNote' arg mpath'/'newarg
                          ELSE SAY 'Rename failed on external file!'
                        END
                      files.brfilenum=STRIP(WORD(files.brfilenum,1)) newarg
                      anum=files.brfilenum.0
                      alpha.anum=OVERLAY(newarg,alpha.anum,1,WORDINDEX(alpha.anum,2)-2)
                      argname=newarg
                      sortalphaflag=1
                      savefileflag=1
                      CALL DELETE(libpath||plaindir'/.'STRIP(LEFT(plaindir,15)))
                    END
                END
            END
          mvdir=getinput(0 0 'Move' argname 'to Library (name|number) ')
          IF mvdir~='' THEN
            DO
              IF DATATYPE(mvdir,'W') THEN
                DO
                  dirnum=mvdir
                  IF UPPER(dirs.dirnum)~=UPPER(WORD(files.brfilenum,1)) THEN
                    DO
                      IF chdir2()=0 THEN
                        DO
                          CALL readlines(arg 1)
                          CALL movefile(brfilenum dirs.dirnum)
                        END
                    END
                END
              ELSE
                DO
                  mvdir=STRIP(mvdir)
                  IF UPPER(mvdir)~=UPPER(WORD(files.brfilenum,1)) THEN
                    DO
                      DO mj=1 TO level+1
                        IF UPPER(mvdir)=UPPER(dirs.mj) THEN LEAVE mj
                      END
                      IF mj<=level THEN CALL movefile(brfilenum mvdir)
                    END
                END
            END
          IF savefileflag>0 THEN CALL savefilelist()
          CALL setdir(curdir)
        END
      ELSE IF brcom='N' THEN
        DO
          brfilenum=brfilenum-1
          nonstop=1
          SAY pen3'To EXIT non-stop scrolling of text, press CTRL-E'def
          SAY
          CALL DELAY(99)
          brcom=''
        END
      ELSE IF brcom='C' THEN
        DO
          temp=STRIP(WORD(STATEF(arg),8))
          IF temp='' THEN temp=libpath||plaindir'/'argname
          CALL Contents.rexx(temp)
          IF EXISTS('RAM:CONTENTS') THEN
            DO
              CALL showtext('RAM:CONTENTS' 0)
              IF waitchar~='Q' THEN CALL waiting()
              nonstop=0
            END
          ELSE SAY pen3'Not an archived file.'def
        END
      ELSE IF brcom='D' THEN
        DO
          arg2=arg
          arg=brfilenum
          CALL dload()
          arg=arg2
        END
      ELSE IF brcom='E' THEN
        DO
          IF level>sysoplevel | name=WORD(lynes.3,2) THEN
            DO
              firstedit=5
              IF level>sysoplevel THEN firstedit=1
              CALL bbsEd.rexx(firstedit arg name)
              CALL checkfilechanges()
            END
        END
      ELSE IF brcom='K' THEN
        DO
          IF level>sysoplevel | name=WORD(lynes.3,2) THEN
            DO
              IF getinput(1 1 pen3'Do you really want to kill this file? (nY) >'def)~='N' THEN
                DO
                  tempnum=WORD(lynes.1,2)
                  IF tempnum=lastfile THEN
                    DO
                      CALL DELETE(bbspath'Numbers/LastFile')
                      CALL DELAY(28)
                      lastfile=lastfile-1
                      CALL countcheck('LastFile' lastfile)
                    END
                  files.tempnum=''
                  tempnum2=files.tempnum.0
                  alpha.tempnum2='0 0' tempnum '100'
                  savefileflag=1
                  CALL savefilelist()
                  finfo=STATEF(arg)
                  IF WORDS(finfo)>7 THEN argname=WORD(finfo,8)
                  CALL DELETE(argname)
                  CALL DELETE(arg)
                  SAY argname pen3'has been deleted.'def
                  CALL DELETE(libpath||plaindir'/.'STRIP(LEFT(plaindir,15)))
                END
            END
        END
      ELSE IF brcom='R' & (endtest='.TXT' | UPPER(argname)='.'UPPER(STRIP(LEFT(plaindir,15)))) THEN
        DO
          vcount=WORD(lynes.2,7)+1
          lynes.2=STRIP(DELWORD(lynes.2,7,1)) vcount
          edtype=''
          CALL savelines(arg)
          CALL showtext(argname 1)
        END
      ELSE brfilenum=brfilenum-1
    END
END
CALL setdir(brdir)
waitchar=''
IF nonstop THEN CALL waiting()
nonstop=0
CALL savedata(0)
RETURN


movefile:
PARSE ARG fnum movdir .
fromdir=STRIP(WORD(files.fnum,1))
farg=STRIP(WORD(files.fnum,2))
md=libpath||movdir
mf=md'/'farg
fd=libpath||fromdir
ff=fd'/'farg
CALL DELETE(md'/.'STRIP(LEFT(movdir,15)))
CALL DELETE(fd'/.'STRIP(LEFT(fromdir,15)))
fn=bbspath'FileNotes/'fromdir'/'farg
commen=WORD(STATEF(fn),8)
IF commen~='' THEN
  DO
    ff=commen
    n=LASTPOS('/',ff)
    IF n>1 THEN
      DO
        xf=SUBSTR(ff,n+1)
        tfd=LEFT(ff,n-1)
        n=LASTPOS('/',tfd)
        IF n=0 THEN n=LASTPOS(':',tfd)
        IF n>0 THEN
          DO
            tmd=LEFT(tfd,n)||movdir
            SAY 'Rename external file'pen3 ff||def
            IF getinput(1 1 '                  to'pen3 tmd'/'farg||def'? (Ny) > ')='Y' THEN
              DO
                fd=tfd
                md=tmd
                mf=md'/'farg
                commen=md'/'xf
              END
            ELSE IF getinput(1 1 '          or move to'pen3 mf||def'? (Ny) > ')='Y' THEN
              DO
                fd=tfd
                commen=''
              END
          END
      END
  END
CALL MAKEDIR(md)
IF RENAME(ff,mf)=0 THEN
  DO
    ADDRESS COMMAND 'C:COPY' ff mf
    IF EXISTS(mf) THEN
      IF DELETE(ff)~=1 THEN SAY pen3'Unable to delete'def ff||pen3'.'def
  END
files.fnum=movdir farg
lynes.3=DELWORD(lynes.3,WORDS(lynes.3),1)
lynes.3=STRIP(lynes.3) movdir
CALL MAKEDIR(bbspath'FileNotes/'movdir)
mn=bbspath'FileNotes/'movdir'/'farg
CALL savelines(mn)
ndx=files.fnum.0
dnum=finddirnum(movdir)
alpha.ndx=OVERLAY(RIGHT(dnum,2) movdir,alpha.ndx,31,15)
IF EXISTS(mn) THEN
  DO
    CALL DELETE(fn)
    IF commen~='' THEN ADDRESS COMMAND 'C:FileNote' mn commen
  END
savefileflag=1
line='Moved:' fromdir'/'farg 'to' movdir
SAY line
RETURN


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


bbsSEARCH:
smenu=menu
test=UPPER(LEFT(arg,1))
IF test='F' THEN smenu='FILE'
IF test='M' THEN smenu='MSG'
IF test='U' THEN smenu='MAIN'
IF smenu='ALL' THEN
  DO
    junk=getinput(1 1 'Search ['pen3'F'def']iles ['pen3'M'def']essages or ['pen3'U'def']sers (fmu) > ')
    IF junk='F' THEN smenu='FILE'
    ELSE IF junk='M' THEN smenu='MSG'
    ELSE IF junk='U' THEN smenu='MAIN'
    ELSE RETURN
  END
IF WORDS(arg)>1 THEN searcharg=UPPER(SUBSTR(arg,WORDINDEX(arg,2)))
ELSE searcharg=getinput(0 0 pen3'Search Phrase: 'def)
IF LENGTH(STRIP(searcharg))=0 THEN RETURN
searcharg=COMPRESS(searcharg,'*')
IF smenu='NEW' | smenu='MAIN' THEN
  DO
    SAY 'Searching Userlist...'
    CALL FileList(bbspath'Users/*'searcharg'*',sl)
    SAY 'Found' sl.0 'matches                    '
    DO i=1 TO sl.0
      SAY sl.i
      IF ~nonstop THEN
        IF i//linesperpage=0 THEN
          IF waiting2() THEN LEAVE i
    END
    DROP sl.
  END
IF smenu='MSG' THEN
  DO
    CALL SETCLIP('BBSMSG_SEARCH',searcharg)
    SAY lm
    CALL bbsMsg.rexx(3000 name password) 
  END
IF smenu='FILE' THEN
  DO
    lne=pen3'Searching'
    curdironly=0
    IF getinput(1 1 'Search one library only? (Ny) > ')='Y' THEN
      DO
        IF chdir()>0 THEN RETURN
        curdironly=1
        lne=lne 'the'def plaindir pen3'library'
        SAY
      END
    ELSE
      DO
        lne=lne 'all file libraries'
        SAY
        SAY pen3'WARNING!'def 'Searching' RIGHT(files.0,5) '['pen3'F'def']ull descriptions may take'pen3 TRUNC(files.0/(114*cpu)+.05,1) def'minutes!'
      END
    test=getinput(1 1 '   ['pen3'A'def']lphaList search or ['pen3'F'def']ull descriptions? (Afq) > ')
    IF test='Q' THEN RETURN
    SAY
    SAY lne 'for'def UPPER(searcharg)
    SAY pen3' - To ABORT, press CTRL-E -'def
    SAY
    IF test~='F' THEN
      DO
        CALL fileheader()
        IF curdironly=1 THEN
          DO
            af=libpath||dirs.dirnum'/.'STRIP(LEFT(dirs.dirnum,15))
            IF EXISTS(af) THEN
              DO
                CALL readlines(af 1)
                DO i=1 TO lynes.0
                  CALL busywait(8 i lynes.0)
                  tempnum=POS(UPPER(searcharg),UPPER(lynes.i))
                  IF tempnum>0 THEN
                    DO
                      CALL busywait(4 0)
                      SAY lynes.i
                      SAY pen3||LEFT(' ',tempnum-1)||lineup||UPPER(searcharg)||def
                      CALL busywait(4 1)
                    END
                END
              END
          END
        IF curdironly=0 | ~EXISTS(af) THEN
          DO i=1 TO alpha.0
            CALL busywait(60 i alpha.0)
            ii=WORD(alpha.i,4)
            IF ii>level THEN ITERATE i
            IF curdironly=1 & ii~=dirnum THEN ITERATE i
            ii=WORD(alpha.i,3)
            IF POS(UPPER(WORD(files.ii,1)),data.21)>0 THEN ITERATE i
            tempnum=POS(UPPER(searcharg),UPPER(alpha.i))
            IF tempnum>0 THEN
              DO
                CALL busywait(4 0)
                SAY alpha.i
                SAY pen3||LEFT(' ',tempnum-1)||lineup||UPPER(searcharg)||def
                CALL busywait(4 1)
              END
          END
      END
    ELSE
      DO
        cck=countcheck('LastFile' 0)
        nonstop=1
        DO i=1 TO cck
          iii=cck+1-i
          IF files.iii='' THEN ITERATE i
          ii=files.iii.0
          ii=WORD(alpha.ii,4)
          IF ii>level THEN ITERATE i
          IF curdironly=1 & ii~=dirnum THEN ITERATE i
          IF POS(UPPER(WORD(files.iii,1)),data.21)>0 THEN ITERATE i
          farg=WORD(files.iii,1)'/'WORD(files.iii,2)
          SAY '1B'x'M' RIGHT(farg,40) LEFT(iii,7)
          IF textsearch(bbspath'FileNotes/'farg searcharg) THEN
            DO
              savei=i
              CALL readlines(bbspath'FileNotes/'farg 1)
              nonstop=1
              CALL seelines(2)
              i=savei
              SAY
              SAY
            END
        END
      END
    CALL busywait(4 0)
  END
searcharg=''
nonstop=0
SAY
IF i<999999 THEN SAY 'All available items have been searched.'
SAY
CALL waiting()
RETURN


finddirnum:
ARG fdirname .
DO fdir=1 TO 99
  IF UPPER(dirs.fdir)=UPPER(fdirname) THEN RETURN fdir
END
RETURN 100


writebuffer:
PARSE ARG bufname .
CALL DELETE(bufname)
startnum=lynes.0+1
OPTIONS PROMPT ''
SAY pen3'LOCAL logon! Input cannot exceed 250 characters per line!'def
SAY 'Type 'pen3'/E'def 'or' pen3'/S'def' on a new line to exit and' pen3'DO YOUR OWN WORDWRAP!'def
DO bufloop=startnum
  PARSE PULL line
  IF LEFT(UPPER(STRIP(line)),2)='/E' | LEFT(UPPER(STRIP(line)),2)='/S' THEN
    LEAVE bufloop
  lynes.bufloop=line
END
lynes.0=bufloop-1
CALL wrapbuf(startnum)
CALL DELETE(bufname)     /* these 4 lines make wordwrap more consistent */
CALL savelines(bufname)
CALL readlines(bufname 1)
CALL wrapbuf(startnum)
RETURN


wrapbuf:
ARG startnum .
CALL cleanline(1)
IF startnum>=lynes.0 & LENGTH(lynes.startnum)<80 THEN RETURN
SAY pen3'Wordwrapping...'def
lynes.startnum=TRANSLATE(lynes.startnum,' ','09'x)
lynes.startnum=COMPRESS(lynes.startnum,'0C'x)  /* no FF */
DO wi=startnum WHILE wi<=lynes.0
  wj=wi+1
  lynes.wj=COMPRESS(lynes.wj,'08'x||'0C'x||'7F'x)
  tabpos=POS('09'x,lynes.wi)
  DO WHILE tabpos>0
    lynes.wi=DELSTR(lynes.wi,tabpos,1)
    lynes.wi=INSERT('  ',lynes.wi,tabpos-1)
    tabpos=POS('09'x,lynes.wi)
  END
  IF LENGTH(lynes.wi)>75 THEN
    DO
      testchar=''
      IF lynes.wj~='' THEN testchar=LEFT(lynes.wj,1)
      IF testchar=' ' | testchar='.' | testchar=':' THEN
        DO
          DO wjj=lynes.0 TO wi+1 BY -1
            wk=wjj+1
            lynes.wk=lynes.wjj
          END
          lynes.wj=''
          lynes.0=lynes.0+1
        END
      DO wl=WORDS(lynes.wi) TO 1 BY -1 WHILE LENGTH(lynes.wi)>74
        IF WORDS(lynes.wi)=1 THEN
          lynes.wi=LEFT(lynes.wi,74) SUBSTR(lynes.wi,75)
        lynes.wj=WORD(lynes.wi,wl) lynes.wj
        lynes.wi=STRIP(DELWORD(lynes.wi,wl,1))
      END
    END
END
RETURN


seelines:
ARG fancy .
DO i=1 TO lynes.0
  IF fancy=0 THEN SAY lynes.i||def
  ELSE
    DO
      IF LEFT(lynes.i,2)=': ' & WORDS(lynes.i)=2 THEN ITERATE i
      ELSE IF LEFT(lynes.i,10)='Directory ' | LEFT(lynes.i,5)='=====' THEN
        SAY pen3||lynes.i||def
      ELSE SAY lynes.i
      IF fancy=2 & colorflag=1 THEN
        DO
          IF searcharg~='' THEN
            DO
              testpos=POS(UPPER(searcharg),UPPER(lynes.i))
              IF testpos>0 THEN
                SAY LEFT(' ',testpos-1)||pen3||lineup||UPPER(searcharg)||def
            END
          IF i=1 THEN
            IF WORD(lynes.1,3)='Reply' THEN
              DO
                testpos=WORDINDEX(lynes.1,3)
                SAY LEFT(' ',testpos-1)||pen3||lineup||SUBSTR(lynes.1,testpos)||def
              END
        END
    END
  IF i//linesperpage=0 & i<lynes.0 THEN
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
DO ri=lynes.0 TO 0 BY -1 WHILE LENGTH(lynes.ri)=0 | LEFT(UPPER(lynes.ri),2)='/E' | LEFT(UPPER(lynes.ri),2)='/S'
END
lynes.0=ri
RETURN 0


savelines:
PARSE ARG tempname .
IF EXISTS(tempname) & edtype='MAIL' THEN
  DO
    ok=OPEN(f,tempname,'A')
    IF ok~=0 THEN CALL WRITELN(f,LEFT('',74,'^'))
  END
ELSE ok=OPEN(f,tempname,'W')
IF ok=0 THEN
  DO
    line='***' tempname 'failed to open for saving!'
    SAY line
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


sortuserlist:
uf=bbspath'Lists/USERS'
IF sortuserflag THEN CALL DELETE(uf)
sortuserflag=0
IF ~EXISTS(uf) THEN
  DO
    users=bbsSortUsers.rexx(bbspath bbsname)
    RETURN
  END
ELSE
  DO
    IF OPEN(f,uf,'R')=0 THEN RETURN
    users=0
    DO i=1
      dat=READCH(f,65000)
      IF EOF(f) THEN LEAVE i
      users=users+WORDS(dat)
    END
    CALL CLOSE(f)
  END
SAY CENTER(RIGHT(users,8) 'Users on'pen3 bbsname,74)||def
RETURN


showuserlist:
IF data.5='' THEN line='Here are the EMail names of your fellow users.'
ELSE line='   'users 'users. Use these names to address messages.'
SAY pen3||line||def
CALL showtext(bbspath'Lists/USERS' 1)
IF data.5~='' THEN CALL waiting()
RETURN


msgcount:
ARG countdir .
lastmess=0
totmsgs=0
unred=0
IF ~EXISTS(msgpath||countdir) THEN RETURN
IF STATEF(msgpath||countdir)=msg.countdir.1 THEN totmsgs=msg.countdir.0
ELSE
  DO
    totmsgs=WORDS(SHOWDIR(msgpath||countdir))
    msg.countdir.0=totmsgs
    msg.countdir.1=STATEF(msgpath||countdir)
  END
IF countdir>level | FIND(data.21,i)>0 THEN RETURN
lastread.countdir=WORD(data.22,countdir)
IF ~DATATYPE(lastread.countdir,'W') THEN lastread.countdir=0
lastmess=countcheck('LastMessage'countdir 0)
IF lastread.countdir<0 THEN RETURN
firstmess=countcheck('FirstMessage'countdir 0)
IF lastread.countdir<firstmess THEN lastread.countdir=firstmess-1
IF lastmess>0 THEN
  IF lastread.countdir>=0 THEN
    DO
      IF lastread.countdir<(firstmess-1) THEN lastread.countdir=firstmess-1
      unred=lastmess-lastread.countdir
      IF unred>totmsgs THEN unred=totmsgs
      IF unred>0 | ~logonflag THEN
        DO
          cline=RIGHT(unred,5) 'new of' RIGHT(lastmess,5) 'messages,'
          cline=cline RIGHT(totmsgs,5) 'still online in' 
          cline=cline RIGHT(countdir,2)',' msg.countdir
          SAY pen6||cline||def
        END
    END
RETURN


counts:
SAY
SAY 'Working...'
SAY
temp=''
DO i=1 TO 4
  temp=temp||CENTER(copyright.i,75)||'0A'x
END
CALL SETCLIP('BBS_copyright',temp)
IF emailonline<0 THEN CALL countmail()
CALL bbsSTATS.rexx(name colorflag 0 emailonline grand grand2 files.0 users)
SAY
CALL waiting2()
IF waitchar='Q' THEN RETURN
CALL showmarked(1)
CALL logonstats()
nonstop=0
CALL waiting()
RETURN


countmail:
SAY '   Counting online email...'lineup
emailonline=0
t=SHOWDIR(bbspath'Users')
DO ti=1 TO WORDS(t)
  emailonline=emailonline+WORDS(SHOWDIR(bbspath'Email/'WORD(t,ti)))
END
SAY lineup'       'emailonline' letters online.'
RETURN


hourly:
IF level=99 & nonstop~=1 THEN
  DO
    IF getinput(1 1 'Zero The Hourly Averages? (Ny) > ')='Y' THEN
      ADDRESS COMMAND 'C:Delete >*' bbspath'Numbers/Hourly/#?'
    CALL cleanline(1)
  END
SAY lm
CALL ShowHourly.rexx(name linesperpage colorflag nonstop)
RETURN


logonstats:
IF level=0 THEN RETURN
SAY bak2||name||def 'Last on' DATE('W',lastondate,'I') DATE(,lastondate,'I') lastontime
tempnum=countcheck('LastFile' 0)-lastbrowse
IF tempnum>files.0 THEN tempnum=files.0
line=RIGHT(countcheck('LastFile' 0),5) 'uploaded,'
line=line RIGHT(files.0,5) 'files online.'
IF tempnum>0 THEN SAY RIGHT(tempnum,5) 'new of' line
ELSE SAY '   No new of' line
totmsg=0
grand=0
grand2=0
DO i=1 TO 99
  IF msg.i='' THEN ITERATE i
  CALL msgcount(i)
  totmsg=totmsg+unred
  grand=grand+totmsgs
  grand2=grand2+lastmess
END
line=RIGHT(grand2,5) 'messages,' RIGHT(grand,5) 'still online.' 
IF totmsg>0 THEN SAY RIGHT(totmsg,5) 'new of' line
ELSE SAY '   No new of' line
RETURN


readopen:
PARSE ARG fname
ok=OPEN(f,fname,'R')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for reading!'
SAY line
RETURN 0


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok~=0 THEN RETURN 1
line=fname 'failed to open for writing!'
SAY line
RETURN 0


set_grand:
SAY 'Setting up public message conferences...'
grand=0
DO i=1 TO 99
  IF msg.i='' THEN ITERATE i
  msg.i.0=WORDS(SHOWDIR(msgpath||i,'F'))
  msg.i.1=STATEF(msgpath||i)
  grand=grand+msg.i.0
END
RETURN


SYNTAX:
FAILURE:
lin.1='[36m'ERRORTEXT(RC)'[0m'
lin.2=SIGL-1     SOURCELINE(SIGL-1)
lin.3=SIGL '[36m'SOURCELINE(SIGL)'[0m'
lin.4=SIGL+1     SOURCELINE(SIGL+1)
DO er=1 TO 4
  SAY lin.er
END
IF newpassword='' THEN SIGNAL DONE2  /* no user logged on, quit quietly */
CALL CLOSE(f)
IF level>sysoplevel THEN
  DO
    junk=getinput(1 1 'ReStart: (Ny) > ')
    IF junk~='Y' THEN SIGNAL LOGOUT
  END
string=''
waitchar=''
IF data.1~='' & data.5~='' & data.20~='' THEN CALL savedata(0)
SIGNAL RESTART


BREAK_E:
CALL CLOSE(f)
SAY pen3'*** CTRL-E BREAK ***'def
waitchar=''
string=''
nonstop=0
rnonstop=0
brostop=0
i=999999
wi=999999
ni=-1
SAY
RETURN 0


BREAK_C:
CALL CLOSE(f)

LOGOUT:
LOGOUT2:
secs=TIME('E')
mins=secs%60
secs=TRUNC(secs//60)
IF secs<10 THEN secs='0'secs
SAY
SAY 'Public messages now online: 'RIGHT(comma(grand),9)
SAY 'Public    files now online: 'RIGHT(comma(files.0),9)
SAY
SAY 'Time used this call:' mins':'secs
SAY
arg=bbspath'BBS_TEXT/GOODBYE'
IF EXISTS(arg) THEN
  DO
    CALL DELAY(14)
    nonstop=1
    CALL showtext(arg 0)
    nonstop=0
  END
SAY
IF bbsprefs.2 THEN CALL doGrin()
SAY 
IF EXISTS('rexx:bbsLOGOFF.rexx') THEN CALL bbsLOGOFF.rexx(name level 0)

OUT:
data.18=winnings

OUT2:

DONE:

DONE2:
CALL SETCLIP('BBS_LOCAL')
CALL SETCLIP('BBS_LOCAL_MSG')
IF newfilesflag=1 THEN
  DO
    newfilesdate=DATE('S') TIME()
    lastbrowse=countcheck('LastFile' 0)
  END
IF clear_marked=1 THEN data.24=''
CALL saveData(0)
IF EXISTS(bbspath'EmailFiles/'name'/QUICKIN.lha') THEN
  DO
    IF sortalphaflag>0 | savefileflag>0 THEN
      CALL SETCLIP('BBS_QUICK_WAIT',1)
    ADDRESS AREXX bbsQUICKIN.rexx name level sysoplevel bbsprefs.6
  END
IF sortuserflag=1 THEN
  DO
    CALL sortuserlist()
    IF SHOW('P','BBBBS') THEN
      DO
        CALL SETCLIP('BBS_mainusers')
        CALL SETCLIP('BBS_localusers',1)
      END
    sortuserflag=0
  END
IF sortalphaflag>0 | savefileflag>0 | GETCLIP('BBS_resave_local')~='' THEN
  DO
    loadalphaflag=0
    x=GETCLIP('BBS_resave_local')
    IF savefileflag>0 THEN CALL savefilelist2()
    ELSE IF x='' THEN CALL savealphalist()
    x=GETCLIP('BBS_resave_local')
    CALL SETCLIP('BBS_resave_local')
    IF x=1 THEN
      DO
        sortalphaflag=1
        savefileflag=1
        SIGNAL DONE2
      END
    IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localfiles',2)
    CALL SETCLIP('BBS_QUICK_WAIT')
  END
IF getinput(1 1 'Reset for next local user? (nY) > ')='N' THEN EXIT
clear_marked=0
data.=''
IF loadalphaflag=1 THEN CALL loadalpha(1)
SIGNAL BIG_LOOP


checkclips:
IF GETCLIP('BBS_mainusers')~='' THEN
  DO
    CALL sortuserlist()
    CALL SETCLIP('BBS_mainusers')
  END
IF GETCLIP('BBS_mainfiles')~='' THEN
  DO
    CALL SETCLIP('BBS_mainfiles')
    CALL loadfiles()
    CALL loadalpha(1)
  END
RETURN


checkalias:
addressee=''
IF alias.0=0 THEN RETURN 0
DO i=1 TO alias.0
 IF UPPER(alias.i)=UPPER(string) THEN
  DO
   addressee=realname.i
   LEAVE i
  END
END
IF addressee='' THEN RETURN 0
string=''
SAY pen3'Email to 'def||addressee
CALL editor('MAIL' addressee)
RETURN 0

/* bbsLOCAL.rexx */
