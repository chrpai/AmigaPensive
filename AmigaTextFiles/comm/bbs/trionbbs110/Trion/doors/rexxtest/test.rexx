/* RexxDoor "doors:rexxtest/test"      */
/* starten met RX "test" node          */

Arg node_number                         /* node we were run from */
Host = 'TRONREXX.'node_number           /* the Tron Arexx port */
/* Host = 'TRONREXX.'node_number'1' */  /* the Tron Arexx port */
/* Host = 'TRONREXX.01'  */             /* the Tron Arexx port */
Options results                         /* this is how Tron answers */
Address value host
options failat 15

CRLF  = '0d'x||'0a'x
CSI   = '1b'x||'['
ESC   = '1b'x||'['
HIGH   = CSI'1m'
GREEN  = CSI'32m'
YELLOW = CSI'33m'
CYAN   = CSI'36m'
WHITE  = CSI'37m'
PK     = ' '                /* ';' */

/* say Host  */

/* Quit */

cls

send    HIGH

print   HIGH""CYAN"    hello"
print   GREEN"test1"
print   YELLOW"test2"

/* print   ESC"20c bla bla bla" */
print   ESC"3;20H bla bla bla"

print

send    "    hello"
send    "hello"
send    '    hello'
send    'hello'

print   "start"
print
print   "end"

print   "    hello2"
print   " hello2"
print   '  '||'     hello2'
print   ' hello2'

do n = 1 to 10 by 1
   print "getal = "n
end

DisplayFile 'ram:b'
SendfileNoPause 'ram:b'
print

sample   'tron:samples/startrek-alert'

download 's:startup-sequence'


send "Press a key"
GetCharacter
char = result                /* char = rc */
print ""
print "input was : "char


print "Type a line (quit = end)"
send ">"
GetLine 6 NORMAL  'blerk'
line = result
print "input was : '"line"'"
print "input was : >"line
if line = "quit" then do
   print "Bye bye."
   exit 0
end



MayGetCharacter
char = result                /* char = rc */
if char = "" then do
   print "no input"
end
else do
   if char = "w" then    print "input was : wwwww"
end


MayGetCharacter
char = result                /* char = rc */
if char = "" then    print "no input"
else                 print "input was : "char



more

/*
do n = 10 to -1000 by -2
   say n n*2 n**2
end
*/



Carrier
if result = "0" then do
   exit                      /* carrier is dropped , exit script */
end
else do
   print "Carrier still present"
end


do n = 1 to 4 by 1            /* to 100 */
   Carrier
   car = result
   print "carrier = "car      /* "carrier = 0" or "carrier = 1" */
end


exit 0                         /* einde programma */



test:          /*  een functie: 'call test' */
return 0

