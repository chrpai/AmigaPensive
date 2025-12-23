/* mdb.trans (c) '94 by Georg 'Gio' Magschok                                 */
/*                                                                           */
/* $VER: mdb.trans 1.0a (29.11.94)                                           */
/* an ARexx-script for TechnoBBS to access                                   */
/* MovieMUI V3.0 and later written by Andre 'ABernie' Bernhardt              */
/* Adapted to TBBS by Kenneth Fribert                                        */

options results
ln=arg(1)
LineName=Con_LineActive(ln)
If LineName = "" Then Exit 10
Address value linename
LogEntry "MovieMui started"

CR = D2C(13)
LF = D2C(10)
CRLF = CR||LF
ESC = D2C(27)
WHITE = ESC||"[0m"
RED = ESC||"[31m"
GREEN = ESC||"[32m"
YELLOW = ESC||"[33m"
BLUE = ESC||"[34m"
PURPLE = ESC||"[35m"
CYAN = ESC||"[36m"
CLEAR = D2C(12)

menuname ='bbs:text/mdbmenu'
selmenu  ='bbs:text/mdbselmenu'
mmui     ='run >nil: work:moviedatabase/MovieMUI errorfile errorlevel=0 debuglevel=0 quiet'
autoquit = 1            /* specify 1 to QUIT MovieMUI after use,0 to */
tempfile ='t:mdbtemp'||ln


if (~show('p','MOVIEMUI')) then do
	SendModem 'MovieMUI is being launched...'
	address command mmui
	address command 'waitforport MOVIEMUI'
end
SendModem CRLF

if autoquit=1 then do
	if Open('mdbuse','env:mdbusers',R) then do
		num=Readln('mdbuse')
        	Close('mdbuse')
	end
	else
		num=0
	Open('mdbuse','env:mdbusers',W)
	WriteLn('mdbuse',num+1)
	Close('mdbuse')
end

do until cmd='Q'
	if Upper(GetUserExtension(ln))='.TXT' then sendascii menuname
	cmd=Upper(AskInput(ln, "("||GetTimeLeft(ln)||") "||CYAN||"[MDB - Commands: M A S I Q ]"||WHITE, "", 255))
	select
		when cmd='Q' then SendModem CRLF
		when cmd='M' then call moviesearch
		when cmd='A' then call personsearch
		when cmd='S' then call selectivesearch
		when cmd='I' then call info
		otherwise do
			GetHotKey(ln,CRLF||RED||'Unknown command'||WHITE||CRLF||CYAN||"Press Any key to continue..."||WHITE)
		end
	end
end

stopit:
if autoquit=1 then do
	Open('mdbuse','env:mdbusers',R)
	num=Readln('mdbuse')
       	Close('mdbuse')
	Open('mdbuse','env:mdbusers',W)
	WriteLn('mdbuse',num-1)
	Close('mdbuse')
	if num=1 then do
		SendModem YELLOW||"Shutting down Movie Data Base"||WHITE||CRLF
		address 'MOVIEMUI' 'QUIT'
	end
end
LogEntry "MovieMUI ended"
exit


moviesearch:
Movie=AskInput(ln,CRLF||"("||GetTimeLeft(ln)||") "||CYAN||"What film to search for?"||WHITE, "", 255)
LogEntry "MovieMUI searching for movie="||Movie
if movie~='' then do
	SendModem CRLF||CYAN||"MovieDataBase is being searched for '"||YELLOW||movie||CYAN||"'"||WHITE||CRLF
	address 'MOVIEMUI' 'GET "'||movie||'" FROM MOVIE'
	mdbtext=result
	if mdbtext='RESULT' | RC~=0 then
		GetHotKey(ln, CRLF||RED||"The movie was not found! Try with selective search!"||CRLF||CYAN||"Press any key to continue..."||WHITE)
	else
		call showresult
	end
return

personsearch:
person=AskInput(ln,CRLF||"("||GetTimeLeft(ln)||") "||CYAN||"Person to search for (Lastname, Firstname)?"||WHITE, "", 255)
LogEntry "MovieMUI searching for person="||person
if person~='' then do
	SendModem CRLF||CYAN||"MovieDataBase is being searched for '"||YELLOW||person||CYAN||"'"||WHITE||CRLF
	address 'MOVIEMUI' 'GET "'||person||'" FROM ANY'
	mdbtext=result
	if mdbtext='RESULT' | RC~=0 then
		GetHotKey(ln, CRLF||RED||"The person was not found! Try with selective search!"||CRLF||CYAN||"Press any key to continue..."||WHITE)
	else
		call showresult
	end
return

selectivesearch:
address 'MOVIEMUI' 'UNSELECT KEY=0' ; skey=result
numentries='all'
stl=''
do until cmd='Q'
	if Upper(GetUserExtension(ln))='.TXT' then sendascii selmenu
	SendModem CRLF||YELLOW||numentries||WHITE||' Datasets are selected!'||CRLF
	cmd=Upper(AskInput(ln, "("||GetTimeLeft(ln)||") "||CYAN||" [Select - Commands: 0-9 U N G F Q ] "||WHITE, "", 255))
	einschr=0
	select
		when cmd='Q' then SendModem CRLF
		when cmd=''  then SendModem CRLF
		when cmd='0' then do
			sts='MOVIE'
			stl='MovieTitel'
			einschr=1
		end
		when cmd='1' then do
			sts='ACR'
			stl='Actor'
			einschr=1
		end
		when cmd='2' then do
			sts='ACS'
			stl='Actress'
			einschr=1
		end
		when cmd='3' then do
			sts='COMP'
			stl='Composer'
			einschr=1
		end
		when cmd='4' then do
			sts='COST'
			stl='Costume Designer'
			einschr=1
		end
		when cmd='5' then do
			sts='DIR'
			stl='Regisseur'
			einschr=1
		end
		when cmd='6' then do
			sts='ED'
			stl='Editor'
			einschr=1
		end
		when cmd='7' then do
			sts='PRDE'
			stl='Production Designer'
			einschr=1
		end
		when cmd='8' then do
			sts='PROD'
			stl='Producer'
			einschr=1
		end
		when cmd='9' then do
			sts='WRITE'
			stl='Author'
			einschr=1
		end
		when cmd='U' then do
			address 'MOVIEMUI' 'UNDOSELECT KEY='||skey
			numentries=result
		end
		when cmd='N' then do
			numentries='all'
			address 'MOVIEMUI' 'ENDSELECTION KEY='||skey
			address 'MOVIEMUI' 'UNSELECT KEY=0'
			skey=result
		end
		when cmd='G' then call getsel
		when cmd='F' then call getfromsel
	otherwise
		GetHotKey(ln,CRLF||RED||"Unknown command"||CRLF||CRLF||CYAN||"Press any key to continue..."||WHITE||CRLF)
	end
	if einschr=1 then do
		pattern=AskInput(ln,'Limiting pattern (in AmigaOS-conventions, ?, #? and *)'||CRLF||"for "||stl||":","",255)
		if pattern~='' then do
			do while pos('*',pattern)>0
				sternpos=pos('*',pattern)
				pattern=left(pattern,sternpos-1)||'#?'||right(pattern,length(pattern)-sternpos)
			end
			SendModem "MovieDataBase is searched with "||CYAN||stl||WHITE||"='"||YELLOW||pattern||WHITE||"'..."
			address 'MOVIEMUI' 'SELECT KEY='||skey||' "'||pattern||'" FROM '||sts ; numentries=result
			if RC~=0 then do
				GetHotKey(ln,CRLF||RED||'Error with last pattern, removing last selection...'||CRLF||CRLF||CYAN||"Press any key to continue..."||WHITE||CRLF)
				address 'MOVIEMUI' 'UNDOSELECT KEY='||skey
				numentries=result
			end
		end
	end
end
address 'MOVIEMUI' 'ENDSELECTION KEY='||skey
cmd=''
return

getsel:
if numentries=0 | numentries='all' | stl='' then
	GetHotKey(ln,CRLF||RED||"No Data is selected!"||CRLF||CYAN||"Press any key to continue..."||WHITE||CRLF)
else
	if ((numentries > 100) | (numentries='all')) then do
		GetHotKey(1,RED||"There's more than 100 datasets selected!"||CRLF||"Please limit the search further!"||CRLF||CYAN||"Press any key to continue..."||WHITE)
	end
	else do
		address 'MOVIEMUI' 'GETSELECTION KEY='||skey
		mdbtext=result
		if mdbtext='RESULT' | RC~=0 then
			GetHotKey(ln,CRLF||RED||"Well, nothing found, sorry!"||CRLF||CYAN||"Press any key to continue..."||WHITE||CRLF)
		else
			call showresult
	end
return

getfromsel:
if numentries=0 | numentries='all' | stl='' then
	GetHotKey(ln,CRLF||RED||"No Data is selected!"||CRLF||CYAN||"Press any key to continue..."||WHITE||CRLF)
else do
	frose=AskInput(ln,"("||GetTimeLeft(ln)||") "||CYAN||"Which "||stl||" (Number 1.."||numentries||") do you wish to view? "||WHITE, "", 255)
	if frose~='' & datatype(frose)='NUM' then do
		if frose>0 & frose<numentries+1 then do
			address 'MOVIEMUI' 'GETFROMSELECTION KEY='||skey||' NUMBER='||frose
			mdbtext=result
			if mdbtext='RESULT' | RC~=0 then
				GetHotKey(ln,CRLF||RED||"Well, nothing found, sorry!"||CRLF||CYAN||"Press any key to continue..."||WHITE||CRLF)
			else
				call showresult
		end
	end
end
return

info:
	SendModem YELLOW||"mdb.rexx V1.0a (C)'94 by Georg 'Gio' Magschok"||CRLF||"Adapted to english, cleaned up and done for TechnoBBS by Kenneth Fribert"||CRLF
	address 'MOVIEMUI' 'INFO "title"'
	mdbtext=CYAN||result
	address 'MOVIEMUI' 'INFO "author"'
	mdbtext=mdbtext||", by "||result||CRLF
	address 'MOVIEMUI' 'INFO "copyright"'
	mdbtext=mdbtext||"© "||result||CRLF
	address 'MOVIEMUI' 'INFO "description"'
	mdbtext=mdbtext||result||CRLF
	address 'MOVIEMUI' 'INFO "version"'
	mdbtext=mdbtext||result||CRLF
	address 'MOVIEMUI' 'GETSTATISTIC'
	mdbtext=mdbtext||result||CRLF
	call showresult
return

showresult:
	Open('mdbfile',tempfile,W)
	Writeln('mdbfile',mdbtext)
	Close('mdbfile')
	SendAscii tempfile
	GetHotKey(ln,CRLF||CYAN||"Press any key to continue..."||WHITE)
return
