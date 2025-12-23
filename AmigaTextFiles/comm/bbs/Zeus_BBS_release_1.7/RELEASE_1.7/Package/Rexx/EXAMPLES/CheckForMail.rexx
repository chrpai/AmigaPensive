/*
 * Checks to see if there is mail to import and if there is it runs
 * the specified command
 *
 * $VER: CheckForMail.rexx 1.0 (2.10.97)
 *
 * ©1997 Neil Williams. All Rights Reserved.
 */

options results
options failat 21

parse arg dir cmd



address command 'List >T:cfm1.tmp '|| dir ||'#?.((MO|TU|WE|TH|FR|SA|SU)[0-9]|PKT|TIC)'

Address Command 'List >T:cfm2.tmp T:cfm1.tmp LFormat "%l"'

if ~open( 'filelist', 't:cfm2.tmp', 'r' ) then do
        say 'Couldn''t open t:cmf2.tmp...'
        exit
        end

data=readln( 'filelist' )

if data~='empty' then
   address command 'run <>NIL: 'cmd

address command 'Delete >NIL: T:cmf?.tmp'

