/* ORC - Sends status results to user in ORC       */
options results
parse arg orcport a1
a1=trim(a1)
fname='t:'orcport''time('S')'.txt'
address command 'status 'a1' >'fname

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"Status Results:'
  Open(fh, fname, 'r');
  do while ~eof(fh)
    line = readln(fh)
    address value orcport
      cnetirccmd '/"'line
  end
  x = Close(fh)
end
address command 'delete 'fname

