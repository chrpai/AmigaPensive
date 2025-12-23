/* For Comparing CallerID to User Data Number   */
/*           CALLERID Version 1.11              */
/* This program written by Gerald Affeldt.      */
/* Copy righted 15 Jan 1996. All rights reserved*/

options results

sendfile 'doors:callerid/Verify'

Caller = 'doors:callerid/caller' /*Path to callerID log file from TrapDoor*/
if open(idfile,CALLER,read) = 0 then exit

do i = 1 to 3
   id1 = readln(idfile)
   end

id2 = right(id1,7)

close idfile

callers = 'bbs:text/logs/callers'
  if exists(callers) then do
  open(log,callers,append)
  end ; else open(log,callers,write)

call nameid

if right(id2,1) = P then call blocked
if right(id2,1) = O then call outarea
if right(id2,1) = E then call errorid

getuser 29 ; bbsno = result
bb1 = compress(bbsno,'-')
bb2 = right(bb1,7)

if compare(id2,bb2) = 0 then call good
else call bad

exit

blocked:

 sendfile 'doors:callerid/blocked'
 writeln(log,'\c1CallerID Blocked')
 close(log)
 exit
return

outarea:

 sendfile 'doors:callerid/error'
 writeln(log,'\c6CallerID Out of Area')
 close(log)
 exit
return

errorid:

 sendfile 'doors:callerid/error'
 writeln(log,'\c6CallerID Error')
 close(log)
 exit
return

good:

 writeln(log,'\c2CallerID Matches BBS Records')
 sendfile 'doors:callerid/confirmed'
 close(log)
 exit
return

bad:

 writeln(log,'\c1***** CALLERID MISMATCH *****')
 writeln(log,'\c3Records Show Number As \c2'bb2)
 sendfile 'doors:callerid/bad'
 close(log)
 exit
return

nameid:

 getuser 40 ; idnum = result
 getuser 1  ; alias = result
 writeln(log,'\c3Verification On \c7'alias',\c3 ID# \c7'idnum)
return
