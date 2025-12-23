/*
** CheckForNewFiles.rexx
**
** This finds if the filesize of the NewFiles list is
** above a certain size, and if it is, post it as there
** are new files.
**
** $VER: CheckForNewFiles.rexx 1.3 (13.7.99)
**
** HISTORY:
**
** CheckForNewFiles.rexx 1.0 (22.12.96)
**  First version
** CheckForNewFiles.rexx 1.1 (19.1.97)
**  Posts different lists for Amiga & Complete
** CheckForNewFiles.rexx 1.2 (9.4.97)
**  Added Zeus Doors entry
** CheckForNewFiles.rexx 1.3 (13.7.99)
**  Switched to Zeus. I need to make *loads* of improvements to msg posting for Zeus 2 :(
*/

Options Results

Address Command 'List >t:File.temp Files:TKG_Stuff/TodaysFiles.txt LFormat "%l"'

if ~open( 'filelist', 't:file.temp', 'r' ) then do
        say 'Couldn''t open t:file.temp...'
        exit
        end

data=readln( 'filelist' )

if data>760 then do   /* if the file is greater than 680 bytes, there are files listed */

   say 'File size is greater that 1000 bytes - posting the file lists...'

/*   address command 'RX MM:Rexx/MM_AutoSend NEWFILES'
   address command 'RX MM:Rexx/MM_AutoSend D_NEWFILES' */

   address command 'PostMsg "Neil Williams" "All" "New files @ TKG" Files:TKG_Stuff/TodaysFiles.txt NOWRAP TAG=KILLER-NEWS'
   address command 'PostMsg "Neil Williams" "All" "New files @ TKG" Files:TKG_Stuff/TodaysFiles.txt NOWRAP TAG=FILES_442'

End

call Close( 'filelist' )

address command 'Delete >NIL: t:file.temp'



Address Command 'List >t:File.temp Files:TKG_Stuff/TodaysAmigaFiles.txt LFormat "%l"'

if ~open( 'filelist', 't:file.temp', 'r' ) then do
        say 'Couldn''t open t:file.temp...'
        exit
        end

data=readln( 'filelist' )

if data>760 then do   /* if the file is greater than 680 bytes, there are files listed */

   say 'File size is greater that 1000 bytes - posting the file lists...'

/*   address command 'RX MM:Rexx/MM_AutoSend AMY_NEWFILES' */
   address command 'PostMsg "Neil Williams" "All" "New files @ TKG" Files:TKG_Stuff/TodaysAmigaFiles.txt NOWRAP TAG=AMIGA_NF'


End

call Close( 'filelist' )

address command 'Delete >NIL: t:file.temp'


/* Zeus Doors & Utils */

Address Command 'List >t:File.temp Files:TKG_Stuff/NewZeusDoors.txt LFormat "%l"'

if ~open( 'filelist', 't:file.temp', 'r' ) then do
        say 'Couldn''t open t:file.temp...'
        exit
        end

data=readln( 'filelist' )

if data>401 then do   /* if the file is greater than 400 bytes, there are files listed -- nb. the current file comes to exactly 400 bytes empty, so I've added one extra byte for security */

   say 'File size is greater that 400 bytes - posting the file list...'

/*   address command 'RX MM:Rexx/MM_AutoSend NEW_ZEUS_UTILS'*/
   address command 'PostMsg "Neil Williams" "All" "New files @ TKG" Files:TKG_Stuff/NewZeusDoors.txt NOWRAP TAG=ZEUS_AMY'
   address command 'PostMsg "Neil Williams" "All" "New files @ TKG" Files:TKG_Stuff/NewZeusDoors.txt NOWRAP TAG=ZEUS_DISTRIBUTOR'


End

call Close( 'filelist' )

address command 'Delete >NIL: t:file.temp'


/* Music */

Address Command 'List >t:File.temp Files:TKG_Stuff/NewMusic.txt LFormat "%l"'

if ~open( 'filelist', 't:file.temp', 'r' ) then do
        say 'Couldn''t open t:file.temp...'
        exit
        end

data=readln( 'filelist' )

if data>600 then do   /* if the file is greater than 600 bytes, there are files listed */

   say 'File size is greater that 600 bytes - posting the file list...'

/*   address command 'RX MM:Rexx/MM_SendMsg VIBEZ_INFO Files:TKG_Stuff/NewMusic.txt "Music @ TKG" FromName "The Killing^Ground" ToName "All" Origin "the year we took back what was ours" Export Split 15'*/
/*Usage: [rx] MM_SendMsg[.rexx] AREA/A,MSGFILE/A,SUBJECT/A,FROMNAME/K,FROMADDR/K,TONAME/K,TOADDR/K,FLAGS/K/M,ORIGIN/K,DELETE/S,EXPORT/S,SPLIT/K/N,QUEUE/S*/
   address command 'PostMsg "The Killing^Ground" "All" "Music @ TKG" Files:TKG_Stuff/NewMusic.txt NOWRAP TAG=VIBEZ_INFO'

End

call Close( 'filelist' )

address command 'Delete >NIL: t:file.temp'

