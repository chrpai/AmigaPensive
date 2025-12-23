/* Logging of CallerID info into Excelsior! caller log */
/*           CALLERID Version 1.11                     */
/* This program written by Gerald Affeldt.             */
/* Copy righted 15 Jan 1996. All rights reserved       */

Caller = 'doors:callerid/caller'  /*Path to callerID log file from TrapDoor*/
open(idfile,CALLER,read)

callers = 'bbs:text/logs/callers' /*Path to BBS caller Log*/
   if exists(callers) then do
   open(log,callers,append)
   end ; else open(log,callers,write)

writeln(log,'\c1-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=')
writeln(log,'\c2Call Received\c3')

do i = 1 to 4
   id = readln(idfile)
   writeln(log,id)
   end

close idfile
close(log)

exit
