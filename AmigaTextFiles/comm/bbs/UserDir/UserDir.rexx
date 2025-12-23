/* $VER: UserDir.rexx 1.0 (19.12.94)
 *
 * Created by Lauri Aalto <lauri.aalto@fipnet.fi> , placed in Public Domain.
 *
 * Resets user direcory comments for TechnoBBS
 */

address command
options failat 21

call pragma d, 'bbs:user'
'list >t:userdir.tmp dirs #[0-9] lformat %n'
if open(file, 't:userdir.tmp', r) then do
	do until eof(file)
		l = readln(file)
		if l ~= '' then
			if open(data, l || '/user.dat', r) then do
				name = readln(data)
				'filenote' l '"' || name || '"'
				say right(l, 3) || ':' name
				call close data
		end
	end
	call close file
end
'delete >nil: t:userdir.tmp'
