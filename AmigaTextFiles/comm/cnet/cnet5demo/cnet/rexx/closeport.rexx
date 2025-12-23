/* Small script for the small arexxclass demo */

options results

/* get port number from args */
parse arg port

/* adress CNet control ARexx port */
if show('P', "CONTROLREXX.1") then do
	address CONTROLREXX.1

	/* Tell control to run the port  */
	CLOSEPORT 'PORT='port

	/* echo result to user */
	say 'CLOSEPORT 'port' RESULT: ' RESULT
end
