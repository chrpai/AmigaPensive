/* startcli.rexx
**
** $VER: startcli.rexx 0.0.1 (25.4.93)
**
** ARexx program for Starting up a CLI door by ApT-Design.
**
** Copyright © 1993 ApT-Design All Rights Reserved,DM
**
*/


parse arg name' 'handler

options results

lineno=substr(handler,7,1)
path='t:AeD-startup'||lineno
open('temp',path,'W')
writeln('temp','stack 10000')
writeln('temp','DOORS:aed/aed 78 25 T:TESTCLI :: :: "USER:'name'"')
writeln('temp','endcli')
close('temp')

ADDRESS COMMAND 'run >nil: newshell >nil: 'handler'from 'path
EXIT

