/* Attempt (oo-er) to automate Internet calls.
 * Holger Kruse, rot in hell for your owndevunit handling
 * (dispite a very nice TCP/IP stack)
 *
 * NOTE TO ZEUS SYSOPS: this script is 100% specific to my BBS,
 * it is included as an EXAMPLE of what you can do but cannot
 * be used as-is. It must be modified.
 *
 * 28.12.2000 -- new offat argument to keep connection open.
 * 7.9.2000
 */

options results
failat 21
parse arg nooffline offat

/* Command template is ON offat/S :
 * Specify ON as "ON" and the script will not hangup.
 * Specify ON as "anything_else,_maybe_OFF_is_good" and you can
 * use offat to make the script hang up at a specific time.
 * C:WAIT format: "Wait until 23:55". Be warned: this might start
 * AmIRC to broadcast the BBS's IP address.
 *
 * Specify nothing to do a normal Internet mail run; to pick up
 * and send email and news then hang up.
 */

call pragma 'Directory','Zeus:S'

/* Configurable bits */

interf = 'ppp0'         /* MiamiDx interface to use.  For Miami, try '' or 'mi0'..*/

/* End of configurable bits */

stayon = 0              /* 1 = do not go offline */

if ~show( 'p', 'MIAMI.1' ) then do
   say 'MiamiDx is not active.'
   exit
end

say 'Running ZMP'
address command 'ZMP Import Export Tick'
say 'Done.'


address MIAMI.1

isonline interf
if RC = 1 then do
   say 'The interface 'interf' is already online!  I will not place it offline.'
   stayon = 1
end

if upper(nooffline) = 'ON' then do
   say 'Argument request to stay online.  I will not place the interface offline.'
   stayon = 1
end

offat = strip( offat )

online interf

isonline interf
if RC = 1 then do
   say interf ||' is now online.'

   say 'Collecting mail by POP3'
   address command 'CollectMail.scr'
   say 'Done.'

   say 'Collecting news by NNTP'
   address command 'CollectNews.scr'
   say 'Done.'

   say 'Sending mail'
   address command 'Zeus:S/SendMailQueue.scr'
   say 'Done. Exiting.'

   isonline interf
   if RC = 1 then do
      if stayon = 1 then do
         say 'Miami must stay online, not going offline.'

         say 'Running Background ZMP'
         address command 'RUN <>NIL: ZMP Import Export Tick'
         say 'Done.'
      end
      else do

         if offat ~= ""  then do
            say 'Waiting until 'offat', as asked, before hanging up. StrICQ and TelNet calls should keep the connection up. CTRL-C to Wait not script to abort.'
            say 'Running Background ZMP'
            address command 'RUN <>NIL: ZMP Import Export Tick'

            address command 'Wait until 'offat
         end


         address ZEUS IsUserOnline '"Neil Williams"'
         if RC = 0 then do
            if RESULT ~= '0' then do
               say 'While Miami can be put offline, the SysOp is online so we''ll wait until he''s gone.'

               sysoff = 0
               do until sysoff = 1
                  /* Delay 10 seconds.
                   */
                  call delay( 50*10 )

                  address ZEUS IsUserOnline '"Neil Williams"'
                  if RC ~= 0 then
                     sysoff = 1
                  else
                     if RC = 0 then
                        if RESULT = '0' then
                           sysoff = 1  /* moved to the local line now, not telnetting */
               end /* do */
            end /* if local line */
         end

         say 'Miami can be put offline.  Going offline...'

         minsleft = 3

         do until minsleft = 0
            anyoneon = 0
            do i = 1 to 5
               address ZEUS WhosOnline i
               if RC = 0 then anyoneon = 1
            end
            if anyoneon = 0 then minsleft = 0

            if minsleft > 0 then do
               address command 'Zeus:Bin/SendMsg * " * The BBS is going offline from the Internet in 'minsleft' minutes."'
               address command 'Zeus:Bin/SendMsg * "   Anyone online via telnet at this time will find that their"'
               address command 'Zeus:Bin/SendMsg * "   connection will drop."'
               minsleft = minsleft-1
               call delay( 50*60 )
            end
            else do
               do i = 1 to 5
                  address command 'Zeus:Bin/BootUser 'i
               end
            end
         end

         address command "rx zeus:rexx/dtdns_offline.rexx"

         offline interf

         isonline interf
         if RC = 1 then
            say interf ||' is STILL online!  Not my fault..'
         else
            say interf ||' went offline.'

         say 'Running ZMP'
         address command 'ZMP Import Export Tick'
         address command 'Run <>NIL: Zeus:Bin/FileFind/FileFind'
         say 'Done.'

      end /* stayonline */
   end /* isonline2 */
   else do
      say 'Freeserve has lost carrier. WHAT A SUPRISE. Oh, comedy! Ha. Ha.'
      address command 'date >>Logs:Internet.log'
      address command 'echo >>Logs:Internet.log "Freeserve dropper carrier*n"'
   end


end
else do
   say interf ||' did not go online.  Probably was busy on each attempt or PPP failed due to server trouble.'
   address command 'date >>Logs:Internet.log'
   address command 'echo >>Logs:Internet.log "Failed to connect*n"'
end

