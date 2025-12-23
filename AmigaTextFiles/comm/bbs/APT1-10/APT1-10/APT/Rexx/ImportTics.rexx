/* importtics.rexx
**
** $VER: importtics 0.0.1 (1.4.93)
**
** ARexx program for controlling ApT-BBS by ApT-Design.
**
** Copyright © 1993 ApT-Design All Rights Reserved
**
** This script will do all of the importing of your .tic files. It
** will use the actual 'diverted' to defines that should have already
** been created within the Manager. You may also create your own
** log-charts to your own design which should give you some idea of
** what files came in during the session.
**
** Optional Extras:
**
** For people that wish to do additional 'CRC' checking you will find
** that you will be able to find the CRC and 'BodyName' + 'Inbound dir'
** name which will allow you to do your own archive testing before
** deciding to allow the file to be added to your system.
**
*/

CR = 'A'x

tearline = "-----------------------------------------------------------------------------"

options results
portname = 'APTMANAGER'
address value portname

say CR CR "Scanning Tic directory...." CR

/*
** Some nice little left-margin error warning codes, better than all
** of those +, * and ! and whatever..
**/
LSTART = "|\/|"
LOK    = "|OK|"
LERR   = "|ER|"
LEND   = "|/\|"

reportfile	=	"ram:tic.report"
logfile 		=	"ram:TicReport.Log"

if open('rfh',reportfile,'W') = 0 then do
	'LOGWRITE' logfile "Failed to open '"reportfile"' for writing.."
	exit
end

/*
** START: we now start the actually running of the system, first writing
** to log that we have started our session..
**/
'LOGWRITE' logfile LSTART
'LOGWRITE' logfile LOK "Tic Session Started..."

'INITTICSCAN' 0
if(result=1) THEN DO

	DO FOREVER
		'FINDTIC'
		if(result==0) THEN BREAK

		/*
		** Get all the information that may be
		** required to actually see us through
		** our tic session..
		**/
		'GETTICINFO' 0	; area 		 = result /* original tic destination filearea 	*/
		'GETTICINFO' 1	; file 		 = result /* actual name of body-file				*/
		'GETTICINFO' 2	; desc 		 = result /* description of file..					*/
		'GETTICINFO' 3	; ticfle 	 = result /* name of .tic file							*/
		'GETTICINFO' 4	; origin		 = result /* Original Fido Origin of .tic & body	*/
		'GETTICINFO' 5	; from 		 = result /* Origin of which Fido you received it*/
		'GETTICINFO' 6	; crc  		 = result /* CRC archiver code							*/
		'GETTICINFO' 7	; createdby  = result /* Version of Tic used to create .tic	*/
		'GETTICINFO' 8	; divertedto = result /* Which dir it is being diverted to	*/
		'GETTICINFO' 9	; inbounddir = result /* Inbound directory for .tic's			*/

		/*
		** <filename>.ADS 'long file descriptions' have yet to be
		** implemented, we thus check to see if we have hit one
		** and if so we skip it, .. unless you wish to have the
		** file hit your system.. (some do..)
		**/
		if index(file,"ADS")=0 then do

			dummy = writeln('rfh',tearline)

			if(divertedto="") then dummy=writeln('rfh',">> AREA NOT DEFINED:" area)
			else dummy = writeln('rfh',"Area: "area" -> "divertedto)
			dummy = writeln('rfh',"File: "file)
			dummy = writeln('rfh',"Desc: "desc)

			if(divertedto~="") then do
				'LOGWRITE' logfile LOK "Added File '"file"' to area '"divertedto"'"
				'ADDTIC'
				if(result=0) then 'LOGWRITE' logfile LERR "Failure to add: '"file"' to '"divertedto"'"
			end
			else 'LOGWRITE' logfile LERR "File '"file"' has no destination area '"area"'.."
		end

	END

END

dummy = close('rfh')

'LOGWRITE' logfile LOK "Tic Session ended..."
'LOGWRITE' logfile LEND

say CR "Finished...." CR

EXIT
