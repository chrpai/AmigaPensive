/* ORC - TRAP JOIN                                                  */
/* do automatic voice on moderated channel, check no-voice list     */
/* '/alias <password> trap1----005 voicetrap.rexx JOIN              */
options results
parse arg orcport user cmd args
parse var user ':'nick'!'site
parse var args ':'args
fname = 'ram:novoice.txt'


voice = 1

if Open(fh, fname, 'r') then do
  do while ~eof(fh) & (voice == 1)
    if pos(readln(fh), user) then voice = 0
  end
  x = Close(fh)
end

if show('P', orcport) & (voice == 1) then do
  address value orcport
    cnetirccmd '/"'user' 'cmd' 'args
    cnetirccmd '/mode 'args' +v 'nick
end


