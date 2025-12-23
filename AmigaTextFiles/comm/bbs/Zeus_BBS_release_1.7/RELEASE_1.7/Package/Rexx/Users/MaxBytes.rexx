/*
** MaxBytes - Test of SetUserFileData Rexx Function
*/

Options Results
Address 'ZEUS'

InitUserKey 'CHRONOFORWARD'
Key = RESULT
if RC ~= 0 then do
	say 'Error initialising userkey.'
	exit
	end

do forever
	UserFromKey Key
	User = RESULT
	if RC = 0 then do
		GetUserData 'NAME' User
		say 'Converting 'RESULT'...'

		SetUserFileData 'FREEFILES' '100' User

		SaveUser User
		if RC ~= 0 then say 'Error 'RC' saving user...'

		UnLoadUser User
		end

	NextUserKey Key
	if RC ~= 0 then leave
	end

EndUserKey Key
