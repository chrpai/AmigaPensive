/* Chatop reminder - by Dreamer/Supplex/1oo% */

options results ; signal on SYNTAX 
address command 'c:date >pipe:datum'
open(date,'pipe:datum','R')
datum=readln(date)
close(date)
getuser 23; port=result
call setclip(port'ChTime',datum)
exit

syntax:
sendstring "Error occured! Line: "sigl", "rc" - "errortext(rc)
exit
