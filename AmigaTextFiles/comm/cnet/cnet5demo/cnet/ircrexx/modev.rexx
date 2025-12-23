/* ORC -  MODE voice    /v nick1 nick2 nick3 nick4 nick5   */
options results
parse arg orcport args

if show('P', orcport) then do
  address value orcport
    cnetirccmd '/"hello'
    cnetircvar c
    parse var result chan
    cnetirccmd '/mode 'chan' +vvvvv 'args
end


