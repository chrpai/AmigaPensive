/* RexxDoor "doors:rexxtest/ReadNLA.rexx"      */
/* start with    RexxDoor  "doors:rexxtest/ReadNLA.rexx"         */


Arg node_number                         /* node we were run from */
Host = 'TRONREXX.'node_number           /* the Trion Arexx port */
/* Host = 'TRONREXX.'node_number'1' */  /* the Trion Arexx port */
/* Host = 'TRONREXX.01'  */             /* the Trion Arexx port */
Options results                         /* this is how Trion answers */
Address value host
options failat 15

ESCSEQ   = '1b'x||'['
YELLOW   = ESCSEQ'1;33m'


quit = 0

do n = 100 to 150 by 1
   Carrier
   if result = "1" & quit = 0 then do     /* if carrier present and not stopt */
      ChangeMessageArea n
      if result = "1" then do
         ReadNew
         quit = result                    /* is 1 when User quits reading */

         if quit = 1 then do
            /* print YELLOW"User terminated" */
         end

      end
      else do
         /* print YELLOW"There is no msg area "n" !" */
      end
   end
end


exit 0                         /* einde programma */

