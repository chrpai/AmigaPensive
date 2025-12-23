/*
**
*/

Options Results

/* --- Config ---------- */

Inbound = 'Inbound:'
UUX     = 'UUXT x'

/* --- End of Config --- */

tempfn = 'T:ProcMail.'pragma( 'i' )
infile = 0
olddir = pragma( 'd', Inbound )

do while ~eof( stdin )
	data = readln( stdin )

	select
		when left( data, 6 ) = 'begin ' then do
			parse var data temp1 temp2 fname
			fname = strip( fname )
			
			if exists( fname ) then do
				do while exists( fname )
					tempch = right( fname, 1 )
				
					if tempch >= '0' & tempch < '9' then do
						tempch = tempch + 1
						fname  = left( fname, length( fname ) - 1 )
						fname  = fname || tempch
						end
					else do
						fname = fname',1'
						end
					end
				end

			if ~open( 'tempfh', tempfn, 'w' ) then do
				say 'Cannot open 'tempfn' for writing...'
				signal fin
				end

			call writeln( 'tempfh', temp1' 'temp2' 'fname )

			infile = 1
			end

		when infile = 1 & left( data, 3 ) = 'end' then do
			call writeln( 'tempfh', 'end' )
			call close( 'tempfh' )
			infile = 0
			
			Address Command UUX' 'tempfn
			
			call delete( tempfn )
			end

		otherwise do
			if infile = 1 then call writeln( 'tempfh', data )
			end
		end
	end

fin:
call pragma( 'd', olddir )
