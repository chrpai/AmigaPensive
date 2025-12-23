/*
**
*/

Options Results

TempDir = 'Mail:Temp'
DestDir = 'Mail:Files'
UUEDec  = 'UUXT x'
B64Dec  = 'Base64Decode'
Temp    = 'T:Decode.tmp'

Address Command 'List 'TempDir'/#?.(uue|b64|1) >'Temp' QUICK NOHEAD FILES'

if ~open( 'in', Temp, 'r' ) then do
	say 'Can''t open 'Temp' for reading...'
	exit
	end

OldDir = pragma( 'd', DestDir )

do while ~eof( 'in' )
	file = readln( 'in' )
	
	if file ~= '' then do
		say 'Processing [1m'file'[0m...'

		stat = statef( TempDir'/'file )
		parse var stat ftype fsize fblks fbits fstuff

		if ftype ~= 'FILE' then iterate

		if fsize = 0 then do
			say 'File is zero length - deleting...'
			call delete( TempDir'/'file )
			iterate
			end

		ext  = right( file, length( file ) - lastpos( '.', file ) )

		if ext ~= '1' then do
			file = left( file, length( file ) - ( length( ext ) + 1 ) )
			
			call DecodeFile( TempDir'/'file'.'ext, DestDir'/'file, ext )

			end
		else do
			part = ext
			file = left( file, length( file ) - ( length( part ) + 1 ) )
			totl = right( file, length( file ) - lastpos( '.', file ) )
			file = left( file, length( file ) - ( length( totl ) + 1 ) )
			ext  = right( file, length( file ) - lastpos( '.', file ) )
			file = left( file, length( file ) - ( length( ext ) + 1 ) )
			join = TempDir'/'file'.'ext'.'totl'.1'
			
			do part = 2 to totl + 1
				if ~exists( TempDir'/'file'.'totl'.'part ) then leave
				join = join' 'TempDir'/'file'.'totl'.'part
				end

			if part = totl + 1 then do
				say 'All parts appear to be here.  Joining...'
				Address Command 'Join 'join' AS 'TempDir'/'file'.'ext
				if RC = 0 then do
					call delete( TempDir'/'file'.'ext'.'totl'.1' )
					
					do part = 2 to totl
						call delete( TempDir'/'file'.'totl'.'part )
						end
						
					call DecodeFile( TempDir'/'file'.'ext, DestDir'/'file, ext )
						
					end
				end
			end
		end
	end

call close( 'in' )

if exists( Temp ) then call delete( Temp )

pragma( 'd', OldDir )

exit

DecodeFile:
	parse arg src, dst, ext

	select
		when ext = 'uue' then Address Command UUEDec' 'src

		when ext = 'b64' then Address Command B64Dec' 'src' 'dst

		end

	if RC = 0 then call delete( src )
	else				call delete( dst )

	return
