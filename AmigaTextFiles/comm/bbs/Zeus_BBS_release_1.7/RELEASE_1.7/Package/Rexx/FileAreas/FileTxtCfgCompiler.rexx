/*
**
** FileTxtCfgCompiler.rexx -- Reads the file created by 'FileTxtCfgDump' and
**                           creates a new area list
**
*/

Options Results
Address 'ZEUS'

Parse Arg FileName

if FileName = '' then FileName = 'Zeus:Config/File/FileAreas.Txt'

if ~open( 'arealist', FileName, 'r' ) then do
	say 'Can''t open 'FileName' for read.'
	exit
	end

Number    = 0
cfgline   = 0
firstpath = 1

SetFileArea 'RemoveAll'

do until eof( 'arealist' )
	ln = readln( 'arealist' )

	cfgline = cfgline + 1

	if left( ln, 1 ) ~= ';'&length( ln ) > 0 then do
		NiceLine '"'ln'"'
		ln = RESULT

		Select
			when Left( ln, 6 ) = 'Number' then do
				Number = Right( ln, Length( ln ) - 7 )
				SetFileArea 'Area' '"'Number'"'
				firstpath = 1
				end

			when Left( ln, 4 ) = 'Name' then do
				if Number ~= '0' then do
					Name = Right( ln, Length( ln ) - 5 )
					SetFileArea 'Area' '"'Number'"' 'Name' '"'Name'"'

					say 'Adding Area [[1m'Left(Number,5)'- 'Name'[0m]...'

					end
				end

			when Left( ln, 4 ) = 'Path' then do
				if Number ~= '0' then do
					Path = Right( ln, Length( ln ) - 5 )

					if firstpath = 1 then do
						SetFileArea 'Area' '"'Number'"' 'ClearPaths'
						SetFileArea 'Area' '"'Number'"' 'Path' '"'Path'"'
						firstpath = 0
						end
					else do
						SetFileArea 'Path' '"'Number'"' 'PathList' '"'Path'"'
						end
					end
				end


			when Left( ln, 3 ) = 'Low' then do
				if Number ~= '0'  then do
					Low = Right( ln, Length( ln ) - 4 )
					SetFileArea 'Area' '"'Number'"' 'Low' '"'Low'"'
					end
				end

			when Left( ln, 4 ) = 'High' then do
				if Number ~= '0'  then do
					High = Right( ln, Length( ln ) - 5 )
					SetFileArea 'Area' '"'Number'"' 'High' '"'High'"'
					end
				end

			when Left( ln, 9 ) = 'MinDlBaud' then do
				if Number ~= '0'  then do
					DlBaud = Right( ln, Length( ln ) - 10 )
					SetFileArea 'Area' '"'Number'"' 'DlBaud' '"'DlBaud'"'
					end
				end

			when Left( ln, 9 ) = 'MinUlBaud' then do
				if Number ~= '0'  then do
					UlBaud = Right( ln, Length( ln ) - 10 )
					SetFileArea 'Area' '"'Number'"' 'UlBaud' '"'UlBaud'"'
					end
				end

			when Left( ln, 6 ) = 'MinAge' then do
				if Number ~= '0'  then do
					UlBaud = Right( ln, Length( ln ) - 7 )
					SetFileArea 'Area' '"'Number'"' 'Age' '"'Age'"'
					end
				end

			when Left( ln, 5 ) = 'Flags' then do
				if Number ~= '0'  then do
					if SubStr( ln, 7, 1 ) = 'D'  then SetFileArea 'Area' '"'Number'"' 'Downloads'
					else				  SetFileArea 'Area' '"'Number'"' 'NoDownloads'

					if SubStr( ln, 8, 1 ) = 'U'  then SetFileArea 'Area' '"'Number'"' 'Uploads'
					else				  SetFileArea 'Area' '"'Number'"' 'NoUploads'

					if SubStr( ln, 9, 1 ) = 'V'  then SetFileArea 'Area' '"'Number'"' 'Viewing'
					else				  SetFileArea 'Area' '"'Number'"' 'NoViewing'

					if SubStr( ln, 10, 1 ) = 'F' then SetFileArea 'Area' '"'Number'"' 'FreeFiles'
					else				  SetFileArea 'Area' '"'Number'"' 'NoFreeFiles'

					if SubStr( ln, 11, 1 ) = 'L' then SetFileArea 'Area' '"'Number'"' 'Validation'
					else				  SetFileArea 'Area' '"'Number'"' 'NoValidation'

					if SubStr( ln, 12, 1 ) = 'N' then SetFileArea 'Area' '"'Number'"' 'DosNames'
					else				  SetFileArea 'Area' '"'Number'"' 'NoDosNames'

					if SubStr( ln, 13, 1 ) = 'C' then SetFileArea 'Area' '"'Number'"' 'CopyToTemp'
					else				  SetFileArea 'Area' '"'Number'"' 'NoCopyToTemp'
					end
				end

			otherwise do
				say 'Unknown configuration tag on line 'cfgline'.'

				SetFileArea 'Reload'

				exit
				end
			end
		end
	end

SetFileArea 'Save'

SetFileArea 'Reload'

call close( 'arealist' )

exit

