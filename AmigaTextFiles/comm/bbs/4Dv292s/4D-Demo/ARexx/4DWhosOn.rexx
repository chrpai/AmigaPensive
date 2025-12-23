/*    WhosOn4D.Rexx                  Copyright 1993 © CornerStone Software
 *                                   Written By Dale E. Reed Jr.
 *                                   All Rights Reserved
 *
 *  This program simply shows who is on the other lines of 4D.
 *  It does this by checking for port names from the base name to
 *  the base name +"16".
 *
 */

options results

ARG port stdio

IF length(port)=0 THEN
	port = '4D-BBS0'

basename=port
users=0

if datatype( right(basename,1)) = NUM then
	basename=left(basename, length(basename)-1)

if datatype( right(basename,1)) = NUM then
	basename=left(basename, length(basename)-1)


address VALUE port

call sas( ' ')
call sas( 'Node      User Name            Baud    Status' )
call sas( '--------  -------------------  ------  -------------------------------' )


if show(p, "TrapDoor") then
	do
	
	address VALUE port
	if ~show(p, "4D-BBS0") then 
		call sas( 'TrapDoor                       57600   Waiting for a call')
		
	else
		call sas( 'TrapDoor                       57600   Spawning 4D-BBS Node 0')
		
	end

do numb=0 to 9
	
	portname=basename||numb

	if show(p, portname) then
		do
		address VALUE portname

		first = ' '
		if portname = port then
			do
			first = '*'
			st=254 
			end
		else do
			'systemstatus' 0
			st=RESULT
			end
			
		'userinfo' 'A'
		name=RESULT

		'baud' '0'
		ubaud=RESULT
		if ubaud = 0 then
			ubaud='LOCAL'

		call getdo(st)
		whatdo = RESULT

		bigstr = left(portname, 9, ' ') || first || left(name, 21, ' ') || left(ubaud, 8, ' ') || whatdo

		address VALUE port
		call sas( bigstr )
		if  st ~=8 then
			users=users+1
		end
	
	end


call sas( ' ' )

if(users~=0) then
	bigstr = ' Users On-line: ' users
else
	bigstr = ' There is no users on-line'

call sas (bigstr)
call sas( ' ' )

IF stdio = '' then
	shutdown
exit


getdo:

parse arg s

select

	when s=0	then t='Just starting up'
	when s=1	then t='Posting a Message'
	when s=2	then t='Reading a Message'
	when s=3	then t='In CHAT with sysop'
	when s=4	then t='Adding BBS to list'
	when s=5	then t='Running standard Door'
	when s=6	then t='Running Editor Via Door'
	when s=7	then t='Viewing Bulletins'
	when s=8	then t='Waiting for a call'
	when s=9	then t='Full Logon'
	when s=10 then t='Shutting down the system'
	when s=11 then t='At Menu Prompt'
	when s=12 then t='Viewing Userlist'
	when s=13 then t='Searching for a user'
	when s=14 then t='Executing Dos Command'
	when s=15 then t='Running a FIFO Door'
	when s=16 then t='Logging Off of BBS'
	when s=17 then t='Running an Arexx Program'
	when s=18 then t='Reading Personal Mail'
	when s=19 then t='Uploading'
	when s=20 then t='Downloading'
	when s=21 then t='Listing Download Sections'
	when s=22 then t='Viewing File Lists'
	when s=23 then t='No Logon Yet, BUT connected'
	when s=24 then t='Registering as a New User'
	when s=25 then t='Editing User Options'
	when s=26 then t='Reading Internet Mail'
	when s=27 then t='Running an Editor'
	when s=28 then t='Listing Message Base Boards'
	when s=29 then t='Changing MBase Area'
	when s=30 then t='Changing File Transfer Area'
	when s=31 then t='Reading a Message'
	when s=32 then t='Posting a Message'
	when s=33 then t='Scanning Messages'
	when s=34 then t='Paging the Sysop'
	when s=35 then t='Editing Macros'
	when s=36 then t='Editing Masks'
	when s=37 then t='Entering a File Description'
	when s=38 then t='Entering File Name to Download'


	when s=80 then t='Multi User Chatting'

	/* this ISN'T a 4D option, and only used here */

	when s=254 then t='Spying on Other Users'

	otherwise t='Nothing...Just Sitting There!'
	end

return t


/* this routine will handle printing to either the arexx port or */
/* to this local stdio if the port name was "STDIO" */


sas:

parse arg op

IF stdio ~= '' then
	say op

else
	transmit op

return
