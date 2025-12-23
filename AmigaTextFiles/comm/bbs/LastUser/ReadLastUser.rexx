/* Read LastUser Quote */

options results
signal on syntax
'bufferflush'

_User=""
_Quote=""

if ~Open(_File,'EX.LastUser','R') then exit
    _User=readln(_File)
    _Quote=readln(_File)
close (_File)

'sendstring' '\c7The last user was \c6'_User
if _Quote ~= "" then 'sendstring' '\c7, who said:\c6 '_Quote'\q1'
exit
