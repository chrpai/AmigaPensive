/*      $VER: bbsQUICK.rexx 8.3 (11.12.94)
     copyright © 1993-94 Richard Lee Stockton
FREELY DISTRIBUTABLE AS LONG AS THIS NOTICE REMAINS

requires: libs:rexxsyslib.library             c:LhA
          libs:rexxsupport.library            c:join
          libs:rexxarplib.library             c:copy
          libs:rexxmathlib.library            c:delete
          libs:screenshare.library            c:version
          libs:arp.library

 - Preferences -
prefs.0 = color data
prefs.1 = work dir
prefs.2 = download dir
prefs.3 = upload dir
prefs.4 = editor
prefs.5 = signature file  - Added by S.Griffiths 1/8/94
prefs.6 = interlace: 0=OFF or 1=ON
*/

IF SHOW('P','QUICKHOST') THEN
  DO
    ADDRESS QUICKPORT FRONT
    EXIT 0
  END
IF ~SHOW('L','rexxsupport.library') THEN
  CALL ADDLIB('rexxsupport.library',0,-30,0)
IF ~SHOW('L','rexxarplib.library') THEN
  CALL ADDLIB('rexxarplib.library',0,-30,0)

prefs.=''
laceflag=0
resetflag=1

RESET:
CALL setup()

IF EXISTS(prefspath) & resetflag THEN SIGNAL START

START_PREFS:
p.title=CENTER('Please Enter Your Preferences',44)
CALL OpenWindow(QUICKHOST,120,20,400,170,w.idcmp,w.flags,p.title)

CALL AddMenu(QUICKHOST,'Preferences    ')
CALL AddItem(QUICKHOST,' Help','HELP','H')
CALL AddItem(QUICKHOST,' Colors ...','COLORS','C')
CALL AddItem(QUICKHOST,' Signature','SIG')
CALL AddItem(QUICKHOST,' Interlace','LACE','I')
CALL AddItem(QUICKHOST,' Load ...','LOAD','L')
CALL AddItem(QUICKHOST,' Save ...','SAVE',';')
CALL AddItem(QUICKHOST,' Use','USE','U')
CALL AddItem(QUICKHOST,' Cancel','CLOSEWINDOW','Q')
CALL AddItem(QUICKHOST,' About','ABOUT')

pp.1='Work Dir:'
pp.2='Download Dir:'
pp.3='Upload Dir:'
pp.4='Editor:'
pp.5='Signature:'
p.=''
DO i=0 TO 9
  p.i=prefs.i
END
DO i=1 TO 5
  CALL SETAPEN(QUICKHOST,1)
  CALL MOVE(QUICKHOST,18,29+17*i)
  CALL TEXT(QUICKHOST,RIGHT(pp.i,16))
  CALL AddGadget(QUICKHOST,158,23+17*i,i,p.i,'PREFS' i '%g',220,RIDGEBORDER)
END

CALL AddGadget(QUICKHOST, 40,150,4,' Save ','SAVE')
CALL AddGadget(QUICKHOST,180,150,5,' Use ','USE')
CALL AddGadget(QUICKHOST,300,150,6,' Cancel ','CLOSEWINDOW')
CALL AddGadget(QUICKHOST, 30,16,7,' HELP ','HELP')
CALL AddGadget(QUICKHOST,110,16,8,' Load ','LOAD')
CALL AddGadget(QUICKHOST,190,16,9,' Colors ','COLORS')
CALL AddGadget(QUICKHOST,286,16,10,' Signature ','SIG')
CALL AddGadget(QUICKHOST,57,132,11,' Interlace ','LACE')
IF laceflag THEN CALL SetGadget(QUICKHOST,11,'ON')

CALL tofront(QUICKHOST)
CALL box(QUICKHOST,1,1,11,30,378,95)
CALL SetAPen(QUICKHOST,3)
CALL Flood(QUICKHOST,1,5,15)
lastgad=1
CALL SCREENTOFRONT(QUICKSCREEN)
CALL SetAPen(QUICKHOST,1)
DO FOREVER
  CALL ActivateGadget(QUICKHOST,lastgad)
  t=waitpkt(QUICKPORT)
  DO ff=1
    p=GETPKT(QUICKPORT)
    IF c2d(p)=0 THEN LEAVE ff
    command=GETARG(p)
    PARSE VAR command arg1' 'arg2' 'arg3
    t=REPLY(p,0)
    SELECT
      WHEN arg1='LOAD' THEN CALL config('LOAD')
      WHEN arg1='SAVE' | arg1='USE' | arg1='CLOSEWINDOW' THEN
        DO
          DO i=1 TO 9 WHILE arg1~='CLOSEWINDOW'
            prefs.i=p.i
          END
          CALL LOADSIG()
          CALL MAKEDIR(prefs.1)
          CALL PRAGMA('D',prefs.1)
          arcdir=makepath(prefs.3,'OUT')
          CALL MAKEDIR(arcdir)
          IF arg1='SAVE' THEN CALL config('SAVE')
          CALL CloseWindow(QUICKHOST,'CONTINUE')
          resetflag=1
          SIGNAL START
        END
      WHEN arg1='LACE' THEN
        DO
          IF p.6=1 THEN
            DO
              prefs.6=0
              laceflag=0
              CALL SetGadget(QUICKHOST,11,'OFF')
            END
          ELSE
            DO
              prefs.6=1
              laceflag=1
              CALL SetGadget(QUICKHOST,11,'ON')
            END
          CALL Stop(QUICKHOST)
          CALL CloseScreen(QUICKSCREEN)
          resetflag=0
          SIGNAL RESET
        END
      WHEN arg1='HELP' THEN CALL Request(,,prefshelp,,,,QUICKSCREEN)
      WHEN arg1='ABOUT' THEN CALL Request(200,45,abouttext,,,,QUICKSCREEN)
      WHEN arg1='PREFS' THEN
        DO
          n=arg2
          p.n=arg3
          n=n+1
          IF n>6 THEN n=1
          lastgad=n
        END
      WHEN arg1='COLORS' THEN
        /* INTERPRET */ ADDRESS AREXX Palette.rexx QUICKSCREEN 4 QUICKPORT 
      WHEN arg1='NEW_COLORS' THEN p.0=new_colors()
      WHEN arg1='SIG' THEN CALL edit_sig()
      WHEN arg1='MOUSEBUTTONS' THEN NOP
      WHEN arg1='SELECTUP' THEN NOP
      WHEN arg1='CONTINUE' THEN NOP
      OTHERWISE CALL Request(,100,command,,,,QUICKSCREEN)
    END
  END
END


START:
projects=SHOWDIR(prefs.1)
IF WORDS(projects)=1 THEN
  DO
    arcdir=makepath(prefs.3,'OUT/'projects)
    CALL MAKEDIR(arcdir)
    workdir=makepath(prefs.1,projects)
    CALL MAKEDIR(workdir)
    CALL PRAGMA('D',workdir)
  END
ELSE IF projects~='' THEN CALL newarea()

IF EXISTS('.ID') THEN CALL read_ID()
ELSE IF projects~='' THEN CALL openarc()

RESTART:
IF mode=0 THEN w.title='Notices and Information'
IF mode=1 THEN w.title='Private EMail Messages'
IF mode=2 THEN w.title='New Conferences Messages'
IF mode=3 THEN w.title='Public Message Conferences'
IF mode=4 THEN w.title='New File Descriptions'
IF mode=5 THEN w.title='Public File Libraries'
p.title=''
w.title=RIGHT(bbsname w.title,75)
CALL OpenWindow(QUICKHOST,,9,w.cols,w.rows-8,w.idcmp,w.flags,w.title)
CALL SetFont(QUICKHOST,'topaz.font',8)

CALL AddMenu(QUICKHOST,'Project       ')
CALL AddItem(QUICKHOST,' New','NEW')
CALL AddItem(QUICKHOST,' Open ...','OPEN','O')
CALL AddItem(QUICKHOST,' Archive','ARC','A')
CALL AddItem(QUICKHOST,' Edit','EDIT','E')
CALL AddItem(QUICKHOST,' Info','INFO','I')
CALL AddItem(QUICKHOST,' About','ABOUT')
CALL AddItem(QUICKHOST,' Quit','CLOSEWINDOW','Q')

CALL AddMenu(QUICKHOST,'Movement            ')
CALL AddItem(QUICKHOST,' Line Forward','LINE UP','.')
CALL AddItem(QUICKHOST,' Line Back','LINE BACK',',')
CALL AddItem(QUICKHOST,' Page Forward','PAGE UP','F')
CALL AddItem(QUICKHOST,' Page Back','PAGE BACK','B')
CALL AddItem(QUICKHOST,' Zip to Beg/End','PAGE ZIP','Z')

CALL AddMenu(QUICKHOST,'Commands      ')
CALL AddItem(QUICKHOST,' Mode ...','CONTINUE')
CALL AddSubItem(QUICKHOST,'   Notices','MODE 0','1',62)
CALL AddSubItem(QUICKHOST,'   EMail','MODE 1','2',61)
CALL AddSubItem(QUICKHOST,'   Messages','MODE 2','3',59)
CALL AddSubItem(QUICKHOST,'   Conferences','MODE 3','4',55)
CALL AddSubItem(QUICKHOST,'   FileList','MODE 4','5',47)
CALL AddSubItem(QUICKHOST,'   Libraries','MODE 5','6',31)
CALL AddItem(QUICKHOST,' Print','PRINT','[')
CALL AddItem(QUICKHOST,' Search','SEARCH','S')
IF mode=1 & level>sysoplevel THEN CALL AddItem(QUICKHOST,' User','USER','')
IF mode<2 THEN
  DO
    CALL AddItem(QUICKHOST,' Delete','KILL','K')
    CALL AddItem(QUICKHOST,' Save As','SAVEAS','S')
  END
IF mode<3 THEN
  DO
    CALL AddItem(QUICKHOST,' Previous','NEXT BACK','P')
    CALL AddItem(QUICKHOST,' Next','NEXT FORWARD','N')
  END
IF mode=1 | mode=2 | mode=3 THEN
    CALL AddItem(QUICKHOST,' Write','WRITE','W')
IF mode=4 THEN
  DO
    CALL AddItem(QUICKHOST,' Download','DOWNLOAD','D')
    IF level>=sysoplevel THEN
      DO
        CALL AddItem(QUICKHOST,' Kill','DELETE')
        CALL AddItem(QUICKHOST,' Change','CHANGE')
      END
    IF WORDS(SHOWDIR(makepath(workdir,'EmailFiles')'/'name))>0 THEN
      CALL AddItem(QUICKHOST,' EmailFiles','EFILES','E')
  END
IF mode=5 THEN CALL AddItem(QUICKHOST,' Upload','UPLOAD','U')
CALL SetItem(QUICKHOST,2,0,mode,'ON')

CALL AddMenu(QUICKHOST,'Preferences     ')
CALL AddItem(QUICKHOST,' Set ...','CONFIG SET','/')
CALL AddItem(QUICKHOST,' Load ...','CONFIG LOAD','L')
CALL AddItem(QUICKHOST,' Save ...','CONFIG SAVE',';')
CALL AddItem(QUICKHOST,' Colors ...','COLORS','C')

IF mode<3 THEN
  DO
    CALL AddGadget(QUICKHOST,110,14,2,' Previous ','NEXT BACK')
    CALL AddGadget(QUICKHOST,210,14,3,' Next ','NEXT FORWARD')
  END
IF mode=1 | mode=2 | mode=3 THEN
  CALL AddGadget(QUICKHOST,20,14,1,' Write ','WRITE')
IF mode=4 THEN
  DO
    CALL AddGadget(QUICKHOST,14,14,1,' Download ','DOWNLOAD')
    IF level>=sysoplevel THEN
      CALL AddGadget(QUICKHOST,108,14,2,' Kill ','DELETE')
    IF WORDS(SHOWDIR(makepath(workdir,'EmailFiles')'/'name))>0 THEN
      CALL AddGadget(QUICKHOST,170,14,3,' EmailFiles ','EFILES')
  END
IF mode=5 THEN CALL AddGadget(QUICKHOST,20,14,1,' Upload ','UPLOAD')
CALL AddGadget(QUICKHOST,w.cols-130,14,4,' Page Forward ','PAGE UP')
CALL AddGadget(QUICKHOST,w.cols-235,14,5,' Page Back ','PAGE BACK')
CALL AddGadget(QUICKHOST,280,14,6,CENTER(modetext.mode,13),'NEW_MODE')
CALL SetGadget(QUICKHOST,6,'ON')
CALL tofront(QUICKHOST)
CALL boxes()
CALL SetAPen(QUICKHOST,3)
CALL Flood(QUICKHOST,1,5,15)
CALL read_QUICK()
IF mode=0 | mode=2 THEN CALL do_marked()


                             /* Wait Loop */
LOOP:
CALL SCREENTOFRONT(QUICKSCREEN)
CALL SetAPen(QUICKHOST,1)
DO FOREVER
  t=waitpkt(QUICKPORT)
  DO ff=1
    p=GETPKT(QUICKPORT)
    IF c2d(p)=0 THEN LEAVE ff
    command=GETARG(p)
    PARSE VAR command arg1' 'arg2' 'arg3' 'arg4
    t=REPLY(p,0)
    SELECT
      WHEN arg1='FRONT'        THEN CALL tofront(QUICKHOST)
      WHEN arg1='NEW'          THEN CALL new()
      WHEN arg1='OPEN'         THEN CALL openarc()
      WHEN arg1='EDIT'         THEN CALL edit_request()
      WHEN arg1='INFO'         THEN CALL info()
      WHEN arg1='ABOUT'        THEN CALL Request(200,45,abouttext,,,,QUICKSCREEN)
      WHEN arg1='CLOSEWINDOW'  THEN CALL ALL_DONE('')
      WHEN arg1='LINE'         THEN CALL line(arg2)
      WHEN arg1='PAGE'         THEN CALL page(arg2)
      WHEN arg1='PRINT'        THEN CALL print()
      WHEN arg1='WRITE'        THEN CALL write()
      WHEN arg1='SEARCH'       THEN CALL searchreq()
      WHEN arg1='NEXT'         THEN CALL nextitem(arg2)
      WHEN arg1='UPLOAD'       THEN CALL upload()
      WHEN arg1='DOWNLOAD'     THEN CALL do_action('Download')
      WHEN arg1='DELETE'       THEN CALL do_action('Delete')
      WHEN arg1='EFILES'       THEN CALL efiles()
      WHEN arg1='USER'         THEN CALL user()
      WHEN arg1='KILL'         THEN CALL kill()
      WHEN arg1='SAVEAS'       THEN CALL saveas()
      WHEN arg1='CHANGE'       THEN CALL do_action('Change')
      WHEN arg1='ARC'          THEN CALL archive()
      WHEN arg1='MODE'         THEN DO;mode=arg2;CALL whichmode(0);END
      WHEN arg1='CONFIG'       THEN CALL config(arg2)
      WHEN arg1='COLORS'       THEN
        /* INTERPRET */ ADDRESS AREXX Palette.rexx QUICKSCREEN 4 QUICKPORT 
      WHEN arg1='NEW_COLORS'   THEN prefs.0=new_colors()
      WHEN arg1='NEW_MODE'     THEN CALL whichmode(1)
      WHEN arg1='MOUSEBUTTONS' THEN NOP
      WHEN arg1='SELECTUP'     THEN NOP
      WHEN arg1='CONTINUE'     THEN NOP
      OTHERWISE CALL Request(,100,command 'is not finished...',,,,QUICKSCREEN)
    END
  END
END
CALL ALL_DONE('This error should never occur.')
EXIT


/* Functions */

ALL_DONE:
PARSE ARG air
IF air~='' THEN
  DO
    CALL usermsg(air)
    CALL waiting()
  END
IF WORDS(SHOWDIR(arcdir,'F'))>0 THEN
  DO
    temp='There are files ready to be archived for sending... Do it now?'
    IF Request(,36,temp,,' NOW ',' LATER ',QUICKSCREEN)='OKAY' THEN
      CALL archive()
  END
IF SHOW('P','QUICKHOST') THEN CALL Stop('QUICKHOST')
CALL CloseScreen(QUICKSCREEN)
EXIT


do_marked:
IF ~EXISTS('Marked') THEN RETURN
x=OPEN(f,'Marked','R')
IF x=0 THEN RETURN
t=''
t=STRIP(LEFT(READLN(f),74))
DO i=2 TO 16
  line=READLN(f)
  IF EOF(f) THEN LEAVE i
  t=t'\'STRIP(LEFT(line,74))
END
IF i>16 THEN t=t'\--- more ---'
CALL CLOSE(f)
IF Request(,,t,,' Don''t Tell Me Again ',' Thanks ',QUICKSCREEN)='OKAY' THEN
  CALL DELETE('Marked')
RETURN


efiles:
ef=SHOWDIR('EmailFiles/'name)
IF ef="" THEN
  DO
    CALL usermsg('No emailfiles were found!')
    CALL waiting()
    RETURN
  END
ename=GetFile(150,16,makepath(workdir,'EmailFiles/'name),,'Select File To Move',QUICKSCREEN)
IF ename='' | ~EXISTS(ename) THEN RETURN
sname=SUBSTR(ename,LASTPOS('/',ename)+1)
dname=GetFile(150,16,prefs.2,sname,'Select Destination',QUICKSCREEN)
IF dname='' THEN RETURN
IF WORD(STATEF(dname),1)='FILE' THEN
  DO
    IF Request(,36,dname 'exists! Replace it?',,' REPLACE ',' CANCEL ',QUICKSCREEN)='OKAY' THEN
      CALL DELETE(dname)
    ELSE RETURN
  END
ELSE IF ~EXISTS(dname) THEN
  DO
    dd=LASTPOS('/',dname)-1
    IF dd<1 THEN dd=LASTPOS(':',dname)
    ddir=LEFT(dname,dd)
    IF WORD(STATEF(ddir),1)~='DIR' THEN
      DO
        CALL usermsg(ddir 'does not exist!')
        CALL waiting()
        RETURN
      END
  END
ADDRESS COMMAND 'copy' ename dname
CALL DELETE(ename)
RETURN


archive:
IF WORDS(SHOWDIR(arcdir))=0 THEN
  DO
    CALL usermsg('Nothing To Archive!')
    CALL waiting()
    RETURN
  END
IF arcdir=makepath(prefs.3,'OUT') & WORDS(SHOWDIR(arcdir,'D'))>0 THEN
  DO
    dirs=SHOWDIR(arcdir,'D')
    temp='Select BBS by number...\'
    iii=0
    DO ii=1 TO WORDS(dirs)
      IF WORDS(SHOWDIR(makepath(arcdir,WORD(dirs,ii))))=0 THEN ITERATE ii
      iii=iii+1
      IF iii=16 & WORDS(dirs)>ii THEN
        DO
          temp=temp'\ - Too many to fit! -'
          LEAVE ii
        END
      temp=temp'\'RIGHT(ii,2)'.' WORD(dirs,ii)
    END
    IF iii=0 THEN
      DO
        CALL usermsg('Nothing To Archive!')
        CALL waiting()
        RETURN
      END
    temp=Request(150,,temp,'',,' CANCEL ',QUICKSCREEN)
    IF temp='' | ~DATATYPE(temp,'W') THEN RETURN
    IF temp<1 | temp>WORDS(dirs) THEN RETURN
    arcdir=makepath(arcdir,WORD(dirs,temp))
    CALL archive()
    RETURN
  END
arcname=GetFile(150,16,prefs.3,'To'bbsnym'.lha','Name This QUICKIN Archive',QUICKSCREEN)
IF arcname='' THEN RETURN
IF EXISTS(arcname) THEN
  DO
    IF Request(,36,arcname 'exists. Delete the old one?',,' DELETE ',' CANCEL ',QUICKSCREEN)='OKAY' THEN
      CALL DELETE(arcname)
    ELSE RETURN
  END
CALL usermsg('Building Archive' arcname'...')
ADDRESS COMMAND 'CD' arcdir LF 'lha -2arq m' arcname '#?'
CALL PostMsg()
RETURN


editor:
PARSE ARG tname 
IF prefs.4='' THEN prefs.4='Ed'
clicomm='CD' arcdir LF prefs.4 tname
IF ksversion>=37 & UPPER(prefs.4)='ED' THEN
  clicomm=clicomm' WINDOW=CON:/2/'ScreenCols()'/'Screenrows()-2'/'QUICK'/C'
CALL ScreenToBack(QUICKSCREEN)
ADDRESS COMMAND clicomm
CALL ScreenToFront(QUICKSCREEN)
RETURN


user:
uc='User_Changes'
RETURN


saveas:
temp=SUBSTR(tfile,LASTPOS('/',tfile)+1)
x=GetFile(150,16,prefs.2,temp,'Copy to where?',QUICKSCREEN)
IF x='' THEN RETURN
ADDRESS COMMAND 'copy' makepath(workdir,tfile) x 'CLONE'

kill:
IF mode=0 THEN
  DO
    IF Request(,36,'Delete' tfile'?',,' DELETE ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN
      RETURN
    CALL DELETE(makepath(workdir,tfile))
    noticecount=noticecount+1
    CALL CloseWindow(QUICKHOST,'CONTINUE')
    SIGNAL RESTART
  END
IF Request(,36,'Delete Email' email.emailcount'?',,' DELETE ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN
  RETURN
IF DELETE(makepath(workdir,'Email/'name'/'email.emailcount)) THEN
  DO
    CALL usermsg('Deleting' email.emailcount'...')
    email.emailcount=''
    CALL nextitem('FORWARD')
  END
RETURN


write:
IF name='' THEN RETURN
IF mode=1 THEN
  DO
    IF DATATYPE(email.0,'W') & email.emailcount~='' THEN
      DO
        IF LEFT(email.emailcount,3)='MSG' THEN
          DO
            IF Request(,100,'Return this message to the conferences?',,' YES ',' NO ',QUICKSCREEN)~='OKAY' THEN
              RETURN
            CALL usermsg('Returning' email.emailcount 'to message conferences...')
            CALL MAKEDIR(arcdir'/MSG')
            com='COPY' makepath(workdir,'Email/'name'/'email.emailcount)
            com=com makepath(arcdir,'MSG')
            mADDRESS COMMAND com
            CALL DELETE(makepath(workdir,'Email/'name'/'email.emailcount))
            CALL PostMsg()
            IF Request(,100,'Edit this message?',,' YES ',' NO ',QUICKSCREEN)~='OKAY' THEN
              RETURN
            CALL editor('MSG/'email.emailcount)
            RETURN
          END
        dot=LASTPOS('.',email.emailcount)
        IF dot<1 THEN
          DO
            subj=''
            toname=''
            tonamecity=''
          END
        ELSE
          DO
            toname=LEFT(email.emailcount,dot-1)
            tonamecity=SUBSTR(text.2,8)
            subj=STRIP(LEFT(SUBSTR(text.4,8),66))
          END
      END
    ELSE
      DO
        subj=''
        toname=''
        tonamecity=''
      END
    hdr.=''
    hdr.1=' Mail: QUICK'
    hdr.2=' From: 'name '-' city
    temp=hdr.1'\'hdr.2'\   To:' tonamecity
    x=Request(,90,temp,toname,,,QUICKSCREEN)
    IF x='' THEN RETURN
    x=SPACE(UPPER(cleanstring(x)),1,'_')
    IF x='SYSOP' & sysop~='' THEN x=sysop
    IF x~=toname THEN
      DO
        toname=check_alias(x)
        tonamecity=toname
      END
    IF tonamecity='' THEN tonamecity=toname
    hdr.3='   To: 'tonamecity
    temp=hdr.1'\'hdr.2'\'hdr.3'\ Subj:'
    subj=Request(,90,temp,subj,,,QUICKSCREEN)
    IF subj='' THEN subj='?'
    hdr.4=' Subj: 'subj
    hdr.5=' Date: 'DATE('W') DATE() TIME('C')
    hdr.6=RIGHT(WORD(SOURCELINE(1),4)'=====',74,'=')
    temp=hdr.1'\'hdr.2'\'hdr.3'\'TRIM(LEFT(hdr.4,74))'\'hdr.5'\'hdr.6
    IF Request(,90,temp,,' WRITE ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN
      RETURN
    msgnum=get_number()
    IF write_hdr(6) THEN RETURN
    IF tonamecity~=toname THEN
      DO
        IF Request(,90,'Include Original Email From' toname '?',,' INCLUDE ',' LEAVE OUT ',QUICKSCREEN)='OKAY' THEN
          DO
            x=OPEN(f,txtfile,'W')
            IF x=0 THEN
              DO
                CALL usermsg(txtfile 'failed to open for writing!')
                CALL waiting()
              END
            ELSE
              DO i=7 TO text.0
                CALL WRITELN(f,'>'text.i)
              END
            CALL CLOSE(f)
          END
      END
    CALL INSERTSIG(txtfile) /* Check for a signature to add S.Griffiths 3/8/94 */
    CALL editor(txtfile)
    IF Request(,90,'Attach a file to this letter?',,' YES ',' NO ',QUICKSCREEN)~='OKAY' THEN
      RETURN
    mailfile=GetFile(150,16,prefs.3,,'Select file to attach.',QUICKSCREEN)
    IF mailfile='' | ~EXISTS(mailfile) THEN RETURN
    CALL MAKEDIR(arcdir'/Files')
    ADDRESS COMMAND 'copy' mailfile arcdir'/Files'
    hdr.1=hdr.1'    FILE:' getfilename(mailfile)
    CALL write_hdr(6)
    CALL usermsg('Mail will be sent to' toname'.')
    CALL waiting()
    RETURN
  END
IF mode=2 THEN
  DO
    n=Request(350,100,'Reply To What Message NUMBER?','',,' CANCEL ',QUICKSCREEN)
    IF ~DATATYPE(n,'W') THEN RETURN
    ln=search(1 '  Msg:' n)
    IF ln=0 THEN
      DO
        CALL usermsg('Msg:' n 'not found in the' WORD(mess.0.1,messcount) 'Conference!')
        CALL waiting()
        RETURN
      END
    temp=text.ln
    DO lnn=ln+1 TO ln+12
      temp=temp'\'TRIM(LEFT(TRANSLATE(text.lnn,' ','\'),74))
    END
    IF Request(,,temp,,' REPLY ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN RETURN
    lnn=ln+1
    toname=WORD(text.lnn,2)
    lnn=ln+3
    subj=STRIP(LEFT(SUBSTR(text.lnn,8),66))
    conf=WORD(mess.0.1,messcount)
  END
IF mode=3 THEN
  DO
    confnum=''
    DO WHILE ~DATATYPE(confnum,'W')
      confnum=Request(380,105,'Select Message\Conference by NUMBER','',,' CANCEL ',QUICKSCREEN)
      IF confnum='' THEN RETURN
    END
    confnum=' 'confnum%1'.'
    linenum=search(1 confnum)
    IF linenum=0 THEN
      DO
        CALL usermsg('Unknown Message Conference number!')
        CALL waiting()
        RETURN
      END
    wordnum=FIND(text.linenum,confnum)+1
    conf=WORD(text.linenum,wordnum)
    toname=Request(200,100,'Post This Message To Who?','',,,QUICKSCREEN)
    toname=SPACE(UPPER(cleanstring(toname)),1,'_')
    IF toname='' THEN toname='ALL'
    IF toname='SYSOP' & sysop~='' THEN toname=sysop
    toname=check_alias(toname)
    subj=''
  END
hdr.=''
hdr.1='  Msg: QUICK'
hdr.2=' From: 'name '-' city
hdr.3='   To: 'toname
IF mode=2 THEN hdr.3=hdr.3'   Msg' n
temp=hdr.1'\'hdr.2'\'hdr.3'\ Subj:'
subj=Request(,100,temp,subj,,,QUICKSCREEN)
IF subj='' THEN subj='?'
hdr.4=' Subj:' subj
hdr.5=' Date:' DATE('W') DATE() TIME('C')
hdr.5=LEFT(hdr.5,39) 'Conference:' conf
hdr.6=RIGHT(WORD(SOURCELINE(1),4)'=====',74,'=')
temp=hdr.1'\'hdr.2'\'hdr.3'\'TRIM(LEFT(hdr.4,74))'\'TRIM(LEFT(hdr.5,74))'\'hdr.6
IF Request(,10,temp,,' WRITE ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN
  RETURN
msgnum=get_number()
IF write_hdr(6) THEN RETURN
IF mode=1 & tonamecity~=toname THEN
  DO
    IF Request(,100,'Include Original Email From' toname '?',,' INCLUDE ',' LEAVE OUT ',QUICKSCREEN)='OKAY' THEN
      DO
        x=OPEN(f,txtfile,'W')
        IF x=0 THEN
          DO
            CALL usermsg(txtfile 'failed to open for writing!')
            CALL waiting()
          END
        ELSE
          DO i=7 TO text.0
            CALL WRITELN(f,'>'text.i)
          END
        CALL CLOSE(f)
      END
  END
CALL INSERTSIG(txtfile)  /* Check for signature to add S.Griffiths 3/8/94 */
CALL editor(txtfile)
RETURN


check_alias:
PARSE ARG ali .
IF ~DATATYPE(Friends.0,'W') THEN RETURN ali
DO ii=1 TO Friends.0
  IF UPPER(WORD(Friends.ii,1))=UPPER(ali) THEN RETURN WORD(Friends.ii,2)
END
RETURN ali


write_hdr:
ARG hdrlines 
x=OPEN(f,hdrfile,'W')
IF x=0 THEN
  DO
    CALL usermsg(hdrfile 'failed to open for writing!')
    CALL waiting()
    RETURN 1
  END
DO i=1 TO hdrlines
  CALL WRITELN(f,hdr.i)
END
CALL CLOSE(f)
RETURN 0


get_number:
DO i=.001 TO .999 BY .001
  hdrfile=makepath(arcdir,RIGHT(i,3)'.HDR')
  txtfile=makepath(arcdir,RIGHT(i,3)'.TXT')
  IF ~EXISTS(hdrfile) THEN RETURN i
END
RETURN


cleanstring:
PARSE ARG cstr
bot=TRIM(XRANGE(,' '))
top=XRANGE('7B'x)
cstr=STRIP(COMPRESS(cstr,bot||top))
RETURN cstr


upload:
IF name='' THEN RETURN
fullfile=GetFile(150,16,prefs.3,,'Select file to upload.',QUICKSCREEN)
IF fullfile='' | ~EXISTS(fullfile) THEN RETURN
finfo=STATEF(fullfile)
namelen=22-LENGTH(WORD(finfo,2))
ufile=getfilename(fullfile)
ufile=COMPRESS(cleanstring(ufile),'()#?*;=')  /* remove DOS reserved chars */
DO test=1
  IF LENGTH(ufile)<=namelen THEN LEAVE test
  temp='Your filename,' ufile', is' LENGTH(ufile)-namelen 'character'
  IF (LENGTH(ufile)-namelen)>1 THEN temp=temp's'
  temp=temp 'too long!'
  temp=temp'\Please enter new filename:'
  ufile=Request(100,120,temp,ufile,,' CANCEL ',QUICKSCREEN)
  ufile=COMPRESS(cleanstring(ufile),'()#?*;=')
  IF ufile='' THEN RETURN
END
libnum=''
DO WHILE ~DATATYPE(libnum,'W')
  libnum=Request(380,115,'Select Library by NUMBER','',,' CANCEL ',QUICKSCREEN)
  IF libnum='' THEN RETURN
END
libnum=' 'libnum%1'.'
linenum=search(1 libnum)
IF linenum=0 THEN
  DO
    CALL usermsg('Unknown library number!')
    CALL waiting()
    RETURN
  END
wordnum=FIND(text.linenum,libnum)+1
filedir=WORD(text.linenum,wordnum)
temp='FileName:' ufile
temp=temp'\ Library:' filedir'\'
temp=temp'\Please Enter Keywords'
temp=temp'\(or a condensed description)'
temp=temp'\32 characters MAXIMUM...........|'
keywords=''
DO WHILE keywords=''
  keywords=Request(,36,temp,'',,,QUICKSCREEN)
END
keywords=STRIP(LEFT(cleanstring(keywords),32))
bytes=WORD(STATEF(fullfile),2)
hdr.=''
hdr.1='File: QUICK   KeyWords:' keywords
hdr.2='Name: 'LEFT(ufile,27)' Size: 'bytes' bytes   Downloads: 0'
hdr.3='From: 'LEFT(name,27)' Date: 'DATE() TIME('C')'  Lib: 'filedir
hdr.4=RIGHT(WORD(SOURCELINE(1),4)'=====',74,'=')
msgnum=get_number()
temp=hdr.1'\'hdr.2'\'TRIM(LEFT(hdr.3,74))'\'hdr.4
IF Request(,100,'Add a longer file description?',,' YES ',' NO ',QUICKSCREEN)='OKAY' THEN
  DO
    CALL editor(txtfile)
    CALL DELAY(50)
    x=OPEN(f,txtfile,'R')
    IF x~=0 THEN
      DO
        DO p=1 TO 9
          line=TRIM(LEFT(READLN(f),74))
          IF EOF(f) THEN LEAVE p
          temp=temp'\'line
        END
        CALL CLOSE(f)
      END
  END
IF Request(,36,temp,,' UPLOAD ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN
  DO
    CALL DELETE(txtfile)
    CALL DELETE(hdrfile)
    RETURN
  END
IF write_hdr(4) THEN RETURN
udir=makepath(arcdir,'Files')
CALL MAKEDIR(udir)
ADDRESS COMMAND 'copy' fullfile udir'/'ufile
temp='If' ufile 'replaces\a previous upload of yours,\'
temp=temp'please enter the file number\of that previous upload.'
n=Request(150,36,temp,'',,' CANCEL ',QUICKSCREEN)
IF ~DATATYPE(n,'W') THEN RETURN
action='Delete'

add_number:
fname=arcdir'/File_Requests'
IF action='Delete' THEN fname=arcdir'/File_Deletes'
IF EXISTS(fname) THEN x=OPEN(f,fname,'A')
ELSE x=OPEN(f,fname,'W')
IF x=0 THEN 
  DO
    CALL usermsg(fname 'failed to open for writing!')
    CALL waiting()
    RETURN
  END
CALL WRITELN(f,n)
CALL CLOSE(f)
CALL usermsg('File number' n 'added to' action 'request list.')
CALL waiting()
RETURN


do_action:
PARSE ARG action 
IF name='' THEN RETURN
n=Request(150,100,'Enter File To' action 'by NUMBER','',,' CANCEL ',QUICKSCREEN)
IF n='' THEN RETURN
IF ~DATATYPE(n,'W') THEN
  DO
    CALL usermsg(action 'requests MUST be made by file NUMBER only!')
    RETURN
  END
ln=search(1 'FILE:' n)
IF ln=0 THEN
  temp='File:' n 'not found in current list\Include in the' action 'Request anyway?'
ELSE
  DO
    lnn=ln+1
    lnnn=ln+2
    temp=text.ln'\'text.lnn'\'TRIM(LEFT(text.lnnn,74))
  END
IF Request(,100,temp,,' 'UPPER(action)' ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN RETURN
IF action~='Change' THEN
  DO
    CALL add_number()
    RETURN
  END
IF sysoplevel>level THEN RETURN
temp3=' ^ indicates no change from the original data in that position.\\'
IF ln>0 THEN temp3=temp'\\'temp3
hdr.=''
hdr.1='File:' n'   KeyWords: ^'
hdr.2='Name: 'LEFT('^',27)' Size: ^ bytes   Downloads: ^'
hdr.3='From: 'LEFT('^',27)' Date: ^ ^ ^ ^  Lib: ^'
hdr.4=RIGHT(WORD(SOURCELINE(1),4)'=====',74,'=')
temp=''
IF ln>0 THEN PARSE VAR text.ln . 'KeyWords: 'temp
x=Request(,20,'Change KeyWords',temp,,' NO ',QUICKSCREEN)
IF x~='' & x~=temp THEN hdr.1='File:' n'   KeyWords:' STRIP(LEFT(x,32))
temp=''
IF ln>0 THEN PARSE VAR text.lnn 'Name: 'temp .
DO test=1
  x=Request(,20,temp3||hdr.1'\\Change FileName',temp,,' NO ',QUICKSCREEN)
  IF x=temp | LENGTH(x)<17 THEN LEAVE test
  CALL usermsg('Your filename is' LENGTH(x)-16 'characters too long!')
  CALL waiting()
END
IF x~='' & x~=temp THEN hdr.2='Name: 'LEFT(x,27)' Size: ^ bytes   Downloads: ^'
temp=''
IF ln>0 THEN PARSE VAR text.lnnn 'From: 'temp .
x=Request(,20,temp3||hdr.1'\'hdr.2'\\Change Uploader',temp,,' NO ',QUICKSCREEN)
IF x~='' & x~=temp THEN hdr.3='From: 'LEFT(x,27)' Date: 'DATE() TIME('C')'  Lib: ^'
temp=''
IF ln>0 THEN PARSE VAR text.lnnn . 'Lib: 'temp
PARSE VAR hdr.3 'From:' temp2 .
x=Request(,20,temp3||hdr.1'\'hdr.2'\'TRIM(LEFT(hdr.3,74))'\\Change Library',temp,,' NO ',QUICKSCREEN)
IF x~='' & x~=temp THEN
  hdr.3='From: 'LEFT(temp2,27)' Date: 'DATE() TIME('C')'  Lib:' x
msgnum=get_number()
IF Request(,20,'Edit the longer file description?',,' YES ',' NO ',QUICKSCREEN)='OKAY' THEN
  DO
    IF ln>0 THEN
      DO
        ii=ln+5
        IF text.ii~='=====' & text.0>=ii THEN
          DO
            x=OPEN(f,txtfile,'W')
            IF x~=0 THEN
              DO ii=ln+4 WHILE text.ii~='=====' & text.0>=ii
                CALL WRITELN(f,text.ii)
              END
            CALL CLOSE(f)
            CALL DELAY(28)
          END
      END
    CALL editor(txtfile)
  END
temp=temp3||hdr.1'\'hdr.2'\'TRIM(LEFT(hdr.3,74))'\'hdr.4
IF Request(,20,temp,,' CHANGE ',' CANCEL ',QUICKSCREEN)~='OKAY' THEN
  DO
    CALL DELETE(txtfile)
    CALL DELETE(hdrfile)
    RETURN
  END
CALL write_hdr(4)
RETURN


searchreq:
x=Request(350,100,'Search Phrase?','',,' CANCEL ',QUICKSCREEN)
IF x='' THEN RETURN
num=search(0 x)
IF num=0 THEN
  DO
    CALL usermsg(x 'was not found!')
    CALL waiting()
  END
ELSE CALL show_QUICK(num)
RETURN


search:
ARG flag' 'searcharg
DO si=1 TO text.0
  IF POS(searcharg,UPPER(text.si))>0 THEN
    DO
      IF flag & LEFT(text.si,1)='>' THEN ITERATE si
      RETURN si
    END
END
RETURN 0


print:
temp=modetext.mode
IF mode=1 THEN temp=email.emailcount 'Email'
IF mode=2 THEN temp=WORD(mess.0.1,messcount) 'Conference'
pdev=Request(150,100,'Print' temp 'To:','PRT:',,' CANCEL ',QUICKSCREEN)
IF pdev='' THEN RETURN
x=OPEN(p,pdev,'W')
IF x=0 THEN
  DO
    CALL usermsg(pdev 'failed to open for printing!')
    CALL waiting()
    RETURN
  END
DO i=1 TO text.0
  CALL WRITELN(p,text.i)
END
CALL CLOSE(p)
RETURN


newarea:
x=GetFile(150,36,prefs.1,,'Select BBS...',QUICKSCREEN)
IF x='' | ~EXISTS(x) THEN RETURN
finfo=STATEF(x)
IF WORD(finfo,1)='FILE' THEN x=getdir(x)
CALL MAKEDIR(x)
IF ~EXISTS(x) THEN RETURN
workdir=x
CALL PRAGMA('D',workdir)
arcdir=makepath(prefs.3,'OUT/'getfilename(workdir))
CALL MAKEDIR(arcdir)
CALL read_ID()
RETURN


new:
IF Request(150,100,'Clear this area or change areas?',,' CLEAR ',' NEW BBS ',QUICKSCREEN)='OKAY' THEN
  DO
    CALL usermsg('Clearing work area...')
    CALL clear()
    CALL read_ID()
  END
ELSE CALL newarea()
CALL CloseWindow(QUICKHOST,'CONTINUE')
SIGNAL RESTART
RETURN


reset_vars:
friends.=''
text.=''
mess.=''
name=''
city=''
bbsname=''
sysop=''
keys=''
mode=0
notices=''
level=0
sysoplevel=99
arcdate=''
RETURN


clear:
CALL reset_vars()
emdir=''
efdir=''
wemdir=''
wefdir=''
notices=''
ndir=''
IF EXISTS('Notices') THEN ndir='Notices/#? Notices'
IF EXISTS('Email') THEN
  DO
    emdir=SHOWDIR('Email')
    emdir=makepath('Email',WORD(emdir,1))
    wemdir=emdir'/#?'
    emdir=emdir 'Email'
  END
IF EXISTS('EmailFiles') THEN
  DO
    efdir=SHOWDIR('EmailFiles')
    efdir=makepath('EmailFiles',WORD(efdir,1))
    wefdir=efdir'/#?'
    efdir=efdir 'EmailFiles'
  END
IF ndir~='' | wemdir~='' | wefdir~='' | emdir~='' | efdir~='' THEN
  DO
    comm='CD' workdir LF 'delete' ndir wemdir wefdir emdir efdir 'QUIET'
    ADDRESS COMMAND comm
  END
CALL DELETE('Messages')
IF EXISTS('Mess1') THEN
  DO i=1 WHILE EXISTS('Mess'i)
    CALL DELETE('Mess'i)
  END
CALL DELETE('FileList')
IF EXISTS('List1') THEN
  DO i=1 WHILE EXISTS('List'i)
    CALL DELETE('List'i)
  END
RETURN


openarc:
arcfile=GetFile(150,16,prefs.2,'QUICK_','Select archive to process.',QUICKSCREEN)
IF arcfile='' THEN RETURN
CALL usermsg('Processing archive' arcfile' ...')
newname=name
newbbs=bbsname
comm='CD RAM:' LF 'lha -q x' arcfile '.ID'
ADDRESS COMMAND comm
x=OPEN(f,'RAM:.ID','R')
IF x=0 THEN
  DO
    CALL usermsg(arcfile 'is not a BBBBS QUICKOUT archive!')
    CALL waiting()
    RETURN
  END
newname=SUBSTR(READLN(f),8)
CALL READLN(f)
newbbs=SUBSTR(READLN(f),8)
CALL CLOSE(f)
newbbsnym=STRIP(LEFT(COMPRESS(newbbs," '/:;."),12))
workdir=makepath(prefs.1,newbbsnym)
CALL MAKEDIR(workdir)
CALL PRAGMA('D',workdir)
CALL DELETE('RAM:.ID')
aflag=1
IF EXISTS('Messages') & newname=name & newbbs=bbsname THEN
  DO
    IF Request(200,100,'Append Old Messages To New?',,' APPEND ',' CLEAR ',QUICKSCREEN)='OKAY' THEN
      DO
        CALL RENAME('Messages','Mess0')
        aflag=0
      END
    ELSE
      DO
        CALL DELETE('Messages')
        mess.=''
      END
  END
IF EXISTS('FileList') & newname=name & newbbs=bbsname THEN
  DO
    IF Request(200,100,'Append Old FileList To New?',,' APPEND ',' CLEAR ',QUICKSCREEN)='OKAY' THEN
      DO
        CALL RENAME('FileList','List0')
        aflag=0
      END
    ELSE CALL DELETE('FileList')
  END
CALL usermsg('Processing archive' arcfile' ...')
IF aflag THEN CALL clear()
comm='CD' workdir LF 'lha -q x' arcfile
ADDRESS COMMAND comm
IF EXISTS('Mess0') THEN
  DO
    mess.=''
    IF EXISTS('Messages') THEN
      DO
        CALL RENAME('Mess0','Mess1')
        CALL RENAME('Messages','Mess2')
        ADDRESS COMMAND 'CD' workdir||LF'JOIN Mess2 Mess1 AS Mess0'
        IF RC=0 THEN
          DO
            CALL DELETE('Mess1')
            CALL DELETE('Mess2')
          END
      END
    CALL RENAME('Mess0','Messages')
  END
IF EXISTS('List0') THEN
  DO
    IF EXISTS('FileList') THEN
      DO
        CALL RENAME('List0','List1')
        CALL RENAME('FileList','List2')
      END
  END
IF EXISTS('List1') THEN
  DO
    alist=''
    DO i=1 WHILE EXISTS('List'i)
      alist=STRIP(alist 'List'i)
    END
    ADDRESS COMMAND 'CD' workdir||LF'JOIN' alist 'AS List0' 
    IF RC=0 THEN
      DO i=1 WHILE EXISTS('List'i)
        CALL DELETE('List'i)
      END
  END
CALL RENAME('List0','FileList')
CALL read_ID()
CALL load_notices()
CALL sort_email()
dups.=''
dups.0=email.0
indx=1
DO i=1 TO email.0-1
  IF LEFT(email.i,3)='MSG' THEN email.i=''
  n=LASTPOS('.',email.i)
  IF n=0 THEN email.i=''
  IF email.i='' THEN ITERATE i
  thismail=LEFT(email.i,n)
  DO j=i+1 TO email.0
    n=LASTPOS('.',email.j)
    IF n=0 THEN email.j=''
    IF email.j='' THEN ITERATE i
    thatmail=LEFT(email.j,n)
    IF thatmail=thismail THEN
      DO
        dups.i=email.j dups.i
        email.j=''
      END
  END
END
DO i=1 TO dups.0
  IF dups.i='' THEN ITERATE i
  dups.i=email.i dups.i
  m=''
  DO j=1 TO WORDS(dups.i)
    x=OPEN(f,'Email/'name'/'WORD(dups.i,j),'R')
    IF x=0 THEN
      DO
        CALL usermsg('Email/'name'/'WORD(dups.i,j) 'failed to open for reading!')
        CALL waiting()
        RETURN
      END
    m=m||READCH(f,65000)||LF||LF
    CALL CLOSE(f)
    CALL DELETE('Email/'name'/'WORD(dups.i,j))
  END
  x=OPEN(f,'Email/'name'/'WORD(dups.i,1),'W')
  IF x=0 THEN
    DO
      CALL usermsg('Email/'name'/'WORD(dups.i,1) 'failed to open for writing!')
      CALL waiting()
      RETURN
    END
  CALL WRITECH(f,m)
  CALL CLOSE(f)
END
CALL sort_email()
CALL PostMsg()
CALL CloseWindow(QUICKHOST,'CONTINUE')
SIGNAL RESTART
RETURN


makepath:
PARSE ARG tdir,tfile
xx=RIGHT(tdir,1)
tpath=tdir
IF xx~=':' & xx~='/' THEN tpath=tpath'/'
tpath=tpath||tfile
RETURN tpath


read_ID:
CALL CLOSE(f)
x=OPEN(f,'.ID','R')
IF x=0 THEN
  DO
    CALL usermsg('.ID failed to open!')
    CALL waiting()
    RETURN
  END
id.=''
DO ii=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE ii
  id.ii=line
END
id.0=ii-1
CALL CLOSE(f)
name=SUBSTR(id.1,8)
city=SUBSTR(id.2,8)
bbsname=SUBSTR(id.3,8)
bbsnym=STRIP(LEFT(COMPRESS(bbsname," '/:;."),12))
sysop=SUBSTR(id.4,8)
arcdate=SUBSTR(id.5,8)
keys=SUBSTR(id.6,8)
level=WORD(keys,2)
sysoplevel=WORD(keys,3)
x=makepath(prefs.3,'OUT')
arcdir=makepath(x,bbsnym)
CALL MAKEDIR(arcdir)
workdir=makepath(prefs.1,bbsnym)
CALL MAKEDIR(workdir)
CALL PRAGMA('D',workdir)
x=OPEN(f,'Friends','R')
IF x=0 THEN RETURN
DO ii=1
  line=READLN(f)
  IF EOF(f) THEN LEAVE ii
  friends.ii=line
END
CALL CLOSE(f)
friends.0=ii-1
RETURN


load_notices:
notices=SHOWDIR('Notices')
n=FIND(notices,'HELLO')
IF n>0 THEN notices=DELWORD(notices,n,1)
n=FIND(notices,'WELCOME')
IF n>0 THEN notices=DELWORD(notices,n,1)
n=FIND(notices,'GOODBYE')
IF n>0 & n<WORDS(notices) THEN notices=DELWORD(notices,n,1) 'Goodbye'
IF EXISTS('Notices/WELCOME') THEN notices='Welcome' notices
IF EXISTS('Notices/HELLO') THEN notices='Hello' notices
noticecount=1
IF notices='' THEN noticecount=0
RETURN


sort_email:
email.=''
emailcount=0
email=SHOWDIR('Email/'name)
email.0=WORDS(email)
IF email.0=0 THEN RETURN 1
email=sortnumbers(email)
DO i=1 TO WORDS(email)
  email.i=WORD(email,i)
END
IF emailcount>email.0 | emailcount<1 THEN emailcount=1
RETURN 0


read_QUICK:
CALL usermsg('Loading' modetext.mode'...')
tfile=modetext.mode
IF mode=0 THEN
  DO
    IF notices='' THEN CALL load_notices()
    IF notices='' THEN tfile=''
    ELSE tfile='Notices/'WORD(notices,noticecount)
  END
IF mode=1 THEN
  DO
    tfile=''
    IF DATATYPE(email.0,'W') & email.0>0 THEN
      DO
        DO WHILE emailcount<email.0 & email.emailcount=''
          emailcount=emailcount+1
        END
        tfile='Email/'name'/'email.emailcount
      END
    ELSE IF sort_email()=0 THEN
      DO
        tfile='Email/'name'/'email.1
        emailcount=1
      END
    ELSE emailcount=0
  END
IF mode=2 & DATATYPE(mess.0,'W') THEN
  DO
    messcount=1
    CALL nextitem('FORWARD')
    CALL PostMsg()
    RETURN
  END
IF tfile='' | ~EXISTS(tfile) THEN
  DO
    CALL PostMsg()
    CALL Move(QUICKHOST,220,100)
    CALL Text(QUICKHOST,'- No' modetext.mode 'Found -')
    RETURN
  END
text.=''
IF mode=2 THEN mess.=''
x=OPEN(f,tfile,'R')
IF x~=0 THEN
  DO i=1
    line=READLN(f)
    IF mode=2 THEN mess.i=line
    ELSE text.i=line
    IF EOF(f) THEN LEAVE i
  END
CALL CLOSE(f)
IF mode=2 THEN
  DO
    mess.0=i
    CALL parse_mess()
  END
ELSE
  DO
    text.0=i
    CALL show_QUICK(1)
  END
CALL PostMsg()
RETURN


show_QUICK:
ARG startline 
CALL SetAPen(QUICKHOST,0)
CALL RectFill(QUICKHOST,12,28,w.cols-12,w.rows-14)
CALL SetAPen(QUICKHOST,1)
CALL SetBPen(QUICKHOST,0)
IF startline+lines_per_page>text.0 THEN startline=text.0+1-lines_per_page
IF startline<1 THEN startline=1
DO i=1 TO lines_per_page
  ii=i-1+startline
  CALL Move(QUICKHOST,24,28+i*8)
  CALL Text(QUICKHOST,LEFT(text.ii,chars))
  IF searcharg='' THEN ITERATE i
  n=POS(searcharg,UPPER(text.ii))
  IF n=0 THEN ITERATE i
  CALL SetAPen(QUICKHOST,2)
  CALL SetBPen(QUICKHOST,1)
  CALL Move(QUICKHOST,16+8*n,28+i*8)
  CALL Text(QUICKHOST,searcharg)
  CALL SetAPen(QUICKHOST,1)
  CALL SetBPen(QUICKHOST,0)
END
lastline=startline+i-2
IF mode<3 THEN
  DO
    CALL SetAPen(QUICKHOST,1)
    CALL SetBPen(QUICKHOST,3)
    CALL Move(QUICKHOST,30,7)
    IF mode=0 THEN
      temp=noticecount'/'WORDS(notices) WORD(notices,noticecount)
    ELSE IF mode=2 THEN temp=messcount'/'WORDS(mess.0.1) WORD(mess.0.1,messcount)
    ELSE temp=emailcount'/'email.0 email.emailcount
    temp=LEFT(temp,74-LENGTH(STRIP(w.title)))
    CALL Text(QUICKHOST,temp)
  END
IF text.0=0 THEN temp=100
ELSE temp=((.005+lastline/text.0)*100)%1
IF temp>100 THEN temp=100
temp=RIGHT(temp,4)
CALL SetAPen(QUICKHOST,2)
CALL SetBPen(QUICKHOST,1)
CALL Move(QUICKHOST,w.cols-58,34)
CALL Text(QUICKHOST,temp'% ')
searcharg=''
RETURN


parse_mess:
CALL PostMsg()
CALL usermsg('Setting Up Message Conferences...')
mess.0.0=''
mess.0.1=''
DO i=1 TO mess.0
  conf=''
  IF LEFT(mess.i,1)='>' THEN ITERATE i
  PARSE VAR mess.i 'Entering' conf 'Message Conference..'
  IF conf='' THEN
    DO
      PARSE VAR mess.i '= Custom archived for' who .
      IF who='' THEN ITERATE i
      conf='Header'
    END
  mess.0.0=mess.0.0 i
  mess.0.1=mess.0.1 conf
END
messcount=1
CALL nextitem('FORWARD')
RETURN


filltext:
ARG top bot 
text.=''
DO i=top TO bot
  ii=1+i-top
  text.ii=mess.i
END
text.0=i-top
RETURN


page:
ARG direction
IF direction='UP' THEN temp=lastline
IF direction='BACK' THEN temp=lastline+2-2*lines_per_page
IF direction='ZIP' THEN
  DO
    IF lastline>lines_per_page THEN temp=1
    ELSE temp=text.0
  END
CALL show_QUICK(temp)
RETURN


line:
ARG direction
IF direction='UP' THEN temp=startline+1
IF direction='BACK' THEN temp=startline-1
CALL show_QUICK(temp)
RETURN


nextitem:
ARG direction 
IF mode=0 & notices~='' THEN
  DO
    IF direction='FORWARD' THEN noticecount=noticecount+1
    ELSE noticecount=noticecount-1
    IF noticecount>WORDS(notices) THEN noticecount=1
    IF noticecount<1 THEN noticecount=WORDS(notices)
    CALL read_QUICK()
  END
IF mode=1 & DATATYPE(email.0,'W') THEN
  DO
    IF direction='FORWARD' THEN emailcount=emailcount+1
    ELSE
      DO
        emailcount=emailcount-1
        DO WHILE emailcount>0 & email.emailcount=''
          emailcount=emailcount-1
        END
      END
    IF emailcount>email.0 THEN emailcount=1
    IF emailcount<1 THEN emailcount=email.0
    CALL read_QUICK()
  END
IF mode=2 & WORDS(mess.0.1)>0 THEN
  DO
    IF direction='FORWARD' THEN messcount=messcount+1
    ELSE messcount=messcount-1
    IF messcount>WORDS(mess.0.1) THEN messcount=1
    IF messcount<1 THEN messcount=WORDS(mess.0.1)
    a=WORD(mess.0.0,messcount)
    IF messcount=WORDS(mess.0.1) THEN b=mess.0+1
    ELSE b=WORD(mess.0.0,messcount+1)-1
    CALL filltext(a b)
    CALL show_QUICK(1)
  END
RETURN


sortnumbers: PROCEDURE
PARSE ARG slist
IF STRIP(slist)='' THEN RETURN ''
sorted.=''
oldest=999999
newest=0
newlist=''
DO si=1 TO WORDS(slist)
  testword=WORD(slist,si)
  IF ~DATATYPE(testword,'W') THEN
    DO
      testpos=LASTPOS('.',testword)
      IF testpos>0 THEN tempnum=SUBSTR(testword,testpos+1)
      ELSE
        DO
          newlist=testword newlist
          ITERATE si
        END
    END
  ELSE tempnum=testword/1
  IF sorted.tempnum='' THEN
    DO
      sorted.tempnum=testword
      sorted.tempnum.0=1
      IF DATATYPE(tempnum,'W') THEN
        DO
          IF tempnum>newest THEN newest=tempnum
          IF tempnum<oldest THEN oldest=tempnum
        END
    END
  ELSE newlist=newlist testword
END
IF oldest~=999999 & newest~=0 THEN
  DO si=oldest TO newest
    IF sorted.si.0=1 THEN newlist=newlist sorted.si
  END
RETURN STRIP(newlist)


whichmode:
ARG test 
IF test=1 THEN mode=mode+1
IF mode>5 THEN mode=0
CALL CloseWindow(QUICKHOST,CONTINUE)
SIGNAL RESTART
RETURN;


boxes:
CALL box(QUICKHOST,1,1,11,27,w.cols-22,w.rows-40)
RETURN


box:
ARG host,pen1,pen2,upleft,uptop,width,height
CALL SetAPen(host,pen2)
CALL Move(host,upleft+width+1,uptop)
CALL Draw(host,upleft+width+1,uptop+height)
CALL Draw(host,upleft-1,uptop+height)
CALL Move(host,upleft+width,uptop+1)
CALL Draw(host,upleft+width,uptop+height)
CALL SetAPen(host,pen1)
CALL Move(host,upleft,uptop)
CALL Draw(host,upleft+width,uptop)
CALL Move(host,upleft,uptop+height-1)
CALL Draw(host,upleft,uptop)
CALL Move(host,upleft-1,uptop)
CALL Draw(host,upleft-1,uptop+height)
RETURN


usermsg:
PARSE ARG umsg
umsg=STRIP(LEFT(umsg,74))
CALL PostMsg()
IF ScreenCols(QUICKSCREEN)>0 THEN
  CALL PostMsg((w.cols-40-LENGTH(umsg)*8)%2,60,umsg,QUICKSCREEN)
ELSE SAY umsg
RETURN


waiting:
CALL DELAY(150)
CALL PostMsg()
RETURN


info:
infotext=        '     User:' name'\'
infotext=infotext'     City:' city'\\'
infotext=infotext'      BBS:' bbsname'\'
infotext=infotext'    SysOp:' sysop'\'
infotext=infotext'     Keys:' keys'\'
temp=arcfile
IF temp='ARCFILE' | temp='' THEN temp='QUICK_'WORD(keys,1)'.lha'
ELSE
  DO
    n=LASTPOS('/',temp)
    IF n=0 THEN n=LASTPOS(':',temp)
    IF n>0 THEN temp=SUBSTR(temp,n+1)
  END
infotext=infotext'  Arcfile:' temp'\'
infotext=infotext' Archived:' arcdate'\\'
infotext=infotext'KickStart:' ksversion'\'
infotext=infotext'WorkBench:' wbversion'\\'
infotext=infotext'   Memory:' comma(STORAGE()) 'bytes free.'
CALL Request(170,45,infotext,,,,QUICKSCREEN)
RETURN


set_colors:
ARG host
IF prefs.0='' THEN prefs.0='10 10 10 0 0 0 15 15 15 12 8 8'
CALL SetRGB4(host,0,WORD(prefs.0,1),WORD(prefs.0,2),WORD(prefs.0,3))
CALL SetRGB4(host,1,WORD(prefs.0,4),WORD(prefs.0,5),WORD(prefs.0,6))
CALL SetRGB4(host,2,WORD(prefs.0,7),WORD(prefs.0,8),WORD(prefs.0,9))
CALL SetRGB4(host,3,WORD(prefs.0,10),WORD(prefs.0,11),WORD(prefs.0,12))
RETURN


tofront:
ARG host
CALL set_colors(host)
CALL ActivateWindow(host)
CALL ScreenToFront(QUICKSCREEN)
RETURN


getnames:
PARSE ARG tdir,tfile,ptext
x=GetFile(150,16,tdir,tfile,ptext,QUICKSCREEN)
RETURN x


getdir:
PARSE ARG full .
IF full='' THEN RETURN ''
n=LASTPOS('/',full)
IF n=0 THEN n=LASTPOS(':',full)
IF n=0 THEN tdir=''
ELSE
  DO
    tdir=LEFT(full,n)
    IF RIGHT(tdir,1)='/' THEN tdir=LEFT(tdir,n-1)
  END
RETURN tdir


getfilename:
PARSE ARG full .
IF full='' THEN RETURN ''
n=LASTPOS('/',full)
IF n=0 THEN n=LASTPOS(':',full)
IF n=0 THEN tfile=full
ELSE tfile=SUBSTR(full,n+1)
tfile=COMPRESS(tfile,' ,;|#?*')  /* be sure no illegals here */
RETURN tfile


config:
ARG action .
IF action='LOAD' THEN
  DO
    x=getnames(prefsdir,prefsfile,'Load Preferences File ')
    IF x='' THEN RETURN 1
    prefsdir=getdir(x)
    prefsfile=getfilename(x)
    CALL get_prefs()
    CALL set_colors(QUICKHOST)
    CALL CloseWindow(QUICKHOST,'CONTINUE')
    IF p.title='' THEN SIGNAL RESTART
    ELSE SIGNAL START_PREFS
  END
IF action='SAVE' THEN
  DO
    x=getnames(prefsdir,prefsfile,'Save Preferences File ')
    IF x='' THEN RETURN 2
    prefsdir=getdir(x)
    prefsfile=getfilename(x)
    CALL save_prefs()
  END
IF action='SET' THEN
  DO
    CALL CloseWindow(QUICKHOST,'CONTINUE')
    SIGNAL START_PREFS
  END
RETURN 0


get_prefs:
prefspath=makepath(prefsdir,prefsfile)
x=OPEN(f,prefspath,'R')
IF x=0 THEN
  DO
    CALL usermsg(prefspath 'failed to open!')
    CALL waiting()
    RETURN
  END
prefs.=''
DO i=0 TO 9
  prefs.i=READLN(f)
END
CALL CLOSE(f)
CALL MAKEDIR(prefs.1)
CALL MAKEDIR(prefs.2)
CALL MAKEDIR(prefs.3)
CALL PRAGMA('D',prefs.1)
arcdir=makepath(prefs.3,'OUT')
CALL MAKEDIR(arcdir)
CALL LOADSIG()
laceflag=0
IF prefs.6=1 THEN laceflag=1
ELSE prefs.6=0
RETURN


/* Modification to support signature files S.Griffiths 01/08/94 */
LOADSIG:
sig.=''
sig.0=0 /* Set as a flag for later on (0 = no signature, ~0 = signature) */
IF prefs.5~='' THEN /* If the prefered name of sigfile is not blank */
  DO
    i=1
    if EXISTS(prefs.5) THEN
      DO
        x=OPEN(f,prefs.5,'R') /* Open the signature file */
        IF x=0 THEN
          DO
            CALL usermsg(prefs.5 'failed to open!')
            CALL waiting()
            RETURN
          END
        /* Read the signature file */
        DO UNTIL EOF(f)
          sig.i=READLN(f)
          i=i+1
        END
        CALL CLOSE(f)
        sig.0=i-1 /* if there was no signature file,or it was empty i = 0 */
      END
  END
RETURN


/* Routine to add the signature to a text file S.Griffiths 3/8/94 */
INSERTSIG:
PARSE ARG filename
IF sig.0~=0 THEN /* if there is a signature to add */
  DO
    IF Request(,90,'Place Signature In Message?',,' YES ',' NO ',QUICKSCREEN)='OKAY' THEN
      DO
        IF ~EXISTS(filename) THEN x=OPEN(f,filename,'W')
        ELSE x=OPEN(f,filename,'A')
        IF x=0 THEN
          DO
            CALL usermsg(filename 'failed to open for writing!')
            CALL waiting()
          END
        ELSE
          DO i=1 TO sig.0
            CALL WRITELN(f,sig.i)
          END
        CALL CLOSE(f)
      END
  END
RETURN


edit_request:
CALL FileList(arcdir'/???.HDR',hdrs)
text.=''
DO i=1 TO hdrs.0
  IF OPEN(f,hdrs.i,'R')=0 THEN ITERATE i
  line=READLN(f)
  type=STRIP(WORD(line,1))
  line=READLN(f)
  IF type~='File:' THEN line=READLN(f)
  text.i=RIGHT(i,2) RIGHT(type,5) WORD(line,2)
  If type~='File:' THEN
    DO
      line=READLN(f)
      text.i=text.i '-' STRIP(SUBSTR(line,8))
    END
  CALL CLOSE(f)
END
text.0=hdrs.0
IF EXISTS(arcdir'/File_Requests') THEN
  DO
    i=text.0+1
    text.0=i
    text.i=i 'File_Requests'
  END
IF EXISTS(arcdir'/File_Deletes') THEN
  DO
    i=text.0+1
    text.0=i
    text.i=i 'File_Deletes'
  END
IF text.0=0 THEN
  DO
    CALL usermsg('- No Requests pending -')
    CALL waiting()
    RETURN
  END
IF ksversion>36 THEN
  DO
    fil=RequestList(1,text.0,text,50,20,w.cols-100,w.rows-40,QUICKSCREEN)
    num=WORD(fil,1)
  END
ELSE
  DO
    CALL show_QUICK(1)
    num=Request(380,115,'Select Request by NUMBER','',,' CANCEL ',QUICKSCREEN)
  END
IF ~DATATYPE(num,'W') THEN RETURN
efile=LEFT(hdrs.num,LENGTH(hdrs.num)-3)
IF WORD(text.num,2)='File_Requests' THEN CALL editor(arcdir'/File_Requests')
ELSE IF WORD(text.num,2)='File_Deletes' THEN CALL editor(arcdir'/File_Deletes')
ELSE IF Request(,90,'Edit or Kill this Request?',,' EDIT ',' KILL ',QUICKSCREEN)='OKAY' THEN
  CALL editor(efile'TXT')
ELSE
  DO
    CALL DELETE(efile'HDR')
    CALL DELETE(efile'TXT')
    IF WORD(text.num,2)='File:' THEN CALL DELETE(arcdir'/Files/'WORD(text.num,3))
    CALL usermsg('Request' num 'deleted')
    CALL waiting()
  END
RETURN


edit_sig:
sigdir=getdir(prefs.5)
IF sigdir='' THEN sigdir=prefsdir
sfile=getfilename(prefs.5)
IF sfile='' THEN sfile=sigfile
x=getnames(sigdir,sfile,'Create/Edit Signature File ')
IF x='' THEN RETURN
prefs.5=x
CALL editor(prefs.5)
CALL CloseWindow(QUICKHOST,'CONTINUE')
SIGNAL START_PREFS
RETURN


new_colors:
col=''
DO i=0 TO 3
  col=col ScreenColor(QUICKSCREEN,i)
END
RETURN STRIP(col)


save_prefs:
prefspath=makepath(prefsdir,prefsfile)
x=OPEN(f,prefspath,'W')
IF x=0 THEN
  DO
    CALL usermsg(prefspath 'failed to open!')
    CALL waiting()
    RETURN
  END
prefs.0=new_colors()
DO i=0 TO 9
  CALL WRITELN(f,prefs.i)
END
CALL CLOSE(f)
RETURN


comma:
NUMERIC DIGITS 14     /* largest number of digits ARexx can handle */
ARG num .
mant = num - num%1  /* anything to the right of the decimal point? */
IF mant = 0 THEN mant = ''
ELSE
  DO
    mant = SUBSTR(mant,2)       /* if yes, then remove leading "0" */
    num = num % 1                      /* make num into an integer */
  END
dgt = LENGTH(num)                       /* number of digits in num */
numtext = ''
IF dgt>3 THEN numtext = ','RIGHT(num,3)
IF dgt>6 THEN numtext = ','LEFT(RIGHT(num,6),3)||numtext
IF dgt>9 THEN numtext = ','LEFT(RIGHT(num,9),3)||numtext
IF dgt>12 THEN
  DO
    numtext = ','LEFT(RIGHT(num,12),3)||numtext
    numtext = LEFT(num,dgt-12)||numtext
  END
ELSE IF dgt>9 THEN numtext = LEFT(num,dgt-9)||numtext
ELSE IF dgt>6 THEN numtext = LEFT(num,dgt-6)||numtext
ELSE IF dgt>3 THEN numtext = LEFT(num,dgt-3)||numtext
ELSE numtext = num
numtext = numtext||mant          /* replace any decimal point data */
RETURN numtext


getversions:
ADDRESS COMMAND 'version >RAM:VERSION'
x=OPEN(f,'RAM:VERSION','R')
line=READLN(f)
CALL CLOSE(f)
CALL DELETE('RAM:VERSION')
wbversion=WORD(line,4)
IF ~DATATYPE(wbversion,'N') THEN
  DO
    wbversion=WORD(line,6)
    ksversion=WORD(line,3)
  END
ELSE ksversion=WORD(line,2)
IF RIGHT(ksversion,1)=',' THEN ksversion=LEFT(ksversion,LENGTH(ksversion)-1)
RETURN


setup:
CALL reset_vars()
CALL OPENPORT('QUICKPORT')
modetext.=''
modetext.0='Notices'
modetext.1='EMail'
modetext.2='Messages'
modetext.3='Conferences'
modetext.4='FileList'
modetext.5='Libraries'
startline=1
CALL getversions()
prefsdir='s:'
sigfile='bbsQUICK.sig'
prefsfile='bbsQUICK.prefs'
prefspath=makepath(prefsdir,prefsfile)
IF EXISTS(prefspath) & resetflag THEN CALL get_prefs()
ELSE
  DO
    prefs.1='RAM:Q'
    CALL MAKEDIR(prefs.1)
    CALL PRAGMA('D',prefs.1)
    arcdir=makepath(prefs.3,'OUT')
    CALL MAKEDIR(arcdir)
  END
x=SOURCELINE(1)
prgversion=''
DO i=3 TO 4
  prgversion=STRIP(prgversion WORD(x,i))
END
abouttext=CENTER(prgversion,32)'\\
  © 1993 Richard Lee Stockton\
     Voice:   206-776-1253\
       FAX:   206-775-7573\
       BBS:   206-744-1254\\
      FREELY DISTRIBUTABLE\
 AS LONG AS THIS NOTICE REMAINS'
prefshelp='
      Work Dir => This is the directory where the QUICK_ archive\
will be decompressed, and from which the data files then will be read.\\
  Download Dir => This should be your terminal program''s download\
directory where the QUICK_ archive (from the BBS to you) can be located.\\
    Upload Dir => This should be your terminal program''s upload\
directory where the QUICKIN archive (from you to the BBS) can be built.\
Make sure it is located somewhere other than RAM: so that data won''t\
be lost if the power goes off! A directory called OUT is made here and\
the OUTgoing (back to BBS) files are stored here for later archival.\\
        Editor => Your choice of editor. Memacs works well, as does Ed.\\
Signature File => This should be the name of your signature file that\
can be used to personalise your E-Mail and Messages.'
LF='0A'x
FF='0C'x
QT='22'x
ADDRESS AREXX  "'x=createhost(QUICKHOST,QUICKPORT,QUICKSCREEN)'"
DO 200 WHILE ~SHOW('Ports','QUICKHOST')
  CALL DELAY 10  /* 200 ms */
END
IF ~SHOW('Ports','QUICKHOST') THEN
  CALL ALL_DONE('Could not open host QUICKHOST.')
s.=''
s.modes='HIRES'
IF laceflag THEN s.modes='HIRES+LACE'
s.title=prgversion'   -   BBBBS OffLine Read/Reply Module'
IF ScreenRows(QUICKSCREEN)>0 THEN RETURN
IF OpenScreen(,2,s.modes,s.title,QUICKSCREEN)=0 THEN
  CALL ALL_DONE('Error!  Could not open bbsQUICK screen... Exiting.')
w.=''
w.cols=ScreenCols(QUICKSCREEN)
w.rows=ScreenRows(QUICKSCREEN)-1
/* w.cols=640; w.rows=200 */
lines_per_page=(w.rows-43)%8
chars=(w.cols-48)%8
w.idcmp='CLOSEWINDOW+MENUPICK+GADGETUP'
w.flags='WINDOWCLOSE+ACTIVATE'
RETURN

/* bbsQUICK.rexx */
