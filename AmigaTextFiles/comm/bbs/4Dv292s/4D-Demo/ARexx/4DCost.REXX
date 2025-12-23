/* this will calculate a cost of call so far price by asking 4D what the */
/* time left of the user is, what thier time-per-call is, and subtarcting */
/* two two. Then multplying by a defined cost. */

/* call this script from 4d like:    t:c A Rexx:4DCost.Rexx    */

options results

COST_PER_MINUTE=.75

/* lets make sure 4D is running.... :) */


if ~show(p, '4D-BBS0') then
	do
	say '4D-BBS ARexx Port Not Found...'
	exit
	end


/* send all our commands by default to 4D */

address '4D-BBS0'

/* get the info that we need. */
/* note: Most arexx commands return the info in RESULT. I made these */
/* special.  use same parameter with 'systeminfo' command to get results */
/* in the RESULT string as a STRING, not numeric. */

'userinfo' 'n'
usertime=RC
say RESULT

'message' 'You Are allowed' usertime 'minutes per call.'

'userinfo' 'm'
timeleft=RC
say RESULT

'message' 'You Have' usertime 'minutes left this call.'

timesofar=usertime-timeleft

'message' 'You have been on-line for ' usertime 'minutes.'

cost=timesofar*COST_PER_MINUTE

'message' ' '
'message' ' '
'message' ' '

'message' 'This Call so Far Will Cost $' cost

'message' ' '
'message' ' '


/* tell 4D we're done */

'exit'

exit

