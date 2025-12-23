/*
** AddFile.rexx
*/

Options Results
Address 'ZEUS'

Options Prompt 'Name of file to add: '
Pull FileName
if FileName = '' then exit

Options Prompt 'Description of file: '
Parse Pull Description
if Description = '' then exit

fin = 0
do until fin = 1
	i  = 0
	nl = 0

	do forever
		GetFileArea 'Ordinal' '"'i'"' 'Number'
		if RC = 0 then do
			Num = RESULT
			Num = Right( Num, 6 )

			GetFileArea 'Ordinal' '"'i'"' 'Name'
			Name = RESULT
			Name = Left( Name, 30 )

			if nl = 0 then do
				String = Num') 'Name
				nl = 1
				end
			else do
				say String||Num') 'Name
				nl = 0
				end
			i = i + 1
			end
		else
			break
		end
	if nl = 1 then say String

	Options Prompt 'File Area Number   : '
	Pull AreaNumber
	if AreaNumber = '' then exit
 
	GetFileArea '"'AreaNumber'"' 'Number'
	if RC = 0 then 	fin = 1
	else   		say 'No such area!'
	end

say

AddFile '"'FileName'"' '"'AreaNumber'"' 'DESCRIPTION' '"'Description'"'
select
	when RC = 0 then do
		say 'File saved OK!'
		end
	when RC = 1 then do
		say 'Error saving 'FileName' - 'RESULT
		end
	when RC = 2 then do
		say 'Could not move file!'
		end
	when RC = 3 then do
		say 'Bad description!'
		end
	when RC = 4 then do
		say 'Could not access file!'
		end
	when RC = 6 then do
		say 'Could not add file, no available paths for upload.'
		end
	otherwise do
		say 'Fatal error! Is Zeus running?'
		end
	end
exit
