/*
**
** MsgTxtCfgCompiler.rexx -- Reads the file created by 'MsgTxtCfgDump' and
**                           creates a new area list
**
*/

Options Results
Address 'ZEUS'

Parse Arg FileName

if FileName = '' then FileName = 'Zeus:Config/Msg/MsgAreas.Txt'

if ~open( 'arealist', FileName, 'r' ) then do
	say 'Can''t open 'FileName' for read.'
	exit
	end

Number = 0
cfgline = 0

SetMsgArea 'RemoveAll'

do until eof( 'arealist' )
	ln = readln( 'arealist' )

	cfgline = cfgline + 1

	if left( ln, 1 ) ~= ';'&length( ln ) > 0 then do
		NiceLine '"'ln'"'
		ln = RESULT

		Select
			when Left( ln, 6 ) = 'Number' then do
				Number = Right( ln, Length( ln ) - 7 )
				SetMsgArea 'Area' '"'Number'"'
				end

			when Left( ln, 4 ) = 'Name' then do
				if Number ~= '0' then do
					Name = Right( ln, Length( ln ) - 5 )
					SetMsgArea 'Area' '"'Number'"' 'Name' '"'Name'"'

					say 'Adding Area [[1m'Left(Number,5)'- 'Name'[0m]...'
					end
				end

			when Left( ln, 7 ) = 'TagName' then do
				if Number ~= '0' then do
					TagName = Right( ln, Length( ln ) - 8 )
					SetMsgArea 'Area' '"'Number'"'  'TagName' '"'TagName'"'
					end
				end

			when Left( ln, 8 ) = 'BaseType' then do
				if Number ~= '0' then do
					BaseType = Right( ln, Length( ln ) - 9 )
					SetMsgArea 'Area' '"'Number'"'  'BaseType' '"'BaseType'"'
					end
				end

			when Left( ln, 7 ) = 'MaxMsgs' then do
				if Number ~= '0' then do
					MaxMsgs = Right( ln, Length( ln ) - 8 )
					SetMsgArea 'Area' '"'Number'"'  'MaxMsgs' '"'MaxMsgs'"'
					end
				end

			when Left( ln, 6 ) = 'LowAcc' then do
				if Number ~= '0' then do
					LowAcc = Right( ln, Length( ln ) - 7 )	
					SetMsgArea 'Area' '"'Number'"'  'LowAcc' '"'LowAcc'"'
					end
				end

			when Left( ln, 7 ) = 'HighAcc' then do
				if Number ~= '0' then do
					HighAcc = Right( ln, Length( ln ) - 8 )
					SetMsgArea 'Area' '"'Number'"'  'HighAcc' '"'HighAcc'"'
					end
				end

			when Left( ln, 7 ) = 'Address' then do
				if Number ~= '0' then do
					Address = Right( ln, Length( ln ) - 8 )
					SetMsgArea 'Area' '"'Number'"'  'Address' '"'Address'"'
					end
				end

			when Left( ln, 3 ) = 'Exe' then do
				if Number ~= '0' then do
					Exe = Right( ln, Length( ln ) - 4 )
					SetMsgArea 'Area' '"'Number'"'  'Exe' '"'Exe'"'
					end
				end

			when Left( ln, 8 ) = 'FollowUp' then do
				if Number ~= '0' then do
					Followup = Right( ln, Length( ln ) - 9 )
					SetMsgArea 'Area' '"'Number'"'  'FollowUp' '"'FollowUp'"'
					end
				end

			when Left( ln, 6 ) = 'Origin' then do
				if Number ~= '0' then do
					Origin = Right( ln, Length( ln ) - 7 )
					SetMsgArea 'Area' '"'Number'"'  'Origin' '"'Origin'"'
					end
				end

			when Left( ln, 6 ) = 'MsgLib' then do
				if Number ~= '0' then do
					MsgLib = Right( ln, Length( ln ) - 7 )
					SetMsgArea 'Area' '"'Number'"'  'MsgLib' '"'MsgLib'"'
					end
				end

			when Left( ln, 3 ) = 'Age' then do
				if Number ~= '0' then do
					Age = Right( ln, Length( ln ) - 4 )
					SetMsgArea 'Area' '"'Number'"'  'Area' '"'Area'"'
					end
				end

			when Left( ln, 6 ) = 'Gender' then do
				if Number ~= '0' then do
					Gender = Right( ln, Length( ln ) - 7 )
					SetMsgArea 'Area' '"'Number'"'  'Gender' '"'Gender'"'
					end
				end

			when Left( ln, 5 ) = 'Flags' then do
				if Number ~= '0' then do
					if SubStr( ln, 7, 1 ) = 'K' then SetMsgArea 'Area' '"'Number'"' 'Kludges'
					else				 SetMsgArea 'Area' '"'Number'"' 'NoKludges'

					if SubStr( ln, 8, 1 ) = 'P' then SetMsgArea 'Area' '"'Number'"' 'Paths'
					else				 SetMsgArea 'Area' '"'Number'"' 'NoPaths'

					if SubStr( ln, 9, 1 ) = 'S' then SetMsgArea 'Area' '"'Number'"' 'SeenBys'
					else				 SetMsgArea 'Area' '"'Number'"' 'NoSeenBys'

					if SubStr( ln, 10, 1 ) = 'V' then SetMsgArea 'Area' '"'Number'"' 'Private'
					else				 SetMsgArea 'Area' '"'Number'"' 'NoPrivate'

					if SubStr( ln, 11, 1 ) = 'H' then SetMsgArea 'Area' '"'Number'"' 'HiAscii'
					else	 			  SetMsgArea 'Area' '"'Number'"' 'NoHiAscii'

					if SubStr( ln, 12, 1 ) = 'Q' then SetMsgArea 'Area' '"'Number'"' 'Quoting'
					else				  SetMsgArea 'Area' '"'Number'"' 'NoQuoting'

					if SubStr( ln, 13, 1 ) = 'A' then SetMsgArea 'Area' '"'Number'"' 'Alias'
					else				  SetMsgArea 'Area' '"'Number'"' 'NoAlias'

					if SubStr( ln, 14, 1 ) = 'U' then SetMsgArea 'Area' '"'Number'"' 'UserDelMsgs'
					else				  SetMsgArea 'Area' '"'Number'"' 'NoUserDelMsgs'

					if SubStr( ln, 15, 1 ) = 'M' then SetMsgArea 'Area' '"'Number'"' 'Moderator'
					else				  SetMsgArea 'Area' '"'Number'"' 'NoModerator'

					if SubStr( ln, 16, 1 ) = 'T' then SetMsgArea 'Area' '"'Number'"' 'TextStyles'
					else				  SetMsgArea 'Area' '"'Number'"' 'NoTextStyles'

					if SubStr( ln, 17, 1 ) = 'N' then SetMsgArea 'Area' '"'Number'"' 'AnsiBase'
					else				  SetMsgArea 'Area' '"'Number'"' 'NoAnsiBase'

					end
				end

			otherwise do
				say 'Unknown configuration tag on line 'cfgline'.'

				SetMsgArea 'Reload'

				exit
				end
			end
		end
	end

SetMsgArea 'Save'

SetMsgArea 'Reload'

call close( 'arealist' )

exit

