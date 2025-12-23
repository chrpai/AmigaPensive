/***************************************************************************
**																																				**
** $VER: GetID.rexx 2.26 (28.5.95)																				**
** Public domain by Lauri Aalto <lauri.aalto@fipnet.fi>										**
** See documentation for details																					**
**																																				**
** tab size = 2																														**
**																																				**
***************************************************************************/

ver = '2.26'

options results
options failat 21

parse arg ln ' ' file
diz = 'FILE_ID.DIZ'
xdiz = '~(' || diz || ')'

CR = D2C(13)
LF = D2C(10)
CRLF = CR || LF
TAB = D2C(9)
ESC = D2C(27)
WHITE = ESC || '[0m'
RED = ESC || '[31m'
GREEN = ESC || '[32m'
YELLOW = ESC || '[33m'
BLUE = ESC || '[34m'
PURPLE = ESC || '[35m'
CYAN = ESC || '[36m'
CLEAR = D2C(12)

linelen = 45		/* length of description lines */
dims = ''
adder = ''

LineName = Con_LineActive(ln)
if LineName = '' then exit 10
address value LineName

SendModem CRLF || CRLF || 'GetID v' || ver 'by Lauri Aalto' || CRLF || GREEN || 'Reading configuration...' || WHITE
SetStatus 'Checking uploaded files'

if open('cfg', 'BBS:Cfg/BBS' || ln || '.Cfg', 'r') then do
	do forever
		line = readln('cfg')
		if eof('cfg') then break
		if upper(word(line, 1)) = 'MAXDESCLINES' then do
			MaxDescLines = word(line, 2)
			if ~datatype(MaxDescLines, 'num') then MaxDescLines = 20
		end
	end
	call close 'cfg'
end

if ~open('cfg', 'BBS:Doors/GetID/GetID.config', 'r') then do
	SendModem RED || '*** Can''t open configuration file' || WHITE || CRLF || CRLF
	exit 10
end

TempDir = 'T:' || ln
TabSize = 8
VirusCheck = 0
GetDiz = 1
AskLongDesc = 1
InsertDiz = 0
FailUnsupported = 1
AllowExe = 1
TouchLocal = 1
LhaDeleteList = ''
LzxDeleteList = ''
ZipDeleteList = ''
ViewTek = ''
LhaConv = ''
LzxConv = ''
ZipConv = ''
ArjConv = ''
DmsConv = ''
MaxDescLines = 20

current = 1
do forever
	line = translate(readln('cfg'), ' ', TAB)
	if eof('cfg') then break
	pos = pos(';', line)
	if pos ~= 0 then line = left(line, pos - 1)
	if strip(line) ~= '' then do
		keyword = upper(word(line, 1))
		line = strip(delword(line, 1, 1))
		select
			when keyword = 'TEMPDIR' then do
				pos = pos('%u', line)
				if pos ~= 0 then line = insert(ln, delstr(line, pos, 2), pos - 1)
				if line = '' then SendModem RED || '*** Error on line' current || ': Value after keyword' keyword 'missing' || WHITE || CRLF
				else TempDir = line
				if right(TempDir, 1) = '/' then TempDir = left(TempDir, length(TempDir) - 1)
			end
			when keyword = 'LHA' then lha = line
			when keyword = 'LZX' then lzx = line
			when keyword = 'ZIP' then zip = line
			when keyword = 'UNZIP' then unzip = line
			when keyword = 'UNARJ' then unarj = line
			when keyword = 'MAPUS' then mapus = line
			when keyword = 'DMS' then dms = line
			when keyword = 'GZIP' then gzip = line
			when keyword = 'VIEWTEK' then ViewTek = line
			when keyword = 'VIRUSCHECK' then VirusCheck = 1
			when keyword = 'NOVIRUSCHECK' then VirusCheck = 0
			when keyword = 'GETDIZ' then GetDiz = 1
			when keyword = 'NOGETDIZ' then GetDiz = 0
			when keyword = 'ASKLONGDESC' then AskLongDesc = 1
			when keyword = 'NOASKLONGDESC' then AskLongDesc = 0
			when keyword = 'INSERTDIZ' then InsertDiz = 1
			when keyword = 'NOINSERTDIZ' then InsertDiz = 0
			when keyword = 'ALLOWEXE' then AllowExe = 1
			when keyword = 'NOALLOWEXE' then AllowExe = 0
			when keyword = 'TOUCHLOCAL' then TouchLocal = 1
			when keyword = 'NOTOUCHLOCAL' then TouchLocal = 0
			when keyword = 'TABSIZE' then do
				if line = '' then SendModem RED || '*** Error on line' current || ': Value after keyword' keyword 'missing' || WHITE || CRLF
				else 
					if datatype(line, 'num') then TabSize = line
					else SendModem RED || '*** Error on line' current || ': Value after keyword' keyword 'invalid' || WHITE || CRLF
			end
			when keyword = 'MAXDESCLINES' then do
				if line = '' then SendModem RED || '*** Error on line' current || ': Value after keyword' keyword 'missing' || WHITE || CRLF
				else 
					if datatype(line, 'num') then MaxDescLines = line
					else SendModem RED || '*** Error on line' current || ': Value after keyword' keyword 'invalid' || WHITE || CRLF
			end
			when keyword = 'NOFAILUNSUPPORTED' then FailUnsupported = 0
			when keyword = 'FAILUNSUPPORTED' then FailUnsupported = 1
			when right(keyword, 10) = 'DELETELIST' then do
				a = left(keyword, 3)
				if line = '' then SendModem RED || '*** Error on line' current || ': Value after keyword' keyword 'missing' || WHITE || CRLF
				else do
					select
						when a = 'LHA' then LhaDeleteList = line
						when a = 'ZIP' then ZipDeleteList = line
						when a = 'LZX' then LzxDeleteList = line
						otherwise SendModem RED || '*** Error on line' current || ': Unknown keyword' keyword || WHITE || CRLF
					end
				end
			end
			when right(keyword, 4) = 'CONV' then do
				a = left(keyword, 3)
				if line = '' then SendModem RED || '*** Error on line' current || ': Value after keyword' a || 'CONV missing' || WHITE || CRLF
				else do
					pos = pos('%s', line)
					if pos ~= 0 then line = insert(diz, delstr(line, pos, 2), pos - 1)
					select
						when a = 'LHA' then LhaConv = line
						when a = 'LZX' then LzxConv = line
						when a = 'ZIP' then ZipConv = line
						when a = 'ARJ' then ArjConv = line
						when a = 'DMS' then DmsConv = line
						otherwise SendModem RED || '*** Error on line' current || ': Unknown keyword:' keyword || WHITE || CRLF
					end
				end
			end
			otherwise SendModem RED || '*** Error on line' current || ': Unknown keyword' keyword || WHITE || CRLF
		end
	end
	current = current + 1
end
call close 'cfg'

if maxdesclines = 0 then maxdesclines = 20

address command 'c:makedir' TempDir
if ~exists(TempDir) then do
	LogEntry "Couldn't create temporary directory '" || TempDir || "'"
	SendModem RED || '*** Couldn''t create temporary directory' || WHITE || CRLF || CRLF
	exit FailUnsupported
end

SendModem CR || ESC || '[K'

address command 'c:list >' || addpart(TempDir, 'name') file 'lformat "%f%n*n%n"'
if RC ~= 0 | ~open('tempfile', addpart(TempDir, 'name'), 'r') then exit 20
filename = readln('tempfile')
qfilename = '"' || filename || '"'
file = readln('tempfile')
call close 'tempfile'
qtemp = '"' || tempdir || '"'

if TouchLocal then do
	if ln = 0 then address command 'c:setdate' qfilename 'today 00:00:00'
end

olddir = pragma('d', TempDir)
address command 'c:delete #? "' || filename || '.desc" all'

LogEntry "Checking '" || file || "'"

if ~open('arc', filename, 'r') then do
	LogEntry "Couldn't open '" || file || "'"
	call cleanup
	exit 10
end
buff = readch('arc', 48)
call close 'arc'

SendModem GREEN || 'Testing...' WHITE || CRLF
broken = 0

select

	/* .(lha|lzh|run) ? */
	when substr(buff, 3, 3) == '-lh' | substr(buff, 45, 4) == 'SFX!' & lha ~= '' then do
		address command lha '>TECHIO:' || ln 't' qfilename
		if RC ~= 0 then broken = 1
		else do
			if LhaDeleteList ~= '' then do
				SendModem GREEN || 'Removing unwanted files...' || WHITE
				address command lha '-i' || LhaDeleteList 'd' qfilename
				SendModem CRLF
			end
			if VirusCheck then do
				SendModem GREEN || 'Checking for viruses...' || WHITE
				address command lha '-mM x' qfilename
				call check qtemp
				address command 'c:delete' xdiz 'all'
				SendModem CRLF
			end
			else if GetDiz then address command lha 'x' qfilename diz
			if GetDiz & LhaConv ~= '' & exists(diz) then address command LhaConv
			adder = lha 'a' qfilename diz
		end
	end

	/* .lzx ? */
	when left(buff, 3) == 'LZX' & lzx ~= '' then do
		address command lzx '>t:lzxtest.temporary -m -X0 t' qfilename
		DumpASCII 't:lzxtest.temporary'
		if open('temp', 't:lzxtest.temporary', 'r') then do
			do forever
				l = readln('temp')
				if eof('temp') then break
				p = pos('***', l)
				if p ~= 0 then broken = 1
			end
		end
		address command 'c:delete t:lzxtest.temporary'
		if ~broken then do
			if LzxDeleteList ~= '' then do
				SendModem GREEN || 'Removing unwanted files...' || WHITE
				address command lzx '-m -X0 d' qfilename splatdeletelist(LzxDeleteList)
				SendModem CRLF
			end
			if VirusCheck then do
				SendModem GREEN || 'Checking for viruses...' || WHITE
				address command lzx '-m -X0 x' qfilename
				call check qtemp
				address command 'c:delete' xdiz 'all'
				SendModem CRLF
			end
			else if GetDiz then address command lzx '-m -X0 x' qfilename diz
			if GetDiz & LzxConv ~= '' & exists(diz) then address command LzxConv
			adder = lzx '-m -X0 a' qfilename diz
		end
	end

	/* .zip ? */
	when left(buff, 2) == 'PK' & Zip ~= '' & UnZip ~= '' then do
		if stacker(20000, zip '>TECHIO:' || ln '-Tv' qfilename) ~= 0 then broken = 1
		else if ZipDeleteList ~= '' then do
			SendModem GREEN || 'Removing unwanted files...' || WHITE
			call stacker 20000, zip '<nil: -z' qfilename || LF || zip '-d' qfilename splatdeletelist(ZipDeleteList)
			SendModem CRLF
		end
		if VirusCheck then do
			SendModem GREEN || 'Checking for viruses...' || WHITE
			call stacker 20000, unzip qfilename
			call check qtemp
			address command 'c:delete' xdiz 'all'
			SendModem CRLF
		end
		else if GetDiz then call stacker 20000, unzip qfilename diz
		if GetDiz & ZipConv ~= '' & exists(diz) then address command ZipConv
		adder = zip qfilename diz
	end

	/* .arj ? */
	when left(buff, 2) == '60ea'x & unarj ~= '' then do
		address command unarj '>TECHIO:' || ln 'x' qfilename
		if RC ~= 0 then broken = 1
		else do
			if VirusCheck then do
				SendModem GREEN || 'Checking for viruses...' || WHITE
				call check qtemp
				SendModem CRLF
			end
			address command 'c:delete' xdiz 'all'
			if GetDiz & ArjConv ~= '' & exists(diz) then address command ArjConv
		end
	end

	/* .dms ? */
	when left(buff, 4) == 'DMS!' & mapus ~= '' then do
		address command mapus '>TECHIO:' || ln '-crkR' qfilename
		if RC ~= 0 then broken = 1
		else if GetDiz & dms ~= '' then do
			address command dms 'viewdiz' qfilename 'save' diz
			if DmsConv ~= '' & exists(diz) then address command DmsConv
		end
	end

	/* .(gz|z|Z) ? */
	when left(buff, 2) == '1f8b'x | left(buff, 2) == '1f9d'x & gzip ~= '' then do
		address command gzip '-t' qfilename
		if RC ~= 0 then broken = 1
	end

	/* .(gif|jpg|ilbm) ? */
	when left(buff, 4) == 'GIF8' | substr(buff, 7, 5) == 'JFIF' || '00'x | (left(buff, 4) == 'FORM' & substr(buff, 9, 4) == 'ILBM') & ViewTek ~= '' then do
		address command ViewTek '>dimensions.tmp' qfilename 'in'
		if RC ~= 0 then broken = 1
		else if open('dim', 'dimensions.tmp', 'r') then do
			/* discard vt header, 3 lines for 1.0x and 2 lines for newer versions */
			if index(readln('dim'), '1.0') ~= 0 then call readln 'dim'
			call readln 'dim'
			line = translate(readln('dim'), ' ', TAB)
			if index(line, '**') ~= 0 then broken = 1
			else dims = '(' || word(line, 2) || ') '
			call close 'dim'
		end
	end

	/* .mpg ? */
	when left(buff, 4) == '000001b3'x then SendModem RED || 'MPEG animation, can''t check' || WHITE || CRLF

	/* .exe ? */
	when left(buff, 4) == '000003f3'x & AllowExe then if VirusCheck then do
		SendModem GREEN || 'Checking for viruses...' || WHITE
		call check qfilename
		SendModem CRLF
	end		

	otherwise do
		SendModem CRLF || RED || file 'is not of one of the supported archive formats!' || WHITE || CRLF || CRLF
		if FailUnsupported then do
			call cleanup
			exit 1
		end
	end
end

if broken then SendModem CRLF || RED || '*** Corrupt file:' file || CRLF
else if open('dizfile', diz, 'r') then do
	if GetDiz then do
		SendModem CLEAR || GREEN || 'Found a' diz 'from' file || ':' || WHITE || CRLF || CRLF
		SendASCII addpart(TempDir, diz)
		if ~GetYesNo(ln, CRLF || 'Do you want to keep this? ', 1) then LogEntry 'Discarding' diz
		else if open('descfile', filename || '.desc', 'w') then do
			line = ''
			do until line ~= '' | eof('dizfile')
				line = myreadln('dizfile')
			end
			if line ~= '' then do
				LogEntry '>' line
				lines = 1
				if length(line) <= linelen then do
					call writeln 'descfile', line
					do until eof('dizfile') | lines > maxdesclines
						line = myreadln('dizfile')
						if line ~= '' then do
							LogEntry '>' line
							if length(line) > linelen then line = left(line, linelen)
							call writeln 'descfile', line
							lines = lines + 1
						end
					end
				end
				else do
					SendModem LF || CRLF || GREEN || 'Reformatting description...' || WHITE
					do until eof('dizfile')
						l = myreadln('dizfile')
						if l ~= '' then do
							line = line l
							LogEntry '>' l
						end
					end
					ret = ''
					c = 0
					do i = 1 to words(line)
						if lines <= maxdesclines then do
							w = word(line, i)
							wl = length(w)
							select
								when wl > linelen then do
									do until w = ''
										if wl > linelen then do
											call writeln 'descfile', strip(ret || left(w, linelen))
											ret = ''
											lines = lines + 1
											wl = wl - linelen
										end
										else do
											ret = ret || w
											wl = 0
										end
										w = right(w, wl)
									end
									c = 0
								end
								when c + wl > linelen then do
									call writeln 'descfile', strip(ret)
									lines = lines + 1
									ret = w
									c = wl
								end
								otherwise do
									ret = ret w
									c = c + wl + 1
								end
							end
						end
					end
					SendModem CRLF
				end
			end
			call close 'descfile'
		end
	end
	call close 'dizfile'
end

else if dims ~= '' | AskLongDesc then
	if open('descfile', filename || '.desc', 'w') then do
		SendModem CRLF || YELLOW || 'Enter long description for' file || CRLF || PURPLE || '(max.' MaxDescLines 'lines, enter on blank line to end)' || WHITE || CRLF
		lines = 1
		l = foo
		do until lines > MaxDescLines | l = ''	
			l = trim(AskInput(ln, GREEN || right(lines, length(MaxDescLines)) || ':' WHITE, dims, linelen))
			if l ~= '' then do
				LogEntry '>' l
				call writeln 'descfile', l
				dims = ''
				lines = lines + 1
			end
		end
		call close 'descfile'
		if InsertDiz & adder ~= '' then do
			SendModem CRLF || GREEN || 'Inserting description...'
			address command 'c:copy' filename || '.desc' diz
			address command adder
			SendModem CRLF || WHITE
		end
end

if open('descfile', filename || '.desc', 'r') then do
	if seek('descfile', 0, 'E') = 0 then do
		call close 'descfile'
		address command 'c:delete' filename || '.desc'
	end
	else call close 'descfile'
end

SendModem WHITE || CRLF
call cleanup
exit broken

/* clean up allocated resources */
cleanup:
	call pragma 'd', olddir
	address command 'c:delete' TempDir 'all'
return	

/* join path components together */
addpart:
	a = arg(1); b = arg(2)
	c = right(a, 1)
	if c = ':' | c = '/' | a = '' then return a || b
return a || '/' || b

/* readline improvement */
myreadln:
	l = trim(translate(readln(arg(1)), '  ', CR || d2c(26)))
	do forever
		p = pos(TAB, l)
		if p = 0 then break
		l = insert(left('', TabSize), delstr(l, p, 1), p - 1)
	end
return l

/* check for viruses */
check:
	address 'Virus_Checker' 'checkdrive\' || arg(1)
	if VCHECK.0.0 > 0 then do
		LogEntry '*** VIRUS INFECTION DETECTED ***'
		SendModem CRLF || RED || '*** VIRUS ALERT ***' || CRLF 'The following files have been infected:' || CRLF
		do i = 1 to VCHECK.0.0
			LogEntry VCHECK.i.1 '(' || VCHECK.i.2 || ')'
			SendModem CYAN || VCHECK.i.1 PURPLE || '(' || VCHECK.i.2 || ')' || CRLF
		end
		LogEntry '*** VIRUS INFECTION DETECTED ***'
		SendModem WHITE || CRLF
		call pragma 'd', olddir
		address command 'c:delete' TempDir 'all'
		exit 20
	end
return

/* run a command with different stack size */
stacker:
	size = arg(1); cmd = arg(2);
	if open('stackertemp', 't:getid.stacker', 'w') then do
		call writeln 'stackertemp', 'stack' size
		call writeln 'stackertemp', cmd
		call close 'stackertemp'
		address command 'c:execute t:getid.stacker'
		ret = RC
		address command 'c:delete t:getid.stacker'
	end
	else ret = 20
return ret

/* build a flat delete list from a line based one */
splatdeletelist:
	list = ''
	if open('flist', arg(1), 'r') then do
		do until eof('flist')
			list = list strip(readln('flist'))
		end
		call close 'flist'
	end
return list
