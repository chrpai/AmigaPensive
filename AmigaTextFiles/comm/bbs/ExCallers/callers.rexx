/* EXCELSIOR! BBS Caller Info ARexx Script */

options results

'getuser' 1
_USER = result

signal on BREAK_C

'transmit' "\q1"
'transmit' "   \z4\c3 W H O ' S   C A L L E D ? \z0\c6 for Excelsior! BBS \c2version 1.00"
'transmit' "                               \c6by Renze de Ruiter \c2(c)1993 All rights reserved"

address command 'IPLRead TEXT/Logs/Callers to RAM:TempLog F'

'transmit' "\q1"
'transmit' "      Callers are displayed in reverse order, with the last caller first."

ret = open(logfile,'RAM:TempLog','R')
if ret = 0 then do
	'transmit' "\Q1 "
	'transmit' "\O1ERROR:\O0 Could not open the Callers file for input!"
	'logentry' "\z4CALLERS:\z0 Could not open the Callers file for input!"
	'transmit' " "
	'bufferflush'
	exit
end
lines=0
do until eof(logfile)
	lines = lines + 1
	log.lines = readln(logfile)
end
close(logfile)
address command 'delete >nil: RAM:TempLog'

_TIMEON = ""
_CALLDATE = ""
_ALIAS = ""
_BAUDRATE = ""

'transmit' "\q1"
'transmit' "  \c7Date of Call:                  Alias:                     Baud:    Online:"
'transmit' "  ~~~~~~~~~~~~~~~~~~~~~~~    ~~~~~~~~~~~~~~~~~~~~~~~~~~     ~~~~~    ~~~~~~~~~"

do l = lines to 1 by -1
	workline = log.l
	if index(workline,'LOGOFF:') ~= 0 then do
		nn = words(workline)
		_TIMEON = word(workline,nn-1)
	end
	if index(workline,'DATE:') ~= 0 then do
		nn = words(workline)
		_CALLDATE = word(workline,nn-2)" "word(workline,nn-1)" "word(workline,nn)
	end
	if index(workline,'ALIAS:') ~= 0 then do
		nn = words(workline)
		np = nn - 8
		do i = 1 to np
			_ALIAS = _ALIAS||word(workline,1+i)||" "
		end
		_ALIAS = strip(_ALIAS)
		_BAUDRATE = word(workline,nn-3)
	end
	if index(workline,'CALLER#') ~= 0 then do
		sendline = "  \c5"left(_CALLDATE,23)"        \c3"left(_ALIAS,18)"         \c2"right(_BAUDRATE,5)"    \c6"right(_TIMEON,3)" mins.\q1"
		if _USER = _ALIAS then do
			sendline = "  \c5"left(_CALLDATE,23)"   \z4     \c3"left(_ALIAS,18)"     \z0    \c2"right(_BAUDRATE,5)"    \c6"right(_TIMEON,3)" mins.\q1"
		end
		'transmit' sendline
		_OFFMODE = ""
		_TIMEON = ""
		_CALLDATE = ""
		_ALIAS = ""
		_BAUDRATE = ""
	end
end
'transmit' "\Q1"
'sendstring' "Press any key to Continue."
'getchar'
'transmit' "\Q1"
'bufferflush'
exit

BREAK_C:
'transmit' "\Q1"
'transmit' "*** BREAK ***"
'bufferflush'
exit
