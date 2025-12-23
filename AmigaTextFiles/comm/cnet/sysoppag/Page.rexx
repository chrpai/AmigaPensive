/* SysopPager by David Prothero */

options results
parse arg port

killtime = 1      /* Change to 1 to have user's time cut to zero when user is
                     "DUMPED" */

/* Add more for ports that you run */
     if port = 0 then address CNETREXX0
else if port = 1 then address CNETREXX1
else if port = 2 then address CNETREXX2
else if port = 3 then address CNETREXX3
else if port = 4 then address CNETREXX4
else if port = 5 then address CNETREXX5
else if port = 6 then address CNETREXX6
else if port = 7 then address CNETREXX7
else if port = 8 then address CNETREXX8
else if port = 9 then address CNETREXX9

if ~show('l','apig.library') then do
   x = addlib("apig.library",0,-30,0)
   if ~x then do
      say 'Unable to open apig.library'
      exit
   end
end
if ~show('l','rexxsupport.library') then do
   x = addlib("rexxsupport.library",-5,-30,0)
   if ~x then do
      say 'Unable to open rexxsupport.library'
      exit
   end
end

call set_apig_globals()
portname = 'pager'
p = openport(portname)

call open('tmp','ram:ct.'port,'R')
user = readln('tmp')
reason = readln('tmp')
call close('tmp')
win = openwindow(portname,0,0,1,1,2,4,0,0,'',null(),0,0,0)
reqtitle = 'SysopPager by David Prothero'
cr = '0a'x
reqtext =  'You are being paged on port 'port'!'cr||cr'User: 'user||cr'Reason: "'reason'"'
reqgads  = " OKAY | DUMP USER | UNAVAILABLE"
address command 'run >nil: sound sysdata:chat.snd'
reqx = EASYREQUEST(win,reqtitle,reqtext,reqgads,null(),0,0)
call CLOSEWINDOW(win)

if reqx = 0 then setclip(avail,'1')
else if reqx = 2 then do
   transmit
   sendstring 'Session terminated.'
   if killtime = 1 then do
      getuser 7 ; time = result
      timeleft = (0-time)
      addtime timeleft
   end
   dropcarrier
end

address command 'delete ram:ct.'port' quiet'
exit
