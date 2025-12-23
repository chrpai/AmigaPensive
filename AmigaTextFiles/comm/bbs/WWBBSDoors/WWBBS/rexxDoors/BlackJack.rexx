/*                     Black Jack Version 2.0
 This game is written specially to run on BBBBS, the BBS software
 package written by Richard Stockton. This game was written by
 Matt English on 5-7-92. You have my premission to use this game
 on your BBS as long as you leave my name in the code as original writer
 and you don't make any cash from this game (in any way).
 This is version 1.0. I have other ideas for this game that I will put in
 future versions (if I get the time). To run the game, just put this file
 in your Rexxdoors directory. Hope you like it.

                              6-21-92
 Version 2.0 now includes the ability to play from INSIDE of Chicago 1.3!
 Version 2.0 wil also run fine by itself, without Chicago 1.3, but it's
 more fun that way!*/

options results

FF='0C'x  /* FormFeed */
CR='0D'x  /* Carraige Return */
CALL TIME('R')
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

PARSE arg Username winnings . colorflag secs .
IF secs='' THEN secs=3600
signal on syntax

BBSIDENTIFY EMULATION
PARSE VAR RESULT type .
if type = "ANSI" then
  colorflag=1
else
  colorflag=0

BBSIDENTIFY USER
PARSE VAR RESULT Username From Acces .
Username = substr(Username,2,length(Username)-2)

if ~show('L','rexxsupport.library') then do
  addlib('rexxsupport.library',0,-30,0)
  end

i = random(1,999,time('S')) /* initalize random number generator */


def=''
pen2=''
pen3=''
pen4=''
pen5=''
pen6=''
pen7=''
bak2=''
bak3=''
bak4=''
bak5=''
bak6=''
bak7=''
unln=''
CLS = ''

CHICCOLOR = GetClip('CHICCOLOR')

IF colorflag=1 | CHICCOLOR = 1 THEN
  DO
    def='[0m'
    pen2='[32m'
    pen3='[33m'
    pen4='[34m'
    pen5='[35m'
    pen6='[36m'
    pen7='[37m'
    bak2='[42m'
    bak3='[43m'
    bak4='[44m'
    bak5='[45m'
    bak6='[46m'
    bak7='[47m'
    unln='[4m'
  END
  CLS = ''

/*This checks to see if the game has been called from Chicago 1.3
and makes the proper adjustments.*/

chicplay = GetClip('CHICMON')
if chicplay ~='' then do
   avc = chicplay
   chicgame = 1
   secs=360  /* 6 minute limit from Chicago */
   call play
   end

/*-----------------MAIN SECTION----------------------*/

START:
avc = 500
say ''cls''cr
say ''cr
call header
say ''cr
call sac70'Welcome to BlackJack!'
say ''cr
call sac70'Version 2.0  6-21-92'
say ''cr
call sac70'Written by Matt English'
say ''cr
call sac70'SYSOP - The Alternative BBS - Portland Oregon'
call sac70'503-761-3043'
say ''cr
say ''cr
say ''cr
say ''cr
say ''cr
call pressreturn
say ''cr

Play:
    CALL checkBBS()
    cc = 0
    dc = 0
    h1.1='';h1.2='';h1.3='';h1.4='';h1.5='';h1.6='';h1.7='';h1.8='' /* Player hand 1 */
    h2.1='';h2.2='';h2.3='';h2.4='';h2.5='';h2.6='';h2.7='';h2.8=''/* Player hand 2 */
    d.1='';d.2='';d.3='';d.4='';d.5='';d.6='';d.7='';d.8=''       /*Dealer hand */
    say ''cr
    if avc < 1 then do
     call sac70'Your available funds seem to have dropped below the minimum bet!'
     say ''cr
     prompt('       Would you like to play again? (y,n) >')
     answer=readstr()
      if answer = Y then do
       avc = 500
       say ''cr
       call sac70'OK, let''s try it again!'
       say ''cr
       end
      else call QUIT
     end
  /*  prompt(' How much would you like to bet? (Q = Quit) >')
    choice=readstr()
    choice = strip(choice)   */

   call bet

   avc = (avc - abet)
    say ''cr
    say ''cr
    ptot1 = 0    /* Player total card points hand 1 */
    ptot2 = 0    /* Player total card points hand 2 */
    dtot = 0    /* Dealer total card points */
    busted = 0
    busted1 = 0
    busted2 = 0
    dlbusted = 0
    twone = 0
    twone1 = 0
    twone2 = 0
    bj1 = 0
    bj2 = 0
    acecount = 0
    call Play1

PLAY1:

  say ''cr
  say ''cr
  do i = 1 to 2
    call getcard
    h1.i = RCD
  end
  do i = 1 to 2
    call getcard
    d.i = RCD
  end
  BJ = 0  /* BlackJack if = 1 */
  cc = 2  /* card count */
  dc = 2  /* dealers card count */
  splh = 0    /* This hand is a split hand  if 1 */
  splm = 0   /* display the Split menu  if 1 */
  ddnm = 0   /* Display the DD menu if 1 */
  ddnh = 0    /* This hand is Double Down'ed if 1 */

  call play2

PLAY2:
  if chicgame ~=1 then call header
  say ''cr
  say ''cr
  say'     Amount bet > $'abet'      Your available cash > $'avc''cr
  say ''cr

    say' 'bak6'Your Hand >'def' 'pen6''h1.1' 'h1.2' 'h1.3' 'h1.4' 'h1.5' 'h1.6' 'h1.7' 'h1.8''def''cr
   call CPTOT h1 splh
   say ''cr
   say' 'bak6'Your total >'def' 'pen6''ptot1''def''cr
   say ''cr

  say' 'bak2'Dealers Hand >'def''pen2' # 'd.2' 'd.3' 'd.4' 'd.5' 'd.6' 'd.7' 'd.8''def''cr
  say ''cr
  say' 'bak2'Dealers total >'def''pen2' ?'def''cr
  say ''cr
  say ''cr
  if cc = 2 & ptot1 = 21 then call BLACKJACK
  say '             1.Draw a card    (Dealer must stand on 17, hit on 16)'cr
  say '             2.Stand'cr
   if cc = 2 & h1.1 = h1.2 then splm = 1
   else splm = 0
   if splm = 1 then do
   if abet > avc then do
    say '  You don''t have enough cash to Split this hand!'cr
    splm = 0
    end
   else say '             3.Split'cr
   end
  if cc = 2 then call checkdd
  say ''cr
  if chicgame = 1 then quitopt = ''
  else quitopt = '(Q = Quit)'
  prompt( ' Your choice ? 'quitopt' > ')
  ans=readstr()
   select
     when ans = 'Q' then do
      if chicgame ~=1 then call QUIT
      else call PLAY2
      end
     when ans = 1 then do
      h1 = 1
      call DRAW h1 splh
      end
     when ans = 2 then do
      h1 = 1
      call STAND h1 splh
      end
     when ans = 3 then do
      select
       when splm = 1 then call SPLIT
       when splm = 0 & ddnm = 1 then call DDOWN
       otherwise call PLAY2
       end
      end
     when ans = 4 then do
      if splm = 1 & ddnm = 1 then call DDOWN
       else call PLAY2
      end
     otherwise call PLAY2
     end

/*---------------------ROUTINES-----------------------*/

DDOWN:
   say ''cr
   ddbet = abet + abet
   avc = avc - abet
   abet = ddbet
   call sac70'OK, we''ll double your bet to $'ddbet','
   call sac70'and you''ll get one card.'
   say ''cr
   call sac70'Your available cash is now $'avc''
   call delay(50)
   say ''cr
   call getcard
   h1.3 = rcd
   whatcard = h1
   call cptot whatcard splh
   call sac70'You draw your one card and get'
   say ''cr
   select
    when rcd = A then pre = 'an'
    when rcd = 8 then pre = 'an'
    otherwise pre = 'a'
    end
   call sac70''pre' 'h1.3''
   say ''cr
   say'             'bak6'Your cards :'def' 'pen6''h1.1' 'h1.2' 'h1.3''def'   'bak6'Your total :'def' 'pen6''ptot1''def''cr
   say ''cr
   call pressreturn
   say ''cr
   call sac70'Ok, let''s see if the Dealer can beat that!'
   say ''cr
   call dlrturn

SPLIT:
 splh = 1 /* This is a split hand */
 say ''cr
 call sac70'OK, let''s split your 'pen6''h1.1''def'''s into two hands.'
 call sac70''def'You''ll be playing for $'abet' on 'pen3'EACH hand!'def''
 avc = avc - abet
 totwager = abet * 2
 say ''cr
 h1done = 0   /* signals when hand 1 is done and move to hand 2 */
 cch1 = 1
 cch2 = 1
 busted1 = 0
 busted2 = 0
 twone1 = 0
 h1winner = 0
 h2winner = 0
 select
 when h1.1 = 'A' then ptot1 = 11
 when h1.1 = 'K' then ptot1 = 10
 when h1.1 = 'Q' then ptot1 = 10
 when h1.1 = 'J' then ptot1 = 10
 otherwise ptot1 = h1.1
 end
 select
 when h1.2 = 'A' then ptot2 = 11
 when h1.2 = 'K' then ptot2 = 10
 when h1.2 = 'Q' then ptot2 = 10
 when h1.2 = 'J' then ptot2 = 10
 otherwise ptot2 = h1.2
 end
 call pressreturn
 say ''cr
 call SPLIT1

SPLIT1:

 if chicgame ~=1 then call header
 say ''cr
 say'   Amount bet > $'abet' per hand.    Your available cash > $'avc''cr
 say ''cr
 say' 'bak6'Your Hand # 1 >'def' 'pen6''h1.1' 'h1.3' 'h1.4' 'h1.5' 'h1.6' 'h1.7' 'h1.8''def''cr
 say ''cr
 if busted1 = 1 then isitbusted = ''pen3'BUSTED!'def''
 if busted1 = 0 then isitbusted = ''
 say' 'bak6'Your Total # 1 >'def' 'pen6''ptot1'  'isitbusted''cr
 say ''cr
 say' 'bak5'Your Hand # 2 >'def' 'pen5''h1.2' 'h2.2' 'h2.3' 'h2.4' 'h2.5' 'h2.6' 'h2.7' 'h2.8''def''cr
 say ''cr
 say' 'bak5'Your Total # 2 >'def' 'pen5''ptot2''def''cr
 say ''cr
 say' 'bak2'Dealers Hand >'def''pen2' # 'd.2' 'd.3' 'd.4' 'd.5' 'd.6' 'd.7' 'd.8''def''cr
 say ''cr
 if busted1 = 1 then h1done = 1
 if h1done = 0 then hnum = ''pen6'Hand # 1'def''
 if h1done = 1 then hnum = ''pen5'Hand # 2'def''
 say' 'bak2'Dealers total >'def''pen2' ?'def'      'bak4'Currently playing'def' 'hnum''cr
 say ''cr
  say '             1.Draw a card    (Dealer must stand on 17, hit on 16)'cr
  say '             2.Stand'cr
  say ''cr
  if chicgame = 1 then quitopt = ''
  else quitopt = '(Q = Quit)'
  prompt( ' Your choice ? 'quitopt' > ')

  ans=readstr()
   if h1done = 0 then whatcard = h1
   if h1done = 1 then whatcard = h2
   select
     when ans = 'Q' then do
      if chicgame = 1 then call SPLIT1
      else call QUIT
      end
     when ans = 1 then call DRAW whatcard splh
     when ans = 2 then call STAND whatcard splh
     otherwise call SPLIT1
     end

BLACKJACK:
 call delay(175)
 call sac70''bak4'BLACKJACK !'def''
 say ''cr
 call sac70''bak4'BLACKJACK !'def''
 say ''cr
 call sac70''bak4'BLACKJACK !'def''
 say ''cr
 call sac70''bak4'BLACKJACK !'def''
 say ''cr
 call sac70''pen3'You win double your bet!'def''
 say ''cr
 amwon = abet*2
 call sac70'$'amwon''
 avc = amwon + abet  + avc
 amwon = 0
 if chicgame = 1 then call chicquit
 else call PLAY


STAND:
arg whatcard splh
if whatcard = h1 & splh = 0 then do
  say ''cr
  call sac70'OK, you''ll stand on 'ptot1''
  say ''cr
  call sac70'Now it''s the Dealers turn'
  say''cr
  call delay(75)
  call dlrturn
 end

if whatcard = h1 & splh = 1 then do
  say ''cr
  call sac70''def'OK, you''ll stand on 'ptot1''def''
  say ''cr
  call sac70'Now it''s time to play 'pen5'Hand # 2'def''
  say''cr
  call delay(75)
  h1done = 1
  call SPLIT1
 end

if whatcard = h2 & splh = 1 then do
  say ''cr
  call sac70'OK, you''ll stand on 'ptot2''
  say ''cr
  call sac70'Now it''s the Dealers turn.'
  say''cr
  call delay(75)
  call dlrturnspl
 end

dlrturn:
 say ''cr
  select
   when d.1 = A then pre = 'an'
   when d.1 = 8 then pre = 'an'
   otherwise pre = 'a'
   end
   call sac70'The Dealer turns over his hole card to reveal 'pre' 'd.1''
   say '' cr
   say'             'bak2'The Dealers cards :'def''pen2' 'd.1' 'd.2' 'd.3' 'd.4' 'd.5' 'd.6' 'd.7' 'd.8''def''cr
   call cptot d
   say'             'bak2'The Dealers total :'def''pen2' 'dtot''def'   'bak6'Your total'def' 'pen6''ptot1''def''cr
   say ''cr
   call pressreturn
   if dtot < 22 & dtot > ptot1 & dtot > 16 then call loser
   if dtot < 17 then do
    do until dtot > 16
     call getcard
     if dc = 2 then d.3 = RCD
     if dc = 3 then d.4 = RCD
     if dc = 4 then d.5 = RCD
     if dc = 5 then d.6 = RCD
     dc = dc +1
     say ''cr
     select
      when rcd = A then pre = 'an'
      when rcd = 8 then pre = 'an'
      otherwise pre = 'a'
      end
     call sac70'The Dealer draws 'pre' 'rcd''
     say ''cr
     say'             'bak2'The Dealers cards :'def''pen2' 'd.1' 'd.2' 'd.3' 'd.4' 'd.5' 'd.6' 'd.7' 'd.8''def''cr
     call cptot d
     say'             'bak2'The Dealers total :'def''pen2' 'dtot''def'   'bak6'Your total:'def' 'pen6''ptot1''def''cr
     say ''cr
      call pressreturn
     if dtot > 21 then do
      say ''cr
       say'                        The Dealer has 'pen3'BUSTED !'def''cr
      say ''cr
      call winner
     end
    end
   end
   if dtot < ptot1 then call winner
   if dtot = ptot1 then call push
   if dtot < 22 & dtot > ptot1 & dtot > 16 then call loser

DLRTURNSPL:
 say ''cr
  select
   when d.1 = A then pre = 'an'
   when d.1 = 8 then pre = 'an'
   otherwise pre = 'a'
   end
   call sac70'The Dealer turns over his hole card to reveal 'pre' 'd.1''
   say '' cr
   say'             'bak2'The Dealers cards :'def''pen2' 'd.1' 'd.2' 'd.3' 'd.4' 'd.5' 'd.6' 'd.7' 'd.8''def''cr
   call cptot d
   say'             'bak2'The Dealers total :'def''pen2' 'dtot''def''cr
   say''cr
   say'             'bak6'Your total Hand # 1 :'def' 'pen6''ptot1'   'bak5'Your total Hand # 2 :'def' 'pen5''ptot2''def''cr
   say ''cr
   call pressreturn
   if dtot < 22 & dtot >  16 then call CALCSPL
   if dtot < 17 then do
    do until dtot > 16
     call getcard
     if dc = 2 then d.3 = RCD
     if dc = 3 then d.4 = RCD
     if dc = 4 then d.5 = RCD
     if dc = 5 then d.6 = RCD
     if dc = 6 then d.7 = RCD
     if dc = 7 then d.8 = RCD
     dc = dc +1
     say ''cr
     select
      when rcd = A then pre = 'an'
      when rcd = 8 then pre = 'an'
      otherwise pre = 'a'
      end
     call sac70'The Dealer draws 'pre' 'rcd''
     say ''cr
     say'             'bak2'The Dealers cards :'def''pen2' 'd.1' 'd.2' 'd.3' 'd.4' 'd.5' 'd.6' 'd.7' 'd.8''def''cr
     call cptot d
     say'             'bak2'The Dealers total :'def''pen2' 'dtot''def''cr
   say''cr
     say'             'bak6'Your total Hand # 1 :'def' 'pen6''ptot1' 'pen3''isitbusted'   'bak5'Your total Hand # 2 :'def' 'pen5''ptot2''def''cr
     say ''cr
     call pressreturn
     if dtot > 21 then do
      dlbusted = 1
      say ''cr
      say'                          The Dealer has 'pen3'BUSTED!'def''cr
      say ''cr
      call calcspl
     end
    end
   end
   call calcspl

CALCSPL:  /* Calculate split wins. */
    if dlbusted = 1 then do
     if bj1 = 0 then do
      if busted1 = 0 then do
       say'                        'pen6'Your Hand # 1 is a winner!'def''cr
       say'                            'pen6'Hand # 1 pays $'abet''def''cr
       say ''cr
       avc = abet + abet + avc
       end
      else do
       say'                        'pen6'Your Hand # 1 is 'pen3'BUSTED.'def''cr
       say'                           'pen6'Hand # 1 'pen3'loses 'pen6'$'abet''def''cr
       say ''cr
       end
      end
     else do
        say'                       'pen6'Your Hand # 1 is a 'def''bak6'BLACKJACK!'def''cr
        say'                           'pen6'Hand # 1 pays 'def''bak6'2 'def''pen6'times your bet!'def''cr
        bjwin = abet * 2
        call sac70'$'bjwin''
        avc = avc + abet + bjwin
        bjwin = 0
        say ''cr
        end

     if bj2 = 0 then do
      if busted2 = 0 then do
       say'                        'pen5'Your Hand # 2 is a 'def''bak5'winner!'def''cr
       say'                            'pen5'Hand # 2 pays 'def''bak5'$'abet''def''cr
       say ''cr
       avc = abet + abet + avc
       end
      else do
       say'                        'def''pen5'Your Hand # 2 is 'pen3'BUSTED.'def''cr
       say'                                'pen5'Hand # 2 'pen3'loses 'pen5'$'abet''def''cr
       say ''cr
       end
      end
     else do
       say'                        'pen5'Your Hand # 2 is a 'def''bak5'BLACKJACK!'def''def''cr
       say'                            'pen5'Hand # 2 pays 'def''bak5'2 'def''pen5'times your bet!'def''cr
        bjwin = abet * 2
        call sac70'$'bjwin''
        avc = avc + abet + bjwin
        bjwin = 0
        say ''cr
        end
       if chicgame = 1 then call chicquit
       else call play
      end
    if dtot > ptot1 & dlbusted = 0 then do
       say'                        'pen6'Your Hand # 1 is a 'pen3'loser!'def''cr
       say'                            'pen6'Hand # 1 'pen3'loses 'pen6'$'abet''def''cr
     say ''cr
    end
    if dtot < ptot1 & busted1 = 0 & bj1 = 0 then do
       say'                        'pen6'Your Hand # 1 is a winner!'def''cr
       say'                            'pen6'Hand # 1 wins $'abet''def''cr
     say ''cr
     avc = avc + (abet * 2)
    end
    if bj1 = 1 then do
       say'                        'pen6'Your Hand # 1 is a 'def''bak4'BLACKJACK!'def''cr
       say'                            'pen6'Hand # 1 wins 2 times your bet!'def''cr
     bjwin = abet * 2
     call sac70'$'bjwin''
     say ''cr
     avc = avc + abet + bjwin
     bj1 = 0
     bjwin = 0
     end
    if dtot = ptot1 & busted1 = 0 then do /* Means this is a push. */
       say'                    Your Hand # 1 is a push!'cr
       say'              Your bet will be returned for Hand # 1.'cr
     say ''cr
     avc = avc + abet
    end
    if busted1 = 1 then do
       say'                        'pen6'Your Hand # 1 is 'pen3'BUSTED!'def''cr
       say'                            'pen6'Hand # 1 'pen3'loses $'abet'!'def''cr
     say ''cr
     end
    if dtot > ptot2 & dlbusted = 0 then do
       say'                        'pen5'Your Hand # 2 is a 'pen3'loser!'def''cr
       say'                            'pen5'Hand # 2 loses 'pen3'$'abet''def''cr
     say ''cr
    end
    if dtot < ptot2 & busted2 = 0 then do
       say'                        'pen5'Your Hand # 2 is a winner!'def''cr
       say'                            'pen5'Hand # 2 wins $'abet''def''cr
     say ''cr
     avc = avc + (abet * 2)
    end
    if bj2 = 1 then do
       say'                        'pen5'Your Hand # 2 is a 'def''bak2'BLACKJACK!'def''cr
       say'                            'pen5'Hand # 2 wins 2 times your bet!'def''cr
     bjwin = abet * 2
     call sac70'$'bjwin''
     say ''cr
     avc = avc + abet + bjwin
     bj2 = 0
     bjwin = 0
     end
    if dtot = ptot2 then do /* Means this is a push */
       say'                         'pen5'Your Hand # 2 is a push!'def''cr
       say'                 'pen5'Your bet will be returned for Hand # 2.'def''cr
     say ''cr
     avc = avc + abet
    end
    if busted2 = 1 then do
       say'                        'pen5'Your Hand # 2 is 'pen3'BUSTED!'def''cr
       say'                           'pen5'Hand # 2 'pen3'loses $'abet'!'def''cr
     say ''cr
     end

if chicgame = 1 then call chicquit
else call play

WINNER:
 say ''cr
 say'                   'bak2'Congratulations, you win this hand!'def''cr

   avc = (abet * 2) + avc
   say ''cr
   say'                          'pen2'You have just won $'abet'!'def''cr
   say''cr
   say ''cr
 if chicgame = 1 then call chicquit
 else call PLAY

LOSER:
 say ''cr
 say'                        'bak3'Bummer!'def''pen3' You lose this hand.'def''cr
 say ''cr
 say'                             'def''pen3'You just lost $'abet'!'def''def''cr
 if chicgame = 1 then call chicquit
 else call PLAY

PUSH:
 say ''cr
 call sac70'You and the Dealer tie!'
 say ''cr
 call sac70'You don''t win, you don''t lose.'
 say ''cr
 call sac70'Your bet is returned!'
 avc = abet + avc
 if chicgame = 1 then call chicquit
 else call PLAY


DRAW:
arg whatcard splh
call getcard
if whatcard = h1 & splh = 0 then do
  if cc = 2 then h1.3 = rcd
  if cc = 3 then h1.4 = rcd
  if cc = 4 then h1.5 = rcd
  if cc = 5 then h1.6 = rcd
  if cc = 6 then h1.7 = rcd
  if cc = 7 then h1.8 = rcd
  cc = cc + 1
  say ''cr
  call sac70'You draw a card and get'
  call delay(100)
  say ''cr
    select
   when rcd = A then pre = 'an'
   when rcd = 8 then pre = 'an'
   otherwise pre = 'a'
   end
  call sac70''pre' 'rcd''
  say ''cr
  call delay(100)
call cptot h1 splh
call chk21 h1 splh
if twone = 1 then call dlrturn  /* if total is 21 Dealers turn */
call chkbust h1 splh
if busted = 1 then do
 if chicgame = 1 then call chicquit
 else call Play
 end
call play2
end


if whatcard = h1 & splh = 1 then do
  if cch1 = 1 then h1.3 = rcd
  if cch1 = 2 then h1.4 = rcd
  if cch1 = 3 then h1.5 = rcd
  if cch1 = 4 then h1.6 = rcd
  if cch1 = 5 then h1.7 = rcd
  if cch1 = 6 then h1.8 = rcd
  cch1 = cch1 + 1
  say ''cr
  call sac70'You draw a card and get'
  call delay(100)
  say ''cr
  select
   when rcd = A then pre = 'an'
   when rcd = 8 then pre = 'an'
   otherwise pre = 'a'
   end
  call sac70''pre' 'rcd''
  say ''cr
  call delay(100)
call cptot h1 splh
call chk21 h1 splh
if twone1 = 1 then do
 h1done = 1
 call SPLIT1
 end
call chkbust h1 splh
if busted1 = 1 then do
 h1done = 1
 call SPLIT1
 end
call split1
end

if whatcard = h2 & splh = 1 then do
  if cch2 = 1 then h2.2 = rcd
  if cch2 = 2 then h2.3 = rcd
  if cch2 = 3 then h2.4 = rcd
  if cch2 = 4 then h2.5 = rcd
  if cch2 = 5 then h2.6 = rcd
  if cch2 = 6 then h2.7 = rcd
  cch2 = cch2 + 1
  say ''cr
  call sac70'You draw a card and get'
  call delay(100)
  say ''cr
  select
   when rcd = A then pre = 'an'
   when rcd = 8 then pre = 'an'
   otherwise pre = 'a'
   end
  call sac70''pre' 'rcd''
  say ''cr
  call delay(100)
call cptot h2 splh
call chk21 h2 splh
if twone2 = 1 then do
 call dlrturnspl
 end
call chkbust h2 splh
if busted2 = 1 then do
 if busted1 = 1 then do
  say ''cr
  call sac70'Both of your Hands are 'pen3'Busted!'def''
  call sac70'You lose your entire wager, 'pen3'$'totwager'!'def''
  if chicgame = 1 then call chicquit
  else call PLAY
  end
 else call dlrturnspl
 end
call split1
end


chkbust:
arg whatcard splh
select
  when whatcard = h1 & splh = 0 then do
   if ptot1 > 21 then do
    say ''cr
    say'                              Your total is 'ptot1''cr
    say ''cr
    say'                                  'pen3'BUSTED!'def''cr
    say ''cr
    say'                              You just lost 'pen3'$'abet''def''cr
    busted = 1
    say ''cr
    call pressreturn
    say ''cr
   end
  end

when whatcard = h1 & splh = 1 then do
   if ptot1 > 21 then do
    say ''cr
    say'                                 Your total is 'ptot1''cr
    say ''cr
    say'                                   'pen3'BUSTED!'def''cr
    busted1 = 1
    say ''cr
    call sac70'Now let''s play Hand # 2'
    say ''cr
    call pressreturn
    say ''cr
   end
  end

  when whatcard = h2 & splh = 1 then do
   if ptot2 > 21 then do
    say ''cr
    say'                                 Your total is 'ptot2''cr
    say ''cr
    say'                                   'pen3'BUSTED!'def''cr
    busted2 = 1
    say ''cr
    call pressreturn
    say ''cr
   end
  end
END
return


chk21:
arg whatcard splh
  if whatcard = h1 & splh = 0 then do
   if ptot1 = 21 then do
    say ''cr
    call sac70'Your total is 21!'
    say ''cr
    call sac70'Now it''s the Dealers turn.'
    twone = 1
   end
  end

  if whatcard = h1 & splh = 1 then do
   if ptot1 = 21 then do
    say ''cr
    call sac70'Your total is 21!'
    say ''cr
    call sac70'Now let''s play Hand # 2.'
    call delay(75)
    twone1 = 1
   end
  end

  if whatcard = h2 & splh = 1 then do
   if ptot2 = 21 then do
    say ''cr
    call sac70'Your total is 21!'
    say ''cr
    call sac70'Now let''s see what kinda luck the Dealer has.'
    twone2 = 1
   end
  end

return

CHECKDD:

   if h1.1 = '10' then Return
   if h1.1 = 'J' then Return
   if h1.1 = 'Q' then Return
   if h1.1 = 'K' then Return
   if h1.1 = 'A' then Return

   if h1.2 = '10' then Return
   if h1.2 = 'J' then Return
   if h1.2 = 'Q' then Return
   if h1.2 = 'K' then Return
   if h1.2 = 'A' then Return

   if cc = 2 & h1.1 + h1.2 > 9 & h1.1 + h1.2 < 12 then do
    if abet > avc then do
     say '    You don''t have enough cash to Double Down.'cr
     return
     end
    if splm = 1 then say '             4.Double Down'cr
    else say '             3.Double Down'cr
    ddnm = 1  /* This says display Double Down menu */
   end

Return

CPTOT:

arg whatcard splh
acecount = 0
  if whatcard = h1 & splh = 0 then do
     do i = 1 to 8
      if h1.i = '' then t.i = 0
      if h1.i = 'J' then t.i = 10
      if h1.i = 'Q' then t.i = 10
      if h1.i = 'K' then t.i = 10
      if h1.i = 'A' then do
       acecount = acecount + 1
       t.i = 0
       end
      if h1.i > 1 & h1.i < 11 then t.i = h1.i
      end
      tptot = t.1 + t.2 + t.3 + t.4 + t.5 + t.6 + t.7 + t.8   /* temp point total*/
      if acecount = 1 then do
       if tptot < 11 then tptot = tptot + 11
       else tptot = tptot + 1
       end
      if acecount = 2 then do
       if tptot < 10 then tptot = tptot + 12
       else tptot = tptot + 2
       end
      if acecount = 3 then do
       if tptot < 9 then tptot = tptot + 13
       else tptot = tptot + 3
       end
      if acecount = 4 then do
       if tptot < 8 then tptot = tptot + 14
       else tptot = tptot + 4
       end
      ptot1 = tptot
      acecount = 0
      tptot = 0
     end
  if whatcard = d then do
     do i = 1 to 8
      if d.i = '' then t.i = 0
      if d.i = 'J' then t.i = 10
      if d.i = 'Q' then t.i = 10
      if d.i = 'K' then t.i = 10
      if d.i = 'A' then do
       acecount = acecount + 1
       t.i = 0
       end
      if d.i > 1 & d.i < 11 then t.i = d.i
      end
      tptot = t.1 + t.2 + t.3 + t.4 + t.5 + t.6 + t.7 + t.8   /* temp point total*/
      if acecount = 1 then do
       if tptot < 11 then tptot = tptot + 11
       else tptot = tptot + 1
       end
      if acecount = 2 then do
       if tptot < 10 then tptot = tptot + 12
       else tptot = tptot + 2
       end
      if acecount = 3 then do
       if tptot < 9 then tptot = tptot + 13
       else tptot = tptot + 3
       end
      if acecount = 4 then do
       if tptot < 8 then tptot = tptot + 14
       else tptot = tptot + 4
       end
      dtot = tptot
      acecount = 0
      tptot = 0
     end

  if whatcard = h1 & splh = 1 then do
     do i = 1 to 8
      if h1.i = '' then t.i = 0
      if h1.i = 'J' then t.i = 10
      if h1.i = 'Q' then t.i = 10
      if h1.i = 'K' then t.i = 10
      if h1.i = 'A' then do
       acecount = acecount + 1
       t.i = 0
       end
      if h1.i > 1 & h1.i < 11 then t.i = h1.i
      end
      tptot = t.1 + t.3 + t.4 + t.5 + t.6 + t.7 + t.8   /* temp point total*/
      if h1.2 = 'A' then acecount = acecount - 1
      if acecount = 1 then do
       if tptot < 11 then tptot = tptot + 11
       else tptot = tptot + 1
       end
      if acecount = 2 then do
       if tptot < 10 then tptot = tptot + 12
       else tptot = tptot + 2
       end
      if acecount = 3 then do
       if tptot < 9 then tptot = tptot + 13
       else tptot = tptot + 3
       end
      if acecount = 4 then do
       if tptot < 8 then tptot = tptot + 14
       else tptot = tptot + 4
       end
      ptot1 = tptot
      acecount = 0
      tptot = 0
     end

  if whatcard = h2 & splh = 1 then do
     do i = 1 to 8
      if h2.i = '' then t.i = 0
      if h2.i = 'J' then t.i = 10
      if h2.i = 'Q' then t.i = 10
      if h2.i = 'K' then t.i = 10
      if h2.i = 'A' then do
       acecount = acecount + 1
       t.i = 0
       end
      if h2.i > 1 & h2.i < 11 then t.i = h2.i
      end
      tptot = t.2 + t.3 + t.4 + t.5 + t.6 + t.7 + t.8   /* temp point total*/
      if h1.2 > 1 & h1.2 < 11 then tptot = tptot + h1.2
      if h1.2 = 'J' then tptot = tptot + 10
      if h1.2 = 'Q' then tptot = tptot + 10
      if h1.2 = 'K' then tptot = tptot + 10
      if h1.2 = 'A' then do
       if tptot < 11 then tptot = tptot + 11
       else tptot = tptot + 1
       end
      if acecount = 1 then do
       if tptot < 11 then tptot = tptot + 11
       else tptot = tptot + 1
       end
      if acecount = 2 then do
       if tptot < 10 then tptot = tptot + 12
       else tptot = tptot + 2
       end
      if acecount = 3 then do
       if tptot < 9 then tptot = tptot + 13
       else tptot = tptot + 3
       end
      if acecount = 4 then do
       if tptot < 8 then tptot = tptot + 14
       else tptot = tptot + 4
       end
      ptot2 = tptot
      acecount = 0
      tptot = 0
     end

Return

getcard:
   RND = RANDOM(1,13)
    if RND > 1 & RND < 11 then do
      RCD = RND
      end
    if RND = 1 then RCD = 'A'
    if RND = 11 then RCD = 'J'
    if RND = 12 then RCD = 'Q'
    if RND = 13 then RCD = 'K'
Return


bet:
   say ''cr
   say ''cr
   say ' You have $'avc' available'cr
   say ''cr
   prompt( '  How much would you like to bet? 'pen3'$500 max. 'def'(Q = Quit) > $')
   abet=readstr()
   abet = strip(abet)
   if abet = 'Q' then do
    if chicgame = 1 then do
     say ''cr
     say'  Sorry Bud, you can''t sit down and then refuse to bet!'cr
     say'  Now bet something! Or should I get some muscle over here?'cr
     say ''cr
     say ''cr
     call bet
     end
    else call quit
   end
   if abet = ''| abet = 0 then do
    say''cr
    say'    OK Buddy, if your not going to bet,'cr
    say'    we''re going to have to ask you to'cr
    say'    leave so we can seat another player!'cr
    call bet
   end
   if datatype(abet,W) ~= 1 then do
    say ''cr
    say '   The Dealer glares at you and says'cr
    say '   "We have rules at this table Buddy!'cr
    say '   You can only bet 'pen3'WHOLE'def' dollars'cr
    say '   and Your ABC''s aren''t worth anything here!"'cr
    say ''cr
    abet = ''
    call BET
    end
   if abet > 500 then do
    say ''cr
    say' Sorry pal, there''s a $500 limit at this table!'cr
    say' How ''bout betting a little less...'cr
    abet = ''
    call bet
    end
   if abet > avc then do
     say' Sorry, You can''t bet more than you have!'cr
     say' We do not extend credit at this establishment!'cr
     say' Please try again.'cr
     abet = ''
     call BET
     end


   if abet = '' then call bet
   say ''cr
   call delay(25)
Return

QUIT:
  say ''cr
  say'  Thanks for playing BlackJack! Come back soon!'cr
  say'  Returning now to the BBS.....'cr
  exit

chicquit:
  call setclip('CHICMON',avc)
  say ''cr
  say ''cr
  say' Thanks for trying BlackJack, come back any time (that ya got cash).'cr
  say ''cr
  exit

SAC70:  /* SAY CENTER TEXT*/
PARSE ARG string
SAY CENTER(string,75)||CR
RETURN

SAC110:  /* SAY CENTER TEXT*/
PARSE ARG string
SAY CENTER(string,110)||CR
RETURN

HEADER:
say'          'pen3'----------------------------------------------------------'cr
say'          'pen3'| 'pen5'BlackJack'pen6'BlackJack'pen5'BlackJack'pen6'BlackJack'pen5'BlackJack'pen6'BlackJack 'pen3'|'cr
say'          'pen3'| 'pen6'Jack'pen5'BlackJack'pen6'BlackJack'pen5'BlackJack'pen6'BlackJack'pen5'BlackJack'pen6'Black 'pen3'|'cr
say'          'pen3'----------------------------------------------------------'def''cr
return

pressreturn:
SAY' 'pen3' Press return to continue'def''
out=readch(STDIN)
return

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

SYNTAX:
   SAY '*BOOM*  Got a syntax error.  PLEASE notify the sysop.'
   SAY 'Line:' SIGL '   Error Code:' RC
exit


checkBBS:
IF ADDRESS()~='BAUD' THEN RETURN 0
IF TIME('e')>secs THEN SIGNAL QUIT
dcd
IF RC=0 THEN EXIT
temp=secs-TIME('E')
IF temp<120 THEN SAY '*** Only' temp 'seconds left! ***'CR 
RETURN 0


BREAK_C:
BREAK_E:
say'BREAK * BREAK * BREAK *'cr
say''cr
if chicgame = 1 then call chicquit
call quit
exit
