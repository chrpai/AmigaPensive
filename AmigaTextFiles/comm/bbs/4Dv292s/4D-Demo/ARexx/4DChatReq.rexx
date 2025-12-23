/*    ChatReq.Rexx                  Copyright 1993 © CornerStone Software
 *                                   Written By Dale E. Reed Jr.
 *                                   All Rights Reserved
 *
 *  This program will log the date/time and reason why someone requested
 *  chat into a logfile called "Text:Logs/ChatReq.Log"
 *
 */

options results

logfile='Text:Logs/ChatReq.Log'


ARG port username chatreason

if open(log, logfile, 'Append') = 1 then
	do
	bigline=date() || ' ' || username || chatreason 
	writeln(log, bigline)
	close(log)
	end

