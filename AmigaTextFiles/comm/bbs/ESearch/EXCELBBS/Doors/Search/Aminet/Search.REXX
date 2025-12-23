/*search door for excelsior BBS software                 */
/* written by Paul Compton pwcomp@elevate.demon.co.uk or */
/*Paul Compton 2:251/18.0@fidonet                        */


  bufferflush
  options results
  tr=transmit
  gu=getuser
  SS=Sendstring
  estring = 'No more matches'
  saa='#?'
  bbsname='\vr'
  PATHF = 'bbsfiles:'          /* Path to main file base            */
  PATHCD1 = 'aminet19:aminet/' /* path to cd 1                      */
  PATHM = 'doors:search/'      /* path to where it all is           */
  PATHX = 'doors:search/'      /* path to the find executable       */
  FORBID = ''                  /* Area you don't want search to look*/
  call CARRIER
  le = 'User used search door'
  ld = 'User downloaded a file with search tool'
main:

  sresult1 = ''
  sresult2 = ''
  sresult3 = ''
  sresult4 = ''
  sresult5 = ''
  cdresult1 = ''
  cdresult2 = ''
  cdresult3 = ''
  cdresult4 = ''
  cdresult5 = ''

logentry le

bufferflush
Sendfile Pathm'searchmenu'
TR ' \n1\s1\c3\z4Search which area 1-3? \z0'
TR '\^1\q1\@4'
  GetChar
   a=(result)
   if upper(result) = '###PANIC' then call Carrier

    Select
     when a = '1' then call FB
     when a = '2' then call CD
     when a = '3' then call BOTH

     otherwise;
    TR '\^1\c2\s1Returning you to 'bbsname
    TR '\q1'
    bufferflush
  end
exit

FB:

TR '\^1Please enter file name, part words can be used, eg. arg for argument'

receive
sn=result

if sn='###PANIC' then call CARRIER
if sn='' then call finish

TR '\^1Searching for 'sn' in filebase please wait...                         '

snc = '#?'sn'#?'

address command ''pathx'Find 'snc' 'pathf' depth 2 quiet files > 'pathm'searchresult'


      if result='###PANIC' then call CARRIER
call display

Exit

CD:
TR '\^1Please enter file name, part words can be used, eg. arg for argument'

receive
sn=result
    if sn='###PANIC' then call CARRIER
    if sn='' then call finish
TR '\^1Searching for 'sn' in 'pathcd1'please wait...                        '
snc = '#?'sn'#?'

address command ''pathx'Find 'snc' 'pathcd1' quiet files > 'pathm'searchresult'

      if result='###PANIC' then call CARRIER
call display



BOTH:

TR '\^1Please enter file name, part words can be used, eg. arg for argument'

receive
sn=result
    if sn='###PANIC' then call CARRIER
    if sn='' then call finish

TR '\^1Searching for 'sn' in 'pathcd1 'please wait...                '

snc = '#?'sn'#?'

address command ''pathx'Find 'snc' 'pathcd1' quiet files > 'pathm'searchresultcd1'


TR '\^1Searching for 'sn' in filebase please wait...              '

snc = '#?'sn'#?'

address command ''pathx'Find 'snc' 'pathf' depth 2 quiet files > 'pathm'searchresult'

      if result='###PANIC' then call CARRIER



OPEN('cdres1', ''pathm'searchresultcd1', 'R')

 cdresult1 = READLN('cdres1')
 cdresult2 = READLN('cdres1')
 cdresult3 = READLN('cdres1')
 cdresult4 = READLN('cdres1')
 cdresult5 = READLN('cdres1')
 cdresult6 = READLN('cdres1')
 cdresult7 = READLN('cdres1')
 cdresult8 = READLN('cdres1')
 cdresult9 = READLN('cdres1')
 cdresult10 = READLN('cdres1')
CLOSE('cdres1')


OPEN('sres', ''pathm'searchresult', 'A')

WRITELN('sres',cdresult1)
WRITELN('sres',cdresult2)
WRITELN('sres',cdresult3)
WRITELN('sres',cdresult4)
WRITELN('sres',cdresult5)
WRITELN('sres',cdresult6)
WRITELN('sres',cdresult7)
WRITELN('sres',cdresult8)
WRITELN('sres',cdresult9)
WRITELN('sres',cdresult10)

close('sres')

 if cdresult1 = '' then cdresult1 = estring
 if cdresult2 = '' then cdresult2 = estring
 if cdresult3 = '' then cdresult3 = estring
 if cdresult4 = '' then cdresult4 = estring
 if cdresult5 = '' then cdresult5 = estring
 if cdresult6 = '' then cdresult6 = estring
 if cdresult7 = '' then cdresult7 = estring
 if cdresult8 = '' then cdresult8 = estring
 if cdresult9 = '' then cdresult9 = estring
 if cdresult10 = '' then cdresult10 = estring
call display

Exit

DISPLAY:

OPEN('sres', ''pathm'searchresult', 'R')
sresult1 = READLN('sres')
sresult2 = READLN('sres')
sresult3 = READLN('sres')
sresult4 = READLN('sres')
sresult5 = READLN('sres')
sresult6 = READLN('sres')
sresult7 = READLN('sres')
sresult8 = READLN('sres')
sresult9 = READLN('sres')
sresult10 = READLN('sres')
close('sres')
  if sresult1 = '' then sresult1 = estring
  if sresult2 = '' then sresult2 = estring
  if sresult3 = '' then sresult3 = estring
  if sresult4 = '' then sresult4 = estring
  if sresult5 = '' then sresult5 = estring
  if sresult6 = '' then sresult6 = estring
  if sresult7 = '' then sresult7 = estring
  if sresult8 = '' then sresult8 = estring
  if sresult9 = '' then sresult9 = estring
  if sresult10 = '' then sresult10 = estring

re1 = INDEX(sresult1,forbid)
 if re1 >0 then sresult1 = estring

re2 = INDEX(sresult2,forbid)
 if re2 >0 then sresult2 = estring

re3 = INDEX(sresult3,forbid)
 if re3 >0 then sresult3 = estring

re4 = INDEX(sresult4,forbid)
 if re4 >0 then sresult4 = estring

re5 = INDEX(sresult5,forbid)
 if re5 >0 then sresult5 = estring

re6 = INDEX(sresult6,forbid)
 if re6 >0 then sresult6 = estring

re7 = INDEX(sresult7,forbid)
 if re7 >0 then sresult7 = estring

re8 = INDEX(sresult8,forbid)
 if re8 >0 then sresult8 = estring

re9 = INDEX(sresult9,forbid)
 if re9 >0 then sresult9 = estring

re10 = INDEX(sresult10,forbid)
 if re10 >0 then sresult10 = estring


Sendfile Pathm'resultmenu'


TR '1  'sresult1
TR '2  'sresult2
TR '3  'sresult2
TR '4  'sresult4
TR '5  'sresult5
TR '6  'sresult6
TR '7  'sresult7
TR '8  'sresult8
TR '9  'sresult9
TR '10 'sresult10



TR ''
DLOAD:

TR '\^1Download which file 1-10? Enter to quit, S for new search'

  GetChar
   a=upper(result)
   if upper(result) = '###PANIC' then call Carrier

    Select
     when a = '1' then MARKFILE sresult1
     when a = '2' then MARKFILE sresult2
     when a = '3' then MARKFILE sresult3
     when a = '4' then MARKFILE sresult4
     when a = '5' then MARKFILE sresult5
     when a = '6' then MARKFILE sresult5
     when a = '7' then MARKFILE sresult5
     when a = '8' then MARKFILE sresult5
     when a = '9' then MARKFILE sresult5
     when a = '10' then MARKFILE sresult5
     when a = "S" then call main

     otherwise;
    call finish
  end

TR '\^1Do you want to add another file?(Y/N) Enter to download              '
GetChar
   a=UPPER(result)
     if upper(result) = '###PANIC' then call Carrier
     if a = 'Y' then call dload
    bufferflush

CLS
logentry ld
DOWNLOAD

call finish

exit

FINISH:
BBSCOMMAND CF
bufferflush
exit



/*----------------------------- Leave Alone --------------------------------*/

CARRIER:
    if result~='###PANIC' then return
    le= '[33mFileServer Reports:[36m NoCarrier: [35mDropped Carrier Detected![0m'
    logentry le
    bufferflush
exit

------------------------- For Expansion Use : )-------------------------------------
