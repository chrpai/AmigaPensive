
                          ----------------------
                          FAMEMsgScanModify v1.0
                          ----------------------

FAMEMsgScanModify allows you to modify the Message Base scan pointers
of your users. You can set the scan pointers to a position of your
choice also to 0 to reset them completely.

You can modify them in a specific Conference or in all Conferences
in one go. The same for your Users. You can modify a specific User
entry or all User entries of one/all Conferences in one go.

How to use:

You need to call FAMEMsgScanModify with a set of arguments.
The template is: CONFNR/K/N,ALLCONFS/S,USERNR/K/N,ALLUSERS/S,NEWPOINTER/K/N

CONFNR     - The Conference (number) you want to modify.

ALLCONFS   - To modify all Conferences (overrides CONFNR).

USERNR     - The User (number) you want to modify.

ALLUSERS   - To modify all Users (overrides USERNR).

NEWPOINTER - The new Message Base scan pointer.
             Use 0 to reset it completely to the beginning.
             You can also enter a higher number to set the
             scan pointer(s) to a higher message. In this case
             take care that the number is not higher than the
             last written mail, else scanning will find no matches
             until that number has been reached with written
             (new) messages. In this case 'Last message auto scanned'
             will be higher than 'Total messages' in the Conference
             join statistics if you join that Conference.
             Best is to check out for each Conference what the
             last written mail number is (use "R") or check the
             Conference join statistics on joining that Conference
             ('Total messages') and substract a value of your choice
             like 10 or more to have some mails to be scanned left.


------------------------------------------------------------------------------

                           Disclaimer & Copyright
                          ~~~~~~~~~~~~~~~~~~~~~~~~

         This Software is provided "As Is" without warranty of any
    kind, either express or implied.  The entire risk as to the use of
        this information is assumed by the user.  In no event will
   David Wettig or his affiliated developers be liable for any damages,
     direct, indirect, incidental, special or consequential, resulting
      from any claim arising out of the information presented herein,
      even if it has been advised of the possibility of such damages.
       Some states do not allow the exclusion or limitation of such
        implied warranties, so the above limitations may not apply.

     No guarantee of any kind is given that the programs described in this
   document are 100% reliable. You are using this material on your own risk.

                      - FAMEMsgScanModify is FREEWARE -

    No profit must be made by distributing `FAMEMsgScanModify', except to
    charge fees for copying and creating disks, cds or other media costs.

    The source of FAMEMsgScanModify is included - feel free to use it for
    your own pleasure if you do not modify it or the other depending files
      in any case to redistribute it under my name. If you redistribute
      FAMEMsgScanModify you have to change the name of the tool and the
        author (me)  and you have to include a note about the original
                author (me) in your program's documentation.

          ----------------------------------------------------------
           All the files in this archive are freely redistributable
          as long as the above Copyright, Warning and Disclaimer are
                     accepted and present in all copies.
          ----------------------------------------------------------

------------------------------------------------------------------------------

FAMEMsgScanModify v1.0 programmed by:

--------------------------------------------------------------------
David Wettig - FAME Innovation      ______  ____ :____  ________ tGø
Team AMIGA - CUCUG - ICOA member    \  __/_/    \|    \/   /  _/___
ICQ...: 7225710                     |    _//     \    \/  /  _|   /
E-Mail: David.Wettig@fhtw-berlin.de |____|________\___/  /|_______\
URL...: http://www.trsi.de/inno/strider/index.html   /___\ Developer
--------------------------------------------------------------------
<EOF>
