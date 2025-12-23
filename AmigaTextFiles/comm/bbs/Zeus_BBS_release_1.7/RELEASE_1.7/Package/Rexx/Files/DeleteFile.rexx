
/*
** DeleteFile.rexx -- Delete a file from the file catalogue
*/

Options Results
Parse Arg Filename

Address 'ZEUS'

if Filename = '' then Signal Usage

DeleteCatFile 'FileName' '"'Filename'"'
Select
	When RC = 0  then say Filename' deleted Successfully.'
	When RC = 5  then say 'Could not delete file!'
	When RC > 5  then say 'Fatal error!'
	end

exit

Usage:
	say 'Usage: DeleteFile.rexx <FileName>'
