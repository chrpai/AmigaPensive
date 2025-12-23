/* EXCELSIOR BBS SYSTEM VOTING DOOR
   Version 1.3 (Arexx Version)
   © 1994 Daniel Makovec
	Project initiated Saturday 2nd July 1994
	Completion: Sunday 17th July 1994
*/

Signal on BREAK_C						/* User can't terminate the prog with a CTRL-C */
clear

/*****************************************************************************************************/

/*                  Change these settings to personal preference                                     */

AddAccess	= 20						/* Access at which user can add topics                       */
DelAccess	= 20						/*   "     "   "    "    "  delete topics                    */
EdtAccess	= 20                        /*   "     "   "    "    "  edit topics                      */

TempPath	= 'RAM:t/'					/* Path where new files are written to for increase in speed */
TopicPath	= 'ExcelBBS:Doors/'			/* Path were 'hard copies' of files are kept on HD           */
TopicFile	= 'vote.data'				/* Name of file where topics and vote numbers are kept       */
UserFile	= 'vote.users'				/* Name of file where user voting records are kept           */

/*****************************************************************************************************/


TR			= Transmit
RC 			= Receive
QU			= Query
GU			= getuser
BI			= BBSIDENTIFY

TFileName	= TempPath||TopicFile		/* Conjunction for full file path */

UFileName	= TempPath||UserFile
Final		= 0
NumOfUsers	= 0
header 		= 'header'					/* Address references for data structure */
choice  	= 'choice'
line 		= 'line'
Seen		= 'seen'
Rec			= 'rec'
call LoadFile

call CheckUser

call MainMenu
if CheckSaveFinal ~= Final then
	call SaveFile
if Alter = 1 then
	call FinishFile	
exit



CheckUser:

options results
GU 40
ID = RESULT
GU 15
Access = RESULT
GU 28
TermType = RESULT
GU 3
RN = RESULT
call TermSet


If Access > AddAccess then
	AddString = green'('white'A'green')dd, '
else
	AddString = ''
If Access > DelAccess then
	DelString = green'('white'D'green')elete, '
else
	DelString = ''
If Access > EdtAccess then
	EdtString = green'('white'E'green')dit, '
else
	EdtString = ''

call UserDataLoad
return

UserDataLoad:
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
			do
				NumOfUsers = ID
				do Num = 1 to Final
					rec.ID.Num = 0
				end
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


TermSet:
if TermType = 1 then
	do
		bblack		= ''
		bred		= ''
		bgreen		= ''
		byellow		= ''
		bblue		= ''
		bmag		= ''
		bcyan		= ''
		bwhite		= ''
		black		= ''
		red			= ''
		green		= ''
		yellow		= ''
		blue		= ''
		mag			= ''
		cyan		= ''
		white		= ''
	end

else if TermType >1 & TermType < 5 then
	do
		bblack		= '\Z0'
		bred		= '\Z1'
		bgreen		= '\Z2'
		byellow		= '\Z3'
		bblue		= '\Z4'
		bmag		= '\Z5'
		bcyan		= '\Z6'
		bwhite		= '\Z7'
		black		= '\C0'
		red			= '\C1'
		green		= '\C2'
		yellow		= '\C3'
		blue		= '\C4'
		mag			= '\C5'
		cyan		= '\C6'
		white		= '\C7'

	end
return


MainMenu:
options results

Num 	= 0

do until MainChoice = 'Q'
	Num = 0 
	CLS
	call ScrHeader
	TR  white'                                    MAIN MENU'
	TR
	TR  yellow'Voting topics ( 'mag'*'yellow' indicates you have not voted) :'
	TR
	If Final ~= 0 then
		do until Num = Final
			Num = Num + 1
			If User.ID.Num.Seen = 'Y' then
				Done = ' '
			else
				do
					User.ID.Num.Seen = 'N'
					Done = '*'
				end
			TR mag'          'Done' 'white||Num||green') 'yellow Topic.Num.header	
		end
	else
		TR cyan'No topics to vote on'white
	TR


	TR white'#'green'=Topic number, 'AddString||DelString||EdtString'('red'Q'green')uit'
	options results
	QU cyan'Enter your choice: 'white
	MainChoice = RESULT
	MainChoice = UPPER(MainChoice)
	TestANS = MainChoice
		call CheckDrop
	select
		when MainChoice = 'A' then
			if Access > AddAccess | RN = 'Daniel Makovec' then				/* <grin>  After all no point in writing a program if _I_ can't test it for bugs, is there? :) */
				call NewTopics
		when MainChoice = 'E' then
			if Access > EdtAccess | RN = 'Daniel Makovec' then
				call EditTopic
		when MainChoice = 'D' then
			if Access > DelAccess | RN = 'Daniel Makovec' then
				call DeleteTopic
		when MainChoice > 0 & MainChoice <= Final then
			call Vote
		otherwise
			a = a
	end
end
Return

Vote:
if User.ID.MainChoice.Seen = 'N' then
	do
		CLS
		call ScrHeader
		TR yellow'Topic: 'mag||topic.MainChoice.header
		TR white
		L	= 1

		do until topic.MainChoice.line.L = '.'
			TR topic.MainChoice.line.L
			L = L + 1
		end

		TR
		TR cyan'Possible answers:'
		C = 1

		do until topic.MainChoice.choice.C = '.'
			TR white'  'C||green') 'yellow||topic.MainChoice.choice.C
			C = C + 1
		end

		TR
		options results
		QU cyan'Your choice ('white'#'yellow'=Choice, 'white'RETURN'yellow'=See frequency table'cyan'): 'white
		Number = RESULT
		TestANS = Number
		call CheckDrop
		if Number ~= '' then
			if Number > 0 & Number < C then
				do
					User.ID.MainChoice.Seen = 'Y'
					Rec.ID.MainChoice = Number
					topic.MainChoice.choice.Number.votes = topic.MainChoice.choice.Number.votes + 1
					call SaveFile
				end

	end

CLS
TR mag||topic.MainChoice.header||white' voting frequency table'
TR '';
TotalVotes = 0
C = 1
TR mag 'Possible Choices:'
do until topic.MainChoice.choice.C = '.'
	TotalVotes = TotalVotes + topic.MainChoice.choice.C.votes
	TR white||C||green') 'yellow||topic.MainChoice.choice.C
	C = C + 1
end
	
If TotalVotes = 0 then
	do
		TR
		TR red'No votes have been made.  'green'Try voting yourself! :)'
	end
else
	do
		TR
		TR cyan'   Choice   Votes    Relative Opinion'
		TR yellow'_____________________________________________________________________________'
		TR
		C = 1
		do until topic.MainChoice.choice.C = '.'
			if Rec.ID.MainChoice = C then
				Done = bblack||mag'*'
			else
				Done = bblack||' '
			Relative = topic.mainchoice.choice.c.votes/TotalVotes
			Perc = Relative * 100
			Graph = trunc(Relative * 48)
			TR Done||bblack||white'  'right(c,3) '      'right(topic.mainchoice.choice.c.votes,3)'     'bblue||white||right(trunc(perc,1),5)'%'blue' 'left('', graph, '#')
			C = C + 1
		end
		TR bblack||yellow'_____________________________________________________________________________'
		TR
		TR mag'* 'white'indicates the number you voted.'

	end
TR white
CONTINUE
TestANS = RESULT
call CheckDrop
Return

DeleteTopic:

Num = 'xx'
if Final < 1 then
	do
		TR
		TR cyan'No topics to delete!  Try creating a new one first.'white
		CONTINUE
		TestANS = RESULT
		call CheckDrop
	end
	
else
	do while Num ~= ''
		CLS
		call ScrHeader
		TR cyan'                                  POLL DELETER'
		TR
		TR yellow'Available topics:'
		TR

		Num = 0
		if Final > 0 then
			do until Num >= Final
				Num = Num + 1
				TR white'         'Num||green') 'yellow||Topic.Num.header	
			end
		else
			TR red'No more topics'

		TR
		QU yellow'Which topic do you wish to delete? ('white'RETURN'yellow' to exit to voting screen): 'white
		Num = RESULT
		TestANS = Num
		call CheckDrop
	    if Num ~== '' then
			if Num > 0 & Num < Final + 1 then 
				do
					do Num = Num to Final - 1
						NextOne = Num + 1
						topic.Num.header = topic.NextOne.header
						L	= 0
					
						do until topic.NextOne.line.L = '.'
							L = L + 1
							topic.Num.line.L = topic.NextOne.line.L
						end

						C	= 0
						do until topic.NextOne.choice.C = '.'
							C = C + 1
							topic.Num.choice.C = topic.NextOne.choice.C
							topic.Num.choice.C.votes = topic.NextOne.choice.C.votes
						end
						
						User.ID.Num.seen = User.ID.NextOne.seen
						
						do UID = 1 to NumOfUsers
							Rec.UID.Num = Rec.UID.NextOne
						end

					end

					Final = Final - 1
				end

	end
if Final ~= CheckSaveFinal then
	call SaveFile
Return	

EditTopic:

/* Main editor subprogram.  Lets you select a poll to edit, then calls 
   subprograms according to what you want to do. */

Num = 'xx'
if Final < 1 then
	do
		TR
		TR cyan'No topics to edit!  Try creating a new one first.'white
		CONTINUE
		TestANS = RESULT
		call CheckDrop
	end
else
	do while Num ~= ''
		CLS
		call ScrHeader
		TR cyan'                                   POLL EDITOR'
		TR
		TR green'Available topics:'
		TR

		Num = 0
		do until Num = Final
			Num = Num + 1
			TR white'         'Num||green') 'yellow||Topic.Num.header	
		end

		TR
		QU yellow'Which topic do you wish to edit? ('white'RETURN'yellow' to exit to voting screen): 'white
		Num = RESULT
		TestANS = Num
		call CheckDrop
	    if Num ~== '' then
			do
				option = 'x'
				if Num > 0 & Num < Final + 1 then 

/* Clear screen and display characteristics of particular vote node */

					do while option ~= '' 
						CLS
						TR green'Number: 'white||Num
						TR green' Topic:'white||Topic.Num.header
						TR
						TR green'Text:'white

	   	 				L 	= 1
						do until Topic.Num.line.L = '.'
							TR Topic.Num.line.L
							L = L + 1
						end

						TR
						TR green'Voting choices: ' 
						C	= 1
						do until Topic.num.choice.C = '.'
							TR '                'yellow||C||green') 'white||Topic.num.choice.C
							C = C + 1
						end

						TR
						QUERY mag'Options: ('white'T'mag')opic, te('white'X'mag')t, ('white'C'mag')hoices, 'white'RETURN'mag'=exit: 'white
						Option = UPPER(RESULT)
						TestANS = Option
						call CheckDrop
						select
							when option = 'T' then
								call NewTopic
							
							when option = 'C' then
								do
									MainVarPrompt = mag||'Change which choice'
									EditType = 'choice'
									call EditVariables
								end

							when option = 'X' then
								do
									MainVarPrompt = mag||'Edit which line'
									EditType = 'line'
									call EditVariables
								end

							otherwise
								a=a
						end

					end
		call SaveFile

	end
end

Return


NewTopics: 

/* This creates a new subject for polling.  Topic headers, lines and voting
   options are all handled by this subprogram */


Num = Final
ok = 'N'

do while ok = 'N'
    
	L = 0

	CLS
	call ScrHeader
	TR cyan'                                  TOPIC CREATOR'
	TR
	QU yellow'Enter new topic: 'white
	HeadThing = RESULT
	TestANS = HeadThing
	call CheckDrop
	if HeadThing ~= '' then
		do
			Num = Num + 1
			topic.Num.header = HeadThing						/*Enter text to be displayed for topic lists */ 
			TR

			TR green'Enter topic text (Just type ''.'' on a blank line to end.' 
			TR green'Typing ''.'' on the first line will have leave a blank topic text):'
			do until topic.Num.line.L = '.'								/* Enter topic information text */
				L = L + 1
				QU cyan'> 'white
				topic.Num.line.L = RESULT
				TestANS = topic.Num.line.L
				call CheckDrop
			end
			if L < 2 then 
				do
					TR cyan'No text will be included.'white
					topic.Num.line.1 = '   '
					topic.Num.line.2 = .
				end
			L = 0															/* Reconfirm entries */

			QUERYN cyan'Is this OK? (y/N) : 'white
			ok = left(RESULT,1)
			TestANS = ok
			call CheckDrop
			if ok = 'N' then
				Num = Num - 1
		end
	else
			Ok = 'Y'

end

if HeadThing ~= '' then
	do
		Ok = 'N'
		do while Ok = 'N'
			TR yellow'Enter user voting choices (''.'' to exit):'
			C = 0												/* Enter Voting options */

			do until topic.Num.choice.C = '.'
				C = C + 1
				QU white||C'. 'cyan'> 'white
				topic.Num.choice.C = RESULT
				TestANS = topic.Num.choice.C
				call CheckDrop
				topic.Num.choice.C.votes = 0
			end
			if C < 2 then
				TR red'Must input at least 1 choice!!!'white
			else		
				do
					QUERYN cyan'Are these OK? (y/N) : 'white
					Ok = left(RESULT,1)							/* Reconfirm options */
					TestANS = Ok
					call CheckDrop
				end
		end
	end
topic.Num.choice.options = C - 1
Final = Num
if Final ~= CheckSaveFinal then
	call SaveFile
Return

SaveFile:														/* Save all info */
if Open('file', TFileName, 'W') then
	do
		TR
		TR yellow'Saving New Data...'
		Num = 0
		L 	= 0
		C	= 0
		writeln('file', Final)										/* Number of topics */

		if Final ~= 0 then
			do until Num = Final
				Num = Num + 1
				writeln('file', topic.num.header)							/* Topic headers */
				L = 0
				do until topic.num.line.L = '.'
					L = L + 1
					writeln('file', topic.num.line.L)                         /* Topic text */
				end
				C = 0
				do until topic.num.choice.C = '.'
					C = C + 1
					writeln('file', topic.num.choice.C)                 /* Topic choices */            
					writeln('file', topic.num.choice.C.votes)
				end
			end
	
			if close('file') then
				Alter = 1
		CheckSaveFinal = Final											/* Prevents unnecessary saves */
	end
else
	do
		TR red'System IO Access Error 0'
		TR 'Unable to open Ram data file'
		TR 'Please alert SysOp immediately.'white
		exit
	end


if open('UFile', UFileName, 'W') then
	do
		if ID > NumOfUsers then
			NumOfUsers = ID
		
		Writeln('UFile',NumOfUsers)								/* Get total registered booth system users */
		Writeln('UFile',Final)
		do count = 1 to NumOfUsers
			Rec.Count = count
			writeln('UFile',Rec.Count)							/* User ID of record */

			do Num = 1 to Final
				if Rec.Count.Num = 'REC.'||Count||'.'||Num then
					Rec.Count.Num = 0

				writeln('UFile', Rec.Count.Num)					/* User's vote */
			end

/*			writeln('UFile', '.')*/
		end
		close('UFile')
	end
else
	do
		TR red'System IO Access Error 1'
		TR 'Unable to open User data file'
		TR 'Please alert SysOp immediately.'white
		exit
	end



return

LoadFile:														/* Load all info */
if Open('file', TFileName, 'R') then							/* File exists! */
	do
		Num = 0
		L = 0
		C = 0

		Final = readln('file')										/* Number of topics */
		if Final ~= 0 then
			do until Num = Final
				Num = Num + 1
				topic.Num.Header = readln('file')							/* Topic headers */
				L = 0		
				do until topic.num.line.L = '.'
					L = L + 1
					topic.num.line.L = readln('file')							/* Topic text */
				end
				C = 0
				do until topic.num.choice.C = '.'
					C = C + 1
					topic.num.choice.C = readln('file')					/* Topic choices */   
					topic.num.choice.C.votes = readln('file')
				end  
			end

			if close('file') then
				a=a
	end
else														/* If RAM file not already installed, */
	if exists(TopicPath||TopicFile) then					/* See if hardcopy file exists.*/
		do
			address command
			'run >NIL: copy 'TopicPath||TopicFile' 'TempPath/* If hardcopy file exists, copy to RAM */
			Call LoadFile									/* and re attempt file load */
		end
	else
		Final = 0											/* Otherwise start from scratch */
CheckSaveFinal = Final
return

NewTopic:

/* Called from editor.  Changes topic of poll */

ok = 'N'
do while ok = 'N'
	QU green'Enter a NEW TOPIC for this item (RETURN=exit): 'white
	NewHeader = RESULT
	TestANS = NewHeader
	call CheckDrop
	if Newheader ~= '' then
		do
			QUERYN cyan'Is this OK? (y/N): 'white
			ok = left(RESULT,1)
			TestANS = ok
			call CheckDrop
			if ok = 'Y' then
				topic.num.header = Newheader
		end
	else
		ok = 'Y'
end
	
Return				

EditVariables:						

/* This subprogram is called from within the main vote editor.  It is used to 
   edit program text lines and voting options.  This was originally devided
   into two seperate subprograms for each of the abovementioned arrays, but
   after lots of fiddling I managed to make 'one do all' :) */

selection = 'xx'
do while selection ~= ''
	CLS
	if EditType = 'Choice' then
		do
			TR red'NOTE: To avoid confusion to users, any actions undertaken here will erase'
			TR '      all voting data records for this topic.'
		end
	TR yellow'Current 'EditType's:'
	V = 1
	do until topic.num.EditType.V = '.'
		TR white'                 'V||green') 'yellow||topic.num.EditType.V
		V = V + 1
	end
	TR
	QU ''MainVarPrompt'? 'white'#'yellow'='EditType' number, ('white'D'yellow')elete, ('white'A'yellow')dd, 'white'RETURN'yellow'=Exit: 'white 
	selection = Upper(RESULT)
	TestANS = selection
	call CheckDrop
	select

/* Delete choice or line, determined by EditType, which = line or = choice */

		when selection = 'D' then
			do

				QU red'Delete which 'EditType'? ('white'RETURN'yellow'=exit'red'): '
				DelV = RESULT											/*choice/line to delete */
				TestANS = DelV
				call CheckDrop
				if DelV ~= '' then
					if DelV ~< 0 & DelV ~> (V - 1) then					/* Valid range ok */
						do
							V = DelV - 1
							do until topic.num.EditType.V = '.'
								V = V + 1
								NextOne = V + 1
								topic.num.EditType.V = topic.num.EditType.NextOne
							end
						end
				if EditType = 'choice' then
					do UID = 1 to TotalUsers
						rec.UID.num = 0
					end
			end

/* Add a new choice or line */

		when selection = 'A' then
			do
				ok = 'N'
				do while ok = 'N'
					TR
					TR yellow'Enter new 'EditType': 'white
					QU V||green') 'white
					TempVariable = RESULT
					TestANS = TempVariable
					call CheckDrop
					if TempVariable ~= '' then
						do
							topic.num.EditType.V = TempVariable
							dotbit = V + 1
							topic.num.EditType.dotbit = '.'
							QUERYN cyan'Is this OK? (y/N): 'white
							ok = left(RESULT,1)
							TestANS = ok
							call CheckDrop
						end
					else
						ok = 'Y'
				end
				if EditType = 'choice' then
					do UID = 1 to TotalUsers
						rec.UID.num = 0
					end

			end
							
/* Change a variable */

		when selection < V & selection > 0 then
				do
					ok = 'N'
					do while ok = N
						CLS
						TR yellow'Current 'EditType':'cyan
						TR topic.num.EditType.selection
						TR
						TR green'Enter replacement: 'white
						QU selection||green') 'white
						TempVariable = RESULT
						TestANS = TempVariable
						call CheckDrop
						if TempVariable ~= '' then
							do
								QUERYN cyan'Is this OK? (y/N): 'white
								ok = left(RESULT,1)
								TestANS = ok
								call CheckDrop
								if ok = 'Y' then
									topic.num.EditType.selection = TempVariable
							end
						else
							ok = 'Y'
					end
					
					if EditType = 'choice' then
					do UID = 1 to TotalUsers
						rec.UID.num = 0
					end


				end
		otherwise
			a=a          /* Allows screen to re update if user entered non-valid choice */
	end
end
Return

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
	TR  '              ' yellow'                    Version 1.3'
	TR  '              ' cyan'              By Daniel (Mav) Makovec'
	TR
Return

CheckDrop:
If upper(TestANS)= '###PANIC' then
	exit
Return