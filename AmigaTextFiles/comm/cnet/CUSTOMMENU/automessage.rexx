/* AutoMessage.rexx v2.0 by Darkman
   for use with CustomMenu v1.5
   Instructions: Make an file called "Automessage" in your system: dir
   containing 3 lines of garbage.  Then, add to your sys.welcome file:
   \*1system:automessage\
   The rest is up to you. */

options results
bufferflush
call AMain
BYE:
        spawn 'pfiles:custommenu'; exit
AMain:
        transmit '\f1\c9AutoMessage 2.0'
        transmit '\n1\caW\c2) \cbWrite an AutoMessage'
        transmit '\caR\c2) \cbRead the AutoMessage'
        transmit '\caQ\c2) \cbExit AutoMessage Menu'
        sendstring '\n2\c5Command: \cd'; getchar; choice = upper(result); transmit choice
        if choice = 'W' then call write
        if choice = 'R' then call read
        if choice = 'Q' then call bye
        call AMain
write:
        x = 0
        getuser 1; name = result
        sendstring '\cdSubject: \c5'; receive; subj = result
        transmit '\n1\c9Start typing.  You have up to 10 lines.  Type \ce. \c9on a blank line to save.'
        open('Amess','system:AutoMessage','W')
        writeln('Amess','\c3/=====\cbAutoMessage\c3===============================================================')
        writeln('Amess','\cf[\caFrom: \c2'name'\cf]')
        writeln('Amess','\cf[\cdSubject: \c5'subj'\cf]\n1\c9')
        do i = 1 to 10
                sendstring '\c9'i'\c1> '; receive; line.i = result
                if line.i = '.' then leave /* End of file! */
                        else x = x + 1; end
        do i = 1 to x
                writeln('Amess',line.i); end
        close('Amess'); transmit '\caYour message has been saved.  Thank you.'; return
read:
        transmit '\f1'
        open('Amess','system:AutoMessage','R')
        do until eof('Amess')
                line = readln('Amess'); transmit line
                end
        transmit '\n1Hit any key...'; getchar; close('Amess'); return
