/* User settings script */

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
	SetStatus "User Settings:"
	SendModem CRLF||"(1)           "||CYAN||"Name: "||WHITE||GetUserName(ln)||CRLF
	SendModem "(2) "||CYAN||"Street address: "||WHITE||GetUserMisc(ln, "ADDRESS")||CRLF
	SendModem "(3)           "||CYAN||"City: "||WHITE||GetUserMisc(ln, "CITY")||CRLF
	SendModem "(4)    "||CYAN||"Postal code: "||WHITE||GetUserMisc(ln, "POSTCODE")||CRLF
	SendModem "(5)        "||CYAN||"Country: "||WHITE||GetUserMisc(ln, "COUNTRY")||CRLF
	SendModem "(6)   "||CYAN||"Voice number: "||WHITE||GetUserMisc(ln, "VPHONE")||CRLF
	SendModem "(7)    "||CYAN||"Data number: "||WHITE||GetUserMisc(ln, "DPHONE")||CRLF
	SendModem "(8)       "||CYAN||"Password: "||WHITE||Left("", Length(GetPassword(ln)), "-")||CRLF
	SendModem CRLF||"(Q) "||CYAN||"Quit"||WHITE||CRLF||CRLF
	cmdstr = AskInput(ln, "("||GetTimeLeft(ln)||") "||CYAN||"User Settings: "||WHITE, "", 255)
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
			SendModem CRLF||"Ask the SysOp if you want your name changed"||CRLF||CRLF
/*
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then SetUserName GetArgs()
				end
			else SetUserName AskInput(ln, "Enter your full name: ", GetUserName(ln), 36, "CAPITAL")
*/
			end
		when cmdid = "2" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then call SetUserMisc ln, "ADDRESS", GetArgs()
				end
			else call SetUserMisc ln, "ADDRESS", AskInput(ln, "Street address: ", GetUserMisc(ln, "ADDRESS"), 80)
			end
		when cmdid = "3" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then call SetUserMisc ln, "CITY", GetArgs()
				end
			else call SetUserMisc ln, "CITY", AskInput(ln, "City: ", GetUserMisc(ln, "CITY"), 40)
			end
		when cmdid = "4" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then call SetUserMisc ln, "POSTCODE", GetArgs
				end
			else call SetUserMisc ln, "POSTCODE", AskInput(ln, "Postal code: ", GetUserMisc(ln, "POSTCODE"), 40)
			end
		when cmdid = "5" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then call SetUserMisc ln, "COUNTRY", GetArgs()
				end
			else call SetUserMisc ln, "COUNTRY", AskInput(ln, "Country: ", GetUserMisc(ln, "COUNTRY"), 40)
			end
		when cmdid = "6" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then call SetUserMisc ln, "VPHONE", GetArgs()
				end
			else call SetUserMisc ln, "VPHONE", AskInput(ln, "Voice number: ", GetUserMisc(ln, "VPHONE"), 40)
			end
		when cmdid = "7" then do
			SendModem CRLF
			if Length(CmdStr) > 0 then do
				do while Left(CmdStr, 1) = " "
					CmdStr = Right(CmdStr, Length(CmdStr) - 1)
					end
				if CmdStr ~= "" then call SetUserMisc ln, "DPHONE", GetArgs()
				end
			else call SetUserMisc ln, "DPHONE" , AskInput(ln, "Data number: ", GetUserMisc(ln, "DPHONE"), 40)
			end
		when cmdid = "8" then do
			SendModem CRLF
			Password = AskInput(ln, "Enter your old password: ", "", 16, "SECURE")
			SendModem CRLF
			if CheckPassword(ln, Password) then do
				Password = ""
				do while Password = ""
					Password = AskInput(ln, "Enter a new password: ", "", 16, "SECURE")
					SendModem CRLF
					Pwv = AskInput(ln, "Enter again for verification: ", "", 16, "SECURE")
					SendModem CRLF
					if Password ~= Pwv then Password = ""
					end
				SetPassword Password
				end
			else SendModem "Invalid password"||CRLF
			end
		when cmdid = "Q" then exit 0
		otherwise
		end
	end
