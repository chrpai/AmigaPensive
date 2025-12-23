/* Small script for the small arexxclass demo */

options results

/* get port number from args */
parse arg port

/* adress CNet control ARexx port */
if show('P', "CONTROLREXX.1") then do
	address CONTROLREXX.1

	/* Tell control to run the port  */
	BOTLOGON 'PORT='port

	/* echo result to user */
	say 'BOTLOGON 'port' RESULT: ' RESULT

	/* address CNet BBS port */
	cnetport = "CNETREXX"port
	if show('P', cnetport) then do
		address value cnetport

		/* satisfy Handle/account prompt */
	   ADDKEYS "1" d2c(13)

		/* Enter SYSOP's password */
	   ADDKEYS "PITS"
	   ADDKEYS d2c(13)

		/* do other online commands, etc. */

	end
end
