
/*
**
** FileTxtCfgDump.rexx -- Output the file config to a file.
**
*/

Options Results
Address 'ZEUS'

Parse Arg FileName

CR = '0a'x

if FileName = '' then FileName = 'Zeus:Config/File/FileAreas.Txt'

if ~open( 'arealist', FileName, 'w' ) then do
	say 'Can''t open 'FileName' for output.'
	exit
	end

fin = 0
i   = 0

hdr = '; File Area Configuration'CR';'CR'; Number     -- File Area Number'CR'; Name       -- Name of File Area'CR'; Path       -- Path to Area'CR';  ..        -- Multiple Paths'CR'; Low        -- Low Access to Area'CR'; High       -- High Access to Area'CR'; MinDlBaud  -- Minimum Download Baud'CR'; MinUlBaud  -- Minimum Upload Baud'CR'; MinAge     -- Minimum Age to Access Area'CR'; Flags [DUVFDC] - 'CR';  Downloads  -- Allow Downloads?'CR';  Uploads    -- Allow Uploads?'CR';  Viewing    -- View Area?'CR';  FreeFiles  -- Make Files Free?'CR';  Validation -- File Area requires validation?'CR';  DosNames   -- 8.3 format file names?'CR';  CopyToTemp -- Copy files to temp. dir.'CR';'CR

call writech( 'arealist', hdr )

say 'Outputting file areas...'

do until fin = 1
	GetFileArea 'Ordinal' '"'i'"' 'Number'
	if RC = 0 then do
		call writeln( 'arealist', 'Number     'RESULT )

		GetFileArea 'Ordinal' '"'i'"' 'Name'
		call writeln( 'arealist', 'Name       'RESULT )

		GetFileArea 'Ordinal' '"'i'"' 'Path'
		call writeln( 'arealist', 'Path       'RESULT )

		path = 1
		Ret  = ''
		do until Ret = 'RESULT'
			GetFileArea 'Ordinal' '"'i'"' 'Path' '"'path'"'
			if RC = 5 then RESULT = 'RESULT'

			Ret = RESULT
			if Ret ~= 'RESULT' then do
				call writeln( 'arealist', 'Path       'Ret )

				path = path + 1
				end
			end

		GetFileArea 'Ordinal' '"'i'"' 'Low'
		call writeln( 'arealist', 'Low        'RESULT )

		GetFileArea 'Ordinal' '"'i'"' 'High'
		call writeln( 'arealist', 'High       'RESULT )

		GetFileArea 'Ordinal' '"'i'"' 'MinDlBaud'
		call writeln( 'arealist', 'MinDlBaud  'RESULT )

		GetFileArea 'Ordinal' '"'i'"' 'MinUlBaud'
		call writeln( 'arealist', 'MinUlBaud  'RESULT )

		GetFileArea 'Ordinal' '"'i'"' 'MinAge'
		call writeln( 'arealist', 'MinAge     'RESULT )

		Flags = 'Flags      '
		call writech( 'arealist', Flags )

		GetFileArea 'Ordinal' '"'i'"' 'Downloads'
		if RESULT = '1' then	call writech( 'arealist', 'D' )
				else	call writech( 'arealist', '-' )

		GetFileArea 'Ordinal' '"'i'"' 'Uploads'
		if RESULT = '1' then	call writech( 'arealist', 'U' )
				else	call writech( 'arealist', '-' )

		GetFileArea 'Ordinal' '"'i'"' 'Viewing'
		if RESULT = '1' then	call writech( 'arealist', 'V' )
				else	call writech( 'arealist', '-' )

		GetFileArea 'Ordinal' '"'i'"' 'FreeFiles'
		if RESULT = '1' then	call writech( 'arealist', 'F' )
				else	call writech( 'arealist', '-' )

		GetFileArea 'Ordinal' '"'i'"' 'Validation'
		if RESULT = '1' then	call writech( 'arealist', 'L' )
				else	call writech( 'arealist', '-' )

		GetFileArea 'Ordinal' '"'i'"' 'DosNames'
		if RESULT = '1' then	call writech( 'arealist', 'N' )
				else	call writech( 'arealist', '-' )

		GetFileArea 'Ordinal' '"'i'"' 'CopyToTemp'
		if RESULT = '1' then	call writeln( 'arealist', 'C' )
				else	call writeln( 'arealist', '-' )

		call writeln( 'arealist', ';' )

		i = i + 1
		end
	else
		fin = 1
	end

call close( 'arealist' )

say i' File Areas scanned.'

