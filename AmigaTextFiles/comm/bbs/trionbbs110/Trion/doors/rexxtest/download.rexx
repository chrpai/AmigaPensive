/* RexxDoor "doors:rexxtest/download.rexx "      */

Arg node_number                         /* node we were run from */
Host = 'TRONREXX.'node_number           /* the Tron Arexx port */
Options results                         /* this is how Tron answers */
Address value host
options failat 15

CRLF  = '0d'x||'0a'x
ESC   = '1b'x
HIGH   = ESC||'[1m'
GREEN  = ESC||'[32m'
YELLOW = ESC||'[33m'
CYAN   = ESC||'[36m'
WHITE  = ESC||'[37m'
RED    = ESC||'[31m'




/* Get a filename from file 'Infile' (in this case ram:downloadfile) */
/* and send the file with the name we read to the User               */

Infile = "ram:downloadfile"

cls

/* if (Open('fp',Infile,'R')) then do */

call Open('fp',Infile,'R')
if result then do

   name = readln('fp')

   call Close('fp')

   print HIGH""GREEN"File "Infile" succesfully read" 

   print HIGH""CYAN"Sending file : "name
   download name

   cli "delete >nil: \""Infile"\""

end
else do
   print HIGH""RED"Could not open Infile" Infile" for reading."
   /* exit */
end

more

exit 0                         /* einde programma */

