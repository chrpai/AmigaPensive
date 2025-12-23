/* Small script to test/demo ARExx OLM command */

options results

/* get port number from args */
/* adress CNet control ARexx port */

if show('P', "CNETREXX0") then do
	address CNETREXX0

	/* OLM the port */
	OLM 0 This is a test from the user on port 0.  Will be a system OLM if no one is on port 0!
	success = result

	OLM 100 "This is a SYSTEM olm (so is any olm with a 'from' port equal to that of a non-occipied port."
	success = result

	OLM 100 "Just ONE MORE test!"
	success = result

	end
