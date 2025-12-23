/* Write LastUser Quote */

options results
signal on syntax
'bufferflush'

'sendstring' "\n1\c7Do you want to log-off [y/N] \c6";getchar
   if result = '###PANIC' then Call Abort
   if upper(result) = "Y" then do
   'transmit' "Yes"

   'getuser' 1;_User=result

   if ~Open(_File,'EX.LastUser','W') then exit
   writeln(_File,_User)
   'query' '\c7Please enter a logoff quote: \c6';_Quote=result
   if result = '###PANIC' then Call CloseDown
   writeln(_File,_Quote)

	if _Quote ~= "" then do
		'sendstring' "\n1Please wait..."
		ret = open(q1,'RAM:TempQuote1','W')
		writeln(q1,"\c6"_Quote"\n1   \c7-- \q1"_User", \c1"date()"\q1\n1")
		close(q1)
		address command "Copy EX.Quotes RAM:TempQuote2 clone"
		address command "Trimfile RAM:TempQuote2 -49"
		address command "Join RAM:TempQuote2 RAM:TempQuote1 AS EX.Quotes"
		'transmit' "\n1"
	end

   'sendfile' "Xcel:Text/Text.Logout"
CloseDown:
   close (_File)
Abort:
   'Bufferflush'
   'hangup'
   exit
end

'sendstring' 'No'
exit
