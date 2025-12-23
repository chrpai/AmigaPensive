
/*
** EditFile.rexx -- Example REXX script for updating a files size.
*/

Options Results
Parse Arg Filename Size

Address 'ZEUS'

if Filename = '' then Signal Usage
if Size     = '' then Signal Usage

EditFile 'FileName' '"'Filename'"' 'Size' '"'Size'"'
Select
	When RC = 0  then say 'File Edited Successfully.'
	When RC = 2  then say 'Could not edit file!'
	When RC = 5  then say 'No such file!'
	When RC > 5  then say 'Fatal error!'
	end

exit

Usage:
	say 'Usage: EditFile.rexx <FileName> <New file size>'
