/* EXCELSIOR BBS SYSTEM VOTING DOOR
   Version 1.2a (Arexx Version)

***  LOGON FRONT DOOR  ***

   © 1994 Daniel Makovec
	Project initiated Saturday 2nd July 1994
	Completion: Sunday 17th July 1994
*/

Signal on BREAK_C						/* User can't terminate the prog with a CTRL-C */
clear

/*****************************************************************************************************/

/*                  Change these settings to personal preference                                     */

MinAccess	= 1							/* Access at which user can add topics                       */
MaxAccess	= 32
DoorPath	= 'Doors:'
TempPath	= 'RAM:t/'					/* Path where new files are written to for increase in speed */
TopicPath	= 'Doors:'					/* Path were 'hard copies' of files are kept on HD           */
UserFile	= 'vote.users'				/* Name of file where user voting records are kept           */

/*****************************************************************************************************/

options results

address command 'c:copy 'TopicPath||UserFile' 'TempPath||UserFile	/* If hardcopy file exists, copy to RAM */

TR			= Transmit
QU			= Query
GU			= getuser

UFileName	= TempPath||UserFile

Final		= 0
Rec = 'rec'
User = 'user'

call CheckUser
call CheckUnTouched
exit



CheckUser:


GU 40
ID = RESULT
GU 15
Access = RESULT
if Access >= MinAccess & Access <= MaxAccess then
	call ScanForTopics
Return
ScanForTopics:

if open('UFile', UFileName, 'R') then
	do
		NumOfUsers = Readln('UFile')								/* Get total registered booth system users */
		Final = Readln('UFile')
		do count = 1 to NumOfUsers
			rec.count = readln('UFile')								/* User ID of record */
			do Num = 1 to Final
				Rec.count.Num = readln('UFile')						/* User's vote */
			end
		end
		if ID > NumOfUsers then										/* If user has never used system before, initialize account */
			do Num = 1 to Final
				rec.ID.Num = 0

			end
		do Num = 1 to Final
			if rec.ID.Num ~= 0 then
				User.ID.Num.Seen = 'Y'
			else
				User.ID.Num.Seen = 'N'
		end
		close('UFile')
	end
else
	do
		if exists(TopicPath||UserFile) then						/* See if hardcopy file exists.*/
			do
				address command
				'run >NIL: copy 'TopicPath||UserFile' 'TempPath	/* If hardcopy file exists, copy to RAM */
				Call UserDataLoad								/* and re attempt file load */
			end
		else
			do
				Users = 0
				do Num = 1 to final	
					User.ID.Num.Seen = 'N'
				end
			end
	end
RETURN

CheckUnTouched:
UnSeen = 0
do Num = 1 to final
	if User.ID.Num.Seen = 'N' then
		UnSeen = UnSeen + 1
end
if UnSeen > 0 then
	do
		TR
		if UnSeen > 1 then
			TR '[32mYou have not voted on 'UnSeen' topics.'
		else 
			TR '[32mYou have not voted on a topic.'

		QueryY 'Do you wish to vote now? (Y/n) : [37m'
		ANS = left(RESULT,1)

		if ANS = 'Y' then
			RUNDOOR DoorPath||'ExcelPoll.rexx'
	end
exit
RETURN

BREAK_C:
/* This will allow a program to exit cleanly and safely, instead of breaking and
   leaving files open, possibly DOS interface running */

Return

FinishFile:
address command
'Run >NIL: C:Copy >NIL: 'TFileName' 'TopicPath
'Run >NIL: C:Copy >NIL: 'UFileName' 'TopicPath
Return

ScrHeader:
	TR  '              ' red'                     EXCELPOLL'
	TR  '              ' yellow'                    Version 1.2'
	TR  '              ' cyan'              By Daniel (Mav) Makovec'
	TR
Return