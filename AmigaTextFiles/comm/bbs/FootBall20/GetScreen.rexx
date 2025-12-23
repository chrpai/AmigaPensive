/* Run this program to get a PUBLIC SCREEN name. by Matt English

 You can find the file "GETPUBNAME" in the archive "GetPubName.lha"
 available at The Alternative BBS (503)761-3043 and many other BBS's.

   1. BE SURE YOU HAVE THE FILE "GETPUBNAME" IN YOUR C: DIRECTORY !
   2. Type "rx GetScreen" in a shell
   3. Flip to your BBS Screen and wait 30 seconds
   4. If you have the command "SAY" in your Path, you'll here
      the message "Got the name."
   5. Flip back to the shell window. The screen name will be
      displayed there AND in the file "RAM:ScreenName"

*/

CR='0D'x  /* Carraige Return */

say' Checking the PUBLIC SCREEN NAME, ...'cr
say' Flip to your BBS screen now and comeback here in 30 seconds...'cr

call delay(1000)

address command'C:getpubname > ram:ScreenName'

call open(nm,'ram:ScreenName','r')

thename = readln(nm)
thename = upper(strip(thename))
call close(nm)

address command'SAY Got the name!'

say' The Public Screen name is     'thename||cr
say cr
say' This name can also be found in the file RAM:ScreenName'cr

say' Bye...'cr

exit

