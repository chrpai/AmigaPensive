
/*
** ChangeBaseLib.rexx
*/

Options Results
Parse Arg mblib

Address 'ZEUS'

if mblib = '' then do
	say 'Usage: ChangeBaseLib.rexx <Message Library Name>'
	exit
	end

fin = 0
i   = 1

do until fin = 1
	GetMsgArea 'Ordinal' '"'i'"' 'Number'
	if RC = 0 then do
		Number = RESULT

		SetMsgArea 'Area' '"'Number'"' 'MsgLib' '"'mblib'"'

        	i = i + 1
		end
	else
		fin = 1
    	end

if i ~= 0 then SetMsgArea 'Save'

say i' Message Areas scanned.'

exit
