/***********************************************************/
/*  L10CList V3.00 by Blowfly -  49092272542               */
/*  Callers list for Pfiles or wherever you want to        */
/*  let the user see the Last 10 Callers again !           */
/***********************************************************/
options results
call open file, ("sysdata:log/l10cs"), 'R'
date = readln(file)
callsto = readln(file) ; callsto = left(callsto,3)
uf = readln(file) ; uf = left(uf,3)
uk = readln(file) ; uk = left(uk,5)
df = readln(file) ; df = left(df,3)
dk = readln(file) ; dk = left(dk,5)
callsyes = readln(file) ; callsyes = left(callsyes,3)
yuf = readln(file) ; yuf = left(yuf,3)
yuk = readln(file) ; yuk = left(yuk,5)
ydf = readln(file) ; ydf = left(ydf,3)
ydk = readln(file) ; ydk = left(ydk,5)
call close file
transmit "\f1 "
transmit "\c7 |\\_   |\\_____|\\___|\\_____    _/|\\___  |\\___|\\_____|\\_  |\\_  |\\___|\\___|\\___"
transmit " |  |  |   _  |  __|      |  /  | __ | |  __|   _  |  | |  | |  __| __ |  __| "
transmit " |  |__|  [_] | |__ _    _| / | ||  || | |  |  [_] |  |_|  |_| |_ |    | |__ "
transmit " |     |   _  |__  | |  |   \\/| ||__|| | |__|   _  |    |    |  _|| |\\ \\__  | "
transmit " |_____|__| |_|____| |__|     |_|____| |____|__| |_|____|____|____|_| \\/____| "
transmit "\c4  ~~~~~~~~~  ~~~~~~~  ~~~      ~~~~~~~  ~~~~~~~~  ~~~~~~~~~~~~~~~~~~~  ~~~~\c6BlF\q1 "
transmit "\c3Handle [\c1\r1 \q1\c3=NewUser]  \c2City/Location          \c6Spd \c3Activ: \c5Cps  \c1Ca#  \c6In\c2    Out\c3   Min\q1"
transmit "\c4===============================================================================\q1\^1"
transmit "\*0sysdata:log/L10C\\c4===============================================================================\q1"
transmit "    \q1\c1      L10C V3.00 by Blowfly - Lego Land BBS : 49-(0)-9227-2542  "
transmit "        \c4[\cbE\c4]\c6Mail Written      \c4[\cbB\c4]\c6ase-Mail Posted     \c4[\c7U\c4]\c6ploaded File"
transmit "        \c4[\cbG\c4]\c6files Read        \c4[\cbP\c4]\c6files Played        \c4[\cbD\c4]\c6ownloaded File"
transmit "\n1\z4\c6YESTERDAY:\z0\q1   \c2Calls:\c3 "||callsyes||"  \c2DL KB:\c3 "||ydk||"  \c2UL KB:\c3 "||yuk||"  \c2DL`s:\c3 "ydf||"  \c2UL`s:\c3 "||yuf
transmit "\q1    \z4\c6TODAY:\z0\q1   \c2Calls:\c3 "||callsto||"  \c2DL KB:\c3 "||dk||"  \c2UL KB:\c3 "||uk||"  \c2DL`s:\c3 "df||"  \c2UL`s:\c3 "||uf

Sendstring '\n1\c7Display the last 10 transfers \c2[\c1No\c2]\c7?';getchar;qu=upper(result)
if qu ~="Y" then exit
transmit " One moment...."
bufferflush;tr=transmit;getuser 15;acc#=result
if ~exists('sysdata:log/uloads') then do
Tr "Sorry, cant find transfer logs.."
exit
end
if ~exists('sysdata:log/dloads') then do
Tr "Sorry, cant find transfer logs.."
exit
end
call open(logu,'sysdata:log/uloads','R')
uu=1;do until EOF(logu)=1;uup.uu = readln(logu)
if uup.uu ~= '' then uu=uu+1;end;call close(logu)
call open(logd,'sysdata:log/dloads','R');dd=1
do until EOF(logd)=1;ddown.dd = readln(logd)
if ddown.dd ~= '' then dd=dd+1;end
call close(logd);d=1;u=1;do a=1 to 11
down.d=ddown.dd;dd=dd-1;d=d+1;end 
do a=1 to 11;up.u= uup.uu;uu=uu-1
u=u+1;end;d=1;u=1;do a=1 to 11
DCPS.a = right(left(down.d,53),5);d=d+1;end;do a=1 to 11
UCPS.a = right(left(up.u,53),5);u=u+1;end;u=1;d=1;do a=1 to 11
ULDER.a = right(left(up.u,39),20);u=u+1;end;do a=1 to 11
DLDER.a = right(left(down.d,39),20);d=d+1;end;x=1;y=1;do a=1 to 11
udate.a = right(left(up.x,16),13);x=x+1;end;do a=1 to 11
ddate.a = right(left(down.y,16),13);y=y+1;end;u=1;do a=1 to 11
ufile.a = right(left(up.u,70),17);u=u+1;end;d=1;do a=1 to 11
dfile.a = right(left(down.d,70),17);d=d+1;end
tr "\f1\n2\c3"
tr"         __  __ ___   ___ __         __  __             __  __  __  __"
tr"    |   |__||__  |     | |_  |\\ |   |  \\|  ||  ||\\ ||  |  ||__||  \\|__"
tr"    |__ |  | __| |     | |__ | \\|   |__/|__||/\\|| \\||__|__||  ||__/ __|"
tr"\c1  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\q1"
tr "\c6     Downloaded on:  \c7Downloaded by:      \c2 Filename:        \c1    Cps: "
tr "\c4    ================================================================\q1"
do a=2 to 11
tr  '    \c6'ddate.a'    \c7'dlder.a'\c2 'dfile.a'\c1    'dcps.a      
end
tr "\c4    ================================================================\q1"
tr "    \z4\c6Last 10 Transfers V1.0 by Blowfly - Lego Land BBS +49-09227-2542\q1"
tr "\n1\c7Key...\g0"
tr "\f1\n2\c3"
tr"             __  __ ___   ___ __             __     __  __  __  __"
tr"        |   |__||__  |     | |_  |\\ |   |  ||__||  |  ||__||  \\|__"
tr"        |__ |  | __| |     | |__ | \\|   |__||   |__|__||  ||__/ __|"
tr"\c1      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\q1"
tr "\c6     Uploaded on:    \c7Uploaded by:        \c2 Filename:       \c1     Cps: "
tr "\c4    ================================================================\q1"
do a=2 to 11
tr  '    \c6'udate.a'    \c7'ulder.a'\c2 'ufile.a'\c1    'ucps.a
end  
tr "\c4    ================================================================\q1"
tr "    \z4\c6Last 10 Transfers V1.0 by Blowfly - Lego Land BBS +49-09227-2542\q1"
tr "\n1\c7Key...\g0"
exit
