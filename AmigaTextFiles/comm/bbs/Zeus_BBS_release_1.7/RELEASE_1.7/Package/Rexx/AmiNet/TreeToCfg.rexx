/*
** TreeToCfg.rexx - Written by Alex May
*/

Options Results
Address 'ZEUS'

Parse Arg Base Area

Base = strip( Base )
Area = strip( Area )

if Base = '' | Area = '' | ~datatype( Area, 'n' ) then do
	say 'Usage: rx TreeToCfg <Aminet Base Directory> <File Area>'
	exit
	end
	
if right( Base, 1 ) ~= ':' & right( Base, 1 ) ~= '/' then Base = Base'/'

if ~open( 'tree', Base || 'TREE', 'r' ) then do
	say 'Can''t open 'Base || 'TREE for reading...'
	exit
	end

count = 0

do while ~eof( 'tree' )
	data = readln( 'tree' )
	
	if data ~= '' then do
		parse var data dir desc
		
		dir  = strip( dir )
		desc = strip( desc )
		
		if length( dir ) > 0 & length( desc ) > 0 & index( dir, '/' ) > 0 then do
			if MakePath( Base, dir || '/' ) then do
				say 'Generating area 'area' ('dir' - 'desc')'
			
				SetFileArea 'AREA' area
				
				if RC = 0 then do
					SetFileArea 'AREA' area 'NAME' '"Aminet: 'desc'"'
					SetFileArea 'AREA' area 'CLEARPATHS'
					SetFileArea 'AREA' area 'PATH' '"'Base || dir'/"'
					SetFileArea 'AREA' area 'LOW' 0
					SetFileArea 'AREA' area 'HIGH' 255
					SetFileArea 'AREA' area 'DLBAUD' 300
					SetFileArea 'AREA' area 'ULBAUD' 0
					SetFileArea 'AREA' area 'AGE' 0
					SetFileArea 'AREA' area 'DOWNLOADS'
					SetFileArea 'AREA' area 'NOUPLOADS'
					SetFileArea 'AREA' area 'VIEWING'
					SetFileArea 'AREA' area 'NOFREEFILES'
					SetFileArea 'AREA' area 'NOVALIDATION'
					SetFileArea 'AREA' area 'NODOSNAMES'
					SetFileArea 'AREA' area 'NOCOPYTOTEMP'
				
					count = count + 1
					area  = area  + 1
					end
				end
			end
		end
	end

call close( 'tree' )

if count > 0 then do
	say 'Saving new file areas...'
	SetFileArea 'Save'
	say 'Reloading file areas...'
	SetFileArea 'Reload'
	end

exit

MakePath: procedure
	parse arg dir, path

	old = pragma( 'd', dir )

	done = 0
	lpth = ''
	ret  = 0

	do while done = 0
		plen = length( path )
		spos = pos( '/', path )

		if spos > 0 then do
			dirbit = left( path, spos - 1 )
			
			if lpth ~= '' then lpth = lpth'/'dirbit
			else			       lpth = dirbit
			
			path = right( path, plen - spos )
			
			if ~exists( lpth ) & ~makedir( lpth ) then do
				say '+++ Error creating 'dir || lpth
				done = 1
				end
			end
		else do
			done = 1
			ret  = 1
			end
		end

	call pragma( 'd', old )

	return( ret )
