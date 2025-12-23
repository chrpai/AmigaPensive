/**********************************************************************/
/*  L10C V3.00 by Blowfly -  49092272542 - Lego Land BBS              */
/*  THIS FILE IS ONLY FOR THE LOGOFF PROCESS                          */
/**********************************************************************/

                /*******************************/
SYSOP = 0	/*  0 = Never add SYSOP        */
		/*  1 = Ask SYSOP to be added  */
		/*  2 = Always add SYSOP       */
                /*******************************/
options results
getuser 1 ; HA = result ; getuser 24 ; BA = result ; BA = BA / 10
getuser 22 ; TC = result ; getuser 23 ; PO = result
getuser 4 ; LO = result ; getuser 15 ; AC = result
getuser 1200032 ; TU = result ; tu = tu / 10
if tu > 100 then tu = left(tu,3) ; if tu > 10 then tu = left(tu,2) ; if tu < 10 then tu = left(tu,1)
COL = "\c3" ; if AC = 0 then COL = "\c1"
getuser 1208324 ; CPS = result
file='sysdata:log/port'||PO
if SYSOP = 1 & AC = 23 & PO = 0 then do 
sendstring '\q1Add yourself to the callerslist \c1[\c3NO\c1]\q1? '
getchar ; qu=upper(result)
if qu~="Y" then do
exit ; end ; end
If SYSOP = 0 & AC = 23 & PO = 0 then do
exit ; end
OFF = time() ; OFF = left(off,5)
call open file, ("Sysdata:log/L10COn"), 'R'
on = readln(file) ; call close file
do i=1 to 9 ; a.i=0 ; end i
call open(f1,file,'r')
do j=1 until eof(f1) ; c.j=readln(f1) ; end j
call close(f1) ; do i=1 to j
if index(c.i,'SIGNON')>0   then A1=i
if index(c.i,'SIGNOFF')>0  then A2=i ; end i
A3=A2+1 ; do i=A3 to j
if index(c.i,'uf')>0 then UPL=1 else upl = 0
if index(c.i,'df')>0 then DOW=1 else dow = 0
if index(c.i,'pf')>0 then PFL=1 else pfl = 0
if index(c.i,'gf')>0 then GFL=1 else gfl = 0
if index(c.i,'ms')>0 then MES=1 else mes = 0
if index(c.i,'po')>0 then POS=1 else pos = 0 ; end i
if ba = 24 & cps = 0 & dow = 0 then cps = 237
if ba = 96 & cps = 0 & dow = 0 then cps = 976
if ba = 144 & cps = 0 & dow = 0 then cps = 1823
if ba = 24 & cps = 0 & upl = 0 then cps = 237
if ba = 96 & cps = 0 & upl = 0 then cps = 976
if ba = 144 & cps = 0 & upl = 0 then cps = 1823
if CPS = 0 then CPS = "----"
call open file, ("Sysdata:log/L10C"), 'R'
do i = 1 to 10
line.i=readln(file) ; end ; call close file
MS = "\c3E\q1" ; GF = "\c3G\q1" ; PF = "\c3P\q1" ; UF = "\c7U\q1" ; DF = "\c3D\q1" ; SY = "\c3S\q1" ; PO = "\c3B\q1" ; CH = "\c3C\q1"
if POS > 0 then PO = "\c4-\q1" ; if MES > 0 then MS = "\c4-\q1" ; if GFL > 0 then GF = "\c4-\q1"
if PFL > 0 then PF = "\c4-\q1" ; if UPL > 0 then UF = "\c4-\q1" ; if DOW > 0 then DF = "\c4-\q1"
Call open file, ("Sysdata:log/L10C"), 'W'
call writeln file, "\c3"||col||left(HA,20)||"\q1\c2"||left(LO,23)||"\c6"||left(BA,4)||MS||PO||GF||PF||UF||DF"\c5 "left(CPS,5)||"\c1"||left(TC,5)||"\c6"||left(ON,6)||"\c2"||left(OFF,6)"\c3"||left(tu,3)||"\q1"
do i = 1 to 9 ; call writeln file, line.i ; end ; call close file
If SYSOP = 0 & AC = 23 & PO = 0 then do
exit ; end
getuser 1200532 ; uploads = result ; getuser 1400524 ; uploadk = result
getuser 1200534 ; downloads = result ; getuser 1400528 ; downloadk = result
uploadk = uploadk / 1000 ; if uploadk < 1 then uploadk = 0 ; if uploadk < 10 then uploadk = left(uploadk,1) ; if uploadk < 100 then uploadk = left(uploadk,2)
if uploadk < 1000 then uploadk = left(uploadk,3) ; if uploadk < 10000 then uploadk = left(uploadk,4); if uploadk > 10000 then uploadk = left(uploadk,5)
downloadk = downloadk / 1000 ; if downloadk < 1 then downloadk = 0 ; if downloadk < 10 then downloadk = left(downloadk,1) ; if downloadk < 100 then downloadk = left(downloadk,2)
if downloadk < 1000 then downloadk = left(downloadk,3) ; if downloadk < 10000 then downloadk = left(downloadk,4); if downloadk > 10000 then downloadk = left(downloadk,5)
nowdate = date(c)
if ~exists("SYSDATA:log/L10CS") then do
call open file, ("SYSDATA:log/L10CS"), 'W'
call writeln file, nowdate
call writeln file, "0" ; call writeln file, "0" ; call writeln file, "0" ; call writeln file, "0"
call writeln file, "0" ; call writeln file, "0" ; call writeln file, "0" ; call writeln file, "0" ; call writeln file, "0" ; call writeln file, "0"
call close file ; end
call open file, ("SYSDATA:log/L10CS"), 'R'
date = readln(file)
callstoday = readln(file)
uf = readln(file) ; uk = readln(file)  ; df = readln(file) ; dk = readln(file)
callsyesterday = readln(file)
yuf = readln(file) ; yuk = readln(file) ; ydf = readln(file) ; ydk = readln(file)
call close file
if nowdate ~= date then do
callsyesterday = callstoday
yuf = uf ; yuk = uk ; ydf = df ; ydk = dk
date = nowdate ; callstoday = 0
uf = 0 ; uk = 0 ; df = 0 ; dk = 0 ; end
callstoday = callstoday + 1
uf = uf + uploads ; uk = uk + uploadk
df = df + downloads ; dk = dk + downloadk
call open file, ("SYSDATA:log/L10CS"), 'W'
call writeln file, date
call writeln file, callstoday ; call writeln file, uf
call writeln file, uk ; call writeln file, df
call writeln file, dk ; call writeln file, callsyesterday 
call writeln file, yuf ; call writeln file, yuk
call writeln file, ydf ; call writeln file, ydk
call close file 
CPS = 0 ; setobject CPS
putuser 1208324
exit
