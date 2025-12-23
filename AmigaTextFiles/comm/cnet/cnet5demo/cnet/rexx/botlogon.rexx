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

	/* adress the CNet port now that the botlogon has begun */
	address CNETREXX0

	/* tell the terminal type input prompt we want ANSI */
	addkeys 'I'

	/* enter the bot user's account number */
	addkeys "1!"||d2c(13)

	/* enter bot user's password */
	addkeys "botpass"||d2c(13)

	/* bypass all prompts */
	addkeys d2c(13)
	addkeys d2c(13)

	/* enter some commands to demonstrate bot capability */
	addkeys "WHO"||d2c(13)
	lastcommand
	lastcmd=result

	addkeys "OL0!"||d2c(13)
	addkeys "Hi, how are you?"||d2c(13)

	/* bypass OLM from myself */
	addkeys d2c(13)
	addkeys d2c(13)
	addkeys d2c(13)

	/* repeast command held in last command variable, retrieved after WHO was entered! */
	addkeys lastcmd||d2c(13)

	/* logoff */
	addkeys "o!"||d2c(13)


end
