/* Extra stuff for logon */

CR = D2C(13)
LF = D2C(10)
CRLF = CR||LF
ESC = D2C(27)
ln = arg(1)
LineName = Con_LineActive(ln)
if LineName = "" then exit 10
address value LineName
SetStatus "News"
SendModem CRLF||"Checking for news.."
LastNews = GetUserMisc(ln, "LASTNSCAN")
if open('news', 'BBS:Text/News.txt', 'R') = 1 then do
	qflag = 0
	do while ~qflag
		if readln('news') < LastNews then qflag = 1
		if eof('news') then qflag = 1
		if ~qflag then do
			newsln = readln('news')
			if length(newsln) > 0 then do
				if open('tmpnews', "T:tmp.news"||ln, 'W') then do
					do cpyln = 1 to newsln
						call writeln 'tmpnews', readln('news')
						end
					call close 'tmpnews'
					SendModem CRLF||CRLF
					SendASCII "t:tmp.news"||ln
					SendModem CRLF
					call GetHotkey ln, "Press any key to continue..."
					address command
					"delete t:tmp.news"||ln
					address VALUE LineName
					end
				end
			end
		end
	call close 'news'
	call SetUserMisc ln, "LASTNSCAN", GetDateVal(ln)
	end
SendModem CRLF||CRLF
if GetUserMisc(ln, "MSGCHECK") = 1 then do
	SetStatus "Message check"
	SendModem CRLF||"Checking your mail.."||CRLF||CRLF
	if MessageCheck(ln) then do
		SendModem CRLF
		if GetYesNo(ln, "Read these messages now? ", 1, 1) then do
			oldarea = GetMsgArea(ln)
			ReadMarked
			ChgMsgArea oldarea
			end
		end
	else SendModem "No new mail found.."||CRLF||CRLF
	end
if GetUserMisc(ln, "FILECHECK") = 1 then do
	SetStatus "File check"
	SendModem CRLF||"Checking for new files.."||CRLF||CRLF
	ListFiles "-SO -A -N -B -D"||GetUserMisc(ln, "LASTFSCAN")
	call SetUserMisc ln, "LASTFSCAN", GetDateVal(ln)
	end
SendModem CRLF
exit 0
