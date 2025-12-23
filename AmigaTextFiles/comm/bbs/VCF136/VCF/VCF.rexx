/*
VirusChkULs V1.36
		Program to automaticaly check uploaded files for Viruses
		By Iain Black @ 2:263/154 (Fido) & iblack@dit.ie (Internet)
		Modifications by Ian Willis:
									*/

option results

filesdir = "T:VCF_Files"             /* No trailing slash - Drives cannot be used */
fndvirus = "S:VCF/FoundVirus"     /* Warns user of Virus by opening a Window */ 	
novirus = "S:VCF/NoVirus"
logfile = "S:VCF/VCF.log"
vchckr = "Work:Utilities/misc./VirusChecker/virus_checker"
unlhacmd = "lha e -M"                /* Make sure that lha is in the search path! */
addlhacmd = "lha a"
unzipcmd = "unzip"

i_started_vc = 0               /* variable which gets set if the script starts VC */

if exists(filesdir"/") then address command "delete "filesdir"/#?"
else address command "makedir "filesdir

address command 'bbs:bin/trFL -oram:CheckFiles -cBBS:configs/ -a250 -d1 -n'

open('chkfiles','ram:CheckFiles','R')
open('filecfg','bbs:configs/file.cfg','R')

ScanArea:

	do until arean='Area'
		line=readln('chkfiles')
		if eof('chkfiles')=1 then signal end
		arean=left(line,4)
	end
found_area:
	files=readln('chkfiles')
	files=readln('chkfiles')
	if length(files)=1 then signal ScanArea
	filear=delword(line,1,3)
	fileare=length(filear)
	filearea=left(filear,fileare-1)
	do until filearea=line
		line=readln('filecfg')
	end
	path=readln('filecfg')
scanfiles:
	filename=word(files,1)
	ext=right(filename,3)
	filetype=upper(ext)
	totalfile=insert(path,filename)
        if ((~exists(totalfile)) | (filename="")) then signal scan_for_next_file
	say ""
	say "Checking "totalfile"...."
	if filetype='LHA' then call lha
	else if filetype='ZIP' then call zip
	else address command 'copy 'totalfile' to 'filesdir

        if ~show(Ports,'Virus_Checker') then do
           address command vchckr' -i -c1'
           address command 'waitforport Virus_Checker'
           i_started_vc = 1
        end
	address 'Virus_Checker'
	'checkdrive\'filesdir
	if VCHECK.0.0 = 0 then do      /* No virus in upload */
        	if open('info',logfile,'A') =0  then open('info',logfile,"W")
		writeln('info','No known Virus Contained in 'totalfile)
		if filetype='LHA' then call addlha
		end
        else do
		do i =1 to VCHECK.0.0
		VFILE=VCHECK.i.1
		VINFO=VCHECK.i.2
		end
		if open('info',logfile,'A') =0  then open('info',logfile,"W")
		writeln('info','')
		writeln('info','****** Virus Alert!!! Delete The Virus NOW Using Virus Checker Manually!! ******')
		writeln('info',VINFO' 'VFILE' in the archive ')
		writeln('info',totalfile)
		writeln('info','')
		say 'Problem with file '||totalfile||' 'VINFO' 'VFILE
		address command 'run 'fndvirus
		end
	address command 'delete all 'filesdir'/#?'
scan_for_next_file:
	files=readln('chkfiles')
	if left(files,5)="Area " then do
		line = files
		signal found_area
	end
        if (left(files,21)~="Number of file areas:") then signal scanfiles
end:
	if (i_started_vc) then 'quit'
	close('chkfiles')
        address command "delete RAM:CheckFiles"
	exit

lha:
	address command unlhacmd totalfile filesdir'/'
	if open('fileid',filesdir'/FILE_ID.DIZ','R') =1 then call file_id
	return

zip:
	address command 'copy 'totalfile' to 'filesdir
	open('zipbit','t:unzipit',"W")
	writeln('zipbit','cd 'filesdir)
	writeln('zipbit',unzipcmd filesdir'/'filename)
	close('zipbit')
	address command 'execute t:unzipit'
	address command 'delete t:unzipit'
	if open('fileid',filesdir'/FILE_ID.DIZ','R') =1 then call file_id
	return

file_id:
	open('desc',totalfile||'.desc','A')
	do until eof('fileid') =1
		idline=readln('fileid')
		writeln('desc',idline)
		end
	close('fileid')
	close('desc')
	if open('info',logfile,'A') =0  then open('info',logfile,"W")
	writeln('info','A FILE_ID.DIZ has been added to the long desc. of 'totalfile)
	return

addlha:
	address command addlhacmd totalfile novirus
        return
