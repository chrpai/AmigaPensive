/* TrapCall v1.04 11-20-93 */

/*
    A sample AREXX script that can be launched as a CNet event to have
    TrapDoor call your FidoNet host system.
*/
 
 Options Results
 addlib('rexxsupport.library',0,-30,0)
 Address "TrapDoor"

 tries=0
 say ''
 say 'Attempting to call Fido HOST...'
 say ''

 CLEARLOOP:
   tries=tries+1
   if tries=10 then do
     say ''
     say 'ABORTING.  Unable to command TrapDoor at this time.'
     say ''
     Exit
   End

   call Delay(250)

   '@Status S'
   If Result~='IDLE' Then do
     say 'STATUS:' result||'; Trapdoor busy, trying again...'
     call CLEARLOOP
   End

/*
   At this point I like to put a command to backup the contents of my
   Inbound and Outbound directories, just to be safe.  Of course that
   is not too practical on a high volume regional hub system tossing
   mondo megabytes every day.  :=)
*/

 HOST='1:2410/215.0'   /*  Fill in your OWN host's address here!!  */
 tries=0

CALLHOST:
  tries=tries+1
  if tries=99 then do
    say ''
    say 'Host connection FAILED!'
    say ''
    exit
  End
  call DOIT
  call CALLHOST

DOIT:
  Say 'Clearing Line...'
  'sclear'
  Say 'Line Cleared, Calling...'
  'CALL '||HOST
  'STATUS D'
  if Result='RESULT'Then do
    Say 'Break or FQFA error'
    Exit
  End
  if Left(Result,7)='CONNECT' Then Do
    say ''
    say 'Host session successful!'
    say ''
    Exit RC
  End
  Say Result
  'reset'
  Return

