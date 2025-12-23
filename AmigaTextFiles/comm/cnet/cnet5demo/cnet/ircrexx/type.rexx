/* ORC - Sends results of "type" command to user in ORC       */
options results
parse arg orcport nick a1 .
nick = trim(nick)
a1 = trim(a1)

if show('P', orcport) then do
  Open(fh, a1, 'r');
  line = readln(fh)
  do while ~eof(fh)
    if line = '' then line = ' '
    address value orcport
      cnetirccmd '/m 'nick' 'line
    line = readln(fh)
  end
  x = Close(fh)
end

