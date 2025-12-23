/* ORC - Sends finger results to user in ORC             */
options results
parse arg orcport a1
orcport=trim(orcport)
a1=trim(a1)
fname='t:'orcport''time('S')'.txt'
address command 'finger 'a1' >'fname

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"Finger Results:'
  x = Open(fh, fname, 'r');
  line = readln(fh)
  do while ~eof(fh)
    if line = '' then line = ' '
    address value orcport
      cnetirccmd '/"'line
    line = readln(fh)
  end
  x = Close(fh)
end
address command 'delete >nil: 'fname
