/*
** TheChef.rexx -- Bork Bork Bork your message bases!
**
** Usage: Add a new Text Processor in MsgConfig as:
**
**  "rx Zeus:Rexx/TheChef.rexx %s" Stack: 8192 Pri: 0
**
** Make sure "chef" is in 'c:'. Change 'ChefPath' to point it
** somewhere else.
**
** Thanks to Mat Bettinson for the concept!
*/

Options Results
Parse Arg FileName

ChefPath = 'Zeus:Bin/'

if FileName = '' then do
	say 'Usage: TheChef.rexx <File Name>'
	exit
	end

if ~show( libraries, 'rexxsupport.library' ) then
	if ~addlib( 'rexxsupport.library', 0, -30, 0 ) then exit

if ~open( 'fl', FileName, 'r' ) then do
	say 'Could not open 'filename'!'
	exit
	end

ChefIn = 't:ChefIn_'||time('s')
Chef   = 't:Chef_'||time('s')

do until eof( 'fl' )
	ln = readln( 'fl' )

	pos = index( ln, '>' )
	if pos > 0 & pos < 5 then do
		/* Don't process quote */
		If ~Open(out,Chef,'A') then Call Open(out,Chef,'W')
		Call WriteLN(out,ln)
		Call Close(out)
		end
	else do
		/* Do process quote */
		Call Open(out,ChefIn,'W')
		Call WriteLN(out,ln)
		Call Close(out)
		address command ChefPath'Chef <'ChefIn' >>'Chef
		end
	end

call close( 'fl' )

address command 'c:copy 'Chef' 'FileName

call delete( Chef )
call delete( ChefIn )

exit
