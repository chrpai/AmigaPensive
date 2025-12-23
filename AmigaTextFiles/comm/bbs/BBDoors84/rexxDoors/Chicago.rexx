/* Chicago Gangland Version 1.21 by Steve Robbins*/
/* Converted by David Prothero */
/* Converted by David Simpson for BBBBS */
/* Version 1.3 by Matt English 6-21-92.
   New Features: Bugs fixxed, plays better in local mode, and
   UnderWorld Casino at location 25, 25!

   New Features in version 1.4: by Matt English 7-10-92
   Full menu option added, Leave a comment to next gangster,
   Game resets and adds winners name to "GangLord Hall of Fame"
   when player reaches 500,000 points.

   Version 1.41 7-25-92 by Matt Egnlish
   There was a small bug! Yikes!

   Version 1.42 7-26-92 - 1.44 10-22-93 by RLStockton
   To work better without BaudBandit in LOCAL mode
*/


options results

CR='0D'x  /* Carraige Return */
CALL TIME('R')
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

bbspath=GETCLIP('BBS_path')
bbspath1 = bbspath'rexxdoors/'
bbspath2=bbspath'rexxDoors/Data/'

PARSE arg Username winnings savewinnings colorflag secs .
IF secs='' THEN secs=3600

signal on syntax

/* Open the support library if it is not already open. */
if ~show('L',"rexxsupport.library") then do
    addlib('rexxsupport.library',0,-30,0)
     end

info='s:CONFIG.BBS'
IF ~EXISTS(info) THEN info='BBS:BBS_TEXT/CONFIG.BBS'
call open(con,info,'R')
IF con=0 THEN EXIT
lynes.1=readln(con)

compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
 bbsname=STRIP(lynes.1)
call close(con)

Name = Username
Spacename = Name
/*ansiflag = result*/
LINES = 20  /* upper(result) - 2 */
ADDRESS rexx 'random(1,2,time('S'))'
if colorflag = 1 then
  do
    WHITE = '[37m'
    RED = '[31m'
    YELLOW ='[33m'
    BLACK = '[30m'
    GREEN = '[32m'
    CYAN = '[36m'
    MAGENTA = '[35m'
    BLUE = '[34m'
    IF ADDRESS()~='BAUD' THEN BLUE=RED
    CLEAR = '[0m'
    HOM = '[H'
    CLSCREEN = ''
  end 
else
  do
    WHITE = ''
    RED = ''
    YELLOW =''
    BLACK = ''
    GREEN = ''
    CYAN = ''
    MAGENTA = ''
    BLUE = ''
    CLEAR = ''
    HOM = ''
    CLSCREEN = ''
end
File.1 =  bbspath2'gangland/instructions'
File.2 =  bbspath2'gangland/news7'
File.3 =  bbspath2'gangland/news6'
File.4 =  bbspath2'gangland/news5'
File.5 =  bbspath2'gangland/news4'
File.6 =  bbspath2'gangland/news3'
File.7 =  bbspath2'gangland/news2'
File.8 =  bbspath2'gangland/news1'
File.9 =  bbspath2'gangland/protect'
File.10 = bbspath2'gangland/position'
File.11 = bbspath2'gangland/chic.oneliner'
File.12 = bbspath2'gangland/chic.fame'
File.13 = bbspath2'gangland/aliases'
File.14 = bbspath2'gangland/top10'
/* Starting Stats */
turnsleft = 25
gangmembers = 50
datec = date()
points = 5000
protection = 0
totalgangmembers = 50
money = 1000
bootleg = 0
costfor = 1
pos = 0
deathflag = 0
menus = 0
  longdashes='-----------------------------------------------------------------------------'
/*begin*/
if exists(bbspath2'gangland/position') then
  do
    call open(lastpos,bbspath2'gangland/position','R')
    do i = 1 to 10000
      mypos.i = readln(lastpos)
      posstartx.i = Word(mypos.i,1)
      posstarty.i = Word(mypos.i,2)
      posgang.i = Word(mypos.i,3)
      posmoney.i = Word(mypos.i,4)
      posalias.i = Delword(mypos.i,1,4)
      if EOF(lastpos) then break
      pos = pos + 1
    end
    call close(lastpos)
  end
call open(prot,file.9,'R')
say'Loading Chicago Gangland...'cr
do c = 1 to 25
   CALL WRITECH(STDOUT,'.')
   do c1 = 1 to 25
      protect.c.c1 = readln(prot)
   end
end
datel = readln(prot)
if datec ~= datel then
  do
    SAY ''||CR
    SAY ''||CR
    SAY CYAN'Standby, running maintenance program.'CLEAR||CR
    call seek(prot,0,'B')
    do i = 1 to 625
        proln.i = readln(prot)
    end
    call close(prot)
    call open(prot,File.9,'W')
    do i = 1 to 625
        call writeln(prot,proln.i)
    end
    call writeln(prot,datec)
    if exists(file.8) then address command 'c:delete 'file.8' quiet'
    if exists(file.7) then address command 'c:rename 'file.7' 'file.8' quiet'
    if exists(file.6) then address command 'c:rename 'file.6' 'file.7' quiet'
    if exists(file.5) then address command 'c:rename 'file.5' 'file.6' quiet'
    if exists(file.4) then address command 'c:rename 'file.4' 'file.5' quiet'
    if exists(file.3) then address command 'c:rename 'file.3' 'file.4' quiet'
    if exists(file.2) then address command 'c:rename 'file.2' 'file.3' quiet'
    call open(news7,file.2,'W')
    call writeln(news7,'******************************************************')
    call writeln(news7,'                THE CHICAGO DAILY NEWS')
    call writeln(news7,'                     'datec)
    call writeln(news7,'******************************************************')
    call writeln(news7,' ')
    call close(news7)

    call open(aliases,bbspath2'gangland/aliases','R')
    i = 0
    do until EOF(aliases)
        i = i + 1
        line.i = readln(aliases)
    end
    call close(aliases)
    do u = 1 to 10000
        if u = i then signal cont
        Namem = word(line.u,1)
        if namem = '' then namem = junk
        alias = Delword(line.u,1,1)
        if exists(bbspath2'gangland/'Namem) then
          do
            call open(stats,bbspath2'gangland/'Namem,'R')
            aliasa = readln(stats)
            Turnsleftm = readln(stats)
            gangmembersm = readln(stats)
            datem = readln(stats)
            startxm = readln(stats)
            startym = readln(stats)
            pointsm = readln(stats)
            moneym = readln(stats)
            totalgangmembersm = readln(stats)
            protectionm = readln(stats)
            bootlegm = readln(stats)
            call close(stats)
            protectmoney = protectionm * 200
            bootlegmoney = bootlegm * 500
            moneym = moneym + protectmoney + bootlegmoney
            turnsleftm = 25
            call open(stats,bbspath2'gangland/'Namem,'W')
            call writeln(stats,aliasa)
            call writeln(stats,turnsleftm)
            call writeln(stats,gangmembersm)
            call writeln(stats,datem)
            call writeln(stats,startxm)
            call writeln(stats,startym)
            call writeln(stats,pointsm)
            call writeln(stats,moneym)
            call writeln(stats,totalgangmembersm)
            call writeln(stats,protectionm)
            call writeln(stats,bootlegm)
            call close(stats)
          end
    end
  end
cont:
SAY ''||CR
SAY ''||CR
SAY BLUE'Done'||CR
call close(prot)
                                             
street.1 = First
street.2 = Second
street.3 = Third
street.4 = Fourth
street.5 = Fifth
street.6 = Sixth
street.7 = Seventh
street.8 = Eighth
street.9 = Ninth
street.10 = Tenth
street.11 = Eleventh
street.12 = Twelveth
street.13 = Thirteenth
street.14 = Fourteenth
street.15 = Fifteenth
street.16 = Sixteenth
street.17 = Seventeenth
street.18 = Eighteenth
street.19 = Nineteenth
street.20 = Twentieth
street.21 = Twentyfirst
street.22 = Twentysecond
street.23 = Twentythird
street.24 = Twentyfourth
street.25 = Twentyfifth

say CLSCREEN
SAY ''||CR
SAY ''||CR
SAY BLUE'                 +----------------------------------------------+'CLEAR||CR
SAY BLUE'                 |               'YELLOW'CHICAGO GANGLAND'BLUE'               |'CLEAR||CR
SAY BLUE'                 |            'YELLOW'Written for SkyLine BBS'BLUE'           |'CLEAR||CR
SAY BLUE'                 |               'YELLOW'by Steve Robbins'BLUE'               |'CLEAR||CR
SAY BLUE'                 |                'YELLOW'Copyright 1990'BLUE'                |'CLEAR||CR
SAY BLUE'                 |                                              |'CLEAR||CR
SAY BLUE'                 |            'RED'Converted to TransAmiga'BLUE'           |'CLEAR||CR
SAY BLUE'                 |               'RED'by David Prothero'BLUE'              |'CLEAR||CR
SAY BLUE'                 |                                              |'CLEAR||CR
SAY BLUE'                 |              'RED'Converted to BBBBS'BLUE'              |'CLEAR||CR
SAY BLUE'                 |               'RED'by David Simpson'BLUE'               |'CLEAR||CR
SAY BLUE'                 |                                              |'CLEAR||CR
SAY BLUE'                 |       'MAGENTA'Version 1.44 includes UnderWorld'BLUE'       |'CLEAR||CR
SAY BLUE'                 |       'MAGENTA'Casino feature, located at 25, 25.'BLUE'     |'CLEAR||CR
SAY BLUE'                 |             'MAGENTA'by Matt English 7-25-92'BLUE'          |'CLEAR||CR
SAY BLUE'                 |                                              |'
SAY BLUE'                 +----------------------------------------------+'CLEAR||CR
SAY CR
say ''cr
call pressreturn
/*continue*/

aliascheck: 
call open(aliases,bbspath2'gangland/aliases','R')
u = 1
do until EOF(aliases)
   alias1.u = readln(aliases)
   alias2.u = word(alias1.u,1)
   alias3.u = Delword(alias1.u,1,1)
   u = u + 1
end
call close(aliases)
marker = u - 1
if ~exists(bbspath2'gangland/'Name) then
  do
    options prompt Magenta'Please enter an alias: '
    pull alias
    options prompt ' '
    if alias = '' then
      do
        SAY RED'You must enter an alias!'CLEAR||CR
        signal aliascheck
      end
    u = 1
    do i = 1 to marker
        if alias = alias3.u then
          do
            SAY RED'That alias is already being used!'||CR
            CALL delay(100)
            signal aliascheck
          end                  
    u = u + 1
    end
    call open(aliases,bbspath2'gangland/aliases','W')
    do i = 1 to (u + 1)
        call writeln(aliases,alias1.i)
    end
    call writeln(aliases,Name' 'alias)
    call close(aliases)
  end
signal begin 

begin:
if ~exists(bbspath2'gangland/'Name) then do
   call open(stats,bbspath2'gangland/'Name,'W')
   call writeln(stats,alias)
   call writeln(stats,turnsleft)
   call writeln(stats,gangmembers)
   call writeln(stats,datec)
   startx = upper(random(1,25,time('s')))
   call writeln(stats,startx)
   starty = upper(random(1,25,time('s')))
   call writeln(stats,starty)
   call writeln(stats,points)
   call writeln(stats,money)
   call writeln(stats,totalgangmembers)
   call writeln(stats,protection)
   call writeln(stats,bootleg)
   call close(stats)
   call open(news7,file.2,'A')
   call writeln(news7,'Ganglord 'alias' has moved into the Chicago!')
   call close(news7)
   signal menuopt
end
else do    
   call open(stats,bbspath2'gangland/'Name,'R')
   alias = readln(stats)
   Turnsleft = readln(stats)
   gangmembers = readln(stats)
   date = readln(stats)
   startx = readln(stats)
   starty = readln(stats)
   points = readln(stats)
   money = readln(stats)
   totalgangmembers = readln(stats)
   protection = readln(stats)
   bootleg = readln(stats)
   call close(stats)
   if datec = date & turnsleft = 0 then do
      say CLSCREEN
      SAY ''||CR
      SAY RED'You are out of turns for today!'CLEAR||CR
      CALL delay(100)
      signal quickexit
   end   
   if datec ~= date then turnsleft = 25
   call open(news7,file.2,'A')
   call writeln(news7,'Ganglord 'alias' is active again!')
   call close(news7)
   signal menuopt
end

menuopt:
 say ''cr
options prompt'   Would you like to have full menus for this game? (N,y) > '
pull menuans
if menuans = 'Y' then menus = 1
if menuans = 'N' then do
 say ''cr
 say' OK, no menus. If you change your mind, type ''T'' at the prompt.'cr
 end
say ''cr
signal Fame

Fame:
 if exists(file.12) then do
  call open(fm,file.12,'R')
  fameline = '********************************************'
  fameline = center(fameline,70)
  say MAGENTA''fameline''cr
  headerline = 'GangLord Hall of Fame'
  headerline = center(headerline,70)
  say YELLOW''headerline''cr
  say MAGENTA''fameline''CLEAR''cr
  say ''cr
   do until eof(fm)
    fmline = readln(fm)
    fmline = center(fmline,70)
    say GREEN''fmline''clear''cr
    end
  say ''cr
  say ''cr
  call close(fm)
  end
  CALL delay(100)
 signal readoneliner

readoneliner:
if ~exists(file.11) then signal MAIN
say ''cr
call open(fny,file.11,'R');mess=readln(fny);bywhom=readln(fny)
say ''longdashes''cr;say ''CYAN''mess ''YELLOW''bywhom''CLEAR''cr;say ''longdashes''cr
call close(fny);call delay(200)
signal MAIN

main:
if startx = 1 & starty = 1 then signal pool
if startx = 25 & starty = 25 then signal casino
say CLSCREEN
if menus = 1 then do
  SAY ''||CR
   SAY '          'YELLOW'Q = Quit to' bbsname ''||CR
   SAY '          S = Shootout'||CR
   SAY '          O = Offer protection to the storeowners'||CR
   SAY '          A = Add more protection to this block'||CR
   SAY '          H = Hit the the protected block of another gang'||CR
   SAY '          M = Move to another block'||CR
   SAY '          G = Ganglord scores'||CR
   SAY '          N = Chicago Daily News'||CR
   SAY '          I = Instructions'||CR
   SAY '          Y = Your statistics'||CR
   SAY '          T = Toggle menus on/off'CLEAR||CR
   SAY ''||CR
  end
SAY center(''GREEN''alias'',70)||CR
SAY ''||CR
SAY CYAN'You are at the corner of 'street.startx' North and 'street.starty' East'||CR
SAY 'You have 'gangmembers' gangmembers with you'||CR
protect_alias = Delword(protect.startx.starty,1,1)
protect_gang = Word(protect.startx.starty,1)
if protect_alias ~= '' then do
   proflag = 0
   SAY 'This block is protected by' GREEN||protect_alias||CLEAR||CR
end
else do
   SAY 'This block is not under protection'||CLEAR||CR
   proflag = 1
end
if 1 <= pos then do i = 1 to pos
   if posstartx.i = startx & posstarty.i = starty & alias ~= posalias.i then
   SAY 'You also see 'posalias.i' on this block there are 'posgang.i' gangmembers with him!'||CR
end
SAY ''||CR
Options prompt MAGENTA'Q S O A H M G N I Y T ? ('YELLOW||turnsleft||MAGENTA')>'CLEAR
pull ans
CALL checkBBS()
if ans = '###PANIC' then signal goodbye
if ans = 'Q' then signal goodbye
if ans = 'S' then signal shootout
if ans = 'O' then signal protection
if ans = 'A' then signal addon
if ans = 'H' then signal hitother
if ans = 'M' then signal move
if ans = 'G' then signal highscores
if ans = 'N' then signal news
if ans = 'I' then signal instructions
if ans = 'Y' then signal stats
if ans = 'T' then do
 say ''cr
 if menus = 0 then do
  say'  OK, I''ll turn your menus on!'cr
  menus = 1
  say ''cr
  signal main
  end
 if menus = 1 then do
  say'  OK, your menus are now turned off.'cr
  menus = 0
  say ''cr
  signal main
  end
end
if ans = '' then signal main
if ans = '?' then do
   SAY ''||CR
   SAY '          'YELLOW'Q = Quit to' bbsname ''||CR
   SAY '          S = Shootout'||CR
   SAY '          O = Offer protection to the storeowners'||CR
   SAY '          A = Add more protection to this block'||CR
   SAY '          H = Hit the the protected block of another gang'||CR
   SAY '          M = Move to another block'||CR
   SAY '          G = Ganglord scores'||CR
   SAY '          N = Chicago Daily News'||CR
   SAY '          I = Instructions'||CR
   SAY '          Y = Your statistics'||CR
   SAY '          T = Toggle menus'CLEAR||CR
   SAY ''||CR
   call pressreturn
   /*continue*/
end
signal main
     
pool:
say CLSCREEN
if menus = 1 then do
   SAY ''||CR
   SAY '          'YELLOW'M = Move'||CR
   SAY '          H = Hire new gangmembers'||CR
   SAY '          B = Buy a bootleg operation'||CR
   SAY '          G = Gamble at a game of pool'||CR
   SAY '          Q = Quit to 'bbsname ''CLEAR||CR
   SAY ''||CR
  end
say yellow'Check out the UnderWorld Casino! At 25, 25.'clear||cr
say ''cr
SAY GREEN'Welcome to the Southside Pool Hall'||CR
SAY 'What is your pleasure?'CLEAR||CR
SAY ''||CR
Options Prompt YELLOW'M H B G Q ? ('turnsleft')>'CLEAR
pull ANS
if ans = '###PANIC' then signal goodbye
if ans = 'M' then signal move
if ans = 'H' then signal hire
if ans = 'B' then signal still
if ans = 'G' then signal gamble
if ans = 'Q' then do
   startx = upper(random(1,25,time('s')))
   starty = upper(random(1,25,time('s')))
   signal goodbye
end
if ans = '' then signal pool
if ans = '?' then do
   SAY ''||CR
   SAY '          'YELLOW'M = Move'||CR
   SAY '          H = Hire new gangmembers'||CR
   SAY '          B = Buy a bootleg operation'||CR
   SAY '          G = Gamble at a game of pool'||CR
   SAY '          Q = Quit to 'bbsname ''CLEAR||CR
   SAY ''||CR
   call pressreturn
   /*continue*/
end
signal main

casino:
say CLSCREEN
if menus = 1 then do
   SAY ''||CR
   SAY '          'YELLOW'M = Move'||CR
   SAY '          H = Hire new gangmembers'||CR
   SAY '          B = Play a hand of BlackJack'||CR
   SAY '          S = Try the Slot machine'||CR
   SAY '          Q = Quit to 'bbsname ''CLEAR||CR
   SAY ''||CR
   end
SAY GREEN'   Welcome to the Underworld Casino'||CR
SAY '   What is your pleasure?'CLEAR||CR
say'   You have $'money''cr
SAY ''||CR
Options Prompt YELLOW'M H B S Q ? ('turnsleft')>'CLEAR
pull ANS
if ans = '###PANIC' then signal goodbye
if ans = 'M' then signal move
if ans = 'H' then signal hire
if ans = 'B' then signal BlackJack
if ans = 'S' then signal Slots
if ans = 'Q' then do
   startx = upper(random(1,25,time('s')))
   starty = upper(random(1,25,time('s')))
   signal goodbye
end
if ans = '' then signal pool
if ans = '?' then do
   SAY ''||CR
   SAY '          'YELLOW'M = Move'||CR
   SAY '          H = Hire new gangmembers'||CR
   SAY '          B = Play a hand of BlackJack'||CR
   SAY '          S = Try the Slot machine'||CR
   SAY '          Q = Quit to 'bbsname ''CLEAR||CR
   SAY ''||CR
   call pressreturn
   /*continue*/
end
signal main

BlackJack:
   if money < 1 then do
    say'  You don''t seem to have any cash at this time!'cr
    say'  Sorry, but we don''t play for the fun of it!'
    call casino
    end
   if turnsleft < 1 then do
    say'  Sorry, you don''t have any turns left!'cr
    say'  Try again next time!'cr
    say''cr
    call casino
    end
   turnsleft = turnsleft - 1
   CALL SetClip('CHICMON',money)
   if colorflag = 1 then CALL SetClip('CHICCOLOR','1')
   say ''cr
   say'   OK, let''s play a hand of BlackJack!'

/*trace ?r  */

curdir=PRAGMA('D')
CALL setdir(bbspath'rexxDoors')

   say ''cr
   /*INTERPRET BlackJack.rexx*/
   doors.temp = BlackJack.rexx
   INTERPRET 'call' doors.temp

   money = GetClip('CHICMON')
   CALL SetClip('CHICMON','')
   CALL SetClip('CHICCOLOR','')
   say ''cr
   CALL setdir(curdir)
   Call casino

Slots:
   if money < 1 then do
    say'  You don''t seem to have any cash at this time!'cr
    say'  Sorry, but we don''t play for the fun of it!'
    call casino
    end
   if turnsleft < 1 then do
    say'  Sorry, you don''t have any turns left!'cr
    say'  Try again next time!'cr
    say''cr
    call casino
    end
   turnsleft = turnsleft - 1
   CALL SetClip('CHICMON',money)
   if colorflag = 1 then CALL SetClip('CHICCOLOR','1')
   say ''cr
   /*say'   OK, let''s play a hand of BlackJack!'*/

/*trace ?r  */

curdir=PRAGMA('D')
CALL setdir(bbspath'rexxDoors')

   say ''cr
   /*INTERPRET BlackJack.rexx*/
   doors.temp = One_Armed_Bandit.rexx
   INTERPRET 'call' doors.temp

   money = GetClip('CHICMON')
   CALL SetClip('CHICMON','')
   CALL SetClip('CHICCOLOR','')
   say ''cr
   CALL setdir(curdir)
   Call casino


setdir:
PARSE ARG tempdir
CALL PRAGMA('D',STRIP(tempdir))
directory=PRAGMA('D')
slash=LASTPOS('/',directory)
IF slash=0 THEN slash=LASTPOS(':',directory)
plaindir=directory
IF slash>0 THEN plaindir=SUBSTR(plaindir,slash+1)
RETURN


hire:
say CLSCREEN
   if turnsleft < 1 then do
    say'  Sorry, you don''t have any turns left!'cr
    say'  Try again next time!'cr
    say''cr
    signal main
    end
cost = random(100,200,time('s'))
if costfor > cost then cost = costfor
costfor = cost
SAY CYAN'New gangmembers salaries are 'cost' dollars each.'||CR
SAY 'You currently have 'gangmembers 'gangmembers and 'money' dollars.'||CR
SAY ''||CR
possible = money % cost
options prompt MAGENTA'How many gangmembers would you like to hire?('possible' max)'
pull newgang
options prompt ' '
if newgang = 0 | newgang = '' then signal /*pool*/main
do i = 1 to length(newgang)
   if ~datatype(substr(newgang,i,1),N) then do
      SAY RED'You must enter a valid number!'||CR
      CALL delay(100)
      /*signal pool */
      signal main
   end
end
totcost = cost * newgang
if totcost > money then do
   SAY RED'You don''t have enough money for 'newgang' gangmembers!'||CR
   CALL delay(100)
   /*signal pool*/
   signal main
end
SAY MAGENTA'The total of the salaries will be 'totcost' dollars.'||CR
SAY ''||CR
options prompt CYAN'Are you sure you want to hire them? 'CLEAR
pull ans
options prompt ' '
if ans = N then signal /*pool*/main
money = money - totcost
gangmembers = gangmembers + newgang
totalgangmembers = totalgangmembers + newgang
turnsleft = turnsleft - 1
/*signal pool*/
signal main

still:
   if turnsleft < 1 then do
    say'  Sorry, you don''t have any turns left!'cr
    say'  Try again next time!'cr
    say''cr
    signal main
    end
if money < 5000 then do
   SAY RED'You do not have enough money!'CLEAR||CR
   CALL delay(100)
   signal pool
end
SAY ''||CR
SAY ''||CR
SAY MAGENTA'Bootleg operations cost $5000 each. You have 'money' dollars.'||CR
options prompt 'How many bootleg operations would you like to purchase?  '
pull ans
do i = 1 to length(ans)
   if ~datatype(substr(ans,i,1),N) then do
      SAY RED'You must enter a valid number!'||CR
      CALL delay(100)
      /*signal pool */
      signal still
   end
end
if ans = '' then signal pool
cost = 5000 * ans
if money < cost then do
   SAY RED'You do not have that much money!'||CR
   signal still
end
options prompt 'It will cost you 'cost' dollars. Are you sure? '
pull ans2

if ans2 = 'N' then signal pool
money = money - cost
bootleg = bootleg + ans
SAY ''||CR
SAY CYAN'Nice doing business with you.'CLEAR||CR
turnsleft = turnsleft - 1
CALL delay(100)
signal pool

gamble:
say CLSCREEN
   if turnsleft < 1 then do
    say'  Sorry, you don''t have any turns left!'cr
    say'  Try again next time!'cr
    say''cr
    signal pool
    end
options prompt CYAN'You have $'money', how much would you like to bet?($500 limit)'CLEAR
pull bet
options prompt ' '
if bet > 500 then signal gamble
do i = 1 to length(bet)
   if ~datatype(substr(bet,i,1),N) then do
      SAY RED'You must enter a valid whole number!'||CR
      CALL delay(100)
      signal pool
   end
end
SAY ''||CR
if bet > money then do
  SAY RED'You don''t have that much money'CLEAR||CR
  signal pool
  end
win = random(1,3,time('s'))
if bet = '' then signal pool
turnsleft = turnsleft - 1
SAY YELLOW'Rack''em up'||CR
CALL delay(100)
SAY ''||CR
if win = 2 then do
   how = random(1,6,time('s'))
   select
      when how = 1 then SAY MAGENTA'You sank the 8 ball off the break!'||CR
      when how = 2 then SAY MAGENTA'You left him with all 7 balls on the table'||CR
      when how = 3 then SAY MAGENTA'You squeak out a victory by sinking the 8 ball'||CR
      when how = 4 then SAY MAGENTA'He scratches on the 8 ball.'||CR
      when how = 5 then SAY MAGENTA'You leave him with 3 balls on the table. Good work.'||CR
      otherwise SAY MAGENTA'You run the table on him!'||CR
   end
   money = money + bet
   SAY ''||CR
   SAY GREEN'You win!'||CR
end
if win = 1 | win = 3 then do
   how = random(1,6,time('s'))
   select
      when how = 1 then SAY GREEN'He sinks the 8 ball off the break!'||CR
      when how = 2 then SAY GREEN'He left you with all 7 balls on the table'||CR
      when how = 3 then SAY GREEN'He squeaks out a victory by sinking the 8 ball'||CR
      when how = 4 then SAY GREEN'You scratch on the 8 ball. Tough Luck!'||CR
      when how = 5 then SAY GREEN'He leaves you with 3 balls on the table.'||CR
      otherwise SAY GREEN'He runs the table on you!'||CR
   end
   money = money - bet
   SAY ''||CR
   SAY RED'You lose!'||CR
end
CALL delay(100)
signal pool
 
shootout:killflag = 0
shoottotal = 0
say CLSCREEN
do i = 1 to pos
   if posstartx.i = startx & posstarty.i = starty & alias ~= posalias.i then do
      shoottotal = shoottotal + 1
   end
end
if shoottotal = 0 then do
   SAY RED'There is no one to attack!'||CR
   CALL delay(100)
   signal main
end
SAY MAGENTA'Ganglords at your block:'||CR
SAY ''||CR
do i = 1 to pos
   if posstartx.i = startx & posstarty.i = starty & alias ~= posalias.i then do
      SAY BLUE posalias.i  'with' posgang.i 'gangmembers'||CR
      shoottotal = shoottotal + 1
   end
end
SAY ''||CR
w = 0
options prompt MAGENTA'Who do you want to attack?(Return to Quit) '
pull victim
options prompt ' '
if victim = "" then signal main
do i = 1 to pos
   if victim = posalias.i then do
      w = 1
      r = i 
      fight2 = posgang.i
      break
   end
   if w = 1 then break
end
if posalias.r ~= victim then do
   SAY RED'There is no one at this block by that name!'||CR
   CALL delay(100)
   signal main
end
fight3 = fight2
yourper1 = (gangmembers / (gangmembers + fight2)) * 100
if yourper1 > 95 then yourper1 = 95
if yourper1 < 5 then yourper1 = 5
if w = 0 then signal shootout
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' tries to have a shootout with 'victim'.')
call close(news7)
fight1 = gangmembers
say CLSCREEN
SAY MAGENTA'         Your gang:                      His gang:'CR
SAY BLUE   '        ----------                      ---------'CR

shootagain:
say cyan   '             'gangmembers'                                'fight2''CR
SAY ''||CR
if killflag = 0 then do
   options prompt YELLOW'RETURN to Continue  K to Kill  Q to Quit '
   pull ans
   if ans = Q then signal quitter2
   if ans = K then killflag = 1
end
losers1 = Random(1,100)
if yourper1 >= losers1 then fight2 = fight2 - 1
else gangmembers = gangmembers - 1
/*if gangmembers <= -1 then do
   gangmembers = -1
   signal shootyou
end
if fight2 <= -1 then do
   fight2 = -1
   signal shoothim */
if gangmembers <= 0 then do
   gangmembers = 0
   signal shootyou
end
if fight2 <= 0 then do
   fight2 = 0
   signal shoothim
end
/*if colorflag = 1 then do
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY '                                                  '||CR
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
end      */
signal Shootagain 
              
shootyou:
/*if colorflag = 1 then do
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY '                                                  '||CR
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
end  */
say cyan   '             'gangmembers'                                'fight2''CR
SAY ''||CR
CALL delay(50)
call open(aliases,bbspath2'gangland/aliases','R')
u = 1
do until EOF(aliases)
  alias1.u = readln(aliases)
   alias2.u = word(alias1.u,1)
   alias3.u = Delword(alias1.u,1,1)
   if alias3.u = victim then do
      aliasfix = alias2.u
      signal shootyou1
   end
   u = u + 1
end

shootyou1:
call close(aliases)
call open(provictim,bbspath2'gangland/'aliasfix,'R')
vicalias = readln(provictim)
vicTurnsleft = readln(provictim)
vicgangmembers = readln(provictim)
vicdate = readln(provictim)
vicstartx = readln(provictim)
vicstarty = readln(provictim)
vicpoints = readln(provictim)
vicmoney = readln(provictim)
victotalgangmembers = readln(provictim)
vicprotection = readln(provictim)
vicbootleg = readln(provictim)
call close(provictim)
vicgangmembers = vicgangmembers - (fight3 - fight2)
victotalgangmembers = victotalgangmembers - (fight3 - fight2)
pointsg = victotalgangmembers * 100
pointsp = vicprotection * 1000
vicpoints = pointsg + pointsp
vicmoney = vicmoney + money
vicbootleg = vicbootleg + bootleg
call open(provictim,bbspath2'gangland/'aliasfix,'W')
call writeln(provictim,vicalias)
call writeln(provictim,victurnsleft)
call writeln(provictim,vicgangmembers)
call writeln(provictim,vicdate)
call writeln(provictim,vicstartx)
call writeln(provictim,vicstarty)
call writeln(provictim,vicpoints)
call writeln(provictim,vicmoney)
call writeln(provictim,victotalgangmembers)
call writeln(provictim,vicprotection)
call writeln(provictim,vicbootleg)
call close(provictim)
call open(top10,bbspath2'gangland/top10','R')
do i = 2 to 11
   tops = readln(top10)
   topalias.i = delword(tops,1,1)
   topscore.i = word(tops,1)
   if victim = topalias.i then topscore.i = vicpoints
end
call close(top10)
call open(top10,bbspath2'gangland/top10','W')
do i = 1 to 10
   call writeln(top10,topscore.i' 'topalias.i)
end
call close(top10)
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' is killed by 'victim'''s gang!')
call close(news7)
signal death

shoothim:
/*if colorflag = 1 then do
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY '                                                  '||CR
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
end   */
say cyan   '             'gangmembers'                                'fight2''CR
SAY ''||CR
SAY center(YELLOW'You wipe him out!',76)||CR
totalgangmembers = totalgangmembers - (Fight1 - gangmembers)
call open(aliases,bbspath2'gangland/aliases','R')
u = 1
do until EOF(aliases)
   alias1.u = readln(aliases)
   alias2.u = word(alias1.u,1)
   alias3.u = Delword(alias1.u,1,1)
   if alias3.u = victim then break
   u = u + 1
end
call close(aliases)
call open(provictim,bbspath2'gangland/'alias2.u,'R')
vicalias = readln(provictim)
vicTurnsleft = readln(provictim)
vicgangmembers = readln(provictim)
vicdate = readln(provictim)
vicstartx = readln(provictim)
vicstarty = readln(provictim)
vicpoints = readln(provictim)
vicmoney = readln(provictim)
victotalgangmembers = readln(provictim)
vicprotection = readln(provictim)
vicbootleg = readln(provictim)
call close(provictim)
money = vicmoney + money
bootleg = vicbootleg + bootleg
do i = 1 to pos
   if victim = posalias.i then do
      money = money + posmoney.i
      mypos.i = ''
      posstartx.i = ''
      posstarty.i = ''
      posgang.i = ''
      posmoney.i = ''
      posalias.i = ''
   end
end
call open(aliases,bbspath2'gangland/aliases','R')
do o = 1 to 10000
   alias1.o = readln(aliases)
   alias2.o = Word(alias1.o,1)
   alias3.o = Delword(alias1.o,1,1)
   if victim = alias3.o then do
      /*delete(bbspath2'gangland/'alias2.o)*/
      address command 'c:delete 'bbspath2'gangland/'alias2.o' quiet'
      alias1.o = ''
   end
   if EOF(aliases) then break
end
j = o
call close(aliases)
call open(aliases,bbspath2'gangland/aliases','W')
do o = 1 to 10000
   if alias1.o ~= '' then call writeln(aliases,alias1.o)
   if o = j then break 
end
call close(aliases)
do c = 1 to 25
   do c1 = 1 to 25
      if Delword(protect.c.c1,1,1) = victim then
      protect.c.c1 = 0
    end
end
SAY ''||CR
call open(top10,bbspath2'gangland/top10','R')
do i = 1 to 10
   tops.i = readln(top10)
   if delword(tops.i,1,1) = victim then tops.i = '0 'victim
end
call close(top10)
call open(top10,bbspath2'gangland/top10','W')
do i = 1 to 10
   call writeln(top10,tops.i)
end
call close(top10)
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' slaughters 'victim)
call close(news7)
signal main

quitter2:
do i = 1 to pos
   if victim = posalias.i then do
      w = 1 
      posgang.i = fight2
      break
   end
end
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' runs from the shootout.')
call close(news7)
SAY center(YELLOW'Chicken!',76)||CR
CALL delay(100)
signal main

protection:
if gangmembers < 1 then do
   SAY ''||CR
   SAY RED'You don''t have enough gangmembers with you to offer protection!'CLEAR||CR
   CALL delay(100)
   signal main
end
protectors = word(protect.startx.starty,1)
if protectors = 0 then signal noprotect
do
   protect_alias = Delword(protect.startx.starty,1,1)
   SAY ''||CR
   SAY ''||CR
   SAY RED'The block already has protection from 'protect_alias'!'CLEAR||CR
   CALL delay(100)
   signal main 
end

noprotect:
SAY ''||CR
options prompt GREEN'How many gangmembers do you want to leave here?'CLEAR
pull protectors
options prompt ' '
if protectors = 0 then signal main
if protectors = ' ' then signal main
do i = 1 to length(protectors)
   if ~datatype(substr(protectors,i,1),N) then do
      SAY RED'You must enter a valid whole number!'||CR
      CALL delay(100)
      signal main
   end
end
if gangmembers - protectors < 0 then do
   SAY RED'You don''t have that many gangmembers!'||CR
   signal noprotect
end
gangmembers = gangmembers - protectors
protection = protection + 1
protect.startx.starty = protectors' 'alias
turnsleft = turnsleft - 1
signal main

addon:
say CLSCREEN
protect_gang = word(protect.startx.starty,1)
if protect_alias ~= alias then do
   SAY RED'You have to move to a block you are protecting to add gangmembers'CLEAR||CR
   CALL delay(100)
   signal main
end
SAY GREEN'You currently have 'protect_gang' gangmembers protecting this block.'||CR
options prompt GREEN'How many gangmembers do you want to add?'
pull addto
options prompt ' '
if addto = '' then signal main
if gangmembers - addto < 0 then do
   SAY RED'You don''t have that many gangmembers!'||CR
   CALL delay(100)
   signal main
end
if protect_gang + addto < 1 then do
   SAY RED'You can''t have less than 1 gangmember!'||CR
   CALL delay(100)
   signal main
end
protect_add = addto + protect_gang
protect.startx.starty = protect_add ' ' alias
gangmembers = gangmembers - addto
signal main

hitother:
killflag = 0
say CLSCREEN
if proflag = 1 then do
   SAY RED'There is no one protecting this block!'CLEAR||CR
   CALL delay(100)
   signal main
end
if alias = protect_alias then do
   SAY RED'You cannot attack yourself!'||CR
   CALL delay(100)
   signal main
end
lostgang = protect_gang
yourper = (gangmembers / (gangmembers + lostgang)) * 100
if yourper > 95 then youiner = 95
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' tries to hit 'protect_alias'''s block.')
call close(news7)
SAY MAGENTA'           Your gang:                      His gang:'CR
SAY BLUE   '          ----------                      ---------'CR

hitagain:
say cyan   '               'gangmembers'                              'protect_gang''CR
SAY ''||CR
if killflag = 0 then do
   options prompt YELLOW'RETURN to Continue  K to kill  Q to Quit '
   pull ANS
   if ans = Q then signal quitter
   if ans = K then killflag = 1
end
losers = random(1,100)
if yourper >= losers then Protect_gang = Protect_gang - 1
else gangmembers = gangmembers - 1
if gangmembers <= -1 then do
   gangmembers = -1
   signal hityou
end
if protect_gang <= 0 then do
   protect_gang = 0
   signal hitthem
end
/*if colorflag = 1 then do
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY '                                                  '||CR
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
end  */
signal hitagain   

Hityou:
/*if colorflag = 1 then do
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY '                                                  '||CR
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
end  */
say cyan   '               'gangmembers'                              'protect_gang''CR
SAY ''||CR
protect.startx.starty = protect_gang' 'protect_alias
CALL delay(100)
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' is killed by 'protect_gang'''s gang!')
call close(news7)
signal death

hitthem:
/*if colorflag = 1 then do
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY ''||CR
   SAY '                                                  '||CR
   SAY HOM||CR
   SAY ''||CR
   SAY ''||CR
end    */
say cyan   '               'gangmembers'                              'protect_gang''CR
SAY ''||CR
protect.startx.starty = 0
SAY ''||CR
SAY center(YELLOW'You wipe him out!',76)||CR
CALL delay(100)
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' slaughters 'protect_alias'''s gang!')
call close(news7)
call open(aliases,bbspath2'gangland/aliases','R')
u = 0
do until EOF(aliases)
   u = u + 1
   alias1.u = readln(aliases)
   alias2.u = word(alias1.u,1)
   alias3.u = Delword(alias1.u,1,1)
   if alias3.u = protect_alias then do
      alias2 = alias2.u
      alias3 = alias3.u
      break
   end 
end
call close(aliases)
call open(provictim,bbspath2'gangland/'alias2,'R')
vicalias = readln(provictim)
vicTurnsleft = readln(provictim)
vicgangmembers = readln(provictim)
vicdate = readln(provictim)
vicstartx = readln(provictim)
vicstarty = readln(provictim)
vicpoints = readln(provictim)
vicmoney = readln(provictim)
victotalgangmembers = readln(provictim)
vicprotection = readln(provictim)
vicbootleg = readln(provictim)
call close(provictim)
vicprotection = vicprotection - 1
victotalgangmembers = victotalgangmembers - lostgang
call open(provictim,bbspath2'gangland/'alias2,'W')
call writeln(provictim,vicalias)
call writeln(provictim,victurnsleft)
call writeln(provictim,vicgangmembers)
call writeln(provictim,vicdate)
call writeln(provictim,vicstartx)
call writeln(provictim,vicstarty)
call writeln(provictim,vicpoints)
call writeln(provictim,vicmoney)
call writeln(provictim,victotalgangmembers)
call writeln(provictim,vicprotection)
call writeln(provictim,vicbootleg)
call close(provictim)
call open(top10,bbspath2'gangland/top10','R')
do i = 1 to 10
   tops = readln(top10)
   topalias.i = delword(tops,1,1)
   topscore.i = word(tops,1)
   if protect_alias = topalias.i then topscore.i = (topscore.i - 1000 - (lostgang * 100))
end
call close(top10)
call open(top10,bbspath2'gangland/top10','W')
do i = 1 to 10
   call writeln(top10,topscore.i' 'topalias.i)
end
call close(top10)
signal main

quitter:
protect.startx.starty = protect_gang' 'protect_alias
call open(news7,file.2,'A')
call writeln(news7,'Ganglord 'alias' runs from the fight.')
call close(news7)
SAY center(YELLOW'Chicken!',76)||CR
CALL delay(100)
signal main

move:
if turnsleft < 1 then do
   SAY ''||CR
   SAY RED'You do not have any turns left!'||CR
   CALL delay (100)
   signal main
end
SAY ''||CR
SAY ''||CR
SAY BLUE'What location do you want to move to?'||CR
options prompt '                               North:'
pull startxtemp1
options prompt ' '
startxtemp = strip(startxtemp1,'L','0')
do i = 1 to 25
   if startxtemp = i then leave
   if i = 25 then do
      SAY ''||CR
      SAY RED'Street numbers can only be whole numbers between 1 and 25'CLEAR||CR
      CALL delay(100)
      signal main
   end
end
options prompt '                                East:'
pull startytemp1
options prompt ' '
startytemp = strip(startytemp1,'L','0')
do i = 1 to 25
   if startytemp = i then leave
   if i = 25 then do
      SAY ''||CR
      SAY RED'Street numbers can only be whole numbers between 1 and 25'CLEAR||CR
      CALL delay(100)
      signal main
   end
end
startx = startxtemp
starty = startytemp
turnsleft = turnsleft - 1
signal main

highscores:
call open(top10,bbspath2'gangland/top10','R')
pointsg = totalgangmembers * 100
pointsp = protection * 1000
points = pointsg + pointsp
topalias.1 = alias
topscore.1 = points
do i = 2 to 11
   tops = readln(top10)
   topalias.i = delword(tops,1,1)
   topscore.i = word(tops,1)
   if topalias.1 = topalias.i then topscore.i = -1
end
call close(top10)
do i = 1 to 10
   do h = i + 1 to 11
      if topscore.i < topscore.h then do
         temp = topscore.i
         topscore.i = topscore.h
         topscore.h = temp
         tempor = topalias.i
         topalias.i = topalias.h
         topalias.h = tempor
      end
   end
end
say CLSCREEN

/*----------------------------------------------------------------------
                         Check for a winner!
-----------------------------------------------------------------------*/
wingoal = 500000
 do i = 1 to 10
  if datatype(topscore.i,'N') = 1 & topscore.i >= wingoal then do
   winner = topalias.i
   winscore = topscore.i
    if ~exists(file.12) then do
     call open(winfile,file.12,'W')
     call writeln(winfile,date'    'winner'    'winscore)
     call close(winfile)
     temp.file = 'Ram:temp.file12'
     address command 'c:copy 'file.12' 'temp.file' quiet'
     end
    else do
     call open(winfile,file.12,'A')
     writeln(winfile,date'    'winner'    'winscore)
     call close(winfile)
     temp.file = 'Ram:temp.file12'
     address command 'c:copy 'file.12' 'temp.file' quiet'
     end
  say ''cr
  say' 'YELLOW'                 We have a winner!!!'cr
  say' 'RED' 'winner' is being added to ''The GangLord Hall of Fame!'''Clear''cr
  say ''cr
  signal reset
  end
 end

SAY YELLOW||center('-----------------------------------------',70)||CR
SAY RED||center('Top 10 Chicago Ganglords',70)||CR
SAY YELLOW||center('-----------------------------------------',70)||CR
SAY ''||CR
call open(top10,bbspath2'gangland/top10','W')
do i = 1 to 10
   say cyan||CENTER(LEFT(topalias.i,20) RIGHT(topscore.i,7),70)||CR
   call writeln(top10,topscore.i' 'topalias.i)
end
call close(top10)
SAY ''||CR
if quiting = 1 then do
 SAY ''||CR
 SAY ''||CR
 SAY '              'RED' Thank you for playing Chicago Gangland'||CR
 SAY ''||CR
 SAY '            'MAGENTA'Now returning you to the 'bbsname' 'CLEAR||CR
 SAY ''||CR
 CALL delay(100)
exit
end
call pressreturn
/*continue*/
signal main

news:
say CLSCREEN
q = 0
do i = 2 to 8
   select
      when i = 2 then SAY CYAN||CR
      when i = 3 then SAY RED||CR
      when i = 4 then SAY MAGENTA||CR
      when i = 5 then SAY BLUE||CR
      when i = 6 then SAY GREEN||CR
      when i = 7 then SAY YELLOW||CR
      otherwise SAY WHITE||CR
   end
   if exists(file.i) then call open(gamenews,file.i,'R')
   if exists(file.i) then do until EOF(gamenews)
      q = q + 1
      SAY Readln(gamenews)||CR
      if q // (LINES - 3) = 0 then do
         options prompt 'Hit RETURN to continue or Q to quit '
         pull ANS
         if ans = 'Q' then do
            call close(gamenews)
            signal main
         end
      end
   end
   call close(gamenews)
end
/*call pressreturn*/
/*continue*/
if i = 8 /*& eof(file.1) = 1 then do*/then signal main
 call pressreturn
 signal main
 end

instructions:
say CLSCREEN
SAY ''||CR
i = 1
call open(instr,file.1,'R')
do until EOF(instr)
   SAY Readln(instr)||CR
   if i // LINES = 0 
   then do
      call pressreturn
      /*continue*/
   end
   i = i + 1
end
call close(instr)
call pressreturn
/*continue*/
signal main

stats:
say CLSCREEN
SAY center(RED'-----------------------------------------',76)||CR
SAY center(YELLOW||Spacename'''s' 'STATISTICS',76)||CR
SAY center(RED'-----------------------------------------',76)||CR
SAY ''||CR
Say CYAN '   Alias: 'Alias''cr
Say CYAN '   Position: ' Street.startx 'North ' Street.starty 'East'CR
Say CYAN '   Total Gangmembers: 'totalgangmembers''CR
Say CYAN '   Gangmembers with you: 'gangmembers''CR
Say CYAN '   Money: 'money''CR
Say CYAN '   Bootleg Operations: 'bootleg''CR
SAY ''||CR
SAY CYAN 'Blocks protected:'||CR
i = 12
do c = 1 to 25
   do c1 = 1 to 25
      protect_alias = Delword(protect.c.c1,1,1)
      if protect_alias = alias then do
         SAY MAGENTA street.c 'North' street.c1 'East with' word(protect.c.c1,1) 'gangmembers'||CR
         if i // LINES - 1 = 0 then call pressreturn
         i = i + 1
      end
   end
end
call pressreturn
/*continue*/
signal main

reset:
 say ''cr
 say ' Please standby, the game is being reset!'cr
 say ''cr
 address command 'c:delete 'bbspath2'Gangland/#? quiet'
 backuppath = bbspath2'GangBackups/'

 address command 'c:copy 'backuppath'instructions 'file.1' quiet'
 address command 'c:copy 'backuppath'news6 'file.3' quiet'
 address command 'c:copy 'backuppath'news5 'file.4' quiet'
 address command 'c:copy 'backuppath'news4 'file.5' quiet'
 address command 'c:copy 'backuppath'news3 'file.6' quiet'
 address command 'c:copy 'backuppath'news2 'file.7' quiet'
 address command 'c:copy 'backuppath'news1 'file.8' quiet'
 address command 'c:copy 'backuppath'aliases 'file.13' quiet'
 address command 'c:copy 'backuppath'protect 'file.9' quiet'
 address command 'c:copy 'backuppath'top10 'file.14' quiet'
 address command 'c:copy 'temp.file' 'file.12' quiet'
 call delete(temp.file)
    call open(news7,file.2,'W')
    call writeln(news7,'******************************************************')
    call writeln(news7,'                THE CHICAGO DAILY NEWS')
    call writeln(news7,'                     'datec)
    call writeln(news7,'******************************************************')
    call writeln(news7,' ')
    call writeln(news7,' GangLord 'winner' wins the game with a score of 'winscore'!')
    call writeln(news7,' The game has been reset!')
    call close(news7)

 mess = ''winner' has won the game, and the game has been reset!'
 call open(fnx,file.11,'W');call writeln(fnx,mess);call close(fnx)

 say ''cr
 say' The game is now reset!'cr
 say' Play again to be the first gangster in the new game!'cr
 say ''cr
 exit


death:
say CLSCREEN
SAY center(RED'You have died 'SpaceName'.',76)||CR
SAY ''||CR
Say CYAN'Clearing your stats..'CR
do c = 1 to 25
   CALL WRITECH(STDOUT,'.')
   do c1 = 1 to 25
      protect_alias = Delword(protect.c.c1,1,1)
      if protect_alias = alias then
      protect.c.c1 = 0  
   end
end
/*delete(bbspath2'gangland/'Name)     */
address command 'c:delete 'bbspath2'gangland/'Name' quiet'
totalgangmembers = 0
protection = 0
deathflag = 1
signal goodbye

goodbye:
say CLSCREEN
Say CYAN'Saving game stats..'cr
call open(aliases,bbspath2'gangland/aliases','W')
u = 1
do i = 1 to marker - 1
   if name' 'alias ~= alias1.u then
   call writeln(aliases,alias1.u)
   u = u + 1
end
if deathflag = 0 then call writeln(aliases,name' 'alias)
call close(aliases)
call open(lastpos,file.10,'W')
q = 1
if pos ~= 0 then do i = 1 to pos
   if alias ~= posalias.q then
   call writeln(lastpos,mypos.q)
   q = q + 1
end
if deathflag = 0 then call writeln(lastpos,startx' 'starty' 'gangmembers' 'money' 'alias)
call close(lastpos)
call open(prot,File.9,'W')
do c = 1 to 25
   CALL WRITECH(STDOUT,'.')
   do c1 = 1 to 25
      call writeln(prot,protect.c.c1)
   end
end
call writeln(prot,datec)
call close(prot)
pointsg = totalgangmembers * 100
pointsp = protection * 1000
points = pointsg + pointsp
call open(top10,bbspath2'gangland/top10','R')
topalias.1 = alias
topscore.1 = points
do i = 2 to 11
   tops = readln(top10)
   topalias.i = delword(tops,1,1)
   topscore.i = word(tops,1)
   if topalias.1 = topalias.i then topscore.i = -1
end
call close(top10)
do i = 1 to 10
   do h = i + 1 to 11
      if topscore.i < topscore.h then do
         temp = topscore.i
         topscore.i = topscore.h
         topscore.h = temp
         tempor = topalias.i
         topalias.i = topalias.h
         topalias.h = tempor
      end
   end
end
call open(top10,bbspath2'gangland/top10','W')
do i = 1 to 10
   call writeln(top10,topscore.i' 'topalias.i)
end
call close(top10)
if exists(bbspath2'gangland/'Name) then do
   call open(stats,bbspath2'gangland/'Name,'W')
   call writeln(stats,alias)
   call writeln(stats,turnsleft)
   call writeln(stats,gangmembers)
   call writeln(stats,datec)
   call writeln(stats,startx)
   call writeln(stats,starty)
   call writeln(stats,points)
   call writeln(stats,money)
   call writeln(stats,totalgangmembers)
   call writeln(stats,protection)
   call writeln(stats,bootleg)
   call close(stats)
end
say CLSCREEN
call oneliner
quiting = 1
call highscores
SAY ''||CR
SAY ''||CR
SAY '              'RED' Thank you for playing Chicago Gangland'||CR
SAY ''||CR
SAY '            'MAGENTA'Now returning you to the 'bbsname' 'CLEAR||CR
CALL delay(100)
exit

ONELINER:
say ''cr
options prompt ''CYAN' Do you want to leave a message for the next Gangster (y/N)?'CLEAR' > '

pull result;ans=upper(result)
if ans~='Y' then do
  say ''YELLOW'  No'CLEAR''cr
  return
  end
say ''WHITE'  Yes'CLEAR''cr;say ''cr
options prompt''GREEN' Enter a ONE LINE message:'CLEAR' ';say ''cr

pull result; mess=result
if length(mess)<10 then do
  say ''YELLOW'Message too short, Entry Aborted.'CLEAR''cr;signal ONELINER
  end
say ''cr
say ''longdashes''cr
say ''MAGENTA''mess ''alias''CLEAR''cr
say ''longdashes''cr
say ''cr
options prompt ''BLUE' Does this look OK?'CLEAR' > '


pull result;ans=upper(result)
if ans='N' then do
  say ' Nope!'cr;signal ONELINER
  end
say ''MAGENTA' Yep!'CLEAR''cr
if exists(file.11) then do
  address command 'c:delete 'file.11' quiet'
  end
call open(fnx,file.11,'W');call writeln(fnx,mess);call writeln(fnx,alias);call close(fnx)
return


quickexit:
   say CLSCREEN
   call oneliner
   quiting = 1
   call highscores
   SAY ''||CR
   SAY ''||CR
   SAY '              'RED' Thank you for playing Chicago Gangland'||CR
   SAY ''||CR
   SAY '            'MAGENTA'Now returning you to 'bbsname' 'CLEAR||CR
   CALL delay(100)
   exit

pressreturn:
say ''cr
options prompt'  Press return to continue'
pull response
return

/************************* Error Checking *********************************/

syntax:
   SAY '*BOOM*  Got a syntax error.  PLEASE notify the sysop.'||CR
   SAY 'Line:' SIGL '   Error Code:' RC||CR
exit

checkBBS:
IF ADDRESS()~='BAUD' THEN RETURN 0
IF TIME('e')>secs THEN SIGNAL goodbye
dcd
IF RC=0 THEN SIGNAL goodbye
temp=secs-TIME('E')
IF temp<120 THEN SAY '*** Only' temp 'seconds left! ***'CR 
RETURN 0


BREAK_C:
BREAK_E:
call goodbye
CALL CLOSE(f)
RETURN(count);
EXIT;                           /* a little redundant, so sue me! */

/**************************************************************************/