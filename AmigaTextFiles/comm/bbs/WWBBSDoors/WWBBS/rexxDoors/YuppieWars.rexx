/* YUPPIE WARS!  V2.6-bb4 22-Nov-90/21-Feb-91  -  Dave Cole */
/* Special thanks to Mike Soucy for the Slot Machine routine! */
/* Special thanks to D.F. Duck for the Craps routine! */
/* Special thanks to Jim Foley for a couple suggestions! & Mem-Safeguard!*/
/* Special thanks to ME for the rest of the routine */
/* This is my "Hello World" program so bare with me! */
/* For updates/questions/whatever call Wolverine's Den (616)533-6760 */
/* If anyone finds a nifty way to shorten the code call my board! */
/* Converted to Tempest BBS by Platinum Dragon  05/92 */
/* Converted to BBBBS or CLI by Richard Stockton 1-Nov-92 (bb2) */
/* Updated for BBBBS or CLI by Richard Stockton 8-Aug-93 (bb3) */
/* Updated for BBBBS or CLI by Richard Stockton 1-Sep-93 (bb4) */

options results

if ~show('L',"rexxsupport.library") then do
   if addlib('rexxsupport.library',0,-30,0) then nop
   else do
      say 'Hmm.. Cant seem to find the support library!'CR
      exit 10
   end
end

CALL TIME('R')
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E
CR='0D'x
CLS=d2c(12)
LINES=20
bbsname='the BBS '
bbspath='wwbbs:'

PARSE ARG handle winnings secondsflag gfx secs .
IF ~DATATYPE(secs,'N') THEN secs=3600


BBSIDENTIFY EMULATION
PARSE VAR RESULT type .
if type = "ANSI" then
  gfx=1
else
  gfx=0

BBSIDENTIFY USER
PARSE VAR RESULT Username From Acces .
handle = substr(Username,2,length(Username)-2)


IF handle='' THEN
  DO
    prompt( 'Please Enter Your Name > ')
    handle=readstr()
    handle=SPACE(handle,1,'_')
    prompt( 'Do You Want Color Graphics? (nY) > ')
    gfx=readstr()
    IF LEFT(gfx,1)~='N' THEN gfx=1
  END
IF gfx~=1 THEN gfx=0

BBSIDENTIFY SYSOP
sysop=result
bbsname='WWBBS'
bbspath='wwbbs:'

IF bbspath~='' THEN
  DO
    datapath=bbspath'rexxDoors/Data/Yuppie'
    CALL MAKEDIR(datapath)
  END
ELSE IF EXISTS('rexx:YuppieWars.rexx') THEN
  DO
    datapath='rexx:Yuppie'
    CALL MAKEDIR(datapath)
  END
IF datapath~='' THEN datapath=datapath'/'

CALL MESSAGE CLS
CALL MESSAGE "Loading Yuppie Wars..."

 /* Define a few variables... */

howm = 0       /* PUT HOW MANY PEOPLE ARE ALREADY PLAYING HERE!  VERY */
               /* IMPORTANT THAT YOU DO THIS! IF NONE THAN LEAVE AS 0. */
               /* (ONLY CHANGE THIS WHEN FIRST INSTALLING THE PROGRAM) */
             /* (WHEN OTHER USERS ARE ADDED THE PROGRAM WILL COMPENSATE) */
             /* ALSO!  ONLY edit this when "STATS" file is deleted! */

file.1=datapath'Instructions1';file.2=datapath'Instructions2';file.6=datapath'NEWS'
file.3=datapath'Instructions3';file.4=datapath'Instructions4';file.5=datapath'Instructions5'
lowmem = 150000
CALL NOCR "."
if gfx=1 then do
RED = '[31m';GRE = '[32m';YEL = '[33m';BLU = '[34m';MAG = '[35m';CYA = '[36m';WHI = '[37m'
WhiBlk = '[47;30m';BluCya = '[44;36m';WhiGre = '[47;32m';RedMag = '[41;35m';ITAL = '[3m';UNDER = '[4m';BOLD = '[1m';RESET = '[0m'
end;else do
RED = '';GRE = '';YEL = '';BLU = '';MAG = '';CYA = '';WHI = ''
WhiBlk = '';BluCya = '';WhiGre = '';RedMag = '';ITAL = '';UNDER = '';BOLD = '';RESET = ''
end
IF ADDRESS()~='BAUD' THEN
  DO
    BLU=GRE
    CR=''
  END
time=time(); date=date()
CALL NOCR "."
blanks = '                                 '
day = 0
if ~exists(datapath'NEWS') then do
   call open(news,datapath'NEWS','W')
         CALL WRITELN(news,' ')
         CALL WRITELN(news,'         'BLUCYA'   -=< The Yuppie Times >=-   'RESET' ')
         CALL WRITELN(news,' 'RED'               For 'date' ')
         CALL WRITELN(news,' 'CYA)
   call close(news)
  end
if ~exists(datapath'STATS') then do
   call open(dc,datapath'STATS','W')
   call writeln(dc,date)
   call writeln(dc,day)
   call writeln(dc,howm)
   call close(dc)
  end
if exists(datapath'STATS') then do
   call open(dc,datapath'STATS','R')
   dt = readln(dc)
   day = readln(dc)
   howm = readln(dc)
   if howm = ' ' | howm = '' then howm = 1
   call close(dc)
   if date ~= dt then do
      day = day + 1
      if exists(datapath'NEWS') then do
         call open(news,datapath'NEWS','A')
         CALL WRITELN(news,' ')
         CALL WRITELN(news,'         'BLUCYA'   -=< The Yuppie Times >=-   'RESET' ')
         CALL WRITELN(news,' 'RED'               For 'date' ')
         CALL WRITELN(news,' 'CYA)
         call close(news)
      end
   call open(dc,datapath'STATS','W')
   CALL WRITELN(dc,date)
   CALL WRITELN(dc,day)
   CALL WRITELN(dc,howm)
   call close(dc)
   end
end
CALL NOCR "."
if day = 5 then do
   day = 0
   call delete(file.6)
   call open(news,datapath'NEWS','W')
   CALL WRITELN(news,' ')
   CALL WRITELN(news,'         'BLUCYA'   -=< The Yuppie Times >=-   'RESET' ')
   CALL WRITELN(news,' 'RED'               For 'date' ')
   CALL WRITELN(news,' 'CYA)
   CALL WRITELN(news,'Newspaper was reset on 'date'.')
   call close(news)
   call open(dc,datapath'STATS','W')
   call writeln(dc,date)
   call writeln(dc,day)
   call writeln(dc,howm)
   call close(dc)
  end

if exists(datapath'NEWS') then do
   call open(news,datapath'NEWS','A')
   call writeln(news,handle' joins us at 'time', 'date'.')
  end

if ~exists(datapath'TopOnes') then do
   call open(top10,datapath'TopOnes','W')
   do i = 1 to 10
      call writeln(top10,'0' '..........')
   end
   call close(top10)
  end

if ~exists(datapath'PLAYERS') then do
   call open(play,datapath'PLAYERS','W')
   do i = 1 to howm
      call writeln(play,'0' '...........')
   end
   call close(play)
  end
CALL NOCR "."

/* WHAT THE WEAPONS ARE */
wep1 = "Squirt Gun"
wep2 = "Spitwad Launcher"
wep3 = "Frisbee"
wep4 = "Laser Tag Gun"
wep5 = "Paint Ball Gun"
wep6 = "Darts"
wep7 = "Baseball"
wep8 = "Bart Simpson Doll"
wep9 = "Chainsaw"
wep10 = "Teenage Mutant Ninja Turtle Doll"
wep11 = "New Kids On The Block Poster"
wep12 = "New Kids On The Block CD"
wep13 = "Atomic Can-Opener"
wep14 = "Guile"
     /* WHAT THE VEHICLES ARE */
arm1 = "Barbie Ferrari"
arm2 = "Roller Skates"
arm3 = "Vision Skateboard"
arm4 = "Go-Cart"
arm5 = "10-Speed Bike"
arm6 = "Suzuki Motorbike"
arm7 = "BMW Motorcycle"
arm8 = "Yugo"
arm9 = "Volkswagon"
arm10 = "Ferrari F40"
arm11 = "Corvette Mako Shark"
arm12 = "Stealth Bomber"
arm13 = "Nuclear Submarine"
arm14 = "Starship USS Enterprise"
     /* THE RANKS */
ran1 = "Window_Shopper"
ran2 = "Browser"
ran3 = "Consumer"
ran4 = "Credit_Card_User"
ran5 = "Bouncy_Check_Writer"
ran6 = "Blue_Collar_Worker"
ran7 = "Republican"
ran8 = "Millionare_Tycoon"
ran9 = "Chairman_of_Macy's_Department_Store"
ran10 = "President_of_Macy's_Department_Store"
ran11 = "Founder_of_Macy's_Department_Store"
ran12 = "THE_ONE."
ran13 = "MORE_THAN_THE_ONE."
ran14 = "LEADER_OF_MORE_THAN_THE_ONE."
    /* BEGINNING GUY STATS */
turnsleft = 26
maxturns = 26
maxpoints = 30
hitpoints = 30
points = 6100
money = 5000
armor = arm1
weapon = wep1
rank = ran1
canfight = 4
deadf = 0
killer = "NOBODY"
kills = 0
bmoney = 1000
CALL NOCR "."
    /* LETS BEGIN THE GAME! */
if ~exists(datapath||handle'.dat') then do
   howm = howm + 1
  end
if exists(datapath||handle'.dat') then do
   call open(File,datapath||handle'.DAT','R')
   turnsleft = readln(File)
   maxpoints = readln(File)
   hitpoints = readln(File)
   points = readln(File)
   money = readln(File)
   armor = readln(File)
   weapon = readln(File)
   rank = readln(File)
   canfight = readln(File)
   lastpl = readln(File)
   deadf = readln(File)
   killer = readln(File)
   kills = readln(File)
   bmoney = readln(File)
  end
call close(File)
CALL NOCR "."
if lastpl ~= date then do
   CALL MESSAGE MAG"Running Maintenance...."
   call delay(20)
   CALL MESSAGE YEL"  You have "WHI"25"YEL" fresh turns!"
   turnsleft = 26
   rannum = RANDOM(10,50,Time('s'))
   hitpoints = hitpoints + rannum
   if hitpoints > maxpoints then hitpoints = maxpoints
   CALL MESSAGE YEL"  You have gained "WHI||rannum||YEL" hitpoints!"
   money = money + 2000
   CALL MESSAGE YEL"  You have earned "WHI"$2000"YEL"!"
   CALL MESSAGE RED"  You are all set up and ready to go!"RESET
   canfight = 4
   CALL MESSAGE " "
   if deadf = 1 then do
      CALL MESSAGE RED" "KILLER" BEAT YOU ON THE HIGHWAY!"
      CALL MESSAGE " "
   end
   deadf = 0
   call delay(100)
   prompt( RESET"Press "RED"RETURN"RESET" when finished: "RESET)
   junk=readstr()
   if checkBBS() then signal BYE
  end
call CHECKRANK
call EVENER
signal TITLE

CHECKRANK:
if kills >= 17 then rank = ran2
if kills >= 25 then rank = ran3
if kills >= 35 then rank = ran4
if kills >= 45 then rank = ran5
if kills >= 60 then rank = ran6
if kills >= 75 then rank = ran7
if kills >= 100 then rank = ran8
if kills >= 130 then rank = ran9
if kills >= 160 then rank = ran10
if kills >= 215 then rank = ran11
if kills >= 300 then rank = ran12
if kills >= 444 then rank = ran13
if kills >= 999 then rank = ran14
if rank = ran2 then maxpoints = 40
if rank = ran3 then maxpoints = 50
if rank = ran4 then maxpoints = 60
if rank = ran5 then maxpoints = 75
if rank = ran6 then maxpoints = 90
if rank = ran7 then maxpoints = 100
if rank = ran8 then maxpoints = 120
if rank = ran9 then maxpoints = 140
if rank = ran10 then maxpoints = 170
if rank = ran11 then maxpoints = 200
if rank = ran12 then maxpoints = 250
if rank = ran13 then maxpoints = 310
if rank = ran14 then maxpoints = 380
return

EVENER:
zod1 = compress(hitpoints,'0123456789')
zod2 = compress(points,'0123456789')
zod3 = compress(money,'0123456789')
zod4 = compress(bmoney,'0123456789')
if zod1 = '.' then do
 parse var hitpoints hit1 '.' hit2
 hitpoints = hit1
 hitpoints = hitpoints + 1
end
if zod2 = '.' then do
 parse var points poi1 '.' poi2
 points = poi1
 points = points + 1
end
if zod3 = '.' then do
 parse var money moy1 '.' moy2
 money = moy1
 money = money + 1
end
if zod4 = '.' then do
 parse var bmoney bmy1 '.' bmy2
 bmoney = bmy1
 bmoney = bmoney + 1
end
return

TITLE:
CALL MESSAGE RESET CLS
CALL MESSAGE " "
CALL MESSAGE " "
CALL MESSAGE " "
CALL MESSAGE RED center("LONG AGO, IN A SHOPPING MALL FAR FAR AWAY...",78)
CALL MESSAGE BLU "                          ___  ___    ___         __  ___  ___"
CALL MESSAGE BLU "               /  / /  / /  / /  / / /      \   \ \ \ \  \ \"
CALL MESSAGE RED "        ------"BLU"/__/"RED"-"BLU"/"RED"--"BLU"/"RED"-"BLU"/__/"RED"-"BLU"/__/"RED"-"BLU"/"RED"-"BLU"/_"RED"-------"BLU"\"RED"---"BLU"\"RED"-"BLU"\_\"RED"-"BLU"\__\"RED"-"BLU"\__"RED"------"RESET
CALL MESSAGE RED "       ---------"BLU"/"RED"-"BLU"/"RED"--"BLU"/"RED"-"BLU"/"RED"----"BLU"/"RED"----"BLU"/"RED"-"BLU"/"RED"----------"BLU"\"RED"-"BLU"\"RED"-"BLU"\"RED"-"BLU"\"RED"-"BLU"\"RED"-"BLU"\"RED"-"BLU"\"RED"----"BLU"\"RED"------"
CALL MESSAGE BLU "             __/ /__/ /    /    / /___         \_\_\ \ \ \ \  __\"
CALL MESSAGE CYA center("Version 2.6-bb4",78)
CALL MESSAGE " "
CALL MESSAGE GRE center("By Dave Cole",78)
CALL MESSAGE GRE center("Wolverine's Den - (616)533-6760",78)
CALL MESSAGE " "
CALL MESSAGE GRE center("Special thanks to Mike Soucy and D.F. Duck for the help!",78)
CALL MESSAGE MAG "                        Now playing on"YEL bbsname||MAG"!"  /* You may want to center this appropriately... */
call delay(100)
prompt( RED center("--= Press Return =-- ",78)||RESET)
junk=readstr()
if checkBBS() then signal BYE
signal BEGINNINGPROMPTS

BEGINNINGPROMPTS:
CALL MESSAGE " "
CALL MESSAGE " "
prompt( MAG"Need Instructions ("GRE"y"MAG"/"RED"N"MAG")? "RESET)
info=readstr()
if checkBBS() then signal BYE
 if info = "Y" then do
  do i = 1 to 5
     CALL MESSAGE CLS
     call open(instr,file.i,'R')
     do until EOF(instr)
        CALL MESSAGE Readln(instr)
     end
     call close(instr)
     prompt( " Press "RED"RETURN"WHI" when finished: "RESET)
     junk=readstr()
     if checkBBS() then signal BYE
     CALL MESSAGE " "
    end
 end
CALL MESSAGE " "
prompt( MAG"Read The News ("GRE"y"MAG"/"RED"N"MAG")? "RESET)
newz=readstr()
if checkBBS() then signal BYE
if newz = "Y" then do
  q = 0
  CALL MESSAGE CLS
  x=open(newz,datapath'NEWS','R')
  IF x~=0 THEN do until EOF(newz)
     q = q + 1
     CALL MESSAGE GRE||Readln(newz)
     if q // (LINES - 3) = 0 then do
        prompt( RESET"Hit "RED"RETURN"RESET" to continue: "RESET)
        ans=readstr()
        if checkBBS() then signal BYE
        if ans = "Q" then do
           call close(newz)
           signal MENU
        end
     end
  end
  call close(newz)
  prompt( RESET"Press "RED"RETURN"RESET" when finished: "RESET)
  junk=readstr()
  if checkBBS() then signal BYE
  signal MENU
 end
CALL MESSAGE " "
signal MENU

MAINGAME:
currentmem=storage()
  if currentmem<lowmem then do
    CALL MESSAGE CLS
    CALL MESSAGE " "
    CALL MESSAGE RED CENTER("Sorry!  Out of memory!  Saving and exiting...",78)
    call delay(190)
    signal BYE
  end
CALL MESSAGE " "
prompt( BLU"["YEL||turnsleft||BLU"]"CYA" Your Choice?"BLU"> "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice="" THEN turnsleft=turnsleft+1
if Choice=" " | Choice="  " | Choice="    " | Choice="        " then
  if turnsleft<25 THEN turnsleft=turnsleft+LENGTH(Choice)
if Choice = "T" then signal ARENA
if Choice = "H" then signal HIGHWAY
if Choice = "R" then signal REST
if Choice = "S" then signal TOP
if Choice = "Y" then signal YOURSTATS
if Choice = "G" then signal TRIP
if Choice = "L" then signal VEGAS
if Choice = "V" then signal BUYORSELLA
if Choice = "W" then signal BUYORSELLW
if Choice = "B" then signal BANK
if Choice = "P" then signal PLAYERS                                                                  ; if (handle=sysop) & (Choice=8) then signal LALA
if Choice = "Q" then signal QUIT
if Choice = "?" then signal MENU
else do
signal MENU
end
signal MENU

MENU:
if hitpoints > maxpoints then hitpoints = maxpoints
if hitpoints < 0 then hitpoints = 0
if money < 0 then money = 0
turnsleft=turnsleft-1
if turnsleft <= 0 then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE center("        "BLUCYA"   You must like this game!   "RESET,78)
   CALL MESSAGE " "RESET
   CALL MESSAGE RED center("But anyway, you have played all your 25 turns out.",78)
   CALL MESSAGE RED center("Try calling back tomarrow and playing again!",78)
   CALL MESSAGE " "
   prompt( RESET"Press "RED"RETURN"WHI" when finished: "RESET)
   junk=readstr()
   signal OUTTAHERE
  end
CALL MESSAGE CLS
CALL MESSAGE BLU"                         _  _        _    _"
CALL MESSAGE RED"          ==============="BLU"\\//"RED"========"BLU"\\/\//"RED"==============="
CALL MESSAGE RED"         ================="BLU"||UPPIE"RED"====="BLU"\/\/ARS"RED"=============="
CALL MESSAGE BLU"       --------"CYA" Can The Richest & Greediest Survive?"BLU" --------"
CALL MESSAGE " "
CALL MESSAGE "           "MAG"["YEL"T"MAG"]"WHI" To The Arena              "MAG"["YEL"G"MAG"]"WHI" Go On A Trip      "
CALL MESSAGE "           "MAG"["YEL"H"MAG"]"WHI" The Highway               "MAG"["YEL"L"MAG"]"WHI" Go To Las Vegas   "
CALL MESSAGE "           "MAG"["YEL"R"MAG"]"WHI" Rest In A Hotel           "MAG"["YEL"V"MAG"]"WHI" Shop For Vehicle  "
CALL MESSAGE "           "MAG"["YEL"S"MAG"]"WHI" Top Scores                "MAG"["YEL"W"MAG"]"WHI" Shop For Weapons  "
CALL MESSAGE "           "MAG"["YEL"Y"MAG"]"WHI" List Your Stats           "MAG"["YEL"P"MAG"]"WHI" List Players      "
CALL MESSAGE "           "MAG"["YEL"B"MAG"]"WHI" The Bank                  "MAG"["YEL"Q"MAG"]"WHI" Quit The Game     "
CALL MESSAGE " "
signal MAINGAME

PLAYERS:
CALL MESSAGE CLS
turnsleft = turnsleft + 1
call HICL
call SHOWPL
signal MENU

HICL:
call open(play,datapath'Players','R')
PHandle.1 = handle
PRank.1 = rank
do i = 2 to howm+1
   plays = readln(play)
   PHandle.i = delword(plays,1,1)
   PRank.i = word(plays,1)
   if PHandle.1 = PHandle.i then PRank.i = -1
end
call close(play)
do i = 1 to howm
   do h = i + 1 to howm+1
      if PRank.i < PRank.h then do
         tp = PRank.i
         PRank.i = PRank.h
         PRank.h = tp
         tpor = PHandle.i
         PHandle.i = PHandle.h
         PHandle.h = tpor
      end
   end
end
return

SHOWPL:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE center("       "BLUCYA"   List Of Yuppies   "RESET,78)
CALL MESSAGE BLU center("------------------------------------------------------",73)RESET
CALL MESSAGE " "
call open(play,datapath'Players','W')
do i = 1 to howm
   if PRank.i ~= -1 then do
    CALL MESSAGE BLU right(PHandle.i,20,' -- ') ' ' PRank.i
    call writeln(play,PRank.i' 'PHandle.i)
   end
end
call close(play)
q = 0
do i = 1 to howm
 q = q + 1
 if q // (LINES - 3) = 0 then do
 CALL MESSAGE RESET"Press "RED"RETURN"WHI" to continue: "BLU
 ans=readstr()
 if ans = "Q" then signal MENU
end
CALL MESSAGE " "
prompt( RESET"Press "RED"RETURN"WHI" when finished: "RESET)
junk=readstr()
if checkBBS() then signal BYE
signal MENU

BANK:
call EVENER
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE " "
CALL MESSAGE GRE"                  Due North-Northeast Savings and Loan Terminal"
CALL MESSAGE " "
CALL MESSAGE WHI"                       Account#:"RED" 0993-24300"handle"Z883"
CALL MESSAGE WHI"                   Pocket Money:"RED money
CALL MESSAGE WHI"                     Bank Money:"RED bmoney
CALL MESSAGE " "
CALL MESSAGE " "RESET
prompt( BLU"["YEL"D"BLU"]"CYA" Deposit, "BLU"["YEL"W"BLU"]"CYA" Withdraw, "BLU"["YEL"L"BLU"]"CYA" Leave Bank"BLU"> "RESET)
ans=readstr()
if checkBBS() then signal BYE
if ans = "D" then signal DEPO
if ans = "W" then signal WITH
if ans = "Q" then signal MENU
if ans = "L" then signal MENU
signal BANK

WITH:
CALL MESSAGE " "
prompt( GRE"Withdraw How Much Money?"BLU"> "RESET)
wit=readstr()
if checkBBS() then signal BYE
if wit > bmoney | wit < 0 then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("You don't have that much money in the bank!",78)
   CALL MESSAGE " "
   call delay(130)
   signal BANK
  end
if wit <= bmoney then do
   bmoney = bmoney - wit
   money = money + wit
   CALL MESSAGE RED"Money withdrawn.  Thank you."
   call delay(100)
   signal BANK
  end
signal BANK

DEPO:
CALL MESSAGE " "
prompt( GRE"Deposit How Much Money?"BLU"> "RESET)
dep=readstr()
if checkBBS() then signal BYE
if dep > money | dep < 0 then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU DON'T HAVE THAT MUCH MONEY!",78)
   CALL MESSAGE " "RESET
   call delay(110)
   signal BANK
  end
if dep <= money then do
   CALL MESSAGE RED"Depositing..."
   money = money - dep
   bmoney = bmoney + dep
   CALL MESSAGE RED"Deposited."
   call delay(100)
   signal BANK
  end
signal BANK

ARENA:
call EVENER
CALL MESSAGE CLS
CALL MESSAGE WHI"  .                         .   "BLU" ___"WHI"           ."
CALL MESSAGE BLU"    ___ "WHI".   "BLU" __________   "WHI". "BLU"  __|___|_   "WHI".   "BLU"__"
CALL MESSAGE BLU"   |...|_   |o\\\\\\\\\\__   |. . . . |    _/__|_"
CALL MESSAGE BLU"   |._.|.|  |oo\\\\\\\\\\.|"WHI"."BLU" |. . . . |   |....._\  "WHI" ."
CALL MESSAGE WHI"___"BLU"||_||"WHI"_"BLU"|"WHI"__"BLU"|"WHI"___"BLU"\\\\\\\\\\|"WHI"__"BLU"|________|"WHI"___"BLU"|_____\_\"WHI"_____"
CALL MESSAGE BLU"________________________________________________________"
CALL MESSAGE GRE"__                                                    "GRE"__"
CALL MESSAGE GRE"|\\"CYA"OOOooooo,,,,,,,.........."BLU"__"CYA".......,,,,,ooooooooOOO"GRE"// |"
CALL MESSAGE GRE"|  \\"CYA"OOOooooo,,,,,,,......."BLU"/__\"CYA"....,,,,,ooooooooOOO"GRE"//   |"
CALL MESSAGE GRE"|___"RED"/==/______"MAG"___"CYA",,,,,......"YEL"_____________"CYA"oooooOOO"GRE"//_____|"
CALL MESSAGE BLU" __ "CYA") _  .... "MAG"\__\ "BLU"_______ "WHI"/_)"YEL"/\___/     \ "BLU"______________"
CALL MESSAGE BLU" -= "RED"-'"CYA"O"RED"`------'"CYA"O"RED"-'"BLU" -=-=-=- "YEL"`---"WHI"O"YEL"-------"WHI"O"YEL"-' "BLU"-=-=-=-=-=-=-"
CALL MESSAGE BLU" -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="
CALL MESSAGE RED"                                  __    __"
CALL MESSAGE RED"          Below the streets...     \HE  \-\RENA"
CALL MESSAGE " "
prompt( CYA"Enter the arena? ("GRE"Y"CYA"/"RED"n"CYA")"BLU"> "RESET)
answ=readstr()
if checkBBS() then signal BYE
if answ = "N" then signal MENU
signal FIGHTWHO

FIGHTWHO:
randm = RANDOM(1,3,Time('s'))
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE center("      "BLUCYA"   Today's Fighters Available:   "RESET,78)
CALL MESSAGE " "
if randm = 1 then do
   CALL MESSAGE center("[W] A Window Shopper      ",78)
   CALL MESSAGE center("[R] A Record Store Owner  ",78)
   CALL MESSAGE center("[A] An Arcade Game Buff   ",78)
   CALL MESSAGE center("[G] The Jolly Green Giant ",78)
  end
if randm = 2 then do
   CALL MESSAGE center("[W] A Window Shopper      ",78)
   CALL MESSAGE center("[A] An Arcade Game Freak  ",78)
   CALL MESSAGE center("[M] A Mortitioner         ",78)
   CALL MESSAGE center("[Y] Macy's Store President",78)
  end
if randm = 3 then do
   CALL MESSAGE center("[K] A K-Mart Attendant    ",78)
   CALL MESSAGE center("[R] A Record Store Owner  ",78)
   CALL MESSAGE center("[A] An Arcade Game Freak  ",78)
   CALL MESSAGE center("[P] A Paris Cafe Owner    ",78)
  end
CALL MESSAGE " "
prompt( CYA"Fight which one? "BLU"["YEL"Q"BLU"]"CYA" Quits"BLU"> "RESET)
answ=readstr()
if checkBBS() then signal BYE
if answ = "Q" then signal MENU
if randm = 1 then do
   if answ = "W" then signal WINDOW
   if answ = "R" then signal RECORD
   if answ = "A" then signal BUFF
   if answ = "G" then signal GIANT
  end
if randm = 2 then do
   if answ = "W" then signal WINDOW
   if answ = "A" then signal FREAK
   if answ = "M" then signal MORT
   if answ = "Y" then signal PRES
  end
if randm = 3 then do
   if answ = "K" then signal KMART
   if answ = "R" then signal RECORD
   if answ = "A" then signal FREAK
   if answ = "P" then signal PARIS
  end
signal FIGHTWHO

WINDOW:
CALL MESSAGE CLS
wh = "Window Shopper"
hitoppo = 30
wepoppo = wep2
armoppo = arm2
call RFZ
monoppo = randm * 50
signal FIGHTSC

RECORD:
CALL MESSAGE CLS
wh = "Record Store Owner"
hitoppo = 55
wepoppo = wep4
armoppo = arm4
call RFZ
monoppo = randm * 350
signal FIGHTSC

RFZ:
randm = RANDOM(10,30,Time('s'))
return

BUFF:
CALL MESSAGE CLS
wh = "Arcade Game Buff"
hitoppo = 70
wepoppo = wep5
armoppo = arm6
call RFZ
monoppo = randm * 400
signal FIGHTSC

GIANT:
CALL MESSAGE CLS
wh = "Jolly Green Giant"
hitoppo = 200
wepoppo = wep8
armoppo = arm8
call RFZ
monoppo = randm * 1200
signal FIGHTSC

FREAK:
CALL MESSAGE CLS
wh = "Arcade Game Freak"
hitoppo = 80
wepoppo = wep6
armoppo = arm7
call RFZ
monoppo = randm * 500
signal FIGHTSC

MORT:
CALL MESSAGE CLS
wh = "Mortitioner"
hitoppo = 180
wepoppo = wep8
armoppo = arm10
call RFZ
monoppo = randm * 1050
signal FIGHTSC

PRES:
CALL MESSAGE CLS
wh = "President Of Macy's"
hitoppo = 390
wepoppo = wep11
armoppo = arm11
call RFZ
monoppo = randm * 2500
signal FIGHTSC

KMART:
CALL MESSAGE CLS
wh = "K-Mart Attendant"
hitoppo = 52
wepoppo = wep4
armoppo = arm3
call RFZ
monoppo = randm * 300
signal FIGHTSC

PARIS:
CALL MESSAGE CLS
wh = "Paris Cafe Owner"
hitoppo = 180
wepoppo = wep9
armoppo = arm7
call RFZ
monoppo = randm * 1250
signal FIGHTSC

FIGHTSC:
CALL MESSAGE " "
CALL MESSAGE " "
CALL MESSAGE MAG"You are fighting a "WHI||wh||MAG"."
CALL MESSAGE " "
prompt( CYA"Fight Commands: "BLU"["YEL"A"BLU"] "WHI"Attack, "BLU"["YEL"R"BLU"] "WHI"Retreat, "BLU"["YEL"H"BLU"]"WHI" Hit his vehicle"BLU"> "RESET)
opt=readstr()
if checkBBS() then signal BYE
if opt = "A" then signal ATT
if opt = "R" then signal RET
if opt = "H" then signal RAMH
if opt = "Q" then signal RET
signal FIGHTSC

ATT:
call STA
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE RED"You hit him for "hithim" hitpoints!"
   hitoppo = hitoppo - hithim
   if hitoppo <= 0 then signal WONFIT
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSFIT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He rammed your "armor" and"
      CALL MESSAGE GRE"hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      hitoppo = hitoppo - ramlos
      if hitpoints <= 0 & hitoppo <= 0 then signal DRAWFIT
      if hitpoints <= 0 then signal LOSFIT
      if hitoppo <= 0 then signal WONFIT
     end
  end
if rannum = 2 then do
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSFIT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He rammed your "armor" and"
      CALL MESSAGE GRE"hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      hitoppo = hitoppo - ramlos
      if hitpoints <= 0 & hitoppo <= 0 then signal DRAWFIT
      if hitpoints <= 0 then signal LOSFIT
      if hitoppo <= 0 then signal WONFIT
     end
   CALL MESSAGE " "
   CALL MESSAGE RED"You hit him for "hithim" hitpoints!"
   hitoppo = hitoppo - hithim
   if hitoppo <= 0 then signal WONFIT
  end
signal FIGHTSC

RAMH:
call STA
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE RED"You rammed his "armoppo" and"
   CALL MESSAGE RED"hit him for "hithim+5" hitpoints!"
   CALL MESSAGE RED"You also lost "ramlos" by hitting him."
   hitpoints = hitpoints - ramlos
   hitoppo=hitoppo-(hithim+5)
   if hitpoints <= 0 & hitoppo <=0 then signal DRAWFIT
   if hitoppo <= 0 then signal WONFIT
   if hitpoints <= 0 then signal LOSFIT
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSFIT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He rammed your "armor" and"
      CALL MESSAGE GRE"hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      hitoppo = hitoppo - ramlos
      if hitpoints <= 0 & hitoppo <= 0 then signal DRAWFIT
      if hitpoints <= 0 then signal LOSFIT
      if hitoppo <= 0 then signal WONFIT
     end
  end
if rannum = 2 then do
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE GRE"He hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSFIT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE GRE"He rammed your "armor" and"
      CALL MESSAGE GRE"hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      hitoppo = hitoppo - ramlos
      if hitpoints <= 0 & hitoppo <= 0 then signal DRAWFIT
      if hitpoints <= 0 then signal LOSFIT
      if hitoppo <= 0 then signal WONFIT
     end
   CALL MESSAGE " "
   CALL MESSAGE RED"You rammed his "armoppo" and"
   CALL MESSAGE RED"hit him for "hithim+5" hitpoints!"
   CALL MESSAGE RED"You also lost "ramlos" by hitting him."
   hitpoints = hitpoints - ramlos
   hitoppo=hitoppo-(hithim+5)
   if hitpoints <= 0 & hitoppo <=0 then signal DRAWFIT
   if hitoppo <= 0 then signal WONFIT
   if hitpoints <= 0 then signal LOSFIT
  end
signal FIGHTSC


RET:
call STA
CALL MESSAGE " "
CALL MESSAGE RED"YOU RUN!"
if rank = ran1 | rank = ran2 | rank = ran3 then rannum = RANDOM(1,2,Time('s'))
if rank = ran4 | rank = ran5 | rank = ran6 then rannum = RANDOM(1,3,Time('s'))
if rank = ran7 | rank = ran8 | rank = ran9 then rannum = RANDOM(1,4,Time('s'))
if rank = ran10 | rank = ran11 then rannum = RANDOM(1,5,Time('s'))
if rank = ran12 | rank = ran13 | rank = ran14 then rannum = RANDOM(1,6,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE GRE"`I don't think so.' says the "wh"."
   CALL MESSAGE GRE"You can't escape from me right now!"
  end
if rannum = 2 | rannum = 3 | rannum = 4 | rannum = 5 | rannum = 6 then do
   ex = RANDOM(51,122,Time('s'))
   CALL MESSAGE " "
   CALL MESSAGE GRE"You ESCAPED from The Arena!"
   CALL MESSAGE " "
   call delay(110)
   signal MENU
  end
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE GRE"The "wh" hits you for "hityou" hitpoints!"
   hitpoints = hitpoints - hityou
   if hitpoints <= 0 then signal LOSFIT
  end
if rannum = 2 then do
   CALL MESSAGE " "
   CALL MESSAGE GRE"The "wh" misses you!"
  end
CALL MESSAGE " "
call delay(20)
signal FIGHTSC

WONFIT:
kills = kills + 1
call delay(140)
CALL MESSAGE CLS
CALL MESSAGE RESET"               *               *"
CALL MESSAGE "   *                       "YEL"|"RESET"            *"
CALL MESSAGE "        *      "YEL"\/OU \/\/ON o"
CALL MESSAGE "               /    "RESET"*             *"
CALL MESSAGE " *                      *               *"
CALL MESSAGE " "
CALL MESSAGE RED"  You Won!  "MAG" You Get "WHI"$"monoppo||MAG"!"
money = money + monoppo
call CHECKRANK
call delay(160)
signal MENU

LOSFIT:
hitpoints = 0
call delay(130)
CALL MESSAGE CLS
CALL MESSAGE RED"                                   _ ___"
CALL MESSAGE "              \_/ /\ | |    |  /\ /   |  |||"
CALL MESSAGE "               |  \/ |_|    |_ \/ _\  |  ooo"
CALL MESSAGE " "
CALL MESSAGE MAG"         You are the disgrace of The Arena!"
CALL MESSAGE " "
CALL MESSAGE GRE" You have 0 hitpoints."
CALL MESSAGE CYA" Oh well, better luck next time!"
call delay(150)
signal MENU

DRAWFIT:
kills = kills + 1
hitpoints = 0
call delay(150)
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED center("AMAZING!",78)
CALL MESSAGE " "
CALL MESSAGE MAG center("Both of you killed each other with your final blows!",78)
CALL MESSAGE MAG center("You both have 0 hitpoints, you didn't win anything.",78)
CALL MESSAGE " "
call CHECKRANK
call delay(200)
signal MENU

HIGHWAY:
call EVENER
CALL MESSAGE CLS
CALL MESSAGE WHI"   ________ "BLU"____"WHI" _ "BLU"__"WHI" ______ "BLU"_"WHI" _______ "BLU"__"WHI" __________"
CALL MESSAGE YEL"   ___   ___ "BLU"/"YEL"_"BLU"/_//_"YEL"_  _"BLU"/_///_"YEL"_"BLU"/_//"YEL"_"BLU" //_//_/"YEL"__   ___"
CALL MESSAGE WHI"   _________"BLU"/"WHI"_"BLU"/"WHI"_"BLU"//__"WHI"___"BLU"/"WHI"_"BLU"///_//"WHI"_"BLU"///|//"WHI"_"BLU"/"WHI"_"BLU"/"WHI"__________"RESET
CALL MESSAGE " "
prompt( CYA"Do you wish to fight another player ("GRE"Y"CYA"/"RED"n"CYA")? "RESET)
ans=readstr()
if checkBBS() then signal BYE
if ans = "Q" then signal MENU
if ans = "N" then signal MENU
CALL MESSAGE " "
CALL MESSAGE " "
prompt( GRE"Enter your opponents name: "RESET)
who=readstr()
if checkBBS() then signal BYE
if who = "Q" then signal MENU
if who = "" | who = " " | who = "  " then signal MENU
if exists(datapath||who'.DAT') then do
   call open(oppo,datapath||who'.DAT','R')
   nulla = readln(oppo)
   maxoppo = readln(oppo)
   hitoppo = readln(oppo)
   nullb = readln(oppo)
   monoppo = readln(oppo)
   armoppo = readln(oppo)
   wepoppo = readln(oppo)
   ranoppo = readln(oppo)
   nullc = readln(oppo)
   nulld = readln(oppo)
   defoppo = readln(oppo)
   kildoppo = readln(oppo)
   kiloppo = readln(oppo)
   nulle = readln(oppo)
   call close(oppo)
   wutoppo=(hitoppo+maxoppo)/2
  end
else do
   CALL MESSAGE " "
   CALL MESSAGE GRE"That yuppie doesn't exist! -- Maybe you mispelled it?"
   CALL MESSAGE " "
   call delay(150)
   signal HIGHWAY
  end
signal FIGHT

FIGHT:
if defoppo = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE RED"   Sorry "handle", but "kildoppo" already killed him!"
   CALL MESSAGE " "
   call delay(160)
   signal MENU
  end
canfight = canfight - 1
CALL MESSAGE CLS
CALL MESSAGE " "
if canfight = 0 then do
   CALL MESSAGE BLU center("Oh.. you are too tired to fight anymore, try",78)
   CALL MESSAGE BLU center("fighting tomarrow.",78)
   CALL MESSAGE " "
   call delay(160)
   signal MENU
  end
call writeln(news,handle' picked a fight with 'who'!')
CALL MESSAGE center(BLUCYA"   LET'S FIGHT!!!   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE MAG"   Ladies and Gentlemen!  Tonight on the highway"
CALL MESSAGE MAG" we have a fight of all fights!  Between "WHI||handle||MAG","
CALL MESSAGE MAG" and "WHI||who||MAG".  Let's kick some butt!"
CALL MESSAGE RED"--------------------------------------------------- "RESET
CALL MESSAGE BLU"  Stats for"WHI" "who||BLU":"
CALL MESSAGE BLU"      Hitpoints: "RED||wutoppo
CALL MESSAGE BLU"           Rank: "RED||ranoppo
CALL MESSAGE BLU" Lifetime Kills: "RED||kiloppo
CALL MESSAGE " "
CALL MESSAGE BLU"  Stats for"WHI" "handle||BLU":"
CALL MESSAGE BLU"      Hitpoints: "RED||hitpoints
CALL MESSAGE BLU"           Rank: "RED||rank
CALL MESSAGE BLU"         Weapon: "RED||weapon
CALL MESSAGE BLU"        Vehicle: "RED||armor
CALL MESSAGE BLU" Lifetime Kills: "RED||kills
CALL MESSAGE " "
signal FIGHTOPT

FIGHTOPT:
CALL MESSAGE " "
prompt( CYA"Fight Commands: "BLU"["YEL"A"BLU"] "WHI"Attack, "BLU"["YEL"R"BLU"] "WHI"Retreat, "BLU"["YEL"H"BLU"]"WHI" Hit his vehicle"BLU"> "RESET)
opt=readstr()
if checkBBS() then signal BYE
if opt = "A" then signal ATTACK
if opt = "R" then signal RETREAT
if opt = "H" then signal RAMHIM
if opt = "Q" then signal RETREAT
signal FIGHTOPT

  /* If anyone finds a way to make this routine shorter let me know! */
 /* But if you change it without me knowing I'll be looking for you. */

STA:
if armoppo = arm1 then ar = RANDOM(0,2,Time('s'))
if armoppo = arm2 then ar = RANDOM(0,3,Time('s'))
if armoppo = arm3 then ar = RANDOM(0,4,Time('s'))
if armoppo = arm4 then ar = RANDOM(0,5,Time('s'))
if armoppo = arm5 then ar = RANDOM(0,6,Time('s'))
if armoppo = arm6 then ar = RANDOM(0,7,Time('s'))
if armoppo = arm7 then ar = RANDOM(0,8,Time('s'))
if armoppo = arm8 then ar = RANDOM(0,9,Time('s'))
if armoppo = arm9 then ar = RANDOM(1,10,Time('s'))
if armoppo = arm10 then ar = RANDOM(2,11,Time('s'))
if armoppo = arm11 then ar = RANDOM(3,12,Time('s'))
if armoppo = arm12 then ar = RANDOM(4,13,Time('s'))
if armoppo = arm13 then ar = RANDOM(5,14,Time('s'))
if armoppo = arm14 then ar = RANDOM(6,15,Time('s'))
if armor = arm1 then as = RANDOM(0,2,Time('s'))
if armor = arm2 then as = RANDOM(0,3,Time('s'))
if armor = arm3 then as = RANDOM(0,4,Time('s'))
if armor = arm4 then as = RANDOM(0,5,Time('s'))
if armor = arm5 then as = RANDOM(0,6,Time('s'))
if armor = arm6 then as = RANDOM(0,7,Time('s'))
if armor = arm7 then as = RANDOM(0,8,Time('s'))
if armor = arm8 then as = RANDOM(1,9,Time('s'))
if armor = arm9 then as = RANDOM(2,10,Time('s'))
if armor = arm10 then as = RANDOM(3,11,Time('s'))
if armor = arm11 then as = RANDOM(4,12,Time('s'))
if armor = arm12 then as = RANDOM(5,13,Time('s'))
if armor = arm13 then as = RANDOM(6,14,Time('s'))
if armor = arm14 then as = RANDOM(7,15,Time('s'))
if wepoppo = wep1 then wr = RANDOM(2,4,Time('s'))
if wepoppo = wep2 then wr = RANDOM(3,5,Time('s'))
if wepoppo = wep3 then wr = RANDOM(4,6,Time('s'))
if wepoppo = wep4 then wr = RANDOM(5,7,Time('s'))
if wepoppo = wep5 then wr = RANDOM(6,8,Time('s'))
if wepoppo = wep6 then wr = RANDOM(7,9,Time('s'))
if wepoppo = wep7 then wr = RANDOM(8,10,Time('s'))
if wepoppo = wep8 then wr = RANDOM(9,11,Time('s'))
if wepoppo = wep9 then wr = RANDOM(10,12,Time('s'))
if wepoppo = wep10 then wr = RANDOM(11,13,Time('s'))
if wepoppo = wep11 then wr = RANDOM(12,14,Time('s'))
if wepoppo = wep12 then wr = RANDOM(13,15,Time('s'))
if wepoppo = wep13 then wr = RANDOM(14,16,Time('s'))
if wepoppo = wep14 then wr = RANDOM(15,17,Time('s'))
if weapon = wep1 then ws = RANDOM(2,4,Time('s'))
if weapon = wep2 then ws = RANDOM(3,5,Time('s'))
if weapon = wep3 then ws = RANDOM(4,6,Time('s'))
if weapon = wep4 then ws = RANDOM(5,7,Time('s'))
if weapon = wep5 then ws = RANDOM(6,8,Time('s'))
if weapon = wep6 then ws = RANDOM(7,9,Time('s'))
if weapon = wep7 then ws = RANDOM(8,10,Time('s'))
if weapon = wep8 then ws = RANDOM(9,11,Time('s'))
if weapon = wep9 then ws = RANDOM(10,12,Time('s'))
if weapon = wep10 then ws = RANDOM(11,13,Time('s'))
if weapon = wep11 then ws = RANDOM(12,14,Time('s'))
if weapon = wep12 then ws = RANDOM(13,15,Time('s'))
if weapon = wep13 then ws = RANDOM(14,16,Time('s'))
if weapon = wep14 then ws = RANDOM(15,17,Time('s'))
ramlos = RANDOM(2,7,Time('s'))
hithim = ws - ar
if hithim < 0 then hithim = 0
hityou = wr - as
if hityou < 0 then hityou = 0
CALL MESSAGE " "
return

ATTACK:
call STA
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE RED"You hit "WHI||who||RED" for "hithim" hitpoints!"
   wutoppo = wutoppo - hithim
   if wutoppo <= 0 then signal WONFIGHT
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSTFIGHT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" rammed your "armor" and"
      CALL MESSAGE GRE"hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      wutoppo = wutoppo - ramlos
      if hitpoints <= 0 & wutoppo <= 0 then signal DRAWFIGHT
      if hitpoints <= 0 then signal LOSTFIGHT
      if wutoppo <= 0 then signal WONFIGHT
     end
  end
if rannum = 2 then do
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSTFIGHT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" rammed your "armor" and"
      CALL MESSAGE GRE"hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      wutoppo = wutoppo - ramlos
      if hitpoints <= 0 & wutoppo <= 0 then signal DRAWFIGHT
      if hitpoints <= 0 then signal LOSTFIGHT
      if wutoppo <= 0 then signal WONFIGHT
     end
   CALL MESSAGE " "
   CALL MESSAGE RED"You hit "WHI||who||RED" for "hithim" hitpoints!"
   wutoppo = wutoppo - hithim
   if wutoppo <= 0 then signal WONFIGHT
  end
signal FIGHTOPT

RAMHIM:
call STA
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE RED"You rammed "WHI||who"'S"RED" "armoppo
   CALL MESSAGE RED"and hit him for "hithim+5" hitpoints!"
   CALL MESSAGE RED"You also lost "ramlos" by hitting him."
   hitpoints = hitpoints - ramlos
   wutoppo=wutoppo-(hithim+5)
   if hitpoints <= 0 & wutoppo <=0 then signal DRAWFIGHT
   if wutoppo <= 0 then signal WONFIGHT
   if hitpoints <= 0 then signal LOSTFIGHT
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSTFIGHT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" rammed your "armor
      CALL MESSAGE GRE"and hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      wutoppo = wutoppo - ramlos
      if hitpoints <= 0 & wutoppo <= 0 then signal DRAWFIGHT
      if hitpoints <= 0 then signal LOSTFIGHT
      if wutoppo <= 0 then signal WONFIGHT
     end
  end
if rannum = 2 then do
   ransum = RANDOM(1,4,Time('s'))
   if ransum = 1 | ransum = 2 | ransum = 3 then do
      CALL MESSAGE WHI||who||GRE" hit you for "hityou" hitpoints!"
      hitpoints = hitpoints - hityou
      if hitpoints <= 0 then signal LOSTFIGHT
     end
   if ransum = 4 then do
      CALL MESSAGE " "
      CALL MESSAGE WHI||who||GRE" rammed your "armor
      CALL MESSAGE GRE"and hit you for "hityou+5" hitpoints!"
      CALL MESSAGE GRE"He also lost "ramlos" by hitting you."
      hitpoints=hitpoints-(hityou+5)
      wutoppo = wutoppo - ramlos
      if hitpoints <= 0 & wutoppo <= 0 then signal DRAWFIGHT
      if hitpoints <= 0 then signal LOSTFIGHT
      if wutoppo <= 0 then signal WONFIGHT
     end
   CALL MESSAGE " "
   CALL MESSAGE RED"You rammed "WHI||who"'S"RED" "armoppo
   CALL MESSAGE RED"and hit him for "hithim+5" hitpoints!"
   CALL MESSAGE RED"You also lost "ramlos" by hitting him."
   hitpoints = hitpoints - ramlos
   wutoppo=wutoppo-(hithim+5)
   if hitpoints <= 0 & wutoppo <=0 then signal DRAWFIGHT
   if wutoppo <= 0 then signal WONFIGHT
   if hitpoints <= 0 then signal LOSTFIGHT
  end
signal FIGHTOPT

CHEATIT:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE CYA" Hi Dave, What do you feel like doing today? "
CALL MESSAGE " "
CALL MESSAGE center("          "WHIGRE"   Supreme Being Cheat Menu   "RESET" ",78)
CALL MESSAGE " "
CALL MESSAGE RED center("[A] Raise Weapon 1 Level  ",78)
CALL MESSAGE WHI center("[B] Raise Vehicle 1 Level ",78)
CALL MESSAGE BLU center("[C] Raise Rank 1 Level    ",78)
CALL MESSAGE RED center("[D] Delete A Player!      ",78)
CALL MESSAGE BLU center("[E] QUIT                  ",78)
CALL MESSAGE " "
prompt( CYA"Oh great one, what is your command?"BLU"> "RESET)
ans=readstr()
if checkBBS() then signal BYE
if ans = "E" then signal MENU
if ans = "A" then do
if weapon = wep13 then weapon = wep14
if weapon = wep12 then weapon = wep13
if weapon = wep11 then weapon = wep12
if weapon = wep10 then weapon = wep11
if weapon = wep9 then weapon = wep10
if weapon = wep8 then weapon = wep9
if weapon = wep7 then weapon = wep8
if weapon = wep6 then weapon = wep7
if weapon = wep5 then weapon = wep6
if weapon = wep4 then weapon = wep5
if weapon = wep3 then weapon = wep4
if weapon = wep2 then weapon = wep3
if weapon = wep1 then weapon = wep2
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED center("Weapon Changed To A "WHI||weapon||RED"!",78)
CALL MESSAGE " "
call delay(120)
signal CHEATIT
end
if ans = "B" then do
if armor = arm13 then armor = arm14
if armor = arm12 then armor = arm13
if armor = arm11 then armor = arm12
if armor = arm10 then armor = arm11
if armor = arm9 then armor = arm10
if armor = arm8 then armor = arm9
if armor = arm7 then armor = arm8
if armor = arm6 then armor = arm7
if armor = arm5 then armor = arm6
if armor = arm4 then armor = arm5
if armor = arm3 then armor = arm4
if armor = arm2 then armor = arm3
if armor = arm1 then armor = arm2
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED center("Armor Raised To "WHI||armor||RED"!",78)
CALL MESSAGE " "
call delay(120)
signal CHEATIT
end
if ans = "C" then do
kills = 900
call CHECKRANK
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED center("Rank Raised To "WHI||rank||RED"!",78)
CALL MESSAGE " "
call delay(120)
signal CHEATIT
end
if ans = "D" then do
CALL MESSAGE " "
CALL NOCR GRE"Enter name of player to delete:"RESET' '
ss=readstr()
if checkBBS() then signal BYE
if exists(datapath||ss) then do
   CALL MESSAGE " "
   prompt( CYA"Delete "WHI||ss||CYA" ("GRE"y"CYA"/"RED"N"CYA")? "RESET)
   st=readstr()
   if checkBBS() then signal BYE
   if st = "Y" then do
      call Delete(datapath||ss'.DAT')
      CALL MESSAGE " "
      CALL MESSAGE RED"Deleted..."
      howm = howm - 1
      call delay(100)
   end
end
if ~exists(datapath||ss'.DAT') then do
   CALL MESSAGE " "
   CALL MESSAGE RED ss" doesn't exist!"RESET
   CALL MESSAGE " "
   call delay(120)
   signal CHEATIT
end
signal CHEATIT
end
signal CHEATIT

RETREAT:
call STA
CALL MESSAGE " "
CALL MESSAGE RED"YOU RUN!"
if rank = ran1 | rank = ran2 | rank = ran3 then rannum = RANDOM(1,2,Time('s'))
if rank = ran4 | rank = ran5 | rank = ran6 then rannum = RANDOM(1,3,Time('s'))
if rank = ran7 | rank = ran8 | rank = ran9 then rannum = RANDOM(1,4,Time('s'))
if rank = ran10 | rank = ran11 then rannum = RANDOM(1,5,Time('s'))
if rank = ran12 | rank = ran13 | rank = ran14 then rannum = RANDOM(1,6,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE GRE"`I don't think so.' says "WHI||who||GRE"."
   CALL MESSAGE GRE"You can't escape me right now!"
  end
if rannum = 2 | rannum = 3 | rannum = 4 | rannum = 5 | rannum = 6 then do
   ex = RANDOM(51,122,Time('s'))
   CALL MESSAGE " "
   CALL MESSAGE GRE"You ESCAPED off of exit "ex"!"
   CALL MESSAGE " "
   call writeln(news,handle' ran away from 'who'!')
   call delay(110)
   signal MENU
  end
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then do
   CALL MESSAGE " "
   CALL MESSAGE WHI||who||GRE" hits you for "hityou" hitpoints!"
   hitpoints = hitpoints - hityou
   if hitpoints <= 0 then signal LOSTFIGHT
  end
if rannum = 2 then do
   CALL MESSAGE " "
   CALL MESSAGE WHI||who||GRE" misses you!"
  end
CALL MESSAGE " "
call delay(20)
signal FIGHTOPT

WONFIGHT:
defoppo = 1
wutoppo = 0
kills = kills + 1
call delay(110)
CALL MESSAGE CLS
CALL MESSAGE RESET"               *               *"
CALL MESSAGE "   *                       "YEL"|"RESET"            *"
CALL MESSAGE "        *      "YEL"\/OU \/\/ON o"
CALL MESSAGE "               /    "RESET"*             *"
CALL MESSAGE " *                      *               *"
CALL MESSAGE " "
CALL MESSAGE RED"  You Won!  "MAG" You Get "WHI"$"monoppo/2||MAG"!"
call writeln(news,handle' beat 'who' on the highway!')
money = money + monoppo/2
monoppo = monoppo/2
call CHECKRANK
call SAVEOPPO
call EVENER
call delay(110)
signal MENU

SAVEOPPO:
call open(oppo,datapath||who'.DAT','W')
   call writeln(oppo,nulla)
   call writeln(oppo,maxoppo)
   call writeln(oppo,wutoppo)
   call writeln(oppo,nullb)
   call writeln(oppo,monoppo)
   call writeln(oppo,armoppo)
   call writeln(oppo,wepoppo)
   call writeln(oppo,ranoppo)
   call writeln(oppo,nullc)
   call writeln(oppo,nulld)
   call writeln(oppo,defoppo)
   call writeln(oppo,handle)
   call writeln(oppo,kiloppo)
   call writeln(oppo,nulle)
   call close(oppo)
call delay(200)
return

LOSTFIGHT:
kiloppo = kiloppo + 1
hitpoints = 0
call delay(150)
CALL MESSAGE CLS
CALL MESSAGE RED"                                   _ ___"
CALL MESSAGE "              \_/ /\ | |    |  /\ /   |  |||"
CALL MESSAGE "               |  \/ |_|    |_ \/ _\  |  ooo"
CALL MESSAGE " "
CALL MESSAGE MAG"   Serves ya right for thinking you were top yuppie!"
CALL MESSAGE " "
call writeln(news,handle' got his butt kicked by 'who'!')
money = money/2
monoppo = monoppo + money/2
CALL MESSAGE GRE" You lost "WHI"$"money/2||GRE"."
CALL MESSAGE GRE" You have 0 hitpoints."
CALL MESSAGE CYA" Oh well, better luck next time!"
call SAVEOPPO
call EVENER
signal MENU

DRAWFIGHT:
kills = kills + 1
kiloppo = kiloppo + 1
hitpoints = 0
wutoppo = 0
defoppo = 1
call delay(150)
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED center("AMAZING!",78)
CALL MESSAGE " "
CALL MESSAGE MAG center("Both of you killed each other with your final blows!",78)
CALL MESSAGE MAG center("You both have 0 hitpoints, you kept your money.",78)
CALL MESSAGE " "
call writeln(news,handle' and 'who' killed each other!')
call CHECKRANK
call SAVEOPPO
call delay(110)
signal MENU

TOP:
CALL MESSAGE " "
CALL MESSAGE RED"One Moment, Calculating High Scores..."
call POINTCK
turnsleft = turnsleft + 1
call HICK
call SHOWIT
signal MENU

HICK:
call open(top10,datapath'TopOnes','R')
topalias.1 = handle
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
return

SHOWIT:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE center("       "BLUCYA"   Top 10 Yuppies   "RESET,78)
CALL MESSAGE BLU center("--------------------------------------------",73)RESET
CALL MESSAGE " "
call open(top10,datapath'TopOnes','W')
do i = 1 to 10
   CALL NOCR CYA right(topalias.i,34,' ')
   CALL MESSAGE MAG ' ' topscore.i
   call writeln(top10,topscore.i' 'topalias.i)
end
call close(top10)
do i = 1 to Lines - 18
   CALL MESSAGE ''
end
prompt( RESET"Press "RED"RETURN"WHI" when finished: "RESET)
junk=readstr()
if checkBBS() then signal BYE
signal MENU

TRIP:
call EVENER
randm = RANDOM(1,3,Time('s'))
rna = RANDOM(1,6,Time('s'))
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE center("             "BLUCYA"   First of the Last Travel Agency   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE CYA center("Today's places to travel",78)
CALL MESSAGE BLU center("--------------------------",78)
if randm = 1 then CALL MESSAGE WHI center("A. Washington D.C.   ",78)
if randm = 2 then CALL MESSAGE WHI center("A. Wall Street - NY  ",78)
if randm = 3 then CALL MESSAGE WHI center("A. Outer Mongolia    ",78)
if randm = 1 then CALL MESSAGE WHI center("B. Peru              ",78)
if randm = 2 then CALL MESSAGE WHI center("B. Peru              ",78)
if randm = 3 then CALL MESSAGE WHI center("B. England           ",78)
if randm = 1 then CALL MESSAGE WHI center("C. Germany           ",78)
if randm = 2 then CALL MESSAGE WHI center("C. Switzerland       ",78)
if randm = 3 then CALL MESSAGE WHI center("C. Niagra Falls - NY ",78)
if randm = 1 then CALL MESSAGE WHI center("D. Verona, Italy     ",78)
if randm = 2 then CALL MESSAGE WHI center("D. Athens, Greece    ",78)
if randm = 3 then CALL MESSAGE WHI center("D. Disney World      ",78)
CALL MESSAGE " "RESET
prompt( CYA"Where do you want to travel?  "BLU"["YEL"Q"BLU"]"CYA" Quits:"BLU"> "RESET)
trav=readstr()
if checkBBS() then signal BYE
if trav = "Q" then signal MENU
if randm = 1 & trav = "A" then signal WASHINGTON
if randm = 1 & trav = "B" then signal PERU
if randm = 1 & trav = "C" then signal GERMANY
if randm = 1 & trav = "D" then signal VERONA
if randm = 2 & trav = "A" then signal WALLSTREET
if randm = 2 & trav = "B" then signal PERU
if randm = 2 & trav = "C" then signal SWITZERLAND
if randm = 2 & trav = "D" then signal ATHENS
if randm = 3 & trav = "A" then signal MONGOLIA
if randm = 3 & trav = "B" then signal ENGLAND
if randm = 3 & trav = "C" then signal NIAGARA
if randm = 3 & trav = "D" then signal DISNEY
signal TRIP

WASHINGTON:
price = 9000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Washington D.C.   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Washington D.C. is the political gut of our nation.  All the"
CALL MESSAGE " largest decisions are made here that effect our lives greatly."
CALL MESSAGE " (And hopefully our president has nothing to do with them.)"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Washington D.C."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"In Washington D.C. you meet the"
   CALL MESSAGE CYA"president and get a loan from him!"
   CALL MESSAGE YEL"You get $30000!"
   CALL MESSAGE BLU"There's nothing like living off of"
   CALL MESSAGE BLU"honest taxpayers' money, eh?"
   money = money + 30000
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Lean too far out of the Washington"
   CALL MESSAGE CYA"Monument and fall!"
   CALL MESSAGE YEL"You lose 20 hitpoints."
   if hitpoints < 20 then hitpoints = 0
   if hitpoints >= 20 then hitpoints = hitpoints - 20
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Get appointed a supreme court judge!"
   CALL MESSAGE YEL"You earn $20000!"
   money = money + 20000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Get locked in a taxi from hell!"
   CALL MESSAGE CYA"Tour the entire city 800 times over."
   CALL MESSAGE YEL"Lose $15000 cab fair."
   if money < 15000 then money = 0
   if money >= 15000 then money = money - 15000
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Read the constitution and feel"
   CALL MESSAGE CYA"intellectually enlightened!"
   CALL MESSAGE YEL"Recieve 40 hitpoints."
   if maxpoints >= 40 then hitpoints = hitpoints + 40
   if maxpoints < 40 then hitpoints = maxpoints
  end
signal RATRACE

RATRACE:
call delay(100)
CALL MESSAGE " "
prompt( RESET"Press "RED"RETURN"WHI" when finished: "RESET)
junk=readstr()
if checkBBS() then signal BYE
CALL MESSAGE " "
CALL MESSAGE GRE"Well, it's been fun but you gotta"
CALL MESSAGE GRE"get back into the rat race!"
CALL MESSAGE " "RESET
call delay(120)
signal MENU

PERU:
price = 7000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Peru   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Peru is a land of beauty and mystery, just beware, it's a"
CALL MESSAGE " jungle out there!"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Peru."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Learn ancient healing spells from"
   CALL MESSAGE CYA"old scrolls..."
   CALL MESSAGE YEL"Gain "maxpoints" hitpoints!"
   hitpoints = maxpoints
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Fall off of cliff!"
   CALL MESSAGE YEL"Lost 30 hitpoints."
   if hitpoints < 30 then hitpoints = 0
   if hitpoints >= 30 then hitpoints = hitpoints - 30
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Discover gold in an ancient tomb!"
   CALL MESSAGE YEL"Gain $50000!"
   money = money + 50000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Get bitten by a snake and go to the"
   CALL MESSAGE CYA"witch doctor!"
   CALL MESSAGE YEL"He charges you $10000 for services."
   if money < 10000 then money = 0
   if money >= 10000 then money = money - 10000
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Learn to dine CANNIBAL style!"
   CALL MESSAGE YEL"Gain 40 hitpoints!"
   if maxpoints >= 40 then hitpoints = hitpoints + 40
   if maxpoints < 40 then hitpoints = maxpoints
  end
CALL MESSAGE " "
signal RATRACE

GERMANY:
price = 10000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Germany   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Ah Germany, good ol' Deutschland.  Home to many beers and"
CALL MESSAGE " bratwursts, a country of great prosperity risen out of the ashes"
CALL MESSAGE " of World War II.  (Not to mention it is great for partying!)"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Germany."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Win a German Bratwurst eating contest!"
   CALL MESSAGE YEL"Win $30000 and a Volkswagen!"
   money = money + 30000
   if armor = arm1 | armor = arm2 | armor = arm3 | armor = arm4 | armor = arm5 | armor = arm6 | armor = arm7 | armor = arm8 then armor = arm9
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Party too much and have a hangover the"
   CALL MESSAGE CYA"size of Africa!"
   CALL MESSAGE YEL"Lose 20 hitpoints."
   if hitpoints < 20 then hitpoints = 0
   if hitpoints >= 20 then hitpoints = hitpoints - 20
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Total your vehicle on the autobahn!"
   CALL MESSAGE YEL"Lose $25000 for repairs."
   if money < 25000 then money = 0
   if money >= 25000 then money = money - 25000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Recieve a vitamin mixture from the"
   CALL MESSAGE CYA"local Apothecary!"
   CALL MESSAGE YEL"Gain "maxpoints" hitpoints!"
   hitpoints = maxpoints
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Get caught on the cirle drives for"
   CALL MESSAGE CYA"three days!"
   CALL MESSAGE YEL"Lose $1000 for gas."
   if money < 1000 then money = 0
   if money >= 1000 then money = money - 1000
  end
CALL MESSAGE " "
signal RATRACE

VERONA:
price = 4000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Verona, Italy   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Verona, Italy -- the host city to the immortal love story of"
CALL MESSAGE " Shakespeare's Romeo and Juliet, truly the greatest love story"
CALL MESSAGE " ever written...  (although a bit cryptic!)"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Verona, Italy."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,4,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"See a play done of Romeo and Juliet!"
   CALL MESSAGE YEL"Pay $4000 to see it."
   if money < 4000 then money = 0
   if money >= 4000 then money = money - 4000
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Visit the Capulet's Tomb and get"
   CALL MESSAGE CYA"trapped for a week!"
   CALL MESSAGE YEL"Lose 40 hitpoints!"
   if hitpoints < 40 then hitpoints = 0
   if hitpoints >= 40 then hitpoints = hitpoints - 40
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Some people think you look just like"
   CALL MESSAGE CYA"Romeo! (Which is odd if you are a woman.)"
   CALL MESSAGE YEL"Recieve in gifts $18000!"
   money = money + 18000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Some people think you look just like"
   CALL MESSAGE CYA"Juliet! (Whick is odd if you are a man.)"
   CALL MESSAGE YEL"Recieve in gifts $19000!"
   money = money + 19000
  end
signal RATRACE

WALLSTREET:
price = 12500
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Wall Street!   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Wall Street, where money is made and fortunes are lost.  Where"
CALL MESSAGE " great men are made and greedy men are broken.  This crazy place"
CALL MESSAGE " is locked in the heart of a crazy city, where else, New York."
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived at Wall Street."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"You break even on the stocks..."
   CALL MESSAGE YEL"Recieve $12500 salary."
   money = money + 12500
  end
if rannum = 2 then do
   CALL MESSAGE CYA"HIT IT BIG IN DOW JONES!"
   CALL MESSAGE YEL"Recieve $75000!!!"RESET
   money = money + 75000
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Get mugged in the big apple!"
   CALL MESSAGE YEL"Lose $20000."
   if money < 20000 then money = 0
   if money >= 20000 then money = money - 20000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Get hit by a maniac cab driver!"
   CALL MESSAGE YEL"You lose 50 hitpoints."
   if hitpoints < 50 then hitpoints = 0
   if hitpoints >= 50 then hitpoints = hitpoints - 50
  end
if rannum = 5 then do
   CALL MESSAGE CYA"AT&T goes through the roof!"
   CALL MESSAGE YEL"Gain $55000!"
   money = money + 55000
  end
signal RATRACE

SWITZERLAND:
price = 8500
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Switzerland   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Switzerland is famous for skiing, watches, and many other things"
CALL MESSAGE " that I guess I'm too stupid to think of right now.  The swiss,"
CALL MESSAGE " locked in a mountainous cage, stands protected from enemy countries"
CALL MESSAGE " thus remaining one of the FEW neutral countries."
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Swizerland."
call delay(80)
CALL MESSAGE " "RESET
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Go skiing in the Swiss Alps and"
   CALL MESSAGE CYA"break your leg!"
   CALL MESSAGE YEL"Lose 40 hitpoints."
   if hitpoints < 40 then hitpoints = 0
   if hitpoints >= 40 then hitpoints = hitpoints - 40
  end
if rannum = 2 then do
   CALL MESSAGE CYA"AVALANCHE!!! You are buried in"
   CALL MESSAGE CYA"snow!"
   CALL MESSAGE YEL"Lose 40 hitpoints."
   if hitpoints < 40 then hitpoints = 0
   if hitpoints >= 40 then hitpoints = hitpoints - 40
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Save a millionare's life in a chair"
   CALL MESSAGE CYA"lift accident! (He fell on you.)"
   CALL MESSAGE YEL"Win $50000!"
   money = money + 50000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Eat enough to feed a horse!"
   CALL MESSAGE YEL"Gain "maxpoints" hitpoints."
   hitpoints = maxpoints
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Find a cave with ancient artifacts!"
   CALL MESSAGE YEL"Get $30000!"
   money = money + 30000
  end
signal RATRACE

ATHENS:
price = 10000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Athens   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Athens, Greece, the heart of Greek mythology.  Many stories and"
CALL MESSAGE " plays were made of this place relating to the Gods, and their"
CALL MESSAGE " adventures."
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Athens, Greece."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Go to watch a play of Antigone."
   CALL MESSAGE YEL"Pay $5000."
   if money < 5000 then money = 0
   if money >= 5000 then money = money - 5000
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Meet Zeus!  He gives you a gift!"
   CALL MESSAGE YEL"Recieve $30000!"
   money = money + 30000
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Hades kidnaps you to the underword!"
   CALL MESSAGE YEL"Lose 50 hitpoints."
   if hitpoints < 50 then hitpoints = 0
   if hitpoints >= 50 then hitpoints = hitpoints - 50
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Eat ambrosia and drink nectar!"
   CALL MESSAGE YEL"Gain "maxpoints" hitpoints!"
   hitpoints = maxpoints
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Buy a colluseum!"
   CALL MESSAGE YEL"Pay $22000!"
   if money < 22000 then money = 0
   if money >= 22000 then money = money - 22000
  end
signal RATRACE

MONGOLIA:
price = 4000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Outer Mongolia!   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Outer Mongolia... hmm.. hard to explain, but in itself an "
CALL MESSAGE " adventure.  This country was the fertle ground in which Ghengis"
CALL MESSAGE " Kahn rose out of to ravage China. (And we all know what he's like!"
CALL MESSAGE " -imagine what the country must be like!) "
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Outer Mongolia."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Learn the ancient art of wizardry"
   CALL MESSAGE CYA"and heal yourself!"
   CALL MESSAGE YEL"Gain "maxpoints" hitpoints!"
   hitpoints = maxpoints
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Climb to a mountaintop to get words"
   CALL MESSAGE CYA"of wisdom from an old wiseman only"
   CALL MESSAGE CYA"to find him dead!"
   CALL MESSAGE YEL"Lose 40 hitpoints for the effort."
   CALL MESSAGE YEL"Pay $10000 for funeral services."
   if hitpoints < 40 then hitpoints = 0
   if hitpoints >= 40 then hitpoints = hitpoints - 40
   if money < 10000 then money = 0
   if money >= 10000 then money = money - 10000
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Win Mongolia's famous gameshow:"
   CALL MESSAGE CYA"STALK DOWN THE WILD BEAR AND KILL"
   CALL MESSAGE CYA"IT WITH YOUR BARE HANDS AND TONGUE!"
   CALL MESSAGE YEL"Win $40000!"
   money = money + 40000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Record Mongolia's #1 song:"
   CALL MESSAGE CYA"Ode to Ghengis Kahn."
   CALL MESSAGE YEL"Win $25000!"
   money = money + 25000
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Break Mongolia's law of not"
   CALL MESSAGE CYA"using utensils."
   CALL MESSAGE YEL"Fined $20000."
   if money < 20000 then money = 0
   if money >= 20000 then money = money - 20000
  end
signal RATRACE

ENGLAND:
price = 9500
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to England   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Jolly Ol' England, as they say, is a whole different culture from"
CALL MESSAGE " the American sort.  Here queens and kings are supported (as well as"
CALL MESSAGE " Amigas!)"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in England."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Become the Price of Wales!"
   CALL MESSAGE YEL"Earn $32000!"
   money = money + 32000
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Discover the genetic secret of"
   CALL MESSAGE CYA"why the english have extremely"
   CALL MESSAGE CYA"large noses!"
   CALL MESSAGE YEL"Win $23000!"
   money = money + 23000
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Get envolved in a 20 bicycle"
   CALL MESSAGE CYA"pileup!"
   CALL MESSAGE YEL"Lose 35 hitpoints."
   if hitpoints < 35 then hitpoints = 0
   if hitpoints >= 35 then hitpoints = hitpoints - 35
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Get pulled over for driving on the"
   CALL MESSAGE CYA"wrong side of the road!"
   CALL MESSAGE YEL"Fined $35000!"
   if money < 35000 then money = 0
   if money >= 35000 then money = money - 35000
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Accidentally get trapped watching a"
   CALL MESSAGE CYA"9 hour movie that is extremely boring"
   CALL MESSAGE CYA"and not even in the english language!"
   CALL MESSAGE YEL"Lose $23000 parking meter fees."
   CALL MESSAGE YEL"Lose 10 hitpoints for IQ point loss."
   if money < 23000 then money = 0
   if money >= 23000 then money = money - 23000
   if hitpoints < 10 then hitpoints = 0
   if hitpoints >= 10 then hitpoints = hitpoints - 10
  end
signal RATRACE

NIAGARA:
price = 11000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   Going to Niagara Falls, NY   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   Niagara Falls, the famed gigantic falls which have claimed lives"
CALL MESSAGE " and made fame.  Their sight is breathtaking.  --Plus there are a"
CALL MESSAGE " multitude of wax museums for those who don't like water!"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Niagara Falls, NY."
call delay(80)
CALL MESSAGE " "
rannum = RANDOM(1,6,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Walk across the falls on a"
   CALL MESSAGE CYA"tightrope!"
   CALL MESSAGE YEL"Win $20000!"
   money = money + 20000
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Attempt at walking across the"
   CALL MESSAGE CYA"falls on a tightrope--AND FALL"
   CALL MESSAGE CYA"IN!  (Smooth move ex-lax!)"
   CALL MESSAGE YEL"Lose 50 hitpoints."
   if hitpoints < 50 then hitpoints = 0
   if hitpoints >= 50 then hitpoints = hitpoints - 50
  end
if rannum = 3 then do
   CALL MESSAGE CYA"Go down the falls in a barrel!"
   CALL MESSAGE CYA"And.. well, it isn't pretty."
   CALL MESSAGE YEL"Lose 50 hitpoints."
   if hitpoints < 50 then hitpoints = 0
   if hitpoints >= 50 then hitpoints = hitpoints - 50
  end
if rannum = 4 then do
   CALL MESSAGE CYA"Get stuck in wax museum hell!"
   CALL MESSAGE YEL"Lose $19000!"
   if money < 19200 then money = 0
   if money >= 19000 then money = money - 19000
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Discover PLUTONIUM under the falls!"
   CALL MESSAGE YEL"Awarded $55000!"
   money = money + 55000
  end
if rannum = 6 then do
   CALL MESSAGE CYA"Discover the fountain of youth in"
   CALL MESSAGE CYA"the falls!"
   CALL MESSAGE YEL"You get "maxpoints" hitpoints!"
   hitpoints = maxpoints
  end
signal RATRACE

DISNEY:
price = 13000
CALL MESSAGE CLS
CALL MESSAGE center(BLUCYA"   I'm Going to Disney World!   "RESET,78)
CALL MESSAGE " "
CALL MESSAGE "   "handle", you have just one the Super Bowl!  Where are you"
CALL MESSAGE " going next?  ;-)  Yes, childrens' paradise and adults wonder,"
CALL MESSAGE " this wonderous place was founded by a man who wanted a fun place"
CALL MESSAGE " different from the dirty, dusty carnivals in this world, Walt Disney"
CALL MESSAGE " "
call GOING
CALL MESSAGE RED"Arrived in Disney World."
call delay(50)
CALL MESSAGE " "
rannum = RANDOM(1,5,Time('s'))
if rannum = 1 then do
   CALL MESSAGE CYA"Uncover Disney's multibillion"
   CALL MESSAGE CYA"dollar plot to initiate SDI!"
   CALL MESSAGE YEL"Awarded $33000!"
   money = money + 33000
  end
if rannum = 2 then do
   CALL MESSAGE CYA"Get trapped on Space Mountain"
   CALL MESSAGE CYA"roller coaster without a seatbelt!"
   CALL MESSAGE YEL"Lose 60 hitpoints."
   if hitpoints < 60 then hitpoints = 0
   if hitpoints >= 60 then hitpoints = hitpoints - 60
  end
if rannum = 3 then do
   CALL MESSAGE CYA"You get to star in a Disney movie!"
   CALL MESSAGE YEL"Payed $20000!"
   money = money + 20000
  end
if rannum = 4 then do
   CALL MESSAGE CYA"You are mysteriously turned into an"
   CALL MESSAGE CYA"animated penguin!"
   CALL MESSAGE YEL"Lose $40000! (Penguins have no"
   CALL MESSAGE YEL"pockets.. do they?)"
   if money < 40000 then money = 0
   if money >= 40000 then money = money - 40000
  end
if rannum = 5 then do
   CALL MESSAGE CYA"Your fairy godfather grants you"
   CALL MESSAGE CYA"hitpoints!"
   CALL MESSAGE YEL"Granted "maxpoints" hitpoints!"
   hitpoints = maxpoints
  end
signal RATRACE

GOING:
CALL MESSAGE " "
CALL MESSAGE CYA"You have $"WHI||money||CYA".  The trip costs "WHI||price||CYA"."
CALL MESSAGE " "
prompt( CYA"Wanna go? ("GRE"Y"CYA"/"RED"n"CYA"):"BLU"> "RESET)
ans=readstr()
if checkBBS() then signal BYE
if ans = "N" then signal TRIP
CALL MESSAGE CLS
if money < price then do
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU DON'T HAVE ENOUGH MONEY TO GO!",78)
   CALL MESSAGE " "RESET
   call delay(150)
   signal TRIP
  end
money = money - price
CALL MESSAGE " "
CALL MESSAGE GRE"Money Paid..."RESET
CALL MESSAGE MAG"Taking flight..."RESET
rando = RANDOM(1,20,Time('s'))
call delay(90)
CALL MESSAGE YEL" You ate the plane's food -- gain "rando" hitpoints."RESET
call delay(90)
hitpoints = hitpoints + rando
CALL MESSAGE " "
CALL MESSAGE MAG"Landing..."
call delay(100)
ransak = RANDOM(1,200,Time('s'))
if ransak = 13 | ransak = 21 | ransak = 192 | ransak = 112 | ransak = 80 then do
   if money >= 50000 then money = money - 20000
   if money < 50000 then money = money - money
   if hitpoints >= 80 then hitpoints = hitpoints - 30
   if hitpoints < 80 then hitpoints = hitpoints - hitpoints
   CALL MESSAGE RED"YOUR PLANE CRASH LANDED!!!"
   CALL MESSAGE RED"YOU LOSE 80 HITPOINTS!"
   CALL MESSAGE RED"YOU LOSE 50000 DOLLARS TO GET A TRIP BACK & HOSPITAL FEES!"
   CALL MESSAGE " "
   prompt( CYA"Sue the pilot (Y/n)? ")
   ans=readstr()
   if checkBBS() then signal BYE
   if ans = "N" then do
      CALL MESSAGE MAG"Ok.. not sueing him."
      call delay(100)
      signal MENU
     end
   signal SUEHIM
  end
return

SUEHIM:
rannum = RANDOM(1,2,Time('s'))
if rannum = 1 then verdict = "GUILTY"
if rannum = 2 then verdict = "NOT GUILTY"
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED"A long a tedious trial goes on..."
CALL MESSAGE RED"and now here's the charge..."
CALL MESSAGE " "RESET
CALL MESSAGE CYA"We the jury, find in the case of"
CALL MESSAGE CYA"the Pilot: "RED||verdict||CYA"!"
CALL MESSAGE " "
if rannum = 1 then do
   CALL MESSAGE MAG"Money in the amount of $150000"
   CALL MESSAGE MAG"to the plaintiff "handle"!"
   money = money + 150000
   call writeln(news,handle' won $150000 in court!')
   call delay(80)
  end
if rannum = 2 then do
   CALL MESSAGE MAG"It was the planes falt, not"
   CALL MESSAGE MAG"the Pilot's falt.  Nothing"
   CALL MESSAGE MAG"gained, nothing lost."
   CALL MESSAGE " "
   randy = RANDOM(1,6,Time('s'))
   randy = randy * 1000
   CALL MESSAGE CYA"Pay "WHI"$"randy" "CYA"in lawyer fees."
   if money < randy then money = 0
   if money >= randy then money = money - randy
   call delay(80)
  end
CALL MESSAGE " "
CALL MESSAGE BLU"Justice is served."RESET
CALL MESSAGE " "
prompt( "Press "RED"RETURN"WHI" when finished: "RESET)
junk=readstr()
if checkBBS() then signal BYE
signal MENU

LALA:
CALL MESSAGE " "
CALL MESSAGE CYA"E"YEL"n"GRE"t"BLU"e"RED"r "WHI"t"MAG"h"YEL"e"RED" m"CYA"a"WHI"g"MAG"i"GRE"c"BLU" w"YEL"o"WHI"r"RED"d"BLU": "RESET
pass=readstr()
if checkBBS() then signal BYE
if pass = "CHEATING!" then signal P2
signal MENU

REST:
lost = maxpoints - hitpoints
fiz = RANDOM(1,750,Time('s'))
if rank = ran1 then pernight = 500
if rank = ran2 then pernight = 700
if rank = ran3 then pernight = 900
if rank = ran4 then pernight = 1100
if rank = ran5 then pernight = 1400
if rank = ran6 then pernight = 2000
if rank = ran7 then pernight = 2500
if rank = ran8 then pernight = 2800
if rank = ran9 then pernight = 3000
if rank = ran10 then pernight = 3300
if rank = ran11 then pernight = 3600
if rank = ran12 then pernight = 4000
if rank = ran13 then pernight = 4400
if rank = ran14 then pernight = 5000
room = pernight + fiz
CALL MESSAGE CLS
CALL MESSAGE " "RESET
CALL MESSAGE "      _____/_________________________________________________\_____"
CALL MESSAGE "     |_____________________"MAG"-= LE GRAND HOTEL =-"WHI"____________________|"
CALL MESSAGE "        |\_________/     ________________________    \_________/|"
CALL MESSAGE "        |  \_____/       \______________________/      \_____/  |"
CALL MESSAGE "        |   || ||         |\__________________/|        || ||   |"
CALL MESSAGE "        |   || ||         | "BLU"| |     ||     | | "WHI"|        || ||   |"
CALL MESSAGE "        |   || ||         | "BLU"| |"CYA" \"BLU"   ||     | | "WHI"|        || ||   |"
CALL MESSAGE "        |   || ||         | "BLU"| |"CYA" \  "YEL"["BLU"||"YEL"]"BLU"    | | "WHI"|        || ||   |"RESET
CALL MESSAGE "        |   || ||         | "BLU"| |     || "CYA"\"BLU"   | | "WHI"|        || ||   |"RESET
CALL MESSAGE "      __|___||_||_________|/"GRE"#"BLU"\|_____||_____|/"GRE"#"WHI"\|________||_||___|__"RESET
CALL MESSAGE GRE"      #####"WHI"/_____\"GRE"###########"BLU"/"RED"/            \"BLU"\"GRE"##########"WHI"/_____\"GRE"#####"RESET
CALL MESSAGE GRE"      #######################"RED"/              \"GRE"######################"RESET
CALL MESSAGE GRE"      ######################"RED"/                \"GRE"#####################"RESET
CALL MESSAGE GRE"      #####################"RED"/                  \"GRE"####################"RESET
CALL MESSAGE GRE"      ####################"RED"/____________________\"GRE"###################"RESET
CALL MESSAGE " "
CALL MESSAGE MAG"                     Right now it costs $"WHI||room||MAG" per night."
CALL MESSAGE CYA"             30 hitpoints healed a night. - You need to heal "WHI||lost||CYA"."
CALL MESSAGE " "
CALL MESSAGE CYA" Stay how many nights? "BLU"["YEL"Q"BLU"]"CYA" Quits"BLU":"RESET
stay=readstr()
if checkBBS() then signal BYE
if stay = "Q" then signal MENU
if stay = "" | stay = " " | stay = "  " | stay = "   " | stay = "    " | stay = "     " then signal MENU
if stay = 0 then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE CYA"What, are you just going to mope around in the lounge all day?"
   CALL MESSAGE " "RESET
   call delay(150)
   signal MENU
  end
if stay > 9999999999999999999999999999999999999999999999999999999999999999999999999999999999 then signal YOT
if stay < 0 then signal YOT
dogz = compress(stay,'ABCDEFGHIJKLMNOPQRSTUVWXYZ.,{}[];:''"?/\=+-_)(*&^%$#@!~`<>')
payme = dogz * room
if payme > money then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("You don't have enough money to stay here that long!",78)
   CALL MESSAGE " "RESET
   call delay(130)
  end
if money = payme then do
   CALL MESSAGE " "
   CALL MESSAGE MAG"You are going to be "RED"BROKE"MAG" if you stay here that long!"RESET
   CALL MESSAGE " "
   prompt( CYA"Stay here anyway ("GRE"Y"CYA"/"RED"n"CYA")? "RESET)
   stayany=readstr()
   if checkBBS() then signal BYE
   if stayany = "N" then do
      CALL MESSAGE BLU"Ok.. leaving Hotel..."RESET
      call delay(100)
      signal MENU
     end
  end
if payme <= money then signal RESTHERE
signal MENU

YOT:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED center("You must enter a NUMBER! ",78)
CALL MESSAGE RESET" "
call delay(110)
signal REST

RESTHERE:
money = money - payme
runny = RANDOM(1,200,Time('s'))
if runny >= money then runny = 0
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE YEL center("Enjoy your stay!",78)
CALL MESSAGE " "
CALL MESSAGE CYA"Sleeping."
  do i = 1 to 20
   CALL NOCR CYA"."
   call delay(20)
   i=i+1
  end
CALL MESSAGE " "
hitpoints=hitpoints+(dogz*30)
if hitpoints > maxpoints then hitpoints = maxpoints
CALL MESSAGE CYA"Ahhh.. you feel refreshed!"
CALL MESSAGE CYA"You order $"WHI||runny||CYA" in room service."
if money = 0 then CALL MESSAGE CYA"By the way.. "RED"You're Broke!"RESET
CALL MESSAGE " "
prompt( RESET" Press "RED"RETURN"WHI" when finished: "RESET)
junk=readstr()
if checkBBS() then signal BYE
signal MENU

YOURSTATS:
turnsleft = turnsleft + 1
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE "                "BLUCYA"   Player Statistics For The Yuppie "handle"   "RESET
CALL MESSAGE BLU center(" ----------------------------------------------------------------------- ",78)
call POINTCK
CALL MESSAGE " "
CALL MESSAGE MAG"                         Points: "WHI||points
CALL MESSAGE MAG"                           Rank: "WHI||rank
CALL MESSAGE MAG"                      Hitpoints: "WHI||hitpoints
CALL MESSAGE MAG"                     Turns Left: "WHI||turnsleft
CALL MESSAGE MAG"                         Weapon: "WHI||weapon
CALL MESSAGE MAG"                        Vehicle: "WHI||armor
CALL MESSAGE MAG"                          Money: "WHI"$"money
CALL MESSAGE MAG"                     Bank Money: "WHI"$"bmoney
CALL MESSAGE MAG"                 Lifetime Kills: "WHI||kills
CALL MESSAGE " "RESET
CALL MESSAGE " "
prompt( " Hit "RED"RETURN"WHI" when finished: "RESET)
junk=readstr()
if checkBBS() then signal BYE
signal MENU

VEGAS:
currentmem=storage()
  if currentmem<lowmem then do
    CALL MESSAGE CLS
    CALL MESSAGE " "
    CALL MESSAGE RED CENTER("Sorry!  Out of memory!  Saving and exiting...",78)
    call delay(190)
    signal BYE
  end
call EVENER
if turnsleft <= 0 then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE center("        "BLUCYA"   You must like this game!   "RESET,78)
   CALL MESSAGE " "RESET
   CALL MESSAGE RED center("But anyway, you have played all your 25 turns out.",78)
   CALL MESSAGE RED center("Try calling back tomarrow and playing again!",78)
   CALL MESSAGE " "
   prompt( RESET"Press "RED"RETURN"WHI" when finished: "RESET)
   junk=readstr()
   signal OUTTAHERE
  end
CALL MESSAGE CLS
CALL MESSAGE "                 "WHI"*"RED"                ____"RESET
CALL MESSAGE RED" __ "WHI" *"RED"      ____  ___     /\  "WHI" *"RED" / __/"RESET
CALL MESSAGE RED" \ \    __ /___/ /___\   //\\   / /"RESET
CALL MESSAGE RED"  \ \  / ///_   //  "WHI"*"RED"\\ / \/ \ / /"RESET
CALL MESSAGE RED"   \ \/ //__/   \\___/// ____ \\ \___ "WHI"*"RESET
CALL MESSAGE RED"    \  / \\_____ \__ // /  __\ \\_  /"RESET
CALL MESSAGE WHI"  * "RED" \/   \____/___// \/   \__\/___/"RESET
CALL MESSAGE WHI"        *  "RED"/_______/"BLU" __________________________"RESET
CALL MESSAGE BLU"                    /------"ITAL||CYA"What To Play"RESET||BLU"-------/ "
CALL MESSAGE BLU"                   /                         / "
CALL MESSAGE BLU"                  /     "ITAL" ["RESET||YEL"S"ITAL||BLU"] "CYA"Slots"RESET||BLU"          / "
CALL MESSAGE BLU"                 /     "ITAL" ["RESET||YEL"L"ITAL||BLU"] "CYA"Lotto"RESET||BLU"          / "
CALL MESSAGE BLU"                /     "ITAL" ["RESET||YEL"C"ITAL||BLU"] "CYA"Craps"RESET||BLU"          / "
CALL MESSAGE BLU"               /     "ITAL" ["RESET||YEL"F"ITAL||BLU"] "CYA"Fifty-Fifty"RESET||BLU"    / "
CALL MESSAGE BLU"              /     "ITAL" ["RESET||YEL"Q"ITAL||BLU"] "CYA"Leave Vegas"RESET||BLU"    / "
CALL MESSAGE BLU"             /_________________________/ "
CALL MESSAGE " "RESET
prompt( CYA"What game do you want to play?"BLU"> "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice = "Q" then do
turnsleft = turnsleft + 1
signal MENU
end
if Choice = "S" then do
turnsleft = turnsleft - 1
call SLITS
end
if Choice = "L" then do
turnsleft = turnsleft - 1
signal LOTTO
end
if Choice = "C" then do
turnsleft = turnsleft - 1
call CRAPS
end
if Choice = "F" then do
turnsleft = turnsleft - 1
signal FIFTY
end
signal VEGAS

FIFTY:
if money <= 0 then do
   turnsleft = turnsleft + 1
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU NEED MONEY TO PLAY!",78)
   CALL MESSAGE " "
   call delay(150)
   signal VEGAS
  end
t = b
CALL MESSAGE CLS
CALL MESSAGE "                        "BLUCYA"   Playing Fifty-Fifty   "RESET
CALL MESSAGE " "
CALL MESSAGE CYA center("You have a 50% chance of winning double your bet.",78)
CALL MESSAGE " "
prompt( BLU" You have $"CYA||money||BLU", your bet (Max $"CYA"5000"BLU")?"RESET)
Betty=readstr()
if checkBBS() then signal BYE
if Betty = 'Q' then signal VEGAS
if Betty > money then do
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU DON'T HAVE ENOUGH MONEY TO BET THAT MUCH!",78)
   CALL MESSAGE " "
   call delay(110)
   signal FIFTY
  end
if Betty > 5000 | Betty < 1 then do
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("You must bet between 1 and 5000!",78)
   CALL MESSAGE " "
   call delay(110)
   signal FIFTY
  end
if Betty <= 5000 & Betty >= 1 then signal LATER
else call YO
signal FIFTY

LATER:
CALL MESSAGE " "
rnm = RANDOM(1,4,Time('s'))
if rnm = 1 | rnm = 3 then do
   money = money - Betty
   CALL MESSAGE YEL" You lost this time, bud!"
   CALL MESSAGE MAG" You lost $"WHI||Betty||MAG"."RESET
   CALL MESSAGE " "
   call delay(150)
   signal PLAYGEN
  end
if rnm = 2 | rnm = 4 then do
   CALL MESSAGE YEL" You Won!!!"
   CALL MESSAGE MAG" You won $"WHI||Betty*2||MAG"!"RESET
   money=money+(Betty*2)
   CALL MESSAGE " "
   call delay(150)
   signal PLAYGEN
  end
signal PLAYGEN

LOTTO:
if money <= 0 then do
   turnsleft = turnsleft + 1
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU NEED MONEY TO PLAY THIS GAME!",78)
   CALL MESSAGE " "
   call delay(150)
   signal VEGAS
  end
t = a
CALL MESSAGE CLS
CALL MESSAGE "                         "BLUCYA"   Playing the Lotto   "RESET
CALL MESSAGE " "
flag = f
CALL MESSAGE BLU" You have $"CYA||money||BLU".  Maximum bet is $"CYA"5000"BLU"."
prompt( CYA" How much money do you want to bet?: "RESET)
Wager=readstr()
if checkBBS() then signal BYE
if Wager = "Q" then signal VEGAS
if Wager > money then do
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU DON'T HAVE ENOUGH MONEY TO BET THAT MUCH!",78)
   CALL MESSAGE " "
   call delay(110)
   signal LOTTO
  end
if Wager > 5000 then do
   CALL MESSAGE " "
   CALL MESSAGE RED center("MAXIMUM BET IS $5000!",78)
   CALL MESSAGE " "
   call delay (110)
   signal LOTTO
  end
if Wager >= 1 & Wager <= 5000 then signal WEZ
else call YO
signal LOTTO

WEZ:
CALL MESSAGE " "
prompt( CYA" Bet "YEL||Wager||CYA" ("GRE"Y"CYA"/"RED"n"CYA")?: ")
okeydokey=readstr()
if checkBBS() then signal BYE
if okeydokey = "N" then signal LOTTO
signal GOON

GOON:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE CYA center("Enter a number between 1 and 50, if that number comes up, you win",78)
CALL MESSAGE CYA center("five times that amount, if you lose, you get one last chance to",78)
CALL MESSAGE CYA center("enter a number between 1 and 50, if you win, you will get triple",78)
CALL MESSAGE CYA center("your bet.  If you win any of the two, you get a chance to go on",78)
CALL MESSAGE CYA center("to the finals where you get a chance to win 10 times your bet, but",78)
CALL MESSAGE CYA center("your chances are one in 60.",78)
CALL MESSAGE " "
if flag = lasttime then do
   CALL MESSAGE GRE center("This is your last chance to win your money back,",78)
   CALL MESSAGE GRE center("if you don't win, you just lose what you originally",78)
   CALL MESSAGE GRE center("betted, you won't lose more than you betted.",78)
   CALL MESSAGE " "
  end
prompt( MAG"   Enter a number between 1 & 50: ")
numb=readstr()
if checkBBS() then signal BYE
if numb <= 50 & numb >= 1 then signal WEENIE
else call YO
signal GOON

WEENIE:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE " "
rannum = RANDOM(1,50,Time('s'))
CALL NOCR MAG " And, the number is... "RESET
call delay(50)
CALL MESSAGE rannum||MAG"!"RESET
CALL MESSAGE " "
if rannum = numb then do
   CALL MESSAGE YEL center("You Won!!!",78)
   CALL MESSAGE GRE center("       You won $"RESET||Wager*5||GRE"!!!",78)
   CALL MESSAGE MAG center("Good Job!!",78)
   if flag = f then money=money+(Wager*5)
   if flag = lasttime then money=money+(Wager*3)
   signal FINALDRAW
  end
if rannum ~= numb then do
   if flag = f then money = money - Wager
   CALL MESSAGE YEL center("Oh, to bad, you lose!",78)
   if flag = f then CALL MESSAGE GRE center("        You lost $"WHI||Wager||GRE".",78)
   if flag = lasttime then CALL MESSAGE GRE center("You didn't lose anything this time.",78)
   CALL MESSAGE MAG center("Oh well, better luck next time!",78)
   if flag = lasttime then do
      CALL MESSAGE RED center("You didn't win your money back!",78)
      call delay(170)
      signal PLAYGEN
  end
signal LASTCHANCE

FINALDRAW:
call delay(150)
randu = RANDOM(1,60,Time('s'))
CALL MESSAGE CLS
CALL MESSAGE RESET"    *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *"
CALL MESSAGE " "
CALL MESSAGE "    *  "RED" You have made made it this far, now lets see if you"WHI"  *"
CALL MESSAGE RED"       can win the REALLY BIG bucks!"
CALL MESSAGE WHI"    *                                                        *"
CALL MESSAGE "    *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *"RESET
CALL MESSAGE " "
call delay (200)
CALL MESSAGE BLU center("Using your winning number for the draw.",78)
CALL MESSAGE " "
CALL NOCR MAG"And... The winning number is... "RESET
call delay(50)
CALL MESSAGE randu||MAG"!"RESET
CALL MESSAGE " "
if randu = numb then do
   CALL MESSAGE YEL center("YOU WIN!!!",78)
   CALL MESSAGE GRE center("       You won $"WHI||Wager*10||GRE"!!",78)
   CALL MESSAGE RED center("GOOD JOB!",78)
   money=money+(Wager*10)
   CALL MESSAGE " "
   call writeln(news,handle' wins 'wager*10' in the lotto at vegas!')
   CALL MESSAGE CYA"The bright-red faced casino manager quickly ushers you out the door."
   CALL MESSAGE " "
   call delay(300)
   signal PLAYGEN
  end
if randu ~= numb then do
   CALL MESSAGE YEL center("Awww.. too bad, you lost.",78)
   CALL MESSAGE GRE center("But at least you didn't lose anything!",78)
   CALL MESSAGE RED center("Better luck next time!",78)
   CALL MESSAGE " "
   CALL MESSAGE CYA"The relieved casino manager asks you to come back anytime."
   CALL MESSAGE " "
   call delay(200)
   signal PLAYGEN
  end
signal PLAYGEN

PLAYGEN:
CALL MESSAGE CLS
CALL MESSAGE " "
prompt( CYA" Would you like to play again ("GRE"y"CYA"/"RED"N"CYA")? ")
comegen=readstr()
if checkBBS() then signal BYE
if comegen = "N" then signal VEGAS
if comegen = "Y" then do
   if t = a then signal LOTTO
   if t = b then signal FIFTY
end
signal VEGAS

LASTCHANCE:
call delay(150)
flag = lasttime
signal GOON

YO:
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOUR ANSWER MUST BE A NUMBER BETWEEN 1 AND 50!",78)
   CALL MESSAGE " "
   call delay(110)
   return

BUYORSELLW:
CALL MESSAGE CLS
CALL MESSAGE " "
if weapon~='' then CALL MESSAGE CYA" Your current weapon is a "weapon"."RESET
/*
if weapon = wep1 then CALL MESSAGE CYA" Your current weapon is a "wep1"."RESET
if weapon = wep2 then CALL MESSAGE CYA" Your current weapon is a "wep2"."RESET
if weapon = wep3 then CALL MESSAGE CYA" Your current weapon is a "wep3"."RESET
if weapon = wep4 then CALL MESSAGE CYA" Your current weapon is a "wep4"."RESET
if weapon = wep5 then CALL MESSAGE CYA" Your current weapon is a "wep5"."RESET
if weapon = wep6 then CALL MESSAGE CYA" Your current weapons are "wep6"."RESET
if weapon = wep7 then CALL MESSAGE CYA" Your current weapon is a "wep7"."RESET
if weapon = wep8 then CALL MESSAGE CYA" Your current weapon is a "wep8"."RESET
if weapon = wep9 then CALL MESSAGE CYA" Your current weapon is a "wep9"."RESET
if weapon = wep10 then CALL MESSAGE CYA " Your current weapon is a "wep10"."RESET
if weapon = wep11 then CALL MESSAGE CYA " Your current weapon is a "wep11"."RESET
if weapon = wep12 then CALL MESSAGE CYA " Your current weapon is a "wep12"."RESET
*/
CALL MESSAGE " "
prompt( GRE" Buy or Sell your weapon (B/S)? "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice = B then signal BUYWEAPON
if Choice = S then signal SELLWEAPON
if Choice = Q then signal MENU
signal MENU

BUYORSELLA:
CALL MESSAGE CLS
CALL MESSAGE " "
if armor~='' THEN  CALL MESSAGE CYA" Your current vehicle is a "armor"."RESET
/*
if armor = arm1 then CALL MESSAGE CYA" Your current vehicle is a "arm1"."RESET
if armor = arm2 then CALL MESSAGE CYA" Your current vehicle is a "arm2"."RESET
if armor = arm3 then CALL MESSAGE CYA" Your current vehicle is a "arm3"."RESET
if armor = arm4 then CALL MESSAGE CYA" Your current vehicle is a "arm4"."RESET
if armor = arm5 then CALL MESSAGE CYA" Your current vehicle is a "arm5"."RESET
if armor = arm6 then CALL MESSAGE CYA" Your current vehicle is a "arm6"."RESET
if armor = arm7 then CALL MESSAGE CYA" Your current vehicle is a "arm7"."RESET
if armor = arm8 then CALL MESSAGE CYA" Your current vehicle is a "arm8"."RESET
if armor = arm9 then CALL MESSAGE CYA" Your current vehicle is a "arm9"."RESET
if armor = arm10 then CALL MESSAGE CYA " Your current vehicle is a "arm10"."RESET
if armor = arm11 then CALL MESSAGE CYA " Your current vehicle is a "arm11"."RESET
if armor = arm12 then CALL MESSAGE CYA " Your current vehicle is a "arm12"."RESET
*/
CALL MESSAGE " "
prompt( GRE" Buy or Sell your vehicle (B/S)? "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice = B then signal BUYARMOR
if Choice = S then signal SELLARMOR
if Choice = Q then signal MENU
signal MENU

BUYWEAPON:
w = b
CALL MESSAGE CLS
CALL MESSAGE "                   "BluCya"  Macy's Department Store Weapons  "RESET
CALL MESSAGE BLU" ----------------------------------------------------------------------"
CALL MESSAGE YEL"   A."MAG" Squirt Gun                         "RED"Rank:"MAG" 1   "GRE" Price:"MAG" $100   "
CALL MESSAGE YEL"   B."MAG" Spitwad Launcher                   "RED"Rank:"MAG" 2   "GRE" Price:"MAG" $2000  "
CALL MESSAGE YEL"   C."MAG" Frisbee                            "RED"Rank:"MAG" 3   "GRE" Price:"MAG" $10000  "
CALL MESSAGE YEL"   D."MAG" Laser Tag Gun                      "RED"Rank:"MAG" 4   "GRE" Price:"MAG" $25000 "
CALL MESSAGE YEL"   E."MAG" Paint Ball Gun                     "RED"Rank:"MAG" 5   "GRE" Price:"MAG" $40000 "
CALL MESSAGE YEL"   F."MAG" Darts                              "RED"Rank:"MAG" 6   "GRE" Price:"MAG" $55000 "
CALL MESSAGE YEL"   G."MAG" Baseball                           "RED"Rank:"MAG" 7   "GRE" Price:"MAG" $75000 "
CALL MESSAGE YEL"   H."MAG" Bart Simpson Doll                  "RED"Rank:"MAG" 8   "GRE" Price:"MAG" $85000 "
CALL MESSAGE YEL"   I."MAG" Chainsaw                           "RED"Rank:"MAG" 9   "GRE" Price:"MAG" $100000"
CALL MESSAGE YEL"   J."MAG" Teenage Mutant Ninja Turtles Doll  "RED"Rank:"MAG" 10  "GRE" Price:"MAG" $200000"
CALL MESSAGE YEL"   K."MAG" New Kids On The Block Poster       "RED"Rank:"MAG" 11  "GRE" Price:"MAG" $420000"
CALL MESSAGE YEL"   L."MAG" New Kids On The Block CD           "RED"Rank:"MAG" 12  "GRE" Price:"MAG" $750000"RESET
CALL MESSAGE YEL"   M."MAG" Atomic Can-Opener                  "RED"Rank:"MAG" 13  "GRE" Price:"MAG" $1400000"RESET
CALL MESSAGE YEL"   N."MAG" Guile (here you can buy Wisdom?-)  "RED"Rank:"MAG" 14  "GRE" Price:"MAG" $3000000"RESET
CALL MESSAGE " "
CALL MESSAGE WHI"                               Press Q To Quit      "
CALL MESSAGE " "
prompt( CYA"You have $"WHI||money||CYA".  What weapon do you want?"BLU"> "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice = "Q" then signal MENU
if Choice = "A" then do
   if weapon = wep1 then signal ALREADYHAVE
   if money < 10 then signal NOMONEY
   money = money - 10
   weapon = wep1
   call BOT
  end
if Choice = "B" then do
   if weapon = wep2 then signal ALREADYHAVE
   if money < 2000 then signal NOMONEY
   money = money - 2000
   weapon = wep2
   call BOT
  end
if Choice = "C" then do
   if weapon = wep3 then signal ALREADYHAVE
   if money < 10000 then signal NOMONEY
   money = money - 10000
   weapon = wep3
   call BOT
  end
if Choice = "D" then do
   if weapon = wep4 then signal ALREADYHAVE
   if money < 25000 then signal NOMONEY
   money = money - 25000
   weapon = wep4
   call BOT
  end
if Choice = "E" then do
   if weapon = wep5 then signal ALREADYHAVE
   if money < 40000 then signal NOMONEY
   money = money - 40000
   weapon = wep5
   call BOT
  end
if Choice = "F" then do
   if weapon = wep6 then signal ALREADYHAVE
   if money < 55000 then signal NOMONEY
   money = money - 55000
   weapon = wep6
   call BOT
  end
if Choice = "G" then do
   if weapon = wep7 then signal ALREADYHAVE
   if money < 75000 then signal NOMONEY
   money = money - 75000
   weapon = wep7
   call BOT
  end
if Choice = "H" then do
   if weapon = wep8 then signal ALREADYHAVE
   if money < 85000 then signal NOMONEY
   money = money - 85000
   weapon = wep8
   call BOT
  end
if Choice = "I" then do
   if weapon = wep9 then signal ALREADYHAVE
   if money < 100000 then signal NOMONEY
   money = money - 100000
   weapon = wep9
   call BOT
  end
if Choice = "J" then do
   if weapon = wep10 then signal ALREADYHAVE
   if money < 200000 then signal NOMONEY
   money = money - 200000
   weapon = wep10
   call BOT
  end
if Choice = "K" then do
   if weapon = wep11 then signal ALREADYHAVE
   if money < 420000 then signal NOMONEY
   money = money - 420000
   weapon = wep11
   call BOT
  end
if Choice = "L" then do
   if weapon = wep12 then signal ALREADYHAVE
   if money < 750000 then signal NOMONEY
   money = money - 750000
   weapon = wep12
   call BOT
  end
if Choice = "M" then do
   if weapon = wep13 then signal ALREADYHAVE
   if money < 1400000 then signal NOMONEY
   money = money - 1400000
   weapon = wep13
   call BOT
  end
if Choice = "N" then do
   if weapon = wep14 then signal ALREADYHAVE
   if money < 3000000 then signal NOMONEY
   money = money - 3000000
   weapon = wep14
   call BOT
  end
signal BUYWEAPON

BUYARMOR:
w = a
CALL MESSAGE CLS
CALL MESSAGE "                  "BluCya"  Macy's Department Store Vehicles  "RESET
CALL MESSAGE BLU" ----------------------------------------------------------------------"
CALL MESSAGE YEL"   A."MAG" Barbie Ferrari                     "RED"Rank:"MAG" 1   "GRE" Price:"MAG" $100   "
CALL MESSAGE YEL"   B."MAG" Roller Skates                      "RED"Rank:"MAG" 2   "GRE" Price:"MAG" $2000  "
CALL MESSAGE YEL"   C."MAG" Vision Skateboard                  "RED"Rank:"MAG" 3   "GRE" Price:"MAG" $10000  "
CALL MESSAGE YEL"   D."MAG" Go-Cart                            "RED"Rank:"MAG" 4   "GRE" Price:"MAG" $25000 "
CALL MESSAGE YEL"   E."MAG" 10-Speed Bike                      "RED"Rank:"MAG" 5   "GRE" Price:"MAG" $40000 "
CALL MESSAGE YEL"   F."MAG" Suzuki Motorbike                   "RED"Rank:"MAG" 6   "GRE" Price:"MAG" $55000 "
CALL MESSAGE YEL"   G."MAG" BMW Motorcycle                     "RED"Rank:"MAG" 7   "GRE" Price:"MAG" $75000 "
CALL MESSAGE YEL"   H."MAG" Yugo                               "RED"Rank:"MAG" 8   "GRE" Price:"MAG" $85000 "
CALL MESSAGE YEL"   I."MAG" Volkswagon                         "RED"Rank:"MAG" 9   "GRE" Price:"MAG" $100000"
CALL MESSAGE YEL"   J."MAG" Ferrari F40                        "RED"Rank:"MAG" 10  "GRE" Price:"MAG" $200000"
CALL MESSAGE YEL"   K."MAG" Corvette Mako Shark                "RED"Rank:"MAG" 11  "GRE" Price:"MAG" $420000"
CALL MESSAGE YEL"   L."MAG" Stealth Bomber                     "RED"Rank:"MAG" 12  "GRE" Price:"MAG" $750000"RESET
CALL MESSAGE YEL"   M."MAG" Nulcear Submarine                  "RED"Rank:"MAG" 13  "GRE" Price:"MAG" $1400000"RESET
CALL MESSAGE YEL"   N."MAG" Starship USS Enterprise            "RED"Rank:"MAG" 14  "GRE" Price:"MAG" $3000000"RESET
CALL MESSAGE " "
CALL MESSAGE WHI"                               Press Q To Quit      "
CALL MESSAGE " "
prompt( CYA"You have $"WHI||money||CYA".  What vehicle do you want?"BLU"> "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice = "Q" then signal MENU
if Choice = "A" then do
   if armor = arm1 then signal ALREADYHAVEA
   if money < 10 then signal NOMONEYA
   money = money - 10
   armor = arm1
   call BOT
  end
if Choice = "B" then do
   if armor = arm2 then signal ALREADYHAVEA
   if money < 2000 then signal NOMONEYA
   money = money - 2000
   armor = arm2
   call BOT
  end
if Choice = "C" then do
   if armor = arm3 then signal ALREADYHAVEA
   if money < 10000 then signal NOMONEYA
   money = money - 10000
   armor = arm3
   call BOT
  end
if Choice = "D" then do
   if armor = arm4 then signal ALREADYHAVEA
   if money < 25000 then signal NOMONEYA
   money = money - 25000
   armor = arm4
   call BOT
  end
if Choice = "E" then do
   if armor = arm5 then signal ALREADYHAVEA
   if money < 40000 then signal NOMONEYA
   money = money - 40000
   armor = arm5
   call BOT
  end
if Choice = "F" then do
   if armor = arm6 then signal ALREADYHAVEA
   if money < 55000 then signal NOMONEYA
   money = money - 55000
   armor = arm6
   call BOT
  end
if Choice = "G" then do
   if armor = arm7 then signal ALREADYHAVEA
   if money < 75000 then signal NOMONEYA
   money = money - 75000
   armor = arm7
   call BOT
  end
if Choice = "H" then do
   if armor = arm8 then signal ALREADYHAVEA
   if money < 85000 then signal NOMONEYA
   money = money - 85000
   armor = arm8
   call BOT
  end
if Choice = "I" then do
   if armor = arm9 then signal ALREADYHAVEA
   if money < 100000 then signal NOMONEYA
   money = money - 100000
   armor = arm9
   call BOT
  end
if Choice = "J" then do
   if armor = arm10 then signal ALREADYHAVEA
   if money < 200000 then signal NOMONEYA
   money = money - 200000
   armor = arm10
   call BOT
  end
if Choice = "K" then do
   if armor = arm11 then signal ALREADYHAVEA
   if money < 420000 then signal NOMONEYA
   money = money - 420000
   armor = arm11
   call BOT
  end
if Choice = "L" then do
   if armor = arm12 then signal ALREADYHAVEA
   if money < 750000 then signal NOMONEYA
   money = money - 750000
   armor = arm12
   call BOT
  end
if Choice = "M" then do
   if armor = arm13 then signal ALREADYHAVEA
   if money < 1400000 then signal NOMONEYA
   money = money - 1400000
   armor = arm13
   call BOT
  end
if Choice = "N" then do
   if armor = arm14 then signal ALREADYHAVEA
   if money < 3000000 then signal NOMONEYA
   money = money - 3000000
   armor = arm14
   call BOT
  end
signal BUYARMOR

BOT:
CALL MESSAGE CLS
CALL MESSAGE " "
if w = a then CALL MESSAGE MAG center("You have bought a "WHI||armor||MAG"!",78)
if w = b then CALL MESSAGE MAG center("You have bought a "WHI||weapon||MAG"!",78)
CALL MESSAGE " "
call delay(110)
return

SELLWEAPON:
CALL MESSAGE " "
CALL MESSAGE MAG" Macy's will take your weapon for half price."RESET
CALL MESSAGE " "
prompt( YEL"Are You Sure (y/N)? "RESET)
Choice=readstr()
if checkBBS() then signal BYE
CALL MESSAGE " "
if Choice = "Y" then do
   if weapon = wep1 then do
      CALL MESSAGE RED"Macy's won't take that piece of junk!"
      call delay(110)
      signal MENU
     end
   if weapon = wep2 then do
      CALL MESSAGE CYA"Macy's pays you $1000!"
      weapon = wep1
      money = money + 1000
      call delay(110)
      signal MENU
     end
   if weapon = wep3 then do
      CALL MESSAGE CYA"Macy's pays you $5000!"
      weapon = wep1
      money = money + 5000
      call delay(110)
      signal MENU
     end
   if weapon = wep4 then do
      CALL MESSAGE CYA"Macy's pays you $12500!"
      weapon = wep1
      money = money + 12500
      call delay(110)
      signal MENU
     end
   if weapon = wep5 then do
      CALL MESSAGE CYA"Macy's pays you $20000!"
      weapon = wep1
      money = money + 20000
      call delay(110)
      signal MENU
     end
   if weapon = wep6 then do
      CALL MESSAGE CYA"Macy's pays you $27500!"
      weapon = wep1
      money = money + 27500
      call delay(110)
      signal MENU
     end
   if weapon = wep7 then do
      CALL MESSAGE CYA"Macy's pays you $37500!"
      weapon = wep1
      money = money + 37500
      call delay(110)
      signal MENU
     end
   if weapon = wep8 then do
      CALL MESSAGE CYA"Macy's pays you $42500!"
      weapon = wep1
      money = money + 42500
      call delay(110)
      signal MENU
     end
   if weapon = wep9 then do
      CALL MESSAGE CYA"Macy's pays you $50000!"
      weapon = wep1
      money = money + 50000
      call delay(110)
      signal MENU
     end
   if weapon = wep10 then do
      CALL MESSAGE CYA"Macy's pays you $100000!"
      weapon = wep1
      money = money + 100000
      call delay(110)
      signal MENU
     end
   if weapon = wep11 then do
      CALL MESSAGE CYA"Macy's pays you $210000!"
      weapon = wep1
      money = money + 210000
      call delay(110)
      signal MENU
     end
   if weapon = wep12 then do
      CALL MESSAGE CYA"Macy's pays you $375000!"
      weapon = wep1
      money = money + 375000
      call delay(110)
      signal MENU
     end
   if weapon = wep13 then do
      CALL MESSAGE CYA"Macy's pays you $700000!"
      weapon = wep1
      money = money + 700000
      call delay(110)
      signal MENU
     end
   if weapon = wep14 then do
      CALL MESSAGE CYA"Macy's pays you $1500000!"
      weapon = wep1
      money = money + 1500000
      call delay(110)
      signal MENU
     end
   end
signal MENU

SELLARMOR:
CALL MESSAGE " "
CALL MESSAGE MAG" Macy's will take your vehicle for half price."RESET
CALL MESSAGE " "
prompt( YEL"Are You Sure (y/N)? "RESET)
Choice=readstr()
if checkBBS() then signal BYE
CALL MESSAGE " "
if Choice = "Y" then do
   if armor = arm1 then do
      CALL MESSAGE RED"Macy's won't take that piece of junk!"
      call delay(110)
      signal MENU
     end
   if armor = arm2 then do
      CALL MESSAGE CYA"Macy's pays you $1000!"
      armor = arm1
      money = money + 1000
      call delay(110)
      signal MENU
     end
   if armor = arm3 then do
      CALL MESSAGE CYA"Macy's pays you $5000!"
      armor = arm1
      money = money + 5000
      call delay(110)
      signal MENU
     end
   if armor = arm4 then do
      CALL MESSAGE CYA"Macy's pays you $12500!"
      armor = arm1
      money = money + 12500
      call delay(110)
      signal MENU
     end
   if armor = arm5 then do
      CALL MESSAGE CYA"Macy's pays you $20000!"
      armor = arm1
      money = money + 20000
      call delay(110)
      signal MENU
     end
   if armor = arm6 then do
      CALL MESSAGE CYA"Macy's pays you $27500!"
      armor = arm1
      money = money + 27500
      call delay(110)
      signal MENU
     end
   if armor = arm7 then do
      CALL MESSAGE CYA"Macy's pays you $37500!"
      armor = arm1
      money = money + 37500
      call delay(110)
      signal MENU
     end
   if armor = arm8 then do
      CALL MESSAGE CYA"Macy's pays you $42500!"
      armor = arm1
      money = money + 42500
      call delay(110)
      signal MENU
     end
   if armor = arm9 then do
      CALL MESSAGE CYA"Macy's pays you $50000!"
      armor = arm1
      money = money + 50000
      call delay(110)
      signal MENU
     end
   if armor = arm10 then do
      CALL MESSAGE CYA"Macy's pays you $100000!"
      armor = arm1
      money = money + 100000
      call delay(110)
      signal MENU
     end
   if armor = arm11 then do
      CALL MESSAGE CYA"Macy's pays you $210000!"
      armor = arm1
      money = money + 210000
      call delay(110)
      signal MENU
     end
   if armor = arm12 then do
      CALL MESSAGE CYA"Macy's pays you $375000!"
      armor = arm1
      money = money + 375000
      call delay(110)
      signal MENU
     end
   if armor = arm13 then do
      CALL MESSAGE CYA"Macy's pays you $700000!"
      armor = arm1
      money = money + 700000
      call delay(110)
      signal MENU
     end
   if armor = arm14 then do
      CALL MESSAGE CYA"Macy's pays you $1500000!"
      armor = arm1
      money = money + 1500000
      call delay(110)
      signal MENU
     end
   end
signal MENU

ALREADYHAVE:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED" YOU ALREADY HAVE THAT WEAPON! "RESET
call delay(150)
signal BUYWEAPON

NOMONEY:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED" YOU DON'T HAVE ENOUGH MONEY TO BUY THAT WEAPON!"
if money = 0 then CALL MESSAGE MAG" --You don't have any money!"
call delay(150)
signal BUYWEAPON

ALREADYHAVEA:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED" YOU ALREADY HAVE THAT VEHICLE! "RESET
call delay(150)
signal BUYARMOR

P2:
CALL MESSAGE " "
prompt( WHI"O"RED"k"GRE" D"YEL"a"BLU"v"MAG"i"CYA"d"WHI","RED" e"GRE"n"YEL"t"BLU"e"MAG"r"CYA" p"WHI"a"RED"s"YEL"s"BLU"w"MAG"o"CYA"r"WHI"d "RED"#"GRE"2"YEL"!"BLU": ")
pass=readstr()
if checkBBS() then signal BYE
if pass = "206-744-1254" then signal CHEATIT
signal MENU

NOMONEYA:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE RED" YOU DON'T HAVE ENOUGH MONEY TO BUY THAT VEHICLE!"
if money = 0 then CALL MESSAGE MAG" --You don't have any money!"
call delay(150)
signal BUYARMOR

QUIT:
CALL MESSAGE " "
prompt( CYA"Are you sure ("GRE"n"CYA"/"RED"Y"CYA")? "RESET)
Choice=readstr()
if checkBBS() then signal BYE
if Choice~='N' then do

QUIT2:
   CALL MESSAGE CLS
   call close(news)
   CALL MESSAGE " "
   CALL MESSAGE BluCya"Saving Stats..."RESET
   call POINTCK
   call open(Fill,datapath||handle'.DAT','W')
   call writeln(Fill,turnsleft)
   call writeln(Fill,maxpoints)
   call writeln(Fill,hitpoints)
   call writeln(Fill,points)
   call writeln(Fill,money)
   call writeln(Fill,armor)
   call writeln(Fill,weapon)
   call writeln(Fill,rank)
   call writeln(Fill,canfight)
   call writeln(Fill,date)
   call writeln(Fill,deadf)
   call writeln(Fill,killer)
   call writeln(Fill,kills)
   call writeln(Fill,bmoney)
   call close(Fill)
   CALL MESSAGE " "
   CALL MESSAGE BluCya"Saved."RESET
   CALL MESSAGE " "
   CALL MESSAGE GRE"Calculating High Scores..."
   call CHECKHI
   CALL MESSAGE " "
   CALL MESSAGE RED center("Returning to "bbsname"...",78)
   CALL MESSAGE RESET
   call delay(110)
   signal DIE
end
CALL MESSAGE " "
CALL MESSAGE CYA"Returning To Main..."RESET
turnsleft = turnsleft + 1
call delay(30)
signal MENU

OUTTAHERE:
CALL MESSAGE CLS
CALL MESSAGE " "
call close(news)
CALL MESSAGE BluCya"Saving Stats..."RESET
call POINTCK
call open(Fill,datapath||handle'.DAT','W')
call writeln(Fill,turnsleft)
call writeln(Fill,maxpoints)
call writeln(Fill,hitpoints)
call writeln(Fill,points)
call writeln(Fill,money)
call writeln(Fill,armor)
call writeln(Fill,weapon)
call writeln(Fill,rank)
call writeln(Fill,canfight)
call writeln(Fill,date)
call writeln(Fill,deadf)
call writeln(Fill,killer)
call writeln(Fill,kills)
call writeln(Fill,bmoney)
call close(Fill)
CALL MESSAGE " "
CALL MESSAGE BluCya"Saved."RESET
CALL MESSAGE " "
CALL MESSAGE GRE"Calculating High Scores..."
call CHECKHI
CALL MESSAGE " "
CALL MESSAGE RED center("Returning to "bbsname"...",78)
CALL MESSAGE RESET
call delay(110)
signal DIE

BYE:
call close(news)
call POINTCK
call open(File,datapath||handle'.DAT','W')
call writeln(File,turnsleft)
call writeln(File,maxpoints)
call writeln(File,hitpoints)
call writeln(File,points)
call writeln(File,money)
call writeln(File,armor)
call writeln(File,weapon)
call writeln(File,rank)
call writeln(File,canfight)
call writeln(File,date)
call writeln(File,deadf)
call writeln(File,killer)
call writeln(File,kills)
call writeln(File,bmoney)
call close(File)
call CHECKHI
signal DIE

POINTCK:
call EVENER
if turnsleft = '' then turnsleft = 2
if maxturns = 21 then maxturns = 26
if maxturns = '' then maxturns = 26
if canfight = '' then canfight = 4
if deadf = '' then deadf = 0
if killer = '' then killer = "NOBODY"
if weapon = '' then weapon = wep1
if weapon = wep1 then po1 = 100
if weapon = wep2 then po1 = 200
if weapon = wep3 then po1 = 400
if weapon = wep4 then po1 = 800
if weapon = wep5 then po1 = 1600
if weapon = wep6 then po1 = 3200
if weapon = wep7 then po1 = 6400
if weapon = wep8 then po1 = 12800
if weapon = wep9 then po1 = 25600
if weapon = wep10 then po1 = 51200
if weapon = wep11 then po1 = 102400
if weapon = wep12 then po1 = 204800
if weapon = wep13 then po1 = 409600
if weapon = wep14 then po1 = 819200
if armor = '' then armor = arm1
if armor = arm1 then po2 = 100
if armor = arm2 then po2 = 200
if armor = arm3 then po2 = 400
if armor = arm4 then po2 = 800
if armor = arm5 then po2 = 1600
if armor = arm6 then po2 = 3200
if armor = arm7 then po2 = 6400
if armor = arm8 then po2 = 12800
if armor = arm9 then po2 = 25600
if armor = arm10 then po2 = 51200
if armor = arm11 then po2 = 102400
if armor = arm12 then po2 = 204800
if armor = arm13 then po2 = 409600
if armor = arm14 then po2 = 819200
if bmoney = '' then bmoney = 0
if money = '' then money = 0
po3 = money + bmoney
if rank = '' then rank = ran1
if rank = ran1 then po4 = 500
if rank = ran2 then po4 = 1000
if rank = ran3 then po4 = 2000
if rank = ran4 then po4 = 4000
if rank = ran5 then po4 = 6000
if rank = ran6 then po4 = 8000
if rank = ran7 then po4 = 10000
if rank = ran8 then po4 = 15000
if rank = ran9 then po4 = 30000
if rank = ran10 then po4 = 45000
if rank = ran11 then po4 = 60000
if rank = ran12 then po4 = 80000
if rank = ran13 then po4 = 90000
if rank = ran14 then po4 = 100000
if hitpoints = '' then hitpoints = 30
po5 = hitpoints * 10
if maxpoints = '' then maxpoints = 30
po6 = maxpoints * 10
if ~DATATYPE(kills,'N') then kills = 0
po7 = kills * 1000
/* say po1'-'po2'-'po3'-'po4'-'po5'-'po6'-'po7 */
points=po1+po2+po3+po4+po5+po6+po7
return

CHECKHI:
call HICL
call open(play,datapath'Players','W')
do i = 1 to howm
   if PRank.i ~= -1 then do
    call writeln(play,PRank.i' 'PHandle.i)
   end
end
call close(play)
call open(dc,datapath'STATS','W')
call writeln(dc,date)
call writeln(dc,day)
call writeln(dc,howm)
call close(dc)
call HICK
call open(top10,datapath'TopOnes','W')
do i = 1 to 10
call writeln(top10,topscore.i' 'topalias.i)
end
call close(top10)
return

DIE:
exit

SLITS:           /* Thanks a lot for some of this routine Mike! */
ESC = '['
RESET = '[0m'
CLL = '                                                                     '
POS = '[2;5H'
POS1 = '[3;5H'
POS2 = '[4;5H'
POS3 = '[5;0H'
POS4 = '[6;5H'
POS5 = '[7;5H'
POS6 = '[8;5H'
POS7 = '[9;5H'
POS8 = '[10;5H'
POS9 = '[11;5H'
POS10 = '[12;5H'
POS11 = '[13;5H'
POS12 = '[14;5H'
POS13 = '[15;5H'
POS14 = '[16;5H'
POS15 = '[17;5H'
POS16 = '[18;5H'
POS17 = '[19;5H'
POS18 = '[20;5H'
POS19 = '[21;5H'
POS20 = '[22;5H'
POS21 = '[23;5H'
WHITEONRED = '[37;41m'
BLACKONYELLOW = '[30;43m'
YELLOWONBLUE = '[33;44m'
CYANONBLACK = '[36;40m'
YELLOWONBLACK = '[33;40m'
PURPLEONBLACK = '[35;40m'
BLUEONBLACK = '[34;40m'
REDONYELLOW = '[31;43m'
REDONBLUE = '[31;44m'
BLACK = '[30m'

bet = 0
Tl = 0
Dl = 0

BEAN:
CALL MESSAGE CLS
CALL MESSAGE " "
CALL MESSAGE "                          "BluCya"   Playing The Slots   "RESET
CALL MESSAGE " "
prompt( MAG"You have $"money" "BLU"["YEL"Q"BLU"]"MAG" Quits -- Your bet (Max 5000)?"BLU"> "RESET)
if checkBBS() then signal BYE
Bet=readstr()
if Bet = 0 | Bet = 'Q' then signal WHATTHEHELL
if Bet > money | Bet > 5000 then do
CALL MESSAGE " "
CALL MESSAGE "Sorry "handle", you can't bet that much!"
call delay(100)
signal BEAN
 end
 if Bet = '' | Bet = ' ' | Bet <= . then signal BEAN
else
CALL MESSAGE "  "
CALL MESSAGE POS3 CLL POS3 "Bet taken..."
signal BEANS

BEANS:
prompt( POS4 CLL POS4 CYA"Press 'P' to pull for your chance at big bucks! > ")
chr=readstr()
if checkBBS() then signal BYE
if chr = 'P' then do
CALL MESSAGE POS6 CLL POS6'     Your slots: '
call ROLL
numslots = 1
do for 3
call ROLL
numslots=numslots+1
end
call OUTPUT
numslots = 1
do for 3
call ROLL
numslots=numslots+1
end
call OUTPUT
numslots = 1
do for 3
call ROLL
numslots=numslots+1
end
call OUTPUT
numslots = 1
do for 3
call ROLL
numslots=numslots+1
end
call OUTPUT
call COMPARE
end
else signal WHATTHEHELL

ROLL:
slots.numslots = RANDOM(1,6,Time('s'))
slot.numslots = slots.numslots
  if slots.numslots = 1 then do
    slot.numslots = '='
  end
  if slots.numslots = 2 then do
    slot.numslots = '?'
  end
  if slots.numslots = 3 then do
    slot.numslots = '#'
  end
  if slots.numslots = 4 then do
    slot.numslots = '$'
  end
  if slots.numslots = 5 then do
    slot.numslots = '*'
  end
  if slots.numslots = 6 then do
    slot.numslots = '7'
  end
RETURN

OUTPUT:
if checkBBS() then signal BYE
CALL MESSAGE POS7 CLL POS7'                                             '
do i = 1 to numslots-1
CALL NOCR ' [33;44m -<->- [0m'
end
CALL MESSAGE ''
do i = 1 to numslots-1
CALL NOCR ' [33;40m|     |[0m'
end
CALL MESSAGE ''
do i = 1 to numslots-1
CALL NOCR ' [33;40m|  [36;40m'slot.i'[33;40m  |[0m'
end
CALL MESSAGE ''
do i = 1 to numslots-1
CALL NOCR ' [33;40m|     |[0m'
end
CALL MESSAGE ''
do i = 1 to numslots-1
CALL NOCR ' [33;40m|ooooo|[0m'
end
CALL MESSAGE ''
do i = 1 to numslots-1
CALL NOCR ' [33;44m  < >  [0m'
end
CALL MESSAGE ''
call delay(50)
RETURN

COMPARE:
if slot.1 = slot.2 & slot.1 ~= slot.3 then signal WINNER
if slot.2 = slot.3 & slot.2 ~= slot.1 then signal WINNER
if slot.1 = slot.2 & slot.1 = slot.3 then signal WINNER1
 else money = money - bet
if purse = 0 | purse < 0 then signal OUTTACASH
 else signal LOSER

WINNER:
CALL MESSAGE " "
CALL MESSAGE POS15 CLL POS15 "You won double your bet which was "bet*2"!"
CALL MESSAGE " "
money=money+(bet*2)
call DBL
signal WHATTHEHELL

WINNER1:
CALL MESSAGE " "
CALL MESSAGE POS15 CLL POS15 "You won triple your bet which was "bet*3"!"
CALL MESSAGE " "
money=money+(bet*3)
call TPL
signal WHATTHEHELL

LOSER:
CALL MESSAGE " "
CALL MESSAGE POS15 CLL POS15 "You didn't win your bet!"
call delay(100)
signal WHATTHEHELL

WHATTHEHELL:
CALL MESSAGE " "
prompt( POS17 CLL POS17 "Do you want to continue playing (Y/N)? ")
ans=readstr()
if checkBBS() then signal BYE
if ans = 'Y' then signal BEAN
if ans = 'N' then signal VEGAS
if ans ~= 'N' | ans ~= 'Y' then signal WHATTHEHELL
signal WHATTHEHELL

TPL:
Tl=Tl+1
if Tl = 5 then call BROKEN
RETURN

DBL:
Dl=Dl+1
if Dl = 8 then call BROKEN
RETURN

OUTTACASH:
CALL MESSAGE CLS
CALL MESSAGE "    "
CALL MESSAGE POS17 CLL POS17 "You ain't got no money left! Boy, you sure did a lousy job!"
CALL MESSAGE "    "
call delay(100)
CALL MESSAGE POS18 CLL POS18 "Well since you're outta cash, we'll help you outta here!"
signal VEGAS

BROKEN:
CALL MESSAGE CLS
CALL MESSAGE "    "
CALL MESSAGE POS14 CLL POS14 "The Casino Manager wants to talk to you now!"
CALL MESSAGE "    "
call delay(100)
CALL MESSAGE POS15 CLL POS15 "It appears he isn't too happy with your Playing Skills!"
call delay(100)
CALL MESSAGE POS16 CLL POS16 "I'm afraid I will have to ask you to leave now, you won too much!"
CALL MESSAGE POS17 CLL POS17 "You Broke the Bank!! Try again later if you feel lucky...."
CALL MESSAGE "     "
call delay(100)
signal VEGAS


checkBBS:
IF ADDRESS()~='BAUD' THEN RETURN 0
IF TIME('e')>secs THEN RETURN 1
dcd
IF RC=0 THEN RETURN 1
temp=secs-TIME('E')
IF temp<120 THEN SAY '*** Only' temp 'seconds left! ***'CR 
RETURN 0


CRAPS:          /* Thanks to D.F. Duck for some of this routine! */
if money <= 0 then do
   turnsleft = turnsleft + 1
   CALL MESSAGE CLS
   CALL MESSAGE " "
   CALL MESSAGE RED center("YOU NEED MONEY TO PLAY THIS GAME!",78)
   CALL MESSAGE " "
   call delay(150)
   return
  end
CALL MESSAGE CLS
CALL MESSAGE "                         "BluCya"   Playing Craps   "RESET
CALL MESSAGE " "
call MAIN
return

Main:
        RollNum = 0
        RollOld = 0
        call Bet
        call RollIt
return

Bet:
        CALL MESSAGE " "
                CALL MESSAGE BLU" You have $"CYA||money||BLU".  Maximum bet is $"CYA"5000"BLU"."RESET
                prompt( CYA" How much do you wish to bet "BLU"["YEL"Q"BLU"]"CYA" To Quit?: "RESET)
                TimeBet=readstr()
                if checkBBS() then signal BYE
                if TimeBet = "q" | TimeBet = "Q" then signal VEGAS
                if money < TimeBet then do
                   CALL MESSAGE CLS
                   CALL MESSAGE " "
                   CALL MESSAGE RED"              YOU DON'T HAVE ENOUGH MONEY TO BET THAT MUCH!"RESET
                   CALL MESSAGE " "
                   call delay(150)
                   call BET
                   return
                end
                if TimeBet = 1 then Minutes = "Dollar"
                else Minutes = "Dollars"
                if TimeBet < 1 then call BET
                if TimeBet > 5000 then call BET
                if TimeBet = "" | TimeBet = " " then signal VEGAS
return

RollIt:
   do forever
        CALL MESSAGE " "
        prompt( "Press ["RED"RETURN"RESET"] To Roll... ")
        YesPlay=readstr()
        if checkBBS() then signal BYE
        call Roller
    end
return

Roller:
        CALL MESSAGE CLS
        RollNum = RollNum + 1
        CALL MESSAGE "Roll #"RollNum
        CALL MESSAGE " "
        Rand1 = RANDOM(1,6,Time('s'))
        DicePos = ""
        DiceSet = insert("Dice",Rand1)
        interpret "call" DiceSet
        Rand2 = RANDOM(1,6,Time('s'))
        DicePos = "        "
        DiceSet = insert("Dice",Rand2)
        interpret "call" DiceSet
        CALL MESSAGE " "
        RandSum = Rand1 + Rand2
        CALL MESSAGE "You Rolled A "RandSum"."
        CALL MESSAGE " "
        if RandSum = 7 | RandSum = 11 then do
                if RollNum = 1 then call Win
                else call Lost
                end
        if RollNum = 1 then RollOld = RandSum
        if RollOld = RandSum then do
                if RollNum ~= 1 then call Win
                end
        CALL MESSAGE "You Need Another "GRE||RollOld||RESET" To Win!"
return


Win:
        if RollNum = 1 then do
                CALL MESSAGE "You Rolled A "RandSum" On The 1st Roll Which "GRE"Doubles"RESET" Your Bet!"
                TimeBet = TimeBet * 2
        end
        CALL MESSAGE "Craps! You "RED"Won "TimeBet" "Minutes||RESET"!"
        money = money + TimeBet
        if TimeBet = 1 then Minutes = "Dollar"
        else Minutes = "Dollars"
        call Again
        signal VEGAS
return


Lost:
        CALL MESSAGE "You Crapped Out, You "GRE"Lost "TimeBet" "Minutes||RESET"!"
        if TimeBet = 1 then Minutes = "Dollar"
        else Minutes = "Dollars"
        money = money - TimeBet
        call Again
        signal Vegas
return


Again:
        CALL MESSAGE " "
        prompt( GRE"Do You Want To Play Again (Y/N)? "RESET)
        PlayAgain=readstr()
        if checkBBS() then signal BYE
        if PlayAgain = "y" | PlayAgain = "Y" then call CRAPS
        if PlayAgain = "n" | PlayAgain = "N" then signal VEGAS
return

Dice1:
        CALL MESSAGE DicePos||WHIBLK"       "RESET
        CALL MESSAGE DicePos||WHIBLK"   *   "RESET
        CALL MESSAGE DicePos||WHIBLK"       "RESET
return

Dice2:
        CALL MESSAGE DicePos||WHIBLK"     * "RESET
        CALL MESSAGE DicePos||WHIBLK"       "RESET
        CALL MESSAGE DicePos||WHIBLK" *     "RESET
return

Dice3:
        CALL MESSAGE DicePos||WHIBLK"     * "RESET
        CALL MESSAGE DicePos||WHIBLK"   *   "RESET
        CALL MESSAGE DicePos||WHIBLK" *     "RESET
return

Dice4:
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
        CALL MESSAGE DicePos||WHIBLK"       "RESET
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
return

Dice5:
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
        CALL MESSAGE DicePos||WHIBLK"   *   "RESET
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
return

Dice6:
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
        CALL MESSAGE DicePos||WHIBLK" *   * "RESET
return


MESSAGE:
PARSE ARG string 
SAY string||CR
RETURN


NOCR:
PARSE ARG string 
x=OPEN(f,'*','W')
IF x=0 THEN RETURN
CALL WRITECH(f,string)
CALL CLOSE(f)
RETURN

readstr: procedure
str=''
out=readch(STDIN)
 call WRITECH(STDOUT,out)
 do while out~=D2C(13)
      if out=D2C(8) then do
         str=SUBSTR(str,1,LENGTH(str)-1)
         call WRITECH(STDOUT,' ')
         call WRITECH(STDOUT,out)
         end
      else
         str=INSERT(str,out)
      out=readch(STDIN)
      call WRITECH(STDOUT,out)
    end
say '0D'x
return(UPPER(str))

prompt: procedure
parse arg str
writech(STDOUT,str)
return 1

BREAK_C:
BREAK_E:
SAY 'Exiting back to BBS...'CR
EXIT

/* YuppieWars.rexx */
