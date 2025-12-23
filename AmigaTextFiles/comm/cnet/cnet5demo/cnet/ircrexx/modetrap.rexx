/* ORC - TRAP MODE                                                 */
/* '/alias <password> trap1----006 modetrap.rexx MODE              */
options results
parse arg orcport user cmd chan mode args

/*  :pascoe!~pascoe@205.162.185.177 MODE #bible +b *!*test@* */

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"hello'
    if pos('MIIND', user) & (pos('+b', mode)) then
      cnetirccmd '/mode 'chan' -b 'args
end


