/* script for file-task DCC send */

options results

/* get port number from args */
parse arg id

/* address CNet file-task ARexx port */
if show('P', "FILETASK_RX.1") then do
	address FILETASK_RX.1

	/* Tell file-task to send the file */
	DCCSEND 'DCCID='id
end
