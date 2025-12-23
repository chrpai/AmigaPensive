/*

    STR.Graph.rexx - Creates charts & graphs from STR.Top10.rexx's data.

    Version 1.06

*/

options results
options failat 21

parse arg bbsport' 'baud

address 'MASTER_REXX1'

if ~show('l','rexxsupport.library') then do
  if ~addlib('rexxsupport.library',0,-30,0) then do
    print 'RexxSupport.Library missing! Can''t continue...'
    exit 10
  end
end

datapath = 'Doors:STR.Utils/'  /*  Path to STR.Top10.rexx's data files.   */

maxtopnum = 10         /*  Maximum number of items per Top "10" list.     */
topfiledn = 100        /*  Download this many files to be on God's list.  */
topfileup = 50         /*  Upload this many files to be on God's list.    */
topbytedn = 20000000   /*  Download this many bytes to be on God's list.  */
topbyteup = 10000000   /*  Upload this many bytes to be on God's list.    */

path 5; textpath = result

inp = 'input'
out = 'output'
inp1 = 'input1'
inp2 = 'input2'
out1 = 'output1'
out2 = 'output2'
hour = time('h')

datafile = datapath'STR.Top10.data'
hourfile = datapath'STR.Top10.hour'
baudfile = datapath'STR.Top10.baud'
highfile = datapath'STR.Top10.high'
Dcpsfile = datapath'STR.Top10.cpsD'
Ucpsfile = datapath'STR.Top10.cpsU'
 newfile = datapath'STR.Top10.new'
lastfile = datapath'STR.Top10.last'

if bbsport ~= '' then do
  address value bbsport
  sendstring '\n1\@6\q1Currently Updating: \c6STR\c7.\c6LastCallers'
end

if ~open(inp,lastfile,'r') then do
  logentry 'Couldn''t open "'lastfile'"!'
  exit 10
end

last. = ''
line = readln(inp)
do i = 1 until eof(inp)
  parse var line last.1.i';'last.2.i';'last.3.i';'last.4.i';'last.5.i';'last.6.i';'last.7.i
  ampm = right(last.6.i,2)
  if ampm ~= 'am' & ampm ~= 'pm' then last.6.i = last.6.i||'  '
  line = readln(inp)
end
call close(inp)

if ~open(out1,textpath'STR.LastCallers.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.LastCallers.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.LastCallers','w') then do
  logentry 'Couldn''t open "'textpath'STR.LastCallers"!'
  exit 10
end

call writeln(out1,'\n1\c4ษออออัออออออออออออออออออออออัอออออัอออออัอออออออออัอออออออออัออออออออัออออออป')
call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
call writeln(out1,'\c4บ    ณ     \c3Last Callers     \c4ณ\c3Call#\c4ณ\c3#Msgs\c4ณ\c3BytesUp''d\c4ณ\c3BytesDl''d\c4ณ  \c3Time  \c4ณ \c3Baud \c4บ')
call writeln(out2,'\c4|    |     \c3Last Callers     \c4|\c3Call#\c4|\c3#Msgs\c4|\c3BytesUp''d\c4|\c3BytesDl''d\c4|  \c3Time  \c4| \c3Baud \c4|')
call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤลฤฤฤฤฤลฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤลฤฤฤฤฤฤถ')
call writeln(out2,'\c4|----|----------------------|-----|-----|---------|---------|--------|------|')
do j = i to 1 by -1
  k = i-j+1
  call writech(out1,'\c4บ \c3'left(k,2)' \c4ณ\c7'left(last.1.j,22)'\c4ณ\c7')
  call writech(out2,'\c4| \c3'left(k,2)' \c4|\c7'left(last.1.j,22)'\c4|\c7')
  call writech(out1,right(last.2.j,5)'\c4|\c7'right(last.3.j,5)'\c4|\c7')
  call writech(out2,right(last.2.j,5)'\c4ณ\c7'right(last.3.j,5)'\c4ณ\c7')
  call writech(out1,right(last.4.j,9)'\c4ณ\c7'right(last.5.j,9)'\c4ณ\c7')
  call writech(out2,right(last.4.j,9)'\c4|\c7'right(last.5.j,9)'\c4|\c7')
  call writeln(out1,right(last.6.j,8)'\c4ณ\c7'right(last.7.j,6)'\c4บ')
  call writeln(out2,right(last.6.j,8)'\c4|\c7'right(last.7.j,6)'\c4|')
end
call writeln(out1,'\c4ศออออฯออออออออออออออออออออออฯอออออฯอออออฯอออออออออฯอออออออออฯออออออออฯออออออผ')
call writeln(out2,'\c4`---------------------------------------------------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6LastCallers \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6LastCallers \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

drop last.

if bbsport ~= '' then sendstring '\h9\h2NewCallers'

if open(inp,newfile,'r') then do
  new. = ''
  line = readln(inp)
  do i = 1 until eof(inp)
    parse var line new.1.i';'new.2.i';'new.3.i';'new.4.i
    if substr(new.3.i,6,1) = '-' then new.3.i = word(new.3.i,1)'  'word(new.3.i,2)
    if length(word(new.2.i,words(new.2.i))) = 2 then new.2.i = left(subword(new.2.i,1,words(new.2.i)-1),19)||word(new.2.i,words(new.2.i))
    line = readln(inp)
  end
  call close(inp)

  if ~open(out1,textpath'STR.NewCallers.ibm','w') then do
    logentry 'Couldn''t open "'textpath'STR.NewCallers.ibm"!'
    exit 10
  end

  if ~open(out2,textpath'STR.NewCallers','w') then do
    logentry 'Couldn''t open "'textpath'STR.NewCallers"!'
    exit 10
  end

  call writeln(out1,'\n1\c4ษออออัออออออออออออออออออออออัอออออออออออออออออออออัออออออออออออออออออัออออออป')
  call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
  call writeln(out1,'\c4บ    ณ     \c3New Callers      \c4ณ    \c3Calling From     \c4ณ    \c3Called On     \c4ณ \c3Baud \c4บ')
  call writeln(out2,'\c4|    |     \c3New Callers      \c4|    \c3Calling From     \c4|    \c3Called On     \c4| \c3Baud \c4|')
  call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤถ')
  call writeln(out2,'\c4|----|----------------------|---------------------|------------------|------|')
  do j = i to 1 by -1
    k = i-j+1
    call writech(out1,'\c4บ \c3'left(k,2)' \c4ณ\c7'left(new.1.j,22)'\c4ณ\c7')
    call writech(out2,'\c4| \c3'left(k,2)' \c4|\c7'left(new.1.j,22)'\c4|\c7')
    call writech(out1,left(new.2.j,21)'\c4ณ\c7'left(new.3.j,18)'\c4ณ\c7')
    call writech(out2,left(new.2.j,21)'\c4|\c7'left(new.3.j,18)'\c4|\c7')
    call writeln(out1,right(new.4.j,6)'\c4บ')
    call writeln(out2,right(new.4.j,6)'\c4|')
  end
  call writeln(out1,'\c4ศออออฯออออออออออออออออออออออฯอออออออออออออออออออออฯออออออออออออออออออฯออออออผ')
  call writeln(out2,'\c4`---------------------------------------------------------------------------''')
  call writeln(out1,'  \c6STR\c7.\c6NewCallers  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
  call writeln(out2,'  \c6STR\c7.\c6NewCallers  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
  call close(out2)
  call close(out1)

  drop new.
end

if bbsport ~= '' then sendstring '\h9\h1BaudGraph'

if ~open(inp,baudfile,'r') then do
  logentry 'Couldn''t open "'baudfile'"!'
  exit 10
end

baud. = 0
total = 0
count. = 0
line = readln(inp)
do i = 1 until eof(inp)
  parse var line baud.i';'count.i
  total = total+count.i
  line = readln(inp)
end
call close(inp)

if ~open(out1,textpath'STR.BaudGraph.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.BaudGraph.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.BaudGraph','w') then do
  logentry 'Couldn''t open "'textpath'STR.BaudGraph"!'
  exit 10
end

do j = 1 to i
  percent  = trunc((count.j/total)+.005,2)
  percent1 = trunc(percent*70)
  percent2 = right(trunc(percent*100),2,'0')
  color = '4'
  if baud = baud.j then color = '5'
  if (percent2 < 1 & i > 1) & baud ~= baud.j then iterate
  if j = 1 then do
    call writeln(out1,'\n1      \c4า')
    call writeln(out2,'\n1      \c4.')
  end
  else do
    call writeln(out1,'      \c4บ')
    call writeln(out2,'      \c4|')
  end
  call writech(out1,'\c3'left(baud.j'\c4',9,'อ')'ฮ\c'color)
  call writech(out2,'\c3'left(baud.j'\c4',9,'-')'+\c7\z'color)
  if percent1 > 3 then do
    do k = 1 to percent1-3
      call writech(out1,'Û')
      call writech(out2,'*')
    end
  end
  if percent2 < 1 then do
    call writeln(out1,'\c6[\c2'count.j'\c6]')
    call writeln(out2,'\z0\c6[\c2'count.j'\c6]')
  end
  else do
    call writeln(out1,'\c7\z'color||percent2'%\z0 \c6[\c2'count.j'\c6]')
    call writeln(out2,'\c7\z'color||percent2'%\z0 \c6[\c2'count.j'\c6]')
  end
end
call writeln(out1,'      \c4ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ'right('\c2'total,9,'ฤ')'\c4ฤฤฤ')
call writeln(out2,'      \c4`-------------------------------------------------------------'right('\c2'total,9,'-')'\c4---')
call writeln(out1,'   \c6STR\c7.\c6BaudGraph  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'   \c6STR\c7.\c6BaudGraph  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

drop baud. count.

if bbsport ~= '' then sendstring '\h9HourGraph\c7...'

if ~open(inp,hourfile,'r') then do
  logentry 'Couldn''t open "'hourfile'"!'
  exit 10
end

hour. = 0
total = 0
count. = 0
line = readln(inp)
do i = 0 until eof(inp)
  parse var line hour.i';'count.i
  total = total+count.i
  line = readln(inp)
end
call close(inp)

if ~open(out1,textpath'STR.HourGraph.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.HourGraph.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.HourGraph','w') then do
  logentry 'Couldn''t open "'textpath'STR.HourGraph"!'
  exit 10
end

max = 0
do i = 0 to 23
  if count.i/total > max then max = count.i/total
end

tall = 18
offset = 0
if trunc((max/tall)+.005) = 0 then offset = 1
call writeln(out1,'\n1    \c4ึฤ\c2'total'\c4ฤ')
call writeln(out2,'\@6\n1    \c4.-\c2'total'\c4-')
do i = tall to 1 by -1
  call writech(out1,'\c3'right(trunc(((i+offset)*(max/tall)+.005)*100),3)'\c7%\c4บ ')
  call writech(out2,'\c3'right(trunc(((i+offset)*(max/tall)+.005)*100),3)'\c7%\c4| ')
  do j = 0 to 23
    color = '4'
    if j = hour then color = '5'
    k = i-1
    if count.j/total > k*(max/tall) & count.j/total ~> i*(max/tall) & count.j > 0 then do
      call writech(out1,'\c2'left(count.j,3))
      call writech(out2,'\c2'left(count.j,3))
      iterate
    end
    if count.j/total > i*(max/tall) then do
      k = i+1
      if count.j/total ~> k*(max/tall) & count.j/total ~= 1 then do
        call writech(out1,'\c7\z'color||right(trunc(((count.j/total)+.005)*100),2,'0')'\z0%')
        call writech(out2,'\c7\z'color||right(trunc(((count.j/total)+.005)*100),2,'0')'\z0%')
      end
      else do
        call writech(out1,'\c'color'ÛÛ ')
        call writech(out2,'\c7\z'color'**\z0 ')
      end
    end
    else do
      writech(out1,'   ')
      writech(out2,'   ')
    end
  end
  call writeln(out1,'')
  call writeln(out2,'')
  sendstring '\h3...'
end
call writeln(out1,'    \c4ำฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ')
call writeln(out2,'    \c4`-------------------------------------------------------------------------')
call writeln(out1,'    \c3  00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23')
call writeln(out2,'    \c3  00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23')
call writeln(out1,'\n1   \c6STR\c7.\c6HourGraph  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'\n1   \c6STR\c7.\c6HourGraph  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

drop hour. count.

if bbsport ~= '' then sendstring '\h9\h3\c6TopCallers'

if ~open(inp,datafile,'r') then do
  logentry 'Couldn''t open "'datafile'"!'
  exit 10
end

user. = ''
line = readln(inp)
do i = 1 until eof(inp)
  parse var line user.1.i';'user.2.i';'user.3.i';'user.4.i';'user.5.i';'user.6.i';'user.7.i';'user.8.i';'user.9.i';'user.10.i';'user.11.i
  line = readln(inp)
end
call close(inp)

call open(out1,'T:STR.Top10.Calls','w')
call open(out2,'T:STR.Top10.Msgs','w')
do j = 1 to i
  call writeln(out1,user.9.j';'j)
  call writeln(out2,user.10.j';'j)
end
call close(out1)
call close(out2)

address command 'Sort T:STR.Top10.Calls T:STR.Top10.Calls Numeric'
address command 'Sort T:STR.Top10.Msgs  T:STR.Top10.Msgs  Numeric'

msgs. = 0
calls. = 0
call open(inp1,'T:STR.Top10.Calls','r')
call open(inp2,'T:STR.Top10.Msgs','r')
line1 = readln(inp1)
line2 = readln(inp2)
do j = 1 to i
  parse var line1 junk';'calls.j
  parse var line2 junk';'msgs.j
  line1 = readln(inp1)
  line2 = readln(inp2)
end
call close(inp1)
call close(inp2)

call delete('T:STR.Top10.Calls')
call delete('T:STR.Top10.Msgs')

if ~open(out1,textpath'STR.TopCallers.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.TopCallers.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.TopCallers','w') then do
  logentry 'Couldn''t open "'textpath'STR.TopCallers"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4ษออออัอออออออออออออออออออออออออออออออออออัออออออออออออออออออออออออออออออออออป')
call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
call writeln(out1,'\c4บ    ณ            \c3Top Callers            \c4ณ            \c3Top Posters           \c4บ')
call writeln(out2,'\c4|    |            \c3Top Callers            \c4|            \c3Top Posters           \c4|')
call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ')
call writeln(out2,'\c4|----|-----------------------------------|----------------------------------|')
do j = i to i-maxtopnum+1 by -1
  k = calls.j; m = msgs.j; count = count+1
  call writech(out1,'\c4บ \c3'left(count,2)' \c4ณ\c7'left(user.1.k,28))
  call writech(out2,'\c4| \c3'left(count,2)' \c4|\c7'left(user.1.k,28))
  call writech(out1,right(user.9.k,7)'\c4ณ\c7'left(user.1.m,28))
  call writech(out2,right(user.9.k,7)'\c4|\c7'left(user.1.m,28))
  call writeln(out1,right(user.10.m,6)'\c4บ')
  call writeln(out2,right(user.10.m,6)'\c4|')
end
call writeln(out1,'\c4ศออออฯอออออออออออออออออออออออออออออออออออฯออออออออออออออออออออออออออออออออออผ')
call writeln(out2,'\c4`---------------------------------------------------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6TopCallers  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6TopCallers  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

drop msgs. calls.

if bbsport ~= '' then sendstring '\h9\h1BytesDown'

call open(out1,'T:STR.Top10.Down','w')
call open(out2,'T:STR.Top10.Up','w')
do j = 1 to i
  if user.7.j < topbytedn then call writeln(out1,user.7.j';'j)
  else call writeln(out1,'0;0')
  if user.8.j < topbyteup then call writeln(out2,user.8.j';'j)
  else call writeln(out2,'0;0')
end
call close(out1)
call close(out2)

address command 'Sort T:STR.Top10.Down T:STR.Top10.Down Numeric'
address command 'Sort T:STR.Top10.Up   T:STR.Top10.Up   Numeric'

up. = 0
down. = 0
call open(inp1,'T:STR.Top10.Down','r')
call open(inp2,'T:STR.Top10.Up','r')
line1 = readln(inp1)
line2 = readln(inp2)
do j = 1 to i
  parse var line1 junk';'down.j
  parse var line2 junk';'up.j
  line1 = readln(inp1)
  line2 = readln(inp2)
end
call close(inp1)
call close(inp2)

call delete('T:STR.Top10.Down')
call delete('T:STR.Top10.Up')

if ~open(out1,textpath'STR.BytesDown.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.BytesDown.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.BytesDown','w') then do
  logentry 'Couldn''t open "'textpath'STR.BytesDown"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4ษออออัอออออออออออออออออออออออออออออออออออัออออออออออออออออออออออออออออออออออป')
call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
call writeln(out1,'\c4บ    ณ         \c3Bytes Downloaded          \c4ณ          \c3Bytes Uploaded          \c4บ')
call writeln(out2,'\c4|    |         \c3Bytes Downloaded          \c4|          \c3Bytes Uploaded          \c4|')
call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ')
call writeln(out2,'\c4|----|-----------------------------------|----------------------------------|')
do j = i to i-maxtopnum+1 by -1
  k = down.j; m = up.j; count = count+1
  call writech(out1,'\c4บ \c3'left(count,2)' \c4ณ\c7'left(user.1.k,24))
  call writech(out2,'\c4| \c3'left(count,2)' \c4|\c7'left(user.1.k,24))
  call writech(out1,right(user.7.k,11)'\c4ณ\c7'left(user.1.m,24))
  call writech(out2,right(user.7.k,11)'\c4|\c7'left(user.1.m,24))
  call writeln(out1,right(user.8.m,10)'\c4บ')
  call writeln(out2,right(user.8.m,10)'\c4|')
end
call writeln(out1,'\c4ศออออฯอออออออออออออออออออออออออออออออออออฯออออออออออออออออออออออออออออออออออผ')
call writeln(out2,'\c4`---------------------------------------------------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6BytesDown   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6BytesDown   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

if bbsport ~= '' then sendstring '\h9FilesDown'

call open(out1,'T:STR.Top10.Down','w')
call open(out2,'T:STR.Top10.Up','w')
do j = 1 to i
  if user.5.j < topfiledn then call writeln(out1,user.5.j';'j)
  else call writeln(out1,'0;0')
  if user.6.j < topfileup then call writeln(out2,user.6.j';'j)
  else call writeln(out2,'0;0')
end
call close(out1)
call close(out2)

address command 'Sort T:STR.Top10.Down T:STR.Top10.Down Numeric'
address command 'Sort T:STR.Top10.Up   T:STR.Top10.Up   Numeric'

up. = 0
down. = 0
call open(inp1,'T:STR.Top10.Down','r')
call open(inp2,'T:STR.Top10.Up','r')
line1 = readln(inp1)
line2 = readln(inp2)
do j = 1 to i
  parse var line1 junk';'down.j
  parse var line2 junk';'up.j
  line1 = readln(inp1)
  line2 = readln(inp2)
end
call close(inp1)
call close(inp2)

call delete('T:STR.Top10.Down')
call delete('T:STR.Top10.Up')

if ~open(out1,textpath'STR.FilesDown.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.FilesDown.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.FilesDown','w') then do
  logentry 'Couldn''t open "'textpath'STR.FilesDown"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4ษออออัอออออออออออออออออออออออออออออออออออัออออออออออออออออออออออออออออออออออป')
call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
call writeln(out1,'\c4บ    ณ         \c3Files Downloaded          \c4ณ          \c3Files Uploaded          \c4บ')
call writeln(out2,'\c4|    |         \c3Files Downloaded          \c4|          \c3Files Uploaded          \c4|')
call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ')
call writeln(out2,'\c4|----|-----------------------------------|----------------------------------|')
do j = i to i-maxtopnum+1 by -1
  k = down.j; m = up.j; count = count+1
  call writech(out1,'\c4บ \c3'left(count,2)' \c4ณ\c7'left(user.1.k,24))
  call writech(out2,'\c4| \c3'left(count,2)' \c4|\c7'left(user.1.k,24))
  call writech(out1,right(user.5.k,11)'\c4ณ\c7'left(user.1.m,24))
  call writech(out2,right(user.5.k,11)'\c4|\c7'left(user.1.m,24))
  call writeln(out1,right(user.6.m,10)'\c4บ')
  call writeln(out2,right(user.6.m,10)'\c4|')
end
call writeln(out1,'\c4ศออออฯอออออออออออออออออออออออออออออออออออฯออออออออออออออออออออออออออออออออออผ')
call writeln(out2,'\c4`---------------------------------------------------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6FilesDown   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6FilesDown   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

if bbsport ~= '' then sendstring '\h9GodsBytes'

call open(out1,'T:STR.Top10.Down','w')
call open(out2,'T:STR.Top10.Up','w')
do j = 1 to i
  if user.7.j >= topbytedn then call writeln(out1,user.7.j';'j)
  else call writeln(out1,'0;0')
  if user.8.j >= topbyteup then call writeln(out2,user.8.j';'j)
  else call writeln(out2,'0;0')
end
call close(out1)
call close(out2)

address command 'Sort T:STR.Top10.Down T:STR.Top10.Down Numeric'
address command 'Sort T:STR.Top10.Up   T:STR.Top10.Up   Numeric'

up. = 0
down. = 0
call open(inp1,'T:STR.Top10.Down','r')
call open(inp2,'T:STR.Top10.Up','r')
line1 = readln(inp1)
line2 = readln(inp2)
do j = 1 to i
  parse var line1 junk';'down.j
  parse var line2 junk';'up.j
  line1 = readln(inp1)
  line2 = readln(inp2)
end
call close(inp1)
call close(inp2)

call delete('T:STR.Top10.Down')
call delete('T:STR.Top10.Up')

if ~open(out1,textpath'STR.GodsBytes.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.GodsBytes.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.GodsBytes','w') then do
  logentry 'Couldn''t open "'textpath'STR.GodsBytes"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4ษออออัอออออออออออออออออออออออออออออออออออัออออออออออออออออออออออออออออออออออป')
call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
call writeln(out1,'\c4บ    ณ       \c3Bytes Gods Downloaded       \c4ณ       \c3Bytes Gods Uploaded        \c4บ')
call writeln(out2,'\c4|    |       \c3Bytes Gods Downloaded       \c4|       \c3Bytes Gods Uploaded        \c4|')
call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ')
call writeln(out2,'\c4|----|-----------------------------------|----------------------------------|')
do j = i to i-maxtopnum+1 by -1
  k = down.j; m = up.j; count = count+1
  call writech(out1,'\c4บ \c3'left(count,2)' \c4ณ\c7'left(user.1.k,24))
  call writech(out2,'\c4| \c3'left(count,2)' \c4|\c7'left(user.1.k,24))
  call writech(out1,right(user.7.k,11)'\c4ณ\c7'left(user.1.m,24))
  call writech(out2,right(user.7.k,11)'\c4|\c7'left(user.1.m,24))
  call writeln(out1,right(user.8.m,10)'\c4บ')
  call writeln(out2,right(user.8.m,10)'\c4|')
end
call writeln(out1,'\c4ศออออฯอออออออออออออออออออออออออออออออออออฯออออออออออออออออออออออออออออออออออผ')
call writeln(out2,'\c4`---------------------------------------------------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6GodsBytes   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6GodsBytes   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

if bbsport ~= '' then sendstring '\h9GodsFiles'

call open(out1,'T:STR.Top10.Down','w')
call open(out2,'T:STR.Top10.Up','w')
do j = 1 to i
  if user.5.j >= topfiledn then call writeln(out1,user.5.j';'j)
  else call writeln(out1,'0;0')
  if user.6.j >= topfileup then call writeln(out2,user.6.j';'j)
  else call writeln(out2,'0;0')
end
call close(out1)
call close(out2)

address command 'Sort T:STR.Top10.Down T:STR.Top10.Down Numeric'
address command 'Sort T:STR.Top10.Up   T:STR.Top10.Up   Numeric'

up. = 0
down. = 0
call open(inp1,'T:STR.Top10.Down','r')
call open(inp2,'T:STR.Top10.Up','r')
line1 = readln(inp1)
line2 = readln(inp2)
do j = 1 to i
  parse var line1 junk';'down.j
  parse var line2 junk';'up.j
  line1 = readln(inp1)
  line2 = readln(inp2)
end
call close(inp1)
call close(inp2)

call delete('T:STR.Top10.Down')
call delete('T:STR.Top10.Up')

if ~open(out1,textpath'STR.GodsFiles.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.GodsFiles.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.GodsFiles','w') then do
  logentry 'Couldn''t open "'textpath'STR.GodsFiles"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4ษออออัอออออออออออออออออออออออออออออออออออัออออออออออออออออออออออออออออออออออป')
call writeln(out2,'\n1\c4`---------------------------------------------------------------------------''')
call writeln(out1,'\c4บ    ณ       \c3Files Gods Downloaded       \c4ณ       \c3Files Gods Uploaded        \c4บ')
call writeln(out2,'\c4|    |       \c3Files Gods Downloaded       \c4|       \c3Files Gods Uploaded        \c4|')
call writeln(out1,'\c4วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ')
call writeln(out2,'\c4|----|-----------------------------------|----------------------------------|')
do j = i to i-maxtopnum+1 by -1
  k = down.j; m = up.j; count = count+1
  call writech(out1,'\c4บ \c3'left(count,2)' \c4ณ\c7'left(user.1.k,24))
  call writech(out2,'\c4| \c3'left(count,2)' \c4|\c7'left(user.1.k,24))
  call writech(out1,right(user.5.k,11)'\c4ณ\c7'left(user.1.m,24))
  call writech(out2,right(user.5.k,11)'\c4|\c7'left(user.1.m,24))
  call writeln(out1,right(user.6.m,10)'\c4บ')
  call writeln(out2,right(user.6.m,10)'\c4|')
end
call writeln(out1,'\c4ศออออฯอออออออออออออออออออออออออออออออออออฯออออออออออออออออออออออออออออออออออผ')
call writeln(out2,'\c4`---------------------------------------------------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6GodsFiles   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6GodsFiles   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

drop user. up. down.

if bbsport ~= '' then sendstring '\h9CPSRates'

if ~open(inp,highfile,'r') then do
  logentry 'Couldn''t open "'highfile'"!'
  exit 10
end

cps. = ''
line = readln(inp)
do i = 1 until eof(inp)
  parse var line cps.1.i';'cps.2.i';'cps.3.i';'cps.4.i';'cps.5.i
  if substr(cps.4.i,6,1) = '-' then cps.4.i = word(cps.4.i,1)'  'word(cps.4.i,2)
  line = readln(inp)
end
call close(inp)

if ~open(out1,textpath'STR.CPSRates.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.CPSRates.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.CPSRates','w') then do
  logentry 'Couldn''t open "'textpath'STR.CPSRates"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4     ษออออัออออออออออออออออออออออัอออออออออออออออัอออออออัอออออัอออออออป')
call writeln(out2,'\n1\c4     `-----------------------------------------------------------------''')
call writeln(out1,'\c4     บ    ณ         \c3Name         \c4ณ      \c3Date     \c4ณ  \c3Baud \c4ณ \c3CPS \c4ณ\c3Percent\c4บ')
call writeln(out2,'\c4     |    |         \c3Name         \c4|      \c3Date     \c4|  \c3Baud \c4| \c3CPS \c4|\c3Percent\c4|')
call writeln(out1,'\c4     วฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤลฤฤฤฤฤฤฤลฤฤฤฤฤลฤฤฤฤฤฤฤถ')
call writeln(out2,'\c4     |----|----------------------|---------------|-------|-----|-------|')
do j = i to i-maxtopnum+1 by -1
  count = count+1
  rate = ((cps.1.j/(cps.3.j/10))*100)%1
  call writech(out1,'\c4     บ \c3'left(count,2)' \c4ณ\c7'left(cps.2.j,22)'\c4ณ\c7')
  call writech(out2,'\c4     | \c3'left(count,2)' \c4|\c7'left(cps.2.j,22)'\c4|\c7')
  call writech(out1,cps.4.j'\c4|\c7'right(cps.3.j,7)'\c4|\c7')
  call writech(out2,cps.4.j'\c4ณ\c7'right(cps.3.j,7)'\c4ณ\c7')
  call writeln(out1,right(cps.1.j,5)'\c4ณ\c7'right(rate,5)'\c3% \c4บ\c7')
  call writeln(out2,right(cps.1.j,5)'\c4|\c7'right(rate,5)'\c3% \c4|\c7')
end
call writeln(out1,'\c4     ศออออฯออออออออออออออออออออออฯอออออออออออออออฯอออออออฯอออออฯอออออออผ')
call writeln(out2,'\c4     `-----------------------------------------------------------------''')
call writeln(out1,'   \c6STR\c7.\c6CPSRates  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'   \c6STR\c7.\c6CPSRates  \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

drop cps.

if bbsport ~= '' then print '\c7 - \q1Done!\n1'

exit
