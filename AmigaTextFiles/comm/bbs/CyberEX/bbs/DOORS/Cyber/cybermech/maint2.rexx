/* Cybermhen call restart */
parse arg n
yty=0
if upper(n)='AUTO' then signal automaint
if datatype(n)=NUM then signal main2

main:
say '0 - Exit'
say '1 - ReGenerate Game Map'
say '2 - Create new Mechs'
say '3 - Place Pirates on map'
say '4 - Re-start Game'
say '5 - Set City Computer'
say '6 - Set Cryon Parts'
pull n
main2:
if n=1 then call map
if n=2 then call mech
if n=3 then call pirate
if n=0 then exit
if n=4 then call flush
if n=5 then call mainc
if n=6 then call part
signal main
return


map:
say 'Create a new map? In other words,'
say 'Totally wipe the map of both players and pirates? (YN)'
pull n
n=upper(n)
if n='N' then signal nope

if open('ir','DH2:BBS/Doors/cyber/cybermech/map','W') then nop

do i=1 to 625
     m=0
     t=random(0,5,time('S'));
     o.i=t;p.i=0
     t.i=random(0,20,time('S'));if t=10 then t=1
	else t.i=0
     call writeln('ir',m'-'o.i'-'t.i'-'p.i'-0')
     end
call close('ir')
i=0
say 'Searching for unattached pirates, you must create new pirates'

looper:
i=i+1
if i=101 then return
file='DH2:BBS/Doors/cyber/cybermech/'||i
if open('ir',file,'R') then nop
	else signal looper

 name=readln('ir');
 handle=readln('ir');
call close('ir');
if handle='PIRATE' then address command delete file
if handle='PIRATE' then say 'Deleted Pirate #'i
if i>100 then return
signal looper


nope:
yty=0
say 'Clear the pirate base?';pull yy;yy=upper(yy)
if yy='Y' then yty=1
automet:
open('log','DH2:BBS/Doors/cyber/cybermech/log','A')
call writeln('log',' ')
call writeln('log','Cybermech Maintence Program Run')
call writeln('log','Meteor Shower, ore is replentished')
call writeln('log','Arexx interface cleared')
if yty=1 then call writeln('log','The Pirate City has Been reconstructed!')
call close('log')
if open('r','DH2:BBS/Doors/cyber/cybermech/map','R') then nop
do i=1 to 625
     temp=readln('r')
     parse var temp m.i '-' o.i '-' t.i '-' p.i '-' s.i
     if datatype(m.i)=CHAR then say 'Found player #'m.i' at location 'i;
     if yty=1 then if m.i=999 then m.i=0
     if datatype(o.i)=CHAR then nop
	else o.i=random(0,5,time('S'))
     y=random(0,20,time('S'))
     if y=10 then y=1
	else y=0
     if datatype(t.i)=CHAR then y=t.i
     t.i=y
end
call close('r')

signal gg



/* CyberMech Creation Kit V1.00 */
mech:
i=2
start:
m='DH2:BBS/Doors/cyber/cybermech/mech'||i
if exists(m) then nop
     else signal create
i=i+1
signal start

create:
if open('q',m,'W') then nop
say 'Enter name of this mech'
pull mname
say 'Maximum amount of cargo room'
pull xcargo
say 'maximum amount of armor'
pull xarmor
say 'Does this mech have action slots? (1=yes 0=no)'
pull slot
call writeln('q',xarmor)
call writeln('q',slot)
call writeln('q',mname)
call writeln('q',xcargo)
say 'Small, Medium or Large Frame?';pull ty
if ty='S' then call writeln('q',5)
if ty='M' then call writeln('q',20)
if ty='L' then call writeln('q',50)

cost=xarmor*2;cost=cost+xcargo;cost=cost+cost;if slot then cost=cost+500
call writeln('q',cost)

call close('q')


return

/* ff */




pirate:
say 'How many pirates to generate?'
pull gen
if gen>50 then say 'Thats a lot of pirates'

if open('r','DH2:BBS/Doors/cyber/cybermech/map','R') then nop
do i=1 to 625
     temp=readln('r')
     parse var temp m.i '-' o.i '-' t.i '-' p.i '-' s.i
     end
call close('r')




i=1
m='DH2:BBS/Doors/cyber/cybermech/'||i
do e=1 to gen

do while exists(m)
i=i+1
m='DH2:BBS/Doors/cyber/cybermech/'||i

end

create:
name=Pirate
handle=PIRATE
score=0;mech=4;weapon=3;armor=400;hp=50;money=0;mines=0;cargoamt=0
cargocash=0;tag=0;sw=0;lweapon=0;lshell=0;pod=0;slot=0
call rew


if open('ir',m,'W') then nop
     else exit
call writeln('ir',name)
call writeln('ir',handle)
call writeln('ir',score)
call writeln('ir',mech)
call writeln('ir',weapon)
call writeln('ir',armor)
call writeln('ir',hp)
call writeln('ir',money)
call writeln('ir',location)
call writeln('ir','10')
call writeln('ir',cargoamt)
call writeln('ir',cargocash)
call writeln('ir',tag)
call writeln('ir',sw)
call writeln('ir','3')
call writeln('ir','2432')
call writeln('ir',lweapon)
call writeln('ir','5')
call writeln('ir',pod)
call writeln('ir',slot)
call writeln('ir','1')
call writeln('ir','rough-black-1')
call writeln('ir','301')
call writeln('ir','400')
call writeln('ir','0')
call writeln('ir','0-0-0-0-0-0-0-0-0')
call writeln('ir','0-0-0-0')
call writeln('ir','999')
call close('ir')

m.location=i
end

say 'Do you wish to place the pirate base? (YN)'
pull loo
loo=UPPER(loo)
if loo='N' then signal gg
say 'Where?';pull location
m.location=999



gg:
if open('ir','DH2:BBS/Doors/cyber/cybermech/map','W') then nop
say '';say '';say 'Please wait -- Saving game files'

do i=1 to 625
     call writeln('ir',m.i'-'o.i'-'t.i'-'p.i'-'s.i)
     end

call close('ir')




return
rew:
l=1
do while l
numeric digits 3
location=randu(time('S'))*1000
if location<626 then if location >10 then l=0
end
return

flush:
say 'This will restart the game, including pirate removal, player removal,'
say 'restarting of the namelist,etc. this does not regen the map or pirates'
say 'Contunue?';pull yn;if upper(yn)='Y' then signal flush1
return
flush1:
open('nn','DH2:BBS/Doors/cyber/cybermech/namelist','R')
n=readln('nn')
do i=1 to n
nname.i=readln('nn')
address command 'delete "DH2:BBS/Doors/cyber/cybermech/'nname.i'"'

nhandle.i=readln('nn')
nscore.i=readln('nn')
end
call close('nn')
say 'deleteing game and player files'
address command 'delete DH2:BBS/Doors/cyber/cybermech/namelist'
address command 'delete DH2:BBS/Doors/cyber/cybermech/mess#?'
address command 'delete DH2:BBS/Doors/cyber/cybermech/fort#?'
address command 'delete DH2:BBS/Doors/cyber/cybermech/log'
address command 'delete DH2:BBS/Doors/cyber/cybermech/tac#?'
address command 'delete DH2:BBS/Doors/cyber/cybermech/map'
address command 'delete ram:map'
address command 'delete DH2:BBS/Doors/cyber/cybermech/spr#?'
address command 'delete DH2:BBS/Doors/cyber/cybermech/kitty'
address command 'delete DH2:BBS/Doors/cyber/cybermech/roster'
address command 'delete DH2:BBS/Doors/cyber/cybermech/team#?'
address command 'delete DH2:BBS/Doors/cyber/cybermech/plays'
say 'deleting pirates'
do yy=1 to 200
if exists('DH2:BBS/Doors/cyber/cybermech/'yy) then address command 'delete "DH2:BBS/Doors/cyber/cybermech/'yy'"'
end
say 'ALL DONE!'
t=0
return

mainc:

say 'City Computer files generation'
open('g','DH2:BBS/Doors/cyber/cybermech/main','W')
if open('r','DH2:BBS/Doors/cyber/cybermech/map','R') then say 'Reading map'
do i=1 to 625
     temp=readln('r')
     parse var temp m.i '-' o.i '-' t.i '-' p.i '-' s.i
     end
call close('r')
say 'Enter password for player name listing';pull st
call writeln('g',st)
call wmap
say 'Enter password for pirate city locator';pull st
call writeln('g',st)
call wmap
say 'Enter password for player lookup';pull st
call writeln('g',st)
call wmap
call writeln('g','MECH')
st='MECH'
call wmap
call writeln('g','HCEM')
st='HCEM'
call wmap
say 'Enter password for player locator';pull st
call writeln('g',st)
call wmap

say 'Enter password for telemetry matrix networking';pull st
call writeln('g',st)
call wmap
call close('g')
if open('ir','DH2:BBS/Doors/cyber/cybermech/map','W') then nop
say '';say '';say 'Please wait -- Saving game files'

do i=1 to 625
     call writeln('ir',m.i'-'o.i'-'t.i'-'p.i'-'s.i)
     end

call close('ir')
return

wmap:
numeric digits 3

r=1000*randu(time(S))
if r>10 then if r<600 then t.r='City Mainframe Code :'st
else signal wmap
return

part:
if open('r','DH2:BBS/Doors/cyber/cybermech/map','R') then say 'Reading map'
do i=1 to 625
     temp=readln('r')
     parse var temp m.i '-' o.i '-' t.i '-' p.i '-' s.i
     end
call close('r')
do y=1 to 4
say 'Put part 'y' in what sector?'
pull yy
o.yy=100+y
end
if open('ir','DH2:BBS/Doors/cyber/cybermech/map','W') then nop
say '';say '';say 'Please wait -- Saving game files'

do i=1 to 625
     call writeln('ir',m.i'-'o.i'-'t.i'-'p.i'-'s.i)
     end

call close('ir')
return

automaint:
say 'Reading Namelist file...'
if open('nn','DH2:BBS/Doors/cyber/cybermech/namelist','R') then nop
	else exit(0)
n=readln('nn')
if n=1 then do
	if readln('nn')='NNAME.1' then do
		say 'Severe Namelist error. Restarting file'
		address command 'delete DH2:BBS/Doors/cyber/cybermech/namelist'
		end
	say 'Auto-Maint Unecessary.'
	call close('nn')
	exit(0)
	end
do i=1 to n
nname.i=readln('nn')
nhandle.i=readln('nn')
nscore.i=readln('nn')
end
call close('nn')

say 'Done, 'n' Players Active'
if open('r','DH2:BBS/Doors/cyber/cybermech/map','R') then say 'Reading map'
do i=1 to 625
     temp=readln('r')
     parse var temp m.i '-' o.i '-' t.i '-' p.i '-' s.i
     end
call close('r')
i=0;redo=0
autone:
if redo=1 then i=i-1
redo=0
i=i+1
if i>n then signal autotwo
say 'Working on player #'i' out of 'n
 If Open('ir','DH2:BBS/Doors/cyber/cybermech/'nname.i,'R') Then nop
 else signal autone
 name=readln('ir');say 'Checking Player 'name
 handle=readln('ir');if handle='RESULT' then handle='Unknown'
 score=readln('ir');
 mech=readln('ir');
 weapon=readln('ir');if datatype(weapon)=CHAR then weapon=0
 armor=readln('ir');
 hp=readln('ir');if hp<0 then nscore.i='Deceased'
 money=readln('ir');
 location=readln('ir');
 mines=readln('ir');
 cargoamt=readln('ir');
 cargocash=readln('ir');
 tag=readln('ir');
 sw=readln('ir');
 turnz=readln('ir');
 dat=readln('ir');
 lweapon=readln('ir');
 lshell=readln('ir');if datatype(lshell)=CHAR then lshell=0
 pod=readln('ir');
 slot=readln('ir');
 body=readln('ir');if datatype(body)=CHAR then body=4
 bdesc=readln('ir')
 energy=readln('ir');if datatype(energy)=CHAR then energy=50
 xenergy=readln('ir')
 jjet=readln('ir')
 allslot=readln('ir')
part=readln('ir')
team=readln('ir')
call close('ir');
say 'Parsing map for ghosts...'
do u=1 to 625
if m.u=name then do
	if location=u then nop
		else m.u=0
	end
end

if open('ir','DH2:BBS/Doors/cyber/cybermech/'nname.i,'W') then nop
     else exit(0)
call writeln('ir',name)
call writeln('ir',handle)
call writeln('ir',score)
call writeln('ir',mech)
call writeln('ir',weapon)
call writeln('ir',armor)
call writeln('ir',hp)
call writeln('ir',money)
call writeln('ir',location)
call writeln('ir',mines)
call writeln('ir',cargoamt)
call writeln('ir',cargocash)
call writeln('ir',tag)
call writeln('ir',sw)
call writeln('ir',turnz)
call writeln('ir',dat)
call writeln('ir',lweapon)
call writeln('ir',lshell)
call writeln('ir',pod)
call writeln('ir',slot)
 call writeln('ir',body)
 call writeln('ir',bdesc)
 call writeln('ir',energy)
 call writeln('ir',xenergy)
 call writeln('ir',jjet)
call writeln('ir',allslot)
call writeln('ir',part)
call writeln('ir',team)
call close('ir')

g=dat+7;gg=date('D')
if g<gg then nscore.i='Inactive Player'
g=dat+15;gg=date('D')
	if g<gg then do
		m.location=0
		address command 'delete "DH2:BBS/Doors/cyber/cybermech/'nname.i'"'
		if i=n then do
			n=n-1
			end
		else if i<n then do
			say nname.i' Deleted. 'nname.n' Moved to slot 'i
			nname.i=nname.n
			nscore.i=nscore.n
			nhandle.i=nhandle.n
			n=n-1
			redo=1
			end
	end
signal autone
autotwo:

say 'Saving Namelist'
open('nn','DH2:BBS/Doors/cyber/cybermech/namelist','W')
call writeln('nn',n)
do i=1 to n
call writeln('nn',nname.i)
call writeln('nn',nhandle.i)
call writeln('nn',nscore.i)
end
call close('nn')

say 'Saving Map'
if open('ir','DH2:BBS/Doors/cyber/cybermech/map','W') then nop

do i=1 to 625
     call writeln('ir',m.i'-'o.i'-'t.i'-'p.i'-'s.i)
     end

call close('ir')

say 'Checking game plays for auto-regeneration'
if open('i','DH2:BBS/Doors/cyber/cybermech/plays','R') then nop
else exit(0)
n=readln('i')
call close('i')
if n>10 then call automet
if n>10 then do
	if open('y','DH2:BBS/Doors/cyber/cybermech/plays','W') then nop
	call writeln('y','0')
	call close('y')
	end


say 'Auto-Maint Completed.'
exit(0)
