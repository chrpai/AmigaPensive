/* OneLiner Writer 1.0 By George Veeder */
options results
sendstring 'Would you like to add a random one line comment? [No]'
getchar
a=result
if upper(a)='Y' then signal yes
exit
yes:
transmit '\n2\c7Enter your One Liner Now.\c6'
receive
z=result
getuser 1
name=result
z=z||'\n1By '||name
call open(1,'pfiles:lines','a')
call writeln(1,z)
call close(1)
call open(1,'pfiles:nlines','r')
nl=readln(1)
call close(1)
nl=nl+1
call open(1,'pfiles:nlines','w')
call writeln(1,nl)
call close(1)
transmit '\n2\c5Thanks!'
exit
