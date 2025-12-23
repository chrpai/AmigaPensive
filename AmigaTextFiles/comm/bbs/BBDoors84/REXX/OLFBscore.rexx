/* OLFBmail.rexx by Matt English

   This file is for use with Online FootBall to process
   score information sent from the game. This file will
   ONLY work on BBBBS systems, BUT can be easily edited
   for use on any BBS system. See the OLFB.guide file
   for more info on this file, or call me at (503)761-3043

*/

PARSE ARG ONM GRES UNM SCO SCU

/* The above line recieves information from OLFB in the
   following format:

   OpponentName GameResult UserName OpponentScore UserScore */

CALL PRAGMA('P',-2)
ONM = strip(ONM)
GRES = strip(GRES)
UNM = strip(UNM)
SCO = strip(SCO)
SCU = strip(SCU)

/* This section gets the BBSpath and the Sysop name */

info= 's:CONFIG.BBS'
call open(con,info,'R')
ll = readln(con)
sysln = readln(con)
compos=POS('/*',sysln)
IF compos>0 THEN sysln = LEFT(sysln,compos-1)
sysname = upper(STRIP(sysln))
sysname = translate(sysname,'_',' ')
do 3
 line = readln(con)
 end
BBSpath = readln(con)
BBSpath = word(BBSpath,1)
BBSpath = strip(BBSpath)
call close(con)

/* You can edit the following variables to disable features you
   don't wish to use or change PATHS */

MAIL = 1                       /* If not 1 then no mail sent */
SENDSYS = 0                    /* Send mail to Sysop if set at 1 */
POSTfile = bbspath'BBS_TEXT/UNTIL.19990101' /* Path to a file that will
                                               be shown once per day.
                                               Change to "0" to avoid this
                                               file from being generated. */

SCBfile = bbspath'rexxdoors/data/olfb/OLFB.sb'   /* If this file exists
                                                    a scoreboard will be
                                                    shown at the beggining
                                                    of each OLFB game.
                                                    Change to "0" to avoid this
                                                    file from being generated. */

tempONM = LEFT(ONM,22)
tempUNM = LEFT(UNM,22)
tempSCO = RIGHT(SCO,3)
tempSCU = RIGHT(SCU,3)
thedate = date('w')' 'date()

if MAIL = 1 then call WRITEMAIL
if POSTFILE ~= 0 then call Writefile(POSTfile)
if SCBfile ~= 0 then call Writefile(SCBfile)
exit

Writefile:
 arg thefile
 if ~exists(thefile) then do
  call open(pfile,thefile,'w')
  call writeln(pfile,'                           Online FootBall ScoreBoard')
  call writeln(pfile,' ')
  call writeln(pfile,'                             'thedate)
  call writeln(pfile,'                           'tempUNM' 'tempSCU)
  call writeln(pfile,'                           'tempONM' 'tempSCO)
  call writeln(pfile,' ')
  call close(pfile)
  end
 else do
  call open(pfile,thefile,'r')
  do i = 1 to 22
   ln.i = readln(pfile)
   end
  call close(pfile)
  call open(pfile,thefile,'w')
  call writeln(pfile,'                           Online FootBall ScoreBoard')
  call writeln(pfile,' ')
  call writeln(pfile,'                              'thedate)
  call writeln(pfile,'                           'tempUNM' 'tempSCU)
  call writeln(pfile,'                           'tempONM' 'tempSCO)
  do i = 2 to 14
   call writeln(pfile,ln.i)
   end
  call close(pfile)
  end
RETURN

WRITEMAIL:
 If sendsys = 0 then do
  if sysname = ONM then RETURN
  end

/* This section checks to see if the Opponent is a REAL BBS user
   or a Imaginary OLFB player */

 call open(names,bbspath'lists/users','r')
 foundhim = 0
 do until foundhim = 1
  nameline = readln(names)
  do i = 1 to 3
   user = strip(word(nameline,i))
   if user = '' then RETURN            /* This name is not a REAL user */
   if user = ONM then foundhim = 1
   end
  end

 IF GRES = 'W' then SubjText = 'You won a Game!'
 ELSE SubjText = 'You lost at FootBall!'
 call open(last,bbspath'Numbers/LastMail','r')
 enum = readln(last) + 1
 call close(last)
 call open(last,bbspath'Numbers/LastMail','w')
 call writeln(last,enum)
 call close(last)
 thetime = time('c')
 call open(note,bbspath'email/'ONM'/Online_FootBall.'enum,'w')
 call writeln(note,'Mail: 'enum)
 call writeln(note,'From: Online_FootBall')
 call writeln(note,'  To: 'ONM)
 call writeln(note,'Subj: 'SubjText)
 call writeln(note,'Date: 'thedate'  'thetime)
 call writeln(note,'====================================================================')
 if GRES = 'W' then call writeln(note,' You beat 'UNM' at a game of Online FootBall!')
 ELSE call writeln(note,' 'UNM' beat you in a game of Online FootBall!')
 call writeln(note,' ')
 call writeln(note,'                  The Final score was:')
 call writeln(note,' ')
 call writeln(note,'               'tempONM'  'tempSCO)
 call writeln(note,'               'tempUNM'  'tempSCU)
 call writeln(note,' ')
 call writeln(note,'            Get back in there and play again!')
 call writeln(note,' ')
 call close(note)
RETURN