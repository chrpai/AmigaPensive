/*
** AddMsgAreas.rexx
**
** Usage: AddMsgAreas.rexx <Area List> <Start Number>
**
** (c) 1995 Nick Loman
*/

Options Results

Parse Arg AreaList StartNo

Address 'ZEUS'

if AreaList = '' then Signal Usage
if StartNo  = '' then Signal Usage

if ~open( 'arealist', AreaList, 'r' ) then do
	say 'Can''t open 'AreaList' for input.'
	exit
	end

i = StartNo

say 'Adding message areas...'

do until eof( 'arealist' )
	area = readln( 'arealist' )

	if length( area ) > 0 then do
		SETMSGAREA 'Area' '"'i'"'

		SETMSGAREA 'Area' '"'i'"' 'Name' 		'"'area'"'
		SETMSGAREA 'Area' '"'i'"' 'TagName' 	'"'area'"'
		SETMSGAREA 'Area' '"'i'"' 'BaseType' 	'"Echomail"'
		SETMSGAREA 'Area' '"'i'"' 'MaxMsgs' 	'200'
		SETMSGAREA 'Area' '"'i'"' 'LowAcc' 		'0'
		SETMSGAREA 'Area' '"'i'"' 'HighAcc' 	'255'
		SETMSGAREA 'Area' '"'i'"' 'Address' 	'2:441/58.0'
		SETMSGAREA 'Area' '"'i'"' 'Origin' 		'"Running ZEUS * The BEST Amiga BBS"'
		SETMSGAREA 'Area' '"'i'"' 'MsgLib' 		'"mbzeus.library"'
		SETMSGAREA 'Area' '"'i'"' 'Gender' 		'UNKNOWN'
		SETMSGAREA 'Area' '"'i'"' 'Age' 			'0'

		SETMSGAREA 'Save'
		i = i + 1
		end
	end

call close( 'arealist' )

say 'Finished: Added 'i-StartNo' areas!'

exit

Usage:
	say 'Usage: AddMsgAreas.rexx AreaList StartNo'
	exit
	end
