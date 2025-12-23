/* $VER: bbsFile.rexx 8.5 (3.6.95)
Copyright © 1994-95 Richard Lee Stockton
BBBBS local file manager
FREELY DISTRIBUTABLE
*/

SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

IF GETCLIP('BBS_UPLOAD')~='' THEN
  DO
    SAY 'Uploading is temporarily suspended while the filelists are rebuilding.'
    EXIT
  END

ARG name pw

CALL OPENPORT('BBSFILE')
CALL PRAGMA('P',-1)
title.=''
title.1='BBBBS File Manager'
title.2='Version 8.5'
title.3='3-Jun-95'
def='[0m'
pen3='[33m'
lineup='1B'x'M'
files.=''
files.0=0
dlist=''
numlist=''
newlist.=''
newlist.0=0
savefileflag=0
linesperpage=20
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')

topath='RAM:'
figarg='s:CONFIG.BBS'
IF ~EXISTS(figarg) THEN figarg='BBS:BBS_TEXT/CONFIG.BBS'
x=OPEN(f,figarg,'R')
IF x=0 THEN
  DO
    SAY 's:CONFIG.BBS and BBS:BBS/CONFIG.BBS are both missing!'
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
bbsdevice=word(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'
    EXIT
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
libpath=WORD(lynes.8,1)
IF ~EXISTS(libpath) THEN
  DO
    SAY libpath 'does not exist!'
    EXIT
  END
testchar=RIGHT(libpath,1)
IF testchar~='/' & testchar~=':' THEN libpath=libpath'/'
IF WORD(lynes.25,1)=1 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'
CALL MAKEDIR(scratch)
extension=WORD(lynes.32,1)
compos=POS('/*',lynes.33)
IF compos>0 THEN lynes.33=LEFT(lynes.33,compos-1)
arccom=STRIP(lynes.33)
IF LEFT(extension,1)~='.' THEN
  DO
    extension='.lzh'
    arccom='lharc -m m'
  END

IF name='' THEN
  DO
    OPTIONS PROMPT ' Are you 'sysop'? (Yn) > '
    PULL answer
    IF answer='N' THEN
      DO
        SAY
        OPTIONS PROMPT ' Please enter your name > '
        PULL name
        name=cleanstring('1:'name)
        IF name='' THEN EXIT 
      END
    ELSE name=sysop
  END
IF ~EXISTS(bbspath'Users/'name) THEN
  DO
    SAY name 'does not exist!'
    EXIT
  END

CALL loaddata()
IF pw~=password THEN
  DO
    passprompt=pen3' Please Enter Password: [30m'
    DO tries=1 TO 3
      OPTIONS PROMPT passprompt
      PULL newpassword
      SAY def
      IF(password=newpassword) THEN LEAVE tries; /* correct password */
      IF tries=3 THEN
        DO
          SAY 
          SAY 'Access terminated.'
          SAY '*** Bad password ***' newpassword '***'
          EXIT
        END
      passprompt='Incorrect.  Password: ' /* ask again */
    END
    SAY
    SAY' OK, 'name' here we go....'
    SAY
  END

dirs.=''
IF readopen(bbspath'Lists/Libraries') THEN
  DO
    SAY 'Loading library list...'
    DO i=1
      line=READLN(f)
      IF line='END' | EOF(f) THEN LEAVE i
      num=WORD(line,1)
      IF DATATYPE(num,'W') THEN
        DO
          IF num>level THEN ITERATE i
          dirs.num=STRIP(WORD(line,2))
        END
    END
    CALL CLOSE(f)
    CALL sortlibraries()
  END
SAY

uloadflag=0
DO loop=1
  SAY
  DO i=1 TO 3
    SAY CENTER(title.i,34)
  END
  SAY
  SAY '        1. Add Files'
  SAY '        2. Add FileNote (file already exists)'
  SAY '        3. Browse'
  t='        Q. Quit'
  IF savefileflag | newlist.0>0 THEN t=t 'and Save Changes.'
  SAY t
  SAY
  OPTIONS PROMPT 'Select: '
  PULL temp
  IF temp='Q' THEN LEAVE loop
  ELSE IF temp=1 THEN
    DO subloop=1
      IF uload()>1 THEN LEAVE subloop
    END
  ELSE IF temp=2 THEN
    DO
      IF chdir()=0 THEN
        DO
          rfiles=SHOWDIR(libpath||dirs.dirnum)
          rnotes=SHOWDIR(bbspath'FileNotes/'dirs.dirnum)
          IF WORDS(rfiles)~=WORDS(rnotes) THEN
            DO
              DO j=1 TO WORDS(rfiles)
                IF FIND(UPPER(rnotes),UPPER(WORD(rfiles,j)))=0 THEN
                  DO
                    arg=WORD(rfiles,j)
                    CALL DELETE(libpath||dirs.dirnum'/.'STRIP(LEFT(dirs.dirnum,15)))
                    IF testfile()=0 THEN CALL editnote()
                  END
              END
            END
        END
    END
  ELSE IF temp=3 THEN CALL browse()
  IF dlist~='' THEN
    DO
      ab=0
      cpy=1
      SAY
      IF WORD(STATEF(todir),1)='DIR' THEN
        IF getinput(1 1 'Copy filelist to' todir '? (nY) > ')='N' THEN ab=1
      ELSE ab=1
      IF ab THEN
        IF ~pickdir() THEN cpy=0
      IF cpy THEN
        DO i=1 TO WORDS(dlist) WHILE cpy
          CALL copyfile(WORD(dlist,i))
        END
      ELSE IF getinput(1 1 'Clear copylist? (nY) > ')~='N' THEN dlist=''
    END
END

IF savefileflag | newlist.0>0 THEN
  DO
    CALL fileupdate()
    IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localfiles',2)
    ELSE CALL SETCLIP('BBS_localfiles')
    IF SHOW('P','BBBBS_LOCAL') THEN CALL SETCLIP('BBS_mainfiles',2)
    ELSE CALL SETCLIP('BBS_mainfiles')
  END

EXIT


/* SUBROUTINES */

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
DROP sdirs.
RETURN


bbsspace:
ARG tabspace .
ADDRESS COMMAND 'C:info >'scratch'/filinfout' bbsdevice
ok=OPEN(f,scratch'/filinfout','R')
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
bbsk=bbsk*512
IF bbsk<1 THEN bbsk=0
SAY RIGHT(comma(bbsk),tabspace) 'bytes available for uploads.'
RETURN


comma: PROCEDURE
ARG num .
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
RETURN numtext


is_here:
ARG newname 
SAY 'Checking filelist...'
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
RETURN 0


uload:
CALL bbsspace(12)
SAY
IF bbsk<1 THEN
  DO
    SAY pen3'Upload area is full!'def
    RETURN 2
  END
frompath=GETCLIP('BBS_frompath')
IF frompath='' THEN frompath='RAM:'
fdir=''
DO loop=1
  fromfile=GetFile(200,,frompath,'',' Select File to Upload ')
  IF fromfile='' THEN RETURN 3
  finfo=STATEF(fromfile)
  IF WORD(finfo,1)='DIR' THEN RETURN 3
  IF WORD(finfo,1)='FILE' THEN LEAVE loop
  SAY
  SAY fromfile 'does not exist!'
  CALL DELAY(100)
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
arg=COMPRESS(arg,' :/,;|#?*()+[]"{}')  /* be sure no illegals here */
x=LASTPOS('/',fromfile)
IF x=0 THEN x=LASTPOS(':',fromfile)
IF x>0 THEN
  DO
    IF DATATYPE(SUBSTR(fromfile,x+1),'W') THEN
      DO
        SAY 'Numbers only are not allowed as filenames!'
        CALL waiting()
        RETURN 1
      END
  END
size=WORD(STATEF(fromfile),2)
IF ~DATATYPE(size,'W') THEN size='654321'
tempnum=LENGTH(arg)+LENGTH(size)-22
DO WHILE tempnum>0
  temp='          'pen3||arg def'is'pen3 tempnum||def
  IF tempnum=1 THEN temp=temp 'character'
  ELSE temp=temp 'characters'
  temp=temp 'too long for a filename.'
  SAY temp  
  arg=getinput(0 0 'Filename: ')
  arg=cleanstring('0:'arg)
  arg=COMPRESS(arg,' :/,;|#?*')
  tempnum=LENGTH(arg)+LENGTH(size)-22
END
IF arg='' THEN RETURN 1
IF is_here(arg) THEN RETURN 1
SAY
SAY 'Please select an appropriate library for -' pen3||arg def'-'
IF chdir()>0 THEN RETURN 1
SAY 'Working...'lineup
ADDRESS COMMAND 'C:COPY' fromfile PRAGMA('D')'/'arg
IF testfile() THEN RETURN 1
CALL DELETE(libpath||plaindir'/.'STRIP(LEFT(plaindir,15)))
IF getinput(1 1 'Kill original file' fromfile '? (Ny) > ')='Y' THEN
  DO
    CALL DELETE(fromfile)
    SAY 'Killed' fromfile'...'
  END
DO ui=sysoplevel+2 TO 100
  IF UPPER(dirs.ui)=UPPER(plaindir) THEN RETURN 0
END
DO WHILE editnote() /* INSIST on a filenote */
END
RETURN 0


testfile:
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
        RETURN 1
      END
  END
RETURN 0


editnote:
comment=''
IF ~EXISTS(arg) THEN
  DO
    IF level<sysoplevel THEN RETURN 0
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
    ELSE temp=getinput(1 1 'Is this file on an another device? (Nqy)')
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
filedir=plaindir
CALL MAKEDIR(bbspath'FileNotes/'filedir)
IF ~EXISTS(bbspath'FileNotes/'filedir) THEN
  DO
    SAY pen3'*** Failed to open directory!' filedir||def
    RETURN 0
  END
notename=bbspath'FileNotes/'filedir'/'arg
lynes.=''
filenum=countcheck('Numbers/LastFile' 0)
IF level>sysoplevel THEN firstedit=1
ELSE firstedit=5
IF EXISTS(notename) THEN
  DO
    IF comment~='' THEN ADDRESS COMMAND 'C:filenote' notename comment
    CALL bbsED.rexx(firstedit notename name)
    CALL readlines(notename 1)
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
CALL writebuffer(scratch'/NoteFile')
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
      IF bbsED.rexx(firstedit notename name)>0 THEN RETURN 0
      CALL readlines(notename 1)
      CALL checkfilechanges()
      fncom='R'
    END
  ELSE IF fncom~='R' THEN
    DO
      SAY 'Adjusting filelist...'
      IF filenum<1 THEN filenum=1
      IF GETCLIP('BBS_level')~='' THEN CALL SETCLIP('BBS_localfiles',1)
      CALL countcheck('Numbers/LastFile' filenum)
      IF files.0>0 THEN files.filenum=plaindir arg
      IF EXISTS(bbspath'Lists/Files') THEN
        x=OPEN(f,bbspath'Lists/Files','A')
      ELSE x=OPEN(f,bbspath'Lists/Files','W')
      IF x=0 THEN
        DO
          SAY '*** Failed to open' bbspath'Lists/Files'
          RETURN 0
        END
      CALL WRITELN(f,filenum plaindir arg)
      CALL CLOSE(f)
      libnum=finddirnum(plaindir)
      PARSE VAR lynes.1 . 'KeyWords:' keywords
      alpha=LEFT(arg,22-LENGTH(WORD(lynes.2,4)))
      alpha=alpha WORD(lynes.2,4) RIGHT(filenum,5)
      alpha=alpha RIGHT(libnum,2) LEFT(plaindir,12)
      alpha=alpha STRIP(LEFT(STRIP(keywords),32))
      IF SHOW('P','BBBBS') THEN CALL SETCLIP('BBS_localfiles',1)
      IF SHOW('P','BBBBS_LOCAL') THEN CALL SETCLIP('BBS_mainfiles',1)
      newlist.0=newlist.0+1
      nl=newlist.0
      newlist.nl=alpha
      SAY alpha
      tf=bbspath'Lists/Files.ALPHA.add'
      IF EXISTS(tf) THEN ft='A'
      ELSE ft='W'
      x=OPEN(a,tf,ft)
      IF x=0 THEN SAY 'Unable to open File.ALPHA.add for writing!'
      ELSE
        DO
          CALL WRITELN(a,alpha)
          CALL CLOSE(a)
        END
    END
END
RETURN 0


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


savefilelist:
xarg=bbspath'Lists/Files'
CALL DELETE(xarg)
filenum=countcheck('Numbers/LastFile' 0)
IF filenum<1 | files.0<1 THEN RETURN
IF ~writeopen(xarg) THEN RETURN
SAY 'Saving filelist...'
DO i=1 TO filenum
  IF files.i='' THEN ITERATE
  CALL WRITELN(f,i files.i)
END
CALL CLOSE(f)
RETURN


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


cleanline:
ARG lflag .
cline=lineup||LEFT(' ',78)
IF lflag=1 THEN cline=cline||lineup
SAY cline
RETURN


countcheck:
PARSE ARG fname' 'cknum .
fname=bbspath||fname
IF ~EXISTS(fname) THEN
  DO
    IF cknum=0 THEN RETURN 0
    IF ~writeopen(fname) THEN RETURN 0
    CALL WRITELN(f,cknum)
    CALL CLOSE(f)
    RETURN cknum
  END
IF ~readopen(fname) THEN RETURN cknum
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
CALL DELETE(bufname)
CALL savelines(bufname)
CALL readlines(bufname 1)
CALL wrapbuf(startnum)
RETURN


wrapbuf:
ARG startnum .
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
DO i=1 TO lynes.0
  SAY lynes.i||def
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
    SAY '***' tempname 'failed to open for saving!'
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


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
top=XRANGE('7F'x)
cstr=COMPRESS(cstr,bot||top)
IF nflag=0 THEN cstr=STRIP(cstr)
RETURN cstr


getinput:
PARSE ARG upflag' 'oneflag' 'pline
OPTIONS PROMPT pline
PARSE PULL inarg
inarg=STRIP(inarg)
IF upflag THEN inarg=UPPER(inarg)
IF oneflag THEN inarg=LEFT(inarg,1)
RETURN inarg


setdir:
PARSE ARG tempdir
CALL PRAGMA('D',STRIP(tempdir))
plaindir=PRAGMA('D')
slash=LASTPOS('/',plaindir)
IF slash=0 THEN slash=LASTPOS(':',plaindir)
IF slash>0 THEN plaindir=SUBSTR(plaindir,slash+1)
RETURN


waiting2:
IF nonstop=1 THEN RETURN 0
waitchar=getinput(1 1 pen3'   Q=Quit   N=Non-Stop   RETURN=Continue  'def)
IF waitchar='N' THEN
  DO
    nonstop=1
    SAY pen3'To EXIT non-stop scrolling of text, press CTRL-E        'def
    SAY
    CALL DELAY(100)
    waitchar=''
  END
IF waitchar='Q' THEN RETURN 1
RETURN 0


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
RETURN


finddirnum:
ARG fdirname .
DO fdir=1 TO 99
  IF UPPER(dirs.fdir)=UPPER(fdirname) THEN RETURN fdir
END
RETURN 100


chdir:
string=''
SAY pen3||LEFT('-',75,'-')||def
DO i=1 TO libs.0
  SAY libs.i
END
dirnum=getinput(1 0 pen3'Select Library Number: 'def)
IF clr~='' THEN Send clr
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
CALL MAKEDIR(libpath||dirs.dirnum)
CALL setdir(libpath||dirs.dirnum)
t=libpath||plaindir'.txt'
IF ~EXISTS(t) THEN RETURN 0
nonstop=1
SAY
CALL readlines(t 1)
CALL seelines(1)
SAY
nonstop=0
RETURN 0


fileupdate:
IF savefileflag THEN CALL savefilelist()
CALL checkliblists()
/*
SAY 'DEBUG:'
DO i=1 TO newlist.0
  SAY i newlist.i
END
SAY 'END DEBUG:'
*/
IF newlist.0>1 THEN CALL QSort(1,newlist.0,newlist)
IF ~listOPEN(a,bbspath'Lists/Files.ALPHA','R') THEN
  DO
    SAY 'Files.ALPHA failed to open for reading!'
    RETURN
  END
CALL CLOSE(a)
tf=bbspath'Lists/Files.ALPHA.temp'
IF EXISTS(tf) THEN CALL DELETE(tf)
CALL RENAME(bbspath'Lists/Files.ALPHA',tf)
IF ~listOPEN(f,tf,'R') THEN
  DO
    SAY 'Unable to open Files.ALPHA.temp for reading!'
    RETURN
  END
IF ~listOPEN(a,bbspath'Lists/Files.ALPHA','W') THEN
  DO
    SAY 'Unable to open Files.ALPHA for writing!'
    RETURN
  END
nl=1
totfiles=0
SAY 'Writing new alphabetical filelists...'
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  filenum=WORD(line,3)
  IF files.0>0 THEN
    DO
      IF DATATYPE(filenum,'W') & files.filenum='' THEN
        DO
          SAY 'Deleted:' WORD(line,1)
          ITERATE i
        END
      DO j=1 TO WORDS(numlist)
        IF filenum=WORD(numlist,j) THEN
          DO
            CALL STRIP(DELWORD(numlist,j,1))
            SAY 'Changed:' WORD(line,1)
            ITERATE i
          END
      END
    END
  IF nl<=newlist.0 THEN
    DO WHILE (UPPER(newlist.nl)<UPPER(line)) & (nl<=newlist.0)
      SAY newlist.nl
      CALL WRITELN(a,newlist.nl)
      CALL updateliblists(newlist.nl)
      totfiles=totfiles+1
      nl=nl+1
    END
  CALL WRITELN(a,line)
  CALL updateliblists(line)
  totfiles=totfiles+1
END
totfiles=totfiles+newlist.0-nl-1
DO i=nl TO newlist.0
  CALL WRITELN(a,newlist.i)
  CALL updateliblists(newlist.i)
END
CALL CLOSE(a)
CALL CLOSE(f)
CALL closeliblists()
CALL DELETE(tf)
CALL DELETE(bbspath'Lists/Files.ALPHA.add')
CALL bbsALPHA.rexx(totfiles SUBSTR(extension,2) arccom)
RETURN


listOPEN:
PARSE ARG fh,listfile,flag
CALL CLOSE(fh)
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
dirs.=''
IF ~listOPEN(f,bbspath'Lists/Libraries','R') THEN RETURN
DO i=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  num=WORD(line,1)
  IF ~DATATYPE(num,'W') THEN ITERATE i
  dirs.num=WORD(line,2)
  finfo=STATEF(libpath||dirs.num'/.'STRIP(LEFT(dirs.num,15)))
  IF finfo='' THEN liblist=liblist num
END
CALL CLOSE(f)
liblist=STRIP(liblist)
DO j=1 TO WORDS(liblist)
  tt=WORD(liblist,j)
  CALL MAKEDIR(libpath||dirs.tt)
  lf=libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15))
  flg='W'
  IF EXISTS(libpath||dirs.tt'.txt') THEN
    DO
      ADDRESS COMMAND 'COPY' libpath||dirs.tt'.txt' lf
      flg='A'
    END
  IF ~listOPEN(f,lf,flg) THEN ITERATE j
  IF flg='A' THEN CALL WRITELN(f,'')
  CALL WRITELN(f,'Filename          Bytes File# Library         KeyWords')
  CALL WRITELN(f,LEFT('=',77,'='))
  CALL CLOSE(f)
END
RETURN


updateliblists:
PARSE ARG ln 
x=FIND(liblist,WORD(ln,4))
IF x=0 THEN RETURN
tt=WORD(liblist,x)
IF tt~=lastlib THEN
  DO
    CALL CLOSE(b)
    lastlib=tt
    x=OPEN(b,libpath||dirs.tt'/.'STRIP(LEFT(dirs.tt,15)),'A')
    IF x=0 THEN
      DO
        lastlib=0
        RETURN
      END
  END
CALL WRITELN(b,ln)
cnt.tt=cnt.tt+1
RETURN


closeliblists:
CALL CLOSE(b)
DO i=1 TO WORDS(liblist)
  tt=WORD(liblist,i)
  dname='.'STRIP(LEFT(dirs.tt,15))
  SAY ' 'dname
  x=OPEN(f,libpath||dirs.tt'/'dname,'A')
  IF x~=0 THEN
    DO
      CALL WRITELN(f,LEFT('-',77,'-'))
      temp='file'
      IF cnt.tt>1 THEN temp=temp's'
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
  IF x~=0 THEN
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


browse:
IF files.0=0 THEN CALL loadfiles()
arg=''
curdironly=0
brdir=PRAGMA('D')
brfilenum=1
nonstop=0
IF files.0<1 THEN RETURN
lastfile=countcheck('Numbers/LastFile' 0)
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
      line=''
      IF FIND('.ARC .ARJ .DMS .LZH .LHA .RUN .ZIP .ZOO',endtest)>0 THEN
        line='['pen3'C'def']ontents '
      line=line'['pen3'D'def']ownload '
      IF level>sysoplevel | name=WORD(lynes.3,2) THEN
        line=line'['pen3'E'def']dit'
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
          wordnum=FIND(dlist,brfilenum)
          IF wordnum=0 THEN
            DO
              dlist=STRIP(dlist brfilenum)
              SAY lineup||argname 'marked to copy later.'
              SAY
            END
          ELSE
            DO
              dlist=STRIP(DELWORD(dlist,wordnum,1))
              SAY argname 'removed from copy list.'
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
                      keywords=STRIP(SUBSTR(lynes.1,23))
                      CALL addfile()
                      argname=newarg
                      savefileflag=1
                      CALL DELETE(libpath||plaindir'/.'STRIP(LEFT(plaindir,15)))
                    END
                END
            END
          IF getinput(1 1 'Move' argname '? (Ny) > ')='Y' THEN
            DO
              IF chdir()=0 THEN
                DO
                  IF UPPER(dirs.dirnum)~=UPPER(WORD(files.brfilenum,1)) THEN
                    DO
                      CALL readlines(arg 1)
                      CALL movefile(brfilenum dirs.dirnum)
                    END
                END
            END
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
              CALL cleanline(1)
              CALL readlines('RAM:CONTENTS' 1)
              CALL seelines(0)
              IF waitchar~='Q' THEN CALL waiting()
              nonstop=0
            END
          ELSE SAY pen3'Not an archived file.'def
        END
      ELSE IF brcom='D' THEN
        DO
          IF pickdir() THEN CALL copyfile(brfilenum)
        END
      ELSE IF brcom='E' THEN
        DO
          IF level>sysoplevel | name=WORD(lynes.3,2) THEN
            DO
              firstedit=5
              IF level>sysoplevel THEN firstedit=1
              CALL bbsEd.rexx(firstedit arg name)
              CALL readlines(arg 1)
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
                      CALL countcheck('Numbers/LastFile' lastfile)
                    END
                  files.tempnum=''
                  savefileflag=1
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
          CALL showtext(argname)
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


pickdir:
topath=GETCLIP('BBS_topath')
IF topath='' THEN topath=GETCLIP('BBS_frompath')
todir=GetFile(150,36,topath,'',' Destination ',,'NOFILES')
IF RIGHT(todir,1)='/' THEN todir=LEFT(todir,LENGTH(todir)-1)
IF WORD(STATEF(todir),1)='DIR' THEN
  DO
    IF todir~=topath THEN CALL SETCLIP('BBS_topath',todir)
    RETURN 1
  END
ELSE SAY pen3'*'def||todir||pen3'* is not a valid directory!'def
RETURN 0


copyfile:
ARG fnum .
copyarg=libpath||SPACE(files.fnum,1,'/')
fname=WORD(files.num,2)
finfo=STATEF(arg)
IF WORDS(finfo)>7 THEN copyarg=WORD(finfo,8)
SAY 'Copying' fname 'to' todir'...'
ADDRESS COMMAND 'COPY' copyarg todir 'CLONE'
RETURN


movefile:
PARSE ARG fnum movdir .
savefileflag=1
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
IF EXISTS(mn) THEN
  DO
    CALL DELETE(fn)
    comm='C:FileNote' mn
    IF commen~='' THEN comm=comm commen
    ADDRESS COMMAND comm
  END
keywords=STRIP(SUBSTR(lynes.1,23))
CALL addfile()
SAY 'Moved:' fromdir'/'farg 'to' movdir
RETURN


savedata:
SAY 'Updating...             'lineup
IF lastbrowse~=WORD(data.16,1) THEN data.16=lastbrowse DATE('S') TIME()
IF ~writeopen(bbspath'USERS/'name) THEN RETURN
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
lastbrowse=WORD(data.16,1)
IF ~DATATYPE(lastbrowse,'W') THEN lastbrowse=0
level=data.20
password=data.5
linesperpage=20
colorflag=1
IF FIND(UPPER(data.8),'CLEAR')>0 THEN clr='0C'x
ELSE clr=''
RETURN 1


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
  IF DATATYPE(num,'W') THEN files.num=WORD(line,2) WORD(line,3)
END
files.0=i-1
CALL CLOSE(f)
RETURN


checkfilechanges:
t=GETCLIP('BBS_FileChangeLOCAL')
IF t='' THEN RETURN
CALL SETCLIP('BBS_FileChangeLOCAL')
tn=WORDS(t)
IF tn>1 THEN
  DO
    fnum=WORD(t,tn)%1
    t=STRIP(DELWORD(t,tn,1))
    CALL SETCLIP('BBS_FileChangeLOCAL',t)
  END
ELSE fnum=t%1
keywords=GETCLIP('BBS_Keywords_'fnum)
CALL SETCLIP('BBS_Keywords_'fnum)
CALL addfile()
RETURN


addfile:
filenum=WORD(lynes.1,2)
num=0
DO ii=1 TO newlist.0
  IF WORD(newlist.ii,3)=filenum THEN num=ii
END
IF num=0 THEN
  DO
    num=newlist.0+1
    newlist.0=num
  END
libnum=finddirnum(WORD(lynes.3,9))
newlist.num=LEFT(WORD(lynes.2,2),22-LENGTH(WORD(lynes.2,4)))
newlist.num=newlist.num WORD(lynes.2,4) RIGHT(filenum,5)
newlist.num=newlist.num RIGHT(libnum,2) LEFT(WORD(files.filenum,1),12)
newlist.num=newlist.num STRIP(LEFT(keywords,32))
IF FIND(numlist,filenum)=0 THEN numlist=filenum numlist
RETURN


edinfo:
PARSE ARG t1,t2,t3
IF level<sysoplevel THEN RETURN 0
IF getinput(1 1 'Edit the'pen3 t2 def||t3 'info file? (Ny) > ')='Y' THEN
  DO
    IF ~EXISTS(t) THEN
      DO
        IF writeopen(t1) THEN
          DO
            CALL WRITELN(f,TRIM(CENTER('***'pen3 t2 def||t3 '***',75)))
            CALL WRITELN(f,LEFT('',75,'='))
            CALL CLOSE(f)
            CALL DELAY(28)
          END
      END
    CALL bbsEd.rexx(1 t1 name)
    RETURN 1
  END
RETURN 0


busywait:
ARG bii bi bt 
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


BREAK_E:
i=999999
ri=999999
wi=999999
RETURN


BREAK_C:
EXIT

/* bbsFile.rexx */
