/*			Phone Security v2.5
                   ©1992 UseFull & UseLess Software

 	     A Phone Security System for All C-Net systems

  			  Complements of:
                           
			  UseFull & UseLess
                              Software

			  By: Troy Fridley

      For support Call:
			 The Wild Wild West
			    614-927-2076
		      19.2k HST/ARQ/ASL/V.42bis

      	      Or Write:
		         Troy Fridley
			 13597 Sudbury Dr. NW
			 Pickerington OH, 43147

To register this utility send Money, Candy, Computer Equipment, Stuff
you've written, ect, or whatever you may think it is worth.  Or if none of 
these how about calling my board and just letting me know your using it.

This file may be freely redistributed as long as this arcive remains
intact and no charge is made except for a nominal duplication fee.

And the all famous, The author of this software is not responsible for any
dammages that may arize from its use, Basilcly, USE IT AT YOUR OWN RISK!
*/

OPTIONS RESULTS
SIGNAL ON SYNTAX

ReqString = "\q1Please enter the last 4 digits of your phone #>\c1"
IllegalNote = "\q1The Sysop has been notified of your Illegal Logon Attempt!"
IllegalNote2 = "\c2Get Lost!\q1"
GoodLogin ="Thank you, you are cleared for Logon." 

CC = 4		/*How often you would like the system to check the callers
		  Phone #. Replace with 'CC = Random(x,y)' Where X=Minimum   
		  Number and Y=Maximum Number*/

Start:
	Getuser 1;handle = Result
	check = Exists('Pfiles:PhoneSecurity/'handle)
	if check = '1' then signal CheckUser
	GETUSER 22;Calls = RESULT
	IF (Calls%CC) = (Calls/CC) THEN SIGNAL Check
        Exit

Check:
	GETUSER 10;Phone = RESULT
	PARSE VAR Phone 1 Pre '-' 4 Sub 7 Post
	Num = LENGTH(Phone)
	IF Num < 8 | Num > 11 THEN SIGNAL Error
	IF Num <= 8 THEN PARSE VAR Phone '-' 1 Sub 4 Post
	DO i = 1 TO 3
		SENDSTRING ReqString 
		RECEIVE;Enter = RESULT
		IF Enter = '###PANIC' THEN SIGNAL Drop
		IF Post = Enter THEN SIGNAL Logon
	END

Illegal:
	GETUSER 1;Handle = RESULT
	GETUSER 1200540;LogonAtmpt = RESULT
	LOGENTRY Handle" tryed Illegal Logon!"
	TRANSMIT IllegalNote
	TRANSMIT IllegalNote2
	SETOBJECT (LogonAtmpt+1);PUTUSER 1200540
	CALL OPEN user,('Pfiles:PhoneSecurity/'handle),'W'
	CALL WRITELN user,handle
	DROPCARRIER
	EXIT

Logon:
	TRANSMIT GoodLogin
	EXIT

Error:
	TRANSMIT "Something is wrong with your Phone #,"
	TRANSMIT "it does not conform to the \c2XXX-XXX-XXXX\q1 Form."
	TRANSMIT "Please notify the sysop!"
	LOGENTRY Handle"'s Phone # does not conform to specs!"
	EXIT

Syntax:
	TRANSMIT '\c1A Syntax Error has occured.  The SysOp has been notified.'
	TRANSMIT 'Line:' SIGL '  Severity:' RC'\q1'
	LOGENTRY '\c1Syntax Error occured in \c3PhoneSecurity\q1'
	LOGENTRY '\c1---> Line:\c3' SIGL '  \c1Error Code:\c3' RC'\q1'
	EXIT

Drop:
	LOGENTRY "User Droped Carrier"
	EXIT
