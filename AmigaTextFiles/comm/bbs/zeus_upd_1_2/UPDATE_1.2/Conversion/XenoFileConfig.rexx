/*
** XenoFileConfig.rexx -- Xenolink file config converter.
*/

Options Results
Address 'ZEUS'

Parse Arg FileName

if FileName = '' then FileName = 'Xenolink:Configuration/File.Config'

if ~open( 'arealist', FileName, 'r' )  then do
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
			when Left( ln, 13 ) = 'SectionNumber' then do
				Number = Right( ln, Length( ln ) - 14 )
				if Number = '0' then do
					say 'Area number 0 not supported in Zeus... skipping!'
					end
				else do
					SetFileArea 'Area' '"'Number'"'

					SetFileArea 'Area' '"'Number'"' 'Low' '0'
					SetFileArea 'Area' '"'Number'"' 'High' '255'

					SetFileArea 'Area' '"'Number'"' 'Downloads'
					SetFileArea 'Area' '"'Number'"' 'Uploads'
					SetFileArea 'Area' '"'Number'"' 'Viewing'
					SetFileArea 'Area' '"'Number'"' 'Validation'
					firstpath = 1
					end
				end

			when Left( ln, 11 ) = 'SectionName' then do
				if Number ~= '0' then do
					Name = Right( ln, Length( ln ) - 12 )
					SetFileArea 'Area' '"'Number'"' 'Name' '"'Name'"'

					say 'Adding Area [[1m'Left(Number,6)'- 'Name'[0m]...'

					end
				end

			when Left( ln, 19 ) = 'MSDOSFileNamesOnly' then do
				if Number ~= '0' then do
					if Right( ln, Length( ln ) - 20 ) = 'YES' then do
						SetFileArea 'Area' '"'Number'"' 'DosNames'
						end
					end
				end

			when Left( ln, 13 ) = 'FileDirectory' then do
				if Number ~= '0' then do
					Path = Word( ln, 3 )

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


			otherwise do
				nop
/*
				say 'Unknown configuration tag on line 'cfgline'.'

				SetFileArea 'Reload'

				exit
*/
				end
			end
		end
	end

SetFileArea 'Save'

SetFileArea 'Reload'

call close( 'arealist' )

exit

