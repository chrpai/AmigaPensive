
/*
**
** MsgTxtCfgDump.rexx -- Output the message config to a file, for use with 'MsgTxtCfgCompiler'
**
*/

Options Results
Address 'ZEUS'

Parse Arg FileName

CR = '0a'x

if FileName = '' then FileName = 'Zeus:Config/Msg/MsgAreas.Txt'

if ~open( 'arealist', FileName, 'w' ) then do
	say 'Can''t open 'FileName' for output.'
	exit
	end

fin = 0
i   = 0

hdr = '; Message Area Configuration'CR';'CR';   Number -- Number of Message Area'CR';     Name -- Descriptive Name of Area'CR'; BaseType -- Local, Echomail, Netmail, UUCP, PvtUUCP'CR';  MaxMsgs -- Maximum Number of Messages to keep'CR';   LowAcc -- Low Access for Area'CR';  HighAcc -- High Access for Area'CR';  Address -- 3,4,5 Dimensional FidoNet address'CR';   Origin -- Origin Number (0 for Random)'CR';      Exe -- Executable Number (0 for no processing)'CR';   MsgLib -- Message Base Library'CR'; FollowUp -- UseNet followup base'CR';      Age -- Minimum Age to Access Area'CR';   Gender -- Gender Restrictions (n/a, Male, Female)'CR';    Flags -- [KPSPHQAU]'CR';        K -- Show Kludges'CR';        P -- Show Paths'CR';        S -- Show Seenbys'CR';        V -- Private Message'CR';        H -- Hi Ascii base'CR';        Q -- Quoting allowed'CR';        A -- Aliases allowed'CR';        U -- Users may delete messages'CR';        M -- Moderator base'CR

call writech( 'arealist', hdr )

do until fin = 1
	GetMsgArea 'Ordinal' '"'i'"' 'Number'
    	if RC = 0 then do
        	call writeln( 'arealist', 'Number      'RESULT )

	        GetMsgArea 'Ordinal' '"'i'"' 'Name'
		call writeln( 'arealist', 'Name        'RESULT )

	        GetMsgArea 'Ordinal' '"'i'"' 'TagName'
        	if RESULT ~= 'RESULT' then call writeln( 'arealist', 'TagName     'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'BaseType'
		BaseType = RESULT
		call writeln( 'arealist', 'BaseType    'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'MaxMsgs'
		call writeln( 'arealist', 'MaxMsgs     'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'LowAcc'
		call writeln( 'arealist', 'LowAcc      'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'HighAcc'
		call writeln( 'arealist', 'HighAcc     'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'Exe'
		call writeln( 'arealist', 'Executable  'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'MbLib'
		call writeln( 'arealist', 'MsgLib      'RESULT )

		if BaseType = 'UUCP' then do
			GetMsgArea 'Ordinal' '"'i'"' 'FollowUp'
			if RESULT ~= 'RESULT' then do
				call writeln( 'arealist', 'FollowUp    'RESULT )
				end
			end

		if BaseType ~= 'Local' then do
			GetMsgArea 'Ordinal' '"'i'"' 'Origin'
			call writeln( 'arealist', 'Origin      'RESULT )

			GetMsgArea 'Ordinal' '"'i'"' 'Address'
			if BaseType ~= 'Local' then call writeln( 'arealist', 'Address     'RESULT )
			end

		GetMsgArea 'Ordinal' '"'i'"' 'Age'
		call writeln( 'arealist', 'Age         'RESULT )

		GetMsgArea 'Ordinal' '"'i'"' 'Gender'
		call writeln( 'arealist', 'Gender      'RESULT )

		Flags = 'Flags       '
		call writech( 'arealist', Flags )

		GetMsgArea 'Ordinal' '"'i'"' 'Kludges'
		if RESULT = '1' then	call writech( 'arealist', 'K' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'Paths'
		if RESULT = '1' then	call writech( 'arealist', 'P' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'SeenBys'
		if RESULT = '1' then 	call writech( 'arealist', 'S' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'Private'
		if RESULT = '1' then 	call writech( 'arealist', 'V' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'HiAscii'
		if RESULT = '1' then 	call writech( 'arealist', 'H' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'Quoting'
		if RESULT = '1' then 	call writech( 'arealist', 'Q' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'Alias'
		if RESULT = '1' then 	call writech( 'arealist', 'A' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'UserDelMsgs'
		if RESULT = '1' then	call writech( 'arealist', 'U' )
				else	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'Moderator'
		if RESULT = '1' then	call writech( 'arealist', 'M' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'TextStyles'
		if RESULT = '1' then	call writech( 'arealist', 'T' )
				else 	call writech( 'arealist', '-' )

		GetMsgArea 'Ordinal' '"'i'"' 'AnsiBase'
		if RESULT = '1' then	call writeln( 'arealist', 'N' )
				else 	call writeln( 'arealist', '-' )

		call writeln( 'arealist', ';' )
        	i = i + 1
        	end
    	else
        	fin = 1
    	end

say i' Message Areas scanned.'
