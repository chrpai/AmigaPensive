/* (c)1991 DFD Ltd.  16-Mar-92  -  Download Count For Skyline BBS 1.3x
*/

options results
Signal on break_c

parse arg opt
call Flags

InFile  = 'BBS:Datbase.active'
OutFile = 'TXT:2/File.List'    /* -edit- output path:filename */

Filter = "16 8 20"             /* -edit- DB Blacklist Filter  */

CLS      = '0C'x
PrevLine = '0B'x
NL       = '0A'x
x        = '1B'x
c1       = x'[41;30m'
c2       = x'[42;33m'
c3       = x'[33m'
res      = x'[0m'

Say CLS||c1||center('(c)1992 DFD Ltd.  16-Mar-92  -  Download Count
 For Skyline BBS 1.3x',77)
say c3||center(copies('~',67),77)
say center('CLI Usage:  RX DLCount.rexx <opt opt opt ...>',77)
say center('opts:   a = auto       d = descriptions   f = filter off',77)
say center(' n = username   t = timestamp      0 = db0:',77)
say res

if ~(auto) then do
  say Prevline||center(Dmes'   'Fmes'   'Nmes'   'Smes'   'Cmes,77);say
  options prompt copies(' ',20)'d, f, n, t, 0, [Q]uit, [RETURN]  > '
  Pull opt
  If index(upper(opt),'Q') >0 Then Do
    Say PrevLine'1B'x'[K'center('Program Terminated At User Request',77)
    Exit
  End
  call Flags
  say copies(PrevLine,3)
end
 else call delay(60)

say copies(PrevLine'1B'x'[K',4);say
say PrevLine||center(copies(' ',14)' =--> ',
c2' CTRL-C To Abort 'res' <--= 'res,77);say
say center(Dmes'   'Fmes'   'Nmes'   'Smes'   'Cmes,77)
say center('Reading: "'InFile'"    Writing: "'OutFile'"',77)
say '1B'x'[K'
call time('r')

If ~open('input',InFile,'R') then do 
  say center(copies(' ',12)c1' Error: Can''t Find  File "'InFile'" 'res,77)
  exit
end

call open(output,OutFile,'W')
call SetFilter
count=0

Do FOREVER 
  bp=0
  Section = C2D(ReadCH('input',1))
  If EOF('input') then Break

  if Section = '0' & ~(Count0) then bp=1
  if ~(NoFilt) then do i = 1 to Fword
    if Section = Filter.i then bp=1
  end
  if bp=1 then do
    bs=readch('input',209)
    iterate
  end

  count=(count)+1
  FileName = readch('input',31)
  User=readch('input',21) 
  bs= readch('input',6)
  UpDate = readch('input',20)
  bs= readch('input',13)
  Description = readch('input',79)
  bs = readch('input',30)
  DL = readch('input',1)
  bs = readch('input',8)

  FileName = 'DB'Section':'Left(FileName, Pos('00'X, FileName) - 1)
  Description = Left(Description, Pos('00'X, Description) - 1)
  UpDate = (left(UpDate,6) right(UpDate,2))
  if (Stamp) then UpDate='   UL: 'UpDate
   else UpDate =''
  if (Name) then User = '   By:'compress(User,'00'x)
   else User =''

  Say center(copies(' ',12)c2 FileName res'1B'x'[K',77) PrevLine

  call writeln(output,left(FileName,25)'DL''s: 'right(c2d(DL),3)UpDate User)
  if (DoDesc) then call writeln(output,' :'Description)
end

t=time('e')
min=(t)%60
sec=((t)-(min*60))%1

Say '07'x||center('Processed 'count' File Listings, In 'min 'Min. 'sec' Sec.',77)
call writeln(output,NL'File Count: 'count'    'date()'   'time('c'))
call close output
If ~Close('input') then say center('Error Encountered Closing 'InFile,77)

exit

Flags:
opt=upper(opt)
if index(opt,'A') >0 then Auto=1   ; else Auto=0
if index(opt,'D') >0 then DoDesc=1 ; else DoDesc=0
if index(opt,'F') >0 then NoFilt=1 ; else NoFilt=0
if index(opt,'N') >0 then Name=1   ; else Name=0
if index(opt,'T') >0 then Stamp=1  ; else Stamp=0
if index(opt,'0') >0 then Count0=1 ; else Count0=0
Dmes='Desc. OFF';  Cmes='DB0: OFF'; Nmes='Name OFF'
Smes='TimeStamp OFF'; Fmes='Filter ON'
if (DoDesc) then Dmes='Desc. ON'
if (Count0) then Cmes='DB0: ON'
if (Name) then Nmes='Name ON'
if (Stamp) then Smes='TimeStamp ON'
if (NoFilt) then Fmes='Filter OFF'
return

SetFilter:
Fword = words(Filter)
do i = 1 to Fword
  Filter.i = compress(word(Filter,i))
end
return

break_c:
Say center('Caught CTRL-C, Exiting...',77)
exit
