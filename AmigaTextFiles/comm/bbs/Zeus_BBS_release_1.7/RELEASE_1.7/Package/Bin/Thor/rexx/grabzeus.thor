/*
** GrabZeus.thor -- Grabs the latest messages for your current BBS
**
** (c) 1995 Nick Loman
*/

/* needs THOR functions */

options results

p = address() || ' ' || show('P',,)
thorport = pos('THOR.',p)

if thorport > 0 then do
	thorport = word(substr(p,thorport),1)
	end
else do
	say 'No THOR port found!'
	exit 10
	end

/* get pubscreen */

GetGlobalConfig GCfg
if rc = 0 then do

	/* get current system */

	CurrentSystem Sys
	if RC = 0 then do

		call open( 'pn', 'Env:Thor/THORPATH' )
		thorpath = readln( 'pn' )
		call close( 'pn' ) 

		address command thorpath'Bin/parsezeus BBSNAME 'Sys.BBSNAME' PUBSCREEN 'GCfg.PubScreenName

		end
	else
		say 'Could not get current system!'

	end
else
	say 'Could not get global config!'

exit
