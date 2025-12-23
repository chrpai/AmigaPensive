/* Log off stuff */

CR = D2C(13)
LF = D2C(10)
CRLF = CR||LF
ESC = D2C(27)
ln = arg(1)
CmdStr = arg(2)
LineName = Con_LineActive(ln)
if LineName = "" then exit 10
address value LineName
SetStatus "Logging off"
SendASCII "Text/Goodbye.txt"
LogEntry GetUserName(ln)||" logged off"
Disconnect
/*
if ln = 1 then do
	address command
	'toggledtr'
	end */
exit 5
