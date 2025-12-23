/*
** Call.rexx - Simple non-async node calling script for ZMailer
*/

Options Results
Parse Arg Line Node

if Line = '' | Node = '' then do
	say 'Usage: rx Call <Line Number> <Node Number>'
	exit
	end

Host = 'ZMAILER.'Line

Address Value Host

'Call' Node

Say 'RC     = 'RC
Say 'RESULT = 'RESULT
