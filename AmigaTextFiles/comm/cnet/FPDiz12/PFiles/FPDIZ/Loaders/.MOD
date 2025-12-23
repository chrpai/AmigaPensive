/**/
options results;parse arg ar1'|'ar2;call pragma('P',-5)

/* == Place your code here! "AR1" is the path/file, "AR2" is the temp file == */

/*               ProTracker Loader Module - Made for FPDiz

                                   V1.0

     If you find any bugs in this please report them to mUb@912:1000/0.0,
               Chris Brundell@2:2502/12.0 or mub@reliant.psu.edu             */

	call open(module,ar1,'R')
	mod=readch(module,950)
	call close(module)
	mod=translate(mod,'00'x,xrange('00'x,'1F'x)xrange('7F'x,'A0'x),'20'x)
	parse var mod modname'00'x 20 mi.1'00'x 49 mi.2'00'x 78 mi.3'00'x 107 mi.4'00'x 136 mi.5'00'x 165 mi.6'00'x 194 mi.7'00'x 223 mi.8'00'x 252 mi.9'00'x 281 mi.10'00'x 310 mi.11'00'x 339 mi.12'00'x 368 mi.13'00'x 397 mi.14'00'x 426 mi.15'00'x 455 mi.16'00'x 484 mi.17'00'x 513 mi.18'00'x 542 mi.19'00'x 571 mi.20'00'x 600 mi.21'00'x 629 mi.22'00'x 658 mi.23'00'x 687 mi.24'00'x 716 mi.25'00'x 745 mi.26'00'x 774 mi.27'00'x 803 mi.28'00'x 832 mi.29'00'x 861 mi.30'00'x 890 mi.31'00'x .
	samples=0
	do n=1 to 31
		if strip(mi.n)~='' then do
			samples=samples+1
			sname.samples=mi.n
			end
		end
	if trunc(samples/2)~=samples/2 then do
		samples=samples+1
		sname.samples=' '
		end
	lines=min(trunc(samples/2),7)
	descname=center(translate(translate(strip(modname)),'-',' '),42,'-')
	if substr(descname,22,1)='-' then descname=left(descname,21)'.'right(descname,20)
	if substr(descname,21,1)='-' then descname=left(descname,20)'.'right(descname,21)
	desc='.'descname'.'||'0a'x
	do n=1 to lines-1
		x=n
		y=(n+lines)
		a=left(strip(sname.x),21)
		if right(a,1)=' ' then a=left(a,20)'|'
		b=right(strip(left(strip(sname.y),21)),21)
		if left(b,1)=' ' then b='|'right(b,20)
		desc=desc'|'a||b'|'||'0a'x
		end
	x=n
	y=(n+lines)
	a=left(strip(sname.x),21,'-')
	if right(a,1)='-' then a=left(a,20)"'"
	b=right(strip(left(strip(sname.y),21)),21,'-')
	if left(b,1)='-' then b="`"right(b,20)
	desc=desc"`"a||b"'"

/* =========== Writes the "DESC" variable to the temp file. =========== */

call open(fp,ar2,'W');call writeln(fp,desc);call close(fp)
exit
