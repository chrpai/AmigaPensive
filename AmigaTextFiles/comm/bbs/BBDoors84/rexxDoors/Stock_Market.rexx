/*************************************
 *    StockMarket v2.03 in AREXX     *
 * Adapted for BBBBS by Matt English *
 *    Last Edited: 22 Oct 1993 - RLS *
 *************************************/

options results
FF='0C'x
CR='0D'x
CALL TIME('R')
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

bbspath=GETCLIP('BBS_path')
bbspath=bbspath'rexxDoors/Data/'

PARSE arg Username winnings . colorflag secs .
IF secs='' THEN secs=3600
signal on syntax

CLS = '[H[J'
def=''
pen2=''
pen3=''
pen4=''
pen5=''
pen6=''
pen7=''
IF colorflag=1 THEN
  DO
    def='[0m'
    pen2='[32m'
    pen3='[33m'
    pen4='[34m'
    pen5='[35m'
    pen6='[36m'
    pen7='[37m'
  END

if ~show('L',"rexxsupport.library") then do
   addlib('rexxsupport.library',0,-30,0)
    end

info= 's:CONFIG.BBS'
IF ~EXISTS(info) THEN info='BBS:BBS_TEXT/CONFIG.BBS'
call open(con,info,'R')
IF con=0 THEN EXIT 666
lynes.1=readln(con)

compos=POS('/*',lynes.1)
IF compos>0 THEN lynes.1=LEFT(lynes.1,compos-1)
 bbsname=STRIP(lynes.1)

lynes.2=readln(con)
sysopname=WORD(lynes.2,1)
call close(con)

file1=''bbspath'SM.players'
file2=''bbspath'SM.playerstats'
file3=''bbspath'SM.companystats'
file4=''bbspath'SM.fame'
file5=''bbspath'SM.news1'
file6=''bbspath'SM.news2'
file7=''bbspath'SM.oneliner'
file8=''bbspath'SM.help'
file9=''bbspath'SM.log'

smv1=1000000;smv2=-1500;smv3=50000;smv4=7
smv5='New Game';smv6=10;smv7=24;smv8=1;smv9=1
smv10=0;smv11=0;smv12=0;smv13=10000;smv14=.51
smv15=.65;smv16=4;smv17=1;smv18=100;smv19=100;smv20=5

dashes='--------------------------------------'

name = UPPER(username)

  spaces='                 '
  longdashes='-----------------------------------------------------------------------------'
  say ''cr

norxit = 1 /* Normal exit, = 0 if ctrl c is hit */

START:
say ''cr
say ''cr
say ''spaces''pen4'$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$'def''cr
say ''spaces''pen4'$$                                   $$'def''cr
say ''spaces''pen4'$$'def'    'pen6'StockMarket v2.03 in ARexx'def'     'pen4'$$'def''cr
say ''spaces''pen4'$$'def'                'pen6'by'def'                 'pen4'$$'def''cr
say ''spaces''pen4'$$'def'             'pen6'Ken Koch'def'              'pen4'$$'def''cr
say ''spaces''pen4'$$                                   $$'def''cr
say ''spaces''pen4'$$'def' 'pen6'adapted for BBBBS by Matt English'def' 'pen4'$$'def''cr
say ''spaces''pen4'$$                                   $$'def''cr
say ''spaces''pen4'$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$'def''cr
say ''cr
time=time();date=date()
say ''cr
say 'Checking Board of Members'cr
if ~exists(file1) then do

  call open(fn1,file1,'W')
  call writeln(fn1,smv7 smv8 smv9)
  call writeln(fn1,date);call writeln(fn1,name)
  call writeln(fn1,smv10)
  call close(fn1)
  end
call open(fn2,file1,'R')
line=readln(fn2)
parse var line al np jd .
addaday=readln(fn2)
do n=1 to np
  line=readln(fn2);name.n=upper(line);lc.n=readln(fn2)
  end
call close(fn2)
do n=1 to np
  CALL WRITECH(STDOUT,'.')
  if name=name.n then cu=n
  end
SAY '.'CR
if ~exists(file2) then do
  call ALIAS
  call open(fn3,file2,'W');call writeln(fn3,smv5);call writeln(fn3,name);call writeln(fn3,alias);call writeln(fn3,smv3)
  do j = 1 to smv4
    call writeln(fn3,'0' '0')
    end
  call close(fn3)
  end
call open(fn4,file3,'R')
nc = readln(fn4)
do i=1 to nc
  cn$.i=readln(fn4);line=readln(fn4)
  parse var line cv.i vi.i vs.i sf.i tq.i rv.i aa.i .
  end
call close(fn4)



call open(fn5,file2,'R');line=readln(fn5)
parse var line lp$
do n=1 to np
  name.n=readln(fn5);alias.n=readln(fn5);cr.n=readln(fn5)
  do j=1 to smv4
    line=readln(fn5)
    parse var line cn.n.j so.n.j .
    end
  end
call close(fn5)

call STATCK

say ''cr
say ''pen3'Connecting to Wall Street...'def''cr
say ''cr
if ~exists(file4) then do
  call open(fn6,file4,'W');call writeln(fn6,'  Members of the Millionaire''s Club');call writeln(fn6,dashes);call close(fn6)
  end
call open(fa7,file4,'r')
do until eof(fa7)
  line=readln(fa7)
  say ''spaces''line''cr
  end
call close(fa7)
say ''spaces''dashes''cr;say ''cr
options prompt 'Tap 'pen3'Return'def' to continue.'

pull result
say ''CLS''cr
say ''cr
if ~exists(file6) then do
  call open(fn8,file6,'W')
  call writeln(fn8,spaces dashes)
  call writeln(fn8,spaces time date)
  call writeln(fn8,spaces'Stock Market opens for business')
  call close(fn8)
  end
if ~exists(file5) then do
  call open(fn9,file5,'W')
  call writeln(fn9,spaces dashes)
  call writeln(fn9,spaces'  Stock Market News for' date)
  call close(fn9)
  end
if date=addaday then nop
else do
  addaday=date;jd=jd+1

  address command 'c:delete 'file6' quiet'
  address command 'c:rename 'file5' 'file6''

  call open(fn12,file5,'W')
  call writeln(fn12,spaces dashes)
  call writeln(fn12,spaces ' Stock Market News for 'date)
  call close(fn12)
  do i=1 to nc
    vi.i=0
    end
  do i=1 to nc
    vs.i=0
    end
  do i=1 to nc
    zz=(((randu(time('s'))-.45 )*10)%1)/10


    tq.i=tq.i+zz
    end

  address command 'c:delete 'file3' quiet'
  call open(fn13,file3,'W')
  call writeln(fn13,nc)
  do i=1 to nc
    cv.i=trunc(cv.i+.005,2);vs.i=trunc(vs.i+.005,2);aa.i=trunc(aa.i+.005,2)
    call writeln(fn13,cn$.i);call writeln(fn13,cv.i vi.i vs.i sf.i tq.i rv.i aa.i)
    end
  call close(fn13)
  end
say ''cr
say ''cr

say'             Use option 'pen5'[8] 'def'to read the lastest Stock Market news!'cr

say ''cr;say ''cr
say ''spaces'Last Investor:'pen5'' lp$''def''cr
lc.cu=jd;call delay(100)
if jd=smv18 & smv17=1 then do
  say ''cr;say ''pen6'There has been no winners in the last'def' 'pen3''smv18''def' 'pen6'days.'def'  The game is being reset.'cr
  call RESETPLAYERS;call RESETCOMPANIES;call RESETNEWS
  signal BYE
  end
if ~exists(file7) then signal MAIN
say ''cr
call open(fny,file7,'R');mess=readln(fny);bywhom=readln(fny)
say ''longdashes''cr;say ''pen6''mess ''pen3''bywhom''def''cr;say ''longdashes''cr
call close(fny);call delay(200)
signal MAIN


MAIN:
call MENU;signal PROMPT

MENU:
say ''cr
say ''spaces''pen6'['pen3'1'pen6'] 'pen7'The Big Board'def''cr
say ''spaces''pen6'['pen3'2'pen6'] 'pen7'Your Portfolio'def''cr
say ''spaces''pen6'['pen3'3'pen6'] 'pen7'List of Investors and Net Worth'def''cr
say ''spaces''pen6'['pen3'4'pen6'] 'pen7'Check a Company''s Stockholders'def''cr
say ''spaces''pen6'['pen3'5'pen6'] 'pen7'Visit the Broker (Buy/Sell Stock)'def''cr
say ''spaces''pen6'['pen3'6'pen6'] 'pen7'Help and Instructions'def''cr
say ''spaces''pen6'['pen3'7'pen6'] 'pen7'Update Stock Market Prices'def''cr
say ''spaces''pen6'['pen3'8'pen6'] 'pen7'Read Stock Market News'def''cr
say ''spaces''pen6'['pen3'X'pen6'] 'pen7'Exit Stock Market to'pen3'' bbsname''def''cr
x=randu(time('s'))
if x>smv15 then signal RNDNOTICES
say ''cr
return

PROMPT:
say ''cr
options prompt ''pen2'Enter Your Stock Market Option?'def' > '
pull result
choice=upper(result)
CALL checkBBS()

if result='###PANIC' then SIGNAL GETOUT
if choice='?' then call MENU
if choice='Z' then call SYSOPMENU
if choice='1' then signal BIGBOARD
if choice='2' then call PORTFOLIO
if choice='3' then signal INVESTORWORTH
if choice='4' then signal EXAMINECO
if choice='5' then signal BUYSELL
if choice='6' then signal HELP
if choice='7' then signal USERUPDATEFLAG
if choice='8' then signal NEWS
if choice='X' then signal EXITREG
signal MAIN

STATCK:
do n=1 to np
  if name.n=name then return
  end
if np>=al then do
  say ''cr
  say ''pen3'Sorry, the maximum number of players are now registered.'def''cr
  call delay(100);say ''cr
  say ''pen5'Check back daily for new openings.'def''cr
  call delay(100);say ''cr
  say 'Returning to'pen2'' bbsname''def''cr
  call delay(100)
  end
if np>=al then exit
if name=lp$ then do
  say ''cr
  say ''spaces''pen3'You cannot re-register at this time'def''spaces''cr
  say ''cr;say 'Returning to'pen2'' bbsname''def''cr
  call delay(100);exit
  end
call ALIAS
say ''cr;say ''pen7'Adding your name to the Board of Members...'def''cr
np=np+1;alias.np=alias;lc.np=smv10;cu=np
address command 'c:delete 'file1' quiet'
call open(fn14,file1,'W');call writeln(fn14,al np jd);call writeln(fn14,addaday)
do n=1 to np
  call writeln(fn14,name.n);call writeln(fn14,lc.n)
  end
call close(fn14)
say ''cr
say''pen5'Setting up your account'def''cr
call open(fn15,file2,'A');call writeln(fn15,name);call writeln(fn15,alias.cu);call writeln(fn15,smv3)
do j=1 to smv4
  CALL WRITECH(STDOUT,'.');call writeln(fn15,'0' '0')
  end
say '.'cr
call close(fn15)
call open(fn5,file2,'R');lp$=readln(fn5)
do n=1 to np
  name.n=readln(fn5);alias.n=readln(fn5);cr.n=readln(fn5)
  do j=1 to smv4
    line=readln(fn5)
    parse var line cn.n.j so.n.j .
    end
  end
call close(fn5)
return

PORTFOLIO:
time=time();date=date()
say ''cr;say ''spaces''pen2''center('Your Investment Portfolio as of:',40)''def''cr
say ''spaces''pen3''center(time date,40)''def''cr
say ''cr;say ''spaces'   'pen5'Company              Shares    Net'def''cr
say ''spaces' 'pen5'# Name                  Owned   Gain'def''cr
say ''spaces''dashes''cr
do j=1 to smv4
  do i=1 to nc
    if cn.cu.j~=i then nop
    else do
     
mi=right(i,2);mcn$.i=left(cn$.i,20);mso.cu.j=right(so.cu.j,6);mvs.i=trunc(vs.i+.005,2);mvs.i=right(mvs.i,6);say ''spaces''pen6''mi' 'mcn$.i' 'mso.cu.j' 'mvs.i''def''cr
      end
    end
  end
say ''spaces''dashes''cr
say ''spaces'Your available cash: 'pen3'$'cr.cu''def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr
return


EXITREG:
say ''cr;say ''pen3'Leaving the Stock Market...'def''cr
call NEGCASH
say ''cr;options prompt ''pen3'Are you sure (Y/n)?'def' > '


pull result
choice=upper(result)


if result='###PANIC' then SIGNAL GETOUT
if choice='N' then signal MAIN
if choice='Y' then SIGNAL GETOUT

GETOUT:
if jd=smv18 & smv17=1 then do
  address command 'c:delete 'file3' quiet'
  call open(fn16,file3,'W');call writeln(fn16,nc);cv.i=trunc(cv.i+.005,2);vs.i=trunc(vs.i+.005,2);aa.i=trunc(aa.i+.005,2)
    do i=1 to nc
    call writeln(fn16,cn$.i);call writeln(fn16,cv.i vi.i vs.i sf.i tq.i rv.i aa.i)
    end
  call close(fn16)
  end
winner=0;call CHECKFORWINNER
if jd=smv18 then do
  address command 'c:delete 'file3' quiet'
  call open(fn17,file3,'W');call writeln(fn17,nc)
  do i=1 to nc
    cv.i=trunc(cv.i+.005,2);vs.i=trunc(vs.i+.005,2);aa.i=trunc(aa.i+.005,2);call writeln(fn17,cn$.i);call writeln(fn17,cv.i vi.i vs.i sf.i tq.i rv.i aa.i)
    end
  call close(fn17)
  end
if alias.cu~=lp$ then do
  say ''cr;say ''pen7'Processing Update...'def''cr;call PRICEUPDATE
  end
say ''cr;say ''pen3'Updating Stock Market files...'def''cr
say ''cr;ct=np
do n=1 to np
  badso.n=0
  do j=1 to smv4
    badso.n=badso.n+so.n.j
    end
  if badso.n<1 & cr.n<100 then do
    time=time();date=date()
    say ''spaces''dashes''cr
    say ''spaces''time'' date''cr
    say ''spaces''pen5'alias.n'def 'was removed from the'cr
    say ''spaces'Board due to Bad Debts!'cr
    say ''cr
    call open(fn19,file5,'A');call writeln(fn19,dashes);call writeln(fn19,time date);call writeln(fn19,alias.n 'was removed from the');call writeln(fn19,'Board due to Bad Debts!');call close(fn19)
    alias.n='^';ct=ct-1
    do j=1 to smv4
      sf.j=sf.j+so.n.j
      end
    end
  end
if alias.cu~='^' & cr.cu<-1500 then do
  time=time();date=date()
  say ''spaces''dashes''cr
  say ''spaces''time'' date''cr
  say ''spaces''pen3''alias.cu ''def'was removed from the'cr
  say ''spaces'Board due to Bad Debts!'cr
  say ''cr
  call open(fn19,file5,'A');call writeln(fn19,''dashes);call writeln(fn19,''time date);call writeln(fn19,''alias.cu 'was removed from the');call writeln(fn19,'Board due to Bad Debts!');call close(fn19)
  alias.cu='^';ct=ct-1
  do j=1 to smv4
    sf.j=sf.j+so.cu.j
    end
  end
do n=1 to np
  if smv19<jd-lc.n then do
    time=time();date=date()
    say ''spaces''dashes''cr
    say ''spaces''time'' date''cr
    say ''spaces''pen3''alias.n ''def'was removed from the'cr
    say ''spaces'Board due to inactivity!'cr
    say ''cr
    call open(fn18,file5,'A');call writeln(fn18,dashes);call writeln(fn18,time date);call writeln(fn18,alias.n 'was removed from the');call writeln(fn18,'Board due to inactivity!');call close(fn18)
    alias.n='^';ct=ct-1
    do j=1 to smv4
      sf.j=sf.j+so.n.j
      end
    end
  end
address command 'c:delete 'file1' quiet'
call open(fn20,file1,'W');call writeln(fn20,al ct jd);call writeln(fn20,addaday)
do n=1 to np
  if alias.n~='^' then do
    call writeln(fn20,name.n);call writeln(fn20,lc.n)
    end
  end
call close(fn20)
address command 'c:delete 'file2' quiet'
call open(fn21,file2,'W');call writeln(fn21,alias.cu)
do n=1 to np
  if alias.n~='^' then do
    call writeln(fn21,name.n);call writeln(fn21,alias.n);call writeln(fn21,cr.n)
    do j=1 to smv4
      call writeln(fn21,cn.n.j so.n.j)
      end
    end
  end
call close(fn21)
address command 'c:delete 'file3' quiet'
call open(fn22,file3,'W');call writeln(fn22,nc)
do i=1 to nc
  cv.i=trunc(cv.i+.005,2);vs.i=trunc(vs.i+.005,2);aa.i=trunc(aa.i+.005,2);call writeln(fn22,cn$.i);call writeln(fn22,cv.i vi.i vs.i sf.i tq.i rv.i aa.i)
  end
call close(fn22)
say ''cr
if norxit = 1 then do /* Normal exit, = 0 if ctrl c is hit */
  call ONELINER
  end
say ''cr
say ''spaces''pen2'Thank you for playing Stock Market'def''spaces''cr
say ''cr;call delay(100)
say 'Returning to 'pen6''bbsname''def''cr;call delay(100);y=0.05
do j=1 to smv4
  do i=1 to smv6
    if cn.cu.j=i then do
      y=y+(so.cu.j*cv.i)
      end
    end 
  end
y=y+cr.cu


 if ~exists(''bbspath'SM.log') then do
     call open(log,''bbspath'SM.log','W')
     end
 else call open(log,''bbspath'SM.log','A')

call writeln(log,name date() time())
call close(log)

exit



NEGCASH:
if cr.cu>smv2 then return
say ''cr;say ''pen3'FLASH'spaces''pen2'Your available cash is in the 'pen3'red!'def''spaces ''cr
say ''cr;say ''pen6'If you leave the Stock Market with a with a debt of more than 'pen5'$'smv2 ''pen6'in cash,'def''cr
say ''pen6'your account will be 'pen3'DELETED!'def''cr
return

USERUPDATEFLAG:
say ''cr
if alias.cu=lp$ then do
  say ''cr;say ''pen3'You did the last update.'def''cr;say ''cr;call delay(100)
  say ''pen5'Another investor must do the next update!'def''cr;call delay(100)
  end
else call PRICEUPDATE
signal MAIN

PRICEUPDATE:
time=time();date=date()
if alias.cu=lp$ then return
lp$=alias.cu
do i=1 to nc
  div.i=0
  if cv.i>0 then do
    x=(randu(time('s'))*(rv.i/10)+((tq.i/20)-(rv.i/20)));cv.i=cv.i+x;vs.i=vs.i+x
    if x>smv14 then div.i=1
    end
  end
call DIVIDENDS
do i=1 to nc
  split.i=0
  if cv.i>(aa.i*2.2) then split.i=1
  end
call SPLIT
do i=1 to nc
  if cv.i<1 then do
    time=time();date=date()
    say ''spaces''dashes''cr
    say ''spaces''time'' date''cr
    say ''spaces''pen5'' cn$.i ''def'goes 'pen3'BANKRUPT!'def' 'cr
    say ''cr
    call open(fn25,file5,'A');call writeln(fn25,dashes);call writeln(fn25,time date);call writeln(fn25,cn$.i 'goes BANKRUPT!');call close(fn25)
    cv.i=0;sf.i=0
    do n=1 to np
      do j=1 to smv4
      if cn.n.j=i then do
        cn.n.j=0;so.n.j=0
        end
      end
    end
  end
address command 'c:delete 'file3' quiet'
call open(fn26,file3,'W');call writeln(fn26,nc)
do i=1 to nc
  cv.i=trunc(cv.i+.005,2);vs.i=trunc(vs.i+.005,2);aa.i=trunc(aa.i+.005,2);call writeln(fn26,cn$.i);call writeln(fn26,cv.i vi.i vs.i sf.i tq.i rv.i aa.i)
  end
call close(fn26)
return

DIVIDENDS:
do i=1 to nc
  if div.i=1 then do
    pd=(((randu(time('s'))*smv16)+1)%1)/2
    time=time();date=date()
    say ''spaces''dashes''cr
    say ''spaces''time'' date''cr
    say ''spaces''pen3''cn$.i''def''cr
    say ''spaces'paid 'pen6'$'trunc(pd+.005,2) ''def'per share in Dividends'cr
    say ''spaces'P/S 'pen5'$'trunc(cv.i+.005,2)''def'       N/G 'pen6'$'trunc(vs.i+.005,2)''def''cr
    say ''cr

    call close(fn23)
    call open(fn23,file5,'A')
    call writeln(fn23,longdashes)
    call writeln(fn23,time cn$.i ':' 'paid $'trunc(pd+.005,2) '/ share, P/S $'trunc(cv.i+.005,2)', N/G $'trunc(vs.i+.005,2))
    call close(fn23)


    end
  end
call PAYDIVIDENDS
return

PAYDIVIDENDS:
do i=1 to nc
  if div.i=1 then do
    do n=1 to np
      do j=1 to smv4
        if cn.n.j=i then do
          cr.n=(cr.n+(pd*so.n.j)+.5)%1
          end
        end
      end
    end
  end
return

SPLIT:
do i=1 to nc
  if split.i=1 then do
    time=time();date=date();cv.i=(cv.i*1.07)/2;aa.i=cv.i
    say ''spaces''dashes''cr
    say ''spaces''time'' date''cr
    say ''spaces''pen3''cn$.i''def''cr
    say ''spaces'Stock has split!  P/S 'pen5'$'trunc(cv.i+.005,2)''def''cr
    say ''cr
    call open(fn38,file5,'A');call writeln(fn38,dashes);call writeln(fn38,time date);call writeln(fn38,cn$.i);call writeln(fn38,'Stock has split!  P/S $'trunc(cv.i+.005,2));call close(fn38)
    sf.i=sf.i*2
    do n=1 to np
      do j=1 to smv4
        if cn.n.j=i then so.n.j=so.n.j*2
        end
      end
    end
  end
return

CHECKFORWINNER:
say ''cr;say ''pen6'Checking your holdings...'def''cr
y=0.05;fame=0
do j=1 to smv4
  do i=1 to smv6
    if cn.cu.j=i then y=y+(so.cu.j*cv.i)
    end
  end

y=y+cr.cu;my=trunc(y);my=right(my,8);malias=left(alias.cu,19)

/* y=y+cr.cu;my='$'trunc(y);my=right(my,8);malias=left(alias.cu,19)*/ /*changed y,8 to my,8 */
if y>=smv1 then do
  date=date(U)
  say ''cr;say ''pen3'FLASH 'pen6'Congratulations,'pen4'' alias.cu''def''cr;winner=1;call delay(100)
  say ''cr;say 'Your Net Worth is 'pen3'$'my'.  'pen5'You have won the game!'def''cr;call delay(100)
  say ''cr;say ''pen3'Adding your name to the Millionaire''s Club'def''cr
  call open(fn37,file4,'A');call writeln(fn37,date malias '$'my);call close(fn37)
  do i=1 to nc
    sf.i=smv13;cv.i=((randu(time('s'))*4500)+500)%1/100;aa.i=cv.i
    end
  fame=1;np=0;jd=999;call RESETPLAYERS;call RESETCOMPANIES;call RESETNEWS;signal GAMEWON
  end
say ''cr;say ''spaces'Your Net Worth is : 'pen3'$'trunc(y)''def''cr;call delay(100)
say ''cr
say ''pen2'Checking other members...'def''cr;say ''cr
do n=1 to np
  if n=cu then iterate
  say ''pen3''alias.n''def''cr
  yy=0
  do j=1 to smv4
    do i=1 to smv6
      if cn.n.j=i then yy=yy+(so.n.j*cv.i)
      end 
    end
yy.n=trunc(yy+cr.n);myy.n=right(yy.n,8);malias.n=left(alias.n,19)
  if yy.n>=smv1 & fame=0 then do
    date=date(U)
    say ''cr;say ''pen3'FLASH Notice!'def''cr;winner=1
    say ''cr;say ''pen6''alias.n' 'pen3'has won the game!'def''cr;call delay(100)
    say ''cr;say 'Adding 'pen3''alias.n''def' to the Millionaire''s Club'cr
    call open(fn37,file4,'A');call writeln(fn37,date malias.n '$'myy.n);call close(fn37)
    do i=1 to nc
      sf.i=smv13;cv.i=((randu(time('s'))*4500)+500)%1/100;aa.i=cv.i
      end 
    np=0;jd=999;call RESETPLAYERS;call RESETCOMPANIES;call RESETNEWS;signal GAMEWON
    end
  end
return

BIGBOARD:
time=time();date=date()
say ''cr;say ''spaces''pen3''center('Market Prices as of:',40)''def''cr
say ''spaces''pen2''center(time date,40)''def''cr
say ''cr;say ''spaces'    'pen6'Company             Shares Current'def''cr
say ''spaces'    'pen2'Name             Available   Price'def''cr
say ''spaces''dashes''cr
vi=smv11;vs=smv12
do i=1 to nc
  vi=vi+vi.i;vs=vs+vs.i;mi=right(i,2);mcn$.i=left(cn$.i,20);msf.i=right(sf.i,6);mcv.i=trunc(cv.i+.005,2);mcv.i=right(mcv.i,7)
  if cv.i=0 then do
    data='***BANKRUPT***';say ''spaces''pen3''mi' 'mcn$.i' 'data''def''cr
    end

  else say ''spaces''pen2''mi'' mcn$.i'' msf.i'' mcv.i''def''cr

 end
say ''spaces''dashes''cr
say ''spaces'Volume Traded:'pen6' 'vi''def''cr
say ''spaces'Price Index  :'pen3'' trunc(vs+.005,2)''def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'

pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN


INVESTORWORTH:
time=time();date=date()
say ''cr;say ''spaces''pen3''center('Stockholders as of:',40)''def''cr
say ''spaces''pen3''center(time date,40)''def''cr
say ''cr;say ''spaces''pen2'Investor                     Net Worth'def''cr
say ''spaces''dashes''cr
do n=1 to np
  y=0.05
  do j=1 to smv4
    do i=1 to nc
      if cn.n.j=i then do
        y=y+(so.n.j*cv.i)
        end
      end
    end
  y=y+cr.n;my='$'trunc(y);my=right(my,8);malias.n=left(alias.n,29);say ''spaces''pen6''malias.n' 'my''def''cr
  end
say ''spaces''dashes''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

EXAMINECO:
say ''cr
do i=1 to nc
  if i<10 then say ''spaces' 'pen6'['pen3''i''pen6'] 'pen5'' cn$.i''def''cr
  else say ''spaces''pen6'['pen3''i''pen6']'pen5'' cn$.i''def''cr
  end
say ''spaces' 'pen6'['pen3'X'pen6']'def' Exit'cr
say ''cr


options prompt 'Enter the Number of the Company you wish to check > '
pull result;i=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if i='X' then signal MAIN
if i<1 | i>nc then signal EXAMINECO
time=time();date=date()
say ''cr;say ''spaces''pen3''center('Stock Report for:',40)''def''cr
say ''spaces''pen2''center(cn$.i,40)''def''cr
say ''spaces'   'pen5'As of:' time'' date''def''cr
say ''cr;say ''spaces''pen7'Investor                  Shares Owned'def''cr
say ''spaces''dashes''cr
if cv.i=0 then do
  say ''spaces'     'pen3'>>>>> COMPANY BANKRUPT <<<<<'def'      'cr;say ''spaces''dashes''cr;say ''cr;call delay(100)
  end
if cv.i=0 then signal EXAMINECO
do n=1 to np
  do j=1 to smv4
    if cn.n.j~=i then nop
    else do
      mso.n.j=right(so.n.j,8);malias.n=left(alias.n,28,);say ''spaces''pen5''malias.n' 'mso.n.j''def''cr
      end
    end
  end
say ''spaces''dashes''cr;say ''spaces'Net Gain today      :'pen7'' trunc(vs.i+.005,2)''def''cr
say ''spaces'Volume traded today :'pen3'' vi.i''def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal EXAMINECO

BUYSELL:
say ''cr
say ''spaces''pen6'['pen3'B'pen6']  'pen7'Buy Stocks'def''cr
say ''spaces''pen6'['pen3'S'pen6']  'pen7'Sell Stocks'def''cr
say ''spaces''pen6'['pen3'X'pen6']  'pen3'Exit'def''cr
say ''cr;options prompt 'Do you wish to BUY or SELL Stocks > '


pull result;choice=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if choice='B' then signal BUY
if choice='S' then signal SELL
if choice='X' then signal MAIN
else signal BUYSELL

BUY:
say ''cr
do i=1 to nc
  if i<10 then say ''spaces' 'pen6'['pen3''i''pen6']  'pen5''cn$.i''def''cr
  else say ''spaces''pen6'['pen3''i''pen6']  'pen5''cn$.i''def''cr
  end
say ''spaces' 'pen6'['pen3'X'pen6']  'pen3'Exit'def''cr;say ''cr


options prompt 'Enter Number of the Company you wish to Buy > '
pull result;i=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if i='X' then signal BUYSELL
if i=0 then signal BUYSELL
if i<1 | i>smv6 then signal BUY
say ''cr;say ''spaces''pen2'Company Name   :'pen6'' cn$.i''def''cr
say ''spaces''pen7'Shares Free    :'pen6'' sf.i''def''cr
if cv.i<0 then say ''spaces''pen7'Price per Share:    'pen3'>>>>> BANKRUPT <<<<<   'def''cr
else say ''spaces'Price per Share: 'pen6'$'trunc(cv.i+.005,2)''def''cr
say ''spaces'Net Gain today : 'pen6'$'trunc(vs.i+.005,2)''def''cr
say ''spaces'Available Cash : 'pen3'$'cr.cu''def''cr
say ''cr;say ''pen4'Enter the Amount of Shares you wish to Purchase'def''cr


options prompt 'Enter ''A'' to buy the MAXIMUM amount > '
pull result;buyme=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if buyme='A' then do
  buyme=(cr.cu*(1-(smv20/100)))%1;buyme=(buyme/cv.i)%1
  if buyme>sf.i then buyme=sf.i
  end
if buyme='' then signal BUYSELL
if ~datatype(buyme,'w') then signal BUY
if buyme<=0 then signal BUYSELL
if buyme<5 then do
  say ''cr;say ' 'pen3'Five Shares MINIMUM purchase 'def''cr;say ''cr;call delay(100)
  end
if buyme>sf.i then do
  say ''cr;say ''pen3'' sf.i ''def'Shares 'pen6'MAXIMUM'def' Purchase 'cr;say ''cr
  end
if buyme<5 | buyme>sf.i then signal BUY
price=buyme*cv.i;price=((price+(price*(smv20/100)))%1)*100/100
if price>cr.cu then do
  say ''cr;say ' 'pen3'You don''t have enough Cash!'def' 'cr;say ''cr;call delay(100)
  end
if price>cr.cu then signal BUY
say ''cr;say ''spaces'Total shares will be :'pen6'' buyme''def''cr
say ''spaces'Total cost will be   : 'pen7'$'price''def''cr
say ''spaces''pen3'(including a 'smv20'% Broker''s Fee)'def''cr
say ''cr;options prompt ''pen5'Execute purchase order (Y/n)?'def' > '


pull result;choice=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if choice='N' then signal BUY
bought=0
do j=1 to smv4
  if cn.cu.j=i then do
    bought=1;so.cu.j=so.cu.j+buyme;vi.i=vi.i+buyme;sf.i=sf.i-buyme;cr.cu=cr.cu-price
    say ''cr;say ''pen3''buyme ''pen7'shares have been added to your portfolio'def''cr;say ''cr;call delay(100)
    say ''pen7'Total purchase price was 'pen3'$'price' 'pen4'(including a 'smv20'% Broker''s fee)'def''cr;say ''cr;call delay(100)
    say ''pen7'Your available cash is now 'pen3'$'cr.cu''def''cr;call delay(100)
    end
  end
if bought=1 then signal BUYSELL
do j=1 to smv4
  if bought=0 & cn.cu.j=0 then do
    bought=1;cn.cu.j=i;so.cu.j=buyme;vi.i=vi.i+buyme;sf.i=sf.i-buyme;cr.cu=cr.cu-price
    say ''cr;say ''pen3''buyme ''pen7'shares have been added to your portfolio'def''cr;say ''cr;call delay(100)
    say ''pen7'Total purchase price was 'pen3'$'price' 'pen4'(including a 'smv20'% Broker''s fee)'def''cr;say ''cr;call delay(100)
    say ''pen5'Your available cash is now 'pen3'$'cr.cu''def''cr;call delay(100)
    end
  end
if bought=1 then signal BUYSELL
say ''cr;say ' 'pen2'You may own 'pen3''smv4' 'pen2'companies 'pen3'MAXIMUM! 'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal BUYSELL

SELL:
nostock=0;time=time();date=date()
say ''cr;say ''spaces'   'pen5'Your Investment Portfolio as of:'def''cr
say ''spaces'        'pen5' 'time'' date''def''cr
say ''cr;say ''spaces'   'pen3'Company              Shares    Net'def''cr
say ''spaces' 'pen6'# Name                  Owned   Gain'def''cr
say ''spaces''dashes''cr
do j=1 to smv4
  do i=1 to nc
    if cn.cu.j~=i then nop
    else do  
      mi=right(i,2);mcn$.i=left(cn$.i,20);mso.cu.j=right(so.cu.j,6);mvs.i=trunc(vs.i+.005,2);mvs.i=right(mvs.i,6)
      say ''spaces''pen7''mi'' mcn$.i'' mso.cu.j'' mvs.i''def''cr
      end
    end
  end
say ''spaces''dashes''cr;say ''cr;say ''spaces''pen7'Your available cash: 'pen3'$'cr.cu''def''cr
say ''cr;say ''pen4'Enter the Number of the Company you wish to Sell'def''cr


options prompt 'Tap ''X'' to Exit > '
pull result;i=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if i=0 then signal BUYSELL
if i='X' then signal BUYSELL
if i<1 | i>smv6 then signal SELL
do j=1 to smv4
  if cn.cu.j=i then do
    nostock=1;sale=j
    end
  end
if nostock=0 then do
  say ''cr;say ' 'pen3'You don''t own'pen6'' cn$.i ''def''cr;call delay(100)
  end
if nostock=0 then signal SELL
say ''cr;say ''spaces''pen2'Company Name    :'pen6'' cn$.i''def''cr
say ''spaces''pen2'Price per Share : 'pen3'$'trunc(cv.i+.005,2)''def''cr
say ''spaces''pen2'Shares Owned    :'pen5'' so.cu.sale''def''cr
say ''cr;say ''pen4'Enter the number of SHARES to sell'def''cr

options prompt 'Enter ''A'' to sell all your Shares of 'cn$.i' > '
pull result;sellme=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if sellme='A' then sellme=so.cu.sale
if sellme='' then signal BUY/SELL
if ~datatype(sellme,'w') then signal SELL
if sellme<1 then signal BUYSELL
if sellme<5 then do
  say ''cr
  say ' 'pen3'Five Shares MINIMUM Transaction 'def''cr;say ''cr;call delay(100)
  end
if sellme>so.cu.sale then do
  say ''cr;say ''pen3'' so.cu.sale ''def'Shares 'pen3'MAXIMUM 'def'Transaction 'cr;say ''cr;call delay(100)
  end
if sellme<5 | sellme>so.cu.sale then signal SELL
sold=sellme*cv.i;sold=((sold-(sold*(smv20/100)))%1)*100/100
say ''cr;say ''spaces''pen7'Number of Shares being sold :'pen3'' sellme''def''cr
say ''spaces''pen7'Total sale will be : 'pen5'$'sold''def''cr
say ''spaces''pen4'(including a 'smv20'% Broker''s Fee)'def''cr
say ''cr;options prompt ''pen3'Execute sell order (Y/n)?'def' > '


pull result;choice=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if choice='N' then signal SELL
vi.i=vi.i+sellme;sf.i=sf.i+sellme;cr.cu=cr.cu+sold;so.cu.sale=so.cu.sale-sellme
if so.cu.sale<=0 then do
  cn.cu.sale=0;so.cu.sale=0
  end
say ''cr
say ''pen3'$'sold ''pen2'has been added to your cash holdings'def''cr;say ''cr;call delay(100)
say ''pen2'You now have 'pen5'$'cr.cu' 'pen2'in cash'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal BUYSELL

HELP:
say ''cr
if ~exists(file8) then do
  say ' 'pen3'Please ask SYSOP to install the Help File for Stock Market! 'def''cr;call delay(200)
  end
if exists(file8) then do
  call open(hlp,file8)
   do until eof(hlp)

  do
   do for 18
   helpln = readln(hlp)
   say''pen7''helpln''def''cr
   end
  say ''cr
  options prompt'Hit 'pen3'return to continue'def''
  pull result
  say ''cr
  end

  end
call close(hlp)
say ''cr


if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

NEWS:
call open(fn27,file6,'r')
  call readfl (fn27)
call close(fn27)
say ''spaces''dashes''cr;say ''cr
options prompt 'Tap 'pen3'Return'def' to continue.'
pull result

say ''cr
call open(fn28,file5,'r')
  call readfl (fn28)
call close(fn28)
say ''spaces''dashes''cr;say ''cr
options prompt 'Tap 'pen3'Return'def' to continue.'
pull result

say ''cr;signal MAIN

RNDNOTICES:
i=((randu(time('s'))*99)%1)+1
if i<2 then signal IRSAUDIT
if i<10 & cr.cu<300 then signal FURNITURE
if i<15 then signal BURGLAR
if i<40 then signal LUCKY
if i<50 then signal INTERESTPAID
if i<65 then signal ILLEGAL
if i<80 then signal INSIDEINFO
if i<95 then signal FEESDUE
if i<98 then signal FOLD
else signal RNDNOTICES

FURNITURE:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
x=((randu(time('s'))*1900)%1)+100;cr.cu=cr.cu-x
say ''pen7'New office furniture arrives C.O.D.  The amount of 'pen3'$' x ''pen7'has been'def''cr
say ''pen7'subtracted from your cash account.'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

BURGLAR:
if cr.cu<1 then signal RNDNOTICES
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
cr.cu=0
say ''pen3'Your office was burglarized!  'pen2'All your cash was stolen.'def''cr
say ''pen5'The SHOP VAC BANDIT strikes again!'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'cr


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

LUCKY:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
x=(((randu(time('s'))*10)+1)%1)*1000;cr.cu=cr.cu+x
say ''pen2'This is your lucky day!  On your way to'pen3'' bbsname''def''cr
say ''pen6'you find 'pen3'$' x ''pen6'in cash!'def''cr
say ''cr
options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

/*-------------------------------------------------------------------*/

readfl:

arg shfl
   do until eof(shfl)


   do for 6
   shln = readln(shfl)
   say''pen3''shln''def''cr
   if eof(shfl) then return
   shln = readln(shfl)
   say''pen5''shln''def''cr
   if eof(shfl) then return
   shln = readln(shfl)
   say''pen6''shln''def''cr
   if eof(shfl) then return
   end
  say ''cr
  options prompt'Hit 'pen3'return'def' to continue or 'pen6'Q'def' to quit >'
  pull result
  if result = 'Q' then return
  say ''cr
  end

return
INTERESTPAID:
if cr.cu<1 then signal RNDNOTICES
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
x=((cr.cu*.14)%1);cr.cu=cr.cu+x
say ''pen5'Interest has been paid on your cash.  'pen3'$' x ''pen5'has been added'def''cr
say ''pen5'to your cash available!'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

ILLEGAL:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
x=(((randu(time('s'))*10)+1)%1)*1000;cr.cu=cr.cu-x
say ''pen4'You''ve been caught 'pen3'red-handed!  'pen4'The Board Commission has discovered'def''cr
say ''pen4'illegal practices in your transactions.  You are fined 'pen3'$' x ''pen4'!'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

IRSAUDIT:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
lc.cu=jd-(smv19+1);time=time();date=date()
call open(fn29,file5,'A');call writeln(fn29,dashes);call writeln(fn29,time date);call writeln(fn29,alias.cu 'seat is sold due to');call writeln(fn29,'I.R.S audit!');call close(fn29)
say ''pen5'An I.R.S. audit has revealed that you owe back taxes in excess of'def''cr
say ''pen3'3.5 million dollars!  'pen4'Your seat on the Board has been 'pen3'SOLD 'pen4'in order to'def''cr
say ''pen4'satisfy your debts.'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'
pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr
SIGNAL GETOUT

FOLD:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
x=((randu(time('s'))*nc)%1)+1
if cv.x=0 then signal FOLD
time=time();date=date()
call open(fn30,file5,'A');call writeln(fn30,dashes);call writeln(fn30,time date);call writeln(fn30,cn$.x 'FOLDS!');call close(fn30)
cv.x=0;sf.x=0
do i=1 to np
  do j=1 to smv4
    if cn.i.j=x then do
      cn.i.j=0;so.i.j=0
    end
  end
say ''pen3''cn$.x ''pen6'has folded due to bad business practices!'def''cr
say ''cr
options prompt 'Tap 'pen3'Return'def' to continue.'
pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

INSIDEINFO:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
x=(((randu(time('s'))*30)%1)+5)*100;cr.cu=cr.cu+x
say ''pen2'Inside information pays off!  Receive 'pen3'$' x ''pen2'in windfall profits.'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'
pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

FEESDUE:
say ''cr;say ''pen3'FLASH Notice!'def''cr;say ''cr
cr.cu=cr.cu-2500
say ''pen7'Board membership fees are due.  'pen3'$2500 'pen7'has been deducted from'def''cr
say ''pen7'your cash account.'def''cr
say ''cr;options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal MAIN

RESETPLAYERS:
say ''cr;say ''pen3'Resetting PLAYERS file...'def''cr
address command 'c:delete 'file1' quiet'
call open(fn34,file1,'W');call writeln(fn34,smv7 smv8 smv9);call writeln(fn34,date);call writeln(fn34,name);call writeln(fn34,smv10);call close(fn34)
say ''pen6'Resetting PLAYERSTATS file...'def''cr
address command 'c:delete 'file2' quiet'
call open(fn35,file2,'W');call writeln(fn35,smv5);call writeln(fn35,name.cu);call writeln(fn35,alias.cu);call writeln(fn35,smv3)
do j=1 to smv4
  call writeln(fn35,'0' '0')
  end
call close(fn35)
return

RESETCOMPANIES:
say ''pen4'Resetting COMPANYSTATS file...'def''cr
do i=1 to smv6
  cv.i=aa.i;vi.i=smv11;vs.i=smv12;sf.i=smv13
  rv.i=random(1,100,time('s'))-50
  end
address command 'c:delete 'file3' quiet'
call open(fn36,file3,'W');call writeln(fn36,smv6)
do i=1 to smv6
  cv.i=trunc(cv.i+.005,2);vs.i=trunc(vs.i+.005,2);aa.i=trunc(aa.i+.005,2);call writeln(fn36,cn$.i);call writeln(fn36,cv.i vi.i vs.i sf.i tq.i rv.i aa.i)
  end
call close(fn36)
return

RESETNEWS:
say ''pen3'Resetting NEWS1 and NEWS2 files...'def''cr
address command 'c:delete 'file5' quiet';address command 'c:delete 'file6' quiet'
return

SYSOPMENU:
if name ~= sysopname then return
say ''cr;say ''spaces'Stock Market Sysop Functions'cr;say ''cr
say ''spaces'[1]  Change Company Stats'cr
say ''spaces'[2]  Delete Player'cr
say ''spaces'[3]  View Stock Market Files'cr
say ''spaces'[4]  Reset Game'cr
say ''spaces'[X]  Exit to Stock Market'cr
say ''cr;options prompt 'Your choice,' name '> '


pull result;mci=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if mci='X' then signal MAIN
if mci<1 | mci>4 then signal SYSOPMENU
if mci=1 then signal CHANGECOMPANY
if mci=2 then signal DELETEPLAYER
if mci=3 then signal VIEWFILES
if mci=4 then signal RESETGAME
return

CHANGECOMPANY:
say ''cr;say ''spaces'Change Company Stats'cr;say ''cr
do i=1 to nc
  if i<10 then say ''spaces'['i']' cn$.i''cr
  else say ''spaces'['i']' cn$.i''cr
  end
say ''spaces'[X]  Exit to Sysop Menu'cr;say ''cr


options prompt ''pen2'Enter the Number of the Company you wish to check'def' > '
pull result;i=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if i='X' then signal SYSOPMENU
if i<1 | i>nc then signal CHANGECOMPANY
say ''cr;say ''spaces''center('Company Stats for :',40)''cr
say ''spaces''center(cn$.i,40)''cr
say ''cr;say ''spaces'[1]  Company Name   : 'cn$.i''cr
say ''spaces'[2]  Current Price  :' cv.i''cr
say ''spaces'[3]  Initial Price  :' aa.i''cr
say ''spaces'[4]  Shares Free    :' sf.i''cr
say ''spaces'[5]  Trend Value    :' tq.i''cr
say ''spaces'[6]  Risk Value     :' rv.i''cr
say ''spaces'[X]  Exit to Sysop Menu'cr
say ''cr;options prompt 'Enter companystat you wish to change > '


pull result;di=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if di='X' then signal SYSOPMENU
if di<1 | di>6 then signal CHANGECOMPANY
if di=1 then do
  say 'Enter new Company Name.  Maximum length 20 characters.'cr
  say ' >--------------------<'cr


  options prompt ' > ';pull result;newcn$=result
  if result='###PANIC' then SIGNAL GETOUT
  cn$.i=newcn$
  end
if di=2 then do


  options prompt 'Enter new Current Price > '
  pull result;cv.i=result
  if result='###PANIC' then SIGNAL GETOUT
  end
if di=3 then do

  options prompt 'Enter new Initial Price > '
  pull result;aa.i=result
  if result='###PANIC' then SIGNAL GETOUT
  end
if di=4 then do


  options prompt 'Enter the new number of Shares Free > '
  pull result;sf.i=result
  if result='###PANIC' then SIGNAL GETOUT
  end
if di=5 then do


  options prompt 'Enter the new Trend Value > '
  pull result;tq.i=result
  if result='###PANIC' then SIGNAL GETOUT
  end
if di=6 then do


  options prompt 'Enter new Risk Value > '
  pull result;rv.i=result
  if result='###PANIC' then SIGNAL GETOUT
  end
signal SYSOPMENU

DELETEPLAYER:
say ''cr;say ''spaces'Delete a Player'cr;say ''cr
do n=1 to np
  if n<10 then say ''spaces' ['n']  'alias.n''cr
  else say ''spaces'['n']  'alias.n''cr
  end
say ''spaces' [X]  Exit to Sysop Menu'cr;say ''cr


options prompt 'Enter the number of the player you wish to delete > '
pull result;delplyr=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if delplyr='X' then signal SYSOPMENU
if delplyr<1 | delplyr>n then signal DELETEPLAYER
say ''cr;say ''alias.delplyr' will be deleted'cr
say ''cr;options prompt 'Are you sure (Y/n)? > '


pull result;choice=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if choice='N' then signal DELETEPLAYER
n=delplyr 
do j=1 to smv7
  if cn.n.j=0 then nop
  else do
    sf.cn.n.j=sf.cn.n.j+so.n.j;so.n.j=0;cn.n.j.=0
    end
  end
cr.n=0
say ''pen3'Player files will be updated when you exit the Stock Market.'def''cr
signal SYSOPMENU

VIEWFILES:
say '';say spaces'View Stock Market Files';say ''
say ''spaces'[1]  'file1''cr
say ''spaces'[2]  'file2''cr
say ''spaces'[3]  'file3''cr
say ''spaces'[4]  'file9''cr
say ''spaces'[X]  Exit to Sysop Menu'cr
say ''cr;options prompt 'Enter the number of the file to view > '


pull result;vfile=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if vfile='X' then signal SYSOPMENU
if vfile=1 then do
   call open(vf1,file1,'R')
   call readfl (vf1)
   call close (vf1)
   end
if vfile=2 then do
   call open(vf2,file2,'R')
   call readfl (vf2)
   call close (vf2)
   end
if vfile=3 then do
   call open(vf3,file3,'R')
   call readfl (vf3)
   call close (vf3)
   end
if vfile=4 then do
   call open(vf4,file9,'R')
   call readfl (vf4)
   call close (vf4)
   end
if vfile<1 | vfile>4 then signal VIEWFILES
say ''cr
options prompt 'Tap 'pen3'Return'def' to continue.'


pull result
if result='###PANIC' then SIGNAL GETOUT
say ''cr;signal VIEWFILES

RESETGAME:
time=time();date=date()
say ''cr;say ''spaces'Reset Game'spaces''cr;say ''cr
say 'A reset will delete all players and reset all companies to default'cr
say 'number of shares free.'cr
say ''cr;options prompt 'Are you sure (Y/n)? > '


pull result;choice=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if choice='N' then signal SYSOPMENU
call RESETPLAYERS;call RESETCOMPANIES;call RESETNEWS
EXIT
signal SYSOPMENU

ONELINER:
say ''cr
options prompt ''pen6'Leave a one line tip for the next Investor (y/N)?'def' > '


pull result;ans=upper(result)
if result='###PANIC' then signal BYE
if ans~='Y' then do
  say ''pen3'No'def''cr
  return
  end
say ''pen7'Yes'def''cr;say ''cr
options prompt''pen2'Enter a ONE LINE tip:'def' ';say ''cr

pull result; mess=result
if result='###PANIC' then signal BYE
if length(mess)<10 then do
  say ''pen3'Message too short, Entry Aborted.'def''cr;signal ONELINER
  end
say ''cr;say ''longdashes''cr;say ''pen5''mess ''alias.cu''def''cr;say ''longdashes''cr;say ''cr
options prompt ''pen4'Does this look OK?'def' > '


pull result;ans=upper(result)
if result='###PANIC' then signal BYE
if ans='N' then do
  say 'Nope!'cr;signal ONELINER
  end
say ''pen5'Yep!'def''cr
address command 'c:delete 'file7' quiet'
call open(fnx,file7,'W');call writeln(fnx,mess);call writeln(fnx,alias.cu);call close(fnx)
return

ALIAS:
say ''cr
options prompt ''pen6'Do you wish to use an 'pen3'ALIAS 'pen6'(Y/n)? >'def' '


pull result
choice=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if choice='N' then do
  say ''pen3'No'def''cr;alias=name;say ''cr;say ''pen6'You shall be known as'pen3'' alias''def''cr;return
  end
say ''pen5'Yes'def''cr
say ''cr
options prompt''pen2'Enter your 'pen3'ALIAS 'pen5'(Max 20 chars):'def' '
pull result
say ''cr

alias=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if length(alias)<1 then do
  say ''pen3'Alias too short, Entry Aborted.'def''cr
  signal ALIAS
  end
do n=1 to np
  if alias=alias.n then do
    say''cr;say ''pen4'Sorry, that ALIAS is already in use!'def''cr;signal ALIAS
    end
  end
say ''cr
say ''pen3''alias''def''cr
say ''cr
options prompt ''pen2'Does this look OK (Y/n)?'def' > '

pull result;ans=upper(result)
if result='###PANIC' then SIGNAL GETOUT
if ans='N' then do
  say ''pen3'Nope!'def''cr
  signal ALIAS
  end
say ''pen6'Yep!'def''cr
return

GAMEWON:
say ''cr
call ONELINER
say ''cr
say ''spaces''pen5'Thank you for playing Stock Market'def''spaces''cr
say ''cr;call delay(100)
say ''pen6'Returning to'pen3'' bbsname''def''cr;call delay(100)

BYE:
exit

SYNTAX:
code=rc
say'OOPS! Please tell SYSOP that the Stock Market Crashed!'cr
  SAY 'Line:' SIGL '   Error Code:' RC



 if ~exists(''bbspath'SM.errlog') then do
     call open(err,''bbspath'SM.errlog','W')
     end
 else call open(err,''bbspath'SM.errlog','A')
call writeln(err,date() time())
call writeln(err,name' - Stock Market')
call writeln(err,'Line - 'sigl' Code - 'code)
call writeln(err,errortext(code))
call writeln(err,'')
call close(err)

BREAK_C:
BREAK_E:
norxit = 0 /* Normal exit, = 0 if ctrl c is hit */
SIGNAL GETOUT
exit

checkBBS:
IF ADDRESS()~='BAUD' THEN RETURN 0
IF TIME('E')>secs THEN SIGNAL GETOUT
dcd
IF RC=0 THEN EXIT
temp=secs-TIME('E')
IF temp<120 THEN SAY '*** Only' temp 'seconds left! ***'CR 
RETURN 0

/**************************************
 *     call The PUB (817)690-3387     *
 **************************************

 *****************************************
 * call The Alternative BBS 503-761-3043 *
 *****************************************/