/* 
Mat's Mail Bomb Defuser V3.11   For CHRIST'S SAKE: ** Read doc file! ** 
By Mat Bettinson FiDO 2:254/205 E-Mail mat@darkside.demon.co.uk
Now incorporating Trap Prong TM.
2.5. URGENT RE-WRITE! Some systems rename packets depending on the BBS
software. The system worked with Xenolink but not others.
THIS ONLY APPLIES TO TRAP PRONG bogus archive detection.
3.0. LHA packet RESUME damage GREATLY expanded as different LHA settings
would defeat the simplistic (but fast) routine in 2.5. Now this will do the
job PROPERLY at the expense of speed. Those wishing to just use Mail bomb
protection can now set the TP flag to 0 as below.
3.1 LHA checking expanded for even more bizaar archives.

** Once again the 'London Xenolink Alliance' saves your butt. :-) **

If you AREN'T running Xenolink then you MUST CHANGE BBSTEMP below!
it must be what your system renames MAIL packets to before unarching or
it MUST be a null string as below.
TP = 1 means to check for RESUME DAMAGE. NEW! Instead of VERBOSE on the
command line they are LHA options and will be parsed to LHA. For instance: 
rx Defuser %s -b100
Will parse -b100 to LHA. 
*/
EXEdir = 'C:'
MaxSize = 100000
MaxArc = 5000000
Logfile = 'MAIL:Defuser.log'
BadPKT = 'XIN:BAD/'
LHAOpt = 'x -q'
BBSTemp = ''
CheckResume = 'YES'

/* If you don't know what you're doing I don't recommend pissing about with
it from here on in as it works right now. The slightest mistake will lose
you mail packets. */

Arg file specLHA
Dateline = Date('E')
Dateline = TIME('N')' 'Dateline
IF file = '' THEN EXIT 10
specLHA =  strip(specLHA)
file = strip(file)
IF EXISTS(logfile) THEN CALL OPEN(log,logfile,"A")
ELSE CALL OPEN(log,logfile,"W")
IF ~ exists(file) THEN DO
 CALL WRITELN(log,Dateline' 'file' not found.')
 CALL WRITELN(log,'---')
 CALL CLOSE(log)
 EXIT 10
 END
address COMMAND
EXEdir'LHA >T:Defuse.temp l "'file'"'
CALL OPEN(check,'T:Defuse.temp','R')
total = 0
Error = 0
CALL WRITELN(log,Dateline' Defuser session started:')
DO until EOF(check)
 line = READLN(check)
 parse VAR line dat1 dat2 dat3 dat4 dat5 dat6
 IF right(strip(dat6),5) = 'files' THEN break
 IF right(strip(dat3),1) = '%' THEN DO
   filename = strip(dat6)
   filesize = strip(dat1)
   IF Filesize > maxsize THEN CALL ErrorSize
   IF upper(right(filename,4)) ~= '.PKT' THEN CALL ErrorName
   END
 END
CALL CLOSE(check)
Tfiles = strip(word(dat6,1))
Total = strip(dat1)
ArcSz = strip(dat2)
Numfiles = word(dat6,1)
If CheckResume = 'YES' then Call CheckTPDamage
EXEdir'Delete >NIL: t:Defuse.temp'
IF total > maxarc THEN DO
 CALL WRITELN(log,' PKT *FAILED!* Archive total is 'total' which exceeds 'maxarc)
 Error = 1
 END
IF Error = 0 THEN DO
 CALL WRITELN(log,' LHA session OK. Total uncomp size: 'total)
 CALL WRITELN(log,'---')
 CALL CLOSE(log)
 EXEdir'LHA 'LHAOpt' 'SpecLHA' "'file'" #?.PKT'
 EXIT
 END
CALL WRITELN(log,'Packet rejected. CALL the BOMB SQUAD!')
CALL WRITELN(log,'Suspect moved to 'BadPKT' ... Phew!')
CALL WRITELN(log,'---')
CALL CLOSE(log)
EXEdir'Copy >NIL: "'file'" to "'BadPKT'"'
EXEdir'Delete >NIL: "'file'"'
EXIT 10
ErrorSize:
CALL WRITELN(log,' PKT *FAILED!* File in archive larger than 'maxsize)
Error = 1
RETURN
ErrorName:
CALL WRITELN(log,' PKT *FAILED!* 'filename' not a valid PKT name!')
Error = 1
RETURN
TrapdoorBlues:
CALL WRITELN(log,' PKT *FAILED!* File longer than archive!')
CALL GetNewName
CALL OPEN('new',newfile,'W')
CALL OPEN('sus',file,'R')
CALL SEEK('sus',Reallength,'B')
DO until EOF('sus')
 Buf = READCH('sus',256)
 CALL WRITECH('new',Buf)
 END
CALL CLOSE('sus')
CALL CLOSE('new')
RETURN
GetNewName:
D = d2x(date('S'))
S = d2x(time('S'))
stamp = right(d||S,8)'.'upper(left(strip(date('W')),2))
asc = 47
DO forever
 asc = asc + 1
 x = D2C(asc)
 IF ~ EXISTS(fdir||stamp||x) THEN break
 END
newfile = fdir||stamp||x
CALL WRITELN(log,'**     Trap Prong ACTIVATED!     **')
CALL WRITELN(log,' Writing 'Diff' bytes remainder to 'newfile)
CALL WRITELN(log,'** Trap Prong session completed. **')
RETURN

CheckTPDamage:
temp = translate(file,"  ",":/")
fname = strip(word(temp,words(temp)))
fdir = left(file,length(file)-Length(fname))
IF fname = BBSTemp | BBSTemp = '' THEN DO
 CALL CheckProng
 If bogus = 1 then CALL TrapDoorBlues
 END
RETURN

CheckProng:
Call open(test,file,"R")
CALL WRITELN(log,'Archive checked for Resume Damage.')
Block = 0
Count = 0
Lenf.0 = 0
OldPos = 0
Call Seek(test,0,'B')
Do until EOF(test)
 Call Seek(test,Block,'B')
 Buff = READCH(test,520)
 chk = INDEX(buff,'-lh')
 IF chk ~= 0 then DO
  subind = 1
  Do forever
   chk = INDEX(Buff,'-lh',subind)
   If chk < 8 & Block ~= 0 then break
   If chk = 0 then break
   count = count + 1
   temp = Block + chk + subind
   IF count = (Tfiles + 1) then bang = temp
   Lenf.Count = temp - oldpos
   Oldpos = temp
   Subind = chk + 1
   END
  END
 Block = Block + 512
 END
IF Count ~= Tfiles then DO
 RealLength = bang - 3
 Diff = len - RealLength
 bogus = 1
 END
ELSE bogus = 0
Call Close(test)
RETURN
