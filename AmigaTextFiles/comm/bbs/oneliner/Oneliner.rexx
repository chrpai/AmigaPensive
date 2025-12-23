/* OneLiner 1.0 By George Veeder */
call open(1,'pfiles:nlines','r')
nl=readln(1)
call close(1)
lc=random(1,nl,time('s'))
call open(1,'pfiles:lines','r')
do i=1 to lc
 z=readln(1)
end
call close(1)
mci='\c'
cl=random(1,7,time('s'))
mci=mci||cl
transmit '\n1\c6Oneliner 1.0'
transmit mci''z'\n1'
exit
