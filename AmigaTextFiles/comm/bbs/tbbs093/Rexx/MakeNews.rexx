/* Adds news to BBS:Text/News.txt */

address command

if ~open('tmp', 'T:mknews.tmp', 'W') then do
	say "Couldn't create T:mknews.tmp"
	exit 0
	end

if open('hdr', 'BBS:Text/NewsHeader.txt', 'R') then do
	do while ~eof('hdr')
		hdrln = readln('hdr')
		if ~eof('hdr') then do
			pos = index(hdrln, '@DATE@')
			if pos > 0 then do
				thedate = date('E')
				thedate = substr(thedate, 1, 2)||"-"||substr(date('M'), 1, 3)||"-"||substr(thedate, 7, 2)
				hdrln = substr(hdrln, 1, pos - 1)||thedate||substr(hdrln, pos + 6, length(hdrln) - (pos + 5))
				end
			pos = index(hdrln, '@TIME@')
			if pos > 0 then do
				hdrln = delstr(hdrln, pos, 6)
				thetime = time('N')
				hdrln = insert(hdrln, thetime, pos, length(thetime))
				end
			call writeln 'tmp', hdrln
			end
		end
	call close 'hdr'
	end

call close 'tmp'

"ced T:mknews.tmp -keepio"

if ~open('tmp', 'T:mknews.tmp', 'R') then do
	say "Couldn't find T:mknews.tmp"
	exit 0
	end

"delete BBS:Text/OldNews.txt QUIET"
"rename BBS:Text/News.txt BBS:Text/OldNews.txt QUIET"

if open('news', 'BBS:Text/News.txt', 'W') then do
	numlines = 0
	do while ~eof('tmp')
		call readln 'tmp'
		numlines = numlines + 1
		end
	if numlines ~= 0 then numlines = numlines - 1
	call seek 'tmp', 0, 'B'
	call writeln 'news', date('I')
	call writeln 'news', numlines
	do ln = 1 to numlines
		call writeln 'news', readln('tmp')
		end
	if open('oldnews', 'BBS:Text/OldNews.txt', 'R') then do
		do while eof('oldnews') = 0
			call writeln 'news', readln('oldnews')
			end
		call close 'oldnews'
		end
	call close 'news'
	"delete BBS:Text/OldNews.txt QUIET"
	end

call close 'tmp'

"delete T:mknews.tmp QUIET"

exit 0
