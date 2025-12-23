/* Closeport */

options results

/* get port number from args */
parse arg port

/* adress CNet control ARexx port */
if show('P', "CONTROLREXX.1") then do
	address CONTROLREXX.1

	/* Tell control to close the port  */
	CLOSEPORT 'PORT='port

end
