/* $VER: AreaHeaders.rexx v1.0
 *
 * Creates area header files for all file areas for use with Lightning
 * Filelister (c)1996 Rod schnell.
 *
 */

Options Results
TDir = 't:'

CR	 		= '0d'x
LF	 		= '0a'x
CRLF 		= '0d0a'x

Address 'ZEUS'

fin   = 0
i     = 0
count = 0
high  = 500  /* Highest area number to create headers for */

say 'Creating Lightning Filelister area headers...'
say ''

do until fin = 1
	GetFileArea '"'i'"' 'Number'
	if RC = 0 then do
		Number = RESULT
		count  = count + 1
		GetFileArea '"'i'"' 'Name'
		Name   = RESULT

		say '[A[1KArea: 'i' 'Name

		FileName = TDir||'Header.'i'.txt'
		if open( 'HeaderFile', FileName, 'w' ) then do
			TempLine = 'Area 'Number' 'Name 
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			TempLine = 'Executables virus checked using Virus_Checker'
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			call close( 'HeaderFile' )
		end
		else do
			say 'Can''t open 'FileName' for output.'
			fin = 1
		end


		FileName = TDir||'Header.'i'.ibm'
		if open( 'HeaderFile', FileName, 'w' ) then do
			TempLine = 'Area 'Number' 'Name 
			TempLine = center( TempLine, 80)
			call writech( 'HeaderFile', TempLine||CRLF)
			TempLine = 'Executables virus checked using Virus_Checker'
			TempLine = center( TempLine, 80)
			call writech( 'HeaderFile', TempLine||CRLF)
			call close( 'HeaderFile' )
		end
		else do
			say 'Can''t open 'FileName' for output.'
			fin = 1
		end

		FileName = TDir||'Header.'i'.guide'
		if open( 'HeaderFile', FileName, 'w' ) then do
			TempLine = 'Area 'Number' 'Name 
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			TempLine = 'Executables virus checked using Virus_Checker'
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			call close( 'HeaderFile' )
		end
		else do
			say 'Can''t open 'FileName' for output.'
			fin = 1
		end

		FileName = TDir||'NewHeader.'i'.txt'
		if open( 'HeaderFile', FileName, 'w' ) then do
			TempLine = 'Area 'Number' 'Name 
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			TempLine = 'Executables virus checked using Virus_Checker'
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			call close( 'HeaderFile' )
		end
		else do
			say 'Can''t open 'FileName' for output.'
			fin = 1
		end

		FileName = TDir||'NewHeader.'i'.ibm'
		if open( 'HeaderFile', FileName, 'w' ) then do
			TempLine = 'Area 'Number' 'Name 
			TempLine = center( TempLine, 80)
			call writech( 'HeaderFile', TempLine||CRLF)
			TempLine = 'Executables virus checked using Virus_Checker'
			TempLine = center( TempLine, 80)
			call writech( 'HeaderFile', TempLine||CRLF)
			call close( 'HeaderFile' )
		end
		else do
			say 'Can''t open 'FileName' for output.'
			fin = 1
		end

		FileName = TDir||'NewHeader.'i'.guide'
		if open( 'HeaderFile', FileName, 'w' ) then do
			TempLine = 'Area 'Number' 'Name 
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			TempLine = 'Executables virus checked using Virus_Checker'
			TempLine = center( TempLine, 80)
			call writeln( 'HeaderFile', TempLine)
			call close( 'HeaderFile' )
		end
		else do
			say 'Can''t open 'FileName' for output.'
			fin = 1
		end
	end
	i = i + 1
	if i >= high then fin = 1
end

say 'File area headers created for 'count' areas.'

