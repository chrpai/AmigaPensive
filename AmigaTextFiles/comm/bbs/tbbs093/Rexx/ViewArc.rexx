/* Simple archive viewing script */

options failat 10

signal on halt
signal on ioerr
signal on break_c

cmd = ""

cmd.arc = "arc v"
cmd.zoo = "zoo v"
cmd.lha = "lha v"
cmd.zip = "unzip -v"
cmd.arj = "unarj l"

ext = "ZOO LZH ARC ZIP ARJ"

parse upper arg infile

if ~open('in', infile, 'R') then do
	say "Couldn't open '"||infile||"' for input"
	exit 5
	end

buff = readch('in', 8)
call close('in')

select
	when substr(buff, 3, 3) == '-lh' then type = lha
	when left(buff, 4) == 'ZOO ' then type = zoo
	when left(buff, 2) == 'PK' then type = zip
	when left(buff, 2) == '60'x||'ea'x then type = arj
	when left(buff, 1) == '1A'x then type = arc
	otherwise do
		say "Unknown archive type"
		exit 5
		end
	end

address Command cmd.type infile
exit RC

halt:
ioerr:
break_c:
exit 10

