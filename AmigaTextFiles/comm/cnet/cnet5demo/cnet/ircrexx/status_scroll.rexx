/* BBS status scroll.. you must have the scroll.rexx in this dir */

c=0

options results
parse arg orcport .
orcport=trim(orcport)
tr=transmit;gs=getscratch


TOP:
who=''
address CNETREXX0
do scan=1 to 20
 if ~show('P',orcport) then EXIT
 getportid scan;pid=result
 if pid>0 then do
  c=c+1
  loadscratch pid
  savescratch (-pid)
  gs 1;who = result' 'who
 end
end

address command  'rx cnet:ircrexx/scroll.rexx 'orcport' There are 'c' users online:' who

delay(500)
call TOP
