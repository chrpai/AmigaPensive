
/*
** MoveFile.rexx -- Example REXX script for moving a file.
*/

Options Results
Parse Arg Filename Area

Address 'ZEUS'

if Filename = '' then Signal Usage
if Area     = '' then Signal Usage

MoveFile 'FileName' '"'Filename'"' 'Area' '"'Area'"'
Select
	When RC = 0  then say 'File Moved Successfully.'
	When RC > 1  then say 'Could not move file. RC: 'RC
	end

exit

Usage:
	say 'Usage: MoveFile.rexx <FileName> <New area>'
