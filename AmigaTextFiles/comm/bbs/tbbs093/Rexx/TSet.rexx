/* Terminal settings script */

CR = D2C(13)
LF = D2C(10)
CRLF = CR||LF
ESC = D2C(27)
WHITE = ESC||"[0m"
RED = ESC||"[31m"
GREEN = ESC||"[32m"
YELLOW = ESC||"[33m"
BLUE = ESC||"[34m"
PURPLE = ESC||"[35m"
CYAN = ESC||"[36m"
CLEAR = D2C(12)
ln = arg(1)
LineName = Con_LineActive(ln)
if LineName = "" then exit 10
address value LineName
do forever
	SetStatus "Terminal Settings:"
	if GetStripAll(ln) = 1 then des = "ON"
	else des = "OFF"
	SendModem CRLF||"(1)       "||CYAN||"Strip All: "||WHITE||des||CRLF
	if GetStripCols(ln) = 1 then des = "ON"
	else des = "OFF"
	SendModem "(2)     "||CYAN||"Strip Color: "||WHITE||des||CRLF
	if GetSimpleTerm(ln) = 1 then des = "ON"
	else des = "OFF"
	SendModem "(3) "||CYAN||"Simple Terminal: "||WHITE||des||CRLF
	SendModem "(4) "||CYAN||"  Character set: "||WHITE||GetUserCharSet(ln)||CRLF
	SendModem "(5)  "||CYAN||"Terminal lines: "||WHITE||GetTerminalLines(ln)||CRLF
	select
		when Upper(GetUserExtension(ln)) = ".TXT" then des = "Normal"
		when Upper(GetUserExtension(ln)) = ".ETXT" then des = "Short"
		otherwise des = "INVALID"
		end
	SendModem "(6)       "||CYAN||"Menu type: "||WHITE||des||CRLF
	if GetUserMisc(ln, "MSGCHECK") = 1 then des = "ON"
	else des = "OFF"
	SendModem "(7)   "||CYAN||"Message check: "||WHITE||des||CRLF
	if GetUserMisc(ln, "FILECHECK") = 1 then des = "ON"
	else des = "OFF"
	SendModem "(8)      "||CYAN||"File check: "||WHITE||des||CRLF
	SendModem "(9)        "||CYAN||"Protocol: "||WHITE||GetUserProtocol(ln)||CRLF
	SendModem CRLF||"(Q) "||CYAN||"Quit"||WHITE||CRLF||CRLF
	cmdstr = AskInput(ln, "("||GetTimeLeft(ln)||") "||CYAN||"Terminal Settings: "||WHITE, "", 255)
	sendmodem CRLF
	do while (left(cmdstr, 1) = " ") & (length(cmdstr) > 0)
		cmdstr = right(cmdstr, length(cmdstr) - 1)
		end
	parse var cmdstr cmdid cmdstr
	cmdid = upper(cmdid)
	do while (left(cmdstr, 1) = " ") & (length(cmdstr) > 0)
		cmdstr = right(cmdstr, length(cmdstr) - 1)
		end
	select
		when cmdid = "1" then do
			if GetStripAll(ln) = 1 then SetStripAll "0"
			else SetStripAll "1"
			end
		when cmdid = "2" then do
			if GetStripCols(ln) = 1 then SetStripCols "0"
			else SetStripCols "1"
			end
		when cmdid = "3" then do
			if GetSimpleTerm(ln) = 1 then SetSimpleTerm "0"
			else SetSimpleTerm "1"
			end
		when cmdid = "4" then do
			CSet = ""
			do while CSet = ""
				if Length(cmdstr) > 0 then do
					CSet = cmdstr
					cmdstr = ""
					end
				else do
					SendModem CRLF
					SendASCII "Text/CharSet.txt"
					CSet = AskInput(ln, "Select: ", "", 2, "NUMERIC")
					end
				select
					when CSet = "0" then CSet = "ISO"
					when CSet = "1" then CSet = "IBM"
					when CSet = "2" then CSet = "SF7"
					when CSet = "3" then CSet = "ISOSF7"
					when CSet = "4" then CSet = "IBMSF7"
					otherwise CSet = ""
					end
				end
			SetUserCharSet CSet
			end
		when cmdid = "5" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then SetTerminalLines GetArgs()
			else SetTerminalLines AskInput(ln, "Terminal lines: ", GetTerminalLines(ln), 3, "NUMERIC")
			end
		when cmdid = "6" then do
			if GetUserExtension(ln) ~= ".txt" then MenuType = ".txt"
			else MenuType = ".etxt"
			SetUserExtension MenuType
			Ext = MenuType
			end
		when cmdid = "7" then do
			if GetUserMisc(ln, "MSGCHECK") = 1 then call SetUserMisc ln, "MSGCHECK", 0
			else call SetUserMisc ln, "MSGCHECK", 1
			end
		when cmdid = "8" then do
			if GetUserMisc(ln, "FILECHECK") = 1 then call SetUserMisc ln, "MSGCHECK", 0
			else call SetUserMisc ln, "FILECHECK", 1
			end
		when cmdid = "9" then do
			Protocol = ""
			do while Protocol = ""
				if Length(cmdstr) > 0 then do
					Protocol = cmdstr
					cmdstr = ""
					end
				else do
					SendModem CRLF
					SendASCII "Text/Protocol.Txt"
					Protocol = Upper(AskInput(ln, "Select: ", "", 2))
					end
				select
					when Protocol = "Z" then Protocol = "ZModem"
					when Protocol = "8" then Protocol = "ZModem8k"
					otherwise Protocol = ""
					end
				end
			SetUserProtocol Protocol
			end
		when cmdid = "Q" then exit 0
		otherwise
		end
	end
