/**************************************************************************\
    Ced.Rexx for startup from CNET

This command will take arguments! For instance:

When I want to edit the bbsmenu, I can type this RIGHT from my main 
prompt:

'ced cnet:bbsmenu'

And ced will run, and load that file! VERY handy for me! If you use
a different editor, just edit the script as you see fit. :)

Of course you need to add this to your BBSMENU, preferably menu #1, 
sysop only, as 'CED'...and it would make sense to only run this locally,
although it would probably do no harm otherwise, because CED detaches itself
from the shell it is started from. If you have an editor that does NOT
do that...hmm...then you better not use this script. :)

Edward D. Peyton
Atlantis BBS: (904) 479-3133 | 479-4080
WebSite: http://www.pen.net/~atlantis
email : edp@pen.net  Edward Peyton@1:3612/369.0

\**************************************************************************/
options results

getuser 1302307;path=result
address command 'c:CED' path

exit
