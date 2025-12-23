/*

    STR.Top10.rexx - Makes Top 10 lists for E! BBS.

    Version 1.06

    Written By:  Douglas McLaughlin [Black Fire]
                 Robotech BBS
                 1-808-456-7745
                 Excelsior! Sysop #0300

    FidoNet   :  1:345/37.0
    AmigaNet  :  40:411/1.0

    Special Thanks to Kenneth Brill for the original Excelsior! program
    "Mega_ANSI_CPS," upon which this program is loosely based.

*/

options results
options failat 21

call pragma('w','n')

if ~show('l','rexxsupport.library') then do
  if ~addlib('rexxsupport.library',0,-30,0) then do
    print 'RexxSupport.Library missing! Can''t continue...'
    exit 10
  end
end

datapath = 'Doors:STR.Utils/'  /*  Path to data directory.          */

sysop    = 31  /*  Access levels ABOVE this don't get logged.       */
maxdays  = 30  /*  Delete from top lists after this many days.      */
maxcday  = 15  /*  Delete from cps lists after this many days.      */
topnum   = 10  /*  Maximum number of items per "Top10" list.        */
xtratime = 30  /*  If a user beaks a record, add this many minutes. */

realname = 0   /*  1 = Use the user's real name in all output.      *
                *  0 = Use the user's alias in all output.          */

graphnow = 0   /*  1 = Run STR.Graph.rexx synchroniously, this will *
                *      slow the user's logon.                       *
                *  0 = Do not run STR.Graph.rexx at all.  You must  *
                *      run the program from some other point.       *
                *      See the docs for more information on this.   */

/*
 * The following are the comments for approximately each 10% of the current
 * baud rate.  ie.  at 14,400 baud, a cps rate of 1440 would give comment 10.
 *
 */

comment.1  = 'Horrible. Why did you even bother?'
comment.2  = 'Pathetic. US Mail would have been faster!'
comment.3  = 'That was pretty lousy.'
comment.4  = 'Needs a lotta work.' 
comment.5  = '(YAWN).... OH your finally done....'
comment.6  = 'How about dropping by with a disk next time.' 
comment.7  = 'Getting there....'
comment.8  = 'Looks about right!'
comment.9  = 'Zooming right along!' 
comment.10 = 'Right on target!'
comment.11 = 'Vroom!'
comment.12 = 'Speed Demon!' 
comment.13 = 'Broke the sound barrier on that one!'
comment.14 = 'Outta Sight!' 
comment.15 = 'Lord Almighty!'
comment.16 = 'Flaming Phone Lines!'
comment.17 = 'Melting Modems!'
comment.18 = 'HOLY SHIT!'

today = date('b')
hour  = time('h')
inp = 'input'
out = 'output'

nolog = 0
getuser 24; baud   = result; baud1 = baud
getuser 15; access = result
getuser 1 ; alias  = result
getuser 3 ; name   = result
getuser 12; date   = result
getuser 4 ; city   = result
getuser 28; ansi   = result
getuser 23; node   = result
getuser 44; avgcps = result
getuser 37; ttldl  = result
getuser 35; ttlup  = result
getuser 36; bytedl = result
getuser 34; byteup = result
getuser 22; calls  = result
getuser 38; posts  = result

if realname = 0 then do
  temp = name
  name = alias
  alias = temp
end

online = word(date,3)
date = subword(date,1,2)

cpsdl = 0
cpsup = 0

getwhat node; location = result
xfer = upper(left(location,1))
if pos('Logging',location) > 0 then avgcps = 0
if pos('STR.Top10',location) = 0 & baud = 'LOCAL' then exit
if baud = 'LOCAL' | baud > 28800 then do
  baud = '14400'
  avgcps = random(1000,2000,time('s'))
  nolog = 1
  xfer = 'D'
end
if xfer = 'D' then do
  cpsdl = avgcps
  type = 'Download'
end
else do
  cpsup = avgcps
  type = 'Upload'
end

datafile = datapath'STR.Top10.data'
hourfile = datapath'STR.Top10.hour'
baudfile = datapath'STR.Top10.baud'
highfile = datapath'STR.Top10.high'
 cpsfile = datapath'STR.Top10.cps'xfer
 newfile = datapath'STR.Top10.new'
lastfile = datapath'STR.Top10.last'
rexxfile = datapath'STR.Graph.rexx'

if exists('T:STR.Top10.wait') then do
  do until ~exists('T:STR.Top10.wait') 
    call delay(800)
  end
end
address command 'Echo >T:STR.Top10.wait "Hold on a bit yet...."'

if avgcps > 0 then call docps()

if access > sysop then call cleanup(0)

if ~exists(datafile) then do
  if ~open(out,datafile,'w') then do
    sysoplog 'Couldn''t open "'datafile'"!.'
    call cleanup(10)
  end
  call writeln(out,name';'alias';'city';'baud';'ttldl';'ttlup';'bytedl';'byteup';'calls';'posts';'today)
  call close(out)
end

if ~open(inp,datafile,'r') then do
  sysoplog 'Couldn''t open "'datafile'"!.'
  call cleanup(10)
end
if ~open(out,'T:STR.Top10.D','w') then do
  sysoplog 'Couldn''t open "T:STR.Top10.D"!.'
  call cleanup(10)
end

found = 0
line = readln(inp)
do until eof(inp)
  parse var line na';'al';'ci';'ba';'fd';'fu';'bd';'bu';'ca';'po';'to
  if to+maxdays < today then do
    line = readln(inp)
    iterate
  end
  if (na = name | na = alias) & found = 1 then do
    line = readln(inp)
    iterate
  end
  if na = name | na = alias then do
    found = 1
    if ba > baud1 then baud1 = ba
    if (cpsdl = 0) | (cd > cpsdl) then do
      cpsdl = cd
      baud1 = baud
    end
    if (cpsup = 0) | (cu > cpsup) then do
      cpsup = cu
      baud1 = baud
    end
    call writeln(out,name';'alias';'city';'baud1';'ttldl';'ttlup';'bytedl';'byteup';'calls';'posts';'today)
    line = readln(inp)
    iterate
  end
  call writeln(out,line)
  line = readln(inp)
end
if found = 0 then call writeln(out,name';'alias';'city';'baud';'ttldl';'ttlup';'bytedl';'byteup';'calls';'posts';'today)
call close(out)
call close(inp)

call delete(datafile)
address command 'Sort T:STR.Top10.D 'datafile
call delete('T:STR.Top10.D')

if ~exists(baudfile) then do
  if ~open(out,baudfile,'w') then do
    sysoplog 'Couldn''t open "'baudfile'"!.'
    call cleanup(10)
  end
  call writeln(out,baud';0')
  call close(out)
end

if ~open(inp,baudfile,'r') then do
  sysoplog 'Couldn''t open "'baudfile'"!.'
  call cleanup(10)
end
if ~open(out,'T:STR.Top10.B','w') then do
  sysoplog 'Couldn''t open "T:STR.Top10.B"!.'
  call cleanup(10)
end

found = 0
line = readln(inp)
do until eof(inp)
  parse var line baud1';'count
  if (baud+0) = (baud1+0) then do
    found = 1
    count = count+1
  end
  call writeln(out,baud1';'count)
  line = readln(inp)
end
if found = 0 then call writeln(out,baud';1')
call close(out)
call close(inp)

call delete(baudfile)
address command 'Sort T:STR.Top10.B 'baudfile' Numeric'
call delete('T:STR.Top10.B')

if ~exists(hourfile) then do
  if ~open(out,hourfile,'w') then do
    sysoplog 'Couldn''t open "'hourfile'"!.'
    call cleanup(10)
  end
  do i = 0 to 23
    call writeln(out,i';0')
  end
  call close(out)
end

if ~open(inp,hourfile,'r') then do
  sysoplog 'Couldn''t open "'hourfile'"!.'
  call cleanup(10)
end
if ~open(out,'T:STR.Top10.H','w') then do
  sysoplog 'Couldn''t open "T:STR.Top10.H"!.'
  call cleanup(10)
end

found = 0
line = readln(inp)
do until eof(inp)
  parse var line hour1';'count
  if (hour+0) = (hour1+0) then do
    found = 1
    count = count+1
  end
  call writeln(out,hour1';'count)
  line = readln(inp)
end
if found = 0 then call writeln(out,hour';1')
call close(out)
call close(inp)

call delete(hourfile)
address command 'Sort T:STR.Top10.H 'hourfile' Numeric'
call delete('T:STR.Top10.H')

if calls = 0 then do
  if ~exists(newfile) then do
    if ~open(out,newfile,'w') then do
      sysoplog 'Couldn''t open "'newfile'"!.'
      call cleanup(10)
    end
    do i = 1 to topnum
      call writeln(out,'Wendy Who;Whoville CA;Sun 25-Dec-1994;28800')
    end
    call close(out)
  end

  if ~open(inp,newfile,'r') then do
    sysoplog 'Couldn''t open "'newfile'"!.'
    call cleanup(10)
  end
  if ~open(out,'T:STR.Top10.N','w') then do
    sysoplog 'Couldn''t open "T:STR.Top10.N"!.'
    call cleanup(10)
  end

  new. = ''
  new.1 = readln(inp)
  do i = 2 to topnum until eof(inp)
    new.i = readln(inp)
  end

  found = 0
  do i = 2 to topnum
    parse var new.i na';'ci';'da';'ba
    if name = na then found = 1
    call writeln(out,new.i)
  end
  if found = 0 then call writeln(out,name';'city';'date';'baud)
  call close(out)
  call close(inp)

  call delete(newfile)
  address command 'Copy T:STR.Top10.N 'newfile
  call delete('T:STR.Top10.N')
  drop new.
end

if ~exists(lastfile) then do
  if ~open(out,lastfile,'w') then do
    sysoplog 'Couldn''t open "'lastfile'"!.'
    call cleanup(10)
  end
  do i = 1 to topnum
    call writeln(out,'Wendy Who;0;0;0;0;24:00;28800')
  end
  call close(out)
end

if ~open(inp,lastfile,'r') then do
  sysoplog 'Couldn''t open "'lastfile'"!.'
  call cleanup(10)
end
if ~open(out,'T:STR.Top10.L','w') then do
  sysoplog 'Couldn''t open "T:STR.Top10.L"!.'
  call cleanup(10)
end

last. = ''
last.1 = readln(inp)
do i = 2 to topnum until eof(inp)
  last.i = readln(inp)
end

found = 0
do i = 2 to topnum
  call writeln(out,last.i)
  line = readln(inp)
end
call writeln(out,name';'calls';'posts';'byteup';'bytedl';'online';'baud)
call close(out)
call close(inp)

call delete(lastfile)
address command 'Copy T:STR.Top10.L 'lastfile
call delete('T:STR.Top10.L')
drop last.

if graphnow = 1 then address command 'Rx 'rexxfile' 'address()' 'baud

call cleanup(0)

/*
 *  End of first part of program.  Execution stops here during logon.
 *
 *  Start of second part.  Execution takes up here only after an upload
 *  or download.
 *
 */

docps:

if access > sysop & baud ~= 'LOCAL' then nolog = 1

if ~exists(cpsfile) then do
  if ~open(out,cpsfile,'w') then do
    sysoplog 'Couldn''t open "'cpsfile'"!.'
    call cleanup(10)
  end
  call writeln(out,baud';'name';'avgcps';'date';'today)
  call close(out)
end

if ~open(inp,cpsfile,'r') then do
  sysoplog 'Couldn''t open "'cpsfile'"!.'
  call cleanup(10)
end
if ~open(out,'T:STR.Top10.C','w') then do
  sysoplog 'Couldn''t open "T:STR.Top10.C"!.'
  call cleanup(10)
end

found = 0
newrecord = 0
line = readln(inp)
do until eof(inp)
  parse var line ba';'na';'cp';'da';'to
  if to+maxcday < today then do
    line = readln(inp)
    iterate
  end
  if baud ~= ba then do
    call writeln(out,line)
    line = readln(inp)
    iterate
  end
  found = 1
  if avgcps > cp then do
    newrecord = 1
    parse var line lastbaud';'lastname';'lastcps';'lastday';'daysleft
    call writeln(out,baud';'name';'avgcps';'date';'today)
  end
  else call writeln(out,line)
  line = readln(inp)
end
if found = 0 then call writeln(out,baud';'name';'avgcps';'date';'today)
call close(out)
call close(inp)

if nolog = 0 then do
  call delete(cpsfile)
  address command 'Sort T:STR.Top10.C 'cpsfile' Numeric'
end
call delete('T:STR.Top10.C')

if ~exists(highfile) then do
  if ~open(out,highfile,'w') then do
    sysoplog 'Couldn''t open "'highfile'"!.'
    call cleanup(10)
  end
  call writeln(out,avgcps';'name';'baud';'date';'today)
  call close(out)
end

if ~open(inp,highfile,'r') then do
  sysoplog 'Couldn''t open "'highfile'"!.'
  call cleanup(10)
end
if ~open(out,'T:STR.Top10.H','w') then do
  sysoplog 'Couldn''t open "T:STR.Top10.H"!.'
  call cleanup(10)
end

found = 0
userhigh = 0
userrecord = 0
line = readln(inp)
do until eof(inp)
  parse var line hi';'na';'ba';'da';'to
  if to+maxcday < today then do
    line = readln(inp)
    iterate
  end
  if baud ~= ba then do
    call writeln(out,line)
    line = readln(inp)
    iterate
  end
  if name = na then do
    found = 1
    parse var line userhigh';'username';'userbaud';'userday';'daysleft
    if avgcps > hi then do
      userrecord = 1
      call writeln(out,avgcps';'name';'baud';'date';'today)
      line = readln(inp)
      iterate
    end
  end
  call writeln(out,line)
  line = readln(inp)
end
if found = 0 then call writeln(out,avgcps';'name';'baud';'date';'today)
call close(out)
call close(inp)

if nolog = 0 then do
  call delete(highfile)
  address command 'Sort T:STR.Top10.H 'highfile' Numeric'
end
call delete('T:STR.Top10.H')

if ansi > 1 then call ansinum()
else call asciinum()

rate = ((avgcps/(baud/10))*100)%1; r = rate%10
if r < 1 then r = 1
else if r > 18 then r = 18

line. = '\q1'
do k = 1 to 4+ansi
  do i = 1 to length(avgcps)
    m = substr(avgcps,i,1)
    line.k = line.k||n.m.ansi.k' '
  end
  line.k = line.k'  \c3'cps.ansi.k
end

sendstring '\f1\c7\@6'
print center('The average rate for this 'baud' 'type' was',76)
do i=1 to 4+ansi
  print center(line.i,82)
end

print '\c7\z4'center(comment.r,79)'\z0'

sendstring '[13C\c50\c7%\>7\c525\c7%\>7\c550\c7%\>7\c5'
print '75\c7%\>6\c5100\c7%\>6\c5125\c7%\c6'
do i=1 to 3
  print sh.ansi.i
end

color='54321'
sendstring '\^2\>e'
d=(rate*.4)%1
if d>49 then d=49
else if d<1 then d=1
do i=1 to d
  sendstring '\c'substr(color,(i%10)+1,1)sh.ansi.4
end
print '\@4\>1\c7'rate'%\n1'

if ~open(inp,highfile,'r') then do
  sysoplog 'Couldn''t open "'highfile'" after sort!'
  call cleanup(10)
end

userrank = 1
line = readln(inp)
do i = 1 until eof(inp)
  parse var line hi';'na';'ba';'da';'to
  if name = na & baud = ba then userrank = i
  line = readln(inp)
end
close(inp)
userrank = i-userrank+1
if userrank < 1 then userrank = 1

if userrecord then print '\c2'center('You have beat your personal best of 'userhigh' cps!',79)
else print '\c2'center('Your personal best is 'userhigh' cps.  Your rank is #'userrank' of 'i'.',79)

if newrecord ~= 1 then do
  if ~open(inp,cpsfile,'r') then do
    sysoplog 'Couldn''t open "'cpsfile'" after sort!'
    call cleanup(10)
  end
  print
  line = readln(inp)
  do until eof(inp)
    parse var line ba';'na';'cp';'da';'to
    if substr(da,6,1) = '-' then da = word(da,1)'  'word(da,2)
    if to+maxcday-today = 1 then plural = ' '
    else plural = 's'
    sendstring '\@4\c6 Record at 'left(ba,5)'\c3: 'right(cp,4)' cps '
    print '\c6By\c3: 'left(na,15)' \c6On\c3: 'substr(da,5)'\c6! (\c4'right(to+maxcday-today,2)' day'plural' left\c6)'
    line = readln(inp)
  end
  close(inp)
end
else do
  da = lastday
  if substr(da,6,1) = '-' then da = word(da,1)'  'word(da,2)
  if substr(date,6,1) = '-' then date = word(date,1)'  'word(date,2)
  sendstring '\@4\n1\c1 You broke the \c7CPS \c1record of\c3: 'left(lastcps,4)' '
  print '\c1Set by\c3: 'left(lastname,18)' \c1On\c3: 'substr(da,5)'\c1!'
  sendstring '\n1\c7 NEW \c1Record \c7CPS \c1at this baud\c3: 'left(avgcps,4)' '
  print '\c1Set by\c3: 'left(name,18)' \c1On\c3: 'substr(date,5)'\c1!'
  getuser 7; ttltime = result
  addtime xtratime
  getuser 7; ttlnew = result
  print '\n1'center('As a reward, your online time is increased from 'ttltime' to 'ttlnew'.',79)
end

sendstring '\n1     \c6STR\c7.\c6Top10  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\w3'

call cleanup(0)

asciinum:

	n.0.1.1=' ##### ' ; n.0.1.2='#   ###' ; n.0.1.3='#  #  #' ; n.0.1.4='###   #' ; n.0.1.5=' ##### '
	n.1.1.1=' ## '    ; n.1.1.2='### '    ; n.1.1.3=' ## '    ; n.1.1.4=' ## '    ; n.1.1.5='####'
	n.2.1.1=' ##### ' ; n.2.1.2='##   ##' ; n.2.1.3='   ##  ' ; n.2.1.4='###    ' ; n.2.1.5='#######'
	n.3.1.1=' ##### ' ; n.3.1.2='##   ##' ; n.3.1.3='   ### ' ; n.3.1.4='##   ##' ; n.3.1.5=' ##### '
	n.4.1.1='##  ## ' ; n.4.1.2='##  ## ' ; n.4.1.3='#######' ; n.4.1.4='    ## ' ; n.4.1.5='    ## '
	n.5.1.1='###### ' ; n.5.1.2='##     ' ; n.5.1.3='###### ' ; n.5.1.4='     ##' ; n.5.1.5='###### '
	n.6.1.1='  #### ' ; n.6.1.2=' ##    ' ; n.6.1.3='###### ' ; n.6.1.4='##   ##' ; n.6.1.5=' ##### '
	n.7.1.1='#######' ; n.7.1.2='     ##' ; n.7.1.3='    ## ' ; n.7.1.4='   ##  ' ; n.7.1.5='  ##   '
	n.8.1.1=' ##### ' ; n.8.1.2='##   ##' ; n.8.1.3=' ##### ' ; n.8.1.4='##   ##' ; n.8.1.5=' ##### '
	n.9.1.1=' ##### ' ; n.9.1.2='##   ##' ; n.9.1.3=' ######' ; n.9.1.4='    ## ' ; n.9.1.5=' ####  '
	cps.1.1=' #####  ######   ##### ' ; cps.1.2='##   ## ##   ## ##     ' ; cps.1.3='##      ######   ##### '
	cps.1.4='##   ## ##           ##' ; cps.1.5=' #####  ##       ##### '
	sh.1.1='[14C|--------|---------|---------|--------|----------|' ; sh.1.4='*'
	sh.1.2='[14C|        |         |         |        |          |' ; ansi=1
	sh.1.3='[14C|--------`---------`---------`--------`----------|'

return

ansinum:

	n.0.0.1=' ÜÜÜÜÜ ' ; n.0.0.2='ÛÛ  ÜÛÛ' ; n.0.0.3='ÛÛÜß ÛÛ' ; n.0.0.4='ßÛÜÜÜÛß'
	n.1.0.1=' ÜÜ '    ; n.1.0.2='ßÛÛ '    ; n.1.0.3=' ÛÛ '    ; n.1.0.4='ÜÛÛÜ'
	n.2.0.1=' ÜÜÜÜÜ ' ; n.2.0.2='ßß   ÛÛ' ; n.2.0.3='ÜÛßßßß ' ; n.2.0.4='ÛÛÜÜÜÜÜ'
	n.3.0.1=' ÜÜÜÜÜ ' ; n.3.0.2='ßß   ÛÛ' ; n.3.0.3='   ßßÛÜ' ; n.3.0.4='ßÛÜÜÜÛß'
	n.4.0.1='ÜÜ   ÜÜ' ; n.4.0.2='ÛÛ   ÛÛ' ; n.4.0.3=' ßßßßÛÛ' ; n.4.0.4='     ÛÛ'
	n.5.0.1='ÜÜÜÜÜÜÜ' ; n.5.0.2='ÛÛÜÜÜÜ ' ; n.5.0.3='     ÛÛ' ; n.5.0.4='ßÛÜÜÜÛß'
	n.6.0.1='  ÜÜÜ  ' ; n.6.0.2='ÜÛß    ' ; n.6.0.3='ÛÛßßßÛÜ' ; n.6.0.4='ßÛÜÜÜÛß'
	n.7.0.1='ÜÜÜÜÜÜÜ' ; n.7.0.2='     ÛÛ' ; n.7.0.3='   ÜÛß ' ; n.7.0.4='  ÛÛ   '
	n.8.0.1=' ÜÜÜÜÜ ' ; n.8.0.2='ÛÛ   ÛÛ' ; n.8.0.3='ÜÛßßßÛÜ' ; n.8.0.4='ßÛÜÜÜÛß'
	n.9.0.1=' ÜÜÜÜÜ ' ; n.9.0.2='ÛÛ   ÛÛ' ; n.9.0.3=' ßßßßÛÛ' ; n.9.0.4='ßÛÜÜÜÛß'
	cps.0.1=' ÜÜÜÜÜ  ÜÜÜÜÜÜ   ÜÜÜÜÜ ' ; cps.0.2='ÛÛ   ßß ÛÛ   ÛÛ ÛÛ   ßß'
	cps.0.3='ÛÛ      ÛÛßßßß   ßßßßÛÜ' ; cps.0.4='ßÛÜÜÜÛß ÛÛ      ßÛÜÜÜÛß'
	sh.0.1='[14CÃÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄ´' ; sh.0.4='Û'
	sh.0.2='[14C³        ³         ³         ³        ³          ³' ; ansi=0
	sh.0.3='[14CÃÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ´'

return

cleanup:

parse arg error

call delete('T:STR.Top10.wait')

exit error
