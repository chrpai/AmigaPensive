/* Military Commander version 1.21 by DragonLord*/
options results
SS=Sendstring;TR=Transmit;GC=GetChar;Changewhere 'MC'
versionn='v1.21';sf=sendfile
numeric digits 14;SIGNAL ON BREAK_C;SIGNAL ON BREAK_E;SIGNAL ON ERROR;SIGNAL ON SYNTAX;SIGNAL ON IOERR
p1='[0m[31m';p2='[0m[32m';p4='[0m[35m';p6='[0m[36m';p7='[0m[37m';cls='[H[J'
call time('r');tr p4'Loading Military Commander...';maxn=99999999999999
getuser 7;xx=length(result)-1;timeleft=left(result,xx);left_secs=timeleft*60
if left_secs<60 then signal not_enough_time;Getuser 1;handle=result;handle_player=handle
if ~show('L','rexxsupport.library') then do;if addlib('rexxsupport.library',0,-30,0) then nop;else do;tr'Can''t find support library!';exit 10;end;end
BBSIDENTIFY name;bbsname=result;BBSIDENTIFY sysop;sysop=result;path='pfiles:military/';path1=path'users/';path2=path'text/'
if exists(path'WIN!') then do;tr'Somebody won! Try again in a few minutes.';signal bye;end

st111:
 call open(1,path2'game.data',R);xlx=readln(1);call close(1)
 parse var xlx tt_left','spy_max','npc_max','player_max','village_max','hof_score','secs','max_ter','s_funds','s_off','s_troops','s_mun','s_gren','s_food','xlx
 shag=npc_max;shat=player_max;shap=village_max
 if secs>left_secs then secs=left_secs;minutes=secs%60;
 do i=1 to max_ter
  parse var xlx ter.i','mod.i','xlx
 end i
 request_drop=0;date12=date();train_l=0;spy_times=0
 m.1.1=0;m.1.2=20;m.1.3='Food';m.1.4=10;m.2.1=0;m.2.2=17;m.2.3='Ammunitions';m.2.4=10;m.3.1=0;m.3.2=13;m.3.3='Troops';m.3.4=500;m.4.1=0;m.4.2=10;m.4.3='Grenades';m.4.4=250;m.5.1=10;m.5.2=20;m.5.3='Missile Launchers';m.5.4=3000;m.6.1=0;m.6.2=30;m.6.3='Missiles Class A';m.6.4=1000;m.7.1=10000;m.7.2=100000;m.7.3='Missiles Class B';m.7.4=750;m.8.3='Missiles Class C';m.8.4=500;m.9.3='Missiles Class D';m.9.4=250;m.10.3='Homing Missiles';m.10.4=1500;m.11.3='Fire Bomb Missiles';m.11.4=1250;m.12.3='Atomic Bombs';m.12.4=100000000;m.13.3='Tanks';m.13.4=1500000
 sell_on=0
 call check_use
 call forbid;f_flag=1;call check_use
  iam=0;do i=1 until iam=1
   iam=open(1,'ram:'handle'.MCO','W');if i=100000 then break
  end
  call close(1)
 call permit;f_flag=0
 tr;tr'You have 'minutes' minutes for this game session...';tr
 if ~exists(path1''handle) then call new_user
signal load_game

check_use:
 if exists('ram:'handle'.MCU') then do;tr'Your character is being attacked on another port. Try again in a few minutes.';signal bye;end
return

new_user:
 tr;tr'Ahh, a new user.. Let me setcha up...';tr
 tr'Your squadron will be named: 'handle'.';tr
 tr'You need to name your commander...'
 query'Commander of your Squadron: ';call gcc(0)
 if com1=0 then com1='Commander'
 commander=com1
 tr;tr'You have 's_troops' Troops at your disposal...';tr;tr'You have 's_off' Officers at your disposal...'
 left=s_off
 do i=1 to 11;off.i=0;end
 call officer_list
signal befort

officer_list:
 ss p7;sf path2'gen.list';tr
return

pick_off:
 if com1>11|com1<1 then return
 off.com1=off.com1+1;gotcha=1
return

befort:
 if left<1 then signal beforth
 pr_str='[1APick one. (Left='left'):           [10D';high_p1=11;call tmp0
 call pick_off
 if gotcha=1 then left=left-1
signal befort

beforth:
 funds=s_funds;tr;battles=0;tr p4'You have 's_funds'$ in funds...'
 lost=0;tr;won=0;troops=s_troops;tr p4'Now saving Platoon...';moral=100
 mun=s_mun;gren=s_gren;food=s_food;do i=0 to 6;m.i=0;end;tank=0;m.7=0;last_date=date()
 call save_stats
 call forbid;f_flag=1
  jj='W';if exists(path'users.dat') then jj='A'
  iam=0;do i=1 until iam=1
   iam=open(1,path'users.dat',jj);if i=100000 then break
  end
  call writeln(1,handle);call close(1)
 call permit;f_flag=0
 tr p4'Done!'
signal load_game

load_game:
 tr;tr;tr p2'			MILITARY COMMANDER'
 tr'			'versionn
 tr;tr p4'			Written by DragonLord';tr
 tr p1''center('If you enjoy this game, ask "'sysop'" to Support this Game!',78);tr
 tr p4'Loading your character...'
 handle1=handle
 call load_stats
 tr p4'Done!'
 call pcr;call update_gen;call check_log;call beg1
signal start_000

update_gen:
 call forbid;f_flag=1
  ss p4'Updating NPCs...'
  iam=0;do i=1 until iam=1
   iam=open(1,path2'generals.dat','R');;if i=100000 then break
  end;do i=1 to 10;xlx.i=readln(1);end;call close(1)
  do h=1 to 10
   ss'.';call p_p_p
   pc1=rnd(0,4);pc2=rnd(0,20);pc3=rnd(0,9);pc4=pc2'.'pc3
   do j=1 to 12
    pc5=0
    if tt.j<1 then tt.j=1
    if pc4~=0 then pc5=tt.j%pc4
    if pc4~=0 & pc5<1 then pc5=1
    if pc1=0 then pc5=pc5-pc5-pc5
    if pc5~=0 then tt.j=tt.j+pc5
    if tt.j<1 then tt.j=1
   end
   call p_p_o
  end
  iam=0;do i=1 until iam=1
   iam=open(1,path2'generals.dat','W');if i=100000 then break
  end;do i=1 to 10;call writeln(1,xlx.i);end;call close(1)
  tr
 call permit;f_flag=0
return

p_p_o:;xlx.h=g_name'|'tt.1'|'tt.2'|'tt.3'|'tt.4'|'tt.5'|'tt.6'|'tt.7'|'tt.8'|'tt.9'|'tt.10'|'tt.11'|'tt.12;return

p_p_p:;parse var xlx.h g_name'|'tt.1'|'tt.2'|'tt.3'|'tt.4'|'tt.5'|'tt.6'|'tt.7'|'tt.8'|'tt.9'|'tt.10'|'tt.11'|'tt.12;return

check_log:
 if ~exists(path1''handle'.log') then do;tr p4'Nothing new has happened to you since you last played.';return;end
 tr p1'Since the last time you played, this happened:';tr p4;sf path1''handle'.log'
 address command 'delete "'path1''handle'.log"'
 call pcr
return

beg1:
 tr;hof_sc=comadd(hof_score);tr p7'Minimum Hall of Fame Score Neccissary: 'hof_sc
 call start_001;tr;tr p2'--Hall of Fame--'p4
 if ~exists(path2'hof.dat') then tr p1'None!';else sf path2'hof.dat';tr
return

start_001:
 tr p1'Checking for high score...';tr
 file_name=handle;call figure_score
 parse var file_name score' 'value
 score_a=comadd(score);value_a=comadd(value)
 tr p4''left(handle,23)' - 'Left('Score: 'score_a,25)' - Value: 'value_a
 if score>=hof_score then do
  tr p1'YOU MADE IT TO HALL OF FAME!';tr'Congratulations!!'
  tr'Adding name to list, and resetting Military Commander...'
  jj='W';if exists(path2'hof.dat') then jj='A'
  tr'Kicking other players playing game out...'
  call forbid;f_flag=1
   call open(1,path'WIN!',w);call close(1)
  call permit;f_flag=0
  tr'Done!'
  call open(1,path2'hof.dat',jj);call writeln(1,left(handle,23)' - 'Left('Score: 'score_a,25)' - Value: 'value_a);call close(1)
  address command 'delete "'path'users.dat"';address command 'delete 'path1'#?'
  tr'Resetting NPC Generals...';address command 'delete "'path2'generals.dat"';address command 'copy "'path2'generals.original TO 'path2'generals.dat"';tr p1'Done! Please play again!'
  address command 'delete "'path'WIN!"'
  signal bye_bye2
 end
 tr
 if last_date~=date() then do;T_LEFT=tt_left;last_date=date();end
 if t_left<1 then signal no_turns
 if off.9>0 then do
  tr p4'Your Propaganda Officers got you 'off.9' new troops!'
  if off.3=0 then do;tr p4'But you have no-one to train them, so they leave.';end
  else do;troops_c=off.9;call train_troops;end
 end
return

check_won:
 if exists(path'WIN!') then do;tr'Somebody won the game!';signal bye_bye2;end
return

start_000:
do forever
 temp=secs-time('e');temp=temp/60;if temp>=10 then temp=left(temp,4);if temp<10 then temp=left(temp,3)
 tr p2'--Military Commander--'
 tr p4'Turns left: 'T_LEFT' -- Minutes Left: 'temp;tr p7;sf path2'main.menu'
 tr'Q) Quit back to 'bbsname
 tr;ss'Command> ';gc;call gcc(1)
 select
  when com1='Q' then signal bye_bye
  when com1='A' then call view_stats
  when com1='B' then do;sf path'users.dat';call pcr;end
  when com1='C' then call show_score
  when com1='D' then signal supply_request
  when com1='E' then signal mi_co
  when com1='F' then signal market
  when com1='G' then call spe_op
  when com1='H' then signal new_turn
  otherwise nop
 end
end

show_score:
 tr p7'Player Scores:';score_on=1;call save_stats
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(1,path'users.dat','R');if i=100000 then break
  end
  do i=1 until eof(1);i=i+1;xllx.i=readln(1);end;call close(1)
 call permit;f_flag=0;i=i-1
 do j=1 to i
  if xllx.j~=handle then do;if exists('ram:'xllx.j'.MCO')|exists('ram:'xllx.j'.MCU') then iterate j;end
  file_name=xllx.j;call figure_score
  if file_name~=xllx.j then do;parse var file_name score' 'value;score_a=comadd(score);value_a=comadd(value);tr p4''left(xllx.j,23)' - 'Left('Score: 'score_a,25)' - Value: 'value_a;end
 end
 tr p7'NPC Scores:'
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(1,path2'generals.dat','R');if i=100000 then break
  end
  do i=1 to 10;xlx.i=readln(1);end;call close(1)
 call permit;f_flag=0
 do i=1 to 11;off.i=0;end
 do h=1 to 10
  call p_p_p;troops=tt.1;funds=tt.2;moral=100;food=tt.4;mun=3;gren=tt.5;m.0=tt.6;m.1=tt.7;m.2=tt.8;m.3=tt.9;m.4=tt.10;m.5=tt.11;m.6=tt.12;tank=0;m.7=0;no_loaf=1;call figure_score
  parse var file_name score' 'value;score_a=comadd(score);value_a=comadd(value);tr p4''left(g_name,23)' - 'Left('Score: 'score_a,25)' - Value: 'value_a;no_loaf=0
 end
 handle1=handle;call load_stats;score_on=0;call pcr
return

spe_op:
 done1=0
 do until done1=1
  tr p2'--Special Operations--'
  tr p7'A) Send a Peace Treaty'
  tr'B) Send a Declaration of War'
  tr'C) Send a spy'
  tr'D) Update NPCs'
  tr'E) Reroll'
  tr'Q) Quit to main'
  ss'Command> ';gc;call gcc(1)
  select
   when com1='Q' then done1=1
   when com1='A' then do;query'Send a Peace Treaty to who: ';call gcc(0);treati=handle' has proposed a Peace Treaty with you.';call send_treaty1;tr p1'Treaty sent...';end
   when com1='B' then do;query'Send a Declaration of War to who: ';call gcc(0);treati=handle' has declared War on you!';signal send_treaty1;tr p1'Declaration sent...';end
   when com1='C' then do
    if off.5<1 then do;tr p1'You have no spies!';signal spe_op;end
    query'Spy on who: ';call gcc(0)
    if com1=handle then signal spe_op
    if ~exists(path1''com1)|com1='' then signal spe_op
    handle_x=com1
    if spy_times=spy_max then do;tr p1'You may only spy 'spy_max' times a turn.';signal spe_op;end
    spy_times=spy_times+1;caught=rnd(1,100);best=off.5*5
    if caught>best|caught=100 then do;tr p1'Your spy was caught!';treaty=3;treati=handle' attempted to spy on you!';signal send_treaty1;end
    tr p4'Your spy was successful.';call pcr;call save_stats;handle1=handle_x;call load_stats;handle_u=handle;handle=handle_x;call view_stats;handle=handle_u;handle1=handle;call load_stats
   end
   when com1='D' then call update_gen
   when com1='E' then do
    ss p1'Are you sure? [y/N]: ';gc;call gcc(1)
    if com1~='Y' then signal spe_op
    tr p1'Deleting army!!!';tr;tr p4'You may try again whenever you feel like it...';tr
    signal moral_check1
   end
   otherwise nop
  end
 end
return

send_treaty1:
 if com1=handle then return
 if ~exists(path1''com1)|com1='' then return
 jj='W';if exists(path1''com1'.log') then jj='A'
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(1,path1''com1'.log',jj);if i=100000 then break
  end
  call writeln(1,treati);call close(1)
 call permit;f_flag=0
return

supply_request:
 if off.1=0 then do;tr p4'You do not have any communications officers!';signal start_000;end
 if request_drop=1 then do;tr p4'You have already sent a request!';signal start_000;end
 tr p2'--Requests--'
 tr p7'A) Food'
 tr'B) Ammunition'
 tr'C) Troops'
 ss'Request which: ';gc;call gcc(1)
 if com1='A' then com1=1
 if com1='B' then com1=2
 if com1='C' then com1=3
 if com1<1|com1>3 then signal start_000
 request=com1;tr;tr p4'Your request has been sent...';request_drop=1
signal start_000

market:
 if sell_on=0 then tr p2'--Buy Menu--'p7
 if sell_on=1 then tr p2'--Sell Menu--'p7
 sf path2'item.menu'
 if sell_on=0 then tr'14) Sell menu'
 if sell_on=1 then tr'14) Buy menu'
 tr'  Q) Quit'
 funds_a=comadd(funds)
 tr p4'Funds: 'funds_a
 pr_str='Command> ';high_p1=14;call tmp0
 if com1=14 then signal sell_switch
 if com1<1|com1>13 then signal start_000
 call gen1
signal market

sell_switch:
 if sell_on=0 then do;sell_on=1;signal market;end
 if sell_on=1 then sell_on=0
signal market

figg1:
 m.1.5=food;m.2.5=mun;m.3.5=troops;m.4.5=gren;m.5.5=m.0;m.6.5=m.1;m.7.5=m.2;m.8.5=m.3;m.9.5=m.4;m.10.5=m.5;m.11.5=m.6;m.12.5=m.7;m.13.5=tank
return

figg2:
 food=m.1.5;mun=m.2.5;troops=m.3.5;gren=m.4.5;m.0=m.5.5;m.1=m.6.5;m.2=m.7.5;m.3=m.8.5;m.4=m.9.5;m.5=m.10.5;m.6=m.11.5;m.7=m.12.5;tank=m.13.5
return

gen1:
 call figg1
 if sell_on=0 then call generic1
 if sell_on=1 then call generic2
 call figg2
return

generic1:
 com0=com1;sb_a=comadd(m.com0.4);tr p4''m.com0.3' costs 'sb_a' each.'
 if funds<m.com0.4 then signal market
 maxx=funds%m.com0.4;maxx_a=comadd(maxx);query'Buy how many? (max='maxx_a') ';call gcc(0)
 if com1<1 then return
 if com1>maxx then com1=maxx
 funds=funds-(com1*m.com0.4);m.com0.5=m.com0.5+com1;tr'Bought!'
return

generic2:
 com0=com1;m.com0.99=m.com0.4%2;sc_a=comadd(m.com0.99);tr p4''m.com0.3' sell at 'sc_a' each.'
 if m.com0.5<1 then return
 sty_a=comadd(m.com0.5);query'Sell how many? (max='sty_a') ';call gcc(0)
 if com1<1 then return
 if com1>m.com0.5 then com1=m.com0.5
 funds=funds+(com1*m.com0.99);m.com0.5=m.com0.5-com1;tr'Sold!'
return

moral_check:
 moral=moral%1
 if moral<0 then do
  tr p1'Your platoon revolts, and scatters. You no longer have an army.'

moral_check1:
  address command 'delete "'path1''handle'"'
  call forbid;f_flag=1
   iam=0;do i=1 until iam=1
    iam=open(users,path'users.dat','R');if i=100000 then break
   end
   i=0
   do until eof(users);i=i+1;xlx.i=readln(users);end
   call close(users)
  call permit;f_flag=0
  top=i-1
  call forbid;f_flag=1
   iam=0;do i=1 until iam=1
    iam=open(users,path'users.dat','W');if i=100000 then break
   end
   do j=1 to top;if xlx.j~=handle then call writeln(users,xlx.j);end j
   call close(users)
  call permit;f_flag=0
  signal bye
 end
return

train_troops:
 train=off.3
 if train>troops_c then train=troops_c
 tr p4'You have trained 'train' of the troops.';train_l=train_l+(troops_c-train)
 if train_l>0 then tr p1'You have 'train_l' troops left to train, but they leave.'
 troops=troops+train
return

new_turn:
 tr p4'Waiting...'
 if T_LEFT=0 then signal no_turns
 spy_times=0;shag=npc_max;shat=player_max;shap=village_max
 T_LEFT=T_LEFT-1;tr p4'Feeding platoon...';feed=troops%5
 if feed>food then do;tr p1'Out of food! Your men are starving!';food=0;moral=moral-24;call moral_check;end
 food=food-feed;funds_c=off.4*100;tr p4'You got 'funds_c' new funds!';funds=funds+funds_c
 if train_l>0 & off.3>0 then do;call train_troops;end
 tr p4'New turn...'
 if request_drop=1 then do
  tr'Your request came...'
  if request=1 then do;food_c=off.1*25;tr'You got an additional 'food_c' food.';food=food+food_c;	end
  if request=2 then do;munition_c=off.1*100;tr'You got an additional 'munition_c' Ammunition.';mun=mun+munition_c;end
  if request=3 then do
   troops_c=off.1*2
   tr'You got an additional 'troops_c' Troops.'
   if off.3=0 then do;tr p1'But, you have no ability to train them, so they leave.';troops_c=0;end
   if off.3>0 then do;troops_c=off.3;call train_troops;end
  end
 request_drop=0
 end
signal start_000

no_turns:
 tr p1'Out of turns...'
signal bye_bye

mi_co:
 tr p2'--Military Commands--'
 if troops=0 then tr p1'You have no army!!'
 tr p7'A) Go to war with another player'
 tr'B) Attack an NPC'
 tr'C) Attack local people for supplies'
 tr'D) Hire another officer'
 tr'Q) Quit back to main menu'
 tr
 ss'Command> ';gc;call gcc(1)
 select
  when com1='A' then signal attack_player
  when com1='B' then signal npc_atk
  when com1='C' then signal attack_local
  when com1='D' then signal hire_off
  otherwise nop
 end
signal start_000

npc_atk:
 if troops=0 then signal mi_co
 if shag=0 then do;tr p1'Out of NPC attacks for this turn.';call mi_co;end
 pr_str='Which NPC? (1-10): ';high_p1=10;call tmp0
 if com1<1|com1>10 then signal mi_co
 spunk=com1;call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(gen,path2'generals.dat','R');if i=100000 then break
  end
  do h=1 to com1;xlx.h=readln(gen);end
  call close(gen)
 call permit;f_flag=0
 h=com1;call p_p_p;b_troops=tt.1*(3/4);b_troops=b_troops%1
 if b_troops<1 then do;tr p1''g_name' does not have a military!';signal mi_co;end
 t_troops=b_troops;leader=g_name;g.1=tt.4%4;g.2=tt.3%4;g.8=tt.2%4;g.9=tt.5%4;g.3=tt.6%4;g.4=tt.7%4;g.5=tt.8%4;g.6=tt.9%4;g.7=tt.10%4;g.10=tt.11%4;g.11=tt.12%4;e_high1=rnd(3,10);e_high2=rnd(0,10);y_mod=off.2;y_mod=y_mod+off.8;e_mod=y_mod;type=3;shag=shag-1
signal battle

hire_off:
 if funds<10000 then do;tr p1'You need 10,000 funds first!';signal mi_co;end
 call officer_list
 pr_str='Pick one: ';high_p1=11;call tmp0
 if com1>0 & com1<12 then do
  maxx=funds%10000;com2=com1;maxxa=comadd(maxx)
  query'How many? (max='maxxa') ';call gcc(0)
  if com1>maxx then com1=maxx
  if com1<1 then signal mi_co
  tr p4'Bought!';funds=funds-(com1*10000);off.com2=off.com2+com1
 end
signal mi_co

attack_player:
 if shat=0 then do;tr p1'Out of player attacks for this turn.';call mi_co;end
 if troops=0 then signal mi_co
 query'Attack who: ';call gcc(0)
 if ~exists(path1''com1)|com1='' then signal mi_co
 if com1=handle then signal mi_co
 if exists('ram:'com1'.MCO') then do;tr p1'That player is currently playing Military Commander. Try again later.';signal mi_co;end
 if exists('ram:'com1'.MCU') then do;tr p1'That player is currently being attacked. Try again later.';signal mi_co;end
 call save_stats
 score_on=1;handle1=com1
 call load_stats
 type=1
 if troops=0 then do;tr p1''com1' does not have a military!';handle1=handle;call load_stats;signal mi_co;end
 b_troops=troops*(3/4);b_troops=b_troops%1;t_troops=b_troops;leader=commander;g.1=food%4;g.2=mun%4;g.8=funds%4;g.9=gren%4;g.3=m.0%4;g.4=m.1%4;g.5=m.2%4;g.6=m.3%4;g.7=m.4%4;g.10=m.5%4;g.11=m.6%4;e_mod=off.2;e_mod=e_mod+off.8;e_high1=moral%3;e_high1=e_high1+off.6;e_high2=moral%3;e_high2=e_high2+off.10;handle1=handle
 call load_stats
 if b_troops=0 then do;tr p1''com1' does not have a military!';signal mi_co;end
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(ramm,'ram:'com1'.MCU','W');if i=100000 then break
  end
  call close(ramm)
 call permit;f_flag=0
 handle_x=com1;y_mod=off.2;y_mod=y_mod+off.8;score_on=0;shat=shat-1
call battle

attack_local:
 if shap=0 then do;tr p1'Out of Local Village attacks for this turn.';call mi_co;end
 if troops=0 then signal mi_co
 loww0=troops%10;higgh0=troops%5;loww1=loww0*2;higgh1=higgh0*2;loww2=loww0%5;higgh2=higgh0%5;tr p4'Your military advances against a village...';type=2;leader='a Commander';e_high1=rnd(2,6);e_high2=rnd(0,3);b_troops=rnd(loww0,higgh0);y_mod=off.2;y_mod=y_mod+off.8;e_mod=y_mod
 if b_troops<1 then b_troops=1;shap=shap-1
call battle

battle:
 q=rnd(1,15);trn=ter.q;mod_ter=mod.q;high1=moral%3;high1=high1+off.6;high2=moral%3;high2=high2+off.10;e_high1=e_high1-high2
 if e_high1<1 then e_high1=1
 high1=high1-e_high2
 if high1>10 then high1=10
 if high1<1 then high1=1
 if high2>10 then high2=10
 if high2<1 then high2=1
 if e_high1<1 then e_high1=1
 if e_high1>10 then e_high1=10
 if e_high2<1 then e_high2=1
 if e_high2>10 then e_high2=10
 t_mod=y_mod-e_mod;tr'You walk across the 'trn'.';ter_mod=off.2+off.8+t_mod;modd=mod_ter+ter_mod;battles=battles+1;modda=comadd(modd);tr p1'Your modifier is: 'modda;tr;b_troopsa=comadd(b_troops)
 tr p4'You see 'b_troopsa' troops, led by 'leader'!'

battle1:
 ss'Quick mode? (Y/n) ';gc;call gcc(1);q_mode=1
 if com1='N' then q_mode=0
 if modd=0 then signal battle2
 if modd>0 then signal battle3
 if modd<0 then signal battle4

battle2:
 tr p2'--Battle--'
 b_troopsa=comadd(b_troops)
 tr p4'Enemy Troops: 'b_troopsa
 troopsa=comadd(troops)
 tr' Your Troops: 'troopsa
 tr p7'A) Normal attack'
 tr'B) Use missile'
 tr'C) Use grenade'
 tr'D) Use tank'
 tr'E) Flee'
 tr;ss'Command> ';gc;call gcc(1)
 ss p4''cls
 if com1=0 then signal battle2
 if com1='Q'|com1='E' then signal flee
 if com1='D' then do
  if tank<1 then do;tr p1'No tank!';signal battle2;end
  if mun<100 then do;tr p1'Not enough ammo!';signal battle2;end
  tr'Your troop fires the tank at the enemy!'
  mun=mun-100;hit=rnd(0,100)
  if hit>b_troops then hit=b_troops
  tr'He killed 'hit' enemies!'
  b_troops=b_troops-hit
  breakk=rnd(1,100)
  if breakk>30+off.11|breakk=100 then do;tr p1'Your tank broke!';tank=tank-1;end
  if b_troops=0 then signal victory
  signal battle2x
 end
 if com1='A' then do
  if mun<100 then do;tr p1'Not enough ammo!';signal battle2;end
  com1='N'
  if b_troops>99 & mun>99 & troops>99 then do;ss'Do some quick rounds? (Y/n) ';gc;call gcc(1);end
  if com1='N' then do;tr'Your troop fires at the enemy!';mun=mun-10;hit=rnd(0,high1);if hit>b_troops then hit=b_troops;tr'He killed 'hit' enemies!';b_troops=b_troops-hit;if b_troops=0 then signal victory;signal battle2x;end
  if b_troops<=mun%10 & b_troops<=troops then liga=b_troops%10
  if mun%10<=b_troops & mun%10<=troops then liga=mun%100
  if troops<=b_troops & troops<=mun then liga=troops%10
  tr'Your troops fire at the enemies!';tr'The enemies fire at your troops!'
  mun=mun-(liga*10);hit=rnd(1,high1);hit=hit*liga
  if hit>b_troops then hit=b_troops
  hita=comadd(hit);tr'Your troops killed 'hita' enemies!'
  b_troops=b_troops-hit;hit=rnd(1,e_high1);hit=hit*liga
  if hit>troops then hit=troops
  troops=troops-hit;hita=comadd(hit);tr'The enemy killed 'hita' of your troops!'
  if b_troops=0 then signal victory
  if troops=0 then signal defeat
  signal battle2
 end
if com1='C' then do
	if gren<1 then do;tr p1'No grenades!';signal battle2;end
	tr'Your troop tosses a grenade at the enemy!';gren=gren-1;hit=rnd(0,6)
	if hit>b_troops then hit=b_troops
	tr'He killed 'hit' enemies!';b_troops=b_troops-hit
	if b_troops=0 then signal victory
	signal battle2x
end
if com1='B' then do
	if m.0<1 then do;tr p1'No missile launcher!';signal battle2;end
	tr p7'1) Class A'
	tr'2) Class B'
	tr'3) Class C'
	tr'4) Class D'
	tr'5) Homing'
	tr'6) Fire Bomb'
	tr'7) Atomic Bomb'
	ss'Type> ';gc;call gcc(1);ss p4
	if com1<1|com1>7 then signal battle2
	signal o1
end
signal battle2

o1:
 if m.com1<1 then do;tr p1'No missiles!';signal battle2;end
 m.com1=m.com1-1;c1=m.com1.1;c2=m.com1.2
signal missile_hitter

battle2x:
 tr p4'An enemy troop fires at you!';hit=rnd(0,e_high1)
 if hit>troops then hit=troops
 troops=troops-hit;hita=comadd(hit);tr'He killed 'hita' of your troops!'
 if troops=0 then signal defeat
signal battle2

missile_hitter:
 hit=rnd(c1,c2);tr'Your troop fires at the enemy!'
 if hit>b_troops then hit=b_troops
 tr'He killed 'hit' enemies!';b_troops=b_troops-hit
 call break_checker
 if b_troops=0 then signal victory
signal battle2x

break_checker:
 breakk=rnd(1,100)
 if breakk>50+off.11|breakk=100 then do;tr p1'Your missile launcher broke!';m.0=m.0-1;end
return

battle4:
 modd=abs(modd)
 if b_troops<modd then modd=b_troops
 if q_mode=1 then do;tr'The enemies fire at your troops!';hit=rnd(0,e_high1*modd);if hit>troops then hit=troops;troops=troops-hit;hita=comadd(hit);tr'The enemy killed 'hita' of your troops!';if troops=0 then signal defeat;signal battle2;end
 do i=1 to modd;tr p4'An enemy troop fires at you!';hit=rnd(0,e_high1);if hit>troops then hit=troops;troops=troops-hit;tr'He killed 'hit' of your troops!';if troops=0 then leave i;end
 if troops=0 then signal defeat
signal battle2

battle3:
 ss'Use your first attack ability? (Y/n) ';gc;call gcc(1)
 if com1='N' then signal battle2
 if troops<modd then modd=troops
 if q_mode=1 then do;tr'Your troops fire at the enemies!';mun=mun-(modd*10);hit=rnd(0,high1*modd);if hit>b_troops then hit=b_troops;hita=comadd(hit);tr'Your troops killed 'hita' enemies!';b_troops=b_troops-hit;if b_troops=0 then signal victory;signal battle2;end
 do i=1 to modd;if mun>9 then do;tr p4'Your troop fires into the enemy!';mun=mun-10;hit=rnd(0,high1);if hit>b_troops then hit=b_troops;tr'He killed 'hit' enemies!  ';b_troops=b_troops-hit;if b_troops=0 then leave i;end;if mun<10 then leave i;end
 if b_troops=0 then signal victory
 if mun<10 then signal flee_am
signal battle2

save_log:
 call save_stats
 score_on=1;handle1=handle_x
 call load_stats
 if win=1 then do;food=food-g.1;mun=mun-g.2;funds=funds-g.8;gren=gren-g.9;m.0=m.0-g.3;m.1=m.1-g.4;m.2=m.2-g.5;m.3=m.3-g.6;m.4=m.4-g.7;m.5=m.5-g.10;m.6=m.6-g.11;end
 troops=troops-(t_troops-b_troops);player1=handle;handle=handle1
 call save_stats
 handle=player1;handle1=handle
 call load_stats
 if exists(path1''handle_x'.log') then how1='A';else how1='W'
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(logg,path1''handle_x'.log',how1);if i=100000 then break
  end
  call writeln(logg,le)
  call close(logg)
 call permit;f_flag=0
 if exists('ram:'handle_x'.MCU') then address command 'delete "ram:'handle_x'.MCU"'
 score_on=0
return

save_log1:
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(gen,path2'generals.dat','R');if i=100000 then break
  end
  do i=1 to 10;xlx.i=readln(gen);end
  call close(gen)
  iam=0;do i=1 until iam=1
   iam=open(gen,path2'generals.dat','W');if i=100000 then break
  end
  do h=1 to 10
   if h=spunk then do
    call p_p_p
    tt.1=tt.1-(t_troops-b_troops)
    if win=1 then do;tt.2=tt.2-g.8;tt.3=tt.3-g.2;tt.4=tt.4-g.1;tt.5=tt.5-g.9;tt.6=tt.6-g.3;tt.7=tt.7-g.4;tt.8=tt.8-g.5;tt.9=tt.9-g.6;tt.10=tt.10-g.7;tt.11=tt.11-g.10;tt.12=tt.12-g.11;end
    call p_p_o
   end
   call writeln(gen,xlx.h)
  end
  call close(gen)
 call permit;f_flag=0
return

flee:
 win=0
 new_m=8
 if off.7>0 then do;do i=1 to off.7;new_m=new_m/2;end;end
 moral=moral-new_m
 call moral_check
 tr p1'Your army flees!'
 le=handle' attacked you, but fled, on 'date()'.'
 if type=1 then call save_log
 if type=3 then call save_log1
signal mi_co

defeat:
 win=0
 new_m=32;if off.7>0 then do;do i=1 to off.7;new_m=new_m/2;end;end
 moral=moral-new_m
 call moral_check
 tr p1'Your platoon has been annialated...'
 lost=lost+1;le=handle' attacked you, but you won, on 'date()'.'
 if type=1 then call save_log
 if type=3 then call save_log1
signal bye_bye

victory:
 win=1
 if type=3 then do
	tr p4'You have defeated 'g_name'!'
	call save_log1
 end
 if type=2 then do
	tr p4'You have defeated all of your foes!'
	g.1=rnd(loww0,higgh0)
	g.2=rnd(loww1,higgh1)
	do i=3 to 7;g.i=rnd(loww2,higgh2);end
	g.8=rnd(loww0,higgh0)
	do i=9 to 11;g.i=rnd(loww2,higgh2);end
 end
 if type=1 then do
	tr p4'You have defeated 'handle_x'!'
	le=handle' attacked you, and won, on 'date()'.'
	call save_log
 end
 tr'You found:'
 do i=1 to 11;g.i.1=comadd(g.i);end
 tr''g.1.1' food, 'g.2.1' ammunition, 'g.8.1' funds, 'g.9.1' grenades, 'g.3.1' missile launchers, 'g.4.1' missiles class a, 'g.5.1' missiles class b, 'g.6.1' missiles class c, 'g.7.1' missiles class d, 'g.10.1' homing missiles, and 'g.11.1' fire bomb missiles!'
 tr
 gren=gren+g.9;food=food+g.1;mun=mun+g.2;funds=funds+g.8;won=won+1;moral=moral+4;m.0=m.0+g.3;m.1=m.1+g.4;m.2=m.2+g.5;m.3=m.3+g.6;m.4=m.4+g.7;m.5=m.5+g.10;m.6=m.6+g.11
 if moral>100 then moral=100
 call save_stats
signal mi_co

view_stats:
 tr p2'                  --Stats--'
 tr p4'            Platoon Name: 'handle
 tr'                Commander: 'commander
 troops_a=comadd(troops)
 tr'                   Troops: 'troops_a
 do i=1 to 11;off.i.1=comadd(off.i);end
 tr'  Communications Officers: 'off.1.1
 tr'    Intelligence Officers: 'off.2.1
 tr'        Training Officers: 'off.3.1
 tr'         Finance Officers: 'off.4.1
 tr'Enemy Operation Officers: 'off.5.1
 tr'        Tactical Officers: 'off.6.1
 tr'           Moral Officers: 'off.7.1
 tr'          Region Officers: 'off.8.1
 tr'      Propaganda Officers: 'off.9.1
 tr'         Defense Officers: 'off.10.1
 tr'       Mechanics Officers: 'off.11.1
 funds_a=comadd(funds)
 tr'                    Funds: 'funds_a
 battles_a=comadd(battles);won_a=comadd(won);lost_a=comadd(lost)
 tr'           Battles Fought: 'battles_a' -- Won/Lost: 'won_a'/'lost_a
 win_percent=100
 if battles>0 then win_percent=(won/battles)*100
 win_percent=win_percent%1
 tr'              Win Percent: 'win_percent'%'
 tr'              Troop moral: 'moral'%'
 food_a=comadd(food);foodaa=troops%5;foodaa=comadd(foodaa)
 tr'                     Food: 'food_a' -- Needed daily: 'foodaa
 mun_a=comadd(mun);trops=troops*10;tropsa=comadd(trops)
 tr'               Ammunition: 'mun_a' -- Needed / Round: 'tropsa
 grena=comadd(gren)
 tr'                 Grenades: 'grena
 do i=0 to 7;m.i.1=comadd(m.i);end
 tr'        Missile Launchers: 'm.0.1
 tr'         Missiles Class A: 'm.1.1
 tr'         Missiles Class B: 'm.2.1
 tr'         Missiles Class C: 'm.3.1
 tr'         Missiles Class D: 'm.4.1
 tr'          Homing Missiles: 'm.5.1
 tr'       Fire Bomb Missiles: 'm.6.1
 tr'             Atomic Bombs: 'm.7.1
 tr'                    Tanks: 'tank
 tr
 call pcr
return

load_stats:
 call open(1,path1''handle1,'R');do i=1 to 6;xlx.i=readln(1);end i;call close(1)
 parse var xlx.1 commander'|'troops
 parse var xlx.2 off.1'|'off.2'|'off.3'|'off.4'|'off.5'|'off.6'|'off.7'|'off.8'|'off.9'|'off.10'|'off.11
 parse var xlx.3 funds'|'battles'|'lost'|'won'|'moral
 parse var xlx.4 food'|'mun'|'gren'|'m.0'|'tank
 parse var xlx.5 m.1'|'m.2'|'m.3'|'m.4'|'m.5'|'m.6'|'m.7
 parse var xlx.6 last_date'|'t_left
 if last_date=date12 then T_LEFT=tt_left
return

save_stats:
 call forbid;f_flag=1
  call open(1,path1''handle,'W')
  call writeln(1,commander'|'troops)
  call writeln(1,off.1'|'off.2'|'off.3'|'off.4'|'off.5'|'off.6'|'off.7'|'off.8'|'off.9'|'off.10'|'off.11)
  call writeln(1,funds'|'battles'|'lost'|'won'|'moral)
  call writeln(1,food'|'mun'|'gren'|'m.0'|'tank)
  call writeln(1,m.1'|'m.2'|'m.3'|'m.4'|'m.5'|'m.6'|'m.7)
  call writeln(1,last_date'|'t_left);call close(1)
 call permit;f_flag=0
return

tmp0:
 abba=0;xyz.2=-999;xyz.3=-999;ss p7''pr_str
signal tmp1

tmp1:
 abba=abba+1;gc;call gcc(0)
 if c2x(com1)~='0D' then do
  if c2x(com1)='08' & abba<2 then do;abba=abba-1;signal tmp1;end
  if c2x(com1)='08' & abba~<2 then do;abba=abba-2;ss com1' 'com1;signal tmp1;end
  xyz.abba=com1
 end
 if c2x(com1)='0D' then do;abba=abba-1;signal tmp2;end
 ss com1
 if xyz.1=0|xyz.1='Q' then signal tmp2
 if high_p1<10 & xyz.1>0 & xyz.1<=high_p1 then signal tmp2
 if high_p1>9 then do
  x1=length(high_p1);x2=left(high_p1,1);x4=right(high_p1,1)
  if x2=9 then x3=x1;else x3=x1-1
  if x3=1 then do;if xyz.1>x2|xyz.1<x2 & xyz.2>=0 & xyz.2<10|xyz.1=x2 & xyz.2>=0 & xyz.2<=x4 then signal tmp2;end
 end
signal tmp1

tmp2:
 com1='';do i=1 to abba;com1=com1''xyz.i;end;tr
return

gcc:
 arg typ1
 com1=compress(result,',');GETCARRIER;if result="FALSE" then signal bye_bye;if time('e')>secs then signal out_time
 call check_won
 if typ1=2 then do;if datatype(com1)~='NUM' then com1=0;com1=com1%1;end
 if typ1=1 then tr p7''com1
RETURN

rnd: procedure
 arg lo,hi
 r=hi;if lo=hi|lo>hi then return r;d=hi-lo+1;p=d;x=length(d);n1=time('E');n1=compress(n1,'.');n1=right(n1,x);n2=time('S');n2=right(n2,x);if x>3 then p=999;r=n1+n2+random(0,p);r=r//d+lo
return r

pcr:
 ss p7'Press any key to continue!';gc;call gcc(0);tr;ss'[1A[K'
return

figure_score:
 if no_loaf~=1 then do
	if ~exists(path1''file_name) then return
	handle1=file_name
	call load_stats
 end
 officers=0
 do i=1 to 11;officers=officers+off.i;end
 pnt1=troops*1000;pnt1=pnt1+(officers*1000);pnt1=pnt1+(funds%100);pnt1=pnt1+(moral%10);pnt1=pnt1+(food%10);pnt1=pnt1+(mun%10);pnt1=pnt1+(gren*100);pnt1=pnt1+(m.0*3000);pnt1=pnt1+(m.1*1000);pnt1=pnt1+(m.2*500);pnt1=pnt1+(m.3*250);pnt1=pnt1+(m.4*100);pnt1=pnt1+(m.5*1500);pnt1=pnt1+(m.6*1250);pnt1=pnt1+(tank*1500000);pnt1=pnt1+(m.7*100000000)
 pnt2=troops*1000;pnt2=pnt2+(officers*10000);pnt2=pnt2+(funds%10);pnt2=pnt2+(food%1);pnt2=pnt2+(mun%10);pnt2=pnt2+(gren*100);pnt2=pnt2+(m.0*3000);pnt2=pnt2+(m.1*1000);pnt2=pnt2+(m.2*500);pnt2=pnt2+(m.3*250);pnt2=pnt2+(m.4*100);pnt2=pnt2+(m.5*1500);pnt2=pnt2+(m.6*1250);pnt2=pnt2+(tank*1500000);pnt2=pnt2+(m.7*100000000)
 if pnt1>maxn then pnt1=maxn;if pnt2>maxn then pnt2=maxn
 file_name=pnt1' 'pnt2
return

COMADD: procedure
 arg x
 if datatype(x)~='NUM' then return x;x=x%1
 if x>maxn then return x
 y=length(x);if y<4 then return x
 z=y%3;if z*3=y then z=z-1;if z*3>y then z=z-1;j=y-(z*3)
 do intt=1 to z;ii=y-(3*intt);x=insert(',',x,ii);end intt
return x

ERROR:;SIGL1=SIGL;call enderror

SYNTAX:;SIGL1=SIGL;call enderror

IOERR:;SIGL1=SIGL;call enderror

ENDERROR:
 if error1=1 then exit
 error1=1;tetp='';if sigl1=0 then tetp='-!c1='c1' & c2='c2'!-'
 le='Error Code: 'rc'* 'errortext(rc)', on line 'SIGL1'. User: 'handle', on 'date()', at 'time()'. 'versionn''tetp
 tr p1''le
 jj='W';if exists(path'ERROR.LOG') then jj='A';call open(1,path'ERROR.LOG',jj);call writeln(1,le);call close(1)
 tr'Report what you were doing last when the bug occured to DragonLord, or'
 tr'your SysOp.'
 tr
 tr'My Internet Email address is:'
 tr
 tr'     dragonlord@omahalink.net'
 tr
 tr'I currently am not hosting any support BBSes. Sorry.'
 tr
call bye_bye

not_enough_time:
 tr p1'You do not have enough online time to play!'
signal bye

time_up:
 tr p1'Time''s up!'

bye_bye:
 if score_on=1 then signal bye_bye2
 tr p4'Saving Platoon...'
 if t_left>0 then t_left=t_left-1
 if handle_player~=handle then signal bye_bye2
 call save_stats
 tr'Done!'

bye_bye2:
 if exists('ram:'handle_x'.MCU') then address command 'delete "ram:'handle_x'.MCU"'
 if exists('ram:'handle'.MCU') then address command 'delete "ram:'handle'.MCU"'
 if exists('ram:'handle_player'.MCO') then address command 'delete "ram:'handle_player'.MCO"'

bye:
 if f_flag=1 then call permit
 tr p1'This game is free, so, please donate to me!';bufferflush
exit
