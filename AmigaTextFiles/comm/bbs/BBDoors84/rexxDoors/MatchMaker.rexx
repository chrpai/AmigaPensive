/* The MatchMaker by Patrick Baker 12-11-89 */
/* Revised on 12-12-89 This is version 1.1  */
/* Revised May-Oct 1991 by RLStockton for BBBBS */
/* Revised Apr 1992 by RLStockton & 7-Aug-93 */

options results
signal on syntax

CALL TIME('R')
SIGNAL ON BREAK_C

ARG REALNAME . . . secs .
IF secs='' THEN secs=3600
direct=GETCLIP('BBS_path')'rexxDoors/Data'
CALL MAKEDIR(direct)
direct=direct'/'
CALL MAKEDIR(direct||'users')
TOP = '0D'x||'0A'x||'Patrick Baker''s MatchMaker v 1.3'

EP:
 CALL TRANSMIT TOP
 CALL TRANSMIT ''
 CALL TRANSMIT 'Please enter alias: '
 OPTIONS PROMPT '> '
 PARSE PULL alias
 if alias = '' then EXIT
 if exists(direct'users/Total') then do
    call open file,direct'users/Total','R'
         tot = readln(file)
    call close file
 end
 if ~exists(direct'users/Total') then do
     call open file,direct'users/Total','W'
     call writeln file, 0
     call close file
     tot = 0
 end 
  if exists(direct'users/'ALIAS) then do
     call open file,direct'users/'ALIAS,'R'
          realname = readln(file)
          alias1 = readln(file)
          pass   = readln(file)
     call close file
     OPTIONS PROMPT 'Password: '
     PARSE PULL anspass
     if anspass = pass then
       DO
         CALL FINDMESSAGES
         SIGNAL MAIN
       END
     CALL TRANSMIT 'Wrong!  Please logon and type the correct password!'
     CALL TRANSMIT 'If you FORGOT it, contact the SysOp!'
     CALL TRANSMIT ''
     OPTIONS PROMPT 'Hit RETURN: '
     PULL junk
     EXIT
   end
 if ~exists(direct'users/'ALIAS) then do
     CALL TRANSMIT 'Your name is not found among the MatchMaker members!'
     OPTIONS PROMPT 'Are you a member(Y/N)? '
     PARSE PULL ans
     if ans = 'Y' | ans = 'y' then do
        SIGNAL EP
     end
     CALL TRANSMIT ''
     CALL TRANSMIT 'You must first answer a few questions to register!'
     OPTIONS PROMPT 'Do you wish to register(Y/N)? '
     PARSE PULL ans
     if ans = 'N' | ans = 'n' then EXIT
     if ans = '' then EXIT
/* Register */
     call REG
     CALL TRANSMIT 'Alias = 'alias
     CALL TRANSMIT 'Pass  = 'pass
     CALL TRANSMIT ''
     OPTIONS PROMPT 'Correct(Y/N)? '
     PULL ans
     if ans = '' then EXIT
     IF ans = 'N' | ans = 'n' then do 
        call REG
        CALL TRANSMIT 'You just entered:'
        CALL TRANSMIT 'Alias = 'alias 
        CALL TRANSMIT 'Pass  = 'pass
        CALL TRANSMIT ''
        CALL TRANSMIT 'If it is incorrect, quit and contact the SysOp!'
     end  
     CALL TRANSMIT ''
     OPTIONS PROMPT 'Hit RETURN: '
     PULL junk
     call QUEST

MAIN:
  CALL TRANSMIT TOP
  CALL TRANSMIT ''
  CALL TRANSMIT ' 1)  Make Your Match'
  CALL TRANSMIT ' 2)  View Your Stats'
  CALL TRANSMIT ' 3)  View Others Stats'
  CALL TRANSMIT ' 4)  Change Your Stats'
  CALL TRANSMIT ' 5)  User Listing'
  CALL TRANSMIT ' 6)  Send A Message'
  CALL TRANSMIT ' 7)  Check For Messages To 'ALIAS
  CALL TRANSMIT ' '
  CALL TRANSMIT ' E)xit to BBBBS '
  CALL TRANSMIT ''
  OPTIONS PROMPT '> '
  PULL ans
  CALL checkBBS()
  if ans = '' then SIGNAL MAIN
  if ans = 'E' then EXIT
  if ans = '1' then call MATCH
  if ans = '2' then call YOURSTATS
  if ans = '3' then call OTHERSTATS
  if ans = '4' then call QUEST
  if ans = '5' then call USERLIST
  if ans = '6' then call SENDMESSAGE
  if ans = '7' then call FINDMESSAGES
SIGNAL MAIN


SENDMESSAGE:
OPTIONS PROMPT 'Send Message To: '
PARSE PULL toname
IF toname='' THEN RETURN
IF ~EXISTS(direct'users/'toname) THEN
  DO
    CALL TRANSMIT '***' toname' not found in the MatchMaker userlist!'
    RETURN
  END
CALL TRANSMIT 'Note: Lines are limited to 75 characters!'
CALL TRANSMIT 'There is no editing, so start over if you mess up.'
CALL TRANSMIT 'Enter your message.  Enter a blank line to exit.'
lynes.=''
OPTIONS PROMPT ': '
DO i=1
  PARSE PULL lynes.i
  IF lynes.i='' THEN LEAVE i
  IF LENGTH(lynes.i)>75 THEN lynes.i=LEFT(lynes.i,75)
END
lynes.0=i-1
OPTIONS PROMPT 'Send this MatchMaker Message to 'toname'? (n/Y) > '
PULL temp
IF LEFT(temp,1)~='N' THEN
  DO
    mess=direct'users/Messages'
    CALL MAKEDIR(mess)
    mess=mess'/'ALIAS'.'toname
    IF EXISTS(mess) THEN x=OPEN(f,mess,'A')
    ELSE x=OPEN(f,mess,'W')
    IF x=0 THEN RETURN
    CALL WRITELN(f,LEFT('=',40,'='))
    CALL WRITELN(f,'MatchMaker Message to' toname 'from' ALIAS)
    CALL WRITELN(f,DATE() 'at' TIME('C'))
    CALL WRITELN(f,LEFT('=',40,'='))
    DO i=1 TO lynes.0
      CALL WRITELN(f,lynes.i)
    END
    CALL CLOSE(f)
  END
RETURN


FINDMESSAGES:
CALL TRANSMIT 'Checking your MatchMaker Mail...'
foundmsg=0
mess=direct'users/Messages'
CALL MAKEDIR(mess)
CALL FileList(mess'/*',msgs)
DO i=1 TO msgs.0
  dotpos=LASTPOS('.',msgs.i)
  IF UPPER(ALIAS)=UPPER(SUBSTR(msgs.i,dotpos+1)) THEN
    DO
      x=OPEN(f,msgs.i,'R')
      IF x=0 THEN ITERATE i
      DO j=1
        line=READLN(f)
        IF EOF(f) THEN LEAVE j
        CALL TRANSMIT line
      END
      CALL CLOSE(f)
      OPTIONS PROMPT 'Delete this message? (n/Y) > '
      PULL temp
      IF LEFT(temp,1)~='N' THEN CALL DELETE(msgs.i)
      foundmsg=1
    END
END
IF foundmsg=0 THEN CALL TRANSMIT 'No messages to' ALIAS 'were found.'
RETURN


QUEST:
/* ANSWER QUESTIONARE */
CALL TRANSMIT 'Please answer these 18 questions truthfully'
CALL TRANSMIT ''  
CALL TRANSMIT 'Question #1'
OPTIONS PROMPT 'Gender (1=Male/2=Female)? '
PULL gender
if gender = '' then EXIT
CALL TRANSMIT ''
CALL TRANSMIT 'Question #2'
CALL TRANSMIT 'What is your sexual preference: '
CALL TRANSMIT '     1) Heterosexual'
CALL TRANSMIT '     2) Homosexual'
CALL TRANSMIT '     3) BiSexual'
CALL TRANSMIT '     4) TrySexual (Ill try anything!)'
OPTIONS PROMPT '1-4> '
PULL pref
if pref = '' then EXIT
CALL TRANSMIT ''
CALL TRANSMIT 'Question #3'
CALL TRANSMIT 'What is your weight:'
CALL TRANSMIT '     1) 90-100 '
CALL TRANSMIT '     2) 101-120 '
CALL TRANSMIT '     3) 121-130 '
CALL TRANSMIT '     4) 131-150 '
CALL TRANSMIT '     5) 151-180 '
CALL TRANSMIT '     6) 181-210 '
CALL TRANSMIT '     7) over 210 '
OPTIONS PROMPT '1-7> '
PULL weight
if weight = '' then EXIT
CALL TRANSMIT 'Question #4'
CALL TRANSMIT ''
CALL TRANSMIT 'What weight do you prefer to match with: '
CALL TRANSMIT '     1) 90-100 '
CALL TRANSMIT '     2) 101-120 '
CALL TRANSMIT '     3) 121-130 '
CALL TRANSMIT '     4) 131-150 '
CALL TRANSMIT '     5) 151-180 '
CALL TRANSMIT '     6) 181-210 '
CALL TRANSMIT '     7) over 210 '
OPTIONS PROMPT '1-7> '
PULL weightpref
if weightpref = '' then EXIT
CALL TRANSMIT ''
CALL TRANSMIT 'Question #5'    
CALL TRANSMIT 'Eye color:'
CALL TRANSMIT '     1) Blue'
CALL TRANSMIT '     2) Grey'
CALL TRANSMIT '     3) Brown'
CALL TRANSMIT '     4) Green'
CALL TRANSMIT '     5) Hazel'
CALL TRANSMIT '     6) Black'
OPTIONS PROMPT '1-6> '
PULL eye
if eye = '' then EXIT
CALL TRANSMIT ''
CALL TRANSMIT 'Question #6'
CALL TRANSMIT 'Height: '
CALL TRANSMIT '     1) 4 ft - 4.5 ft'
CALL TRANSMIT '     2) 4.6  - 5.0 ft'
CALL TRANSMIT '     3) 5.1  - 5.5 ft'
CALL TRANSMIT '     4) 5.6  - 5.8 ft'
CALL TRANSMIT '     5) 5.9  - 5.11 ft'
CALL TRANSMIT '     6) 6.0  - 6.5 ft'
CALL TRANSMIT '     7) 6.6  - up'
CALL TRANSMIT '(period denotes a quote for feet)'
OPTIONS PROMPT '1-7> ' 
PULL height
if height = '' then EXIT
CALL TRANSMIT 'Question #7'
CALL TRANSMIT 'Age: '
CALL TRANSMIT '     1) 13-15'
CALL TRANSMIT '     2) 16-18'
CALL TRANSMIT '     3) 19-25'
CALL TRANSMIT '     4) 26-30'
CALL TRANSMIT '     5) 31-35'
CALL TRANSMIT '     6) 36-40'
CALL TRANSMIT '     7) 41-45'
CALL TRANSMIT '     8) 46-50'
CALL TRANSMIT '     9) 51-60'
CALL TRANSMIT '    10) Other'
OPTIONS PROMPT '1-10> '
PULL age
if age = '' then EXIT
CALL TRANSMIT 'Question #8'
CALL TRANSMIT 'What age would you like to match with: '
CALL TRANSMIT '     1) 13-15'
CALL TRANSMIT '     2) 16-18'
CALL TRANSMIT '     3) 19-25'
CALL TRANSMIT '     4) 26-30'
CALL TRANSMIT '     5) 31-35'
CALL TRANSMIT '     6) 36-40'
CALL TRANSMIT '     7) 41-45'
CALL TRANSMIT '     8) 46-50'
CALL TRANSMIT '     9) 51-60'
CALL TRANSMIT '    10) Other'
OPTIONS PROMPT '1-10> '
PULL agepref
if agepref = '' then EXIT

CALL TRANSMIT 'Question #9'
CALL TRANSMIT 'What kind of music do you like?'
CALL TRANSMIT '     1) Rock and Roll'
CALL TRANSMIT '     2) Punk/New Wave'
CALL TRANSMIT '     3) Heavy Metal dude!'
CALL TRANSMIT '     4) Classical'
CALL TRANSMIT '     5) Country'
CALL TRANSMIT '     6) Opera'
CALL TRANSMIT '     7) Soft Rock'
CALL TRANSMIT '     8) Jazz'
CALL TRANSMIT '     9) Rap'
CALL TRANSMIT '    10) Folk'
CALL TRANSMIT '    11) Other'
CALL TRANSMIT '    12) All Music'
OPTIONS PROMPT '1-12> '
PULL music
if music = '' then EXIT

CALL TRANSMIT 'Question #10'
CALL TRANSMIT 'What are your hobbies?'
CALL TRANSMIT '     1) Sex'
CALL TRANSMIT '     2) Sports or exercise'
CALL TRANSMIT '     3) Parties'
CALL TRANSMIT '     4) Watching TV'
CALL TRANSMIT '     5) Computers'
CALL TRANSMIT '     6) Reading'
CALL TRANSMIT '     7) All of the Above'
CALL TRANSMIT '     8) Other'
OPTIONS PROMPT '1-8> '
PULL hobby
if hobby = '' then EXIT

CALL TRANSMIT 'Question #11'
CALL TRANSMIT 'How much do you make a year?'
CALL TRANSMIT '     1) under 5000'
CALL TRANSMIT '     2) 5001-10000'
CALL TRANSMIT '     3) 10001-15000'
CALL TRANSMIT '     4) 15001-25000'
CALL TRANSMIT '     5) 25001-40000'
CALL TRANSMIT '     6) 40001-60000'
CALL TRANSMIT '     7) 60001-up'
OPTIONS PROMPT '1-7> '
PULL salary
if salary = '' then EXIT

CALL TRANSMIT 'Question #12'
CALL TRANSMIT 'What do you do on a first date?'
CALL TRANSMIT '     1) go to a movie'
CALL TRANSMIT '     2) go out to dinner'
CALL TRANSMIT '     3) go dancing/bars'
CALL TRANSMIT '     4) go to a play/concert'
CALL TRANSMIT '     5) I hit the MOTEL ROOM with my date'
CALL TRANSMIT '     6) I go and meet the mother'
CALL TRANSMIT '     7) I have never been on a date'
CALL TRANSMIT '     8) Other'
OPTIONS PROMPT '1-8> '
PULL firstdate
if firstdate = '' then EXIT

CALL TRANSMIT 'Question #13'
CALL TRANSMIT 'Do you believe in womens lib?'
CALL TRANSMIT '     1) YES'
CALL TRANSMIT '     2) No'
CALL TRANSMIT '     3) No opinion'
OPTIONS PROMPT '1-3> '
PULL womenslib
if womenslib = '' then EXIT

CALL TRANSMIT 'Question #14'
CALL TRANSMIT 'Are you:'
CALL TRANSMIT '     1) Democrat'
CALL TRANSMIT '     2) Republican'
CALL TRANSMIT '     3) Communist'
CALL TRANSMIT '     4) Socialist'
CALL TRANSMIT '     5) Liberal'
CALL TRANSMIT '     6) Independent'
CALL TRANSMIT '     7) Other'
OPTIONS PROMPT '1-7> '
PULL politics
if politics = '' then EXIT

CALL TRANSMIT 'Question #15'
CALL TRANSMIT 'Do you believe in Racial equality?'
CALL TRANSMIT '     1) Heck YES!'
CALL TRANSMIT '     2) No.'
CALL TRANSMIT '     3) No Opinion'
OPTIONS PROMPT '1-3> '
PULL RACIALEQUAL
if RACIALEQUAL = '' then EXIT

CALL TRANSMIT 'Question #16'
CALL TRANSMIT 'Would you date someone of another RACE?'
CALL TRANSMIT '     1) Yes!'
CALL TRANSMIT '     2) No.'
CALL TRANSMIT '     3) Depends'
OPTIONS PROMPT '1-3> '
PULL RACEPREF
if RACEPREF = '' then EXIT

CALL TRANSMIT 'Question #17'
CALL TRANSMIT 'What race are you?'
CALL TRANSMIT '     1) Hispanic'
CALL TRANSMIT '     2) Asian'
CALL TRANSMIT '     3) African American'
CALL TRANSMIT '     4) Native American'
CALL TRANSMIT '     5) Caucasion'
CALL TRANSMIT '     6) NewComer'
CALL TRANSMIT '     7) Other'
OPTIONS PROMPT '1-7> '
PULL race
if race = '' then EXIT

CALL TRANSMIT 'Question #18'
CALL TRANSMIT 'Do you believe in abortion?'
CALL TRANSMIT '     1) Yes'
CALL TRANSMIT '     2) No '
CALL TRANSMIT '     3) only in certain circumstances.'
OPTIONS PROMPT '1-3> '
PULL abort
if abort = '' then EXIT

OPTIONS PROMPT 'Was everything entered correctly(Y/N)? '
PULL ans
if ans = '' then EXIT
if ans = 'n' | ans = 'N' then call QUEST
CALL TRANSMIT 'Writing questionare.....'

/* WRITE STATS to TOTAL, PEOPLE, and CREATE FILE */
IF ~EXISTS(direct'users/'ALIAS) THEN DO
  if exists(direct'users/Total') then do
    call open file,direct'users/Total','W'
        tot = tot + 1
        call writeln file, tot
        call close file
  end
  if ~exists(direct'users/Total') then do
    tot = 1
    call open file,direct'users/Total','W'
    call writeln file, tot
    call close file
  end
  if exists(direct'users/People') then do
       call open file,direct'users/People', 'A'
       call writeln file, alias
       call close file
  end 
  if ~exists(direct'users/People') then do
    call open file,direct'users/People','W'
    call writeln file, alias
    call close file
  end
END
CALL MAKEDIR(direct'users')
call open(file,direct'users/'ALIAS,'W')
CALL WRITELN FILE, REALNAME
call writeln file, ALIAS 
call writeln file, PASS
call writeln file, gender
call writeln file, pref
call writeln file, weight
call writeln file, weightpref
call writeln file, eye
call writeln file, height
call writeln file, AGE
call writeln file, AGEPREF

call writeln file, MUSIC
call writeln file, HOBBY
call writeln file, SALARY
call writeln file, FIRSTDATE
call writeln file, WOMENSLIB
call writeln file, POLITICS
call writeln file, RACIALEQUAL
call writeln file, RACEPREF
call writeln file, RACE
call writeln file, ABORT
CALL close file
RETURN


MATCH:
call open file1,direct'users/Total', 'R'
tot = readln(file1)
call close file1 
call open file,direct'users/'alias,'R'
NAME = READLN(FILE)
alias1 = readln(file)
pass = readln(file)
gender = readln(file)
pref = readln(file)
weight = readln(file)
weightpref = readln(file)
eye = readln(file)
height = readln(file)
age = readln(file)
agepref = readln(file)
MUSIC = readln(file)
HOBBY = readln(file)
SALARY = readln(file)
FIRSTDATE = readln(file)
WOMENSLIB = readln(file)
POLITICS = readln(file)
RACIALEQUAL = readln(file)
RACEPREF = readln(file)
RACE = readln(file)
ABORT = readln(file)
call close file
CALL TRANSMIT 'Matching...'
call open file,direct'users/People','R'
do j = 1 to tot
  matchname.j = readln(file)
end
do i = 1 to tot
  call open file2,direct'users/'matchname.i,'R'
  othername.i = readln(file2)
  otheralias.i = readln(file2)
  otherpass.i = readln(file2)
  othergender.i = readln(file2) 
  otherpref.i = readln(file2)
  otherweight.i = readln(file2)
  otherweightpref.i = readln(file2)
  othereye.i = readln(file2)
  otherheight.i = readln(file2)
  otherage.i = readln(file2)
  otheragepref.i = readln(file2)
  otherMUSIC = readln(file2)
  otherHOBBY = readln(file2)
  otherSALARY = readln(file2)
  otherFIRSTDATE = readln(file2)
  otherWOMENSLIB = readln(file2)
  otherPOLITICS = readln(file2)
  otherRACIALEQUAL = readln(file2)
  otherRACEPREF = readln(file2)
  otherRACE = readln(file2)
  otherABORT = readln(file2)
  call close file2
end
call close file
ct = 0
CALL TRANSMIT 'Name                    %      Gender'
CALL TRANSMIT ''
matches=0
do i = 1 to tot
  if othergender.i = 1 then AGEN='MALE'
  if othergender.i = 2 then AGEN='FEMALE'
  if otherpref.i = pref then do
     ct=ct+1
  end
  if otherweight.i = weightpref then ct = ct + 1
  if otherweightpref.i = weight then ct = ct + 1
  if othereye.i = eye then ct = ct + 1
  if gender = 1 then do
     if height > otherheight.i then ct = ct + 1
  end
  if gender = 2 then do
     if height < otherheight.i then ct = ct + 1
  end
  if age = otheragepref.i then ct = ct + 1
  if otherage.i = agepref then ct = ct + 1

  if otherpref.i = pref then do
     if pref = 4 | pref = 3 then do
       percentage = (ct/7)* 100
       percentage = left(percentage,4) 
       matchname.i = left(matchname.i,20)
       gen = left(gen,10)
       CALL TRANSMIT matchname.i'  'percentage'%    'Agen
       matches=matches+1
       ct=0
     end 
     if pref = 2 then do
       if othergender.i = gender then do
         percentage = (ct/7)* 100
         percentage = left(percentage,4) 
         matchname.i = left(matchname.i,20)
         gen = left(gen,10)
         CALL TRANSMIT matchname.i'  'percentage'%    'Agen
         matches=matches+1
         ct=0
       end
     end 
     if pref = 1 then do
       if otherpref.i = 1 then do
         if othergender.i ~= gender then do
           percentage = (ct/7)* 100
           percentage = left(percentage,4) 
           matchname.i = left(matchname.i,20)
           gen = left(gen,10)
           CALL TRANSMIT matchname.i'  'percentage'%    'Agen
           matches=matches+1
           ct=0
         end
       end
     end
     ct=0
  end
end

CALL TRANSMIT ''
if matches = 0 then do
  CALL TRANSMIT 'No Matches!  Try later!'
end
OPTIONS PROMPT 'Hit RETURN'
PULL junk
RETURN


USERLIST:
  CALL TRANSMIT TOP
  CALL TRANSMIT ''
  CALL TRANSMIT 'MatchMaker Userlist: '
  CALL TRANSMIT ''
  count = 0
  call open file,direct'users/People','R'
  do loop=1 until EOF(File)
     count = count + 1  
     name = readln(file)
     CALL TRANSMIT Name
     if count = 20 then do
        CALL TRANSMIT ''
        OPTIONS PROMPT 'Continue(Y/N)? '
        PULL ans
        if ans = 'N' then LEAVE loop
        count = 0
     end
  end
  call close file
  CALL TRANSMIT ''
  OPTIONS PROMPT 'Hit RETURN: '
  PULL junk
RETURN


OTHERSTATS:
  OPTIONS PROMPT 'View who? '
  PARSE PULL name
  IF NAME='' THEN RETURN
IF exists(direct'users/'NAME) then do
  call open file,direct'users/'NAME,'R'
       OTHERNAME = READLN(FILE)
       otheralias = readln(file)
       otherpass = readln(file)
       othergender = readln(file)
       otherpref = readln(file)
       otherweight = readln(file)
       otherweightpref = readln(file)
       othereye = readln(file)
       otherheight = readln(file)
       otherage = readln(file)
       otheragepref = readln(file)
       otherMUSIC = readln(file)
       otherHOBBY = readln(file)
       otherSALARY = readln(file)
       otherFIRSTDATE = readln(file)
       otherWOMENSLIB = readln(file)
       otherPOLITICS = readln(file)
       otherRACIALEQUAL = readln(file)
       otherRACEPREF = readln(file)
       otherRACE = readln(file)
       otherABORT = readln(file)
  call close file

  if othermusic = 1 then othermusic = 'Rock and Roll'
  if othermusic = 2 then othermusic = 'Punk/New Wave'
  if othermusic = 3 then othermusic = 'Heavy Metal dude!'
  if othermusic = 4 then othermusic = 'Classical'
  if othermusic = 5 then othermusic = 'Country'
  if othermusic = 6 then othermusic = 'Opera'
  if othermusic = 7 then othermusic = 'Soft Rock'
  if othermusic = 8 then othermusic = 'Jazz'
  if othermusic = 9 then othermusic = 'Rap'
  if othermusic =10 then othermusic = 'Folk'
  if othermusic =11 then othermusic = 'Other'
  if othermusic =12 then othermusic = 'All Music'

  if otherhobby = 1 then otherhobby = 'Sex'
  if otherhobby = 2 then otherhobby = 'Sports or exercise'
  if otherhobby = 3 then otherhobby = 'Parties!!'
  if otherhobby = 4 then otherhobby = 'Watching TV'
  if otherhobby = 5 then otherhobby = 'Computers'
  if otherhobby = 6 then otherhobby = 'Reading'
  if otherhobby = 7 then otherhobby = 'All of the above'
  if otherhobby = 8 then otherhobby = 'Other'

  if othersalary = 1 then othersalary = 'under 5000'
  if othersalary = 2 then othersalary = '5001-10000'
  if othersalary = 3 then othersalary = '10001-15000'
  if othersalary = 4 then othersalary = '15001-25000'
  if othersalary = 5 then othersalary = '25001-40000'
  if othersalary = 6 then othersalary = '40001-60000'
  if othersalary = 7 then othersalary = '60001-up'

  if otherfirstdate = 1 then otherfirstdate = 'go to a movie'
  if otherfirstdate = 2 then otherfirstdate = 'go out to dinner'
  if otherfirstdate = 3 then otherfirstdate = 'go dancing/bars'
  if otherfirstdate = 4 then otherfirstdate = 'go to a play/concert'
  if otherfirstdate = 5 then otherfirstdate = 'I hit the MOTEL ROOM with my date'
  if otherfirstdate = 6 then otherfirstdate = 'I go and meet the mother'
  if otherfirstdate = 7 then otherfirstdate = 'I have never been on a date'
  if otherfirstdate = 8 then otherfirstdate = 'Other'

  if otherwomenslib = 1 then otherwomenslib = 'YES'
  if otherwomenslib = 2 then otherwomenslib = 'No'
  if otherwomenslib = 3 then otherwomenslib = 'No opinion'
 
  if otherpolitics = 1 then otherpolitics = 'Democrat'
  if otherpolitics = 2 then otherpolitics = 'Republican'
  if otherpolitics = 3 then otherpolitics = 'Communist'
  if otherpolitics = 4 then otherpolitics = 'Socialist'
  if otherpolitics = 5 then otherpolitics = 'Liberal'
  if otherpolitics = 6 then otherpolitics = 'Independent'
  if otherpolitics = 7 then otherpolitics = 'Other'

  if otherRacialequal = 1 then otherRacialequal = 'Heck YES!'
  if otherRacialequal = 2 then otherRacialequal = 'No.'
  if otherRacialequal = 3 then otherRacialequal = 'No Opinion'
  
  if otherracepref = 1 then otherracepref = 'Yes!'
  if otherracepref = 2 then otherracepref = 'No.'
  if otherracepref = 3 then otherracepref = 'Depends'

  if otherRACE = 1 then otherrace = 'Hispanic'
  if otherRACE = 2 then otherrace = 'Asian'
  if otherRACE = 3 then otherrace = 'African American'
  if otherRACE = 4 then otherrace = 'Native American'
  if otherRACE = 5 then otherrace = 'Caucasion'
  if otherRACE = 6 then otherrace = 'Newcomer'
  if otherRACE = 7 then otherrace = 'Other'

  if otherabort = 1 then otherabort = 'Yes'
  if otherabort = 2 then otherabort = 'No'
  if otherabort = 3 then otherabort = 'only in certain circumstances'




/* OLD QUESTIONS */
  if othergender = 1 then othergender = 'Male'
  if othergender = 2 then othergender = 'Female'

  if otherpref = 1 then otherpref = 'HeteroSexual'
  if otherpref = 2 then otherpref = 'HomoSexual'
  if otherpref = 3 then otherpref = 'BiSexual'
  if otherpref = 4 then otherpref = 'TrySexual'

  if otherweight = 1 then otherweight = '90-100'
  if otherweight = 2 then otherweight = '101-120'
  if otherweight = 3 then otherweight = '121-130'
  if otherweight = 4 then otherweight = '131-150'
  if otherweight = 5 then otherweight = '151-180'
  if otherweight = 6 then otherweight = '181-210'
  if otherweight = 7 then otherweight = 'over 210'

  if otherweightpref = 1 then otherweightpref = '90-100'
  if otherweightpref = 2 then otherweightpref = '101-120'
  if otherweightpref = 3 then otherweightpref = '121-130'
  if otherweightpref = 4 then otherweightpref = '131-150'
  if otherweightpref = 5 then otherweightpref = '151-180'
  if otherweightpref = 6 then otherweightpref = '181-210'
  if otherweightpref = 7 then otherweightpref = 'over 210'

  if othereye = 1 then othereye = 'Blue'
  if othereye = 2 then othereye = 'Grey'
  if othereye = 3 then othereye = 'Brown'
  if othereye = 4 then othereye = 'Green'
  if othereye = 5 then othereye = 'Hazel'
  if othereye = 6 then othereye = 'Black'

  if otherheight = 1 then otherheight = '4ft-4.5ft'
  if otherheight = 2 then otherheight = '4.6ft-5ft'
  if otherheight = 3 then otherheight = '5.1ft-5.5ft'
  if otherheight = 4 then otherheight = '5.6ft-5.8ft'
  if otherheight = 5 then otherheight = '5.9ft-5.11ft'
  if otherheight = 6 then otherheight = '6ft-6.5ft'
  if otherheight = 7 then otherheight = '6.6ft-UP'

  if otherage = 1 then otherage = '13-15'
  if otherage = 2 then otherage = '16-18'
  if otherage = 3 then otherage = '19-25'
  if otherage = 4 then otherage = '26-30'
  if otherage = 5 then otherage = '31-35'
  if otherage = 6 then otherage = '36-40'
  if otherage = 7 then otherage = '41-45'
  if otherage = 8 then otherage = '46-50'
  if otherage = 9 then otherage = '51-60'
  if otherage = 10 then otherage = 'Other'

  if otheragepref = 1 then otheragepref = '13-15'
  if otheragepref = 2 then otheragepref = '16-18'
  if otheragepref = 3 then otheragepref = '19-25'
  if otheragepref = 4 then otheragepref = '26-30'
  if otheragepref = 5 then otheragepref = '31-35'
  if otheragepref = 6 then otheragepref = '36-40'
  if otheragepref = 7 then otheragepref = '41-45'
  if otheragepref = 8 then otheragepref = '46-50'
  if otheragepref = 9 then otheragepref = '51-60'
  if otheragepref = 10 then otheragepref = 'Other'

  CALL TRANSMIT ''
  CALL TRANSMIT 'Alias:       'otheralias
  CALL TRANSMIT 'Gender:      'othergender
  CALL TRANSMIT 'Sex Pref:    'otherpref
  CALL TRANSMIT 'Weight:      'otherweight
  CALL TRANSMIT 'Weight Pref: 'otherweightpref
  CALL TRANSMIT 'Eye color:   'othereye
  CALL TRANSMIT 'Height:      'otherheight
  CALL TRANSMIT 'Age:         'otherage
  CALL TRANSMIT 'Age Pref:    'otheragepref
  CALL TRANSMIT 'Music Pref:  'otherMUSIC
  CALL TRANSMIT 'Hobby:       'otherHOBBY
  CALL TRANSMIT 'Salary:      $'otherSALARY
  CALL TRANSMIT ''
  CALL TRANSMIT 'Like to do on 1ST date:  'otherFIRSTDATE
  CALL TRANSMIT 'Believes in womens Lib?  'otherWOMENSLIB
  CALL TRANSMIT 'Political preference?    'otherPOLITICS
  CALL TRANSMIT 'All races are equal?     'otherRACIALEQUAL
  CALL TRANSMIT 'Will date another race?  'otherRACEPREF
  CALL TRANSMIT 'Race:                    'otherRACE
  CALL TRANSMIT 'Abortion views:          'otherABORT
  CALL TRANSMIT ''
  OPTIONS PROMPT 'Hit RETURN: '
  PULL junk
  SIGNAL MAIN
  END
CALL TRANSMIT 'User not FOUND!'
CALL TRANSMIT ''
OPTIONS PROMPT 'Hit RETURN: '
PULL junk
RETURN


YOURSTATS:
  call open file,direct'users/'ALIAS,'R'
       NAME = READLN(FILE)
       alias1 = readln(file)
       pass = readln(file)
       gender = readln(file)
       pref = readln(file)
       weight = readln(file)
       weightpref = readln(file)
       eye = readln(file)
       height = readln(file)
       age = readln(file)
       agepref = readln(file)
       MUSIC = readln(file)
       HOBBY = readln(file)
       SALARY = readln(file)
       FIRSTDATE = readln(file)
       WOMENSLIB = readln(file)
       POLITICS = readln(file)
       RACIALEQUAL = readln(file)
       RACEPREF = readln(file)
       RACE = readln(file)
       ABORT = readln(file)



  call close file
  if gender = 1 then gender = 'Male'
  if gender = 2 then gender = 'Female'
  if pref = 1 then pref = 'HeteroSexual'
  if pref = 2 then pref = 'HomoSexual'
  if pref = 3 then pref = 'BiSexual'
  if pref = 4 then pref = 'TrySexual'
  if weight = 1 then weight = '90-100'
  if weight = 2 then weight = '101-120'
  if weight = 3 then weight = '121-130'
  if weight = 4 then weight = '131-150'
  if weight = 5 then weight = '151-180'
  if weight = 6 then weight = '181-210'
  if weight = 7 then weight = 'over 210'
  if weightpref = 1 then weightpref = '90-100'
  if weightpref = 2 then weightpref = '101-120'
  if weightpref = 3 then weightpref = '121-130'
  if weightpref = 4 then weightpref = '131-150'
  if weightpref = 5 then weightpref = '151-180'
  if weightpref = 6 then weightpref = '181-210'
  if weightpref = 7 then weightpref = 'over 210'
  if eye = 1 then eye = 'Blue'
  if eye = 2 then eye = 'Grey'
  if eye = 3 then eye = 'Brown'
  if eye = 4 then eye = 'Green'
  if eye = 5 then eye = 'Hazel'
  if eye = 6 then eye = 'Black'
  if height = 1 then height = '4ft-4.5ft'
  if height = 2 then height = '4.6ft-5ft'
  if height = 3 then height = '5.1ft-5.5ft'
  if height = 4 then height = '5.6ft-5.8ft'
  if height = 5 then height = '5.9ft-5.11ft'
  if height = 6 then height = '6ft-6.5ft'
  if height = 7 then height = '6.6ft-UP'

  if age = 1 then age = '13-15'
  if age = 2 then age = '16-18'
  if age = 3 then age = '19-25'
  if age = 4 then age = '26-30'
  if age = 5 then age = '31-35'
  if age = 6 then age = '36-40'
  if age = 7 then age = '41-45'
  if age = 8 then age = '46-50'
  if age = 9 then age = '51-60'
  if age = 10 then age = 'Other'

  if agepref = 1 then agepref = '13-15'
  if agepref = 2 then agepref = '16-18'
  if agepref = 3 then agepref = '19-25'
  if agepref = 4 then agepref = '26-30'
  if agepref = 5 then agepref = '31-35'
  if agepref = 6 then agepref = '36-40'
  if agepref = 7 then agepref = '41-45'
  if agepref = 8 then agepref = '46-50'
  if agepref = 9 then agepref = '51-60'
  if agepref = 10 then agepref = 'Other'

  if music = 1 then music = 'Rock and Roll'
  if music = 2 then music = 'Punk/New Wave'
  if music = 3 then music = 'Heavy Metal dude!'
  if music = 4 then music = 'Classical'
  if music = 5 then music = 'Country'
  if music = 6 then music = 'Opera'
  if music = 7 then music = 'Soft Rock'
  if music = 8 then music = 'Jazz'
  if music = 9 then music = 'Rap'
  if music =10 then music = 'Folk'
  if music =11 then music = 'Other'
  if music =12 then music = 'All Music'

  if hobby = 1 then hobby = 'Sex'
  if hobby = 2 then hobby = 'Sports or exercise'
  if hobby = 3 then hobby = 'Parties!!'
  if hobby = 4 then hobby = 'Watching TV'
  if hobby = 5 then hobby = 'Computers'
  if hobby = 6 then hobby = 'Reading'
  if hobby = 7 then hobby = 'All of the above'
  if hobby = 8 then hobby = 'Other'

  if salary = 1 then salary = 'under 5000'
  if salary = 2 then salary = '5001-10000'
  if salary = 3 then salary = '10001-15000'
  if salary = 4 then salary = '15001-25000'
  if salary = 5 then salary = '25001-40000'
  if salary = 6 then salary = '40001-60000'
  if salary = 7 then salary = '60001-up'

  if firstdate = 1 then firstdate = 'go to a movie'
  if firstdate = 2 then firstdate = 'go out to dinner'
  if firstdate = 3 then firstdate = 'go dancing/bars'
  if firstdate = 4 then firstdate = 'go to a play/concert'
  if firstdate = 5 then firstdate = 'I hit the MOTEL ROOM with my date'
  if firstdate = 6 then firstdate = 'I go and meet the m'
  if firstdate = 7 then firstdate = 'I have never been on a date'
  if firstdate = 8 then firstdate = 'Other'

  if womenslib = 1 then womenslib = 'YES'
  if womenslib = 2 then womenslib = 'No'
  if womenslib = 3 then womenslib = 'No opinion'
 
  if politics = 1 then politics = 'Democrat'
  if politics = 2 then politics = 'Republican'
  if politics = 3 then politics = 'Communist'
  if politics = 4 then politics = 'Socialist'
  if politics = 5 then politics = 'Liberal'
  if politics = 6 then politics = 'Independent'
  if politics = 7 then politics = 'Other'

  if Racialequal = 1 then Racialequal = 'Heck YES!'
  if Racialequal = 2 then Racialequal = 'No.'
  if Racialequal = 3 then Racialequal = 'No Opinion'
  
  if racepref = 1 then racepref = 'Yes!'
  if racepref = 2 then racepref = 'No.'
  if racepref = 3 then racepref = 'Depends'

  if RACE = 1 then race = 'Hispanic'
  if RACE = 2 then race = 'Asian'
  if RACE = 3 then race = 'African American'
  if RACE = 4 then race = 'Native American'
  if RACE = 5 then race = 'Caucasion'
  if RACE = 6 then race = 'Newcomer'
  if RACE = 7 then race = 'Other'

  if abort = 1 then abort = 'Yes'
  if abort = 2 then abort = 'No'
  if abort = 3 then abort = 'only in certain circumstances'


  CALL TRANSMIT ''
  CALL TRANSMIT 'Name:        'NAME
  CALL TRANSMIT 'Alias:       'alias1
  CALL TRANSMIT 'Password:    'pass
  CALL TRANSMIT 'Gender:      'gender
  CALL TRANSMIT 'Sex Pref:    'pref
  CALL TRANSMIT 'Weight:      'weight
  CALL TRANSMIT 'Weight Pref: 'weightpref
  CALL TRANSMIT 'Eye color:   'eye
  CALL TRANSMIT 'Height:      'height
  CALL TRANSMIT 'Age:         'age
  CALL TRANSMIT 'Age Pref:    'agepref
  CALL TRANSMIT 'Music Pref:  'MUSIC
  CALL TRANSMIT 'Hobby:       'HOBBY
  CALL TRANSMIT 'Salary:      $'SALARY
  CALL TRANSMIT ''
  CALL TRANSMIT 'Like to do on 1ST date:  'FIRSTDATE
  CALL TRANSMIT 'Believes in womens Lib?  'WOMENSLIB
  CALL TRANSMIT 'Political preference?    'POLITICS
  CALL TRANSMIT 'All races are equal?     'RACIALEQUAL
  CALL TRANSMIT 'Will date another race?  'RACEPREF
  CALL TRANSMIT 'Race:                    'RACE
  CALL TRANSMIT 'Abortion views:          'ABORT
  CALL TRANSMIT ''
  OPTIONS PROMPT 'Hit RETURN: '
  PULL junk
RETURN


/* Procedure to REGISTER */  
REG: 
     OPTIONS PROMPT 'Verify ALIAS you wish to use: '
     PARSE PULL alias
     OPTIONS PROMPT 'Enter PASSWORD you wish to use: '
     PARSE PULL pass
RETURN 


/* Procedures for Errors, Logging off and re-entering Abyss */

SYNTAX:
   errorline='Line:' SIGL '   Error Code:' RC
   CALL TRANSMIT '*BOOM*  Got a syntax error.  PLEASE notify the sysop.'
   CALL TRANSMIT errorline
   call CloseShop
exit


CloseShop: procedure
   CALL TRANSMIT ' '
   CALL TRANSMIT ' '
   OPTIONS PROMPT 'Press RETURN:'
   PULL junk
   EXIT
return


TRANSMIT:
PARSE ARG string
SAY string'0D'x
RETURN;


checkBBS:
IF ADDRESS()~='BAUD' THEN RETURN 0
IF TIME('E')>secs THEN EXIT
dcd
IF RC=0 THEN EXIT
temp=secs-TIME('E')
IF temp<120 THEN SAY '*** Only' temp 'seconds left! ***'CR 
RETURN 0


Quit: procedure
BREAK_C:
SAY '0D'x
   exit
return
