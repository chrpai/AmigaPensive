
/*
** Sets all the message bases to Text Styles ON!
*/

Options Results

Address 'ZEUS'

fin = 0
  i = 1

say 'Setting areas...'

do until fin = 1
	GetMsgArea 'Ordinal' '"'i'"' 'Number'
	if RC = 0 then do
		Number = RESULT

		SetMsgArea 'Area' '"'Number'"' 'TextStyles'
		SetMsgArea 'Area' '"'Number'"' 'NoAnsiBase'

		i = i + 1
		end
	else
		fin = 1
	end

say 'Saving configuration...'

SetMsgArea 'Save'

say 'Reloading configuration...'

SetMsgArea 'Reload'

say 'Done.'

exit
