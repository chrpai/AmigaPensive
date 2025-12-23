/* ORC - Sends figlet results to destination specified       */
options results
parse arg orcport nick font a1
orcport = trim(orcport)
nick = trim(nick)
font = 'libs:figletfonts/'trim(font)
fname = 't:'orcport''time('S')
address command 'echo >'fname'.msg "'trim(a1)'"'
address command 'figlet 'font' <'fname'.msg >'fname'.txt'
address command 'delete 'fname'.msg'

if show('P', orcport) then do
  Open(fh, fname'.txt', 'r');
  line = readln(fh)
  do while ~eof(fh)
    if line ~= '' then do
      address value orcport
        cnetirccmd '/m 'nick' 'line
    end
    line = readln(fh)
  end
  x = Close(fh)
end
address command 'delete 'fname'.txt'
