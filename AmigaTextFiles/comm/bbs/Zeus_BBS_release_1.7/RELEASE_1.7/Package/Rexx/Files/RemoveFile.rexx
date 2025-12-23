
/*
** DeleteFile.rexx -- Delete a file from the file catalogue
**                     AND the physical file from the Hard Drive
*/

Options Results
Parse Arg Filename

Address 'ZEUS'

if Filename = '' then Signal Usage

DeleteCatFile 'FileName' '"'Filename'"' 'Delete'
Select
	When RC = 0  then say Filename' deleted Successfully.'
	When RC = 5  then say 'Could not delete file!'
	When RC > 5  then say 'Fatal error!'
	end

exit

Usage:
	say 'Usage: DeleteFile.rexx <FileName>'
