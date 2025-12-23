* CopyData ****************************************************************
*
* Copys a string from one buffer to another
*
*INPUTS
*    A0 = String to copy
*    A1 = Buffer to copy into
*    D0 = Number of characters to copy
*
*RESULTS
*    A0 = End of string copied
*    A1 = End of buffer to be copied to (after last character)
*
*NOTES
*    Will not copy quote (") marks.  Will stop copying if a CR (ASCII 10)
*    or a Null (ASCII 0) is found in the source string.
*
***************************************************************************
CopyData:
	Moveq	#0,D1
	Bsr	CopyDataExclude
	Rts
