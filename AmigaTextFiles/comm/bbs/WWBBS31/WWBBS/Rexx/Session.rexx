/*
** $VER: Session.rexx 2.0 (21.8.94)
*/

options results

dummy

newuser = 0

if local=0 then do
	delay 150
end

ansidetect

if local=0 then do
	logon
end
if local=1 then do
	logon 'Sysop'
end
resultbuff = result
if resultbuff='NEWUSER' then do
	/* New user routine */
	register
	rcbuff = rc
	if rcbuff = 1 then newuser = 1
end

dologon
rcbuff = rc
if rcbuff = 1 then do
	/* New user routine */
	if newuser = 1 then do
		edituser
		editprefs
		initmessageglobal
		initfileglobal
	end
	/* Regular session routine */
	readnews new
	shownewemail
	runmenu 'Main'
	dologoff
end
