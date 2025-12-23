/* $VER: bbsEd.rexx 8.4 (21.1.95)
copyright © 1994-95 Richard Lee Stockton
BBBBS text editor
FREELY DISTRIBUTABLE
*/

IF ~SHOW('P','QuickSortPort') THEN CALL setup.rexx()
IF ~SHOW('P','QuickSortPort') THEN EXIT 666

OPTIONS RESULTS
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
SIGNAL ON FAILURE
SIGNAL ON SYNTAX

PARSE ARG firstedit editarg name maxtime .
IF ~DATATYPE(maxtime,'N') THEN maxtime=3000

CALL TIME('R')
namemask=COMPRESS(XRANGE(),XRANGE('A','Z')' _-')

def='[0m'
pen2='[32m'
pen3='[33m'
bak2='[42m'
IF colorflag=0 THEN
  DO
    def=''
    pen2=''
    pen3=''
    bak2=''
  END
lineup='1B'x'M'
CR=''
IF ADDRESS()='BAUD' THEN
  DO
    CR='0D'x
    frombb=1
  END
ELSE frombb=0

SAY '                  'lineup||CR
SAY '                   'pen3'Entering the EDITOR module..'def||CR
SAY CR
CALL config()
CALL loaddata()
pasted.=''
pasted.0=0
notchanged=1
IF readlines(editarg 1) THEN EXIT 1
IF OPEN(f,scratch'/edclip','R')~=0 THEN
  DO
    DO i=1
      line=READLN(f)
      IF EOF(f) THEN LEAVE i
      pasted.i=line
    END
    CALL CLOSE(f)
    pasted.0=i-1
  END
finfo=STATEF(editarg)
IF WORDS(finfo)>7 THEN finfo=SUBSTR(finfo,WORDINDEX(finfo,8))
ELSE finfo=''
count=1
DO edloop=1
  IF edcom='S' & bbsprefs.5 THEN  /* spell check */
    DO
      SAY pen3'You must use ['def'R'pen3']eplace to make corrections.  'pen2'Spellchecking...'def||CR
      CALL DELETE(scratch'/SpellFile')
      CALL savelines(scratch'/SpellFile')
      curdir=PRAGMA('D')
      CALL setdir(spellpath)
      CALL SpellChk.rexx(scratch'/SpellFile')
      CALL setdir(curdir)
    END
  ELSE
    DO
      IF edcom='R' | edcom='I' | edcom='L' THEN CALL wrapbuf(7)
      IF edcom~='L' THEN count=count-linesperpage
      IF count>=lynes.0 | count<1 THEN count=1
      startcount=count
      DO i=startcount TO lynes.0+1
        IF ((i+1-startcount)//linesperpage)=0 & i<lynes.0 THEN
          DO
            pline='                 ['pen3'E'def']dit'
            pline=pline '  ['pen3'RETURN'def']=Continue '
            edcom=getinput(1 1 pline)
            IF edcom~='' THEN LEAVE i
            CALL cleanline(1)
          END
        SAY pen3||RIGHT(i,3)||def lynes.i||CR
        count=count+1
      END
    END
  CALL checktime()
  SAY lineup'     ['pen3'A'def']ppend ['pen3'C'def']ut     ['pen3'I'def']nsert  ['pen3'K'def']ill       ['pen3'?'def'] Help'CR
  pline='     ['pen3'L'def']ist   ['pen3'P'def']aste   ['pen3'R'def']eplace'
  IF bbsprefs.5 THEN pline=pline '['pen3'S'def']pellcheck'
  pline=pline '['pen3'U'def']pload-Text > '
  edcom=getinput(1 0 pline)
  IF edcom='Q' | edcom='X' THEN edcom=''
  IF edcom='?' THEN
    DO
      SAY CR
      SAY '                   Editor Help'CR
      SAY '----------------------------------------------------------'CR
      SAY '    an empty RETURN tells the editor you are done editing.'CR
      SAY ' 7  edits line number 7, if it exists.'CR
      SAY ' a  Append text to this file.'CR
      SAY ' c  Cut selected line(s) of text to buffer.'CR
      SAY ' i  Insert blank line.'CR
      SAY ' k  Kill (delete) this file.'CR
      SAY ' l  List this file from selected line.'CR
      SAY ' p  Paste buffer contents to selected line number.'CR
      SAY ' r  Replace a phrase or line of text.'CR
      SAY ' s  Spellcheck this file.'CR
      SAY ' u  Upload a textfile to append to this file.'CR
      SAY '----------------------------------------------------------'CR
      SAY CR
      OPTIONS PROMPT ''
      PULL
    END
  IF edcom='K' THEN
    DO
      junk=getinput(1 1 'Are you' pen3'sure'def 'you want to delete' editarg'? (Ny) > ')
      IF junk='Y' THEN
        DO
          IF DELETE(editarg)=1 THEN SAY editarg 'DELETED.'CR
          IF WORD(lynes.1,1)='Mail:' & WORDS(lynes.2)>3 THEN
            DO
              IF DELETE(bbspath'EmailFiles/'WORD(lynes.3,2)'/'WORD(lynes.2,4))=1 THEN
                SAY WORD(lynes.2,4) 'DELETED.'CR
            END
          EXIT 2
        END
    END
  IF edcom='' THEN
    DO
      SAY '                   'pen3'Leaving the EDITOR module.'def||CR
      IF pasted.0>0 THEN
        IF OPEN(f,scratch'/edclip','W')~=0 THEN
          DO
            IF pasted.0>99 THEN pasted.0=99
            DO i=1 TO pasted.0
              CALL WRITELN(f,pasted.i)
            END
            CALL CLOSE(f)
          END
      IF notchanged THEN EXIT 0
      IF getinput(1 1 '                     Save changes? (nY)'pen3' > 'def)='N' THEN
        EXIT 1
      CALL DELETE(editarg)
      IF savelines(editarg) THEN EXIT 1
      CALL DELAY(28)
      IF finfo~='' THEN ADDRESS COMMAND 'C:filenote' editarg finfo
      SAY pen3'                        Changes saved.'def||CR
      EXIT 0
    END
  ELSE IF edcom='C' THEN  /* Cut */
    DO
      firstnum=getinput(1 0 '   Enter line number or range 'pen3'(5-7)'def' to cut' pen3'>'def)
      IF firstnum='' THEN ITERATE edloop
      dash=POS('-',firstnum)
      IF dash>0 THEN
        DO
          lastnum=STRIP(SUBSTR(firstnum,dash+1))
          firstnum=STRIP(LEFT(firstnum,dash-1))
        END
      ELSE lastnum=firstnum
      IF ~DATATYPE(firstnum,'W') | ~DATATYPE(lastnum,'W') THEN
        DO
          junk=getinput(1 1 pen3'*** You must enter numbers here! 'def)
          ITERATE edloop
        END
      IF lastnum>lynes.0 THEN lastnum=lynes.0
      IF firstnum<firstedit THEN
        DO
          SAY '*** You are not authorized to delete that line!'CR
          SAY CR
          ITERATE edloop
        END
      IF firstnum>lastnum THEN
        DO
          SAY '*** Input error!  First number larger than last number.'CR
          ITERATE edloop
        END
      notchanged=0
      numdiff=lastnum+1-firstnum
      pasted.=''
      pasted.0=numdiff
      k=0
      DO i=firstnum TO lynes.0
        j=i+numdiff
        k=k+1
        IF k<=numdiff THEN pasted.k=lynes.i
        lynes.i=lynes.j
        lynes.j=''
      END
      lynes.0=lynes.0-numdiff
      count=1
    END
  ELSE IF edcom='A' THEN  /* append */
    DO
      IF frombb THEN temp='File'
      ELSE temp='LOCAL'
      CALL writebuffer(scratch'/Editor'temp)
      notchanged=0
    END
  ELSE IF edcom='U' THEN  /* Upload a textfile to append */
    DO
      CALL txup(editarg)
      notchanged=0
    END
  ELSE IF edcom='I' | edcom='R' | edcom='L' | edcom='P' | DATATYPE(edcom,'W') THEN
    DO
      IF DATATYPE(edcom,'W') THEN
        DO
          ednum=edcom
          edcom='R'
        END
      ELSE
        DO
          line=pen3'   '
          IF edcom='L' | edcom='P' THEN line=line'Starting '
          line=line'Line Number? > 'def
          ednum=getinput(1 0 line)
        END
      IF ~DATATYPE(ednum,'W') THEN ITERATE edloop
      IF ednum>(lynes.0+1) THEN ITERATE edloop
      IF edcom='L' THEN
        DO
          count=ednum
          ITERATE edloop
        END
      IF ednum=1 & UPPER(WORD(lynes.1,1))='FILE:' THEN
        DO
          IF getinput(1 1 pen3'Edit KeyWords:? (Ny) > 'def)='Y' THEN
            DO
              filenum=STRIP(WORD(lynes.1,2))
              keywords=edkeywords(editarg)
              lynes.1=LEFT(lynes.1,21) keywords
              suf='LOCAL'
              IF frombb THEN suf=''
              t=GETCLIP('BBS_FileChange'suf)
              CALL SETCLIP('BBS_FileChange'suf,STRIP(t filenum))
              CALL SETCLIP('BBS_Keywords_'filenum,keywords)
              notchanged=0
              ITERATE edloop
            END
        END
      IF ednum<firstedit THEN
        DO
          SAY '*** You are not authorized to alter that line!'CR
          SAY CR
          ITERATE edloop
        END
      IF edcom='R' THEN   /* replace */
        DO
          SAY '   Now reads:'CR
          SAY pen3||RIGHT(ednum,2)||def lynes.ednum||CR
          OPTIONS PROMPT pen3'........Search text? >'def
          PARSE PULL stext
          IF LENGTH(stext)=0 THEN
            DO
              IF getinput(1 1 lineup||pen3'Replace entire line? (nY) >'def)='N' THEN
                ITERATE edloop
              lynes.ednum=getinput(0 0 pen3||RIGHT(ednum,2)' 'def)
              notchanged=0
              ITERATE edloop
            END
          found=POS(UPPER(stext),UPPER(lynes.ednum))
          IF found=0 THEN
            DO
              SAY CR
              SAY stext' was not found!'CR
              SAY CR
              ITERATE edloop
            END
          OPTIONS PROMPT pen3'...Replacement text? >'def
          PARSE PULL rtext
          lynes.ednum=DELSTR(lynes.ednum,found,LENGTH(stext))
          lynes.ednum=INSERT(rtext,lynes.ednum,found-1)
          IF ednum<4 & LEFT(lynes.1,6)='File: ' THEN
            DO
              PARSE VAR lynes.1 'File: 'filenum . 'KeyWords: 'keywords
              PARSE VAR lynes.3 . 'Lib:' libnam
              filenum=STRIP(filenum)
              newc=files.filenum.0
              libnum=finddirnum(libnam)
              alpha.newc=LEFT(WORD(lynes.2,2),22-LENGTH(WORD(lynes.2,4)))
              alpha.newc=alpha.newc WORD(lynes.2,4) RIGHT(filenum,5)
              alpha.newc=alpha.newc RIGHT(libnum,2) LEFT(STRIP(libnam),12)
              alpha.newc=alpha.newc STRIP(LEFT(STRIP(keywords),32))
              savefileflag=1
            END
          SAY 'Done.'CR
          SAY CR
          notchanged=0
        END
      ELSE IF edcom='I' THEN  /* insert */
        DO
          DO i=lynes.0 TO ednum BY -1
            j=i+1
            lynes.j=lynes.i
          END
          lynes.ednum=''
          notchanged=0
          lynes.0=lynes.0+1
          OPTIONS PROMPT pen3||RIGHT(ednum,2)'>'def
          PARSE PULL lynes.ednum
        END
      ELSE IF edcom='P' THEN   /* paste */
        DO
          DO i=lynes.0 TO ednum BY -1
            j=i+pasted.0
            lynes.j=lynes.i
          END
          DO k=1 TO pasted.0
            kk=ednum+k-1
            lynes.kk=pasted.k
          END
          notchanged=0
          lynes.0=lynes.0+pasted.0
        END
    END
END
EXIT 0


writebuffer:
PARSE ARG bufname .
IF frombb THEN Capture OFF
CALL DELETE(bufname)
startnum=lynes.0+1
SAY 'Type 'pen3'/E'def' or 'pen3'/S'def' on a new line to Exit and Save.'CR
IF EXISTS(bufname) THEN
  DO
    CALL DELAY(56)
    CALL DELETE(bufname)
    CALL DELAY(56)
  END
IF frombb THEN
  DO
    CaptWrap 74
    Send pen3
    Capture bufname
    Send def
    TimeOut 120
    DO bufloop=1
      Wait '/E,/S,RING,NO CARRIER'
      Status 'L'
      test=LEFT(UPPER(cleanstring(0':'RESULT)),2)
      CALL checkdcd()
      IF test='/E' | test='/S' | test='/X' THEN LEAVE bufloop
    END
    IF test~='/X' THEN Send '\b\b'pen3
    Capture OFF
    CALL checkdcd()
    TimeOut maxidle
    SAY def||CR
    CALL readlines(bufname startnum)
    CALL wrapbuf(startnum)
    QUEUE CR
  END
ELSE
  DO
    OPTIONS PROMPT ''
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
    SAY
  END
RETURN


wrapbuf:
ARG startnum .
CALL cleanline(1)
SAY pen3'Wordwrapping...'def||CR
lynes.startnum=TRANSLATE(lynes.startnum,' ','09'x)
lynes.startnum=cleanstring(2':'lynes.startnum)
DO wi=startnum WHILE wi<=lynes.0
  wj=wi+1
  lynes.wj=TRANSLATE(lynes.wj,' ','09'x)
  lynes.wj=cleanstring(2':'lynes.wj)
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


txup:
PARSE ARG uparg .
IF frombb THEN
  DO
    SAY 'Ready to append' pen3'TEXT ONLY'def 'using'pen3 protocol||def||CR
    pline='Are you SURE your file is un-compressed text? (Ny) > '
    IF getinput(1 1 pline)~='Y' THEN RETURN
  END
savearg=arg
arg='Upload'
arg2='tempfile1'
IF frombb THEN arg=arg'File'
ELSE
  DO
    arg=arg'LOCAL'
    arg2=arg2'LOCAL'
  END
curdir=PRAGMA('D')
CALL setdir(scratch)
CALL DELETE(arg)
CALL DELETE(arg2)
IF uload()=0 THEN
  DO
    ADDRESS COMMAND 'C:copy' uparg scratch'/'arg2 'CLONE'
    CALL DELETE(uparg)
    ADDRESS COMMAND 'C:join' scratch'/'arg2 PRAGMA('D')'/'arg 'AS' uparg
  END
CALL readlines(uparg 1)
notchanged=0
CALL setdir(curdir)
arg=savearg
RETURN


chpro:
arg=UPPER(LEFT(arg,1))
IF(arg='') THEN
  DO
    SAY CR
    SAY '['pen3'W'def']- WXModem'CR
    SAY '['pen3'X'def']- XModem-CRC'CR
    SAY '['pen3'K'def']- XModem-1K'CR
    SAY '['pen3'Y'def']- YModem'CR
    SAY '['pen3'G'def']- YModem-G'CR
    SAY '['pen3'Z'def']- ZModem'CR
    SAY CR
    arg=getinput(1 0 STRIP(protocol) '> ')
 END
IF LEFT(UPPER(arg),1)='A' THEN arg='Z'
Set arg
Status Transfer
protocol=STRIP(RESULT)
SAY protocol||CR
RETURN


uload:
CALL bbsspace(12)
SAY CR
IF bbsk<1 THEN
  DO
    line='Upload area is full!'
    CALL send2log(line)
    SAY pen3||line||def||CR
    RETURN 1
  END
IF frombb THEN
  DO
    checkproto='T'
    targ=arg
    DO WHILE checkproto='T'
      arg=''
      SAY CR
      SAY 'Library:'pen3 plaindir def'  Filename:'pen3 targ def'  Protocol:'pen3 protocol||def||CR
      pline=' ['pen3'Q'def']uit ['pen3'T'def']ransfer-protocol'
      pline=pline '['pen3'U'def']pload (qtU) > '
      checkproto=getinput(1 1 pline)
      IF checkproto='Q' THEN RETURN 1
      IF checkproto='T' THEN CALL chpro()
    END
    arg=targ
    IF bbsprefs.13~=1 THEN ADDRESS AREXX bbsSounds.rexx bbspath'/Sounds' 'UPLOAD'
    uploadtime=TIME('E')
    CALL checktime()
    SAY 'Starting' protocol 'transfer.  Press' pen3'Esc'def 'to abort.'CR
    DownLoad arg
    IF RC>0 THEN RETURN 2
    IF bbsXferStats.baud(14 arg colorflag protocol) THEN RETURN 2
    rbytes=WORD(STATEF(arg),2)
    IF rbytes<1 THEN
      DO
        CALL DELETE(arg)
        RETURN 2
      END
    temp=''
    DO WHILE temp~='N' & temp~='Y'
      temp=getinput(1 1 'Received' rbytes 'bytes. Was your upload successful? (ny) > ')
    END
    IF temp='N' THEN RETURN 2
  END
ELSE
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
    ADDRESS COMMAND 'C:COPY' fromfile PRAGMA('D') 'CLONE'
    rbytes=WORD(STATEF(fromfile),2)
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
IF TestArc.rexx(PRAGMA('D')'/'arg)>0 THEN
  DO
    SAY CR
    SAY pen3'***'def arg pen3'failed archive check!'def||CR
    SAY CR
    temp=getinput(1 1 'Do you believe the archive checker made a mistake? (Ny) > ')
    IF temp~='Y' THEN
      DO
        CALL DELETE(arg)
        SAY CR
        RETURN 2
      END
  END
IF ~frombb THEN RETURN 0
CALL bytes2user(14 rbytes)
ADDRESS AREXX bbsNewFile.rexx name PRAGMA('D')'/'arg
IF bbsprefs.9 & name~=sysop THEN
  DO
    newufile=bbspath'EMail/'sysop'/NEW_FILES'
    IF EXISTS(newufile) THEN ok=OPEN(f,newufile,'A')
    ELSE
      DO
        ok=OPEN(f,newufile,'W')
        IF ok~=0 THEN CALL WRITELN(f,'*** New Files ***') 
      END
    IF ok~=0 THEN CALL WRITELN(f,name 'uploaded' plaindir'/'arg'  'DATE() TIME())
    CALL CLOSE(f)
  END
RETURN 0


bytes2user:
PARSE ARG indx bytes .
tfiles=WORD(data.indx,1)
tbytes=WORD(data.indx,3)
IF ~DATATYPE(tfiles,'W') THEN tfiles=0
IF ~DATATYPE(tbytes,'W') THEN tbytes=0
tbytes=tbytes+bytes
tfiles=tfiles+1
IF tfiles>1 THEN data.indx=tfiles 'files' tbytes 'bytes.'
ELSE data.indx='1 file' bytes 'bytes.'
data.indx=data.indx DATE()
CALL savedata(0)
RETURN


bbsspace:
ARG tabspace .
ADDRESS COMMAND 'C:info >'scratch'/infout' bbsdevice
ok=OPEN(f,scratch'/infout','R')
IF ok=0 THEN RETURN 20
line=READLN(f)
line=READLN(f)
line=READLN(f)
line=READLN(f)
CALL CLOSE(f)
IF tabspace<14 THEN SAY CR
bbsk=WORD(line,4)
IF ~DATATYPE(bbsk,'N') THEN
  DO
    line=bbsdevice 'is not an info compatible device!'
    CALL send2log(line)
    SAY pen3||line||def||CR
    bbsk=0
    RETURN
  END
bbsk=bbsk*512-SYSTEM_SPACE_LIMIT
IF bbsk<1 THEN bbsk=0
SAY RIGHT(comma(bbsk),tabspace) 'bytes available for uploads.'CR
RETURN


comma:
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


loaddata:
IF name='' THEN RETURN 0
IF OPEN(f,bbspath'USERS/'name,'R')=0 THEN RETURN 0
data.=''
DO i=1
  line=READLN(f)
  IF EOF(f) THEN BREAK
  data.i=line
END
data.0=i-1
CALL CLOSE(f)
protocol=data.6
IF ~DATATYPE(data.7,'W') | data.7<5 | ~frombb THEN data.7=20
linesperpage=data.7
IF ~frombb THEN linesperpage=20
IF FIND(UPPER(data.8),'COLOR')>0 THEN colorflag=1
ELSE colorflag=0
level=data.20
RETURN 1


savedata:
IF OPEN(f,bbspath'USERS/'name,'W')=0 THEN RETURN
IF data.0<27 THEN data.0=27
DO i=1 TO data.0
  CALL WRITELN(f,data.i)
END
CALL CLOSE(f)
SAY 'User' name 'has been updated.'CR
IF frombb THEN CALL SETCLIP('BBS_interpret','CALL loaddata()')
RETURN


edkeywords:
PARSE ARG kwarg
templine=''
DO WHILE LENGTH(templine)<3
  SAY CR
  SAY pen3'Please enter a list of keywords (or a condensed description)'def||CR
  SAY pen3'to be used in the alphabetic list and by the search routine.'def||CR
  SAY '    Note that only the first 32 characters will be used.'CR
  SAY LEFT('',43)'|'LEFT('',31,'=')'|'CR
  templine=getinput(0 0 ' 'RIGHT(STRIP(RIGHT(kwarg,32)),32) pen3'KeyWords: 'def)
  templine=cleanstring('0:'templine)
  templine=STRIP(LEFT(templine,32))
  SAY CR
END
RETURN templine


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
    line='***' tempname 'failed to open for saving!'
    CALL send2log(line)
    SAY line||CR
    RETURN 1
  END
DO wi=1 TO lynes.0
  CALL WRITELN(f,lynes.wi)
END
CALL CLOSE(f)
RETURN 0


setdir:
PARSE ARG tempdir
CALL PRAGMA('D',STRIP(tempdir))
directory=PRAGMA('D')
IF frombb THEN Data directory
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
    SAY 's:CONFIG.BBS and BBS:BBS_TEXT/CONFIG.BBS are both missing!'CR
    EXIT 666
  END
bbsdevice=WORD(lynes.4,1)
sysoplevel=WORD(lynes.5,1)
bbspath=WORD(lynes.6,1)
IF ~EXISTS(bbspath) THEN
  DO
    SAY bbspath 'does not exist!'CR
    EXIT 666
  END
testchar=RIGHT(bbspath,1)
IF testchar~='/' & testchar~=':' THEN bbspath=bbspath'/'
SYSTEM_SPACE_LIMIT=WORD(lynes.12,1)
bbsprefs.=''
DO i=16 TO 41
  j=i-15
  bbsprefs.j=STRIP(WORD(lynes.i,1))
END
spellpath=WORD(lynes.9,1)
IF bbsprefs.5 & ~EXISTS(spellpath) THEN
  DO
    SAY spellpath 'does not exist!'CR
    bbsprefs.5=0
  END
IF bbsprefs.10 THEN scratch=bbspath'Scratch'
ELSE scratch='RAM:Scratch'
CALL MAKEDIR(scratch)
RETURN


finddirnum:
ARG fdirname .
IF ~DATATYPE(dirs.0,'W') THEN CALL loaddirs()
DO fdir=1 TO 99
  IF UPPER(dirs.fdir)=UPPER(fdirname) THEN RETURN fdir
END
RETURN 100


loaddirs:
dirs.=''
IF readopen(bbspath'Lists/Libraries')=0 THEN EXIT
SAY 'Loading library list...'CR
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
RETURN


send2log:
PARSE ARG sendline
logfile=bbspath'Logs/log.'DATE('S')
IF ~OPEN('log',logfile,'A') THEN
  DO
    IF ~OPEN('log',logfile,'W') THEN
      DO
        SAY 'failed to open log file'
        RETURN
     END
  END
CALL WRITELN('log','bbsEd:' sendline)
CALL CLOSE('log')
RETURN


checktime:
IF ~frombb THEN RETURN
IF TIME('E')>maxtime THEN EXIT 0
IF TIME('E')>(maxtime-120) THEN SAY '*** Less than 2 minutes left! ***'CR
MSG RIGHT(' ',66-LENGTH(name)) '1B'x'M'||'[30m'||'[41m'||' 'name' level 'level' '||'[0m'
CALL checkdcd()
RETURN


cleanline:
ARG lflag .
IF nonstop=0 & clr~='' THEN
  DO
    Send clr
    RETURN
  END
IF colorflag~=1 & lflag=1 THEN RETURN
cline=lineup||LEFT(' ',78)
IF lflag=1 THEN cline=cline||lineup
SAY cline||CR
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
Capture
IF RC=0 & xmsg~='' THEN
  DO
    CALL SETCLIP('BBS_MESSAGE')
    SAY CR
    SAY bak2' Message From BBBBS: 'def||CR
    SAY xmsg||CR
    SAY CR
    CALL waiting()
  END
IF POS('G',GETCLIP('BBS_COMMAND'))>0 THEN EXIT 0
RETURN


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


BREAK_E:
i=999999
ri=999999
wi=999999
RETURN


BREAK_C:
EXIT 2


FAILURE:
SYNTAX:
lin.1='[36m'ERRORTEXT(RC)'[0m'
lin.2=SIGL-1     SOURCELINE(SIGL-1)
lin.3=SIGL '[36m'SOURCELINE(SIGL)'[0m'
lin.4=SIGL+1     SOURCELINE(SIGL+1)
DO er=1 TO 4
  IF level>sysoplevel | ~frombb THEN SAY 'bbsEd:' lin.er||CR
  IF frombb THEN CALL send2log(lin.er)
END
EXIT 2

/* bbsEd.rexx */
