/* Hack & Slash   rpg.trans 
   by: Rod Schnell & Roger Clark */

options results

call pragma('stack',10000)
UserInfo 'a' ; Name = upper(result)
SystemInfo '8' ; Line = result
id=time(s)
Transmit "Loading Hack & Slash..."
SetNodeLocation Name' is playing Hack & Slash'

path='t:rpg-startup'||line
open('temp',path,'W')
writeln('temp','stack 10000')
writeln('temp','BBS:Extras/Hack&Slash/Hack&Slash -c 'Name)
writeln('temp','endcli')
close('temp')

address command 'NewShell from 'path' FIFO:hack'id'/rwkecs' 
address command 'BBS:Bin/TrShell -n'line' -fhack'id
address command 'delete t:rpg-startup'||line||' quiet'
exit

/* end of Hack & Slash */
