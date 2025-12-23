/* judgementday.rexx
**
** $VER: judgementday 0.0.1 (1.3.93)
**
** ARexx program for controlling ApT-BBS by ApT-Design.
**
** Copyright © 1993 ApT-Design All Rights Reserved
**
** You should expand this file to do 'everything' concerning auto-management
** all within one call. This will save much time as only the one directory
** parse is required.
**
** You will also be able to do such things as 'Top Up/Downloader charts'
** as well as hundreds of other different tasks. Just use your imagination.
**
*/

CR = 'A'x
parse arg lineno

options results
portname = 'APTMANAGER'
address value portname

say CR CR "Scanning user directory...." CR

reportlog ="apt:logs/accounts.report"

dirname.0 = "Users/"
dirname.1 = "New/"
dirname.2 = "Review/"

/*
** Setup some variables that will be used in gathering
** statistical information.
*/
MovedVAL			= 0
DeletedVAL		= 0
ScannedVAL		= 0
ExpiredVAL		= 0
UntouchedVAL	= 0

MovedBUF			= 0
DeletedBUF		= 0
ExpiredBUF		= 0
UntouchedBUF	= 0

ActualNewVAL	= 0
ActualExpireVAL = 0

ActualNewBUF	= 0
ActualExpireBUF = 0

header1 = "Name                | Last Called | ALev | K-Up | K-Dn | Up | Dn | Rv | Call"
header2 = "--------------------+-------------+------+------+------+----+----+----+-----+"

/*
** We start off by locking into our 'apt:users/' directory. Most of
** our users are here so the majority of the work is going to be
** carried out here,.. the other directories can be done later and
** need less work in terms of logging information and scanning.
*/
'LOCKUSERDIR' 0
if(result==1) THEN DO

	DO FOREVER
		'SCANUSERDIR'
		if(result==0) THEN BREAK

		ScannedVAL=ScannedVAL+1

		'GETSTR' 0		; uname = result
		'GETSTR' 1		; handle = result
		'GETVAR' 0		; level = result
		'GETVAR' 7		; kup = result%1024
		'GETVAR' 8		; kdn = result%1024
		'GETVAR' 5		; ups = result
		'GETVAR' 6		; dns = result
		'GETVAR' 2		; calls = result
		'HASEXPIRED'	; review=result
		'GETSTATICSTR' 0	; lastcalldate=result
		'GETSTATICSTR' 1	; lastcalltime=result
		'GETSTATICVAR' 0	; daysleft=result

		str=left(uname,20,' ')"| "left(lastcalldate,12)"| "left(level,5)"| "left(kup,5)"| "left(kdn,5)"| "left(ups,3)"| "left(dns,3)"| "left(daysleft,3)"| "left(calls,3)

		/*
		** First check to see if the user has 'expired/come up for review'
		** If that is the case then we would take a different cause of
		** action for this given user.
		*/

		select
			/*
			** ACCESS LEVEL 0:
			**
			** We mend any accounts that have a level of 0 by moving
			** them to the new-user directory. If they are in the
			** users/ directory then they are here by mistake.
			*/
			when level=0 then do
				'MOVEUSER' 1
				MovedBUF.MovedVAL = str
				MovedVAL=MovedVAL +1
			end

			/*
			** REVIEW:
			**
			** Has this user come up for review,.. if he has then
			** have his account moved over to the review/ directory.
			**
			** We also check to see that the user has made at least
			** 1 call other than his first time application call.
			** If he has not done so then he obviously does not
			** like the system, ... so we cant be bothered to keep
			** his account.
			*/
			when review=1 then do
				if(calls > 0) then do
					'MOVEUSER' 2
					ExpiredBUF.ExpiredVAL = str
					ExpiredVAL=ExpiredVAL+1
				end
				else do
					'DELETEACCOUNT' uname
					DeletedBUF.DeletedVAL=str
					DeletedVAL=DeletedVAL+1
				end
			end

			otherwise do
				UntouchedBUF.UntouchedVAL=str
				UntouchedVAL=UntouchedVAL+1
			end
        end
	END

	'UNLOCKUSERDIR'

/*
** Now scan the NEW user directory and build up a list of
** all users that are still un-validated.
*/
'LOCKUSERDIR' 1
if(result==1) then do
	do forever
		'SCANUSERDIR'
		if(result==0) THEN BREAK

		call MAKELINE ; str=result

		ActualNewBUF.ActualNewVAL = str
		ActualNewVAL=ActualNewVAL + 1

		ScannedVAL=ScannedVAL+1
	end
end

/*
** Now scan the REVIEW user directory and build up a list of
** all users that are still un-validated.
*/
'LOCKUSERDIR' 2
if(result==1) then do
	do forever
		'SCANUSERDIR'
		if(result==0) THEN BREAK

		call MAKELINE ; str=result

		ActualExpireBUF.ActualExpireVAL = str
		ActualExpireVAL=ActualExpireVAL + 1

		ScannedVAL=ScannedVAL+1
	end
end

	/*
	** We now gather the information we have collected and send
	** this out to a logfile. The file will then contain information
	** on almost all of the accounts within our system and what
	** has happend to them during this session.
	*/

	options failat 21

	open('rfh',reportlog,'W')

	/*
	** The first couple of blocks reflect information only to changes
	** carried out within the 'users/' directory. This could be moving
	** and deleting of accounts to one of the other two directories.
	*/

	if(DeletedVAL>0) then do
		writeln('rfh',CR "Accounts DELETED" CR)
		writeln('rfh',Header1||CR||Header2)

		do i=0 while i < DeletedVAL
			err=writeln('rfh',DeletedBUF.i)
		end

		writeln('rfh',CR "Total Deleted: "DeletedVAL)
	end

	if(MovedVAL>0) then do
		writeln('rfh',CR "BAD-NEW Accounts Moved" CR)
		writeln('rfh',Header1||CR||Header2)
		do i=0 while i < MovedVAL
			writeln('rfh', MovedBUF.i)
		end
		writeln('rfh',CR "Total Bad-Moved: "MovedVAL)
	end

	if(ExpiredVAL>0) then do
		writeln('rfh',CR "Accounts for REVIEW" CR)
		writeln('rfh',Header1||CR||Header2)
		do i=0 while i < ExpiredVAL
			writeln('rfh',ExpiredBUF.i)
		end
		writeln('rfh',CR "Total Newly Expired: "ExpiredVAL)
	end

	/*
	** The following 3 blocks display the 'actual' contents
	** of the user directories. The previous batch would show
	** things that have 'changed' from the normal users/ directory
	** only.
	*/

	if(ActualNewVAL>0) then do
		writeln('rfh',CR "Actual New Users" CR)
		writeln('rfh',Header1||CR||Header2)
		do i=0 while i < ActualNewVAL
			writeln('rfh',ActualNewBUF.i)
		end
		writeln('rfh',CR "Total New Users: "ActualNewVAL)
	end

	if(ActualExpireVAL>0) then do
		writeln('rfh',CR "Actual Expired Accounts" CR)
		writeln('rfh',Header1||CR||Header2)
		do i=0 while i < ActualExpireVAL
			writeln('rfh',ActualExpireBUF.i)
		end
		writeln('rfh',CR "Total Expired Accounts: "ActualExpireVAL)
	end

	if(UntouchedVAL>0) then do
		writeln('rfh',CR "Untouched Accounts" CR)
		writeln('rfh',Header1||CR||Header2)
		do i=0 while i < UntouchedVAL
			writeln('rfh',UntouchedBUF.i)
		end
		writeln('rfh',CR "Total Untouched: "UntouchedVAL)
	end

	writeln('rfh',CR "Total accounts scanned: "ScannedVAL)
	writeln('rfh',CR "Total accounts in use : "UntouchedVAL+ActualNewVAL+ActualExpireVAL)

	close('rfh')

END


say CR "Finished...." CR

EXIT

/*##########################################################################*/

/*
** MAKELINE:
**
** Just saves a little bit of space,.. and a little easier to manage.
**
** Its just the 'str'ing we are interested in, so we are not bothered
** about the other variables being contained with data that can be
** used globally.
**
*/
MAKELINE: procedure
	'GETSTR' 0		; uname = result
	'GETSTR' 1		; handle = result
	'GETVAR' 0		; level = result
	'GETVAR' 7		; kup = result%1024
	'GETVAR' 8		; kdn = result%1024
	'GETVAR' 5		; ups = result
	'GETVAR' 6		; dns = result
	'GETVAR' 2		; calls = result
	'HASEXPIRED'	; review=result
	'GETSTATICSTR' 0	; lastcalldate=result
	'GETSTATICSTR' 1	; lastcalltime=result
	'GETSTATICVAR' 0	; daysleft=result
	str=left(uname,20,' ')"| "left(lastcalldate,12)"| "left(level,5)"| "left(kup,5)"| "left(kdn,5)"| "left(ups,3)"| "left(dns,3)"| "left(daysleft,3)"| "left(calls,3)
return str

