/*    AfterPost.Rexx                 Copyright 1993 © CornerStone Software
 *                                   Written By Dale E. Reed Jr.
 *                                   All Rights Reserved
 *
 *  This program simply checks to see if the person who just got a message
 *  written to them is on another line of 4D. It does this by checking for
 *  port names from the base name to the base name +"16".
 *
 */

options results

ARG port sub username

IF length(port)=0 THEN port = '4D-BBS'

basename=port

if datatype( right(basename,1)) = NUM then
	basename=left(basename, length(basename)-1)

if datatype( right(basename,1)) = NUM then
	basename=left(basename, length(basename)-1)

do numb=0 to 8
	
	portname=basename||numb

	if numb=0 then
		portname=basename

	if show(p, portname) then
		do
		address VALUE portname

		'userinfo' 'A'
		name=RESULT

		if upper(name)=upper(username) then
			do
			string='** A Message Was Just Left To You On Sub-Board' sub '**'
			'messqueue' string
			end

		end
	
	end

exit
