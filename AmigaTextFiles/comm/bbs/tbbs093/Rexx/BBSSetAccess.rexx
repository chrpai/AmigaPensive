/* Sets access level for user */

if arg() = 1 then do
	ln = arg(1)
	address 'TechCon'
	LineName = Con_LineActive(ln)
	if LineName = "" then do
		say 'Node '||ln||' not active'
		exit 0
		end
	address command
	'requestfile DRAWER BBS:User FILE Normal.acc >t:tmpsetacc'
	if open('fname', 't:tmpsetacc', 'READ') then do
		file = strip(readln('fname'), 'B', '"')
		close('fname')
		'delete t:tmpsetacc'
		address VALUE LineName
		if Exists(file) then do
			SetAccessPreset file
			SaveUser
			end
		else say 'File '||file||' does not exist!'
		end
	end
else say 'Usage: BBSSetAccess.rexx <node>'
exit 0
