/* 
$VER: UserLast.rexx 2.0  (28 Sep. 1999) by Gnome
*/

/*  Utility to read or write date of last call to user record.

New in version 1.05
 Precaution in "Plant:" for when no user record exists (guest called).
New in this version 2.00
 Rewrite userlast date as 6 digits (not 8)

arguments: <bbsline>:
           'r:<username>' to read, 
or         'w:<username>' to write current date as yyyymmdd e.g. 20010625

e.g. call 'bbs:utils/userlast.rexx 1:w:John Marchant'

Note that the arguments are separated by colon ':' since the name will
have a space in it, so the usual space separator is unsuitable.

If reading, this program writes date of last call to ENV:UserLast

===================================================================*/

options results

/*
call open('STDERR','ram:tracefile','w')
trace results
*/

parse arg bbsline':'mode':'username

mnth.01='Jan'; mnth.02='Feb'; mnth.03='Mar'; mnth.04='Apr'; mnth.05='May'; mnth.06='Jun'
mnth.07='Jul'; mnth.08='Aug'; mnth.09='Sep'; mnth.10='Oct'; mnth.11='Nov'; mnth.12='Dec'

userfile='bbs:users/'||username

if mode='w' then signal Plant

if ~exists(userfile) then lastcall=date('n') /* format '21 May 1999' */
else do
  call open('fred',userfile,'r')
  do 9
    lastcall=readln('fred')
  end
  call close('fred')  /* 'lastcall' holds date of last call */

  if length(lastcall)<8 then do
    do while length(lastcall)<6
      lastcall='0'||lastcall
    end
    yr=left(lastcall,2)
    if yr<80 then lastcall='20'||lastcall
    else lastcall='19'||lastcall
  end

  last_day=right(lastcall,2)||' '
  month=substr(lastcall,5,2); last_month=mnth.month||' '
  last_year=left(lastcall,4)

  lastcall=last_day||last_month||last_year
end

/* say lastcall */

address command 'Setenv UserLast.'bbsline' 'lastcall

return

/*-----------------------------*/

Plant:

if ~exists(userfile) then return

call open('in',userfile,'r')

line. = ''
do i=1 to 45
  line.i=readln('in')
end
call close('in')

line.9 = right(date('s'),6) /* today's date as yymmdd */

call open('out',userfile,'w')
do i=1 to 45
  call writeln('out',line.i)
end
call close('out')

return
