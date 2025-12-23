/*    Suggestion Box Maint 1.3      The Back Burner BBS - 303/697-1225     */
/*    Please look at the "SuggBox" for info on how to install, etc.        */

options results

transmit '\f1\c5Suggestion Box  \c2v1.2'
transmit '\c3By The Brazilian, \c41990'
transmit '\n1\c1\z6Reading Suggestions...\z0'
transmit '\*1system:log.sugg\'
sendstring '\^2\c7Delete Suggestions? \c1(\c4N\c1/y) : \c5'
getchar;ans=upper(result)
if ans = "#" then exit          /* (Detects the ###PANIC, Hopefully..!!  :)  */
if ans ~= 'Y' then do
 transmit 'No'
 exit
end
sendstring '\c2Deleting'
address command "delete system:log.sugg"
getuser 12
date = result
call open file, ('system:log.sugg'), "W"
call writeln file, '\c3\z6Last Deleted On 'date'...\z0\n1'
transmit '\h8\c5Deleted\c1.'
exit
