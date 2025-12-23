/* PISS OFF */
options results
arg bbs_node
address value 'TempRexx-'bbs_node
SIGNAL ON ioerr;SIGNAL ON syntax;SIGNAL ON error;SIGNAL ON HALT
CLS
Transmit 'Dont expect miracles, this is a SIMPLE Util.'
TRansmit ' '
TRansmit ' '
D='BBS:AREXX.CODES'
X=0;i=0
OPEN(x,D,'R')
DO i=0 to 17
    PDAT.I=READLN(x)
END
DO i=0 to 100
    GVDAT.I=READLN(x)
END

Close(x)

MAIN:
Getchar '(S)creen or (F)ile?'
IF Upper(result)='S' then signal screen
IF Upper(result)='F' then signal FILE
SIGNAL MAIN

FILE:
Transmit ' '
Prompt 'Enter PATH and Filename for file (RAM:TEST) >>'
FILE=UPPER(RESULT)
x=0
open(data,FILE,'W')
writeln(data,'PATHS:')
do x = 0 to 17
Path x;Writeln(data,(x' - 'LEFT(result,30)''LEFT(PDAT.x,40)))
end
x=0
writeln(data,'GetVar:')
do x = 0 to 100
Getvar x;Writeln(data,(x' - 'LEFT(result,30)''LEFT(GVDAT.x,40)))
end
close(data)

SCREEN:
x=0
TRANSMIT 'PATHS:'
do x = 0 to 17
Path x;TRANSMIT x' - 'LEFT(result,30)''LEFT(GVDAT.x,40)
end
x=0
TRANSMIT 'GetVar:'
do x = 0 to 100
Getvar x;TRANSMIT x' - 'LEFT(result,30)''LEFT(GVDAT.x,40)
end
SIGNAL DONE

IOERR:
SYNTAX:
ERROR:
ENDOFERR:
TRANSMIT "  Line: "SIGL
TRANSMIT "Error:  "RC errortext(RC)
TRANSMIT "Please leave a FEEDBACK to the Sysop!"
TRANSMIT "RETURNING to "BBSname"..."
HALT:
T ' '
T "EMERGENCY BRAKES HAVE BEEN PULLED!!!"
DONE:
'exit'
EXIT
