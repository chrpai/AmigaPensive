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
BLACK  = ESC||'[30m'

LOW    = ESC||'[0m'
BWHITE = ESC||'[47m'


/* Write a text to file 'Outfile' (in this case ram:downloadfile) */
/* and send the file to the user                                  */

Outfile = "ram:downloadfile"

cls

/* White background , black text */

print LOW || BLACK || BWHITE "Trying to write a file.." 

/* Black back ground */

print LOW


/* if (Open('fp',Outfile,'W')) then do */

call Open('fp',Outfile,'W')
if result then do

   call writeln('fp', "file: "Outfile )
   call writeln('fp', "" )
   call writeln('fp', "Testing a rexx script to make a text file 'live' and" )
   call writeln('fp', 'sending it to a User.' )

   call writeln('fp', '' )

   do n = 1 to 9 by 1
      call writeln('fp',  'line 'n'  -------------' )
   end

   call writeln('fp', "" )
   call Close('fp')

   print HIGH""GREEN"File "Outfile" succesfully written" 

end
else do
   print HIGH""RED"Could not open Outfile" Outfile" for Writing."
   /* exit */
end


print HIGH""CYAN"Sending file : "Outfile

download Outfile

/* cli "delete >nil: \""Outfile"\"" */
/* cli "delete >nil: "Outfile       */
/* delete "ram:downloadfile"        */

more

exit 0                         /* einde programma */

