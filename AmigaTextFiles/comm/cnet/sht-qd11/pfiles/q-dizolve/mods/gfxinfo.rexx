/*
Tiny rexx-script for using gfxinfo with 44 char file_ids
You must have RX at a path to use it.
*/

options results
parse arg port
say port
open(i,"ram:gfxinfo"port,"R")
	do a=1 to 5
		line.a=readln(i)
	end
close(i)

open(o,"ram:file_id.diz"port,"W")
	writeln(o,left(line.2,36))
	writeln(o,left(line.4,36))
close(o)
exit
