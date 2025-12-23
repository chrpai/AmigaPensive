/**************************************************************************\
    $VER: Dragon's Lord, v1.00i1 Beta (17-Dec-97) by DragonLord!
\**************************************************************************/
options results;call time(r)
SF=SendFile;SS=Sendstring;TR=Transmit;GC=GetChar;Changewhere 'DL';DL_acct=1
SIGNAL ON ERROR;SIGNAL ON SYNTAX;SIGNAL ON IOERR
a=sourceline(2);parse var a . ", "ver" ("vdate")" .;getuser 23;port2=result
numeric digits 14;def='[0m';cls='[H[J';p1='[0m[31m';p2='[0m[32m';p3='[0m[33m';p4='[0m[34m';p5='[0m[35m';p6='[0m[36m';p7='[0m[37m';p8='[30m[1m';p9='[31m[1m';pa='[32m[1m';pb='[33m[1m';pc='[34m[1m';pd='[35m[1m';pe='[36m[1 m';pf='[37m[1m';ss cls'z0'p7
tr'You feel your soul being sucked from your body... Suddenly you appear';tr'in some unknown local...';tr;tr;tr'Welcome to "Dragon''s Lord" 'ver', by DragonLord'
a="rexxsupport.library";if ~show("l",a) then if ~addlib(a,0,-30) then exit
parse source . . fp .;df=left(fp,max(lastpos('/',fp),lastpos(':',fp)))
getuser 40;idnum=result;getuser 1;handle=result;bbsidentify name;bbsname=result;getuser 23;portid=result
getuser 7;xx=length(result)-1;timeleft=left(result,xx);secs=3600;left_secs=timeleft*60;if left_secs<60 then do;tr'Not enough time to play "Dragon''s Lord".';signal goodbye;end;if secs>left_secs then secs=left_secs

call load_t;call load_r;call load_c;call load_i;call logplayer(1)
if exists(path9''idnum) then call load_user

main:
 do forever
  if loc=1 then call Genadil
  if loc=2 then do;inout=1;call forest;end
  if loc=3 then call cave
  if loc=0 then break
 end
signal goodbye

l_map:
 parse arg mnam
 call open(1,path10''mnam,r);do i=1 until eof(1);map1.i=readln(1);end i;map_t=i-1;call close(1)
 call open(1,path10''mnam'.s',r);stair=readln(1);call close(1)
 parse var stair st_num','stair;if st_num>0 then do;do i=1 to st_num;parse var stair stair_.i','stair;end i;end
return

Genadil1:
 you1='à';coo_.1=8;coo_.2=24;call l_map('City.1')
 do forever
  if o_coo.1~=coo_.1|o_coo.2~=coo_.2 then call p_map1
  ss'[22;1HCommand >> ';gc;call gcc(1);h=coo_.1;j=coo_.1-1;k=coo_.1+1
  select
   when com1='8' & right(left(map1.j,coo_.2),1)=' ' then coo_.1=coo_.1-1
   when com1='4' & right(left(map1.h,coo_.2-1),1)=' ' then coo_.2=coo_.2-1
   when com1='6' & right(left(map1.h,coo_.2+1),1)=' ' then coo_.2=coo_.2+1
   when com1='2' & right(left(map1.k,coo_.2),1)=' ' then coo_.1=coo_.1+1
   otherwise nop
  end
  if com1='Q' then break
 end
 loc=0
return

p_map1:
 ss'[1;1H'
 do i=1 to 20
  disp=map1.i
  if i=coo_.1 then disp=overlay(you1,disp,coo_.2,1)
  if st_num>0 then do
   do ii=1 to st_num
    parse var stair_.ii x1';'y1';'dir1
    if x1=i then disp=overlay('#',disp,y1,1)
   end ii
  end
  tr disp
 end i
 o_coo.1=coo_.1;o_coo.2=coo_.2
return

forest:
 do forever
  if inout=1 then tr cls'You are standing in the outskirts of the forest.'
  if inout=2 then tr cls'You are standing in the middle of the forest.'
  tr
  tr'L)ook for something to kill'
  if inout=1 then tr'T)ravel Deeper into Forest'
  if inout=2 then tr'T)ravel Out of the Dense Forest'
  tr'V)iew stats'
  if study_hours~='STUDY_HOURS' then tr'C)ast a Spell'
  if inout=1 then tr'R)eturn back to town'
  tr;tr'HP: 'ud.7' / 'ud.8', MP: 'ud.9' / 'ud.10;tr
  ss'Command >> ';gc;call gcc(1)
  select
   when com1='L' then call find_fight_f(inout)
   when com1='T' & inout=1 then do;inout=2;tr'Moving deeper into the forest...';call pcr;end
   when com1='T' & inout=2 then do;inout=1;tr'Moving out of the dense forest...';call pcr;end
   when com1='V' then call view_stats
   when com1='C' & study_hours~='STUDY_HOURS' then call cast_spell(2)
   when com1='R' & inout=1 then do;loc=1;return;end
   otherwise nop
  end
 end
return

find_fight_f:
 parse arg inout
 if inout=1 then do;flvl=1;himon=8;end;if inout=2 then do;flvl=3;himon=14;end
 mty=rnd(1,flvl);local=1
 do ii=1 to mty
  /* 1-8 is outskirt monsters, 1-14 is deep forest */
  monst_.ii=rnd(1,himon);whomi=monst_.ii
  if mon.whomi.1='MON.'whomi'.1' then call load_monster(whomi)
 end ii
 call battleprocedure
return

cave:
 ss cls;loc_x1=1;loc_y1=1;loc_f1=1;dir1=2;loc=1
 do forever
  if loc_x1=0 then break
  if room_.loc_f1.loc_x1.loc_y1.1='ROOM_.'loc_f1'.'loc_x1'.'loc_y1'.1' then call load_room(loc_f1'.'loc_x1'.'loc_y1)
  if olocx~=loc_x1|olocy~=loc_y1|olocf~=loc_f1 then call display_room
  tr'8 = Forward, 4 = Left, 6 = Right, V)iew Stats, Q)uit';tr
  ss'Command >> ';gc;call gcc(1)
  select
   when com1=8 & room_.loc_f1.loc_x1.loc_y1.dir1~=0 then do
    parse var room_.loc_f1.loc_x1.loc_y1.dir1 flr1'.'xm'.'ym;loc_f1=flr1;loc_x1=xm;loc_y1=ym;call offse
   end
   when com1=4 then do;dir1=dir1-1;if dir1<2 then dir1=5;call offse;end
   when com1=6 then do;dir1=dir1+1;if dir1>5 then dir1=2;call offse;end
   when com1='V' then do;call view_stats;ss cls;call display_room;end
   when com1='C' & study_hours~='STUDY_HOURS' then call cast_spell(2)
   when com1='Q' then do;loc_x1=0;loc=0;end
   otherwise nop
  end
 end
return

offse:
 offset1=offset1+1;if offset1>1 then offset1=0
return

display_room:
 ss'[1;1H'
 n1=0;if room_.loc_f1.loc_x1.loc_y1.dir1~=0 then n1=1
 dir2=dir1+1;if dir2>5 then dir2=2;e1=0;if room_.loc_f1.loc_x1.loc_y1.dir2~=0 then e1=1
 dir2=dir1-1;if dir2<2 then dir2=5;w1=0;if room_.loc_f1.loc_x1.loc_y1.dir2~=0 then w1=1
 if room.n1.e1.w1.offset1.1='ROOM.'n1'.'e1'.'w1'.'offset1'.1' then call load_roompic(n1'.'e1'.'w1'.'offset1)
 do i=1 to 12;tr center(room.n1.e1.w1.offset1.i,78);end i
return

load_roompic:
 parse arg fn1
 call open(1,path1'Room.'fn1,r);do i=1 to 12;room.n1.e1.w1.offset1.i=readln(1);end i;call close(1)
return

load_room:
 parse arg fn1
 call open(1,path'cave/'fn1,r);dat1=readln(1);call close(1)
 do i=1 to 9;parse var dat1 room_.loc_f1.loc_x1.loc_y1.i','dat1;end i
return

load_monster:
 parse arg whomi
 call open(1,path6''local'.'whomi,r);dat1=readln(1);call close(1)
 do i=1 to 32;parse var dat1 mon.whomi.i','dat1;end i
 do iii=21 to 22
  j=iii-20
  parse var mon.whomi.iii mon_.whomi.j.0'/'mon.whomi.iii
  do i=1 to mon_.whomi.j.0
   parse var mon.whomi.iii mon_.whomi.j.i'/'mon.whomi.iii
  end i
 end iii
return

Genadil:
 do forever
  if ~exists(path9''idnum) then call g_3
  if loc=0 then return
  tr cls'You are standing along the main streets of Genadil.';tr'There are numerous places you may visit.';sf path1'GMain';if study_hours~='STUDY_HOURS' then tr p7'C)ast a Spell';tr p7'Q)uit'
  tr;tr'HP: 'ud.7' / 'ud.8', MP: 'ud.9' / 'ud.10;tr
  ss'Command >> ';gc;call gcc(1)
  select
   when com1='T' then call g_1
   when com1='D' then call g_2
   when com1='G' then call g_3
   when com1='R' then call g_4
   when com1='J' then call g_5
   when com1='B' then call g_6
   when com1='K' then call g_7
   when com1='M' then call g_8
   when com1='V' then call view_stats
   when com1='C' & study_hours~='STUDY_HOURS' then call cast_spell(2)
   when com1='L' then do
    ss'Enter F)orest or C)ave > ';gc;call gcc(1)
    if com1='F' then loc=2
    if com1='C' & ud.5<5 then do;tr'You''re a little weak to be playing in the dungeon...';call pcr;end
    if com1='C' & ud.5>4 then loc=3
    if loc>1 then return
   end
   when com1='Q' then do;loc=0;return;end
   otherwise nop
  end
 end
return

list_spells:
 call get_spell_chart;call save_spell(0,st_hr);tt=ud.4
 temp=clas_dat.tt.18;do i=1 to 5;parse var temp splma.i'/'splst.i'/'temp;end i
 do i=1 to 5
  if splma.i>0 & ud.5>=splst.i then do
   tr center('>> 'spell_class.i' Spells <<',78)
   tr'Spell Name                     Code Cost Used'
   tr copies('-',78)
   tempp=spell_a.i
   do until tempp=""
    parse var tempp spellid':'tempp;if spellid~='' then do
     if spldat_.spellid.1='SPLDAT_.'spellid'.1' then call load_spell(spellid)
     tmp=spldat_.spellid.9;parse var tmp ttt'/'tmp'/'cost;tr left(spldat_.spellid.1,30)' 'spellid'  'right(cost,4)' 'spluse.tmp
    end
   end
   tr copies('-',78);call pcr
  end
 end
return

check_spell:
 parse arg iffy,spel_nam /* 1=Battle 2=Non-Battle */
 okay=0;call get_spell_chart;found=0;do i=1 to 5
  if found=1 then break;tmp=spell_a.i
  do forever
   if tmp=""|found=1 then break
   parse var tmp spell':'tmp
   if spell=spel_nam then found=1
  end
 end i
 if found=1 then do
  if spldat_.spel_nam.1='SPLDAT_.'spel_nam'.1' then call load_spell(spel_nam)
  tmp=spldat_.spel_nam.9;parse var tmp ttt'/'tmp'/'sp_cost
  if tmp=iffy|tmp=0|iffy=0 then okay=1;if tmp>0 & iffy~=tmp & iffy>0 then okay=2
 end
return

cast_spell:
 parse arg where
 do forever
  query'Enter Spell Code [L)ist]: ';call gcc(0);com1=upper(com1)
  if com1='L'|com1='LIST' then do;call list_spells;okay=3;end
  else break
 end
 call check_spell(where,com1);if okay=1 then call figure_type(com1)
 if okay=2 then tr'Cannot use this spell now.'
 if okay=0 then tr'No such spell.'
return

figure_type:
 parse arg spl_nam
 if sp_cost>ud.9 then do;tr'Not enough magic points.';call pcr;return;end
 attck.0=1;ud.9=ud.9-sp_cost
 jj=0;do i=2 to 8
  if spldat_.spl_nam.i~=spl_mod.i then do;jj=jj+1;s_code.jj=i;end
 end i
 if jj>0 then call divide_code
return

divide_code:
 do il=1 to jj
  if s_code.il=2 then call spelltype1(2)
  if s_code.il=3 then call spelltype1(3)
  if s_code.il=4 then call spelltype2
  if s_code.il=5 then call spelltype3
  if s_code.il=6 then call spelltype4(2)
  if s_code.il=7 then call spelltype4(3)
  if s_code.il=8 then call spelltype5
 end il
return

spelltype1:
 parse arg tmpy;if tmpy=2 then tmp1=7;else tmp1=9;tmp2=tmp1+1
 parse var spldat_.spel_nam.tmpy loty'/'hity
 if loty>-1 then do;tr'You cast 'spldat_.spl_nam.1'!';heal=rnd(loty,hity);if heal>ud.8-ud.7 then heal=ud.8-ud.7;tr spldat_.spel_nam.11;tr'You are healed 'heal' 'spl_ty.tmpy'.';ud.7=ud.7+heal;end
 if loty<0 then do
  groop=1;if mty>1 then call select_group;tr'You cast 'spldat_.spl_nam.1'!'
  dmg=rnd(abs(loty),abs(hity));tt=monst_.groop;ypow=ud.15+bat_cod.0.3+ud.16+bat_cod.0.4+ud.18+bat_cod.0.6+rnd(1,10);tpow=mon.tt.24+bat_cod.groop.3+mon.tt.25+bat_cod.groop.4+mon.tt.27+bat_cod.groop.6+rnd(1,10);ss'('ypow':'tpow') 'spldat_.spl_nam.11' '
  if tpow>ypow then do;tr spldat_.spl_nam.13;tr'You missed the 'mon.tt.1'.';end
  else do;tr spldat_.spl_nam.12;tr'You hit the 'mon.tt.1' for 'dmg' 'spl_ty.tmpy' of damage!';monstb_.groop.1.1=monstb_.groop.1.1-dmg;call check_dead;end
 end
return

spelltype2:
 tmp=spldat_.spel_nam.4;do iln=1 to 10
  parse var tmp pow.iln'/'tmp
  if pow.iln~=0 then do
   if pow.iln>0 then do
    tr'You cast 'spldat_.spl_nam.1'!';bat_cod.0.iln=bat_cod.0.iln+pow.iln;tr spldat_.spel_nam.11;tr'Your 'stat_n.iln' is raised 'pow.iln' point(s).';if iln=7 then bat_cod.0.7=pow.iln
   end
   else do
    groop=1;if mty>1 then call select_group;tr'You cast 'spldat_.spl_nam.1'!'
    tt=monst_.groop;ypow=ud.15+bat_cod.0.3+ud.16+bat_cod.0.4+ud.18+bat_cod.0.6+rnd(1,10);tpow=mon.tt.24+bat_cod.groop.3+mon.tt.25+bat_cod.groop.4+mon.tt.27+bat_cod.groop.6+rnd(1,10);ss'('ypow':'tpow') 'spldat_.spl_nam.11' '
    if tpow>ypow then do;tr spldat_.spl_nam.13;tr'You missed the 'mon.tt.1'.';end
    else do;tr spldat_.spl_nam.12;bat_cod.groop.iln=bat_cod.groop.iln+pow.iln;tr spldat_.spel_nam.11;tt=monst_.groop;tr"The "mon.tt.1"'s "stat_n.iln" is lowered "pow.i" point(s).";end
   end
  end
 end iln
return

spelltype3:
 parse var spldat_.spel_nam.5 lor'/'hir'/'typ
 groop=1;if mty>1 then call select_group;tr'You cast 'spldat_.spl_nam.1'!'
 do ii=1 to monstc_.groop
  tt=monst_.groop;ypow=ud.15+bat_cod.0.3+ud.16+bat_cod.0.4+ud.18+bat_cod.0.6+rnd(1,10);tpow=mon.tt.24+bat_cod.groop.3+mon.tt.25+bat_cod.groop.4+mon.tt.27+bat_cod.groop.6+rnd(1,10);ss'('ypow':'tpow') 'spldat_.spl_nam.11' '
  if tpow>ypow then do;tr spldat_.spl_nam.13;tr'You missed the 'mon.tt.1'.';end
  else do;tr spldat_.spl_nam.12;tr'You hit the 'mon.tt.1'!';monstb_.groop.ii.3=typ;monstb_.groop.ii.4=rnd(lor,hir);end
 end ii
return

spelltype4:
return

spelltype5:
return

g_1:
 do forever
  tr cls'You walk into Talin''s Equipment Shop.';tr'B)uy something';tr'S)ell something';tr'I)dentify something';tr'L)eave shop';tr
  ss'Command >> ';gc;call gcc(1)
  select
   when com1='B' then call buy_shop
   when com1='S' then call sell_shop
   when com1='I' then call identify_shop
   when com1='L' then break
   otherwise nop
  end
 end
return

identify_shop:
 do forever
  ss cls;call view_eq
  query'Identify which Item? ';call gcc(2)
  if com1>0 & com1<ud.36+1 then do
   parse var eq_.com1 ity'.'itid;com2=com1;if typeu_.com2=0 then do;tr'This item is already identified!';call pcr;end
   else do
    tr'>> ?'def_type.ity'? <<'
    tr'Talin says: "It''ll cost you 'ita.ity.itid.9%2' gold pieces to identify this."'
    if ud.6<ita.ity.itid.9%2 then do;tr'Not enough gold.';call pcr;end
    else do
     ss'Identify Item? (y/N) ';gc;call gcc(1)
     if com1='Y' then do;tr'Identified!';ud.6=ud.6-ita.ity.itid.9%2;tr'You now have 'ud.6' GPs.';typeu_.com2=0;end
    end
   end
  end
  if com1=0 then break
 end
return

buy_shop:
 tr'View which list?';tr;tr'W)eapons';tr'A)rmor';tr'S)hields';tr'H)elmets';tr'G)loves';tr
 ss'Command >> ';gc;call gcc(1)
 nov=0
 if com1='W' then nov=1;if com1='A' then nov=2;if com1='S' then nov=3;if com1='H' then nov=4;if com1='G' then nov=5;ity=nov
 if nov>0 then do
  tr'Talin says: "One second, let me find my list..."'
  call read_shop;base_loc=(nov-1)*300-1;totl=0
  tr'"Ahhh, here it is!"'
  tr cls'##   Name                     Weight Cost'
  do i=1 to 60
   itmdat=c2d(right(left(itmsho_all,base_loc+i),1))
   if itmdat>0 then do
    totl=totl+1;itml.totl=itmdat;itid=itmdat;call get_item(nov,i);trck.totl=i
    if ita.ity.itid.1='' then do;totl=totl-1;iterate i;end
    call check_equip;spacer=' ';if okay=1 then spacer='#';if okay=2 then spacer='@'
    tr right(totl,2)' 'spacer' 'left(ita.ity.itid.1,24)' 'left(ita.ity.itid.2,6)' 'ita.ity.itid.9
   end
  end i
  if totl=0 then do;tr'Nothing for sale!';call pcr;end
  if totl>0 then do
   tr'You have 'ud.6' GPs. # or @ means you cannot equip this item.'
   query'Enter Item Number >> ';call gcc(2)
   if com1>0 & com1<totl+1 then do
    itid=shii.com1;call get_item(nov,trck.com1)
    tr'>> 'ita.ity.itid.1' <<'
    if ud.6<ita.ity.itid.9 then do;tr'Not enough gold.';call pcr;end
    else do
     call check_equip
     if okay=1 then tr'Your class cannot equip this.'
     if okay=2 then tr'This is too heavy for you to equip.'
     ss'Are you sure you want to buy this? (y/N) ';gc;call gcc(1)
     if com1='Y' then do;ud.36=ud.36+1;tt=ud.36;typeu_.tt=0;eq_.tt=ity'.'itid;ud.6=ud.6-ita.ity.itid.9;tr'Sold!';call update_shop(1,ity,itid);end
    end
   end
  end
 end
return

get_item:
 parse arg ity,itid
 if ita.ity.itid.1='ITA.'ity'.'itid'.1' then do
  vl1=((ity-1)*300+itid)
  itemdata=right(left(a_items_dat,(vl1-0)*36),36)
  ita.ity.itid.1=strip(left(itemdata,24),T,d2c(0))
  do lii=2 to 8
   ita.ity.itid.lii=c2d(right(left(itemdata,24+lii-1),1))
  end lii
  ita.ity.itid.9=c2d(right(left(itemdata,34),3))
  ita.ity.itid.10=c2d(right(left(itemdata,35),1))
  ita.ity.itid.11=c2d(right(itemdata,1))
 end
return

load_i:
 call open(1,path3'Items.dat',r);a_items_dat=readch(1,54000);call close(1)
return

read_shop:
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(1,path3'ItemShop',r);if i=100000 then do;tr'Error with ItemShop datafile. Contact DragonLord immediately!';break;end
  end
  itmsho_all=readch(1,1500);call close(1)
 call permit;f_flag=0
return

update_shop:
 parse arg gag;call read_shop
 base_loc=(ity-1)*300+itid;if ity>1 then base_loc=base_loc-1
 itmdat=c2d(right(left(itmsho_all,base_loc),1))
 if gag=1 then itmdat=itmdat-1
 if gag=2 then itmdat=itmdat+1;if itmdat>250 then itmdat=250;if itmdat<0 then itmdat=0
 itmsho_all=overlay(d2c(itmdat),itmsho_all,base_loc,1)
 call forbid;f_flag=1
  iam=0;do i=1 until iam=1
   iam=open(1,path3'ItemShop',w);if i=100000 then do;tr'Error with ItemShop datafile. Contact DragonLord immediately!';break;end
  end
  call writech(1,itmsho_all);call close(1)
 call permit;f_flag=0
return

sell_shop:
 do forever
  ss cls;call view_eq
  query'Sell which Item? ';call gcc(2)
  if com1>0 & com1<ud.36+1 then do
   parse var eq_.com1 ity'.'itid;com2=com1;nam_ob=ita.ity.itid.1;if typeu_.com2=1 then nam_ob='?'def_type.ity'?'
   tr'>> 'nam_ob' <<'
   tr'Talin says: "I''ll give you 'ita.ity.itid.9%2' gold pieces for this."'
   ss'Sell Item? (y/N) ';gc;call gcc(1)
   if com1='Y' then do
    tr'Sold!';ud.6=ud.6+ita.ity.itid.9%2;tr'You now have 'ud.6' GPs.';nov=ity
    call drop_obj;call update_shop(2)
   end
  end
  if com1=0 then break
 end
return

g_2:
 tr cls'You are standing in the Dragon''s Deep Inn.'
 sf path1'DDInn'
 tr;tr p7'You have 'ud.6' GPs. HP: 'ud.7' / 'ud.8', MP: 'ud.9' / 'ud.10
 ss'Command >> ';gc;call gcc(1)
 jjk=0;if com1='S' then jjk=1;if com1='C' then jjk=2;if com1='E' then jjk=3;if com1='M' then jjk=4;if com1='T' then jjk=5
 if jjk>0 then do
  if ud.6<inn.jjk then do;tr'Not enough gold.';call pcr;return;end
  ud.6=ud.6-inn.jjk;tr'You hand the clerk your money, and he gives you your key. You quickly hurry to 'inn_.jjk' and lie down for some much needed rest...'
  tr;tr;heal=inn.jjk;mheal=inn.jjk;if heal>ud.8-ud.7 then heal=ud.8-ud.7;if mheal>ud.10-ud.9 then mheal=ud.10-ud.9;tr'You were healed 'heal' HPs and 'mheal' MPs.';ud.7=ud.7+heal;ud.9=ud.9+mheal
  tr;tr'You awaken and feel refreshed.';call pcr
 end
return

g_3:
 do forever
  if loc=0 then break
  tr cls'You feel a strange presence of souls in this room.';tr
  tr'S)elect a Character to Use'
  tr'F)ind A Special Character'
  tr'R)egister Character Onto Roster'
  tr'L)ist "Souls" in Room'
  if exists(path9''idnum) then tr'Q)uit to Streets'
  else tr'Q)uit Game!'
  ss'Command >> ';gc;call gcc(1)
  select
   when com1='S' then call register
   when com1='L' then call g_3_l(0)
   when com1='F' then call find_char
   when com1='R' then call roster
   when com1='Q' & exists(path9''idnum) then break
   when com1='Q' & ~exists(path9''idnum) then loc=0
   otherwise nop
  end
 end
return

find_char:
 tr'Find which type of character?';tr'I)dentify';tr
 ss'Command >> ';gc;call gcc(1)
 select
  when com1='I' then call wizi_cl
  otherwise nop
 end
return

wizi_cl:
 call g_3_l(23)
 if ii=0 then tr'There are none on the roster!'
 if ii>0 then do
  query'Command >> ';call gcc(2)
  if com1>0 & com1<ii+1 then do
   zipp=com1
   call load_o_user(path'user/'char1.zipp)
   do forever
    tr;call view_eq
    query'Enter Item Number to Identify> ';call gcc(2)
    if com1>0 & com1<ud.36+1 then call id_options
    if com1=0 then break
   end
  end
 end
 call pcr
return

id_options:
 com2=com1;tr;parse var eq_.com1 ity'.'itid;nam_ob=ita.ity.itid.1
 if typeu_.com1~=1 then do;tr'This item is already identified!';return;end
 call identify_item(com2,od,od.1,'does');call save_o_user(path'user/'char1.zipp)
return

roster:
 if ud.23=0 then return
 if ud.23>0 then call class_add('Wizard.R',1)
return

class_add:
 parse arg xxx,yyy
 if spec_cl.yyy=1 then do
  ss'Do you wish to remove yourself from the roster? (y/N) ';gc;call gcc(1)
  if com1='Y' then do;tr'Removed!';spec_cl.yyy=0;end
 end
 else do
  sf path1''xxx
  ss'Do you wish to add your name to the roster? (y/N) ';gc;call gcc(1)
  if com1='Y' then do;tr'Added!';spec_cl.yyy=1;end
 end
return

g_3_l:
 parse arg which_cl;path_h=path'wizi/';if which_cl=0 then path_h=path7
 files=showdir(path_h,FILE);ii=0
 tr cls'##) 'left('Name',26)' 'left('Sex',6)' 'left('Race',10)' 'left('Class',11)' Lvl 'right('Experience',14)
 do forever
  oi=ii;ii=ii+1;parse var files num'.'nam'_' files
  if num="" then break
  char1.ii=strip(num,L)'.'nam'_'
  if exists(path7''char1.ii) then do
   call load_o_user(path7''char1.ii);sex1=od.2;rac1=od.3;cla1=od.4
   tr right(ii')',3)' 'left(od.1,26)' 'left(sex.sex1,6)' 'left(race.rac1,10)' 'left(class.cla1,11)' 'right(od.5,3)' 'right(od.11,14)
  end
  if ~exists(path7''char1.ii) then do;address command 'delete "'path_h''char1.ii'"';ii=ii-1;end
 end;ii=ii-1
 if which_cl=0 then call pcr
return

g_4:
 tr cls'You stand before the Review Board.'
 tr'A)dvance Experience Level'
 tr'L)eave the Review Board'
 tr;ss'Command >> ';gc;call gcc(1)
 if com1='A' then call check_advance
return

check_advance:
 if ud.11<ud.12-1 then do;tr'You still need 'ud.12-ud.11' experience points before you can advance to level 'ud.5+1'.';call pcr;return;end
 ud.5=ud.5+1;tr cls'You have advanced to level 'ud.5'!';spec=rnd(1,100);ud.12=(ud.12+ud.12*1.5)%1
 bogy=2;if spec>65 then bogy=0;if spec>90 then bogy=1
 tt=ud.4;hp=rnd(clas_dat.tt.2,clas_dat.tt.3)+ud.17%2;ud.8=ud.8+hp;ud.7=ud.8
 tr'You gained 'hp' HP. New HPMAX: 'ud.8
 ttt=clas_dat.tt.1;if ttt>2 & ttt<6 then do
  if ttt=1|ttt=2 then bonus=ud.15;if ttt=3|ttt=4 then bonus=ud.16;if ttt=5 then bonus=ud.15+ud.16
  mp=rnd(1,2+bonus);ud.10=ud.10+mp;ud.9=ud.10;tr'You gained 'mp' MP. New MPMAX: 'ud.10
  bonus_st=ud.5*10;tr'You are also awarded 'bonus_st' Study Hours!';study_hours=study_hours+bonus_st;call save_spell(0,st_hr)
 end
 tr'Stats- S:'ud.13' D:'ud.14' I:'ud.15' W:'ud.16' C:'ud.17' L:'ud.18
 t1=ud.3;if bogy>0 then do;spec=rnd(1,100)
  do i=1 to 6
   if spec>stl.i.t1 & spec<sth.i.t1 then do
    jj=i+12;if bogy=1 & ud.jj>1 then do;tr'One of your statistics dropped!';ud.jj=ud.jj-1;end
    if bogy=2 then do;tr'One of your statistics went up!';ud.jj=ud.jj+1;end
   end
  end i;tt=ud.4
  tr'Stats- S:'ud.13' D:'ud.14' I:'ud.15' W:'ud.16' C:'ud.17' L:'ud.18
 end
 do i=4 to 17
  sta1=clas_dat.tt.i;jj=i+16;parse var sta1 bbb1'/'bbb2'/'bbb3'/'bbb4
  if bbb3~=0 & ud.5>bbb3-1 then do;if ud.5+1>bbb4 & ud.5//bbb3=0 then ud.jj=ud.jj+bbb2;end
  if ud.jj>100 then ud.jj=100
 end i
 call pcr
return

g_5:
g_6:
g_7:
tr'Under Construction'
return

g_8:
 tt=ud.4;ttt=clas_dat.tt.1;if ttt>2 & ttt<6 then do
  tr cls'You are standing in a large room, with tons of books.'
  tr'S)tudy Magic'
  tr'L)eave the Magic Temple'
  ss'Command >> ';gc;call gcc(1)
  if com1='S' then call study_spells
 end
 else do;tr'Your class cannot learn magic!';call pcr;end
return

get_spell_chart:
 tmp=ud.34;do i=1 to 5;parse var tmp '('spell_a.i')'tmp;end i;study_hours=tmp
return

view_spells:
 tr cls''center('>> 'spell_class.tt' Spells <<',78);sf path1'SpellList.'tt;call pcr
return

study_spells:
 temp=clas_dat.tt.18
 do i=1 to 5;parse var temp splma.i'/'splst.i'/'temp;end i
 tr'Study which book?'
 j=0;do i=1 to 5
  if splma.i>0 & ud.5>=splst.i then do;j=j+1;tr j') 'spell_class.i;spl_u.j=i;end
 end
 query'Command >> ';call gcc(2)
 if com1<1|com1>j then return
 else do;tt=spl_u.com1
  do until again=0
   call get_spell_chart;new_sp=0;torem=tt
   if study_hours="" then do
    if tt=1|tt=2 then bonus=ud.15
    if tt=3|tt=4 then bonus=ud.16
    if tt=5 then bonus=ud.15+ud.16
    study_hours=rnd(10,17+bonus);call save_spell(0,st_hr)
   end
   tr'You have 'study_hours' study hours left.'
   tr;tr'S)tudy Spells';tr'V)iew All Spells';tr'L)eave';tr
   ss'Command >> ';gc;call gcc(1);again=0
   if com1='V' then call view_spells
   if com1='S' then do
    again=1
    call open(2,path3'SpellList.'torem,r);top=0
    do forever
     top=top+1;xlx=readln(2);if xlx='' then break;parse var xlx spelln_.top' 'spellsc_.top
     if spellsc_.top>study_hours then break
     call check_spell(0,spelln_.top);if okay~=0 then top=top-1
    end
    call close(2);top=top-1;if top=0 then do;tr'Not enough Study Hours to study a spell.';call pcr;return;end
    tr'##) Name                          Study Hours Needed'
    do ii=1 to top
     tj=spelln_.ii;if spldat_.tj.1='SPLDAT_.'tj'.1' then call load_spell(tj)
     tr right(ii,2)') 'left(spldat_.tj.1,30)' 'spellsc_.ii
    end ii
    tr;query'Command >> ';call gcc(2)
    if com1>0 & com1<ii+1 then do
     put_back=0;study_hours=study_hours-spellsc_.com1;tj=spelln_.com1;tr'You sit down to learn the spell 'spldat_.tj.1'.'
     tr'You read slowly down the page, intaking all the components...';call delay(50)
     ss'You slowly start to memorize the material.';do iii=1 to 10;call delay(50);ss'.';end;tr
     tr'You stand up and close the book. You have just learned the spell 'spldat_.tj.1'!'
     if spell_a.torem~='' then spell_a.torem=spell_a.torem':'tj
     else spell_a.torem=tj
     call save_spell(0,st_hr);call pcr
    end
   end
  end
 end
return

save_spell:
 parse arg who,st_hr
 if who=0 then do;ud.34='';do i=1 to 5;ud.34=ud.34'('spell_a.i')';end i;ud.34=ud.34''study_hours;end
 if who=1 then do;parse var od.34 j1')'j2')'j3')'j4')'j5')'st_hr1;if datatype(st_hr1)='NUM' then st_hr1=st_hr1+st_hr;else st_hr=0;od.34=j1')'j2')'j3')'j4')'j5')'st_hr1;end
return

load_spell:
 parse arg obj_nam
 call open(1,path8''obj_nam,r);dat1=readln(1);call close(1)
 do j=1 to 13;parse var dat1 spldat_.obj_nam.j','dat1;end j
return

wep_pow:
 wp=ud.13;if typeq_.1~=0 then do;wpp=typeq_.1;parse var eq_.wpp ity'.'itid;wp=wp+ita.ity.itid.2;end
return

arm_val:
 av=ud.13%2;do vi=2 to 5;if typeq_.vi~=0 then do;avv=typeq_.vi;parse var eq_.avv ity'.'itid;av=av+ita.ity.itid.2;end;end vi
return

view_stats:
 t1=ud.2;tr cls''left(' Name: 'ud.1,44)'Sex: 'sex.t1
 t1=ud.3;t2=ud.4;tr left(' Race: 'race.t1,42)'Class: 'class.t2
 tr'Level: 'left(ud.5,38)'GP: 'ud.6
 tr'HPMAX: 'left(ud.8,38)'HP: 'ud.7
 tr'MPMAX: 'left(ud.10,38)'MP: 'ud.9
 tr '   XP: 'left(ud.11,33)'XP Need: 'ud.12
 call arm_val;call wep_pow
 tr 'ArmVa: 'left(av,28)'Attack Power: 'wp
 tr 'Str: 'left(ud.13,8)'Dex: 'left(ud.14,8)'Int: 'left(ud.15,8)'Wis: 'left(ud.16,8)'Con: 'left(ud.17,8)'Luk: 'ud.18
 do forever
  tr;call view_eq
  query'Enter Item Number, or Return to Continue> ';call gcc(2)
  if com1>0 & com1<ud.36+1 then do;call eq_options;end
  if com1=0 then break
 end
return

eq_options:
 itemun=0;com2=com1;tr;parse var eq_.com1 ity'.'itid;nam_ob=ita.ity.itid.1;if typeu_.com1=1 then do;itemun=1;nam_ob='?'def_type.ity'?';end
 tr '>> 'nam_ob' <<';tr;nol=1;do i=1 to 5;if typeq_.i=com1 then do;nol=0;tr'U)nequip this item';end;end i
 if nol=1 then tr'E)quip this item'
 if itemun=1 & ud.23>0 then tr'I)dentify this item'
 tr'D)rop this item';tr'Q)uit'
 ss'Command >> ';gc;call gcc(1)
 if com1='E' & nol=1 then call equip_obj
 if com1='U' & nol=0 then call unequip_obj
 if com1='I' & itemun=1 & ud.23>0 then call identify_item(com2,ud,'You','do')
 if com1='D' then do;ss'Are you sure you wish to drop this item? (y/N) ';gc;call gcc(1);if com1='Y' then call drop_obj;end
return

identify_item:
 parse arg com2,whm,whm1,whm2
 if whm=ud then do;do i=1 to 35;jkl.i=ud.i;end i;end
 if whm=od then do;do i=1 to 35;jkl.i=od.i;end i;end
 if jkl.7>1 then do
  succ=rnd(1,100);ss'('jkl.23':'succ') ';if succ<jkl.23+1 then do
   tr whm1' successfully Identified it!';typeu_.com2=0;xpgain=ita.ity.itid.9%2;stgain=xpgain%100;jkl.11=jkl.11+xpgain
   call study_gain(xpgain/100,whm);tr whm1' gained 'xpgain' xp.';if st_hr>0 then tr whm1' also gained 'st_hr' Study Hours.';if whm=ud then do;st_hr1=study_hours;ss left('XP: 'ud.11,30);if st_hr=0 then tr;else tr'Study Hours: 'st_hr1;end
  end
  else do;tr whm1' could not identify it! 'whm1' lost 1 HP!';jkl.7=jkl.7-1;end
 end
 else tr whm1' 'whm2' not have enough HPs.'
 if whm=ud then do;do i=1 to 25;ud.i=jkl.i;end i;end
 if whm=od then do;do i=1 to 25;od.i=jkl.i;end i;end
return

study_gain:
 parse arg st_hr,whm;if study_hours='STUDY_HOURS' & whm=ud then do;st_hr=0;return;end
 parse var st_hr f1'.'f2;if f2~='' then do;f2=left(f2,2);st_hr=f1'.'f2;end
 if whm=ud then do;study_hours=study_hours+st_hr;call save_spell(0,st_hr);end
 if whm=od then call save_spell(1,st_hr)
return

unequip_obj:
 parse var eq_.com2 ity'.'itid
 typeq_.ity=0
return

equip_obj:
 parse var eq_.com2 ity'.'itid;if typeu_.com2=1 then do;tr'You cannot equip an unidentified item!';return;end
 call check_equip;if okay=0 then typeq_.ity=com2
 if okay=1 then do;tr'Your class cannot equip this.';call pcr;end
 if okay=2 then do;tr'This is too heavy for you to equip.';call pcr;end
return

check_equip:
 okay=0;wgt=ita.ity.itid.2
 if wgt>ud.13 then okay=2
 tt=ud.4;tu=clas_dat.tt.1+2
 if ita.ity.itid.tu=0 then okay=1
return

drop_obj:
 parse var eq_.com2 ity'.'itid
 if typeq_.ity=com2 then typeq_.ity=0
 if com2<ud.36 then do
  tt=ud.36;eq_.com2=eq_.tt;parse var eq_.tt ity1'.'itid1;if typeq_.ity1=tt then typeq_.ity1=com2;typeu_.com2=typeu_.tt;end
 ud.36=ud.36-1
return

view_eq:
 tr'## Weight E 'left('Name',45)' Type';tr copies('-',78);tot_w=0
 if ud.36>0 then do
  do i=1 to ud.36
   tes=left(eq_.i,1);eq1=' '
   parse var eq_.i ity'.'itid
   if typeq_.tes=i then eq1='e'
   call get_item(ity,itid)
   tot_w=tot_w+ita.ity.itid.2;nam_ob=ita.ity.itid.1;if typeu_.i=1 then nam_ob=copies('?',45)
   tr right(i,2)' 'right(ita.ity.itid.2,6)' 'eq1' 'left(nam_ob,45)' 'def_type.tes
  end i
 end
 tr copies('-',78)
 tr right(tot_w,9)' <- Total Weight   Max Weight -> 'ud.13*3
return

get_tot_wgt:
 tot_w=0;if ud.36>0 then do;do i=1 to ud.36;parse var eq_.i ity'.'itid;tot_w=tot_w+ita.ity.itid.2;end i;end
return

get_mon_wgt:
 tot_mw=0;whomi=monst_.1
 do i=1 to 5
  jj=13+i
  if mon.whomi.jj~=0 then do
   parse var mon.whomi.jj ity'.'itid
   call get_item(ity,itid)
   tot_mw=tot_mw+ita.ity.itid.2
  end
 end i
return

round_check:
 yourd=rnd(1,10)+ud.14+bat_cod.0.2-tot_w%2
 call get_mon_wgt;whomi=monst_.1
 therd=rnd(1,10)+mon.whomi.5=bat_cod.1.2-tot_mw%2
return

run_away:
 call round_check
 if yourd>therd-1 then do;tr'You turn and run away!';run1=1;end
 else do;tr'You turn to run but are blocked!';attck.0=1;end
return

select_group:
 tr'Attack which enemy group?'
 do i=1 to mty
  sss='';whomi=monst_.i;if monstc_.i>1 then sss='s';tr i') 'monstc_.i' 'mon.whomi.1''sss
 end i
 query'Command >> ';call gcc(2)
 if com1<1|com1>mty then com1=1
 groop=com1
return

your_attack:
 bat_cod.0.12=bat_cod.0.12-1;if bat_cod.0.12>0 then do
  tt=bat_cod.0.11;tr'You are still 'comotose.tt.1
  if bat_cod.0.11=4 then do;loss=rnd(1,ud.5);tr'You lose 'loss' HPs!';ud.7=ud.7-loss;if ud.7<1 then call your_dead;end
 end
 if bat_cod.0.11~=3|bat_cod.0.12<1 then do
  do att=1 to attacks+bat_cod.0.7
   tr'What do you want to do?'
   tr'A)ttack'
   tr'R)un'
   if study_hours~='STUDY_HOURS' then tr'C)ast Spell'
   tr'HP: 'ud.7' / 'ud.8', MP: 'ud.9' / 'ud.10
   ss'Command >> ';gc;call gcc(1)
   if com1='R' then call run_away
   if com1='C' & study_hours~='STUDY_HOURS' then call cast_spell(1)
   if run1=1 then return
   if attck.0~=1 then do
    groop=1;if mty>1 then call select_group
    whomi=monst_.groop
    call wep_pow
    ypow=rnd(1,10)+wp+bat_cod.0.1+bat_cod.0.8
    tpow=rnd(1,10)+mon.whomi.6+bat_cod.groop.9+(bat_cod.groop.1%2);spec1=rnd(1,30);if spec1=30 & ypow<tpow then ypow=tpow;ss'('ypow':'tpow') '
    select
     when ypow<mon.whomi.5+bat_cod.groop.2 then tr'You attack the 'mon.whomi.1' but miss completely!'
     when ypow<tpow then tr'You attack the 'mon.whomi.1' but his 'mon.whomi.23' absorbed the damage!'
     otherwise do
     jj=typeq_.1;lod=0;hid=1;wgt=ud.13+bat_cod.0.1
     if jj~=0 then do;parse var eq_.jj ity'.'itid;lod=ita.ity.itid.10;hid=ita.ity.itid.11;wgt=ita.ity.itid.2+ud.13;end
     spec=rnd(1,100);if surprise=1 then spec=100;if spec=100 then do;lod=lod*2;hid=hid*2;end;if wgt<0 then wgt=0
     dmg=rnd(lod+bat_cod.0.10,hid+bat_cod.0.10)+rnd(0,wgt);if dmg<0 then dmg=0
      tr'You attack the 'mon.whomi.1' and hit for 'dmg' hit points of damage!'
      monstb_.groop.1.1=monstb_.groop.1.1-dmg;call check_dead;if done1=1 then return
     end
    end
   end
  end att
 end
 if attck.1=0 then call mon_attack
return

check_dead:
 whomi=monst_.groop
 if monstb_.groop.1.1<0 then do
  tr'You killed the 'mon.whomi.1'!';call get_win
  select
   when monstc_.groop=1 & mty>1 then do
    if groop=mty then mty=mty-1
    else do
     monstc_.groop=monstc_.mty
     do vi=1 to monstc_.groop
      do vii=1 to 4;monstb_.groop.vi.vii=monstb_.mty.vi.vii;end vii
     end vi
     monst_.groop=monst_.mty;mty=mty-1
    end
   end
   when monstc_.groop>1 then do
    top=monstc_.groop;do vii=1 to 4;monstb_.groop.1.vii=monstb_.groop.top.vii;end vii;monstc_.groop=monstc_.groop-1
   end
   otherwise do;tr'You have vanquished your foes!';call pcr;done1=1;return;end
  end
 end
return

get_win:
 whomi=monst_.groop;xpwin=monstb_.groop.1.2;gpwin=rnd(mon.whomi.19,mon.whomi.20)
 if mon.whomi.9~=0 then xpwin=xpwin+(mon.whomi.4*20)
 if mon.whomi.10~=0 then xpwin=xpwin+(mon.whomi.10*3)
 if mon.whomi.11~=0 then xpwin=xpwin+(mon.whomi.4*50)
 if mon.whomi.12~="0/0/0" then do;parse var mon.whomi.12 bas1'/'bas2'/'bas3;xpwin=xpwin+((bas2+bas3)*10+(bas1*3));end
 if mon.whomi.13~=0 then xpwin=xpwin+(mon.whomi.4*10+(mon.whomi.13*2))
 tr'You gained 'xpwin' xp and 'gpwin' gp.';ud.6=ud.6+gpwin;ud.11=ud.11+xpwin
 tr left('XP: 'ud.11,30)' GP: 'ud.6
 if study_hours~='STUDY_HOURS' then do;call study_gain(xpwin/100,ud);tr'You also gained 'st_hr' Study Hours. Total Study Hours: 'study_hours;end
 if mon.whomi.14~=0 then do
  tr'The monster had the following items:'
  do i=1 to 5;jj=i+13
   if mon.whomi.jj~=0 then do
    parse var mon.whomi.jj ity'.'itid;call get_item(ity,itid)
    tr'>> ?'def_type.ity'? <<';ss'Take it? (y/N) ';gc;call gcc(1)
    if com1='Y' then do;tr'Taken!';ud.36=ud.36+1;tt=ud.36;eq_.tt=ity'.'itid;typeu_.tt=1;end
   end
  end i
 end
return

mon_wep:
 whomi=monst_.groop;mwp=mon.whomi.4;if mon.whomi.14~=0 then do
  parse var mon.whomi.14 ity'.'itid
  call get_item(ity,itid)
  mwp=mwp+ita.ity.itid.2
 end
return

mon_attack:
 if done1=1 then return
 do ii=1 to mty
  do iii=1 to monstc_.ii
   whomi=monst_.ii;groop=ii
   if monstb_.ii.iii.3=0 then do
    call mon_wep;call arm_val;mon_att=mon.whomi.5%5;if mon_att<1 then mon_att=1
    do jk=1 to mon_att
     ypow=rnd(1,10)+av+bat_cod.0.9+(bat_cod.groop.1%2)
     tpow=rnd(1,10)+mwp+bat_cod.groop.1+bat_cod.groop.8;spec1=rnd(1,30);if spec1=30 & tpow<ypow then tpow=ypow;ss'('ypow':'tpow') '
     wh1=rnd(1,mon_.whomi.1.0);wh2=rnd(1,mon_.whomi.2.0)
     mnw1=0;if mon.whomi.32~=0 then do;spc1=rnd(1,100);if spc1<mon.whomi.32 then mnw1=1;end;cont1=0
     if mnw1=1 & mon.whomi.13~=0 then do
      cont1=1;tr'The 'mon.whomi.1' calls for help!';spcn1=rnd(1,100)
      if spcn1<mon.whomi.13 then do
       sss1='';sss2='';mty=mty+1;monst_.mty=whomi;call figure_mon_st(mty)
       if monstc_.mty>1 then sss1='s';else sss2='s';tr'And 'monstc_.mty' more 'mon.whomi.1''sss1' appear'sss2'!'
      end
      else tr'But fails!'
     end
     parse var mon.whomi.12 bas1'/'bas2'/'bas3
     if mnw1=1 & bas1~=0 then do
      cont1=1;tr'The breaths at you!';spcn1=rnd(1,100)
      dmg=rnd(bas2,bas3);if spcn1>bas1 then dmg=dmg%2
      tr'You take 'dmg' HPs of damage!';ud.7=ud.7-dmg;if ud.7<1 then call your_dead
     end
     if cont1=0 then do
      select
       when tpow<ud.14 then tr'The 'mon.whomi.1' 'mon_.whomi.1.wh1' at you but misses completely!'
       when ypow>tpow then tr'The 'mon.whomi.1' 'mon_.whomi.1.wh1' at you but your armor absorbed the damage!'
       otherwise do
        jj=mon.whomi.14;lod=0;hid=1;wgt=mon.whomi.4+bat_cod.groop.1
        if jj~=0 then do;parse var jj ity'.'itid;lod=ita.ity.itid.10;hid=ita.ity.itid.11;wgt=ita.ity.itid.2+mon.whomi.4;end
        else do;lod=mon.whomi.28;hid=mon.whomi.29;wgt=mon.whomi.4;end
        spec=rnd(1,100);if surprise=1 then spec=100;if spec=100 then do;lod=lod*2;hid=hid*2;end;if wgt<0 then wgt=0
        dmg=rnd(lod+bat_cod.groop.10,hid+bat_cod.groop.10)+rnd(0,wgt);if dmg<0 then dmg=0
        spcn1=rnd(1,100);if spcn1<mon.whomi.9 then do;dmg=dmg*2;tr'>> Critical Hit! <<';end
        tr'The 'mon.whomi.1' 'mon_.whomi.1.wh1' at you and hits for 'dmg' hit points of damage!'
        if spcn1<mon.whomi.10 then do
         tr'You are poisoned!';bat_cod.0.11=4;bat_cod.0.12=999
        end
        if spcn1<mon.whomi.11 then do
         tr'You are paralyzed!';bat_cod.0.11=3;bat_cod.0.12=rnd(1,mon.whomi.4)
        end
        ud.7=ud.7-dmg;if ud.7<1 then call your_dead
       end
      end
     end
    end jk
   end
   if monstb_.ii.iii.3>0 then do
    tt=monstb_.ii.iii.3
    if monstb_.ii.iii.4>0 then do
     tr'The 'mon.whomi.1' is still 'comotose.tt.1'.'
     monstb_.ii.iii.4=monstb_.ii.iii.4-1
     end
    else do;tr'The 'mon.whomi.1' has 'comotose.tt.2'!';monstb_.ii.iii.3=0;end
   end
  end iii
 end ii
 if attck.0=0 then call your_attack
return

your_dead:
 tr'Alas, the enemies have prevailed...'
 tr;tr' ...but all hope is not lost. The gods feel mercy upon your soul and decide'
 tr'to give you another chance.';ud.7=ud.8;ud.6=ud.6%2;ud.9=ud.10
signal goodbye

figure_mon_st:
 parse arg xxy1
 whomi=monst_.xxy1;do imn=1 to 12;bat_cod.xxy1.imn=0;end imn
 hibat=mon.whomi.3;monstc_.xxy1=rnd(mon.whomi.2,hibat)
 do imn=1 to monstc_.xxy1
  monstb_.xxy1.imn.1=rnd(mon.whomi.7,mon.whomi.8)
  monstb_.xxy1.imn.2=((mon.whomi.4*mon.whomi.5)*2)+(mon.whomi.6*5)+(monstb_.xxy1.imn.1*3)+0
  monstb_.xxy1.imn.3=0;monstb_.xxy1.imn.4=0
 end imn
return

battleprocedure:
 done1=0;attacks=ud.14%5;if attacks<1 then attacks=1;run1=0;surprise=0;tr'You encountered:'
 do ii=1 to 12;bat_cod.0.ii=0;end ii
 do i=1 to mty
  call figure_mon_st(i)
  sss='';if monstc_.i>1 then sss='s';tr monstc_.i' 'mon.whomi.1''sss
 end i
 call get_tot_wgt
 call round_check
 select
  when yourd-10>therd & tot_w<ud.13*3+1 then do;tr'You surprised the enemy!';attck.1=1;attck.0=0;surprise=1;call your_attack;end
  when therd-10>yourd then do;tr'The enemy surprised you!';attck.1=0;attck.0=1;call mon_attack;end
  otherwise nop
 end
 if done1=1 then return
 do until done1=1
  if run1=1 then return;surprise=0;call round_check;attck.1=0;attck.0=0
  select
   when yourd>therd then call your_attack
   otherwise call mon_attack
  end
 end
return

load_o_user:
 parse arg path_u
 call open(1,path_u,r);dat1=readln(1);call close(1)
 do i=1 to 35;parse var dat1 od.i','dat1;end i
return

save_o_user:
 parse arg path_u
 dat1=od.1;do i=2 to 35;dat1=dat1','od.i;end i
 call open(1,path_u,w);call writeln(1,dat1);call close(1)
return

load_user:
 do i=1 to 2;spec_cl.i=0;end i;do i=1 to 5;typeq_.i=0;end i;call open(1,path9''idnum,r);nam1=readln(1);call close(1);typeut=0
 call open(1,path7''idnum'.'nam1'_',r);dat1=readln(1);call close(1)
 do i=1 to 35;parse var dat1 ud.i','dat1;end i
 tt=ud.4;if clas_dat.tt.1='CLAS_DAT.'tt'.1' then call load_class(tt);i=0
 if ud.35~='' then do
  do i=1 until ud.35=''
   parse var ud.35 eq_.i';'ud.35
   parse var eq_.i ity'.'itid;obj_nam=ity'.'itid
   typeu_.i=0
   if left(ity,1)='*' then do
    parse var ity '*'ity;parse var eq_.i '*'eq_.i;eqid=ity;typeq_.eqid=i
   end
   if left(ity,1)='?' then do
    parse var ity '?'ity;parse var eq_.i '?'eq_.i;eqid=ity;typeu_.i=1
   end
   call get_item(ity,itid)
  end i
 end;ud.36=i;call get_spell_chart;call save_spell(0,st_hr);if study_hours='' then drop study_hours
 if exists(path'wizi/'idnum'.'nam1'_') then do;address command 'delete "'path'wizi/'idnum'.'nam1'_"';spec_cl.1=1;end
 if exists(path'heal/'idnum'.'nam1'_') then do;address command 'delete "'path'heal/'idnum'.'nam1'_"';spec_cl.2=1;end
return

load_class:
 parse arg tt
 call open(1,path2''class.tt,r);xlx=readln(1);call close(1)
 do i=1 to 18;parse var xlx clas_dat.tt.i'|'xlx;end i
return

save_user:
 if ~exists(path9''idnum) then return
 tr'Saving character...';ud.35=''
 if ud.36>0 then do
  do i=1 to ud.36
   do ii=1 to 5
    if typeq_.ii=i then eq_.i='*'eq_.i
   end
   if typeu_.i=1 then eq_.i='?'eq_.i
   if i~=ud.36 then ud.35=ud.35''eq_.i';'
   else ud.35=ud.35''eq_.i
  end i
 end
 dat1=ud.1;do i=2 to 35;dat1=dat1','ud.i;end i
 call open(1,path7''idnum'.'ud.1'_',w);call writeln(1,dat1);call close(1)
 if spec_cl.1=1 then do;call open(1,path'wizi/'idnum'.'ud.1'_',w);call writeln(1,1);call close(1);end
 if spec_cl.2=1 then do;call open(1,path'heal/'idnum'.'ud.1'_',w);call writeln(1,1);call close(1);end
return

register:
 if exists(path9''idnum) then call save_user
 ss'#0 'path5'char.reg}'
 if exists(path9''idnum) then call load_user
return

load_r:
 call open(1,path3'Races',r);do i=1 until eof(1);xlx.i=readln(1);parse var xlx.i race.i','stats;do ii=1 to 6;parse var stats stl.ii.i','sth.ii.i','stats;end ii;end i;call close(1);tot1=i-1
return

load_c:
 call open(1,path3'Classes',r);do i=1 until eof(1);xlx.i=readln(1);parse var xlx.i class.i'|'stats;do ii=1 to 3;parse var stats ali.ii.i'|'stats;end ii;do ii=1 to 6;parse var stats nst.ii.i'|'stats;end ii;end i;call close(1);tot2=i-1
return

load_t:
 path='pfiles:DL/';path1=path'text/';path2=path'char/';path3=path'data/';path4=path'info/'
 path5=path'prgm/';path6=path'mons/';path7=path'user/';path8=path'magi/';path9=path'save/';path10=path'maps/'
 def_type.1='Weapon';def_type.2='Armor';def_type.3='Shield';def_type.4='Helmet';def_type.5='Gloves';def_type.6='Item'
 stat_type.1='Strength';stat_type.2='Dexterity';stat_type.3='Intelligence';stat_type.4='Wisdom';stat_type.5='Constitution';stat_type.6='Luck'
 inn.1=1;inn.2=10;inn.3=20;inn.4=50;inn.5=100;inn_.1='The Stables';inn_.2='The Cot';inn_.3='The Economy Room';inn_.4='The Merchant Suite';inn_.5='The Royal Suite'
 spell_class.1='Mage';spell_class.2='ArchMage';spell_class.3='Cleric';spell_class.4='Warlock';spell_class.5='Illusionist';spl_ty.2='HPs';spl_ty.3='MPs';loc=1;offset1=1
 sex.1='Male';sex.2='Female';d_flag=0;spluse.0='Anywhere';spluse.1='In Battle';spluse.2='Non-Battle'
 spl_mod.2='0/0';spl_mod.3='0/0';spl_mod.4='0/0/0/0/0/0/0/0/0/0';spl_mod.5='0/0/0';spl_mod.6='0/0/0/0';spl_mod.7='0/0/0/0/0/0/0/0/0/0/0/0/0/0';spl_mod.8='0/0'
 stat_n.1='Strength';stat_n.2='Dexterity';stat_n.3='Intelligence';stat_n.4='Wisdom';stat_n.5='Constitution';stat_n.6='Luck';stat_n.7='Attack';stat_n.8='To Hit';stat_n.9='Armor Value';stat_n.10='Weapon Value'
 comotose.1.1="sleeping";comotose.2.1="confused";comotose.3.1="stunned";comotose.4.1="Poisoned";comotose.1.2="awoken";comotose.2.2="come to it's senses";comotose.3.2="come to it's senses";comotose.4.2='is cured!'
return

logplayer:
 parse arg jfk
 xlm='W';if exists(path'DL.log') then xlm='A';call open(1,path'DL.log',xlm)
 if jfk=1 then call writeln(1,handle' entered Dragon''s Lord 'ver' at 'time()' on 'date()'.')
 if jfk=2 then call writeln(1,handle' quit Dragon''s Lord 'ver' at 'time()' on 'date()'.')
 if jfk=3 then call writeln(1,handle' dropped carrier in Dragon''s Lord 'ver' at 'time()' on 'date()'.')
 call close(1)
return

gd: procedure
 parse arg var,di,cn
 x=left(var,di);x=right(x,cn)
return x

rnd: procedure
 arg lo,hi
 r=hi;if lo=hi|lo>hi then return r;d=hi-lo+1;p=d;x=length(d);n1=time('E');n1=compress(n1,'.');n1=right(n1,x);n2=time('S');n2=right(n2,x);if x>3 then p=999;r=n1+n2+random(0,p);r=r//d+lo
return r

pcr:
 ss p7'Press any key to continue!';gc;call gcc(0);ss'[1A[K'
return

out_time:
 tr'You are out of time for this turn...'
signal goodbye

gcc:
 arg typ1
 com1=result;GETCARRIER;if result="FALSE" & d_flag=0 then do;c_flag=1;call logplayer(3);signal goodbye;end;if time('e')>secs then signal out_time
 if typ1=2 then do;if datatype(com1)~='NUM' then com1=0;com1=com1%1;end
 if typ1=1 then tr p7''com1
 if typ1=0 then tr
RETURN

SYNTAX:;ERROR:;IOERR:;e1=" Error: "rc" ("errortext(rc)")";e2="  Line: "
 e2=e2||left(sigl,4)"File:";c="`"fp", "ver"'";e2=e2" "c;tr "n1"e1;tr e2
 logentry e1;logentry e2;e=strip(translate(sourceline(sigl),"\{",""));e4=e
 do while e~="";e3="Source: "left(e,37);tr e3;logentry e3;e=substr(e,38);end
 e_flag=1
signal goodbye

cleanup:
 if f_flag=1 then call permit
return

goodbye:
 if e_flag~=1 then call save_user
 if c_flag~=1 then call logplayer(2)
 call cleanup
 bufferflush
exit
