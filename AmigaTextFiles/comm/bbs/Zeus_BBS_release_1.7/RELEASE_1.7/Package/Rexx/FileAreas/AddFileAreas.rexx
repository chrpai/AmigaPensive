/*
** AddFileAreas.rexx
**
** Usage: AddFileAreas.rexx <Area List> <Start Number>
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

do until eof( 'arealist' )
	area = readln( 'arealist' )

	if length( area ) > 0 then do
		SETFILEAREA 'Area' i 'Name' '"'area'"'
		say RC
		say RESULT

		SETFILEAREA 'Area' i 'Path' '"'area'"'
		SETFILEAREA 'Area' i 'Low' '0'
		SETFILEAREA 'Area' i 'High' '255'
		SETFILEAREA 'Area' i 'DlBaud' '1200'
		SETFILEAREA 'Area' i 'UlBaud' '300'
		SETFILEAREA 'Area' i 'Downloads'
		SETFILEAREA 'Area' i 'Uploads'
		SETFILEAREA 'Area' i 'Viewing'
		SETFILEAREA 'Area' i 'Validation'

		SETFILEAREA 'Save'
		i = i + 1
		end
	end

call close( 'arealist' )

exit

Usage:
	say 'Usage: AddFileAreas.rexx AreaList StartNo'
	exit
	end
