/*    Suggestion Box Maint 1.0           */
/*    Please look at the "SuggBox" for info on how to install, etc.        */

options results

transmit '\f1\c5Suggestion Box  \c2v1.2'
transmit '\c3By Greg Fitch, \c41990'
transmit '\n1\c1\z6Reading Suggestions...\z0'
transmit '\*1bbs:text/logs/Suggestion.log\'
sendstring '\^2\c7Delete Suggestions? \c1(\c4N\c1/y) : \c5'
getchar;ans=upper(result)
if ans = "#" then exit          /* (Detects the ###PANIC, Hopefully..!!  :)  */
if ans ~= 'Y' then do
 transmit 'No'
 exit
end
sendstring '\c2Deleting'
address command "delete bbs:text/logs/Suggestion.log"
getuser 12
date = result
call open(file,"bbs:text/logs/Suggestion.log","W")
call writeln(file,'\c3\z6Last Deleted On 'date'...\z0\n1')
transmit '\h8\c5Deleted\c1.'
exit
