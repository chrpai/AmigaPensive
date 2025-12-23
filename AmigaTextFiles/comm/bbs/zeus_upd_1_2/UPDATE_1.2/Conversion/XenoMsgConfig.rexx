/*
** XenoMsgConfig.rexx -- Xenolink message config converter.
*/

Options Results
Address 'ZEUS'

Parse Arg FileName

if FileName = '' then FileName = 'Xenolink:Configuration/Msg.Config'

if ~open( 'arealist', FileName, 'r' )  then do
	say 'Can''t open 'FileName' for read.'
	exit
	end

Number    = 0
cfgline   = 0
firstpath = 1

SetMsgArea 'RemoveAll'

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
					SetMsgArea 'Area' '"'Number'"'

					SetMsgArea 'Area' '"'Number'"' 'LowAcc' '0'
					SetMsgArea 'Area' '"'Number'"' 'HighAcc' '255'

					SetMsgArea 'Area' '"'Number'"' 'MsgLib' '"mbzeus.library"'

					SetMsgArea 'Area' '"'Number'"' 'Quoting'
					SetMsgArea 'Area' '"'Number'"' 'TextStyles'

					firstpath = 1
					end
				end

			when Left( ln, 11 ) = 'SectionName' then do
				if Number ~= '0' then do
					Name = Right( ln, Length( ln ) - 12 )
					SetMsgArea 'Area' '"'Number'"' 'Name' '"'Name'"'

					say 'Adding Area [[1m'Left(Number,6)'- 'Name'[0m]...'

					end
				end

			when Left( ln, 11 ) = 'SectionType' then do
				if Number ~= '0' then do
					Type = Word( ln, 2 )
					SetMsgArea 'Area' '"'Number'"' 'BaseType' '"'Type'"'
					end
				end

			when Left( ln, 7 ) = 'TagName' then do
				if Number ~= '0' then do
					TagName = Word( ln, 2 )
					SetMsgArea 'Area' '"'Number'"' 'TagName' '"'TagName'"'
					end
				end

			when Left( ln, 13 ) = 'MyAddressList' then do
				if Number ~= '0' then do
					Address = Word( ln, 2 )
					SetMsgArea 'Area' '"'Number'"'  'Address' '"'Address'"'
					end
				end

			when Left( ln, 11 ) = 'MaxMessages' then do
				if Number ~= '0' then do
					MaxMsgs = Word( ln, 2 )
					SetMsgArea 'Area' '"'Number'"'  'MaxMsgs' '"'MaxMsgs'"'					
					end
				end

			when Left( ln, 12 ) = 'AllowHandles' then do
				if Number ~= '0' then do
					Handles = Word( ln, 2 )
					if Handles = 'YES' then SetMsgArea 'Area' '"'Number'"'  'Alias'
					end
				end

			when Left( ln, 16 ) = 'AllowPrivateMail' then do
				if Number ~= '0' then do
					Private = Word( ln, 2 )
					if Private = 'YES' then SetMsgArea 'Area' '"'Number'"'  'Private'
					end
				end

			otherwise do
				nop
/*
				say 'Unknown configuration tag on line 'cfgline'.'

				SetMsgArea 'Reload'

				exit
*/
				end
			end
		end
	end

SetMsgArea 'Save'

SetMsgArea 'Reload'

call close( 'arealist' )

exit

