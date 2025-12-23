/*  STAR TREK - THE FINAL FRONTIER  - Online BBS Game
    Written By Steve Berry
    Rev A - 4/24/87 IBM 370
    Rev B - 5/21/88
         modified to run on Amiga                          
    Rev C - 10/23/91
         modified to run from CLI or BBBBS
         also updated enemy from Klingons to Romulans
    Mods for Atredes by Bob Hassel and Michael Cox
    ANSI version & other enhancements by Michael Cox
    BBBBS mods by Richard Lee Stockton
    3 bugs fixed August 24,1993 by Charles Johnston
    (1) Saved games would not survive a reload - FIXED
    (2) After torpedos were used up one could continue firing them - FIXED
    (3) After phasers were used up one could continue firing them -FIXED
*/

CALL TIME('R')
SIGNAL ON BREAK_C
bbspath=GETCLIP('BBS_path')
PARSE ARG UName . . . secs .
IF secs='' THEN secs=3600
gamepath=''
IF UName~='' & bbspath~='' THEN gamepath=bbspath'rexxDoors/Data/'
maxtime=5000
justcleared='true'
highest=0
highname=0
HOM='[H'
RESET='[0m'
POS='[13;0H'
CLS='[H[J'
CLR='[J'
CLL='[K'
WHITEONBLUE='[37;44m'
REDONBLUE='[31;44m'
YELLOWONBLUE='[33;44m'
CYANONBLACK='[36;40m'
YELLOWONBLACK='[33;40m'
MAGENTAONBLACK='[35;40m'
BLUEONBLACK='[34;40m'
REDONYELLOW='[31;43m'
WHITE='[37m'
RED='[31m'
YELLOW='[33m'
BLACK='[30m'
GREEN='[32m'
CYAN='[36m'
MAGENTA='[35m'
BLUE='[34m'
IF ADDRESS()~='BAUD' THEN
  DO
    BLUE=GREEN
    WHITEONBLUE='[37;42m'
    REDONBLUE='[31;42m'
    YELLOWONBLUE='[33;42m'
  END
call time 'R'           /* reset timer */
do i=1 to 10
 highscore.i=0
 highname.i=' '
end
stardate=time('E')
junk=randu(time('S'))
blanks='                                 '

call old_highest
CALL mess CLS
CALL mess BLUE||'                           Welcome to STARTREK!'
CALL mess
CALL mess YELLOW||'                             TOP TEN PLAYERS'
CALL mess WHITE
do i=1 to 10
   if highscore.i=0 then iterate
   CALL mess substr(blanks,1,20) highname.i substr(blanks,1,30-length(highname.i)) RIGHT(highscore.i,6)
end

CALL mess CYAN
CALL mess '         (You need an ANSI-compatible terminal to play this game.)'
CALL mess 
OPTIONS PROMPT '               Do you need instructions (y/N)? '
PULL res
 if res='Y' then
   call instructions
  else
  do                             /* Anyone ever heard of BRACKETS? */
   CALL mess
   CALL mess 'You can get the instructions at any time by entering the word HELP.'
   OPTIONS PROMPT 'Hit [RETURN] to continue: '
   PULL res
  end
CALL mess CLS
CALL mess 
CALL mess
OPTIONS PROMPT 'Load and play an old game (y/N)? '
PULL fn
CALL mess CLS

mission=1

/* Are we playing an OLD Game? */

if fn='Y' then
    call load_old_game fn
else

newmission:
 CALL mess 'Please stand by... universe under construction'
 select
  when mission=1 then call init_variables_1
  when mission=2 then call init_variables_2
  when mission=3 then call init_variables_3
  when mission=4 then call init_variables_4
  when mission=5 then call init_variables_5
  when mission=6 then call init_variables_6
  when mission=7 then call init_variables_7
  when mission=8 then call init_variables_8
  when mission=9 then call init_variables_9
  when mission=10 then call init_variables_10
/*  otherwise */
 end


/* ********** */
/* Main do loop */
/* ************ */

main: /* added this function name so 'load_old_game:' could signal to it*/
 
starttime=stardate
CALL mess CLS
do until energy=0
CALL checkBBS()
CALL mess HOM
 call print_quadrent x,y,ex,ey
 t0=time('R')

/* get user input and parse the commands */
 if stardate - starttime > maxtime then call toolong
 OPTIONS PROMPT 'Your command, Captain? '
 PULL instring
 rc=parse_command(instring)

/* process the commands */

  if rc=0 then do
    do loop=1 to 3
     if arg1.loop='' then leave
     call process_command arg1.loop,arg2.loop
     if alive='no' | energy <= 0 then leave
    end
   end
  else CALL mess "Command not understood. Enter HELP for reminder."

/* Check for illegal input */

  if missstat='completed' then do
    CALL mess CLS
    CALL mess
    CALL mess ' CONGRATULATIONS !!! You have completed the mission!'
    CALL mess
     exit 0
    end

/* Did player kill himself? */

 If alive='no' | energy <= 0 then do
   CALL mess CLS
   CALL mess
   CALL mess
   CALL mess
   CALL mess
   CALL mess ' Suicide is not in the BEST interests of the FEDERATION.'
   CALL mess
   CALL mess ' But you did get 'points' points.'
   CALL mess
   CALL mess
   CALL mess
   CALL mess 'Game over.'
   CALL mess
   CALL mess
   exit
  end

/* Time for a little wear and tear on the enterprize */

 call wear_and_tear

/* Is the Enterprize DEAD in Space? */

 If alive='no' then do
   CALL mess CLS
   CALL mess
   CALL mess
   CALL mess
   CALL mess
   CALL mess ' Sorry but you let the Enterprise deteriorate into '
   CALL mess ' into a rusting hunk of space debris.'
   CALL mess
   CALL mess ' But you did get 'points' points.'
   CALL mess
   CALL mess
   CALL mess
   CALL mess 'Game over.'
   CALL mess
   CALL mess
   exit
  end

/* Ok. now it's the Bad guy's turn */

/* now call mission specific routine */
/* for bad guy routine and points awarded */

 select
  when mission=1 then call mission1
  when mission=2 then call mission2
  when mission=3 then call mission3
  when mission=4 then call mission4
  when mission=5 then call mission5
  when mission=6 then call mission6
  when mission=7 then call mission7
  when mission=8 then call mission8
  when mission=9 then call mission9
  when mission=10 then call mission10
 end

/* Is the Enterprize KILLED? KRUSHED? DESTROYED? */

 If alive='no' then do
   CALL mess
   CALL mess
   CALL mess
   CALL mess
   CALL mess ' Sorry but you let your crew and the Federation down.'
   CALL mess ' Perhaps you should consider another career.'
   CALL mess
   CALL mess ' But you did get 'points' points.'
   CALL mess
   CALL mess
   CALL mess
   CALL mess 'Game over.'
   CALL mess
   CALL mess
   exit
  end

/* end of the loop - now go print the screen */
end


/* ********************************************** */
/* ********************************************** */
/*          SUBROUTINE SECTION                    */
/* ********************************************** */
/* ********************************************** */



/* ********************************************** */
/* ********************************************** */
/*          SUBROUTINE LOAD_OLD_GAME              */
/*  PASSED VALUES - fn                            */
/* ********************************************** */
/* ********************************************** */

load_old_game:


bstr='   |o| * <->(B))\_ # '
missstat='not yet'
CALL mess 'Loading your previously saved game...'
/* get variables */
 if ~Open(fnn,gamepath||UName,'R') then

do
CALL mess "Can't find a saved game for you! Exiting STARTREK."
exit
end
 inn=Readln(fnn)
 parse var inn stardate mission energy points x y ex ey shields klshields docked kltot
 inn=Readln(fnn)
 parse var inn status.0 status.1 status.2 status.3 status.4 status.5

/* start interval timer */

stardate=stardate + time('E')

/* Load galactic map */

 do l=0 to 9
  inn1=Readln(fnn)
  parse var inn1 map.l.0 map.l.1 map.l.2 map.l.3 map.l.4 map.l.5 map.l.6 map.l.7 map.l.8 map.l.9
  
 end

 do l=0 to 9
  inn1=Readln(fnn)
  parse var inn1 quad.l.0 quad.l.1 quad.l.2 quad.l.3 quad.l.4 quad.l.5 quad.l.6 quad.l.7 quad.l.8 quad.l.9
 end
alive='yes'
junk=Close(fnn)
signal main /* bug fix so old game is loaded properly and not overwritten as fresh game*/
return

/* ********************************************** */
/* ********************************************** */
/*          SUBROUTINE INIT_VARIABLES_1           */
/*  PASSED VALUES - none                          */
/* ********************************************** */
/* ********************************************** */

init_variables_1:

bstr='   |o| * <->(B))\_ # '
alive='yes'
energy=1000000
x=0
y=0
points=0
mission=1
shields='down'
kltot=0
missstat='not yet'

do a=0 to 9
 do b=0 to 9
   map.a.b=random(0,1) * 100 + random(0,1)*10 + random(0,9)
   if map.a.b > 99 then kltot=kltot + 1
 end
end

klshields=5000
status.0=0
status.1=0
status.2=0
status.3=0
status.4=0
status.5=0
ex=5
ey=4


newscreen:
/* initialize quadrent matrix */

enemies=map.x.y%100
bases=(map.x.y - enemies * 100)%10
stars=map.x.y - enemies * 100 - bases * 10

/* zero quad matrix */

do i=0 to 9
 do j=0 to 9
  quad.i.j=0
 end
end

/* locate the enterprize */

quad.ex.ey=1

/* put the stars in */

do a=0 to stars - 1
 rx=random(0,9)
 ry=random(0,9)
 if quad.rx.ry=0 then quad.rx.ry=2
 else a=a-1
end

/* enemies? */

if enemies ~= 0 then do
 do a=0 to enemies - 1
   rx=random(0,9)
   ry=random(0,9)
   if quad.rx.ry=0 then quad.rx.ry=3
   else a=a-1
 end
end
/* bases ? */

if bases ~= 0 then do
 do a=0 to bases - 1
   rx=random(0,9)
   ry=random(0,9)
   if quad.rx.ry=0 then quad.rx.ry=4
   else a=a-1
 end
end
docked='no'

return

/* ********************************************** */
/* ********************************************** */
/*          SUBROUTINE PRINT_QUARDENT             */
/*  PASSED VALUES - x, y ,ex ,ey                  */
/* ********************************************** */
/* ********************************************** */

print_quadrent:

call dock?  /* check to see if the Enterprize is docked */

stardate=stardate + time('E')

if status.5 < 1 | docked='yes' then astr=bstr
else   astr='XXX|o|XXXXXXXXXXXXXXX'
CALL mess RESET
do i0=0 to 9
 line='         '|| WHITEONBLUE
 do j0=0 to 9
  select
    when quad.i0.j0=0 then line=line || substr(astr,quad.i0.j0 * 3 + 1,3)
    when quad.i0.j0=1 then line=line || WHITE || substr(astr,quad.i0.j0 * 3 + 1,3)
    when quad.i0.j0=2 then line=line || YELLOW || substr(astr,quad.i0.j0 * 3 + 1,3)||WHITE
    when quad.i0.j0=3 then line=line || RED || substr(astr,quad.i0.j0 * 3 + 1,3)||WHITE
    when quad.i0.j0=4 then line=line || GREEN || substr(astr,quad.i0.j0 * 3 + 1,3)||WHITE
    when quad.i0.j0=5 then line=line || CYAN || substr(astr,quad.i0.j0 * 3 + 1,3)||WHITE
    when quad.i0.j0=6 then line=line || MAGENTA || substr(astr,quad.i0.j0 * 3 + 1,3)||WHITE
  otherwise nop
  end
 end
 select
  when i0=0 then line=line || YELLOWONBLACK ||'  Stardate: 'stardate'    '
  when i0=1 then
  do
   if energy > 10000 then line=line || YELLOWONBLACK ||'  Energy: 'energy'    '
   else
   line=line || YELLOWONBLACK ||'  Energy: '||REDONYELLOW||energy'    '
  end
  when i0=2 then line=line || YELLOWONBLACK ||'  Quadrant:' y',' x'    '
  when i0=3 then line=line || YELLOWONBLACK ||'  Sector:  ' ex',' ey '  '
  when i0=4 & docked='yes' then line=line || YELLOWONBLACK || '  Condition: '||GREEN||'DOCKED'
  when i0=4 & map.x.y > 99 & docked='no' then line=line || YELLOWONBLACK ||'  Condition: '||RED||'RED    '
  when i0=4 & map.x.y < 100 & docked='no' then line=line || YELLOWONBLACK || '  Condition: '||GREEN||'Green    '
  when i0=5 then

   do
    if shields='UP' then line=line || YELLOWONBLACK ||'  Shields:' CYANONBLACK||'Up    '
    else
    line=line || YELLOWONBLACK ||'  Shields:' MAGENTAONBLACK||'Down    '
   end
  when i0=6 then line=line || YELLOWONBLACK ||'  Romulans left:' kltot'    '
  when i0=7 then line=line || YELLOWONBLACK ||'  Points:' points'    '
  otherwise line=line || RESET||'                       '
 end
 if (line ~= oldline.i0)|(justcleared='true') then CALL mess line; else CALL mess ""
 oldline.i0=line
end
CALL mess RESET
call clearbottom
justcleared='false'
return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE PARSE_COMMAND             */
/*  PASSED VALUES - INSTRING                      */
/* ********************************************** */
/* ********************************************** */

parse_command:

parse arg arg1.1 arg2.1 arg1.2 arg2.2 arg1.3 arg2.3 .

/* string for recognition of commands */

comstr='COMFIRTORPHADISMAPSHOSENLONRAILOWSHIWARIMPLAUPROCAPLOGTRASHIDOWSAVGAMINSQUIDRISTAHELPOWSCASEE'

/* Check for no input */

if arg1.1='' then return -1
if arg1.1='#' then
  do
  exit
  end

/* trash all but the first three letters of each command */

do i=1 to 3
 arg1.i=left(arg1.i,3)
 arg2.i=left(arg2.i,3)
 if arg1.i='' then leave
 do j=0 to 30
  if arg1.i=substr(comstr,j * 3 + 1,3) then leave
 end
 if j > 30  then return -1
 if arg2.i='' then return 0
  do j=0 to 30
   if arg2.i=substr(comstr,j * 3 + 1,3) then leave
  end
 if j > 30  then return -1
end

return 0

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE PROCESS_COMMAND           */
/*  PASSED VALUES - word1 , word2                 */
/* ********************************************** */
/* ********************************************** */

process_command:

arg word1,word2
select
  when word1='FIR' then call weapon_control word2
  when word1='COM' then call compute word2
  when word2='SEN' then call sensors word1
  when word2='SCA' then call sensors word1
  when word1='SEN' then call sensors word1
  when word1='SCA' then call sensors word1
  when word2='SHI' then call shield_control word1
  when word2='DRI' then call engine_control word1
  when word2='POW' then call engine_control word1
  when word1='INS' then call instructions
  when word2='INS' then call instructions
  when word1='HEL' then call instructions
  when word1='SAV' then call save_game word2
  when word1='DIS' then call display_info word2
  when word1='SEE' then call display_info word2
  when word1='STA' then call display_info word2
  when word1='LAU' then call launch_probe word2
  when word1='CAP' then call captains_log word2
  when word1='QUI' then
  do
  call new_highest
  CALL mess 'Exiting STARTREK... play again soon!'
exit
  end
  otherwise CALL mess 'Sorry - not understood'
end

return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE WEAPON_CONTROL            */
/*  PASSED VALUES - weapon                        */
/* ********************************************** */
/* ********************************************** */

weapon_control:

arg weapon

if (weapon ~= 'PHA') & (weapon ~= 'TOR') then do
   CALL mess
   CALL mess 'Weapon does not exist.'
   return
  end

/* Phasers section */

if weapon='PHA' then do
  if map.x.y < 100 then do
    CALL mess 'No targets to fire on.   '
    return
   end

 /* compute the distance  between Enterprize and Romulan */

 kx=0
 ky=0
 do i=0 to 9
  do j=0 to 9
   if quad.i.j=3 then do
     kx=i
     ky=j
    end
  end
 end
/* compute the distance (rough approximation) */
 distance=((ex-kx)**2 + (ey-ky)**2)
 do i=1 to 13 by .5
  t0=distance/i
  if t0 < i then leave
 end
 distance=i
 energy=energy - 5000
 hit=(1 - status.1)* 5000 - (distance * 500)
 if hit < 0 then hit=0
 klshields=klshields - hit
 if hit=0 then
   do  /* Bug fix for when out of phasers, old code let keep shooting endlessly*/
    call mess 'Sorry Captain you have exhausted the Phaser Banks!'
    call delay(100)
    return
   end
 CALL mess
 CALL mess 'PHASERS LOCKED ON TARGET '
 CALL mess 'Phasers fired -'
 if klshields <= 0 then do
    CALL mess 'ROMULAN HIT WITH' hit ' units of energy'
    CALL mess '*** ROMULAN DESTROYED! ***'
    points=points + 50
    klshields=5000
    quad.kx.ky=0
    map.x.y=map.x.y - 100
    kltot=kltot - 1
   end
 else
   do
    CALL mess 'KILINGON HIT WITH' hit ' units of energy'
    CALL mess 'The Romulan ship is still alive, Captain!'
   end
 end /* end of phaser routine */
else do
  /* Torpedo routine */
    if weapon='TOR' & status.2 >.9 then  
    do /* bug fix for when out of torpedos, old code let you shoot endlessly*/
     call mess 'Sorry Captain we are all out of torpedos!'
     call delay(100)
     return
    end
     CALL mess 'Torpedos ready Captain.'
     do until c=0
      OPTIONS PROMPT 'X vector, Captain (-1=left, 0=middle, 1=right)? '
      PULL dy
      OPTIONS PROMPT 'Y vector, Captain (-1=up, 0=middle, 1=down)? '
      PULL dx
      if dx='' then dx=0
      if dy='' then dy=0
      if (dx=-1 | dx=0 | dx=1) & (dy=-1|dy=0|dy=1) then c=0
      else
       do
        c=1
        CALL mess 'Invalid vectors. Valid range for each is -1 to 1.'
       end
     end
    tx=ex
    ty=ey
    do lp=0 to 9
     t0=tx+dx
     t1=ty+dy
     if (t0 < 0) | (t0 > 9) | (t1 < 0) | (t1 > 9) then do
       CALL mess ' Torpedo missed!'
       return
      end
     space=quad.t0.t1
     if space ~= 0 then select
       when space=2 then do
         CALL mess '*** Torpedo hits a STAR ***'
         return
        end
       when space=3 then do
         CALL mess '*** Torpedo hits a ROMULAN ***'
         CALL mess '*** ROMULAN DESTROYED ***'
         CALL mess 'Good shot, Captain!'
         quad.t0.t1=0
         map.x.y=map.x.y - 100
         kltot=kltot - 1
         points=points + 100
         return
        end
       when space=4 then do
         CALL mess '*** Torpedo hits a BASE ***'
         CALL mess 'COURT MARTIAL IS IMMINENT!'
         return
        end
       otherwise do
         CALL mess '*** Torpedo hits the DEATHSTAR ***'
         CALL mess 'Torpedo seems to have no effect!'
         return
        end
       end
      quad.t0.t1=6
      tx=t0
      ty=t1
      CALL mess HOM
      call print_quadrent
      quad.tx.ty=0
     end
 end /* end of Torpedo routine */
return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE WEAR_AND_TEAR             */
/*  PASSED VALUES - none                          */
/* ********************************************** */
/* ********************************************** */

wear_and_tear:

pick=random(0,5)
if docked='yes' then
 if status.pick=0 then return
 else status.pick=status.pick - .1
else if status.pick < 1 then do
  status.pick=status.pick + .1
  if status.pick=1 then do
    CALL mess 'Status report sir -'
    select
     when pick=0 then CALL mess 'Computer is in need of repair!'
     when pick=1 then CALL mess 'Phasers are in need of repair!'
     when pick=2 then CALL mess 'Torpedos are in need of repair!'
     when pick=3 then CALL mess 'Warp Drive is in need of repair!'
     when pick=4 then CALL mess 'Impulse Drive is in need of repair!'
     when pick=5 then CALL mess 'Sensors are out of order!'
    end
   if status.3 > .9 & status.4 > .9 then do
     CALL mess 'Captain sir ... we seem to be dead in space!'
     CALL mess 'The warp drive and the impulse drive are out!'
     alive='no'
    end
  end
 end
return


/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE engine_control            */
/*  PASSED VALUES - power                         */
/* ********************************************** */
/* ********************************************** */

engine_control:

arg power

CALL mess 'Scotty here captain.'
if power='WAR' then do
  if status.3 > 1.2 then
  do
  CALL mess 'Sorry Captain, warp drive is out of commission!'
  return
  end
  OPTIONS PROMPT 'What Warp factor Captain? '
  PULL warp
  if warp='' then warp=0
  do forever
   OPTIONS PROMPT 'Destination X vector (0-9)? '
   PULL dy
   OPTIONS PROMPT 'Destination Y vector (0-9)? '
   PULL dx
   if datatype(dx,'W') & datatype(dy,'W') then leave
   CALL mess 'Where?'
  end
   if x > 9 | y < 0 | x  < 0 | y > 9 then do
    energy=energy - 5000
    CALL mess 'Captain, the warp drive is in a bad way'
    CALL mess 'and we are losing energy fast!'
   end
 /* if warp drive damaged degrade efficiency of energy consumption */
  energy=energy - ((((dx-x)**2 + (dy-y)**2)/2.5)*1000+status.3*5000)
  if dx > 9 | dy < 0 | dx < 0 | dy > 9 then do
     CALL mess
     CALL mess 'Warping out of the galaxy is not a good idea. Range 0-9 please.'
     CALL mess
     if shields='down' then do
        CALL mess ' SHIELDS are DOWN --- the ENTERPRISE is caught, in the void of intergalatic space'
        CALL mess ' Damage control report - WARP drive damaged, Impulse drive damaged, sensors out!'
        status.3=status.3 + 1
        status.4=status.4 + 1
        status.5=1.1
      end
     else energy=energy - 5000
    end
  x=dx
  y=dy
  if x > -1 & x < 10 & y > -1 & y < 10 then call newscreen
  end
 else
  do
   if power ~= 'IMP' | status.4 > .9 then do
     CALL mess 'Sorry Captain, impulse power is out of commission!'
     return
    end
   else
    do
     CALL mess 'Aye Captain - Impulse power it is'
     OPTIONS PROMPT 'What factor, Captain? '
     do forever      
       PULL factor
       if datatype(factor,'W') then leave
       CALL mess 'Whats that, Captain? '
     end
     if factor <= 0 & num='yes' then return
     do until c=0
      OPTIONS PROMPT ' X vector (-1=left, 0=middle, 1=right)? '
      PULL dy
      OPTIONS PROMPT ' Y vector (-1=up, 0=middle, 1=down)? '
      PULL dx
      if dx='' then dx=0
      if dy='' then dy=0
      if (dx=-1 | dx=0 | dx=1) & (dy=-1|dy=0|dy=1) then c=0
      else
       do
        c=1
        CALL mess 'Invalid vectors - valid range for each is -1 to 1.'
       end
     end
    do i=0 to factor - 1
     if ((ex+dx) < 0) | ((ex+dx) > 9) | ((ey+dy) < 0) |((ey+dy) > 9) then
      do
       CALL mess ' You have to use the Warp drive to exit the quadrant.'
       return
      end
     t0=ex+dx
     t1=ey+dy
     space=quad.t0.t1
     if space ~= 0 then select
       when space=2 then do
         CALL mess '*** COLLISION with STAR ***'
         alive='no'
        end
       when space=3 then do
         CALL mess '*** COLLISION with ROMULAN ***'
         alive='no'
        end
       when space=4 then do
         CALL mess '*** COLLISION with BASE ***'
         alive='no'
        end
       otherwise do
         CALL mess '*** SWALLOWED BY THE DEATHSTAR ***'
         alive='no'
        end
       end
      quad.ex.ey=0
      quad.t0.t1=1
      ex=t0
      ey=t1
      if alive='yes' then do
        CALL mess HOM
        call print_quadrent
      end
     end
    end
   end

/* Got to check to see if Enterprize is docked */
dock?:

docked='no'
do i1=-1 to 1
 do j1=-1 to 1
  if i1+ex > -1 & i1+ex < 10 & j1+ey > -1 & j1+ey < 10 then do
    ta=i1+ex
    tb=j1+ey
    if quad.ta.tb=4 then do

     docked='yes'
     t=status.0 + status.1 + status.2 + status.3 + status.4 + status.5
     stardate=stardate + t
     status.0=0
     status.1=0
     status.2=0
     status.3=0
     status.4=0
     status.5=0
    end
   end
 end
end
if kltot=0 then missstat='completed'

return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE SAVE_GAME                 */
/*  PASSED VALUES - filename                      */
/* ********************************************** */
/* ********************************************** */

save_game:



missstat='not yet'

/* save variables */


 if ~Open(fnn,gamepath||UName,'W') then do
  CALL mess '***Open return code=' rc 'on 'bbspath'rexxDoors/Data/'||UName
  CALL mess 'File may be in use, or is unable to be opened. Try again.'
  return
 end
CALL mess 'Storing your game...'
if Writeln(fnn,stardate mission energy points x y ex ey shields klshields docked kltot) < 1 then
 do
exit
end
if Writeln(fnn, status.0 status.1 status.2 status.3 status.4 status.5) < 1 then
 do
exit
end

/* Save galactic map */

 do l=0 to 9
  if Writeln(fnn,map.l.0 map.l.1 map.l.2 map.l.3 map.l.4 map.l.5 map.l.6 map.l.7 map.l.8 map.l.9) < 1 then
  do
    exit
  end
 end

 do l=0 to 9
if Writeln(fnn,quad.l.0 quad.l.1 quad.l.2 quad.l.3 quad.l.4 quad.l.5 quad.l.6 quad.l.7 quad.l.8 quad.l.9) < 1 then
  do
  exit
  end
 end
 if ~Close(fnn) then
  do
exit
end
return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE SENSORS                   */
/*  PASSED VALUES - type                          */
/* ********************************************** */
/* ********************************************** */

sensors:

arg type

if status.5 < 1.1 then do
  do i=-1 to 1
   CALL mess '-------------------'
   line=': '
   do j=-1 to 1
     t0=x+i
     t1=y+j
     if t0 < 0 | t0 > 9 | t1 < 0 | t1 > 9 then line=line || 'XXX :'
     else
      do
       if map.t0.t1 < 100 & map.t0.t1 > 9 then line=line || ' '
       if map.t0.t1 < 10 then line=line || '  '
       line=line || map.t0.t1 || ' :'
      end
   end
   CALL mess line
  end
end
else
  if status.5 > 1 then do
    CALL mess ' Long-range sensors are out, captain!'
    CALL mess
  end
 CALL mess '-------------------'
 CALL mess
 OPTIONS PROMPT 'Hit RETURN to resume: '
 PULL res
 call clearbottom
return


/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE SHIELD_CONTROL            */
/*  PASSED VALUES - direction                     */
/* ********************************************** */
/* ********************************************** */

shield_control:

arg direction

if direction='RAI' then
 if shields='UP' then do
   CALL mess 'Shields are already up.'
  end
 else
  do
   shields='UP'
   CALL mess 'Shields up, Captain.'
   energy=energy - 3000
  end
else
 if direction='LOW' then
  if shields='down' then do
    CALL mess 'Shields are already down.'
   end
  else
   do
    shields='down'
    CALL mess 'Shields down, Captain.'
    energy=energy + 3000
   end
 else
  do
   CALL mess ' I am confused by that order, Captain.'
   return
  end
return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE DISPLAY_INFO              */
/*  PASSED VALUES - what                          */
/* ********************************************** */
/* ********************************************** */

display_info:

arg what

  CALL mess CLS
  CALL mess
  CALL mess ' Status report for Enterprise - Stardate ' stardate
  CALL mess ' ----------------------------------'
  c=100 - status.0*100
  CALL mess ' Computer      status ' c'%'
  CALL mess ' ----------------------------------'
  c=100 - status.1*100
  CALL mess ' Phasers       status ' c'%'
  CALL mess ' ----------------------------------'
  c=100 - status.2*100
  CALL mess ' Torpedos      status ' c'%'
  CALL mess ' ----------------------------------'
  c=100 - status.3*100
  CALL mess ' Warp drive    status ' c'%'
  CALL mess ' ----------------------------------'
  c=100 - status.4*100
  CALL mess ' Impulse drive status ' c'%'
  CALL mess ' ----------------------------------'
  c=100 - status.5*100
  CALL mess ' Sensors       status ' c'%'
  CALL mess ' ----------------------------------'
  OPTIONS PROMPT 'Hit RETURN to resume: '
  PULL res
CALL mess CLS
justcleared='true'
return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE INSTRUCTIONS              */
/*  PASSED VALUES - none                          */
/* ********************************************** */
/* ********************************************** */

instructions:

CALL mess CLS
CALL mess 'Welcome to STARTREK.'
CALL mess
CALL mess 'This version of STARTREK is very simple and has been heavily'
CALL mess 'modified for better playability. The universe consists of a 10 by 10'
CALL mess 'grid of quadrants, each quadrant also consisting of a 10 by 10 grid.'
CALL mess '0,0 is the top left corner; Y is vertical and X is horizontal.'
CALL mess
CALL mess 'The object is to destroy all the Romulan ships in the galaxy.'
CALL mess
CALL mess 'You will see several objects as you play; the most common ones are:'
CALL mess
CALL mess '   |o|=the Enterprise'
CALL mess '   <->=Romulans'
CALL mess '   (B)=a Starbase'
CALL mess '    * =a star'
CALL mess
OPTIONS PROMPT 'Hit [RETURN] to continue: '
PULL res
CALL mess CLS
CALL mess 'COMMANDS'
CALL mess
CALL mess 'The command line takes two-word combinations. You can abbreviate'
CALL mess 'words to 3 letters. The most common commands are'
CALL mess
CALL mess '   RAISE SHIELDS (hint!)'
CALL mess '   LOWER SHIELDS'
CALL mess '   FIRE PHASERS'
CALL mess '   FIRE TORPEDOS'
CALL mess '   IMPULSE POWER'
CALL mess '   WARP DRIVE'
CALL mess '   SCANNERS (or SENSORS)'
CALL mess '   STATUS'
CALL mess '   SAVE (Saves game WITHOUT exiting)'
CALL mess '   QUIT (Exit game entirely)'
CALL mess '   HELP (Gives you this file again)'
CALL mess
OPTIONS PROMPT 'Hit [RETURN] to continue: '
PULL res
CALL mess CLS
CALL mess 'VECTORS: When firing torpedoes or moving the ship you are asked'
CALL mess 'for X and Y vectors. Each of these can be -1, 0, or 1. This'
CALL mess 'indicates the X or Y direction. For instance, if X=0 and Y=1,'
CALL mess 'this indicates a vector straight down. If both are -1, this would'
CALL mess 'indicate an upper-left direction.'
CALL mess
CALL mess 'IN ORDER TO STAY ALIVE you must NOT run out of energy, exceed'
CALL mess 'Stardate 5000.0, or be caught with your shields down.'
CALL mess
CALL mess 'The Enterprise will from time to time need maintenance. Do this'
CALL mess 'by pulling up next to a Starbase.'
CALL mess
OPTIONS PROMPT 'Hit [RETURN] to continue: '
PULL res
CALL mess CLS
justcleared='true'
return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE CAPTAINS_LOG              */
/*  PASSED VALUES - word2                         */
/* ********************************************** */
/* ********************************************** */

captains_log:

CALL mess 'Sorry, not implemented'

return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE LAUNCH_PROBE              */
/*  PASSED VALUES - word2                         */
/* ********************************************** */
/* ********************************************** */

launch_probe:

CALL mess 'Sorry, not implemented'

return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE COMPUTE                   */
/*  PASSED VALUES - word2                         */
/* ********************************************** */
/* ********************************************** */

compute:
CALL mess 'Sorry, not implemented'


return

/* ********************************************** */
/* ********************************************** */
/*           SUBROUTINE MISSION1                  */
/*  PASSED VALUES - none                          */
/* ********************************************** */
/* ********************************************** */

mission1:

/* make the Romulans active aggressors */
/* Mission 1 - WAR with the ROMULANS! */

/* Any Romulans in the sector? */
if map.x.y > 99 then do
  do i=0 to 9     /* locate the Romulan in the quadrent */
   do j=0 to 9
    if quad.i.j=3 then
     do
      tx=i
      ty=j
     end
   end
  end

 /* got him - now move and fire */
 if tx > ex then t0=-1
 if tx=ex then t0=0
 if tx < ex then t0=1
 if ty > ey then t1=-1
 if ty=ey then t1=0
 if ty < ey then t1=1
 /* now how much? */
 factor=((tx-ex)**2 + (ty-ey)**2)
 do i=1 to 13 by .5
  tm=factor/i
  if tm < i then leave
 end
 factor=i%1
 /* now move him */

kl='yes'
 do i=0 to factor - 1
  if factor < 1 then leave
  t2=tx+t0
  t3=ty+t1
  if t2 < 0 | t2 > 9 | t3 < 0 | t3 > 9 then leave
  space=quad.t2.t3
  if space=1 then leave
  if space ~= 0 & kl='yes' then select
    when space=2 then do
      CALL mess '*** ROMULAN COLLIDES with STAR ***'
      quad.tx.ty=0
      map.x.y=map.x.y - 100
      kltot=kltot - 1
      kl='no'
     end
    when space=3 then do
      CALL mess '*** ROMULAN COLLIDES with ROMULAN ***'
      quad.tx.ty=0
      quad.t2.t3=0
      map.x.y=map.x.y - 200
      kltot=kltot - 2
      kl='no'
     end
    when space=4 then do
      CALL mess '*** ROMULAN COLLIDES with BASE ***'
      quad.tx.ty=0
      quad.t2.t3=0
      map.x.y=map.x.y - 100
      kltot=kltot - 1
      kl='no'
     end
    otherwise
     do
      CALL mess space
      CALL mess '*** ROMULAN EATEN FOR LUNCH! ***'
      points=points + 50
      quad.tx.ty=0
      map.x.y=map.x.y - 100
      kltot=kltot - 1
      kl='no'
     end
   end
  if kl='yes' then do
    quad.tx.ty=0
    quad.t2.t3=3
    tx=t2
    ty=t3
    CALL mess HOM
    call print_quadrent
   end
 end
if docked='yes' then return
/* now fire */
if kl='yes' then do
  amount=klshields/5000 * random(100,500) * 10
  if amount > 1000 & shields='down' then do
   CALL mess '--- ROMULAN FIRES PHASERS ---'
   CALL mess '*** ENTERPRISE SHIELDS ARE DOWN ***'
   CALL mess '*** ENTERPRISE HIT with 'amount ' UNITS OF ENERGY ***'
   CALL mess '*** ENTERPRISE DESTROYED ***'
   alive='no'
  end
  if amount < 3000 & shields='UP' then do
   CALL mess '--- ROMULAN FIRES PHASERS ---'
   CALL mess '*** ENTERPRISE SHIELDS ARE UP ***'
   CALL mess '*** Shields absorb impact --- no damage ***'
   energy=energy - amount
  end
  if amount > 3000 & shields='UP' then do
   CALL mess '--- ROMULAN FIRES PHASERS ---'
   CALL mess '*** ENTERPRISE SHIELDS ARE UP ***'
   hit=amount - 3000
   CALL mess '*** ENTERPRISE HIT WITH' hit ' UNITS OF ENERGY ***'
   energy=energy - amount
   do i=0 to hit/100
    call wear_and_tear
   end
  end
  if amount < 1001 & shields='down' then do
    CALL mess '--- ROMULAN FIRES PHASERS ---'
    CALL mess '*** ENTERPRISE SHIELDS ARE DOWN ***'
    hit=amount
    CALL mess '*** ENTERPRISE HIT WITH' hit ' UNITS OF ENERGY ***'
    do i=0 to hit/100
     call wear_and_tear
    end
   end
  end
end

/* end for Romulans in sector ... if < 3 here go get some */

if map.x.y < 299 & alive='yes' then do
 found='no'

  do i=-1 to 1     /* locate a Romulan in the Galaxy */
   do j=-1 to 1
     tx=i+x
     ty=j+y
     if (tx > -1) & (tx < 10) & (ty > -1) & (ty < 10) then do
       if (map.tx.ty > 99) & ((i ~=0 )|(j ~=0 )) then do
         found='yes'
         i=1
         j=1
        end
      end
   end
  end
if found='yes' then do
  CALL mess 'CAPTAIN! A Romulan warship has just warped into the quadrant!'
  map.tx.ty=map.tx.ty - 100
  map.x.y=map.x.y + 100
  do until a=1
   rx=random(0,9)
   ry=random(0,9)
   if quad.rx.ry=0 then do
     quad.rx.ry=3
     a=1
    end
   else a=0
  end
 end
end
if kltot=0 then missstat='completed'
return

/* MISSION 2 */

init_variables_2:

return


mission2:

return

/* MISSION3 */

init_variables_3:

return

mission3:


/* MISSION4 */

init_variables_4:

return

mission4:


/* MISSION5 */

init_variables_5:

return

mission5:

/* MISSION6 */

init_variables_6:

return

mission6:

/* MISSION7 */

init_variables_7:

return

mission7:

/* MISSION8 */

init_variables_8:

return

mission8:

/* MISSION9 */

init_variables_9:

return

mission9:

return

old_highest:
 if ~Open(fnn,gamepath'HighScore','R') then return
 do i=1 to 10
  highname.i=Readln(fnn)
  highscore.i=Readln(fnn)
 end
 junk=Close(fnn)
return



new_highest:
 if UName='' then return  /* saves from screwups from running offline */
 if points <= highscore.10 then return
 CALL mess 'CONGRATULATIONS!'
 CALL mess 'You are in the top 10! Saving your name and score for posterity...'
 highscore.10=points
 highname.10=UName
 do forever
   changes=0
   do i=1 to 9
    ihatearexx=i+1
    if highscore.ihatearexx > highscore.i then
     do
      temp=highscore.i
      tempname=highname.i
      highscore.i=highscore.ihatearexx          /* Bubblehead sort */
      highname.i=highname.ihatearexx
      highscore.ihatearexx=temp
      highname.ihatearexx=tempname
      changes=1
     end
   end
   if changes=0 then leave
 end
   if ~Open(fnn,gamepath'HighScore','W') then return
   do i=1 to 10
     if Writeln(fnn,highname.i) < 1 then return
     if Writeln(fnn,highscore.i) < 1 then return
   end
   junk=Close(fnn)
return


clearbottom:
 CALL mess POS
 CALL mess CLR
return


toolong:
CALL mess ''
CALL mess 'Sorry, time up for this round.'
OPTIONS PROMPT 'Would you like to SAVE your game before exiting (Y/N)? '
PULL choice
if choice='Y' then call save_game
if points > highest then call new_highest

BREAK_C:
CALL mess 'Exiting STARTREK... play again soon!'
exit
return


checkBBS:
IF ADDRESS()~='BAUD' THEN RETURN 0
IF TIME('E')>secs THEN SIGNAL toolong
dcd
IF RC=0 THEN EXIT
temp=secs-TIME('E')
IF temp<120 THEN SAY '*** Only' temp 'seconds left! ***'CR 
RETURN 0


mess:
PARSE ARG string
SAY string'0D'x
RETURN

/* end */
