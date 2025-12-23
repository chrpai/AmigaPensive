
/*###################################################################
##                                                                 ##
##      Aminet Tosser - ARexx script for W.F.M.H. MaxManager       ##
##                                                                 ##
##                    © 1996 Marcin Orîowski                       ##
##                                                                 ##
#####################################################################
##                                                                 ##
## $VER: AminetTosser 1.0 (04.04.96)                               ##
##                                                                 ##
#####################################################################
##                                                                 ##
## For a long time, we here at SilverDream!'s BBS got the Aminet   ##
## mirror. This is demo script but we're using very simmilar one   ##
## and I decided to included it to the program distribution to     ##
## show you the potential power of usage of MaxManager Rexx port.  ##
## Its main usage is to simplify the new file tossing process      ##
## used to do manually (having no ARexx port in MaxManager... :-)  ##
## Now, using this small script we can do this automatically,      ##
## and what's more important, uncomparable faster.                 ##
##                                                                 ##
##                                                                 ##
##  Basic principles:                                              ##
##                                                                 ##
##  * Files to toss are placed in SOURCE section (due to internal  ##
##    structure of SD!'s - new files always appears in NEW FILES   ##
##    section, and then after defined period are moved to their    ##
##    destination sections.                                        ##
##                                                                 ##
##  * Script recognizes type of file basing on it's comment,       ##
##    which is expected to be the same as Aminet's SHORT: field    ##
##    of .README file preceded by TYPE: field. Example:            ##
##                                                                 ##
##    Filename          Description                                ##
##    ¯¯¯¯¯¯¯¯¯¯        ¯¯¯¯¯¯¯¯¯¯¯¯¯                              ##
##    MaxManager.lha    (comm/maxs) DirectoryOpus for MAX's BBS    ##
##                                                                 ##
##    There's no possibility to badly match the file by its name   ##
##    because of using / (slash) char, not allowed in file names.  ##
##                                                                 ##
#####################################################################
##                                                                 ##
## Last update: Thu Apr  4 23:03:02 1996                           ##
##                                                                 ##
###################################################################*/


OPTIONS RESULTS
ADDRESS MAX_MANAGER.1


/* Setting source section */

SOURCE = 14


/* Setting patterns and destination sections */
/* We can't use brackets due to its special  */
/* meaning for AmigaDOS pattern functions    */

/* Change this to fit your needs.            */

MaxPatterns = 3

Dest.1.1 = 92 ; Dest.1.2 = "((?dev/gcc#?)|(?dev/c#?))"
Dest.2.1 = 56 ; Dest.2.2 = "?pix/#?"
Dest.3.1 = 99 ; Dest.3.2 = "Girls#?"


/* Deiconify program if it sleeps */

 SHOW


/* Is there something to work on? If no, load it */

'QUERY 0'
IF RC = 0 THEN
	DO
		'OKAY2 "No database loaded.*nCan I load it now?"'
		IF RC = 1 THEN
			LOAD_BBS
		ELSE
			DO
				'OKAY1 "Nothing to do... Exiting."'
				EXIT
			END
	END


/* Ask for SysOp's confirmation */

'OKAY2 "We are about to toss some files*nShall we continue?"'
IF rc = 0 THEN EXIT


/* Disabling info requesters and storing previous state */

'INFO_REQ 0'
req_state = RC


/* Setting source section */

SECTION SOURCE LEFT
SRC_LEFT
NONE


/* We want MaxManager to match filecoments as well */

'MATCH_BASE_MODE 1'


/* Get, set, go... */

Temp = TIME(R)


/* So here the loop begins... */

Total = 0
DO Pattern = 1 FOR MaxPatterns

	MATCH Dest.Pattern.2
	Total = Total + RC
	IF RC > 0 THEN
	DO
		SECTION Dest.Pattern.1 RIGHT
		MOVE
		NONE
	END

END


/* Restoring user state of info requesters switch */

INFO_REQ req_state


/* Finito... */

IF Total > 0 THEN
	OKAY1 """Done!*n*nAre you sure you*ncan toss " || Total || " files*nfaster by your hands*n(it took me " || TIME(E) || " secs)?"""
ELSE
	OKAY1 """Zero files? Are you kidding?*nCan't you do nothing yourself?"""


/* Open save window at the end */

SAVE_BBS
