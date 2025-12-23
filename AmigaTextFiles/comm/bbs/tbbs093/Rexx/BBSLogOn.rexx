/* A login script for TechnoBBS */

CR = D2C(13)
LF = D2C(10)
CRLF = CR||LF
ESC = D2C(27)
ln = arg(1)
LineName = Con_LineActive(ln)
if LineName = "" then LineName = Con_LogOn(ln)
address command
address VALUE LineName
SendASCII "Text/PreLogOn.txt"
UserName = ""
retry = 0
SetTimeLeft 5
LogEntry "Line 1: Connection"
do forever
	SetStatus "Logging on"
	SetUserCharSet "ISO"
	UserName = AskInput(ln, "Enter username: ", "", 36, "CAPITAL")
	if UserName ~= "" then do
		LoadUser UserName
		SendModem CRLF
		if GetUserName(ln) = "" then do
			retry = retry + 1
			IsNew = GetYesNo(ln, UserName||" - is this correct? ", 0, 1)
			SendModem CRLF
			if IsNew = 1 then do
				SetTimeLeft 30
				SetStatus "NewUser"
				SendASCII "Text/NewUser.txt"
				ClearUser
				accept = 0
				do while accept = 0
					UserName = AskInput(ln, "Enter your full name: ", UserName, 36, "CAPITAL")
					if Length(UserName) > 4 then do
						invalid = 0
						do pos = 1 to Length(UserName)
							char = substr(UserName, pos, 1)
							if (datatype(char) = 'NUM') then invalid = 1
							end
						if invalid ~= 0 then SendModem CRLF||"Name contains invalid characters"||CRLF||CRLF
						else do
							LoadUser UserName
							if GetUserName(ln) ~= "" then do
								ClearUser
								SendModem CRLF||"A user by that name already exists!"||CRLF||CRLF
								end
							else accept = 1
							end
						end
					else SendModem CRLF||"Your name must contain at least 5 characters"||CRLF||CRLF
					end
				SendModem CRLF
				call SetUserMisc ln, "ADDRESS", AskInput(ln, "Street address: ", "", 80)
				call SetUserMisc ln, "CITY", AskInput(ln, "City: ", "", 40)
				call SetUserMisc ln, "POSTCODE", AskInput(ln, "Postal code: ", "", 40)
				call SetUserMisc ln, "COUNTRY", AskInput(ln, "Country: ", "", 40)
				SendModem CRLF
				call SetUserMisc ln, "VPHONE", AskInput(ln, "Voice number: ", "", 40)
				call SetUserMisc ln, "DPHONE" , AskInput(ln, "Data number: ", "", 40)
				SendModem CRLF
/*				SendASCII "Text/StripAll.txt" */
/*				SetStripAll GetYesNo(ln, "Strip All ANSI-Codes? ", 0, 1) */
/*				SendModem CRLF */
				SendModem CRLF
				if GetStripAll(ln) = 0 then do
/*					SendASCII "Text/StripCols.txt" */
					if GetYesNo(ln, "Would you like to use ANSI-Colors? ", 1, 1) = 1 then SetStripCols 0
					else SetStripCols 1
					SendModem CRLF
					SendModem CRLF
					SendASCII "Text/InsDel.txt"
					if GetYesNo(ln, "Can your terminal display ANSI Insert/Delete lines? ", 1, 1) = 0 then SetSimpleTerm 1
					SendModem CRLF
					SendModem CRLF
					end
				SetTerminalLines AskInput(ln, "Terminal lines: ", "24", 3, "NUMERIC")
				SendModem CRLF
				CSet = ""
				do while CSet = ""
					SendModem CRLF
					SendASCII "Text/CharSet.txt"
					CSet = AskInput(ln, "Select: ", "", 2, "NUMERIC")
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
				SendModem CRLF
/*				if GetYesNo(ln, "Use short menus? ", 1, 1) then MenuType = ".etxt" */
/*				else MenuType = ".txt" */
				MenuType = ".etxt"
				SendModem CRLF
				SetUserExtension MenuType
				SendModem CRLF
				call SetUserMisc ln, "MSGCHECK", GetYesNo(ln, "Message check every logon? ", 1, 1)
				SendModem CRLF
				call SetUserMisc ln, "FILECHECK", GetYesNo(ln, "New files check every logon? ", 1, 1)
				SendModem CRLF
				call SetUserMisc ln, "LASTFSCAN", GetDateVal(ln)
				Protocol = ""
				do while Protocol = ""
					SendModem CRLF
					SendASCII "Text/Protocol.Txt"
					Protocol = Upper(AskInput(ln, "Select: ", "", 2))
					select
						when Protocol = "Z" then Protocol = "ZModem"
						when Protocol = "8" then Protocol = "ZModem8k"
						end
					end
				SetUserProtocol Protocol
				SendASCII "Text/Password.txt"
				Password = ""
				do while Password = ""
					Password = AskInput(ln, "Enter password to use: ", "", 16, "SECURE")
					SendModem CRLF
					Pwv = AskInput(ln, "Enter again for verification: ", "", 16, "SECURE")
					SendModem CRLF
					if Password ~= Pwv then Password = ""
					end
				SetPassword Password
				SetAccessPreset "User/Preset/NewUser.acc"
				SetUserName UserName
				SaveUser
				LogEntry "New User: "||UserName
				DoneLogOn
				LogEntry GetUserName(ln)||" logged on"
				SetStatus "Logged on"
				SendASCII "Text/LoggedOn.txt"
				call 'BBS:Rexx/BBSLogOnExtra.rexx' ln
				address command
/*				"BBS:Bin/TechMenu "||ln||" BBS:Menu/MainMenu.menu" */
/*				call 'BBS:Rexx/BBSGoodbye.rexx' ln */
				exit 0
				end
			else do
				if retry > 5 then do
					Disconnect
					exit 10
					end
				end
			end
		else do
			Password = AskInput(ln, "Enter password: ", "", 16, "SECURE")
			if CheckPassword(ln, Password) ~= 1 then do
				SendModem CRLF||"Invalid password"||CRLF||CRLF
				LogEntry "Password failure for "||GetUserName(ln)
				ClearUser
				UserName = ""
				retry = retry + 1
				if retry > 2 then do
					Disconnect
					exit 10
					end
				end
			else do
				LogEntry "It's "||GetUserName(ln)
				DoneLogOn
				LogEntry GetUserName(ln)||" logged on"
				SetStatus "Logged on"
				SendASCII "Text/LoggedOn.txt"
				call 'BBS:Rexx/BBSLogOnExtra.rexx' ln
				address command
/*				"BBS:Bin/TechMenu "||ln||" BBS:Menu/MainMenu.menu" */
/*				call 'BBS:Rexx/BBSGoodbye.rexx' ln */
				exit 0
				end
			end
		end
	end
