/* WWF settings script */

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
	SetStatus "WWF Settings:"
	if GetUserMisc(ln, "WWFASYNC") = 1 then des = "ON"
	else des = "OFF"
	SendModem CRLF||"(1)  "||CYAN||"Asynchronous operation: "||WHITE||des||CRLF
	des = GetUserMisc(ln, "WWFUNARC")
	select
		when des = "lha x" then des = "LhA"
		when des = "unzip" then des = "Zip"
		when des = "arc x" then des = "Arc"
		when des = "zoo e" then des = "Zoo"
		when des = "unarj" then des = "Arj"
		when des = "" then des = "LhA"
		otherwise des = "Unknown"
		end
	SendModem "(2)  "||CYAN||"Input archiving method: "||WHITE||des||CRLF
	des = GetUserMisc(ln, "WWFARC")
	select
		when des = "lha a" then des = "LhA"
		when des = "arc a" then des = "Arc"
		when des = "zoo a" then des = "Zoo"
		when des = "" then des = "LhA"
		otherwise des = "Unknown"
		end
	SendModem "(3) "||CYAN||"Output archiving method: "||WHITE||des||CRLF
	SendModem CRLF||"(Q) "||CYAN||"Quit"||CRLF||CRLF||WHITE
	cmdstr = AskInput(ln, "("||GetTimeLeft(ln)||") "||CYAN||"WWF Settings: "||WHITE, "", 255)
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
			if GetUserMisc(ln, "WWFASYNC") = 1 then do
				call SetUserMisc ln, "WWFTASKPRI", 1
				call SetUserMisc ln, "WWFASYNC", 0
				end
			else do
				call SetUserMisc ln, "WWFTASKPRI", -1
				call SetUserMisc ln, "WWFASYNC", 1
				end
			end
		when cmdid = "2" then do
			des = GetUserMisc(ln, "WWFUNARC")
			select
				when des = "lha x" then des = "unzip"
				when des = "unzip" then des = "arc x"
				when des = "arc x" then des = "zoo e"
				when des = "zoo e" then des = "unarj"
				when des = "unarj" then des = "lha x"
				when des = "" then des = "unzip"
				otherwise des = "lha x"
				end
			call SetUserMisc ln, "WWFUNARC", des
			end
		when cmdid = "3" then do
			des = GetUserMisc(ln, "WWFARC")
			select
				when des = "lha a" then des = "arc a"
				when des = "arc a" then des = "zoo a"
				when des = "zoo a" then des = "lha a"
				when des = "" then des = "zoo a"
				otherwise des = "lha a"
				end
			call SetUserMisc ln, "WWFARC", des
			end
		when cmdid = "Q" then exit 0
		otherwise
		end
	end
