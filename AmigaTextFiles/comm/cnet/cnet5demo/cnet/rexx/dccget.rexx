/* script for file-task DCC get */

options results

/* get port number from args */
parse arg id

/* address CNet file-task ARexx port */
if show('P', "FILETASK_RX.1") then do
	address FILETASK_RX.1

	/* Tell file-task to get an offered DCC file */
	DCCGET 'DCCID='id
end
