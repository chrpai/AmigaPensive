/*

    STR.LogDL.rexx - Most popular up/downloaded files top list maker.

    Version 1.0

*/

options results
options failat 21

call pragma('p',-1)

parse upper arg mode

address 'MASTER_REXX1'

datapath = 'Doors:STR.Utils/'  /*  Path to STR.Utilities data files  */

FBASE = 1         /*  1 = Search for files in the FBASE.  */
MBASE = 0         /*  1 = Search for files in the MBASE.  */
                  /*  0 = Ignore this area.               */

maxtopnum   = 18  /*  Maximum number of items in the list            */
keepminimum = 4   /*  Only keep files downloaded this many times or  *
                   *  more.                                          */

deletedls   = 0   /*  1 = BBS:TEXT/Logs/Downloads will be deleted    *
                   *      after each run of this program.            *
                   *  0 = The file will be concatenated to the end   *
                   *      of BBS:TEXT/Logs/Downloads.bak in reverse  *
                   *      order.  Newest first, oldest last.         */

revcallers  = 1   /*  1 = BBS:TEXT/Logs/Callers will be concatenated *
                   *      to BBS:TEXT/Logs/Callers.bak in reverse    *
                   *      order.  Newest first, oldest last.         *
                   *  0 = BBS:TEXT/Logs/Callers will not be touched. */

badareas    = 'Adult Personal'

                  /*  Text in this variable is compared against the  *
                   *  area the file was upload or downloaded from.   *
                   *  If there is a match, the file is not put on    *
                   *  the list.  This can exclude things like files  *
                   *  from Adult areas, Personal areas, or even      *
                   *  Warez areas.                                   */

badfiles    = '.rep .QWK EMail'

                  /*  Text in this variable is compared against the  *
                   *  the file that was upload or downloaded.        *
                   *  If there is a match, the file is not put on    *
                   *  the list.  This can exclude things like QWK    *
                   *  uploads and downloads.                         */

inp = 'input'
inp1 = inp'1'
out = 'output'
out1 = out'1'
out2 = out'2'

path 1; logspath = result'Logs/'
path 5; textpath = result

if mode = 'RESCAN' then call rescan()

total = 0

if exists(datapath'STR.LogDL.data') then call readdata(1)
if exists(logspath'Downloads') then call parsedata()

call readdata(keepminimum)

ulttl = 0
ulfile. = ''
ularea. = ''

if ~open(inp,logspath'Uploads','r') then do
  logentry 'Couldn''t open 'logspath'Uploads!'
end
else do
  line = readln(inp)
  do i = 1 until eof(inp)
    file = strip(substr(line,30,15))
    area = strip(substr(line,46,13))
    if file = '' | checkareas(badareas,area) > 0 | checkfiles(badfiles,file) > 0 then do
      line = readln(inp)
      i = i-1
      iterate
    end
    ulfile.i = file
    ularea.i = area
    line = readln(inp)
  end
  call close(inp)
  ulttl = i
end

if ~open(out1,textpath'STR.Downloads.ibm','w') then do
  logentry 'Couldn''t open "'textpath'STR.Downloads.ibm"!'
  exit 10
end

if ~open(out2,textpath'STR.Downloads','w') then do
  logentry 'Couldn''t open "'textpath'STR.Downloads"!'
  exit 10
end

count = 0
call writeln(out1,'\n1\c4ÉÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»')
call writeln(out2,'\n1\c4`------------------------------------------=--------------------------------''')
call writeln(out1,'\c4º    ³ \c3Top Downloads  \c4³     \c3Area     \c4³\c3Count\c4º \c3Newest Uploads \c4³     \c3Area      \c4º')
call writeln(out2,'\c4|    | \c3Top Downloads  \c4|     \c3Area     \c4|\c3Count\c4! \c3Newest Uploads \c4|     \c3Area      \c4|')
call writeln(out1,'\c4ÇÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶')
call writeln(out2,'\c4|----|----------------|--------------|-----!----------------|---------------|')
do j = total to total-maxtopnum+1 by -1
  if checkfiles(badfiles,dlfile.j) > 0 | checkareas(badareas,dlarea.j) > 0 then iterate
  count = count+1
  call writech(out1,'\c4º \c3'left(count,2)' \c4³\c7'left(dlfile.j,16)'\c4³\c7')
  call writech(out2,'\c4| \c3'left(count,2)' \c4|\c7'left(dlfile.j,16)'\c4³\c7')
  call writech(out1,left(dlarea.j,14)'\c4³\c7'right(dlcount.j,5)'\c4º\c7')
  call writech(out2,left(dlarea.j,14)'\c4|\c7'right(dlcount.j,5)'\c4!\c7')
  call writeln(out1,left(ulfile.ulttl,16)'\c4³\c7'left(ularea.ulttl,15)'\c4º')
  call writeln(out2,left(ulfile.ulttl,16)'\c4|\c7'left(ularea.ulttl,15)'\c4|')
  ulttl = ulttl-1
end
call writeln(out1,'\c4ÈÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÊÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼')
call writeln(out2,'\c4`------------------------------------------=--------------------------------''')
call writeln(out1,'  \c6STR\c7.\c6Downloads   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call writeln(out2,'  \c6STR\c7.\c6Downloads   \c5Program By\c7: \c2Black Fire \c7[\c2Call \c5Robotech BBS \c61-808-456-7745\c7]\g0')
call close(out2)
call close(out1)

if revcallers = 0 then exit

address command 'Join 'logspath'Callers 'logspath'Callers.bak TO 'logspath'Call.bak'
call delete(logspath'Callers.bak')
call rename(logspath'Call.bak',logspath'Callers.bak')
call delete(logspath'Callers')

exit


parsedata:

if ~open(inp,logspath'Downloads','r') then return

if dlfile.1 = 'DLFILE.1' then do
  dlfile. = ''
  dlarea. = ''
  dlcount. = 0
end

line = readln(inp)
do until eof(inp)
  file = strip(substr(line,30,15))
  area = strip(substr(line,46,13))
  if file = '' then do
    line = readln(inp)
    iterate
  end
  found = 0
  if total > 0 then do i = 1 to total
    if file = dlfile.i then do
      found = 1
      dlcount.i = dlcount.i+1
      if area ~= 'A-Rexx' then dlarea.i = area
      if dlcount.i >= keepminimum then say dlfile.i' - 'dlcount.i'; Total: 'total
    end
  end
  if found = 0 then do
    total = total+1
    dlfile.total = file
    dlarea.total = area
    dlcount.total = 1
  end
  line = readln(inp)
end
call close(inp)

if deletedls = 0 then do
  address command 'Join 'logspath'Downloads 'logspath'Downloads.bak TO 'logspath'Down.bak'
  call delete(logspath'Downloads.bak')
  call rename(logspath'Down.bak',logspath'Downloads.bak')
end
call delete(logspath'Downloads')

if ~open(out,datapath'STR.LogDL.data','w') then do
  logentry 'Couldn''t open 'datapath'STR.LogDL.data!'
  exit 10
end

do i = 1 to total
  call writeln(out,dlcount.i';'dlfile.i';'dlarea.i)
end
call close(out)

drop dlfile. dlarea. dlcount.

call sortdata()

return


readdata:

parse arg keepit

dlfile. = ''
dlarea. = ''
dlcount. = 0

if ~open(inp,datapath'STR.LogDL.data','r') then return

i = 0
line = readln(inp)
do until eof(inp)
  parse var line count';'file';'area
  if count >= keepit then do
    i = i+1
    dlfile.i = file
    dlarea.i = area
    dlcount.i = count
  end
  line = readln(inp)
end
call close(inp)
total = i

return


checkareas: procedure

parse upper arg badareas, area

do i = 1 to words(badareas)
  if pos(word(badareas,i),area) > 0 then return 1
end

return 0


checkfiles: procedure

parse upper arg badfiles, file

do i = 1 to words(badfiles)
  if pos(word(badfiles,i),file) > 0 then return 1
end

return 0


rescan:

/*  This part of the program reads ALL files on your system and creates *
 *  a master STR.LogDL.data file.  This is not necessary for normal     *
 *  operation, but it is nice if you have been running your BBS for a   *
 *  while and have not been keeping the Downloads file.                 *

 *  This is guaranteed to take a long time.  ;)
 *  This will probably only work with version 1.21 to 1.21e of the BBS.
 */

dldata. = ''
total = 0

say

if FBASE = 1 then call scan('FBASE')
if MBASE = 1 then call scan('MBASE')

say 'Found 'total' files.'
say

if ~open(out,datapath'STR.LogDL.data','w') then do
  say 'Couldn''t open 'datapath'STR.LogDL.data!'
  say
  exit 10
end

do i = 1 to total
  call writeln(out,dldata.i.1';'dldata.i.2';'dldata.i.3)
end
call close(out)

drop dldata.

call sortdata()

return


scan:

parse arg dir

path 0; data = result
path 9;  bbs = result

config  = data||dir'.dat'
dirpath =  bbs||dir'/'

if ~open(inp,config,'r') then do
  say 'Couldn''t open "'config'" for reading.'
  say
  exit 10
end

do until eof(inp)
  junk = readch(inp,31); dlarea = delstr(junk,pos('00'x,junk))
  junk = readch(inp,31); dlpath = delstr(junk,pos('00'x,junk))
  junk = readch(inp,270)
  if dlpath = '' then iterate
  if ~open(inp1,dirpath||dlpath'/_itemdata','r') then do
    say 'Couldn''t open "'dirpath||dlpath'/_itemdata" for reading.'
    say
    iterate
  end
  say '--> 'dlarea
  say
  found = 0
  do until eof(inp1)
    junk = readch(inp1,82); dlfile = delstr(junk,pos('00'x,junk))
    junk = readch(inp1,16)
    junk = readch(inp1,4); count = c2d(junk)
    junk = readch(inp1,68)
    if eof(inp1) then leave
    if count < keepminimum then iterate
    say left(dlfile,20)' = 'count
    total = total + 1
    dldata.total.1 = count
    dldata.total.2 = trim(left(dlfile,15))
    dldata.total.3 = trim(left(dlarea,13))
    found = 1
  end
  call close(inp1)
  if found = 1 then say
end
call close(inp)

return


sortdata:

address command 'Sort 'datapath'STR.LogDL.data 'datapath'STR.LogDL.data1 Numeric'
call delete(datapath'STR.LogDL.data')
call rename(datapath'STR.LogDL.data1',datapath'STR.LogDL.data')

return
