/*

    STR.LogOn.rexx - An editable log-on manager for each user.

    Version 1.11

*/

options results

if ~show('l','rexxsupport.library') then do
  if ~addlib('rexxsupport.library',0,-30,0) then do
    print 'RexxSupport.Library missing! Can''t continue...'
    exit 10
  end
end

datapath = 'Doors:STR.Utils/'  /*  Path to the STR.LogOn.Mcfg file   */

runagain = 1  /*  1 = User is allowed to re-run the log-on sequence  *
               *      when the program is executed as a door.        *
               *  0 = Only allows user to re-edit the sequence, no   *
               *      run through is allowed.                        */

mandatory = 1 /*  1 = User must create a log-on sequence.            *
               *  0 = User may abort log-on manager without creating *
               *      a log-on sequence.                             */

inp = 'input'
out = 'output'

title = '\n2  \c6STR\c7.\c6LogOnManager \c5Program By\c7: \c2Black Fire \c6[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\n1'
wait  = '\n1\c6STR\c7.\c6LogOnManager \c7- \q1Press \c7\z4 RETURN \z0\q1 to continue\c7:\g0\n2'

path 2; userpath = result
getuser 40; userpath = userpath||result'/'
getuser 23; node = result

getwhat node; location = result

update = 0
if exists(userpath'STR.LogOn.Ucfg') then do
  file1 = statef(userpath'STR.LogOn.Ucfg')
  file2 = statef(datapath'STR.LogOn.Mcfg')

  day1 = word(file1,5); day2 = word(file2,5)
  min1 = word(file1,6); min2 = word(file2,6)

  if (day2 > day1) then update = 1
  else if (min2 > min1) & (day2 = day1) then update = 1
end

bufferflush

if pos('Logon Manager',location) > 0 then do
  print title
  queryy '\q1Are you sure you want to re-edit your log-on sequence? (\c7Y\q1/n): '
  ans = upper(result)
  if ans = 'YES' then call makechoice()
  if runagain = 1 then do
    queryy '\q1Would you like to run through your log-on sequence now? (\c7Y\q1/n): '
    ans = upper(result)
    if ans = 'YES' then call douserchoice()
  end
  exit
end

if update = 1 then do
  print title
  if mandatory = 1 then do
    print '\q1Updating your log-on sequence is mandatory!'
    ans = 'YES'
  end
  else do
    queryy '\q1The log-on choices have changed!\n2Would you like to re-edit your sequence? (\c7Y\q1/n): '
    ans = upper(result)
  end
  if ans = 'YES' then call makechoice()
  call douserchoice()
  exit
end

if exists(userpath'STR.LogOn.Ucfg') then do
  call douserchoice()
  exit
end

print title
if mandatory = 1 then do
  print '\q1Creation of a log-on sequence is mandatory!'
  ans = 'YES'
end
else do
  queryy '\q1You have not created your log-on sequence yet!\n2Would you like to do so now? (\c7Y\q1/n): '
  ans = upper(result)
end
if ans = 'NO' then exit
call makechoice()
call douserchoice()

exit


makechoice:

if ~exists(datapath'STR.LogOn.Mcfg') then do
  print '\n1\q1Please notify the Sysop that the Log-on Manager cannot find its datafiles!\n1'
  exit
end

if ~open(inp,datapath'STR.LogOn.Mcfg','r') then do
  logentry 'STR.LogOn.rexx cannot open "'datapath'STR.LogOn.Mcfg"!'
  exit 10
end

if ~open(out,userpath'STR.LogOn.Ucfg','w') then do
  logentry 'STR.LogOn.rexx cannot open "'userpath'STR.LogOn.Ucfg"!'
  exit 10
end

desc = readln(inp)
cmd  = readln(inp)
line = readln(inp)
do until eof(inp)
  parse var line flag';'arg
  if bitand(d2c(flag),'4'x) = '4'x then do
    print '\n1\q1Item: "\c2'desc'\q1" is mandatory!  Adding....'
    ans = 'YES'
  end
  else do
    queryy '\n1\q1Do you want to see "\c2'desc'\q1"? (\c7Y\q1/n): '
    ans = upper(result)
  end
  getcarrier
  if result = 'FALSE' then do
    call close(out)
    call close(inp)
    call delete(userpath'STR.LogOn.Ucfg')
    exit
  end
  if ans = 'YES' then do
    call writeln(out,desc)
    call writeln(out,cmd)
    call writeln(out,line)
  end
  desc = readln(inp); if eof(inp) then leave
  cmd  = readln(inp)
  line = readln(inp)
end
call close(out)
call close(inp)

print '\n1You may re-edit your sequence at any time by running the Logon Manager.'
print 'Which can be found in the Online Programs, Utilities area.'
sendstring wait

return


douserchoice:

file1 = statef(userpath'STR.LogOn.Ucfg')
if word(file1,2) = 0 then return

if ~open(inp,userpath'STR.LogOn.Ucfg','r') then do
  logentry 'STR.LogOn.rexx cannot open "'userpath'STR.LogOn.Ucfg"!'
  exit 10
end

desc = readln(inp)
cmd  = readln(inp)
line = readln(inp)
do until eof(inp)
  parse var line flag';'arg
  if bitand(d2c(flag),'1'x) = '1'x then sendstring d2c(12)
  if bitand(d2c(flag),'8'x) = '8'x then address command 'Echo >T:RexxArg'node' "'arg'"'
  sendstring cmd
  getcarrier
  if result = 'FALSE' then do
    call close(inp)
    return
  end
  if bitand(d2c(flag),'2'x) = '2'x then sendstring wait
  call delete('T:RexxArg'node)
  desc = readln(inp); if eof(inp) then leave
  cmd  = readln(inp)
  line = readln(inp)
end
call close(inp)

return
